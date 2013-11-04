#include <limits> // for min float point value
#include "rigid_physics.h"
#include "geometry.h"
#include "matrix.h"
#include "plane.h"
#include <stdlib.h>
#include <time.h>

#define min( a, b ) a < b ? a : b

static const vector3 g( 0.0, -0.2, 0.0);
static const float e = 2.5;
static const vector3 StartingVelocity( 0.0, 0.0, 0.0 );
static const vector3 origin;

//******************************************************************************
RIGID_PHYSICS::RIGID_PHYSICS( GEOMETRY* Geo )
    : PHYSICS( Geo )
{
    Reset();
}
//******************************************************************************

//******************************************************************************
RIGID_PHYSICS::~RIGID_PHYSICS()
{
}
//******************************************************************************

//******************************************************************************
void RIGID_PHYSICS::Reset()
{
    static const float MaxRandomRotation = 10.0;
    w.x = (float)rand()/((float)RAND_MAX/MaxRandomRotation);
    w.y = (float)rand()/((float)RAND_MAX/MaxRandomRotation);
    w.z = (float)rand()/((float)RAND_MAX/MaxRandomRotation);
    v = StartingVelocity;
    *Pending = *Geometry;
}
//******************************************************************************

//******************************************************************************
void RIGID_PHYSICS::Update( const float DeltaTime )
{
    // update linear velocity due to gravity
    v = v + g*DeltaTime;

    // decay the rotation (approximation of air resistance, just decay linearly)
    w = w * 0.999;

    // Update position due to linear velocity
    Pending->Position = Pending->Position + v * DeltaTime;

    // Update rotation due to angular velocity
    Pending->Rotation = Pending->Rotation + w * DeltaTime;
}
//******************************************************************************

//******************************************************************************
bool RIGID_PHYSICS::DetectCollision( const GEOMETRY* In )
{
    std::vector<vector3> simplex;
    
    // start with any point in the mikowski difference
    vector3 start( (vector3(Pending->VertexList[0]) + vector3(Pending->Position)) -
                   vector3(In->VertexList[0]) + vector3(In->Position) );
    simplex.push_back( start ); 
    // go towards the origin
    vector3 d = -start;
    while( true ) {
        vector3 a = Support( d, Pending, In );
        if( d.dot( a ) < 0 ) {
            return false;
        }
        simplex.push_back( a );
        if( Simplex( &simplex, &d, origin ) ) {
            return true;
        }
    }
    return false;
}
//******************************************************************************

//******************************************************************************
void RIGID_PHYSICS::HandleCollision( const GEOMETRY* In )
{
    //v = v * (-e);
    // Temporary: revert back to prevoius position
    // TODO: move the object as close to the colliding object as possible
    //       or better yet, move it to where it would be properly after this time step
    //       after the collision has resolved
    Pending->Position = Geometry->Position; 
    static const float m = 1.0;
    static const matrix3 I( .4, 0.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 0.0, .4 );
    static const matrix3 I_inv = I.inv();
    vector3 n = GetCollisionPlaneNormal( In );
    vector3 r = GetCollisionPoint( In ) - Geometry->Position;

    //-----------------------------------------------------------------------------
    // Simplified impulse response equation (assuming the plane we're colliding with
    // has infinite mass and zero velocity)
    //
    // v_new = v_old + j*n
    //
    // where:
    //              -(1 - e)v dot n
    // j =  --------------------------------------
    //      1/m +  ( (I^-1 * (r x n)) x r) ) dot n
    //
    // j = magnitude of impulse
    // e = elasticity (0.0 - 1.0)
    // v = current velocity
    // n = normal to the plane of impact
    // m = mass
    // r = vector3 to point of impact from center of mass
    // I = moment of inertia
    //----------------------------------------------------------------------------
    vector3 angularVelocity = I * w;
    vector3 velocityAtPoint = v + angularVelocity;
    vector3 angularMomentum = I * r.cross(n);
    float i = -( 1.0 + e ) * velocityAtPoint.dot( n );
    float k = 1/m + angularMomentum.cross( r ).dot( n );
    float j = i / k;
    v = n * j;
    w = r.cross(n) * j;
}
//******************************************************************************

//******************************************************************************
vector3 RIGID_PHYSICS::Support( const vector3 d, const GEOMETRY* Geo )
{
    return Support( d, Geo, Geo->Position );
}
//******************************************************************************
vector3 RIGID_PHYSICS::Support( const vector3 d, const GEOMETRY* Geo, const vector3 s )
{
    float dot_product = -std::numeric_limits<float>::infinity() ;
    vector3 closest;
    for( int i = 0; i < Geo->NumVertices; i++ ) {
        vector3 t = vector3(Geo->VertexList[i]) + s;
        float c = d.dot( t );
        if( c >= dot_product ) {
            dot_product = c;
            closest = t;
        }
    }
    assert( dot_product != -std::numeric_limits<float>::infinity() );
    return closest;
}
//******************************************************************************

//******************************************************************************
vector3 RIGID_PHYSICS::GetCollisionPlaneNormal( const GEOMETRY* In )
{
    vector3 v(0.0, -1.0, 0.0);
    return v;
}
//******************************************************************************

//******************************************************************************
vector3 RIGID_PHYSICS::GetCollisionPoint( const GEOMETRY* In )
{
    vector3 a = Support( v, Geometry );
    return a;
}
//******************************************************************************

//******************************************************************************
//******************************************************************************
// Find the furthest point in the minkowski difference of A and B in the direction d.
// This is the same as saying find the furthest point in A in the direction of d 
// MINUS the furthest point in B in the opposite direction
//******************************************************************************
vector3 RIGID_PHYSICS::Support( const vector3 d, const GEOMETRY* A, const GEOMETRY* B )
{
    return Support( d, A ) - Support( -d, B ); 
}
//******************************************************************************

//******************************************************************************
bool RIGID_PHYSICS::Simplex( std::vector<vector3>* simplex, vector3* d, const vector3 dest )
{
    if( simplex->size() == 2 ) {
        // we have a line:
        // figure out which point is closer to the origin
        
        // NOTE: A is the vector3 we just added, and are testing against
        // AB represents the vector3 from point A to B
        // AO represents the vector3 from A to the origin
        // if the dot product is positive, there is some projection of AB on AO.
        // this tells us that the vector3 AB is part of a tetrahedron that encloses the origin.
        // therefore, we want to keep A, and continue the algorithm to find a third point
        // (also update direction to point towards the origin).
        // otherwise (if dot product is less than zero) we want to pop A and find another point
        vector3 a = simplex->at(1);
        vector3 b = simplex->at(0);
        vector3 ab = b - a;
        vector3 ao = dest - a;
        if( ab.dot( ao ) > 0 ) {
            *d = ab.cross(ao).cross(ab);
        } else { 
            *d = ao;
            simplex->pop_back();
        }
    } else if( simplex->size() == 3 ) {
        // we have a triangle/plane:
        vector3 a = simplex->at(2);
        vector3 b = simplex->at(1);
        vector3 c = simplex->at(0);
        vector3 ab = b - a;
        vector3 ac = c - a;
        vector3 bc = c - b;
        vector3 ao = dest - a;
        if( ab.dot( ao ) > 0 ) {
           if( bc.dot( ao ) > 0 ) {
                *d = ab.cross(ao).cross(ab);
            } else {
                *d = ao;
                simplex->pop_back();
            } 
        } else if( ac.dot( ao ) > 0 ) {
            if( -bc.dot( ao ) > 0 ) {
                *d = ac.cross(ao).cross(ac);
            } else {
                *d = ao;
                simplex->pop_back();
            }
        } else {
            *d = ao;
            simplex->pop_back();
        }

    } else if( simplex->size() == 4 ) {
        // we have a tetrahedron
        // return whether or not the origin is enclosed in the tetrahedron
        vector3 a = simplex->at(3);
        vector3 b = simplex->at(2);
        vector3 c = simplex->at(1);
        vector3 d = simplex->at(0);
        vector3 ab = b - a;
        vector3 ac = c - a;
        vector3 ad = d - a;
        vector3 bc = b - c;
        vector3 bd = b - d;
        vector3 cd = c - d;
        vector3 ao = dest - a;
        if( ab.dot( ao ) > 0 && 
            ac.dot( ao ) > 0 && 
            ad.dot( ao ) > 0 && 
            bc.dot( ao ) > 0 && 
            bd.dot( ao ) > 0 && 
            cd.dot( ao ) > 0 ) {
            return true;
        } else {
            simplex->pop_back();
        }
    } else {
        assert(false);  // should not be possible
    }
    return false;
}
//******************************************************************************
