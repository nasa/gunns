/*
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/thermal/GunnsThermalPhaseChangeBattery.o))
*/

#include "UtGunnsThermalPhaseChangeBattery.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsThermalPhaseChangeBattery::TEST_ID = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermalPhaseChangeBattery class.
///////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalPhaseChangeBattery::UtGunnsThermalPhaseChangeBattery()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tName(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPhaseChangeTemperature(),
    tPhaseChangeHeat(),
    tHotPhaseSpecificHeat(),
    tColdPhaseSpecificHeat(),
    tStructureCapacitance(),
    tMass(),
    tTemperature(),
    tHotPhaseFraction(),
    tMalfHotPhaseLeakFlag(),
    tMalfHotPhaseLeakRate(),
    tPort0(),
    tTimeStep()
{
    // Nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsThermalPhaseChangeBattery class.
///////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalPhaseChangeBattery::~UtGunnsThermalPhaseChangeBattery()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPhaseChangeBattery::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPhaseChangeBattery::setUp()
{
    /// - Declare the standard Gunns Link test data.
    tName                          = "tArticle";
    tNodeList.mNumNodes            = 2;
    tNodeList.mNodes               = tNodes;
    tPort0                         = 0;
    tTimeStep                      = 0.1;

    /// - Initialize nodes.
    tNodes[0].initialize("tNodes_0", 400.0);

    /// - Define nominal configuration data.
    tPhaseChangeTemperature =  273.15;
    tPhaseChangeHeat        =  333.55;
    tHotPhaseSpecificHeat   =    4.22;
    tColdPhaseSpecificHeat  =    2.05;
    tStructureCapacitance   = 1000.0;
    tConfigData = new GunnsThermalPhaseChangeBatteryConfigData(tName, &tNodeList,
                                                               tPhaseChangeTemperature,
                                                               tPhaseChangeHeat,
                                                               tHotPhaseSpecificHeat,
                                                               tColdPhaseSpecificHeat,
                                                               tStructureCapacitance);

    /// - Define nominal input data.
    tMass                   = 10.0;
    tTemperature            = 200.0;
    tHotPhaseFraction       = 0.5;
    tMalfHotPhaseLeakFlag   = true;
    tMalfHotPhaseLeakRate   = 1.0;
    tInputData = new GunnsThermalPhaseChangeBatteryInputData(tMass,
                                                             tTemperature,
                                                             tHotPhaseFraction,
                                                             tMalfHotPhaseLeakFlag,
                                                             tMalfHotPhaseLeakRate);

    /// - Define the test Article.
    tArticle = new FriendlyGunnsThermalPhaseChangeBattery();

    /// - Increment the test identification number.
    ++TEST_ID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPhaseChangeBattery::testConfig()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tName                   == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                  == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tPhaseChangeTemperature == tConfigData->mPhaseChangeTemperature);
    CPPUNIT_ASSERT(tPhaseChangeHeat        == tConfigData->mPhaseChangeHeat);
    CPPUNIT_ASSERT(tHotPhaseSpecificHeat   == tConfigData->mHotPhaseSpecificHeat);
    CPPUNIT_ASSERT(tColdPhaseSpecificHeat  == tConfigData->mColdPhaseSpecificHeat);
    CPPUNIT_ASSERT(tStructureCapacitance   == tConfigData->mStructureCapacitance);

    /// - Check default config construction
    GunnsThermalPhaseChangeBatteryConfigData defaultConfig;
    CPPUNIT_ASSERT(""                      == defaultConfig.mName);
    CPPUNIT_ASSERT(0                       == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                     == defaultConfig.mPhaseChangeTemperature);
    CPPUNIT_ASSERT(0.0                     == defaultConfig.mPhaseChangeHeat);
    CPPUNIT_ASSERT(0.0                     == defaultConfig.mHotPhaseSpecificHeat);
    CPPUNIT_ASSERT(0.0                     == defaultConfig.mColdPhaseSpecificHeat);
    CPPUNIT_ASSERT(0.0                     == defaultConfig.mStructureCapacitance);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPhaseChangeBattery::testInput()
{
    UT_RESULT;

    /// - Check nominal input construction
    CPPUNIT_ASSERT(tMass                 == tInputData->mMass);
    CPPUNIT_ASSERT(tTemperature          == tInputData->mTemperature);
    CPPUNIT_ASSERT(tHotPhaseFraction     == tInputData->mHotPhaseFraction);
    CPPUNIT_ASSERT(tMalfHotPhaseLeakFlag == tInputData->mMalfHotPhaseLeakFlag);
    CPPUNIT_ASSERT(tMalfHotPhaseLeakRate == tInputData->mMalfHotPhaseLeakRate);

    /// - Check default input construction
    GunnsThermalPhaseChangeBatteryInputData defaultInput;
    CPPUNIT_ASSERT(0.0                   == defaultInput.mMass);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mTemperature);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mHotPhaseFraction);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mMalfHotPhaseLeakFlag);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mMalfHotPhaseLeakRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPhaseChangeBattery::testDefaultConstruction()
{
    UT_RESULT;

    /// @test Default values on construction
    CPPUNIT_ASSERT(false == tArticle->mMalfHotPhaseLeakFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfHotPhaseLeakRate);
    CPPUNIT_ASSERT(0.0   == tArticle->mPhaseChangeTemperature);
    CPPUNIT_ASSERT(0.0   == tArticle->mPhaseChangeHeat);
    CPPUNIT_ASSERT(0.0   == tArticle->mHotPhaseSpecificHeat);
    CPPUNIT_ASSERT(0.0   == tArticle->mColdPhaseSpecificHeat);
    CPPUNIT_ASSERT(0.0   == tArticle->mStructureCapacitance);
    CPPUNIT_ASSERT(0.0   == tArticle->mMass);
    CPPUNIT_ASSERT(0.0   == tArticle->mTemperature);
    CPPUNIT_ASSERT(0.0   == tArticle->mHotPhaseFraction);
    CPPUNIT_ASSERT(0.0   == tArticle->mActualLeakRate);
    CPPUNIT_ASSERT(0.0   == tArticle->mExternalHeatFlux[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mSumExternalHeatFluxes);
    CPPUNIT_ASSERT(0.0   == tArticle->mAdmittance);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for initialization without exceptions.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPhaseChangeBattery::testInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test base class init.
    CPPUNIT_ASSERT(tName == tArticle->mName);

    /// @test config data.
    CPPUNIT_ASSERT(tPhaseChangeTemperature == tArticle->mPhaseChangeTemperature);
    CPPUNIT_ASSERT(tPhaseChangeHeat        == tArticle->mPhaseChangeHeat);
    CPPUNIT_ASSERT(tHotPhaseSpecificHeat   == tArticle->mHotPhaseSpecificHeat);
    CPPUNIT_ASSERT(tColdPhaseSpecificHeat  == tArticle->mColdPhaseSpecificHeat);
    CPPUNIT_ASSERT(tStructureCapacitance   == tArticle->mStructureCapacitance);

    /// @test input data.
    CPPUNIT_ASSERT(tMass                   == tArticle->mMass);
    CPPUNIT_ASSERT(tTemperature            == tArticle->mTemperature);
    CPPUNIT_ASSERT(tHotPhaseFraction       == tArticle->mHotPhaseFraction);
    CPPUNIT_ASSERT(tMalfHotPhaseLeakFlag   == tArticle->mMalfHotPhaseLeakFlag);
    CPPUNIT_ASSERT(tMalfHotPhaseLeakRate   == tArticle->mMalfHotPhaseLeakRate);

    /// @test state data.
    CPPUNIT_ASSERT(0.0                     == tArticle->mActualLeakRate);
    CPPUNIT_ASSERT(0.0                     == tArticle->mExternalHeatFlux[0]);
    CPPUNIT_ASSERT(0.0                     == tArticle->mSumExternalHeatFluxes);
    CPPUNIT_ASSERT(0.0                     == tArticle->mAdmittance);

    /// @test node init.
    CPPUNIT_ASSERT(tTemperature            == tArticle->mPotentialVector[0]);
    CPPUNIT_ASSERT(tTemperature            == tNodes[0].getPotential());

    /// @test init flags.
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for initialization with exceptions.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPhaseChangeBattery::testInitializationExceptions()
{
    UT_RESULT;

    /// @test throw for phase change temperature too low.
    tConfigData->mPhaseChangeTemperature = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mPhaseChangeTemperature = tPhaseChangeTemperature;

    /// @test throw for heat of phase change too low.
    tConfigData->mPhaseChangeHeat = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mPhaseChangeHeat = tPhaseChangeHeat;

    /// @test throw for hot phase specific heat too low.
    tConfigData->mHotPhaseSpecificHeat = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mHotPhaseSpecificHeat = tHotPhaseSpecificHeat;

    /// @test throw for cold phase specific heat too low.
    tConfigData->mColdPhaseSpecificHeat = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mColdPhaseSpecificHeat = tColdPhaseSpecificHeat;

    /// @test throw for structure capacitance too low.
    tConfigData->mStructureCapacitance = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mStructureCapacitance = tStructureCapacitance;

    /// @test throw for initial mass too low.
    tInputData->mMass = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tInputData->mMass = tMass;

    /// @test throw for initial temperature too low.
    tInputData->mTemperature = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tInputData->mTemperature = tTemperature;

    /// @test throw for initial hot phase fraction not in range.
    tInputData->mHotPhaseFraction = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tInputData->mHotPhaseFraction = 1.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tInputData->mHotPhaseFraction = tHotPhaseFraction;

    /// @test throw for initial leak rate too low.
    tInputData->mMalfHotPhaseLeakRate = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tInputData->mMalfHotPhaseLeakRate = tMalfHotPhaseLeakRate;

    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the restart method.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPhaseChangeBattery::testRestart()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// - Store some non-checkpointed values prior to restart.
    tArticle->mExternalHeatFlux[0]   = 1.0;
    tArticle->mSumExternalHeatFluxes = 1.0;
    tArticle->mAdmittance            = 1.0;

    /// @test restart method updates model state.
    CPPUNIT_ASSERT_NO_THROW(tArticle->restart());
    CPPUNIT_ASSERT(0.0 == tArticle->mExternalHeatFlux[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mSumExternalHeatFluxes);
    CPPUNIT_ASSERT(0.0 == tArticle->mAdmittance);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the step method.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPhaseChangeBattery::testStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test step with mixed-phase snaps to phase change temperature.
    tArticle->step(tTimeStep);

    double expectedP = tPhaseChangeTemperature;
    double expectedA = tArticle->mIdealAdmittance;
    double expectedW = expectedP * expectedA;
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTemperature,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mPotentialVector[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tNodes[0].getPotential(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);

    /// @test step with an external heat flux in mixed-phase.
    tArticle->mExternalHeatFlux[0] = 1.0;
    tArticle->mExternalHeatFlux[1] = 1.0;
    tArticle->mAdmittanceUpdate    = false;
    tArticle->step(tTimeStep);

    expectedP = tPhaseChangeTemperature + 2.0 / expectedA;
    expectedW = expectedP * expectedA;
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTemperature,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mPotentialVector[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tNodes[0].getPotential(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);

    /// @test step with all cold-phase, hot phase fraction lower-limited to zero.
    tArticle->mPotentialVector[0] = 200.0;
    tArticle->mHotPhaseFraction   = -1.0;
    tArticle->mAdmittanceUpdate   = false;
    tArticle->step(tTimeStep);

    double capacitance = tStructureCapacitance + tMass * tColdPhaseSpecificHeat * 1000.0;
    expectedA = capacitance / tTimeStep;
    expectedP = 200.0 + 2.0 * tTimeStep / capacitance;
    expectedW = expectedP * expectedA;
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mHotPhaseFraction);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTemperature,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mPotentialVector[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tNodes[0].getPotential(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);

    /// @test step with all hot-phase, hot phase fraction upper-limited to 1.
    tArticle->mPotentialVector[0] = 300.0;
    tArticle->mHotPhaseFraction   = 2.0;
    tArticle->mAdmittanceUpdate   = false;
    tArticle->step(tTimeStep);

    capacitance = tStructureCapacitance + tMass * tHotPhaseSpecificHeat * 1000.0;
    expectedA = capacitance / tTimeStep;
    expectedP = 300.0 + 2.0 * tTimeStep / capacitance;
    expectedW = expectedP * expectedA;
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT_EQUAL(1.0,   tArticle->mHotPhaseFraction);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTemperature,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mPotentialVector[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tNodes[0].getPotential(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);

    /// @test step attached to Ground node.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
    tArticle->mPotentialVector[0] = 300.0;
    tArticle->mAdmittanceUpdate   = false;
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT_EQUAL(1,     tArticle->mNodeMap[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,   tNodes[1].getPotential());
    CPPUNIT_ASSERT_EQUAL(1.0,   tArticle->mHotPhaseFraction);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTemperature,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mPotentialVector[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);

    /// @test step with override vector active.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::DEFAULT;
    tArticle->mPotentialVector[0] = 300.0;
    tArticle->mOverrideVector[0]  = true;
    tArticle->mAdmittanceUpdate   = false;
    tArticle->step(tTimeStep);

    expectedA = DBL_EPSILON;
    expectedP = 300.0;
    expectedW = expectedP * expectedA;
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT_EQUAL(0,     tArticle->mNodeMap[0]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mPotentialVector[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);

    /// - Call these methods directly for line coverage because they're getting in-lined by the
    ///   compiler.
    tArticle->zeroExternalFluxes();
    tArticle->sumExternalFluxes();

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeFlows method.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPhaseChangeBattery::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test computeFlows in mixed-phase with no flux and positive external power.
    tArticle->mExternalHeatFlux[0]  = 1.0;
    tArticle->mExternalHeatFlux[1]  = 1.0;
    tArticle->mMalfHotPhaseLeakFlag = false;
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

    double expectedA     = tArticle->mIdealAdmittance;
    double expectedP     = tPhaseChangeTemperature + 2.0 / expectedA;
    double expectedW     = expectedP * expectedA;
    double expectedFlux  = expectedP * expectedA - expectedW;
    double expectedPower = expectedFlux + 2.0;
    double expectedFrac  = tHotPhaseFraction + 0.001 * expectedPower * tTimeStep / tPhaseChangeHeat / tMass;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower, tArticle->mPower,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP,     tArticle->mPotentialDrop,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tNodes[0].getInflux(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tNodes[0].getOutflux(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFrac,  tArticle->mHotPhaseFraction, DBL_EPSILON);

    /// @test computeFlows in cold phase with positive flux, T < phase change T.
    double capacitance = tStructureCapacitance + tMass * tColdPhaseSpecificHeat * 1000.0;
    expectedA = capacitance / tTimeStep;
    expectedP = 200.0 + 2.0 * tTimeStep / capacitance;
    expectedW = expectedP * expectedA;
    expectedP += 1.0;
    tNodes[0].resetFlows();
    tArticle->mAdmittanceMatrix[0] = expectedA;
    tArticle->mSourceVector[0]     = expectedW;
    tArticle->mPotentialVector[0]  = expectedP;
    tArticle->mHotPhaseFraction    = 0.0;
    tArticle->computeFlows(tTimeStep);

    expectedFlux  = expectedP * expectedA - expectedW;
    expectedPower = expectedFlux + 2.0;
    expectedFrac  = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower, tArticle->mPower,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP,     tArticle->mPotentialDrop,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tNodes[0].getInflux(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tNodes[0].getOutflux(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFrac,  tArticle->mHotPhaseFraction, DBL_EPSILON);

    /// @test computeFlows in cold phase with positive flux, T > phase change T.
    expectedP = 273.0 + 2.0 * tTimeStep / capacitance;
    expectedW = expectedP * expectedA;
    expectedP = 274.0;
    tNodes[0].resetFlows();
    tArticle->mSourceVector[0]     = expectedW;
    tArticle->mPotentialVector[0]  = expectedP;
    tArticle->computeFlows(tTimeStep);

    expectedFlux  = expectedP * expectedA - expectedW;
    expectedPower = expectedFlux + 2.0;
    expectedFrac  = tColdPhaseSpecificHeat * (expectedP - tPhaseChangeTemperature) / tPhaseChangeHeat;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower, tArticle->mPower,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP,     tArticle->mPotentialDrop,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tNodes[0].getInflux(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tNodes[0].getOutflux(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFrac,  tArticle->mHotPhaseFraction, DBL_EPSILON);

    /// @test computeFlows in hot phase with negative flux, T > phase change T.
    tArticle->mExternalHeatFlux[0]  = -1.0;
    tArticle->mExternalHeatFlux[1]  = -1.0;
    capacitance = tStructureCapacitance + tMass * tHotPhaseSpecificHeat * 1000.0;
    expectedA = capacitance / tTimeStep;
    expectedP = 400.0 - 2.0 * tTimeStep / capacitance;
    expectedW = expectedP * expectedA;
    expectedP -= 1.0;
    tNodes[0].resetFlows();
    tArticle->mAdmittanceMatrix[0] = expectedA;
    tArticle->mSourceVector[0]     = expectedW;
    tArticle->mPotentialVector[0]  = expectedP;
    tArticle->mHotPhaseFraction    = 1.0;
    tArticle->computeFlows(tTimeStep);

    expectedFlux  = expectedP * expectedA - expectedW;
    expectedPower = expectedFlux + 2.0;
    expectedFrac  = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower, tArticle->mPower,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP,     tArticle->mPotentialDrop,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFlux, tNodes[0].getInflux(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tNodes[0].getOutflux(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFrac,  tArticle->mHotPhaseFraction, DBL_EPSILON);

    /// @test computeFlows in hot phase with negative flux, T < phase change T.
    expectedP = 274.0 - 2.0 * tTimeStep / capacitance;
    expectedW = expectedP * expectedA;
    expectedP = 273.0;
    tNodes[0].resetFlows();
    tArticle->mSourceVector[0]     = expectedW;
    tArticle->mPotentialVector[0]  = expectedP;
    tArticle->computeFlows(tTimeStep);

    expectedFlux  = expectedP * expectedA - expectedW;
    expectedPower = expectedFlux + 2.0;
    expectedFrac  = 1.0 + tHotPhaseSpecificHeat * (expectedP - tPhaseChangeTemperature) / tPhaseChangeHeat;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower, tArticle->mPower,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP,     tArticle->mPotentialDrop,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFlux, tNodes[0].getInflux(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           tNodes[0].getOutflux(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFrac,  tArticle->mHotPhaseFraction, DBL_EPSILON);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the hot phase leak malfunction.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPhaseChangeBattery::testLeakMalf()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// - Test leak does nothing if timestep is zero, and lower-limited rate malf to zero.
    tArticle->mMalfHotPhaseLeakRate = -1.0;
    tArticle->computeFlows(0.0);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mMalfHotPhaseLeakRate);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMass,             tArticle->mMass,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tHotPhaseFraction, tArticle->mHotPhaseFraction, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,               tArticle->mActualLeakRate,   DBL_EPSILON);

    /// - Test leak does nothing when there's no hot phase mass.
    tArticle->mMalfHotPhaseLeakRate = tMalfHotPhaseLeakRate;
    tArticle->mHotPhaseFraction     = 0.0;
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMass,             tArticle->mMass,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,               tArticle->mHotPhaseFraction, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,               tArticle->mActualLeakRate,   DBL_EPSILON);

    /// - Test normal leak.
    tArticle->mHotPhaseFraction = tHotPhaseFraction;
    tArticle->computeFlows(tTimeStep);
    double expectedRate = tMalfHotPhaseLeakRate;
    double expectedMass = tMass - expectedRate * tTimeStep;
    double expectedFrac = (tMass * tHotPhaseFraction - expectedRate * tTimeStep) / expectedMass;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,      tArticle->mMass,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFrac,      tArticle->mHotPhaseFraction, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,      tArticle->mActualLeakRate,   DBL_EPSILON);

    /// - Test leak limited to available hot mass.
    tArticle->mMass = 0.0001;
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(0.0 < tArticle->mMass);
    CPPUNIT_ASSERT(0.0 < tArticle->mHotPhaseFraction);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the setter & getter methods.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPhaseChangeBattery::testAccessors()
{
    UT_RESULT;

    /// @test the setMalfHotPhaseLeak method to set the malf.
    tArticle->setMalfHotPhaseLeak(true, 2.0);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mMalfHotPhaseLeakFlag);
    CPPUNIT_ASSERT_EQUAL(2.0,   tArticle->mMalfHotPhaseLeakRate);

    /// @test the setMalfHotPhaseLeak method rests the malf.
    tArticle->setMalfHotPhaseLeak();
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mMalfHotPhaseLeakFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfHotPhaseLeakRate);

    /// @test the getTemperature method.
    tArticle->mTemperature = 5.0;
    CPPUNIT_ASSERT_EQUAL(5.0,   tArticle->getTemperature());

    /// @test the getHotPhaseFraction method.
    tArticle->mHotPhaseFraction = 0.2;
    CPPUNIT_ASSERT_EQUAL(0.2,   tArticle->getHotPhaseFraction());

    /// @test the getActualLeakRate method.
    tArticle->mActualLeakRate = 3.0;
    CPPUNIT_ASSERT_EQUAL(3.0,   tArticle->getActualLeakRate());

    UT_PASS_LAST;
}
