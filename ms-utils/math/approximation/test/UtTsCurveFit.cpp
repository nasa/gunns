/************************** TRICK HEADER ***********************************************************
 LIBRARY DEPENDENCY:
 (
     (math/approximation/CubicFit.o)
     (math/approximation/ExponentialFit.o)
     (math/approximation/InvLinearFit.o)
     (math/approximation/InvQuadraticFit.o)
     (math/approximation/LinearFit.o)
     (math/approximation/PowerFit.o)
     (math/approximation/ProductFit.o)
     (math/approximation/QuadLinFit.o)
     (math/approximation/QuadLinInvFit.o)
     (math/approximation/QuadraticFit.o)
     (math/approximation/QuadraticRootFit.o)
     (math/approximation/QuarticFit.o)
     (math/approximation/QuinticFit.o)
     (math/approximation/QuotientFit.o)
     (math/approximation/RationalFit.o)
     (math/approximation/ShowmateFit.o)
     (math/approximation/SutherlandFit.o)
 )
 ***************************************************************************************************/

#include "math/approximation/CubicFit.hh"
#include "math/approximation/ExponentialFit.hh"
#include "math/approximation/InvLinearFit.hh"
#include "math/approximation/InvQuadraticFit.hh"
#include "math/approximation/LinearFit.hh"
#include "math/approximation/PowerFit.hh"
#include "math/approximation/ProductFit.hh"
#include "math/approximation/QuadLinFit.hh"
#include "math/approximation/QuadLinInvFit.hh"
#include "math/approximation/QuadraticFit.hh"
#include "math/approximation/QuadraticRootFit.hh"
#include "math/approximation/QuarticFit.hh"
#include "math/approximation/QuinticFit.hh"
#include "math/approximation/QuotientFit.hh"
#include "math/approximation/RationalFit.hh"
#include "math/approximation/ShowmateFit.hh"
#include "math/approximation/SutherlandFit.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

#include "UtTsCurveFit.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this curve fit unit-test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsCurveFit::UtTsCurveFit()
:
CppUnit::TestFixture(),
mArticle(0),
mTolerance(0.0)
{
// Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this curve fit unit-test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsCurveFit::~UtTsCurveFit()
{
//Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::tearDown()
{
    delete mArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::setUp()
{
    mArticle   = 0;
    mTolerance = 0.001;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Linear curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testTsLinear()
{
    std::cout << "--------------------------------------------------------------------------------";
    std::cout << "\n Curve Fit 01: Univariate Linear Test                                   ";

    const double a = +1.0;
    const double b = +2.0;
    const double x = 11.473;
    const double minX = -1000.0;
    const double maxX = +1000.0;
    const double expected = a + b * x;

    mArticle = new LinearFit(a, b, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Quadratic curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testQuadratic()
{
    std::cout << "\n Curve Fit 02: Univariate Quadratic Test                                ";

    const double a = -1.0;
    const double b = -2.0;
    const double c = -3.0;
    const double x = 132.34;
    const double minX = -1000.0;
    const double maxX = +1000.0;
    const double expected = a + b * x + c * x * x;

    mArticle = new QuadraticFit(a, b, c, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Cubic curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testCubic()
{
    std::cout << "\n Curve Fit 03: Univariate Cubic Test                                    ";

    const double a = -1.0;
    const double b = +2.0;
    const double c = -3.0;
    const double d = -4.0;
    const double x = -7.617;
    const double minX = -1000.0;
    const double maxX = +1000.0;
    const double expected = a + b * x + c * x * x + d * x * x * x;

    mArticle = new CubicFit(a, b, c, d, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Quartic curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testQuartic()
{
    std::cout << "\n Curve Fit 04: Univariate Quartic Test                                  ";

    const double a = -1.0;
    const double b = -2.0;
    const double c = -3.0;
    const double d = +4.0;
    const double e = +5.0;
    const double x = +6.0;
    const double minX = -1000.0;
    const double maxX = +1000.0;
    const double expected = a + b * x + c * x * x + d * x * x * x + e * x * x * x * x;

    mArticle = new QuarticFit(a, b, c, d, e, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Quintic curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testQuintic()
{
    std::cout << "\n Curve Fit 05: Univariate Quintic Test                                  ";

    const double a = -1.0;
    const double b = -2.0;
    const double c = -3.0;
    const double d = +4.0;
    const double e = +5.0;
    const double f = +6.0;
    const double x = +7.0;
    const double minX = -1000.0;
    const double maxX = +1000.0;
    const double expected = a + b * x + c * x * x + d * x * x * x + e * x * x * x * x + f * x * x * x * x * x;

    mArticle = new QuinticFit(a, b, c, d, e, f, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Exponential curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testExponential()
{
    std::cout << "\n Curve Fit 06: Univariate Exponential Test                              ";

    const double a = -1.0;
    const double b = +2.0;
    const double c = -3.0;
    const double x = +13.0;
    const double minX = +10.0;
    const double maxX = +1000.0;
    const double expected = exp(a + b / x + c / (x * x));

    mArticle = new ExponentialFit(a, b, c, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Rational curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testRational()
{
    std::cout << "\n Curve Fit 07: Univariate Rational Test                                 ";

    const double a = +1.0;
    const double b = -2.0;
    const double c = +3.0;
    const double d = -4.0;
    const double x = 316.0;
    const double minX = 315.0;
    const double maxX = 317.0;
    const double expected = (a + b * x) / (1.0 + c * x + d * x * x);

    mArticle = new RationalFit(a, b, c, d, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Power curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testPower()
{
    std::cout << "\n Curve Fit 08: Univariate Power Test                                    ";

    const double a = +1.0;
    const double b = -2.0;
    const double x = 316.0;
    const double minX = 315.0;
    const double maxX = 317.0;
    const double expected = a * pow(b, x);

    mArticle = new PowerFit(a, b, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Inverse Linear curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testInvLinearFit()
{
    std::cout << "\n Curve Fit 09: Univariate Inverse Linear Test                           ";

    const double a = +101.0;
    const double b = -21.0;
    const double c = +0.3;
    const double minX = -201.0;
    const double x = -200.0;
    const double maxX = -199.0;
    const double expected = a * x + b + c / x;

    mArticle = new InvLinearFit(a, b, c, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Inverse Quadratic curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testInvQuadraticFit()
{
    std::cout << "\n Curve Fit 10: Univariate Inverse Quadratic Test                        ";

    const double a = -1.0;
    const double b = +22.0;
    const double c = -330.0;
    const double minX = +10.0;
    const double x = +11.0;
    const double maxX = +12.0;
    const double expected = a + b / x + c / (x * x);

    mArticle = new InvQuadraticFit(a, b, c, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Showmate curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testShowmateFit()
{
    std::cout << "\n Curve Fit 11: Univariate Showmate Test                                 ";

    const double a = +1.0;
    const double b = -2.0;
    const double c = +3.0;
    const double d = -4.0;
    const double e = +5.0;
    const double minX = +31600.0;
    const double x = +31610.0;
    const double maxX = +31620.0;
    const double z = x * 0.001;
    const double expected = a + b * z + c * z * z + d * z * z * z + e / (z * z);

    mArticle = new ShowmateFit(a, b, c, d, e, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Sutherland curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testSutherlandFit()
{
    std::cout << "\n Curve Fit 12: Univariate Sutherland Test                               ";

    const double a = +2.0;
    const double b = -3.0;
    const double minX = +99.0;
    const double x = +100.0;
    const double maxX = +101.0;
    const double expected = a * pow(x, 1.5) / (b + x);

    mArticle = new SutherlandFit(a, b, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Bivariate Quotient curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testQuotientFit()
{
    std::cout << "\n Curve Fit 13: Bivariate Quotient Test                                  ";

    const double a = 17.03056 / 8.314472;
    const double minX = +300.0;
    const double x = +310.0;
    const double maxX = +320.0;
    const double minY = +100000.0;
    const double y = +100001.0;
    const double maxY = +100002.0;
    const double expected = a * y / x;

    mArticle = new QuotientFit(a, minX, maxX, minY, maxY);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x, y), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x, y), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Bivariate Product curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testProductFit()
{
    std::cout << "\n Curve Fit 14: Bivariate Product Test                                   ";

    const double a = 8.314472 / 17.03056;
    const double minX = +300.0;
    const double x = +310.0;
    const double maxX = +320.0;
    const double minY = +600.0;
    const double y = 100001.0 / 310.0 / a;
    const double maxY = +700.0;
    const double expected = 100001.0;

    mArticle = new ProductFit(a, minX, maxX, minY, maxY);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x, y), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x, y), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Bivariate Quadratic, Linear curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testQuadLinFit()
{
    std::cout << "\n Curve Fit 15: Bivariate Quadratic Linear Test                          ";

    const double a = 1.0;
    const double b = 2.0;
    const double c = 3.0;
    const double d = 4.0;
    const double e = 5.0;
    const double f = 6.0;
    const double minX = 7.0;
    const double x = 8.0;
    const double maxX = 9.0;
    const double minY = 10.0;
    const double y = 11.0;
    const double maxY = 12.0;
    const double expected = a + b * y + c * x + d * y * x + e * x * x + f * y * x * x;

    mArticle = new QuadLinFit(a, b, c, d, e, f, minX, maxX, minY, maxY);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x, y), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x, y), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Bivariate Quadratic, Linear Inverse curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testQuadLinInvFit()
{
    std::cout << "\n Curve Fit 16: Bivariate Quadratic Linear Inverse Test                  ";

    /// - Reuse coefficients and variables from previous to demonstrate it really is inverse
    const double a = 1.0;
    const double b = 2.0;
    const double c = 3.0;
    const double d = 4.0;
    const double e = 5.0;
    const double f = 6.0;
    const double minX = 7.0;
    const double x = 8.0;
    const double maxX = 9.0;
    const double z = 11.0;
    const double y = a + b * z + c * x + d * z * x + e * x * x + f * z * x * x;
    const double minY = y - 1.0;
    const double maxY = y + 1.0;

    mArticle = new QuadLinInvFit(a, b, c, d, e, f, minX, maxX, minY, maxY);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(z, mArticle->get(x, y), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(z, mArticle->getExceptional(x, y), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Quadratic Root curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testQuadraticRootFit()
{
    std::cout << "\n Curve Fit 17: Univariate Quadratic Root Test                           ";

    const double a = 8.679227E+002;
    const double b = 1.785E-001;
    LinearFit* article = new LinearFit(a, b, 1.0, 500.0);
    const double expected = 300.0;
    double x = article->get(expected) * expected;
    const double minX = x - 1.0;
    const double maxX = x + 1.0;

    mArticle = new QuadraticRootFit(a, b, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), FLT_EPSILON);

    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Linear fit coefficient accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testCoefficientAccessors()
{
    std::cout << "\n Curve Fit 18: Univariate Linear Coefficient Accessors Test             ";

    const double a = 8.679227E+002;
    const double b = 1.785E-001;
    LinearFit article(a, b, 1.0, 500.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(a, article.getA(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(b, article.getB(), FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test curve fit initialization exception on invalid range (min >= max).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testInitializationException()
{
    std::cout << "\n Curve Fit 19: Initialization Exception                                 ";

    CPPUNIT_ASSERT_THROW(mArticle = new LinearFit(0.0, 0.0, 0.0, 0.0), TsInitializationException);
    delete mArticle;

    CPPUNIT_ASSERT_THROW(mArticle = new QuotientFit(1.0, -1.0, +1.0, +1.0, -1.0),
        TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test curve fit invalid range exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testInvalidRangeExceptions()
{
    std::cout << "\n Curve Fit 20: Invalid Range Exception                                  ";

    const double a = +1.0;
    const double b = +2.0;
    const double c = +3.0;
    const double d = +4.0;
    const double e = +5.0;
    const double f = +6.0;
    const double minX = +101.0;
    const double maxX = +102.0;
    const double x = +103.0;
    const double minY = +201.0;
    const double maxY = +202.0;
    const double y = +203.0;

    mArticle = new LinearFit(a, b, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuadraticFit(a, b, c, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new CubicFit(a, b, c, d, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuarticFit(a, b, c, d, e, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuinticFit(a, b, c, d, e, f, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new ExponentialFit(a, b, c, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new RationalFit(a, b, c, d, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new PowerFit(a, b, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new InvQuadraticFit(a, b, c, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new InvLinearFit(a, b, c, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new ShowmateFit(a, b, c, d, e, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new SutherlandFit(a, b, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuotientFit(a, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional( x, 201.5), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x, 201.5), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(101.5, y), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(101.5, -y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new ProductFit(a, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional( x, 201.5), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x, 201.5), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(101.5, y), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(101.5, -y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuadLinFit(a, b, c, d, e, f, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional( x, 201.5), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x, 201.5), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(101.5, y), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(101.5, -y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuadLinInvFit(a, b, c, d, e, f, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional( x, 201.5), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x, 201.5), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(101.5, y), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(101.5, -y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuadraticRootFit(a, b, minX, maxX);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test curve fit invalid range bounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testInvalidRangeBounds()
{
    std::cout << "\n Curve Fit 21: Invalid Range Bounds                                     ";

    const double a = +1.0;
    const double b = +2.0;
    const double c = +3.0;
    const double d = +4.0;
    const double e = +5.0;
    const double f = +6.0;
    const double minX = +100.0;
    const double x0 = +101.0;
    const double maxX = +102.0;
    const double minY = +201.0;
    const double y0 = +201.0;
    const double maxY = +202.0;

    mArticle = new LinearFit(a, b, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX),
        mArticle->get(minX - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX),
        mArticle->get(maxX + 10.0), mTolerance);
    delete mArticle;

    mArticle = new QuadraticFit(a, b, c, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX),
        mArticle->get(minX - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX),
        mArticle->get(maxX + 10.0), mTolerance);
    delete mArticle;

    mArticle = new CubicFit(a, b, c, d, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX),
        mArticle->get(minX - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX),
        mArticle->get(maxX + 10.0), mTolerance);
    delete mArticle;

    mArticle = new QuarticFit(a, b, c, d, e, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX),
        mArticle->get(minX - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX),
        mArticle->get(maxX + 10.0), mTolerance);
    delete mArticle;

    mArticle = new QuinticFit(a, b, c, d, e, f, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX),
        mArticle->get(minX - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX),
        mArticle->get(maxX + 10.0), mTolerance);
    delete mArticle;

    mArticle = new ExponentialFit(a, b, c, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX),
        mArticle->get(minX - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX),
        mArticle->get(maxX + 10.0), mTolerance);
    delete mArticle;

    mArticle = new RationalFit(a, b, c, d, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX),
        mArticle->get(minX - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX),
        mArticle->get(maxX + 10.0), mTolerance);
    delete mArticle;

    mArticle = new InvQuadraticFit(a, b, c, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX),
        mArticle->get(minX - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX),
        mArticle->get(maxX + 10.0), mTolerance);
    delete mArticle;

    mArticle = new InvLinearFit(a, b, c, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX),
        mArticle->get(minX - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX),
        mArticle->get(maxX + 10.0), mTolerance);
    delete mArticle;

    mArticle = new ShowmateFit(a, b, c, d, e, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX),
        mArticle->get(minX - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX),
        mArticle->get(maxX + 10.0), mTolerance);
    delete mArticle;

    mArticle = new SutherlandFit(a, b, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX),
        mArticle->get(minX - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX),
        mArticle->get(maxX + 10.0), mTolerance);
    delete mArticle;

    mArticle = new QuotientFit(a, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX, y0),
        mArticle->get(minX - 10.0, y0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX, y0),
        mArticle->get(maxX + 10.0, y0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(x0, minY),
        mArticle->get(x0, minY - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(x0, maxY),
        mArticle->get(x0, maxY + 10.0), mTolerance);
    delete mArticle;

    mArticle = new ProductFit(a, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX, y0),
        mArticle->get(minX - 10.0, y0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX, y0),
        mArticle->get(maxX + 10.0, y0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(x0, minY),
        mArticle->get(x0, minY - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(x0, maxY),
        mArticle->get(x0, maxY + 10.0), mTolerance);
    delete mArticle;

    mArticle = new QuadLinFit(a, b, c, d, e, f, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX, y0),
        mArticle->get(minX - 10.0, y0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX, y0),
        mArticle->get(maxX + 10.0, y0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(x0, minY),
        mArticle->get(x0, minY - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(x0, maxY),
        mArticle->get(x0, maxY + 10.0), mTolerance);
    delete mArticle;

    mArticle = new QuadLinInvFit(a, b, c, d, e, f, minX, maxX, minY, maxY);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX, y0),
        mArticle->get(minX - 10.0, y0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX, y0),
        mArticle->get(maxX + 10.0, y0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(x0, minY),
        mArticle->get(x0, minY - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(x0, maxY),
        mArticle->get(x0, maxY + 10.0), mTolerance);
    delete mArticle;

    mArticle = new QuadraticRootFit(a, b, minX, maxX);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX),
        mArticle->get(minX - 10.0), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX),
        mArticle->get(maxX + 10.0), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Exponential curve fit initialization exception on range that contains
///           a divide by zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testExponentialException()
{
    std::cout << "\n Curve Fit 22: Univariate Exponential Initialization Exception Test     ";

    const double a = +1.0;
    const double b = +2.0;
    const double c = +3.0;
    const double minX = -1000.0;
    const double maxX = -0.5 * FLT_EPSILON;

    CPPUNIT_ASSERT_THROW(new ExponentialFit(a, b, c, minX, maxX), TsInitializationException);
    CPPUNIT_ASSERT_THROW(new ExponentialFit(a, b, c, -maxX, -minX), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Rational curve fit initialization exception on range that contains a
///           divide by zero or square root of a negative number.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testRationalException()
{
    std::cout << "\n Curve Fit 23: Univariate Rational Initialization Exception Test        ";

    const double a = +1.0;
    const double b = 0.0;
    const double c = -1000.0;
    const double d = +1.0;
    const double minX = -1000.0;
    const double maxX = +1000.0;

    CPPUNIT_ASSERT_THROW(new RationalFit(a, b, c - FLT_EPSILON / 2, d, minX, maxX),
        TsInitializationException);
    CPPUNIT_ASSERT_THROW(new RationalFit(a, b, -c + FLT_EPSILON / 2, d, minX, maxX),
        TsInitializationException);
    CPPUNIT_ASSERT_THROW(new RationalFit(a, b, 0.0, FLT_EPSILON / 256.0, minX, maxX),
        TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Inverse Linear curve fit initialization exception on range that
///           contains a divide by zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testInvLinearException()
{
    std::cout << "\n Curve Fit 24: Univariate Inverse Linear Initialization Exception Test  ";

    const double a = +1.0;
    const double b = 0.0;
    const double c = -1000.0;

    double minX = FLT_EPSILON * 0.5;
    double maxX = 1000.0;
    CPPUNIT_ASSERT_THROW(new InvLinearFit(a, b, c, minX, maxX), TsInitializationException);

    minX = -1000.0;
    maxX = -FLT_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(new InvLinearFit(a, b, c, -maxX, -minX), TsInitializationException);

    minX = -0.5 * FLT_EPSILON;
    maxX = +0.5 * FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(new InvLinearFit(a, b, c, -maxX, -minX), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Inverse Quadratic curve fit initialization exception on range that
///           contains a divide by zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testInvQuadraticException()
{
    std::cout << "\n Curve Fit 25: Univariate Inverse Quadratic Initialize Exception Test   ";

    const double a = +1.0;
    const double b = +2.0;
    const double c = +3.0;

    double minX = FLT_EPSILON * 0.5;
    double maxX = 1000.0;
    CPPUNIT_ASSERT_THROW(new InvQuadraticFit(a, b, c, minX, maxX), TsInitializationException);

    minX = -1000.0;
    maxX = -FLT_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(new InvQuadraticFit(a, b, c, -maxX, -minX), TsInitializationException);

    minX = -0.5 * FLT_EPSILON;
    maxX = +0.5 * FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(new InvQuadraticFit(a, b, c, -maxX, -minX), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Test Univariate Showmate curve fit initialization exception on range that contains a
///            divide by zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testShowmateException()
{
    std::cout << "\n Curve Fit 26: Univariate Showmate Initialization Exception Test        ";

    const double a = +1.0;
    const double b = +2.0;
    const double c = +3.0;
    const double d = +4.0;
    const double e = +5.0;
    const double minX = -2.0 * FLT_EPSILON;
    const double maxX = +0.5 * FLT_EPSILON;

    CPPUNIT_ASSERT_THROW(new ShowmateFit(a, b, c, d, e, minX, maxX), TsInitializationException);
    CPPUNIT_ASSERT_THROW(new ShowmateFit(a, b, c, d, e, -maxX, -minX), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Univariate Sutherland curve fit initialization exception on range that contains a
///           divide by zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testSutherlandException()
{
    std::cout << "\n Curve Fit 27: Univariate Sutherland Initialization Exception Test      ";

    const double a = +1.0;
    const double b = +0.0;
    double minX = -0.5 * FLT_EPSILON;
    double maxX = +0.5 * FLT_EPSILON;

    CPPUNIT_ASSERT_THROW(new SutherlandFit(a, b, minX, maxX), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Bivariate Quotient curve fit initialization exception on range that contains
///           a divide by zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testQuotientException()
{
    std::cout << "\n Curve Fit 28: Bivariate Quotient Initialization Exception Test         ";

    const double a = 1.0;
    double minX = FLT_EPSILON * 0.1;
    double maxX = +1000.0;
    double minY = -1000.0;
    double maxY = +1000.0;
    CPPUNIT_ASSERT_THROW(new QuotientFit(a, minX, maxX, minY, maxY), TsInitializationException);

    minX = -1000.0;
    maxX = -FLT_EPSILON * 0.1;
    CPPUNIT_ASSERT_THROW(new QuotientFit(a, minX, maxX, minY, maxY), TsInitializationException);

    minX = -FLT_EPSILON * 0.1;
    maxX = +FLT_EPSILON * 0.1;
    CPPUNIT_ASSERT_THROW(new QuotientFit(a, minX, maxX, minY, maxY), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Bivariate Quadratic Linear Inverse fit initialization exception on range that
///           contains a divide by zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testQuadLinInvException()
{
    std::cout << "\n Curve Fit 29: Bivariate Quadratic Linear Inverse Init Exception Test   ";

    const double a = 0.0;
    const double b = 0.0;
    const double c = FLT_EPSILON;
    const double d = 0.0;
    const double e = 0.0;
    const double f = FLT_EPSILON;
    const double minX = -1000.0;
    const double maxX = +1000.0;
    const double minY = -1000.0;
    const double maxY = +1000.0;

    CPPUNIT_ASSERT_THROW(new QuadLinInvFit(a, b, c, 0.0, e, -f, 0.0, maxX, minY, maxY),
        TsInitializationException);
    CPPUNIT_ASSERT_THROW(new QuadLinInvFit(a, b, c, d, e, -0.5 * f, minX, 0.0, minY, maxY),
        TsInitializationException);
    CPPUNIT_ASSERT_THROW(new QuadLinInvFit(a, b, 0.0, 0.0, 0.0, 0.0, minX, maxX, minY, maxY),
        TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Test Univariate Quadratic Root curve fit initialization exception on range that
///            contains a negative square root.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testQuadraticRootException()
{
    std::cout << "\n Curve Fit 30: Univariate Quadratic Root Initialization Exception Test  ";

    {
        const double a = 0.0;
        const double b = 1.0;
        const double minX = -10.0;
        const double maxX = 10.0;

        CPPUNIT_ASSERT_THROW(new QuadraticRootFit(a, b, minX, maxX),
            TsInitializationException);
    }
    {
        const double a = 1.0;
        const double b = 0.0;
        const double minX = -10.0;
        const double maxX = 10.0;

        CPPUNIT_ASSERT_THROW(new QuadraticRootFit(a, b, minX, maxX),
            TsInitializationException);
    }

    std::cout << "... Pass";
}

double UtTsCurveFit::dummy(EvaluateFunction eval, TsApproximation* c)
{
    double result =  (c->*eval)(1.0, 0.0);
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Test function pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testFunctionPointer()
{
    std::cout << "\n Curve Fit 31: Function Pointer Test                                    ";

    const double a    = 0.0;
    const double b    = 1.0;
    const double minX = -10.0;
    const double maxX = 10.0;
    LinearFit* article = new LinearFit(a, b, minX, maxX);
    EvaluateFunction eval = &TsApproximation::get;
    double result = (article->*eval)(1.0, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, result, 0.0);
    double returned = dummy(eval, article);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, returned, 0.0);
    delete article;

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Test exception on getExceptional when default constructed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsCurveFit::testEmptyConstructors()
{
    std::cout << "\n Curve Fit 32: Constructor Calls with No Arguments                      ";
    double x = 0.1;
    double y = -0.1;

    mArticle = new LinearFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuadraticFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new CubicFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuarticFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuinticFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new ExponentialFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new RationalFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new PowerFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new InvQuadraticFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new InvLinearFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new ShowmateFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new SutherlandFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuotientFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new ProductFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuadLinFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuadLinInvFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new QuadraticRootFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x), TsOutOfBoundsException);


    std::cout << "... Pass." << std::endl;
    std::cout << ".--------------------------------------------------------------------------------";
}
