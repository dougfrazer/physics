#include "matrix.h"
#include "common.h" // for assert

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
    x1 = 1.0; x2 = 0.0; x3 = 0.0;
    y1 = 0.0; y2 = 1.0; y3 = 0.0;
    z1 = 0.0; z2 = 0.0; z3 = 1.0;
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
    v.z = z1*B.x + z2*B.y + z3*B.z; 
    return v;
}

matrix3 matrix3::t() const {
    matrix3 m( x1, y1, z1,
               x2, y2, z2,
               x3, y3, z3 );
    return m;
}

matrix3 matrix3::inv() const {
    matrix3 m;
    float d = det();
    m.x1 = matrix2( y2, y3, z2, z3 ).det() / d;
    m.x2 = matrix2( x3, x2, z3, z2 ).det() / d;
    m.x3 = matrix2( x2, x3, y2, y3 ).det() / d;
    m.y1 = matrix2( y3, y1, z3, z1 ).det() / d;
    m.y2 = matrix2( x1, x3, z1, z3 ).det() / d;
    m.y3 = matrix2( x3, x1, y3, y1 ).det() / d;
    m.z1 = matrix2( y1, y2, z1, z2 ).det() / d;
    m.z2 = matrix2( x2, x1, z2, z1 ).det() / d;
    m.z3 = matrix2( x1, x2, y1, y2 ).det() / d;

    return m;
}

float matrix3::det() const {
    return x1*y2*z3 - x1*y3*z2 + x2*y3*z1 - x2*y1*z3 + x3*y1*z2 - x3*y2*z1;
}

//******************************************************************************
// 4x4 matrix
//******************************************************************************
matrix4::matrix4()
{ 
	x1 = 1.0; x2 = 0.0; x3 = 0.0; x4 = 0.0;
	y1 = 0.0; y2 = 1.0; y3 = 0.0; y4 = 0.0;
	z1 = 0.0; z2 = 0.0; z3 = 1.0; z4 = 0.0;
	w1 = 0.0; w2 = 0.0; w3 = 0.0; w4 = 1.0;
}
matrix4::matrix4(float a1, float a2, float a3, float a4,
				 float b1, float b2, float b3, float b4,
				 float c1, float c2, float c3, float c4,
				 float d1, float d2, float d3, float d4 )
{ 
	x1 = a1; x2 = a2; x3 = a3; x4 = a4;
	y1 = b1; y2 = b2; y3 = b3; y4 = b4;
	z1 = c1; z2 = c2; z3 = c3; z4 = c4;
	w1 = d1; w2 = d2; w3 = d3; w4 = d4;
}

vector4 matrix4::operator*( const vector4 B ) const { 
	vector4 v; 
	v.x = x1*B.x + x2*B.y + x3*B.z + x4*B.w;
	v.y = y1*B.x + y2*B.y + y3*B.z + y4*B.w;
	v.z = z1*B.x + z2*B.y + z3*B.z + z4*B.w;
	v.w = w1*B.x + w2*B.y + w3*B.z + w4*B.w; 
	return v;
}

matrix4 matrix4::t() const {
	matrix4 m( x1, y1, z1, w1,
		       x2, y2, z2, w2,
		       x3, y3, z3, w3,
			   x4, y4, z4, w4 );
	return m;
}

matrix4 matrix4::inv() const {
	matrix4 m;
	assert(false);
	/*
	float d = det();
	m.x1 = matrix3( y2, y3, z2, z3 ).det() / d;
	m.x2 = matrix3( x3, x2, z3, z2 ).det() / d;
	m.x3 = matrix3( x2, x3, y2, y3 ).det() / d;
	m.y1 = matrix3( y3, y1, z3, z1 ).det() / d;
	m.y2 = matrix3( x1, x3, z1, z3 ).det() / d;
	m.y3 = matrix3( x3, x1, y3, y1 ).det() / d;
	m.z1 = matrix3( y1, y2, z1, z2 ).det() / d;
	m.z2 = matrix3( x2, x1, z2, z1 ).det() / d;
	m.z3 = matrix3( x1, x2, y1, y2 ).det() / d;
	*/

	return m;
}

void matrix4::translate( vector3 t ) {
	x4 = t.x;
	y4 = t.y;
	z4 = t.z;
	w4 = 1;
}

void matrix4::rotate( vector3 r ) {
	vector3 rad = r * (3.14159265/180);
	x1 = cos(rad.y)*cos(rad.z);
	x2 = cos(rad.y)*sin(rad.z);
	x3 = sin(rad.y);
	y1 = -sin(rad.x)*sin(rad.y)*cos(rad.z) - cos(rad.x)*sin(rad.z);
	y2 = -sin(rad.x)*sin(rad.y)*sin(rad.z) + cos(rad.x)*cos(rad.z);
	y3 = sin(rad.x)*cos(rad.y);
	z1 = -cos(rad.x)*sin(rad.y)*cos(rad.z) + sin(rad.x)*sin(rad.z);
	z2 = -cos(rad.x)*sin(rad.y)*cos(rad.z) + sin(rad.x)*cos(rad.z);
	z3 = cos(rad.x)*cos(rad.y);
}

void matrix4::scale( float s ) {
	x1 = x1*s;
	y2 = y2*s;
	z3 = z3*s;
}

float matrix4::det() const {
	return x1*y2*z3*w4 + x1*y3*z4*w2 + x1*y4*z2*w3
		 + x2*y1*z4*w3 + x2*y3*z1*w4 + x2*y4*z3*w1
		 + x3*y1*z2*w4 + x3*y2*z4*w1 + x3*y4*z1*w2
		 + x4*y1*z3*w2 + x4*y2*z1*w3 + x4*y3*z2*w1
		 - x1*y2*z4*w3 - x1*y3*z2*w4 - x1*y4*z3*w2
		 - x2*y1*z3*w4 - x2*y3*z4*w1 - x2*y4*z1*w3
		 - x3*y1*z4*w2 - x3*y2*z1*w4 - x3*y4*z2*w1
		 - x4*y1*z2*w3 - x4*y2*z3*w1 - x4*y3*z1*w2 ;
}


//******************************************************************************
// Specific types of matrices
//******************************************************************************
skew_symmetric_matrix3::skew_symmetric_matrix3( const vector3 A ) 
    : matrix3( 0.0, -A.z, A.y, A.z, 0.0, -A.x, -A.y, A.x, 0.0 )
{
}

