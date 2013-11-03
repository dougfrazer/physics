#ifndef __PLANE_H__
#define __PLANE_H__

#include "vector3.h"
#include "matrix.h"

class plane {
public:
    vector3 a;
    vector3 b;
    vector3 c;

public:
    plane( vector3 x, vector3 y, vector3 z ) {
        a = x;
        b = y;
        c = z;
    }
    vector3 intersection( const vector3 A, const vector3 B ) const {
        vector3 v( A.x - a.x, A.y - a.y, A.z - a.z );
        matrix3 m( A.x - B.x, b.x - a.x, c.x - a.x,
                   A.y - B.y, b.y - a.y, c.y - a.y,
                   A.z - B.z, b.z - a.z, c.z - a.z );
        return m.inv() * v;
    }
};

#endif
