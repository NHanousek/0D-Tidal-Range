// 0D_Genetic_NH_C.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "0D_model_NH.h"

using namespace std;
int main()
{
	//Configure the model
	string cfgName = "BarraCUDA.txt";
	modelConfig config = modelConfig(cfgName);

	//Load the parameters
	//tidalRangeScheme
	tidalRangeScheme TRS = tidalRangeScheme(config.LagoonFileName);
	//t3sMesh
	t3sMesh mesh = t3sMesh(config.meshFileName);
	//externalWaterLevel
	externalWaterLevel downstreamWL = externalWaterLevel(config.externalWaterLevelFileName);
	//turbines
	turbines TRBN = turbines(TRS, config.turbinesFileName);
	//schemeArea
	schemeArea Area = schemeArea(TRS, mesh);
	
	
	//initialise results for first (0) timestep

	results modelResults = results(TRS);
	
	// 



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
