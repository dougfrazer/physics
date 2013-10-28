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
    // Collision Detection:
    // -------------------
    //   This implementation of collision detection uses the GJK algorithm.
    //
    //   The core concept here is that we can detect if two convex shapes intersect
    //   if within the shape, there are two points that when you subtract them equal zero.
    //
    //   That is the same thing as saying that if we construct the shape Ai - Bj (all
    //   points in A minus all points in B, also known as the Minkowski Difference) and 
    //   we can select 4 points which enclose the origin.
    //
    //   We start by selecting a random point in the Minkowski Difference, seeing if we 
    //   can reach the origin, if we can not we are done, if we can, we try and find 
    //   another point to construct our tetrahedron surrounding the origin, and then a third, 
    //   and then a fourth.  If we can find all 4 points, we have a collision, otherwise we do not.
    //
    //*****************************************************************************
    bool DetectCollision( GEOMETRY* Incoming );
    void HandleCollision( GEOMETRY* Incoming );

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
    bool Simplex( std::vector<vector>* list, vector* d );
};
