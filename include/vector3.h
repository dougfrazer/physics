#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include "vector4.h"
#include "common.h"
#include "math.h"

//******************************************************************************
// Vector - Three-Dimensional Vector
//******************************************************************************
class vector3 {
public:
    float x;
    float y;
    float z;

public:
    vector3();
    vector3(const vertex v);
	vector3(const vector4 v);
    vector3(const float _x, const float _y, const float _z);

    vector3 operator-() const;
    vector3 operator-( const vector3 B ) const;
    vector3 operator+( const vector3 B ) const;
    vector3 operator*( const float s )   const;
	vector3 operator/( const float s )   const;

    float    dot  ( const vector3 B ) const;
    vector3  cross( const vector3 B ) const;
    vector3  proj ( const vector3 B ) const;
    float    magnitude( void )        const;
    vector3  normalize( void )        const;
    
    vector3  rotate( vector3 axis, float degrees ) const;
};

#endif
