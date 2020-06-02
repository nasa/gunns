#ifndef UtGunnsDynEuler_EXISTS
#define UtGunnsDynEuler_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_DYNAMICS_EULER Euler Angles Unit Tests
/// @ingroup  UT_TSM_GUNNS_DYNAMICS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Dynamics Euler Angles classes.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/dynamics/GunnsDynEuler123.hh"
#include "aspects/dynamics/GunnsDynEulerFactory.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Euler Angles unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Euler Angles classes within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsDynEuler: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this unit test.
        UtGunnsDynEuler();
        /// @brief    Default destructs this unit test.
        virtual ~UtGunnsDynEuler();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests the factory given an invalid Euler sequence.
        void testInvalidSequence();
        /// @brief    Tests singularity handling in the update method.
        void testSingularity();
        /// @brief    Tests update method.
        void testUpdate();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsDynEuler);
        CPPUNIT_TEST(testInvalidSequence);
        CPPUNIT_TEST(testSingularity);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST_SUITE_END();
        double               tEulerAngles[3]; /**< (rad) Euler angles. */
        GunnsDynEulerFactory tFactory;        /**< (--)  Euler angle factory. */
        static int           TEST_ID;         /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsDynEuler(const UtGunnsDynEuler&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsDynEuler& operator =(const UtGunnsDynEuler&);
};

///@}

#endif
