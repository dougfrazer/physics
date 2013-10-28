#include <limits> // for min float point value
#include "rigid_physics.h"
#include "geometry.h"

static const float g = 0.1;
static const vector StartingVelocity( 0.0, 0.0, 0.0 );
static const vector origin;

RIGID_PHYSICS::RIGID_PHYSICS( GEOMETRY* Geo )
    : PHYSICS( Geo )
{
    Reset();
}

RIGID_PHYSICS::~RIGID_PHYSICS()
{
}

void RIGID_PHYSICS::Reset()
{
    v = StartingVelocity;
}

void RIGID_PHYSICS::Update( float DeltaTime )
{
    // update velocity + position due to gravity
    v.y += g * DeltaTime;
    Geometry->Position.y -= v.y * DeltaTime;
}

bool RIGID_PHYSICS::DetectCollision( GEOMETRY* In )
{
    std::vector<vector> list;
    
    // find a point in our object in the direction of the origin
    vector start( (vector(Geometry->VertexList[0]) + vector(Geometry->Position)) -
                   vector(In->VertexList[0]) + vector(In->Position) ); // start with any point in the mikowski difference
    list.push_back( start ); 
    vector d = -start;
    while( true ) {
        vector a = Support( d, Geometry, In );
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

GEOMETRY* RIGID_PHYSICS::HandleCollision( GEOMETRY* In )
{
    return Geometry;
}

//
// Find the point that is furthest direction in a direction for a specific geometry
//
vector RIGID_PHYSICS::Support( const vector d, const GEOMETRY* Geo )
{
    float dot_product = -std::numeric_limits<float>::infinity() ;
    vector closest;
    for( int i = 0; i < Geo->NumVertices; i++ ) {
        vector t = vector(Geo->VertexList[i]) + vector(Geo->Position);
        float c = d.dot( t );
        if( c >= dot_product ) {
            dot_product = c;
            closest = t;
        }
    }
    assert( dot_product != -std::numeric_limits<float>::infinity() );
    return closest;
}

// Find the furthest point in the minkowski difference of A and B in the direction d.
// This is the same as saying find the furthest point in A in the direction of d 
// MINUS the furthest point in B in the opposite direction
vector RIGID_PHYSICS::Support( const vector d, const GEOMETRY* A, const GEOMETRY* B )
{
    return Support( d, A ) - Support( -d, B ); 
}

bool RIGID_PHYSICS::Simplex( std::vector<vector>* list, vector* d )
{
    if( list->size() == 2 ) {
        // we have a line:
        // figure out which point is closer to the origin
        
        // NOTE: A is the vector we just added, and are testing against
        // AB represents the vector from point A to B
        // AO represents the vector from A to the origin
        // if the dot product is positive, there is some projection of AB on AO.
        // this tells us that the vector AB is part of a tetrahedron that encloses the origin.
        // therefore, we want to keep A, and continue the algorithm to find a third point
        // (also update direction to point towards the origin).
        // otherwise (if dot product is less than zero) we want to pop A and find another point
        vector a = list->at(1);
        vector b = list->at(0);
        vector ab = b - a;
        vector ao = -a;
        if( ab.dot( ao ) > 0 ) {
            *d = ab.cross(ao).cross(ab);
        } else { 
            *d = ao;
            list->pop_back();
        }
    } else if( list->size() == 3 ) {
        // we have a triangle/plane:
        vector a = list->at(2);
        vector b = list->at(1);
        vector c = list->at(0);
        vector ab = b - a;
        vector ac = c - a;
        vector bc = c - b;
        vector ao = -a;
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
        vector a = list->at(3);
        vector b = list->at(2);
        vector c = list->at(1);
        vector d = list->at(0);
        vector ab = b - a;
        vector ac = c - a;
        vector ad = d - a;
        vector ao = -a;
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
