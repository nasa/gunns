#ifndef UtGunnsFluidNode_EXISTS
#define UtGunnsFluidNode_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_NODE    Gunns Fluid Node Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2021 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Node class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// Must list of all required C Code Model includes.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "core/GunnsFluidNode.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

class UtGunnsFluidNode;

class GunnsFluidNodeUnitTest : public GunnsFluidNode
{
    public:
        friend class UtGunnsFluidNode;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details
/// Class containing model tests.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidNode : public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidNode(const UtGunnsFluidNode& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidNode& operator =(const UtGunnsFluidNode& that);

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Test Suite Name.
        ////////////////////////////////////////////////////////////////////////////////////////////
        CPPUNIT_TEST_SUITE(UtGunnsFluidNode);

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// List all unit test methods here.
        ////////////////////////////////////////////////////////////////////////////////////////////
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testPrepareForStart);
        CPPUNIT_TEST(testComputeThermalCapacitance);
        CPPUNIT_TEST(testLowTempThermalCapacitance);
        CPPUNIT_TEST(testZeroVolumeThermalCapacitance);
        CPPUNIT_TEST(testComputeCompression);
        CPPUNIT_TEST(testInitVolume);
        CPPUNIT_TEST(testSetVolume);
        CPPUNIT_TEST(testUpdateMass);
        CPPUNIT_TEST(testCollectInflux);
        CPPUNIT_TEST(testCollectHeatFlux);
        CPPUNIT_TEST(testCollectTc);
        CPPUNIT_TEST(testResetFlows);
        CPPUNIT_TEST(testFlowsToCapacitiveNode);
        CPPUNIT_TEST(testIntegrateFlowsNoInflow);
        CPPUNIT_TEST(testIntegrateFlowsNoOutflow);
        CPPUNIT_TEST(testOutflowOverflow);
        CPPUNIT_TEST(testInflowOverflow);
        CPPUNIT_TEST(testFlowsToNonCapacitiveNode);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST(testPressureCorrection);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testValidate);
        CPPUNIT_TEST(testResetContent);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testTraceCompounds);

        CPPUNIT_TEST_SUITE_END();

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Define any data structures required in setUp.
        ////////////////////////////////////////////////////////////////////////////////////////////
        PolyFluidConfigData* tFluidConfig;
        PolyFluidConfigData* tFluid2Config;
        PolyFluidConfigData* tFluid3Config;
        GunnsFluidTraceCompoundsConfigData* tTcConfig;
        double               tFractions[FluidProperties::NO_FLUID];
        PolyFluidInputData*  tFluidInput;

        DefinedFluidProperties mFluidProperties;

        GunnsFluidNodeUnitTest tNode;
        GunnsFluidNodeUnitTest tNode2;
        GunnsFluidNodeUnitTest tNode3;

    public:
        UtGunnsFluidNode();
        virtual ~UtGunnsFluidNode();
        void tearDown();
        void setUp();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testPrepareForStart();
        void testComputeThermalCapacitance();
        void testLowTempThermalCapacitance();
        void testZeroVolumeThermalCapacitance();
        void testComputeCompression();
        void testInitVolume();
        void testSetVolume();
        void testUpdateMass();
        void testCollectInflux();
        void testCollectTc();
        void testCollectHeatFlux();
        void testResetFlows();
        void testFlowsToCapacitiveNode();
        void testIntegrateFlowsNoInflow();
        void testIntegrateFlowsNoOutflow();
        void testOutflowOverflow();
        void testInflowOverflow();
        void testFlowsToNonCapacitiveNode();
        void testAccessMethods();
        void testPressureCorrection();
        void testInitializationExceptions();
        void testValidate();
        void testResetContent();
        void testRestart();
        void testTraceCompounds();
};

///@}

#endif
