#include "debug.h"

#ifdef DEBUG

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>

struct line {
	vector3 a;
	vector3 b;
	Color c;
};

std::vector<line> debugLines;

//*****************************************************************************
void Debug_DrawLine( vector3 a, vector3 b, Color c )
{
	line l;
	l.a = a;
	l.b = b;
	l.c = c;
	debugLines.push_back(l);
}
//*****************************************************************************
void Debug_Draw()
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);

	int size = debugLines.size();
    glBegin( GL_LINES );
        for(int i = 0; i < size; i++ ) {
            line l = debugLines.at(i);
            glColor3f( l.c.GetR(), l.c.GetG(), l.c.GetB() );
                glVertex3fv( (GLfloat*)&l.a );
                glVertex3fv( (GLfloat*)&l.b );
        }
    glEnd();

	glPopAttrib();
	glPopMatrix();
}
//*****************************************************************************
void Debug_ClearLines()
{
    debugLines.clear();
}
//*****************************************************************************
void Debug_Printf(const char* s, ...)
{
    va_list argptr;
    va_start(argptr, s);
    vfprintf(stderr, s, argptr);
    va_end(argptr);
}

#endif // DEBUG
