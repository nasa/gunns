/**
@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((aspects/electrical/SolarArray/GunnsElectPvString.o)
   (software/exceptions/TsInitializationException.o))
*/
#include "software/exceptions/TsInitializationException.hh"
#include <iostream>
#include "strings/UtResult.hh"
#include "UtGunnsElectPvString.hh"
#include "math/MsMath.hh"

/// @details  Test identification number.
int UtGunnsElectPvString::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectPvString class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvString::UtGunnsElectPvString()
    :
    tName(""),
    tConfigData(0),
    tInputData(0),
    tArticle(0),
    tCellSurfaceArea(0.0),
    tCellEfficiency(0.0),
    tCellSeriesResistance(0.0),
    tCellShuntResistance(0.0),
    tCellOpenCircuitVoltage(0.0),
    tCellRefTemperature(0.0),
    tCellTemperatureVoltageCoeff(0.0),
    tCellTemperatureCurrentCoeff(0.0),
    tBlockingDiodeVoltageDrop(0.0),
    tBypassDiodeVoltageDrop(0.0),
    tBypassDiodeInterval(0),
    tNumCells(0),
    tPhotoFlux(0.0),
    tSourceExposedFraction(0.0),
    tTemperature(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectPvString class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvString::~UtGunnsElectPvString()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::setUp()
{
    tName = "tArticle";

    /// - Define the nominal configuration data.
    tCellSurfaceArea             =  0.05;
    tCellEfficiency              =  1.0;
    tCellSeriesResistance        =  0.017;
    tCellShuntResistance         =  200.0;
    tCellOpenCircuitVoltage      =  0.6;
    tCellRefTemperature          =  294.0;
    tCellTemperatureVoltageCoeff = -0.003;
    tCellTemperatureCurrentCoeff =  0.00065;
    tBlockingDiodeVoltageDrop    =  0.7;
    tBypassDiodeVoltageDrop      =  0.5;
    tBypassDiodeInterval         =  5;
    tNumCells                    =  20;
    tConfigData                  = new GunnsElectPvStringConfigData(tBlockingDiodeVoltageDrop,
                                                                    tBypassDiodeVoltageDrop,
                                                                    tBypassDiodeInterval,
                                                                    tNumCells,
                                                                    tCellSurfaceArea,
                                                                    tCellEfficiency,
                                                                    tCellSeriesResistance,
                                                                    tCellShuntResistance,
                                                                    tCellOpenCircuitVoltage,
                                                                    tCellRefTemperature,
                                                                    tCellTemperatureVoltageCoeff,
                                                                    tCellTemperatureCurrentCoeff);

    /// - Define the nominal input data.
    tPhotoFlux             = 31.626;
    tSourceExposedFraction = 1.0;
    tTemperature           = 284.0;
    tInputData             = new GunnsElectPvStringInputData(tPhotoFlux,
                                                             tSourceExposedFraction,
                                                             tTemperature);

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsElectPvString(tConfigData, tInputData);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Cell configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testCellConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea,             tConfigData->mCellConfig.mSurfaceArea,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellEfficiency,              tConfigData->mCellConfig.mEfficiency,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSeriesResistance,        tConfigData->mCellConfig.mSeriesResistance,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellShuntResistance,         tConfigData->mCellConfig.mShuntResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellOpenCircuitVoltage,      tConfigData->mCellConfig.mOpenCircuitVoltage,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellRefTemperature,          tConfigData->mCellConfig.mRefTemperature,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureVoltageCoeff, tConfigData->mCellConfig.mTemperatureVoltageCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureCurrentCoeff, tConfigData->mCellConfig.mTemperatureCurrentCoeff, 0.0);

    /// @test    Configuration data default construction.
    GunnsElectPvCellConfigData defaultConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mSurfaceArea,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mEfficiency,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mSeriesResistance,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mShuntResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mOpenCircuitVoltage,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mRefTemperature,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mTemperatureVoltageCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mTemperatureCurrentCoeff, 0.0);

    /// @test    Configuration data assignment operator.
    GunnsElectPvCellConfigData assignConfig;
    assignConfig = tConfigData->mCellConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea,             assignConfig.mSurfaceArea,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellEfficiency,              assignConfig.mEfficiency,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSeriesResistance,        assignConfig.mSeriesResistance,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellShuntResistance,         assignConfig.mShuntResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellOpenCircuitVoltage,      assignConfig.mOpenCircuitVoltage,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellRefTemperature,          assignConfig.mRefTemperature,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureVoltageCoeff, assignConfig.mTemperatureVoltageCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureCurrentCoeff, assignConfig.mTemperatureCurrentCoeff, 0.0);

    /// @test    Configuration data self assign.
    GunnsElectPvCellConfigData* assignConfig2 = &assignConfig;
    assignConfig = *assignConfig2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea, assignConfig.mSurfaceArea, 0.0);

    /// @test    New/delete for code coverage.
    GunnsElectPvCellConfigData* testArticle = new GunnsElectPvCellConfigData();
    delete testArticle;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Cell equivalent circuit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testCellEquivProps()
{
    UT_RESULT;

    /// @test    Cell equivalent data default construction.
    GunnsElectPvCellEquivCircuit cellEqData;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqData.mIL,                0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqData.mRsh,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqData.mRs,                0.0);

    /// @test    Initialization with the object name.
    cellEqData.initialize(0, "cellEqData");
    FriendlyGunnsElectPvCellEquivCircuit* visibleCellEqData = static_cast<FriendlyGunnsElectPvCellEquivCircuit*>(&cellEqData);
    CPPUNIT_ASSERT("cellEqData" == visibleCellEqData->mName);

    /// @test    Empty method calls for code coverage.
    cellEqData.clear();
    cellEqData.update(&cellEqData, 0.0, 0.0);
    cellEqData.derive();
    cellEqData.computeCurrent(0.0);
    cellEqData.computeVoltage(0.0);

    /// @test    New/delete for code coverage.
    GunnsElectPvCellEquivCircuit* testArticle = new GunnsElectPvCellEquivCircuit();
    delete testArticle;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic load state data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testLoadState()
{
    UT_RESULT;

    /// @test    Load state default construction.
    GunnsElectPvLoadState loadState;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, loadState.mConductance, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, loadState.mCurrent,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, loadState.mPower,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, loadState.mVoltage,     0.0);

    /// @test    Load state assignment operator.
    loadState.mConductance = 1.0;
    loadState.mCurrent     = 2.0;
    loadState.mPower       = 3.0;
    loadState.mVoltage     = 4.0;
    GunnsElectPvLoadState assignLoadState;
    assignLoadState = loadState;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(loadState.mConductance, assignLoadState.mConductance, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(loadState.mCurrent,     assignLoadState.mCurrent,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(loadState.mPower,       assignLoadState.mPower,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(loadState.mVoltage,     assignLoadState.mVoltage,     0.0);

    /// @test    Load state self assign.
    GunnsElectPvLoadState* assignLoadState2 = &assignLoadState;
    assignLoadState = *assignLoadState2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(loadState.mPower, assignLoadState.mPower, 0.0);

    /// @test    Clear method call for code coverage.
    loadState.clear();

    /// @test    New/delete for code coverage.
    GunnsElectPvLoadState* testArticle = new GunnsElectPvLoadState();
    delete testArticle;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Photovoltaic String model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testConfig()
{
    UT_RESULT;

    /// @test    Configuration nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockingDiodeVoltageDrop, tConfigData->mBlockingDiodeVoltageDrop, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBypassDiodeVoltageDrop,   tConfigData->mBypassDiodeVoltageDrop,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea,          tConfigData->mCellConfig.mSurfaceArea,  0.0);
    CPPUNIT_ASSERT(tBypassDiodeInterval == tConfigData->mBypassDiodeInterval);
    CPPUNIT_ASSERT(tNumCells            == tConfigData->mNumCells);

    /// @test    Configuration data default construction.
    GunnsElectPvStringConfigData defaultConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mBlockingDiodeVoltageDrop, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mBypassDiodeVoltageDrop,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mCellConfig.mSurfaceArea,  0.0);
    CPPUNIT_ASSERT(0 == defaultConfig.mBypassDiodeInterval);
    CPPUNIT_ASSERT(0 == defaultConfig.mNumCells);

    /// @test    Configuration data assignment operator.
    GunnsElectPvStringConfigData assignConfig;
    assignConfig = *tConfigData;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockingDiodeVoltageDrop, assignConfig.mBlockingDiodeVoltageDrop, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBypassDiodeVoltageDrop,   assignConfig.mBypassDiodeVoltageDrop,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea,          assignConfig.mCellConfig.mSurfaceArea,  0.0);
    CPPUNIT_ASSERT(tBypassDiodeInterval == assignConfig.mBypassDiodeInterval);
    CPPUNIT_ASSERT(tNumCells            == assignConfig.mNumCells);

    /// @test    Configuration data self assign.
    GunnsElectPvStringConfigData* assignConfig2 = &assignConfig;
    assignConfig = *assignConfig2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockingDiodeVoltageDrop, assignConfig.mBlockingDiodeVoltageDrop, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Photovoltaic String model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPhotoFlux,             tInputData->mPhotoFlux,                0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceExposedFraction, tInputData->mSourceExposedFraction,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperature,           tInputData->mTemperature,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    tInputData->mMalfPhotoFluxMagnitude,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    tInputData->mMalfPhotoFluxDuration,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    tInputData->mMalfPhotoFluxRampTime,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    tInputData->mMalfExposedFractionValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    tInputData->mMalfTemperatureValue,     0.0);
    CPPUNIT_ASSERT(false == tInputData->mMalfPhotoFluxFlag);
    CPPUNIT_ASSERT(false == tInputData->mMalfExposedFractionFlag);
    CPPUNIT_ASSERT(false == tInputData->mMalfTemperatureFlag);

    /// @test    Input data default construction.
    GunnsElectPvStringInputData defaultInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mPhotoFlux,                0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mSourceExposedFraction,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mTemperature,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mMalfPhotoFluxMagnitude,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mMalfPhotoFluxDuration,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mMalfPhotoFluxRampTime,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mMalfExposedFractionValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mMalfTemperatureValue,     0.0);
    CPPUNIT_ASSERT(false == defaultInput.mMalfPhotoFluxFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfExposedFractionFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfTemperatureFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic String model input data applyOverrides method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testInputOverrides()
{
    UT_RESULT;

    const double dt = 0.1;
    FriendlyGunnsElectPvStringInputData* stringInput =
            static_cast<FriendlyGunnsElectPvStringInputData*>(tInputData);

    /// @test    Photo flux override malf.
    stringInput->setMalfPhotoFlux(true, 1.0, 90.0, 30.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,  stringInput->mMalfPhotoFluxMagnitude, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(90.0, stringInput->mMalfPhotoFluxDuration,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(30.0, stringInput->mMalfPhotoFluxRampTime,  0.0);
    CPPUNIT_ASSERT(true == stringInput->mMalfPhotoFluxFlag);

    /// @test    Photo flux override malf start.
    stringInput->applyOverrides(dt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(dt,         stringInput->mPhotoFluxElapsedTime,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPhotoFlux, stringInput->mPhotoFluxStartMagnitude, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPhotoFlux, stringInput->mPhotoFlux,               0.0);

    /// @test    Photo flux override malf ramp in.
    stringInput->mPhotoFluxElapsedTime = 15.0;
    double expectedFlux = tPhotoFlux + 0.5 * (1.0 - tPhotoFlux);
    stringInput->applyOverrides(dt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL((15.0 + dt),  stringInput->mPhotoFluxElapsedTime, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, stringInput->mPhotoFlux,            DBL_EPSILON);

    /// @test    Photo flux override malf hold.
    stringInput->mPhotoFluxElapsedTime = 45.0;
    expectedFlux = 1.0;
    stringInput->applyOverrides(dt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL((45.0 + dt),  stringInput->mPhotoFluxElapsedTime, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, stringInput->mPhotoFlux,            DBL_EPSILON);

    /// @test    Photo flux override malf ramp out.
    stringInput->mPhotoFlux = 20.0;
    stringInput->mPhotoFluxElapsedTime = 75.0;
    expectedFlux = 20.0 + 0.5 * (1.0 - 20.0);
    stringInput->applyOverrides(dt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL((75.0 + dt),  stringInput->mPhotoFluxElapsedTime, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, stringInput->mPhotoFlux,            DBL_EPSILON);
    stringInput->mPhotoFlux = tPhotoFlux;

    /// @test    Photo flux override malf switches off.
    stringInput->mPhotoFluxElapsedTime = 90.0;
    stringInput->applyOverrides(dt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPhotoFlux, stringInput->mPhotoFlux,            DBL_EPSILON);
    stringInput->applyOverrides(dt);
    CPPUNIT_ASSERT(false == stringInput->mMalfPhotoFluxFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(dt,         stringInput->mPhotoFluxElapsedTime, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPhotoFlux, stringInput->mPhotoFlux,            DBL_EPSILON);

    /// @test    Photo flux malf limits ramp time to 1/2 duration.
    stringInput->setMalfPhotoFlux(true, 1.0, 40.0, 30.0);
    stringInput->applyOverrides(dt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(20.0, stringInput->mMalfPhotoFluxRampTime, DBL_EPSILON);

    stringInput->setMalfPhotoFlux();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, stringInput->mMalfPhotoFluxMagnitude, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, stringInput->mMalfPhotoFluxDuration,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, stringInput->mMalfPhotoFluxRampTime,  0.0);
    CPPUNIT_ASSERT(false == stringInput->mMalfPhotoFluxFlag);

    /// @test    Exposed fraction override malf.
    tInputData->setMalfExposedFraction(true, 0.5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tInputData->mMalfExposedFractionValue, 0.0);
    CPPUNIT_ASSERT(true == tInputData->mMalfExposedFractionFlag);

    tInputData->applyOverrides(dt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tInputData->mSourceExposedFraction, 0.0);

    tInputData->setMalfExposedFraction();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tInputData->mMalfExposedFractionValue, 0.0);
    CPPUNIT_ASSERT(false == tInputData->mMalfExposedFractionFlag);

    /// @test    Temperature override malf.
    tInputData->setMalfTemperature(true, 310.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(310.0, tInputData->mMalfTemperatureValue, 0.0);
    CPPUNIT_ASSERT(true == tInputData->mMalfTemperatureFlag);

    tInputData->applyOverrides(dt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(310.0, tInputData->mTemperature, 0.0);

    tInputData->setMalfTemperature();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tInputData->mMalfTemperatureValue, 0.0);
    CPPUNIT_ASSERT(false == tInputData->mMalfTemperatureFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsElectPvString class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testConstruction()
{
    UT_RESULT;

    /// @test    Nominal construction.
    CPPUNIT_ASSERT(false == tArticle->mMalfCellGroupFlag);
    CPPUNIT_ASSERT(0     == tArticle->mMalfCellGroupValue);
    CPPUNIT_ASSERT(false == tArticle->mMalfDegradeFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfDegradeValue);
    CPPUNIT_ASSERT(""    == tArticle->mName);
    CPPUNIT_ASSERT(tConfigData == tArticle->mConfig);
    CPPUNIT_ASSERT(tInputData  == tArticle->mInput);
    CPPUNIT_ASSERT(0.0   == tArticle->mShortCircuitCurrent);
    CPPUNIT_ASSERT(0.0   == tArticle->mOpenCircuitVoltage);
    CPPUNIT_ASSERT(0     == tArticle->mRefCell);
    CPPUNIT_ASSERT(0     == tArticle->mEqProps);
    CPPUNIT_ASSERT(0.0   == tArticle->mMpp.mConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mTerminal.mConductance);
    CPPUNIT_ASSERT(0     == tArticle->mNumBypassedGroups);
    CPPUNIT_ASSERT(0     == tArticle->mNumActiveCells);
    CPPUNIT_ASSERT(false == tArticle->mShunted);
    CPPUNIT_ASSERT(0.0   == tArticle->mShuntVoltageDrop);
    CPPUNIT_ASSERT(0.0   == tArticle->mSeriesVoltageDrop);

    /// @test    Default construction.
    FriendlyGunnsElectPvString defaultString;
    CPPUNIT_ASSERT(false ==  defaultString.mMalfCellGroupFlag);
    CPPUNIT_ASSERT(0     ==  defaultString.mMalfCellGroupValue);
    CPPUNIT_ASSERT(false ==  defaultString.mMalfDegradeFlag);
    CPPUNIT_ASSERT(0.0   ==  defaultString.mMalfDegradeValue);
    CPPUNIT_ASSERT(""    ==  defaultString.mName);
    CPPUNIT_ASSERT(0.0   ==  defaultString.mShortCircuitCurrent);
    CPPUNIT_ASSERT(0.0   ==  defaultString.mOpenCircuitVoltage);
    CPPUNIT_ASSERT(0     ==  defaultString.mRefCell);
    CPPUNIT_ASSERT(0     ==  defaultString.mEqProps);
    CPPUNIT_ASSERT(0.0   ==  defaultString.mMpp.mConductance);
    CPPUNIT_ASSERT(0.0   ==  defaultString.mTerminal.mConductance);
    CPPUNIT_ASSERT(0     ==  defaultString.mNumBypassedGroups);
    CPPUNIT_ASSERT(0     ==  defaultString.mNumActiveCells);
    CPPUNIT_ASSERT(false ==  defaultString.mShunted);
    CPPUNIT_ASSERT(0.0   ==  defaultString.mShuntVoltageDrop);
    CPPUNIT_ASSERT(0.0   ==  defaultString.mSeriesVoltageDrop);

    /// @test    New/delete for code coverage.
    GunnsElectPvString* article = new GunnsElectPvString;
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic String model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(tName == tArticle->mName);
    CPPUNIT_ASSERT(0.0   == tArticle->mShortCircuitCurrent);
    CPPUNIT_ASSERT(0.0   == tArticle->mOpenCircuitVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mEqProps->mRs);
    CPPUNIT_ASSERT(0.0   == tArticle->mMpp.mConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mTerminal.mConductance);
    CPPUNIT_ASSERT(0     == tArticle->mNumBypassedGroups);
    CPPUNIT_ASSERT(0     == tArticle->mNumActiveCells);
    CPPUNIT_ASSERT(false == tArticle->mShunted);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic String model nominal initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testInitializationErrors()
{
    UT_RESULT;

    /// @test    Exception thrown for empty name.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(""), TsInitializationException);

    /// @test    Exception thrown for missing config data.
    tArticle->mConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tArticle->mConfig = tConfigData;

    /// @test    Exception thrown for bad surface area.
    tConfigData->mCellConfig.mSurfaceArea = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mSurfaceArea = tCellSurfaceArea;

    /// @test    Exception thrown for bad efficiency.
    tConfigData->mCellConfig.mEfficiency = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mEfficiency = 1.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mEfficiency = tCellEfficiency;

    /// @test    Exception thrown for bad cell series resistance.
    tConfigData->mCellConfig.mSeriesResistance = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mSeriesResistance = tCellSeriesResistance;

    /// @test    Exception thrown for bad cell shunt resistance.
    tConfigData->mCellConfig.mShuntResistance = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mShuntResistance = tCellShuntResistance;

    /// @test    Exception thrown for bad open circuit voltage.
    tConfigData->mCellConfig.mOpenCircuitVoltage = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mOpenCircuitVoltage = tCellOpenCircuitVoltage;

    /// @test    Exception thrown for bad # cells.
    tConfigData->mNumCells = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mNumCells = tNumCells;

    /// @test    Exception thrown for bad bypass diode interval.
    tConfigData->mBypassDiodeInterval = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mBypassDiodeInterval = 3;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mBypassDiodeInterval = tBypassDiodeInterval;

    /// @test    Exception thrown for missing input data.
    tArticle->mInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tArticle->mInput = tInputData;

    /// @test    Exception thrown for bad photo flux magnitude.
    tInputData->mPhotoFlux = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tInputData->mPhotoFlux = tPhotoFlux;

    /// @test    Exception thrown for bad source exposed fraction.
    tInputData->mSourceExposedFraction = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tInputData->mSourceExposedFraction = 1.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tInputData->mSourceExposedFraction = tSourceExposedFraction;

    /// @test    Exception thrown for bad temperature.
    tInputData->mTemperature = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tInputData->mTemperature = tTemperature;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic String model determination of number of active cells and
///           bypassed groups.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testActiveCells()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    {
        /// @test    Partial shading of one group.
        tInputData->mSourceExposedFraction = 0.9;
        const int expectedBypGroup = 1;
        const int expectedNumCells = 15;
        tArticle->update();
        CPPUNIT_ASSERT(expectedBypGroup == tArticle->mNumBypassedGroups);
        CPPUNIT_ASSERT(expectedNumCells == tArticle->mNumActiveCells);
    } {
        /// @test    Exposed fraction out of bounds high.
        tInputData->mSourceExposedFraction = 2.1;
        const int expectedBypGroup = 0;
        const int expectedNumCells = 20;
        tArticle->update();
        CPPUNIT_ASSERT(expectedBypGroup == tArticle->mNumBypassedGroups);
        CPPUNIT_ASSERT(expectedNumCells == tArticle->mNumActiveCells);
    } {
        /// @test    Exposed fraction out of bounds low.
        tInputData->mSourceExposedFraction = -4;
        const int expectedBypGroup = 4;
        const int expectedNumCells = 0;
        tArticle->update();
        CPPUNIT_ASSERT(expectedBypGroup == tArticle->mNumBypassedGroups);
        CPPUNIT_ASSERT(expectedNumCells == tArticle->mNumActiveCells);
    } {
        /// @test    Malfunction of 2 groups.
        tInputData->mSourceExposedFraction = 1.0;
        tArticle->mMalfCellGroupFlag  = true;
        tArticle->mMalfCellGroupValue = 2;
        const int expectedBypGroup = 2;
        const int expectedNumCells = 10;
        tArticle->update();
        CPPUNIT_ASSERT(expectedBypGroup == tArticle->mNumBypassedGroups);
        CPPUNIT_ASSERT(expectedNumCells == tArticle->mNumActiveCells);
    } {
        /// @test    Malfunction limit high.
        tArticle->mMalfCellGroupValue = 10;
        const int expectedBypGroup = 4;
        const int expectedNumCells = 0;
        tArticle->update();
        CPPUNIT_ASSERT(expectedBypGroup == tArticle->mNumBypassedGroups);
        CPPUNIT_ASSERT(expectedNumCells == tArticle->mNumActiveCells);
        CPPUNIT_ASSERT_EQUAL(4, tArticle->mMalfCellGroupValue);
    } {
        /// @test    Malfunction limit low.
        tArticle->mMalfCellGroupValue = -1;
        const int expectedBypGroup = 0;
        const int expectedNumCells = 20;
        tArticle->update();
        CPPUNIT_ASSERT(expectedBypGroup == tArticle->mNumBypassedGroups);
        CPPUNIT_ASSERT(expectedNumCells == tArticle->mNumActiveCells);
        CPPUNIT_ASSERT_EQUAL(0, tArticle->mMalfCellGroupValue);
    } {
        /// @test    Malfunction combined with shading.
        tArticle->mMalfCellGroupValue = 2;
        tInputData->mSourceExposedFraction = 0.49;
        const int expectedBypGroup = 3;
        const int expectedNumCells = 5;
        tArticle->update();
        CPPUNIT_ASSERT(expectedBypGroup == tArticle->mNumBypassedGroups);
        CPPUNIT_ASSERT(expectedNumCells == tArticle->mNumActiveCells);
    } {
        /// @test    Malfunction combined with shading.
        tInputData->mSourceExposedFraction = 0.9;
        const int expectedBypGroup = 2;
        const int expectedNumCells = 10;
        tArticle->update();
        CPPUNIT_ASSERT(expectedBypGroup == tArticle->mNumBypassedGroups);
        CPPUNIT_ASSERT(expectedNumCells == tArticle->mNumActiveCells);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic String model step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    {
        /// @test    Update outputs with one cell group bypassed.
        tArticle->mMalfCellGroupFlag  = true;
        tArticle->mMalfCellGroupValue = 1;
        tArticle->update();

        const int    numCells       = tNumCells - tBypassDiodeInterval;
        const double expectedRsh    = tCellShuntResistance;
        const double dT             = tTemperature - tCellRefTemperature;
        const double expectedVsh    = tCellOpenCircuitVoltage * numCells
                                    * (1.0 + tCellTemperatureVoltageCoeff * dT);
        const double expectedRs     = numCells * tCellSeriesResistance;
        const double expectedVs     = tBlockingDiodeVoltageDrop + tBypassDiodeVoltageDrop;
        const double expectedConv   = tCellEfficiency * (1.0 + tCellTemperatureCurrentCoeff * dT);
        const double expectedPwrSrc = tPhotoFlux * numCells * tCellSurfaceArea * expectedConv;
        const double expectedIsrc   = expectedPwrSrc / expectedVsh;
        const double expectedVoc    = expectedVsh - expectedVs + expectedIsrc * expectedRsh / 1.0E6;
        const double expectedIsc    = (expectedIsrc * expectedRsh - expectedVs)
                                    / (expectedRsh + expectedRs);
        const double psrc           = expectedVsh * expectedIsrc;
        const double ish            = expectedVsh / expectedRsh;
        const double is             = expectedIsrc - ish;
        const double expectedPmpp   = psrc - expectedVsh*ish - expectedVs*is
                                    - is*is*numCells*tCellSeriesResistance;
        const double expectedVmpp   = expectedVsh - is*expectedRs - expectedVs;
        const double expectedImpp   = expectedPmpp / expectedVmpp;
        const double expectedGmpp   = expectedImpp / expectedVmpp;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRsh,    tArticle->mEqProps->mRsh,               DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVsh,    tArticle->mShuntVoltageDrop,            DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRs,     tArticle->mEqProps->mRs,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVs,     tArticle->mSeriesVoltageDrop,           DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsrc,   tArticle->mEqProps->mIL,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoc,    tArticle->mOpenCircuitVoltage,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsc,    tArticle->mShortCircuitCurrent,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPmpp,   tArticle->mMpp.mPower,                  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVmpp,   tArticle->mMpp.mVoltage,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImpp,   tArticle->mMpp.mCurrent,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGmpp,   tArticle->mMpp.mConductance,            DBL_EPSILON);
    } {
        /// @test    Update outputs with one cell group bypassed and partial degrade malf.
        tArticle->mMalfDegradeFlag  = true;
        tArticle->mMalfDegradeValue = 0.2;
        tArticle->update();

        const int    numCells       = tNumCells - tBypassDiodeInterval;
        const double expectedRsh    = tCellShuntResistance;
        const double dT             = tTemperature - tCellRefTemperature;
        const double expectedVsh    = tCellOpenCircuitVoltage * numCells
                                    * (1.0 + tCellTemperatureVoltageCoeff * dT);
        const double expectedRs     = numCells * tCellSeriesResistance;
        const double expectedVs     = tBlockingDiodeVoltageDrop + tBypassDiodeVoltageDrop;
        const double expectedConv   = tCellEfficiency * (1.0 + tCellTemperatureCurrentCoeff * dT);
        const double expectedPwrSrc = tPhotoFlux * numCells * tCellSurfaceArea * expectedConv * 0.8;
        const double expectedIsrc   = expectedPwrSrc / expectedVsh;
        const double expectedVoc    = expectedVsh - expectedVs + expectedIsrc * expectedRsh / 1.0E6;
        const double expectedIsc    = (expectedIsrc * expectedRsh - expectedVs)
                                    / (expectedRsh + expectedRs);
        const double psrc           = expectedVsh * expectedIsrc;
        const double ish            = expectedVsh / expectedRsh;
        const double is             = expectedIsrc - ish;
        const double expectedPmpp   = psrc - expectedVsh*ish - expectedVs*is
                                    - is*is*numCells*tCellSeriesResistance;
        const double expectedVmpp   = expectedVsh - is*expectedRs - expectedVs;
        const double expectedImpp   = expectedPmpp / expectedVmpp;
        const double expectedGmpp   = expectedImpp / expectedVmpp;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRsh,    tArticle->mEqProps->mRsh,               DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVsh,    tArticle->mShuntVoltageDrop,            DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRs,     tArticle->mEqProps->mRs,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVs,     tArticle->mSeriesVoltageDrop,           DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsrc,   tArticle->mEqProps->mIL,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoc,    tArticle->mOpenCircuitVoltage,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsc,    tArticle->mShortCircuitCurrent,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPmpp,   tArticle->mMpp.mPower,                  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVmpp,   tArticle->mMpp.mVoltage,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImpp,   tArticle->mMpp.mCurrent,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGmpp,   tArticle->mMpp.mConductance,            DBL_EPSILON);
    } {
        /// @test    Update outputs with zero light source.
        tInputData->mPhotoFlux = 0.0;
        tArticle->update();

        const int    numCells       = tNumCells - tBypassDiodeInterval;
        const double expectedRsh    = tCellShuntResistance;
        const double dT             = tTemperature - tCellRefTemperature;
        const double expectedVsh    = tCellOpenCircuitVoltage * numCells
                                    * (1.0 + tCellTemperatureVoltageCoeff * dT);
        const double expectedRs     = numCells * tCellSeriesResistance;
        const double expectedVs     = tBlockingDiodeVoltageDrop + tBypassDiodeVoltageDrop;
        const double expectedConv   = tCellEfficiency * (1.0 + tCellTemperatureCurrentCoeff * dT);
        const double expectedPwrSrc = 0.0;
        const double expectedIsrc   = 0.0;
        const double expectedVoc    = expectedVsh - expectedVs + expectedIsrc * expectedRsh / 1.0E6;
        const double expectedIsc    = 0.0;
        const double expectedPmpp   = 0.0;
        const double expectedVmpp   = 0.0;
        const double expectedImpp   = 0.0;
        const double expectedGmpp   = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRsh,    tArticle->mEqProps->mRsh,               DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVsh,    tArticle->mShuntVoltageDrop,            DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRs,     tArticle->mEqProps->mRs,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVs,     tArticle->mSeriesVoltageDrop,           DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsrc,   tArticle->mEqProps->mIL,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoc,    tArticle->mOpenCircuitVoltage,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsc,    tArticle->mShortCircuitCurrent,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPmpp,   tArticle->mMpp.mPower,                  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVmpp,   tArticle->mMpp.mVoltage,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImpp,   tArticle->mMpp.mCurrent,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGmpp,   tArticle->mMpp.mConductance,            DBL_EPSILON);
    } {
        /// @test    Update outputs with light source but complete shading.
        tInputData->mPhotoFlux = tPhotoFlux;
        tArticle->mMalfCellGroupValue = 5;
        tArticle->update();

        const double expectedRsh    = tCellShuntResistance;
        const double expectedVsh    = 0.0;
        const double expectedRs     = 1.0 / 1.0E6;
        const double expectedVs     = tBlockingDiodeVoltageDrop + 4 * tBypassDiodeVoltageDrop;
        const double expectedPwrSrc = 0.0;
        const double expectedIsrc   = 0.0;
        const double expectedVoc    = 0.0;
        const double expectedIsc    = 0.0;
        const double expectedPmpp   = 0.0;
        const double expectedVmpp   = 0.0;
        const double expectedImpp   = 0.0;
        const double expectedGmpp   = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRsh,    tArticle->mEqProps->mRsh,               DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVsh,    tArticle->mShuntVoltageDrop,            DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRs,     tArticle->mEqProps->mRs,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVs,     tArticle->mSeriesVoltageDrop,           DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsrc,   tArticle->mEqProps->mIL,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoc,    tArticle->mOpenCircuitVoltage,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsc,    tArticle->mShortCircuitCurrent,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPmpp,   tArticle->mMpp.mPower,                  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVmpp,   tArticle->mMpp.mVoltage,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImpp,   tArticle->mMpp.mCurrent,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGmpp,   tArticle->mMpp.mConductance,            DBL_EPSILON);
    } {
        /// @test    Update MPP with zero source power.
        tArticle->mEqProps->mIL;
        tArticle->updateMpp();

        const double expectedPmpp   = 0.0;
        const double expectedVmpp   = 0.0;
        const double expectedImpp   = 0.0;
        const double expectedGmpp   = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPmpp,   tArticle->mMpp.mPower,                  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVmpp,   tArticle->mMpp.mVoltage,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImpp,   tArticle->mMpp.mCurrent,                DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGmpp,   tArticle->mMpp.mConductance,            DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Photovoltaic String getter and setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testAccessors()
{
    UT_RESULT;

    /// @test    Setter and getter for shunted flag.
    tArticle->setShunted(true);
    CPPUNIT_ASSERT(tArticle->isShunted());

    /// @test    Can get MPP values.
    tArticle->mMpp.mPower = 10.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, tArticle->getMpp().mPower, 0.0);

    /// @test    Can get terminal values.
    tArticle->mTerminal.mPower = 11.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(11.0, tArticle->getTerminal().mPower, 0.0);

    /// @test    Can get open-circuit voltage.
    tArticle->mOpenCircuitVoltage = 12.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(12.0, tArticle->getOpenCircuitVoltage(), 0.0);

    /// @test    Can get equivalent properties values.
//    tArticle->mEqProps->mIL = 13.0;
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(13.0, tArticle->getEqProps()->mIL, 0.0);

    /// @test    Can get short-circuit current.
    tArticle->mShortCircuitCurrent = 7.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, tArticle->getShortCircuitCurrent(), 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Photovoltaic String loadAtMpp method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testLoadAtMpp()
{
    UT_RESULT;

    tArticle->mMpp.mConductance = 42.0;
    tArticle->mMpp.mCurrent     = 43.0;
    tArticle->mMpp.mPower       = 44.0;
    tArticle->mMpp.mVoltage     = 45.0;

    /// @test    loadAtMpp method.
    tArticle->loadAtMpp();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(42.0, tArticle->getTerminal().mConductance, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(43.0, tArticle->getTerminal().mCurrent,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(44.0, tArticle->getTerminal().mPower,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(45.0, tArticle->getTerminal().mVoltage,     0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the loadAtPower method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testLoadAtPower()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    /// - Update the cell state with degrade and cell block malfs.
    tArticle->mMalfCellGroupFlag  = true;
    tArticle->mMalfCellGroupValue = 1;
    tArticle->mMalfDegradeFlag    = true;
    tArticle->mMalfDegradeValue   = 0.2;
    tArticle->update();

    {
        /// @test    Terminal outputs on open-circuit side of the I-V curve.
        const double expectedP = 0.5 * tArticle->mMpp.mPower;
        const bool   shortSide = false;

        tArticle->loadAtPower(expectedP, shortSide);

        const double expectedV = tArticle->mOpenCircuitVoltage
                               - (tArticle->mOpenCircuitVoltage - tArticle->mMpp.mVoltage)
                               * tArticle->mTerminal.mCurrent / tArticle->mMpp.mCurrent;
        const double expectedI = expectedP / expectedV;
        const double expectedG = expectedP / expectedV / expectedV;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, FLT_EPSILON);
    } {
        /// @test    Terminal outputs on short-circuit side of the I-V curve.
        const double expectedP = 0.5 * tArticle->mMpp.mPower;
        const bool   shortSide = true;

        tArticle->loadAtPower(expectedP, shortSide);

        const double expectedV = tArticle->mMpp.mVoltage
                               * (tArticle->mShortCircuitCurrent - tArticle->mTerminal.mCurrent)
                               / (tArticle->mShortCircuitCurrent - tArticle->mMpp.mCurrent);
        const double expectedI = expectedP / expectedV;
        const double expectedG = expectedP / expectedV / expectedV;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, FLT_EPSILON);
    } {
        /// @test    Terminal outputs given zero power load.
        const double expectedP = 0.0;
        const bool   shortSide = true;

        tArticle->loadAtPower(expectedP, shortSide);

        const double expectedV = 0.0;
        const double expectedI = 0.0;
        const double expectedG = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, DBL_EPSILON);
    } {
        /// @test    Terminal outputs given power load > maximum.
        const double expectedP = 0.0;
        const bool   shortSide = true;

        tArticle->loadAtPower(1000.0, shortSide);

        const double expectedV = 0.0;
        const double expectedI = 0.0;
        const double expectedG = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, DBL_EPSILON);
    } {
        /// @test    Terminal outputs with no active cells.
        tArticle->mMalfCellGroupFlag  = true;
        tArticle->mMalfCellGroupValue = 4;
        tArticle->update();

        const double expectedP = 0.0;
        const bool   shortSide = false;

        tArticle->loadAtPower(0.01, shortSide);

        const double expectedV = 0.0;
        const double expectedI = 0.0;
        const double expectedG = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the loadAtVoltage and predictCurrentAtVoltage methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testLoadAtVoltage()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    /// - Update the cell state with degrade and cell block malfs.
    tArticle->mMalfCellGroupFlag  = true;
    tArticle->mMalfCellGroupValue = 1;
    tArticle->mMalfDegradeFlag    = true;
    tArticle->mMalfDegradeValue   = 0.2;
    tArticle->update();

    {
        /// @test    Terminal outputs on open-circuit side of the I-V curve.
        const double expectedP = 0.5 * tArticle->mMpp.mPower;
        const bool   shortSide = false;

        tArticle->loadAtPower(expectedP, shortSide);
        const double expectedV  = tArticle->mTerminal.mVoltage;
        const double expectedI  = expectedP / expectedV;
        const double expectedG  = expectedP / expectedV / expectedV;
        const double predictedI = tArticle->predictCurrentAtVoltage(expectedV);
        tArticle->loadAtVoltage(expectedV);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, predictedI,                       FLT_EPSILON);
    } {
        /// @test    Terminal outputs on short-circuit side of the I-V curve.
        const double expectedP = 0.5 * tArticle->mMpp.mPower;
        const bool   shortSide = true;

        tArticle->loadAtPower(expectedP, shortSide);
        const double expectedV  = tArticle->mTerminal.mVoltage;
        const double expectedI  = expectedP / expectedV;
        const double expectedG  = expectedP / expectedV / expectedV;
        const double predictedI = tArticle->predictCurrentAtVoltage(expectedV);
        tArticle->loadAtVoltage(expectedV);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, predictedI,                       FLT_EPSILON);
    } {
        /// @test    Terminal outputs given zero voltage.
        const double expectedV  = 0.0;
        const double predictedI = tArticle->predictCurrentAtVoltage(expectedV);
        tArticle->loadAtVoltage(expectedV);
        const double expectedP = 0.0;
        const double expectedI = 0.0;
        const double expectedG = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, predictedI,                       DBL_EPSILON);
    } {
        /// @test    Terminal outputs given voltage greater than maximum.
        const double expectedV  = tArticle->mOpenCircuitVoltage + 1.0;
        const double predictedI = tArticle->predictCurrentAtVoltage(expectedV);
        tArticle->loadAtVoltage(expectedV);
        const double expectedP = 0.0;
        const double expectedI = 0.0;
        const double expectedG = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, predictedI,                       DBL_EPSILON);
    } {
        /// @test    Terminal outputs with no active cells.
        tArticle->mMalfCellGroupFlag  = true;
        tArticle->mMalfCellGroupValue = 4;
        tArticle->update();

        const double expectedV  = tArticle->mOpenCircuitVoltage + 1.0;
        const double predictedI = tArticle->predictCurrentAtVoltage(expectedV);
        tArticle->loadAtVoltage(expectedV);
        const double expectedP = 0.0;
        const double expectedI = 0.0;
        const double expectedG = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, predictedI,                       DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the loadAtConductance method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString::testLoadAtConductance()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    /// - Update the cell state with degrade and cell block malfs.
    tArticle->mMalfCellGroupFlag  = true;
    tArticle->mMalfCellGroupValue = 1;
    tArticle->mMalfDegradeFlag    = true;
    tArticle->mMalfDegradeValue   = 0.2;
    tArticle->update();

    {
        /// @test    Terminal outputs on open-circuit side of the I-V curve.
        const double expectedP = 0.5 * tArticle->mMpp.mPower;
        const bool   shortSide = false;

        tArticle->loadAtPower(expectedP, shortSide);
        const double expectedG = tArticle->mTerminal.mConductance;
        const double expectedI = sqrt(expectedP * expectedG);
        const double expectedV = expectedP / expectedI;
        tArticle->loadAtConductance(expectedG);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, FLT_EPSILON);
    } {
        /// @test    Terminal outputs on open-circuit side of the I-V curve.
        const double expectedP = 0.5 * tArticle->mMpp.mPower;
        const bool   shortSide = true;

        tArticle->loadAtPower(expectedP, shortSide);
        const double expectedG = tArticle->mTerminal.mConductance;
        const double expectedI = sqrt(expectedP * expectedG);
        const double expectedV = expectedP / expectedI;
        tArticle->loadAtConductance(expectedG);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, FLT_EPSILON);
    } {
        /// @test    Terminal outputs given zero conductance.
        const double expectedG = 0.0;
        tArticle->loadAtConductance(expectedG);
        const double expectedP = 0.0;
        const double expectedI = 0.0;
        const double expectedV = tArticle->mOpenCircuitVoltage;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, DBL_EPSILON);
    } {
        /// @test    Terminal outputs with no active cells.
        tArticle->mMalfCellGroupFlag  = true;
        tArticle->mMalfCellGroupValue = 4;
        tArticle->update();

        const double expectedG = 1000.0;
        tArticle->loadAtConductance(expectedG);
        const double expectedP = 0.0;
        const double expectedI = 0.0;
        const double expectedV = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, DBL_EPSILON);
    }

    UT_PASS_LAST;
}
