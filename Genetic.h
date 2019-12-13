#ifndef Genetic
#define Genetic

// Define all the variables and functions used in the Genetic algorithm part
// like the start of a Fortran file

#include <string>
#include <vector>

class population {
private:
	std::string name;
	const int size;
public:
	int getSize();
	std::string getName();

	population mutatePop(population initial, double mutationRate);
	population selectPop(population initial, int mode);
	population crossoPop(population initial, int mode);

	population();
	~population();
};
#endif