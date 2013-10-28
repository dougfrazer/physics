#include "physics.h"
#include "geometry.h"
#include <vector>

static std::vector<PHYSICS*> PhysicsList(0);

PHYSICS::PHYSICS( GEOMETRY* Geo )
{
    Geometry = Geo;
    PhysicsList.push_back( this ); 
}

void Physics_Update(float DeltaTime)
{
    int NumObjects = PhysicsList.size();
    
    // Update all objects positions
    for( int i = 0; i < NumObjects; i++ ) {
        PhysicsList.at(i)->Update( DeltaTime ); 
    }

    // For each object, tell it to "apply whatever forces are necessary to get it out of each collision"
    // and do all this until there are no collisions left
    bool Collision = false;
    do {
        Collision = false;
        for( int i = 0; i < NumObjects; i++ ) {
            for( int j = 0; j < NumObjects; j++ ) {
                if( j == i ) continue; // can't collide with yourself
                PHYSICS* a = PhysicsList.at(i);
                PHYSICS* b = PhysicsList.at(j);
                bool Colliding = a->DetectCollision( b->Geometry );
                if( Colliding ) {
                    a->CollisionPending = a->HandleCollision( b->Geometry );
                    Collision = true;
                }
            }
        }
    } while( false /* Collision */ );
}
