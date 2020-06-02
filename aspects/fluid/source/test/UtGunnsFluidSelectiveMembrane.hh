#ifndef UtGunnsFluidSelectiveMembrane_EXISTS
#define UtGunnsFluidSelectiveMembrane_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_SELECTIVE_MEMBRANE    Gunns Fluid Selective Membrane Unit Test
/// @ingroup  UT_GUNNS_FLUID
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Selective Membrane
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/source/GunnsFluidSelectiveMembrane.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSelectiveMembrane and befriend UtGunnsFluidSelectiveMembrane.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSelectiveMembrane : public GunnsFluidSelectiveMembrane
{
    public:
        FriendlyGunnsFluidSelectiveMembrane();
        virtual ~FriendlyGunnsFluidSelectiveMembrane();
        friend class UtGunnsFluidSelectiveMembrane;
};
inline FriendlyGunnsFluidSelectiveMembrane::FriendlyGunnsFluidSelectiveMembrane() : GunnsFluidSelectiveMembrane() {};
inline FriendlyGunnsFluidSelectiveMembrane::~FriendlyGunnsFluidSelectiveMembrane() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns fluid membrane evaporator link unit tests.
///
/// @details  This class provides unit tests within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSelectiveMembrane: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidSelectiveMembrane(const UtGunnsFluidSelectiveMembrane& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidSelectiveMembrane& operator =(const UtGunnsFluidSelectiveMembrane& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidSelectiveMembrane);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testStepGas);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testComputeFlowsGas);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST(testPortMapping);
        CPPUNIT_TEST_SUITE_END();

        std::string                            tLinkName;          /**< (--)       Nominal config data */
        double                                 tMaxConductance;    /**< (m2)       Nominal config data */
        double                                 tAbsorptionCoeff;   /**< (kg/s/kPa) Nominal config data */
        FluidProperties::FluidType             tExternalType;      /**< (--)       Nominal config data */
        FluidProperties::FluidType             tInternalType;      /**< (--)       Nominal config data */
        GunnsFluidSelectiveMembraneConfigData* tConfigData;        /**< (--)       Nominal config data */
        bool                                   tMalfBlockageFlag;  /**< (--)       Nominal input data */
        double                                 tMalfBlockageValue; /**< (--)       Nominal input data */
        GunnsFluidSelectiveMembraneInputData*  tInputData;         /**< (--)       Nominal input data */
        FriendlyGunnsFluidSelectiveMembrane*   tArticle;           /**< (--)       Article under test */
        GunnsFluidNode                         tNodes[6];          /**< (--)       Network nodes */
        GunnsNodeList                          tNodeList;          /**< (--)       Network node list */
        std::vector<GunnsBasicLink*>           tLinks;             /**< (--)       Network links vector */
        int                                    tPort0;             /**< (--)       Nominal init data */
        int                                    tPort1;             /**< (--)       Nominal init data */
        int                                    tPort2;             /**< (--)       Nominal init data */
        double                                 tTimeStep;          /**< (s)        Nominal time step */
        DefinedFluidProperties*                tFluidProperties;   /**< (--)       Nominal config data */
        PolyFluidConfigData*                   tFluidConfig;       /**< (--)       Nominal config data */
        PolyFluidInputData*                    tFluidInput1;       /**< (--)       Nominal input data */
        PolyFluidInputData*                    tFluidInput2;       /**< (--)       Nominal input data */
        double*                                fractions;          /**< (--)       Nominal input data */

    public:
        UtGunnsFluidSelectiveMembrane();
        virtual ~UtGunnsFluidSelectiveMembrane();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testRestart();
        void testStep();
        void testStepGas();
        void testComputeFlows();
        void testComputeFlowsGas();
        void testAccessMethods();
        void testPortMapping();
};

///@}

#endif
