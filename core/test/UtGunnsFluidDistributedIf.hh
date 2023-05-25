#ifndef UtGunnsFluidDistributedIf_EXISTS
#define UtGunnsFluidDistributedIf_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_DISTRIBUTED_IF    GUNNS Fluid Distributed Interface Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2023 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Distributed Interface
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsFluidDistributedIf.hh"
#include "core/test/UtGunnsFluidCapacitor.hh"
#include "core/test/UtGunnsFluidDistributed2WayBus.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidDistributedIf and befriend UtGunnsFluidDistributedIf.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidDistributedIf : public GunnsFluidDistributedIf
{
    public:
        FriendlyGunnsFluidDistributedIf();
        virtual ~FriendlyGunnsFluidDistributedIf();
        friend class UtGunnsFluidDistributedIf;
};
inline FriendlyGunnsFluidDistributedIf::FriendlyGunnsFluidDistributedIf()
    : GunnsFluidDistributedIf() {};
inline FriendlyGunnsFluidDistributedIf::~FriendlyGunnsFluidDistributedIf() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidDistributed2WayBus and befriend UtGunnsFluidDistributedIf.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
//class FriendlyGunnsFluidDistributed2WayBus : public GunnsFluidDistributed2WayBus
//{
//    public:
//        FriendlyGunnsFluidDistributed2WayBus();
//        virtual ~FriendlyGunnsFluidDistributed2WayBus();
//        friend class UtGunnsFluidDistributedIf;
//};
//inline FriendlyGunnsFluidDistributed2WayBus::FriendlyGunnsFluidDistributed2WayBus()
//    : GunnsFluidDistributed2WayBus() {};
//inline FriendlyGunnsFluidDistributed2WayBus::~FriendlyGunnsFluidDistributed2WayBus() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidDistributedIfData and befriend UtGunnsFluidDistributedIf.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidDistributedIfData : public GunnsFluidDistributed2WayBusInterfaceData
{
    public:
        FriendlyGunnsFluidDistributedIfData();
        virtual ~FriendlyGunnsFluidDistributedIfData();
        friend class UtGunnsFluidDistributedIf;
};
inline FriendlyGunnsFluidDistributedIfData::FriendlyGunnsFluidDistributedIfData()
    : GunnsFluidDistributed2WayBusInterfaceData() {};
inline FriendlyGunnsFluidDistributedIfData::~FriendlyGunnsFluidDistributedIfData() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Distributed Interface unit tests.
////
/// @details  This class provides the unit tests for the GunnsFluidDistributedIf class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidDistributedIf: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidDistributedIf(const UtGunnsFluidDistributedIf& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidDistributedIf& operator =(const UtGunnsFluidDistributedIf& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidDistributedIf);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testProcessInputs);
        CPPUNIT_TEST(testProcessOutputs);
        CPPUNIT_TEST(testOutputCapacitance);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testTransportFlows);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testData);
        CPPUNIT_TEST(testForceModes);
        CPPUNIT_TEST_SUITE_END();

        GunnsFluidDistributedIfConfigData*    tConfigData;            /**< (--)   Nominal config data */
        GunnsFluidDistributedIfInputData*     tInputData;             /**< (--)   Nominal input data */
        FriendlyGunnsFluidDistributedIf*      tArticle;               /**< (--)   Article under test */
        FriendlyGunnsFluidDistributed2WayBus* tArticleInterface;      /**< (--)   Interface logic article under test */
        std::string                           tLinkName;              /**< (--)   Nominal config data */
        bool                                  tIsPairMaster;          /**< (--)   Nominal config data */
        bool                                  tUseEnthalpy;           /**< (--)   Nominal config data */
        bool                                  tDemandOption;          /**< (--)   Nominal config data */
        bool                                  tMalfBlockageFlag;      /**< (--)   Nominal input data */
        double                                tMalfBlockageValue;     /**< (--)   Nominal input data */
        GunnsFluidNode                        tNodes[2];              /**< (--)   Test nodes */
        GunnsNodeList                         tNodeList;              /**< (--)   Test node list */
        std::vector<GunnsBasicLink*>          tLinks;                 /**< (--)   Test links vector */
        int                                   tPort0;                 /**< (--)   Nominal init data */
        double                                tTimeStep;              /**< (s)    Test time step */
        DefinedFluidProperties*               tFluidProperties;       /**< (--)   Test fluid properties */
        DefinedChemicalCompounds*             tTcProperties;          /**< (--)   Pointer to predefined chemical compounds properties. */
        ChemicalCompound::Type                tTcTypes[4];            /**< (--)   Array of trace compound types. */
        GunnsFluidTraceCompoundsConfigData*   tFluidTcConfig;         /**< (--)   Pointer to fluid trace compounds config data. */
        PolyFluidConfigData*                  tLocalConfig;           /**< (--)   Test local fluid config */
        PolyFluidInputData*                   tFluidInput1;           /**< (--)   Test fluid input */
        FriendlyGunnsFluidCapacitor           tCapacitorLink;         /**< (--)   Capacitor link */

    public:
        UtGunnsFluidDistributedIf();
        virtual ~UtGunnsFluidDistributedIf();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testProcessInputs();
        void testProcessOutputs();
        void testOutputCapacitance();
        void testStep();
        void testComputeFlows();
        void testTransportFlows();
        void testRestart();
        void testData();
        void testForceModes();
};

///@}

#endif
