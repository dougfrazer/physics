#include "rigid_physics.h"
#include "geometry.h"

static const float g = 0.1;
static const vector4 StartingVelocity = { 0.0, 0.0, 0.0, 0.0 };

RIGID_PHYSICS::RIGID_PHYSICS( GEOMETRY* Geo, vertex* com )
    : PHYSICS( Geo )
{
    x = com;
    Reset();
}

RIGID_PHYSICS::~RIGID_PHYSICS()
{
}

void RIGID_PHYSICS::Reset()
{
    v = StartingVelocity;
}

void RIGID_PHYSICS::Update( float DeltaTime )
{
    bool Collision = DetectCollision();
    if( !Collision ) {
        v.y += g * DeltaTime;
        x->y -= v.y * DeltaTime;
    }
}

bool RIGID_PHYSICS::DetectCollision()
{
    return false;
}
