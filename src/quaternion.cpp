#include "quaternion.h"

quaternion::quaternion() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
    w = 0.0;
}

quaternion::quaternion( float _x, float _y, float _z, float _w ) {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

quaternion::quaternion( vector3 axis, float angle ) {
    float sin_theta = sin( angle / 2 );
    float cos_theta = cos( angle / 2 );
    vector3 n = axis.normalize();

    x = n.x * sin_theta;
    y = n.y * sin_theta;
    z = n.z * sin_theta;
    w = cos_theta;
}

quaternion quaternion::operator*( quaternion B ) const {
    quaternion q;
    q.x = w*B.x + x*B.w + y*B.z - z*B.y;
    q.y = w*B.y + y*B.w + z*B.x - x*B.z;
    q.z = w*B.z + z*B.w + x*B.y - y*B.x;
    q.w = w*B.w - x*B.x - y*B.y - z*B.z;
    return q;
}

quaternion quaternion::normalize() const {
    quaternion q;
    float m = magnitude();
    q.x = x / m;
    q.y = y / m;
    q.z = z / m;
    q.w = w / m;
    return q;
}

float quaternion::magnitude() const {
    return sqrt( x*x + y*y + z*z + w*w );
}

quaternion quaternion::inverse() const {
    quaternion q;
    q.x = -x;
    q.y = -y;
    q.z = -z;
    q.w = w;
    return q;
}
