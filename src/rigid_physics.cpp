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
	srand(time(NULL));
    static const float MaxRandomRotation = 360.0 * PI / 180.0;
    v = StartingVelocity;
	Geometry->Rotation.x = (float)rand()/((float)RAND_MAX/MaxRandomRotation); 
	Geometry->Rotation.y = (float)rand()/((float)RAND_MAX/MaxRandomRotation); 
	Geometry->Rotation.z = (float)rand()/((float)RAND_MAX/MaxRandomRotation); 
    *Pending = *Geometry;
}
//******************************************************************************

//******************************************************************************
void RIGID_PHYSICS::Update( const float DeltaTime )
{
    // update linear velocity due to gravity
    v = v + g*DeltaTime;

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
void RIGID_PHYSICS::HandleCollision( const GEOMETRY* In, const float DeltaTime )
{
    // TODO: make all of this material data stored parallel to geometry
	// Mass
    static const float m = 10.0;
	// Inertia Tensor ( for a circle )
    static const matrix3 I( .4*m*5*5, 0.0, 0.0,
                            0.0, .4*m*5*5, 0.0,
                            0.0, 0.0, .4*m*5*5 );
	// Inverse Inertia Tensor
    static const matrix3 I_inv = I.inv();


    // Find interesting points related to this collision
	vector3 CollisionPoint = GetCollisionPoint( In );
    vector3 n = GetCollisionPlaneNormal( In );
    vector3 r = CollisionPoint - Geometry->Position;

    // Attempt to move the bodies out of the collision
    vector3 none;
    if( v.magnitude() < 0.5 ) { 
        v = none;
    }
    float t = BilateralAdvancement( Geometry, In, v, none, w, none, n, DeltaTime, 0.001 );
    Pending->Position = Geometry->Position + v * t*DeltaTime;
    Pending->Rotation = Geometry->Rotation + w * t*DeltaTime;
 
    // Apply the forces to the bodies
    ApplyImpulseResponse( I_inv, m, r, n, &v, &w );

	
//	Debug_DrawLine( CollisionPoint, CollisionPoint + velocityAtPoint, Color::Maroon );
//	Debug_DrawLine( Geometry->Position, v,                            Color::Navy );
//	Debug_DrawLine( Geometry->Position, w,                            Color::Red );	
//	Debug_DrawLine( Geometry->Position, CollisionPoint,               Color::Green );
//	Debug_DrawLine( vector3(0,0,0),     n,                            Color::Orange );
//	if( !World_GetPaused() ) {
//		World_Pause();
//	}
}
//******************************************************************************

//******************************************************************************
float RIGID_PHYSICS::BilateralAdvancement(const GEOMETRY* A, const GEOMETRY* B,
                                          const vector3 v_a, const vector3 v_b, 
                                          const vector3 w_a, const vector3 w_b, 
                                          const vector3 n,
                                          const float DeltaTime,
                                          const float tolerance )
{
    float t0 = 0.0;
    float t1 = 1.0;
    float t = 0.5;
    float s = std::numeric_limits<float>::infinity();
    int num_iterations = 0;

    float s_range[10];
    for( int i = 1; i <= 10; i++ ) {
        s_range[i-1] = Seperation( A, B, v_a, v_b, w_a, w_b, n, DeltaTime, i/10.0 );
    }

    while( num_iterations < 100 ) {
        t = ( t1 - t0 ) / 2.0;
        s = Seperation( A, B, v_a, v_b, w_a, w_b, n, DeltaTime, t );
        if( abs(s) < tolerance ) {
            break;
        }
        // Use bisection to find the root
        if( s < 0 ) {
            t1 = t;
        } else {
            t0 = t;
        }
        num_iterations++;
    }
    assert( num_iterations != 100 );
    return t; 
}
//******************************************************************************

//******************************************************************************
float RIGID_PHYSICS::Seperation(const GEOMETRY* A, const GEOMETRY* B,
                                const vector3 v_a, const vector3 v_b, 
                                const vector3 w_a, const vector3 w_b, 
                                const vector3 n,
                                const float DeltaTime, const float t)
{
    vector3 p_a, p_b;
    vector3 r_a, r_b;
    matrix4 ta, tb;
    float timestep = DeltaTime * t;
        
    p_a = A->Position + v_a * timestep;
    r_a = A->Rotation + w_a * timestep;
    p_b = B->Position + v_b * timestep;
    r_b = B->Rotation + w_b * timestep;

    ta.translate( p_a );
    ta.rotate( r_a );
    tb.translate( p_b );
    tb.rotate( r_b );

    vector3 a = GetClosestPoint( A, B, ta, tb, n );
    return a.dot( n );
}
//******************************************************************************

//******************************************************************************
vector3 RIGID_PHYSICS::GetClosestPoint( const GEOMETRY* A, const GEOMETRY* B, const matrix4 ta, const matrix4 tb, const vector3 n )
{
    vector3 closest_point;
    float min = std::numeric_limits<float>::infinity();
    for( int i = 0; i < A->NumVertices; i++ ) {
        vector3 a = ta * A->VertexList[i];
        float d = a.dot( n );
        if( d < min ) {
            min = d;
            closest_point = a;
        }
    }
    assert( min != std::numeric_limits<float>::infinity() );
    return closest_point;
}
//******************************************************************************

//******************************************************************************
void RIGID_PHYSICS::ApplyImpulseResponse( const matrix3 I_inv, const float m, const vector3 r, const vector3 n, vector3* v, vector3* w )
{
    vector3 angularVelocity = I_inv * *w;
    vector3 velocityAtPoint = *v + angularVelocity;
    vector3 angularMomentum = I_inv * r.cross(n);
    float i = -( 1.0 + e ) * velocityAtPoint.dot( n );
    float k = 1/m + angularMomentum.cross( r ).dot( n );
    float j = i / k;
	*v = *v + n * ( j / m );
    *w = *w + angularMomentum * ( j / m );
}
//******************************************************************************

//******************************************************************************
vector3 RIGID_PHYSICS::Support( const vector3 d, const GEOMETRY* Geo )
{
	matrix4 transform;
	transform.translate( Geo->Position );
	transform.rotate( Geo->Rotation );
    return Support( d, Geo, &transform );
}
//******************************************************************************
vector3 RIGID_PHYSICS::Support( const vector3 d, const GEOMETRY* Geo, const matrix4* const m )
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
vector3 RIGID_PHYSICS::GetCollisionPlaneNormal( const GEOMETRY* In )
{
    vector3 v(0.0, 1.0, 0.0);
    return v;
}
//******************************************************************************

//******************************************************************************
vector3 RIGID_PHYSICS::GetCollisionPoint( const GEOMETRY* In )
{
    matrix4 m;
    m.translate( Geometry->Position );
    m.rotate( Geometry->Rotation );
    vector3 planeNormal = GetCollisionPlaneNormal( In );
    float min = std::numeric_limits<float>::infinity() ;
    vector3 ret;
    for( int i = 0; i < Geometry->NumVertices; i++ ) {
        vector3 current = vector3( m * vector4(Geometry->VertexList[i]) );
        float d = current.dot( planeNormal );
        if( d < min ) {
            min = d; 
            ret = current;
        }
    }
    assert( min != std::numeric_limits<float>::infinity() );
    return ret;
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
bool RIGID_PHYSICS::Simplex( std::vector<vector3>* simplex, vector3* dir, const vector3 dest )
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
