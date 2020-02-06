
#ifndef ZERO_D_NH
#define ZERO_D_NH

// Define all the variables and functions used in the 0D Model

#include <string>
#include <vector>
#include "General.h"
using namespace std;
/*
class variable {
public:
	double value;				// perhaps a bit generic to assume everything is double...
	const int mode;				// 0 if constant, 1 if to be modified, 2 if output, 999 if error;
	std::string units, name;	// mainly for use in printing etc, not sure how they will be generated

	// functions to modify the internal components
	void setValue(double* newValue);
	void setUnits(std::string* newUnits) { units = *newUnits; };
	void setName(std::string* newName) { name = *newName; };

	// Return mode and variable info as string
	std::string strVariable();
	std::string strMode();
	// dispay full variable info to the console, may change to just return string
	void printVariable();
	// dispay variable mode info to the console, mainly for errors
	// may change to just return string
	void printMode();

	// constructors and destructors
	variable();
	variable(double val, int varMode, std::string varName, std::string varUnits);
	~variable();
};

class GA_Config {
public:
	variable xRate, mRate, pop, mode;
	GA_Config();
	GA_Config(double crossover, double mutation, int population, int Mode);
};

class xdouble3 {
public:
	double x1, x2, x3;
};
*/

class tidalRangeScheme {
public:
	string source, title; //metadata
	double simTime; // in hours
	tdouble3 timeStep; // hours(x), minutes(y), seconds(z) so you dont have to convert
	tdouble3 timeAdjust; // time for changing modes h(x) min(y) sec(z)
	int schemeType, turbineType; // Scheme 1: ebb only 2: 2way generation, Turbine Type: 1:Bulb	2:Rolls Royce (using orifice Eq)
	int numberTurbines; // number of turbines
	double areaSluices; // total area of sluice gates
	double turbineDiameter, turbineArea; // turbine diameter and total area of turbines
	double headDiffMax, headDiffMin; // max and min head differences for generating
	double waterLevelMax, waterLevelMin; // max and min levels for external water level calc
	int numWaterLevelPoints; // how much should external wl be splt up
	double sluiceCoefQ, turbineCoefQ; // discharge coefficients
	double generatorEfficiency;	// generator efficiency

	tidalRangeScheme(const string& fileName);
	tidalRangeScheme();

	double sluiceFlowRate(const double& headDifference);

};

class t3sMesh {
public:
	string title;
	int numberNodes, numberElements;
	vector<tdouble3> nodePoints;
	vector<tint3> neighbourhood;
	vector<double> elementArea, elementHeight;

	t3sMesh(const string& fileName);
	t3sMesh();
};

class externalWaterLevel {
public:
	string title,sourceFile;
	int numberReadings;
	double xUTM, yUTM, dt; //dt in hours
	vector<double> time, level;

	externalWaterLevel(const string& fileName);
	externalWaterLevel();
	double getExternalWL(const double& timeNow);
};

class turbines {
public:
	string title;
	double originalDiameter, turbineDiameter;
	vector<double> powerHeadDifference, powerOutput, flowHeadDifference, flowRate;
	double totalArea, coeffDischarge;
	int numHPPoints, numHQPoints; //number of Head-Power and Head-Discharge points

	turbines(const tidalRangeScheme& trs, const string& fileName);
	turbines();
	double getFlowRate(const double& headDifference, const tidalRangeScheme& trs);
	double getPowerOutput(const double& headDifference, const tidalRangeScheme& trs);
	double getFillingFlow(const double& headDifference);
};

class schemeArea {
public:
	string title, meshTitle, lagoonTitle;
	int numPoints;
	vector<double> level, area;
	
	schemeArea(const tidalRangeScheme& trs, const t3sMesh& mesh);
	schemeArea();
	getWettedArea(const double& internalWaterLevel);

};
/*
//I don't think the sluices class actually needs to exist...
class sluices {
public:

};
*/
class results {
public:

};

#endif
