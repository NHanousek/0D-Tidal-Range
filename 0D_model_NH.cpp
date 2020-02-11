
// Implements functions of the 0D Model

#include "0D_model_NH.h"
#include "General.h"
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
		string tmp = "Start";
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
		cout << "Reading Lagoon file [" << fileName << "]" << endl;
		while (tmp != "End") {
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
				turbineArea = 0.25 * Pi * turbineDiameter * turbineDiameter;
			}
			else if (tmp == "turbineArea(m2)=") {
				lagoonFile >> turbineArea;
			}
			else if (tmp == "headDiffStart(m)=") {
				lagoonFile >> headDiffStart;
			}
			else if (tmp == "headDiffEnd(m)=") {
				lagoonFile >> headDiffEnd;
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
			else if (tmp == "End") {
				cout << "End of Lagoon File." << endl;
			}
			else {

				cout << "Unrecognised Lagoon Parameter [" << tmp << "] read." << endl;
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
	cout << endl << "BLANK TIDAL RANGE SCHEME CLASS CREATED" << endl;
	source = title = "ERROR";
	simTime = 0.0;
	timeStep.x = timeStep.y = timeStep.z = 0.0;
	timeAdjust.x = timeAdjust.y = timeAdjust.z = 0.0;
	schemeType = turbineType = numberTurbines = numWaterLevelPoints= 0;
	areaSluices = turbineDiameter = turbineArea = headDiffStart = 0;
	headDiffEnd = waterLevelMax = waterLevelMin = generatorEfficiency = 0;
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
		cout << "Reading Mesh file [" << fileName << "]" << endl;
		while (tmp != ":EndHeader") {
			meshFile >> tmp;
			if (tmp == ":NodeCount") {
				meshFile >> numberNodes;
			}
			else if (tmp == ":ElementCount") {
				meshFile >> numberElements;
			}
		}
		tdouble3 tmptd3; //temp triple double for loading in coordinates
		for (int i = 0; i < numberNodes; i++) {
			meshFile >> tmptd3.x >> tmptd3.y >> tmp >> tmp >> tmptd3.z;
			nodePoints.push_back(tmptd3);
		}
		tmptd3.~tdouble3();
		tint3 tmpti3; //temp triple int for loading neighbour list
		for (int j = 0; j <= numberElements; j++) {
			meshFile >> tmpti3.x >> tmpti3.y >> tmpti3.z;
			neighbourhood.push_back(tmpti3);
			elementArea.push_back(areaTriangle(nodePoints[neighbourhood[j].x - 1], nodePoints[neighbourhood[j].y - 1], nodePoints[neighbourhood[j].z - 1]));
			elementHeight.push_back(meanHeight(nodePoints[neighbourhood[j].x - 1], nodePoints[neighbourhood[j].y - 1], nodePoints[neighbourhood[j].x - 1]));
		}
		tmpti3.~tint3();
		meshFile.close();
	}
	else {
		cout << "Mesh file [" << fileName << "] could not be opened" << endl;
		t3sMesh();
	}
}
t3sMesh::t3sMesh() {
	cout << endl << "BLANK MESH CLASS CREATED" << endl;
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
		cout << "Reading Water Level file [" << fileName << "]" << endl;
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
				double dtSeconds;

				wlFile >> dtHour >> tempChar >> dtMinute >> tempChar >> dtSeconds;
				dt = (double)dtHour + (double)dtMinute / 60.0 + dtSeconds / 3600.0;
			}
		}
		int i = 0;
		double tmpD;
		while (wlFile >> tmpD) {
			level.push_back(tmpD);
			time.push_back(i * dt);
			i++;
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
	cout << endl << "BLANK EXTERNAL WATER LEVEL CLASS CREATED" << endl;
	title = sourceFile = "ERROR";
	numberReadings = 0;
	xUTM = yUTM = dt = time[0] = level[0] = 0.0;
}
double externalWaterLevel::getExternalWL(const double& timeNow) {
	for (int i = 0; i <= (int)time.size()-1; i++) {
		if (time[i] <= timeNow && timeNow < time[i + 1]) {
			return interpolate(time[i], timeNow, time[i + 1], level[i], level[i + 1]);
		}
	}
	return 999;
}

turbines::turbines(const tidalRangeScheme& trs, const string& fileName) {
	ifstream HQPTurbines;
	title = fileName;
	turbineDiameter = trs.turbineDiameter;
	totalArea = Pi * turbineDiameter * turbineDiameter * 0.25 * trs.numberTurbines;
	coeffDischarge = trs.turbineCoefQ;
	HQPTurbines.open(fileName);
	if (HQPTurbines.is_open()) {
		string tmp;
		cout << "Reading Turbine file [" << fileName << "]" << endl;
		HQPTurbines >> tmp;
		if (tmp == "OriginalDiameter(m)") {
			HQPTurbines >> originalDiameter;
			// scale factor for original turbines to model turbines
			double scaleFactor = (trs.turbineDiameter / originalDiameter) * (trs.turbineDiameter / originalDiameter);
			double tmpD1,tmpD2; // temporary variable to use when scaling
			// this reader allows you to give the power and flow in either order
			HQPTurbines >> tmp;
			if (tmp == "KHP") {
				HQPTurbines >> numHPPoints;
				HQPTurbines >> tmp >> tmp >> tmp;
				for (int i = 1; i <= numHPPoints; i++) {
					HQPTurbines >> tmp >> tmpD1 >>tmpD2;
					powerHeadDifference.push_back(tmpD1);
					powerOutput.push_back(tmpD2 * scaleFactor);
				}
			}
			else if (tmp == "KHQ") {
				HQPTurbines >> numHQPoints;
				HQPTurbines >> tmp >> tmp >> tmp;
				for (int j = 1; j <= numHQPoints; j++) {
					HQPTurbines >> tmp >> tmpD1 >> tmpD2;
					flowHeadDifference.push_back(tmpD1);
					flowRate.push_back(tmpD2 * scaleFactor);
				}
			}
			else {
				cout << "Invalid turbine curve parameter (not KHP or KHQ)" << endl;
			}
			HQPTurbines >> tmp; // reads the row of asterisks
			HQPTurbines >> tmp; // reads the other parameter name
			if (tmp == "KHP") {
				HQPTurbines >> numHPPoints;
				HQPTurbines >> tmp >> tmp >> tmp;
				for (int i = 1; i <= numHPPoints; i++) {
					HQPTurbines >> tmp >> tmpD1 >> tmpD2;
					powerHeadDifference.push_back(tmpD1);
					powerOutput.push_back(tmpD2 * scaleFactor);
				}
			}
			else if (tmp == "KHQ") {
				HQPTurbines >> numHQPoints;
				HQPTurbines >> tmp >> tmp >> tmp;
				for (int j = 1; j <= numHQPoints; j++) {
					HQPTurbines >> tmp >> tmpD1 >> tmpD2;
					flowHeadDifference.push_back(tmpD1);
					flowRate.push_back(tmpD2 * scaleFactor);
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
				<< "Data......." << endl
				<< "******************" << endl
				<< "KHQ 104" << endl << "No	Head(m)	Q (m3/s)" << endl
				<< "Data......." << endl;
		}
	}
	else {
		cout << "Turbine H-Q-P file [" << fileName << "] could not be opened." << endl;
		turbines();
	}
}
turbines::turbines() {
	cout << endl << "BLANK TURBINE CLASS CREATED" << endl;
	title = "ERROR";
	originalDiameter = turbineDiameter = totalArea = coeffDischarge = 0;
	powerHeadDifference[0] = powerOutput[0] = flowHeadDifference[0] = flowRate[0] = 0;
	numHPPoints = numHQPoints = 0;
}
double turbines::getFlowRate(const double& headDifference, const tidalRangeScheme& trs) {
	for (int i = 0; i <= numHQPoints; i++) {
		if (flowHeadDifference[i] <= headDifference && headDifference <= flowHeadDifference[i + 1]) {
			return interpolate(flowHeadDifference[i], headDifference, flowHeadDifference[i + 1], flowRate[i], flowRate[i + 1]);
		}
	}
	return 999;
}
double turbines::getPowerOutput(const double& headDifference, const tidalRangeScheme& trs) {
	for (int i = 0; i <= numHPPoints; i++) {
		if (powerHeadDifference[i] <= headDifference && headDifference <= powerHeadDifference[i + 1]) {
			return interpolate(powerHeadDifference[i], headDifference, powerHeadDifference[i + 1], powerOutput[i], powerOutput[i + 1]);
		}
	}
	return 999;
}
double turbines::getFillingFlow(const double& headDifference) {
	return orificeFlow(coeffDischarge, headDifference, totalArea);
}

schemeArea::schemeArea(const tidalRangeScheme& trs, const t3sMesh& mesh) {
	title = trs.source;
	lagoonTitle = trs.title;
	meshTitle = mesh.title;
	numPoints = trs.numWaterLevelPoints;
	cout << "Composing scheme area..." << endl;
	//initialise the arrays
	//assumes that the waterLevelMin is below the lowest point of bathymetry inside the trs
	area.push_back(0.0);
	level.push_back(trs.waterLevelMin);
	if (title == "ERROR" || lagoonTitle == "ERROR" || meshTitle == "ERROR") {
		cout << "Scheme Area Class created with errors" << endl;
	}
	double diffLevel = (trs.waterLevelMax - trs.waterLevelMin) / numPoints;
	//loop through the height points
	for (int i = 1; i <= numPoints; i++) {
		area.push_back(area[i-1]);
		level.push_back(level[i-1] + diffLevel);
		//loop through the elements
		for (int j = 0; j < mesh.numberElements ; j++) {
			if (level[i-1] < mesh.elementHeight[j] && mesh.elementHeight[j] <= level[i]) {
				area[i] += mesh.elementArea[j];
			}
		}
	}
}

double schemeArea::getWettedArea(const double& internalWaterLevel) {

	if (internalWaterLevel < level.front()) {
		return 0; // if the desired level is below the bed level of the tidal range scheme
	}
	else if (internalWaterLevel > level.back()) {
		return area.back(); // 
	}
	else {
		for (int i = 0; i < numPoints; i++) {
			if (level[i] < internalWaterLevel && internalWaterLevel <= level[i + 1]) {
				return interpolate(level[i], internalWaterLevel, level[i + 1], area[i], area[i + 1]);
			}
		}
	}	
}
double schemeArea::getWaterLevel(const double& wettedArea) {
	if (wettedArea == 0) {
		for (int i = 0; i < numPoints; i++) {
			if (area[i] <= 0 && area[i + 1] > 0) {
				return level[i];
			}
		}
	}
	for (int i = 0; i < numPoints; i++) {
		if (area[i] < wettedArea && wettedArea <= area[i + 1]) {
			return interpolate(area[i], wettedArea, area[i + 1], level[i], level[i + 1]);
		}
	}
}

results::results(const tidalRangeScheme& trs) {
	cout << "Initialising results." << endl;
	title = "REsults from Tidal Range Scheme " + trs.title;
	info = "Time(Hr) WLup(m) WLdown(m) HeadDiff(m) TRSarea(m2) Power(MW) Energy(MWH) QTurb(m3/s) QSluice(m3/s) Mode(-)";
	modelTimeHr.push_back(0);
	upstreamWaterLevel.push_back(0);
	downstreamWaterLevel.push_back(0);
	headDifference.push_back(0);
	wettedArea.push_back(0);
	powerOutput.push_back(0);
	powerGenerated.push_back(0);
	turbineFlow.push_back(0);
	sluiceFlow.push_back(0);
	lagoonMode.push_back(0);
}
results::results(const tidalRangeScheme& trs, const double& time, const double& upstreamWL, const double& downstreamWL, const double& headDiff, const double& wetArea, const double& powerOut, const double& turbineQ, const double& sluiceQ, const int& trsMode) {
	title = "REsults from Tidal Range Scheme " + trs.title;
	info = "Time(Hr) WLup(m) WLdown(m) HeadDiff(m) TRSarea(m2) Power(MW) Energy(MWH) QTurb(m3/s) QSluice(m3/s) Mode(-)";
	modelTimeHr.push_back(time);
	upstreamWaterLevel.push_back(upstreamWL);
	downstreamWaterLevel.push_back(downstreamWL);
	headDifference.push_back(headDiff);
	wettedArea.push_back(wetArea);
	powerOutput.push_back(powerOut);
	powerGenerated.push_back(powerOut);
	turbineFlow.push_back(turbineQ);
	sluiceFlow.push_back(sluiceQ);
	lagoonMode.push_back(trsMode);
}

void results::addResults(const double& time, const double& upstreamWL, const double& downstreamWL, const double& headDiff, const double& wetArea, const double& powerOut, const double& turbineQ, const double& sluiceQ, const int& trsMode) {
	modelTimeHr.push_back(time);
	upstreamWaterLevel.push_back(upstreamWL);
	downstreamWaterLevel.push_back(downstreamWL);
	headDifference.push_back(headDiff);
	wettedArea.push_back(wetArea);
	powerOutput.push_back(powerOut);
	powerGenerated.push_back(powerGenerated.back() + powerOut);
	turbineFlow.push_back(turbineQ);
	sluiceFlow.push_back(sluiceQ);
	lagoonMode.push_back(trsMode);
}
string results::header() {
	return title + "\n" + info;
}
void results::header(const string& fileName) {
	ofstream outFile;
	outFile.open(fileName);
	if (outFile.is_open()) {
		outFile << title << "\n" << info;
	}
	else {
		cout << "Output file [" << fileName << "] could not be opened..." << endl;
	}
}

string results::line(const int& i) {
	string output;
	output += to_string(modelTimeHr[i]) +", ";
	output += to_string(upstreamWaterLevel[i]) + ", ";
	output += to_string(downstreamWaterLevel[i]) + ", ";
	output += to_string(headDifference[i]) + ", ";
	output += to_string(wettedArea[i]) + ", ";
	output += to_string(powerOutput[i]) + ", ";
	output += to_string(powerGenerated[i]) + ", ";
	output += to_string(turbineFlow[i]) + ", ";
	output += to_string(sluiceFlow[i]) + ", ";
	output += to_string(lagoonMode[i]);
	return output;
}
void results::line(const int& i, const string& fileName) {
	ofstream outFile;
	outFile.open(fileName);
	if (outFile.is_open()) {
		outFile << line(i);
	}
	else {
		cout << "Output file [" << fileName << "] could not be opened..." << endl;
	}
}
void results::printFull() {
	cout << header();
	for (int i = 0; i <= modelTimeHr.size(); i++) {
		cout << line(i);
	}
}
void results::printFull(const string& fileName) {
	ofstream outFile;
	outFile.open(fileName);
	if (outFile.is_open()) {
		outFile << header() << endl;
		for (int i = 0; i < modelTimeHr.size(); i++) {
			outFile << line(i) << endl;
		}
	}
	else {
		cout << "Output file [" << fileName << "] could not be opened..." << endl;
	}
}

modelConfig::modelConfig(const string& configFileName) {
	string tmp;
	ifstream cfgFile;
	cfgFile.open(configFileName);
	if (cfgFile.is_open()) {
		getline(cfgFile, tmp);
		cout << "Loading " << tmp << endl;
		cfgFile >> tmp;
		while (tmp != "End") {
			if (tmp == "Lagoon:") {
				cfgFile >> LagoonFileName;
			}
			else if (tmp == "Mesh:") {
				cfgFile >> meshFileName;
			}
			else if (tmp == "ExternalWaterLevel:") {
				cfgFile >> externalWaterLevelFileName;
			}
			else if (tmp == "Turbines:") {
				cfgFile >> turbinesFileName;
			}
			else if (tmp == "Results:") {
				cfgFile >> resultsFileName;
			}
			else {
				cout << "Unexpected file name read from config file..." << endl;
			}
			cfgFile >> tmp;
		}
		cfgFile.close();
	}
	else {
		cout << "Unable to open config file [" << configFileName << "]." << endl;
	}
}

int nextMode(const tidalRangeScheme& trs, const results& previous) {
	if (trs.schemeType == 1) {
		// Ebb only - No Pumping
		if (previous.wettedArea.back() <= 0) {
			return 1;
		}
		switch (previous.lagoonMode.back()) {
			case 1: //Filling/sluicing
				if (previous.headDifference.back() > 0) {
					return 2;
				}
				else {
					return 1;
				}
				break;
			case 2: //Holding
				if (previous.headDifference.back() >= trs.headDiffStart) {	
					return 3;	
				}
				else if (previous.headDifference.back() < 0) { 
					return 1;	
				}
				else {
					return 2;	
				}
				break;
			case 3: //Generating
				if (previous.headDifference.back() <= trs.headDiffEnd) {
					return 2;
				}
				else {
					return 3;
				}
				break;
			default: //Error
				cout << "Error with Tidal Range Scheme Mode..." << endl;
				return 0;
				break;
		}
	}
	else if (trs.schemeType == 2) {
		// Two-way generation - No Pumping
		if (previous.wettedArea.back() <= 0) {
			return 1;
		}
		switch (previous.lagoonMode.back()) {
		case 1: //Filling/sluicing
			if (-0.01 <= previous.headDifference.back() <= 0.01) {
				return 2;
			}
			else {
				return 1;
			}
			break;
		case 2: //Holding
			if (absolute(previous.headDifference.back()) >= trs.headDiffStart) {
				return 3; 
			}
			else {
				return 2;
			}
			break;
		case 3: //Generating
			if (absolute(previous.headDifference.back() <= trs.headDiffEnd)) {
				return 1;
			}
			else {
				return 3;
			}
			break;
		default: //Error
			cout << "Error with Tidal Range Scheme Mode..." << endl;
			return 0;
			break;
		}
	}
	else {
		cout << "Error with Tidal Range Scheme Type..." << endl;
		return 0;
	}
}
int nextMode(const tidalRangeScheme& trs, const results& previous, const double& headDiff) {
	if (trs.schemeType == 1) {
		// Ebb only - No Pumping
		//if (previous.wettedArea.back() <= 0) {
		//	return 1;
		//}
		if (previous.wettedArea.back() <= 0.0) {
			if (headDiff > 0) {
				return 2;
			}
			else {
				return 1;
			}
		}
		switch (previous.lagoonMode.back()) {
		case 1: //Filling/sluicing
			if (previous.wettedArea.back() <= 0.1) {
				if (headDiff > 0) {
					return 2;
				}
				else {
					return 1;
				}
			}
			if (headDiff > 0) {
				return 2;
			}
			else {
				return 1;
			}
			break;
		case 2: //Holding
			if (previous.wettedArea.back() <= 0.1) {
				if (headDiff <= 0) {
					return 2;
				}
				else {
					return 1;
				}
			}
			if (headDiff >= trs.headDiffStart) {
				return 3;
			}
			else if (headDiff < 0) {
				return 1;
			}
			else {
				return 2;
			}
			break;
		case 3: //Generating
			if (previous.wettedArea.back() <= 0.1) {
				if (headDiff <= 0) {
					return 2;
				}
				else {
					return 1;
				}
			}
			if (headDiff <= trs.headDiffEnd) {
				return 2;
			}
			else {
				return 3;
			}
			break;
		default: //Error
			cout << "Error with Tidal Range Scheme Mode..." << endl;
			return 0;
			break;
		}
	}
	else if (trs.schemeType == 2) {

		// Two-way generation - No Pumping
		if (previous.wettedArea.back() <= 0.0) {
			if (headDiff > 0) {
				return 2;
			}
			else {
				return 1;
			}
		}
		if (previous.wettedArea.back() <= 0) {
			return 1;
		}
		switch (previous.lagoonMode.back()) {
		case 1: //Filling/sluicing
			if (-0.01 <= headDiff <= 0.01) {
				return 2;
			}
			else {
				return 1;
			}
			break;
		case 2: //Holding
			if (absolute(headDiff) >= trs.headDiffStart) {
				return 3;
			}
			else {
				return 2;
			}
			break;
		case 3: //Generating
			if (absolute(headDiff) <= trs.headDiffEnd) {
				return 1;
			}
			else {
				return 3;
			}
			break;
		default: //Error
			cout << "Error with Tidal Range Scheme Mode..." << endl;
			return 0;
			break;
		}
	}
	else {
		cout << "Error with Tidal Range Scheme Type..." << endl;
		return 0;
	}
}
double newUpstreamLevel(const double& oldUpstreamLevel, const double& flowTurbines, const double& flowSluices, const double& inFlow, schemeArea& area, const tidalRangeScheme& trs) {
	if (area.getWettedArea(oldUpstreamLevel) <= 0) {
		return oldUpstreamLevel;
	}
	return (oldUpstreamLevel + trs.timeStep.z * (inFlow - flowTurbines - flowSluices) / (area.getWettedArea(oldUpstreamLevel)));
	//tdouble3 dt = trs.timeStep;
	//double oldArea = area.getWettedArea(oldUpstreamLevel);
	//double newArea;
}

//Everything below here is kept for reference only

/*
int wetDry(const double& dH, const results& previous) {
	if (previous.wettedArea.back() <= 0.0) {
		if (dH > 0) {
			return 2;
		}
		else {
			return 1;
		}
	}
	else {
		return previous.lagoonMode.back();
	}
}
*/

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
