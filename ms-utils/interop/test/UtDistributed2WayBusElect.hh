#ifndef UtDistributed2WayBusElect_EXISTS
#define UtDistributed2WayBusElect_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILS_INTEROP_DISTR_2WAY_BUS_ELECTRICAL    Electrical Distributed 2-Way Bus Interface Unit Test
/// @ingroup  UT_UTILS_INTEROP
///
/// @copyright Copyright 2025 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Electrical Distributed 2-Way Bus Interface
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "interop/Distributed2WayBusElect.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from Distributed2WayBusElect and befriend UtDistributed2WayBusElect.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyDistributed2WayBusElect : public Distributed2WayBusElect
{
    public:
        FriendlyDistributed2WayBusElect() {;}
        virtual ~FriendlyDistributed2WayBusElect() {;}
        friend class UtDistributed2WayBusElect;
        friend class UtGunnsElectDistributedIf;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Distributed 2-Way Bus Interface unit tests.
////
/// @details  This class provides the unit tests for the Distributed2WayBusElect within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtDistributed2WayBusElect : public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtDistributed2WayBusElect();
        /// @brief  Nominal destructs
        virtual ~UtDistributed2WayBusElect();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Tests construction of the Distributed2WayBusElectNotification class.
        void testNotificationConstruction();
        /// @brief  Tests default construction.
        void testConstruction();
        /// @brief  Tests the initialization method with nominal values.
        void testNominalInitialization();
        /// @brief  Tests the initialization method with errors.
        void testInitializationErrors();
        /// @brief  Tests the updateFrameCounts method.
        void testUpdateFrameCounts();
        /// @brief  Tests the update method.
        void testUpdate();
        /// @brief  Tests the update method with forced roles.
        void testUpdateForcedRole();
        /// @brief  Tests the accessor methods.
        void testAccessors();

    private:
        /// @brief  Sets up the suite of tests for the Distributed2WayBusElect unit testing.
        CPPUNIT_TEST_SUITE(UtDistributed2WayBusElect);
        CPPUNIT_TEST(testNotificationConstruction);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testUpdateFrameCounts);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testUpdateForcedRole);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();
        /// @brief  Enumeration for the number of nodes.
        FriendlyDistributed2WayBusElect* tArticle; /**< (--) Pointer to article under test. */
        static int                       TEST_ID;  /**< (--) Test identification number. */
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtDistributed2WayBusElect(const UtDistributed2WayBusElect& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        UtDistributed2WayBusElect& operator =(const UtDistributed2WayBusElect& that);
};

///@}

#endif
