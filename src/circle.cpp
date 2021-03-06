#include "circle.h"
#include "geometry.h"
#include "rigid_physics.h"
#include "world.h" // temporary

#include "stdlib.h"
#include "math.h"
#include "string.h"


#if defined( __APPLE__ )
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static const vector3 InitialPosition( 0.0, 40.0, 0.0 );
static const vector3 InitialRotation( 0.0, 0.0, 0.0 );

CIRCLE::CIRCLE(float r)
{
    radius = r;

    // define geometry (todo: this should be actual data)
    Geometry = new GEOMETRY();
    CreateIcosahadron(5 , Geometry , 0);
    Geometry->Position = InitialPosition;

    // attach physics
    Physics = new RIGID_PHYSICS( Geometry );
}

CIRCLE::~CIRCLE()
{

}

void CIRCLE::Reset()
{
    Geometry->Position = InitialPosition;
    Geometry->Rotation = InitialRotation;
    Physics->Reset();
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
	transform.translate( Geometry->Position );
	transform.rotate( Geometry->Rotation );
	transform = transform.t(); // needs to be in column-major form
	transform = transform * matrix4( (float*)modelMatrix );

	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( (GLfloat*)&transform );

	// draw something at the center so it can be distinguished
	glColor3f(1.0,0.0,0.0);
	glutWireSphere( 1, 5, 5 );
	
	glColor3f(0.0,0.0,1.0);
    glBegin( GL_LINE_STRIP );
        for( int i = 0; i < Geometry->NumFaces; i++ ) {
            glVertex3fv((GLfloat*)&Geometry->VertexList[ Geometry->FaceList[ i ].v1 ]);
            glVertex3fv((GLfloat*)&Geometry->VertexList[ Geometry->FaceList[ i ].v2 ]);
            glVertex3fv((GLfloat*)&Geometry->VertexList[ Geometry->FaceList[ i ].v3 ]);
        } 
    glEnd();

	/*
	matrix4 transform;
	transform.translate( Geometry->Position );
	transform.rotate( Geometry->Rotation );
	for( uint i = 0; i < Physics->Pending->NumVertices; i++ ) {
		Physics->Pending->VertexList[i] = transform * Geometry->VertexList[i];
	}

	glColor3f(0.0,0.0,1.0);
	glBegin( GL_LINE_STRIP );
	for( int i = 0; i < Geometry->NumFaces; i++ ) {
		glVertex3fv((GLfloat*)&Physics->Pending->VertexList[ Geometry->FaceList[ i ].v1 ]);
		glVertex3fv((GLfloat*)&Physics->Pending->VertexList[ Geometry->FaceList[ i ].v2 ]);
		glVertex3fv((GLfloat*)&Physics->Pending->VertexList[ Geometry->FaceList[ i ].v3 ]);
	} 
	glEnd();
	*/

    glPopMatrix();

/*
    vector3 vel = Geometry->Position + ((RIGID_PHYSICS*)Physics)->v;
    vector3 rot = Geometry->Position + ((RIGID_PHYSICS*)Physics)->w;
    glColor3f( 0.0, 1.0, 0.0 ); 
    glBegin( GL_LINES );
        glVertex3fv( (GLfloat*)&Geometry->Position );
        glVertex3fv( (GLfloat*)&vel  );
    glEnd();
    glColor3f( 0.0, 1.0, 1.0 );
    glBegin( GL_LINES );
        glVertex3fv( (GLfloat*)&Geometry->Position );
        glVertex3fv( (GLfloat*)&rot  );
    glEnd();
*/
	glPopAttrib();
}

//
// Based off: 
// http://www.fho-emden.de/~hoffmann/ikos27042002.pdf
//
void CIRCLE::CreateIcosahadron(float r, GEOMETRY* Geometry, int subdivision)
{
    // OK, this is just a quick-and-dirty thing to get it working
    // An icosahadron is 2 pentagrams and 2 single points, that looks kind of like a circle
    // we can use this basic shape to subdivide and return a more accurate sphere... maybe later
    
    static const float theta = 26.56505117707799 * PI / 180.0;


    //
    // Vertices
    // 
    Geometry->VertexList = (vertex*)malloc( 12*sizeof(vertex) );
    Geometry->NumVertices = 12;
    float phi = 0;
    for( int i = 1; i < 6; i++ ) {
        Geometry->VertexList[i].x = r * cos( theta ) * cos( phi );
        Geometry->VertexList[i].y = r * cos( theta ) * sin( phi );
        Geometry->VertexList[i].z = r * -sin( theta );
        phi += 2 * PI / 5.0;
    }
    phi = 0;
    for( int i = 6; i < 11; i++ ) {
        Geometry->VertexList[i].x = r * cos( theta ) * cos( phi );
        Geometry->VertexList[i].y = r * cos( theta ) * sin( phi );
        Geometry->VertexList[i].z = r * sin( theta );
        phi += 2 * PI / 5.0;
    }

    // Do the poles
    Geometry->VertexList[0].x = 0.0;
    Geometry->VertexList[0].y = 0.0;
    Geometry->VertexList[0].z = -r;
    Geometry->VertexList[11].x = 0.0;
    Geometry->VertexList[11].y = 0.0;
    Geometry->VertexList[11].z = r;
    

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
    Geometry->FaceList = (face*)malloc( 20*sizeof(face) );
    Geometry->NumFaces = 20;
    memcpy( Geometry->FaceList, &IcosahadronFaces, 20*sizeof(face) );
}
