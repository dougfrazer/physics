#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vector3.h"
#include "vector4.h"

class matrix2 {
public:
    float x1, x2;
    float y1, y2;

public:    
    matrix2();
    matrix2( float a1, float a2,
             float b1, float b2 );
    float det() const;
};

//******************************************************************************
// Matrix3 - Three-Dimensional Matrix
//******************************************************************************
class matrix3 {
public:
    float x1, x2, x3;
    float y1, y2, y3;
    float z1, z2, z3;

public:
    matrix3();
    matrix3(float a1, float a2, float a3,
            float b1, float b2, float b3,
            float c1, float c2, float c3);

    vector3 operator*( const vector3 B ) const;
    
    // inverse
    matrix3 inv() const;
    
    // determinent
    float   det() const;

    // transpose
    matrix3 t() const;
};

//******************************************************************************
// skew_symmetric_matrix4 - Skew-Symmetric Three-Dimensional Matrix
//******************************************************************************
class matrix4 {
public:
	float x1, x2, x3, x4;
	float y1, y2, y3, y4;
	float z1, z2, z3, z4;
	float w1, w2, w3, w4;

public:
	matrix4();
	matrix4(float a1, float a2, float a3, float a4,
		    float b1, float b2, float b3, float b4,
		    float c1, float c2, float c3, float c4,
			float d1, float d2, float d3, float d4 );

	vector4 operator*( const vector4 B ) const;

	// inverse
	matrix4 inv() const;

	// determinent
	float   det() const;

	// transpose
	matrix4 t() const;

	void rotate( vector3 r );
	void translate( vector3 t );
	void scale( float s );
};


//******************************************************************************
// skew_symmetric_matrix3 - Skew-Symmetric Three-Dimensional Matrix
//******************************************************************************
class skew_symmetric_matrix3 : public matrix3 {
public:
    skew_symmetric_matrix3( const vector3 A ); 
};

#endif
