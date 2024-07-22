pipeline {
    agent {
        node {
            label 'slave'
        }
    }
    
    environment {
        REMOTE_HOST = '3.74.74.152'
        REMOTE_USER = 'ubuntu'
        REMOTE_DIR = '/home/ubuntu/TML'
        HARBOR_USERNAME = 'admin'
        HARBOR_PASSWORD = 'Harbor12345'
        JFROG_URL = 'http://3.74.74.152:8081/artifactory'
        JFROG_REPO = 'imran-imran'
        JFROG_USERNAME = 'admin'
        JFROG_PASSWORD = 'Imran@123'
        HARBOR_API_URL = 'http://3.74.74.152/api/v2.0'
        IMAGE_NAME = 'tmldtdc/models/lamp_life_calculator'
        IMAGE_TAG = 'latest'
        PROJECT_ID = '1'  // Update with your Harbor project ID
    }
    
    options {
        disableConcurrentBuilds()
        timestamps()
    }
    
    stages {
        stage('Checkout git') {
            steps {
                checkout scmGit(branches: [[name: '*/lamp_life_calculator']], extensions: [], userRemoteConfigs: [[credentialsId: 'github-jenkins', url: 'https://github.com/iamimrankhann/master.git']])
            }
        }
        stage('Check files') {
            steps {
                sh '''
                    echo "Listing files in the workspace:"
                    ls 
                    
                    echo "\nChecking Git status:"
                    git status
                    
                    echo "\nShowing current branch:"
                    git branch --show-current
                    
                    echo "\nShowing last commit:"
                    git log -1 --oneline
                '''
            }
        }
        stage('Build cpp and CTest') {
            steps {
                sh '''
                    echo 'Building and testing C++ code...' &&\
                    mkdir -p build && \
                    cd build && \
                    cmake .. && \
                    make && \
                    cp ../LAMP.csv . && \
                    cp ../config.txt . && \
                    ctest --output-on-failure && \
                    echo 'Result of Code : ' && \
                    ./lamp_life_calculator
                '''
            }
        }
        stage('Start JFrog Artifactory') {
            steps {
                sh '''
                    echo "Starting JFrog..." &&
                    sudo systemctl start postgresql &&
                    sudo systemctl is-active --quiet postgresql &&
                    echo 'Postgress database started successfully' || 
                    (echo 'Failed to start postgress database' && exit 1) &&
                    sudo systemctl start artifactory &&
                    sudo systemctl is-active --quiet artifactory &&
                    echo 'JFrog Artifactory started successfully' || 
                    (echo 'Failed to start JFrog Artifactory' && exit 1)
                '''
            }
        }
        stage('Upload to JFrog Artifactory') {
            steps {
                sh '''
                    echo "Uploading build artifacts and test results to JFrog Artifactory..." && \
                    cd build && \
                    curl -u ${JFROG_USERNAME}:${JFROG_PASSWORD} -X PUT ${JFROG_URL}/${JFROG_REPO}/lamp_life_calculator/${BUILD_NUMBER}/ -T lamp_life_calculator && \
                    curl -u ${JFROG_USERNAME}:${JFROG_PASSWORD} -X PUT ${JFROG_URL}/${JFROG_REPO}/lamp_life_calculator/${BUILD_NUMBER}/ -T LAMP.csv && \
                    curl -u ${JFROG_USERNAME}:${JFROG_PASSWORD} -X PUT ${JFROG_URL}/${JFROG_REPO}/lamp_life_calculator/${BUILD_NUMBER}/ -T config.txt && \
                    curl -u ${JFROG_USERNAME}:${JFROG_PASSWORD} -X PUT ${JFROG_URL}/${JFROG_REPO}/lamp_life_calculator/${BUILD_NUMBER}/ -T Testing/Temporary/LastTest.log && \
                    echo 'Uploaded lamp_life_calculator, LAMP.csv, config.txt and LastTest.log' && \
                    echo "Artifact upload process completed."
                '''
            }
        }
        stage('Create Podman image') {
            steps {
                script {
                    def buildStatus = sh(script: '''
                        echo "Starting Podman image build..." && \
                        rm -rf build && \
                        podman build -t lamp_life_calculator:latest .
                    ''', returnStatus: true)
                    
                    if (buildStatus == 0) {
                        echo "Podman image created successfully!"
                    } else {
                        error "Podman image build failed"
                    }
                }
            }
        }
        stage('Run Podman container') {
            steps {
                sshagent(credentials: ['3.74.74.152']) {
                    sh '''
                        echo "Running Podman container..."
                        podman run --rm lamp_life_calculator:latest
                        echo "Podman container run completed."
                    '''
                }
            }
        }
        stage('Start Harbor containers') {
            steps {
                sshagent(credentials: ['3.74.74.152']) {
                    sh '''
                        echo "Starting Harbor..."
                        ssh -o StrictHostKeyChecking=no ${REMOTE_USER}@${REMOTE_HOST} "cd harbor && sudo ./install.sh --with-trivy"
                        echo "Harbor Started..."
                    '''
                }
            }
        }
        stage('Upload Podman Image to Harbor Registry and Scan') {
            steps {
                sh '''
                    echo "Logging into Harbor registry..."
                    echo ${HARBOR_PASSWORD} | podman login --username ${HARBOR_USERNAME} --password-stdin ${REMOTE_HOST}:80 && \
                    
                    echo "Tagging and uploading image..."
                    podman tag lamp_life_calculator:latest ${REMOTE_HOST}:80/${IMAGE_NAME}:${IMAGE_TAG} && \
                    podman push ${REMOTE_HOST}:80/${IMAGE_NAME}:${IMAGE_TAG} --tls-verify=false && \
                    
                    echo "Upload successful..."
                '''
            }
        }
        stage('Fetch and Display Harbor Scan Results') {
            steps {
                script {
                    def scanResults = sh(script: '''
                        echo "Fetching vulnerability scan results from Harbor..."
                        # You might need to use the correct artifact digest here instead of "latest"
                        curl -u ${HARBOR_USERNAME}:${HARBOR_PASSWORD} -X GET ${HARBOR_API_URL}/projects/${PROJECT_ID}/repositories/${IMAGE_NAME}/artifacts/${IMAGE_TAG}/scan
                    ''', returnStdout: true).trim()
                    
                    echo "Vulnerability Scan Results:"
                    echo "${scanResults}"
                }
            }
        }
    }
    
    post {
        always {
            echo "Build finished. Status: ${currentBuild.result}"
        }
        failure {
            echo "Build failed. Check the logs for details."
        }
        success {
            echo "Build completed successfully."
        }
    }
}
