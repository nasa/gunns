/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "UtGunnsElectResistiveUserLoad.hh"

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsElectResistiveUserLoad::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Electrical Resistive User Load Spotter link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectResistiveUserLoad::UtGunnsElectResistiveUserLoad()
    :
    CppUnit::TestFixture(),
    tName(""),
    tUnderVoltageLimit(0.0),
    tResistanceNormal(0.0),
    tResistanceStandby(0.0),
    tFuseCurrentLimit(0.0),
    tDutyCycleFraction(0.0),
    tDutyCyclePeriod(0.0),
    tConfigData(0),
    tInitialMode(0),
    tInitialVoltage(0.0),
    tDutyCycleTimer(0.0),
    tInputData(0),
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical Resistive User Load Spotter link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectResistiveUserLoad::~UtGunnsElectResistiveUserLoad()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsElectResistiveUserLoad::setUp()
{
    /// - Define the nominal configuration data.
    tName               = "nominal";
    tUnderVoltageLimit  = 100.0;
    tResistanceNormal   = 40.0;
    tResistanceStandby  = 500.0;
    tFuseCurrentLimit   = 3.0;
    tDutyCycleFraction  = 0.9;
    tDutyCyclePeriod    = 10.0;
    tConfigData         = new GunnsElectResistiveUserLoadConfigData(tName,
                                                                    tUnderVoltageLimit,
                                                                    tResistanceNormal,
                                                                    tResistanceStandby,
                                                                    tFuseCurrentLimit,
                                                                    tDutyCycleFraction,
                                                                    tDutyCyclePeriod);

    /// - Define the nominal input data.
    tInitialMode        = 1;
    tInitialVoltage     = 120.0;
    tDutyCycleTimer     = 8.5;
    tInputData          = new GunnsElectResistiveUserLoadInputData(tInitialMode,
                                                                   tInitialVoltage,
                                                                   tDutyCycleTimer);

    /// - Default construct the nominal test article.
    tArticle            = new FriendlyGunnsElectResistiveUserLoad(tConfigData, tInputData);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectResistiveUserLoad::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Resistive User Load Spotter construction of configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectResistiveUserLoad::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName              == tConfigData->mName);
    CPPUNIT_ASSERT(tUnderVoltageLimit == tConfigData->mUnderVoltageLimit);
    CPPUNIT_ASSERT(tResistanceNormal  == tConfigData->mResistanceNormal);
    CPPUNIT_ASSERT(tResistanceStandby == tConfigData->mResistanceStandby);
    CPPUNIT_ASSERT(tFuseCurrentLimit  == tConfigData->mFuseCurrentLimit);
    CPPUNIT_ASSERT(tDutyCycleFraction == tConfigData->mDutyCycleFraction);
    CPPUNIT_ASSERT(tDutyCyclePeriod   == tConfigData->mDutyCyclePeriod);

    /// @test    Configuration data default construction.
    GunnsElectResistiveUserLoadConfigData defaultConfig;
    CPPUNIT_ASSERT(""  == defaultConfig.mName);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mUnderVoltageLimit);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mResistanceNormal);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mResistanceStandby);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mFuseCurrentLimit);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mDutyCycleFraction);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mDutyCyclePeriod);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Resistive User Load Spotter construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectResistiveUserLoad::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tInitialMode    == tInputData->mInitialMode);
    CPPUNIT_ASSERT(tInitialVoltage == tInputData->mInitialVoltage);
    CPPUNIT_ASSERT(tDutyCycleTimer == tInputData->mDutyCycleTimer);

    /// @test    Input data default construction.
    GunnsElectResistiveUserLoadInputData defaultInput;
    CPPUNIT_ASSERT(0   == defaultInput.mInitialMode);
    CPPUNIT_ASSERT(0.0 == defaultInput.mInitialVoltage);
    CPPUNIT_ASSERT(0.0 == defaultInput.mDutyCycleTimer);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Resistive User Load Spotter default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectResistiveUserLoad::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction of attributes.
    FriendlyGunnsElectResistiveUserLoad* article = new FriendlyGunnsElectResistiveUserLoad();
    CPPUNIT_ASSERT(""   == article->mName);
    CPPUNIT_ASSERT(0.0  == article->mSupplyVoltage);
    CPPUNIT_ASSERT(0    == article->mConfig);
    CPPUNIT_ASSERT(0    == article->mInput);
    CPPUNIT_ASSERT(!article->mLoad.isInitialized());

    /// @test    Default construction of initialization flag.
    CPPUNIT_ASSERT(!article->isInitialized());

    /// @test    New/delete for code coverage.
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Resistive User Load Spotter nominal construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectResistiveUserLoad::testNominalConstruction()
{
    UT_RESULT;

    /// @test    Nominal construction of attributes.
    CPPUNIT_ASSERT(""          == tArticle->mName);
    CPPUNIT_ASSERT(0.0         == tArticle->mSupplyVoltage);
    CPPUNIT_ASSERT(tConfigData == tArticle->mConfig);
    CPPUNIT_ASSERT(tInputData  == tArticle->mInput);
    CPPUNIT_ASSERT(!tArticle->mLoad.isInitialized());

    /// @test    Default construction of initialization flag.
    CPPUNIT_ASSERT(!tArticle->isInitialized());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Resistive User Load Spotter initialization method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectResistiveUserLoad::testNominalInitialization()
{
    UT_RESULT;

    /// @test    Nominal initialization of config & input data.
    FriendlyGunnsElectResistiveUserLoad article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(tConfigData, tInputData));
    CPPUNIT_ASSERT(0.0         == article.mSupplyVoltage);
    CPPUNIT_ASSERT(tConfigData == article.mConfig);
    CPPUNIT_ASSERT(tInputData  == article.mInput);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Resistive User Load Spotter nominal initLoad method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectResistiveUserLoad::testNominalInitLoad()
{
    UT_RESULT;

    /// @test    Nominal initialization of spotter and user load.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initLoad());
    CPPUNIT_ASSERT(tConfigData     == tArticle->mConfig);
    CPPUNIT_ASSERT(tInputData      == tArticle->mInput);
    CPPUNIT_ASSERT(tName           == tArticle->mName);
    CPPUNIT_ASSERT(tInitialVoltage == tArticle->mSupplyVoltage);
    CPPUNIT_ASSERT(tArticle->mLoad.isInitialized());
    CPPUNIT_ASSERT(tArticle->mLoad.getPowerValid());
    CPPUNIT_ASSERT(tArticle->isInitialized());

    /// @test    Double initialization since this is likely to happen often.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initLoad());
    CPPUNIT_ASSERT(tConfigData     == tArticle->mConfig);
    CPPUNIT_ASSERT(tInputData      == tArticle->mInput);
    CPPUNIT_ASSERT(tName           == tArticle->mName);
    CPPUNIT_ASSERT(tInitialVoltage == tArticle->mSupplyVoltage);
    CPPUNIT_ASSERT(tArticle->mLoad.isInitialized());
    CPPUNIT_ASSERT(tArticle->mLoad.getPowerValid());
    CPPUNIT_ASSERT(tArticle->isInitialized());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Resistive User Load Spotter access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectResistiveUserLoad::testAccessors()
{
    UT_RESULT;

    /// @test    getLoad method.
    CPPUNIT_ASSERT(&tArticle->mLoad == tArticle->getLoad());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Resistive User Load Spotter step methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectResistiveUserLoad::testStep()
{
    UT_RESULT;

    /// @test    Spotter stepPreSolver and stepPostSolver methods do nothing (nothing to verify,
    ///          just call for coverage).
    tArticle->stepPreSolver(0.0);
    tArticle->stepPostSolver(0.0);

    /// @test    step method steps the contained load with the given supply voltage.
    CPPUNIT_ASSERT(tArticle->getLoad()->getPowerValid());
    tArticle->setSupplyVoltage(0.0);
    CPPUNIT_ASSERT(0.0 == tArticle->mSupplyVoltage);
    tArticle->step();
    CPPUNIT_ASSERT(!tArticle->getLoad()->getPowerValid());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Resistive User Load Spotter initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectResistiveUserLoad::testInitializationExceptions()
{
    UT_RESULT;

    /// @test    Exception from bad spotter config & input data types.
    BadGunnsElectResistiveUserLoadConfigData badConfig("bad");
    BadGunnsElectResistiveUserLoadInputData badInput;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&badConfig,  tInputData), TsInitializationException);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfigData,  &badInput), TsInitializationException);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&badConfig,   &badInput), TsInitializationException);

    /// @test    Exception thrown from user load initialization.
    tConfigData->mResistanceNormal = -10.0;
    tArticle->initialize(tConfigData, tInputData);
    CPPUNIT_ASSERT_THROW(tArticle->initLoad(), TsInitializationException);
    CPPUNIT_ASSERT(!tArticle->isInitialized());

    /// @test    Exception thrown from duty cycle fraction < 0.
    tConfigData->mDutyCycleFraction = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initLoad(), TsInitializationException);
    tConfigData->mDutyCycleFraction = 1.1;
    CPPUNIT_ASSERT_THROW(tArticle->initLoad(), TsInitializationException);
    tConfigData->mDutyCycleFraction = tDutyCycleFraction;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Resistive User Load Spotter stepDutyCycle function.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectResistiveUserLoad::testDutyCycle()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initLoad());

    /// @test    step with duty cycle turned off.
    int lastMode = tArticle->getLoad()->getLoadOperMode();
    tArticle->mDutyCyclePeriod = 0.0;
    const double timestep = 0.1;
    tArticle->stepDutyCycle(0.1);
    CPPUNIT_ASSERT(tDutyCycleTimer == tArticle->mDutyCycleTimer);
    CPPUNIT_ASSERT(lastMode        == tArticle->getLoad()->getLoadOperMode());

    /// @test    step with duty cycle on, load results in on.
    tArticle->mDutyCyclePeriod = tDutyCyclePeriod;
    double expectedTimer = tDutyCycleTimer + timestep;
    int    expectedMode = static_cast<int>(LoadON);
    tArticle->stepDutyCycle(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTimer, tArticle->mDutyCycleTimer, DBL_EPSILON);
    CPPUNIT_ASSERT(expectedMode == tArticle->getLoad()->getLoadOperMode());

    /// @test    step several more times to get into the off portion of the cycle.
    for (unsigned int steps = 0; steps < 5; steps++) {
        tArticle->stepDutyCycle(timestep);
    }
    expectedMode = static_cast<int>(LoadOFF);
    CPPUNIT_ASSERT(expectedMode == tArticle->getLoad()->getLoadOperMode());

    /// @test    step several times to wrap around back to the start of the cycle.
    for (unsigned int steps = 0; steps < 10; steps++) {
        tArticle->stepDutyCycle(timestep);
    }
    expectedMode  = static_cast<int>(LoadON);
    expectedTimer = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTimer, tArticle->mDutyCycleTimer, DBL_EPSILON);
    CPPUNIT_ASSERT(expectedMode == tArticle->getLoad()->getLoadOperMode());

    UT_PASS_LAST;
}
