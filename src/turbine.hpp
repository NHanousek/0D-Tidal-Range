// turbine.h
// class that holds the inputs from a HQP file, will include options
// for alternative methods of turbine operation later.
// This is for a Single Turbine!

#ifndef TURBINE
#define TURBINE

// Standard libraries
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Generals.hpp"
// Model specific libraries
// none atm...

using namespace std;
class turbineBank {
private:
  int numTurbines = 1; // how many distinct turbines are in bank
  double originalDiameter = 1.0;  // original diameter in file
  double turbineDiameters = 1.0; // what diameter is each variety
  double turbineCenters = -999.0; // mCD
  //turbine turbines; // stores the compiled turbine.
  string turbineFile = "NaN"; // for now it uses just one source file.
  double totalArea = 1.0; // total area of turbines
  double coeffDischarge = 1.0;

  int numHPPoints = 0;          // number of head-power points
  int numHQPoints = 0;          // number of head-flow points
  vector<double> powerHeadDifference; // head point values for power
  vector<double> powerAtHead;         // power point values
  vector<double> flowHeadDifference;  // head point values for flow rate
  vector<double> flowRate;						// flow rates at that head diff

public:
  string title = "NaN"; // file name of input.
  turbineBank(const string& fileName, const int& NumTurbines, const double& TurbDiam, const double& TurbCenterLevels); // constructor

  double getPower(const double& headDifference); // return power at a head difference
  double getFlow(const double& headDifference); // return a flow at a head difference
  double orifice(const double& headDifference); // flow through turbine based n orifice equation
  //double getPumpPower(const double& headDifference); // return energy cost when using as pumps
  inline double area(){return numTurbines * pi * 0.25 * turbineDiameters * turbineDiameters;} // area of turbines n*pi*D^2/4
  inline double safeLevel(const double buffer) { return turbineCenters + turbineDiameters / 2 + buffer; } // minumum safe water level
  ~turbineBank() {}; // destructor
};

//------------------IMPLEMENTATIONS------------------//
double turbineBank::orifice(const double& headDifference) {
    return numTurbines * orificeFlow(coeffDischarge, headDifference, pi * turbineDiameters * turbineDiameters * 0.25);
}
turbineBank::turbineBank(const string& fileName, const int& NumTurbines, const double& TurbDiam, const double& TurbCenterLevels) {
  numTurbines = NumTurbines; turbineDiameters = TurbDiam; turbineCenters = TurbCenterLevels;

  // open the file given
  ifstream HQPTurbines;
  HQPTurbines.open(fileName);

  turbineFile = fileName;
  // assume circular turbines...
  totalArea = Pi * turbineDiameters * turbineDiameters * 0.25 * numTurbines;

  if (HQPTurbines.is_open()) {
    string tmp;
    HQPTurbines >> tmp;
    if (tmp == "OriginalDiameter(m)") {
      HQPTurbines >> originalDiameter;
      // scale factor for original turbines to model turbines
      double scaleFactor = (turbineDiameters / originalDiameter) * (turbineDiameters / originalDiameter);
      double tmpD1,tmpD2; // temporary variable to use when scaling
      // this reader allows you to give the power and flow in either order
      HQPTurbines >> tmp;
      if (tmp == "KHP") {
        HQPTurbines >> numHPPoints;
        HQPTurbines >> tmp >> tmp >> tmp;
        for (int i = 1; i <= numHPPoints; i++) {
          HQPTurbines >> tmp >> tmpD1 >>tmpD2;
          powerHeadDifference.push_back(tmpD1);
          powerAtHead.push_back(tmpD2 * scaleFactor);
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
          powerAtHead.push_back(tmpD2 * scaleFactor);
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
  } else {
    cout << "Turbine H-Q-P file [" << fileName << "] could not be opened." << endl;
  }
	cout << "Turbines loaded from [" << fileName << "]" << endl;
};
// iterates through the turbine bank and finds the flow rate that matches the head level
double turbineBank::getFlow(const double& headDifference) {
    if (headDifference == 0) {
        return 0.0;
    }
    double hd = absolute(headDifference);
    int sgn = (0 < headDifference) - (headDifference < 0); // -1 if negative, 1 if positive

    if (hd < flowHeadDifference.front()) {
        return sgn * numTurbines * flowRate.front();
    }
    else if (hd > flowHeadDifference.back()) {
        return sgn * numTurbines * flowRate.back();
    }
    for (int i = 0; i <= flowHeadDifference.size() - 1; i++) {
        if (flowHeadDifference[i] <= hd && hd <= flowHeadDifference[i + 1]) {
            return sgn * numTurbines * interpolate(flowHeadDifference[i], hd, flowHeadDifference[i + 1], flowRate[i], flowRate[i + 1]);
        }
    }
    return 0.0;
};
// iterates through the vectors to find the match.
double turbineBank::getPower(const double& headDifference) {
    if (headDifference == 0.0) {
      	return 0.0;
    }
    double hd = absolute(headDifference);
    //int sgn = (0 < headDifference) - (headDifference < 0);

    if (hd < powerHeadDifference.front()) {
      	return numTurbines * powerAtHead.front();
    }
    else if (hd >= powerHeadDifference.back()) {
      	return numTurbines * powerAtHead.back();
    }
		for (int i = 0; i <= powerHeadDifference.size() - 1; i++) {
				if (powerHeadDifference[i] <= hd && hd <= powerHeadDifference[i + 1]) {
						return numTurbines*interpolate(powerHeadDifference[i], hd, powerHeadDifference[i + 1], powerAtHead[i], powerAtHead[i + 1]);
				}
		}
    return 0.0;
};
#endif
