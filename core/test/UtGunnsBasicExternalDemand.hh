#ifndef UtGunnsBasicExternalDemand_EXISTS
#define UtGunnsBasicExternalDemand_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_BASIC_EXTERNAL_DEMAND    Gunns Basic External Demand Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Basic External Demand
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsBasicExternalDemand.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicExternalDemand and befriend UtGunnsBasicExternalDemand.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicExternalDemand : public GunnsBasicExternalDemand
{
    public:
        FriendlyGunnsBasicExternalDemand();
        virtual ~FriendlyGunnsBasicExternalDemand();
        friend class UtGunnsBasicExternalDemand;
};
inline FriendlyGunnsBasicExternalDemand::FriendlyGunnsBasicExternalDemand()
    : GunnsBasicExternalDemand() {}
inline FriendlyGunnsBasicExternalDemand::~FriendlyGunnsBasicExternalDemand() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsBasicExternalDemand unit tests.
////
/// @details  This class provides the unit tests for the GunnsBasicExternalDemand class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicExternalDemand : public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsBasicExternalDemand(const UtGunnsBasicExternalDemand& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsBasicExternalDemand& operator =(const UtGunnsBasicExternalDemand& that);

        CPPUNIT_TEST_SUITE(UtGunnsBasicExternalDemand);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testIoMethods);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsBasicExternalDemandConfigData* tConfigData;

        /// --     Pointer to nominal input data
        GunnsBasicExternalDemandInputData*  tInputData;

        /// --     Test Article
        FriendlyGunnsBasicExternalDemand*   tArticle;

        /// --     Link Name
        std::string                         tLinkName;

        /// --     Link Default Conductance
        double                              tInitialConductivity;

        /// --     The default potential rise of the link.
        double                              tInitialPotential;

        /// --     Minimum filtered effective conductivity
        double                              tFilterMinConductivity;

        /// --     Minimum delta-potential for capacitance estimate
        double                              tFilterMinDeltaP;

        /// --     Gain for the estimated capacitance filter
        double                              tFilterCapacitanceGain;

        /// --     Network Nodes
        GunnsBasicNode                      tNodes[3];

        /// --     Node List
        GunnsNodeList                       tNodeList;

        /// --     Network Links
        std::vector<GunnsBasicLink*>        tLinks;

        /// --     Nominal inlet port index
        int                                 tPort0;

        /// --     Nominal outlet port index
        int                                 tPort1;

        /// (s)    Nominal time step
        double                              tTimeStep;

    public:
        UtGunnsBasicExternalDemand();
        virtual ~UtGunnsBasicExternalDemand();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testIoMethods();
        void testStep();
};

///@}

#endif
