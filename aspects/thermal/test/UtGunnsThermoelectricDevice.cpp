/*
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/thermal/GunnsThermoelectricDevice.o))
*/

#include "UtGunnsThermoelectricDevice.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"
#include "UtGunnsThermoelectricEffect.hh"

/// @details  Test identification number.
int UtGunnsThermoelectricDevice::TEST_ID = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermoelectricDevice class.
///////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermoelectricDevice::UtGunnsThermoelectricDevice()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tArticleEffect(),
    tName(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tNumThermocouples(),
    tGeometryFactor(),
    tElectricalResistivityCoeff(),
    tSeebeckCoeff(),
    tThermalConductivityCoeff(),
    tEndPlateThermalConductance(),
    tMinTemperature(),
    tMaxTemperature(),
    tMalfBlockageFlag(),
    tMalfBlockageValue(),
    tMalfThermoelectricEffectsFlag(),
    tMalfThermoelectricEffectsScalar(),
    tPort0(),
    tPort1(),
    tTimeStep()
{
    // Nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsThermoelectricDevice class.
///////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermoelectricDevice::~UtGunnsThermoelectricDevice()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricDevice::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricDevice::setUp()
{
    /// - Declare the standard Gunns Link test data.
    tName                          = "tArticle";
    tNodeList.mNumNodes            = 2;
    tNodeList.mNodes               = tNodes;
    tPort0                         = 0;
    tPort1                         = 1;
    tTimeStep                      = 0.1;

    /// - Initialize nodes.
    tNodes[0].initialize("tNodes_0", 400.0);
    tNodes[1].initialize("tNodes_1", 300.0);

    /// - Define nominal configuration data.
    tNumThermocouples              =  127.0;
    tGeometryFactor                =  0.00118;
    tElectricalResistivityCoeff[0] = -2.332e-6;
    tElectricalResistivityCoeff[1] =  4.251e-8;
    tSeebeckCoeff[0]               =  1.0e-5;
    tSeebeckCoeff[1]               =  5.395e-7;
    tSeebeckCoeff[2]               = -7.895e-10;
    tThermalConductivityCoeff[0]   =  4.441;
    tThermalConductivityCoeff[1]   = -1.768e-2;
    tThermalConductivityCoeff[2]   =  2.672e-5;
    tEndPlateThermalConductance    =  8.366;
    tMinTemperature                =  273.0;
    tMaxTemperature                =  475.0;
    tConfigData = new GunnsThermoelectricDeviceConfigData(tName, &tNodeList,
                                                          tNumThermocouples,
                                                          tGeometryFactor,
                                                          tElectricalResistivityCoeff[0],
                                                          tElectricalResistivityCoeff[1],
                                                          tSeebeckCoeff[0],
                                                          tSeebeckCoeff[1],
                                                          tSeebeckCoeff[2],
                                                          tThermalConductivityCoeff[0],
                                                          tThermalConductivityCoeff[1],
                                                          tThermalConductivityCoeff[2],
                                                          tEndPlateThermalConductance,
                                                          tMinTemperature,
                                                          tMaxTemperature);

    /// - Define nominal input data.
    tMalfBlockageFlag                = true;
    tMalfBlockageValue               = 0.5;
    tMalfThermoelectricEffectsFlag   = true;
    tMalfThermoelectricEffectsScalar = 0.1;
    tInputData = new GunnsThermoelectricDeviceInputData(tMalfBlockageFlag,
                                                        tMalfBlockageValue,
                                                        tMalfThermoelectricEffectsFlag,
                                                        tMalfThermoelectricEffectsScalar);

    /// - Define the test Article.
    tArticle       = new FriendlyGunnsThermoelectricDevice();
    tArticleEffect = static_cast<FriendlyGunnsThermoelectricEffect*>(&tArticle->mThermoelectricEffect);

    /// - Increment the test identification number.
    ++TEST_ID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricDevice::testConfig()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tName                          == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                         == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tNumThermocouples              == tConfigData->mThermoelectricEffect.mNumThermocouples);
    CPPUNIT_ASSERT(tGeometryFactor                == tConfigData->mThermoelectricEffect.mGeometryFactor);
    CPPUNIT_ASSERT(tElectricalResistivityCoeff[0] == tConfigData->mThermoelectricEffect.mElectricalResistivityCoeff[0]);
    CPPUNIT_ASSERT(tElectricalResistivityCoeff[1] == tConfigData->mThermoelectricEffect.mElectricalResistivityCoeff[1]);
    CPPUNIT_ASSERT(tSeebeckCoeff[0]               == tConfigData->mThermoelectricEffect.mSeebeckCoeff[0]);
    CPPUNIT_ASSERT(tSeebeckCoeff[1]               == tConfigData->mThermoelectricEffect.mSeebeckCoeff[1]);
    CPPUNIT_ASSERT(tSeebeckCoeff[2]               == tConfigData->mThermoelectricEffect.mSeebeckCoeff[2]);
    CPPUNIT_ASSERT(tThermalConductivityCoeff[0]   == tConfigData->mThermoelectricEffect.mThermalConductivityCoeff[0]);
    CPPUNIT_ASSERT(tThermalConductivityCoeff[1]   == tConfigData->mThermoelectricEffect.mThermalConductivityCoeff[1]);
    CPPUNIT_ASSERT(tThermalConductivityCoeff[2]   == tConfigData->mThermoelectricEffect.mThermalConductivityCoeff[2]);
    CPPUNIT_ASSERT(tEndPlateThermalConductance    == tConfigData->mThermoelectricEffect.mEndPlateThermalConductance);
    CPPUNIT_ASSERT(tMinTemperature                == tConfigData->mThermoelectricEffect.mMinTemperature);
    CPPUNIT_ASSERT(tMaxTemperature                == tConfigData->mThermoelectricEffect.mMaxTemperature);

    /// - Check default config construction
    GunnsThermoelectricDeviceConfigData defaultConfig;
    CPPUNIT_ASSERT(""                             == defaultConfig.mName);
    CPPUNIT_ASSERT(0                              == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mNumThermocouples);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mGeometryFactor);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mElectricalResistivityCoeff[0]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mElectricalResistivityCoeff[1]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mSeebeckCoeff[0]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mSeebeckCoeff[1]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mSeebeckCoeff[2]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mThermalConductivityCoeff[0]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mThermalConductivityCoeff[1]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mThermalConductivityCoeff[2]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mEndPlateThermalConductance);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mMinTemperature);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermoelectricEffect.mMaxTemperature);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricDevice::testInput()
{
    UT_RESULT;

    /// - Check nominal input construction
    CPPUNIT_ASSERT(tMalfBlockageFlag                == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue               == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tMalfThermoelectricEffectsFlag   == tInputData->mThermoelectricEffect.mMalfThermoelectricEffectsFlag);
    CPPUNIT_ASSERT(tMalfThermoelectricEffectsScalar == tInputData->mThermoelectricEffect.mMalfThermoelectricEffectsScalar);

    /// - Check default input construction
    GunnsThermoelectricDeviceInputData defaultInput;
    CPPUNIT_ASSERT(false                            == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                              == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false                            == defaultInput.mThermoelectricEffect.mMalfThermoelectricEffectsFlag);
    CPPUNIT_ASSERT(0.0                              == defaultInput.mThermoelectricEffect.mMalfThermoelectricEffectsScalar);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricDevice::testDefaultConstruction()
{
    UT_RESULT;

    /// @test Default values on construction
    CPPUNIT_ASSERT(0.0   == tArticle->mDefaultConductivity);
    CPPUNIT_ASSERT(false == tArticleEffect->mInitFlag);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for initialization without exceptions.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricDevice::testInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test base class init.
    CPPUNIT_ASSERT(tName == tArticle->mName);
    CPPUNIT_ASSERT(tName + ".mThermoelectricEffect" == tArticleEffect->mName);

    /// @test thermoelectric effect object updated its output state.
    CPPUNIT_ASSERT(tNodes[0].getPotential() == tArticleEffect->mTemperatureHot);
    CPPUNIT_ASSERT(tNodes[1].getPotential() == tArticleEffect->mTemperatureCold);
    CPPUNIT_ASSERT(0.0                       < tArticleEffect->mVoltage);

    /// @test init flags.
    CPPUNIT_ASSERT(tArticleEffect->mInitFlag);
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for initialization with exceptions.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricDevice::testInitializationExceptions()
{
    UT_RESULT;

    /// @test a throw from the thermoelectric effect.
    tConfigData->mThermoelectricEffect.mNumThermocouples = 0.499;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mThermoelectricEffect.mNumThermocouples = tNumThermocouples;

    CPPUNIT_ASSERT(false == tArticleEffect->mInitFlag);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the restart method.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricDevice::testRestart()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// - Store initial voltage prior to restart.
    const double initVoltage = tArticleEffect->mVoltage;

    /// - Store non-checkpoint value in the base class.
    tArticle->mSystemConductance = 1.0;

    /// @test restart method updates model state.
    tArticleEffect->setTemperatureHot(301.0);
    CPPUNIT_ASSERT_NO_THROW(tArticle->restart());
    CPPUNIT_ASSERT(initVoltage > tArticleEffect->mVoltage);

    /// @test base class restart.
    CPPUNIT_ASSERT(0.0 == tArticle->mSystemConductance);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the step method
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricDevice::testStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// - Set up initial conditions for step.
    const double initKt = tArticleEffect->mThermalConductance;
    tNodes[tPort0].setPotential(375.0);
    tNodes[tPort1].setPotential(325.0);
    tArticle->mPotentialVector[tPort0] = tNodes[tPort0].getPotential();
    tArticle->mPotentialVector[tPort1] = tNodes[tPort1].getPotential();
    tArticleEffect->mCurrent = 1.0;

    /// @test step method updates the thermoelectric effect & uses its outputs.
    tArticle->step(tTimeStep);

    const double expectedA = tArticleEffect->mThermalConductance * tMalfBlockageValue;
    CPPUNIT_ASSERT_EQUAL( expectedA,                    tArticle->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT_EQUAL(-expectedA,                    tArticle->mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT_EQUAL(-expectedA,                    tArticle->mAdmittanceMatrix[2]);
    CPPUNIT_ASSERT_EQUAL( expectedA,                    tArticle->mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT_EQUAL(tArticleEffect->mHeatFluxHot,  tArticle->mSourceVector[0]);
    CPPUNIT_ASSERT_EQUAL(tArticleEffect->mHeatFluxCold, tArticle->mSourceVector[1]);
    CPPUNIT_ASSERT_EQUAL(tNodes[tPort0].getPotential(), tArticleEffect->mTemperatureHot);
    CPPUNIT_ASSERT_EQUAL(tNodes[tPort1].getPotential(), tArticleEffect->mTemperatureCold);
    CPPUNIT_ASSERT(initKt != tArticleEffect->mThermalConductance);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeFlows method
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricDevice::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// - Set up initial conditions for step & computeFlows.
    tNodes[tPort0].setPotential(375.0);
    tNodes[tPort1].setPotential(325.0);
    tArticle->mPotentialVector[tPort0] = tNodes[tPort0].getPotential();
    tArticle->mPotentialVector[tPort1] = tNodes[tPort1].getPotential();
    tArticleEffect->mCurrent = 1.0;
    tArticle->step(tTimeStep);

    /// @test flux and power outputs.
    const double expectedFlux  = tArticle->mAdmittanceMatrix[0]
                               * (tNodes[tPort0].getPotential() - tNodes[tPort1].getPotential());
    const double expectedPower = expectedFlux + tArticle->mSourceVector[1]
                               - tArticle->mSourceVector[0];
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,  tArticle->mFlux,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower, tArticle->mPower, DBL_EPSILON);

    /// @test transport to the nodes.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[0], tNodes[0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,               tNodes[0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[1] + expectedFlux,
                                                             tNodes[1].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                        tNodes[1].getOutflux(), DBL_EPSILON);

    tNodes[tPort0].resetFlows();
    tNodes[tPort1].resetFlows();
    tNodes[tPort0].setPotential(325.0);
    tNodes[tPort1].setPotential(375.0);
    tArticle->mPotentialVector[tPort0] = tNodes[tPort0].getPotential();
    tArticle->mPotentialVector[tPort1] = tNodes[tPort1].getPotential();
    tArticle->mSourceVector[0] *= -1.0;
    tArticle->mSourceVector[1] *= -1.0;
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,                tNodes[0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-tArticle->mSourceVector[0], tNodes[0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         tNodes[1].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux - tArticle->mSourceVector[1],
                                                              tNodes[1].getOutflux(), DBL_EPSILON);

    UT_PASS_FINAL;
}
