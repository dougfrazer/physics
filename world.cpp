#include "world.h"
#include "object.h"
#include "circle.h"
#include "square.h"
#include <list>

std::list<OBJECT*> Objects;

static CIRCLE* Circle;
static SQUARE* Square;

void World_Init()
{
    Circle = new CIRCLE( 5 );
    Square = new SQUARE( 15 );
    World_AddObject( Circle );
    World_AddObject( Square );
}

void World_Deinit()
{

}

void World_Draw()
{
    std::list<OBJECT*>::iterator it;
    for( it = Objects.begin(); it != Objects.end(); it++ ) {
        OBJECT* o = *it;
        o->Draw();
    }
}

void World_ResetCircle()
{
    Circle->Reset();
}

void World_Update( float DeltaTime )
{
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
