#include "vector3.h"
#include "matrix.h"

#define abs(x) x > 0 ? x : -x

vector3::vector3() { 
    x = 0.0;
    y = 0.0;
    z = 0.0;
}
vector3::vector3(const vertex v) { 
    x = v.x;
    y = v.y;
    z = v.z;
}
vector3::vector3(const float _x, const float _y, const float _z) {
    x = _x;
    y = _y;
    z = _z;
}

vector3 vector3::operator-() const { 
    vector3 v; 
    v.x = -x;
    v.y = -y;
    v.z = -z;
    return v;
}
vector3 vector3::operator-( const vector3 B ) const { 
    vector3 v; 
    v.x = x - B.x; 
    v.y = y - B.y; 
    v.z = z - B.z; 
    return v;
}
vector3 vector3::operator+( const vector3 B ) const {
    vector3 v;
    v.x = x + B.x;
    v.y = y + B.y;
    v.z = z + B.z;
    return v;
}
vector3 vector3::operator*( const float s ) const {
    vector3 v;
    v.x = x * s;
    v.y = y * s;
    v.z = z * s;
    return v;
}

float vector3::dot( const vector3 B ) const { 
    return x*B.x + y*B.y + z*B.z;
}
vector3 vector3::cross( const vector3 B ) const {
    return skew_symmetric_matrix3( *this ) * B;
}
float vector3::magnitude() const { 
    return sqrt( x*x + y*y + z*z );
}
vector3 vector3::normalize() const {
    vector3 v;
    float m = magnitude();
    v.x = x / m;
    v.y = y / m;
    v.z = z / m;
    return v;
}
vector3 vector3::proj( const vector3 B ) const {
    float B_mag = B.magnitude();
    vector3 v = B * ( dot(B) / (B_mag*B_mag) );
    return v;
}
