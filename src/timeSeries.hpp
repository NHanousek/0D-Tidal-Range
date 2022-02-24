#ifndef TIMESERIES
#define TIMESERIES
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class timeSeries {
	int prevTSi = 1;
public:
    int numReadings = 0;
    double timeStep = 0;
    string source = "NaN";
    vector<double> time, level;	// times and parameter levels
		timeSeries() = default;
    timeSeries(const string& fileName);
    double getLevel(const double& timeNow);
		double trimUpTo(const double& timeNow);
		double endTime() {return time.back();}
};
//-------------------Implementation-------------------------
timeSeries::timeSeries(const string& fileName) {
	// if you want to have a timeseries that does nothing,
	// the filename can be passed as "false" or "none" and
	// the value is set constant at 0 for 1 million hours
	// if your model runs longer than that, change this.
	if (fileName == "False" || fileName == "false" || fileName == "none" || fileName == "None" || fileName == "zero") {
		source = "constantZero"; // reminder for debugging, encodes for quick returns
		numReadings = 2;
		timeStep = 1;
		time.push_back(0);		level.push_back(0);
		time.push_back(1);	level.push_back(0);
	} else if (fileName == "Const" || fileName == "const" || fileName == "Constant" || fileName == "constant" || fileName == "One" || fileName == "one") {
		source = "constantOne"; // reminder for debugging, encodes for quick returns
		numReadings = 2;
		timeStep = 1;
		time.push_back(0);		level.push_back(1);
		time.push_back(1);	level.push_back(1);
	} else {
		// save the file name for documentation
		source = fileName;
	    ifstream tsFile;
		tsFile.open(fileName);
		if (tsFile.is_open()) {

			// string for loading in parameters until ":EndHeader"
			string tmp = "NaN";

			// the file can contain almost anything in the header, the loader
			// won't care.
			while (tmp != ":EndHeader") {
				tsFile >> tmp;
				// only loads delta t for now, can add in other parameters you
				// want pulled, such as x and y in utm, or the source model name.
			 	if (tmp == ":DeltaT") {
					// need some temp values for converting time format to dt in hours
					char tempChar;
					int dtHour, dtMinute;
					double dtSeconds;

					// timeStep is in hours.
					tsFile >> dtHour >> tempChar >> dtMinute >> tempChar >> dtSeconds;
					timeStep = (double)dtHour + (double)dtMinute / 60.0 + dtSeconds / 3600.0;
				}
			}
			// load the data from the rest of the file
			int i = 0;
			double tmpD;
			while (tsFile >> tmpD) {
				level.push_back(tmpD);
				// first reading is always at time = 0;
				time.push_back(i * timeStep);
				i++;
			}
			numReadings = level.size();
			tsFile.close();
		}
		else {
			cout << "TimeSeries file [" << fileName << "] could not be opened" << endl;
		}
	}
};
// return the value of the timeseries at a given time
// not the most efficient method probably, but it seems to work
double timeSeries::getLevel(const double& timeNow) {
	if (source == "constantZero") {
		return 0.0;
	} else if (source == "constantOne") {
		return 1.0;
	}
	// for every step, if the time specified is between two time points, or
	// at a time point, interpolate between the specified times.
	if (time[prevTSi] <= timeNow && timeNow < time[prevTSi + 1]) {
		return interpolate(time[prevTSi], timeNow, time[prevTSi + 1], level[prevTSi], level[prevTSi + 1]);
	}else {
		for (int i = 0; i < time.size(); i++) {
			if (time[i] <= timeNow && timeNow < time[i + 1]) {
				prevTSi = i;
				return interpolate(time[i], timeNow, time[i + 1], level[i], level[i + 1]);
			}
		}
	}
	cout << "Warning: Max value of timeSeries [" << source << "] returned.";
	return level.back();
};
// Gets the level and then trime the time series up to there,
// improves performance in long models.
double timeSeries::trimUpTo(const double& timeNow) {
	if (source == "constantZero") {
		return 0.0;
	} else if (source == "constantOne") {
		return 1.0;
	}
	// for every step, if the time specified is between two time points, or
	// at a time point, interpolate between the specified times.
	double retLevel;
	if (timeNow > time.back()) {
		retLevel = level.back();
		cout << "Warning: Max value of timeSeries [" << source << "] returned.";
	} else {
		int x;
		for (int i = 0; i < (int)time.size(); i++) {
			if (time[i] <= timeNow && timeNow < time[i + 1]) {
				retLevel = interpolate(time[i], timeNow, time[i + 1], level[i], level[i + 1]);
				x = i - 1;
			}
		}
		if (x > 2) {
			level.erase (level.begin(),level.begin()+x - 1);
			time.erase (time.begin(),time.begin()+x - 1);
		}
	}
	return retLevel;

};
#endif
