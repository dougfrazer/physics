#include "matrix.h"

//******************************************************************************
// 2x2 matrix
//******************************************************************************
matrix2::matrix2() { 
    x1 = 0.0; x2 = 0.0;
    y1 = 0.0; y2 = 0.0;
}

matrix2::matrix2( float a1, float a2,
                  float b1, float b2 ) {
    x1 = a1; x2 = a2;
    y1 = b1; y2 = b2;
}

float matrix2::det() const {
    return x1*y2 - x2*y1;
}
//******************************************************************************
// 3x3 matrix
//******************************************************************************
matrix3::matrix3()
{ 
    x1 = 0.0; x2 = 0.0; x3 = 0.0;
    y1 = 0.0; y2 = 0.0; y3 = 0.0;
    z1 = 0.0; z2 = 0.0; z3 = 0.0;
}
matrix3::matrix3(float a1, float a2, float a3,
                 float b1, float b2, float b3,
                 float c1, float c2, float c3)
{ 
    x1 = a1; x2 = a2; x3 = a3; 
    y1 = b1; y2 = b2; y3 = b3; 
    z1 = c1; z2 = c2; z3 = c3;
}

vector3 matrix3::operator*( const vector3 B ) const { 
    vector3 v; 
    v.x = x1*B.x + x2*B.y + x3*B.z;
    v.y = y1*B.x + y2*B.y + y3*B.z;
    v.z = z1*B.x + z2*B.y + y3*B.z; 
    return v;
}

matrix3 matrix3::inv() const {
    matrix3 m;
    float d = det();
   
    m.x1 = d * matrix2( y2, y3, z2, z3 ).det();
    m.x2 = d * matrix2( x3, x2, z3, z2 ).det();
    m.x3 = d * matrix2( x2, x3, y2, y3 ).det();
    m.y1 = d * matrix2( y3, y1, z3, z1 ).det();
    m.y2 = d * matrix2( x1, x3, z1, z3 ).det();
    m.y3 = d * matrix2( x3, x1, z3, z1 ).det();
    m.z1 = d * matrix2( y1, y2, z1, z2 ).det();
    m.z2 = d * matrix2( x2, x1, y2, y1 ).det();
    m.z3 = d * matrix2( x1, x2, y1, y2 ).det();

    return m;
}

float matrix3::det() const {
    return x1*y2*z3 - x1*y3*z2 + x2*y3*z1 + x3*y1*z2 - x3*y2*z1;
}

//******************************************************************************
// Specific types of matrices
//******************************************************************************
skew_symmetric_matrix3::skew_symmetric_matrix3( const vector3 A ) 
    : matrix3( 0.0, -A.z, A.y, A.z, 0.0, -A.x, -A.y, A.x, 0.0 )
{
}

