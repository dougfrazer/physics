#include "physics.h"
#include "vector3.h"
#include "matrix.h"
#include <vector>

class RIGID_PHYSICS : public PHYSICS
{
public:
    RIGID_PHYSICS( GEOMETRY* Geometry );
    ~RIGID_PHYSICS();

public:
    void Update( const float DeltaTime );
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
    bool DetectCollision( const GEOMETRY* Incoming );
    void HandleCollision( const GEOMETRY* Incoming, const float DeltaTime );

// TODO: private
public:
    vertex*   x; // position (center of mass)
    vector3   v; // velocity
    vector3   w; // angular veclocity

private:
    //******************************************************************************
    // Support:
    // --------
    //   This will attempt to find the closest point in the geometry to the vector
    //   passed in.
    //******************************************************************************
    vector3 Support( const vector3 d, const GEOMETRY* Geo );
    vector3 Support( const vector3 d, const GEOMETRY* Geo, const matrix4* m );
    vector3 Support( const vector3 d, const GEOMETRY* A, const GEOMETRY* B );

    //******************************************************************************
    // Simplex:
    // --------
    //   This function will take in a list of points, and a direction.  It knows the last
    //   point was a point that was just added.  We will test to see if that point is part of
    //   a tetrahedron surrounding the origin.  If it is not, we discard it, and change the direction to face
    //   the origin.  If it is part of the tetrahedron, we leave it, and update our search direction
    //   towards the origin but in the direction perpendicular to what we just added.
    //
	// Note: this function will assume the last point in the list be the point we just added
    // Note: this function modifies both the list and the direction.
    //******************************************************************************
    bool Simplex( std::vector<vector3>* list, vector3* d, const vector3 dest );
    
    //******************************************************************************
    // Impulse Response
    // ----------------
    //   Simplified impulse response equation (assuming the plane we're colliding with
    //   has infinite mass and zero velocity)
    //
    //   v_new = v_old + j*n
    //
    //   where:
    //                -(1 + e)v dot n
    //   j =  --------------------------------------
    //        1/m +  ( (I^-1 * (r x n)) x r) ) dot n
    //
    //   j = magnitude of impulse
    //   e = elasticity (0.0 - 1.0)
    //   v = current velocity
    //   n = normal to the plane of impact
    //   m = mass
    //   r = vector to point of impact from center of mass
    //   I = moment of inertia
    //******************************************************************************
    void ApplyImpulseResponse( const matrix3 I_inv, const float m_inv, const vector3 r, const vector3 n, vector3* v, vector3* w );


    //******************************************************************************
    // Bilateral Advancement
    // ---------------------
    //   This is an algorithm from Erin Catto to attempt to find the time of impact.
    //   It will return a value between 0.0 and 1.0 representing the percentage of the 
    //   DeltaTime we should advance that will result in the impact.
    //
    //   Because this is a numerical algorithm, it requires a tolerance (how close
    //   do the objects have to be before we consider it an "impact").
    //******************************************************************************
    float BilateralAdvancement( const GEOMETRY* A, const GEOMETRY* B,
                                const vector3 v_a, const vector3 v_b, 
                                const vector3 w_a, const vector3 w_b, 
                                const vector3 n,
                                const float DeltaTime,
                                const float tolerance );

    //******************************************************************************
    // Utility functions
    //******************************************************************************
    vector3 GetCollisionPoint      ( const GEOMETRY* In );
    vector3 GetCollisionPlaneNormal( const GEOMETRY* In );
    vector3 GetClosestPoint        ( const GEOMETRY* A, const GEOMETRY* B, const matrix4 ta, const matrix4 tb, const vector3 n );
    float   Seperation             ( const GEOMETRY* A, const GEOMETRY* B, 
                                     const vector3 v_a, const vector3 v_b, 
                                     const vector3 w_a, const vector3 w_b, 
                                     const vector3 n,
                                     const float DeltaTime, const float t);
};
