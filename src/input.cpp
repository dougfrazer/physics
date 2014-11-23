#include "input.h"
#include "world.h"
#include "camera.h"
#include "debug.h"

#if defined( __APPLE__ )
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// Forward Declarations
///////////////////////////////////////////////////////////////////////////////
static void releaseKey(int key, int x, int y);
static void pressKey(int key, int x, int y);
static void processNormalKeys(unsigned char key, int x, int y);
static void releaseNormalKeys(unsigned char key, int x, int y);

///////////////////////////////////////////////////////////////////////////////
// Public functions
///////////////////////////////////////////////////////////////////////////////
void Input_Init()
{
    // Glut Input Commands
    glutIgnoreKeyRepeat(1);
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    glutKeyboardFunc(processNormalKeys);
    glutKeyboardUpFunc(releaseNormalKeys);
}

///////////////////////////////////////////////////////////////////////////////
// Private functions
///////////////////////////////////////////////////////////////////////////////

void pressKey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:  Camera_StartMove(ROTATE_LEFT)  ; break;
    case GLUT_KEY_RIGHT: Camera_StartMove(ROTATE_RIGHT) ; break;
    case GLUT_KEY_UP:    Camera_StartMove(ROTATE_UP)    ; break;
    case GLUT_KEY_DOWN:  Camera_StartMove(ROTATE_DOWN)  ; break;
    }
}

void releaseKey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:  Camera_StopMove(ROTATE_LEFT)  ; break;
    case GLUT_KEY_RIGHT: Camera_StopMove(ROTATE_RIGHT) ; break;
    case GLUT_KEY_UP:    Camera_StopMove(ROTATE_UP)    ; break;
    case GLUT_KEY_DOWN:  Camera_StopMove(ROTATE_DOWN)  ; break;
    }

}

void processNormalKeys(unsigned char key, int x, int y)
{
    switch( key )
    {
    case 'w': Camera_StartMove(FORWARD); break;
    case 'a': Camera_StartMove(LEFT);    break;
    case 's': Camera_StartMove(BACK);    break;
    case 'd': Camera_StartMove(RIGHT);   break;
    case 'q': Camera_StartMove(UP);      break;
    case 'e': Camera_StartMove(DOWN);    break;
    case 'r': World_Reset(); Camera_Reset();    break;
    case ' ': World_Pause();                    break;
    case 'p': World_Pause(); World_Update( 0.0166666666 ); World_Pause(); break;
    case 'c': Debug_ClearLines(); break;
    }
}

void releaseNormalKeys(unsigned char key, int x, int y)
{
    switch( key )
    {
    case 'w': Camera_StopMove(FORWARD); break;
    case 'a': Camera_StopMove(LEFT)   ; break;
    case 's': Camera_StopMove(BACK)   ; break;
    case 'd': Camera_StopMove(RIGHT)  ; break;
    case 'q': Camera_StopMove(UP)     ; break;
    case 'e': Camera_StopMove(DOWN)   ; break;
    }
}
