#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "object.h"
#include "common.h"
#include "vector3.h"
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
    const float  m_radius;

    void CreateIcosahadron(float r, GEOMETRY* geometry, int subdivision);
};

#endif
