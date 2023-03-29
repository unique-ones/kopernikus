/* date = March 29th 2023 9:38 pm */

#ifndef MATH_H
#define MATH_H

#include "types.h"

#include <math.h>

#define PI 3.14159265358979323846
#define PI2 (2.0 * PI)
#define C0 299792458
#define AU 149597870700
#define ARCS (3600.0 * 180.0 / PI)

// -----------------------------------------------------------
// UTILITY FUNCTIONS
// -----------------------------------------------------------

f64 math_abs(f64 x);
f64 math_degrees(f64 radians);
f64 math_radians(f64 degrees);

f64 math_fraction(f64 x);
f64 math_mod(f64 a, f64 b);
f64 math_sin(f64 angle);
f64 math_cos(f64 angle);
f64 math_tan(f64 angle);
f64 math_asin(f64 x);
f64 math_acos(f64 x);
f64 math_atan(f64 x);
f64 math_atan2(f64 y, f64 x);

f64 math_daa_to_degrees(f64 degs, f64 arcmins, f64 arcsecs);
f64 math_hms_to_degrees(f64 hours, f64 minutes, f64 seconds);

// -----------------------------------------------------------
// COORDINATE TYPES
// -----------------------------------------------------------

struct Equatorial {
    f64 right_ascension;
    f64 declination;
    f64 radius;
};

struct Horizontal {
    f64 azimuth;
    f64 altitude;
};

struct Geographic {
    f64 latitude;
    f64 longitude;
};

// -----------------------------------------------------------
// VECTORS AND MATRICES
// -----------------------------------------------------------

struct Vector3 {
    f64 x;
    f64 y;
    f64 z;
};

f64 vector3_length(Vector3 vector);
Vector3 vector3_add(Vector3 a, Vector3 b);
Vector3 vector3_sub(Vector3 a, Vector3 b);

struct Matrix3x3 {
    f64 e[3][3];
};

Matrix3x3 matrix3x3_create(f64 diagonal);
Matrix3x3 matrix3x3_transpose(Matrix3x3* matrix);
Matrix3x3 matrix3x3_mult(Matrix3x3* left, Matrix3x3* right);
Vector3 matrix3x3_mult(Matrix3x3* left, Vector3 right);


#endif //MATH_H
