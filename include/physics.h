#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "common.h"
#include "vector3.h"
#include <vector>

class GEOMETRY;

class PHYSICS
{
public:
    PHYSICS( GEOMETRY* Geo );
    virtual ~PHYSICS();

public:
    // General reset (utility function)
    virtual void      Reset           ( void ) = 0;
    virtual vector3   GetGravity      ( void ) = 0;

public:
    GEOMETRY* Geometry;
    GEOMETRY* Pending;
};

void Physics_Update( float DeltaTime );


#endif
