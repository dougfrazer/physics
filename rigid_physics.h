#include "physics.h"
#include "vector.h"
#include <vector>

class RIGID_PHYSICS : public PHYSICS
{
public:
    RIGID_PHYSICS( GEOMETRY* Geometry );
    ~RIGID_PHYSICS();

public:
    void Update( float DeltaTime );
    void Reset( );
protected:
    //******************************************************************************
    // Collision Detection using GJK
    //*****************************************************************************
    bool DetectCollision( GEOMETRY* Incoming );
    GEOMETRY* HandleCollision( GEOMETRY* Incoming );

protected:
    vertex*  x;
    vector   v;

private:
    //******************************************************************************
    // Support:
    // --------
    //   This will attempt to find the closest point in the geometry to the vector
    //   passed in.
    //******************************************************************************
    vector Support( const vector d, const GEOMETRY* Geo );
    vector Support( const vector d, const GEOMETRY* A, const GEOMETRY* B );

    //******************************************************************************
    // Simplex:
    // --------
    //   This function will take in a list of points, and a direction.  It knows the last
    //   point was a point that was just added.  We will test to see if that point is part of
    //   a tetrahedron surrounding the origin.  If it is not, we discard it, and change the direction to face
    //   the origin.  If it is part of the tetrahedron, we leave it, and update our search direction
    //   towards the origin but in the direction perpendicular to what we just added.
    //
    // Note: this function modifies both the list and the direction.
    //******************************************************************************
    bool   Simplex( std::vector<vector>* list, vector* d );
};
