#include "common.h"
#include <vector>

class GEOMETRY;

class PHYSICS
{
public:
    PHYSICS( GEOMETRY* Geo );
    virtual ~PHYSICS();

public:
    // Do anything here that we would need to at the beginning of each frame
    //virtual void      Preupdate       ( void ) = 0;

    // Do the actual update, storing the pending geometry 
    virtual void      Update          ( const float DeltaTime ) = 0;

    // Using a bounding-box, will my object collide with anything this frame
    // this is meant to be an INCREDIBLY cheap and rough test, because most of the time objects
    // are not colliding
    // TODO
    //virtual bool      BoundingTest    ( const float DeltaTime, const GEOMETRY* B ) = 0;

    // This is meant to be a more precise collision detection 
    virtual bool      DetectCollision ( const GEOMETRY* A, const GEOMETRY* B ) const = 0;
    virtual void      HandleCollision ( const GEOMETRY* B_Current, const GEOMETRY* B_Pending, const float DeltaTime ) = 0;

    // Cleanup anything post-update
    //virtual void      Postupdate      ( void ) = 0; 

    // General reset (utility function)
    virtual void      Reset           ( void ) = 0;

public:
    GEOMETRY* Geometry;
    GEOMETRY* Pending;
};

void Physics_Update( float DeltaTime );
