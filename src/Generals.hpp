// general functions and parameters for the modelling,
// mainly maths stuff

#ifndef GENERALS
#define GENERALS

#include "TypesDef.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;
//Maths
const double pi = 3.14159265359;    // very accurate. very good.
const double Pi = 3.14159265359;    // so that i don't need to remember if its caps.
const double g = 9.807;             // Gravity

bool boolStr(const string& str) {
    if (str == "true" || str == "TRUE" || str == "True" || str == "yes" || str == "Yes" || str == "YES" || str == "Y") {
        return true;
    }
    else if (str == "false" || str == "FALSE" || str == "False" || str == "no" || str == "NO" || str == "No" || str == "n") {
        return false;
    }
    else {
        cout << "Invalid bool var [" << str << "], false returned." << endl;
        return false;
    }
}


//constant absolute values for ints and doubles
inline double absolute(const double& input) {
    if (input < 0.0) {
        return (-1.0 * input);
    }
    return input;
}
inline int absolute(const int& input) {
    if (input < 0) {
        return (-1 * input);
    }
    return input;
}

int iOfMaxVal(const vector<double>& vect) {
    int iMax = 0;
    double vMax = vect[0];
    for (int i = 0; i < vect.size(); i++) {
        if (vect[i] > vMax) {
            vMax = vect[i];
            iMax = i;
        }
    }
    return iMax;
}
int iOfMaxVal(const vector<int>& vect) {
    int iMax = 0;
    int vMax = vect[0];
    for (int i = 0; i < vect.size(); i++) {
        if (vect[i] > vMax) {
            vMax = vect[i];
            iMax = i;
        }
    }
    return iMax;
}
int iOfMinVal(const vector<double>& vect) {
    int iMin = 0;
    double vMin = vect[0];
    for (int i = 0; i < vect.size(); i++) {
        if (vect[i] < vMin) {
            vMin = vect[i];
            iMin = i;
        }
    }
    return iMin;
}
int iOfMinVal(const vector<int>& vect) {
    int iMin = 0;
    int vMin = vect[0];
    for (int i = 0; i < vect.size(); i++) {
        if (vect[i] < vMin) {
            vMin = vect[i];
            iMin = i;
        }
    }
    return iMin;
}

double minVal(const vector<double>& vect) {
    int iMin = 0;
    int vMin = vect[0];
    for (int i = 0; i < vect.size(); i++) {
        if (vect[i] < vMin) {
            vMin = vect[i];
            iMin = i;
        }
    }
    return vMin;
}
double maxVal(const vector<double>& vect) {
    double vMax = vect[0];
    for (int i = 0; i < vect.size(); i++) {
        if (vect[i] > vMax) {
            vMax = vect[i];
        }
    }
    return vMax;
}

// takes three sets of xyz points and finds the area in the xy plane
double areaTriangle(const tdouble3& A, const tdouble3& B, const tdouble3& C) {
    //https://www.mathopenref.com/coordtrianglearea.html
    double area, calc1, calc2, calc3;
    calc1 = A.x * (B.y - C.y);
    calc2 = B.x * (C.y - A.y);
    calc3 = C.x * (A.y - B.y);
    area = absolute((calc1 + calc2 + calc3) / 2);
    return area;
}
// takes three sets of xyz points and finds the mean z height
double meanHeight(const tdouble3& A, const tdouble3& B, const tdouble3& C) {
    return (A.z + B.z + C.z) / 3;
}
// orifice equation for flow magnitude with direction through an orifice
double orificeFlow(const double& dischargeCoeff, const double& headDifference, const double& area) {
    if (headDifference >= 0) {
        return (double)dischargeCoeff * area * sqrt(2 * g * absolute(headDifference));
    }
    else {
        return (double)(dischargeCoeff * area * sqrt(2 * g * absolute(headDifference))*-1);
    }
}
// simple trapezium rule for areas and volumes
double trapezium(const double& area1, const double& area2, const double& dh) {    return (dh * (area1 + area2) / 2);    }

// interpolate to find y2 from (x1,y1), (x3,y3) and x2.
double interpolate(const double& x1, const double& x2, const double& x3, const double& y1, const double& y3) {
    // if it's outside the points, use the bound.
    if (x2 > x1 && x2 > x3) {
        return y3;
    }
    else if (x2 < x1 && x2 < x3) {
        return y1;
    }
    else {
        return y1 + (x2 - x1) * (y3 - y1) / (x3 - x1);
    }
    //https://www.ajdesigner.com/phpinterpolation/linear_interpolation_equation.php
}
double interpolate(const int& x1, const int& x2, const int& x3, const int& y1, const int& y3) {
    if (x2 > x1&& x2 > x3) {
        return y3;
    }
    else if (x2 < x1 && x2 < x3) {
        return y1;
    }
    else {
        return (double)y1 + ((double)x2 - (double)x1) * ((double)y3 - (double)y1) / ((double)x3 - (double)x1);
    }
}
double extrapolate(const double& x1, const double& x2, const double& x3, const double& y1, const double& y3) {
    return y1 + (x2 - x1) * (y3 - y1) / (x3 - x1);
}
int kronecker(const int& ia, const int& ib) {
    return (ia == ib);
}
double maximum(const double& val1, const double& val2){
    if (val1 > val2) {
        return val1;
    }
    return val2;
}
double minimum(const double& val1, const double& val2){
    if (val1 < val2) {
        return val1;
    }
    return val2;
}
double trsRamp(const double& simtime, const double& prevswitchtime, const double& ramptime) {
    if ((simtime - prevswitchtime) < ramptime) {
        return (1 - cos(3.1415926 * (simtime - prevswitchtime) / ramptime)) / 2;
    } else {
        return 1.0;
    }
}
#endif
