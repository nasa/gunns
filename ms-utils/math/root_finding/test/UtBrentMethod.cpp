/************************** TRICK HEADER ***********************************************************
 LIBRARY DEPENDENCY:
    ((math/root_finding/BrentMethod.o))
***************************************************************************************************/

#include "UtBrentMethod.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "software/exceptions/TsNumericalException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Brent's Method unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtBrentMethod::UtBrentMethod()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Brent's Method unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtBrentMethod::~UtBrentMethod()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtBrentMethod::setUp()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtBrentMethod::tearDown()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verifies the Brent's method will return the correct root of a given function with
///           sufficient accuracy and within an acceptable number of iterations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtBrentMethod::testNominal()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtBrentMethod ..... 01: testNominal ................................";

    /// Call Brent's with bounds surrounding the known root to the test polynomial.
    double root = 0.0;
    double tol  = FLT_EPSILON;
    UtBrentMethodCurve1 testCurve;
    int iter = testCurve.solve(root, 0.0, 2.0, tol);

    /// @test the root returned by Brent's evaluates the polynomial correctly.
    double actual = testCurve.evaluate(root);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, actual, tol);
    CPPUNIT_ASSERT(11 > iter);

    /// @test the root matches the known real root of the test polynomial.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.369, root, 0.001);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Verifies the Brent's method error return modes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtBrentMethod::testErrors()
{
    std::cout << "\n UtBrentMethod ..... 02: testErrors .................................";

    /// Call Brent's with bounds not surrounding the known root to the test polynomial.
    double root = 0.0;
    UtBrentMethodCurve1 testCurve1;

    /// @test Brent's throws an exception if the root is not bound.
    CPPUNIT_ASSERT_THROW(testCurve1.solve(root, 2.0, 3.0, FLT_EPSILON), TsOutOfBoundsException);

    /// TODO, test a function which exceeds Brent's maximum iteration limit.
//    CPPUNIT_ASSERT_THROW(testCurve1.solve(root, 2.0, 3.0, FLT_EPSILON), TsNumericalException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Overrides the evaluate method in BrentMethod and implements a 5th-order polynomial for
///           the solve method to use.
////////////////////////////////////////////////////////////////////////////////////////////////////
double UtBrentMethodCurve1::evaluate(const double x)
{
    /// - This polynomial has roots (from http://xrjunque.nom.es/precis/rootfinder.aspx):
    ///     Root 1: 1.369 + 0*i
    ///     Root 2: 0.172 - 0.297*i
    ///     Root 3: 0.172 + 0.297*i
    ///     Root 4: 1.46  - 1.034*i
    ///     Root 5: 1.46  + 1.034*i
    double c0 =   1.0;          //0th-order coefficient
    double c1 =  -4.5627;       //1st-order coefficient
    double c2 =  14.272;        //2nd-order coefficient
    double c3 = -17.043;        //3rd-order coefficient
    double c4 =   8.9822;       //4th-order coefficient
    double c5 =  -1.9391;       //5th-order coefficient

    return c0 + (c1 + (c2 + (c3 + (c4 + c5 * x) * x) * x) * x) * x;
}
