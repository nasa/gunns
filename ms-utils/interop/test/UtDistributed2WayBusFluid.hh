#ifndef UtDistributed2WayBusFluid_EXISTS
#define UtDistributed2WayBusFluid_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILS_INTEROP_FLUID_DISTRIBUTED_2WAY_BUS    Fluid Distributed 2-Way Bus Interface Unit Test
/// @ingroup  UT_UTILS_INTEROP
///
/// @copyright Copyright 2025 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Fluid Distributed 2-Way Bus Interface
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "interop/Distributed2WayBusFluid.hh"
#include "core/test/UtGunnsFluidCapacitor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from Distributed2WayBusFluid and befriend UtDistributed2WayBusFluid.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyDistributed2WayBusFluid : public Distributed2WayBusFluid
{
    public:
        FriendlyDistributed2WayBusFluid() {;}
        virtual ~FriendlyDistributed2WayBusFluid() {;}
        friend class UtDistributed2WayBusFluid;
        friend class UtGunnsFluidDistributedIf;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from Distributed2WayBusFluidData and befriend UtDistributed2WayBusFluid.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyDistributed2WayBusFluidData : public Distributed2WayBusFluidInterfaceData
{
    public:
        FriendlyDistributed2WayBusFluidData() {;}
        virtual ~FriendlyDistributed2WayBusFluidData() {;}
        friend class UtDistributed2WayBusFluid;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Distributed 2-Way Bus Interface unit tests.
////
/// @details  This class provides the unit tests for the Distributed2WayBusFluid class within
///           the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtDistributed2WayBusFluid: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtDistributed2WayBusFluid(const UtDistributed2WayBusFluid& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtDistributed2WayBusFluid& operator =(const UtDistributed2WayBusFluid& that);

        CPPUNIT_TEST_SUITE(UtDistributed2WayBusFluid);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testProcessInputs);
        CPPUNIT_TEST(testProcessOutputs);
        CPPUNIT_TEST(testDemandLimit);
        CPPUNIT_TEST(testForceModes);
        CPPUNIT_TEST_SUITE_END();

        FriendlyDistributed2WayBusFluid*  tArticle;      /**< (--) Article under test */
        bool                              tIsPairMaster; /**< (--) Nominal initialization data */
        unsigned int                      tNumBulk;      /**< (--) Nominal initialization data */
        unsigned int                      tNumTc;        /**< (--) Nominal initialization data */
        unsigned int                      tNumIfBulk;    /**< (--) Nominal initialization data */
        unsigned int                      tNumIfTc;      /**< (--) Nominal initialization data */
        Distributed2WayBusFluidFluidState tFluidState;   /**< (--) Test fluid state data */
        Distributed2WayBusFluidFlowState  tFlowState;    /**< (--) Test flow state data */
        static int                        TEST_ID;       /**< (--) Test identification number. */

    public:
        UtDistributed2WayBusFluid();
        virtual ~UtDistributed2WayBusFluid();
        void tearDown();
        void setUp();
        void testDefaultConstruction();
        void testInitialization();
        void testProcessInputs();
        void testProcessOutputs();
        void testDemandLimit();
        void testForceModes();
};

///@}

#endif
