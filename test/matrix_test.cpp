
#include "test.h"
#include "matrix.h"


//******************************************************************************
class TEST_TRANSPOSE : public TEST
{
    public: TEST_TRANSPOSE() : TEST( "Get transpose of 3x3 matrix" ) {}
    protected: bool RunInternal()
    {
        matrix3 m( 1.0, 3.0, 3.0,
                  1.0, 4.0, 3.0,
                  1.0, 3.0, 4.0 );
        matrix3 t = m.t();
        return t.x1 == 1.0 && t.x2 == 1.0 && t.x3 == 1.0 &&
               t.y1 == 3.0 && t.y2 == 4.0 && t.y3 == 3.0 &&
               t.z1 == 3.0 && t.z2 == 3.0 && t.z3 == 4.0; 
    }
};
static TEST_TRANSPOSE TestTranspose;
//******************************************************************************
class TEST_DET : public TEST
{
    public: TEST_DET() : TEST( "Get determinent of 3x3 matrix" ) {}
    protected: bool RunInternal()
    {
        matrix3 m( 1.0, 3.0, 3.0,
                  1.0, 4.0, 3.0,
                  1.0, 3.0, 4.0 );
        return m.det() == 1.0;
    }
};
static TEST_DET TestDet;
//******************************************************************************
class TEST_INVERSE : public TEST
{
    public: TEST_INVERSE() : TEST( "Invert a 3x3 matrix" ) {}
    protected: bool RunInternal()
    {
        matrix3 m( 1.0, 3.0, 3.0,
                  1.0, 4.0, 3.0,
                  1.0, 3.0, 4.0 );
        matrix3 i = m.inv();
        return i.x1 == 7.0 && i.x2 == -3.0 && i.x3 == -3.0 &&
               i.y1 == -1.0 && i.y2 == 1.0 && i.y3 == 0.0 &&
               i.z1 == -1.0 && i.z2 == 0.0 && i.z3 == 1.0; 
    }
};
static TEST_INVERSE TestInverse;
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
