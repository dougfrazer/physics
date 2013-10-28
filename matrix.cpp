#include "matrix.h"
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

vector matrix3::operator*( const vector B ) const { 
    vector v; 
    v.x = x1*B.x + x2*B.y + x3*B.z;
    v.y = y1*B.x + y2*B.y + y3*B.z;
    v.z = z1*B.x + z2*B.y + y3*B.z; 
    return v;
}

skew_symmetric_matrix3::skew_symmetric_matrix3( const vector A ) 
    : matrix3( 0.0, -A.z, A.y, A.z, 0.0, -A.x, -A.y, A.x, 0.0 )
{
}

