
#ifndef ZERO_D_NH
#define ZERO_D_NH

// Define all the variables and functions used in the 0D Model

#include <string>
#include <vector>
#include "TypesDef.h"
using namespace std;


class tidalRangeScheme {
public:
	string source, title;					//metadata
	double simTime;							// in hours
	tdouble3 timeStep;						// hours(x), minutes(y), seconds(z) so you dont have to convert
	tdouble3 timeAdjust;					// time for changing modes h(x) min(y) sec(z)
	int schemeType, turbineType;			// Scheme 1: ebb only 2: 2way generation, Turbine Type: 1:Bulb	2:Rolls Royce (using orifice Eq)
	int numberTurbines;						// number of turbines
	double areaSluices;						// total area of sluice gates
	double turbineDiameter, turbineArea;	// turbine diameter and total area of turbines
	double headDiffStart, headDiffEnd;		// max and min head differences for generating
	double waterLevelMax, waterLevelMin;	// max and min levels for external water level calc
	int numWaterLevelPoints;				// how much should external wl be splt up
	double sluiceCoefQ, turbineCoefQ;		// discharge coefficients
	double generatorEfficiency;				// generator efficiency

	tidalRangeScheme(const string& fileName);	// constructor
	tidalRangeScheme();							// default/error constructor
	~tidalRangeScheme() {};						// destructor

	double sluiceFlowRate(const double& headDifference); // finds flow rate thrugh sluices using
														 // orifice equation

};

class t3sMesh {
public:
	string title;								// metadata, typically just filename of mesh t3s
	int numberNodes, numberElements;			// number of node/element in mesh
	vector<tdouble3> nodePoints;				// xyz coordinates of nodes
	vector<tint3> neighbourhood;				// neighbours that meake up each element
	vector<double> elementArea, elementHeight;	// area and mean height of each element

	t3sMesh(const string& fileName);			// full constructor
	t3sMesh();									// error cnstructor
	~t3sMesh() {};								// destructor
};

class externalWaterLevel {
public:
	string title,sourceFile;	// metadata, mesh title and model title
	int numberReadings;			// number of steps
	double xUTM, yUTM, dt;		// dt in hours
	vector<double> time, level;	// times and water levels

	externalWaterLevel(const string& fileName);		// full constructor
	externalWaterLevel();							// error constructor
	double getExternalWL(const double& timeNow);	// get the water level at any given time
};

class turbines {
public:
	string title;	// filename typically
	double originalDiameter, turbineDiameter; // original diameter from file, turbine is to be used here
	vector<double> powerHeadDifference, powerOutput; // power output head levels and values
	vector<double> flowHeadDifference, flowRate; // flow rates and their head differences
	double totalArea, coeffDischarge; // total area from area of circle, Cd from tidal range scheme
	int numHPPoints, numHQPoints; //number of Head-Power and Head-Discharge points

	turbines(const tidalRangeScheme& trs, const string& fileName); // constructor
	turbines();	// error constructor
	double getFlowRate(const double& headDifference, const tidalRangeScheme& trs); // get the flow rate from head difference
	double getPowerOutput(const double& headDifference, const tidalRangeScheme& trs); // get the power from head difference
	double getFillingFlow(const double& headDifference); // uses orifice equation
	~turbines() {}; // destructor
};

class schemeArea {
public:
	string title, meshTitle, lagoonTitle; // metaData
	int numPoints;	// number of points at which the water level is calculated
	vector<double> level, area; // levels and corresponding areas
	
	schemeArea(const tidalRangeScheme& trs, const t3sMesh& mesh); // constructor
	double getWettedArea(const double& internalWaterLevel);	// uses interpolation
	~schemeArea() {}; // destructor
};

class results {
public:
	string title, info; // metadata
	vector<double> modelTimeHr; // every timestep
	vector<double> upstreamWaterLevel; // every upstream water level
	vector<double> downstreamWaterLevel; // every downstream water level
	vector<double> headDifference; // every head difference used
	vector<double> wettedArea; // every wetted area
	vector<double> powerOutput; // every power output
	vector<double> powerGenerated; // power output cumulative total
	vector<double> turbineFlow; // flow through turbines
	vector<double> sluiceFlow; // flow through sluices
	vector<int>	lagoonMode; // operating mode of lagoon

	results(const tidalRangeScheme& trs); // blank constructor
	results(const tidalRangeScheme& trs, const double& time, const double& upstreamWL, const double& downstreamWL, const double& headDiff, const double& wetArea, const double& powerOut, const double& turbineQ, const double& sluiceQ, const int& trsMode); // first line constructor
	void addResults(const double& time, const double& upstreamWL, const double& downstreamWL, const double& headDiff, const double& wetArea, const double& powerOut, const double& turbineQ, const double& sluiceQ, const int& trsMode); // effectively push_back for the data
	string header(); // currently header == info, but this may change
	string line(const int& i);	 // one lne of results as string
	void line(const int& i, const string& fileName); // print a singe line to a file
	void printFull();	// full results to user
	void printFull(const string& fileName);	// full results to file
	void calculateNextWL(); // this one needs some work?
};

class modelConfig {
public: // basically stores all the file names
	string LagoonFileName, meshFileName, resultsFileName;
	string externalWaterLevelFileName, turbinesFileName; /*, geneticFileName;*/
	//int genetic;
	modelConfig(const string& configFileName);
};

int nextMode(const tidalRangeScheme& trs, const results& previous);
double newUpstreamLevel(const double& oldUpstreamLevel, const double& flowTurbines, const double& flowSluices, const double& inFlow, const schemeArea& area, const tidalRangeScheme& trs);


#endif

/*
//I don't think the sluices class actually needs to exist...
class sluices {
public:

};
*/
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