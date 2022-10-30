#include "square.h"
#include "geometry.h"
#include "stdlib.h"
#include "static_physics.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>


SQUARE::SQUARE(float s)
{
    size = s;
    Geometry = new GEOMETRY();    
    Geometry->NumVertices = 4;
    Geometry->VertexList = (vertex*)malloc( 4*sizeof(vertex) );
    Geometry->VertexList[0].x = -s;
    Geometry->VertexList[0].y = 0.0;
    Geometry->VertexList[0].z = -s;

    Geometry->VertexList[1].x = s;
    Geometry->VertexList[1].y = 0.0;
    Geometry->VertexList[1].z = -s;

    Geometry->VertexList[2].x = s;
    Geometry->VertexList[2].y = 0.0;
    Geometry->VertexList[2].z = s;

    Geometry->VertexList[3].x = -s;
    Geometry->VertexList[3].y = 0.0;
    Geometry->VertexList[3].z = s;

    Physics = new STATIC_PHYSICS( Geometry );
}

SQUARE::~SQUARE()
{

}

void SQUARE::Reset()
{

}

void SQUARE::Update( float DeltaTime )
{

}
void SQUARE::Draw( void )
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
    glTranslatef( 0.0, 0.0, 0.0 );
	glColor3f( 1, 1, 1 );
    glBegin( GL_LINE_STRIP );
		for( int i = -size; i < size; i++ ) {
			if( i % 2 ) {
				glVertex3f( -size, 0,  i );
				glVertex3f(  size, 0,  i );
			} else {
				glVertex3f(  size, 0,  i );
				glVertex3f( -size, 0,  i );
			}
		}
		for( int i = -size; i < size; i++ ) {
			if( i % 2 ) {
				glVertex3f( -size, 0, -i );
				glVertex3f(  size, 0, -i );
			} else {
				glVertex3f(  size, 0, -i );
				glVertex3f( -size, 0, -i );
			}
		}
		for( int i = -size; i < size; i++ ) {
			if( i % 2 ) {
				glVertex3f( -i, 0,  size );
				glVertex3f(  i, 0,  size );
			} else {
				glVertex3f(  i, 0,  size );
				glVertex3f( -i, 0,  size );
			}
		}
		for( int i = -size; i < size; i++ ) {
			if( i % 2 ) {
				glVertex3f( -i, 0, -size );
				glVertex3f(  i, 0, -size );
			} else {
				glVertex3f(  i, 0, -size );
				glVertex3f( -i, 0, -size );
			}
		}
    glEnd();

	glPopAttrib();
	glPopMatrix();
}
