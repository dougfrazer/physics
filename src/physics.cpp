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
#if 0
//******************************************************************************
std::vector<Collision> GetCollisions(std::vector<PhysUpdateState> updateList)
{
    std::vector<Collision> collisions;
    for (auto it = updateList.begin(); it != updateList.end(); ++it)
    {
        for (auto innerIt = it; innerIt != updateList.end(); ++innerIt)
        {
            if (it == innerIt) 
                continue; // can't collide with yourself

            PhysUpdateState& a = *it;
            PhysUpdateState& b = *innerIt;
            Collision collision;
            if (DetectCollision(a, b, &collision))
            {
                collisions.push_back(collision);
                break;
            }
        }
    }
    return collisions;
}
//******************************************************************************
static void Physics_Update_Iterative(float DeltaTime)
{
    // 1.) Add all objects that need to be updated to the update list
    // 2.) Attempt to move all objects by deltaTime
    // 3.) If there are any collisions, do a binary search to find a deltaTime
    //     where there are no collisions, advance time that much, and apply the
    //     collision response to the objects that would collide.  Continue doing this
    //     until we are done simulating the frametime.

    std::vector<PHYSICS*> updateObjects;
    std::vector<PhysUpdateState> updateStates;
    for (int i = 0; i < s_physicsList.size(); ++i)
    {
        updateStates.push_back({ s_physicsList.at(i)->m_geometry, &s_physicsList.at(i)->m_static, s_physicsList.at(i)->m_dynamic });
        updateObjects.push_back({ s_physicsList.at(i) });
    }

    float percent = 0.0f; // start at 0% complete
    float currentPercent = 1.0f; // start trying a full update
    std::vector<Collision> lastIterationCollisions;
    while (percent < 1.0f)
    {
        printf("Trying update %.4f%% , current = %.4f%%\n", currentPercent * 100.f, percent * 100.f);
        // copy over state before update
        std::vector<PhysUpdateState> updateStatesCopy = updateStates;

        // update states as much as we can
        for (auto it = updateStates.begin(); it != updateStates.end(); ++it)
        {
            it->m_state.Update(currentPercent * DeltaTime, *(it->m_static));
        }

        // See if there are any collisions (for everything in the update list, for everything in the world)
        std::vector<Collision> collisions = GetCollisions(updateStates);

        if (!collisions.size())
        {
            printf("Updating %f (%f total) percent...\n", currentPercent * 100.f, percent * 100.f);

            // See if there are any collisions pending from last iteration
            if (lastIterationCollisions.size())
            {
                for (int i = 0; i < lastIterationCollisions.size(); i++)
                {
                    OnCollision(lastIterationCollisions[i], false);
                }

                // try to finish the rest of the frame now that we have resolved that collision
                percent += currentPercent;
                currentPercent = 1.0f - percent;
            }
            else
            {
                // No pending collisions, no current collision... we must have just finished
                percent += currentPercent;
                assert(FloatEquals(percent, 1.0f));
            }
        }
        else
        {
            // well we had some sort of a collision... reduce our timestep to try and find one where we can safely update without a collision
            lastIterationCollisions = collisions;
            updateStates = updateStatesCopy;
            currentPercent = currentPercent / 2.0f;
            if (currentPercent < 0.001f)
            {
                currentPercent = 0.0f;
            }
        }
    }

    // copy our update states into the objects
    for (int i = 0; i < updateObjects.size(); i++)
    {
        updateObjects[i]->m_dynamic = updateStates[i].m_state;
    }
}
#endif
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