#pragma once

#include "common.h"
#include "vector3.h"
#include "matrix.h"
#include <vector>

class GEOMETRY;

enum COLLISION_RESPONSE
{
    COLLISION_RESPONSE_NONE,
    COLLISION_RESPONSE_IMPULSE,
};
struct StaticPhysicsData
{
    vector3 m_gravity;
    vector3 m_initialPosition;
    vector3 m_initialRotation;

    float   m_mass = 0.0f;
    float   m_elasticity = 0.0f;
    COLLISION_RESPONSE m_collisionResponseType = COLLISION_RESPONSE_NONE;

    float   m_momentOfInertia = 0.0f;
    matrix3 m_inertiaTensor;
    matrix3 m_inverseInertiaTensor;
};
struct DynamicPhysicsData
{
    vector3   m_position;
    vector3   m_rotation;
    vector3   m_linearMomentum;
    vector3   m_angularMomentum;
    void Update(float deltaTime, const StaticPhysicsData& state);
};





class PHYSICS
{
public:
    PHYSICS( GEOMETRY* Geo, const StaticPhysicsData& physicsData );
    ~PHYSICS();

public:
    void      Reset();

public:
    GEOMETRY*          m_geometry;
    DynamicPhysicsData m_dynamic;
    const StaticPhysicsData  m_static;
};

void Physics_Update( float DeltaTime );


struct PhysUpdateState
{
    const GEOMETRY* m_geo;
    const StaticPhysicsData* m_static;
    DynamicPhysicsData m_state;
};
struct CollisionData
{
    vector3 point;
    vector3 planePoint;
    vector3 planeNormal;
    float   depth;
};

struct Collision
{
    const PHYSICS* a;
    const PHYSICS* b;
    CollisionData data;
};
bool DetectCollision(const PHYSICS* a, const PHYSICS* b, Collision* outCollision);
