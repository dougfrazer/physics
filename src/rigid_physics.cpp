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

//static const vector3 g( 0.0, -9.8, 0.0);
//static const float elasticity = 1.0f;
//static const vector3 StartingVelocity( 0.0, 0.0, 0.0 );
//static const vector3 origin;
//// TODO: make all of this material data stored parallel to geometry
//// Mass
//static const float m = 10.0;
//// Inertia Tensor ( for a circle )
//static const float r = 5.0f;
//static const float moment_of_inertia = 0.4f*m*r*r;
//static const matrix3 I( moment_of_inertia, 0.0, 0.0,
//                        0.0, moment_of_inertia, 0.0,
//                        0.0, 0.0, moment_of_inertia );
//// Inverse Inertia Tensor
//static const matrix3 I_inv = I.inv();

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
//******************************************************************************;
static vector3 Support( const vector3 d, const GEOMETRY* Geo, const matrix4* m );
static vector3 Support( const vector3 d, const GEOMETRY* A, const matrix4* at, const GEOMETRY* B, const matrix4* bt);

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

//******************************************************************************
RIGID_PHYSICS::RIGID_PHYSICS( GEOMETRY* Geo, vector3 initialPosition )
    : PHYSICS( Geo, initialPosition)
{
    Reset(initialPosition);
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
//******************************************************************************
void RIGID_PHYSICS::Reset(const vector3& pos)
{
	srand(time(NULL));
    static const float MaxRandomRotation = 360.0 * PI / 180.0;
    m_physData.p = origin;
    m_physData.L = origin;
	m_physData.Rotation.x = (float)rand()/((float)RAND_MAX/MaxRandomRotation);
	m_physData.Rotation.y = (float)rand()/((float)RAND_MAX/MaxRandomRotation);
	m_physData.Rotation.z = (float)rand()/((float)RAND_MAX/MaxRandomRotation);
    m_physData.Position = pos;
}
//******************************************************************************

//******************************************************************************
void RIGID_PHYSICS::Update( float DeltaTime )
{
    m_physData.p = m_physData.p + ( GetGravity() * m) * DeltaTime;

    vector3 v = m_physData.p * 1/m;
    vector3 w = I_inv * m_physData.L;

    m_physData.Position = m_physData.Position + v * DeltaTime;
    m_physData.Rotation = m_physData.Rotation + w * DeltaTime;

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
bool RIGID_PHYSICS::DetectCollision( const GEOMETRY* otherGeo, const PhysicsData& otherPhys, Collision* outCollision) const
{
    std::vector<vector3> simplex;

    // TODO: broad phase

    // start with any point in the mikowski difference
    vector3 start( (vector3(m_geometry->VertexList[0]) + vector3(m_physData.Position)) -
                   (vector3(otherGeo->VertexList[0]) + vector3(otherPhys.Position)) );
    simplex.push_back( start );

    // go towards the origin
    vector3 d = -start;
    while( true ) {
        matrix4 a_transform;
        a_transform.translate(m_physData.Position);
        a_transform.rotate(m_physData.Rotation);
        matrix4 b_transform;
        b_transform.translate(otherPhys.Position);
        b_transform.rotate(otherPhys.Rotation);
        vector3 a = Support( d, m_geometry, &a_transform, otherGeo, &b_transform);
        if( d.dot( a ) < 0 ) {
            return false;
        }
        simplex.push_back( a );
        if( Simplex( &simplex, &d, origin ) ) {
            //if (outCollision)
            //{
            //    outCollision->a = this;
            //    outCollision->b = other;
            //    plane_point_normal collisionPlane = GetCollisionPlane(A,B);
            //    outCollision->collisionPlaneNormal = collisionPlane.normal;
            //    outCollision->collisionPlanePoint = collisionPlane.point;
            //    float collisionDepth = 0.0f;
            //    outCollision->collisionPoint = GetCollisionPoint(A,B,&collisionDepth);
            //    outCollision->collisionDepth = collisionDepth;
            //}
            return true;
        }
    }

    return false;
}
//******************************************************************************
struct plane_point_normal
{
    vector3 normal;
    vector3 point;
};

//******************************************************************************
plane_point_normal GetCollisionPlane(const GEOMETRY* A, const GEOMETRY* B)
{
    // TODO
    return plane_point_normal({ {0.0f, 1.0f, 0.0f}, { 0.0f, 0.0f, 0.0f } });
}
//******************************************************************************
void RIGID_PHYSICS::ApplyCollisionResponse(CollisionData& data)
{
    vector3 n = data.planeNormal;
    vector3 r = data.point - m_physData.Position;

    // Attempt to move the bodies out of the collision
    vector3 v = m_physData.p * 1/m;
    vector3 w = I_inv * m_physData.L;

    // Apply the forces to the bodies
    // rigid body impulse response
    vector3 velocityAtPoint = v + w.cross(r);
    float i = -(1.0 + elasticity) * velocityAtPoint.dot(n);
    float k = 1 / m + ((I_inv * r.cross(n)).cross(r)).dot(n);
    float j = i / k;
    m_physData.p = m_physData.p + n * j;
    m_physData.L = m_physData.L + r.cross(n) * j;
}
//******************************************************************************
//vector3 GetCollisionPoint( const GEOMETRY* A, const GEOMETRY* B, float* depth )
//{
//    matrix4 m;
//    m.translate( A->Position );
//    m.rotate( A->Rotation );
//    plane_point_normal collisionPlane = GetCollisionPlane(A,B);
//    float min = std::numeric_limits<float>::infinity() ;
//    vector3 ret;
//    for( int i = 0; i < A->NumVertices; i++ ) {
//        vector3 current = vector3( m * vector4(A->VertexList[i]) );
//        float d = (current - collisionPlane.point).dot( collisionPlane.normal );
//        if( d < min ) {
//            min = d;
//            ret = current;
//        }
//    }
//    assert( min != std::numeric_limits<float>::infinity() );
//    if(depth) {
//        *depth = min;
//    }
//    return ret;
//}
//******************************************************************************

//******************************************************************************
// Find the furthest point in the minkowski difference of A and B in the direction d.
// This is the same as saying find the furthest point in A in the direction of d
// MINUS the furthest point in B in the opposite direction
//******************************************************************************
vector3 Support( const vector3 d, const GEOMETRY* A, const matrix4* A_transform, const GEOMETRY* B, const matrix4* B_transform )
{
    return Support( d, A, A_transform ) - Support( -d, B, B_transform );
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
