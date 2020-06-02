#ifndef UtUnitConversion_EXISTS
#define UtUnitConversion_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_UTILITIES_MATH_UNIT_CONVERSION    Unit Conversion Unit Tests
/// @ingroup  UT_TSM_UTILITIES_MATH
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Unit Conversion class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Unit conversion unit tests.
////
/// @details  This class provides the unit tests for the Unit Conversion class within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtUnitConversion: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Unit Conversion unit test.
        UtUnitConversion();
        /// @brief    Default destructs this Unit Conversion unit test.
        virtual ~UtUnitConversion();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests miscellaneous conversion factors.
        void testMiscConversion();
        /// @brief    Tests for angle conversion factors.
        void testAngleConversion();
        /// @brief    Tests for length conversion factors.
        void testLengthConversion();
        /// @brief    Tests for mass conversion factors.
        void testMassConversion();
        /// @brief    Tests for pressure conversion factors.
        void testPressureConversion();
        /// @brief    Tests for energy conversion factors.
        void testEnergyConversion();
        /// @brief    Tests for precent conversion factors.
        void testPercentConversion();
        /// @brief    Tests for temperature conversion factors.
        void testTemperatureConversion();
        /// @brief    Tests for power conversion factors.
        void testPowerConversion();
        /// @brief    Comparison against TS_conversions values.
        void testCompareOld();
        /// @brief    Comparison against Trick values.
        void testCompareTrick();
        /// @brief    Comparison against previous values.
        void testRegression();
        /// @brief    Tests the convert method.
        void testConvert();
        /// @brief    Prints conversion factors and constant values.
        void printValues();
    private:
        CPPUNIT_TEST_SUITE(UtUnitConversion);
        CPPUNIT_TEST(testMiscConversion);
        CPPUNIT_TEST(testAngleConversion);
        CPPUNIT_TEST(testLengthConversion);
        CPPUNIT_TEST(testMassConversion);
        CPPUNIT_TEST(testPressureConversion);
        CPPUNIT_TEST(testEnergyConversion);
        CPPUNIT_TEST(testPercentConversion);
        CPPUNIT_TEST(testTemperatureConversion);
        CPPUNIT_TEST(testPowerConversion);
        CPPUNIT_TEST(testCompareOld);
        CPPUNIT_TEST(testCompareTrick);
        CPPUNIT_TEST(testRegression);
        CPPUNIT_TEST(testConvert);
//        CPPUNIT_TEST(printValues);
        CPPUNIT_TEST_SUITE_END();
        static int TEST_ID;               /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtUnitConversion(const UtUnitConversion&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtUnitConversion& operator =(const UtUnitConversion&);
};

/// @}

#endif
