/*
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "software/exceptions/TsInitializationException.hh"
#include "math/approximation/TsLinearInterpolator.hh"
#include "UtGunnsElectBattery.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsElectBattery::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectBattery class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectBattery::UtGunnsElectBattery()
    :
    CppUnit::TestFixture(),
    tConfigData(),
    tInputData(),
    tArticle(),
    tLinks(),
    tName(),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tNumCells(0),
    tCellsInParallel(false),
    tCellResistance(0.0),
    tInterconnectResistance(0.0),
    tMaxCapacity(0.0),
    tSocVocTable(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tMalfThermalRunawayFlag(false),
    tMalfThermalRunawayDuration(0.0),
    tMalfThermalRunawayInterval(0.0),
    tSoc(0.0)
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectBattery class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectBattery::~UtGunnsElectBattery()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::setUp()
{
    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode0", 1.0);
    tNodes[1].initialize("UtNode1", 0.0);

    /// - Initialize the nodes list.
    tNodeList.mNodes         = tNodes;
    tNodeList.mNumNodes      = N_NODES;

    /// - Create nominal configuration data.  Set up the SOC/VOC table so it will interpolate to the
    ///   same Voc output as SOC input (0 - 1).
    tName                    = "tArticle",
    tNumCells                = 10;
    tCellsInParallel         = true;
    tCellResistance          = 1.0;
    tInterconnectResistance  = 0.1;
    tMaxCapacity             = 32.0;
    const double socPoints[] = {0.0, 1.0};
    const double vocPoints[] = {0.0, 1.0};
    tSocVocTable             = new TsLinearInterpolator(socPoints, vocPoints, 2, 0.0, 1.0);
    tConfigData              = new GunnsElectBatteryConfigData(tName,
                                                               &tNodeList,
                                                               tNumCells,
                                                               tCellsInParallel,
                                                               tCellResistance,
                                                               tInterconnectResistance,
                                                               tMaxCapacity,
                                                               tSocVocTable);

    /// - Create nominal input data.
    tMalfBlockageFlag           = true;
    tMalfBlockageValue          = 0.5;
    tSoc                        = 0.9;
    tMalfThermalRunawayFlag     = false;
    tMalfThermalRunawayDuration = 0.0;
    tMalfThermalRunawayInterval = 0.0;
    tInputData                  = new GunnsElectBatteryInputData(tMalfBlockageFlag,
                                                                 tMalfBlockageValue,
                                                                 tSoc,
                                                                 tMalfThermalRunawayFlag,
                                                                 tMalfThermalRunawayDuration,
                                                                 tMalfThermalRunawayInterval);

    /// - Create the test article.
    tPort0                   = 1;
    tPort1                   = 0;
    tArticle                 = new FriendlyGunnsElectBattery;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tSocVocTable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::testConfig()
{
    UT_RESULT_FIRST;

    /// @test nominal config construction.
    CPPUNIT_ASSERT(tName                   == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                  == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tNumCells               == tConfigData->mNumCells);
    CPPUNIT_ASSERT(tCellsInParallel        == tConfigData->mCellsInParallel);
    CPPUNIT_ASSERT(tCellResistance         == tConfigData->mCellResistance);
    CPPUNIT_ASSERT(tInterconnectResistance == tConfigData->mInterconnectResistance);
    CPPUNIT_ASSERT(tMaxCapacity            == tConfigData->mMaxCapacity);
    CPPUNIT_ASSERT(tSocVocTable            == tConfigData->mSocVocTable);

    /// @test default config construction.
    GunnsElectBatteryConfigData defaultConfig;
    CPPUNIT_ASSERT(0                       == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                       == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0                       == defaultConfig.mNumCells);
    CPPUNIT_ASSERT(false                   == defaultConfig.mCellsInParallel);
    CPPUNIT_ASSERT(0.0                     == defaultConfig.mCellResistance);
    CPPUNIT_ASSERT(0.0                     == defaultConfig.mInterconnectResistance);
    CPPUNIT_ASSERT(0.0                     == defaultConfig.mMaxCapacity);
    CPPUNIT_ASSERT(0                       == defaultConfig.mSocVocTable);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::testInput()
{
    UT_RESULT;

    /// @test nominal input construction.
    GunnsElectBatteryInputData nominalInput(tMalfBlockageFlag, tMalfBlockageValue, tSoc,
                                            true, 10.0, 5.0);
    CPPUNIT_ASSERT(tMalfBlockageFlag  == nominalInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == nominalInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tSoc               == nominalInput.mSoc);
    CPPUNIT_ASSERT(true               == nominalInput.mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(10.0               == nominalInput.mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(5.0                == nominalInput.mMalfThermalRunawayInterval);

    /// @test default input construction.
    GunnsElectBatteryInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                == defaultInput.mSoc);
    CPPUNIT_ASSERT(false              == defaultInput.mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfThermalRunawayInterval);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::testDefaultConstruction()
{
    UT_RESULT;

    /// @test state data.
    CPPUNIT_ASSERT(0     == tArticle->mCells);
    CPPUNIT_ASSERT(false == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(false == tArticle->mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfThermalRunawayInterval);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourcePotential);
    CPPUNIT_ASSERT(0     == tArticle->mNumCells);
    CPPUNIT_ASSERT(false == tArticle->mCellsInParallel);
    CPPUNIT_ASSERT(0.0   == tArticle->mInterconnectResistance);
    CPPUNIT_ASSERT(0     == tArticle->mSocVocTable);
    CPPUNIT_ASSERT(0.0   == tArticle->mSoc);
    CPPUNIT_ASSERT(0.0   == tArticle->mCapacity);
    CPPUNIT_ASSERT(0.0   == tArticle->mCurrent);
    CPPUNIT_ASSERT(0.0   == tArticle->mVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mHeat);
    CPPUNIT_ASSERT(0     == tArticle->mThermalRunawayCell);
    CPPUNIT_ASSERT(0.0   == tArticle->mThermalRunawayTimer);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::testNominalInitialization()
{
    UT_RESULT;

    /// @test initialize with initialization data and no exceptions.
    tMalfThermalRunawayFlag                 = true;
    tMalfThermalRunawayDuration             = 10.0;
    tMalfThermalRunawayInterval             = 5.0;
    tInputData->mMalfThermalRunawayFlag     = tMalfThermalRunawayFlag;
    tInputData->mMalfThermalRunawayDuration = tMalfThermalRunawayDuration;
    tInputData->mMalfThermalRunawayInterval = tMalfThermalRunawayInterval;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));
    CPPUNIT_ASSERT(tName                       == tArticle->mName);
    CPPUNIT_ASSERT(tMalfBlockageFlag           == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue          == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(tMalfThermalRunawayFlag     == tArticle->mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(tMalfThermalRunawayDuration == tArticle->mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(tMalfThermalRunawayInterval == tArticle->mMalfThermalRunawayInterval);
    CPPUNIT_ASSERT(tNumCells                   == tArticle->mNumCells);
    CPPUNIT_ASSERT(tCellsInParallel            == tArticle->mCellsInParallel);
    CPPUNIT_ASSERT(tInterconnectResistance     == tArticle->mInterconnectResistance);
    CPPUNIT_ASSERT(tSocVocTable                == tArticle->mSocVocTable);
    CPPUNIT_ASSERT(0                           != tArticle->mCells);

    for (int i=0; i<tNumCells; ++i) {
        CPPUNIT_ASSERT(tCellResistance      == tArticle->mCells[i].getEffectiveResistance());
        CPPUNIT_ASSERT(tSoc                 == tArticle->mCells[i].getEffectiveSoc());
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSoc,         tArticle->mSoc,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCapacity, tArticle->mCapacity, FLT_EPSILON);
    CPPUNIT_ASSERT(tNodes[1].getPotential() == tArticle->mVoltage);
    CPPUNIT_ASSERT(0.0                      == tArticle->mCurrent);
    CPPUNIT_ASSERT(true                     == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::testInitializationExceptions()
{
    UT_RESULT;

    /// @test for exception on missing name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mName = tName;

    /// @test for exception on invalid config data: interconnect resistance.
    tConfigData->mInterconnectResistance = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mInterconnectResistance = tInterconnectResistance;

    /// @test for exception on invalid config data: no cells.
    tConfigData->mNumCells = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mNumCells = tNumCells;

    /// @test for exception on invalid config data: missing SOC/VOC table.
    tConfigData->mSocVocTable = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mSocVocTable = tSocVocTable;

    /// @test for exception on invalid config data: state of charge.
    tInputData->mSoc = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tInputData->mSoc = 1.0 + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tInputData->mSoc = tSoc;

    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set non-config and non-checkpointed base class and article class attributes.
    tArticle->mEffectiveConductivity = 999.0;
    tArticle->mSystemConductance     = 999.0;
    tArticle->mSoc                   = 999.0;
    tArticle->mCapacity              = 999.0;
    tArticle->mCurrent               = 999.0;
    tArticle->mVoltage               = 999.0;

    tArticle->restart();

    /// @test Base class attributes are reset.
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mSystemConductance);

    /// @test Non-config and non-checkpointed class attributes are reset.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSoc,         tArticle->mSoc,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCapacity, tArticle->mCapacity, FLT_EPSILON);
    CPPUNIT_ASSERT(tNodes[1].getPotential() == tArticle->mVoltage);
    CPPUNIT_ASSERT(0.0                      == tArticle->mCurrent);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the updateState method with cells in parallel.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::testUpdateStateParallel()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test nominal contributions to the network system of equations.
    double expectedG = 1.0 / (tInterconnectResistance + tCellResistance / tNumCells);
    double expectedA = expectedG * tMalfBlockageValue;
    double expectedP = tSoc;
    double expectedW = expectedP * expectedA;
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mSourcePotential,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[1],       DBL_EPSILON);

    /// @test Fail open the 0th cell and test contributions to the system of equations with unequal
    ///       cell states.
    tArticle->mCells[0].setMalfOpenCircuit(true);
    expectedG = 1.0 / (tInterconnectResistance + tCellResistance / (tNumCells - 1));
    expectedP = tSoc;
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mSourcePotential,       DBL_EPSILON);

    /// @test Step all the unfailed cells with a current to lower their SOC, and verify battery
    ///       Voc output is lowered.
    // and verify its VOC gets used.
    for (int i=1; i<tNumCells; ++i) {
        tArticle->mCells[i].updateSoc(10.0, 1.0, tSocVocTable);
    }
    tArticle->step(0.0);
    CPPUNIT_ASSERT(expectedP > tArticle->mSourcePotential);

    /// @test Unfail the 0th cell and verify the battery takes its Voc since it's now the cell with
    ///       the highest Voc in the parallel set.
    tArticle->mCells[0].setMalfOpenCircuit();
    expectedG = 1.0 / (tInterconnectResistance + tCellResistance / tNumCells);
    expectedP = tSoc;
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mSourcePotential,       DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the updateState method with cells in series.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::testUpdateStateSeries()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data and with the cells
    ///   configured to be in series.
    tConfigData->mCellsInParallel = false;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test nominal contributions to the network system of equations.
    double expectedG = 1.0 / (tInterconnectResistance + tCellResistance * tNumCells);
    double expectedA = expectedG * tMalfBlockageValue;
    double expectedP = tSoc * tNumCells;
    double expectedW = expectedP * expectedA;
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mSourcePotential,       FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[1],       FLT_EPSILON);

    /// @test Fail open the 0th cell and test contributions to the system of equations with unequal
    ///       cell states.
    tArticle->mCells[0].setMalfOpenCircuit(true);
    expectedG = DBL_EPSILON;
    expectedP = tSoc * (tNumCells - 1);
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mSourcePotential,       FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the updateFlux method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::testUpdateFlux()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test load on the cells reduces battery SOC, and current & voltage output.
    tArticle->mPotentialVector[1] = tSoc;
    const double dt = 0.1;
    tArticle->step(dt);
    tArticle->computeFlows(dt);

    double current          = tArticle->mFlux;
    double expectedCellSoc  = tSoc - current * dt * tNumCells / tMaxCapacity / 3600.0;
    double expectedCapacity = tMaxCapacity;
    double expectedHeat     = current * current / tArticle->mAdmittanceMatrix[0];
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCellSoc,  tArticle->mSoc,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCapacity, tArticle->mCapacity,    FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat,     tArticle->getHeat(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSoc,             tArticle->getVoltage(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlux,  tArticle->mCurrent,     DBL_EPSILON);

    /// @test battery SOC & capacity drop due to a failed cell.
    tArticle->mCells[0].setMalfOpenCircuit(true);
    tArticle->computeFlows(dt);
    expectedCellSoc -= tArticle->mFlux * dt * (tNumCells - 1) / tMaxCapacity / 3600.0;
    expectedCapacity = tMaxCapacity * (tNumCells - 1) / tNumCells;
    double expectedSoc = expectedCellSoc * (tNumCells - 1) / tNumCells;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSoc,      tArticle->mSoc,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCapacity, tArticle->mCapacity,    FLT_EPSILON);

    /// @test method can handle number of cells being zeroed.
    tArticle->mNumCells = 0;
    tArticle->computeFlows(dt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->mSoc,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->mCapacity,    DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the thermal runaway malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::testThermalRunaway()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test inactive malfunction doesn't preclude cell's malfunction.
    const double dt = 0.1;
    tArticle->mCells[0].setMalfThermalRunaway(true, 10.0);
    tArticle->step(dt);
    CPPUNIT_ASSERT(10.0        == tArticle->mCells[0].mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(true        == tArticle->mCells[0].mMalfThermalRunawayFlag);

    /// @test malf setter method and first step.
    tArticle->setMalfThermalRunaway(true, 10.0, 5.0);
    tArticle->step(dt);
    CPPUNIT_ASSERT(10.0        == tArticle->mCells[0].mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(true        == tArticle->mCells[0].mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(1.0 / DBL_EPSILON == tArticle->mCells[0].getEffectiveResistance());
    CPPUNIT_ASSERT(0.0   == tArticle->mCells[1].mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(false == tArticle->mCells[1].mMalfThermalRunawayFlag);
    double expectedA = 0.5 / (tInterconnectResistance + tCellResistance / (tNumCells - 1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT(0.0 < tArticle->mSourceVector[1]);

    tArticle->mPotentialVector[0] = 0.0;
    tArticle->mPotentialVector[1] = tArticle->mSourceVector[1] * 0.999 / tArticle->mAdmittanceMatrix[0];
    tArticle->computeFlows(dt);
    double current1      = tArticle->mFlux;
    double expectedHeat1 = current1 * current1 / tArticle->mAdmittanceMatrix[0]
                         + tArticle->mCells[0].getRunawayPower();
    CPPUNIT_ASSERT(0.0 > tArticle->mPotentialDrop);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat1, tArticle->getHeat(), DBL_EPSILON);
    CPPUNIT_ASSERT(0.0 < current1);
    CPPUNIT_ASSERT(0.0 < tArticle->mCells[0].getRunawayPower());

    /// @test malf progression on the initial cell.
    tArticle->step(dt);
    CPPUNIT_ASSERT(10.0  == tArticle->mCells[0].mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(true  == tArticle->mCells[0].mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mCells[1].mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(false == tArticle->mCells[1].mMalfThermalRunawayFlag);

    tArticle->computeFlows(dt);
    double current2      = tArticle->mFlux;
    double expectedHeat2 = current2 * current2 / tArticle->mAdmittanceMatrix[0]
                         + tArticle->mCells[0].getRunawayPower();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat2, tArticle->getHeat(), DBL_EPSILON);
    CPPUNIT_ASSERT(0.0           < current2);
    CPPUNIT_ASSERT(current2      < current1);
    CPPUNIT_ASSERT(expectedHeat1 < expectedHeat2);
    CPPUNIT_ASSERT(0.0           < tArticle->mCells[0].getRunawayPower());

    /// @test progression into the next cell, and both cells with active malf.
    for (unsigned int i=2; i<52; ++i) {
        tArticle->step(dt);
        tArticle->computeFlows(dt);
    }
    CPPUNIT_ASSERT(10.0  == tArticle->mCells[0].mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(true  == tArticle->mCells[0].mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(10.0  == tArticle->mCells[1].mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(true  == tArticle->mCells[1].mMalfThermalRunawayFlag);
    double current3      = tArticle->mFlux;
    double expectedHeat3 = current3 * current3 / tArticle->mAdmittanceMatrix[0]
                         + tArticle->mCells[0].getRunawayPower()
                         + tArticle->mCells[1].getRunawayPower();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat3, tArticle->getHeat(), DBL_EPSILON);
    CPPUNIT_ASSERT(0.0           < tArticle->mCells[0].getRunawayPower());
    CPPUNIT_ASSERT(0.0           < tArticle->mCells[1].getRunawayPower());

    /// @test malfunction reset.
    tArticle->setMalfThermalRunaway();
    tArticle->step(dt);
    CPPUNIT_ASSERT(0.0   == tArticle->mCells[0].mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(false == tArticle->mCells[0].mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mCells[1].mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(false == tArticle->mCells[1].mMalfThermalRunawayFlag);
    tArticle->computeFlows(dt);
    double current4      = tArticle->mFlux;
    double expectedHeat4 = current4 * current4 / tArticle->mAdmittanceMatrix[0];
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat4, tArticle->getHeat(), DBL_EPSILON);

    /// @test malfunction restart, results after all cells have finished.
    tArticle->setMalfThermalRunaway(true, 1.0, 1.0);
    for (unsigned int i=0; i<11*tNumCells; ++i) {
        tArticle->step(dt);
        tArticle->computeFlows(dt);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSoc,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->getHeat(), FLT_EPSILON);

    /// @test protection against someone forcing cell count out of bounds.
    tArticle->mThermalRunawayCell = tNumCells + 2;
    CPPUNIT_ASSERT_NO_THROW(tArticle->step(dt));
    CPPUNIT_ASSERT(0 == tArticle->mThermalRunawayCell);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the getter and setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBattery::testAccessors()
{
    UT_RESULT;

    tArticle->mVoltage = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->getVoltage(), DBL_EPSILON);

    tArticle->mSoc = 0.5;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tArticle->getSoc(),     DBL_EPSILON);

    tArticle->mCapacity = tMaxCapacity;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxCapacity, tArticle->getCapacity(), DBL_EPSILON);

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test  getCellEffectiveVoltage.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSoc, tArticle->getCellEffectiveVoltage(0),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->getCellEffectiveVoltage(10),    DBL_EPSILON);

    UT_PASS_FINAL;
}
