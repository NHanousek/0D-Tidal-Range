
#ifndef ZERO_D_NH
#define ZERO_D_NH

// Define all the variables and functions used in the 0D Model

#include <string>
#include <vector>

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
}
#endif
