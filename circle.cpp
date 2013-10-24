#include "circle.h"
#include "geometry.h"
#include "rigid_physics.h"

#include "stdlib.h"
#include "math.h"
#include "string.h"

#include <GLUT/glut.h>

#define PI 3.14159265

CIRCLE::CIRCLE(float r)
{
    radius = r;

    // define geometry (todo: this should be actual data)
    Geometry = new GEOMETRY();
    CreateIcosahadron(5 , Geometry , 0);

    // attach physics
    Physics = new RIGID_PHYSICS( Geometry, &Position );

    // define center of mass (also, this should be in data)
    Reset();
}

CIRCLE::~CIRCLE()
{

}

void CIRCLE::Reset()
{
    Position.x = 0.0;
    Position.y = 40.0;
    Position.z = 0.0;
    Physics->Reset();
}

void CIRCLE::Update( float DeltaTime )
{
    // lets rotate it for testing
    Physics->Update( DeltaTime );
}

void CIRCLE::Draw( void )
{
    glTranslatef(Position.x, Position.y, Position.z);
    glBegin( GL_LINE_STRIP );
        for( int i = 0; i < Geometry->NumFaces; i++ ) {
            glVertex3fv((GLfloat*)&Geometry->VertexList[ Geometry->FaceList[ i ].v1 ]);
            glVertex3fv((GLfloat*)&Geometry->VertexList[ Geometry->FaceList[ i ].v2 ]);
            glVertex3fv((GLfloat*)&Geometry->VertexList[ Geometry->FaceList[ i ].v3 ]);
        } 
    glEnd();
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
