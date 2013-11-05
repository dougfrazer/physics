#ifndef __COMMON_H__
#define __COMMON_H__

#include "assert.h"
#include "math.h"

//******************************************************************************
// Vertex - A single point
//******************************************************************************
typedef struct {
    float x;
    float y;
    float z;
} vertex;

//******************************************************************************
// Face - A series of vertices
//******************************************************************************
typedef struct {
    int v1;
    int v2;
    int v3;
    int v4;
} face;

//******************************************************************************
// Vector4 - A four-dimensional vector storage class
//******************************************************************************
class vector4 {
public:
    float x;
    float y;
    float z;
    float w;
};

#define LinearInterpolate(x, x0, x1, y0, y1) ( y0+ ( (float)(y1-y0) * ( (float)(x-x0)/(float)(x1-x0) ) ) )
#define clamp( min, x, max ) x < min ? min : x > max ? max : x
//#define min(a,b) a < b ? a : b
//#define max(a,b) a > b ? a : b
#endif
