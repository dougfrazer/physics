#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "object.h"
#include "common.h"

class GEOMETRY;

class SQUARE : public OBJECT
{
public:
    SQUARE(float s);
    ~SQUARE();
public:
    void Update    ( float DeltaTime );
    void Draw      ( void );
    void Reset     ( void );
private:
    float      size;
    vertex     position;
    GEOMETRY*  Geometry;
    PHYSICS*   Physics;
};

#endif
