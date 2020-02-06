
// Implements functions of the 0D Model

#include "0D_model_NH.h"
#include "General.h"
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
tidalRangeScheme::tidalRangeScheme(const string& fileName) {
	string tmp = source = fileName;
	ifstream lagoonFile;
	lagoonFile.open(fileName);
	if (lagoonFile.is_open()) {
		string tmp;
		/*
		enum argName {
			simTime,
			timeStep_sec,
			timeStep_min,
			timeStep_hrs,
			timeAdjust_sec,
			timeAdjust_min,
			timeAdjust_hrs,
			schemeType_k,
			turbineType_k,
			numberTurbines_int,
			areaSluices_m2,
			turbineDiameter_m,
			turbineArea_m2,
			headDiffMax_m,
			headDiffMim_m,
			waterLevelMax_m,
			waterLevelMin_m,
			numWaterLevelPoints_int,
			sluiceCoefQ_0_1,
			turbineCoefQ_0_1,
			generatorEfficiency_0_1,
			generatorEfficiency_percent
		};
		*/
		getline(lagoonFile, title);
		while (tmp != "end" && tmp != "End" && tmp != "END") {
			lagoonFile >> tmp;

			//this may be the biggest nastiest if else I've ever made
			if (tmp == "simTime(hrs)=") {
				lagoonFile >> simTime;
			}
			else if (tmp == "timeStep(sec)=") {
				lagoonFile >> timeStep.z;
				timeStep.y = timeStep.z / 60.0;
				timeStep.x = timeStep.y / 60.0;
			}
			else if (tmp == "timeStep(min)=") {
				lagoonFile >> timeStep.y;
				timeStep.x = timeStep.y / 60.0;
				timeStep.z = timeStep.y * 60.0;
			}
			else if (tmp == "timeStep(hrs)=") {
				lagoonFile >> timeStep.x;
				timeStep.y = timeStep.x * 60.0;
				timeStep.z = timeStep.y * 60.0;
			}
			else if (tmp == "timeAdjust(sec)=") {
				lagoonFile >> timeAdjust.z;
				timeAdjust.y = timeAdjust.z / 60.0;
				timeAdjust.x = timeAdjust.y / 60.0;
			}
			else if (tmp == "timeAdjust(min)=") {
				lagoonFile >> timeAdjust.y;
				timeAdjust.x = timeAdjust.y / 60.0;
				timeAdjust.z = timeAdjust.y * 60.0;
			}
			else if (tmp == "timeAdjust(hrs)=") {
				lagoonFile >> timeAdjust.x;
				timeAdjust.y = timeAdjust.x * 60.0;
				timeAdjust.z = timeAdjust.y * 60.0;
			}
			else if (tmp == "schemeType(k)=") {
				lagoonFile >> schemeType;
			}
			else if (tmp == "turbineType(k)=") {
				lagoonFile >> turbineType;
			}
			else if (tmp == "numberTurbines(int)=") {
				lagoonFile >> numberTurbines;
			}
			else if (tmp == "areaSluices(m2)=") {
				lagoonFile >> areaSluices;
			}
			else if (tmp == "turbineDiameter(m)=") {
				lagoonFile >> turbineDiameter;
			}
			else if (tmp == "turbineArea(m2)=") {
				lagoonFile >> turbineArea;
			}
			else if (tmp == "headDiffMax(m)=") {
				lagoonFile >> headDiffMax;
			}
			else if (tmp == "headDiffMim(m)=") {
				lagoonFile >> headDiffMin;
			}
			else if (tmp == "waterLevelMax(m)=") {
				lagoonFile >> waterLevelMax;
			}
			else if (tmp == "waterLevelMin(m)=") {
				lagoonFile >> waterLevelMin;
			}
			else if (tmp == "numWaterLevelPoints(int)=") {
				lagoonFile >> numWaterLevelPoints;
			}
			else if (tmp == "sluiceCoefQ(0-1)=") {
				lagoonFile >> sluiceCoefQ;
			}
			else if (tmp == "turbineCoefQ(0-1)=") {
				lagoonFile >> turbineCoefQ;
			}
			else if (tmp == "generatorEfficiency(0-1)=") {
				lagoonFile >> generatorEfficiency;
			}
			else if (tmp == "generatorEfficiency(%)=") {
				double percentEff;
				lagoonFile >> percentEff;
				generatorEfficiency = percentEff / 100;
			}
			else {
				string tmp2;
				lagoonFile >> tmp2;
				cout << "Unrecognised Lagoon Parameter [" << tmp << "] = " << tmp2 << " read." << endl;
			}


			/*switch (tmp) {

				
				//what I was hoping to do...
				case "simTime="
				case "timeStep(sec)="
				case "timeStep(min)="
				case "timeStep(hrs)="
				case "timeAdjust(sec)="
				case "timeAdjust(min)="
				case "timeAdjust(hrs)="
				case "schemeType(k)="
				case "turbineType(k)="
				case "numberTurbines(int)="
				case "areaSluices(m2)="
				case "turbineDiameter(m)="
				case "turbineArea(m2)="
				case "headDiffMax(m)="
				case "headDiffMim(m)="
				case "waterLevelMax(m)="
				case "waterLevelMin(m)="
				case "numWaterLevelPoints(int)="
				case "sluiceCoefQ(0-1)="
				case "turbineCoefQ(0-1)="
				case "generatorEfficiency(0-1)="
				case "generatorEfficiency(%)="
				case default:
				
			}*/
		}

		lagoonFile.close();
	}
	else {
		cout << "Lagoon file [" << fileName << "] could not be opened." << endl;
		tidalRangeScheme();
	}	
}
tidalRangeScheme::tidalRangeScheme() {
	source = title = "ERROR";
	simTime = 0.0;
	timeStep.x = timeStep.y = timeStep.z = 0.0;
	timeAdjust.x = timeAdjust.y = timeAdjust.z = 0.0;
	schemeType = turbineType = numberTurbines = numWaterLevelPoints= 0;
	areaSluices = turbineDiameter = turbineArea = headDiffMax = 0;
	headDiffMin = waterLevelMax = waterLevelMin = generatorEfficiency = 0;
	sluiceCoefQ = turbineCoefQ = 0;
}
double tidalRangeScheme::sluiceFlowRate(const double& headDifference) {
	return orificeFlow(sluiceCoefQ, headDifference, areaSluices);
}



t3sMesh::t3sMesh(const string& fileName) {
	ifstream meshFile;
	meshFile.open(fileName);
	if (meshFile.is_open()) {
		title = fileName;
		string tmp = fileName;
		while (tmp != ":EndHeader") {
			meshFile >> tmp;
			if (tmp == ":NodeCount") {
				meshFile >> numberNodes;
			}
			else if (tmp == ":ElementCount") {
				meshFile >> numberElements;
			}
		}
		for (int i = 1; i <= numberNodes; i++) {
			meshFile >> nodePoints[i].x >> nodePoints[i].y >> tmp >> tmp >> nodePoints[i].z;
		}
		for (int j = 1; j <= numberElements; j++) {
			meshFile >> neighbourhood[j].x >> neighbourhood[j].y >> neighbourhood[j].z;
			elementArea[j] = areaTriangle(nodePoints[neighbourhood[j].x], nodePoints[neighbourhood[j].y], nodePoints[neighbourhood[j].z]);
			elementHeight[j] = meanHeight(nodePoints[neighbourhood[j].x], nodePoints[neighbourhood[j].y], nodePoints[neighbourhood[j].z]);
		}
		meshFile.close();
	}
	else {
		cout << "Mesh file [" << fileName << "] could not be opened" << endl;
		t3sMesh();
	}
}
t3sMesh::t3sMesh() {
	title = "ERROR";
	numberNodes = numberElements = 0;
	nodePoints[0].x = nodePoints[0].y = nodePoints[0].z = 0;
	neighbourhood[0].x = neighbourhood[0].y = neighbourhood[0].z = 0;
}

externalWaterLevel::externalWaterLevel(const string& fileName) {
	ifstream wlFile;
	wlFile.open(fileName);
	if (wlFile.is_open()) {
		title = fileName;
		string tmp = fileName;
		while (tmp != ":EndHeader") {
			wlFile >> tmp;
			if (tmp == ":SourceFile") {
				wlFile >> sourceFile;
			}
			else if (tmp == ":LocationX") {
				wlFile >> xUTM;
			}
			else if (tmp == ":LocationY") {
				wlFile >> yUTM;
			}
			else if (tmp == ":DeltaT") {
				// need some temp values for converting time format to dt in hours
				char tempChar;
				int dtHour, dtMinute;
				float dtSeconds;

				wlFile >> dtHour >> tempChar >> dtMinute >> dtSeconds;
				dt = (double)dtHour + double(dtMinute) / 60.0 + double(dtSeconds) / 3600.0;
			}
		}
		int i = 0;
		double tmpD;
		while (wlFile >> tmpD) {
			level[i] = tmpD;
			time[i] = i * dt;
		}
		numberReadings = level.size();

		wlFile.close();
	}
	else {
		cout << "Water level file [" << fileName << "] could not be opened" << endl;
		externalWaterLevel();
	}
}
externalWaterLevel::externalWaterLevel() {
	title = sourceFile = "ERROR";
	numberReadings = 0;
	xUTM = yUTM = dt = time[0] = level[0] = 0.0;
}
double externalWaterLevel::getExternalWL(const double& timeNow) {

}

turbines::turbines(const tidalRangeScheme& trs, const string& fileName) {
	ifstream HQPTurbines;
	title = fileName;
	HQPTurbines.open(fileName);
	if (HQPTurbines.is_open()) {
		string tmp;
		HQPTurbines >> tmp;
		if (tmp == "OriginalDiameter(m)") {
			HQPTurbines >> originalDiameter;
			HQPTurbines >> tmp;
			if (tmp == "KHP") {
				//Need to apply scaling here VVV
				HQPTurbines >> numHPPoints;
				getline(HQPTurbines, tmp);
				for (int i = 1; i <= numHPPoints; i++) {
					HQPTurbines >> tmp >> powerHeadDifference[i] >> powerOutput[i];
				}
			}
			else if (tmp == "KHQ") {
				HQPTurbines >> numHQPoints;
				getline(HQPTurbines, tmp);
				for (int j = 1; j <= numHQPoints; j++) {
					HQPTurbines >> tmp >> powerHeadDifference[j] >> powerOutput[j];
				}
			}
			else {
				cout << "Invalid turbine curve parameter (not KHP or KHQ)" << endl;
			}
			getline(HQPTurbines, tmp);
			if (tmp == "KHP") {
				HQPTurbines >> numHPPoints;
				getline(HQPTurbines, tmp);
				for (int i = 1; i <= numHPPoints; i++) {
					HQPTurbines >> tmp >> powerHeadDifference[i] >> powerOutput[i];
				}
			}
			else if (tmp == "KHQ") {
				HQPTurbines >> numHQPoints;
				getline(HQPTurbines, tmp);
				for (int j = 1; j <= numHQPoints; j++) {
					HQPTurbines >> tmp >> powerHeadDifference[j] >> powerOutput[j];
				}
			}
			else {
				cout << "Invalid turbine curve parameter (not KHP or KHQ)" << endl;
			}
		}
		else {
			cout << "First line of [" << fileName << "] = '" << tmp << "', this is probably an old file." << endl;
			cout << "Aborting model, please use format similar to:" << endl
				<< "OriginalDiameter(m) 9.0" << endl
				<< "KHP 104" << endl << "No	Head(m)	Power(MW)" << endl
				<< "Data......." << endl;
		}
	}
	else {
		cout << "Turbine H-Q-P file [" << fileName << "] could not be opened." << endl;
		turbines();
	}
}
turbines::turbines() {
	title = "ERROR";
	originalDiameter = turbineDiameter = totalArea = coeffDischarge = 0;
	powerHeadDifference[0] = powerOutput[0] = flowHeadDifference[0] = flowRate[0] = 0;
	numHPPoints = numHQPoints = 0;
}
double turbines::getFlowRate(const double& headDifference, const tidalRangeScheme& trs) {

}
double turbines::getPowerOutput(const double& headDifference, const tidalRangeScheme& trs) {

}
double turbines::getFillingFlow(const double& headDifference) {

}
//Everything below here is kept for reference only
/*
//Printers to console and to string
void variable::printVariable() {
	cout << "<" << name << "> =" << value << units << " MODE <"<<mode<<">"<< endl;
};
string variable::strVariable() {
	return "<" + name + "> =" + value + units + " MODE <" + mode + ">";
}
void variable::setValue(double* newValue) {
	if (mode == 0) {
		cout << "ATTEMPTING TO EDIT FIXED VARIABLE <" << name << ">" << endl;
	}
	value = *newValue;
}
string variable::strMode() {
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
	return  "<" + name + "> variable mode is " + mstring;
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
*/

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
