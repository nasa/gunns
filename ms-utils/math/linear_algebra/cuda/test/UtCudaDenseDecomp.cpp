/************************** TRICK HEADER ***********************************************************
 LIBRARY DEPENDENCY:
    ((math/linear_algebra/cuda/CudaDenseDecomp.o))
***************************************************************************************************/

#include "software/exceptions/TsNumericalException.hh"
#include "UtCudaDenseDecomp.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Cholesky Ldu Decomposition unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtCudaDenseDecomp::UtCudaDenseDecomp()
    :
    tArticle()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Cholesky Ldu Decomposition unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtCudaDenseDecomp::~UtCudaDenseDecomp()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaDenseDecomp::setUp()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaDenseDecomp::tearDown()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that the solution of [A]{x} = {b} satisfies the original system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaDenseDecomp::testNormalSolution()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtCudaDenseDecomp ..... 01: testNormalSolution .....................";

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

    /// - Copy U transpose into L, since test article only populates upper triangle of decomposed
    ///   matrix.
    for (int row=1, rowNcol=4; row<4; ++row, rowNcol=row*4) {
        for (int col=0, colNrow=row; col<row; ++col, colNrow+=4, ++rowNcol) {
            A[rowNcol] = A[colNrow];
        }
    }
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

    /// - Copy U transpose into L, since test article only populates upper triangle of decomposed
    ///   matrix.
    for (int row=1, rowNcol=4; row<4; ++row, rowNcol=row*4) {
        for (int col=0, colNrow=row; col<row; ++col, colNrow+=4, ++rowNcol) {
            A[rowNcol] = A[colNrow];
        }
    }
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

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the decomposition when the matrix is un-conditioned (not positive definite).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaDenseDecomp::testUnConditioned()
{
    std::cout << "\n UtCudaDenseDecomp ..... 02: testUnConditioned ......................";

    /// - Set up a bad row 1.
    double A[4] = {1.0, -1.1,
                  -1.1,  1.0};

    /// - Unlike CholeskyLdu, cusolverDnDsytrf doesn't fail for this kind of unconditioned matrix.
    ///   Its Bunch-Kaufman factorization method is typically used for indefinite matrices.
    tArticle.Decompose(A, 2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0,  A[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.1,  A[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.1,  A[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.21, A[3], DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the decomposition throws an exception when the matrix is singular.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaDenseDecomp::testSingular()
{
    std::cout << "\n UtCudaDenseDecomp ..... 03: testSingular ...........................";

    /// - Set up a system with singular matrix.
    double A[9] = {1.0, -0.1,  0.0,
                  -1.1,  1.0,  0.0,
                   0.0,  0.0,  0.0};

    CPPUNIT_ASSERT_THROW(tArticle.Decompose(A, 3), TsNumericalException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Decomposition handles arithmetic underflows without dying.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaDenseDecomp::testDecompUnderflow()
{
    std::cout << "\n UtCudaDenseDecomp ..... 04: testDecompUnderflow ....................";

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
/// @details  Tests that the solution of [A]{x} = {b} satisfies the original system of equations,
///           and when [A] has some positive off-diagonal terms but is still positive-definite.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaDenseDecomp::testPosOffDiagSolution()
{
    std::cout << "\n UtCudaDenseDecomp ..... 05: testPosOffDiagSolution .................";

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

    /// - Copy U transpose into L, since test article only populates upper triangle of decomposed
    ///   matrix.
    for (int row=1, rowNcol=5; row<5; ++row, rowNcol=row*5) {
        for (int col=0, colNrow=row; col<row; ++col, colNrow+=5, ++rowNcol) {
            A[rowNcol] = A[colNrow];
        }
    }
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the dynamic resizing of the GPU matrix from one decomposition to another.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCudaDenseDecomp::testResizing()
{
    std::cout << "\n UtCudaDenseDecomp ..... 03: testResizing ...........................";

    double tolerance = 1.0E-14;

    /// - Set up size 2 and size 3 systems.
    double A2[4] = {1.0, -0.1,
                   -0.1,  2.0};
    double A3[9] = {1.0, -0.1, -0.2,
                   -0.1,  2.0,  0.0,
                   -0.2,  0.0,  0.4};
    double x[3] = {0.0,  0.0, 0.0};
    double b[3] = {9.3, -3.2, 3.2};

    double C2[4];
    for (int i=0; i<4; ++i) {
        C2[i] = A2[i];
    }
    double C3[9];
    for (int i=0; i<9; ++i) {
        C3[i] = A3[i];
    }

    /// - Test decomposition & solution of size 2 system.
    CPPUNIT_ASSERT_NO_THROW(tArticle.Decompose(A2, 2));
    for (int row=1, rowNcol=2; row<2; ++row, rowNcol=row*2) {
        for (int col=0, colNrow=row; col<row; ++col, colNrow+=2, ++rowNcol) {
            A2[rowNcol] = A2[colNrow];
        }
    }
    CPPUNIT_ASSERT_NO_THROW(tArticle.Solve(A2, b, x, 2));

    double result2[2];
    result2[0] = C2[ 0]*x[0] + C2[ 1]*x[1];
    result2[1] = C2[ 2]*x[0] + C2[ 3]*x[1];

    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[0], result2[0], std::max(b[0] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[1], result2[1], std::max(b[1] * tolerance, 2.0 * tolerance));

    /// - Test decomposition & solution of size 3 system.
    CPPUNIT_ASSERT_NO_THROW(tArticle.Decompose(A3, 3));
    for (int row=1, rowNcol=3; row<3; ++row, rowNcol=row*3) {
        for (int col=0, colNrow=row; col<row; ++col, colNrow+=3, ++rowNcol) {
            A3[rowNcol] = A3[colNrow];
        }
    }
    CPPUNIT_ASSERT_NO_THROW(tArticle.Solve(A3, b, x, 3));

    double result3[3];
    result3[0] = C3[0]*x[0] + C3[1]*x[1] + C3[2]*x[2];
    result3[1] = C3[3]*x[0] + C3[4]*x[1] + C3[5]*x[2];
    result3[2] = C3[6]*x[0] + C3[7]*x[1] + C3[8]*x[2];

    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[0], result3[0], std::max(b[0] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[1], result3[1], std::max(b[1] * tolerance, 2.0 * tolerance));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b[2], result3[2], std::max(b[2] * tolerance, 2.0 * tolerance));

    std::cout << "... Pass";
}
