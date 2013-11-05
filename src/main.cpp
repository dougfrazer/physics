#if __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
//#include <GL/freeglut.h>
#endif

#if defined( _WIN32 ) || defined( _WIN64 )
#include <windows.h>
#endif

#include "time.h"
#include "world.h"
#include "camera.h"

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
int main( int argc, char* argv )
{
    Main_Init( argc, argv );

	glutMainLoop();

    Main_Deinit();

    return 0;
}
//*****************************************************************************


//*****************************************************************************
// Private Interface
//*****************************************************************************
#if defined(_WIN32) || defined(_WIN64)
static LARGE_INTEGER f;
static LARGE_INTEGER lastTime;
#endif

static float Main_GetDeltaTime()
{
#if defined(_WIN32) || defined(_WIN64)
	LARGE_INTEGER t;
	QueryPerformanceFrequency(&f);
	QueryPerformanceCounter(&t);
	float DeltaTime = ( t.QuadPart - lastTime.QuadPart ) * 1000.0 / f.QuadPart;
	lastTime = t;
#else
	 // TODO: calculate an actual time step
	float DeltaTime = 1/60.0;
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
#define clamp( min, x, max ) x < min ? min : x > max ? max : x
static void Main_Idle()
{
    float DeltaTime = clamp( 0.001, Main_GetDeltaTime(), 0.01666 );
	Main_Update( DeltaTime );

    glutPostRedisplay();
#if defined( __LINUX__ ) || defined( __APPLE__ )
	struct timespec SleepReq, SleepRes;
	SleepReq.tv_sec = 0;
	SleepReq.tv_nsec = (DeltaTime)*(1000)*(1000)*(1000);
	nanosleep(&SleepReq, &SleepRes);
#endif
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
}
//*****************************************************************************
static void Main_Deinit()
{
    World_Deinit();
    Camera_Deinit();
}
//*****************************************************************************
