#include "physics.h"
#include "geometry.h"
#include <vector>

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
    int NumObjects = PhysicsList.size();
    
    // Update all objects positions
    for( int i = 0; i < NumObjects; i++ ) {
        PhysicsList.at(i)->Update( DeltaTime ); 
    }

    // For each object, tell it to "apply whatever forces are necessary to get it out of each collision"
    bool Collision = false;
    for( int i = 0; i < NumObjects; i++ ) {
        for( int j = 0; j < NumObjects; j++ ) {
            if( j == i ) continue; // can't collide with yourself
            PHYSICS* a = PhysicsList.at(i);
            PHYSICS* b = PhysicsList.at(j);
            bool Colliding = a->DetectCollision( b->Geometry );
            if( Colliding ) {
                a->HandleCollision( b->Geometry );
            }
        }
    }

    // Apply pending geometry
    for( int i = 0; i < NumObjects; i++) {
        *PhysicsList.at(i)->Geometry = *PhysicsList.at(i)->Pending;
    }
}
