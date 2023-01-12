#include <iostream>
#include <string>
#include "zeroDModel.hpp"

using namespace std;

int main(int argc, char *argv[]){

    ofstream test;
    test.open("run.txt");
    test.close();

    string cfgFileName;
    
    if (argc == 1) {
        cfgFileName = "BarraCUDA.dat";
    } else if (argc == 2){
        cfgFileName = argv[1];
        if (cfgFileName == "--help"){
            cout << "Perhaps try emailing Nick? [HanousekN@Cardiff.ac.uk]" << endl
                 << "or enter your config file name: (default is BarraCUDA.dat) (0 will end the program)" << endl;
            cin >> cfgFileName;
            if (cfgFileName == "0") {
                return 0;
            }
        }
    } else {
        cout << "Too many command line input arguments for this model build." << endl;
        return 1;
    }
    cout << "BarraCUDA 2.18" << endl;
    modelConfig cfg(cfgFileName);


    if (cfg.isGenetic) {
        
    }
    else {
        cout << "Gridded Run Starting" << endl;
        // the model is a singleton run
        zeroDModel model(cfg);
        model.run();
        //model.writeResults(cfg.runResultsFileName);
    }
    cout << "All done!" << endl;
    return 0;
}
