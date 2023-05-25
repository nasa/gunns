#ifndef UtGunnsFluidDistributed2WayBus_EXISTS
#define UtGunnsFluidDistributed2WayBus_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_DISTRIBUTED_2WAY_BUS    GUNNS Fluid Distributed 2-Way Bus Interface Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2023 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Distributed 2-Way Bus Interface
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsFluidDistributed2WayBus.hh"
#include "core/test/UtGunnsFluidCapacitor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidDistributed2WayBus and befriend UtGunnsFluidDistributed2WayBus.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidDistributed2WayBus : public GunnsFluidDistributed2WayBus
{
    public:
        FriendlyGunnsFluidDistributed2WayBus() {;}
        virtual ~FriendlyGunnsFluidDistributed2WayBus() {;}
        friend class UtGunnsFluidDistributed2WayBus;
        friend class UtGunnsFluidDistributedIf;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidDistributed2WayBusData and befriend UtGunnsFluidDistributed2WayBus.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidDistributed2WayBusData : public GunnsFluidDistributed2WayBusInterfaceData
{
    public:
        FriendlyGunnsFluidDistributed2WayBusData() {;}
        virtual ~FriendlyGunnsFluidDistributed2WayBusData() {;}
        friend class UtGunnsFluidDistributed2WayBus;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Distributed 2-Way Bus Interface unit tests.
////
/// @details  This class provides the unit tests for the GunnsFluidDistributed2WayBus class within
///           the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidDistributed2WayBus: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidDistributed2WayBus(const UtGunnsFluidDistributed2WayBus& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidDistributed2WayBus& operator =(const UtGunnsFluidDistributed2WayBus& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidDistributed2WayBus);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testProcessInputs);
        CPPUNIT_TEST(testProcessOutputs);
        CPPUNIT_TEST(testDemandLimit);
        CPPUNIT_TEST(testForceModes);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsFluidDistributed2WayBus*  tArticle;      /**< (--) Article under test */
        bool                                   tIsPairMaster; /**< (--) Nominal initialization data */
        unsigned int                           tNumBulk;      /**< (--) Nominal initialization data */
        unsigned int                           tNumTc;        /**< (--) Nominal initialization data */
        unsigned int                           tNumIfBulk;    /**< (--) Nominal initialization data */
        unsigned int                           tNumIfTc;      /**< (--) Nominal initialization data */
        GunnsFluidDistributed2WayBusFluidState tFluidState;   /**< (--) Test fluid state data */
        GunnsFluidDistributed2WayBusFlowState  tFlowState;    /**< (--) Test flow state data */

    public:
        UtGunnsFluidDistributed2WayBus();
        virtual ~UtGunnsFluidDistributed2WayBus();
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
