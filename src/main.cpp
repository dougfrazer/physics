#if __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
//#include <GL/freeglut.h>
#endif

#if defined( _WIN32 ) || defined( _WIN64 )
#include <windows.h>
#include <strsafe.h>
#else
#include <sys/time.h>
#include <stdio.h>
#endif

#include "world.h"
#include "camera.h"
#include "common.h"
#include "debug.h"

#if defined(_WIN32) || defined(_WIN64)
static LARGE_INTEGER f;
static LARGE_INTEGER lastTime;
#else
static timeval lastTime;
#endif

//*****************************************************************************
// Forward Declarations
//*****************************************************************************
static float  Main_GetDeltaTime ( void );
static void   Main_Update       ( float DeltaTime );
static void   Main_Draw         ( void );
static void   Main_Init         ( int argc, char* argv );
static void   Main_Deinit       ( void );

//*****************************************************************************
// Public Interface
//*****************************************************************************
int main( int argc, char* argv[] )
{
    Main_Init( argc, argv[0] );

	glutMainLoop();

    Main_Deinit();

    return 0;
}
//*****************************************************************************


//*****************************************************************************
// Private Interface
//*****************************************************************************
static float Main_GetDeltaTime()
{
#if defined(_WIN32) || defined(_WIN64)
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	double DeltaTime = ( t.QuadPart - lastTime.QuadPart ) / (float)f.QuadPart;
	lastTime = t;
	char string[256];
	sprintf_s(string, 256, "[DeltaTime] %f\n", DeltaTime);
	OutputDebugString(string);
#else
	timeval t;
	gettimeofday(&t, NULL);
	double DeltaTime = (t.tv_sec - lastTime.tv_sec) + (t.tv_usec - lastTime.tv_usec) / (1000.0*1000.0);
    lastTime = t;
	printf("[DeltaTime] %f (%f FPS)\n", DeltaTime, 1/DeltaTime);
#endif

    return DeltaTime;
}
//*****************************************************************************
static void Main_Update( float DeltaTime )
{
    World_Update( DeltaTime );
    Camera_Update( DeltaTime );	
}
//*****************************************************************************
static void Main_Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    Camera_Draw();
    World_Draw();
	Debug_Draw();

    glutSwapBuffers();
}
//*****************************************************************************
static void Main_Reshape(int width, int height)
{
    if(height == 0) height = 1;
    float ratio = width * 1.0 / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}
//*****************************************************************************
static void Main_Idle()
{
	// Don't want to go any deltatimes greater than 1/30
    float DeltaTime = clamp( 0.0, Main_GetDeltaTime(), 1/30.0 );
	Main_Update( DeltaTime );

    glutPostRedisplay();
}
//*****************************************************************************
static void Main_InitGlut( int argc, char* argv )
{
    glutInit(&argc, &argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640,640);
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("Physics Test");
    
    glutDisplayFunc(Main_Draw);
    glutReshapeFunc(Main_Reshape);
    glutIdleFunc(Main_Idle);
}
//*****************************************************************************
static void Main_Init( int argc, char* argv )
{
    Main_InitGlut( argc, argv );
    World_Init();
    Camera_Init();

#if defined(_WIN32) || defined(_WIN64)
	QueryPerformanceFrequency(&f);
	QueryPerformanceCounter(&lastTime);
#else
	gettimeofday(&lastTime, NULL);
#endif
}
//*****************************************************************************
static void Main_Deinit()
{
    World_Deinit();
    Camera_Deinit();
}
//*****************************************************************************
