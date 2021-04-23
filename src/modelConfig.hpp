// model configuration class,
// built from the file passed in the command line.
// kinda key...

#ifndef MODEL_CONFIG
#define MODEL_CONFIG
#include <string>
#include <iostream>
using namespace std;
class modelConfig {
private:
    string cfgFileName = "BarraCUDA.dat"; // defaults to BarraCUDA.txt if none provided
    string headerLine = "NaN"; // only for the user, no true function.

public:
    string modelName = "NaN"; // used for base of output files

    modelConfig(const string& fileName); // constructor

    int numSchemes = 1; // number of tidal range schemes in this model run
    bool isMultiScheme = false; // true if more than 1...
    vector<string> schemeFileName; // holds (numSchemes) file Names

    bool isGenetic = false; // is this a genetic algorithm assessment?
    string geneticFileName = "NaN"; // if genetic, loads name from file
    inline bool is_genetic() { return isGenetic; }

    bool printFullResults = false; // does this run want a full model printout?
    string runResultsFileName = "NaN"; // if yes, this file name
    inline bool is_printFull() { return printFullResults; }

    bool energyValueChange = false; // does the value of energy change with time?
    string energyValueFileName = "NaN"; // if yes, here is the info

    double maxTime  = 360; // maximum time for each model run in hours
    double timeStep = 0.1; // timestep for the model(s) in hours
};
//------------------IMPLEMENTATIONS---------------
//constructor
modelConfig::modelConfig(const string& fileName){
    // create file stream item and open the config file
    ifstream cfgFile;
    cfgFile.open(fileName);

    // if it opens, read the first line as a header and tell user it's open
    if (cfgFile.is_open()) {
        cout << fileName << ": opened." << endl;
        string tmp = "NaN";
        getline(cfgFile, headerLine);

        // cfg file should contain values in key: agrument pairs or lines
        cfgFile >> tmp; // read first key

        // file ends with "End" for simplicity
        while (tmp != "End") {
            // if else tree to find what the user ants to log
            if (tmp == "modelName:") {
                cfgFile >> modelName;
            } else if (tmp == "numSchemes:"){
                cfgFile >> numSchemes;
                if (numSchemes > 1) {
                    isMultiScheme = true;
                } else {
                    isMultiScheme = false;
                }
            } else if (tmp == "schemeFiles:"){
                // read as many scheme files as required
                for (int i = 0; i < numSchemes; i++) {
                    cfgFile >> tmp;
                    schemeFileName.push_back(tmp);
                }
            } else if (tmp == "schemeFile:"){
                cfgFile >> tmp;
                schemeFileName.push_back(tmp);

            } else if (tmp == "geneticsFile:"){
                isGenetic = true;
                cfgFile >> geneticFileName;

            } else if (tmp == "energyValue:"){
                energyValueChange = true;
                cfgFile >> energyValueFileName;

            } else if (tmp == "maxTime(hrs):") {
                cfgFile >> maxTime;

            } else if (tmp == "timeStep(min):") {
                double tmpD;
                cfgFile >> tmpD;
                timeStep = tmpD/60;
            }else if (tmp == "printFull:") {
                printFullResults = true;
                cfgFile >> runResultsFileName;
            } else {
                cout << "Invalid parameter [" << tmp << "] in config file..." << endl;
                //system("pause");
            }
            // read in the next key.
            cfgFile >> tmp;

        }
        // close the file and tell the user it's done being read.
        cfgFile.close();
        cout << fileName << ": read." << endl;

    // if the file cannot be opened, inform the user and return error key 2.
    } else {
        cout << "Config file [" << fileName << "] could not be opened. ";
        //system("pause");
    }
}
#endif
