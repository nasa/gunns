#ifndef UtFluidProperties_EXISTS
#define UtFluidProperties_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_PROPERTIES_FLUID     Fluid Properties Unit Tests
/// @ingroup  UT_TSM_PROPERTIES
///
/// @copyright Copyright 2022 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Fluid Properties model classes.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "properties/FluidProperties.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from DefinedFluidProperties and befriend UtFluidProperties.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyDefinedFluidProperties : public DefinedFluidProperties
{
    public:
        FriendlyDefinedFluidProperties();
        virtual ~FriendlyDefinedFluidProperties();
        friend class UtFluidProperties;
};
inline FriendlyDefinedFluidProperties::FriendlyDefinedFluidProperties() {};
inline FriendlyDefinedFluidProperties::~FriendlyDefinedFluidProperties() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Predefined Fluid Properties unit tests.
////
/// @details
/// This class provides the unit tests for the Predefined Fluid Properties within the CPPUnit
/// framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtFluidProperties: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Fluid Properties unit test.
        UtFluidProperties();
        /// @brief    Default destructs this Fluid Properties unit test.
        virtual ~UtFluidProperties();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        void testConstruction();
        void testAccessors();
        void testDensityPressureConsistency();
        void testSpecificEnthalpyTemperatureConsistency();
        void testForPositive();
        void testLowP();
        void testInvalidRangeBounds();
        void testHeTable();
        void testXeTable();
        void testN2Table();
        void testO2Table();
        void testCH4Table();
        void testH2Table();
        void testWaterPvtTable();
        void testSaturationCurveConsistency();
    private:
        CPPUNIT_TEST_SUITE(UtFluidProperties);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testDensityPressureConsistency);
        CPPUNIT_TEST(testSpecificEnthalpyTemperatureConsistency);
        CPPUNIT_TEST(testForPositive);
        CPPUNIT_TEST(testLowP);
        CPPUNIT_TEST(testInvalidRangeBounds);
        CPPUNIT_TEST(testHeTable);
        CPPUNIT_TEST(testXeTable);
        CPPUNIT_TEST(testN2Table);
        CPPUNIT_TEST(testO2Table);
        CPPUNIT_TEST(testCH4Table);
        CPPUNIT_TEST(testH2Table);
        CPPUNIT_TEST(testWaterPvtTable);
        CPPUNIT_TEST(testSaturationCurveConsistency);
        CPPUNIT_TEST_SUITE_END();
        /// --  Pointer to the friendly test article
        FriendlyDefinedFluidProperties* mArticle;
        /// --  Tolerance for comparison of expected and returned values
        double                          mTolerance;
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtFluidProperties(const UtFluidProperties&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtFluidProperties& operator =(const UtFluidProperties&);
};

/// @}

#endif
