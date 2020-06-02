/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
    (software/exceptions/TsInitializationException.o)
    (../TsPumpMotorController.o)
 )
*/

#include <iostream>
#include "UtTsPumpMotorController.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"
#include "UtTsValveController.hh"

/// @details  Test identification number.
int UtTsPumpMotorController::TEST_ID = 0;

/// @details  Acts as a redundant test on UnitConversion class.
const double UtTsPumpMotorController::TWOPI = 6.28318530717959;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsPumpMotorController::UtTsPumpMotorController()
    :
    CppUnit::TestFixture(),
    tName(),
    tMotorConfig(),
    tForcingGain(),
    tDampingGain(),
    tDampingCutoff(),
    tTemperatureTripLimit(),
    tTemperatureTripReset(),
    tMinVoltage(),
    tMaxVoltage(),
    tControllerPowerLoad(),
    tNoiseAmplitude(),
    tNoiseFrequency(),
    tStartupCurrentLimit(),
    tNominalConfig(),
    tLoadTorques(),
    tMotorInput(),
    tVoltage(),
    tSensedSpeed(),
    tSensedTemperature(),
    tStartupState(),
    tCommandEnable(),
    tCommandSpeed(),
    tNoisePhase(),
    tNominalInput()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsPumpMotorController::~UtTsPumpMotorController()
{
   // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::setUp()
{
    tName                            = "Test";
    tMotorConfig.mWindingResistance  = 4.38;
    tMotorConfig.mTorqueConstant     = 2.72772E-2;
    tMotorConfig.mFrictionConstant   = 5.34660E-7;
    tMotorConfig.mFrictionMinSpeed   = 4000;
    tMotorConfig.mInertia            = 1.88841E-3;
    tForcingGain                     = 0.4;
    tDampingGain                     = 0.2;
    tDampingCutoff                   = 0.01;
    tTemperatureTripLimit            = 350.0;
    tTemperatureTripReset            = 250.0;
    tMinVoltage                      = 100.0;
    tMaxVoltage                      = 140.0;
    tControllerPowerLoad             = 5.0;
    tNoiseAmplitude                  = 100.0;
    tNoiseFrequency                  = 0.1;
    tStartupCurrentLimit             = 20.0;
    tNominalConfig = new TsPumpMotorControllerConfigData(tMotorConfig,
                                                         tForcingGain,
                                                         tDampingGain,
                                                         tDampingCutoff,
                                                         tTemperatureTripLimit,
                                                         tTemperatureTripReset,
                                                         tMinVoltage,
                                                         tMaxVoltage,
                                                         tControllerPowerLoad,
                                                         tNoiseAmplitude,
                                                         tNoiseFrequency,
                                                         tStartupCurrentLimit);

    tMotorInput.mVoltage             = 124.0;
    tMotorInput.mCurrentLimit        = 0.0;
    tMotorInput.mMotorSpeed          = 2000.0;
    tMotorInput.mLoadTorque1         =-0.001;
    tMotorInput.mLoadTorque2         =-0.002;
    tMotorInput.mLoadTorque3         =-0.003;
    tMotorInput.mLoadTorque4         =-0.004;
    tVoltage                         = 124.0;
    tSensedSpeed                     = 0.0;
    tSensedTemperature               = 300.0;
    tStartupState                    = false;
    tCommandEnable                   = true;
    tCommandSpeed                    = 8000.0;
    tNoisePhase                      = 1.0;
    tNominalInput = new TsPumpMotorControllerInputData(tMotorInput,
                                                       tVoltage,
                                                       tSensedSpeed,
                                                       tSensedTemperature,
                                                       tStartupState,
                                                       tCommandEnable,
                                                       tCommandSpeed,
                                                       tNoisePhase);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::tearDown()
{
    delete tNominalInput;
    delete tNominalConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructors & destructors of the Controller config data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testConfigData()
{
    UT_RESULT_FIRST;

    /// - Test default construction of a test config data article.
    TsPumpMotorControllerConfigData defaultConfig;
    CPPUNIT_ASSERT(0.0    == defaultConfig.mMotor.mWindingResistance);
    CPPUNIT_ASSERT(0.0    == defaultConfig.mForcingGain);
    CPPUNIT_ASSERT(0.0    == defaultConfig.mDampingGain);
    CPPUNIT_ASSERT(0.01   == defaultConfig.mDampingCutoff);
    CPPUNIT_ASSERT(0.0    == defaultConfig.mTemperatureTripReset);
    CPPUNIT_ASSERT(0.0    == defaultConfig.mTemperatureTripLimit);
    CPPUNIT_ASSERT(0.0    == defaultConfig.mMinVoltage);
    CPPUNIT_ASSERT(0.0    == defaultConfig.mMaxVoltage);
    CPPUNIT_ASSERT(0.0    == defaultConfig.mControllerPowerLoad);
    CPPUNIT_ASSERT(0.0    == defaultConfig.mNoiseAmplitude);
    CPPUNIT_ASSERT(0.0    == defaultConfig.mNoiseFrequency);
    CPPUNIT_ASSERT(1.0E15 == defaultConfig.mStartupCurrentLimit);

    /// - Test nominal construction of a test config data article.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMotorConfig.mWindingResistance,
                                 tNominalConfig->mMotor.mWindingResistance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMotorConfig.mTorqueConstant,
                                 tNominalConfig->mMotor.mTorqueConstant,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMotorConfig.mFrictionConstant,
                                 tNominalConfig->mMotor.mFrictionConstant,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMotorConfig.mFrictionMinSpeed,
                                 tNominalConfig->mMotor.mFrictionMinSpeed,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMotorConfig.mInertia,
                                 tNominalConfig->mMotor.mInertia,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tForcingGain,          tNominalConfig->mForcingGain,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDampingGain,          tNominalConfig->mDampingGain,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDampingCutoff,        tNominalConfig->mDampingCutoff,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperatureTripLimit, tNominalConfig->mTemperatureTripLimit, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperatureTripReset, tNominalConfig->mTemperatureTripReset, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinVoltage,           tNominalConfig->mMinVoltage,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxVoltage,           tNominalConfig->mMaxVoltage,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tControllerPowerLoad,  tNominalConfig->mControllerPowerLoad,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNoiseAmplitude,       tNominalConfig->mNoiseAmplitude,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNoiseFrequency,       tNominalConfig->mNoiseFrequency,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tStartupCurrentLimit,  tNominalConfig->mStartupCurrentLimit,  DBL_EPSILON);

    /// - Test copy construction of a test config data article.
    TsPumpMotorControllerConfigData copyConfig(*tNominalConfig);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMotorConfig.mWindingResistance,
                                 copyConfig.mMotor.mWindingResistance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMotorConfig.mTorqueConstant,
                                 copyConfig.mMotor.mTorqueConstant,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMotorConfig.mFrictionConstant,
                                 copyConfig.mMotor.mFrictionConstant,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMotorConfig.mFrictionMinSpeed,
                                 copyConfig.mMotor.mFrictionMinSpeed,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMotorConfig.mInertia,
                                 copyConfig.mMotor.mInertia,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tForcingGain,          copyConfig.mForcingGain,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDampingGain,          copyConfig.mDampingGain,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDampingCutoff,        copyConfig.mDampingCutoff,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperatureTripLimit, copyConfig.mTemperatureTripLimit, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperatureTripReset, copyConfig.mTemperatureTripReset, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinVoltage,           copyConfig.mMinVoltage,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxVoltage,           copyConfig.mMaxVoltage,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tControllerPowerLoad,  copyConfig.mControllerPowerLoad,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNoiseAmplitude,       copyConfig.mNoiseAmplitude,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNoiseFrequency,       copyConfig.mNoiseFrequency,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tStartupCurrentLimit,  copyConfig.mStartupCurrentLimit,  DBL_EPSILON);

    /// - Test assignment of a test config data article.
    defaultConfig = *tNominalConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMotorConfig.mWindingResistance,
                                 defaultConfig.mMotor.mWindingResistance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tForcingGain,          defaultConfig.mForcingGain,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDampingGain,          defaultConfig.mDampingGain,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDampingCutoff,        defaultConfig.mDampingCutoff,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperatureTripLimit, defaultConfig.mTemperatureTripLimit, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperatureTripReset, defaultConfig.mTemperatureTripReset, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinVoltage,           defaultConfig.mMinVoltage,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxVoltage,           defaultConfig.mMaxVoltage,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tControllerPowerLoad,  defaultConfig.mControllerPowerLoad,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNoiseAmplitude,       defaultConfig.mNoiseAmplitude,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNoiseFrequency,       defaultConfig.mNoiseFrequency,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tStartupCurrentLimit,  defaultConfig.mStartupCurrentLimit,  DBL_EPSILON);

    /// - Test self-assignment of a test config data article.
    CPPUNIT_ASSERT(&defaultConfig == &(defaultConfig = defaultConfig));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Controller input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testInputData()
{
    UT_RESULT;

    /// - Test default construction of a test input data article.
    TsPumpMotorControllerInputData defaultInput;
    CPPUNIT_ASSERT(0.0   == defaultInput.mMotor.mVoltage);
    CPPUNIT_ASSERT(0.0   == defaultInput.mVoltage);
    CPPUNIT_ASSERT(0.0   == defaultInput.mSensedSpeed);
    CPPUNIT_ASSERT(0.0   == defaultInput.mSensedTemperature);
    CPPUNIT_ASSERT(false == defaultInput.mStartupState);
    CPPUNIT_ASSERT(false == defaultInput.mCommandEnable);
    CPPUNIT_ASSERT(0.0   == defaultInput.mCommandSpeed);
    CPPUNIT_ASSERT(0.0   == defaultInput.mNoisePhase);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailPower);
    CPPUNIT_ASSERT(false == defaultInput.mMalfPowerOverrideFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfPowerOverrideValue);
    CPPUNIT_ASSERT(false == defaultInput.mMalfCommandSpeedFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfCommandSpeedValue);

    /// - Test nominal construction of a test input data article.
    TsPumpMotorControllerInputData nominalInput(tMotorInput, 16.0, 13000.0, 300.0, true, true, 12000, 0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.0,   nominalInput.mMotor.mVoltage,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.004,  nominalInput.mMotor.mLoadTorque4,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(16.0,    nominalInput.mVoltage,               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13000.0, nominalInput.mSensedSpeed,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0,   nominalInput.mSensedTemperature,     DBL_EPSILON);
    CPPUNIT_ASSERT(true  == nominalInput.mStartupState);
    CPPUNIT_ASSERT(true  == nominalInput.mCommandEnable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(12000.0, nominalInput.mCommandSpeed,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1,     nominalInput.mNoisePhase,            DBL_EPSILON);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailPower);
    CPPUNIT_ASSERT(false == defaultInput.mMalfPowerOverrideFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfPowerOverrideValue);
    CPPUNIT_ASSERT(false == defaultInput.mMalfCommandSpeedFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfCommandSpeedValue);

    /// - Test copy construction of a test config data article.
    nominalInput.mMalfFailPower          = true;
    nominalInput.mMalfPowerOverrideFlag  = true;
    nominalInput.mMalfPowerOverrideValue = 1.0;
    nominalInput.mMalfCommandSpeedFlag   = true;
    nominalInput.mMalfCommandSpeedValue  = 2.0;
    TsPumpMotorControllerInputData copyInput(nominalInput);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.0,   copyInput.mMotor.mVoltage,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.004,  copyInput.mMotor.mLoadTorque4,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(16.0,    copyInput.mVoltage,                  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13000.0, copyInput.mSensedSpeed,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0,   copyInput.mSensedTemperature,        DBL_EPSILON);
    CPPUNIT_ASSERT(true  == copyInput.mStartupState);
    CPPUNIT_ASSERT(true  == copyInput.mCommandEnable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(12000.0, copyInput.mCommandSpeed,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1,     copyInput.mNoisePhase,               DBL_EPSILON);
    CPPUNIT_ASSERT(true  == copyInput.mMalfFailPower);
    CPPUNIT_ASSERT(true  == copyInput.mMalfPowerOverrideFlag);
    CPPUNIT_ASSERT(1.0   == copyInput.mMalfPowerOverrideValue);
    CPPUNIT_ASSERT(true  == copyInput.mMalfCommandSpeedFlag);
    CPPUNIT_ASSERT(2.0   == copyInput.mMalfCommandSpeedValue);

    /// - Test assignment of a test input data article.
    defaultInput = nominalInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.0,   defaultInput.mMotor.mVoltage,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.004,  defaultInput.mMotor.mLoadTorque4,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(16.0,    defaultInput.mVoltage,               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13000.0, defaultInput.mSensedSpeed,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0,   defaultInput.mSensedTemperature,     DBL_EPSILON);
    CPPUNIT_ASSERT(true  == defaultInput.mStartupState);
    CPPUNIT_ASSERT(true  == defaultInput.mCommandEnable);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(12000.0, defaultInput.mCommandSpeed,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1,     defaultInput.mNoisePhase,            DBL_EPSILON);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfFailPower);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfPowerOverrideFlag);
    CPPUNIT_ASSERT(1.0   == defaultInput.mMalfPowerOverrideValue);
    CPPUNIT_ASSERT(true  == defaultInput.mMalfCommandSpeedFlag);
    CPPUNIT_ASSERT(2.0   == defaultInput.mMalfCommandSpeedValue);

    /// - Test self-assignment of a test input data article.
    CPPUNIT_ASSERT(&defaultInput == &(defaultInput = defaultInput));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Controller class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testConstruction()
{
    UT_RESULT;

    /// - Test default construction of the test article.
    FriendlyTsPumpMotorController article;
    CPPUNIT_ASSERT(""                                    == article.mName);
    CPPUNIT_ASSERT(false                                 == article.mInitFlag);
    CPPUNIT_ASSERT(0.0                                   == article.mForcingGain);
    CPPUNIT_ASSERT(0.0                                   == article.mDampingGain);
    CPPUNIT_ASSERT(0.0                                   == article.mDampingCutoff);
    CPPUNIT_ASSERT(0.0                                   == article.mTemperatureTripReset);
    CPPUNIT_ASSERT(0.0                                   == article.mTemperatureTripLimit);
    CPPUNIT_ASSERT(0.0                                   == article.mMinVoltage);
    CPPUNIT_ASSERT(0.0                                   == article.mMaxVoltage);
    CPPUNIT_ASSERT(0.0                                   == article.mControllerPowerLoad);
    CPPUNIT_ASSERT(0.0                                   == article.mNoiseAmplitude);
    CPPUNIT_ASSERT(0.0                                   == article.mNoiseFrequency);
    CPPUNIT_ASSERT(0.0                                   == article.mStartupCurrentLimit);
    CPPUNIT_ASSERT(0.0                                   == article.mVoltage);
    CPPUNIT_ASSERT(0.0                                   == article.mSensedSpeed);
    CPPUNIT_ASSERT(0.0                                   == article.mSensedTemperature);
    CPPUNIT_ASSERT(false                                 == article.mStartupState);
    CPPUNIT_ASSERT(false                                 == article.mCommandEnable);
    CPPUNIT_ASSERT(0.0                                   == article.mCommandSpeed);
    CPPUNIT_ASSERT(0.0                                   == article.mNoisePhase);
    CPPUNIT_ASSERT(false                                 == article.mMalfFailPower);
    CPPUNIT_ASSERT(false                                 == article.mMalfPowerOverrideFlag);
    CPPUNIT_ASSERT(0.0                                   == article.mMalfPowerOverrideValue);
    CPPUNIT_ASSERT(false                                 == article.mMalfCommandSpeedFlag);
    CPPUNIT_ASSERT(0.0                                   == article.mMalfCommandSpeedValue);
    CPPUNIT_ASSERT(0.0                                   == article.mPulseWidth);
    CPPUNIT_ASSERT(0.0                                   == article.mSpeedError);
    CPPUNIT_ASSERT(false                                 == article.mTrippedTemperature);
    CPPUNIT_ASSERT(false                                 == article.mControllerPowerBus);
    CPPUNIT_ASSERT(false                                 == article.mMotorPowerBus);
    CPPUNIT_ASSERT(TsPumpMotorController::MAX_RESISTANCE == article.mTotalResistiveLoad);
    CPPUNIT_ASSERT(0.0                                   == article.mTotalWasteHeat);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the initialization of the Controller class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testInitialize()
{
    UT_RESULT;

    /// - Test nominal initialization of the test article.
    FriendlyTsPumpMotorController article;
    tNominalInput->mMalfFailPower          = true;
    tNominalInput->mMalfPowerOverrideFlag  = true;
    tNominalInput->mMalfPowerOverrideValue = 1.0;
    tNominalInput->mMalfCommandSpeedFlag   = true;
    tNominalInput->mMalfCommandSpeedValue  = 2.0;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    CPPUNIT_ASSERT(tName                                 == article.mName);
    CPPUNIT_ASSERT(true                                  == article.mInitFlag);
    CPPUNIT_ASSERT(tForcingGain                          == article.mForcingGain);
    CPPUNIT_ASSERT(tDampingGain                          == article.mDampingGain);
    CPPUNIT_ASSERT(tDampingCutoff                        == article.mDampingCutoff);
    CPPUNIT_ASSERT(tTemperatureTripLimit                 == article.mTemperatureTripLimit);
    CPPUNIT_ASSERT(tTemperatureTripReset                 == article.mTemperatureTripReset);
    CPPUNIT_ASSERT(tMinVoltage                           == article.mMinVoltage);
    CPPUNIT_ASSERT(tMaxVoltage                           == article.mMaxVoltage);
    CPPUNIT_ASSERT(tControllerPowerLoad                  == article.mControllerPowerLoad);
    CPPUNIT_ASSERT(tNoiseAmplitude                       == article.mNoiseAmplitude);
    CPPUNIT_ASSERT(tNoiseFrequency                       == article.mNoiseFrequency);
    CPPUNIT_ASSERT(tStartupCurrentLimit                  == article.mStartupCurrentLimit);
    CPPUNIT_ASSERT(tVoltage                              == article.mVoltage);
    CPPUNIT_ASSERT(tSensedSpeed                          == article.mSensedSpeed);
    CPPUNIT_ASSERT(tSensedTemperature                    == article.mSensedTemperature);
    CPPUNIT_ASSERT(tStartupState                         == article.mStartupState);
    CPPUNIT_ASSERT(tCommandEnable                        == article.mCommandEnable);
    CPPUNIT_ASSERT(tCommandSpeed                         == article.mCommandSpeed);
    CPPUNIT_ASSERT(tNoisePhase                           == article.mNoisePhase);
    CPPUNIT_ASSERT(true                                  == article.mMalfFailPower);
    CPPUNIT_ASSERT(true                                  == article.mMalfPowerOverrideFlag);
    CPPUNIT_ASSERT(1.0                                   == article.mMalfPowerOverrideValue);
    CPPUNIT_ASSERT(true                                  == article.mMalfCommandSpeedFlag);
    CPPUNIT_ASSERT(2.0                                   == article.mMalfCommandSpeedValue);
    CPPUNIT_ASSERT(0.0                                   == article.mPulseWidth);
    CPPUNIT_ASSERT(0.0                                   == article.mSpeedError);
    CPPUNIT_ASSERT(false                                 == article.mTrippedTemperature);
    CPPUNIT_ASSERT(0.0                                   == article.mControllerPowerBus);
    CPPUNIT_ASSERT(0.0                                   == article.mMotorPowerBus);
    CPPUNIT_ASSERT(TsPumpMotorController::MAX_RESISTANCE == article.mTotalResistiveLoad);
    CPPUNIT_ASSERT(0.0                                   == article.mTotalWasteHeat);
    CPPUNIT_ASSERT(2000.0                                == article.mMotor.getSpeed());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the initialization exceptions of the Controller class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testInitializeExceptions()
{
    UT_RESULT;

    /// - Verify exception is thrown when no name is given.
    FriendlyTsPumpMotorController article;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, ""),
                         TsInitializationException);

    /// - Verify exception is thrown when forcing gain is outside 0-1.
    tNominalConfig->mForcingGain = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mForcingGain = 1.0 + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mForcingGain = tForcingGain;

    /// - Verify exception is thrown when damping gain is outside 0-1.
    tNominalConfig->mDampingGain = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mDampingGain = 1.0 + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mDampingGain = tDampingGain;

    /// - Verify exception is thrown when damping cut-off is outside 0-1.
    tNominalConfig->mDampingCutoff = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mDampingCutoff = 1.0 + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mDampingCutoff = tDampingCutoff;

    /// - Verify exception is thrown when minimum temperature is < 0.
    tNominalConfig->mTemperatureTripReset = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mTemperatureTripReset = tTemperatureTripReset;

    /// - Verify exception is thrown when maximum temperature is <= minimum.
    tNominalConfig->mTemperatureTripLimit = tTemperatureTripReset;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mTemperatureTripLimit = tTemperatureTripLimit;

    /// - Verify exception is thrown when minimum voltage is < 0.
    tNominalConfig->mMinVoltage = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mMinVoltage = tMinVoltage;

    /// - Verify exception is thrown when maximum voltage is <= minimum.
    tNominalConfig->mMaxVoltage = tMinVoltage;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mMaxVoltage = tMaxVoltage;

    /// - Verify exception is thrown when controller power load is < 0.
    tNominalConfig->mControllerPowerLoad = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mControllerPowerLoad = tControllerPowerLoad;

    /// - Verify exception is thrown when noise amplitude is < 0.
    tNominalConfig->mNoiseAmplitude = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mNoiseAmplitude = tNoiseAmplitude;

    /// - Verify exception is thrown when noise frequency is < 0.
    tNominalConfig->mNoiseFrequency = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mNoiseFrequency = tNoiseFrequency;

    /// - Verify exception is thrown when initial voltage is < 0.
    tNominalInput->mVoltage = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalInput->mVoltage = tVoltage;

    /// - Verify exception is thrown when motor initialization is bad, and init flag is reset.
    tNominalConfig->mMotor.mWindingResistance = -1.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    CPPUNIT_ASSERT(false == article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the setter & getter methods of the Controller class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testAccessors()
{
    UT_RESULT;

    /// - Set up a test article with nominal config & input data.
    FriendlyTsPumpMotorController article;
    FriendlyDcDynPumpMotor* motor = static_cast<FriendlyDcDynPumpMotor*>(&article.mMotor);

    tNominalInput->mMotor.mLoadTorque1 = 0.0;
    tNominalInput->mMotor.mLoadTorque2 = 0.0;
    tNominalInput->mMotor.mLoadTorque3 = 0.0;
    tNominalInput->mMotor.mLoadTorque4 = 0.0;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Test the methods to set input values.
    article.setVoltage(20.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(20.0,  article.mVoltage,           DBL_EPSILON);
    article.setSensedSpeed(300.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, article.mSensedSpeed,       DBL_EPSILON);
    article.setSensedTemperature(250.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(250.0, article.mSensedTemperature, DBL_EPSILON);
    article.setCommandSpeed(400.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(400.0, article.mCommandSpeed,      DBL_EPSILON);
    article.setCommandEnable(true);
    CPPUNIT_ASSERT(article.mCommandEnable);

    article.step(0.1);

    /// - Test the methods to get output values.
    article.mInitFlag           = true;
    CPPUNIT_ASSERT(article.isInitialized());
    article.mPulseWidth         = 0.1;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1,   article.getPulseWidth(),    DBL_EPSILON);
    article.mTrippedTemperature = true;
    CPPUNIT_ASSERT(article.isTemperatureTripped());
    article.mControllerPowerBus = true;
    CPPUNIT_ASSERT(article.isControllerBusPowered());
    article.mMotorPowerBus      = true;
    CPPUNIT_ASSERT(article.isMotorBusPowered());
    article.mTotalResistiveLoad = 0.5;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,   article.getTotalResistance(),  DBL_EPSILON);
    article.mTotalWasteHeat     = 200.5;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(200.5, article.getTotalWasteHeat(),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article.mTotalCurrent, article.getTotalCurrent(), DBL_EPSILON);
    CPPUNIT_ASSERT(article.getMotorCurrent() < article.getTotalCurrent());
    motor->mMotorSpeed          = 3.0;
    CPPUNIT_ASSERT(3.0 == article.getMotorSpeed());
    motor->mPower               = 5.0;
    CPPUNIT_ASSERT(5.0 == article.getMotorPower());

    article.setMotorLoadTorques(1.0, 2.0, 3.0, 4.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,   motor->mLoadTorques[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,   motor->mLoadTorques[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0,   motor->mLoadTorques[2],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0,   motor->mLoadTorques[3],     DBL_EPSILON);

    article.mVoltage = 124.0;
    article.mTotalCurrent = 1.7;
    double power = article.mVoltage * article.mTotalCurrent;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(power, article.getTotalPower(),    DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the temperature trip logic of the Controller class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testTemperatureTrip()
{
    UT_RESULT;

    /// - Set up a test article with nominal config & input data.
    FriendlyTsPumpMotorController article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify trip for temperature high.
    article.mControllerPowerBus = true;
    article.setSensedTemperature(400.0);
    article.checkTrips();
    CPPUNIT_ASSERT(article.isTemperatureTripped());

    /// - Verify temperature trip not reset by intermediate value.
    article.setSensedTemperature(300.0);
    article.checkTrips();
    CPPUNIT_ASSERT(article.isTemperatureTripped());

    /// - Verify trip reset by temperature low.
    article.setSensedTemperature(200.0);
    article.checkTrips();
    CPPUNIT_ASSERT(!article.isTemperatureTripped());

    /// - Verify temperature trip reset by cycling power.
    article.setSensedTemperature(400.0);
    article.checkTrips();
    CPPUNIT_ASSERT(article.isTemperatureTripped());
    article.setSensedTemperature(300.0);
    article.mControllerPowerBus = false;
    article.checkTrips();
    CPPUNIT_ASSERT(!article.isTemperatureTripped());
    article.mControllerPowerBus = true;
    article.checkTrips();
    CPPUNIT_ASSERT(!article.isTemperatureTripped());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the current trip logic of the Controller class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testCurrentTrip()
{
    UT_RESULT;

    /// - Deleted.

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the determinePower method of the Controller class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testPower()
{
    UT_RESULT;

    /// - Set up a test article with nominal config & input data.
    FriendlyTsPumpMotorController article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify power on with good voltage.
    article.setVoltage(120.0);
    article.determinePower();
    CPPUNIT_ASSERT(article.isControllerBusPowered());
    CPPUNIT_ASSERT(article.isMotorBusPowered());

    /// - Verify power off with under-voltage.
    article.setVoltage(90.0);
    article.determinePower();
    CPPUNIT_ASSERT(!article.isControllerBusPowered());
    CPPUNIT_ASSERT(!article.isMotorBusPowered());

    /// - Verify power off with over-voltage.
    article.setVoltage(142.0);
    article.determinePower();
    CPPUNIT_ASSERT(!article.isControllerBusPowered());
    CPPUNIT_ASSERT(!article.isMotorBusPowered());

    /// - Verify power override malf overrides bad voltage.
    article.mMalfPowerOverrideFlag  = true;
    article.mMalfPowerOverrideValue = 120.0;
    article.determinePower();
    CPPUNIT_ASSERT(article.isControllerBusPowered());
    CPPUNIT_ASSERT(article.isMotorBusPowered());

    /// - Verify power fail malf overrides good voltage.
    article.mMalfFailPower = true;
    article.determinePower();
    CPPUNIT_ASSERT(!article.isControllerBusPowered());
    CPPUNIT_ASSERT(!article.isMotorBusPowered());

    /// - Verify command disable kills motor power.
    article.mMalfFailPower = false;
    article.setCommandEnable(false);
    article.determinePower();
    CPPUNIT_ASSERT(article.isControllerBusPowered());
    CPPUNIT_ASSERT(!article.isMotorBusPowered());

    /// - Verify temperature trip kills motor power.
    article.setCommandEnable(true);
    article.setSensedTemperature(1000.0);
    article.determinePower();
    CPPUNIT_ASSERT(article.isControllerBusPowered());
    CPPUNIT_ASSERT(!article.isMotorBusPowered());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the noise component of the control filter.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testControlFilterNoise()
{
    UT_RESULT;

    /// - Set up a test article with nominal config & input data.
    FriendlyTsPumpMotorController article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify forward propagation of the noise phase.
    double expectedPhase = tNoisePhase + TWOPI * tNoiseFrequency * 0.1;
    article.mMotorPowerBus = true;
    article.updateControlFilter(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPhase, article.mNoisePhase, DBL_EPSILON);

    /// - Verify wrap-around at > 2*pi.
    article.mNoisePhase = 7.0;
    article.updateControlFilter(0.0);
    expectedPhase = 7.0 - TWOPI;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPhase, article.mNoisePhase, FLT_EPSILON);

    /// - Verify wrap-around at < 0.
    article.mNoisePhase = -1.0;
    article.updateControlFilter(0.0);
    expectedPhase = -1.0 + TWOPI;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPhase, article.mNoisePhase, FLT_EPSILON);

    /// - Verify sine function = 1 at pi/2.  Biased speed command is expected to be 2.0.
    article.mNoisePhase     = TWOPI / 4.0;
    article.mCommandSpeed   = 1.0;
    article.mNoiseAmplitude = 1.0;
    article.mSensedSpeed    = 1.0;
    double expectedError    = 0.5;
    article.updateControlFilter(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedError, article.mSpeedError, FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the pulse width output of the control filter.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testControlFilterPulseWidth()
{
    UT_RESULT;

    /// - Set up a test article with nominal config & input data.
    FriendlyTsPumpMotorController article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify a nominal acceleration towards commanded speed.
    article.mMotorPowerBus      = true;
    article.mStartupState       = true;
    article.mNoiseAmplitude     = 0.0;
    article.mCommandSpeed       = 2000.0;
    article.mSensedSpeed        = 1000.0;
    article.mSpeedError         = 0.6;
    article.mPulseWidth         = 0.5;
    double expectedError        = (2000.0 - 1000.0) / 2000.0;
    double dSpeedError          = expectedError - article.mSpeedError;
    double expectedPulseWidth   = 0.5 + tForcingGain * expectedError
                                      + tDampingGain * dSpeedError / expectedError;
    article.updateControlFilter(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedError,      article.mSpeedError, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPulseWidth, article.mPulseWidth, FLT_EPSILON);
    CPPUNIT_ASSERT(article.mStartupState);

    /// - Verify cut-out of damping within 1% speed error.
    article.mSensedSpeed = 1980.1;
    article.mPulseWidth  = 0.5;
    expectedError        = (2000.0 - 1980.1) / 2000.0;
    expectedPulseWidth   = 0.5 + tForcingGain * expectedError;
    article.updateControlFilter(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedError,      article.mSpeedError, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPulseWidth, article.mPulseWidth, FLT_EPSILON);
    CPPUNIT_ASSERT(!article.mStartupState);

    /// - Verify max limit on pulse width output.
    article.mPulseWidth = 0.999;
    expectedPulseWidth  = 1.0;
    article.updateControlFilter(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPulseWidth, article.mPulseWidth, FLT_EPSILON);

    /// - Verify min limit on pulse width output.
    article.mMalfCommandSpeedFlag  = true;
    article.mMalfCommandSpeedValue = 0.0;
    article.mPulseWidth            = 0.0;
    expectedPulseWidth             = 0.0;
    article.updateControlFilter(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPulseWidth, article.mPulseWidth, FLT_EPSILON);

    /// - Verify outputs when disabled/un-powered.
    article.mMalfCommandSpeedFlag  = false;
    article.mMotorPowerBus         = false;
    article.mCommandSpeed          = 2000.0;
    article.mSensedSpeed           = 1000.0;
    article.mSpeedError            = 0.6;
    article.mPulseWidth            = 0.5;
    expectedError                  = 0.0;
    expectedPulseWidth             = 0.0;
    article.updateControlFilter(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedError,      article.mSpeedError, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPulseWidth, article.mPulseWidth, FLT_EPSILON);
    CPPUNIT_ASSERT(article.mStartupState);

    article.mMotorPowerBus         = true;
    article.mCommandSpeed          = 0.0;
    article.updateControlFilter(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedError,      article.mSpeedError, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPulseWidth, article.mPulseWidth, FLT_EPSILON);
    CPPUNIT_ASSERT(article.mStartupState);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the updateMotor method of the Controller class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testUpdateMotor()
{
    UT_RESULT;

    /// - Set up a test article with nominal config & input data.
    FriendlyTsPumpMotorController article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify motor update with normal power
    article.mStartupState  = true;
    article.mVoltage       = 124.0;
    article.mPulseWidth    = 1.0;
    article.updateMotor(0.1);
    double motorCurrent = article.getMotorCurrent();
    double motorPower   = article.getMotorPower();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tStartupCurrentLimit, motorCurrent,              FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(124.0,                motorPower / motorCurrent, FLT_EPSILON);

    /// - Verify motor update with ignore power override
    article.mStartupState           = false;
    article.mMalfPowerOverrideFlag  = true;
    article.mMalfPowerOverrideValue = 120.0;
    article.updateMotor(0.1);
    motorCurrent = article.getMotorCurrent();
    motorPower   = article.getMotorPower();
    CPPUNIT_ASSERT(tStartupCurrentLimit < motorCurrent);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, motorPower / motorCurrent, FLT_EPSILON);

    /// - Verify motor update with zero pulse width
    article.mMalfPowerOverrideFlag = false;
    article.mPulseWidth            = 0.0;
    article.updateMotor(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.getMotorPower(),   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.getMotorCurrent(), FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the updatePowerLoad method of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testPowerLoad()
{
    UT_RESULT;

    /// - Set up a test article with nominal config & input data.
    FriendlyTsPumpMotorController article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify nominal controller power with no motor load.  I=V/R, P=IV, R=V^2/P.
    article.mControllerPowerBus = true;
    article.mMotorPowerBus      = false;
    article.mVoltage            = 18.0;
    double expectedHeat         = tControllerPowerLoad;
    double expectedResistance   = 18.0 * 18.0 / tControllerPowerLoad;
    article.updatePowerLoad();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat,       article.getTotalWasteHeat(),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance, article.getTotalResistance(), FLT_EPSILON);

    /// - Verify controller power override with no motor load.
    article.mMalfPowerOverrideFlag  = true;
    article.mMalfPowerOverrideValue = 16.0;
    expectedResistance              = 16.0 * 16.0 / tControllerPowerLoad;
    article.updatePowerLoad();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat,       article.getTotalWasteHeat(),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance, article.getTotalResistance(), FLT_EPSILON);

    /// - Verify addition of motor power load.
    article.mMotorPowerBus      = true;
    article.mPulseWidth         = 1.0;
    article.updateMotor(0.1);
    article.updatePowerLoad();
    CPPUNIT_ASSERT(expectedHeat       < article.getTotalWasteHeat());
    CPPUNIT_ASSERT(expectedResistance > article.getTotalResistance());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  End-to-end test of the Controller step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testStep()
{
    UT_RESULT;

    /// - Set up a test article with nominal config & input data.
    FriendlyTsPumpMotorController article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    article.setVoltage(124.0);
    article.setSensedSpeed(1000.0);
    article.setSensedTemperature(300.0);
    article.setCommandSpeed(2000.0);
    article.setCommandEnable(true);
    article.step(0.1);

    CPPUNIT_ASSERT(tControllerPowerLoad < article.getTotalWasteHeat());

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  End-to-end test of the Controller stepWithArgs method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsPumpMotorController::testStepWithArgs()
{
    UT_RESULT;

    /// - Set up a test article with nominal config & input data.
    FriendlyTsPumpMotorController article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    article.stepWithArgs(0.1, 124.0, 1000.0, 300.0, 2000.0, true);

    CPPUNIT_ASSERT(tControllerPowerLoad < article.getTotalWasteHeat());

    UT_PASS_LAST;
}
