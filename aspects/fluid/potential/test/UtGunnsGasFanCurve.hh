#ifndef UtGunnsGasFanCurve_EXISTS
#define UtGunnsGasFanCurve_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_POTENTIAL_GAS_FAN_CURVE Gas Fan Curve Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_POTENTIAL
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Gas Fan Curve utility class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/potential/GunnsGasFanCurve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsGasFanCurve and befriend UtGunnsGasFanCurve.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsGasFanCurve : public GunnsGasFanCurve
{
    public:
        FriendlyGunnsGasFanCurve();
        virtual ~FriendlyGunnsGasFanCurve();
        friend class UtGunnsGasFanCurve;
    private:
        FriendlyGunnsGasFanCurve(const FriendlyGunnsGasFanCurve&);
        FriendlyGunnsGasFanCurve& operator =(const FriendlyGunnsGasFanCurve&);
};
inline FriendlyGunnsGasFanCurve::FriendlyGunnsGasFanCurve() : GunnsGasFanCurve() {}
inline FriendlyGunnsGasFanCurve::~FriendlyGunnsGasFanCurve() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Gas Fan Curve unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Gas Fan Curve utility class
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsGasFanCurve : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Gas Fan Curve unit test.
        UtGunnsGasFanCurve();
        /// @brief    Default destructs this Gas Fan Curve unit test.
        virtual ~UtGunnsGasFanCurve();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests the setCoeffs setter method.
        void testSetCoeffs();
        /// @brief    Tests the evaluate method.
        void testEvaluate();
        /// @brief    Tests the improveRoot method.
        void testImproveRoot();
        /// @brief    Tests exceptions thrown by the improveRoot method.
        void testImproveRootExceptions();
        /// @brief    Tests accessor methods.
        void testAccessors();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsGasFanCurve);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testSetCoeffs);
        CPPUNIT_TEST(testEvaluate);
        CPPUNIT_TEST(testImproveRoot);
        CPPUNIT_TEST(testImproveRootExceptions);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsGasFanCurve* tArticle;  /**< (--) Article under test */
        static int                TEST_ID;   /**< (--) Test identification number. */

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsGasFanCurve(const UtGunnsGasFanCurve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsGasFanCurve& operator =(const UtGunnsGasFanCurve&);
};

///@}

#endif
