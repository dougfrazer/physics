
#include "physics.h"

class STATIC_PHYSICS : public PHYSICS
{
public:
    STATIC_PHYSICS( GEOMETRY* Geo ) : PHYSICS( Geo ) {}
    ~STATIC_PHYSICS() {}

public:
//    void Update( const float DeltaTime ) {}
//    void TryUpdate( const float deltaTime ) {}
    void Reset() {}
protected:
//    bool DetectCollision( const GEOMETRY* A, const GEOMETRY* B ) const { return false; }
//    void HandleCollision( const GEOMETRY* Incoming, const float DeltaTime ) { }
//    void ApplyCollisionResponse( const GEOMETRY* B ) { }
};
