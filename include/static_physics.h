
#include "physics.h"

class STATIC_PHYSICS : public PHYSICS
{
public:
    STATIC_PHYSICS( GEOMETRY* Geo ) : PHYSICS( Geo ) {}
    ~STATIC_PHYSICS() {}

public:
    void Update( const float DeltaTime ) {}
    void Reset() {}
protected:
    bool DetectCollision( const GEOMETRY* Incoming ) { return false; }
    void HandleCollision( const GEOMETRY* Incoming ) { }
};