#ifndef UtGunnsFluidHeater_EXISTS
#define UtGunnsFluidHeater_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_Heater   Heater Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Heater link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsFluidHeater.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidHeater and befriend UtGunnsFluidHeater.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidHeater : public GunnsFluidHeater
{
    public:
        FriendlyGunnsFluidHeater();
        virtual ~FriendlyGunnsFluidHeater();
        friend class UtGunnsFluidHeater;
};
inline FriendlyGunnsFluidHeater::FriendlyGunnsFluidHeater() : GunnsFluidHeater() {};
inline FriendlyGunnsFluidHeater::~FriendlyGunnsFluidHeater() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidNode and befriend UtGunnsFluidHeater.
///
/// @details  Create a friendly fluid node class that we can use to get access to the node's
///           protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidNode : public GunnsFluidNode
{
    public:
        FriendlyGunnsFluidNode();
        virtual ~FriendlyGunnsFluidNode();
        friend class UtGunnsFluidHeater;
};
inline FriendlyGunnsFluidNode::FriendlyGunnsFluidNode() : GunnsFluidNode() {};
inline FriendlyGunnsFluidNode::~FriendlyGunnsFluidNode() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Heater unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Heater link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidHeater: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Heater unit test.
        UtGunnsFluidHeater();
        /// @brief    Default constructs this Heater unit test.
        virtual ~UtGunnsFluidHeater();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config and input data.
        void testConfigAndInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests initialization exceptions.
        void testInitializationExceptions();
        /// @brief    Tests step method.
        void testStep();
        /// @brief    Tests computeFlows method.
        void testComputeFlows();
        /// @brief    Tests access methods
        void testAccessMethods();
        /// @brief    Tests modifier methods
        void testModifierMethods();
        /// @brief    Tests restart method.
        void testRestart();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidHeater);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST(testModifierMethods);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();
        /// @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 1};
        /// @brief    --     Constituent fluid types array.
        FluidProperties::FluidType           tTypes[N_FLUIDS];
        /// @brief    --     Constituent fluid mass fractions array.
        double                               tFractions[N_FLUIDS];
        /// @brief    --     Predefined fluid properties.
        DefinedFluidProperties*              tFluidProperties;
        /// @brief    --     Fluid config data.
        PolyFluidConfigData*                 tFluidConfig;
        /// @brief    --     Fluid input data for nodes.
        PolyFluidInputData*                  tFluidInput;
        /// @brief    --     Link vector.
        std::vector<GunnsBasicLink*>         tLinks;
        /// @brief    --     Nominal name.
        std::string                          tName;
        /// @brief    --     Nominal connected nodes.
        FriendlyGunnsFluidNode               tNodes[N_NODES];
        /// @brief    --     Network node structure.
        GunnsNodeList                        tNodeList;
        /// @brief    --     Nominal inlet port index.
        int                                  tPort0;
        /// @brief    --     Nominal outlet port index.
        int                                  tPort1;
        /// @brief    (--)   Heater efficiency.
        double                               tHeaterEfficiency;
        /// @brief    --     Pointer to nominal configuration data.
        GunnsFluidHeaterConfigData*          tConfigData;
        /// @brief    --     Blockage malfunction flag.
        bool                                 tMalfBlockageFlag;
        /// @brief    --     Blockage malfunction value.
        double                               tMalfBlockageValue;
        /// @brief    (--)   Heater power consumed.
        double                               tHeaterPower;
        /// @brief    --     Pointer to nominal input data.
        GunnsFluidHeaterInputData*           tInputData;
        /// @brief    --     Pointer to the friendly Heater under test.
        FriendlyGunnsFluidHeater*            tArticle;
        /// @brief    (s)    Nominal time step.
        double                               tTimeStep;
        static int                           TEST_ID; /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidHeater(const UtGunnsFluidHeater&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidHeater& operator =(const UtGunnsFluidHeater&);
};

///@}

#endif
