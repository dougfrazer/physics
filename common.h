#ifndef __COMMON_H__
#define __COMMON_H__

// A vertex is constructed of several points
typedef struct {
    float x;
    float y;
    float z;
} vertex;

// A face is constructed of several vertices
typedef struct {
    int v1;
    int v2;
    int v3;
    int v4;
} face;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vector4;

#define LinearInterpolate(x, x0, x1, y0, y1) ( y0+ ( (float)(y1-y0) * ( (float)(x-x0)/(float)(x1-x0) ) ) )

#endif
