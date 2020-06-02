/*
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/thermal/GunnsThermoelectricEffect.o))
*/

#include "UtGunnsThermoelectricEffect.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsThermoelectricEffect::TEST_ID = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermoelectricEffect class.
///////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermoelectricEffect::UtGunnsThermoelectricEffect()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tName(),
    tNumThermocouples(),
    tGeometryFactor(),
    tElectricalResistivityCoeff(),
    tSeebeckCoeff(),
    tThermalConductivityCoeff(),
    tEndPlateThermalConductance(),
    tMinTemperature(),
    tMaxTemperature(),
    tMalfThermoelectricEffectsFlag(),
    tMalfThermoelectricEffectsScalar(),
    tCurrent(),
    tTemperatureHot(),
    tTemperatureCold()
{
    // Nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsThermoelectricEffect class.
///////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermoelectricEffect::~UtGunnsThermoelectricEffect()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricEffect::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricEffect::setUp()
{
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
    tConfigData = new GunnsThermoelectricEffectConfigData(tNumThermocouples,
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
    tMalfThermoelectricEffectsFlag   = true;
    tMalfThermoelectricEffectsScalar = 0.1;
    tCurrent                         = 1.0;
    tTemperatureHot                  = 400.0;
    tTemperatureCold                 = 300.0;
    tInputData = new GunnsThermoelectricEffectInputData(tMalfThermoelectricEffectsFlag,
                                                        tMalfThermoelectricEffectsScalar,
                                                        tCurrent,
                                                        tTemperatureHot,
                                                        tTemperatureCold);

    /// - Define the test Article.
    tName    = "tArticle";
    tArticle = new FriendlyGunnsThermoelectricEffect();

    /// - Increment the test identification number.
    ++TEST_ID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricEffect::testConfig()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tNumThermocouples              == tConfigData->mNumThermocouples);
    CPPUNIT_ASSERT(tGeometryFactor                == tConfigData->mGeometryFactor);
    CPPUNIT_ASSERT(tElectricalResistivityCoeff[0] == tConfigData->mElectricalResistivityCoeff[0]);
    CPPUNIT_ASSERT(tElectricalResistivityCoeff[1] == tConfigData->mElectricalResistivityCoeff[1]);
    CPPUNIT_ASSERT(tSeebeckCoeff[0]               == tConfigData->mSeebeckCoeff[0]);
    CPPUNIT_ASSERT(tSeebeckCoeff[1]               == tConfigData->mSeebeckCoeff[1]);
    CPPUNIT_ASSERT(tSeebeckCoeff[2]               == tConfigData->mSeebeckCoeff[2]);
    CPPUNIT_ASSERT(tThermalConductivityCoeff[0]   == tConfigData->mThermalConductivityCoeff[0]);
    CPPUNIT_ASSERT(tThermalConductivityCoeff[1]   == tConfigData->mThermalConductivityCoeff[1]);
    CPPUNIT_ASSERT(tThermalConductivityCoeff[2]   == tConfigData->mThermalConductivityCoeff[2]);
    CPPUNIT_ASSERT(tEndPlateThermalConductance    == tConfigData->mEndPlateThermalConductance);
    CPPUNIT_ASSERT(tMinTemperature                == tConfigData->mMinTemperature);
    CPPUNIT_ASSERT(tMaxTemperature                == tConfigData->mMaxTemperature);

    /// - Check default config construction
    GunnsThermoelectricEffectConfigData defaultConfig;
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mNumThermocouples);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mGeometryFactor);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mElectricalResistivityCoeff[0]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mElectricalResistivityCoeff[1]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mSeebeckCoeff[0]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mSeebeckCoeff[1]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mSeebeckCoeff[2]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermalConductivityCoeff[0]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermalConductivityCoeff[1]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mThermalConductivityCoeff[2]);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mEndPlateThermalConductance);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mMinTemperature);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mMaxTemperature);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricEffect::testInput()
{
    UT_RESULT;

    /// - Check nominal input construction
    CPPUNIT_ASSERT(tMalfThermoelectricEffectsFlag   == tInputData->mMalfThermoelectricEffectsFlag);
    CPPUNIT_ASSERT(tMalfThermoelectricEffectsScalar == tInputData->mMalfThermoelectricEffectsScalar);
    CPPUNIT_ASSERT(tCurrent                         == tInputData->mCurrent);
    CPPUNIT_ASSERT(tTemperatureHot                  == tInputData->mTemperatureHot);
    CPPUNIT_ASSERT(tTemperatureCold                 == tInputData->mTemperatureCold);

    /// - Check default input construction
    GunnsThermoelectricEffectInputData defaultInput;
    CPPUNIT_ASSERT(false                            == defaultInput.mMalfThermoelectricEffectsFlag);
    CPPUNIT_ASSERT(0.0                              == defaultInput.mMalfThermoelectricEffectsScalar);
    CPPUNIT_ASSERT(0.0                              == defaultInput.mCurrent);
    CPPUNIT_ASSERT(0.0                              == defaultInput.mTemperatureHot);
    CPPUNIT_ASSERT(0.0                              == defaultInput.mTemperatureCold);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricEffect::testDefaultConstruction()
{
    UT_RESULT;

    /// @test Default values on construction
    CPPUNIT_ASSERT(false == tArticle->mMalfThermoelectricEffectsFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfThermoelectricEffectsScalar);
    CPPUNIT_ASSERT(""    == tArticle->mName);
    CPPUNIT_ASSERT(0.0   == tArticle->mResistanceCoeffs[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mResistanceCoeffs[1]);
    CPPUNIT_ASSERT(0.0   == tArticle->mSeebeckCoeffs[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mSeebeckCoeffs[1]);
    CPPUNIT_ASSERT(0.0   == tArticle->mSeebeckCoeffs[2]);
    CPPUNIT_ASSERT(0.0   == tArticle->mThermalConductanceCoeffs[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mThermalConductanceCoeffs[1]);
    CPPUNIT_ASSERT(0.0   == tArticle->mThermalConductanceCoeffs[2]);
    CPPUNIT_ASSERT(0.0   == tArticle->mMinTemperature);
    CPPUNIT_ASSERT(0.0   == tArticle->mMaxTemperature);
    CPPUNIT_ASSERT(0.0   == tArticle->mCurrent);
    CPPUNIT_ASSERT(0.0   == tArticle->mTemperatureHot);
    CPPUNIT_ASSERT(0.0   == tArticle->mTemperatureCold);
    CPPUNIT_ASSERT(0.0   == tArticle->mThermalConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mHeatFluxHot);
    CPPUNIT_ASSERT(0.0   == tArticle->mHeatFluxCold);
    CPPUNIT_ASSERT(0.0   == tArticle->mHeatFluxThru);
    CPPUNIT_ASSERT(0.0   == tArticle->mElectricalConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mVoltage);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for initialization without exceptions.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricEffect::testInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tName));

    /// @test name.
    CPPUNIT_ASSERT(tName == tArticle->mName);

    /// @test config data.
    const double twoN      = 2.0 * tNumThermocouples;
    const double twoNG     = twoN * tGeometryFactor;
    const double twoNoverG = twoN / tGeometryFactor;
    const double K0 = 1.0 / (1.0/twoNG/tThermalConductivityCoeff[0] + 2.0/tEndPlateThermalConductance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(twoNoverG * tElectricalResistivityCoeff[0], tArticle->mResistanceCoeffs[0],         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(twoNoverG * tElectricalResistivityCoeff[1], tArticle->mResistanceCoeffs[1],         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(twoN      * tSeebeckCoeff[0],               tArticle->mSeebeckCoeffs[0],            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(twoN      * tSeebeckCoeff[1],               tArticle->mSeebeckCoeffs[1],            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(twoN      * tSeebeckCoeff[2],               tArticle->mSeebeckCoeffs[2],            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(K0,                                         tArticle->mThermalConductanceCoeffs[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(twoNG     * tThermalConductivityCoeff[1],   tArticle->mThermalConductanceCoeffs[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(twoNG     * tThermalConductivityCoeff[2],   tArticle->mThermalConductanceCoeffs[2], DBL_EPSILON);
    CPPUNIT_ASSERT(tMinTemperature == tArticle->mMinTemperature);
    CPPUNIT_ASSERT(tMaxTemperature == tArticle->mMaxTemperature);

    /// @test input data.
    CPPUNIT_ASSERT(tMalfThermoelectricEffectsFlag   == tArticle->mMalfThermoelectricEffectsFlag);
    CPPUNIT_ASSERT(tMalfThermoelectricEffectsScalar == tArticle->mMalfThermoelectricEffectsScalar);
    CPPUNIT_ASSERT(tCurrent                         == tArticle->mCurrent);
    CPPUNIT_ASSERT(tTemperatureHot                  == tArticle->mTemperatureHot);
    CPPUNIT_ASSERT(tTemperatureCold                 == tArticle->mTemperatureCold);

    /// @test initial state got updated.  We'll do detailed values checks later.
    CPPUNIT_ASSERT(0.0 < tArticle->mVoltage);

    /// @test init flag.
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    /// @test init of thermal conductance coeffs with zero end-plate conudctance.
    tConfigData->mEndPlateThermalConductance = 0.0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tName));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(twoNG     * tThermalConductivityCoeff[0],   tArticle->mThermalConductanceCoeffs[0], DBL_EPSILON);
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for initialization with exceptions.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricEffect::testInitializationExceptions()
{
    UT_RESULT;

    /// @test throw on # thermocouples too low.
    tConfigData->mNumThermocouples = 0.499;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tConfigData->mNumThermocouples = tNumThermocouples;

    /// @test throw on geometry factor too low.
    tConfigData->mGeometryFactor = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tConfigData->mGeometryFactor = tGeometryFactor;

    /// @test throw on end-plate thermal conductance too low.
    tConfigData->mEndPlateThermalConductance = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tConfigData->mEndPlateThermalConductance = tEndPlateThermalConductance;

    /// @test throw on minimum temperature too low.
    tConfigData->mMinTemperature = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tConfigData->mMinTemperature = tMinTemperature;

    /// @test throw on maximum temperature too low.
    tConfigData->mMaxTemperature = tMinTemperature;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tConfigData->mMaxTemperature = tMaxTemperature;

    /// @test throw on hot-side temperature too low.
    tInputData->mTemperatureHot = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tInputData->mTemperatureHot = tTemperatureHot;

    /// @test throw on cold-side temperature too low.
    tInputData->mTemperatureCold = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tInputData->mTemperatureCold = tTemperatureCold;

    /// @test throw on effects malf scale factor too low.
    tInputData->mMalfThermoelectricEffectsScalar = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tName), TsInitializationException);
    tInputData->mMalfThermoelectricEffectsScalar = tMalfThermoelectricEffectsScalar;

    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the restart method.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricEffect::testRestart()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tName));

    /// - Store initial voltage prior to restart.
    const double initVoltage = tArticle->mVoltage;

    /// @test restart method updates model state.
    tArticle->mTemperatureHot = tTemperatureCold + 1.0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->restart());
    CPPUNIT_ASSERT(initVoltage > tArticle->mVoltage);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the update method.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricEffect::testUpdate()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tName));

    /// @test nominal update with dT & malf, with hot & cold upper/lower limits.
    tArticle->mTemperatureHot  = 1000.0;
    tArticle->mTemperatureCold =    0.0;

    double dT    = tMaxTemperature - tMinTemperature;
    double dT2   = tMaxTemperature * tMaxTemperature
                 - tMinTemperature * tMinTemperature;
    double dT3   = tMaxTemperature * tMaxTemperature * tMaxTemperature
                 - tMinTemperature * tMinTemperature * tMinTemperature;
    double avgT  = 0.5 * (tMaxTemperature + tMinTemperature);
    double Ke    = 1.0 / (tArticle->mResistanceCoeffs[0] + tArticle->mResistanceCoeffs[1] * avgT);
    double Kt    = tArticle->mThermalConductanceCoeffs[0]
                 + tArticle->mThermalConductanceCoeffs[1] * dT2/dT/2.0
                 + tArticle->mThermalConductanceCoeffs[2] * dT3/dT/3.0;
    double Savg  = tMalfThermoelectricEffectsScalar
                 * (tArticle->mSeebeckCoeffs[0]
                  + tArticle->mSeebeckCoeffs[1] * dT2/dT/2.0
                  + tArticle->mSeebeckCoeffs[2] * dT3/dT/3.0);
    double Shot  = tMalfThermoelectricEffectsScalar
                 * (tArticle->mSeebeckCoeffs[0]
                  + tArticle->mSeebeckCoeffs[1] * tMaxTemperature
                  + tArticle->mSeebeckCoeffs[2] * tMaxTemperature * tMaxTemperature);
    double Scold = tMalfThermoelectricEffectsScalar
                 * (tArticle->mSeebeckCoeffs[0]
                  + tArticle->mSeebeckCoeffs[1] * tMinTemperature
                  + tArticle->mSeebeckCoeffs[2] * tMinTemperature * tMinTemperature);
    double Qthru = Kt * 1000.0;
    double Qj    = tCurrent * tCurrent / Ke;
    double Qph   = tCurrent * tMaxTemperature * Shot;
    double Qpc   = tCurrent * tMinTemperature * Scold;
    double Qhot  = 0.5 * Qj - Qph;
    double Qcold = 0.5 * Qj + Qpc;
    double V     = Savg * dT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->update());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Ke,    tArticle->mElectricalConductance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Kt,    tArticle->mThermalConductance,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Qthru, tArticle->mHeatFluxThru,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Qhot,  tArticle->mHeatFluxHot,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Qcold, tArticle->mHeatFluxCold,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(V,     tArticle->mVoltage,               DBL_EPSILON);

    /// @test update with dT & zero-limited malf, with cold & hot upper/lower limits.
    tArticle->mTemperatureHot                  =    0.0;
    tArticle->mTemperatureCold                 = 1000.0;
    tArticle->mMalfThermoelectricEffectsScalar =   -1.0;

    dT    = tMinTemperature - tMaxTemperature;
    dT2   = tMinTemperature * tMinTemperature
          - tMaxTemperature * tMaxTemperature;
    dT3   = tMinTemperature * tMinTemperature * tMinTemperature
          - tMaxTemperature * tMaxTemperature * tMaxTemperature;
    Kt    = tArticle->mThermalConductanceCoeffs[0]
          + tArticle->mThermalConductanceCoeffs[1] * dT2/dT/2.0
          + tArticle->mThermalConductanceCoeffs[2] * dT3/dT/3.0;
    Savg  = 0.0;
    Shot  = 0.0;
    Scold = 0.0;
    Qthru = Kt * -1000.0;
    Qph   = tCurrent * tMinTemperature * Shot;
    Qpc   = tCurrent * tMaxTemperature * Scold;
    Qhot  = 0.5 * Qj - Qph;
    Qcold = 0.5 * Qj + Qpc;
    V     = Savg * dT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->update());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Ke,    tArticle->mElectricalConductance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Kt,    tArticle->mThermalConductance,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Qthru, tArticle->mHeatFluxThru,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Qhot,  tArticle->mHeatFluxHot,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Qcold, tArticle->mHeatFluxCold,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(V,     tArticle->mVoltage,               DBL_EPSILON);

    /// @test update with zero dT, malf off.
    tArticle->mTemperatureHot                = tTemperatureCold;
    tArticle->mTemperatureCold               = tTemperatureCold;
    tArticle->mMalfThermoelectricEffectsFlag = false;

    avgT  = tTemperatureCold;
    Ke    = 1.0 / (tArticle->mResistanceCoeffs[0] + tArticle->mResistanceCoeffs[1] * avgT);
    Kt    = tArticle->mThermalConductanceCoeffs[0];
    Shot  = tArticle->mSeebeckCoeffs[0]
          + tArticle->mSeebeckCoeffs[1] * tTemperatureCold
          + tArticle->mSeebeckCoeffs[2] * tTemperatureCold * tTemperatureCold;
    Scold = Shot;
    Qthru = 0.0;
    Qj    = tCurrent * tCurrent / Ke;
    Qph   = tCurrent * tTemperatureCold * Shot;
    Qpc   = Qph;
    Qhot  = 0.5 * Qj - Qph;
    Qcold = 0.5 * Qj + Qpc;
    V     = 0.0;

    CPPUNIT_ASSERT_NO_THROW(tArticle->update());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Ke,    tArticle->mElectricalConductance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Kt,    tArticle->mThermalConductance,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Qthru, tArticle->mHeatFluxThru,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Qhot,  tArticle->mHeatFluxHot,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Qcold, tArticle->mHeatFluxCold,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(V,     tArticle->mVoltage,               DBL_EPSILON);

    UT_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the setter & getter methods.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermoelectricEffect::testAccessors()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tName));

    /// @test setTemperatureHot method.
    tArticle->setTemperatureHot(375.0);
    CPPUNIT_ASSERT(375.0 == tArticle->mTemperatureHot);

    /// @test setTemperatureCold method.
    tArticle->setTemperatureCold(325.0);
    CPPUNIT_ASSERT(325.0 == tArticle->mTemperatureCold);

    /// @test setMalfThermoelectricEffects method to activate malf.
    tArticle->setMalfThermoelectricEffects(true, 0.5);
    CPPUNIT_ASSERT(true  == tArticle->mMalfThermoelectricEffectsFlag);
    CPPUNIT_ASSERT(0.5   == tArticle->mMalfThermoelectricEffectsScalar);

    /// @test setMalfThermoelectricEffects method resets malf with default args.
    tArticle->setMalfThermoelectricEffects();
    CPPUNIT_ASSERT(false == tArticle->mMalfThermoelectricEffectsFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfThermoelectricEffectsScalar);

    /// @test getThermalConductance method.
    CPPUNIT_ASSERT(tArticle->mThermalConductance    == tArticle->getThermalConductance());

    /// @test getHeatFluxHot method.
    CPPUNIT_ASSERT(tArticle->mHeatFluxHot           == tArticle->getHeatFluxHot());

    /// @test getHeatFluxCold method.
    CPPUNIT_ASSERT(tArticle->mHeatFluxCold          == tArticle->getHeatFluxCold());

    /// @test getHeatFluxThru method.
    CPPUNIT_ASSERT(tArticle->mHeatFluxThru          == tArticle->getHeatFluxThru());

    /// @test getElectricalConductance method.
    CPPUNIT_ASSERT(tArticle->mElectricalConductance == tArticle->getElectricalConductance());

    /// @test getVoltage method.
    CPPUNIT_ASSERT(tArticle->mVoltage               == tArticle->getVoltage());

    /// @test isInitialized method.
    CPPUNIT_ASSERT(tArticle->mInitFlag              == tArticle->isInitialized());

    UT_PASS_LAST;
}
