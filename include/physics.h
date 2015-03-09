#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "common.h"
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

public:
    GEOMETRY* Geometry;
    GEOMETRY* Pending;
};

void Physics_Update( float DeltaTime );


#endif
