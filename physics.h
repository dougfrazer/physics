#include "common.h"

class GEOMETRY;

class PHYSICS
{
public:
    PHYSICS( GEOMETRY* Geo );
    virtual ~PHYSICS() {}

public:
    virtual void Update( float DeltaTime ) = 0;
    virtual void Reset() = 0;

protected:
    GEOMETRY* Geometry;
    
protected:
    static GEOMETRY* GetGeo( int i );
};

