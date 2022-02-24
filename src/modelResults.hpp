
#ifndef ZERO_D_RESULTS
#define ZERO_D_RESULTS

#include <vector>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;
class modelResults {
public:
	string title = "NaN"; // stores a title line?
	int numRows = 0; // number of rows of data
	string runResultsFile;
	vector<string> schemeName; // stores the names of the schemes for header line
	ofstream resultsFile; // the file that the results get pushed to

	// opens the output file stream and adds the header.
	modelResults(const string& RunResultsFileName, const vector<string> SchemeNames);
	modelResults() = default;

	// adds a line of results and increases the numRows counter.
	void add(const double& Time,
		const vector<double>& DownStream,
		const vector<double>& UpStream,
		const vector<double>& WetArea,
		const vector<double>& PowerOut,
		const vector<double>& TurbineQ,
		const vector<double>& SluiceQ,
		const vector<int>& Mode,
		const double& EnergySum,
		const double& Profit
	//	const vector<int>& ancMode,
	//	const vector<double>& ancPrice
	);
};

modelResults::modelResults(const string& RunResultsFileName, const vector<string> SchemeNames) {
	// initialise the file stream as an 'append' type.
	schemeName = SchemeNames;
	runResultsFile = RunResultsFileName;
	ofstream resultsFile;
	resultsFile.open(runResultsFile);
	if (resultsFile.is_open()) {
		resultsFile << "Time[Hrs],";
		for (int i = 0; i < schemeName.size(); i++) {
			resultsFile << SchemeNames[i] + "_DS[mCD]," << SchemeNames[i] + "_US[mCD],"
				<< SchemeNames[i] + "_Area[km2]," << SchemeNames[i] + "_power[MW],"
				<< SchemeNames[i] + "_turbQ[m3/s]," << SchemeNames[i] + "_SlQ[m3/s],"
				<< SchemeNames[i] + "_Mode[-],";
		}
		resultsFile << "energy[MWh],value[Currency]" << endl;
	}
	else {
		cout << "Unable to open [" << runResultsFile << "]..." << endl;
		//system("pause");
	}
	resultsFile.close();

};
void modelResults::add(const double& Time, const vector<double>& DownStream, const vector<double>& UpStream, const vector<double>& WetArea, const vector<double>& PowerOut, const vector<double>& TurbineQ, const vector<double>& SluiceQ, const vector<int>& Mode, const double& EnergySum, const double& Profit) {
	ofstream resultsFile;
	resultsFile.open(runResultsFile, ofstream::app);
	if (resultsFile.is_open()) {
		resultsFile << Time << ",";
		for (int i = 0; i < schemeName.size(); i++) {
			resultsFile << DownStream[i] << "," << UpStream[i] << "," << WetArea[i]/1000000 << "," << PowerOut[i] << "," << TurbineQ[i] << "," << SluiceQ[i] << "," << Mode[i] << ",";
		}
		resultsFile << EnergySum << "," << Profit << endl;
		numRows++;
	}
	else {
		cout << "Unable to open [" << runResultsFile << "]..." << endl;
		//system("pause");
	}
	resultsFile.close();
}; //time ds us wa po tq sq mo es pr
#endif
