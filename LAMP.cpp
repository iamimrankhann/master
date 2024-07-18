#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map> //FOR CONFIG

using namespace std;

map<string, string> readConfigFile(const string& filename) {
    ifstream configFile(filename);

    if (!configFile) {
        cerr << "Failed to open " << filename << endl;
        return {};
    }

    map<string, string> configData;

    string line;
    while (getline(configFile, line)) {
        size_t delimiterPos = line.find('=');
        if (delimiterPos != string::npos) {
            string key = line.substr(0, delimiterPos);
            string value = line.substr(delimiterPos + 1);

            // Trim whitespace from key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Store key-value pair in the map
            configData[key] = value;
        }
    }

    configFile.close();
    return configData;
}

int main()
{
    string configFile = "config.txt";//FOR CONFIG

    map<string, string> configData = readConfigFile(configFile);//FOR CONFIG
    // WE NEED TO ADD VARIABLES OF CONFIG FILE AS configData["variable_name"]

    // READ from CSV to vector<vector<string>> data
    string filename = configData["CAN_PATH"]; 
    ifstream infile(filename);
    if (!infile.is_open())
    {
        cerr << "Error while opening file: "<< filename << endl;
        return 1;
    }

    string headerLine;
    getline(infile, headerLine);

    vector<vector<string>> data;

    string line;
    while(getline(infile, line))
    {
        istringstream iss(line);
        vector<string> row;
        string cell;
        while (getline(iss, cell, ','))
        {
            row.push_back(cell);
        }
        data.push_back(row);
    }
    infile.close();

    // ROWS 
    // cout << "TOTAL ROWS : " << data.size() << endl;
    
    // COLUMNS
    // cout << "TOTAL COLUMNS : " << data[0].size() << endl;

    // PRINTING THE DATA
    // for (size_t col = 0; col < data[0].size(); ++col){
    //         cout<< "Column " << col + 1 << ":" <<endl;
    //         for (size_t row = 0; row < data.size(); ++row)
    //         {
    //             cout<< data[row][col] << endl;
    //         }
    //         cout << endl;
    // }

        

    //VARIABLES
    // vector<string> ignitionon;
    vector<string> spnName;
    vector<int> spnValue;
    vector<int> time;

    // cout << "---------------------" << endl;

    // ASSIGNING INITIAL VALUES   //NEED TO CHANGE ACCORDING TO REQUIREMENT
    float A = stoi(configData["left_lamp_time"]) ; //250 hrs into sec
    float B = stoi(configData["right_lamp_time"]) ; // 250 hrs into sec
    float a = 0, b=0;

    // cout << "****************" << endl;
    // CREATING SPNNAME AND SPNVALUE
    for(int row = 0 ; row < data.size() ; ++row){
     
        if(data[row][1]=="turn_indication"){ //spnname is 21the column in can.csv
            spnName.push_back(data[row][1]);

            spnValue.push_back(stoi(data[row][2])); //spnvalue is 24th column in can.csv
        }
    }

    // PRINTING SLICED DATA
   
    // for (size_t row = 0; row < spnName.size(); ++row){
    //     {
    //         cout << spnName[row] << endl;
    //     }
    //     cout << endl;
    // }

    // cout << "++++++++++++++++++++++++++++" << endl;

    // LOGIC 
    for(int row = 0 ; row < spnName.size() ; ++row){
        if (spnName[row] == "turn_indication") {
            if (spnValue[row] == 0) {
                continue;
            } 
            else if (spnValue[row] == 1) {
                a = a+1; // RIGHT LAMP
            } 
            else if (spnValue[row] == 2) {
                b = b+1; // LEFT LAMP
            }
        }
    }

    // cout << "################################" << endl;


    // REMAINING TIME FOR LAMPS
    cout << "REMAINING TIME FOR RIGHT LAMP : " << A-(a*30/3600) << endl;
    cout << "REMAINING TIME FOR LEFT LAMP : " << B-(b*30/3600) << endl;


}
