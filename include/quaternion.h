#include "vector3.h"
#include <cmath>


class quaternion
{
public:
    float x;
    float y;
    float z;
    float w;

    quaternion();
    quaternion( float _x, float _y, float _z, float _w );
    quaternion( vector3 axis, float angle );

    quaternion operator*( quaternion B ) const;

    quaternion normalize() const;
    float      magnitude() const;
    quaternion inverse()   const;
};
