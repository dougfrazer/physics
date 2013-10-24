#ifndef __OBJECT_H__
#define __OBJECT_H__

class GEOMETRY;
class PHYSICS;

class OBJECT
{
public:
    virtual void Update ( float DeltaTime ) = 0;
    virtual void Draw   ( void ) = 0;
protected:
    GEOMETRY*  Geometry;
    PHYSICS*   Physics;
};


#endif
