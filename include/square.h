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
    const float m_size;
    GEOMETRY*  m_geometry;
    PHYSICS*   m_physics;
};

#endif
