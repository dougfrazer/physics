#include "physics.h"
#include "vector3.h"
#include "matrix.h"
#include <vector>


void RigidPhysics_TryUpdate(GEOMETRY* To, const GEOMETRY* From, vector3 gravity, float DeltaTime);
bool RigidPhysics_DetectCollision(const GEOMETRY* A, const GEOMETRY* B, Collision* outCollision);
void RigidPhysics_ApplyCollisionResponse(Collision& collision);


class RIGID_PHYSICS : public PHYSICS
{
public:
    RIGID_PHYSICS( GEOMETRY* geometry, vector3 initialPosition );
    ~RIGID_PHYSICS();

public:
    void Reset(const vector3& pos);
    vector3 GetGravity() { return vector3(0.0f,-9.8f,0.0f); }
    bool DetectCollision(const GEOMETRY* otherGeo, const PhysicsData& otherPhys, Collision* outCollision) const override;
    void Update(float deltaTime) override;
    void ApplyCollisionResponse(CollisionData& collision) override;

};
