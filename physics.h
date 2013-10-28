#include "common.h"

class GEOMETRY;

class PHYSICS
{
public:
    PHYSICS( GEOMETRY* Geo );
    virtual ~PHYSICS() {}

public:
    virtual void      Update          ( float DeltaTime ) = 0;
    virtual void      Reset           ( void ) = 0;
    virtual bool      DetectCollision ( GEOMETRY* Object ) = 0;
    virtual void      HandleCollision ( GEOMETRY* Object ) = 0;
    GEOMETRY* Geometry;
};

void Physics_Update( float DeltaTime );
