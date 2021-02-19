#ifndef UtGunnsDynUtils_EXISTS
#define UtGunnsDynUtils_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_DYNAMICS_UTILS Math Utilities Unit Tests
/// @ingroup  UT_TSM_GUNNS_DYNAMICS
///
/// @copyright Copyright 2020 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Dynamics math utilities class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Euler Angles unit tests.
///
/// @details  This class provides the unit tests for the GUNNS math utilities class within the
///           CPPUnit framework.  Most function of GynnsDynUtils are already covered by unit tests
///           for the other dynamics classes.  This test covers the remaining functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsDynUtils: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this unit test.
        UtGunnsDynUtils();
        /// @brief    Default destructs this unit test.
        virtual ~UtGunnsDynUtils();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests the dotV method.
        void testDotV();
        /// @brief    Tests the MtoQ method.
        void testMtoQ();
        /// @brief    Tests the normalizeQ method.
        void testNormalizeQ();
        /// @brief    Tests the normalizeV method.
        void testNormalizeV();
        /// @brief    Tests the normalizeVSuccess method.
        void testNormalizeVSuccess();
        /// @brief    Tests the setM method.
        void testSetM();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsDynUtils);
        CPPUNIT_TEST(testDotV);
        CPPUNIT_TEST(testMtoQ);
        CPPUNIT_TEST(testNormalizeQ);
        CPPUNIT_TEST(testNormalizeV);
        CPPUNIT_TEST(testNormalizeVSuccess);
        CPPUNIT_TEST(testSetM);
        CPPUNIT_TEST_SUITE_END();
        static int           TEST_ID;         /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsDynUtils(const UtGunnsDynUtils&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsDynUtils& operator =(const UtGunnsDynUtils&);
};

///@}

#endif
