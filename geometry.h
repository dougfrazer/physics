#include "common.h"

class GEOMETRY
{
public:
    int       NumVertices;
    vertex*   VertexList;
    int       NumFaces;
    face*     FaceList;
    vertex    Position; // todo: take this out of the geometry - this is not raw data

public:
    GEOMETRY() { }
    GEOMETRY( GEOMETRY* a ) { *this = *a; }
};
