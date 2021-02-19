#ifndef UtGunnsFluidShadow_EXISTS
#define UtGunnsFluidShadow_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_SHADOW    Gunns Fluid Shadow Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Shadow
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsFluidShadow.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidShadow and befriend UtGunnsFluidShadow.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidShadow : public GunnsFluidShadow
{
    public:
        FriendlyGunnsFluidShadow();
        virtual ~FriendlyGunnsFluidShadow();
        friend class UtGunnsFluidShadow;
};
inline FriendlyGunnsFluidShadow::FriendlyGunnsFluidShadow()
    : GunnsFluidShadow() {};
inline FriendlyGunnsFluidShadow::~FriendlyGunnsFluidShadow() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Shadow unit tests.
////
/// @details  This class provides the unit tests for the GunnsFluidShadow class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidShadow: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidShadow(const UtGunnsFluidShadow& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidShadow& operator =(const UtGunnsFluidShadow& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidShadow);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testShadowModing);
        CPPUNIT_TEST(testProcessInputs);
        CPPUNIT_TEST(testProcessOutputs);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsFluidShadowConfigData*         tConfigData;

        /// --     Pointer to nominal input data
        GunnsFluidShadowInputData*          tInputData;

        /// --     Test Article
        FriendlyGunnsFluidShadow*           tArticle;

        /// --     Potential Name
        std::string                         tLinkName;

        /// --     Link Max Conductivity
        double                              tMaxConductivity;

        /// --     Link Expansion Scale Factor
        double                              tExpansionScaleFactor;

        /// --     Link Target for Activating Shadowing
        int                                 tControlTarget;

        /// --     Link Input Mixture Type
        GunnsFluidUtils::MixtureTypes       tInputMixtureType;

        /// --     Link Output Mixture Type
        GunnsFluidUtils::MixtureTypes       tOutputMixtureType;

        /// --     Link Blockage
        double                              tBlockage;

        /// --     Link Source Pressure
        double                              tSourcePressure;

        /// --     Fluid type to convert extra external constituents to
        FluidProperties::FluidType          tConvertToType;

        /// --     Network Nodes
        GunnsFluidNode                      tNodes[3];

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

        /// --     Predefined fluid properties
        DefinedFluidProperties*             tFluidProperties;
        /// --     Local fluid config data
        PolyFluidConfigData*                tLocalConfig;
        /// --     External fluid config data
        PolyFluidConfigData*                tExternalConfig;
        /// --     Fluid 1 input data
        PolyFluidInputData*                 tFluidInput1;
        /// --     Fluid 2 input data
        PolyFluidInputData*                 tFluidInput2;

    public:
        UtGunnsFluidShadow();
        virtual ~UtGunnsFluidShadow();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testShadowModing();
        void testProcessInputs();
        void testProcessOutputs();
        void testAccessors();
};

///@}

#endif
