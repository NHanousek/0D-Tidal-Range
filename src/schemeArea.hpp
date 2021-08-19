// schemeArea.h

#ifndef SCHEME_AREA
#define SCHEME_AREA

// Standard libraries
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Model specific libraries
#include "TypesDef.h"
#include "Generals.hpp"
using namespace std;

class schemeArea {
private:
  int numPoints = 100; // number of points in series. defaults to 100
  vector<double> level; // vector of levels
  vector<double> area; // vector of areas

public:
  string title = "NaN"; // file name of input
  double minInternalLevel = 0.0; // point with area = 0.
  schemeArea() = default;
  schemeArea(const string& fileName, const double& minLevel, const double& maxLevel); // constructor, works differently depending on filetype
  double getWettedArea(const double& level); // return area at a given level
  double getWaterLevel(const double& area); // return level for a given area
  double newWaterLevel(const double& initialWaterLevel, const double& volumeChange); // volume change, via trapezium rule
  void writeScheme(const string& fileName); // should write a scheme that the constructor can read
  inline bool dry(const double& checkLevel) { return (checkLevel <= minInternalLevel); }; // returns true if dry, false if wet.
  ~schemeArea() {}; // destructor
};
schemeArea::schemeArea( const string& fileName, const double& minLevel, const double& maxLevel) {
	string fileExt = ".";
  if (fileName[fileName.size()-4] == '.') {
    for (int i = 3; i > 0; i--) {
      fileExt += fileName[fileName.size()-i];
    }
  }
	ifstream schm;
	schm.open(fileName);
	string tmp = "NaN";
	// is it a blue kenue mesh file?
  if (fileExt == ".t3s") {
		// vectors to store the mesh data
		vector<tint3> neighbourhood;
		vector<tdouble3> nodePoints;
		vector<double> elementArea, elementHeight;

		// mesh parameters
		int numberNodes = 0;
		int numberElements = 0;

		// temmp variables for data loading
		tint3 tmpInt3;
		tdouble3 tmpDouble3;


		cout << "Reading Mesh file [" << fileName << "]" << endl;
		while (tmp != ":EndHeader") {
			schm >> tmp;
			if (tmp == ":NodeCount") {
				schm >> numberNodes;
			}
			else if (tmp == ":ElementCount") {
				schm >> numberElements;
			}
		}

		for (int i = 0; i < numberNodes; i++) {
			schm >> tmpDouble3.x >> tmpDouble3.y >> tmp >> tmp >> tmpDouble3.z;
			nodePoints.push_back(tmpDouble3);
		}
		for (int j = 0; j <= numberElements; j++) {
			schm >> tmpInt3.x >> tmpInt3.y >> tmpInt3.z;
			neighbourhood.push_back(tmpInt3);
			elementArea.push_back(areaTriangle(nodePoints[neighbourhood[j].x - 1], nodePoints[neighbourhood[j].y - 1], nodePoints[neighbourhood[j].z - 1]));
			elementHeight.push_back(meanHeight(nodePoints[neighbourhood[j].x - 1], nodePoints[neighbourhood[j].y - 1], nodePoints[neighbourhood[j].x - 1]));
		}
		schm.close();
		double base = 0;
		double diffLevel = (maxLevel - minLevel)/numPoints;

		// establish the base area
		for (int i = 0; i < numberElements; i++){
			if (elementHeight[i] < minLevel){
				base += elementArea[i];
			}
		}
		area.push_back(base);
		level.push_back(minLevel);
		for (int i = 1; i <= numPoints; i++) {
			area.push_back(area[i-1]);
			level.push_back(level[i-1] + diffLevel);
			//loop through the elements
			for (int j = 0; j < numberElements ; j++) {
				if (level[i-1] < elementHeight[j] && elementHeight[j] <= level[i]) {
					area[i] += elementArea[j];
				}
			}
		}
		//minInternalLevel = minVal(elementHeight);
		writeScheme("Scheme_Area.out");
  } else {
		numPoints = 0;
		for (int i = 1; i <=3; i++){
			schm >> tmp;
			if (tmp == "Scheme:"){
				getline(schm, title);
			} else if (tmp == "Mesh:") {
				getline(schm, tmp);
			} else if (tmp == "Lagoon:") {
				getline(schm, tmp);
			} else {
				cout << "Invalid parameter[" << tmp << "] in schemeArea file.." << endl;
			}
		}
		getline(schm,tmp);
			// Not sure if this will work...
		while(!schm.eof()) {
	    double a, b;
	    schm >> a >> b; // extracts 2 floating point values seperated by whitespace
			level.push_back(a); area.push_back(b);
			numPoints++;
		}
	}
	minInternalLevel = level[0];
	for (int i = 0; i < numPoints - 1; i++){
		if (area[i] <= 0 && area[i+1] > 0){
			minInternalLevel = level[i];
		}
	}
	cout << "Scheme min internal level: " << minInternalLevel << endl;
};
void schemeArea::writeScheme(const string& fileName) {
	ofstream outFile;
	outFile.open(fileName);
	if (outFile.is_open()) {
		outFile << "Scheme: " << title << endl
			<< "Level(m)\tArea(m2)" << endl;
		for (int i = 0; i < level.size(); i++) {
			outFile << level[i] << "\t" << (double)area[i] << endl;
		}
		outFile.close();
	}
	else {
		cout << "Output file [" << fileName << "] could not be opened..." << endl;
	}
};

double schemeArea::getWettedArea(const double& internalWaterLevel) {
	if (internalWaterLevel <= level.front()) {
		cout << "Warning: Possible dry scheme" << endl;
		return 0.0; // if the desired level is below the bed level of the tidal range scheme
	}
	else if (internalWaterLevel >= level.back()) {
		return area.back(); //
	}
	else {
		for (int i = 0; i < numPoints; i++) {
			if (level[i] < internalWaterLevel && internalWaterLevel <= level[i + 1]) {
				return interpolate(level[i], internalWaterLevel, level[i + 1], area[i], area[i + 1]);
			}
		}
	}
	return level.back();
};

double schemeArea::getWaterLevel(const double& wettedArea) {
	if (wettedArea == 0) {
		if (area.front() > 0) {
			return level.front();
		}
		for (int i = 0; i < numPoints; i++) {
			if (area[i] <= 0 && area[i + 1] > 0) {
				return level[i];
			}
		}
	}
	if (wettedArea >= area.back()) {
		cout << "Area too large of " << wettedArea << " used" << endl;
		return level.back();
	}
	for (int i = 0; i < numPoints; i++) {
		if (area[i] < wettedArea && wettedArea <= area[i + 1]) {
			return interpolate(area[i], wettedArea, area[i + 1], level[i], level[i + 1]);
		}
	}
	return area.back();
};

double schemeArea::newWaterLevel(const double& initialWaterLevel, const double& volumeChange){ // volume change, via trapezium rule
	if (volumeChange == 0) {
		return initialWaterLevel;
	}
	else if (volumeChange < 0 && absolute(volumeChange) > 0.5 * getWettedArea(initialWaterLevel) * (initialWaterLevel - getWaterLevel(0))){
	//else if (volumeChange < (0.5 * (getWettedArea(initialLevel) + 0) * (getWaterLevel(0) - initialLevel))) {
		// if the lagoon is draining and the volume change is more than the remaining lagoon volume
		return getWaterLevel(0); // the lagoon will rest at empty
	}
	else if (volumeChange >= 0.5*(getWettedArea(initialWaterLevel) + getWettedArea(level.back()))*(level.back() - initialWaterLevel)) {
		cout << "Warning: Scheme possibly overfilled" << endl;
		double newVolumeChange = volumeChange - 0.5 * (getWettedArea(initialWaterLevel) + getWettedArea(level.back())) * (level.back() - initialWaterLevel);
		return level.back() + newVolumeChange / getWettedArea(level.back());
	} else {
		//Trapezium assumption:
		/*	volumeChange = (area1 + area2) * (level2 - level1) / 2
			but, we don't know level or area 2.
			All we know is that for one, you can find the other.
			(because there are area-level functions for that)
			So we must rearrange the equation, and iterate to find the solution...
			this may make the model quite slow, but I think it's more accurate
			and it allows for wetting and drying.		N. Hanousek */
		double newVolumeChange(0), newLevel(initialWaterLevel), accuracy(0.001);

		// initialise upper and lower bound points for the search.
		double upperLevel(level.back()), lowerLevel(getWaterLevel(0));
		// search...
		// If the volume change makes the resulting level bigger than the given maximum this can hang forever,
		// so...
		while (newVolumeChange < volumeChange - accuracy || volumeChange + accuracy < newVolumeChange ) { // volume change suggested is not volume change desired
			if (newVolumeChange < volumeChange) {// too low
				lowerLevel = newLevel; // bring lower bound up
			}
			else /*if (newVolumeChange > volumeChange)*/ {// too high
				upperLevel = newLevel; // bring upper bound down
			}
			newLevel = (lowerLevel + upperLevel) / 2;
			newVolumeChange = 0.5 * (getWettedArea(initialWaterLevel) + getWettedArea(newLevel)) * (newLevel - initialWaterLevel);
		}
		return newLevel;
	}
}
#endif
