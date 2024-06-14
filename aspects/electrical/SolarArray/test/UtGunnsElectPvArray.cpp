/**
@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((aspects/electrical/SolarArray/GunnsElectPvArray.o)
   (software/exceptions/TsInitializationException.o))
*/
#include "software/exceptions/TsInitializationException.hh"
#include <iostream>
#include "strings/UtResult.hh"
#include "UtGunnsElectPvArray.hh"
#include "math/MsMath.hh"

/// @details  Test identification number.
int UtGunnsElectPvArray::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectPvArray class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvArray::UtGunnsElectPvArray()
    :
    tLinks(),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tName(""),
    tConfigData(0),
    tInputData(0),
    tArticle(0),
    tNumSections(0),
    tNumStrings(0),
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
/// @details  This is the default destructor for the UtGunnsElectPvArray class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvArray::~UtGunnsElectPvArray()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::setUp()
{
    tName = "tArticle";

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    /// - Define the nominal configuration data.
    tNumSections                 =  3;
    tNumStrings                  =  12;
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
    tConfigData                  = new GunnsElectPvArrayConfigData(tName,
                                                                   &tNodeList,
                                                                   tNumSections,
                                                                   tNumStrings,
                                                                   tSourceAngleExponent,
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
    tSourceAngle           = 0.0;
    tSourceExposedFraction = 1.0;
    tTemperature           = 284.0;
    tInputData             = new GunnsElectPvArrayInputData(tSourceFluxMagnitude,
                                                            tSourceAngle,
                                                            tSourceExposedFraction,
                                                            tTemperature);

    /// - Define the nominal port mapping.
    tPort0 = 0;

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsElectPvArray;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Photovoltaic Array Link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration nominal construction.
    CPPUNIT_ASSERT(tNumSections == tConfigData->mNumSections);
    CPPUNIT_ASSERT(tNumStrings  == tConfigData->mNumStrings);
    CPPUNIT_ASSERT(0            == tConfigData->mNumStringsBySection);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleExponent,         tConfigData->mSectionConfig.mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleExponent,         tConfigData->mSectionConfig.mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBacksideReduction,           tConfigData->mSectionConfig.mBacksideReduction,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleEdgeOn,           tConfigData->mSectionConfig.mSourceAngleEdgeOn,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tRefSourceFluxMagnitude,      tConfigData->mSectionConfig.mRefSourceFluxMagnitude,                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockingDiodeVoltageDrop,    tConfigData->mSectionConfig.mStringConfig.mBlockingDiodeVoltageDrop, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBypassDiodeVoltageDrop,      tConfigData->mSectionConfig.mStringConfig.mBypassDiodeVoltageDrop,   0.0);
    CPPUNIT_ASSERT(tBypassDiodeInterval == tConfigData->mSectionConfig.mStringConfig.mBypassDiodeInterval);
    CPPUNIT_ASSERT(tNumCells            == tConfigData->mSectionConfig.mStringConfig.mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea,             tConfigData->mSectionConfig.mStringConfig.mCellConfig.mSurfaceArea,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellEfficiency,              tConfigData->mSectionConfig.mStringConfig.mCellConfig.mEfficiency,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSeriesResistance,        tConfigData->mSectionConfig.mStringConfig.mCellConfig.mSeriesResistance,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellShuntResistance,         tConfigData->mSectionConfig.mStringConfig.mCellConfig.mShuntResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellOpenCircuitVoltage,      tConfigData->mSectionConfig.mStringConfig.mCellConfig.mOpenCircuitVoltage,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellRefTemperature,          tConfigData->mSectionConfig.mStringConfig.mCellConfig.mRefTemperature,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureVoltageCoeff, tConfigData->mSectionConfig.mStringConfig.mCellConfig.mTemperatureVoltageCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureCurrentCoeff, tConfigData->mSectionConfig.mStringConfig.mCellConfig.mTemperatureCurrentCoeff, 0.0);

    /// @test    Configuration data default construction.
    GunnsElectPvArrayConfigData defaultConfig;
    CPPUNIT_ASSERT(0 == defaultConfig.mNumSections);
    CPPUNIT_ASSERT(0 == defaultConfig.mNumStrings);
    CPPUNIT_ASSERT(0 == defaultConfig.mNumStringsBySection);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mSectionConfig.mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mSectionConfig.mStringConfig.mBlockingDiodeVoltageDrop, 0.0);

    /// @test    Configuration data assignment operator.
    unsigned int* numStringsBySection = new unsigned int[tNumSections];
    tConfigData->mNumStringsBySection = numStringsBySection;

    GunnsElectPvArrayConfigData assignConfig;
    assignConfig = *tConfigData;
    CPPUNIT_ASSERT(tNumSections        == assignConfig.mNumSections);
    CPPUNIT_ASSERT(tNumStrings         == assignConfig.mNumStrings);
    CPPUNIT_ASSERT(numStringsBySection == assignConfig.mNumStringsBySection);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleExponent,      assignConfig.mSectionConfig.mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockingDiodeVoltageDrop, assignConfig.mSectionConfig.mStringConfig.mBlockingDiodeVoltageDrop, 0.0);

    /// @test    Configuration data self assign.
    GunnsElectPvArrayConfigData* assignConfig2 = &assignConfig;
    assignConfig = *assignConfig2;
    CPPUNIT_ASSERT(tNumSections == assignConfig.mNumSections);

    delete [] numStringsBySection;

    /// @test    Version 2 string construction.
    const double cellIsc = 2.6;
    const double cellVmp = 0.5;
    const double cellImp = 2.4;
    const double cellN   = 1.0;
    GunnsElectPvArrayConfigData version2Config(tName,
                                               &tNodeList,
                                               tCellOpenCircuitVoltage,
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
                                               tNumCells,
                                               tNumSections,
                                               tNumStrings);
    CPPUNIT_ASSERT(version2Config.mSectionConfig.mStringConfig.mCellConfig.isVersion2());
    CPPUNIT_ASSERT(tNumSections == version2Config.mNumSections);
    CPPUNIT_ASSERT(tNumStrings  == version2Config.mNumStrings);
    CPPUNIT_ASSERT(0            == version2Config.mNumStringsBySection);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleExponent,         version2Config.mSectionConfig.mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleExponent,         version2Config.mSectionConfig.mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBacksideReduction,           version2Config.mSectionConfig.mBacksideReduction,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleEdgeOn,           version2Config.mSectionConfig.mSourceAngleEdgeOn,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tRefSourceFluxMagnitude,      version2Config.mSectionConfig.mRefSourceFluxMagnitude,                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockingDiodeVoltageDrop,    version2Config.mSectionConfig.mStringConfig.mBlockingDiodeVoltageDrop, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBypassDiodeVoltageDrop,      version2Config.mSectionConfig.mStringConfig.mBypassDiodeVoltageDrop,   0.0);
    CPPUNIT_ASSERT(tBypassDiodeInterval == version2Config.mSectionConfig.mStringConfig.mBypassDiodeInterval);
    CPPUNIT_ASSERT(tNumCells            == version2Config.mSectionConfig.mStringConfig.mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea,             version2Config.mSectionConfig.mStringConfig.mCellConfig.mSurfaceArea,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellIsc,                      version2Config.mSectionConfig.mStringConfig.mCellConfig.mShortCircuitCurrent,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellVmp,                      version2Config.mSectionConfig.mStringConfig.mCellConfig.mMppVoltage,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellImp,                      version2Config.mSectionConfig.mStringConfig.mCellConfig.mMppCurrent,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellN,                        version2Config.mSectionConfig.mStringConfig.mCellConfig.mIdeality,                0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellOpenCircuitVoltage,      version2Config.mSectionConfig.mStringConfig.mCellConfig.mOpenCircuitVoltage,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellRefTemperature,          version2Config.mSectionConfig.mStringConfig.mCellConfig.mRefTemperature,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureVoltageCoeff, version2Config.mSectionConfig.mStringConfig.mCellConfig.mTemperatureVoltageCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureCurrentCoeff, version2Config.mSectionConfig.mStringConfig.mCellConfig.mTemperatureCurrentCoeff, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Photovoltaic Array Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceFluxMagnitude,   tInputData->mSourceFluxMagnitude,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngle,           tInputData->mSourceAngle,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceExposedFraction, tInputData->mSourceExposedFraction, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperature,           tInputData->mTemperature,           0.0);

    /// @test    Input data default construction.
    GunnsElectPvArrayInputData defaultInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mSourceFluxMagnitude,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mSourceAngle,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mSourceExposedFraction, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mTemperature,           0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsElectPvArray class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testConstruction()
{
    UT_RESULT;

    /// @test    Default construction.
    CPPUNIT_ASSERT(0     == tArticle->mSections);
    CPPUNIT_ASSERT(0     == tArticle->mConfig.mNumSections);
    CPPUNIT_ASSERT(false == tArticle->mOpenCircuitSide);
    CPPUNIT_ASSERT(false == tArticle->mCommonStringsOutput);
    CPPUNIT_ASSERT(0.0   == tArticle->mPercentInsolation);
    CPPUNIT_ASSERT(0.0   == tArticle->mShortCircuitCurrent);
    CPPUNIT_ASSERT(0.0   == tArticle->mOpenCircuitVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mMpp.mVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mTerminal.mVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mIvCornerVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mIvCornerCurrent);
    CPPUNIT_ASSERT(""    == tArticle->mName);

    /// @test    New/delete for code coverage.
    GunnsElectPvArray* testArticle = new GunnsElectPvArray();
    delete testArticle;

    UT_PASS;
}

#include "UtGunnsElectPvSection.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Array Link nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    FriendlyGunnsElectPvSection* sectionPtr[tNumSections];
    for (unsigned int i=0; i<tNumSections; ++i) {
        sectionPtr[i] = static_cast<FriendlyGunnsElectPvSection*>(&tArticle->mSections[i]);
    }

    /// @test    Nominal config data.
    CPPUNIT_ASSERT(tNumSections == tArticle->mConfig.mNumSections);
    CPPUNIT_ASSERT(tNumStrings  == tArticle->mConfig.mNumStrings);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleExponent,         tArticle->mConfig.mSectionConfig.mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleExponent,         tArticle->mConfig.mSectionConfig.mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBacksideReduction,           tArticle->mConfig.mSectionConfig.mBacksideReduction,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleEdgeOn,           tArticle->mConfig.mSectionConfig.mSourceAngleEdgeOn,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tRefSourceFluxMagnitude,      tArticle->mConfig.mSectionConfig.mRefSourceFluxMagnitude,                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockingDiodeVoltageDrop,    tArticle->mConfig.mSectionConfig.mStringConfig.mBlockingDiodeVoltageDrop, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBypassDiodeVoltageDrop,      tArticle->mConfig.mSectionConfig.mStringConfig.mBypassDiodeVoltageDrop,   0.0);
    CPPUNIT_ASSERT(tBypassDiodeInterval == tArticle->mConfig.mSectionConfig.mStringConfig.mBypassDiodeInterval);
    CPPUNIT_ASSERT(tNumCells            == tArticle->mConfig.mSectionConfig.mStringConfig.mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea,             tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mSurfaceArea,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellEfficiency,              tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mEfficiency,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSeriesResistance,        tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mSeriesResistance,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellShuntResistance,         tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mShuntResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellOpenCircuitVoltage,      tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mOpenCircuitVoltage,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellRefTemperature,          tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mRefTemperature,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureVoltageCoeff, tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mTemperatureVoltageCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureCurrentCoeff, tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mTemperatureCurrentCoeff, 0.0);

    /// @test    Init of sections.
    const unsigned int expectedSectionNumStrings = tNumStrings / tNumSections;
    CPPUNIT_ASSERT(tName + ".mSections_0"            == sectionPtr[0]->mName);
    CPPUNIT_ASSERT(tName + ".mSections_1"            == sectionPtr[1]->mName);
    CPPUNIT_ASSERT(tName + ".mSections_2"            == sectionPtr[2]->mName);
    CPPUNIT_ASSERT(&tArticle->mConfig.mSectionConfig == sectionPtr[0]->mConfig);
    CPPUNIT_ASSERT(&tArticle->mConfig.mSectionConfig == sectionPtr[1]->mConfig);
    CPPUNIT_ASSERT(&tArticle->mConfig.mSectionConfig == sectionPtr[2]->mConfig);
    CPPUNIT_ASSERT(tSourceFluxMagnitude              == sectionPtr[0]->mInput.mSourceFluxMagnitude);
    CPPUNIT_ASSERT(tSourceFluxMagnitude              == sectionPtr[1]->mInput.mSourceFluxMagnitude);
    CPPUNIT_ASSERT(tSourceFluxMagnitude              == sectionPtr[2]->mInput.mSourceFluxMagnitude);
    CPPUNIT_ASSERT(expectedSectionNumStrings         == sectionPtr[0]->mNumStrings);
    CPPUNIT_ASSERT(expectedSectionNumStrings         == sectionPtr[1]->mNumStrings);
    CPPUNIT_ASSERT(expectedSectionNumStrings         == sectionPtr[2]->mNumStrings);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(true  == tArticle->mOpenCircuitSide);
    CPPUNIT_ASSERT(true  == tArticle->mCommonStringsOutput);
    CPPUNIT_ASSERT(0.0   == tArticle->mPercentInsolation);
    CPPUNIT_ASSERT(0.0   == tArticle->mShortCircuitCurrent);
    CPPUNIT_ASSERT(0.0   == tArticle->mOpenCircuitVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mMpp.mVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mTerminal.mVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mIvCornerVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mIvCornerCurrent);
    CPPUNIT_ASSERT(tName == tArticle->mName);
    CPPUNIT_ASSERT(true  == tArticle->mInitFlag);

    //TODO test version 2 strings

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Array Link nominal initialization without exceptions, using a
///           custom number of strings for each section.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testCustomStringsInitialization()
{
    UT_RESULT;

    /// - Create & configure the list of custom # strings for each section.
    unsigned int* numStringsBySection = new unsigned int[tNumSections];
    numStringsBySection[0] = 4;
    numStringsBySection[1] = 5;
    numStringsBySection[2] = 6;
    tConfigData->mNumStringsBySection  = numStringsBySection;
    const unsigned int totalNumStrings = numStringsBySection[0]
                                       + numStringsBySection[1]
                                       + numStringsBySection[2];

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    FriendlyGunnsElectPvSection* sectionPtr[tNumSections];
    for (unsigned int i=0; i<tNumSections; ++i) {
        sectionPtr[i] = static_cast<FriendlyGunnsElectPvSection*>(&tArticle->mSections[i]);
    }

    /// @test    Nominal config data.
    CPPUNIT_ASSERT(tNumSections    == tArticle->mConfig.mNumSections);
    CPPUNIT_ASSERT(totalNumStrings == tArticle->mConfig.mNumStrings);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleExponent,         tArticle->mConfig.mSectionConfig.mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleExponent,         tArticle->mConfig.mSectionConfig.mSourceAngleExponent,                    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBacksideReduction,           tArticle->mConfig.mSectionConfig.mBacksideReduction,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourceAngleEdgeOn,           tArticle->mConfig.mSectionConfig.mSourceAngleEdgeOn,                      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tRefSourceFluxMagnitude,      tArticle->mConfig.mSectionConfig.mRefSourceFluxMagnitude,                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockingDiodeVoltageDrop,    tArticle->mConfig.mSectionConfig.mStringConfig.mBlockingDiodeVoltageDrop, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBypassDiodeVoltageDrop,      tArticle->mConfig.mSectionConfig.mStringConfig.mBypassDiodeVoltageDrop,   0.0);
    CPPUNIT_ASSERT(tBypassDiodeInterval == tArticle->mConfig.mSectionConfig.mStringConfig.mBypassDiodeInterval);
    CPPUNIT_ASSERT(tNumCells            == tArticle->mConfig.mSectionConfig.mStringConfig.mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea,             tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mSurfaceArea,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellEfficiency,              tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mEfficiency,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSeriesResistance,        tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mSeriesResistance,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellShuntResistance,         tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mShuntResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellOpenCircuitVoltage,      tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mOpenCircuitVoltage,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellRefTemperature,          tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mRefTemperature,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureVoltageCoeff, tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mTemperatureVoltageCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureCurrentCoeff, tArticle->mConfig.mSectionConfig.mStringConfig.mCellConfig.mTemperatureCurrentCoeff, 0.0);

    /// @test    Init of sections.
    const unsigned int expectedSectionNumStrings = tNumStrings / tNumSections;
    CPPUNIT_ASSERT(tName + ".mSections_0"            == sectionPtr[0]->mName);
    CPPUNIT_ASSERT(tName + ".mSections_1"            == sectionPtr[1]->mName);
    CPPUNIT_ASSERT(tName + ".mSections_2"            == sectionPtr[2]->mName);
    CPPUNIT_ASSERT(&tArticle->mConfig.mSectionConfig == sectionPtr[0]->mConfig);
    CPPUNIT_ASSERT(&tArticle->mConfig.mSectionConfig == sectionPtr[1]->mConfig);
    CPPUNIT_ASSERT(&tArticle->mConfig.mSectionConfig == sectionPtr[2]->mConfig);
    CPPUNIT_ASSERT(tSourceFluxMagnitude              == sectionPtr[0]->mInput.mSourceFluxMagnitude);
    CPPUNIT_ASSERT(tSourceFluxMagnitude              == sectionPtr[1]->mInput.mSourceFluxMagnitude);
    CPPUNIT_ASSERT(tSourceFluxMagnitude              == sectionPtr[2]->mInput.mSourceFluxMagnitude);
    CPPUNIT_ASSERT(numStringsBySection[0]            == sectionPtr[0]->mNumStrings);
    CPPUNIT_ASSERT(numStringsBySection[1]            == sectionPtr[1]->mNumStrings);
    CPPUNIT_ASSERT(numStringsBySection[2]            == sectionPtr[2]->mNumStrings);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(true  == tArticle->mOpenCircuitSide);
    CPPUNIT_ASSERT(true  == tArticle->mCommonStringsOutput);
    CPPUNIT_ASSERT(0.0   == tArticle->mPercentInsolation);
    CPPUNIT_ASSERT(0.0   == tArticle->mShortCircuitCurrent);
    CPPUNIT_ASSERT(0.0   == tArticle->mOpenCircuitVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mMpp.mVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mTerminal.mVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mIvCornerVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mIvCornerCurrent);
    CPPUNIT_ASSERT(tName == tArticle->mName);
    CPPUNIT_ASSERT(true  == tArticle->mInitFlag);

    delete [] numStringsBySection;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Array Link nominal initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testInitializationErrors()
{
    UT_RESULT;

    /// @test    Exception thrown for bad # sections.
    tConfigData->mNumSections = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mNumSections = tNumSections;

    /// @test    Exception thrown for total # strings < # sections.
    tConfigData->mNumStrings = 2;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mNumStrings = tNumStrings;

    /// @test    Exception thrown for # sections not divisible in total # strings.
    tConfigData->mNumStrings = 11;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mNumSections = tNumStrings;

    /// @test    Exception thrown from section for bad source angle exponent.
    tConfigData->mSectionConfig.mSourceAngleExponent = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mSectionConfig.mSourceAngleExponent = tSourceAngleExponent;

    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Array Link restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testRestart()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test    Restart method clears non-config and non-checkpointed data.
    tArticle->mOpenCircuitSide     = true;
    tArticle->mPercentInsolation   = 1.0;
    tArticle->mShortCircuitCurrent = 1.0;
    tArticle->mOpenCircuitVoltage  = 1.0;
    tArticle->mMpp.mVoltage        = 1.0;
    tArticle->mTerminal.mVoltage   = 1.0;
    tArticle->mIvCornerVoltage     = 1.0;
    tArticle->mIvCornerCurrent     = 1.0;
    tArticle->restart();
    CPPUNIT_ASSERT(true == tArticle->mOpenCircuitSide);
    CPPUNIT_ASSERT(true == tArticle->mCommonStringsOutput);
    CPPUNIT_ASSERT(0.0  == tArticle->mPercentInsolation);
    CPPUNIT_ASSERT(0.0  == tArticle->mShortCircuitCurrent);
    CPPUNIT_ASSERT(0.0  == tArticle->mOpenCircuitVoltage);
    CPPUNIT_ASSERT(0.0  == tArticle->mMpp.mVoltage);
    CPPUNIT_ASSERT(0.0  == tArticle->mTerminal.mVoltage);
    CPPUNIT_ASSERT(0.0  == tArticle->mIvCornerVoltage);
    CPPUNIT_ASSERT(0.0  == tArticle->mIvCornerCurrent);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Array Link step and updateState methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    {
        /// @test    Link port assignment control, sections & strings get updated, updateArray
        ///          method outputs and array matches strings when lit and all strings identical,
        ///          link outputs to solver on short-circuit side of the array I-V curve.
        tArticle->mUserPortSelect     = 0;
        tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
        tArticle->mOpenCircuitSide    = false;

        tArticle->step(0.0);

        CPPUNIT_ASSERT(1                     == tArticle->mNodeMap[0]);
        CPPUNIT_ASSERT(GunnsBasicLink::READY == tArticle->mUserPortSetControl);
        CPPUNIT_ASSERT(0.0 < tArticle->mSections[0].getPercentInsolation());
        CPPUNIT_ASSERT(0.0 < tArticle->mSections[0].mStrings[0].getShortCircuitCurrent());
        CPPUNIT_ASSERT(0.0 < tArticle->mSections[0].mStrings[0].getOpenCircuitVoltage());
        CPPUNIT_ASSERT(0.0 < tArticle->mSections[0].mStrings[0].getMpp().mPower);
        CPPUNIT_ASSERT(0.0 < tArticle->mSections[0].mStrings[0].getMpp().mCurrent);
        CPPUNIT_ASSERT(0.0 < tArticle->mSections[0].mStrings[0].getMpp().mVoltage);

        const double expectedSol  = (tArticle->mSections[0].getPercentInsolation()
                                   + tArticle->mSections[1].getPercentInsolation()
                                   + tArticle->mSections[2].getPercentInsolation()) / tNumSections;
        const double expectedIsc  = tNumStrings * tArticle->mSections[0].mStrings[0].getShortCircuitCurrent();
        const double expectedVoc  = tArticle->mSections[0].mStrings[0].getOpenCircuitVoltage();
        const double expectedIivc = tNumStrings * tArticle->mSections[0].mStrings[0].getMpp().mCurrent;
        const double expectedVivc = tArticle->mSections[0].mStrings[0].getMpp().mVoltage;
        const double expectedImpp = expectedIivc;
        const double expectedVmpp = expectedVivc;
        const double expectedPmpp = tNumStrings * tArticle->mSections[0].mStrings[0].getMpp().mPower;
        const double expectedGmpp = expectedImpp / expectedVmpp;
        const double expectedA    = (expectedIsc - expectedIivc) / expectedVivc;
        const double expectedW    = expectedA * expectedVivc;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSol,  tArticle->mPercentInsolation,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsc,  tArticle->mShortCircuitCurrent, FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoc,  tArticle->mOpenCircuitVoltage,  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIivc, tArticle->mIvCornerCurrent,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVivc, tArticle->mIvCornerVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImpp, tArticle->mMpp.mCurrent,        FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVmpp, tArticle->mMpp.mVoltage,        DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPmpp, tArticle->mMpp.mPower,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGmpp, tArticle->mMpp.mConductance,    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA,    tArticle->mAdmittanceMatrix[0], FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,    tArticle->mSourceVector[0],     FLT_EPSILON);
        CPPUNIT_ASSERT(tArticle->needAdmittanceUpdate());
    } {
        /// @test    Mismatched strings, open-circuit side.
        tArticle->mSections[0].setSourceExposedFraction(1.0);
        tArticle->mSections[1].setSourceExposedFraction(0.6666);
        tArticle->mSections[2].setSourceExposedFraction(0.3333);
        tArticle->mOpenCircuitSide = true;

        tArticle->step(0.0);

        CPPUNIT_ASSERT(tArticle->mSections[1].getPercentInsolation()              < tArticle->mSections[0].getPercentInsolation());
        CPPUNIT_ASSERT(tArticle->mSections[1].mStrings[0].getOpenCircuitVoltage() < tArticle->mSections[0].mStrings[0].getOpenCircuitVoltage());

        const double expectedSol  = (tArticle->mSections[0].getPercentInsolation()
                                   + tArticle->mSections[1].getPercentInsolation()
                                   + tArticle->mSections[2].getPercentInsolation()) / tNumSections;
        const double expectedIsc  = tNumStrings / tNumSections *
                                   (tArticle->mSections[0].mStrings[0].getShortCircuitCurrent()
                                  + tArticle->mSections[1].mStrings[0].getShortCircuitCurrent()
                                  + tArticle->mSections[2].mStrings[0].getShortCircuitCurrent());
        const double expectedVoc  = tArticle->mSections[0].mStrings[0].getOpenCircuitVoltage();
        const double expectedIivc = tArticle->mSections[0].mStrings[0].getMpp().mCurrent
                                  * tNumStrings / tNumSections;
        const double expectedVivc = tArticle->mSections[0].mStrings[0].getMpp().mVoltage;
        const double expectedVmpp = 0.5 * expectedIsc * expectedVivc / (expectedIsc - expectedIivc);
        const double expectedPmpp = expectedVmpp * (expectedIsc
                                  - (expectedIsc - expectedIivc) * expectedVmpp / expectedVivc);
        const double expectedImpp = expectedPmpp / expectedVmpp;
        const double expectedGmpp = expectedImpp / expectedVmpp;
        const double expectedA    = expectedIivc / (expectedVoc - expectedVivc);
        const double expectedW    = expectedA * expectedVoc;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSol,  tArticle->mPercentInsolation,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsc,  tArticle->mShortCircuitCurrent, FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoc,  tArticle->mOpenCircuitVoltage,  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIivc, tArticle->mIvCornerCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVivc, tArticle->mIvCornerVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVmpp, tArticle->mMpp.mVoltage,        FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPmpp, tArticle->mMpp.mPower,          FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImpp, tArticle->mMpp.mCurrent,        FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGmpp, tArticle->mMpp.mConductance,    FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA,    tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,    tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(tArticle->needAdmittanceUpdate());
    } {
        /// @test    All strings shaded.
        tArticle->mSections[0].setSourceExposedFraction(0.0);
        tArticle->mSections[1].setSourceExposedFraction(0.0);
        tArticle->mSections[2].setSourceExposedFraction(0.0);

        tArticle->step(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPercentInsolation,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mShortCircuitCurrent, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mOpenCircuitVoltage,  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mIvCornerCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mIvCornerVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMpp.mVoltage,        DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMpp.mPower,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMpp.mCurrent,        DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMpp.mConductance,    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(tArticle->needAdmittanceUpdate());
    } {
        /// @test    All strings zero light source.
        tArticle->mSections[0].setSourceExposedFraction(1.0);
        tArticle->mSections[1].setSourceExposedFraction(1.0);
        tArticle->mSections[2].setSourceExposedFraction(1.0);
        tArticle->mSections[0].setSourceFluxMagnitude(0.0);
        tArticle->mSections[1].setSourceFluxMagnitude(0.0);
        tArticle->mSections[2].setSourceFluxMagnitude(0.0);

        tArticle->step(0.0);

        const double expectedVoc  = tArticle->mSections[0].mStrings[0].getOpenCircuitVoltage();

        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mPercentInsolation,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mShortCircuitCurrent, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoc,  tArticle->mOpenCircuitVoltage,  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mIvCornerCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mIvCornerVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mMpp.mVoltage,        DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mMpp.mPower,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mMpp.mCurrent,        DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mMpp.mConductance,    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT(!tArticle->needAdmittanceUpdate());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Array Link getter and setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testAccessors()
{
    UT_RESULT;

    /// @test    Link is non-linear.
    CPPUNIT_ASSERT(true  == tArticle->isNonLinear());

    /// @test    Can set common strings output flag.
    tArticle->setCommonStringsOutput(true);
    CPPUNIT_ASSERT(true == tArticle->mCommonStringsOutput);

    /// @test    Can get number of sections.
    tArticle->mConfig.mNumSections = 5;
    CPPUNIT_ASSERT(5 == tArticle->getNumSections());

    /// @test    Can get number of strings.
    tArticle->mConfig.mNumStrings = 15;
    CPPUNIT_ASSERT(15 == tArticle->getNumStrings());

    /// @test    Can get MPP state.
    tArticle->mMpp.mVoltage = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->getMpp().mVoltage, DBL_EPSILON);

    /// @test    Can get terminal state.
    tArticle->mTerminal.mVoltage = 2.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, tArticle->getTerminal().mVoltage, DBL_EPSILON);

    /// @test    Can get short-circuit current.
    tArticle->mShortCircuitCurrent = 3.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, tArticle->getShortCircuitCurrent(), DBL_EPSILON);

    /// @test    Can get open-circuit voltage.
    tArticle->mOpenCircuitVoltage = 4.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, tArticle->getOpenCircuitVoltage(), DBL_EPSILON);

    /// @test    Can get I-V corner voltage.
    tArticle->mIvCornerVoltage = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, tArticle->getIvCornerVoltage(), DBL_EPSILON);

    /// @test    Can get I-V corner current.
    tArticle->mIvCornerCurrent = 6.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, tArticle->getIvCornerCurrent(), DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the confirmSolutionAcceptable method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testConfirmSolutionAcceptable()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// - Step the article to update a realistic state.
    tArticle->setCommonStringsOutput(true);
    tArticle->mOpenCircuitSide = false;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(tArticle->needAdmittanceUpdate());
    double admittance = tArticle->mAdmittanceMatrix[0];
    double source     = tArticle->mSourceVector[0];

    /// @test    When solution voltage remains on the previous side of the I-V corner, the link
    ///          confirms.
    tArticle->mPotentialVector[0] = tArticle->mIvCornerVoltage * 0.99;

    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(0, 1));
    CPPUNIT_ASSERT(!tArticle->mOpenCircuitSide);

    /// @test    When solution voltage moves to other side of the I-V curve, the link rejects.
    tArticle->mPotentialVector[0] = tArticle->mIvCornerVoltage * 1.01;

    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(0, 2));
    CPPUNIT_ASSERT(tArticle->mOpenCircuitSide);

    /// @test    Link rejects when moving back the original side of the I-V curve.
    tArticle->mPotentialVector[0] = tArticle->mIvCornerVoltage * 0.99;

    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == tArticle->confirmSolutionAcceptable(0, 3));
    CPPUNIT_ASSERT(!tArticle->mOpenCircuitSide);

    /// @test    Always confirms when strings aren't tied to common output.
    tArticle->setCommonStringsOutput(false);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(0, 1));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeFlows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// - Step the article to set a realistic state.
    tArticle->step(0.0);

    /// @test    Flow outputs, and strings are not loaded when not connected to common output.
    const double terminalVoltage = tArticle->mIvCornerVoltage * 0.5;
    const double expectedDp      = -terminalVoltage;
    const double expectedFlux    = tArticle->mSourceVector[0] - tArticle->mAdmittanceMatrix[0]
                                 * terminalVoltage;
    const double expectedPower   = expectedFlux * terminalVoltage;

    tArticle->setCommonStringsOutput(false);
    tArticle->mPotentialVector[0] = terminalVoltage;

    tArticle->computeFlows(0.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp,    tArticle->mPotentialDrop, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower, tArticle->mPower,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tNodes[0].getInflux(),    DBL_EPSILON);
    CPPUNIT_ASSERT(0.0 == tArticle->mSections[0].mStrings[0].getTerminal().mVoltage);
    CPPUNIT_ASSERT(0.0 == tArticle->mSections[0].mStrings[0].getTerminal().mPower);
    CPPUNIT_ASSERT(0.0 == tArticle->mSections[0].mStrings[0].getTerminal().mCurrent);
    CPPUNIT_ASSERT(0.0 == tArticle->mSections[0].mStrings[0].getTerminal().mConductance);

    /// @test    Strings are loaded at the non-Ground output node voltage.
    tArticle->setCommonStringsOutput(true);

    tArticle->computeFlows(0.0);

    CPPUNIT_ASSERT(terminalVoltage == tArticle->mSections[0].mStrings[0].getTerminal().mVoltage);
    CPPUNIT_ASSERT(0.0 < tArticle->mSections[0].mStrings[0].getTerminal().mPower);
    CPPUNIT_ASSERT(0.0 < tArticle->mSections[0].mStrings[0].getTerminal().mCurrent);
    CPPUNIT_ASSERT(0.0 < tArticle->mSections[0].mStrings[0].getTerminal().mConductance);

    /// @test    Strings are loaded at the array terminal voltage when connected to Ground.
    tArticle->setCommonStringsOutput(true);
    tArticle->mTerminal.mVoltage = tArticle->mMpp.mVoltage;

    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
    tArticle->step(0.0);

    tArticle->computeFlows(0.0);

    CPPUNIT_ASSERT(tArticle->mMpp.mVoltage == tArticle->mSections[0].mStrings[0].getTerminal().mVoltage);
    CPPUNIT_ASSERT(0.0 < tArticle->mSections[0].mStrings[0].getTerminal().mPower);
    CPPUNIT_ASSERT(0.0 < tArticle->mSections[0].mStrings[0].getTerminal().mCurrent);
    CPPUNIT_ASSERT(0.0 < tArticle->mSections[0].mStrings[0].getTerminal().mConductance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the predictLoadAtVoltage method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testLoadAtVoltage()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// - Step the article to set a realistic state.
    tArticle->step(0.0);

    {
        /// @test    Short-circuit side of the array I-V curve.
        const double voltage = 0.5 * tArticle->mIvCornerVoltage;
        const double expectedI = tArticle->mIvCornerCurrent
                + 0.5 * (tArticle->mShortCircuitCurrent - tArticle->mIvCornerCurrent);
        const double expectedP = voltage * expectedI;
        const double expectedG = expectedI / voltage;

        double resultP, resultG;
        tArticle->predictLoadAtVoltage(resultP, resultG, voltage);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, resultP, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, resultG, FLT_EPSILON);
    } {
        /// @test    Open-circuit side of the array I-V curve.
        const double voltage   = tArticle->mIvCornerVoltage
                               + 0.5 * (tArticle->mOpenCircuitVoltage - tArticle->mIvCornerVoltage);
        const double expectedI = 0.5 * tArticle->mIvCornerCurrent;
        const double expectedP = voltage * expectedI;
        const double expectedG = expectedI / voltage;

        double resultP, resultG;
        tArticle->predictLoadAtVoltage(resultP, resultG, voltage);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, resultP, FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, resultG, FLT_EPSILON);
    } {
        /// @test    Voltage higher than max.
        double resultP, resultG;
        tArticle->predictLoadAtVoltage(resultP, resultG, tArticle->mOpenCircuitVoltage * 1.01);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, resultP, FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, resultG, FLT_EPSILON);
    } {
        /// @test    Zero voltage.
        double resultP, resultG;
        tArticle->predictLoadAtVoltage(resultP, resultG, 0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, resultP, FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, resultG, FLT_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the loadAtPower method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvArray::testLoadAtPower()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// - Step the article to set a realistic state.
    tArticle->step(0.0);

    {
        /// @test    Terminal outputs on open-circuit side of the I-V curve.
        const double expectedP = 0.5 * tArticle->mMpp.mPower;
        const bool   shortSide = false;

        tArticle->loadAtPower(expectedP, shortSide);

        const double actualP   = tArticle->mTerminal.mVoltage * tArticle->mTerminal.mCurrent;
        const double expectedG = tArticle->mTerminal.mCurrent / tArticle->mTerminal.mVoltage;
        const double factorI   = tArticle->mTerminal.mCurrent / tArticle->mIvCornerCurrent;
        const double expectedV = tArticle->mOpenCircuitVoltage - factorI
                               * (tArticle->mOpenCircuitVoltage -tArticle->mIvCornerVoltage);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, actualP,                          FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, FLT_EPSILON);
        CPPUNIT_ASSERT(factorI > 0.0);
        CPPUNIT_ASSERT(factorI < 1.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     FLT_EPSILON);
    } {
        /// @test    Terminal outputs on short-circuit side of the I-V curve.
        const double expectedP = 0.5 * tArticle->mMpp.mPower;
        const bool   shortSide = true;

        tArticle->loadAtPower(expectedP, shortSide);

        const double actualP   = tArticle->mTerminal.mVoltage * tArticle->mTerminal.mCurrent;
        const double expectedG = tArticle->mTerminal.mCurrent / tArticle->mTerminal.mVoltage;
        const double factorV   = tArticle->mTerminal.mVoltage / tArticle->mIvCornerVoltage;
        const double expectedI = tArticle->mShortCircuitCurrent - factorV
                               * (tArticle->mShortCircuitCurrent -tArticle->mIvCornerCurrent);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, actualP,                          FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, FLT_EPSILON);
        CPPUNIT_ASSERT(factorV > 0.0);
        CPPUNIT_ASSERT(factorV < 1.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     FLT_EPSILON);
    } {
        /// @test    Terminal outputs given zero power load.
        const double expectedP = 0.0;
        const bool   shortSide = true;

        tArticle->loadAtPower(expectedP, shortSide);

        const double expectedV = tArticle->mOpenCircuitVoltage;
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

        tArticle->loadAtPower(1.0E15, shortSide);

        const double expectedV = 0.0;
        const double expectedI = 0.0;
        const double expectedG = 0.0;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, DBL_EPSILON);
    } {
        /// @test    Terminal outputs with no active strings.
        tArticle->mSections[0].setSourceExposedFraction(0.0);
        tArticle->mSections[1].setSourceExposedFraction(0.0);
        tArticle->mSections[2].setSourceExposedFraction(0.0);
        tArticle->mSections[0].setSourceFluxMagnitude(0.0);
        tArticle->mSections[1].setSourceFluxMagnitude(0.0);
        tArticle->mSections[2].setSourceFluxMagnitude(0.0);
        tArticle->step(0.0);

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

    UT_PASS_LAST;
}
