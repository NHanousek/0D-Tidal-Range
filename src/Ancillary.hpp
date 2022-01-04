#ifndef ANCILLARY
#define ANCILLARY
#include <iostream>
#include <fstream>
#include <vector>
#include "timeSeries.hpp"
/*
How does a TRS operating in ancillary services work?
So it depends on the AC type, because they all work differently...
Lets assume that for a gicen type, you need to be available to deliver P power
in M minutes for a duration of H hours, in at least W hours.

If you are required to generate, you will be given R hours to recover.

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
  ifStream acFile;
  acFile.open(fileName);
  if (acFile.is_open()) {
    string tmp = "NaN";
    while (tmp != "End") {
      inFile >> tmp; // header line
      if (tmp == "End" || tmp == "end" || tmp == "END") {
				break;
			} else if (tmp == "tenderDuration(hrs):") {
				inFile >> tenderDuration;
			} else if (tmp == "deliveryWithin(hrs):") {
				inFile >> deliveryWithin;
			} else if (tmp == "minCapacity(MW):") {
				inFile >> minCapacity;
			} else if (tmp == "availabePrice(£/MWh):") {
				inFile >> availPrice;
			} else if (tmp == "generationPrice(£/MWh):") {
				inFile >> genPrice;
			} else if (tmp == "ancillaryDemandFile:") {
				inFile >> tmp;
				ancDemand = timeSeries(tmp);
    	} else {
				string tmp2;
				inFile >> tmp2;
				cout <<"Invalid parameter [" << tmp << "] = [" << tmp2 << "] in ancilliary file [" << fileName << "] could not be loaded." << endl;
			}
  }
  else{
    cout << "Ancillary services file [" << fileName << "] could not be opened" << endl;
  }


}


#endif
