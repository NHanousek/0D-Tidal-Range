#ifndef General
#define General
#include <string>
#include <vector>
#include <cmath>
#include "TypesDef.h"
using namespace std;

// Definition of maths and other fundamental functions/classess
// that are used in the genetic tidal lagon model but could be used
// elsewhere

//Maths
const double Pi = 3.14159265359;    // for circles etc.
const double g = 9.807;             // Gravity

//constant absolute values for ints and doubles
double absolute(const double& input) {
    double output = input; 
    if (input < 0) {
        output = -1 * input;
    }
    return output;
}
int absolute(const int& input) {
    int output = input;
    if (input < 0) {
        output = -1 * input;
    }
    return output;
}
// takes three sets of xyz points and finds the area in the xy plane
double areaTriangle(const tdouble3& A, const tdouble3& B, const tdouble3& C) {
    //https://www.mathopenref.com/coordtrianglearea.html
    double area, calc1, calc2, calc3;
    calc1 = A.x * (B.y - C.y);
    calc2 = B.x * (C.y - A.y);
    calc3 = C.x * (A.y - B.y);
    area = abs((calc1 + calc2 + calc3) / 2);
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
        return (double)dischargeCoeff * area * sqrt(2 * g * absolute(headDifference)*-1);
    }
}
// simple trapezium rule for areas and volumes
double trapezium(const double& area1, const double& area2, const double& dh) {    return (dh * (area1 + area2) / 2);    }

// interpolate to find y2 from (x1,y1), (x3,y3) and x2.
double interpolate(const double& x1, const double& x2, const double& x3, const double& y1, const double& y3) {
    return y1 + (x2 - x1) * (y3 - y1) / (x3 - x1);
    //https://www.ajdesigner.com/phpinterpolation/linear_interpolation_equation.php
}
double interpolate(const int& x1, const int& x2, const int& x3, const int& y1, const int& y3) {
    return (double)y1 + ((double)x2 - (double)x1) * ((double)y3 - (double)y1) / ((double)x3 - (double)x1);
}

// Following structs taken from DualSPHysics typesDef.h
///Structure of 2 variables of type int.
#endif
