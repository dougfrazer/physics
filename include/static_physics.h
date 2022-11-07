
#include "physics.h"

class STATIC_PHYSICS : public PHYSICS
{
public:
    STATIC_PHYSICS( GEOMETRY* geometry, const vector3& initialLocation ) : PHYSICS(geometry, initialLocation) {}
    ~STATIC_PHYSICS() {}

public:
    void Reset(const vector3& pos, const vector3& rot) {}
    vector3 GetGravity() { return vector3(0.0f,0.0f,0.0f); } // MAJOR HACK
protected:
};
