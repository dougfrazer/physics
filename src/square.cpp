#include "square.h"
#include "geometry.h"
#include "stdlib.h"
#include "physics.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

static const vector3 s_initalPosition = {0,0,0};
SQUARE::SQUARE(float s)
	: m_size(s)
{
    m_geometry = new GEOMETRY();    
    m_geometry->NumVertices = 4;
    m_geometry->VertexList = (vertex*)malloc( 4*sizeof(vertex) );
    m_geometry->VertexList[0].x = -s;
    m_geometry->VertexList[0].y = 0.0;
    m_geometry->VertexList[0].z = -s;

    m_geometry->VertexList[1].x = s;
    m_geometry->VertexList[1].y = 0.0;
    m_geometry->VertexList[1].z = -s;

    m_geometry->VertexList[2].x = s;
    m_geometry->VertexList[2].y = 0.0;
    m_geometry->VertexList[2].z = s;

    m_geometry->VertexList[3].x = -s;
    m_geometry->VertexList[3].y = 0.0;
    m_geometry->VertexList[3].z = s;

	StaticPhysicsData physData;
	physData.m_elasticity = 0.0f;
	physData.m_gravity = {0.0f,0.0f,0.0f};
	physData.m_collisionResponseType = COLLISION_RESPONSE_NONE;
	physData.m_initialPosition = { 0.0f };
	physData.m_initialRotation = { 0.0f };

    m_physics = new PHYSICS( m_geometry, physData);
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
		for( int i = -m_size; i < m_size; i++ ) {
			if( i % 2 ) {
				glVertex3f( -m_size, 0,  i );
				glVertex3f(  m_size, 0,  i );
			} else {
				glVertex3f(  m_size, 0,  i );
				glVertex3f( -m_size, 0,  i );
			}
		}
		for( int i = -m_size; i < m_size; i++ ) {
			if( i % 2 ) {
				glVertex3f( -m_size, 0, -i );
				glVertex3f(  m_size, 0, -i );
			} else {
				glVertex3f(  m_size, 0, -i );
				glVertex3f( -m_size, 0, -i );
			}
		}
		for( int i = -m_size; i < m_size; i++ ) {
			if( i % 2 ) {
				glVertex3f( -i, 0,  m_size );
				glVertex3f(  i, 0,  m_size );
			} else {
				glVertex3f(  i, 0,  m_size );
				glVertex3f( -i, 0,  m_size );
			}
		}
		for( int i = -m_size; i < m_size; i++ ) {
			if( i % 2 ) {
				glVertex3f( -i, 0, -m_size);
				glVertex3f(  i, 0, -m_size);
			} else {
				glVertex3f(  i, 0, -m_size);
				glVertex3f( -i, 0, -m_size);
			}
		}
    glEnd();

	glPopAttrib();
	glPopMatrix();
}
