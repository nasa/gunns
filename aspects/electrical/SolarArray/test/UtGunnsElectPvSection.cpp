/**
@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "software/exceptions/TsInitializationException.hh"
#include <iostream>
#include "strings/UtResult.hh"
#include "UtGunnsElectPvSection.hh"
#include "math/MsMath.hh"
#include "aspects/electrical/SolarArray/test/UtGunnsElectPvString2.hh"

/// @details  Test identification number.
int UtGunnsElectPvSection::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectPvSection class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvSection::UtGunnsElectPvSection()
    :
    tName(""),
    tConfigData(0),
    tInputData(0),
    tArticle(0),
    tSourceAngleExponent(0.0),
    tBacksideReduction(0.0),
    tSourceAngleEdgeOn(false),
    tRefSourceFluxMagnitude(0.0),
    tBlockingDiodeVoltageDrop(0.0),
    tBypassDiodeVoltageDrop(0.0),
    tBypassDiodeInterval(0),
    tNumCells(0),
    tCellSurfaceArea(0.0),
    tCellEfficiency(0.0),
    tCellSeriesResistance(0.0),
    tCellShuntResistance(0.0),
    tCellOpenCircuitVoltage(0.0),
    tCellRefTemperature(0.0),
    tCellTemperatureVoltageCoeff(0.0),
    tCellTemperatureCurrentCoeff(0.0),
    tSourceFluxMagnitude(0.0),
    tSourceAngle(0.0),
    tSourceExposedFraction(0.0),
    tTemperature(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectPvSection class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvSection::~UtGunnsElectPvSection()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvSection::setUp()
{
    tName = "tArticle";

    /// - Define the nominal configuration data.
    tSourceAngleExponent         =  0.8;
    tBacksideReduction           =  0.75;
    tSourceAngleEdgeOn           =  false;
    tRefSourceFluxMagnitude      =  31.636;
    tBlockingDiodeVoltageDrop    =  0.7;
    tBypassDiodeVoltageDrop      =  0.5;
    tBypassDiodeInterval         =  5;
    tNumCells                    =  20;
    tCellSurfaceArea             =  0.05;
    tCellEfficiency              =  1.0;
    tCellSeriesResistance        =  0.017;
    tCellShuntResistance         =  200.0;
    tCellOpenCircuitVoltage      =  0.6;
    tCellRefTemperature          =  294.0;
    tCellTemperatureVoltageCoeff = -0.003;
    tCellTemperatureCurrentCoeff =  0.00065;
    tConfigData                  = new GunnsElectPvSectionConfigData(tSourceAngleExponent,
                                                                     tBacksideReduction,
                                                                     tSourceAngleEdgeOn,
                                                                     tRefSourceFluxMagnitude,
                                                                     tBlockingDiodeVoltageDrop,
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
    tSourceFluxMagnitude   = 31.626;
    tSourceAngle           = 1.5;
    tSourceExposedFraction = 1.0;
    tTemperature           = 284.0;
    tInputData             = new GunnsElectPvSectionInputData(tSourceFluxMagnitude,
                                                              tSourceAngle,
                                                              tSourceExposedFraction,
                                                              tTemperature);

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsElectPvSection(tConfigData);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvSection::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Photovoltaic Section model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvSection::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleExponent,        tConfigData->mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBacksideReduction,          tConfigData->mBacksideReduction,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleEdgeOn,          tConfigData->mSourceAngleEdgeOn,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tRefSourceFluxMagnitude,     tConfigData->mRefSourceFluxMagnitude,                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockingDiodeVoltageDrop,   tConfigData->mStringConfig.mBlockingDiodeVoltageDrop, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBypassDiodeVoltageDrop,     tConfigData->mStringConfig.mBypassDiodeVoltageDrop,   0.0);
    CPPUNIT_ASSERT(tBypassDiodeInterval == tConfigData->mStringConfig.mBypassDiodeInterval);
    CPPUNIT_ASSERT(tNumCells            == tConfigData->mStringConfig.mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea,             tConfigData->mStringConfig.mCellConfig.mSurfaceArea,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellEfficiency,              tConfigData->mStringConfig.mCellConfig.mEfficiency,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSeriesResistance,        tConfigData->mStringConfig.mCellConfig.mSeriesResistance,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellShuntResistance,         tConfigData->mStringConfig.mCellConfig.mShuntResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellOpenCircuitVoltage,      tConfigData->mStringConfig.mCellConfig.mOpenCircuitVoltage,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellRefTemperature,          tConfigData->mStringConfig.mCellConfig.mRefTemperature,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureVoltageCoeff, tConfigData->mStringConfig.mCellConfig.mTemperatureVoltageCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureCurrentCoeff, tConfigData->mStringConfig.mCellConfig.mTemperatureCurrentCoeff, 0.0);

    /// @test    Configuration data default construction.
    GunnsElectPvSectionConfigData defaultConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mBacksideReduction,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mSourceAngleEdgeOn,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mRefSourceFluxMagnitude,                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mStringConfig.mBlockingDiodeVoltageDrop, 0.0);

    /// @test    Configuration data assignment operator.
    GunnsElectPvSectionConfigData assignConfig;
    assignConfig = *tConfigData;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleExponent,      assignConfig.mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBacksideReduction,        assignConfig.mBacksideReduction,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleEdgeOn,        assignConfig.mSourceAngleEdgeOn,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tRefSourceFluxMagnitude,   assignConfig.mRefSourceFluxMagnitude,                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockingDiodeVoltageDrop, assignConfig.mStringConfig.mBlockingDiodeVoltageDrop, 0.0);

    /// @test    Configuration data self assign.
    GunnsElectPvSectionConfigData* assignConfig2 = &assignConfig;
    assignConfig = *assignConfig2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleExponent, assignConfig.mSourceAngleExponent, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Photovoltaic Section model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvSection::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceFluxMagnitude,   tInputData->mSourceFluxMagnitude,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngle,           tInputData->mSourceAngle,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceExposedFraction, tInputData->mSourceExposedFraction, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperature,           tInputData->mTemperature,           0.0);

    /// @test    Input data default construction.
    GunnsElectPvSectionInputData defaultInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mSourceFluxMagnitude,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mSourceAngle,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mSourceExposedFraction, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mTemperature,           0.0);

    /// @test    Input data assignment operator.
    GunnsElectPvSectionInputData assignInput;
    assignInput = *tInputData;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceFluxMagnitude,   assignInput.mSourceFluxMagnitude,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngle,           assignInput.mSourceAngle,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceExposedFraction, assignInput.mSourceExposedFraction, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperature,           assignInput.mTemperature,           0.0);

    /// @test    Input data self assign.
    GunnsElectPvSectionInputData* assignInput2 = &assignInput;
    assignInput = *assignInput2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceFluxMagnitude, assignInput.mSourceFluxMagnitude, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsElectPvSection class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvSection::testConstruction()
{
    UT_RESULT;

    /// @test    Nominal construction.
    CPPUNIT_ASSERT(0           == tArticle->mStrings);
    CPPUNIT_ASSERT(""          == tArticle->mName);
    CPPUNIT_ASSERT(tConfigData == tArticle->mConfig);
    CPPUNIT_ASSERT(0           == tArticle->mNumStrings);
    CPPUNIT_ASSERT(0.0         == tArticle->mInput.mSourceFluxMagnitude);
    CPPUNIT_ASSERT(0.0         == tArticle->mInput.mSourceAngle);
    CPPUNIT_ASSERT(0.0         == tArticle->mInput.mSourceExposedFraction);
    CPPUNIT_ASSERT(0.0         == tArticle->mInput.mTemperature);
    CPPUNIT_ASSERT(0.0         == tArticle->mStringsInput.mPhotoFlux);
    CPPUNIT_ASSERT(0.0         == tArticle->mStringsInput.mSourceExposedFraction);
    CPPUNIT_ASSERT(0.0         == tArticle->mStringsInput.mTemperature);
    CPPUNIT_ASSERT(0.0         == tArticle->mPercentInsolation);

    /// @test    Default construction.
    FriendlyGunnsElectPvSection defaultSection;
    CPPUNIT_ASSERT(0           == defaultSection.mStrings);
    CPPUNIT_ASSERT(""          == defaultSection.mName);
    CPPUNIT_ASSERT(0           == defaultSection.mNumStrings);
    CPPUNIT_ASSERT(0.0         == defaultSection.mInput.mSourceFluxMagnitude);
    CPPUNIT_ASSERT(0.0         == defaultSection.mInput.mSourceAngle);
    CPPUNIT_ASSERT(0.0         == defaultSection.mInput.mSourceExposedFraction);
    CPPUNIT_ASSERT(0.0         == defaultSection.mInput.mTemperature);
    CPPUNIT_ASSERT(0.0         == defaultSection.mStringsInput.mPhotoFlux);
    CPPUNIT_ASSERT(0.0         == defaultSection.mStringsInput.mSourceExposedFraction);
    CPPUNIT_ASSERT(0.0         == defaultSection.mStringsInput.mTemperature);
    CPPUNIT_ASSERT(0.0         == defaultSection.mPercentInsolation);

    /// @test    New/delete for code coverage.
    GunnsElectPvSection* testArticle = new GunnsElectPvSection();
    delete testArticle;

    UT_PASS;
}

#include "UtGunnsElectPvString.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Section model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvSection::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName, *tInputData, tNumStrings));

    FriendlyGunnsElectPvString* stringPtr[tNumStrings];
    stringPtr[0] = static_cast<FriendlyGunnsElectPvString*>(&tArticle->mStrings[0]);
    stringPtr[1] = static_cast<FriendlyGunnsElectPvString*>(&tArticle->mStrings[1]);
    stringPtr[2] = static_cast<FriendlyGunnsElectPvString*>(&tArticle->mStrings[2]);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(tName                              == tArticle->mName);
    CPPUNIT_ASSERT(tInputData->mSourceFluxMagnitude   == tArticle->mInput.mSourceFluxMagnitude);
    CPPUNIT_ASSERT(tInputData->mSourceAngle           == tArticle->mInput.mSourceAngle);
    CPPUNIT_ASSERT(tInputData->mSourceExposedFraction == tArticle->mInput.mSourceExposedFraction);
    CPPUNIT_ASSERT(tInputData->mTemperature           == tArticle->mInput.mTemperature);
    CPPUNIT_ASSERT(tNumStrings                        == tArticle->mNumStrings);
    CPPUNIT_ASSERT(tName + ".mStrings_0"              == stringPtr[0]->mName);
    CPPUNIT_ASSERT(tName + ".mStrings_1"              == stringPtr[1]->mName);
    CPPUNIT_ASSERT(tName + ".mStrings_2"              == stringPtr[2]->mName);
    CPPUNIT_ASSERT(&tConfigData->mStringConfig        == stringPtr[0]->mConfig);
    CPPUNIT_ASSERT(&tConfigData->mStringConfig        == stringPtr[1]->mConfig);
    CPPUNIT_ASSERT(&tConfigData->mStringConfig        == stringPtr[2]->mConfig);
    CPPUNIT_ASSERT(&tArticle->mStringsInput           == stringPtr[0]->mInput);
    CPPUNIT_ASSERT(&tArticle->mStringsInput           == stringPtr[1]->mInput);
    CPPUNIT_ASSERT(&tArticle->mStringsInput           == stringPtr[2]->mInput);
    CPPUNIT_ASSERT(0.0                                == tArticle->mPercentInsolation);

    /// @test    Initialization with version 2 strings.
    const double cellIsc = 2.6;
    const double cellVmp = 0.5;
    const double cellImp = 2.4;
    const double cellN   = 1.0;
    GunnsElectPvSectionConfigData version2Config(tCellOpenCircuitVoltage,
                                                 cellIsc,
                                                 cellVmp,
                                                 cellImp,
                                                 tCellRefTemperature,
                                                 tCellTemperatureVoltageCoeff,
                                                 tCellTemperatureCurrentCoeff,
                                                 cellN,
                                                 tCellSurfaceArea,
                                                 tSourceAngleExponent,
                                                 tBacksideReduction,
                                                 tSourceAngleEdgeOn,
                                                 tRefSourceFluxMagnitude,
                                                 tBlockingDiodeVoltageDrop,
                                                 tBypassDiodeVoltageDrop,
                                                 tBypassDiodeInterval,
                                                 tNumCells);
    CPPUNIT_ASSERT(version2Config.mStringConfig.mCellConfig.isVersion2());
    CPPUNIT_ASSERT(cellN == version2Config.mStringConfig.mCellConfig.mIdeality);
    FriendlyGunnsElectPvSection* article2 = new FriendlyGunnsElectPvSection(&version2Config);
    CPPUNIT_ASSERT_NO_THROW(article2->initialize("article2", *tInputData, tNumStrings));
    FriendlyGunnsElectPvString2* v2string = static_cast<FriendlyGunnsElectPvString2*>(&article2->mStrings[0]);
    CPPUNIT_ASSERT(0 != v2string->mRefCell);
    delete article2;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Section model nominal initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvSection::testInitializationErrors()
{
    UT_RESULT;

    /// @test    Exception thrown for empty name.
    CPPUNIT_ASSERT_THROW(tArticle->initialize("", *tInputData, tNumStrings), TsInitializationException);

    /// @test    Exception thrown for missing config data.
    tArticle->mConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, *tInputData, tNumStrings), TsInitializationException);
    tArticle->mConfig = tConfigData;

    /// @test    Exception thrown for bad source angle exponent.
    tConfigData->mSourceAngleExponent = 0.099;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, *tInputData, tNumStrings), TsInitializationException);
    tConfigData->mSourceAngleExponent = 10.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, *tInputData, tNumStrings), TsInitializationException);
    tConfigData->mSourceAngleExponent = tSourceAngleExponent;

    /// @test    Exception thrown for bad backside reduction.
    tConfigData->mBacksideReduction = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, *tInputData, tNumStrings), TsInitializationException);
    tConfigData->mBacksideReduction = 1.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, *tInputData, tNumStrings), TsInitializationException);
    tConfigData->mBacksideReduction = tBacksideReduction;

    /// @test    Exception thrown for bad number of strings.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, *tInputData, 0), TsInitializationException);

    /// @test    Exception thrown for bad source flux magnitude.
    tInputData->mSourceFluxMagnitude = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, *tInputData, tNumStrings), TsInitializationException);
    tInputData->mSourceFluxMagnitude = tSourceFluxMagnitude;

    /// @test    Exception thrown for bad source exposed fraction.
    tInputData->mSourceExposedFraction = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, *tInputData, tNumStrings), TsInitializationException);
    tInputData->mSourceExposedFraction = 1.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, *tInputData, tNumStrings), TsInitializationException);
    tInputData->mSourceExposedFraction = tSourceExposedFraction;

    /// @test    Exception thrown for bad temperature.
    tInputData->mTemperature = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, *tInputData, tNumStrings), TsInitializationException);
    tInputData->mTemperature = tTemperature;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Section model update method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvSection::testUpdate()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName, *tInputData, tNumStrings));

    {
        /// @test    Nominal outputs.
        tArticle->update(0.0);

        const double expectedFacing = pow(cos(tSourceAngle), tSourceAngleExponent);
        const double expectedFlux   = expectedFacing * tSourceFluxMagnitude * tSourceExposedFraction;
        const double expectedInsol  = 100.0 * expectedFlux / tRefSourceFluxMagnitude;
        const double expectedPower  = -tArticle->mStrings[0].getTerminal().mPower
                                     - tArticle->mStrings[1].getTerminal().mPower
                                     - tArticle->mStrings[2].getTerminal().mPower;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,           tArticle->mStringsInput.mPhotoFlux,             DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceExposedFraction, tArticle->mStringsInput.mSourceExposedFraction, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperature,           tArticle->mStringsInput.mTemperature,           DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInsol,          tArticle->mPercentInsolation,                   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,          tArticle->mTerminalPower,                       DBL_EPSILON);
    } {
        /// @test    Outputs with edge-on source angle, back-lit, 0 reference soure flux mag, and
        ///          string input temperature override.
        tConfigData->mSourceAngleEdgeOn      = true;
        tConfigData->mRefSourceFluxMagnitude = 0.0;
        tArticle->mInput.mSourceAngle        = -1.5;
        tArticle->mPercentInsolation         = 0.0;
        tArticle->mStringsInput.setMalfTemperature(true, 400.0);
        tArticle->update(0.0);

        const double expectedFacing = pow(sin(1.5), tSourceAngleExponent);
        const double expectedFlux   = expectedFacing * tSourceFluxMagnitude * tSourceExposedFraction
                                    * (1.0 - tBacksideReduction);
        const double expectedInsol  = 0.0;
        const double expectedPower  = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,           tArticle->mStringsInput.mPhotoFlux,             DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceExposedFraction, tArticle->mStringsInput.mSourceExposedFraction, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(400.0,                  tArticle->mStringsInput.mTemperature,           DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInsol,          tArticle->mPercentInsolation,                   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,          tArticle->mTerminalPower,                       DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Photovoltaic Section getter and setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvSection::testAccessors()
{
    UT_RESULT;

    /// @test    Can get number of strings.
    tArticle->mNumStrings = 5;
    CPPUNIT_ASSERT(5 == tArticle->getNumStrings());

    /// @test    Can get % insolation.
    tArticle->mPercentInsolation = 11.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(11.0, tArticle->getPercentInsolation(), 0.0);

    /// @test    Can set source flux magnitude.
    tArticle->setSourceFluxMagnitude(1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mInput.mSourceFluxMagnitude, 0.0);

    /// @test    Can set source angle.
    tArticle->setSourceAngle(2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, tArticle->mInput.mSourceAngle, 0.0);

    /// @test    Can set source exposed fraction.
    tArticle->setSourceExposedFraction(3.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, tArticle->mInput.mSourceExposedFraction, 0.0);

    /// @test    Can set temperature.
    tArticle->setTemperature(4.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, tArticle->mInput.mTemperature, 0.0);

    UT_PASS_LAST;
}
