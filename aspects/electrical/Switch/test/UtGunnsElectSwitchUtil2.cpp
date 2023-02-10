/*
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/
#include <iostream>
#include "UtGunnsElectSwitchUtil2.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsElectSwitchUtil2::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor for the test object.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectSwitchUtil2::UtGunnsElectSwitchUtil2()
    :
    tName(""),
    tResistance(0.0),
    tTripPriority(0),
    tPosition(false),
    tPositionCommand(false),
    tResetTripsCommand(false),
    tInputUnderVoltageTripLimit(0.0),
    tInputUnderVoltageTripReset(0.0),
    tInputOverVoltageTripLimit(0.0),
    tInputOverVoltageTripReset(0.0),
    tPosOverCurrentTripLimit(0.0),
    tNegOverCurrentTripLimit(0.0),
    tConfigData(),
    tInputData(),
    tArticle()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor for the test object.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectSwitchUtil2::~UtGunnsElectSwitchUtil2()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Create and assign data for each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::setUp()
{
    tName = "tArticle";

    /// - Create nominal config data.
    tResistance   = 0.0668;
    tTripPriority = 3;

    tConfigData = new GunnsElectSwitchUtil2ConfigData(tResistance, tTripPriority);

    /// - Create nominal input data.
    tPosition                   = true;
    tPositionCommand            = true;
    tResetTripsCommand          = true;
    tInputUnderVoltageTripLimit =  11.0;
    tInputUnderVoltageTripReset =  13.0;
    tInputOverVoltageTripLimit  =  14.0;
    tInputOverVoltageTripReset  =  12.0;
    tPosOverCurrentTripLimit    =  10.0;
    tNegOverCurrentTripLimit    = -10.0;

    tInputData = new GunnsElectSwitchUtil2InputData(tPosition,
                                                    tPositionCommand,
                                                    tResetTripsCommand,
                                                    tInputUnderVoltageTripLimit,
                                                    tInputUnderVoltageTripReset,
                                                    tInputOverVoltageTripLimit,
                                                    tInputOverVoltageTripReset,
                                                    tPosOverCurrentTripLimit,
                                                    tNegOverCurrentTripLimit);

    /// - Create the test article.
    tArticle = new FriendlyGunnsElectSwitchUtil2();

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Clean up variables after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::tearDown()
{
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::testConfigData()
{
    UT_RESULT_FIRST;

    /// @test    Nominal construction.
    CPPUNIT_ASSERT(tResistance   == tConfigData->mResistance);
    CPPUNIT_ASSERT(tTripPriority == tConfigData->mTripPriority);

    /// @test    Copy construction.
    GunnsElectSwitchUtil2ConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tResistance   == copyConfig.mResistance);
    CPPUNIT_ASSERT(tTripPriority == copyConfig.mTripPriority);

    /// @test    Default construction.
    GunnsElectSwitchUtil2ConfigData defaultConfig;
    CPPUNIT_ASSERT(0.0 == defaultConfig.mResistance);
    CPPUNIT_ASSERT(0   == defaultConfig.mTripPriority);

    /// @test    Assignment operator.
    GunnsElectSwitchUtil2ConfigData assignConfig;
    assignConfig = copyConfig;
    CPPUNIT_ASSERT(tResistance   == assignConfig.mResistance);
    CPPUNIT_ASSERT(tTripPriority == assignConfig.mTripPriority);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::testInputData()
{
    UT_RESULT;

    /// @test    Nominal construction.
    CPPUNIT_ASSERT(tPosition                   == tInputData->mPosition);
    CPPUNIT_ASSERT(tPositionCommand            == tInputData->mPositionCommand);
    CPPUNIT_ASSERT(tResetTripsCommand          == tInputData->mResetTripsCommand);
    CPPUNIT_ASSERT(tInputUnderVoltageTripLimit == tInputData->mInputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(tInputUnderVoltageTripReset == tInputData->mInputUnderVoltageTripReset);
    CPPUNIT_ASSERT(tInputOverVoltageTripLimit  == tInputData->mInputOverVoltageTripLimit);
    CPPUNIT_ASSERT(tInputOverVoltageTripReset  == tInputData->mInputOverVoltageTripReset);
    CPPUNIT_ASSERT(tPosOverCurrentTripLimit    == tInputData->mPosOverCurrentTripLimit);
    CPPUNIT_ASSERT(tNegOverCurrentTripLimit    == tInputData->mNegOverCurrentTripLimit);

    /// @test    Copy construction.
    GunnsElectSwitchUtil2InputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tPosition                   == copyInput.mPosition);
    CPPUNIT_ASSERT(tPositionCommand            == copyInput.mPositionCommand);
    CPPUNIT_ASSERT(tResetTripsCommand          == copyInput.mResetTripsCommand);
    CPPUNIT_ASSERT(tInputUnderVoltageTripLimit == copyInput.mInputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(tInputUnderVoltageTripReset == copyInput.mInputUnderVoltageTripReset);
    CPPUNIT_ASSERT(tInputOverVoltageTripLimit  == copyInput.mInputOverVoltageTripLimit);
    CPPUNIT_ASSERT(tInputOverVoltageTripReset  == copyInput.mInputOverVoltageTripReset);
    CPPUNIT_ASSERT(tPosOverCurrentTripLimit    == copyInput.mPosOverCurrentTripLimit);
    CPPUNIT_ASSERT(tNegOverCurrentTripLimit    == copyInput.mNegOverCurrentTripLimit);

    /// @test    Default construction.
    GunnsElectSwitchUtil2InputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mPosition);
    CPPUNIT_ASSERT(false == defaultInput.mPositionCommand);
    CPPUNIT_ASSERT(false == defaultInput.mResetTripsCommand);
    CPPUNIT_ASSERT(0.0   == defaultInput.mInputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(0.0   == defaultInput.mInputUnderVoltageTripReset);
    CPPUNIT_ASSERT(0.0   == defaultInput.mInputOverVoltageTripLimit);
    CPPUNIT_ASSERT(0.0   == defaultInput.mInputOverVoltageTripReset);
    CPPUNIT_ASSERT(0.0   == defaultInput.mPosOverCurrentTripLimit);
    CPPUNIT_ASSERT(0.0   == defaultInput.mNegOverCurrentTripLimit);

    /// @test    Assignment operator.
    GunnsElectSwitchUtil2InputData assignInput;
    assignInput = copyInput;
    CPPUNIT_ASSERT(tPosition                   == assignInput.mPosition);
    CPPUNIT_ASSERT(tPositionCommand            == assignInput.mPositionCommand);
    CPPUNIT_ASSERT(tResetTripsCommand          == assignInput.mResetTripsCommand);
    CPPUNIT_ASSERT(tInputUnderVoltageTripLimit == assignInput.mInputUnderVoltageTripLimit);
    CPPUNIT_ASSERT(tInputUnderVoltageTripReset == assignInput.mInputUnderVoltageTripReset);
    CPPUNIT_ASSERT(tInputOverVoltageTripLimit  == assignInput.mInputOverVoltageTripLimit);
    CPPUNIT_ASSERT(tInputOverVoltageTripReset  == assignInput.mInputOverVoltageTripReset);
    CPPUNIT_ASSERT(tPosOverCurrentTripLimit    == assignInput.mPosOverCurrentTripLimit);
    CPPUNIT_ASSERT(tNegOverCurrentTripLimit    == assignInput.mNegOverCurrentTripLimit);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::testDefaultConstruction()
{
    UT_RESULT;

    FriendlyGunnsElectSwitchUtil2 article;
    CPPUNIT_ASSERT(false == article.mMalfFailClosed);
    CPPUNIT_ASSERT(false == article.mMalfFailOpen);
    CPPUNIT_ASSERT(false == article.mResistance);
    CPPUNIT_ASSERT(false == article.mPosition);
    CPPUNIT_ASSERT(false == article.mPositionCommand);
    CPPUNIT_ASSERT(false == article.mResetTripsCommand);
    CPPUNIT_ASSERT(0.0   == article.mInputUnderVoltageTrip.getLimit());
    CPPUNIT_ASSERT(0.0   == article.mInputUnderVoltageReset.getLimit());
    CPPUNIT_ASSERT(0.0   == article.mInputOverVoltageTrip.getLimit());
    CPPUNIT_ASSERT(0.0   == article.mInputOverVoltageReset.getLimit());
    CPPUNIT_ASSERT(0.0   == article.mPosOverCurrentTrip.getLimit());
    CPPUNIT_ASSERT(0.0   == article.mNegOverCurrentTrip.getLimit());
    CPPUNIT_ASSERT(false == article.mWaitingToTrip);
    CPPUNIT_ASSERT(false == article.mJustTripped);
    CPPUNIT_ASSERT(""    == article.mName);
    CPPUNIT_ASSERT(false == article.mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsElectSwitchUtil2* article2 = new GunnsElectSwitchUtil2;
    delete article2;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for nominal initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::testInitialization()
{
    UT_RESULT;

    /// - Initialize the test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tName));

    /// @test   Initialized state.
    CPPUNIT_ASSERT(false                       == tArticle->mMalfFailClosed);
    CPPUNIT_ASSERT(false                       == tArticle->mMalfFailOpen);
    CPPUNIT_ASSERT(tResistance                 == tArticle->mResistance);
    CPPUNIT_ASSERT(tPosition                   == tArticle->mPosition);
    CPPUNIT_ASSERT(tPositionCommand            == tArticle->mPositionCommand);
    CPPUNIT_ASSERT(tResetTripsCommand          == tArticle->mResetTripsCommand);
    CPPUNIT_ASSERT(tInputUnderVoltageTripLimit == tArticle->mInputUnderVoltageTrip.getLimit());
    CPPUNIT_ASSERT(false                       == tArticle->mInputUnderVoltageTrip.isTripped());
    CPPUNIT_ASSERT(tInputUnderVoltageTripReset == tArticle->mInputUnderVoltageReset.getLimit());
    CPPUNIT_ASSERT(false                       == tArticle->mInputUnderVoltageReset.isTripped());
    CPPUNIT_ASSERT(tInputOverVoltageTripLimit  == tArticle->mInputOverVoltageTrip.getLimit());
    CPPUNIT_ASSERT(false                       == tArticle->mInputOverVoltageTrip.isTripped());
    CPPUNIT_ASSERT(tInputOverVoltageTripReset  == tArticle->mInputOverVoltageReset.getLimit());
    CPPUNIT_ASSERT(false                       == tArticle->mInputOverVoltageReset.isTripped());
    CPPUNIT_ASSERT(tPosOverCurrentTripLimit    == tArticle->mPosOverCurrentTrip.getLimit());
    CPPUNIT_ASSERT(false                       == tArticle->mPosOverCurrentTrip.isTripped());
    CPPUNIT_ASSERT(tNegOverCurrentTripLimit    == tArticle->mNegOverCurrentTrip.getLimit());
    CPPUNIT_ASSERT(false                       == tArticle->mNegOverCurrentTrip.isTripped());
    CPPUNIT_ASSERT(false                       == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(false                       == tArticle->mJustTripped);
    CPPUNIT_ASSERT(tName                       == tArticle->mName);
    CPPUNIT_ASSERT(true                        == tArticle->mInitFlag);
    CPPUNIT_ASSERT(true                        == tArticle->isInitialized());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for errors in initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::testInitializationErrors()
{
    UT_RESULT;

    /// @test    Error thrown for invalid resistance value.
    tConfigData->mResistance = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tConfigData->mResistance = tResistance;

    /// @test    Error thrown for IUV trip reset less than trip value.
    tInputData->mInputUnderVoltageTripReset = tInputUnderVoltageTripLimit - 0.001;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tInputData->mInputUnderVoltageTripReset = tInputUnderVoltageTripReset;

    /// @test    Error thrown for IOV trip reset greater than trip value.
    tInputData->mInputOverVoltageTripReset = tInputOverVoltageTripLimit + 0.001;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tInputData->mInputOverVoltageTripReset = tInputOverVoltageTripReset;

    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    CPPUNIT_ASSERT(false == tArticle->isInitialized());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the appllyConstraints method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::testInputConstraints()
{
    UT_RESULT;

    /// - Initialize the test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// @test    constraint on resistance is applied.
    tArticle->mResistance = 0.0;
    tArticle->updateState();
    CPPUNIT_ASSERT(FLT_EPSILON == tArticle->mResistance);

    /// @test    constraint on input under-volt trip reset value is applied.
    {
        const float expectedLimit = tArticle->mInputUnderVoltageTrip.getLimit();
        tArticle->mInputUnderVoltageReset.setLimit(expectedLimit - 0.001);
        tArticle->updateState();
        CPPUNIT_ASSERT(expectedLimit == tArticle->mInputUnderVoltageReset.getLimit());
    }

    /// @test    constraint on input over-volt trip reset value is applied.
    {
        const float expectedLimit = tArticle->mInputOverVoltageTrip.getLimit();
        tArticle->mInputOverVoltageReset.setLimit(expectedLimit + 0.001);
        tArticle->updateState();
        CPPUNIT_ASSERT(expectedLimit == tArticle->mInputOverVoltageReset.getLimit());
    }

    /// - Call applyConstraints for code coverage - its behavior has been tested above.
    tArticle->applyConstraints();

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the updateState method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::testUpdateState()
{
    UT_RESULT;

    /// - Initialize the test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// - Set the trips.
    GunnsBasicLink::SolutionResult result = GunnsBasicLink::CONFIRM;
    CPPUNIT_ASSERT(true == tArticle->mInputUnderVoltageTrip .checkForTrip(result, tInputUnderVoltageTripLimit - 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mInputUnderVoltageReset.checkForTrip(result, tInputUnderVoltageTripReset + 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mInputOverVoltageTrip  .checkForTrip(result, tInputOverVoltageTripLimit  + 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mInputOverVoltageReset .checkForTrip(result, tInputOverVoltageTripReset  - 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mPosOverCurrentTrip    .checkForTrip(result, tPosOverCurrentTripLimit    + 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mNegOverCurrentTrip    .checkForTrip(result, tNegOverCurrentTripLimit    - 0.001, tTripPriority));

    /// @test    Tripped switch remains tripped if commanded closed, and stays open when tripped
    ///          and commanded closed.
    tArticle->setPositionCommand(true);
    tArticle->mPosition          = false;
    tArticle->mResetTripsCommand = false;
    tArticle->updateState();
    CPPUNIT_ASSERT(true  == tArticle->mInputUnderVoltageTrip.isTripped());
    CPPUNIT_ASSERT(true  == tArticle->getPositionCommand());
    CPPUNIT_ASSERT(false == tArticle->mPosition);

    /// @test    Trips are reset if commanded open.
    tArticle->mJustTripped     = true;
    tArticle->mWaitingToTrip   = true;
    tArticle->mPositionCommand = false;
    tArticle->updateState();
    CPPUNIT_ASSERT(false == tArticle->mInputUnderVoltageTrip .isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputUnderVoltageReset.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputOverVoltageTrip  .isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputOverVoltageReset .isTripped());
    CPPUNIT_ASSERT(false == tArticle->mPosOverCurrentTrip    .isTripped());
    CPPUNIT_ASSERT(false == tArticle->mNegOverCurrentTrip    .isTripped());
    CPPUNIT_ASSERT(false == tArticle->mPosition);
    CPPUNIT_ASSERT(false == tArticle->mJustTripped);
    CPPUNIT_ASSERT(false == tArticle->mWaitingToTrip);

    /// - Set the trips.
    CPPUNIT_ASSERT(true == tArticle->mInputUnderVoltageTrip .checkForTrip(result, tInputUnderVoltageTripLimit - 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mInputUnderVoltageReset.checkForTrip(result, tInputUnderVoltageTripReset + 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mInputOverVoltageTrip  .checkForTrip(result, tInputOverVoltageTripLimit  + 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mInputOverVoltageReset .checkForTrip(result, tInputOverVoltageTripReset  - 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mPosOverCurrentTrip    .checkForTrip(result, tPosOverCurrentTripLimit    + 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mNegOverCurrentTrip    .checkForTrip(result, tNegOverCurrentTripLimit    - 0.001, tTripPriority));

    /// @test    Trips are reset if commanded to reset.
    tArticle->mJustTripped       = true;
    tArticle->mWaitingToTrip     = true;
    tArticle->mPositionCommand   = true;
    tArticle->mResetTripsCommand = true;
    tArticle->updateState();
    CPPUNIT_ASSERT(false == tArticle->mInputUnderVoltageTrip .isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputUnderVoltageReset.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputOverVoltageTrip  .isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInputOverVoltageReset .isTripped());
    CPPUNIT_ASSERT(false == tArticle->mPosOverCurrentTrip    .isTripped());
    CPPUNIT_ASSERT(false == tArticle->mNegOverCurrentTrip    .isTripped());
    CPPUNIT_ASSERT(false == tArticle->mResetTripsCommand);
    CPPUNIT_ASSERT(false == tArticle->mJustTripped);
    CPPUNIT_ASSERT(false == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(true  == tArticle->mPosition);

    /// @test    Position malfunctions override the commanded position.
    tArticle->mMalfFailOpen = true;
    tArticle->updateState();
    CPPUNIT_ASSERT(false == tArticle->mPosition);

    /// @test    getResistance function.
    double expectedR = 1.0 / DBL_EPSILON;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR, tArticle->getResistance(), DBL_EPSILON);

    tArticle->mMalfFailClosed = true;
    tArticle->updateState();
    CPPUNIT_ASSERT(true == tArticle->mPosition);

    tArticle->mMalfFailOpen = false;
    tArticle->updateState();
    CPPUNIT_ASSERT(true == tArticle->mPosition);

    /// - Set the trips.
    CPPUNIT_ASSERT(true == tArticle->mInputUnderVoltageTrip .checkForTrip(result, tInputUnderVoltageTripLimit - 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mInputUnderVoltageReset.checkForTrip(result, tInputUnderVoltageTripReset + 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mInputOverVoltageTrip  .checkForTrip(result, tInputOverVoltageTripLimit  + 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mInputOverVoltageReset .checkForTrip(result, tInputOverVoltageTripReset  - 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mPosOverCurrentTrip    .checkForTrip(result, tPosOverCurrentTripLimit    + 0.001, tTripPriority));
    CPPUNIT_ASSERT(true == tArticle->mNegOverCurrentTrip    .checkForTrip(result, tNegOverCurrentTripLimit    - 0.001, tTripPriority));

    /// @test    Fail closed malf overrides trips.
    tArticle->updateState();
    CPPUNIT_ASSERT(true == tArticle->mPosition);

    /// @test    getResistance function.
    expectedR = tResistance;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR, tArticle->getResistance(), DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the updateTrips method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSwitchUtil2::testUpdateTrips()
{
    UT_RESULT;

    /// - Initialize the test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tName);

    /// @test    no trips for any trip condition if not converged.
    tArticle->updateTrips(tPosOverCurrentTripLimit + 1.0, tInputOverVoltageTripLimit + 1.0, 0);

    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(false == tArticle->mJustTripped);
    CPPUNIT_ASSERT(true  == tArticle->mPosition);

    /// @test    no trips if failed closed.
    tArticle->setMalfFailClosed(true);
    tArticle->updateTrips(tPosOverCurrentTripLimit + 1.0, tInputOverVoltageTripLimit + 1.0, 3);
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(false == tArticle->mJustTripped);
    CPPUNIT_ASSERT(true  == tArticle->mPosition);

    /// @test    waiting to trip for any trip condition if not on trip priority step.
    tArticle->setMalfFailClosed();
    tArticle->updateTrips(tPosOverCurrentTripLimit + 1.0, tInputOverVoltageTripLimit + 1.0, 2);
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(true  == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(false == tArticle->mJustTripped);
    CPPUNIT_ASSERT(true  == tArticle->mPosition);

    /// @test    +OC trip.
    tArticle->updateTrips(tPosOverCurrentTripLimit + 1.0, tInputOverVoltageTripReset - 1.0, 3);
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(true  == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(true  == tArticle->mJustTripped);
    CPPUNIT_ASSERT(false == tArticle->mPosition);

    /// @test    -OC trip.
    tArticle->resetTrips();
    tArticle->updateTrips(tNegOverCurrentTripLimit - 1.0, tInputOverVoltageTripReset - 1.0, 3);
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(true  == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(true  == tArticle->mJustTripped);
    CPPUNIT_ASSERT(false == tArticle->mPosition);

    /// @test    IUV trip.
    tArticle->resetTrips();
    tArticle->updateTrips(tPosOverCurrentTripLimit - 1.0, tInputUnderVoltageTripLimit - 1.0, 3);
    CPPUNIT_ASSERT(true == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(true  == tArticle->mJustTripped);
    CPPUNIT_ASSERT(false == tArticle->mPosition);

    /// @test    No trip resets if failed open.
    tArticle->setMalfFailOpen(true);
    tArticle->updateTrips(tPosOverCurrentTripLimit - 1.0, tInputUnderVoltageTripReset + 1.0, 3);
    CPPUNIT_ASSERT(true  == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(false == tArticle->mJustTripped);
    CPPUNIT_ASSERT(false == tArticle->mPosition);

    /// @test    IUV trip reset.
    tArticle->setMalfFailOpen();
    tArticle->updateTrips(tPosOverCurrentTripLimit - 1.0, tInputUnderVoltageTripReset + 1.0, 3);
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(true  == tArticle->mJustTripped);
    CPPUNIT_ASSERT(true  == tArticle->mPosition);

    /// @test    IOV trip simo with +OC trip.
    tArticle->resetTrips();
    tArticle->updateTrips(tPosOverCurrentTripLimit + 1.0, tInputOverVoltageTripLimit + 1.0, 3);
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(true  == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(true  == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(true  == tArticle->mJustTripped);
    CPPUNIT_ASSERT(false == tArticle->mPosition);

    /// @test    IOV trip reset prevented by present +OC trip.
    tArticle->updateTrips(tPosOverCurrentTripLimit + 1.0, tInputOverVoltageTripReset - 1.0, 3);
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(true  == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(true  == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(false == tArticle->mJustTripped);
    CPPUNIT_ASSERT(false == tArticle->mPosition);

    /// @test    IOV trip.
    tArticle->resetTrips();
    tArticle->updateTrips(tPosOverCurrentTripLimit - 1.0, tInputOverVoltageTripLimit + 1.0, 3);
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(true  == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->mWaitingToTrip);
    CPPUNIT_ASSERT(true  == tArticle->mJustTripped);
    CPPUNIT_ASSERT(false == tArticle->mPosition);

    /// @test     waiting to IOV trip reset.
    tArticle->updateTrips(tPosOverCurrentTripLimit - 1.0, tInputOverVoltageTripReset - 1.0, 2);
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(true  == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(true  == tArticle->isWaitingToTrip());
    CPPUNIT_ASSERT(false == tArticle->hasJustTripped());
    CPPUNIT_ASSERT(false == tArticle->getPosition());

    /// @test     IOV trip reset.
    tArticle->updateTrips(tPosOverCurrentTripLimit - 1.0, tInputOverVoltageTripReset - 1.0, 3);
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputUnderVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getInputOverVoltageReset()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getPosOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->getNegOverCurrentTrip()->isTripped());
    CPPUNIT_ASSERT(false == tArticle->isWaitingToTrip());
    CPPUNIT_ASSERT(true  == tArticle->hasJustTripped());
    CPPUNIT_ASSERT(true  == tArticle->getPosition());

    UT_PASS_LAST;
}
