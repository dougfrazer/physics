#include "common.h"
#include "vector3.h"

class GEOMETRY
{
public:
    int       NumVertices;
    vertex*   VertexList;
    int       NumFaces;
    face*     FaceList;
    vector3   Position; // todo: take this out of the geometry - this is not raw data
    vector3   Rotation;

public:
    GEOMETRY() { }
    GEOMETRY( GEOMETRY* a ) { *this = *a; }
};
