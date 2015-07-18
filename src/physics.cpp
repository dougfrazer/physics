#include "physics.h"
#include "rigid_physics.h"
#include "geometry.h"
#include <vector>
#include <list>

static std::vector<PHYSICS*> PhysicsList(0);
static std::vector<GEOMETRY*> GeometryList(0);

PHYSICS::PHYSICS( GEOMETRY* Geo )
{
    Geometry = Geo;
    Pending = new GEOMETRY( Geo );
    PhysicsList.push_back( this );
    GeometryList.push_back( Geo );
}

PHYSICS::~PHYSICS()
{
    // remove them from the list
}

void Physics_Update( float DeltaTime )
{
    // 1.) Add all objects that need to be updated to the update list
    // 2.) Attempt to move all objects by deltaTime
    // 3.) If there are any collisions, do a binary search to find a deltaTime
    //     where there are no collisions, advance time that much, and apply the
    //     collision response to the objects that would collide.  Continue doing this
    //     until there are no collisions.
    // 3a.) (TODO) If an object does not have a collision on the first update,
    //      complete its update and remove it from the list (nothing is going to change).

    const float NumObjects = PhysicsList.size();
    std::list<PHYSICS*> updateList;
    for(int i = 0; i < NumObjects; ++i) {
        updateList.push_back(PhysicsList.at(i));
    }

    float percent = 0.0f; // start at 0% complete
    float currentPercent = 1.0f; // start trying a full update
    // the two objects that collided at the last iteration (if they exist)
    PHYSICS* collision_a = NULL;
    PHYSICS* collision_b = NULL;
    while(percent < 1.0f)
    {
        printf("Trying update %.4f%% , current = %.4f%%\n", currentPercent, percent);
        // Update as much as we think we can
        for(auto it = updateList.begin(); it != updateList.end(); ++it)
        {
            PHYSICS* physics = *it;
            RigidPhysics_TryUpdate(physics->Pending, physics->Geometry, physics->GetGravity(), currentPercent*DeltaTime);
        }

        // See if there are any collisions (for everything in the update list, for everything in the world)
        bool Collision = false;
        for( auto it = updateList.begin(); it != updateList.end(); ++it )
        {
            // TODO: can this be optimized to only be things in the update list?
            for( int i = 0; i < NumObjects; i++ )
            {
                PHYSICS* a = *it;
                PHYSICS* b = PhysicsList.at(i);
                if( a == b ) continue; // can't collide with yourself
                if( RigidPhysics_DetectCollision(a->Pending, b->Pending) ) {
                    Collision = true;
                    collision_a = a;
                    collision_b = b;
                    break;
                }
            }
            // TODO: support arbitrary number of collisions/objects
            if(Collision) break;
        }

        if( !Collision )
        {
            printf("Updating %f (%f total) percent...\n", currentPercent, percent);
            // apply all pending geometries (man this needs to get refactored...)
            for( int i = 0; i < NumObjects; i++)
            {
                *PhysicsList.at(i)->Geometry = *PhysicsList.at(i)->Pending;
            }
            // See if there are any collisions pending from last iteration
            if( collision_a )
            {
                assert(collision_b);
                RigidPhysics_ApplyCollisionResponse(collision_a->Geometry, collision_b->Geometry);
                collision_a = NULL;
                collision_b = NULL;
                // try to finish the rest of the frame now that we have resolved that collision
                percent += currentPercent;
                currentPercent = 1.0f - percent;
            }
            else
            {
                // No pending collisions, no current collision... we must have just finished
                percent += currentPercent;
                assert(percent == 1.0f);
            }
        }
        else
        {
            // well we had some sort of a collision... reduce our timestep to try and find one where we can safely update without a collision
            currentPercent = currentPercent / 2.0f;
        }
    }
}
