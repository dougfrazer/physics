#if __LINUX__
#include <GL/glut.h>
#include <GL/freeglut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif
#include "time.h"

#include "world.h"
#include "camera.h"

//*****************************************************************************
// Forward Declarations
//*****************************************************************************
static time_t Main_GetDeltaTime ( void );
static void   Main_Update       ( float DeltaTime );
static void   Main_Draw         ( void );
static void   Main_Init         ( void );
static void   Main_Deinit       ( void );

//*****************************************************************************
// Public Interface
//*****************************************************************************
int main()
{
    float DeltaTime = 0.016666666666666666;
    struct timespec SleepReq, SleepRes;
    SleepReq.tv_sec = 0;

    Main_Init();
    while( true ) {
        DeltaTime = Main_GetDeltaTime();
        Main_Update( 0.16666666666666 ); 
        Main_Draw();
        SleepReq.tv_nsec = (DeltaTime)*(1000)*(1000)*(1000);
        nanosleep(&SleepReq, &SleepRes);
    }
    Main_Deinit();

    return 0;
}
//*****************************************************************************


//*****************************************************************************
// Private Interface
//*****************************************************************************
static time_t Main_GetDeltaTime()
{
    // TODO: calculate an actual time step
    return 0.0166666666666666666;
}
//*****************************************************************************
static void Main_Update( float DeltaTime )
{
    World_Update( DeltaTime );
    Camera_Update( DeltaTime );
    
    glutPostRedisplay();
#if __APPLE__
    glutCheckLoop();
#elif __LINUX__
    glutMainLoopEvent();
#endif
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
static void Main_Idle()
{
    glutSwapBuffers();
    glutPostRedisplay();
}
//*****************************************************************************
static void Main_InitGlut()
{
    int argc = 0;
    char *argv = NULL;
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
static void Main_Init()
{
    Main_InitGlut();
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
