#ifndef General
#define General
#include <string>
#include <vector>
#include <cmath>
using namespace std;

// Definition of maths and other fundamental functions/classess
// that are used in the genetic tidal lagon model but could be used
// elsewhere

//Maths

const double Pi = 3.14159265359;
const double g = 9.807;

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
double areaTriangle(const tdouble3& A, const tdouble3& B, const tdouble3& C) {
    //https://www.mathopenref.com/coordtrianglearea.html
    double area, calc1, calc2, calc3;
    calc1 = A.x * (B.y - C.y);
    calc2 = B.x * (C.y - A.y);
    calc3 = C.x * (A.y - B.y);
    area = abs((calc1 + calc2 + calc3) / 2);
    return area;
}
double meanHeight(const tdouble3& A, const tdouble3& B, const tdouble3& C) {
    return (A.z + B.z + C.z) / 3; 
}
double orificeFlow(const double& dischargeCoeff, const double& headDifference, const double& area) {
    return (double)dischargeCoeff * area * sqrt(2 * g * headDifference);
}

double interpolate(const double& x1, const double& x2, const double& x3, const double& y1, const double& y3) {
    return y1 + (x2 - x1) * (y3 - y1) / (x3 - x1);
    //https://www.ajdesigner.com/phpinterpolation/linear_interpolation_equation.php
}
double interpolate(const int& x1, const int& x2, const int& x3, const int& y1, const int& y3) {
    return (double)y1 + ((double)x2 - (double)x1) * ((double)y3 - (double)y1) / ((double)x3 - (double)x1);
}

// Following structs taken from DualSPHysics typesDef.h
///Structure of 2 variables of type int.
typedef struct {
    int x, y;
}tint2;


inline tint2 TInt2(int v) { tint2 p = { v,v }; return(p); }
inline tint2 TInt2(int x, int y) { tint2 p = { x,y }; return(p); }
inline bool   operator ==(const tint2& a, const tint2& b) { return(a.x == b.x && a.y == b.y); }
inline bool   operator !=(const tint2& a, const tint2& b) { return(a.x != b.x || a.y != b.y); }
inline bool   operator  <(const tint2& a, const tint2& b) { return(a.x < b.x && a.y < b.y); }
inline bool   operator  >(const tint2& a, const tint2& b) { return(a.x > b.x&& a.y > b.y); }
inline bool   operator <=(const tint2& a, const tint2& b) { return(a.x <= b.x && a.y <= b.y); }
inline bool   operator >=(const tint2& a, const tint2& b) { return(a.x >= b.x && a.y >= b.y); }
inline tint2 operator  +(const tint2& a, const tint2& b) { return(TInt2(a.x + b.x, a.y + b.y)); }
inline tint2 operator  -(const tint2& a, const tint2& b) { return(TInt2(a.x - b.x, a.y - b.y)); }
inline tint2 operator  *(const tint2& a, const tint2& b) { return(TInt2(a.x * b.x, a.y * b.y)); }
inline tint2 operator  /(const tint2& a, const tint2& b) { return(TInt2(a.x / b.x, a.y / b.y)); }
inline tint2 operator  +(const tint2& a, const int& b) { return(TInt2(a.x + b, a.y + b)); }
inline tint2 operator  -(const tint2& a, const int& b) { return(TInt2(a.x - b, a.y - b)); }
inline tint2 operator  *(const tint2& a, const int& b) { return(TInt2(a.x * b, a.y * b)); }
inline tint2 operator  /(const tint2& a, const int& b) { return(TInt2(a.x / b, a.y / b)); }
inline tint2 MinValues(const tint2& a, const tint2& b) { return(TInt2((a.x <= b.x ? a.x : b.x), (a.y <= b.y ? a.y : b.y))); }
inline tint2 MaxValues(const tint2& a, const tint2& b) { return(TInt2((a.x >= b.x ? a.x : b.x), (a.y >= b.y ? a.y : b.y))); }
inline int TInt2Get(const tint2& a, int c) { return(!c ? a.x : a.y); }
inline tint2 TInt2Set(const tint2& a, int c, int v) { return(TInt2((c ? a.x : v), (c != 1 ? a.y : v))); }


///Structure of 3 variables of type int.
typedef struct {
    int x, y, z;
}tint3;

inline tint3 TInt3(int v) { tint3 p = { v,v,v }; return(p); }
inline tint3 TInt3(int x, int y, int z) { tint3 p = { x,y,z }; return(p); }
inline bool   operator ==(const tint3& a, const tint3& b) { return(a.x == b.x && a.y == b.y && a.z == b.z); }
inline bool   operator !=(const tint3& a, const tint3& b) { return(a.x != b.x || a.y != b.y || a.z != b.z); }
inline bool   operator  <(const tint3& a, const tint3& b) { return(a.x < b.x && a.y < b.y && a.z < b.z); }
inline bool   operator  >(const tint3& a, const tint3& b) { return(a.x > b.x&& a.y > b.y&& a.z > b.z); }
inline bool   operator <=(const tint3& a, const tint3& b) { return(a.x <= b.x && a.y <= b.y && a.z <= b.z); }
inline bool   operator >=(const tint3& a, const tint3& b) { return(a.x >= b.x && a.y >= b.y && a.z >= b.z); }
inline tint3 operator  +(const tint3& a, const tint3& b) { return(TInt3(a.x + b.x, a.y + b.y, a.z + b.z)); }
inline tint3 operator  -(const tint3& a, const tint3& b) { return(TInt3(a.x - b.x, a.y - b.y, a.z - b.z)); }
inline tint3 operator  *(const tint3& a, const tint3& b) { return(TInt3(a.x * b.x, a.y * b.y, a.z * b.z)); }
inline tint3 operator  /(const tint3& a, const tint3& b) { return(TInt3(a.x / b.x, a.y / b.y, a.z / b.z)); }
inline tint3 operator  +(const tint3& a, const int& b) { return(TInt3(a.x + b, a.y + b, a.z + b)); }
inline tint3 operator  -(const tint3& a, const int& b) { return(TInt3(a.x - b, a.y - b, a.z - b)); }
inline tint3 operator  *(const tint3& a, const int& b) { return(TInt3(a.x * b, a.y * b, a.z * b)); }
inline tint3 operator  /(const tint3& a, const int& b) { return(TInt3(a.x / b, a.y / b, a.z / b)); }
inline tint3 MinValues(const tint3& a, const tint3& b) { return(TInt3((a.x <= b.x ? a.x : b.x), (a.y <= b.y ? a.y : b.y), (a.z <= b.z ? a.z : b.z))); }
inline tint3 MaxValues(const tint3& a, const tint3& b) { return(TInt3((a.x >= b.x ? a.x : b.x), (a.y >= b.y ? a.y : b.y), (a.z >= b.z ? a.z : b.z))); }
inline int TInt3Get(const tint3& a, int c) { return(!c ? a.x : (c == 1 ? a.y : a.z)); }
inline tint3 TInt3Set(const tint3& a, int c, int v) { return(TInt3((c ? a.x : v), (c != 1 ? a.y : v), (c != 2 ? a.z : v))); }

///Structure of 2 variables of type double.
typedef struct {
    double x, y;
}tdouble2;

inline tdouble2 TDouble2(double v) { tdouble2 p = { v,v }; return(p); }
inline tdouble2 TDouble2(double x, double y) { tdouble2 p = { x,y }; return(p); }
inline bool operator ==(const tdouble2& a, const tdouble2& b) { return(a.x == b.x && a.y == b.y); }
inline bool operator !=(const tdouble2& a, const tdouble2& b) { return(a.x != b.x || a.y != b.y); }
inline tdouble2 operator +(const tdouble2& a, const tdouble2& b) { return(TDouble2(a.x + b.x, a.y + b.y)); }
inline tdouble2 operator -(const tdouble2& a, const tdouble2& b) { return(TDouble2(a.x - b.x, a.y - b.y)); }
inline tdouble2 operator *(const tdouble2& a, const tdouble2& b) { return(TDouble2(a.x * b.x, a.y * b.y)); }
inline tdouble2 operator /(const tdouble2& a, const tdouble2& b) { return(TDouble2(a.x / b.x, a.y / b.y)); }
inline tdouble2 operator +(const tdouble2& a, const double& b) { return(TDouble2(a.x + b, a.y + b)); }
inline tdouble2 operator -(const tdouble2& a, const double& b) { return(TDouble2(a.x - b, a.y - b)); }
inline tdouble2 operator *(const tdouble2& a, const double& b) { return(TDouble2(a.x * b, a.y * b)); }
inline tdouble2 operator /(const tdouble2& a, const double& b) { return(TDouble2(a.x / b, a.y / b)); }
inline tdouble2 MinValues(const tdouble2& a, const tdouble2& b) { return(TDouble2((a.x <= b.x ? a.x : b.x), (a.y <= b.y ? a.y : b.y))); }
inline tdouble2 MaxValues(const tdouble2& a, const tdouble2& b) { return(TDouble2((a.x >= b.x ? a.x : b.x), (a.y >= b.y ? a.y : b.y))); }


///Structure of 3 variables of type double.
typedef struct {
    double x, y, z;
}tdouble3;

inline tdouble3 TDouble3(double v) { tdouble3 p = { v,v,v }; return(p); }
inline tdouble3 TDouble3(double x, double y, double z) { tdouble3 p = { x,y,z }; return(p); }
inline bool operator ==(const tdouble3& a, const tdouble3& b) { return(a.x == b.x && a.y == b.y && a.z == b.z); }
inline bool operator !=(const tdouble3& a, const tdouble3& b) { return(a.x != b.x || a.y != b.y || a.z != b.z); }
inline bool operator <(const tdouble3& a, const tdouble3& b) { return(a.x < b.x && a.y < b.y && a.z < b.z); }
inline bool operator >(const tdouble3& a, const tdouble3& b) { return(a.x > b.x&& a.y > b.y&& a.z > b.z); }
inline bool operator <=(const tdouble3& a, const tdouble3& b) { return(a.x <= b.x && a.y <= b.y && a.z <= b.z); }
inline bool operator >=(const tdouble3& a, const tdouble3& b) { return(a.x >= b.x && a.y >= b.y && a.z >= b.z); }
inline tdouble3 operator +(const tdouble3& a, const tdouble3& b) { return(TDouble3(a.x + b.x, a.y + b.y, a.z + b.z)); }
inline tdouble3 operator -(const tdouble3& a, const tdouble3& b) { return(TDouble3(a.x - b.x, a.y - b.y, a.z - b.z)); }
inline tdouble3 operator *(const tdouble3& a, const tdouble3& b) { return(TDouble3(a.x * b.x, a.y * b.y, a.z * b.z)); }
inline tdouble3 operator /(const tdouble3& a, const tdouble3& b) { return(TDouble3(a.x / b.x, a.y / b.y, a.z / b.z)); }
inline tdouble3 operator +(const tdouble3& a, const double& b) { return(TDouble3(a.x + b, a.y + b, a.z + b)); }
inline tdouble3 operator -(const tdouble3& a, const double& b) { return(TDouble3(a.x - b, a.y - b, a.z - b)); }
inline tdouble3 operator *(const tdouble3& a, const double& b) { return(TDouble3(a.x * b, a.y * b, a.z * b)); }
inline tdouble3 operator /(const tdouble3& a, const double& b) { return(TDouble3(a.x / b, a.y / b, a.z / b)); }
inline tdouble3 MinValues(const tdouble3& a, const tdouble3& b) { return(TDouble3((a.x <= b.x ? a.x : b.x), (a.y <= b.y ? a.y : b.y), (a.z <= b.z ? a.z : b.z))); }
inline tdouble3 MaxValues(const tdouble3& a, const tdouble3& b) { return(TDouble3((a.x >= b.x ? a.x : b.x), (a.y >= b.y ? a.y : b.y), (a.z >= b.z ? a.z : b.z))); }


#endif
