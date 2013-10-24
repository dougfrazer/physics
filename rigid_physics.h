#include "physics.h"

class RIGID_PHYSICS : public PHYSICS
{
public:
    RIGID_PHYSICS( GEOMETRY* Geometry, vertex* CenterOfMass );
    ~RIGID_PHYSICS();

public:
    void Update( float DeltaTime );
    void Reset( );

protected:
    vertex*  x;
    vector4  v;

private:
    bool DetectCollision();
};
