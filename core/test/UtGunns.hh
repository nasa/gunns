#ifndef UtGunns_EXISTS
#define UtGunns_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS    Gunns Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2023 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns orchestrator class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
// Must list of all required C Code Model includes.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "core/Gunns.hh"
#include "core/GunnsFluidCapacitor.hh"
#include "core/GunnsFluidConductor.hh"
#include "UtGunnsEpsConstantPowerLoad.hh"
#include "core/GunnsBasicConductor.hh"
#include "core/GunnsBasicPotential.hh"
#include "core/GunnsBasicCapacitor.hh"
#include "core/GunnsBasicSource.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "software/exceptions/TsNumericalException.hh"

class UtGunns;

class GunnsUnitTest : public Gunns
{
    public:
        friend class UtGunns;
};

class GunnsFluidConductorUnitTest : public GunnsFluidConductor
{
    public:
        friend class UtGunns;
};

class GunnsBasicConductorUnitTest : public GunnsBasicConductor
{
    public:
        friend class UtGunns;
};

class GunnsFluidNodeUtGunns : public GunnsFluidNode
{
    public:
        friend class UtGunns;
};

class GunnsBasicNodeUtGunns : public GunnsBasicNode
{
    public:
        friend class UtGunns;
};

class UtGunnsFakeLink : public GunnsBasicLink
{
    public:
        friend class UtGunns;
        UtGunnsFakeLink(int numPorts, bool isNonLinear, bool failOnStep);
        virtual ~UtGunnsFakeLink();
        virtual bool isNonLinear();
        virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                         const int absoluteStep);
        virtual bool resetLastMinorStep(const int convergedStep, const int absoluteStep);
        virtual void minorStep(const double timeStep, const int minorStep);
        virtual void processInputs();
        virtual void processOutputs();

        bool   nonLinearFlag;
        bool   failOnStep;
        int    delayToAbsoluteStep;
        int    delayToConvergedStep;
        int    callsToRead;
        int    callsToWrite;
        int    callsToMinorStep;
        double savedPotential[2];
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details
/// Class containing model tests.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunns : public CppUnit::TestFixture
{
    private:
        /// @brief Sets up & initializes a nominal non-linear network for several test methods.
        void setupNominalNonLinearNetwork(const bool converging);
        /// @brief Sets up & initializes a basic network with islands in it for testing islands.
        void setupIslandNetwork();
        /// @brief Sets up & initializes a basic network with a single small islands in it.
        void setupIslandNetwork2();
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunns(const UtGunns& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunns& operator =(const UtGunns& that);

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Test Suite Name.
        ////////////////////////////////////////////////////////////////////////////////////////////
        CPPUNIT_TEST_SUITE(UtGunns);

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// List all unit test methods here.
        ////////////////////////////////////////////////////////////////////////////////////////////
        CPPUNIT_TEST(testConfigData);
        CPPUNIT_TEST(testConfigDataExceptions);
        CPPUNIT_TEST(testDuplicateLinkException);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testNonLinearInitialization);
        CPPUNIT_TEST(testDuplicateInitException);
        CPPUNIT_TEST(testLinkInitException);
        CPPUNIT_TEST(testNodeInitException);
        CPPUNIT_TEST(testNonInitException);
        CPPUNIT_TEST(testLinearStep);
        CPPUNIT_TEST(testNonLinearStep);
        CPPUNIT_TEST(testLinearStepExceptions);
        CPPUNIT_TEST(testNonConvergence);
        CPPUNIT_TEST(testFluidNetworkStep);
        CPPUNIT_TEST(testSolutionRejection);
        CPPUNIT_TEST(testDummyMode);
        CPPUNIT_TEST(testSlaveMode);
        CPPUNIT_TEST(testSolutionDelay);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST(testMatrixConditioning);
        CPPUNIT_TEST(testDebugPreDecomposition);
        CPPUNIT_TEST(testDebugNode);
        CPPUNIT_TEST(testBuildIslands);
        CPPUNIT_TEST(testSolveIslands);
        CPPUNIT_TEST(testWorstCaseTimingConverging);
        CPPUNIT_TEST(testWorstCaseTimingNonConverging);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testNetworkCapacitance);
        CPPUNIT_TEST(testSolveIslands2);
        CPPUNIT_TEST(testCheckStepInputs);
        CPPUNIT_TEST(testGpuSparse);
        CPPUNIT_TEST(testGpuDense);
        CPPUNIT_TEST(testGpuSparseIslands);
        CPPUNIT_TEST(testGpuDenseIslands);

        CPPUNIT_TEST_SUITE_END();

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Define any data structures required in setUp.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsUnitTest                    tNetwork;
        GunnsConfigData                  tNetworkConfig;

        std::string                      tNetworkName;
        double                           tDeltaTime;
        double                           tConvergenceTolerance;
        double                           tMinLinearizationPotential;
        int                              tMinorStepLimit;
        int                              tDecompositionLimit;

        GunnsBasicNodeUtGunns            tBasicNodes[10];
        GunnsFluidNodeUtGunns            tFluidNodes[10];
        GunnsNodeList                    tNodeList;
        std::vector<GunnsBasicLink*>     tLinks;

        DefinedFluidProperties           mFluidProperties;
        PolyFluidInputData*              tFluid;
        PolyFluidConfigData*             tFluidConfig;
        double                           tFractions[FluidProperties::NO_FLUID];

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Link objects used in the various tests.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidCapacitor              tFluidCapacitor1;
        GunnsFluidCapacitorConfigData    tFluidCapacitor1Config;
        GunnsFluidCapacitorInputData*    tFluidCapacitor1Input;

        GunnsFluidConductorUnitTest      tFluidConductor1;
        GunnsFluidConductorConfigData    tFluidConductor1Config;
        GunnsFluidConductorInputData*    tFluidConductor1Input;

        GunnsFluidConductorUnitTest      tFluidConductor2;
        GunnsFluidConductorConfigData    tFluidConductor2Config;
        GunnsFluidConductorInputData*    tFluidConductor2Input;

        GunnsFluidConductorUnitTest      tFluidConductor3;
        GunnsFluidConductorConfigData    tFluidConductor3Config;
        GunnsFluidConductorInputData*    tFluidConductor3Input;

        EpsConstantPowerLoad           tConstantLoad1;
        EpsConstantPowerLoadConfigData tConstantLoad1Config;

        EpsConstantPowerLoad           tConstantLoad2;
        EpsConstantPowerLoadConfigData tConstantLoad2Config;

        GunnsBasicPotential              tPotential;
        GunnsBasicPotentialConfigData    tPotentialConfig;

        GunnsBasicConductorUnitTest      tConductor1;
        GunnsBasicConductorConfigData    tConductor1Config;

        GunnsBasicConductorUnitTest      tConductor2;
        GunnsBasicConductorConfigData    tConductor2Config;

        GunnsBasicConductorUnitTest      tConductor3;
        GunnsBasicConductorConfigData    tConductor3Config;

        GunnsBasicConductorUnitTest      tConductor4;
        GunnsBasicConductorConfigData    tConductor4Config;

        GunnsBasicCapacitor              tCapacitor;
        GunnsBasicCapacitorConfigData    tCapacitorConfig;

        GunnsBasicSource                 tSource;
        GunnsBasicSourceConfigData       tSourceConfig;

        UtGunnsFakeLink                  tFake1;

    public:
        UtGunns();
        virtual ~UtGunns();
        void tearDown();
        void setUp();
        void testConfigData();
        void testConfigDataExceptions();
        void testDuplicateLinkException();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testNonLinearInitialization();
        void testDuplicateInitException();
        void testLinkInitException();
        void testNodeInitException();
        void testNonInitException();
        void testLinearStep();
        void testNonLinearStep();
        void testLinearStepExceptions();
        void testNonConvergence();
        void testFluidNetworkStep();
        void testSolutionRejection();
        void testDummyMode();
        void testSlaveMode();
        void testSolutionDelay();
        void testAccessMethods();
        void testMatrixConditioning();
        void testDebugPreDecomposition();
        void testDebugNode();
        void testBuildIslands();
        void testSolveIslands();
        void testWorstCaseTimingConverging();
        void testWorstCaseTimingNonConverging();
        void testRestart();
        void testNetworkCapacitance();
        void testSolveIslands2();
        void testCheckStepInputs();
        void testGpuSparse();
        void testGpuDense();
        void testGpuSparseIslands();
        void testGpuDenseIslands();
};

///@}

#endif
