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

static inline float LinearInterpolate(float x, float x0, float x1, float y0, float y1)    { return y0 + ( (float)(y1-y0) * ( (x-x0)/(x1-x0) ) ); }
static inline float min  ( float a, float b )                                             { return a < b ? a : b; }
static inline float max  ( float a, float b )                                             { return a > b ? a : b; }
static inline float clamp( float low, float x, float high )                               { return min( high, max( x, low ) ); }
static const float PI = 3.14159265359;
#endif
