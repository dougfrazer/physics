#include "vector4.h"
#include "matrix.h"

vector4::vector4() { 
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 1.0;
}
vector4::vector4(const vertex v) { 
	x = v.x;
	y = v.y;
	z = v.z;
	w = 1.0;
}
vector4::vector4(const float _x, const float _y, const float _z, float _w) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

vector4 vector4::operator-() const { 
	vector4 v; 
	v.x = -x;
	v.y = -y;
	v.z = -z;
	v.w = -w;
	return v;
}
vector4 vector4::operator-( const vector4 B ) const { 
	vector4 v; 
	v.x = x - B.x; 
	v.y = y - B.y; 
	v.z = z - B.z; 
	v.w = w - B.w;
	return v;
}
vector4 vector4::operator+( const vector4 B ) const {
	vector4 v;
	v.x = x + B.x;
	v.y = y + B.y;
	v.z = z + B.z;
	v.w = z + B.w;
	return v;
}
vector4 vector4::operator*( const float s ) const {
	vector4 v;
	v.x = x * s;
	v.y = y * s;
	v.z = z * s;
	v.w = w * s;
	return v;
}

float vector4::dot( const vector4 B ) const { 
	return x*B.x + y*B.y + z*B.z + w*B.w;
}
vector4 vector4::cross( const vector4 B ) const {
	vector4 v;
	return v;
	//	return skew_symmetric_matrix3( *this ) * B;
}
float vector4::magnitude() const { 
	return sqrt( x*x + y*y + z*z + w*w );
}
vector4 vector4::normalize() const {
	vector4 v;
	float m = magnitude();
	v.x = x / m;
	v.y = y / m;
	v.z = z / m;
	v.w = w / m;
	return v;
}
vector4 vector4::proj( const vector4 B ) const {
	float B_mag = B.magnitude();
	vector4 v = B * ( dot(B) / (B_mag*B_mag) );
	return v;
}
