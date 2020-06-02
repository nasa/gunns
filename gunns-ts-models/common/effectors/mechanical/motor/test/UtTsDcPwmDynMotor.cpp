/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 ()

 ASSUMPTIONS AND LIMITATIONS:
 ()


 REFERENCE:
 ()

 LIBRARY DEPENDENCY:
 (
    (software/exceptions/TsInitializationException.o)
    (common/effectors/mechanical/motor/TsDcPwmDynMotor.o)
 )

 PROGRAMMERS:
 (
  ((Jason Harvey) (L-3 Communications) (2012-07) (TS21) (Initial))
 )
 **************************************************************************************************/
#include <iostream>
#include "UtTsDcPwmDynMotor.hh"
#include "software/exceptions/TsInitializationException.hh"

/// @details  Acts as a redundant test on UnitConversion class.
const double UtTsDcPwmDynMotor::RPMTORADS = 9.54929658551372;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsDcPwmDynMotor::UtTsDcPwmDynMotor()
    :
    tName(),
    tPulseWasteFraction(),
    tStallTorqueCoeff1(),
    tStallTorqueCoeff2(),
    tFrictionTorque(),
    tBemfConstant(),
    tArmatureResistance(),
    tInertia(),
    tSpeedLoadRatio(),
    tNominalConfig(),
    tVoltage(),
    tPulseWidth(),
    tMotorSpeed(),
    tLoadTorque1(),
    tLoadTorque2(),
    tLoadTorque3(),
    tLoadTorque4(),
    tNominalInput()
{
   // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsDcPwmDynMotor::~UtTsDcPwmDynMotor()
{
   // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::setUp()
{
    tName               = "Test";
    tPulseWasteFraction = 0.1;
    tStallTorqueCoeff1  =  2.5E-4;
    tStallTorqueCoeff2  = -2.0E-8;
    tFrictionTorque     = -1.5E-5;
    tBemfConstant       = 0.005;
    tArmatureResistance = 2.0;
    tInertia            = 0.0005;
    tSpeedLoadRatio     = 500000.0;
    tNominalConfig = new TsDcPwmDynMotorConfigData(tPulseWasteFraction,
                                                   tStallTorqueCoeff1,
                                                   tStallTorqueCoeff2,
                                                   tFrictionTorque,
                                                   tBemfConstant,
                                                   tArmatureResistance,
                                                   tInertia,
                                                   tSpeedLoadRatio);

    tVoltage            = 16.0;
    tPulseWidth         = 0.5;
    tMotorSpeed         = 0.0;
    tLoadTorque1        = 0.0;
    tLoadTorque2        = 0.0;
    tLoadTorque3        = 0.0;
    tLoadTorque4        = 0.0;
    tNominalInput = new TsDcPwmDynMotorInputData(tVoltage,
                                                 tPulseWidth,
                                                 tMotorSpeed,
                                                 tLoadTorque1,
                                                 tLoadTorque2,
                                                 tLoadTorque3,
                                                 tLoadTorque4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::tearDown()
{
    delete tNominalInput;
    delete tNominalConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructors & destructors of the Motor config data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::testConfigData() {
    std::cout << "\n--------------------------------------------------------------------------------";
    std::cout << "\nTsDcPwmDynMotor unit tests";
    std::cout << "\nTest Config Data                  ";

    /// - Test default construction of a test config data article.
    TsDcPwmDynMotorConfigData defaultConfig;
    CPPUNIT_ASSERT(0.0 == defaultConfig.mPulseWasteFraction);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mStallTorqueCoeff1);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mStallTorqueCoeff2);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mFrictionTorque);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mBemfConstant);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mArmatureResistance);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mInertia);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mSpeedLoadRatio);

    /// - Test nominal construction of a test config data article.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPulseWasteFraction, tNominalConfig->mPulseWasteFraction, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tStallTorqueCoeff1,  tNominalConfig->mStallTorqueCoeff1,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tStallTorqueCoeff2,  tNominalConfig->mStallTorqueCoeff2,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFrictionTorque,     tNominalConfig->mFrictionTorque,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBemfConstant,       tNominalConfig->mBemfConstant,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArmatureResistance, tNominalConfig->mArmatureResistance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInertia,            tNominalConfig->mInertia,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSpeedLoadRatio,     tNominalConfig->mSpeedLoadRatio,     DBL_EPSILON);

    /// - Test copy construction of a test config data article.
    TsDcPwmDynMotorConfigData copyConfig(*tNominalConfig);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPulseWasteFraction, copyConfig.mPulseWasteFraction, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tStallTorqueCoeff1,  copyConfig.mStallTorqueCoeff1,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tStallTorqueCoeff2,  copyConfig.mStallTorqueCoeff2,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFrictionTorque,     copyConfig.mFrictionTorque,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBemfConstant,       copyConfig.mBemfConstant,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArmatureResistance, copyConfig.mArmatureResistance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInertia,            copyConfig.mInertia,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSpeedLoadRatio,     copyConfig.mSpeedLoadRatio,     DBL_EPSILON);

    /// - Test assignment of a test config data article.
    defaultConfig = *tNominalConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPulseWasteFraction, defaultConfig.mPulseWasteFraction, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tStallTorqueCoeff1,  defaultConfig.mStallTorqueCoeff1,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tStallTorqueCoeff2,  defaultConfig.mStallTorqueCoeff2,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFrictionTorque,     defaultConfig.mFrictionTorque,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBemfConstant,       defaultConfig.mBemfConstant,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArmatureResistance, defaultConfig.mArmatureResistance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInertia,            defaultConfig.mInertia,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSpeedLoadRatio,     defaultConfig.mSpeedLoadRatio,     DBL_EPSILON);

    /// - Test self-assignment of a test config data article.
    CPPUNIT_ASSERT(&defaultConfig == &(defaultConfig = defaultConfig));

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Motor input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::testInputData()
{
    std::cout << "\nTest Input Data                   ";

    /// - Test default construction of a test input data article.
    TsDcPwmDynMotorInputData defaultInput;
    CPPUNIT_ASSERT(0.0   == defaultInput.mVoltage);
    CPPUNIT_ASSERT(0.0   == defaultInput.mPulseWidth);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMotorSpeed);
    CPPUNIT_ASSERT(0.0   == defaultInput.mLoadTorque1);
    CPPUNIT_ASSERT(0.0   == defaultInput.mLoadTorque2);
    CPPUNIT_ASSERT(0.0   == defaultInput.mLoadTorque3);
    CPPUNIT_ASSERT(0.0   == defaultInput.mLoadTorque4);
    CPPUNIT_ASSERT(false == defaultInput.mMalfDegradeFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfDegradeValue);
    CPPUNIT_ASSERT(false == defaultInput.mMalfJamFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfJamValue);
    CPPUNIT_ASSERT(false == defaultInput.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(false == defaultInput.mMalfHeatRateBiasFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfHeatRateBiasValue);

    /// - Test nominal construction of a test input data article.
    TsDcPwmDynMotorInputData nominalInput(16.0, 0.5, 13000.0, -1.0, -2.0, -3.0, -4.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(16.0,    nominalInput.mVoltage,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,     nominalInput.mPulseWidth,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13000.0, nominalInput.mMotorSpeed,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0,    nominalInput.mLoadTorque1,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0,    nominalInput.mLoadTorque2,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0,    nominalInput.mLoadTorque3,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0,    nominalInput.mLoadTorque4,    DBL_EPSILON);
    CPPUNIT_ASSERT(false == nominalInput.mMalfDegradeFlag);
    CPPUNIT_ASSERT(0.0   == nominalInput.mMalfDegradeValue);
    CPPUNIT_ASSERT(false == nominalInput.mMalfJamFlag);
    CPPUNIT_ASSERT(0.0   == nominalInput.mMalfJamValue);
    CPPUNIT_ASSERT(false == nominalInput.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(0.0   == nominalInput.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(false == nominalInput.mMalfHeatRateBiasFlag);
    CPPUNIT_ASSERT(0.0   == nominalInput.mMalfHeatRateBiasValue);

    /// - Test copy construction of a test config data article.
    nominalInput.mMalfDegradeFlag        = true;
    nominalInput.mMalfDegradeValue       = 1.0;
    nominalInput.mMalfJamFlag            = true;
    nominalInput.mMalfJamValue           = 0.5;
    nominalInput.mMalfSpeedOverrideFlag  = true;
    nominalInput.mMalfSpeedOverrideValue = 13.0;
    nominalInput.mMalfHeatRateBiasFlag   = true;
    nominalInput.mMalfHeatRateBiasValue  = 42.0;
    TsDcPwmDynMotorInputData copyInput(nominalInput);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(16.0,    copyInput.mVoltage,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,     copyInput.mPulseWidth,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13000.0, copyInput.mMotorSpeed,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0,    copyInput.mLoadTorque1,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0,    copyInput.mLoadTorque2,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0,    copyInput.mLoadTorque3,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0,    copyInput.mLoadTorque4,    DBL_EPSILON);
    CPPUNIT_ASSERT(true == copyInput.mMalfDegradeFlag);
    CPPUNIT_ASSERT(1.0  == copyInput.mMalfDegradeValue);
    CPPUNIT_ASSERT(true == copyInput.mMalfJamFlag);
    CPPUNIT_ASSERT(0.5  == copyInput.mMalfJamValue);
    CPPUNIT_ASSERT(true == copyInput.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(13.0 == copyInput.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(true == copyInput.mMalfHeatRateBiasFlag);
    CPPUNIT_ASSERT(42.0 == copyInput.mMalfHeatRateBiasValue);

    /// - Test assignment of a test input data article.
    defaultInput = nominalInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(16.0,    defaultInput.mVoltage,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,     defaultInput.mPulseWidth,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13000.0, defaultInput.mMotorSpeed,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0,    defaultInput.mLoadTorque1,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0,    defaultInput.mLoadTorque2,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0,    defaultInput.mLoadTorque3,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0,    defaultInput.mLoadTorque4,    DBL_EPSILON);
    CPPUNIT_ASSERT(true == defaultInput.mMalfDegradeFlag);
    CPPUNIT_ASSERT(1.0  == defaultInput.mMalfDegradeValue);
    CPPUNIT_ASSERT(true == defaultInput.mMalfJamFlag);
    CPPUNIT_ASSERT(0.5  == defaultInput.mMalfJamValue);
    CPPUNIT_ASSERT(true == defaultInput.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(13.0 == defaultInput.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(true == defaultInput.mMalfHeatRateBiasFlag);
    CPPUNIT_ASSERT(42.0 == defaultInput.mMalfHeatRateBiasValue);

    /// - Test self-assignment of a test input data article.
    CPPUNIT_ASSERT(&defaultInput == &(defaultInput = defaultInput));

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::testConstruction()
{
    std::cout << "\nTest Construction                 ";

    /// - Test default construction of the test article.
    FriendlyTsDcPwmDynMotor article;
    CPPUNIT_ASSERT(""    == article.mName);
    CPPUNIT_ASSERT(0.0   == article.mPulseWasteFraction);
    CPPUNIT_ASSERT(0.0   == article.mStallTorqueCoeff1);
    CPPUNIT_ASSERT(0.0   == article.mStallTorqueCoeff2);
    CPPUNIT_ASSERT(0.0   == article.mFrictionTorque);
    CPPUNIT_ASSERT(0.0   == article.mBemfConstant);
    CPPUNIT_ASSERT(0.0   == article.mArmatureResistance);
    CPPUNIT_ASSERT(0.0   == article.mInertia);
    CPPUNIT_ASSERT(0.0   == article.mSpeedLoadRatio);
    CPPUNIT_ASSERT(0.0   == article.mVoltage);
    CPPUNIT_ASSERT(0.0   == article.mPulseWidth);
    CPPUNIT_ASSERT(0.0   == article.mMotorSpeed);
    CPPUNIT_ASSERT(0.0   == article.mLoadTorques[0]);
    CPPUNIT_ASSERT(0.0   == article.mLoadTorques[1]);
    CPPUNIT_ASSERT(0.0   == article.mLoadTorques[2]);
    CPPUNIT_ASSERT(0.0   == article.mLoadTorques[3]);
    CPPUNIT_ASSERT(false == article.mMalfDegradeFlag);
    CPPUNIT_ASSERT(0.0   == article.mMalfDegradeValue);
    CPPUNIT_ASSERT(false == article.mMalfJamFlag);
    CPPUNIT_ASSERT(0.0   == article.mMalfJamValue);
    CPPUNIT_ASSERT(false == article.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(0.0   == article.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(false == article.mMalfHeatRateBiasFlag);
    CPPUNIT_ASSERT(0.0   == article.mMalfHeatRateBiasValue);
    CPPUNIT_ASSERT(0.0   == article.mPower);
    CPPUNIT_ASSERT(0.0   == article.mCurrent);
    CPPUNIT_ASSERT(0.0   == article.mGeneratedCurrent);
    CPPUNIT_ASSERT(0.0   == article.mResistance);
    CPPUNIT_ASSERT(0.0   == article.mWasteHeat);
    CPPUNIT_ASSERT(0.0   == article.mEfficiency);
    CPPUNIT_ASSERT(0.0   == article.mStallTorque);
    CPPUNIT_ASSERT(0.0   == article.mDriveTorque);
    CPPUNIT_ASSERT(0.0   == article.mTotalExternalLoad);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the initialization of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::testInitialize()
{
    std::cout << "\nTest Initialize                   ";

    /// - Test nominal initialization of the test article.
    FriendlyTsDcPwmDynMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    CPPUNIT_ASSERT(tName               == article.mName);
    CPPUNIT_ASSERT(tPulseWasteFraction == article.mPulseWasteFraction);
    CPPUNIT_ASSERT(tStallTorqueCoeff1  == article.mStallTorqueCoeff1);
    CPPUNIT_ASSERT(tStallTorqueCoeff2  == article.mStallTorqueCoeff2);
    CPPUNIT_ASSERT(tFrictionTorque     == article.mFrictionTorque);
    CPPUNIT_ASSERT(tBemfConstant       == article.mBemfConstant);
    CPPUNIT_ASSERT(tArmatureResistance == article.mArmatureResistance);
    CPPUNIT_ASSERT(tInertia            == article.mInertia);
    CPPUNIT_ASSERT(tSpeedLoadRatio     == article.mSpeedLoadRatio);
    CPPUNIT_ASSERT(tVoltage            == article.mVoltage);
    CPPUNIT_ASSERT(tPulseWidth         == article.mPulseWidth);
    CPPUNIT_ASSERT(tMotorSpeed         == article.mMotorSpeed);
    CPPUNIT_ASSERT(tLoadTorque1        == article.mLoadTorques[0]);
    CPPUNIT_ASSERT(tLoadTorque2        == article.mLoadTorques[1]);
    CPPUNIT_ASSERT(tLoadTorque3        == article.mLoadTorques[2]);
    CPPUNIT_ASSERT(tLoadTorque4        == article.mLoadTorques[3]);
    CPPUNIT_ASSERT(false               == article.mMalfDegradeFlag);
    CPPUNIT_ASSERT(0.0                 == article.mMalfDegradeValue);
    CPPUNIT_ASSERT(false               == article.mMalfJamFlag);
    CPPUNIT_ASSERT(0.0                 == article.mMalfJamValue);
    CPPUNIT_ASSERT(false               == article.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(0.0                 == article.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(false               == article.mMalfHeatRateBiasFlag);
    CPPUNIT_ASSERT(0.0                 == article.mMalfHeatRateBiasValue);
    CPPUNIT_ASSERT(0.0                 == article.mPower);
    CPPUNIT_ASSERT(0.0                 == article.mCurrent);
    CPPUNIT_ASSERT(0.0                 == article.mGeneratedCurrent);
    CPPUNIT_ASSERT(0.0                 == article.mResistance);
    CPPUNIT_ASSERT(0.0                 == article.mWasteHeat);
    CPPUNIT_ASSERT(0.0                 == article.mEfficiency);
    CPPUNIT_ASSERT(0.0                 == article.mStallTorque);
    CPPUNIT_ASSERT(0.0                 == article.mDriveTorque);
    CPPUNIT_ASSERT(0.0                 == article.mTotalExternalLoad);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the initialization exceptions of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::testInitializeExceptions()
{
    std::cout << "\nTest Initialization Exceptions    ";

    FriendlyTsDcPwmDynMotor article;
    /// - Verify exception is thrown when no object name is given.
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, ""),
                         TsInitializationException);

    /// - Verify exception is thrown when pulse waste fraction not within 0-1.
    tNominalConfig->mPulseWasteFraction = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mPulseWasteFraction = 1.0 + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mPulseWasteFraction = tPulseWasteFraction;

    /// - Verify exception is thrown when friction torque is positive.
    tNominalConfig->mFrictionTorque = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mFrictionTorque = tFrictionTorque;

    /// - Verify exception is thrown when armature resistance < DBL_EPSILON.
    tNominalConfig->mArmatureResistance = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mArmatureResistance = tArmatureResistance;

    /// - Verify exception is thrown when inertia < DBL_EPSILON.
    tNominalConfig->mInertia = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mInertia = tInertia;

    /// - Verify exception is thrown when speed/load ratio < DBL_EPSILON.
    tNominalConfig->mSpeedLoadRatio = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mSpeedLoadRatio = tSpeedLoadRatio;

    /// - Verify exception is thrown when initial voltage < 0.
    tNominalInput->mVoltage = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalInput->mVoltage = tVoltage;

    /// - Verify exception is thrown when initial pulse width not within 0-1.
    tNominalInput->mPulseWidth = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalInput->mPulseWidth = 1.0 + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalInput->mPulseWidth = tPulseWidth;

    /// - Verify exception is thrown when initial speed < 0.
    tNominalInput->mMotorSpeed = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the setter & getter methods of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::testAccessors()
{
    std::cout << "\nTest Access Methods               ";

    /// - Set up a test article with nominal config & input data.
    FriendlyTsDcPwmDynMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));
    CPPUNIT_ASSERT(article.isInitialized());

    /// - Test the methods to set input values.
    article.setVoltage(20.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(20.0,  article.mVoltage,        DBL_EPSILON);
    article.setPulseWidth(0.3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3,   article.mPulseWidth,     DBL_EPSILON);
    article.setLoadTorques(-1.0, -2.0, -3.0, -4.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0,  article.mLoadTorques[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0,  article.mLoadTorques[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0,  article.mLoadTorques[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0,  article.mLoadTorques[3], DBL_EPSILON);

    /// - Test the methods to get output values.
    article.mCurrent    = 21.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(21.0,  article.getCurrent(),    DBL_EPSILON);
    article.mGeneratedCurrent = 2.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,   article.getGeneratedCurrent(), DBL_EPSILON);
    article.mMotorSpeed = 210.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(210.0, article.getSpeed(),      DBL_EPSILON);
    article.mResistance = 5.3;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.3,   article.getResistance(), DBL_EPSILON);
    article.mPower      = 100.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, article.getPower(),      DBL_EPSILON);
    article.mWasteHeat  = 40.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(40.0,  article.getWasteHeat(),  DBL_EPSILON);
    article.mEfficiency = 0.5;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,   article.getEfficiency(), DBL_EPSILON);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the generateMotorTorque method of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::testMotorTorque()
{
    std::cout << "\nTest Motor Torque                 ";

    /// - Set up a test article with nominal config & input data.
    FriendlyTsDcPwmDynMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Test that overspeed doesn't result in negative torque.
    article.mMotorSpeed = 10000.0;
    article.mPulseWidth = 1.0;
    double expectedStallTorque = tStallTorqueCoeff1 * tVoltage
                               + tStallTorqueCoeff2 * tVoltage * tVoltage;
    double expectedDriveTorque = 0.0;

    article.generateMotorTorque();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStallTorque, article.mStallTorque, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDriveTorque, article.mDriveTorque, DBL_EPSILON);

    /// - Test drive torque with degrade malfunction.
    article.mMotorSpeed       = 1000.0;
    article.mPulseWidth       = 0.5;
    article.mMalfDegradeValue = 0.25;
    article.mMalfDegradeFlag  = true;
    expectedStallTorque      *= 0.75;
    expectedDriveTorque       = 0.5 * (expectedStallTorque - 1000.0 / tSpeedLoadRatio);

    article.generateMotorTorque();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStallTorque, article.mStallTorque, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDriveTorque, article.mDriveTorque, DBL_EPSILON);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the gatherExternalLoads method of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::testExternalLoads()
{
    std::cout << "\nTest External Loads               ";

    /// - Set up a test article with nominal config & input data.
    FriendlyTsDcPwmDynMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify the total external load without jam malfunction.
    article.mLoadTorques[0]  = -0.001;
    article.mLoadTorques[2]  = -0.003;
    article.mLoadTorques[3]  =  0.002;
    double expectedTotalLoad = -0.001 - 0.003 + 0.002;

    article.gatherExternalLoads();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTotalLoad, article.mTotalExternalLoad, DBL_EPSILON);

    /// - Verify the total external load with jam malfunction active.
    article.mStallTorque    = 0.01;
    article.mMalfJamValue   = 0.5;
    article.mMalfJamFlag    = true;
    expectedTotalLoad      -= 0.5 * 0.01;

    article.gatherExternalLoads();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTotalLoad, article.mTotalExternalLoad, DBL_EPSILON);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the computeMotorSpeed method of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::testMotorSpeed()
{
    std::cout << "\nTest Motor Speed                  ";

    /// - Set up a test article with nominal config & input data.
    FriendlyTsDcPwmDynMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify motor acceleration without override.
    article.mDriveTorque       =  0.01;
    article.mTotalExternalLoad = -0.001;
    article.mMotorSpeed        =  500.0;
    double expectedMotorSpeed = 500 + (0.01 - 0.001 + tFrictionTorque) * 0.1 * RPMTORADS / tInertia;

    article.computeMotorSpeed(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMotorSpeed, article.getSpeed(), DBL_EPSILON);

    /// - Verify deceleration stops at zero.
    article.mTotalExternalLoad = -1000.0;
    expectedMotorSpeed = 0.0;

    article.computeMotorSpeed(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMotorSpeed, article.getSpeed(), DBL_EPSILON);

    /// - Verify speed override.
    article.mMalfSpeedOverrideFlag  = true;
    article.mMalfSpeedOverrideValue = 666.0;

    article.computeMotorSpeed(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(666.0, article.getSpeed(), DBL_EPSILON);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the computeElectricalOutputs method of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::testElectricalOutputs()
{
    std::cout << "\nTest Electrical Outputs           ";

    /// - Set up a test article with nominal config & input data.
    FriendlyTsDcPwmDynMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify coasting.
    article.mPulseWidth       = 0.1;
    article.mMotorSpeed       = 3000.0;
    double expectedPwmFactor  = tPulseWasteFraction + 0.1 * (1.0 - tPulseWasteFraction);
    double expectedCurrent    = expectedPwmFactor * (tVoltage - tBemfConstant * 3000.0)
                              / tArmatureResistance;
    double expectedPower      = expectedCurrent * tVoltage;
    double expectedResistance = tVoltage / expectedCurrent;
    double expectedGenerated  = tBemfConstant * 3000.0 / tArmatureResistance - expectedCurrent;

    article.computeElectricalOutputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,    article.getCurrent(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,      article.getPower(),            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance, article.getResistance(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGenerated,  article.getGeneratedCurrent(), DBL_EPSILON);

    /// - Verify normal operation with no degrade malf.
    article.mPulseWidth       = 0.5;
    article.mMotorSpeed       = 1000.0;
    expectedPwmFactor  = tPulseWasteFraction + 0.5 * (1.0 - tPulseWasteFraction);
    expectedCurrent    = expectedPwmFactor * (tVoltage - tBemfConstant * 1000.0)
                              / tArmatureResistance;
    expectedPower      = expectedCurrent * tVoltage;
    expectedResistance = tVoltage / expectedCurrent;
    expectedGenerated  = 0.0;

    article.computeElectricalOutputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,    article.getCurrent(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,      article.getPower(),            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance, article.getResistance(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGenerated,  article.getGeneratedCurrent(), DBL_EPSILON);

    /// - Verify effects of degrade malf.
    article.mMalfDegradeValue = 1.0;
    article.mMalfDegradeFlag  = true;
    expectedCurrent           = 0.0;
    expectedPower             = 0.0;
    expectedResistance        = 1.0 / DBL_EPSILON;

    article.computeElectricalOutputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,    article.getCurrent(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,      article.getPower(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance, article.getResistance(), DBL_EPSILON);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the computeWasteHeat method of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::testWasteHeat()
{
    std::cout << "\nTest Waste Heat                   ";

    /// - Set up a test article with nominal config & input data.
    FriendlyTsDcPwmDynMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify normal power with no bias.
    article.mTotalExternalLoad = -0.001;
    article.mMotorSpeed        = 1000.0;
    article.mPower             = 100.0;
    double usefulPower         = 0.001 * 1000.0 / RPMTORADS;
    double expectedWasteHeat   = 100.0 - usefulPower;
    double expectedEfficiency  = usefulPower / 100.0;

    article.computeWasteHeat();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWasteHeat,  article.getWasteHeat(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEfficiency, article.getEfficiency(), DBL_EPSILON);

    /// - Verify no power case and heat bias.
    article.mPower                 = 0.0;
    article.mMalfHeatRateBiasValue = 2000.0;
    article.mMalfHeatRateBiasFlag  = true;
    expectedWasteHeat              = 2000.0;
    expectedEfficiency             = 0.0;

    article.computeWasteHeat();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWasteHeat,  article.getWasteHeat(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEfficiency, article.getEfficiency(), DBL_EPSILON);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  End-to-end test of the Motor step method with all effects.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsDcPwmDynMotor::testStep()
{
    std::cout << "\nTest Step                         ";

    /// - Set up a test article with nominal config & input data.
    FriendlyTsDcPwmDynMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Set up inputs & malfs
    article.mPulseWasteFraction    = 0.0;
    article.mMotorSpeed            = 500.0;
    article.mPulseWidth            = 1.0;
    article.mMalfDegradeValue      = 0.1;
    article.mMalfDegradeFlag       = true;
    article.mMalfJamValue          = 0.1;
    article.mMalfJamFlag           = true;
    article.mLoadTorques[0]        = -0.002;
    article.mMalfHeatRateBiasValue = 2000.0;
    article.mMalfHeatRateBiasFlag  = true;

    double expectedStallTorque = 0.9 * (tStallTorqueCoeff1 * tVoltage
                                      + tStallTorqueCoeff2 * tVoltage * tVoltage);
    double expectedDriveTorque = (expectedStallTorque - 500.0 / tSpeedLoadRatio);
    double expectedTotalLoad   = -0.002 - 0.1 * expectedStallTorque;
    double expectedMotorSpeed  = 500 + (expectedDriveTorque + expectedTotalLoad + tFrictionTorque)
                               * 0.1 * RPMTORADS / tInertia;
    double expectedCurrent     = 0.9 * (tVoltage - tBemfConstant * expectedMotorSpeed)
                               / tArmatureResistance;
    double expectedPower       = expectedCurrent * tVoltage;
    double expectedResistance  = tVoltage / expectedCurrent;
    double usefulPower         = -expectedTotalLoad * expectedMotorSpeed / RPMTORADS;
    double expectedWasteHeat   = expectedPower - usefulPower + 2000.0;
    double expectedEfficiency  = usefulPower / expectedPower;

    article.step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStallTorque, article.mStallTorque,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDriveTorque, article.mDriveTorque,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTotalLoad,   article.mTotalExternalLoad, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMotorSpeed,  article.getSpeed(),         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,     article.getCurrent(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,       article.getPower(),         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance,  article.getResistance(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWasteHeat,   article.getWasteHeat(),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEfficiency,  article.getEfficiency(),    DBL_EPSILON);

    std::cout << "Pass";
    std::cout << "\n--------------------------------------------------------------------------------";
}
