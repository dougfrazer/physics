
#include "physics.h"

class STATIC_PHYSICS : public PHYSICS
{
public:
    STATIC_PHYSICS( GEOMETRY* Geo ) : PHYSICS( Geo ) {}
    ~STATIC_PHYSICS() {}

public:
    void Update( float DeltaTime ) {}
    void Reset() {}
protected:
    bool DetectCollision( GEOMETRY* Incoming ) { return false; }
    GEOMETRY* HandleCollision( GEOMETRY* Incoming ) { return Geometry; }
};
