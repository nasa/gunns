/*
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "software/exceptions/TsInitializationException.hh"
#include "math/approximation/TsLinearInterpolator.hh"
#include "UtGunnsElectBatteryCell.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsElectBatteryCell::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectBatteryCell class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectBatteryCell::UtGunnsElectBatteryCell()
    :
    CppUnit::TestFixture(),
    tConfigData(),
    tInputData(),
    tArticle(),
    tResistance(0.0),
    tMaxCapacity(0.0),
    tMalfOpenCircuit(false),
    tMalfShortCircuit(false),
    tMalfCapacityFlag(false),
    tMalfCapacityValue(0.0),
    tMalfThermalRunawayFlag(false),
    tMalfThermalRunawayDuration(0.0),
    tSoc(0.0),
    tName(),
    tSocVocTable(0)
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectBatteryCell class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectBatteryCell::~UtGunnsElectBatteryCell()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::setUp()
{
    /// - Create nominal configuration data.
    tResistance       = 0.1;
    tMaxCapacity      = 32.0;
    tConfigData       = new GunnsElectBatteryCellConfigData(tResistance, tMaxCapacity);

    /// - Create nominal input data.
    tMalfOpenCircuit            = false;
    tMalfShortCircuit           = false;
    tMalfCapacityFlag           = false;
    tMalfCapacityValue          = 0.0;
    tMalfThermalRunawayFlag     = false;
    tMalfThermalRunawayDuration = 0.0;
    tSoc                        = 0.9;
    tInputData                  = new GunnsElectBatteryCellInputData(tMalfOpenCircuit,
                                                                     tMalfShortCircuit,
                                                                     tMalfCapacityFlag,
                                                                     tMalfCapacityValue,
                                                                     tMalfThermalRunawayFlag,
                                                                     tMalfThermalRunawayDuration,
                                                                     tSoc);

    /// - Create the SOC/VOC table and set it up so it will interpolate to the same Voc output as
    ///   SOC in (0 - 1).
    const double socPoints[] = {0.0, 1.0};
    const double vocPoints[] = {0.0, 1.0};
    tSocVocTable      = new TsLinearInterpolator(socPoints, vocPoints, 2, 0.0, 1.0);

    /// - Create the test article.
    tName             = "tArticle";
    tArticle          = new FriendlyGunnsElectBatteryCell;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tSocVocTable;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::testConfig()
{
    UT_RESULT_INITIAL("GUNNS Electrical Battery Classes");

    /// @test nominal config construction.
    CPPUNIT_ASSERT(tResistance  == tConfigData->mResistance);
    CPPUNIT_ASSERT(tMaxCapacity == tConfigData->mMaxCapacity);

    /// @test default config construction.
    GunnsElectBatteryCellConfigData defaultConfig;
    CPPUNIT_ASSERT(0.0 == defaultConfig.mResistance);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMaxCapacity);

    /// @test copy config construction.
    GunnsElectBatteryCellConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tResistance  == copyConfig.mResistance);
    CPPUNIT_ASSERT(tMaxCapacity == copyConfig.mMaxCapacity);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::testInput()
{
    UT_RESULT;

    /// @test nominal input construction.
    GunnsElectBatteryCellInputData nominalInput(true, true, true, 5.0, true, 10.0, 1.0);
    CPPUNIT_ASSERT(true  == nominalInput.mMalfOpenCircuit);
    CPPUNIT_ASSERT(true  == nominalInput.mMalfShortCircuit);
    CPPUNIT_ASSERT(true  == nominalInput.mMalfCapacityFlag);
    CPPUNIT_ASSERT(5.0   == nominalInput.mMalfCapacityValue);
    CPPUNIT_ASSERT(true  == nominalInput.mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(10.0  == nominalInput.mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(1.0   == nominalInput.mSoc);

    /// @test default input construction.
    GunnsElectBatteryCellInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mMalfOpenCircuit);
    CPPUNIT_ASSERT(false == defaultInput.mMalfShortCircuit);
    CPPUNIT_ASSERT(false == defaultInput.mMalfCapacityFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfCapacityValue);
    CPPUNIT_ASSERT(false == defaultInput.mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(0.0   == defaultInput.mSoc);

    /// @test copy input construction.
    GunnsElectBatteryCellInputData copyInput(nominalInput);
    CPPUNIT_ASSERT(true  == copyInput.mMalfOpenCircuit);
    CPPUNIT_ASSERT(true  == copyInput.mMalfShortCircuit);
    CPPUNIT_ASSERT(true  == copyInput.mMalfCapacityFlag);
    CPPUNIT_ASSERT(5.0   == copyInput.mMalfCapacityValue);
    CPPUNIT_ASSERT(true  == copyInput.mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(10.0  == copyInput.mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(1.0   == copyInput.mSoc);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::testDefaultConstruction()
{
    UT_RESULT;

    /// @test state data.
    CPPUNIT_ASSERT(false == tArticle->mMalfOpenCircuit);
    CPPUNIT_ASSERT(false == tArticle->mMalfShortCircuit);
    CPPUNIT_ASSERT(false == tArticle->mMalfCapacityFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfCapacityValue);
    CPPUNIT_ASSERT(false == tArticle->mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(0.0   == tArticle->mResistance);
    CPPUNIT_ASSERT(0.0   == tArticle->mMaxCapacity);
    CPPUNIT_ASSERT(0.0   == tArticle->mSoc);
    CPPUNIT_ASSERT(0.0   == tArticle->mRunawayPower);
    CPPUNIT_ASSERT(0.0   == tArticle->mRunawayPowerRate);

    /// @test default new & delete for code coverage.
    GunnsElectBatteryCell* article = new GunnsElectBatteryCell;
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::testNominalInitialization()
{
    UT_RESULT;

    tMalfOpenCircuit                        = true;
    tMalfShortCircuit                       = true;
    tMalfCapacityFlag                       = true;
    tMalfCapacityValue                      = 16.0;
    tMalfThermalRunawayFlag                 = true;
    tMalfThermalRunawayDuration             = 10.0;
    tInputData->mMalfOpenCircuit            = tMalfOpenCircuit;
    tInputData->mMalfShortCircuit           = tMalfShortCircuit;
    tInputData->mMalfCapacityFlag           = tMalfCapacityFlag;
    tInputData->mMalfCapacityValue          = tMalfCapacityValue;
    tInputData->mMalfThermalRunawayFlag     = tMalfThermalRunawayFlag;
    tInputData->mMalfThermalRunawayDuration = tMalfThermalRunawayDuration;

    /// @test initialize with initialization data and no exceptions.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tName));
    CPPUNIT_ASSERT(tResistance                 == tArticle->mResistance);
    CPPUNIT_ASSERT(tMaxCapacity                == tArticle->mMaxCapacity);
    CPPUNIT_ASSERT(tResistance                 == tArticle->mResistance);
    CPPUNIT_ASSERT(tMalfOpenCircuit            == tArticle->mMalfOpenCircuit);
    CPPUNIT_ASSERT(tMalfShortCircuit           == tArticle->mMalfShortCircuit);
    CPPUNIT_ASSERT(tMalfCapacityFlag           == tArticle->mMalfCapacityFlag);
    CPPUNIT_ASSERT(tMalfCapacityValue          == tArticle->mMalfCapacityValue);
    CPPUNIT_ASSERT(tMalfThermalRunawayFlag     == tArticle->mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(tMalfThermalRunawayDuration == tArticle->mMalfThermalRunawayDuration);
    CPPUNIT_ASSERT(tSoc                        == tArticle->mSoc);
    CPPUNIT_ASSERT(0.0                         == tArticle->mRunawayPower);
    CPPUNIT_ASSERT(0.0                         == tArticle->mRunawayPowerRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::testInitializationExceptions()
{
    UT_RESULT;

    /// @test for exception on missing name.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, ""), TsInitializationException);

    /// @test for exception on invalid config data: resistance.
    tConfigData->mResistance = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tConfigData->mResistance = tResistance;

    /// @test for exception on invalid config data: maximum capacity.
    tConfigData->mMaxCapacity = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tConfigData->mMaxCapacity = tMaxCapacity;

    /// @test for exception on invalid config data: state of charge.
    tInputData->mSoc = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tInputData->mSoc = 1.0 + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tInputData->mSoc = tSoc;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test nominal updates with no malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::testUpdate()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// @test SOC drops with forward current.
    const double dt          =   0.1;
    double       current     = 100.0;
    double       expectedSoc = tSoc - current * dt / tMaxCapacity / 3600.0;
    double       expectedR   = tResistance;
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSoc, tArticle->mSoc,                              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSoc, tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR,   tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSoc, tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);

    /// @test SOC rises with reverse current.
    current = -100.0;
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSoc,        tArticle->mSoc,                              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSoc,        tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR,   tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSoc,        tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);

    /// @test SOC limited to 1.0.
    current = -1.0e15;
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,         tArticle->mSoc,                              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,         tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR,   tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,         tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);

    /// @test SOC limited to 0.0.
    current = 1.0e15;
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         tArticle->mSoc,                              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR,   tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);

    /// @test protects against divide-by-zero in max capacity.
    tArticle->mMaxCapacity = 0.0;
    tArticle->mSoc         = 1.0;
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         tArticle->mSoc,                              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR,   tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test updates with the open-circuit malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::testUpdateMalfOpen()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// @test setting the open-circuit malfunction.
    tArticle->setMalfOpenCircuit(true);
    CPPUNIT_ASSERT(true == tArticle->mMalfOpenCircuit);

    /// @test SOC remains constant with forward current.
    const double dt          =   0.1;
    double       current     = 100.0;
    double       expectedSoc = tSoc;
    double       expectedR   = 1.0 / DBL_EPSILON;
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSoc, tArticle->mSoc,                              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR,   tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);

    /// @test SOC remains constant with reverse current.
    current = -100.0;
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSoc, tArticle->mSoc,                              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR,   tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);

    /// @test resetting the open-circuit malfunction.
    tArticle->setMalfOpenCircuit();
    CPPUNIT_ASSERT(false == tArticle->mMalfOpenCircuit);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test updates with the short-circuit malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::testUpdateMalfShort()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// @test setting the short-circuit malfunction.
    tArticle->setMalfShortCircuit(true);
    CPPUNIT_ASSERT(true == tArticle->mMalfShortCircuit);

    /// @test SOC goes to zero.
    const double dt          =   0.1;
    double       current     = 100.0;
    double       expectedR   = DBL_EPSILON;
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->mSoc,                              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR, tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);

    /// @test resetting the short-circuit malfunction.
    tArticle->setMalfShortCircuit();
    CPPUNIT_ASSERT(false == tArticle->mMalfShortCircuit);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test updates with the both the open-circuit and short-circuit malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::testUpdateMalfBoth()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// - Set the open-circuit and short-circuit malfunctions.
    tArticle->setMalfOpenCircuit(true);
    tArticle->setMalfShortCircuit(true);

    /// @test SOC goes to zero.
    const double dt          =   0.1;
    double       current     = 100.0;
    double       expectedR   = DBL_EPSILON;
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->mSoc,                              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR, tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test updates with the capacity override malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::testUpdateMalfCapacity()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// - Set the capacity malfunction.
    tArticle->setMalfCapacity(true, 16.0);

    /// @test SOC change affected by capacity.
    const double dt          =   0.1;
    double       current     = 100.0;
    double       expectedSoc = tSoc - current * dt / 16.0 / 3600.0;
    double       expectedR   = tResistance;
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSoc, tArticle->mSoc,                              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSoc, tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR,   tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSoc, tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);
    CPPUNIT_ASSERT(16.0 == tArticle->getEffectiveCapacity());

    /// @test resetting the malfunction.
    tArticle->setMalfCapacity();
    CPPUNIT_ASSERT(false        == tArticle->mMalfCapacityFlag);
    CPPUNIT_ASSERT(0.0          == tArticle->mMalfCapacityValue);
    CPPUNIT_ASSERT(tMaxCapacity == tArticle->getEffectiveCapacity());

    /// @test capacity affected by short and open malfunctions.
    tArticle->setMalfOpenCircuit(true);
    CPPUNIT_ASSERT(0.0 == tArticle->getEffectiveCapacity());
    tArticle->setMalfOpenCircuit();

    tArticle->setMalfShortCircuit(true);
    CPPUNIT_ASSERT(0.0 == tArticle->getEffectiveCapacity());
    tArticle->setMalfShortCircuit();

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test updates with the thermal runaway malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectBatteryCell::testUpdateMalfRunaway()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    double voltage = tArticle->getEffectiveVoltage(tSocVocTable);

    /// - Set the thermal runaway malfunction.
    const double duration = 10.0;
    tArticle->setMalfThermalRunaway(true, duration);

    /// @test single update to start the thermal runaway.
    const double dt              = 0.1;
    double       current         = 100.0;
    const double expectedPwrRate = 2.0 * tSoc * tMaxCapacity * voltage * 3600.0 / duration / duration;
    double       expectedPwr     = dt * expectedPwrRate;
    const double expectedR       = 1.0 / DBL_EPSILON;
    double       expectedIrun    = expectedPwr / voltage;
    double       expectedSoc     = tSoc - (current + expectedIrun) * dt / tMaxCapacity / 3600.0;
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPwr,     tArticle->mRunawayPower,                     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPwrRate, tArticle->mRunawayPowerRate,                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSoc,     tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR,       tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);

    /// @test second update with increasing power trend.  Current will now be zero because
    ///       the cell is acting like open-circuit.
    current      = 0.0;
    expectedPwr += dt * expectedPwrRate;
    voltage      = tSocVocTable->get(expectedSoc);
    expectedIrun = expectedPwr / voltage;
    expectedSoc -= (current + expectedIrun) * dt / tMaxCapacity / 3600.0;
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPwr,     tArticle->mRunawayPower,                     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPwrRate, tArticle->mRunawayPowerRate,                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSoc,     tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR,       tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);

    /// @test resetting the malfunction mid-duration.
    tArticle->setMalfThermalRunaway();
    CPPUNIT_ASSERT(false == tArticle->mMalfThermalRunawayFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfThermalRunawayDuration);
    voltage = tSocVocTable->get(expectedSoc);
    tArticle->updateSoc(current, dt, tSocVocTable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         tArticle->mRunawayPower,                     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         tArticle->mRunawayPowerRate,                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSoc, tArticle->getEffectiveSoc(),                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tResistance, tArticle->getEffectiveResistance(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(voltage,     tArticle->getEffectiveVoltage(tSocVocTable), DBL_EPSILON);

    /// @test resume malfunction and loop updates to the end of the malfunction duration.
    ///       This cell runs out of juice well before the given duration, because the duration
    ///       logic assumes voltage is constant.  This cell has a steep voltage drop v. SoC,
    ///       which throws off the duration a lot.
    tArticle->setMalfThermalRunaway(true, duration);
    for (unsigned int step=0; step<100; step++) {
        tArticle->updateSoc(current, dt, tSocVocTable);
    }
    CPPUNIT_ASSERT(0.0 == tArticle->getEffectiveSoc());
    CPPUNIT_ASSERT(0.0 == tArticle->mRunawayPower);
    CPPUNIT_ASSERT(0.0 == tArticle->mRunawayPowerRate);

    UT_PASS_LAST;
}
