#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "object.h"
#include "common.h"

class CIRCLE : public OBJECT
{
public:
    CIRCLE(float r);
    ~CIRCLE();
public:
    void Update    ( float DeltaTime );
    void Draw      ( void );
    void Reset     ( void );
private:
    float       radius;

    void CreateIcosahadron(float r, GEOMETRY* Geometry, int subdivision);
};

#endif
