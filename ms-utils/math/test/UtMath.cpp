/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ()
***************************************************************************************************/

#include <iostream>

#include "math/Math.hh"

#include "UtMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this math unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtMath::UtMath()
    :
    CppUnit::TestFixture(),
    m_defaultTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this math unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtMath::~UtMath()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::setUp()
{
    // show no mercy
    m_defaultTolerance = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for resolving an angle to the range [-pi,+pi).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testAnglePi()
{
    std::cout << "\n................................................................................";
    std::cout << "\n.Math Test 01: Angle Resolution to [-pi,+pi) Tests..........................";

    /// @test for arguments from -3 Pi to +3 Pi by Pi/2

    double expected  = -1.0  * UnitConversion::PI_UTIL;
    double returned  = Math::anglePi(-3.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = -UnitConversion::PI_OVER_2;
    returned  = Math::anglePi(-2.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.0 * UnitConversion::PI_UTIL;
    returned  = Math::anglePi(-2.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = +0.5 * UnitConversion::PI_UTIL;
    returned  = Math::anglePi(-1.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = -1.0 * UnitConversion::PI_UTIL;
    returned  = Math::anglePi(-1.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = -0.5 * UnitConversion::PI_UTIL;
    returned  = Math::anglePi(-0.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.0;
    returned  = Math::anglePi(0.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = +0.5 * UnitConversion::PI_UTIL;
    returned  = Math::anglePi(+0.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = -1.0 * UnitConversion::PI_UTIL;
    returned  = Math::anglePi(+1.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = -0.5 * UnitConversion::PI_UTIL;
    returned  = Math::anglePi(+1.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.0;
    returned  = Math::anglePi(+2.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = +0.5 * UnitConversion::PI_UTIL;
    returned  = Math::anglePi(+2.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = UnitConversion::PI_UTIL - FLT_EPSILON;
    returned  = Math::anglePi(+3.0 * UnitConversion::PI_UTIL - FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for resolving an angle to the range [0,+2pi).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testAngle2Pi()
{
    std::cout << "\n.Math Test 02: Angle Resolution to [0,+2pi) Tests...........................";

    /// @test for arguments from -4 Pi to +4 Pi by Pi/2

    double expected  = 0.0;
    double returned  = Math::angle2Pi(-4.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.5 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(-3.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.0 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(-3.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.5 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(-2.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.0 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(-2.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.5 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(-1.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.0 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(-1.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.5 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(-0.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.0 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(0.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.5 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(+0.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.0 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(+1.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.5 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(+1.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.0 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(+2.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.5 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(+2.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.0 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(+3.0 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.5 * UnitConversion::PI_UTIL;
    returned  = Math::angle2Pi(+3.5 * UnitConversion::PI_UTIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 2.0 * UnitConversion::PI_UTIL - FLT_EPSILON;
    returned  = Math::angle2Pi(+4.0 * UnitConversion::PI_UTIL - FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for resolving an angle to the range [-180,+180).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testAngle180()
{
    std::cout << "\n.Math Test 03: Angle Resolution to [-180,+180) Tests........................";

    /// @test for arguments from -540 to +540 by 90

    double expected  = -1.0 * 180.0;
    double returned  = Math::angle180(-3.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = -0.5 * 180.0;
    returned  = Math::angle180(-2.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.0 * 180.0;
    returned  = Math::angle180(-2.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = +0.5 * 180.0;
    returned  = Math::angle180(-1.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = -1.0 * 180.0;
    returned  = Math::angle180(-1.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = -0.5 * 180.0;
    returned  = Math::angle180(-0.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.0 * 180.0;
    returned  = Math::angle180(0.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.5 * 180.0;
    returned  = Math::angle180(+0.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = -1.0 * 180.0;
    returned  = Math::angle180(+1.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = -0.5 * 180.0;
    returned  = Math::angle180(+1.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.0 * 180.0;
    returned  = Math::angle180(+2.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = +0.5 * 180.0;
    returned  = Math::angle180(+2.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = +1.0 * 180.0 - FLT_EPSILON;
    returned  = Math::angle180(+3.0 * 180.0 - FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for resolving an angle to the range [0,+360).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testAngle360()
{
    std::cout << "\n.Math Test 04: Angle Resolution to [0,+360) Tests...........................";

    /// @test for arguments from -720 to +720 by 90

    double expected  = 0.0 * 180.0;
    double returned  = Math::angle360(-4.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.5 * 180.0;
    returned  = Math::angle360(-3.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.0 * 180.0;
    returned  = Math::angle360(-3.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.5 * 180.0;
    returned  = Math::angle360(-2.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.0 * 180.0;
    returned  = Math::angle360(-2.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.5 * 180.0;
    returned  = Math::angle360(-1.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.0 * 180.0;
    returned  = Math::angle360(-1.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.5 * 180.0;
    returned  = Math::angle360(-0.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.0 * 180.0;
    returned  = Math::angle360(0.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.5 * 180.0;
    returned  = Math::angle360(+0.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.0 * 180.0;
    returned  = Math::angle360(+1.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.5 * 180.0;
    returned  = Math::angle360(+1.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.0 * 180.0;
    returned  = Math::angle360(+2.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 0.5 * 180.0;
    returned  = Math::angle360(+2.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.0 * 180.0;
    returned  = Math::angle360(+3.0 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 1.5 * 180.0;
    returned  = Math::angle360(+3.5 * 180.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    expected  = 2.0 * 180.0 - FLT_EPSILON;
    returned  = Math::angle360(+4.0 * 180.0 - FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for protected (against domain error) arcsine function.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testProtectedAsin()
{
    std::cout << "\n.Math Test 05: Protected Arcsine Tests......................................";

    /// @test for argument well beyond upper limit
    double expected  = UnitConversion::PI_OVER_2;
    double returned  = Math::protectedAsin(+2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at upper limit
    expected  = UnitConversion::PI_OVER_2;
    returned  = Math::protectedAsin(+1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument just within upper limit
    expected  = asin(+1.0 - 2.0 * FLT_EPSILON);
    returned  = Math::protectedAsin(+1.0 - 2.0 * FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument well within limits
    expected  = 0.0;
    returned  = Math::protectedAsin(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument just within lower limit
    expected  = asin(-1.0 + 2.0 * FLT_EPSILON);
    returned  = Math::protectedAsin(-1.0 + 2.0 * FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at lower limit
    expected  = -UnitConversion::PI_OVER_2;
    returned  = Math::protectedAsin(-1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument well beyond lower limit
    expected  = -UnitConversion::PI_OVER_2;
    returned  = Math::protectedAsin(-2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for protected (against domain error) arccosine function.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testProtectedAcos()
{
    std::cout << "\n.Math Test 06: Protected Arccosine Tests....................................";

    /// @test for argument well beyond upper limit
    double expected  = 0.0;
    double returned  = Math::protectedAcos(+2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at upper limit
    expected  = 0.0;
    returned  = Math::protectedAcos(+1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument just within upper limit
    expected  = acos(+1.0 - 2.0 * FLT_EPSILON);
    returned  = Math::protectedAcos(+1.0 - 2.0 * FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument well within limits
    expected  = UnitConversion::PI_OVER_2;
    returned  = Math::protectedAcos(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument just within lower limit
    expected  = acos(-1.0 + 2.0 * FLT_EPSILON);
    returned  = Math::protectedAcos(-1.0 + 2.0 * FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at lower limit
    expected  = UnitConversion::PI_UTIL;
    returned  = Math::protectedAcos(-1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument well beyond lower limit
    expected  = UnitConversion::PI_UTIL;
    returned  = Math::protectedAcos(-2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for protected (against domain error) sqrt function.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testProtectedSqrt()
{
    std::cout << "\n.Math Test 07: Protected Square Root Tests..................................";

    /// @test for argument well within lower limit
    double expected  = 10.0;
    double returned  = Math::protectedSqrt(+100.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument just within lower limit
    expected  = sqrt(FLT_EPSILON);
    returned  = Math::protectedSqrt(FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at lower limit
    expected  = sqrt(0.0);
    returned  = Math::protectedSqrt(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument just beyond lower limit
    expected  = 0.0;
    returned  = Math::protectedSqrt(-FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument well beyond lower limit
    expected  = 0.0;
    returned  = Math::protectedSqrt(-100.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for protected (against domain error) log10 function.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testProtectedLog10()
{
    std::cout << "\n.Math Test 08: Protected Log 10 Tests.......................................";

    /// @test for argument well within lower limit
    double expected  = log10(+100.0);
    double returned  = Math::protectedLog10(+100.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument just within lower limit
    expected  = log10(FLT_EPSILON);
    returned  = Math::protectedLog10(FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at lower limit
    expected  = 0.0;
    returned  = Math::protectedLog10(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument just beyond lower limit
    expected  = 0.0;
    returned  = Math::protectedLog10(-FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument well beyond lower limit
    expected  = 0.0;
    returned  = Math::protectedLog10(-100.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for protected (against domain error) log function.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testProtectedLog()
{
    std::cout << "\n.Math Test 09: Protected Log Tests..........................................";

    /// @test for argument well within lower limit
    double expected  = log(+100.0);
    double returned  = Math::protectedLog(+100.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument just within lower limit
    expected  = log(FLT_EPSILON);
    returned  = Math::protectedLog(FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at lower limit
    expected  = 0.0;
    returned  = Math::protectedLog(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument just beyond lower limit
    expected  = 0.0;
    returned  = Math::protectedLog(-FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument well beyond lower limit
    expected  = 0.0;
    returned  = Math::protectedLog(-100.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for protected (against domain error) log function.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testprotectedDiv()
{
    std::cout << "\n.Math Test 10: Protected Div Tests..........................................";

    /// @test for negative denominator inside default threshold with default zero return value
    double den       = -DBL_EPSILON * 0.5;
    double expected  =  0.0;
    double returned  =  Math::protectedDiv(2.0, den);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for negative denominator outside default threshold with default zero return value
    den              = -DBL_EPSILON * 2.0;
    expected         = -0.5 / DBL_EPSILON;
    returned         =  Math::protectedDiv(1.0, den);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for positive denominator inside default threshold with default zero return value
     den             =  DBL_EPSILON * 0.5;
     expected        =  0.0;
     returned        =  Math::protectedDiv(-1.0, den);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for positive denominator outside default threshold with default zero return value
    den              =  DBL_EPSILON * 2.0;
    expected         =  0.5 / DBL_EPSILON;
    returned         =  Math::protectedDiv(1.0, den);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for negative denominator inside non-default threshold with default zero return value
    den              = -FLT_EPSILON * 0.5;
    expected         =  0.0;
    returned         =  Math::protectedDiv(-1000.0, den, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for negative denominator outside non-default threshold with default zero return value
    den              = -FLT_EPSILON * 2.0;
    expected         = -0.5 / FLT_EPSILON;
    returned         =  Math::protectedDiv(1.0, den, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for positive denominator inside non-default threshold with default zero return value
    den              =  FLT_EPSILON * 0.5;
    expected         =  0.0;
    returned         =  Math::protectedDiv(1000.0, den, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for positive denominator outside non-default threshold with default zero return value
    den              =  FLT_EPSILON * 2.0;
    expected         = -0.5 / FLT_EPSILON;
    returned         =  Math::protectedDiv(-1.0, den, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for negative denominator inside non-default threshold with non-default zero return value
    den              = -FLT_EPSILON * 0.5;
    expected         =  1.0;
    returned         =  Math::protectedDiv(-123456789.0, den, FLT_EPSILON, expected);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for positive denominator inside non-default threshold with non-default zero return value
    den              =  FLT_EPSILON * 0.5;
    expected         = -1.0;
    returned         =  Math::protectedDiv(987654321.0, den, FLT_EPSILON, expected);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for positive denominator inside negative threshold with default zero return value
    den              =  FLT_EPSILON * 0.5;
    expected         =  1.0 / den;
    returned         =  Math::protectedDiv(1.0, den, -FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for positive denominator outside negative threshold with default zero return value
    den              =  FLT_EPSILON * 2.0;
    expected         =  1.0 / den;
    returned         =  Math::protectedDiv(1.0, den, -FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for negative denominator inside negative threshold with default zero return value
    den              = -FLT_EPSILON * 0.5;
    expected         =  1.0 / den;
    returned         =  Math::protectedDiv(1.0, den, -FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for positive denominator inside negative threshold with default zero return value
    den              =  FLT_EPSILON * 0.5;
    expected         =  1.0 / den;
    returned         =  Math::protectedDiv(1.0, den, -FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for range limiting doubles.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testLimitRangeDouble()
{
    std::cout << "\n.Math Test 11: Limit Range Double Tests.....................................";

    /// @test for argument well within range
    double expected  = 1;
    double returned  = Math::limitRange(0, 1, 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at lower limit
    expected  = 1;
    returned  = Math::limitRange(1, 1, 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument below lower limit
    expected  = 1;
    returned  = Math::limitRange(1, 0, 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at upper limit
    expected  = 1;
    returned  = Math::limitRange(0, 1, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument above upper limit
    expected  = 1;
    returned  = Math::limitRange(0, 2, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for upper < lower
    expected  = 2;
    returned  = Math::limitRange(2, 1, 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for range limiting integers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testLimitRangeInt()
{
    std::cout << "\n.Math Test 12: Limit Range Integer Tests....................................";

    /// @test for argument well within range
    double expected  = 1.0;
    double returned  = Math::limitRange(0.0, 1.0, 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at lower limit
    expected  = 1.0;
    returned  = Math::limitRange(1.0, 1.0, 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument below lower limit
    expected  = 1.0;
    returned  = Math::limitRange(1.0, 0.0, 2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at upper limit
    expected  = 1.0;
    returned  = Math::limitRange(0.0, 1.0, 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument above upper limit
    expected  = 1.0;
    returned  = Math::limitRange(0.0, 2.0, 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for upper < lower
    expected  = 2.0;
    returned  = Math::limitRange(2.0, 1.0, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for checking doubles in range.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testIsInRangeDouble()
{
    std::cout << "\n.Math Test 13: Is In Range Double Tests.....................................";

    /// @test for argument well within range
    CPPUNIT_ASSERT(Math::isInRange(0.0, 1.0, 2.0));

    /// @test for argument at lower limit
    CPPUNIT_ASSERT(Math::isInRange(1.0, 1.0, 2.0));

    /// @test for argument below lower limit
    CPPUNIT_ASSERT(!Math::isInRange(1.0, 0.0, 2.0));

    /// @test for argument at upper limit
    CPPUNIT_ASSERT(Math::isInRange(0.0, 1.0, 1.0));

    /// @test for argument above upper limit
    CPPUNIT_ASSERT(!Math::isInRange(0.0, 2.0, 1.0));

    /// @test for upper < lower
    CPPUNIT_ASSERT(!Math::isInRange(2.0, 1.0, 0.0));


    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for checking integers in range.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testIsInRangeInt()
{
    std::cout << "\n.Math Test 14: Is In Range Integer Tests....................................";

    /// @test for argument well within range
    CPPUNIT_ASSERT(Math::isInRange(0, 1, 2));

    /// @test for argument at lower limit
    CPPUNIT_ASSERT(Math::isInRange(1, 1, 2));

    /// @test for argument below lower limit
    CPPUNIT_ASSERT(!Math::isInRange(1, 0, 2));

    /// @test for argument at upper limit
    CPPUNIT_ASSERT(Math::isInRange(0, 1, 1));

    /// @test for argument above upper limit
    CPPUNIT_ASSERT(!Math::isInRange(0, 2, 1));

    /// @test for upper < lower
    CPPUNIT_ASSERT(!Math::isInRange(2, 1, 0));


    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for range limiting for doubles.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testInnerLimitDouble()
{
    std::cout << "\n.Math Test 15: Inner Limit Double Tests.....................................";

    /// @test for argument well below lower limit
    double expected  = -1.0;
    double returned  = Math::innerLimit(0.0, -1.0, 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at lower limit
    expected  = -1.0;
    returned  = Math::innerLimit(-1.0, -1.0, 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument between lower limit and middle
    expected  = -1.0;
    returned  = Math::innerLimit(-1.0, -0.5, 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at middle
    expected  = +1.0;
    returned  = Math::innerLimit(-1.0, 0.0, 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument between middle and upper limit
    expected  = 5.0;
    returned  = Math::innerLimit(1.0, 4.0, 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument at upper limit
    expected  = 5.0;
    returned  = Math::innerLimit(1.0, 5.0, 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for argument well above upper limit
    expected  = 5.0;
    returned  = Math::innerLimit(-17.0, 5.0, 3.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for DBL_EPSILON limits
    expected  = DBL_EPSILON;
    returned  = Math::innerLimit(-DBL_EPSILON, DBL_EPSILON/3.0, +DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    /// @test for DBL_EPSILON limits with input arg 0
    expected  = DBL_EPSILON;
    returned  = Math::innerLimit(-DBL_EPSILON, 0.0, +DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  m_defaultTolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for range limiting for integers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testInnerLimitInt()
{
    std::cout << "\n.Math Test 16: Inner Limit Integer Tests....................................";

    /// @test for argument well below lower limit
    int expected  = -1;
    int returned  = Math::innerLimit(0, -1, 1);
    CPPUNIT_ASSERT(expected == returned);

    /// @test for argument at lower limit
    expected  = -1;
    returned  = Math::innerLimit(-1, -1, 1);
    CPPUNIT_ASSERT(expected == returned);

    /// @test for argument between lower limit and middle
    expected  = -3;
    returned  = Math::innerLimit(-3, -1, 3);
    CPPUNIT_ASSERT(expected == returned);

    /// @test for argument at middle
    expected  = +1.0;
    returned  = Math::innerLimit(-1, 0, 1);
    CPPUNIT_ASSERT(expected == returned);

    /// @test for argument between middle and upper limit
    expected  = 5.0;
    returned  = Math::innerLimit(1, 4, 5);
    CPPUNIT_ASSERT(expected == returned);

    /// @test for argument at upper limit
    expected  = 5.0;
    returned  = Math::innerLimit(1, 5, 5);
    CPPUNIT_ASSERT(expected == returned);

    /// @test for argument well above upper limit
    expected  = 5.0;
    returned  = Math::innerLimit(-17, 5, 3);
    CPPUNIT_ASSERT(expected == returned);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for rounding doubles to the nearest integer value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testRounding()
{
    std::cout << "\n.Math Test 17: Rounding Tests...............................................";

    /// @test for argument exactly equal to integer
    int expected = 1;
    int returned = Math::round(1.0);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// @test for argument to be rounded down
    expected = 10;
    returned = Math::round(10.25);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    expected = -11;
    returned = Math::round(-10.75);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// @test for argument with fractional value of 0.5 (should be rounded up)
    expected = 16;
    returned = Math::round(15.5);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    expected = -15;
    returned = Math::round(-15.5);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// @test for argument to be rounded up
    expected = 11;
    returned = Math::round(10.75);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    expected = -11;
    returned = Math::round(-11.25);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for quantize.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testQuantize()
{
    std::cout << "\n.Math Test 18: Quantize Tests...............................................";

    double arg = 3.14159;
    double expected = 3.0;
    double returned = Math::quantize(arg, 1);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    expected = 3.1;
    returned = Math::quantize(arg, 0.1);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    expected = 3.14;
    returned = Math::quantize(arg, 0.01);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    expected = 3.142;
    returned = Math::quantize(arg, 0.001);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    expected = 3.1416;
    returned = Math::quantize(arg, 0.0001);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    expected = 3.14159;
    returned = Math::quantize(arg, DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// Zero resolution
    expected = 3.14159;
    returned = Math::quantize(arg, 0.0);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// Negative input.
    arg = -3.14159;
    expected = -3.1;
    returned = Math::quantize(arg, 0.1);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// Negative input.
    expected = -3.14;
    returned = Math::quantize(arg, 0.01);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// Negative input.
    expected = -3.142;
    returned = Math::quantize(arg, 0.001);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// Negative input.
    expected = -3.1416;
    returned = Math::quantize(arg, 0.0001);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// Minimum limit resolution.
    expected = -3.14159;
    returned = Math::quantize(arg, DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// Zero resolution
    expected = -3.14159;
    returned = Math::quantize(arg, 0.0);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// Negative resolution
    expected = -3.14159;
    returned = Math::quantize(arg, -1.0);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// Bigger input, 10 resolution
    arg = 123456.7;
    expected = 123460.0;
    returned = Math::quantize(arg, 10.0);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    /// Bigger input, 100 resolution
    expected = 123500.0;
    returned = Math::quantize(arg, 100.0);
    CPPUNIT_ASSERT_EQUAL(expected, returned);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for fastPow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMath::testFastPow()
{
    std::cout << "\n.Math Test 19: fastPow Tests................................................";

    double base = 0.0;
    double exp  = 1.25;
    double expected = 0.0;
    double returned = Math::fastPow(base, exp);
    CPPUNIT_ASSERT_EQUAL(expected, returned);
    
    base = 16.0;
    expected = 32.0;
    returned = Math::fastPow(base, exp);
    CPPUNIT_ASSERT_EQUAL(expected, returned);
    
    exp = -1.25;
    expected = 0.03125;
    returned = Math::fastPow(base, exp);
    CPPUNIT_ASSERT_EQUAL(expected, returned); 
    
    base = -4.0;
    exp = 0;
    expected = 1.0;
    returned = Math::fastPow(base, exp);
    CPPUNIT_ASSERT_EQUAL(expected, returned);    
    
    exp = 2.0;
    expected = 16.0;
    returned = Math::fastPow(base, exp);
    CPPUNIT_ASSERT_EQUAL(expected, returned); 
    
    exp = -5.0;
    expected = -0.0009765625;
    returned = Math::fastPow(base, exp);
    CPPUNIT_ASSERT_EQUAL(expected, returned);          
    
    std::cout << "Pass";
}    
    
