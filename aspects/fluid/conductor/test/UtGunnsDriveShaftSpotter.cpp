/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((GunnsDriveShaftSpotter.o)
  (GunnsGasTurbine.o)
  (aspects/fluid/potential/GunnsGasFan.o))
***************************************************************************************************/

#include "UtGunnsDriveShaftSpotter.hh"
#include "core/GunnsFluidConductor.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsDriveShaftSpotter::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsDriveShaftSpotter class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDriveShaftSpotter::UtGunnsDriveShaftSpotter()
    :
    tArticle(),
    tName("test article"),
    tConfig(tName),
    tInput(),
    tTurbRef(),
    tFanRef(),
    tTurbine(),
    tFan(),
    tFrictionConstant(2.0E-6),
    tFrictionMinSpeed(100.0),
    tInertia(0.0005),
    tMotorSpeed(3000.0),
    tFrictionTorque(0.0),
    tTotalExternalLoad(0.0)
{
    tTurbRef.push_back(&tTurbine);
    tFanRef.push_back(&tFan);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsDriveShaftSpotter class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDriveShaftSpotter::~UtGunnsDriveShaftSpotter()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDriveShaftSpotter::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDriveShaftSpotter::setUp()
{
    tConfig.mName             = tName;
    tConfig.mFrictionConstant = tFrictionConstant;
    tConfig.mFrictionMinSpeed = tFrictionMinSpeed;
    tConfig.mInertia          = tInertia;
    tInput.mMotorSpeed        = tMotorSpeed;
    tInput.mMalfJamFlag       = false;
    tInput.mMalfJamValue      = 0.0;
    tInput.mMalfSpeedOverrideFlag = false;
    tInput.mMalfSpeedOverrideValue= 0.0;

    tFrictionTorque = -tFrictionConstant*std::max(tMotorSpeed, tFrictionMinSpeed);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsDriveShaftSpotterConfigData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDriveShaftSpotter::testConfig()
{
    UT_RESULT_FIRST;

    /// - Test default config data construction.
    GunnsDriveShaftSpotterConfigData article(tName);
    CPPUNIT_ASSERT(tName == article.mName);
    CPPUNIT_ASSERT(0.0   == article.mFrictionConstant);
    CPPUNIT_ASSERT(0.0   == article.mFrictionMinSpeed);
    CPPUNIT_ASSERT(0.0   == article.mInertia);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsDriveShaftSpotterInputData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDriveShaftSpotter::testInput()
{
    UT_RESULT;

    /// - Test default input data construction.
    GunnsDriveShaftSpotterInputData article;
    CPPUNIT_ASSERT(0.0 == article.mMotorSpeed);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsDriveShaftSpotter class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDriveShaftSpotter::testDefaultConstruction()
{
    UT_RESULT;

    CPPUNIT_ASSERT(""          ==  tArticle.mName);
    CPPUNIT_ASSERT(false       ==  tArticle.mInitFlag);
    CPPUNIT_ASSERT(false       ==  tArticle.mMalfJamFlag);
    CPPUNIT_ASSERT(0.0         ==  tArticle.mMalfJamValue);
    CPPUNIT_ASSERT(false       ==  tArticle.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(0.0         ==  tArticle.mMalfSpeedOverrideValue);
    CPPUNIT_ASSERT(0.0         ==  tArticle.mFrictionConstant);
    CPPUNIT_ASSERT(0.0         ==  tArticle.mFrictionMinSpeed);
    CPPUNIT_ASSERT(0.0         ==  tArticle.mInertia);
    CPPUNIT_ASSERT(0.0         ==  tArticle.mMotorSpeed);
    CPPUNIT_ASSERT(0.0         ==  tArticle.mFrictionTorque);
    CPPUNIT_ASSERT(0.0         ==  tArticle.mTotalExternalLoad);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsDriveShaftSpotter class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDriveShaftSpotter::testInitialize()
{
    UT_RESULT;

    /// - Test exception thrown from missing name.
    tConfig.mName = "";
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    tConfig.mName = tName;

    /// - Test exception thrown from null config data.
    const GunnsDriveShaftSpotterConfigData* nullConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(nullConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown from null input data.
    const GunnsDriveShaftSpotterInputData* nullInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, nullInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown on bad config data pointer type.
    BadSpotterConfigData* badConfig = new BadSpotterConfigData(tName);
    CPPUNIT_ASSERT_THROW(tArticle.initialize(badConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badConfig;

    /// - Test exception thrown on bad input data pointer type.
    BadSpotterInputData* badInput = new BadSpotterInputData();
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, badInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badInput;

    /// - Test exception thrown from negative friction constant.
    tConfig.mFrictionConstant = -1.0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tConfig.mFrictionConstant = 2.0E-6;

    /// - Test exception thrown from negative friction min speed.
    tConfig.mFrictionMinSpeed= -1.0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tConfig.mFrictionMinSpeed = 100.0;

    /// - Test exception thrown from negative inertia.
    tConfig.mInertia= -1.0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tConfig.mInertia = 0.0005;

    /// - Test exception thrown from negative jam malfunction value.
    tInput.mMalfJamValue= -1.0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tInput.mMalfJamValue = 0.0;

    tArticle.initialize(&tConfig, &tInput);

    /// - Test nominal initialization.
    CPPUNIT_ASSERT(tName       ==  tArticle.mName);
    CPPUNIT_ASSERT(true        ==  tArticle.mInitFlag);
    CPPUNIT_ASSERT(0.0         ==  tArticle.mFrictionTorque);
    CPPUNIT_ASSERT(2.0E-6      ==  tArticle.mFrictionConstant);
    CPPUNIT_ASSERT(100.0       ==  tArticle.mFrictionMinSpeed);
    CPPUNIT_ASSERT(0.0005      ==  tArticle.mInertia);
    CPPUNIT_ASSERT(3000.0      ==  tArticle.mMotorSpeed);
    CPPUNIT_ASSERT(0.0         ==  tArticle.mTotalExternalLoad);
    CPPUNIT_ASSERT(false       ==  tArticle.mMalfJamFlag);
    CPPUNIT_ASSERT(0.0         ==  tArticle.mMalfJamValue);
    CPPUNIT_ASSERT(false       ==  tArticle.mMalfSpeedOverrideFlag);
    CPPUNIT_ASSERT(0.0         ==  tArticle.mMalfSpeedOverrideValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the addReference method of the GunnsDriveShaftSpotter class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDriveShaftSpotter::testAddReference()
{
    UT_RESULT;

    GunnsGasFan testFan;
    GunnsGasTurbine testTurbine;
    GunnsFluidConductor badInput;

    try{
        tArticle.addImpeller(&testFan);
        CPPUNIT_ASSERT(tArticle.mFanRef[0]);
    } catch(TsInitializationException& e){
        CPPUNIT_ASSERT(false);
    }
    try{
        tArticle.addImpeller(&testTurbine);
        CPPUNIT_ASSERT(tArticle.mTurbRef[0]);
    } catch(TsInitializationException& e){
        CPPUNIT_ASSERT(false);
    }
    CPPUNIT_ASSERT_THROW(tArticle.addImpeller(&badInput), TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsDriveShaftSpotter class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDriveShaftSpotter::testPreSolver()
{
    UT_RESULT;

    double motorSpeed = 3000.0 + (10 - 0.006) * 0.05 * UnitConversion::SEC_PER_MIN_PER_2PI / 0.0005;

    tArticle.mTotalExternalLoad = 10.0;
    tArticle.addImpeller(&tTurbine);
    tArticle.addImpeller(&tFan);
    tArticle.initialize(&tConfig, &tInput);
    tArticle.stepPreSolver(0.05);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.006, tArticle.mFrictionTorque, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(motorSpeed, tArticle.mMotorSpeed, DBL_EPSILON);
    CPPUNIT_ASSERT(tTurbine.mMotorSpeed == tArticle.mMotorSpeed);
    CPPUNIT_ASSERT(tFan.mMotorSpeed == tArticle.mMotorSpeed);

    tArticle.mMalfSpeedOverrideFlag = true;
    tArticle.mMalfSpeedOverrideValue = 1000.0;
    tArticle.stepPreSolver(0.05);
    CPPUNIT_ASSERT(1000.0 == tArticle.mMotorSpeed);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsDriveShaftSpotter class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDriveShaftSpotter::testPostSolver()
{
    UT_RESULT;

    // Initialize friendlies.
    FriendlyGunnsFluidNode2 nodeFanIn;
    FriendlyGunnsFluidNode2 nodeFanOut;
    FriendlyGunnsFluidNode2 nodeTurbIn;
    FriendlyGunnsFluidNode2 nodeTurbOut;
    FriendlyPolyFluid2 testFluid;

    // Set fan data.
    tFan.mImpellerTorque = 15.0;
    tFan.mWallHeatFlux = 5.0; //W
    tFan.mFlowRate = 1.5; //(kg/s) arbitrary
    testFluid.mSpecificEnthalpy = 10.0; //(J/kg) arbitrary
    nodeFanIn.getContent()->setState(&testFluid);
    testFluid.mSpecificEnthalpy = 60.0; //(J/kg) arbitrary
    nodeFanOut.getContent()->setState(&testFluid);

    // Set tubine data.
    tTurbine.mImpellerTorque = 10.0;
    tTurbine.mEfficiency = 0.9; //--
    tTurbine.mWallHeatFlux = 10.0; //W
    tTurbine.mFlowRate = 2.5; //(kg/s) arbitrary
    testFluid.mSpecificEnthalpy = 224.0; //(J/kg) arbitrary
    nodeTurbIn.getContent()->setState(&testFluid);
    testFluid.mSpecificEnthalpy =  24.0; //(J/kg) arbitrary
    nodeTurbOut.getContent()->setState(&testFluid);

    // Set up article for this test.
    tArticle.addImpeller(&tTurbine);
    tArticle.addImpeller(&tFan);
    tArticle.initialize(&tConfig, &tInput);

    // Check load calculation, and also that 
    // non-initialized turbines and fans don't cause an exception.
    CPPUNIT_ASSERT_NO_THROW(tArticle.stepPostSolver(0.0));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 25.0, tArticle.mTotalExternalLoad, DBL_EPSILON);

    // Pseudo-intialize link objects.
    tFan.allocateMatrixAndVectors("test");
    tFan.mNodes[0] = static_cast<GunnsBasicNode*>(&nodeFanIn);
    tFan.mNodes[1] = static_cast<GunnsBasicNode*>(&nodeFanOut);
    tFan.mInitFlag = true;
    tTurbine.allocateMatrixAndVectors("test");
    tTurbine.mNodes[0] = static_cast<GunnsBasicNode*>(&nodeTurbIn);
    tTurbine.mNodes[1] = static_cast<GunnsBasicNode*>(&nodeTurbOut);
    tTurbine.mInitFlag = true;

    // Test excess power calculation.
    tArticle.stepPostSolver(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(     75.0- 5.0, tArticle.mPowerInFan, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9*500.0-10.0, tArticle.mPowerOutTurb, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(370.0, tArticle.getPowerExcess(), DBL_EPSILON);

    // Test jam malfunction.
    tArticle.mMalfJamFlag = true;
    tArticle.mMalfJamValue = 1.0;
    tArticle.stepPostSolver(0.05);
    tArticle.initialize(&tConfig, &tInput);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle.mTotalExternalLoad, DBL_EPSILON);

    UT_PASS_LAST;
}