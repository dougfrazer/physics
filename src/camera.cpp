#include "camera.h"
#include "world.h"
#include "vector3.h"

#include "common.h"
#if __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

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

static void releaseKey(int key, int x, int y);
static void pressKey(int key, int x, int y);
static void processNormalKeys(unsigned char key, int x, int y);
static void releaseNormalKeys(unsigned char key, int x, int y);
static void Camera_Reset();

enum KEYBOARD_EVENTS
{
    FORWARD,
    BACK,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    PAN_DOWN,
    PAN_UP,
    PAN_LEFT,
    PAN_RIGHT,
};


void Camera_Init( void )
{
    Camera_Reset();

    // Glut Input Commands
    glutIgnoreKeyRepeat(1);
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    glutKeyboardFunc(processNormalKeys);
    glutKeyboardUpFunc(releaseNormalKeys);
}

void Camera_Deinit( void )
{
}

void Camera_Draw( void )
{
    gluLookAt( CameraData.Position.x, CameraData.Position.y, CameraData.Position.z,
               CameraData.Look.x, CameraData.Look.y, CameraData.Look.z,
               0.0, 1.0, 0.0 ); 
    glRotatef( (int)CameraData.orbit.y % 360, 0.0, 1.0, 0.0 );
    glRotatef( (int)CameraData.orbit.z % 360, 0.0, 0.0, 1.0 );
}

void Camera_Update( float DeltaTime )
{
    if( CameraData.Pressed & 1<<FORWARD )   { CameraData.Position.x -= 0.1; CameraData.Look.x -= 0.1; }
    if( CameraData.Pressed & 1<<BACK )      { CameraData.Position.x += 0.1; CameraData.Look.x += 0.1; }
    if( CameraData.Pressed & 1<<LEFT )      { CameraData.Position.z += 0.1; CameraData.Look.z += 0.1; }
    if( CameraData.Pressed & 1<<RIGHT )     { CameraData.Position.z -= 0.1; CameraData.Look.z -= 0.1; }
    if( CameraData.Pressed & 1<<UP )        { CameraData.Position.y -= 0.1; CameraData.Look.y -= 0.1; }
    if( CameraData.Pressed & 1<<DOWN )      { CameraData.Position.y += 0.1; CameraData.Look.y += 0.1; }
    if( CameraData.Pressed & 1<<PAN_LEFT )  { CameraData.orbit.y += 0.1; }
    if( CameraData.Pressed & 1<<PAN_RIGHT ) { CameraData.orbit.y -= 0.1; }
    if( CameraData.Pressed & 1<<PAN_UP )    { CameraData.orbit.z += 0.1; }
    if( CameraData.Pressed & 1<<PAN_DOWN )  { CameraData.orbit.z -= 0.1; }
}

void pressKey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:  CameraData.Pressed |= 1<< PAN_LEFT  ; break;
    case GLUT_KEY_RIGHT: CameraData.Pressed |= 1<< PAN_RIGHT ; break;
    case GLUT_KEY_UP:    CameraData.Pressed |= 1<< PAN_UP    ; break;
    case GLUT_KEY_DOWN:  CameraData.Pressed |= 1<< PAN_DOWN  ; break;
    }
}

void releaseKey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:  CameraData.Pressed &= ~(1<< PAN_LEFT)  ; break;
    case GLUT_KEY_RIGHT: CameraData.Pressed &= ~(1<< PAN_RIGHT) ; break;
    case GLUT_KEY_UP:    CameraData.Pressed &= ~(1<< PAN_UP)    ; break;
    case GLUT_KEY_DOWN:  CameraData.Pressed &= ~(1<< PAN_DOWN)  ; break;
    }

}

void processNormalKeys(unsigned char key, int x, int y)
{
    switch( key )
    {
    case 'w': CameraData.Pressed |= 1<<FORWARD; break;
    case 'a': CameraData.Pressed |= 1<<LEFT;    break;
    case 's': CameraData.Pressed |= 1<<BACK;    break;
    case 'd': CameraData.Pressed |= 1<<RIGHT;   break;
    case 'q': CameraData.Pressed |= 1<<UP;      break;
    case 'e': CameraData.Pressed |= 1<<DOWN;    break;
    case 'r': World_Reset(); Camera_Reset();    break;
    case ' ': World_Pause();                    break;
    case 'p': World_Pause(); World_Update( 0.0166666666 ); World_Pause(); break;
    }
}

void releaseNormalKeys(unsigned char key, int x, int y)
{
    switch( key )
    {
    case 'w': CameraData.Pressed &= ~(1<<FORWARD); break;
    case 'a': CameraData.Pressed &= ~(1<<LEFT)   ; break;
    case 's': CameraData.Pressed &= ~(1<<BACK)   ; break;
    case 'd': CameraData.Pressed &= ~(1<<RIGHT)  ; break;
    case 'q': CameraData.Pressed &= ~(1<<UP)     ; break;
    case 'e': CameraData.Pressed &= ~(1<<DOWN)   ; break;
    }
}

void Camera_Reset()
{
    CameraData.Position = StartingPosition;
    CameraData.Look = StartingLook;
    CameraData.orbit = StartingOrbit;
}
