#include "physics.h"
#include "vector3.h"
#include "matrix.h"
#include <vector>


void RigidPhysics_TryUpdate(GEOMETRY* To, const GEOMETRY* From, float DeltaTime);
bool RigidPhysics_DetectCollision(const GEOMETRY* A, const GEOMETRY* B);
void RigidPhysics_ApplyCollisionResponse(GEOMETRY* A, const GEOMETRY* B);


class RIGID_PHYSICS : public PHYSICS
{
public:
    RIGID_PHYSICS( GEOMETRY* Geometry );
    ~RIGID_PHYSICS();

public:
    void Reset( );
protected:
// TODO: private
public:
    vertex*   x; // position (center of mass)

private:

};
