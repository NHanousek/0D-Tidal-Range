#include <iostream>
#include <string>
#include "zeroDModel.hpp"
#include "genetics.hpp"

using namespace std;

int main(int argc, char *argv[]){

    string cfgFileName;

    if (argc == 1) {
        cfgFileName = "BarraCUDA.dat";
    } else if (argc == 2){
        cfgFileName = argv[1];
    } else {
        cout << "Too many command line input arguments for this model build." << endl;
        return 1;
    }
    cout << "BarraCUDA 2.10" << endl;
    modelConfig cfg(cfgFileName);


    if (cfg.isGenetic) {
        /*
        int nGens = 0;
        // generate initial population
        zeroDMPopulation parents(cfg);

        // run initial population
        parents.runPopulation();

        // evaulate fitness
        parents.evalFitness();

        // record fitness
        parents.writePopulationAndFitness(cfg);

        // while no breakpoint
        while (nGens <= cgf.maxGenerations()) {
            nGens++;

            // select best solutions
            zeroDMPopulation children = parents.selection();

            // crossover between parents
            children.crossover();

            // mutation of children
            children.mutate();

            // run children
            children.run();

            // children fitness
            children.evalFitness();

            // record children
            children.writePopulationAndFitness();

            // check endpoint criteria
            if (children.meetBreakPoint(cfg,nGens)) {
                break;
            }
            parents = children;
            children.~zeroDMPopulation();
        }
        // record results
        */
        //system("pause");
    }
    else {
        cout << "Singleton Run Starting" << endl;
        // the model is a singleton run
        zeroDModel model(cfg);
        model.run();
        //model.writeResults(cfg.runResultsFileName);
        cout << "All done!" << endl;
    }
    return 0;
}
