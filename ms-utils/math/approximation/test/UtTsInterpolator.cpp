/************************** TRICK HEADER ***********************************************************
 LIBRARY DEPENDENCY:
 (
     (math/approximation/TsBilinearInterpolatorReverse.o)
     (math/approximation/TsLinearInterpolator.o)
 )
 ***************************************************************************************************/

#include "math/approximation/TsBilinearInterpolatorReverse.hh"
#include "math/approximation/TsLinearInterpolator.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

#include "UtTsInterpolator.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Interpolator unit-test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsInterpolator::UtTsInterpolator()
:
CppUnit::TestFixture(),
mArticle(0),
mTolerance(0.0)
{
// Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Interpolator unit-test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsInterpolator::~UtTsInterpolator()
{
//Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsInterpolator::tearDown()
{
    delete mArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsInterpolator::setUp()
{
    mArticle       = 0;
    mTolerance     = 0.001;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Linear Interpolator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsInterpolator::testTsLinear()
{
    std::cout << "--------------------------------------------------------------------------------";
    std::cout << "\n Interpolator 1.1: Input Ascending Order Test                           ";

    double X[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    double Z[] = {100, 200, 400, 900, 1600, 2500, 2600, 2700, 2800, 2900};
    double x = 76;
    const int n = 10;
    const double minX = 10;
    const double maxX = +100;
    double scale = ((Z[7] - Z[6]) / (X[7] - X[6]));
    double expected = Z[6] + (x - X[6]) * scale;

    mArticle = new TsLinearInterpolator(X, Z, n, minX, maxX);


    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    delete mArticle;
    std::cout << "... Pass.";

    std::cout << "\n Interpolator 1.2: Input Descending Order Test                          ";

    double X2[] = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10};
    double Z2[] = {3000, 2900, 2800, 2700, 2600, 2500, 1600, 900, 400, 100};
    const double scale2 = ((Z2[3] - Z2[4]) / (X2[3] - X2[4]));
    const double expected2 = Z2[4] + (x - X2[4]) * scale2;

    mArticle = new TsLinearInterpolator(X2, Z2, n, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected2, mArticle->get(x), mTolerance);
    delete mArticle;
    std::cout << "... Pass.";

    std::cout << "\n Interpolator 1.3: Index Test                                           ";

    x = 76;
    scale = ((Z[7] - Z[6]) / (X[7] - X[6]));
    expected = Z[6] + (x - X[6]) * scale;

    mArticle = new TsLinearInterpolator(X, Z, n, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);


    x = 32;
    scale = ((Z[3] - Z[2]) / (X[3] - X[2]));
    expected = Z[2] + (x - X[2]) * scale;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);


    x = -1.0;
    expected = 100.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);

    x = 100.0;
    expected = 2900.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);

    std::cout << "... Pass";


}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Bilinear Interpolator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsInterpolator::testTsBilinear()
{
    std::cout << "\n Interpolator 2.1: Input X Ascending Order Test                         ";

    double X[5] = {10, 20, 30, 40, 50};
    double Y[3] = {11, 22, 33};
    double x = 45;
    double y = 25;
    int m = 5;
    int n = 3;

    double Z[15] = {200, 400, 900,
                    1600, 2500, 2600,
                    2700, 2800, 2900,
                    3000, 3100, 3200,
                    3300, 3400, 3500};
    const double minX = +10;
    const double maxX = +50;
    const double minY = +11;
    const double maxY = +33;

    double Z1 = ((X[4] - x) / (X[4] - X[3]) * Z[10]) + ((x - X[3]) / (X[4] - X[3]) * Z[13]);
    double Z2 = ((X[4] - x) / (X[4] - X[3]) * Z[11]) + ((x - X[3]) / (X[4] - X[3]) * Z[14]);
    double expected = ((Y[2] - y) / (Y[2] - Y[1]) * Z1) + ((y - Y[1]) / (Y[2] - Y[1]) * Z2);

    mArticle = new TsBilinearInterpolator(X, Y, Z, m, n, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x,y), mTolerance);
    std::cout << "... Pass.";
    delete mArticle;

    std::cout << "\n Interpolator 2.2: Input X Descending Order Test                        ";

    double X2[5] = {50, 40, 30, 20, 10};
    double Y2[3] = {11, 22, 33};
    double Zb[15] = {3300, 3400, 3500, 3000, 3100, 3200, 2700, 2800, 2900, 1600, 2500, 2600, 200,
        400, 900};
    Z1 = ((X2[0] - x) / (X2[0] - X2[1]) * Zb[4]) + ((x - X2[1]) / (X2[0] - X2[1]) * Zb[1]);
    Z2 = ((X2[0] - x) / (X2[0] - X2[1]) * Zb[5]) + ((x - X2[1]) / (X2[0] - X2[1]) * Zb[2]);
    expected = ((Y2[2] - y) / (Y2[2] - Y2[1]) * Z1) + ((y - Y2[1]) / (Y2[2] - Y2[1]) * Z2);

    mArticle = new TsBilinearInterpolator(X2, Y2, Zb, m, n, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x,y), mTolerance);

    std::cout << "... Pass.";
    delete mArticle;

    std::cout << "\n Interpolator 2.3: Input Y Descending Order Test                        ";

    double X3[5] = {10, 20, 30, 40, 50};
    double Y3[3] = {33, 22, 11};
    double Zc[15] = {900, 400, 200, 2600, 2500, 1600, 2900, 2800, 2700, 3200, 3100, 3000, 3500,
        3400, 3300};
    Z1 = ((X3[4] - x) / (X3[4] - X3[3]) * Zc[10]) + ((x - X3[3]) / (X3[4] - X3[3]) * Zc[13]);
    Z2 = ((X3[4] - x) / (X3[4] - X3[3]) * Zc[9]) + ((x - X3[3]) / (X3[4] - X3[3]) * Zc[12]);
    expected = ((Y3[0] - y) / (Y3[0] - Y3[1]) * Z1) + ((y - Y3[1]) / (Y3[0] - Y3[1]) * Z2);

    mArticle = new TsBilinearInterpolator(X3, Y3, Zc, m, n, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x,y), mTolerance);
    delete mArticle;

    std::cout << "... Pass.";

    std::cout << "\n Interpolator 2.4: X Index Test                                         ";

    // x decreased sufficiently to change index
    x = 49;
    y = 25;

    Z1 = ((X[4] - x) / (X[4] - X[3]) * Z[10]) + ((x - X[3]) / (X[4] - X[3]) * Z[13]);
    Z2 = ((X[4] - x) / (X[4] - X[3]) * Z[11]) + ((x - X[3]) / (X[4] - X[3]) * Z[14]);
    expected = ((Y[2] - y) / (Y[2] - Y[1]) * Z1) + ((y - Y[1]) / (Y[2] - Y[1]) * Z2);


    mArticle = new TsBilinearInterpolator(X, Y, Z, m, n, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x,y), mTolerance);

    x = 29;

    Z1 = ((X[2] - x) / (X[2] - X[1]) * Z[4]) + ((x - X[1]) / (X[2] - X[1]) * Z[7]);
    Z2 = ((X[2] - x) / (X[2] - X[1]) * Z[5]) + ((x - X[1]) / (X[2] - X[1]) * Z[8]);
    expected = ((Y[2] - y) / (Y[2] - Y[1]) * Z1) + ((y - Y[1]) / (Y[2] - Y[1]) * Z2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x,y), mTolerance);
    std::cout << "... Pass.";
    delete mArticle;


    std::cout << "\n Interpolator 2.5: Y Index Test                                         ";
    // y decreased sufficiently to change index
    x = 45;
    y = 32;

    mArticle = new TsBilinearInterpolator(X, Y, Z, m, n, minX, maxX, minY, maxY);
    mArticle->get(x,y);

    y = 12;

    Z1 = ((X[4] - x) / (X[4] - X[3]) * Z[9]) + ((x - X[3]) / (X[4] - X[3]) * Z[12]);
    Z2 = ((X[4] - x) / (X[4] - X[3]) * Z[10]) + ((x - X[3]) / (X[4] - X[3]) * Z[13]);
    expected = ((Y[1] - y) / (Y[1] - Y[0]) * Z1) + ((y - Y[0]) / (Y[1] - Y[0]) * Z2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x,y), mTolerance);

    // traverse from end to end (and back again...)
    x = -1.0;
    y = -1.0;
    expected = 200.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x, y), mTolerance);
    x = 100.0;
    y = 50.0;
    expected = 3500.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x,y), mTolerance);
    x = -1.0;
    y = -1.0;
    expected = 200.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x, y), mTolerance);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test initialization exception for an unordered array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsInterpolator::testTsLinearException()
{
    std::cout << "\n Interpolator 3.1: Linear Unordered Array Initialization Exception      ";
    double minX = +10.0;
    const double maxX = +100.0;

    // Array does not have enough values to perform interpolation
    double Xshort[] = {10};
    double Zshort[] = {25};
    int n = 1;
    CPPUNIT_ASSERT_THROW(mArticle = new TsLinearInterpolator(Xshort, Zshort, 1, minX, maxX), TsInitializationException);
    delete mArticle;

    // Unordered array values decrease then increase
    double X[] = {10, 5, 15, 40, 50, 60, 70, 80, 90, 100};
    double Z[] = {100, 200, 400, 900, 1600, 2500, 2600, 2700, 2800, 2900, 3000};
    n = 10;
    CPPUNIT_ASSERT_THROW(mArticle = new TsLinearInterpolator(X, Z, n, minX, maxX), TsInitializationException);
    delete mArticle;

    //Unordered array values are equal
    X[8] = 100;

    CPPUNIT_ASSERT_THROW(mArticle = new TsLinearInterpolator(X, Z, n, minX, maxX), TsInitializationException);
    delete mArticle;

    //Unordered array values increase then decrease
    X[1] = 25;

    CPPUNIT_ASSERT_THROW(mArticle = new TsLinearInterpolator(X, Z, n, minX, maxX), TsInitializationException);
    std::cout << "... Pass.";
    delete mArticle;

    std::cout << "\n Interpolator 3.2: Linear X Array Table Range Exception                 ";
    X[1] = 12;
    minX = -1000.0;
    CPPUNIT_ASSERT_THROW(mArticle = new TsLinearInterpolator(X, Z, n, minX, maxX), TsInitializationException);
    std::cout << "... Pass";

}

void UtTsInterpolator::testTsBilinearException()
{
    std::cout << "\n Interpolator 4.1: Bilinear Unordered X Array Initialization Exception  ";

    double X[5] = {7, 2, 10, 50, 60};
    double Y[3] = {11, 22, 33};
    int m = 5;
    int n = 3;

    double Z[15] = {200, 400, 900, 1600, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300,
            3400, 3500};
    double minX = -1000.0;
    double maxX = +1000.0;
    const double minY = -1000.0;
    const double maxY = +1000.0;

    // Unordered X array values decrease then increase
    CPPUNIT_ASSERT_THROW(mArticle = new TsBilinearInterpolator(X, Y, Z, m, n, minX, maxX, minY, maxY),
            TsInitializationException);
    delete mArticle;

    // Unordered X array values are equal
    X[1] = 10;

    CPPUNIT_ASSERT_THROW(mArticle = new TsBilinearInterpolator(X, Y, Z, m, n, minX, maxX, minY, maxY),
            TsInitializationException);
    delete mArticle;

    // Unordered X array values increase then decrease
    double X2[5] = {7, 8, 20, 10, 60};
    double Y2[3] = {11, 22, 33};
    CPPUNIT_ASSERT_THROW(mArticle = new TsBilinearInterpolator(X2, Y2, Z, m, n, minX, maxX, minY, maxY),
            TsInitializationException);
    std::cout << "... Pass.";
    delete mArticle;

    std::cout << "\n Interpolator 4.2: Bilinear Y Array Initialization Exception            ";

    double X3[5] = {10, 20, 30, 40, 50};
    double Y3[3] = {22, 2, 11};

    // Unordered Y array values decrease then increase
    CPPUNIT_ASSERT_THROW(mArticle = new TsBilinearInterpolator(X3, Y3, Z, m, n, minX, maxX, minY, maxY),
            TsInitializationException);
    delete mArticle;

    // Unordered Y array values are equal
    Y3[1] = 22;

    CPPUNIT_ASSERT_THROW(mArticle = new TsBilinearInterpolator(X3, Y3, Z, m, n, minX, maxX, minY, maxY),
            TsInitializationException);
    delete mArticle;

    // Unordered Y array values increase then decrease
    double X4[5] = {10, 20, 30, 40, 50};
    double Y4[3] = {11, 44, 33};
    CPPUNIT_ASSERT_THROW(mArticle = new TsBilinearInterpolator(X4, Y4, Z, m, n, minX, maxX, minY, maxY),
            TsInitializationException);
    std::cout << "... Pass.";
    delete mArticle;

    std::cout << "\n Interpolator 4.3: Bilinear X Array Table Range Exception               ";

    double X5[5] = {10, 20, 30, 40, 50};
    double Y5[3] = {11, 22, 33};

    CPPUNIT_ASSERT_THROW(mArticle = new TsBilinearInterpolator(X5, Y5, Z, m, n, minX, maxX, minY, maxY),
            TsInitializationException);
    std::cout << "... Pass.";
    delete mArticle;

    std::cout << "\n Interpolator 4.4: Bilinear Y Array Table Range Exception               ";
    minX = 10;
    maxX = 50;

    CPPUNIT_ASSERT_THROW(mArticle = new TsBilinearInterpolator(X5, Y5, Z, m, n, minX, maxX, minY, maxY),
            TsInitializationException);
    std::cout << "... Pass";

}

void UtTsInterpolator::testInvalidRangeExceptions()
{
    std::cout << "\n Interpolator 5:   Invalid Range Exceptions                             ";

    double X[5] = {100, 200, 300, 400, 600};
    double Y[3] = {75, 600, 1100};
    double x = 502;
    double y = 25;
    int m = 5;
    int n = 3;

    double Z[15] = {200, 400, 900,
            1600, 2500, 2600,
            2700, 2800, 2900,
            3000, 3100, 3200,
            3300, 3400, 3500};
    double Z2[5] = {400,500,600,700,800};

    const double minX = +100.0;
    const double maxX = +500.0;
    const double minY = +75.0;
    const double maxY = +500.0;

    mArticle = new TsLinearInterpolator(X, Z2, m, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new TsBilinearInterpolator(X, Y, Z, m, n, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x,y), TsOutOfBoundsException);
    delete mArticle;
    x = 100;
    y = 750;

    mArticle = new TsBilinearInterpolator(X, Y, Z, m, n, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,-y), TsOutOfBoundsException);

    std::cout << "... Pass";
}

void UtTsInterpolator::testEmptyConstructors()
{
    std::cout << "\n Interpolator 6:   Constructor Calls with No Arguments                  ";
    double x = 0.1;
    double y = -0.1;

    mArticle = new TsLinearInterpolator();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new TsBilinearInterpolator();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new TsBilinearInterpolatorReverse();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);

    std::cout << "... Pass.";
}

void UtTsInterpolator::testTsLinearInit1()
{
    std::cout << "\n Interpolator 7:    Init method - dim and null arrays                   ";

    TsLinearInterpolator lin;

    CPPUNIT_ASSERT(!lin.isInitialized());

    double X[3] = {1.0, 2.0, 3.0};
    double Y[3] = {2.0, 4.0, 9.0};
    CPPUNIT_ASSERT_NO_THROW(lin.init(X, Y, 3, 1.0, 3.0));

    CPPUNIT_ASSERT_THROW(lin.init(X, Y, 0, 1.0, 3.0), TsInitializationException);
    CPPUNIT_ASSERT_NO_THROW(lin.init(X, Y, 3, 1.0, 3.0));

    CPPUNIT_ASSERT_THROW(lin.init(0, Y, 3, 1.0, 3.0), TsInitializationException);
    CPPUNIT_ASSERT_NO_THROW(lin.init(X, Y, 3, 1.0, 3.0));

    CPPUNIT_ASSERT_THROW(lin.init(X, 0, 3, 1.0, 3.0), TsInitializationException);
    CPPUNIT_ASSERT_NO_THROW(lin.init(X, Y, 3, 1.0, 3.0));
    CPPUNIT_ASSERT(lin.isInitialized());


    CPPUNIT_ASSERT_THROW(lin.init(X, Y, 3, -1.0, 4.0), TsInitializationException);
    CPPUNIT_ASSERT_THROW(lin.init(X, Y, 3,  1.0, 4.0), TsInitializationException);
    CPPUNIT_ASSERT(!lin.isInitialized());
    std::cout << "... Pass.";
}

void UtTsInterpolator::testTsLinearInit2()
{
    std::cout << "\n Interpolator 8:    Init method - ordered array                         ";

    TsLinearInterpolator lin;
    double X[3] = {1.0, 2.0, 3.0};
    double Y[3] = {2.0, 4.0, 9.0};
    CPPUNIT_ASSERT_NO_THROW(lin.init(X, Y, 3, 1.0, 3.0));

    double X2[3]= {3.0, 2.0, 1.0};
    double Y2[3]= {9.0, 4.0, 2.0};

    // out of order ascending
    X[2] = 0.0;
    CPPUNIT_ASSERT_THROW(lin.init(X, Y, 3, 1.0, 3.0), TsInitializationException);
    X[2] = 3.0;
    CPPUNIT_ASSERT_NO_THROW(lin.init(X2, Y2, 3, 1.0, 3.0));

    // out of order descending
    X2[2] = 5.0;
    CPPUNIT_ASSERT_THROW(lin.init(X2, Y2, 3, 1.0, 3.0), TsInitializationException);
    X2[2] = 1.0;
    CPPUNIT_ASSERT_NO_THROW(lin.init(X2, Y2, 3, 1.0, 3.0));

    // equality
    X[1] = X[0]+ DBL_EPSILON/2.0;
    CPPUNIT_ASSERT_THROW(lin.init(X, Y, 3, 1.0, 3.0), TsInitializationException);

    std::cout << "... Pass.";
}

void UtTsInterpolator::testTsBilinearReverse()
{
    std::cout << "\n Interpolator 9:   Bilinear Reverse                                     ";

    double X[4] = {0.0, 1.0, 2.0, 3.0};
    double Y[5] = {4.0, 5.0, 6.0, 7.0, 8.0};
    int m = 4;
    int n = 5;

    //             Y[0] Y[1] Y[2] Y[3] Y[4]
    double Z[20] = {0.1, 0.3, 0.4, 0.7, 0.9,   //X[0]
                    0.3, 0.5, 0.5, 0.3,-0.8,   //X[1]
                    0.7, 0.7, 0.6, 0.5, 0.0,   //X[2]
                    1.0, 0.8, 0.2,-0.3, 0.4};  //X[3]
    const double minX = 0.0;
    const double maxX = 3.0;
    const double minY = 4.0;
    const double maxY = 8.0;
    mArticle = new TsBilinearInterpolatorReverse(X, Y, Z, m, n, minX, maxX, minY, maxY);

    /// - Since the internal y-axis search starts out at Y[0], let's test handling of infinite
    ///   solutions between [X,Y] = [2,0] and [2,1]. It should output halfway between [2,0] & [2,1].
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.5, mArticle->get(2.0, 0.7), mTolerance);

    /// - Test forward search along the y-axis.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, mArticle->get(0.0, 0.9), mTolerance);

    /// - Test backward search along the y-axis.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, mArticle->get(0.0, 0.1), mTolerance);

    /// - Test the nearest solution is found when there are multiple possible solutions.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, mArticle->get(1.0,-0.8), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0 + (0.2 - -0.3)/(0.4 - -0.3), mArticle->get(3.0, 0.2), mTolerance);

    /// - Test wrapping around the y-axis search from end to beginning of the scale.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.5, mArticle->get(3.0, 0.9), mTolerance);

    /// - Test wrapping around the y-axis search from beginning to end of the scale.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0 + (0.0 - 0.3)/(-0.8 - 0.3), mArticle->get(1.0, 0.0), mTolerance);

    /// - Test interpolation on the x-axis.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.5, mArticle->get(2.5, 0.8), mTolerance);

    /// - Test lower-limiting the z argument.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, mArticle->get(1.0,-2.0), mTolerance);

    /// - Test for no solution for y exists, z above the valid range.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, mArticle->get(3.0, 2.0), mTolerance);

    /// - Test for no solution for y exists, z below the valid range.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, mArticle->get(1.0,-2.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->get(2.0, 2.0), mTolerance);

    std::cout << "... Pass." << std::endl;
}
