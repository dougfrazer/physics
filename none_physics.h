
#include "physics.h"

class NONE_PHYSICS : public PHYSICS
{
public:
    NONE_PHYSICS( GEOMETRY* Geo ) : PHYSICS( Geo ) {}
    ~NONE_PHYSICS() {}

public:
    void Update( float DeltaTime ) {}
    void Reset() {}
};
