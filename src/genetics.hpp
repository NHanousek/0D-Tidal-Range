// genetic algorithm components for 0D modelling of trs...
#ifndef ZERO_D_POPULATION
#define ZERO_D_POPULATION

#include "zeroDModel.hpp"
#include "modelConfig.hpp"
#include <vector>

using namespace std;

class zeroDMPopulation {
    private:
        //vector<zeroDModel> modelPopulation; // stores the population objects.
        //vector<zeroDMFitness> modelFitness; // stores the fitness objects.
        string selectionMethod; // determines the selection method to be used
        double mutationRate; // rate of mutation
        int bestN;
    public:
        zeroDMPopulation(const modelConfig& cfg) {}; // constructor.
        void runPopulation() {}; // runs all the models in the population
        void evalFitness() {}; // evaluates the fitness of the population
        void writePopulation() {}; // write the design info of the population to file
        void writePopulationAndFitness() {}; // writes the fitnesses to a file
        void selection() {}; // selects the n best using 'method'
        void crossover() {}; // crossover between the newly models to produce offspring
        void mutate() {}; // mutate population randomly at given rate
};
/*
class zeroDMFitness {
private:
    vector<string> params; // what parameters are to be used for fitness calc
    vector<vector<double>> values; // the values of those parameters
    vector<double> fitness; // the fitnesses?
public:
    zeroDMFitness() {};
};
*/
//------------------IMPLEMENTATIONS------------------//
#endif
