#include "physics.h"
//#include "rigid_physics.h"
#include "geometry.h"
#include <vector>
#include <list>

static std::vector<PHYSICS*> s_physicsList(0);

PHYSICS::PHYSICS( GEOMETRY* Geo, const StaticPhysicsData& physicsData)
    : m_geometry(Geo)
    , m_static(physicsData)
{
    m_dynamic.m_position = physicsData.m_initialPosition;
    m_dynamic.m_rotation = physicsData.m_initialRotation;
    s_physicsList.push_back( this );
}

PHYSICS::~PHYSICS()
{
    // TODO: remove them from the list
}


//******************************************************************************
void ApplyImpulseResponse(PHYSICS* physics, CollisionData& data, bool pushOut)
{
    vector3 n = data.planeNormal;
    vector3 r = data.point - physics->m_dynamic.m_position;

    // Attempt to move the bodies out of the collision
    vector3 v = physics->m_dynamic.m_linearMomentum * (1.0f / physics->m_static.m_mass);
    vector3 w = physics->m_static.m_inverseInertiaTensor * physics->m_dynamic.m_angularMomentum;

    // Apply the forces to the bodies
    // rigid body impulse response
    vector3 velocityAtPoint = v + w.cross(r);
    float i = -(1.0 + physics->m_static.m_elasticity) * velocityAtPoint.dot(n);
    float k = (1.0f / physics->m_static.m_mass) + ((physics->m_static.m_inverseInertiaTensor * r.cross(n)).cross(r)).dot(n);
    float j = i / k;
    physics->m_dynamic.m_linearMomentum = physics->m_dynamic.m_linearMomentum + n * j;
    physics->m_dynamic.m_angularMomentum = physics->m_dynamic.m_angularMomentum + r.cross(n) * j;

    if (pushOut)
    {
        physics->m_dynamic.m_position = physics->m_dynamic.m_position + n * data.depth * 1.01f;
    }
}
//******************************************************************************
// pushOut is a temporary solution to resolve collisions where we adjust the position so its no longer
// pertruding... ideally we could make it so we can re-run the sim until there are no collisions
void OnCollision(Collision& data, bool pushOut) 
{
    switch (data.a->m_static.m_collisionResponseType)
    {
        case COLLISION_RESPONSE_IMPULSE:
        {
            ApplyImpulseResponse(const_cast<PHYSICS*>(data.a), data.data, pushOut);
        }
        default:
            break;
    }

    switch (data.b->m_static.m_collisionResponseType)
    {
        case COLLISION_RESPONSE_IMPULSE:
        {
            ApplyImpulseResponse(const_cast<PHYSICS*>(data.b), data.data, pushOut);
        }
        default:
            break;
    }
}
//******************************************************************************
std::vector<Collision> GetCollisions(std::vector<PHYSICS*> updateList)
{
    std::vector<Collision> collisions;
    for (auto it = updateList.begin(); it != updateList.end(); ++it)
    {
        for (auto innerIt = it; innerIt != updateList.end(); ++innerIt)
        {
            if (it == innerIt)
                continue; // can't collide with yourself

            Collision collision;
            if (DetectCollision(*it, *innerIt, &collision))
            {
                collisions.push_back(collision);
                break;
            }
        }
    }
    return collisions;
}

//******************************************************************************
void Physics_Update( float dt )
{
    // update the objects dynamic physics state for the time passed
    for(auto it = s_physicsList.begin(); it != s_physicsList.end(); ++it)
    {
        PHYSICS* phys = *it;
        phys->m_dynamic.Update(dt, phys->m_static);
    }

    // Check for collisions and apply responses to collisions
    std::vector<Collision> collisions = GetCollisions(s_physicsList);
    for (int i = 0; i < collisions.size(); i++)
    {
        OnCollision(collisions[i], true);
    }
}

//******************************************************************************
void PHYSICS::Reset()
{
    m_dynamic.m_position = m_static.m_initialPosition;
    m_dynamic.m_rotation = m_static.m_initialRotation;
    m_dynamic.m_angularMomentum = { 0.0f };
    m_dynamic.m_linearMomentum = { 0.0f };
}

//******************************************************************************
void DynamicPhysicsData::Update(float deltaTime, const StaticPhysicsData& state)
{
    // update for gravity
    if (!state.m_gravity.IsNone())
    {
        m_linearMomentum = m_linearMomentum + (state.m_gravity * state.m_mass) * deltaTime;
    }

    // should probably be more explicit... but assume massless objects don't move
    if(!FloatEquals(state.m_mass, 0.0f))
    {
        // compute linear and angular velocity
        vector3 v = m_linearMomentum * (1.0f / state.m_mass);
        vector3 w = state.m_inverseInertiaTensor * m_angularMomentum;

        // update position based on velocities
        m_position = m_position + v * deltaTime;
        m_rotation = m_rotation + w * deltaTime;
    }


#if DEBUG_ENERGY
    vector3 vel = To->p * 1 / m;
    vector3 rot = I_inv * To->L;
    float Ke_translate = 0.5f * m * vel.magnitude() * vel.magnitude();
    float Pe_gravity = m * g.magnitude() * From->Position.y;
    float Ke_rotate = 0.5f * moment_of_inertia * rot.magnitude() * rot.magnitude();
    printf("[DT=%f] Kinetic Energy: %f (tran=%f (P=%f,K=%f) | rot=%f)\n", DeltaTime, Ke_translate + Ke_rotate + Pe_gravity, Ke_translate + Pe_gravity, Pe_gravity, Ke_translate, Ke_rotate);
#endif
}