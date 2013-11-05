#include "square.h"
#include "geometry.h"
#include "stdlib.h"
#include "static_physics.h"
#if __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


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
    glTranslatef( 0.0, 0.0, 0.0 );
    glBegin( GL_LINE_STRIP );
        glVertex3fv( (GLfloat*)&Geometry->VertexList[0] );
        glVertex3fv( (GLfloat*)&Geometry->VertexList[1] );
        glVertex3fv( (GLfloat*)&Geometry->VertexList[2] );
        glVertex3fv( (GLfloat*)&Geometry->VertexList[3] );
        glVertex3fv( (GLfloat*)&Geometry->VertexList[0] );
    glEnd();
}
