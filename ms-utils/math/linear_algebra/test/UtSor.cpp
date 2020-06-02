/************************** TRICK HEADER ***********************************************************
 LIBRARY DEPENDENCY:
    ((math/linear_algebra/Sor.o))
***************************************************************************************************/

#include "UtSor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Cholesky Ldu Decomposition unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSor::UtSor()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Cholesky Ldu Decomposition unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSor::~UtSor()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSor::setUp()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSor::tearDown()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the isPositiveDefinite method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSor::testPositiveDefinite()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtSor ............. 01: testPositiveDefinite .......................";

    /// - Test this made-up matrix, which should be positive-definite.
    double A[16] = {10.0,     -0.001,    -0.002,    0.0,
                    -0.001,    8.0,      -0.003,   -0.001,
                    -0.002,   -0.003,    12.0,      0.0,
                     0.0,     -0.001,     0.0,      9.0};
    CPPUNIT_ASSERT(true  == Sor::isPositiveDefinite(A, 4));

    /// - Set a diagonal to low enough value to fail the positive-definite test.
    A[5] = 1.0E-50;
    CPPUNIT_ASSERT(false == Sor::isPositiveDefinite(A, 4));

    //TODO note that the method does not do a proper positive-definite check, for instance
    //     A = {1.0, -1.0, -1.0, 1.0} is not positive-definite but would pass this test.
    //     The proper definition is z(T)Az > 0 for all real vectors z.  Update this test if the
    //     method is ever improved.
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that the solution of [A]{x} = {b} satisfies the original system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSor::testNormalSolution()
{
    std::cout << "\n UtSor ............. 02: testNormalSolution .........................";

    double tolerance = 1.0E-14;

    /// - Test this made-up system.
    double A[16] = {10.0,     -0.001,    -0.002,    0.0,
                    -0.001,    8.0,      -0.003,   -0.001,
                    -0.002,   -0.003,    12.0,      0.0,
                     0.0,     -0.001,     0.0,      9.0};
    double x[4]  = { 0.0,      0.0,       0.0,      0.0};
    double b[4]  = {27.0,      0.03,      0.0,     -1.5};

    double C[16];
    for (int i=0; i<16; ++i) {
        C[i] = A[i];
    }

    CPPUNIT_ASSERT(true == Sor::isPositiveDefinite(A, 4));
    CPPUNIT_ASSERT(0     < Sor::solve(x, A, b, 4, 1.0, 5, tolerance));

    double result[4];
    result[0] = C[0]  * x[0] + C[1]  * x[1] + C[2]  * x[2] + C[3]  * x[3];
    result[1] = C[4]  * x[0] + C[5]  * x[1] + C[6]  * x[2] + C[7]  * x[3];
    result[2] = C[8]  * x[0] + C[9]  * x[1] + C[10] * x[2] + C[11] * x[3];
    result[3] = C[12] * x[0] + C[13] * x[1] + C[14] * x[2] + C[15] * x[3];

    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[0], result[0], std::max(b[0] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[1], result[1], std::max(b[1] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[2], result[2], std::max(b[2] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[3], result[3], std::max(b[3] * tolerance, 2.0 * tolerance));

    /// - Test this system from an electrical GUNNS network.
    A[0]  = 100000000000001.0;
    A[1]  = -1.0;
    A[2]  = 0.0;
    A[3]  = 0.0;
    A[4]  = -1.0;
    A[5]  = 1.046044615937735;
    A[6]  = -0.0005714285714285715;
    A[7]  = 0.0;
    A[8]  = 0.0;
    A[9]  = -0.0005714285714285715;
    A[10] = 0.0006714285714285716;
    A[11] = 0.0;
    A[12] = 0.0;
    A[13] = 0.0;
    A[14] = 0.0;
    A[15] = DBL_EPSILON * 1.0E-15;

    x[0] = 0.0;
    x[1] = 0.0;
    x[2] = 0.0;
    x[3] = 0.0;

    b[0] = 1.25E16;
    b[1] = 0.0;
    b[2] = 0.0;
    b[3] = 0.0;

    for (int i=0; i<16; ++i) {
        C[i] = A[i];
    }

    CPPUNIT_ASSERT(true == Sor::isPositiveDefinite(A, 4));
    CPPUNIT_ASSERT(0     < Sor::solve(x, A, b, 4, 1.0, 10, tolerance));

    result[0] = C[0]  * x[0] + C[1]  * x[1] + C[2]  * x[2] + C[3]  * x[3];
    result[1] = C[4]  * x[0] + C[5]  * x[1] + C[6]  * x[2] + C[7]  * x[3];
    result[2] = C[8]  * x[0] + C[9]  * x[1] + C[10] * x[2] + C[11] * x[3];
    result[3] = C[12] * x[0] + C[13] * x[1] + C[14] * x[2] + C[15] * x[3];

    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[0], result[0], std::max(b[0] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[1], result[1], std::max(b[1] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[2], result[2], std::max(b[2] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[3], result[3], std::max(b[3] * tolerance, 2.0 * tolerance));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the solve method returns unconverged when the matrix is un-conditioned (not
///           positive-definite).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSor::testUnConditioned()
{
    std::cout << "\n UtSor ............. 03: testUnConditioned ..........................";

    /// - Set up a bad row 1.
    double A[4] = {1.0, -1.1,
                  -1.1,  1.0};
    double x[2]  = { 0.0,      0.0};
    double b[2]  = {27.0,      0.03};

    /// - Verify solve never converges, even with a large iteration limit and large convergence
    ///   tolerance.
    CPPUNIT_ASSERT(-1 == Sor::solve(x, A, b, 2, 1.0, 100, 1.0E-4));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the solve method returns unconverged when the matrix is singular.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSor::testSingular()
{
    std::cout << "\n UtSor ............. 04: testSingular ...............................";

    /// - Set up a system with singular matrix.
    double A[9] = {1.0, -0.1,  0.0,
                  -1.1,  1.0,  0.0,
                   0.0,  0.0,  0.0};
    double x[3] = {0.0,  0.0,  0.0};
    double b[3] = {9.3, -3.2,  4.5};

    CPPUNIT_ASSERT(-1 == Sor::solve(x, A, b, 3, 1.0, 100, 1.0E-4));

    std::cout << "... Pass." << std::endl;
}
