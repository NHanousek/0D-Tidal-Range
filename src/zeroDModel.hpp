//zeroDModel.h
//contains definitions of the 0D model class.

#ifndef ZERO_D_MODEL
#define ZERO_D_MODEL

// Standard libraries
#include <string>
#include <vector>
#include <iostream>

// Model specific libraries
#include "tidalRangeScheme.hpp"
#include "timeSeries.hpp"
#include "Generals.hpp"
#include "modelConfig.hpp"
#include "modelResults.hpp"
#include "TypesDef.h"

using namespace std;

class zeroDModel {
private:
	timeSeries energyValue; // value of energy at a given time (£/MW)
	vector<tidalRangeScheme> trs; // the tidal range scheme, pumps, turbines, sluices, area
	modelResults mResults; // results, as yet, don't know how best to treat this...
	int numSchemes = 1; // how many schemes are there in the model
	bool printFullResults = false; // do we want the full results for every timestep?
public:
	zeroDModel(const modelConfig& cfg); // iniitalise using config class
	void run(); // run the model
	//zeroDMFitness fitness(const vector<string>& params); // calculate the fitness

	string title = "NaN";

	double maxTime = 0;		 // maximum time to simulate (hrs) (constant)
	double simTime = 0;		 // current time (hrs) (at time)
	double timeStep = 0;	 // time step (hrs) (constant)
	double energySum = 0;		// MegaWatt Hours (cumulative)
	double profit = 0; // pounds or any other abstract currency (cumulative)

	// vectors here of length numSchemes
	vector<string> schemeNames; // names from the scheme files, for ID of results
	vector<double> downStream;	// downstream (external) water level
	vector<double> upStream;		// upstream (internal) water level
	vector<double> wetArea;			// wetted area of the tidal range scheme
	vector<double> powerOut;		// power output at a time Mega Watts
	vector<double> turbineQ;		// flow rate through all turbines (m3/s)
	vector<double> sluiceQ;			// flow rate through sluices (m3/s)
	vector<int> mode;						// operating mode

	// vector<int> ancMode;				// ancillary services mode
	// vector<double> andPrice;		// ancillary services price/time
};

//------------------IMPLEMENTATIONS------------------//
// constructor reads the config class and builds a model.
zeroDModel::zeroDModel(const modelConfig& cfg) {
	// take the following straight from the cfg file, as they must be constant
	// between schemes etc.
	title = cfg.modelName;
	timeStep = cfg.timeStep;
	maxTime = cfg.maxTime;
	numSchemes = cfg.numSchemes;
	printFullResults = cfg.printFullResults;
	string modResFile = title + "_results.csv";
	vector<string>  schemeTitles;


	cout << "Model has [" << numSchemes << "] scheme(s)." << endl
		<< "The first is [" << cfg.schemeFileName[0] << "]" << endl;
	for (int i = 0; i < numSchemes; i++) {
		cout << "Loading scheme [" << (i+1) << "] from: " << (cfg.schemeFileName[i]) << endl;
		trs.push_back(tidalRangeScheme(cfg.schemeFileName[i]));
		cout << ", done." << endl;
		downStream.push_back(trs[i].getDownStream());
		upStream.push_back(trs[i].getUpstream());
		wetArea.push_back( trs[i].getWetArea());
		powerOut.push_back(0);
		turbineQ.push_back(0);
		sluiceQ.push_back(0);
		mode.push_back(2); // lagoon starts off as holding
		schemeTitles.push_back(trs[i].getName());
	}

	mResults = modelResults(modResFile, schemeTitles);
	energyValue = timeSeries(cfg.energyValueFileName);
	cout << "Config done." << endl;
};

// run the model -- this could be the most important function of all.
void zeroDModel::run() {
	// simtime starts as 0.
	mResults.add(simTime, downStream, upStream, wetArea, powerOut, turbineQ,
		sluiceQ, mode, energySum, profit);// ds us wa po tq sq mo es pr

	while (simTime < maxTime) {
		if (printFullResults) {
				cout << " Time: " << simTime << " Hrs    Mode: " << mode[0] << endl;
		}
		simTime += timeStep;

		// because we index from zero here.
		for (int i = 0; i < numSchemes; i++) {
			trs[i].updateTo(simTime); // behaves differently depending on lagoon config.

			downStream[i] = trs[i].getDownStream();
			upStream[i] = trs[i].getUpstream();
			wetArea[i] = trs[i].getWetArea();
			powerOut[i] = trs[i].getPowerOut();
			turbineQ[i] = trs[i].getTurbineQ();
			sluiceQ[i] = trs[i].getSluiceQ();
			mode[i] = trs[i].getMode();
			energySum += powerOut[i]*timeStep; // timeStep is in hrs, power is in MW
			profit += timeStep*powerOut[i]*energyValue.getLevel(simTime);
		}
		mResults.add(simTime, downStream, upStream, wetArea, powerOut, turbineQ,
			sluiceQ, mode, energySum, profit);// ds us wa po tq sq mo es pr
	}
};

// calculate the fitness
//zeroDFitness zeroDModel::fitness(const vector<string>& params) {};

#endif
