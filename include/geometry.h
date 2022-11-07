#pragma once

#include "common.h"
#include "vector3.h"

class GEOMETRY
{
public:
    int       NumVertices;
    vertex*   VertexList;

    int       NumFaces;
    face*     FaceList;

public:
    GEOMETRY() : NumVertices(0)
        , VertexList(NULL)
        , NumFaces(0)
        , FaceList(NULL) { }
    GEOMETRY( GEOMETRY* a ) { *this = *a; }
};
