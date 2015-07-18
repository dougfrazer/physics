#include "physics.h"
#include "vector3.h"
#include "matrix.h"
#include <vector>


void RigidPhysics_TryUpdate(GEOMETRY* To, const GEOMETRY* From, vector3 gravity, float DeltaTime);
bool RigidPhysics_DetectCollision(const GEOMETRY* A, const GEOMETRY* B);
void RigidPhysics_ApplyCollisionResponse(GEOMETRY* A, const GEOMETRY* B);


class RIGID_PHYSICS : public PHYSICS
{
public:
    RIGID_PHYSICS( GEOMETRY* Geometry );
    ~RIGID_PHYSICS();

public:
    void Reset( );
    vector3 GetGravity() { return vector3(0.0f,-9.8f,0.0f); } // MAJOR HACK
protected:
// TODO: private
public:
    vertex*   x; // position (center of mass)

private:

};
