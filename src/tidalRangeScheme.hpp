// tidalRangeScheme.h
//

#ifndef TIDALRANGESCHEME
#define TIDALRANGESCHEME

// Standard libraries
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Model specific libraries
#include "TypesDef.h"
#include "turbine.hpp"
#include "schemeArea.hpp"
#include "timeSeries.hpp"
#include "Generals.hpp"

using namespace std;

inline void banksWarning(const bool& banksRead, const string& parameter) {
	// this is probably where a proper error handling should be implemented.
	// something that runs through the whole trs file and finds the numBanks keyword...
	if (!banksRead) { cout << "!!!Warning!!! - <" << parameter << "> read before number of turbine banks declared." << endl; }
};

class tidalRangeScheme {
private:
	// the complex bits to set up are here. these will be built externally in GA
	// and then passed via an alternate constructor, which should save time/space
	// in comparison to generating masses of text files.
	timeSeries inFlow; // time series aka inflow hydrograph
	timeSeries energyValue; // to be used if in flexible generation
	vector<timeSeries> externalWaterLevel; // downstream tidal level trace(s)
	vector<turbineBank> turbines; //means we can use the functions innit.
	schemeArea trsArea; //  means we can use the functions innit.

	// configuration of the scheme itself
	string schemeName = "NaN"; // names from the scheme, for ID of results
	int numBanks = 1; // number of turbine banks
	int operation = 1; // 1 = Ebb only, 2 = Two way
	bool isPumping = false; // is pumping used? defaults to no.
	bool isFlexible = false; // is this a flexibly operated lagoon? defaults to no.
	bool isPumpFlexible = false; // is there flexing of the pumping start/stop
	bool isEbbFloodPumpCurves = false; // are there unique curves for ebb and flood and pumping (n==4)
	string flexVariable = "profit"; // parameter that is maximised in flex mode
	string flexType = "time"; // how do we decide when to switch
	string pumpControl = "head"; // time -> Hours of pumping, head -> target head difference

	// operational parameters
	double prevTime = 0;	// time at last step, starts at zero, local time is always
												// increased to dt for first step...
	double prevSwitchTime = 0.0; // the last time the lagoon changed operational modes
	double switchModeBuffer = 0.125; // hours between allowing the lagoon to switch mode (default = 6 mins)
	double headDiffStart = 0; // head difference for generation starting and finishing [m]
	double headDiffEnd = 0; // for fixed head.
	double headDiffStartMax = 0; // parameters for flexible operation
	double headDiffStartMin = 0; // define ranges for flexible operation.
	double headDiffEndMax = 0; // min and max for start and end of generation
	double headDiffEndMin = 0;
	double headDiffStartDelta = 0.1; // start steps to be tested in the flexible generation [m]
	double headDiffEndDelta = 0.1; // end steps to be tested in the flexible generation [m]
	double flexDt = 0.2; // timestep used in flex calculation period [hrs]
	double flexInterval = 12.4; // how often the flex check is carried out[hrs]
	double flexPeriod = 24.8; // how long the check runs for [hrs]
	double flxPrevSwitchTime = 0.0;
	// by default, every 12.4hrs it checks the next 24.8hrs

	double pumpEnd = 0;			// when to end pumping, can be head diff, level, or time based on pumpControl
	double pumpEndMin = 0;		// minimum of pumping control parameter
	double pumpEndMax = 1;		// maximum of pumping control parameter
	double pumpEndDelta = 0.5;	// step of pumping control parameter
	double pumpEff = 1.0;		// efficiency of turbine as pump
	// scheme area parameters
  double minWaterLevel = 0; // min and max levels for the schemeArea calculation [mCD]
  double maxWaterLevel = 0; // always 100 points between min and max.

	// physical parameters
	double areaSluices = 0; // in m2 just use first bank externals for now...
  double sluiceCd = 1; // discharge coefficient of sluices[0-1]
  double turbineCd = 1; // discharge coefficient of turbines [0-1]
  double generatorEfficiency = 1; // efficiency of generator [0-1]
  double safetyBuffer = 0.1; // turbine safety buffer level [m]
  double rampTime = 0.0; // ramping time [hours]

public:

	// active values for the whole scheme combined. vectors of length numBanks
	vector<double> downStream;	// downstream (external) water level (mean) [mCD]
	vector<double> headDiff;		// head difference between up and down stream (mean)[mCD]
	vector<double> powerOut;		// power output at a time Mega Watts (total)
	vector<double> turbineQ;		// flow rate through all turbines (m3/s) (total)
	vector<double> switchTimes; // times to switch
	vector<double> pumpTargets; // the water level to be pumped up to [mCD]
	vector<double> pumpTimes;   // the high and low tide times if using origlevels [hrs]
	double upStream = -999;		// upstream (internal) water level (shared)[mCD]
	double wetArea = 0;			// wetted area of the tidal range scheme (shared)[mCD]
	double sluiceQ = 0;			// flow rate through sluices (m3/s) (total)
	double inFlowRate = 0;	// inflow from other sources (m3/s) (total)
	int mode = 2; // operational mode of the tidal range scheme.

	tidalRangeScheme(const string& fileName); // constructor
	void updateTo(const double& SimTime); // key function, moves the tidal range scheme forward.

	double newWaterLevel(const double& InitialLevel, const double& volumeChange); // get the next water level
	int nextMode(const double& HeadDiff, const double& timeNow, const double& hdStart, const double& hdEnd, const double& pmpEnd, const int& mde, const bool& flex = false); // runs checks to determine next operational mode
	int nextMode(const double& HeadDiff, const double& timeNow, const bool& flex = false); // calls headDiff with the fixed level head diff start and end
	// determines if the lagoon is moving past a flex test/switch point
	bool isSwitchPoint(const double& Time1, const double& Time2);
	// outputs the correct curve if using ebb-flood-gen-pump system.
	int getCurveNumber(const double& HeadDiff, const int& trsMode, const int& i);
	// functions to return the internal values of the scheme
	inline bool is_Flexible() {return isFlexible;};
	inline bool is_Pumping() {return isPumping;};
	inline double getUpstream(){return upStream;};
	inline double getWetArea(){return wetArea;};
	inline string getName() { return schemeName; }
	inline double getSluiceQ(){return sluiceQ;}
	inline int getMode(){return mode;};
	// for the vectorised parameters the average is returned.
	double getDownStream(){
		double ds = 0;
		for (int i = 0; i < numBanks; i++) {
			ds += downStream[i];
		}
		ds /= (double)numBanks;
		return ds;};
	double getPowerOut(){
		double po = 0;
		for (int i = 0; i < numBanks; i++) {
			po += powerOut[i];
		}
		po /= numBanks;
		return po;};
	double getTurbineQ(){
		double tq = 0;
		for (int i = 0; i < numBanks; i++) {
			tq += turbineQ[i];
		}
		tq /= numBanks;
		return tq;};
};
//------------------IMPLEMENTATIONS------------------//

// this decision function is pretty damn granular and not as clean as
// i would like... lots of scope to improve this
// optional parameters hds and hde for when testing in flexible operation
// this is a hack to the overloading/default parameter issue
int tidalRangeScheme::nextMode(const double& HeadDiff, const double& timeNow, const bool& flex) {
	return nextMode(HeadDiff, timeNow, headDiffStart, headDiffEnd, pumpEnd, mode, flex);
}
int tidalRangeScheme::nextMode(const double& HeadDiff, const double& timeNow, const double& hdStart, const double& hdEnd, const double& pmpEnd, const int& mde, const bool& flex) {

	// this is a lambda, it's like a mini function
	auto upDatePrev = [&]() {
		if (flex) { flxPrevSwitchTime = timeNow; }
		else { prevSwitchTime = timeNow; }
	};
	// if we are not in a flexible test, and are still in the buffer zone
	if (!flex){
		 if (timeNow <= prevSwitchTime + switchModeBuffer) {
			 return mde;
		 }
		 //prevSwitchTime = timeNow;
	// if we are in flexible testing
	} else if (flex) {
			if (timeNow <= flxPrevSwitchTime + switchModeBuffer){
				return mde;
			}
			//flxPrevSwitchTime = timeNow;
	}

	double hde = hdEnd;
	double hds = hdStart;
	double hpe = pmpEnd;

	double control = 0;
	if (isPumping) {
		if (pumpControl == "time") {
			if (flex) {
				control = timeNow - flxPrevSwitchTime;
			}
			else {
				control = timeNow - prevSwitchTime;
			}
		}
		else if (pumpControl == "head") {
			control = absolute(HeadDiff);
		}
	}

	if (isPumping) {
		switch (operation) {
			case 1: // ebb only
				switch (mde) {
					case 1: // Filling/Sluicing
						if (HeadDiff > 0 ) {
							upDatePrev(); return 4;
						} else {
							return 1;
						}
						break;
					case 2: // Holding
						if (HeadDiff >= hds) {
							upDatePrev(); return 3;
						} else if (HeadDiff < 0) {
							upDatePrev(); return 4;
						} else {
							return 2;
						}
						break;
					case 3: // Generating
						if (hde > hds) {
							swap(hde,hds);
						}
						if (HeadDiff <= hde) {
							upDatePrev(); return 4;
						} else {
							return 3;
						}
						break;
					case 4: // pumping
						if (pumpControl == "origlevels") {
							for (int i = 0; i <= pumpTimes.size() - 1; i++) { // this could be slow with longer timeseries
								if (timeNow >= pumpTimes[i] && timeNow <= pumpTimes[i+1]) {
									if (pumpTargets[i] > pumpTargets[i + 1]) { // taerget[i] == low, i+1 === high
										if (upStream < pumpTargets[i]) {
											upDatePrev(); return 2;
										} else {
											return 4;
										}
									} else {
										if (upStream > pumpTargets[i]) {
											upDatePrev(); return 2;
										} else {
											return 4;
										}
										break;
									}
								}
							}
							return 2;
							break;
						}
						if (control >= hpe) {
							upDatePrev(); return 2;
						} else {
							return 4;
						}
						break;
					default:
						std::cout << "Tidal range scheme attempted invalid mode..." << endl;
						return 0;
						break;
				}
			case 2: // 2 way
			double hd = absolute(HeadDiff);

				switch (mde) {
					case 1: // Filling/Sluicing
						if (hd < 0.05 ) {
							upDatePrev(); return 4;
						} else {
							return 1;
						}
						break;
					case 2: // Holding
						if (hd >= hds) {
							upDatePrev(); return 3;
						} else if (HeadDiff < 0) {
							upDatePrev(); return 1;
						} else {
							return 2;
						}
						break;
					case 3: // Generating
						if (hde > hds) {
							swap(hde,hds);
						}
						if ( hd <= hde ) {
							upDatePrev(); return 2;
						} else {
							return 3;
						}
						break;
						// this is a troubling part because of the ebb/flood component.
					case 4: // pumping
						if (control >= hpe) {
							upDatePrev(); return 2;
						} else {
							return 4;
						}
						break;
					default:
						cout << "Tidal range scheme attempted invalid mode..." << endl;
						return 0;
						break;
				}

		}
	} else {
		switch (operation) {
			case 1: // ebb only
				switch (mde) {
					case 1: // Filling/Sluicing
						if (HeadDiff >= hds) {
							upDatePrev(); return 3;
						}else if (HeadDiff > 0 ) {
							upDatePrev(); return 2;
						} else {
							return 1;
						}
						break;
					case 2: // Holding
						if (HeadDiff >= hds) {
							upDatePrev(); return 3;
						} else if (HeadDiff < 0) {
							upDatePrev(); return 1;
						} else {
							return 2;
						}
						break;
					case 3: // Generating
						if (hde > hds) {
							swap(hde,hds);
						}
						if (HeadDiff <= hde) {
							upDatePrev(); return 2;
						} else {
							return 3;
						}
						break;
					default: // error has occured.
						cout << "Tidal range scheme attempted invalid mode..." << endl;
						return 0;
						break;
				}
			case 2: // 2 way
			double hd = absolute(HeadDiff);
				switch (mde) {
					case 1: // Filling/Sluicing
						if (hd >= hds) {
							upDatePrev(); return 3;
						}else if (hd < 0.05 ) {
							upDatePrev(); return 2;
						} else {
							return 1;
						}
						break;
					case 2: // Holding
						if (hd >= hds) {
							upDatePrev(); return 3;
						} else {
							return 2;
						}
						break;
					case 3: // Generating
						if (hde > hds) {
							swap(hde,hds);
						}
						if (hd <= hde || hd >= hds) {
							upDatePrev(); return 1;
						} else {
							return 3;
						}
						break;
					default: // error has occured.
						cout << "Tidal range scheme attempted invalid mode..." << endl;
						//system("pause");
						return 0;
						break;
				}
		}
	}
	//system("pause");
	return 0;
}

// 0:EBB_GEN 1:FLOOD_GEN 2:EBB_PMP 3:FLOOD_PMP
int tidalRangeScheme::getCurveNumber(const double& HeadDiff, const int& trsMode, const int& i) {
	if (isEbbFloodPumpCurves) {
		int curve = 0;
		if (HeadDiff < 0) { curve += 1; } //EBB
		curve += (trsMode - 3); // 3 = Generating, 4 = Pumping
		return curve;
	}
	return i;
}

void tidalRangeScheme::updateTo(const double& SimTime) {

	// based on the rates of the previous step
	double volumeChange = sluiceQ + inFlowRate;
	for (int i = 0; i < numBanks; i++) {
		volumeChange += turbineQ[i];
	}
	//determine the fluid volume change of the scheme
	volumeChange *= -1*(SimTime - prevTime)*60*60; // m^3 * seconds in time step

	// and use that to determine new upstream level
	double newUpStream = trsArea.newWaterLevel(upStream, volumeChange);
	// this should stop oscillations...
	if ((upStream > downStream[0] && newUpStream < externalWaterLevel[0].getLevel(SimTime))||(upStream < downStream[0] && newUpStream > externalWaterLevel[0].getLevel(SimTime))) {
		upStream = externalWaterLevel[0].getLevel(SimTime);
	} else {
		upStream = newUpStream;
	}
	// ramp function for when changing modes
	double ramp = 1.0;
	if ((SimTime - prevSwitchTime) < rampTime) {
		ramp = (1 - cos(3.1415926 * (SimTime - prevSwitchTime) / rampTime)) / 2;
	}

	// from the upstream level we can determine the new area
	wetArea = trsArea.getWettedArea(upStream);

	// at every timestep: determine if this is a decision timestep
	// if it is, we run the next tidal cycle at different start/stop points,
	// using a coarser timestep than the model itself (maybe 1/2 hour?)
	// and find the best one(s), then set headDiffStart and headDiffEnd based
	// on that to be used for every timestep until the next change point...
	if (isFlexible && isSwitchPoint(prevTime, SimTime)) {
			cout << " Flexing at " << schemeName;
			// vectors for storing the start and end heads when finding best solution
			vector<double> flxHs; // start
			vector<double> flxHe; // end
			vector<double> flxHpe; // pump end
			vector<double> flxProf; // profit

			// starting head
			double hs = headDiffStartMin;
			//double prevHs = headDiffStart; double prevHe = headDiffEnd;

			while (hs <= headDiffStartMax) {

				// finishing head
				double he = headDiffEndMin;

				while (he <= minimum(headDiffEndMax,(hs - headDiffEndDelta))){

					double hpe;

					if (!isPumpFlexible) {
						hpe = pumpEndMax;
					} else {
						hpe = pumpEndMin;
					}
					while (hpe <= pumpEndMax) {

						// copy lagoon state to local variables
						double flxT = SimTime;
						flxPrevSwitchTime = prevSwitchTime;

						// double flxWetArea = wetArea; not needed here
						double flxUpstream = upStream;
						double flxSluiceQ = sluiceQ;
						double flxInflow = inFlowRate;
						int flxMode = mode;
						double flxProduct = 0; // the variable to be maximised
						double flxMax = flxUpstream;
						double flxMin = flxUpstream; // min and max hold params for range based analysis
						vector<double> flxDownStream = downStream;
						vector<double> flxTurbineQ = turbineQ;
						vector<double> flxHeadDiff = headDiff;
						vector<double> flxPowerOut = powerOut;
                        //ofstream tmp_f;
                        //string tmp_fname = "flx_" + to_string(he) + "_" + to_string(hs) + "_tmp.csv";
                        //tmp_f.open(tmp_fname);
                        //tmp_f << "Time[Hrs],US[mCD],US[mCD],power[MW],turbQ[m3/s],SlQ[m3/s],Mode[-],flxProd[-],flxdV[m3]" << endl;

						// at every coarse timestep in test period
						while (flxT < SimTime + flexPeriod) {
							double flxVolumeChange = flxSluiceQ + flxInflow;
							for (int i = 0; i < numBanks; i++) {
								flxVolumeChange += flxTurbineQ[i];
							}
							flxVolumeChange *= -1 * (flexDt * 60 * 60);
							flxUpstream = trsArea.newWaterLevel(flxUpstream, flxVolumeChange);

							// run the model as if it were fixed head.
							bool flxSafetyHold = false;
							bool flxDryUpstream = false;
							for (int i = 0; i < numBanks; i++) {
								flxDownStream[i] = externalWaterLevel[i].getLevel(flxT);
								flxHeadDiff[i] = flxUpstream - flxDownStream[i];

								// do any of the banks trip the safety warning?
								if (flxUpstream <= turbines[i].safeLevel(safetyBuffer) || flxDownStream[i] <= turbines[i].safeLevel(safetyBuffer)) {
									flxSafetyHold = true;
								}
							}
							if (flxUpstream <= trsArea.minInternalLevel + safetyBuffer) {
								flxDryUpstream = true;
							}
							// this is where it gets tricky/interesting.
							// crude initial guess based on first set of banks
							// 'true' tells the nextmode function that we're in flex operation
							// so that it shouldn't disturb the real prevSwitchMode
							flxMode = nextMode(flxHeadDiff[0], flxT, hs, he, hpe, flxMode, true);

							if (flxSafetyHold == true) {
								if (flxMode != 2) {
									flxPrevSwitchTime = flxT;
								}
								flxMode = 2;
							}

							for (int i = 0; i < numBanks; i++) {
								switch (flxMode) {
								case 1: // Filling/sluicing
									flxPowerOut[i] = 0;
									if (flxDryUpstream) {
										if (flxTurbineQ[i] > 0) {
											flxTurbineQ[i] = 0;
											flxSluiceQ = 0;
										}
									}
									else {
										flxTurbineQ[i] = turbines[i].orifice(flxHeadDiff[i]) * ramp * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
										flxSluiceQ = orificeFlow(sluiceCd, flxHeadDiff[0], areaSluices) * ramp * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
									}
									break;
								case 2: // Holding
									flxPowerOut[i] = 0;
									flxTurbineQ[i] = 0;
									flxSluiceQ = 0;
									break;
								case 3: // Generating
									flxSluiceQ = 0;
									if (flxDryUpstream) {
										if (flxTurbineQ[i] > 0) {
											flxTurbineQ[i] = 0;
											flxPowerOut[i] = 0;
											flxMode = 2; flxPrevSwitchTime = flxT;
										}
									}
									else {
										flxPowerOut[i] = turbines[i].getPower(flxHeadDiff[i]) * ramp * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
										flxTurbineQ[i] = turbines[i].getFlow(flxHeadDiff[i]) * ramp * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
									}
									break;
								case 4: // Pumping
									flxSluiceQ = 0;
									if (flxDryUpstream) {
										flxMode = 2; flxPrevSwitchTime = flxT;
										flxPowerOut[i] = 0;
										flxTurbineQ[i] = 0;
									}
									else {
										flxPowerOut[i] = -1 * turbines[i].getPower(flxHeadDiff[i]) * ramp * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
										flxTurbineQ[i] = -1 * turbines[i].getFlow(flxHeadDiff[i]) * ramp * pumpEff * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
									}
									break;
								}

								// here it optimises based on the desired parameter
								// if you want to add more, you can copy/use the methods from
								// the existing two, and add them to the if-else tree.
								if (flexVariable == "profit" || flexVariable == "Profit") {
									flxProduct += flxPowerOut[i] * energyValue.getLevel(flxT) * flexDt;
								}
								else if (flexVariable == "energy" || flexVariable == "Energy") {
									flxProduct += flxPowerOut[i] * flexDt;
								}
								else if (flexVariable == "intertidal" || flexVariable == "Intertidal") {
									if (flxUpstream < flxMin) {
										flxMin = flxUpstream;
									}
									else if (flxUpstream > flxMax) {
										flxMax = flxUpstream;
									}
									// it might be faster to put this elsewhere, but it keeps things
									// nice and compact.
									flxProduct = flxMax - flxMin;
								}
							}
                            //tmp_f <<  flxT<<","<<flxUpstream<<","<<flxDownStream[0]<<","<<flxPowerOut[0]<<","<<flxTurbineQ[0]<<","<<flxSluiceQ<<","<<flxMode<<","<<flxProduct << "," << flxVolumeChange << endl;
							flxInflow = inFlow.getLevel(flxT);
							flxT += flexDt;
						}
						// save the values found in this sample
						flxHs.push_back(hs);
						flxHe.push_back(he);
						flxHpe.push_back(hpe);

						flxProf.push_back(flxProduct);
                        //tmp_f.close();

						// test the next pump end level
						hpe += pumpEndDelta;
					}
					// test next head diff end
					he += headDiffEndDelta;
				}
				// test the next head diff start
				hs += headDiffStartDelta;
			}
			// set start and end point for the main model based on the optimal
			// values found int the flex test period
			double tmp_maxV = maxVal(flxProf);
			int tmp_i_mV = iOfMaxVal(flxProf);

			if ((maxVal(flxProf) >= 0.001) && (flexVariable == "energy" || flexVariable == "Energy"|| flexVariable == "profit"|| flexVariable == "Profit")) {
				int mxv = iOfMaxVal(flxProf);
				headDiffStart = flxHs[mxv];
				headDiffEnd = flxHe[mxv];
				pumpEnd = flxHpe[mxv];
			}

			ofstream flexInfo; string flxName = schemeName + "_flx_Info.csv";
			flexInfo.open(flxName,ofstream::app);
			flexInfo << prevTime << ", "<< headDiffStart<< ", "<< headDiffEnd << ", " << pumpEnd << ", " << maxVal(flxProf);
			//for (int i = 0; i < flxProf.size(); i++) {
			//	flexInfo << ", " << flxProf[i];
			//}
			flexInfo << endl;
			flexInfo.close();
			cout << endl;
	}
	// determine the mode of the model by working through the banks.
	// Need to implement some kind of heirarchy, here...

	bool safetyHold = false;
	bool dryUpstream = false;

	for (int i = 0; i < numBanks; i++ ) {
		downStream[i] = externalWaterLevel[i].trimUpTo(SimTime);
		headDiff[i] = upStream - downStream[i];

		// this is where it gets tricky/interesting.
		// crude initial guess based on first set of banks
		if (i == 0) {
			mode = nextMode(headDiff[i], SimTime);
		}
		// do any of the banks trip the safety warning?
		if (upStream <= turbines[i].safeLevel(safetyBuffer)||downStream[i] <= turbines[i].safeLevel(safetyBuffer)){
			safetyHold = true;
		}
	}

	if (upStream <= (trsArea.minInternalLevel+ safetyBuffer)){
		dryUpstream = true;
	}

	if (safetyHold == true) {
		if (mode != 2) {
			prevSwitchTime = SimTime;
		}
		mode = 2;
	}

	for (int i = 0; i < numBanks; i++ ) {
		switch (mode) {
			case 1: // Filling/sluicing
				powerOut[i] = 0;
				if (dryUpstream) {
					if (turbineQ[i] > 0) {
						turbineQ[i] = 0;
					}
				}
				else {
					turbineQ[i] = turbines[i].orifice(headDiff[i]) * ramp * kronecker(i, getCurveNumber(headDiff[i], mode, i));
				}
				break;
			case 2: // Holding
				powerOut[i] = 0;
				turbineQ[i] = 0;
				break;
			case 3: // Generating
				if (dryUpstream) {
					if (turbineQ[i] > 0) {
						//mode = 2; //prevSwitchTime = SimTime;
						powerOut[i] = 0;
						turbineQ[i] = 0;
						mode = 2; prevSwitchTime = SimTime;
					}
				}
				else {
					powerOut[i] = turbines[i].getPower(headDiff[i]) * ramp * kronecker(i, getCurveNumber(headDiff[i], mode, i));
					turbineQ[i] = turbines[i].getFlow(headDiff[i]) * ramp * kronecker(i, getCurveNumber(headDiff[i], mode, i));
				}
				break;
			case 4: // Pumping
				if (dryUpstream) {
					mode = 2; prevSwitchTime = SimTime;
					powerOut[i] = 0;
					turbineQ[i] = 0;
				}
				else {
					powerOut[i] = -1 * turbines[i].getPower(headDiff[i]) * ramp * kronecker(i, getCurveNumber(headDiff[i], mode, i));
					turbineQ[i] = -1 * turbines[i].getFlow(headDiff[i]) * ramp * kronecker(i, getCurveNumber(headDiff[i], mode, i)) * pumpEff;
				}
				break;
		}
	}
	// update the flow through the sluices
	if (mode == 1){
		sluiceQ = orificeFlow(sluiceCd, headDiff[0], areaSluices) * ramp;
	} else {
		// 0 m^3/s if not in fill/sluice mode
		sluiceQ = 0;
	}
	inFlowRate = inFlow.trimUpTo(SimTime);
  double tmp = energyValue.trimUpTo(SimTime);

	prevTime = SimTime; // and update time to current step.
};

tidalRangeScheme::tidalRangeScheme(const string& fileName) {
	ifstream inFile(fileName);
	bool banksRead = false;

	if (inFile.is_open()) {
		// the basic string that is used for reading what is in the file.
		string tmp = "NaN";

		//vectors to store the info needed to build the turbine banks
		vector<string> turbFileNames;
		vector<int> numTurbines;
		vector<double> turbDiams;
		vector<double> turbCenterLevels;
		string schmFile;

		getline(inFile, tmp); // reads the header line.

		// this reads the file and checks every key - value pair until it hits the "End"
		// key. It's not pretty but it should work for most situations...
		while (tmp != "End") {
			inFile >> tmp;
			//if (tmp[1] == '/'){
			//	inFile >> tmp;
			//}
			if (tmp == "End" || tmp == "end" || tmp == "END") {
				break;
			}else if (tmp == "schemeName:") {
				inFile >> schemeName;
			} else if (tmp == "numBanks:") {
				inFile >> numBanks;
				banksRead = true;
			} else if (tmp == "externalWaterLevel:") {
				banksWarning(banksRead,tmp);
				for (int i = 0; i < numBanks; i++) {
					inFile >> tmp;
					externalWaterLevel.push_back(timeSeries(tmp));
				}
			} else if (tmp == "operation:") {
				inFile >> tmp;
				if (tmp == "Ebb"||tmp == "ebb"||tmp == "Ebb-Only"||tmp == "ebb-only"||tmp == "1"){
					operation = 1;
				} else if (tmp == "Two-Way"||tmp == "two-way"||tmp == "2wy"||tmp == "2Way"||tmp == "2way"||tmp == "2"){
					operation = 2;
				} else {
					cout << "Invalid operation mode [" << tmp <<  "] entered in: " << fileName << endl;
					//system("pause");
				}
			} else if (tmp == "pumping:") {
				inFile >> tmp;
				isPumping = boolStr(tmp);
			} else if (tmp == "flexible:") {
				inFile >> tmp;
				isFlexible = boolStr(tmp);
			} else if (tmp == "flexVariable:") {
				inFile >> flexVariable;
			} else if (tmp == "inFlowFile:") {
				inFile >> tmp;
				inFlow = timeSeries(tmp);
			} else if (tmp == "schemeArea:") {
				inFile >> schmFile;
			} else if (tmp == "externalWaterLevel:") {
				banksWarning(banksRead,tmp);
				for (int i = 0; i < numBanks; i++) {
					inFile >> tmp;
					externalWaterLevel.push_back(timeSeries(tmp));
				}
			} else if (tmp == "energyValue:") {
				for (int i = 0; i < numBanks; i++) {
					inFile >> tmp;
					energyValue = timeSeries(tmp);
				}
			} else if (tmp == "turbineFile:") {
				banksWarning(banksRead,tmp);
				for (int i = 0; i < numBanks; i++) {
					inFile >> tmp;
					turbFileNames.push_back(tmp);
				}
			}else if (tmp == "areaSluices(m2):") {
				inFile >> areaSluices;
			}else if (tmp == "numberTurbines(int):") {
				banksWarning(banksRead,tmp);
				int tmpInt;
				for (int i = 0; i < numBanks; i++) {
					inFile >> tmpInt;
					numTurbines.push_back(tmpInt);
				}
			} else if (tmp == "turbineDiameter(m):") {
				banksWarning(banksRead,tmp);
				double tmpDub;
				for (int i = 0; i < numBanks; i++) {
					inFile >> tmpDub;
					turbDiams.push_back(tmpDub);
				}
			} else if (tmp == "headDiffStart(m):") {
				if (isFlexible) {
					inFile >> headDiffStartMin >> headDiffStartMax;
					if (headDiffStartMin > headDiffStartMax) {
						swap(headDiffStartMin, headDiffStartMax);
					}
				} else {
					inFile >> headDiffStart;
				}
			} else if (tmp == "headDiffEnd(m):") {
				if (isFlexible) {
					inFile >> headDiffEndMin >> headDiffEndMax;
					if (headDiffEndMin > headDiffEndMax) {
						swap(headDiffEndMin,headDiffEndMax);
					}
				} else {
					inFile >> headDiffEnd;
				}
			} else if (tmp == "waterLevelMax(m):") {
				inFile >> maxWaterLevel;
			} else if (tmp == "waterLevelMin(m):") {
				inFile >> minWaterLevel;
			}else if (tmp == "sluiceCoefQ(0-1):") {
				inFile >> sluiceCd;
			}else if (tmp == "sluiceCoefQ(%):") {
				double tmpD;
				inFile >> tmpD;
				sluiceCd = tmpD/100;
			}else if (tmp == "turbineCoefQ(0-1):") {
				inFile >> turbineCd;
			}else if (tmp == "turbineCoefQ(%):") {
				double tmpD;
				inFile >> tmpD;
				turbineCd = tmpD/100;
			}else if (tmp == "generatorEfficiency(%):") {
				double tmpD;
				inFile >> tmpD;
				generatorEfficiency = tmpD/100;
			}else if (tmp == "generatorEfficiency(0-1):") {
				inFile >> generatorEfficiency;
			}else if (tmp == "turbineCentreLevel(m):") {
				banksWarning(banksRead,tmp);
				for (int i = 0; i < numBanks; i++) {
					double tmpDub;
					inFile >> tmpDub;
					turbCenterLevels.push_back(tmpDub);
				}
			}else if (tmp =="turbineSafetyBuffer(m):") {
				inFile >> safetyBuffer;
			}else if (tmp =="headDiffStartDelta(m):") {
				inFile >> headDiffStartDelta;
			}else if (tmp =="headDiffEndDelta(m):") {
				inFile >> headDiffEndDelta;
			}else if (tmp =="flexDt(hrs):") {
				inFile >> flexDt;
			}else if (tmp =="flexPeriod(hrs):") {
				inFile >> flexPeriod;
			}else if (tmp == "flexInterval(hrs):") {
				inFile >> flexInterval;
			}else if (tmp == "switchModeBuffer(hrs):") {
				inFile >> switchModeBuffer;
			}else if (tmp == "initalInternalWL(mCD):") {
				inFile >> upStream;
			}else if (tmp == "flexType:") {
				inFile >> flexType;
			}else if (tmp == "rampTime(min):") {
				double tmpDub;
				inFile >> tmpDub;
				rampTime = tmpDub / 60.0;
			}else if (tmp == "rampTime(hrs):") {
				inFile >> rampTime;
			}else if (tmp == "rampTime(s):") {
				double tmpDub;
				inFile >> tmpDub;
				rampTime = tmpDub / 3600.0;
			}else if (tmp == "pumpControl:") {
				inFile >> pumpControl;
			}else if (tmp == "pumpEnd:") {
				inFile >> pumpEnd;
			}else if (tmp == "pumpEndMin:") {
				inFile >> pumpEndMin;
			}else if (tmp == "pumpEndMax:") {
				inFile >> pumpEndMax;
			}else if (tmp == "pumpEndDelta:") {
				inFile >> pumpEndDelta;
			}else if (tmp == "flexPumping:") {
				string tmpS;
				inFile >> tmpS;
				isPumpFlexible = boolStr(tmpS);
			}else if (tmp == "pumpEfficiency(0-1):") {
				inFile >> pumpEff;
			}else if (tmp == "pumpEfficiency(%):") {
				double tmpD;
				inFile >> tmpD;
				pumpEff = tmpD / 100;
			}else if (tmp == "ebbFloodPump(ABCD):") {
				string tmpS;
				inFile >> tmpS;
				isEbbFloodPumpCurves = boolStr(tmpS);
				cout << "PLEASE REMEMBER THAT YOU MUST SUPPLY BOTH EBB AND FLOOD/PUMP AND GENERATION CURVE FILES" << endl;
			}else {
				cout << "Invalid parameter <" << tmp << "> in [" << fileName << "] ..."<< endl;
				//system("pause");
			}
		}
		if (upStream == -999) {
			upStream = externalWaterLevel[0].getLevel(0);
		}
		for (int i = 0; i < numBanks; i++) {
			turbines.push_back(turbineBank(turbFileNames[i], numTurbines[i], turbDiams[i], turbCenterLevels[i]));
			downStream.push_back(externalWaterLevel[i].getLevel(0));
			headDiff.push_back(upStream - downStream[i]);
			turbineQ.push_back(0);
			powerOut.push_back(0);
		}
		trsArea = schemeArea(schmFile, minWaterLevel, maxWaterLevel);
	}else {
		cout << "Unable to open [" << fileName << "]..." << endl;
		//system("pause");
	}
	if (isEbbFloodPumpCurves) {
		if (numBanks != 4) {
			cout << "WARNING: <" << numBanks << "> should be 4 for Ebb/Flood Generating/Pumping curve combos." << endl;
		}
	}
	if (isFlexible) {
		headDiffStart = (headDiffStartMin + headDiffStartMax) / 2;
		headDiffEnd = (headDiffEndMin + headDiffEndMax) / 2;
		pumpEnd = (pumpEndMin + pumpEndMax) / 2;

		ofstream flexInfo;
		string flxName = schemeName + "_flx_Info.csv";
		flexInfo.open(flxName);
		flexInfo << schemeName << " Flexible operation info."<< endl
			<< "Time(Hrs), HStart(m), HEnd(m), PmpEnd(-),Optimum"<< endl;
		flexInfo.close();

		switchTimes.push_back(0);

		if (flexType == "time"||pumpControl == "origlevels") {
			double tmp = 0;
			while (tmp < externalWaterLevel[0].endTime()) {
				tmp += flexInterval;
				switchTimes.push_back(tmp);

			}
		} else {
			timeSeries wl = externalWaterLevel[0];
			vector<double> t_aves, z_aves;
			for (int i = 2; i < wl.numReadings - 2; i++) {
				t_aves.push_back(wl.time[i]);
				z_aves.push_back((wl.level[i-2]+wl.level[i-1]+wl.level[i]+wl.level[i+1]+wl.level[i+2])/5.0);
			}
			if (pumpControl == "origlevels") {
				pumpTargets.push_back(0);
				pumpTimes.push_back(0);
				for (int i = 1; i < t_aves.size() - 1; i++) {
					if ((z_aves[i] < z_aves[i - 1] && z_aves[i] < z_aves[i + 1]) || (z_aves[i] > z_aves[i - 1] && z_aves[i] > z_aves[i + 1])) {
						pumpTimes.push_back(t_aves[i]);
						pumpTargets.push_back(z_aves[i]);
					}
				}
			}
			if (flexType == "high"||flexType == "High"||flexType == "HIGH") {
					for (int i = 1; i < t_aves.size() - 1; i++) {
						if (z_aves[i] > z_aves[i-1] && z_aves[i] > z_aves[i+1]) {
							switchTimes.push_back(t_aves[i]);
						}
					}
			}
			else if (flexType == "low"||flexType == "Low"||flexType == "LOW") {
				for (int i = 1; i < t_aves.size() - 1; i++) {
					if (z_aves[i] < z_aves[i - 1] && z_aves[i] < z_aves[i + 1]) {
						switchTimes.push_back(t_aves[i]);
					}
				}
			}
			else if (flexType == "highlow"||flexType == "highLow"||flexType == "HighLow"||flexType == "HIGHLOW") {
				for (int i = 1; i < t_aves.size() - 1; i++) {
					if ((z_aves[i] < z_aves[i - 1] && z_aves[i] < z_aves[i + 1]) || (z_aves[i] > z_aves[i - 1] && z_aves[i] > z_aves[i + 1])) {
						switchTimes.push_back(t_aves[i]);
					}
				}
			} else if (flexType == "highhold"||flexType == "highHold"||flexType == "HighHold"||flexType == "HIGHHOLD") {
				for (int i = 1; i < t_aves.size() - 1; i++) {
					if (z_aves[i] > z_aves[i-1] && z_aves[i] > z_aves[i+1]) {
						switchTimes.push_back(t_aves[i]);
					}
				}
			} else {
				cout << "Invalid flex type entered." << endl;
				//return 0;

			}
		}
	}
};



// for flexible operation switch point determination.
bool tidalRangeScheme::isSwitchPoint(const double& Time1, const double& Time2) {

	int i = 0;
	double nextSwitchPoint = 0.0;
	// default to 0 being a switch point
	// find the first multiple of flexInterval that is biggert than the first time
	while (nextSwitchPoint < Time1) {
		i++;
		nextSwitchPoint = switchTimes[i];

	}
	// if the next timestep is after the interval
	if (Time2 >= nextSwitchPoint) {
		return true;
	}
	return false;
};
#endif
