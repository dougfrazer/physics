#include <limits> // for min float point value
#include "rigid_physics.h"
#include "geometry.h"
#include "matrix.h"
#include "plane.h"
#include "common.h"
#include <stdlib.h>
#include <time.h>

#include "debug.h" // temporary
#include "world.h" // temporary

static const vector3 g( 0.0, -9.8, 0.0);
static const float e = 1.0f;
static const vector3 StartingVelocity( 0.0, 0.0, 0.0 );
static const vector3 origin;
// TODO: make all of this material data stored parallel to geometry
// Mass
static const float m = 10.0;
// Inertia Tensor ( for a circle )
static const float r = 5.0f;
static const float moment_of_inertia = 0.4f*m*r*r;
static const matrix3 I( moment_of_inertia, 0.0, 0.0,
                        0.0, moment_of_inertia, 0.0,
                        0.0, 0.0, moment_of_inertia );
// Inverse Inertia Tensor
static const matrix3 I_inv = I.inv();

#define DEBUG_ENERGY 0
#define DRAW_DEBUG_LINES 1
#define DEBUG_COLLISION 1

#ifdef DEBUG_COLLISION
#if defined( _WIN32 ) || defined( _WIN64 )
#include <windows.h>
#include <strsafe.h>
#else
#include <sys/time.h>
#include <stdio.h>
#endif
#endif // DEBUG_COLLISION

//******************************************************************************
// Support:
// --------
//   This will attempt to find the closest point in the geometry to the vector
//   passed in.
//******************************************************************************
static vector3 Support( const vector3 d, const GEOMETRY* Geo );
static vector3 Support( const vector3 d, const GEOMETRY* Geo, const matrix4* m );
static vector3 Support( const vector3 d, const GEOMETRY* A, const GEOMETRY* B );

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
static bool Simplex( std::vector<vector3>* list, vector3* d, const vector3 dest );

//
// FORWARD DECLARATION
//
static void ApplyImpulseResponse( GEOMETRY* A, const matrix3 I_inv, const float m, const vector3 r, const vector3 n, const vector3 v, const vector3 w );

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

//
// forward declarations
//
static vector3 GetCollisionPoint( const GEOMETRY* A, const GEOMETRY* B, float* depth );
static vector3 GetCollisionPlaneNormal( const GEOMETRY* A, const GEOMETRY* B );

//******************************************************************************
void RIGID_PHYSICS::Reset()
{
	srand(time(NULL));
    static const float MaxRandomRotation = 360.0 * PI / 180.0;
    Geometry->p = origin;
    Geometry->L = origin;
	Geometry->Rotation.x = (float)rand()/((float)RAND_MAX/MaxRandomRotation);
	Geometry->Rotation.y = (float)rand()/((float)RAND_MAX/MaxRandomRotation);
	Geometry->Rotation.z = (float)rand()/((float)RAND_MAX/MaxRandomRotation);
    *Pending = *Geometry;
}
//******************************************************************************

//******************************************************************************
void RigidPhysics_TryUpdate( GEOMETRY* To, const GEOMETRY* From, vector3 gravity, float DeltaTime )
{
    To->p = From->p + ( gravity * m ) * DeltaTime;
    To->L = From->L;

    vector3 v = To->p * 1/m;
    vector3 w = I_inv * To->L;

    To->Position = From->Position + v * DeltaTime;
    To->Rotation = From->Rotation + w * DeltaTime;

#if DEBUG_ENERGY
    vector3 vel = To->p * 1/m;
    vector3 rot = I_inv * To->L;
    float Ke_translate = 0.5f * m * vel.magnitude()*vel.magnitude();
    float Pe_gravity = m * g.magnitude() * From->Position.y;
    float Ke_rotate = 0.5f * moment_of_inertia * rot.magnitude()*rot.magnitude();
    printf("[DT=%f] Kinetic Energy: %f (tran=%f (P=%f,K=%f) | rot=%f)\n", DeltaTime, Ke_translate+Ke_rotate+Pe_gravity, Ke_translate+Pe_gravity, Pe_gravity, Ke_translate, Ke_rotate);
#endif
}
//******************************************************************************
bool RigidPhysics_DetectCollision( const GEOMETRY* A, const GEOMETRY* B)
{
    std::vector<vector3> simplex;

    // start with any point in the mikowski difference
    vector3 start( (vector3(A->VertexList[0]) + vector3(A->Position)) -
                   (vector3(B->VertexList[0]) + vector3(B->Position)) );
    simplex.push_back( start );
#ifdef DEBUG_COLLISION
	Debug_Printf("Trying collision between %p and %p...", A, B);
#endif
    // go towards the origin
    vector3 d = -start;
    while( true ) {
        vector3 a = Support( d, A, B );
        if( d.dot( a ) < 0 ) {
#ifdef DEBUG_COLLISION
            Debug_Printf("false\n");
#endif
            return false;
        }
        simplex.push_back( a );
        if( Simplex( &simplex, &d, origin ) ) {
#ifdef DEBUG_COLLISION
            Debug_Printf("true\n");
#endif
            return true;
        }
    }
#ifdef DEBUG_COLLISION
	Debug_Printf("false\n");
#endif
    return false;
}

//******************************************************************************
void RigidPhysics_ApplyCollisionResponse(GEOMETRY* A, const GEOMETRY* B)
{
    float depth;
	vector3 CollisionPoint = GetCollisionPoint( A, B, &depth );
    vector3 n = GetCollisionPlaneNormal( A, B );
    vector3 r = CollisionPoint - A->Position;

    // Attempt to move the bodies out of the collision
    vector3 none;
    vector3 v = A->p * 1/m;
    vector3 w = I_inv * A->L;

    // Apply the forces to the bodies
    ApplyImpulseResponse( A, I_inv, m, r, n, v, w );
}
//******************************************************************************
void ApplyImpulseResponse( GEOMETRY* A, const matrix3 I_inv, const float m, const vector3 r, const vector3 n, const vector3 v, const vector3 w )
{
    vector3 velocityAtPoint = v + w.cross(r);
    float i = -( 1.0 + e ) * velocityAtPoint.dot( n );
    float k = 1/m + ( (I_inv * r.cross(n) ).cross(r) ).dot(n);
    float j = i / k;
    A->p = A->p + n * j;
    A->L = A->L + r.cross(n) * j;
}
//******************************************************************************


//******************************************************************************
vector3 GetCollisionPlaneNormal( const GEOMETRY* A, const GEOMETRY* B )
{
    // TODO
    vector3 v(0.0, 1.0, 0.0);
    return v;
}
//******************************************************************************

//******************************************************************************
vector3 GetCollisionPoint( const GEOMETRY* A, const GEOMETRY* B, float* depth )
{
    matrix4 m;
    m.translate( A->Position );
    m.rotate( A->Rotation );
    vector3 planeNormal = GetCollisionPlaneNormal( A, B );
    float min = std::numeric_limits<float>::infinity() ;
    vector3 ret;
    for( int i = 0; i < A->NumVertices; i++ ) {
        vector3 current = vector3( m * vector4(A->VertexList[i]) );
        float d = current.dot( planeNormal );
        if( d < min ) {
            min = d;
            ret = current;
        }
    }
    assert( min != std::numeric_limits<float>::infinity() );
    if(depth) {
        *depth = fabs(min);
    }
    return ret;
}
//******************************************************************************

//******************************************************************************
// Find the furthest point in the minkowski difference of A and B in the direction d.
// This is the same as saying find the furthest point in A in the direction of d
// MINUS the furthest point in B in the opposite direction
//******************************************************************************
vector3 Support( const vector3 d, const GEOMETRY* A, const GEOMETRY* B )
{
    return Support( d, A ) - Support( -d, B );
}
//******************************************************************************
vector3 Support( const vector3 d, const GEOMETRY* Geo )
{
	matrix4 transform;
	transform.translate( Geo->Position );
	transform.rotate( Geo->Rotation );
    return Support( d, Geo, &transform );
}
//******************************************************************************
vector3 Support( const vector3 d, const GEOMETRY* Geo, const matrix4* const m )
{
    float dot_product = -std::numeric_limits<float>::infinity() ;
    vector3 closest;
    for( int i = 0; i < Geo->NumVertices; i++ ) {
        vector3 t = vector3(*m * vector4(Geo->VertexList[i]));

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
bool Simplex( std::vector<vector3>* simplex, vector3* dir, const vector3 dest )
{
    if( simplex->size() == 2 ) {
        vector3 a = simplex->at(1);
        vector3 b = simplex->at(0);
        vector3 ab = b - a;
        vector3 ao = dest - a;
        if( ab.dot( ao ) > 0 ) {
            *dir = ab.cross(ao).cross(ab);
        } else {
            *dir = ao;
            simplex->pop_back();
        }
    } else if( simplex->size() == 3 ) {
        // we have a triangle/plane:
        vector3 a = simplex->at(2);
        vector3 b = simplex->at(1);
        vector3 c = simplex->at(0);
		simplex->clear();
        vector3 ab  = b - a;
        vector3 ac  = c - a;
		vector3 abc = ab.cross( ac );
        vector3 ao  = dest - a;

		// first check to see if the point is above AC
        if( abc.cross(ac).dot( ao ) > 0 ) {
           if( ac.dot( ao ) > 0 ) {
                *dir = ac.cross(ao).cross(ac);
				simplex->push_back(a);
				simplex->push_back(c);
            } else if( ab.dot(ao) ) {
				*dir = ab.cross(ao).cross(ab);
				simplex->push_back(a);
				simplex->push_back(b);
			} else {
                *dir = ao;
                simplex->push_back(a);
            }
		// then check to see if the point is below AB
        } else if( ab.cross(abc).dot( ao ) > 0 ) {
			if( ab.dot( ao ) > 0 ) {
				*dir = ab.cross(ao).cross(ab);
				simplex->push_back(a);
				simplex->push_back(b);
			} else if( ab.dot(ao) ) {
				*dir = ac.cross(ao).cross(ac);
				simplex->push_back(a);
				simplex->push_back(c);
			} else {
				*dir = ao;
				simplex->push_back(a);
			}
		// OK, the point is necessarily below AC and above AB (so it is within the triangle),
		// just need to figure out which direction to search
        } else if( abc.dot(ao) > 0 ) {
            *dir = abc;
			simplex->push_back(a);
			simplex->push_back(b);
			simplex->push_back(c);
        } else {
			*dir = -abc;
			simplex->push_back(a);
			simplex->push_back(c);
			simplex->push_back(b);
		}
    } else if( simplex->size() == 4 ) {
        // we have a tetrahedron
        // return whether or not the origin is enclosed in the tetrahedron
        vector3 a = simplex->at(3);
        vector3 b = simplex->at(2);
        vector3 c = simplex->at(1);
        vector3 d = simplex->at(0);
		simplex->clear();
        vector3 ab = b - a;
        vector3 ac = c - a;
        vector3 ad = d - a;
		vector3 ao = dest - a;
		vector3 abd = ab.cross(ad);
		vector3 adc = ad.cross(ac);
		vector3 acb = ac.cross(ab);
		if( abd.dot( ao ) > 0 ) {
			*dir = abd;
			simplex->push_back(a);
			simplex->push_back(b);
			simplex->push_back(d);
		} else if( adc.dot( ao ) > 0 ) {
			*dir = adc;
			simplex->push_back(a);
			simplex->push_back(d);
			simplex->push_back(c);
		} else if( acb.dot( ao ) > 0 ) {
			*dir = acb;
			simplex->push_back(a);
			simplex->push_back(c);
			simplex->push_back(b);
		} else {
			return true;
		}
    } else {
        assert(false);  // should not be possible
    }
    return false;
}
//******************************************************************************
