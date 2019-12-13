#include "0D_model_NH.h"
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

// ReadData: Reads the data file provided in the order defined below (lagoon.txt):
int ReadData(string* Input_Fname) {
	int errorStat = 0;
	string GA_name = *Input_Fname + ".GA";
	string Lag_name = *Input_Fname + ".Lag";

	ifstream GA_input(GA_name);
	ifstream Lag_input(Lag_name);

	if (GA_input.is_open() == false) {
		cout << "Unable to open <" << GA_name << endl;
		if (Lag_input.is_open() == false) {
			cout << "Unable to open <" << Lag_name << endl;
			return 0;
		}
		return 0;
	}

};

GA_Config::GA_Config(double crossover, double mutation, int population, int Mode) {
	variable xRate(crossover, 0, "Crossover rate", "[-]");
	variable mRate(mutation, 0, "Mutation Rate", "[-]");
	variable pop(population, 0, "Population size", "[-]") 
	mode(Mode)

}