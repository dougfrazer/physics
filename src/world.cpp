#include "world.h"
#include "object.h"
#include "circle.h"
#include "square.h"
#include "physics.h"
#include <list>

std::list<OBJECT*> Objects;

static struct {
    bool Paused;
} WorldData;

void World_Init()
{
    WorldData.Paused = false;
    World_AddObject( new CIRCLE( 5 ) );
    World_AddObject( new SQUARE( 15 ) );
}

void World_Deinit()
{
    std::list<OBJECT*>::iterator it;
    for( it = Objects.begin(); it != Objects.end(); it++ ) {
        OBJECT* o = *it;
        delete( o );
    }
}

void World_Draw()
{
    std::list<OBJECT*>::iterator it;
    for( it = Objects.begin(); it != Objects.end(); it++ ) {
        OBJECT* o = *it;
        o->Draw();
    }
}

void World_Reset()
{
    std::list<OBJECT*>::iterator it;
    for( it = Objects.begin(); it != Objects.end(); it++ ) {
        OBJECT* o = *it;
        o->Reset();
    }
}

void World_Update( float DeltaTime )
{
    if( WorldData.Paused ) {
        return;
    }
	Physics_Update( DeltaTime );
    std::list<OBJECT*>::iterator it;
    for( it = Objects.begin(); it != Objects.end(); it++ ) {
        OBJECT* o = *it;
        o->Update( DeltaTime );
    }
}

void World_AddObject( OBJECT* o )
{
    Objects.push_front(o);
}

void World_RemoveObject( OBJECT* o )
{
    Objects.remove(o);
}

void World_Pause()
{
    WorldData.Paused = WorldData.Paused ? false : true;
}

bool World_GetPaused()
{
	return WorldData.Paused;
}