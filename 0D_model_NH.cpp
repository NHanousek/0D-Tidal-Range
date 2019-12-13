
// Implements functions of the 0D Model

#include "0D_model_NH.h"
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;


//Printers
void variable::printVariable() {
	cout << "<" << name << "> =" << value << units << " MODE <"<<mode<<">"<< endl;
};

void variable::setValue(double* newValue) {
	if (mode == 0) {
		cout << "ATTEMPTING TO EDIT FIXED VARIABLE <" << name << ">" << endl;
	}
	value = *newValue;
}

void variable::printMode() {
	string mstring;
	switch (mode) {
		case 0:
			mstring = "a Constant Input"; break;
		case 1:
			mstring = "a Flexible Input"; break;
		case 2:
			mstring = "an Output"; break;
		default:
			mstring = "UNDEFINED"; break;
	}
	cout << "<" << name << "> variable mode is " << mstring << endl;
}
//Constructrs/Destructos
variable::variable() : mode(999) {
	value = 0.0;
	units = "NOUNITS";
	name = "NONAME";
};
variable::variable(double val, int varMode, std::string varName, std::string varUnits) : value(val), mode(varMode), name(varName), units(varUnits) {};

variable::~variable() {};

//http://www.cplusplus.com/doc/tutorial/pointers/

/*
0-D Model of the lagoons
348		!Simulation Time (Hr)!28TIDE348
60		!Time step Second
2		!Scheme 1: ebb only 2: 2way generation
1		!Turbine Type: 1:Bulb	2:Rolls Royce (using orifice Eq)
T       !Flag_PowerCapa Total opening area of sluices(m2)
4       !PowerCapa if Flag_Asluices is F
8       !PowerCapa_max if Flag_Asluices is T
4       !PowerCapa_min if Flag_Asluices is T
1       !delta_PowerCapa if Flag_Asluices is T
T       !Flag_NumTB
100     !NumTB if Flag_NumTB is F
250     !NumTB_max:Total Number of turbines
100     !NumTB_min:Total Number of turbines
5       !delta_NumTB:Total Number of turbines
7.2     !turbinesDi:Diameter of turbines(m)
20      !Rate_per_TB
3.5		!DZmax: Maximum water head for power generation
2.0		!DZmin: Minimum water head for power generation
T		!Oper_Opt_FLG	Flag for optimisation of Zmax (start of the generation) and Zmin (End of the generation)
8.0		!DZmax_max: Maximum of DZ_max
2.0		!DZmax_min: Mainimum of DZ_max
0.1		!DelZ_ZMax: step size of DZ_max for optimisation
4.5		!DZmin_max: Maximum of DZ_min
0.1		!DZmin_min: Maximum of DZ_max
0.1		!DelZ_ZMin: step size of DZ_min for optimisation
0.0		!TimeAD, adjustment time for the barrage(Seconds)
1.0		!CoefGT: efficiency of generator
1.0		!CoefQ_Sluice: Discharge Coefficient for sulice
1.0		!CoefQ_Turb: Discharge Coefficient for Turbines during filling
10      !Hmax
-10     !Hmin
0.1     !Delta_H:(Hmax-Hmin)/Delta_H need to be integer
T       !variable_area:lagoonA is a constant(F) or not(T)
90      !If Lagoon Area is a constant value
8       !POP_SIZE
0.5     !P_MUTATE
0.5     !P_RECOMBINE
1       !P_SELECTION
T       !set target (T) ot not (F)
30000   !TARGET_RESULTS
1       !TIME_MACHINE
1       !input_solution (1:from SA, 2:from constant random number 3:flexible random number)
59      !size of each operational head schemes
T       !circular genetic programming (T) or not[linear] (F)
0.1     !GAUSS distribution in mutation -----KSI
*/
/*
#ifndef 0D_model_NH
#define 0D_model_NH

// Define all the variables and functions used in the 0D Model

#include <string>
#include <vector>

struct var {
	double value;
	int mode;
	std::string units;
	setValue(double newValue);
	setName(std::string newName);
	printVar();
	var();
	var(double val, int varMode, std::string varName);
	~var();
};

#endif
*/