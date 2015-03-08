#include "physics.h"
#include "vector3.h"
#include "matrix.h"
#include <vector>


void RigidPhysics_TryUpdate(GEOMETRY* To, const GEOMETRY* From, float DeltaTime);
bool RigidPhysics_DetectCollision(const GEOMETRY* A, const GEOMETRY* B);
void RigidPhysics_ApplyCollisionResponse(GEOMETRY* A, const GEOMETRY* B);

class RIGID_PHYSICS : public PHYSICS
{
public:
    RIGID_PHYSICS( GEOMETRY* Geometry );
    ~RIGID_PHYSICS();

public:
//    void Update( const float DeltaTime );
//    void TryUpdate( const float deltaTime );
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
//    bool DetectCollision( const GEOMETRY* A, const GEOMETRY* B ) const;
//    void HandleCollision( const GEOMETRY* Incoming, const float DeltaTime );
//    void ApplyCollisionResponse( const GEOMETRY* B );

// TODO: private
public:
    vertex*   x; // position (center of mass)
//    vector3   p; // Linear Momentum
//    vector3   L; // Angular Momentum
//    vector3   v; // velocity
//    vector3   w; // angular veclocity

private:

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
 //   void ApplyImpulseResponse( const matrix3 I_inv, const float m_inv, const vector3 r, const vector3 n, vector3 v, vector3 w );


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
//    float BilateralAdvancement( const GEOMETRY* A, const GEOMETRY* B,
//                                const vector3 v_a, const vector3 v_b,
//                                const vector3 w_a, const vector3 w_b,
//                                const float DeltaTime,
//                                const float tolerance );

    //******************************************************************************
    // Utility functions
    //******************************************************************************
//    float GetClosestPoint        ( const GEOMETRY* A, const GEOMETRY* B, const matrix4 ta, const matrix4 tb );
//    float   Seperation             ( const GEOMETRY* A, const GEOMETRY* B,
//                                     const vector3 v_a, const vector3 v_b,
//                                     const vector3 w_a, const vector3 w_b,
//                                     const float DeltaTime, const float t);
};
