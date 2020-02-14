// 0D_Genetic_NH_C.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "0D_model_NH.h"

using namespace std;
int main()
{
	//Configure the model
	string cfgName = "BarraCUDA.txt";
	modelConfig config(cfgName);

	//Load the parameters
	//tidalRangeScheme
	tidalRangeScheme TRS(config.LagoonFileName);
	//t3sMesh
	t3sMesh mesh(config.meshFileName);
	//externalWaterLevel
	externalWaterLevel downstreamWL(config.externalWaterLevelFileName);
	//turbines
	turbines TRBN(TRS, config.turbinesFileName);
	//schemeArea
	schemeArea Area(TRS, mesh);
	Area.printScheme("SchemeArea.out");

	cout << "Initialising results:" << endl;
	//The current model values at every timestep
	//initialise results for first (0) timestep
	//initial internal water level = initial external water level
	double modelTime = 0;										cout << "Initial time= " << modelTime << endl;
	double downstream = downstreamWL.getExternalWL(modelTime);	cout << "Initial downstream= "<< downstream << endl;
	double upstream = downstream;								cout << "Initial upstream= " << upstream << endl;
	double headDiff = upstream - downstream;					cout << "Initial head difference= " << headDiff << endl;
	double wetArea = Area.getWettedArea(upstream);				cout << "Initial area= " << wetArea << endl;
	double powerOut = TRBN.getPowerOutput(headDiff, TRS);		cout << "Initial power= " << powerOut << endl;
	double turbineQ = TRBN.getFlowRate(headDiff, TRS);			cout << "Initial turbine flowrate= " << turbineQ << endl;
	double sluiceQ = TRS.sluiceFlowRate(headDiff);				cout << "Initial sluice flowrate= " << sluiceQ << endl;
	int trsMode = 2;											cout << "Initial mode= " << trsMode << endl << endl;
	const double inFlow = 0.0; // as yet unbuilt, because the current lagoon does not need it

	cout << "Area is 0m2 at z= "<<  Area.getWaterLevel(0) << endl;

	results Results(TRS, modelTime, upstream, downstream, headDiff, wetArea, powerOut, turbineQ, sluiceQ, trsMode);
	
	//Results.header(config.resultsFileName);
	//Results.line(0, config.resultsFileName);

	cout << Results.header() << endl;

	//FULLY EXPLICIT - EULER MODEL
	cout << "Starting Simulation:" << endl;
	for (int i = 1; Results.modelTimeHr.back() < TRS.simTime; i++) {
		//cout << "Model Time = " << modelTime << " hours" << endl;
		// Externally driven
		modelTime += TRS.timeStep.x; // incremement time
		downstream = downstreamWL.getExternalWL(modelTime); // the tide moves the downstream

		// wetting drying check
		double tmpUpstream = newUpstreamLevel(upstream, turbineQ, sluiceQ, inFlow, Area, TRS);
		double tmpWetArea = Area.getWettedArea(tmpUpstream);

		if (tmpWetArea <= 0) {
			wetArea = 0;
			upstream = Area.getWaterLevel(0);
		}
		else {
			upstream = tmpUpstream;
			wetArea = tmpWetArea;
		}

		// and that tide changes the head difference between upstream and downstream
		headDiff = upstream - downstream; // up - down.

		// Based on previous step and current head difference
		trsMode = nextMode(TRS, Results, headDiff); // determine next lagoon mode	
		
		// Operation mode based
		if (TRS.schemeType == 1 || TRS.schemeType == 2) {
			// determine next mode based on previous conditions
			switch (trsMode) {
			case 1: // Filling/sluicing
				powerOut = 0;
				turbineQ = TRBN.getFillingFlow(headDiff);
				sluiceQ = TRS.sluiceFlowRate(headDiff);
				break;
			case 2: // Holding
				turbineQ = sluiceQ = powerOut = 0;// No flow thrugh the turbines or sluices
				break;
			case 3: // Generating
				powerOut = TRBN.getPowerOutput(headDiff, TRS);
				turbineQ = TRBN.getFlowRate(headDiff, TRS);
				sluiceQ = TRS.sluiceFlowRate(headDiff);
				break;
			default:
				cout << "Invalid tidal range scheme mode [" << trsMode<< "] entered..." << endl;
				return 1;
			}
		} 
		// These will be the codes for pumped ebb and two way operation
		else if (TRS.schemeType == 11 || TRS.schemeType == 21) {
			cout << "Pumping not yet included in model!" << endl;
			return 2;
		}
		else {
			cout << "Invalid tidal range scheme type entered [" << TRS.schemeType << "]..." << endl;
			return 1;
		}
/*
		double tmpUpstream = newUpstreamLevel(upstream, turbineQ, sluiceQ, inFlow, Area, TRS);
		double tmpWetArea = Area.getWettedArea(tmpUpstream);

		 wetting drying check
		if (tmpWetArea <= 0) {
			wetArea = 0;
			upstream = Area.getWaterLevel(0);
		}
		else {
			upstream = tmpUpstream;
			wetArea = tmpWetArea;
		}*/

		//save outputs to results
		Results.addResults(modelTime, upstream, downstream, headDiff, wetArea, powerOut, turbineQ, sluiceQ, trsMode);
		cout << Results.line(i) << endl;
	}
	
	
	// 
	cout << "Printing results to file." << endl;
	Results.printFull(config.resultsFileName);

	cout << "All done." << endl;
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
