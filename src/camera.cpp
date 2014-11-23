#if defined( __APPLE__ )
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "camera.h"
#include "vector3.h"
#include "common.h"

///////////////////////////////////////////////////////////////////////////////
// Static Data
///////////////////////////////////////////////////////////////////////////////
static const vector3 StartingPosition( 60.0, 15.0, 0.0  );
static const vector3 StartingLook    ( 0.0 , 15.0, 0.0  );
static const vector3 StartingOrbit   ( 0.0 , 0.0 , 0.0  );

static struct {
    vector3      Position;
    vector3      Look;
    float        Degrees;
    vector3      orbit;
    unsigned int Pressed;
} CameraData;

///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////
void Camera_Init( void )
{
    Camera_Reset();
}

void Camera_Deinit( void )
{
}

void Camera_Draw( void )
{
    gluLookAt( CameraData.Position.x, CameraData.Position.y, CameraData.Position.z,
               CameraData.Look.x, CameraData.Look.y, CameraData.Look.z,
               0.0, 1.0, 0.0 ); 
	glRotatef( (int)CameraData.orbit.x % 360, 1.0, 0.0, 0.0 );
	glRotatef( (int)CameraData.orbit.y % 360, 0.0, 1.0, 0.0 );
	glRotatef( (int)CameraData.orbit.z % 360, 0.0, 0.0, 1.0 );
}

#define CHECK_PRESSED(flag) ((CameraData.Pressed & 1<<flag) != 0)
void Camera_Update( float DeltaTime )
{
    if( CHECK_PRESSED(FORWARD) )   { CameraData.Position.x -= 0.02; CameraData.Look.x -= 0.02; }
    if( CHECK_PRESSED(BACK) )      { CameraData.Position.x += 0.02; CameraData.Look.x += 0.02; }
    if( CHECK_PRESSED(LEFT) )      { CameraData.Position.z += 0.02; CameraData.Look.z += 0.02; }
    if( CHECK_PRESSED(RIGHT) )     { CameraData.Position.z -= 0.02; CameraData.Look.z -= 0.02; }
    if( CHECK_PRESSED(UP) )        { CameraData.Position.y -= 0.02; CameraData.Look.y -= 0.02; }
    if( CHECK_PRESSED(DOWN) )      { CameraData.Position.y += 0.02; CameraData.Look.y += 0.02; }
    if( CHECK_PRESSED(ROTATE_LEFT) )  { CameraData.orbit.y += 0.1; }
    if( CHECK_PRESSED(ROTATE_RIGHT) ) { CameraData.orbit.y -= 0.1; }
    if( CHECK_PRESSED(ROTATE_UP) )    { CameraData.orbit.x += 0.1; }
    if( CHECK_PRESSED(ROTATE_DOWN) )  { CameraData.orbit.x -= 0.1; }
}

void Camera_StartMove(CAMERA_DIRECTION dir)
{
    CameraData.Pressed |= 1<<dir;
}

void Camera_StopMove(CAMERA_DIRECTION dir)
{
    CameraData.Pressed &= ~(1<<dir);
}


void Camera_Reset()
{
    CameraData.Position = StartingPosition;
    CameraData.Look = StartingLook;
    CameraData.orbit = StartingOrbit;
}
