#include "physics.h"
#include "geometry.h"
#include <vector>

std::vector<GEOMETRY*> GeoList;

PHYSICS::PHYSICS( GEOMETRY* Geo )
{
    Geometry = Geo;
    GeoList.push_back( Geo ); 
}

GEOMETRY* PHYSICS::GetGeo( int i )
{
    return GeoList.at(i);
}

