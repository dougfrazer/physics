#pragma once

#include "vector3.h"
#include "color.h"

#define DEBUG 1

#ifdef DEBUG

void Debug_DrawLine( vector3 a, vector3 b, Color c );
void Debug_Draw();
void Debug_ClearLines();

void Debug_Printf(const char *, ...);

#else

#define Debug_DrawLine(...)    ((void)0)
#define Debug_Draw(...)        ((void)0)
#define Debug_ClearLines(...)  ((void)0)
#define Debug_Printf(...)      ((void)0)

#endif // DEBUG
