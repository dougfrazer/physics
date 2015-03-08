#include "common.h"
#include "vector3.h"

class GEOMETRY
{
public:
    int       NumVertices;
    vertex*   VertexList;

    int       NumFaces;
    face*     FaceList;

    int       NumTriangleVerts;
    vertex*   TriangleVertList;

    // TODO: take this out of the geometry - this is not raw data
    vector3   Position;
    vector3   Rotation;
    vector3   p; // linear momentum
    vector3   L; // angular momentum

public:
    GEOMETRY() { }
    GEOMETRY( GEOMETRY* a ) { *this = *a; }
};
