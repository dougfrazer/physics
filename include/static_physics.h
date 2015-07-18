
#include "physics.h"

class STATIC_PHYSICS : public PHYSICS
{
public:
    STATIC_PHYSICS( GEOMETRY* Geo ) : PHYSICS( Geo ) {}
    ~STATIC_PHYSICS() {}

public:
    void Reset() {}
    vector3 GetGravity() { return vector3(0.0f,0.0f,0.0f); } // MAJOR HACK
protected:
};
