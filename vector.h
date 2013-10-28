#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "common.h"
#include "math.h"

//******************************************************************************
// Vector - Three-Dimensional Vector
//******************************************************************************
class vector {
public:
    float x;
    float y;
    float z;

public:
    vector();
    vector(const vertex v);
    vector(const float _x, const float _y, const float _z);

    vector operator-() const;
    vector operator-( const vector B ) const;
    vector operator+( const vector B ) const;
    vector operator*( const float s ) const;

    float   dot  ( const vector B ) const;
    vector  cross( const vector B ) const;
    float   magnitude()             const;
};

#endif
