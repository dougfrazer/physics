#ifndef __VECTOR4_H__
#define __VECTOR4_H__

#include "common.h"
#include "math.h"

//******************************************************************************
// Vector - Three-Dimensional Vector
//******************************************************************************
class vector4 {
public:
    float x;
    float y;
    float z;
	float w;

public:
    vector4();
    vector4(const vertex v);
    vector4(const float _x, const float _y, const float _z, float _w);

    vector4 operator-() const;
    vector4 operator-( const vector4 B ) const;
    vector4 operator+( const vector4 B ) const;
    vector4 operator*( const float s )   const;

    float    dot  ( const vector4 B ) const;
    vector4  cross( const vector4 B ) const;
    vector4  proj ( const vector4 B ) const;
    float    magnitude( void )        const;
    vector4  normalize( void )        const;
};

#endif
