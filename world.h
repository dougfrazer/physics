//*****************************************************************************
// World
// -----
//   The world is composed of a series of objects, each which has physics
//   attached to it.
// 
// @author Doug Frazer
// October 2013
//*****************************************************************************

class OBJECT;

void World_Init      ( void );
void World_Deinit    ( void );
void World_Draw      ( void );
void World_Update    ( float DeltaTime );

void World_AddObject    ( OBJECT* o );
void World_RemoveObject ( OBJECT* o );

// aggregious hack
void World_ResetCircle();
