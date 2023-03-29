#include "math.h"

// -----------------------------------------------------------
// UTILITY FUNCTIONS
// -----------------------------------------------------------

f64 math_abs(f64 x) {
    return x < 0 ? -x : x;
}

f64 math_degrees(f64 radians) {
    return 180.0 / PI * radians;
}

f64 math_radians(f64 degrees) {
    return PI / 180.0 * degrees;
}

f64 math_fraction(f64 x) {
    return x - floor(x);
}

f64 math_mod(f64 a, f64 b) {
    return b * math_fraction(a / b);
}

f64 math_sin(f64 angle) {
    return sin(math_radians(angle));
}

f64 math_cos(f64 angle) {
    return cos(math_radians(angle));
}

f64 math_tan(f64 angle) {
    return tan(math_radians(angle));
}

f64 math_asin(f64 x) {
    return math_degrees(asin(x));
}

f64 math_acos(f64 x) {
    return math_degrees(acos(x));
}

f64 math_atan(f64 x) {
    return math_degrees(atan(x));
}

f64 math_atan2(f64 y, f64 x) {
    return math_degrees(atan2(y, x));
}

f64 math_daa_to_degrees(f64 degs, f64 arcmins, f64 arcsecs) {
    f64 result = abs(degs) + abs(arcmins) / 60.0 + abs(arcsecs) / 3600.0;
    return degs < 0.0 ? -result : result;
}

f64 math_hms_to_degrees(f64 hours, f64 minutes, f64 seconds) {
    return 15.0 * (hours + minutes / 60.0 + seconds / 3600.0);
}

// -----------------------------------------------------------
// VECTORS AND MATRICES
// -----------------------------------------------------------

f64 vector3_length(Vector3 vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

Vector3 vector3_add(Vector3 a, Vector3 b) {
    Vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Vector3 vector3_sub(Vector3 a, Vector3 b) {
    Vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

Matrix3x3 matrix3x3_create(f64 diagonal) {
    Matrix3x3 result;
    result.e[0][0] = diagonal;
    result.e[1][1] = diagonal;
    result.e[2][2] = diagonal;
    return result;
}

Matrix3x3 matrix3x3_transpose(Matrix3x3* matrix) {
    Matrix3x3 result;
    result.e[0][0] = matrix->e[0][0];
    result.e[0][1] = matrix->e[1][0];
    result.e[0][2] = matrix->e[2][0];
    result.e[1][0] = matrix->e[0][1];
    result.e[1][1] = matrix->e[1][1];
    result.e[1][2] = matrix->e[2][1];
    result.e[2][0] = matrix->e[0][2];
    result.e[2][1] = matrix->e[1][2];
    result.e[2][2] = matrix->e[2][2];
    return result;
}

Matrix3x3 matrix3x3_mult(Matrix3x3* left, Matrix3x3* right) {
    Matrix3x3 result;
    result.e[0][0] = left->e[0][0] * right->e[0][0] + left->e[0][1] * right->e[1][0] + left->e[0][2] * right->e[2][0];
    result.e[0][1] = left->e[0][0] * right->e[0][1] + left->e[0][1] * right->e[1][1] + left->e[0][2] * right->e[2][1];
    result.e[0][2] = left->e[0][0] * right->e[0][2] + left->e[0][1] * right->e[1][2] + left->e[0][2] * right->e[2][2];
    
    result.e[1][0] = left->e[1][0] * right->e[0][0] + left->e[1][1] * right->e[1][0] + left->e[1][2] * right->e[2][0];
    result.e[1][1] = left->e[1][0] * right->e[0][1] + left->e[1][1] * right->e[1][1] + left->e[1][2] * right->e[2][1];
    result.e[1][2] = left->e[1][0] * right->e[0][2] + left->e[1][1] * right->e[1][2] + left->e[1][2] * right->e[2][2];
    
    result.e[2][0] = left->e[2][0] * right->e[0][0] + left->e[2][1] * right->e[1][0] + left->e[2][2] * right->e[2][0];
    result.e[2][1] = left->e[2][0] * right->e[0][1] + left->e[2][1] * right->e[1][1] + left->e[2][2] * right->e[2][1];
    result.e[2][2] = left->e[2][0] * right->e[0][2] + left->e[2][1] * right->e[1][2] + left->e[2][2] * right->e[2][2];
    return result;
}

Vector3 matrix3x3_mult(Matrix3x3* left, Vector3 right) {
    Vector3 result;
    result.x = left->e[0][0] * right.x + left->e[0][1] * right.y + left->e[0][2] * right.z; 
    result.y = left->e[1][0] * right.x + left->e[1][1] * right.y + left->e[1][2] * right.z;
    result.z = left->e[2][0] * right.x + left->e[2][1] * right.y + left->e[2][2] * right.z;
    return result;
}
