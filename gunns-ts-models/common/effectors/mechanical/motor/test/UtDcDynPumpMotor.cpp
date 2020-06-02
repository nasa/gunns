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
    (common/effectors/mechanical/motor/DcDynPumpMotor.o)
 )

 PROGRAMMERS:
 (
  ((Jason Harvey) (L-3 Communications) (2014-03) (TS21) (Initial))
 )
 **************************************************************************************************/
#include <iostream>
#include "UtDcDynPumpMotor.hh"
#include "software/exceptions/TsInitializationException.hh"

/// @details  Acts as a redundant test on UnitConversion class.
const double UtDcDynPumpMotor::RPMTORADS = 9.5492965855137211;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtDcDynPumpMotor::UtDcDynPumpMotor()
    :
    tName(),
    tWindingResistance(),
    tTorqueConstant(),
    tFrictionConstant(),
    tFrictionMinSpeed(),
    tInertia(),
    tNominalConfig(),
    tVoltage(),
    tCurrentLimit(),
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
UtDcDynPumpMotor::~UtDcDynPumpMotor()
{
   // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDcDynPumpMotor::setUp()
{
    tName               = "Test";
    tWindingResistance = 4.38;
    tTorqueConstant    = 2.72772E-2;
    tFrictionConstant  = 5.34660E-7;
    tFrictionMinSpeed  = 4000.0;
    tInertia           = 1.88841E-3;
    tNominalConfig     = new DcDynPumpMotorConfigData(tWindingResistance,
                                                      tTorqueConstant,
                                                      tFrictionConstant,
                                                      tFrictionMinSpeed,
                                                      tInertia);

    tVoltage           = 124.0;
    tCurrentLimit      = 1.0E15;
    tMotorSpeed        = 0.0;
    tLoadTorque1       = 0.0;
    tLoadTorque2       = 0.0;
    tLoadTorque3       = 0.0;
    tLoadTorque4       = 0.0;
    tNominalInput      = new DcDynPumpMotorInputData(tVoltage,
                                                     tCurrentLimit,
                                                     tMotorSpeed,
                                                     tLoadTorque1,
                                                     tLoadTorque2,
                                                     tLoadTorque3,
                                                     tLoadTorque4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDcDynPumpMotor::tearDown()
{
    delete tNominalInput;
    delete tNominalConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructors & destructors of the Motor config data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDcDynPumpMotor::testConfigData() {
    std::cout << "\n--------------------------------------------------------------------------------";
    std::cout << "\nDcDynPumpMotor unit tests";
    std::cout << "\nTest Config Data                  ";

    /// - Test default construction of a test config data article.
    DcDynPumpMotorConfigData defaultConfig;
    CPPUNIT_ASSERT(0.0                == defaultConfig.mWindingResistance);
    CPPUNIT_ASSERT(0.0                == defaultConfig.mTorqueConstant);
    CPPUNIT_ASSERT(0.0                == defaultConfig.mFrictionConstant);
    CPPUNIT_ASSERT(0.0                == defaultConfig.mFrictionMinSpeed);
    CPPUNIT_ASSERT(0.0                == defaultConfig.mInertia);

    /// - Test nominal construction of a test config data article.
    CPPUNIT_ASSERT(tWindingResistance == tNominalConfig->mWindingResistance);
    CPPUNIT_ASSERT(tTorqueConstant    == tNominalConfig->mTorqueConstant);
    CPPUNIT_ASSERT(tFrictionConstant  == tNominalConfig->mFrictionConstant);
    CPPUNIT_ASSERT(tFrictionMinSpeed  == tNominalConfig->mFrictionMinSpeed);
    CPPUNIT_ASSERT(tInertia           == tNominalConfig->mInertia);

    /// - Test copy construction of a test config data article.
    DcDynPumpMotorConfigData copyConfig(*tNominalConfig);
    CPPUNIT_ASSERT(tWindingResistance == copyConfig.mWindingResistance);
    CPPUNIT_ASSERT(tTorqueConstant    == copyConfig.mTorqueConstant);
    CPPUNIT_ASSERT(tFrictionConstant  == copyConfig.mFrictionConstant);
    CPPUNIT_ASSERT(tFrictionMinSpeed  == copyConfig.mFrictionMinSpeed);
    CPPUNIT_ASSERT(tInertia           == copyConfig.mInertia);

    /// - Test assignment of a test config data article.
    defaultConfig = *tNominalConfig;
    CPPUNIT_ASSERT(tWindingResistance == defaultConfig.mWindingResistance);
    CPPUNIT_ASSERT(tTorqueConstant    == defaultConfig.mTorqueConstant);
    CPPUNIT_ASSERT(tFrictionConstant  == defaultConfig.mFrictionConstant);
    CPPUNIT_ASSERT(tFrictionMinSpeed  == defaultConfig.mFrictionMinSpeed);
    CPPUNIT_ASSERT(tInertia           == defaultConfig.mInertia);

    /// - Test self-assignment of a test config data article.
    CPPUNIT_ASSERT(&defaultConfig == &(defaultConfig = defaultConfig));

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Motor input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDcDynPumpMotor::testInputData()
{
    std::cout << "\nTest Input Data                   ";

    /// - Test default construction of a test input data article.
    DcDynPumpMotorInputData defaultInput;
    CPPUNIT_ASSERT(0.0    == defaultInput.mVoltage);
    CPPUNIT_ASSERT(1.0E15 == defaultInput.mCurrentLimit);
    CPPUNIT_ASSERT(0.0    == defaultInput.mMotorSpeed);
    CPPUNIT_ASSERT(0.0    == defaultInput.mLoadTorque1);
    CPPUNIT_ASSERT(0.0    == defaultInput.mLoadTorque2);
    CPPUNIT_ASSERT(0.0    == defaultInput.mLoadTorque3);
    CPPUNIT_ASSERT(0.0    == defaultInput.mLoadTorque4);
    CPPUNIT_ASSERT(false  == defaultInput.mMalfDegradeFlag);
    CPPUNIT_ASSERT(0.0    == defaultInput.mMalfDegradeValue);
    CPPUNIT_ASSERT(false  == defaultInput.mMalfJamFlag);
    CPPUNIT_ASSERT(0.0    == defaultInput.mMalfJamValue);
    CPPUNIT_ASSERT(false  == defaultInput.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(0.0    == defaultInput.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(false  == defaultInput.mMalfHeatRateBiasFlag);
    CPPUNIT_ASSERT(0.0    == defaultInput.mMalfHeatRateBiasValue);

    /// - Test nominal construction of a test input data article.
    DcDynPumpMotorInputData nominalInput(16.0, 20.0, 13000.0, -1.0, -2.0, -3.0, -4.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(16.0,    nominalInput.mVoltage,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(20.0,    nominalInput.mCurrentLimit, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13000.0, nominalInput.mMotorSpeed,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0,    nominalInput.mLoadTorque1,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0,    nominalInput.mLoadTorque2,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0,    nominalInput.mLoadTorque3,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0,    nominalInput.mLoadTorque4,  DBL_EPSILON);
    CPPUNIT_ASSERT(false  == defaultInput.mMalfDegradeFlag);
    CPPUNIT_ASSERT(0.0    == defaultInput.mMalfDegradeValue);
    CPPUNIT_ASSERT(false  == defaultInput.mMalfJamFlag);
    CPPUNIT_ASSERT(0.0    == defaultInput.mMalfJamValue);
    CPPUNIT_ASSERT(false  == defaultInput.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(0.0    == defaultInput.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(false  == defaultInput.mMalfHeatRateBiasFlag);
    CPPUNIT_ASSERT(0.0    == defaultInput.mMalfHeatRateBiasValue);

    /// - Test copy construction of a test config data article.
    nominalInput.mMalfDegradeFlag        = true;
    nominalInput.mMalfDegradeValue       = 1.0;
    nominalInput.mMalfJamFlag            = true;
    nominalInput.mMalfJamValue           = 0.5;
    nominalInput.mMalfSpeedOverrideFlag  = true;
    nominalInput.mMalfSpeedOverrideValue = 2.0;
    nominalInput.mMalfHeatRateBiasFlag   = true;
    nominalInput.mMalfHeatRateBiasValue  = 5.0;
    DcDynPumpMotorInputData copyInput(nominalInput);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(16.0,    copyInput.mVoltage,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(20.0,    copyInput.mCurrentLimit,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13000.0, copyInput.mMotorSpeed,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0,    copyInput.mLoadTorque1,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0,    copyInput.mLoadTorque2,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0,    copyInput.mLoadTorque3,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0,    copyInput.mLoadTorque4,     DBL_EPSILON);
    CPPUNIT_ASSERT(true   == copyInput.mMalfDegradeFlag);
    CPPUNIT_ASSERT(1.0    == copyInput.mMalfDegradeValue);
    CPPUNIT_ASSERT(true   == copyInput.mMalfJamFlag);
    CPPUNIT_ASSERT(0.5    == copyInput.mMalfJamValue);
    CPPUNIT_ASSERT(true   == copyInput.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(2.0    == copyInput.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(true   == copyInput.mMalfHeatRateBiasFlag);
    CPPUNIT_ASSERT(5.0    == copyInput.mMalfHeatRateBiasValue);

    /// - Test assignment of a test input data article.
    defaultInput = nominalInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(16.0,    defaultInput.mVoltage,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(20.0,    defaultInput.mCurrentLimit, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13000.0, defaultInput.mMotorSpeed,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0,    defaultInput.mLoadTorque1,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0,    defaultInput.mLoadTorque2,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0,    defaultInput.mLoadTorque3,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0,    defaultInput.mLoadTorque4,  DBL_EPSILON);
    CPPUNIT_ASSERT(true   == defaultInput.mMalfDegradeFlag);
    CPPUNIT_ASSERT(1.0    == defaultInput.mMalfDegradeValue);
    CPPUNIT_ASSERT(true   == defaultInput.mMalfJamFlag);
    CPPUNIT_ASSERT(0.5    == defaultInput.mMalfJamValue);
    CPPUNIT_ASSERT(true   == defaultInput.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(2.0    == defaultInput.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(true   == defaultInput.mMalfHeatRateBiasFlag);
    CPPUNIT_ASSERT(5.0    == defaultInput.mMalfHeatRateBiasValue);

    /// - Test self-assignment of a test input data article.
    CPPUNIT_ASSERT(&defaultInput == &(defaultInput = defaultInput));

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the constructors & destructors of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDcDynPumpMotor::testConstruction()
{
    std::cout << "\nTest Construction                 ";

    /// - Test default construction of the test article.
    FriendlyDcDynPumpMotor article;
    CPPUNIT_ASSERT(false == article.mMalfDegradeFlag);
    CPPUNIT_ASSERT(0.0   == article.mMalfDegradeValue);
    CPPUNIT_ASSERT(false == article.mMalfJamFlag);
    CPPUNIT_ASSERT(0.0   == article.mMalfJamValue);
    CPPUNIT_ASSERT(false == article.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(0.0   == article.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(false == article.mMalfHeatRateBiasFlag);
    CPPUNIT_ASSERT(0.0   == article.mMalfHeatRateBiasValue);
    CPPUNIT_ASSERT(""    == article.mName);
    CPPUNIT_ASSERT(0.0   == article.mWindingResistance);
    CPPUNIT_ASSERT(0.0   == article.mTorqueConstant);
    CPPUNIT_ASSERT(0.0   == article.mFrictionConstant);
    CPPUNIT_ASSERT(0.0   == article.mFrictionMinSpeed);
    CPPUNIT_ASSERT(0.0   == article.mInertia);
    CPPUNIT_ASSERT(0.0   == article.mVoltage);
    CPPUNIT_ASSERT(0.0   == article.mCurrentLimit);
    CPPUNIT_ASSERT(0.0   == article.mMotorSpeed);
    CPPUNIT_ASSERT(0.0   == article.mLoadTorques[0]);
    CPPUNIT_ASSERT(0.0   == article.mLoadTorques[1]);
    CPPUNIT_ASSERT(0.0   == article.mLoadTorques[2]);
    CPPUNIT_ASSERT(0.0   == article.mLoadTorques[3]);
    CPPUNIT_ASSERT(0.0   == article.mPower);
    CPPUNIT_ASSERT(0.0   == article.mCurrent);
    CPPUNIT_ASSERT(0.0   == article.mResistance);
    CPPUNIT_ASSERT(0.0   == article.mWasteHeat);
    CPPUNIT_ASSERT(0.0   == article.mEfficiency);
    CPPUNIT_ASSERT(0.0   == article.mStallTorque);
    CPPUNIT_ASSERT(0.0   == article.mDriveTorque);
    CPPUNIT_ASSERT(0.0   == article.mFrictionTorque);
    CPPUNIT_ASSERT(0.0   == article.mTotalExternalLoad);
    CPPUNIT_ASSERT(0.0   == article.mDegradedResistance);
    CPPUNIT_ASSERT(false == article.mInitFlag);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the initialization of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDcDynPumpMotor::testInitialize()
{
    std::cout << "\nTest Initialize                   ";

    /// - Test nominal initialization of the test article.
    FriendlyDcDynPumpMotor article;
    tNominalInput->mMalfDegradeFlag          = true;
    tNominalInput->mMalfDegradeValue         = 1.0;
    tNominalInput->mMalfJamFlag              = true;
    tNominalInput->mMalfJamValue             = 0.5;
    tNominalInput->mMalfSpeedOverrideFlag    = true;
    tNominalInput->mMalfSpeedOverrideValue   = 2.0;
    tNominalInput->mMalfHeatRateBiasFlag     = true;
    tNominalInput->mMalfHeatRateBiasValue    = 5.0;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    CPPUNIT_ASSERT(true               == article.mMalfDegradeFlag);
    CPPUNIT_ASSERT(1.0                == article.mMalfDegradeValue);
    CPPUNIT_ASSERT(true               == article.mMalfJamFlag);
    CPPUNIT_ASSERT(0.5                == article.mMalfJamValue);
    CPPUNIT_ASSERT(true               == article.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(2.0                == article.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(true               == article.mMalfHeatRateBiasFlag);
    CPPUNIT_ASSERT(5.0                == article.mMalfHeatRateBiasValue);
    CPPUNIT_ASSERT(tName              == article.mName);
    CPPUNIT_ASSERT(tWindingResistance == article.mWindingResistance);
    CPPUNIT_ASSERT(tTorqueConstant    == article.mTorqueConstant);
    CPPUNIT_ASSERT(tFrictionConstant  == article.mFrictionConstant);
    CPPUNIT_ASSERT(tFrictionMinSpeed  == article.mFrictionMinSpeed);
    CPPUNIT_ASSERT(tInertia           == article.mInertia);
    CPPUNIT_ASSERT(tVoltage           == article.mVoltage);
    CPPUNIT_ASSERT(tCurrentLimit      == article.mCurrentLimit);
    CPPUNIT_ASSERT(tMotorSpeed        == article.mMotorSpeed);
    CPPUNIT_ASSERT(tLoadTorque1       == article.mLoadTorques[0]);
    CPPUNIT_ASSERT(tLoadTorque2       == article.mLoadTorques[1]);
    CPPUNIT_ASSERT(tLoadTorque3       == article.mLoadTorques[2]);
    CPPUNIT_ASSERT(tLoadTorque4       == article.mLoadTorques[3]);
    CPPUNIT_ASSERT(0.0                == article.mPower);
    CPPUNIT_ASSERT(0.0                == article.mCurrent);
    CPPUNIT_ASSERT(0.0                == article.mResistance);
    CPPUNIT_ASSERT(0.0                == article.mWasteHeat);
    CPPUNIT_ASSERT(0.0                == article.mEfficiency);
    CPPUNIT_ASSERT(0.0                == article.mStallTorque);
    CPPUNIT_ASSERT(0.0                == article.mDriveTorque);
    CPPUNIT_ASSERT(0.0                == article.mFrictionTorque);
    CPPUNIT_ASSERT(0.0                == article.mTotalExternalLoad);
    CPPUNIT_ASSERT(0.0                == article.mDegradedResistance);
    CPPUNIT_ASSERT(true               == article.mInitFlag);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the initialization exceptions of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDcDynPumpMotor::testInitializeExceptions()
{
    std::cout << "\nTest Initialization Exceptions    ";

    FriendlyDcDynPumpMotor article;
    /// - Verify exception is thrown when no object name is given.
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, ""),
                         TsInitializationException);

    /// - Verify exception is thrown when winding resistance < DBL_EPSILON.
    tNominalConfig->mWindingResistance = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mWindingResistance = tWindingResistance;

    /// - Verify exception is thrown when torque constant < DBL_EPSILON.
    tNominalConfig->mTorqueConstant = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mTorqueConstant = tTorqueConstant;

    /// - Verify exception is thrown when friction constant < 0.0.
    tNominalConfig->mFrictionConstant = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mFrictionConstant = tFrictionConstant;

    /// - Verify exception is thrown when friction minimum speed < 0.0.
    tNominalConfig->mFrictionMinSpeed = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mFrictionMinSpeed = tFrictionMinSpeed;

    /// - Verify exception is thrown when inertia < DBL_EPSILON.
    tNominalConfig->mInertia = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalConfig->mInertia = tInertia;

    /// - Verify exception is thrown when initial voltage < 0.
    tNominalInput->mVoltage = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);
    tNominalInput->mVoltage = tVoltage;

    /// - Verify exception is thrown when initial speed < 0.
    tNominalInput->mMotorSpeed = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName),
                         TsInitializationException);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the setter & getter methods of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDcDynPumpMotor::testAccessors()
{
    std::cout << "\nTest Access Methods               ";

    /// - Set up a test article with nominal config & input data.
    FriendlyDcDynPumpMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));
    CPPUNIT_ASSERT(article.isInitialized());

    /// - Test the methods to set input values.
    article.setVoltage(20.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(20.0,  article.mVoltage,        DBL_EPSILON);
    article.setCurrentLimit(10.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0,  article.mCurrentLimit,   DBL_EPSILON);
    article.setLoadTorques(-1.0, -2.0, -3.0, -4.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0,  article.mLoadTorques[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0,  article.mLoadTorques[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0,  article.mLoadTorques[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0,  article.mLoadTorques[3], DBL_EPSILON);

    /// - Test the methods to get output values.
    article.mCurrent    = 21.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(21.0,  article.getCurrent(),    DBL_EPSILON);
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
void UtDcDynPumpMotor::testMotorTorque()
{
    std::cout << "\nTest Motor Torque                 ";

    /// - Set up a test article with nominal config & input data.
    FriendlyDcDynPumpMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    article.mDegradedResistance = tWindingResistance;
    article.mCurrent            = 1.5;
    double expectedStallTorque  = tTorqueConstant * tVoltage / tWindingResistance;
    double expectedDriveTorque  = tTorqueConstant * 1.5;

    article.generateMotorTorque();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStallTorque, article.mStallTorque, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDriveTorque, article.mDriveTorque, DBL_EPSILON);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the gatherExternalLoads method of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDcDynPumpMotor::testExternalLoads()
{
    std::cout << "\nTest External Loads               ";

    /// - Set up a test article with nominal config & input data.
    FriendlyDcDynPumpMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify the total external load without jam malfunction.
    article.mLoadTorques[0]  = -0.001;
    article.mLoadTorques[1]  = -0.002;
    article.mLoadTorques[2]  = -0.003;
    article.mLoadTorques[3]  = -0.004;
    double expectedTotalLoad = -0.01;

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
void UtDcDynPumpMotor::testMotorSpeed()
{
    std::cout << "\nTest Motor Speed                  ";

    /// - Set up a test article with nominal config & input data.
    FriendlyDcDynPumpMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify motor acceleration without override.
    article.mDriveTorque       =  0.01;
    article.mTotalExternalLoad = -0.001;
    article.mMotorSpeed        =  4500.0;
    double expectedFriction    = -tFrictionConstant * 4500.0;
    double expectedMotorSpeed  = 4500 + (0.01 - 0.001 + expectedFriction) * 0.1 * RPMTORADS / tInertia;

    article.computeMotorSpeed(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFriction,   article.mFrictionTorque, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMotorSpeed, article.getSpeed(),      DBL_EPSILON);

    /// - Verify deceleration stops at zero.
    article.mTotalExternalLoad = -10000.0;
    expectedMotorSpeed         =  0.0;

    article.computeMotorSpeed(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMotorSpeed, article.getSpeed(),      DBL_EPSILON);

    /// - Verify minimum speed dynamic friction.
    expectedFriction           = -tFrictionConstant * tFrictionMinSpeed;
    article.computeMotorSpeed(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFriction,   article.mFrictionTorque, DBL_EPSILON);

    /// - Verify speed override.
    article.mMalfSpeedOverrideFlag  = true;
    article.mMalfSpeedOverrideValue = 666.0;

    article.computeMotorSpeed(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(666.0,              article.getSpeed(),      DBL_EPSILON);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the computeElectricalOutputs method of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDcDynPumpMotor::testElectricalOutputs()
{
    std::cout << "\nTest Electrical Outputs           ";

    /// - Set up a test article with nominal config & input data.
    FriendlyDcDynPumpMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Verify coasting.
    article.mVoltage          = 0.0;
    article.mMotorSpeed       = 3000.0;
    double expectedDegradedR  = tWindingResistance;
    double expectedCurrent    = 0.0;
    double expectedPower      = 0.0;
    double expectedResistance = 0.0;

    article.computeElectricalOutputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDegradedR,  article.mDegradedResistance,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,    article.getCurrent(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,      article.getPower(),            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance, article.getResistance(),       DBL_EPSILON);

    /// - Verify normal operation with no degrade malf.
    article.mVoltage          = tVoltage;
    article.mMotorSpeed       = 1000.0;
    expectedDegradedR         = tWindingResistance;
    expectedCurrent           = (tVoltage - tTorqueConstant * 1000.0 / UtDcDynPumpMotor::RPMTORADS)
                              / expectedDegradedR;
    expectedPower             = tVoltage * expectedCurrent;
    expectedResistance        = tVoltage / expectedCurrent;

    article.computeElectricalOutputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDegradedR,  article.mDegradedResistance,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,    article.getCurrent(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,      article.getPower(),            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance, article.getResistance(),       DBL_EPSILON);

    /// - Verify effects of degrade malf.
    article.mMalfDegradeValue = 1.0;
    article.mMalfDegradeFlag  = true;
    expectedDegradedR         = tWindingResistance / DBL_EPSILON;
    expectedCurrent           = (tVoltage - tTorqueConstant * 1000.0 / UtDcDynPumpMotor::RPMTORADS)
                              / expectedDegradedR;
    expectedPower             = tVoltage * expectedCurrent;
    expectedResistance        = tVoltage / expectedCurrent;

    article.computeElectricalOutputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDegradedR,  article.mDegradedResistance,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,    article.getCurrent(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,      article.getPower(),            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance, article.getResistance(),       DBL_EPSILON);

    /// - Verify current limiting.
    article.mMalfDegradeFlag  = false;
    article.mCurrentLimit     = 10.0;
    expectedDegradedR         = tWindingResistance;
    expectedCurrent           = 10.0;
    expectedPower             = tVoltage * expectedCurrent;
    expectedResistance        = tVoltage / expectedCurrent;

    article.computeElectricalOutputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDegradedR,  article.mDegradedResistance,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,    article.getCurrent(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,      article.getPower(),            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance, article.getResistance(),       DBL_EPSILON);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the computeWasteHeat method of the Motor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDcDynPumpMotor::testWasteHeat()
{
    std::cout << "\nTest Waste Heat                   ";

    /// - Set up a test article with nominal config & input data.
    FriendlyDcDynPumpMotor article;
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
void UtDcDynPumpMotor::testStep()
{
    std::cout << "\nTest Step                         ";

    /// - Set up a test article with nominal config & input data.
    FriendlyDcDynPumpMotor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tNominalConfig, *tNominalInput, tName));

    /// - Set up inputs & malfs
    article.mMotorSpeed            = 4500.0;
    article.mMalfDegradeValue      = 0.1;
    article.mMalfDegradeFlag       = true;
    article.mMalfJamValue          = 0.1;
    article.mMalfJamFlag           = true;
    article.mLoadTorques[0]        = -0.002;
    article.mMalfHeatRateBiasValue = 2000.0;
    article.mMalfHeatRateBiasFlag  = true;

    double expectedDegradedR   = tWindingResistance / 0.9;
    double expectedCurrent     = (tVoltage - tTorqueConstant * 4500.0 / UtDcDynPumpMotor::RPMTORADS)
                               / expectedDegradedR;
    double expectedPower       = expectedCurrent * tVoltage;
    double expectedResistance  = tVoltage / expectedCurrent;
    double expectedStallTorque = tTorqueConstant * tVoltage / expectedDegradedR;
    double expectedDriveTorque = tTorqueConstant * expectedCurrent;
    double expectedTotalLoad   = -0.002 - 0.1 * expectedStallTorque;
    double expectedFriction    = -tFrictionConstant * 4500.0;
    double expectedMotorSpeed  = 4500 + (expectedDriveTorque + expectedTotalLoad + expectedFriction)
                               * 0.1 * RPMTORADS / tInertia;
    double usefulPower         = -expectedTotalLoad * expectedMotorSpeed / RPMTORADS;
    double expectedWasteHeat   = expectedPower - usefulPower + 2000.0;
    double expectedEfficiency  = usefulPower / expectedPower;

    article.step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDegradedR,   article.mDegradedResistance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,     article.getCurrent(),        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,       article.getPower(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedResistance,  article.getResistance(),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStallTorque, article.mStallTorque,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDriveTorque, article.mDriveTorque,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTotalLoad,   article.mTotalExternalLoad,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFriction,    article.mFrictionTorque,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMotorSpeed,  article.getSpeed(),          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWasteHeat,   article.getWasteHeat(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEfficiency,  article.getEfficiency(),     DBL_EPSILON);

    std::cout << "Pass";
    std::cout << "\n--------------------------------------------------------------------------------";
}
