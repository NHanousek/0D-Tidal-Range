#ifndef ANCILLARY
#define ANCILLARY
#include <iostream>
#include <fstream>
#include <vector>
#include "Generals.hpp"
/*
How does a TRS operating in ancillary services work?
So it depends on the AC type, because they all work differently...
Lets assume that for a given type, you need to be available to deliver P power
in M minutes for a duration of H hours, in at least W hours.

If you are required to gene  rate, you will be given R hours to recover.

You will be paid rA for the time you are available,
and rG for all the energy you generate in this period.

You must determine if it's worth operating only based on the rA×H

TRS in ancillary potential operation will have a number of modes:
0	= Not operating for Ancillary services ouput
1 = Preparing to run in ancillary mode
2 = Available to generate
3 = Generating
4 = Post generation recovery
*/
class ancType {
public:
  string name = "NaN"; // Short name for the output

  double tenderDuration = 0;  // Hours
  double deliveryWithin = 0;  // Hours
	double recoveryPeriod = 0;	// Hours
  double minCapacity = 0;     // MW

  // bool availPayment = false; // Do you get paid for being available?
  double availPrice = 0;            // How much do you get for being available?
  // bool genPayment = false;     // Do you get extra for actiually generating?
  double genPrice = 0;              // How much extra do you get for generating?

	timeSeries ancDemand; // A timeseries of the energy demand of the ancilliary services side

  ancType() = default;
  ancType(const string& fileName);

};

ancType::ancType(const string& fileName) {
    ifstream inFile;
    inFile.open(fileName);
    if (inFile.is_open()) {
        string raw = "NaN";
        string tmp = "NAN";
        while (tmp != "END") {
            inFile >> raw; // header line
            tmp = toUpperCase(raw);
            if (tmp == "END") {
                break;
            }
            else if (tmp == "TENDERDURATION(HRS):") {
                inFile >> tenderDuration;
            }
            else if (tmp == "DELIVERYWITHIN(HRS):") {
                inFile >> deliveryWithin;
            }
            else if (tmp == "MINCAPACITY(MW):") {
                inFile >> minCapacity;
            }
            else if (tmp == "AVAILABLEPRICE(£/MWh):") {
                inFile >> availPrice;
            }
            else if (tmp == "GENERATIONPRICE(£/MWh):") {
                inFile >> genPrice;
            }
            else if (tmp == "ANCILLARYDEMANDFILE:") {
                inFile >> tmp;
                ancDemand = timeSeries(tmp);
            }
            else {
                string tmp2;
                inFile >> tmp2;
                cout << "Invalid parameter [" << raw << "] = [" << tmp2 << "] in ancilliary file [" << fileName << "] could not be loaded." << endl;
            }
        }
    }
    else {
        cout << "Ancillary services file [" << fileName << "] could not be opened" << endl;
    }


};


#endif
