#ifndef UtGunnsTripLogic_EXISTS
#define UtGunnsTripLogic_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TRIP_LOGIC    Trip Logic Model Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Trip Logic Model
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/TripLogic/GunnsTripLogic.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsTripGreaterThan and befriend UtGunnsTripLogic.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsTripGreaterThan : public GunnsTripGreaterThan
{
    public:
        FriendlyGunnsTripGreaterThan() : GunnsTripGreaterThan() {;}
        virtual ~FriendlyGunnsTripGreaterThan() {;}
        friend class UtGunnsTripLogic;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsTripGreaterThan and befriend UtGunnsTripLogic.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsTripLessThan : public GunnsTripLessThan
{
    public:
        FriendlyGunnsTripLessThan() : GunnsTripLessThan() {;}
        virtual ~FriendlyGunnsTripLessThan() {;}
        friend class UtGunnsTripLogic;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electric Trip Logic Model unit tests.
////
/// @details  This class provides the unit tests for the GunnsTripLogic within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsTripLogic: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsTripLogic();
        /// @brief  Nominal destructs
        virtual ~UtGunnsTripLogic();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Tests construction methods.
        void testConstruction();
        /// @brief  Tests the initialization method with nominal values.
        void testNominalInitialization();
        /// @brief  Tests the checkForTrip method.
        void testCheckForTrip();
        /// @brief  Tests the isTripped method.
        void testIsTripped();
        /// @brief  Tests the resetTrip method.
        void testResetTrip();
        /// @brief  Tests the accessor methods.
        void testAccessors();

    private:
        /// @brief  Sets up the suite of tests for the GunnsTripLogicric unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsTripLogic);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testCheckForTrip);
        CPPUNIT_TEST(testIsTripped);
        CPPUNIT_TEST(testResetTrip);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();
        float        tLimit;             /**< (--) Initialization data. */
        unsigned int tPriority;          /**< (--) Initialization data. */
        bool         tIsTripped;         /**< (--) Initialization data. */
        static int   TEST_ID;            /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsTripLogic(const UtGunnsTripLogic& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsTripLogic& operator =(const UtGunnsTripLogic& that);
};

///@}

#endif
