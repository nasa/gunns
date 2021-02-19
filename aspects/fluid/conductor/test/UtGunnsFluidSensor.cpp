/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidSensor.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidSensor.hh"

/// @details  Test identification number.
int UtGunnsFluidSensor::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Sensor link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSensor::UtGunnsFluidSensor()
    :
    CppUnit::TestFixture(),
    mTypes(),
    mFractions(),
    mFluidProperties(0),
    mFluidConfig(0),
    mFluidInput0(0),
    mFluidInput1(0),
    mLinks(),
    mName(""),
    mNodes(),
    mNodeList(),
    mPort0(0),
    mPort1(0),
    mMaxConductivity(0.0),
    mExpansionScaleFactor(0.0),
    mConfigData(0),
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mInputData(0),
    mArticle(0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Sensor link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSensor::~UtGunnsFluidSensor()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSensor::setUp()
{
    /// - Define the nominal port fluids.
    mFluidProperties      = new DefinedFluidProperties();
    mTypes[0]             = FluidProperties::GUNNS_N2;
    mTypes[1]             = FluidProperties::GUNNS_O2;
    mFractions[0]         = 0.5;
    mFractions[1]         = 0.5;
    mFluidConfig          = new PolyFluidConfigData(mFluidProperties, mTypes, N_FLUIDS);
    mFluidInput0          = new PolyFluidInputData(283.0,
                                                   109.0,
                                                     0.0,
                                                     0.0,
                                                     mFractions);
    mFluidInput1          = new PolyFluidInputData(283.0,
                                                   108.0,
                                                     0.0,
                                                     0.0,
                                                     mFractions);

    /// - Initialize the nodes.
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, *mFluidInput0);
    mNodes[1].getContent()->initialize(*mFluidConfig, *mFluidInput1);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Initialize the nodes list.
    mNodeList.mNodes      = mNodes;
    mNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    mName                 = "nominal";
    mMaxConductivity      = 20.0;
    mExpansionScaleFactor = 0.5;
    mConfigData           = new GunnsFluidSensorConfigData(mName,
                                                           &mNodeList,
                                                           mMaxConductivity,
                                                           mExpansionScaleFactor);

    /// - Define the nominal input data.
    mMalfBlockageFlag     = false;
    mMalfBlockageValue    = 0.3;
    mInputData            = new GunnsFluidSensorInputData(mMalfBlockageFlag,
                                                          mMalfBlockageValue);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;

    /// - Default construct the nominal test article.
    mArticle              = new FriendlyGunnsFluidSensor;

    /// - Define the nominal flow rate, time step and comparison tolerance.
    mTimeStep             = 0.1;
    mTolerance            = 1.0e-08;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSensor::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput1;
    delete mFluidInput0;
    delete mFluidConfig;
    delete mFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Fluid Sensor link model configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSensor::testConfigAndInput()
{
    UT_RESULT_INITIAL("GUNNS Fluid Conductor Links");

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName  == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,
                                 mConfigData->mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,
                                 mConfigData->mExpansionScaleFactor,
                                 0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,
                                 mInputData->mMalfBlockageValue,
                                 0.0);

    /// @test    Configuration data default construction.
    GunnsFluidSensorConfigData defaultConfig;
    CPPUNIT_ASSERT(0 == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0 == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mExpansionScaleFactor,
                                 0.0);

    /// @test    Input data default construction.
    GunnsFluidSensorInputData   defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mMalfBlockageValue,
                                 0.0);

    /// @test    Configuration data copy construction.
    GunnsFluidSensorConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxConductivity,
                                 copyConfig.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExpansionScaleFactor,
                                 copyConfig.mExpansionScaleFactor,
                                 0.0);

    /// @test    Input data copy construction.
    GunnsFluidSensorInputData   copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,
                                 copyInput.mMalfBlockageValue,
                                 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sensor link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSensor::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT("" == mArticle->mName);
    CPPUNIT_ASSERT(0  == mArticle->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mExpansionScaleFactor,
                                 0.0);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(!mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMalfBlockageValue,
                                 0.0);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mTemperature,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mPressure,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mDeltaPressure,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mMassFlowRate,
                                 0.0);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidSensor* article = new GunnsFluidSensor();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sensor link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSensor::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidSensor article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName      == article.mName);
    CPPUNIT_ASSERT(&mNodes[0] == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1] == article.mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,
                                 article.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,
                                 article.mExpansionScaleFactor,
                                 0.0);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mMalfBlockageFlag == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,
                                 article.mMalfBlockageValue,
                                 0.0);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidInput0->mTemperature,
                                 article.mTemperature,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidInput0->mPressure,
                                 article.mPressure,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidInput0->mPressure - mFluidInput1->mPressure,
                                 article.mDeltaPressure,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 article.mMassFlowRate,
                                 0.0);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sensor link model restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSensor::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    mArticle->mEffectiveConductivity = 1.0;
    mArticle->restart();
    CPPUNIT_ASSERT(0.0 == mArticle->mEffectiveConductivity);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sensor link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSensor::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Test all access methods.
    mArticle->mTemperature = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getTemperature(), 0.0);

    mArticle->mPressure = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getPressure(), 0.0);

    mArticle->mDeltaPressure = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getDeltaPressure(), 0.0);

    mArticle->mMassFlowRate = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getMassFlowRate(), 0.0);

    mArticle->mVolumetricFlowRate = 6.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, mArticle->getVolumetricFlowRate(), 0.0);

    mArticle->mPartialPressure[0] = 3.0;
    mArticle->mPartialPressure[1] = 7.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, mArticle->getPartialPressure()[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, mArticle->getPartialPressure()[1], 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sensor link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSensor::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nothing (this derived class does not add or override any modifiers).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sensor link model computeFlows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSensor::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Update state sensor truth values.
    mArticle->mPotentialVector[0] = mFluidInput0->mPressure;
    mArticle->mPotentialVector[1] = mFluidInput1->mPressure;
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidInput0->mTemperature,
                                 mArticle->mTemperature,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidInput0->mPressure,
                                 mArticle->mPressure,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidInput0->mPressure - mFluidInput1->mPressure,
                                 mArticle->mDeltaPressure,
                                 0.0);
    CPPUNIT_ASSERT(0.0 < mArticle->mFlowRate);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFlowRate,
                                 mArticle->mMassFlowRate,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mVolFlowRate,
                                 mArticle->mVolumetricFlowRate,
                                 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Sensor link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSensor::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidSensor article;

    /// @test    Initialization exception invalid config data:no name.
    mConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mName = mName;

    /// @test    Initialization exception on invalid config data: max conductivity < 0.
    mConfigData->mMaxConductivity = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mMaxConductivity = mMaxConductivity;

    /// @test    Initialization exception on invalid config data: expansion scale factor < 0.
    mConfigData->mExpansionScaleFactor = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mExpansionScaleFactor = mExpansionScaleFactor;

    /// @test    Initialization exception on invalid input data: mMalfBlockageValue < 0.
    mInputData->mMalfBlockageValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: mMalfBlockageValue > 1.
    mInputData->mMalfBlockageValue = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    UT_PASS_LAST;
}
