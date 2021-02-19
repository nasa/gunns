/************************** TRICK HEADER ***********************************************************
 LIBRARY DEPENDENCY:
    ((math/linear_algebra/CholeskyLdu.o))
***************************************************************************************************/

#include "UtCholeskyLdu.hh"
#include "software/exceptions/TsNumericalException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Cholesky Ldu Decomposition unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtCholeskyLdu::UtCholeskyLdu()
    :
    tArticle()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Cholesky Ldu Decomposition unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtCholeskyLdu::~UtCholeskyLdu()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCholeskyLdu::setUp()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCholeskyLdu::tearDown()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that the solution of [A]{x} = {b} satisfies the original system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCholeskyLdu::testNormalSolution()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtCholeskyLdu ..... 01: testNormalSolution .........................";

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

    CPPUNIT_ASSERT_NO_THROW(tArticle.Decompose(A, 4));
    CPPUNIT_ASSERT_NO_THROW(tArticle.Solve(A, b, x, 4));

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

    b[0] = 1.25E16;
    b[1] = 0.0;
    b[2] = 0.0;
    b[3] = 0.0;

    for (int i=0; i<16; ++i) {
        C[i] = A[i];
    }

    CPPUNIT_ASSERT_NO_THROW(tArticle.Decompose(A, 4));
    CPPUNIT_ASSERT_NO_THROW(tArticle.Solve(A, b, x, 4));

    result[0] = C[0]  * x[0] + C[1]  * x[1] + C[2]  * x[2] + C[3]  * x[3];
    result[1] = C[4]  * x[0] + C[5]  * x[1] + C[6]  * x[2] + C[7]  * x[3];
    result[2] = C[8]  * x[0] + C[9]  * x[1] + C[10] * x[2] + C[11] * x[3];
    result[3] = C[12] * x[0] + C[13] * x[1] + C[14] * x[2] + C[15] * x[3];

    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[0], result[0], std::max(b[0] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[1], result[1], std::max(b[1] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[2], result[2], std::max(b[2] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[3], result[3], std::max(b[3] * tolerance, 2.0 * tolerance));

    /// - Test solution of a size 1 system.
    double* A1 = new double[1];
    double* x1 = new double[1];
    double* b1 = new double[1];
    A1[0] = 2.0;
    x1[0] = 0.0;
    b1[0] = 4.0;
    CPPUNIT_ASSERT_NO_THROW(tArticle.Solve(A1, b1, x1, 1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b1[0] / A1[0], x1[0], DBL_EPSILON);
    delete[] b1;
    delete[] x1;
    delete[] A1;

    /// - Test this made-up system with very large range of magnitudes.
    double As[9] = {1.518500e24,  -2.155129e-5, -1.320176e-21,
                   -2.155129e-5,   1.111111e0,  -3.183099e-1,
                   -1.320176e-21, -3.183099e-1 , 1.000000e1};
    double xs[3] = {0.0,           0.0,          0.0};
    double bs[3] = {9.58e31,       0.0,          0.0};

    double Cs[9];
    for (int i=0; i<9; ++i) {
        Cs[i] = As[i];
    }

    CPPUNIT_ASSERT_NO_THROW(tArticle.Decompose(As, 3));
    CPPUNIT_ASSERT_NO_THROW(tArticle.Solve(As, bs, xs, 3));

    double results[3];
    results[0] = Cs[0] * xs[0] + Cs[1] * xs[1] + Cs[2] * xs[2];
    results[1] = Cs[3] * xs[0] + Cs[4] * xs[1] + Cs[5] * xs[2];
    results[2] = Cs[6] * xs[0] + Cs[7] * xs[1] + Cs[8] * xs[2];

    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[0], result[0], std::max(b[0] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[1], result[1], std::max(b[1] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[2], result[2], std::max(b[2] * tolerance, 2.0 * tolerance));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Decomposition returns the row number where the matrix is un-
///           conditioned (not positive definite).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCholeskyLdu::testUnConditioned()
{
    std::cout << "\n UtCholeskyLdu ..... 02: testUnConditioned ..........................";

    /// - Set up a bad row 1.
    double A[4] = {1.0, -1.1,
                  -1.1,  1.0};

    CPPUNIT_ASSERT_THROW(tArticle.Decompose(A, 2), TsNumericalException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Choleski_LDU_Solve returns the row number where the method fails because the
///           matrix is singular.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCholeskyLdu::testSingular()
{
    std::cout << "\n UtCholeskyLdu ..... 03: testSingular ...............................";

    /// - Set up a system with singular matrix.
    double A[9] = {1.0, -0.1,  0.0,
                  -1.1,  1.0,  0.0,
                   0.0,  0.0,  0.0};
    double x[3] = {0.0,  0.0,  0.0};
    double b[3] = {9.3, -3.2,  4.5};

    CPPUNIT_ASSERT_THROW(tArticle.Solve(A, b, x, 3), TsNumericalException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Decomposition handles arithmetic underflows without dying.  At
///           the time of this writing, Trick sims will die if a variable that is asymptotically
///           approaching zero will terminate with a floating exception when the result gets around
///           E-320 or so, whereas CPP_UNIT tests don't have this problem.  So this unit test can't
///           directly verify that a fault won't occur in the Trick sim.  However, we can check that
///           the Cholesky decomposition algorithm zeroes any value of magnitude less than E-100, in
///           order to avoid the float exception in Trick.
///
///           This test sets up an A matrix identical to one used in an example GUNNS sim that was
///           found to trigger this problem.
///
///           The problem has been found to be unique to the ts-generic repository, there must be
///           some compiler option set wrong.  Expect this test and the corresponding code in
///           CholeskyLdu to go away when it is fixed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCholeskyLdu::testDecompUnderflow()
{
    std::cout << "\n UtCholeskyLdu ..... 04: testDecompUnderflow ........................";

    /// - Set up a matrix that will cause arithmetic underflow when decomposed.
    const int N = 10;
    const int rows = N*N+1;
    const int size = rows*rows;
    double A[size];

    /// - Fill with zeroes.
    for (int i=0; i<size; ++i) {
        A[i] = 0.0;
    }

    int j = 0;
    const int border = N * (N - 1);
    for (int i = 0; i < border; i++) {
        if ((i != 0) && (i % (N - 1) == 0)) {
            j++; //counting the current row of the grid
        }
        A[(i+j)*rows + (i+j) + 1] = -1.0e-14;
    }

    // Initialize the vertical links in the grid network
    j = 0;
    for (int i = border; i <= (2 * border - 1); i++) {
        A[j*rows + j + N] = -1.0e-14;
        ++j;
    }

    // Make symmetrical
    for (int row=0; row<rows; ++row) {
        for (int col=row+1; col<rows; ++col) {
            A[col*rows + row] = A[row*rows + col];
        }
    }

    // Add diagonals
    for (int row=0; row<rows; ++row) {
        for (int col=0; col<row; ++col) {
            A[row*rows + row] -= A[row*rows + col];
        }
        for (int col=row+1; col<rows; ++col) {
            A[row*rows + row] -= A[row*rows + col];
        }
        A[row*rows + row] += 1.0;
    }
    A[size-1] = 4.930380657631324e-32;

    CPPUNIT_ASSERT_NO_THROW(tArticle.Decompose(A, rows));

    /// - Loop over every term in the decomposed matrix and verify its exponent is not below -200.
    for (int i = 0; i < size; ++i) {
        CPPUNIT_ASSERT(A[i] >= 0.0 || A[i] < 1.0E-200);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that [A] multiplied by the inverse of [A] is the identity matrix.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCholeskyLdu::testInvert()
{
    std::cout << "\n UtCholeskyLdu ..... 05: testInvert .................................";

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

    CPPUNIT_ASSERT_NO_THROW(tArticle.Decompose(A, 4));
    tArticle.Invert(A, 4);

    /// - Verify that [A] * inverse[A] = [I].
    double result[16];
    result[0]  = C[0]  * A[0] + C[1]  * A[4] + C[2]  * A[8]  + C[3]  * A[12];
    result[1]  = C[0]  * A[1] + C[1]  * A[5] + C[2]  * A[9]  + C[3]  * A[13];
    result[2]  = C[0]  * A[2] + C[1]  * A[6] + C[2]  * A[10] + C[3]  * A[14];
    result[3]  = C[0]  * A[3] + C[1]  * A[7] + C[2]  * A[11] + C[3]  * A[15];

    result[4]  = C[4]  * A[0] + C[5]  * A[4] + C[6]  * A[8]  + C[7]  * A[12];
    result[5]  = C[4]  * A[1] + C[5]  * A[5] + C[6]  * A[9]  + C[7]  * A[13];
    result[6]  = C[4]  * A[2] + C[5]  * A[6] + C[6]  * A[10] + C[7]  * A[14];
    result[7]  = C[4]  * A[3] + C[5]  * A[7] + C[6]  * A[11] + C[7]  * A[15];

    result[8]  = C[8]  * A[0] + C[9]  * A[4] + C[10] * A[8]  + C[11] * A[12];
    result[9]  = C[8]  * A[1] + C[9]  * A[5] + C[10] * A[9]  + C[11] * A[13];
    result[10] = C[8]  * A[2] + C[9]  * A[6] + C[10] * A[10] + C[11] * A[14];
    result[11] = C[8]  * A[3] + C[9]  * A[7] + C[10] * A[11] + C[11] * A[15];

    result[12] = C[12] * A[0] + C[13] * A[4] + C[14] * A[8]  + C[15] * A[12];
    result[13] = C[12] * A[1] + C[13] * A[5] + C[14] * A[9]  + C[15] * A[13];
    result[14] = C[12] * A[2] + C[13] * A[6] + C[14] * A[10] + C[15] * A[14];
    result[15] = C[12] * A[3] + C[13] * A[7] + C[14] * A[11] + C[15] * A[15];

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, result[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[3],  DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[4],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, result[5],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[6],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[7],  DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[8],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[9],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, result[10], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[11], DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[12], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[13], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[14], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, result[15], DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that the solution of [A]{x} = {b} satisfies the original system of equations
///           when decomposed as separate islands.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCholeskyLdu::testDecomposeVector()
{
    std::cout << "\n UtCholeskyLdu ..... 06: testDecomposeVector ........................";

    double tolerance = 1.0E-14;

    /// - Test this made-up system.
    double A[64] = {10.0,      0.0,      -0.002,    0.0,    0.0,    0.0,   -0.005,  0.0,   // island 0
                     0.0,      8.0,       0.0,     -0.001, -1.0,    0.0,    0.0,   -0.5,   // island 1
                    -0.002,    0.0,      12.0,      0.0,    0.0,    0.0,    0.0,    0.0,   // island 0
                     0.0,     -0.001,     0.0,      9.0,   -0.1,    0.0,    0.0,   -2.0,   // island 1
                     0.0,     -1.0,       0.0,     -0.1,    3.0,    0.0,    0.0,   -0.01,  // island 1
                     0.0,      0.0,       0.0,      0.0,    0.0,    0.0001, 0.0,    0.0,   // island 2
                    -0.005,    0.0,       0.0,      0.0,    0.0,    0.0,    2.0,    0.0,   // island 0
                     0.0,     -0.5,       0.0,     -2.0,   -0.01,   0.0,    0.0,  300.0};  // island 1
    double x[8]  = { 0.0,      0.0,       0.0,      0.0,    0.0,    0.0,    0.0,    0.0};
    double b[8]  = {27.0,      0.03,      0.0,     -1.5,    0.0,    0.0,    1.0,    0.001};

    double C[64];
    for (int i=0; i<64; ++i) {
        C[i] = A[i];
    }

    std::vector<int> island0, island1;
    island0.push_back(0);
    island0.push_back(2);
    island0.push_back(6);
    island1.push_back(1);
    island1.push_back(3);
    island1.push_back(4);
    island1.push_back(7);

    double A0[9] = {10.0, -0.002, -0.005,
                   -0.002, 12.0,   0.0,
                   -0.005, 0.0,    2.0};
    CPPUNIT_ASSERT_NO_THROW(tArticle.Decompose(A0, 3));
    CPPUNIT_ASSERT_NO_THROW(tArticle.Decompose(A, 8, island0));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A0[ 0], A[ 0], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A0[ 1], A[ 2], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A0[ 2], A[ 6], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A0[ 3], A[16], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A0[ 4], A[18], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A0[ 5], A[22], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A0[ 6], A[48], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A0[ 7], A[50], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A0[ 8], A[54], tolerance);

    double A1[16] = {8.0,  -0.001, -1.0,   -0.5,
                    -0.001, 9.0,   -0.1,   -2.0,
                    -1.0,  -0.1,    3.0,   -0.01,
                    -0.5,  -2.0,   -0.01, 300.0};
    CPPUNIT_ASSERT_NO_THROW(tArticle.Decompose(A1, 4));
    CPPUNIT_ASSERT_NO_THROW(tArticle.Decompose(A, 8, island1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[ 0], A[ 9], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[ 1], A[11], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[ 2], A[12], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[ 3], A[15], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[ 4], A[25], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[ 5], A[27], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[ 6], A[28], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[ 7], A[31], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[ 8], A[33], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[ 9], A[35], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[10], A[36], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[11], A[39], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[12], A[57], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[13], A[59], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[14], A[60], tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A1[15], A[63], tolerance);

    CPPUNIT_ASSERT_NO_THROW(tArticle.Solve(A, b, x, 8));

    double result[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    for (int i=0; i<8; ++i) {
        for (int j=0; j<8; ++j) {
            result[i] += C[j*8+i] * x[j];
        }
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[0], result[0], std::max(b[0] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[1], result[1], std::max(b[1] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[2], result[2], std::max(b[2] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[3], result[3], std::max(b[3] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[4], result[4], std::max(b[4] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[5], result[5], std::max(b[5] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[6], result[6], std::max(b[6] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[7], result[7], std::max(b[7] * tolerance, 2.0 * tolerance));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that the solution of [A]{x} = {b} satisfies the original system of equations,
///           and when [A] has some positive off-diagonal terms but is still positive-definite.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCholeskyLdu::testPosOffDiagSolution()
{
    std::cout << "\n UtCholeskyLdu ..... 07: testPosOffDiagSolution .....................";

    double tolerance = 1.0E-14;

    /// - Test this made-up system.  This kind of matrix, with the positive off-diagonals, can come
    ///   from a 6-DOF dynamics equations of motion state derivative model when the center of mass
    ///   has a non-zero offset from the body reference frame.  (We're omitting a DOF in this
    ///   example for brevity.)
    double A[25] = { 1.0e6,  0.0,    0.0,    0.0,    1.0e6,
                     0.0,    1.0e6,  0.0,   -1.0e6,  0.0,
                     0.0,    0.0,    1.0e7, -2.0e4, -3.0e4,
                     0.0,   -1.0e6, -2.0e4,  4.0e7, -5.0e4,
                     1.0e6,  0.0,   -3.0e4, -5.0e4,  6.0e7};
    double x[5]  = { 0.0,    0.0,    0.0,    0.0,    0.0};
    double b[5]  = {27.0,    0.03,  99.9,   -1.5,   -9.9};

    double C[25];
    for (int i=0; i<25; ++i) {
        C[i] = A[i];
    }

    CPPUNIT_ASSERT_NO_THROW(tArticle.Decompose(A, 5));
    CPPUNIT_ASSERT_NO_THROW(tArticle.Solve(A, b, x, 5));

    double result[5];
    result[0] = C[ 0]*x[0] + C[ 1]*x[1] + C[ 2]*x[2] + C[ 3]*x[3] + C[ 4]*x[4];
    result[1] = C[ 5]*x[0] + C[ 6]*x[1] + C[ 7]*x[2] + C[ 8]*x[3] + C[ 9]*x[4];
    result[2] = C[10]*x[0] + C[11]*x[1] + C[12]*x[2] + C[13]*x[3] + C[14]*x[4];
    result[3] = C[15]*x[0] + C[16]*x[1] + C[17]*x[2] + C[18]*x[3] + C[19]*x[4];
    result[4] = C[20]*x[0] + C[21]*x[1] + C[22]*x[2] + C[23]*x[3] + C[24]*x[4];

    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[0], result[0], std::max(b[0] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[1], result[1], std::max(b[1] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[2], result[2], std::max(b[2] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[3], result[3], std::max(b[3] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[4], result[4], std::max(b[4] * tolerance, 2.0 * tolerance));

    std::cout << "... Pass";
}
