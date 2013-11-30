#include "test.h"
#include "vector3.h"
#include "math.h"

static const vector3 a( 8, 12, -23 );
static const vector3 b( -12, 1, 48 );

#define PI 3.1415926

//******************************************************************************
class TEST_NEGATE : public TEST
{
    public: TEST_NEGATE() : TEST( "Negate a 3d vector" ) {}
    protected: bool RunInternal()
    {
        vector3 v( 5.0, 10.0, -20.0 );
        vector3 n = -v;
        return n.x == -5.0 && n.y == -10.0 && n.z == 20.0;
    }
};
static TEST_NEGATE TestNegate;
//******************************************************************************
class TEST_SUBTRACT : public TEST
{
    public: TEST_SUBTRACT() : TEST( "Subtract two 3d vectors" ) {}
    protected: bool RunInternal()
    {
        vector3 c = a - b;
        return c.x == 20 && c.y == 11 && c.z == -71;
    }
};
static TEST_SUBTRACT TestSubtract;
//******************************************************************************
class TEST_ADD : public TEST
{
    public: TEST_ADD() : TEST( "Add two 3d vectors" ) {}
    protected: bool RunInternal()
    {
        vector3 c = a + b;
        return c.x == -4 && c.y == 13 && c.z == 25;
    }
};
static TEST_ADD TestAdd;
//******************************************************************************
class TEST_DOT : public TEST
{
    public: TEST_DOT() : TEST( "Get dot product of two 3d vectors" ) {}
    protected: bool RunInternal()
    {
        float c = a.dot(b);
        return c == -1188;
    }
};
static TEST_DOT TestDot;
//******************************************************************************
class TEST_CROSS : public TEST
{
    public: TEST_CROSS() : TEST( "Get cross product of two 3d vectors" ) {}
    protected: bool RunInternal()
    {
        vector3 n = a.cross(b);
        return n.x == 599 && n.y == -108 && n.z == 152;
    }
};
static TEST_CROSS TestCross;
//******************************************************************************
class TEST_MAGNITUDE : public TEST
{
    public: TEST_MAGNITUDE() : TEST( "Get magnitude of a 3d vector" ) {}
    protected: bool RunInternal()
    {
        float m_a = a.magnitude();
        float m_b = b.magnitude();
        return m_a > sqrt(736) && m_a < sqrt(738) &&
               m_b > sqrt(2448) && m_b < sqrt(2450);
    }
};
static TEST_MAGNITUDE TestMagnitude;
//******************************************************************************
class TEST_NORMALIZE : public TEST
{
    public: TEST_NORMALIZE() : TEST( "Normalize a 3d vector to unit length" ) {}
    protected: bool RunInternal()
    {
        vector3 a_n = a.normalize();
        vector3 b_n = b.normalize();

        vector3 a_calc( 0.294684, 0.442026, -0.847216 );
        vector3 b_calc( -0.242486, 0.0202072, 0.969944 );
        
        vector3 a_zero = a_n - a_calc;
        vector3 b_zero = b_n - b_calc;
        return a_zero.x < 0.0001 && a_zero.x > -0.0001 &&
               a_zero.y < 0.0001 && a_zero.y > -0.0001 &&
               a_zero.z < 0.0001 && a_zero.z > -0.0001 &&
               b_zero.x < 0.0001 && b_zero.x > -0.0001 &&
               b_zero.y < 0.0001 && b_zero.y > -0.0001 &&
               b_zero.z < 0.0001 && b_zero.z > -0.0001;
    }
};
static TEST_NORMALIZE TestNormalize;
//******************************************************************************
class TEST_PROJECT : public TEST
{
    public: TEST_PROJECT() : TEST( "Get the projection of one vector onto another" ) {}
    protected: bool RunInternal()
    {
        vector3 p = a.proj(b);
        vector3 answer( 5.821, -0.485, -23.285 );
        vector3 p_zero = p - answer; 
        return p_zero.x < 0.001 && p_zero.x > -0.001 &&
               p_zero.y < 0.001 && p_zero.y > -0.001 &&
               p_zero.z < 0.001 && p_zero.z > -0.001;
    }
};
static TEST_PROJECT TestProject;
//******************************************************************************
class TEST_ROTATE_SIMPLE : public TEST
{
    public: TEST_ROTATE_SIMPLE() : TEST( "Simple 90 degree rotation" ) {}
    protected: bool RunInternal()
    {
        vector3 v( 1, 0, 0 );
        vector3 x( 0, 0, 1 );
        v = v.rotate( x, PI / 2 );
        return v.x > -0.0001 && v.x < 0.0001 &&
               v.y > 0.9999 && v.y < 1.0001 && 
               v.z >-0.0001 && v.z < 0.0001; 
    }
};
static TEST_ROTATE_SIMPLE TestRotateSimple;
//******************************************************************************
class TEST_ROTATE : public TEST
{
    public: TEST_ROTATE() : TEST( "More complicated rotation (less than 90 degrees)" ) {}
    protected: bool RunInternal()
    {
        vector3 r = a.rotate( b, 17 * PI / 43 );
        vector3 answer( -4.931139, 5.611586, -26.099693 );
        return r.x - answer.x > -0.0001 && r.x - answer.x < 0.0001 &&
               r.y - answer.y > -0.0001 && r.y - answer.y < 0.0001 && 
               r.z - answer.z > -0.0001 && r.z - answer.z < 0.0001; 
    }
};
static TEST_ROTATE TestRotate;
//******************************************************************************



int main()
{
    int counter = 0;
    int PassedCounter = 0;
    srand(time(null));

    printf("*-----------------------------------------------------------\n");
    printf("* Starting Test suite...\n");
    printf("*-----------------------------------------------------------\n");
    TEST* TestIter = TEST::GetFirst();
    while( TestIter != null ) {
        printf("%3d: ", ++counter);
        if(TestIter->Run()) PassedCounter++;
        TestIter = TEST::GetNext( TestIter );
    }
    printf("Finished test suite...\n");
    printf("%d / %d passed\n", PassedCounter, counter);
}
