#include "physics.h"
#include "vector.h"
#include <vector>

class RIGID_PHYSICS : public PHYSICS
{
public:
    RIGID_PHYSICS( GEOMETRY* Geometry );
    ~RIGID_PHYSICS();

public:
    void Update( float DeltaTime );
    void Reset( );
protected:
    bool DetectCollision( GEOMETRY* Incoming );
    GEOMETRY* HandleCollision( GEOMETRY* Incoming );

protected:
    vertex*  x;
    vector   v;

private:
    vector Support( const vector d, const GEOMETRY* Geo );
    vector Support( const vector d, const GEOMETRY* A, const GEOMETRY* B );
    bool   Simplex( std::vector<vector>* list, vector* d );
};
