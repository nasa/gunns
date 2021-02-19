#ifndef UtGunnsFluidEvaporation_EXISTS
#define UtGunnsFluidEvaporation_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_EVAPORATION    Gunns Fluid Evaporation Unit Test
/// @ingroup  UT_GUNNS_FLUID
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Evaporation
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/source/GunnsFluidEvaporation.hh"
#include "aspects/fluid/capacitor/test/UtGunnsFluidAccum.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidEvaporation and befriend UtGunnsFluidEvaporation.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidEvaporation : public GunnsFluidEvaporation
{
    public:
        FriendlyGunnsFluidEvaporation();
        virtual ~FriendlyGunnsFluidEvaporation();
        friend class UtGunnsFluidEvaporation;
};
inline FriendlyGunnsFluidEvaporation::FriendlyGunnsFluidEvaporation() : GunnsFluidEvaporation() {};
inline FriendlyGunnsFluidEvaporation::~FriendlyGunnsFluidEvaporation() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns fluid membrane evaporator link unit tests.
///
/// @details  This class provides unit tests within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidEvaporation: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidEvaporation(const UtGunnsFluidEvaporation& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidEvaporation& operator =(const UtGunnsFluidEvaporation& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidEvaporation);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testNominalInitNoGasNode);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStepGas);
        CPPUNIT_TEST(testStepLiquid);
        CPPUNIT_TEST(testStepBoth);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST(testPortMapping);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsFluidAccum          tAccum;             /**< (--)      Accumulator link */
        std::string                      tLinkName;          /**< (--)      Nominal config data */
        FluidProperties::FluidType       tGasType;           /**< (--)      Nominal config data */
        double                           tEvaporationCoeff;  /**< (1/s/kPa) Nominal config data */
        double                           tPoolMassExponent;  /**< (--)      Nominal config data */
        GunnsFluidAccum*                 tLiquidPoolAccum;   /**< (--)      Nominal config data */
        GunnsFluidEvaporationConfigData* tConfigData;        /**< (--)      Nominal config data */
        bool                             tMalfBlockageFlag;  /**< (--)      Nominal input data */
        double                           tMalfBlockageValue; /**< (--)      Nominal input data */
        double                           tGasTotalPressure;  /**< (--)      Nominal input data */
        double                           tVaporPotential;    /**< (--)      Nominal input data */
        double                           tEvaporationRate;   /**< (--)      Nominal input data */
        GunnsFluidEvaporationInputData*  tInputData;         /**< (--)      Nominal input data */
        FriendlyGunnsFluidEvaporation*   tArticle;           /**< (--)      Article under test */
        GunnsFluidNode                   tNodes[3];          /**< (--)      Network nodes */
        GunnsNodeList                    tNodeList;          /**< (--)      Network node list */
        std::vector<GunnsBasicLink*>     tLinks;             /**< (--)      Network links vector */
        int                              tPort0;             /**< (--)      Nominal init data */
        int                              tPort1;             /**< (--)      Nominal init data */
        double                           tTimeStep;          /**< (s)       Nominal time step */
        DefinedFluidProperties*          tFluidProperties;   /**< (--)      Nominal config data */
        PolyFluidConfigData*             tFluidConfig;       /**< (--)      Nominal config data */
        PolyFluidInputData*              tFluidInput1;       /**< (--)      Nominal input data */
        PolyFluidInputData*              tFluidInput2;       /**< (--)      Nominal input data */
        double*                          tFractions;         /**< (--)      Nominal input data */

    public:
        UtGunnsFluidEvaporation();
        virtual ~UtGunnsFluidEvaporation();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testNominalInitNoGasNode();
        void testInitializationExceptions();
        void testRestart();
        void testStepGas();
        void testStepLiquid();
        void testStepBoth();
        void testComputeFlows();
        void testAccessMethods();
        void testPortMapping();
};

///@}

#endif
