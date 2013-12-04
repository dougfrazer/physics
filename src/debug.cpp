#include "debug.h"

#if __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>

struct line {
	vector3 a;
	vector3 b;
	Color c;
};

std::vector<line> debugLines;

void Debug_DrawLine( vector3 a, vector3 b, Color c )
{
	line l;
	l.a = a;
	l.b = b;
	l.c = c;
	debugLines.push_back(l);
}

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

void Debug_ClearLines()
{
    debugLines.clear();
}
