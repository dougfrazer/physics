
#include "physics.h"

class STATIC_PHYSICS : public PHYSICS
{
public:
    STATIC_PHYSICS( GEOMETRY* Geo ) : PHYSICS( Geo ) {}
    ~STATIC_PHYSICS() {}

public:
    void Reset() {}
protected:
};
