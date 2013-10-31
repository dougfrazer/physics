#include <limits> // for min float point value
#include "rigid_physics.h"
#include "geometry.h"

static const float g = 0.2;
static const float e = 0.85;
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
    v = StartingVelocity;
}
//******************************************************************************

//******************************************************************************
void RIGID_PHYSICS::Update( const float DeltaTime )
{
    // update velocity + position due to gravity
    v.y += g * DeltaTime;
    Pending->Position.y -= v.y * DeltaTime;
}
//******************************************************************************

//******************************************************************************
bool RIGID_PHYSICS::DetectCollision( const GEOMETRY* In )
{
    std::vector<vector3> list;
    
    vector3 start( (vector3(Pending->VertexList[0]) + vector3(Pending->Position)) -
                   vector3(In->VertexList[0]) + vector3(In->Position) ); // start with any point in the mikowski difference
    list.push_back( start ); 
    vector3 d = -start;
    while( true ) {
        vector3 a = Support( d, Pending, In );
        if( d.dot( a ) < 0 ) {
            return false;
        }
        list.push_back( a );
        if( Simplex( &list, &d ) ) {
            return true;
        }
    }
    return false;
}
//******************************************************************************

//******************************************************************************
void RIGID_PHYSICS::HandleCollision( const GEOMETRY* In )
{
    // for now, just pretend its perfectly elastic
    v = v * (-e);
    Pending->Position = Geometry->Position;

    // TODO:
    // 1.) Get the point of impact and normal of the colliding plane
    // 2.) apply a force in the direction of the plane normal at the point of impact
    // 3.) translate force to center of mass

    // v_new = v_old + j*n
    // where j is the magnitude of the impulse and n is the normal to the plane of impact
    //
    //        -(1 - e)v*n
    // j =  -----------------------
    //      1/m +  (r x n)^2 / I^2
    //
    // e = elasticity (0.0 - 1.0)
    // v = current velocity
    // n = normal to the plane of impact
    // m = mass
    // r = vector3 to point of impact from center of mass
    // I = moment of inertia
}
//******************************************************************************

//******************************************************************************
vector3 RIGID_PHYSICS::Support( const vector3 d, const GEOMETRY* Geo )
{
    float dot_product = -std::numeric_limits<float>::infinity() ;
    vector3 closest;
    for( int i = 0; i < Geo->NumVertices; i++ ) {
        vector3 t = vector3(Geo->VertexList[i]) + vector3(Geo->Position);
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
bool RIGID_PHYSICS::Simplex( std::vector<vector3>* list, vector3* d )
{
    if( list->size() == 2 ) {
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
        vector3 a = list->at(1);
        vector3 b = list->at(0);
        vector3 ab = b - a;
        vector3 ao = -a;
        if( ab.dot( ao ) > 0 ) {
            *d = ab.cross(ao).cross(ab);
        } else { 
            *d = ao;
            list->pop_back();
        }
    } else if( list->size() == 3 ) {
        // we have a triangle/plane:
        vector3 a = list->at(2);
        vector3 b = list->at(1);
        vector3 c = list->at(0);
        vector3 ab = b - a;
        vector3 ac = c - a;
        vector3 bc = c - b;
        vector3 ao = -a;
        if( ab.dot( ao ) > 0 ) {
           if( bc.dot( ao ) > 0 ) {
                *d = ab.cross(ao).cross(ab);
            } else {
                *d = ao;
                list->pop_back();
            } 
        } else if( ac.dot( ao ) > 0 ) {
            if( -bc.dot( ao ) > 0 ) {
                *d = ac.cross(ao).cross(ac);
            } else {
                *d = ao;
                list->pop_back();
            }
        } else {
            *d = ao;
            list->pop_back();
        }

    } else if( list->size() == 4 ) {
        // we have a tetrahedron
        // return whether or not the origin is enclosed in the tetrahedron
        vector3 a = list->at(3);
        vector3 b = list->at(2);
        vector3 c = list->at(1);
        vector3 d = list->at(0);
        vector3 ab = b - a;
        vector3 ac = c - a;
        vector3 ad = d - a;
        vector3 ao = -a;
        if( ab.dot( ao ) > 0 && ac.dot( ao ) > 0 && ad.dot( ao ) > 0 ) {
            return true;
        } else {
            list->pop_back();
        }
    } else {
        assert(false);  // should not be possible
    }
    return false;
}
//******************************************************************************
