#include "vector.h"
#include "matrix.h"

vector::vector() { 
    x = 0.0;
    y = 0.0;
    z = 0.0;
}
vector::vector(const vertex v) { 
    x = v.x;
    y = v.y;
    z = v.z;
}
vector::vector(const float _x, const float _y, const float _z) {
    x = _x;
    y = _y;
    z = _z;
}

vector vector::operator-() const { 
    vector v; 
    v.x = -x;
    v.y = -y;
    v.z = -z;
    return v;
}
vector vector::operator-( const vector B ) const { 
    vector v; 
    v.x = x - B.x; 
    v.y = y - B.y; 
    v.z = z - B.z; 
    return v;
}
vector vector::operator+( const vector B ) const {
    vector v;
    v.x = x + B.x;
    v.y = y + B.y;
    v.z = z + B.z;
    return v;
}
vector vector::operator*( const float s ) const {
    vector v;
    v.x = x * s;
    v.y = y * s;
    v.z = z * s;
    return v;
}

float vector::dot( const vector B ) const { 
    return x*B.x + y*B.y + z*B.z;
}
vector vector::cross( const vector B ) const {
    return skew_symmetric_matrix3( *this ) * B;
}
float vector::magnitude() const { 
    return sqrt( x*x + y*y + z*z );
}
