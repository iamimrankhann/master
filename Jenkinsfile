pipeline {
    agent {
        node{
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
    }
    
    options {
        disableConcurrentBuilds()
        timestamps()
    }
    
    stages {
        stage('Checkout git') {
            steps {
                // checkout scmGit(branches: [[name: '*/lamp_life_calculator']], extensions: [], userRemoteConfigs: [[credentialsId: 'github-jenkins', url: 'https://github.com/iamimrankhann/master.git']])
                echo "hIIII checkout succesful."
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
