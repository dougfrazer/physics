#include "test.h"
#include "vector3.h"
#include "math.h"

static const vector3 a( 8, 12, -23 );
static const vector3 b( -12, 1, 48 );
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
        return false;
    }
};
static TEST_NORMALIZE TestNormalize;
//******************************************************************************
class TEST_PROJECT : public TEST
{
    public: TEST_PROJECT() : TEST( "Get the projection of one vector onto another" ) {}
    protected: bool RunInternal()
    {
        return false;
    }
};
static TEST_PROJECT TestProject;
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
