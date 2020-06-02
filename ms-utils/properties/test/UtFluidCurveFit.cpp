/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
     (properties/FluidTsatFit.o)
     (properties/FluidHvapFit.o)
 )
 ***************************************************************************************************/

#include "UtFluidCurveFit.hh"

#include "properties/FluidTsatFit.hh"
#include "properties/FluidHvapFit.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this curve fit unit-test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtFluidCurveFit::UtFluidCurveFit()
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
UtFluidCurveFit::~UtFluidCurveFit()
{
    //Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidCurveFit::tearDown()
{
    delete mArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidCurveFit::setUp()
{
    mArticle   = 0;
    mTolerance = 0.001;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Saturation Temperature curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidCurveFit::testTsat()
{
    std::cout << "--------------------------------------------------------------------------------";
    std::cout << "\n Fluid Curve Fit 01: Saturation Temperature Test                        ";

    const double a        =    5.0;
    const double b        =   -2.0;
    const double c        =    0.2;
    const double x        =  200.0;
    const double minX     =    7.5;
    const double maxX     = 1300.0;
    const double expected = (-b - sqrt(b*b - 4.0*c*(a-log10(x))))/(2*c);

    mArticle = new FluidTsatFit(a, b, c, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Heat of Vaporization curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidCurveFit::testHvap()
{
    std::cout << "\n Fluid Curve Fit 02: Heat of Vaporization Test                          ";

    const double a        = 620.0;
    const double alpha    =  -0.22;
    const double beta     =   0.4;
    const double x        = 150.0/190.0;
    const double minX     =  90.0/190.0;
    const double maxX     =   1.0;
    const double expected = a * exp(-alpha*x) * pow((1.0-x), beta);

    mArticle = new FluidHvapFit(a, alpha, beta, minX, maxX);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->get(x), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getExceptional(x), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test curve fit initialization exception on invalid range (min >= max).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidCurveFit::testInitializationException()
{
    std::cout << "\n Fluid Curve Fit 03: Initialization Exception                           ";

    CPPUNIT_ASSERT_THROW(mArticle = new FluidTsatFit(0.0, 0.0, 0.0, 0.0, 0.0),
                         TsInitializationException);
    delete mArticle;

    CPPUNIT_ASSERT_THROW(mArticle = new FluidHvapFit(1.0, -1.0, +1.0, +1.0, -1.0),
                         TsInitializationException);
    delete mArticle;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test curve fit invalid range exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidCurveFit::testInvalidRangeExceptions()
{
    std::cout << "\n Fluid Curve Fit 04: Invalid Range Exception                            ";

    {
        const double a        =    5.0;
        const double b        =   -2.0;
        const double c        =    0.2;
        const double x        = 1500.0;
        const double minX     =    7.5;
        const double maxX     = 1300.0;

        mArticle = new FluidTsatFit(a, b, c, minX, maxX);

        CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x),  TsOutOfBoundsException);
        CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
        delete mArticle;
    } {
        const double a        = 620.0;
        const double alpha    =  -0.22;
        const double beta     =   0.4;
        const double x        =   2.0;
        const double minX     =  90.0/190.0;
        const double maxX     =   1.0;

        mArticle = new FluidHvapFit(a, alpha, beta, minX, maxX);

        CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x),  TsOutOfBoundsException);
        CPPUNIT_ASSERT_THROW(mArticle->getExceptional(-x), TsOutOfBoundsException);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test curve fit invalid range bounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidCurveFit::testInvalidRangeBounds()
{
    std::cout << "\n Fluid Curve Fit 05: Invalid Range Bounds                               ";

    {
        const double a        =    5.0;
        const double b        =   -2.0;
        const double c        =    0.2;
        const double minX     =    7.5;
        const double maxX     = 1300.0;

        mArticle = new FluidTsatFit(a, b, c, minX, maxX);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX), mArticle->get(minX - 1.0), FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX), mArticle->get(maxX + 1.0), FLT_EPSILON);
        delete mArticle;
    } {
        const double a        = 620.0;
        const double alpha    =  -0.22;
        const double beta     =   0.4;
        const double minX     =  90.0/190.0;
        const double maxX     =   1.0;

        mArticle = new FluidHvapFit(a, alpha, beta, minX, maxX);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(minX), mArticle->get(minX - 1.0), FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getExceptional(maxX), mArticle->get(maxX + 1.0), FLT_EPSILON);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test initialization exceptions specific to Saturation Temperature curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidCurveFit::testTsatException()
{
    std::cout << "\n Fluid Curve Fit 06: Saturation Temperature Init Exception Test         ";

    const double a        =    5.0;
    const double b        =   -2.0;
    const double c        =    0.2;
    const double minX     =    7.5;
    const double maxX     = 1300.0;

    /// @test throws on c = 0.
    CPPUNIT_ASSERT_THROW(new FluidTsatFit(a, b, 0.0, minX, maxX), TsInitializationException);

    /// @test throws on minX causes sqrt negative number.
    CPPUNIT_ASSERT_THROW(new FluidTsatFit(6.0, b, c, minX, maxX), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test initialization exceptions specific to Heat of Vaporization curve fit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidCurveFit::testHvapException()
{
    std::cout << "\n Fluid Curve Fit 07: Heat of Vaporization Init Exception Test           ";

    const double a        = 620.0;
    const double alpha    =  -0.22;
    const double beta     =   0.4;
    const double minX     =  90.0/190.0;
    const double maxX     =   1.0;

    /// @test throws on |alpha| > 50.
    CPPUNIT_ASSERT_THROW(new FluidHvapFit(a,  50.1,  beta, minX, maxX), TsInitializationException);
    CPPUNIT_ASSERT_THROW(new FluidHvapFit(a, -50.1,  beta, minX, maxX), TsInitializationException);

    /// @test throws on |alpha| > 50.
    CPPUNIT_ASSERT_THROW(new FluidHvapFit(a, alpha,  50.1, minX, maxX), TsInitializationException);
    CPPUNIT_ASSERT_THROW(new FluidHvapFit(a, alpha, -50.1, minX, maxX), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Test function pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidCurveFit::testFunctionPointer()
{
    std::cout << "\n Fluid Curve Fit 08: Function Pointer Test                              ";

    {
        const double a        =    5.0;
        const double b        =   -2.0;
        const double c        =    0.2;
        const double x        =  200.0;
        const double minX     =    7.5;
        const double maxX     = 1300.0;
        const double expected = (-b - sqrt(b*b - 4.0*c*(a-log10(x))))/(2*c);

        FluidTsatFit* article = new FluidTsatFit(a, b, c, minX, maxX);
        EvaluateFunction eval = &TsApproximation::get;
        double result = (article->*eval)(x, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, result, FLT_EPSILON);
        delete article;
    } {
        const double a        = 620.0;
        const double alpha    =  -0.22;
        const double beta     =   0.4;
        const double x        = 150.0/190.0;
        const double minX     =  90.0/190.0;
        const double maxX     =   1.0;
        const double expected = a * exp(-alpha*x) * pow((1.0-x), beta);

        FluidHvapFit* article = new FluidHvapFit(a, alpha, beta, minX, maxX);
        EvaluateFunction eval = &TsApproximation::get;
        double result = (article->*eval)(x, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, result, FLT_EPSILON);
        delete article;
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Test exception on getExceptional when default constructed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidCurveFit::testEmptyConstructors()
{
    std::cout << "\n Fluid Curve Fit 09: Constructor Calls with No Arguments                ";
    double x = 0.1;
    double y = -0.1;

    mArticle = new FluidTsatFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);
    delete mArticle;

    mArticle = new FluidHvapFit();
    CPPUNIT_ASSERT_THROW(mArticle->getExceptional(x,y), TsOutOfBoundsException);

    std::cout << "... Pass." << std::endl;
    std::cout << ".--------------------------------------------------------------------------------";
}
