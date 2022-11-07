#include "windows.h"

#include "circle.h"
#include "geometry.h"
#include "physics.h"
#include "world.h" // temporary

#include "stdlib.h"
#include "math.h"
#include "string.h"


#include <GL/gl.h>
#include <GL/glu.h>

static const vector3 s_initialPosition( 0.0, 40.0, 0.0 );
static const float MaxRandomRotation = 360.0 * PI / 180.0;

CIRCLE::CIRCLE(float r)
    : m_radius(r)
{
    // define geometry (todo: this should be actual data)
    m_geometry = new GEOMETRY();
    CreateIcosahadron(5 , m_geometry , 0);

    StaticPhysicsData physData;
    physData.m_gravity = {0.0f,-9.8f,0.0f};
    physData.m_initialPosition = s_initialPosition;
    
    physData.m_initialRotation.x = (float)rand() / ((float)RAND_MAX / MaxRandomRotation);
    physData.m_initialRotation.y = (float)rand() / ((float)RAND_MAX / MaxRandomRotation);
    physData.m_initialRotation.z = (float)rand() / ((float)RAND_MAX / MaxRandomRotation);
    physData.m_mass = 10.0f;
    physData.m_elasticity = 1.0f; // perfectly elastic
    physData.m_momentOfInertia = 0.4f * physData.m_mass * m_radius * m_radius;
    physData.m_inertiaTensor = matrix3(physData.m_momentOfInertia, 0.0f, 0.0f,
                                       0.0f, physData.m_momentOfInertia, 0.0f,
                                       0.0f, 0.0f, physData.m_momentOfInertia);
    physData.m_inverseInertiaTensor = physData.m_inertiaTensor.inv();
    physData.m_collisionResponseType = COLLISION_RESPONSE_IMPULSE;

    // attach physics
    m_physics = new PHYSICS( m_geometry, physData);
}

CIRCLE::~CIRCLE()
{

}

void CIRCLE::Reset()
{
    m_physics->Reset();
}

void CIRCLE::Update( float DeltaTime )
{
}


void CIRCLE::Draw( void )
{
	GLfloat modelMatrix[16];
	GLfloat tempMatrix[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, modelMatrix );

    glPushMatrix();

	glGetFloatv( GL_MODELVIEW_MATRIX, modelMatrix );

	matrix4 transform;
	transform.translate( m_physics->m_dynamic.m_position );
	transform.rotate( m_physics->m_dynamic.m_rotation );
	transform = transform.t(); // needs to be in column-major form
	transform = transform * matrix4( (float*)modelMatrix );

	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( (GLfloat*)&transform );

    // make it red... for no particular reason
	glColor3f(1.0,0.0,0.0);
	
	glBegin( GL_LINE_STRIP );
	for( int i = 0; i < m_geometry->NumFaces; i++ ) {
		glVertex3fv((GLfloat*)&m_physics->m_geometry->VertexList[ m_geometry->FaceList[ i ].v1 ]);
		glVertex3fv((GLfloat*)&m_physics->m_geometry->VertexList[ m_geometry->FaceList[ i ].v2 ]);
		glVertex3fv((GLfloat*)&m_physics->m_geometry->VertexList[ m_geometry->FaceList[ i ].v3 ]);
	} 
	glEnd();
    
    glPopMatrix();

/*
    vector3 vel = m_geometry->Position + ((RIGID_PHYSICS*)m_physics)->v;
    vector3 rot = m_geometry->Position + ((RIGID_PHYSICS*)m_physics)->w;
    glColor3f( 0.0, 1.0, 0.0 ); 
    glBegin( GL_LINES );
        glVertex3fv( (GLfloat*)&m_geometry->Position );
        glVertex3fv( (GLfloat*)&vel  );
    glEnd();
    glColor3f( 0.0, 1.0, 1.0 );
    glBegin( GL_LINES );
        glVertex3fv( (GLfloat*)&m_geometry->Position );
        glVertex3fv( (GLfloat*)&rot  );
    glEnd();
*/
//	glPopAttrib();
}

//
// Based off: 
// http://www.fho-emden.de/~hoffmann/ikos27042002.pdf
//
void CIRCLE::CreateIcosahadron(float r, GEOMETRY* geometry, int subdivision)
{
    // OK, this is just a quick-and-dirty thing to get it working
    // An icosahadron is 2 pentagrams and 2 single points, that looks kind of like a circle
    // we can use this basic shape to subdivide and return a more accurate sphere... maybe later
    
    static const float theta = 26.56505117707799 * PI / 180.0;


    //
    // Vertices
    // 
    geometry->VertexList = (vertex*)malloc( 12*sizeof(vertex) );
    geometry->NumVertices = 12;
    float phi = 0;
    for( int i = 1; i < 6; i++ ) {
        geometry->VertexList[i].x = r * cos( theta ) * cos( phi );
        geometry->VertexList[i].y = r * cos( theta ) * sin( phi );
        geometry->VertexList[i].z = r * -sin( theta );
        phi += 2 * PI / 5.0;
    }
    phi = 0;
    for( int i = 6; i < 11; i++ ) {
        geometry->VertexList[i].x = r * cos( theta ) * cos( phi );
        geometry->VertexList[i].y = r * cos( theta ) * sin( phi );
        geometry->VertexList[i].z = r * sin( theta );
        phi += 2 * PI / 5.0;
    }

    // Do the poles
    geometry->VertexList[0].x = 0.0;
    geometry->VertexList[0].y = 0.0;
    geometry->VertexList[0].z = -r;
    geometry->VertexList[11].x = 0.0;
    geometry->VertexList[11].y = 0.0;
    geometry->VertexList[11].z = r;
    

    static const face IcosahadronFaces[] =
    { 
        { 0, 2, 1, 0 },
        { 0, 3, 2, 0 },
        { 0, 4, 3, 0 },
        { 0, 5, 4, 0 },
        { 0, 1, 5, 0 },

        { 1, 2, 7, 0 },
        { 2, 3, 8, 0 },
        { 3, 4, 9, 0 },
        { 4, 5, 10, 0 },
        { 5, 1, 6, 0 },

        { 1, 7, 6, 0 },
        { 2, 8, 7, 0 },
        { 3, 9, 8, 0 },
        { 4, 10,9, 0 },
        { 5, 6, 10, 0 },

        { 6, 7, 11, 0 },
        { 7, 8, 11, 0 },
        { 8, 9, 11, 0 },
        { 9, 10,11, 0 },
        { 10,6, 11, 0 },
    };

    //
    // Faces
    // 
    geometry->FaceList = (face*)malloc( 20*sizeof(face) );
    geometry->NumFaces = 20;
    memcpy( geometry->FaceList, &IcosahadronFaces, 20*sizeof(face) );
}
