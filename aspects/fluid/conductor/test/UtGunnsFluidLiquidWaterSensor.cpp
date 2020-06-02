/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidLiquidWaterSensor.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidLiquidWaterSensor.hh"

/// @details  Test identification number.
int UtGunnsFluidLiquidWaterSensor::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Liquid Water Sensor link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidLiquidWaterSensor::UtGunnsFluidLiquidWaterSensor()
    :
    CppUnit::TestFixture(),
    mTypes(),
    mFractions(),
    mFluidProperties(0),
    mFluidConfig(0),
    mFluidConfig2(0),
    mFluidInput(0),
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
    mArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Liquid Water Sensor link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidLiquidWaterSensor::~UtGunnsFluidLiquidWaterSensor()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLiquidWaterSensor::setUp()
{
    /// - Define the nominal port fluids.
    mFluidProperties      = new DefinedFluidProperties();
    mTypes[0]             = FluidProperties::GUNNS_N2;
    mTypes[1]             = FluidProperties::GUNNS_H2O;
    mFractions[0]         = 0.5;
    mFractions[1]         = 0.5;
    mFluidConfig          = new PolyFluidConfigData(mFluidProperties, mTypes, N_FLUIDS);

    mTypes[1]             = FluidProperties::GUNNS_O2;
    mFluidConfig2         = new PolyFluidConfigData(mFluidProperties, mTypes, N_FLUIDS);

    mFluidInput           = new PolyFluidInputData(283.0,
                                                   109.0,
                                                     0.0,
                                                     0.0,
                                                     mFractions);

    /// - Initialize the nodes.
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, *mFluidInput);
    mNodes[1].getContent()->initialize(*mFluidConfig, *mFluidInput);

    mNodes[2].initialize("UtNode3", mFluidConfig2);
    mNodes[2].getContent()->initialize(*mFluidConfig2, *mFluidInput);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mNodes[2].resetFlows();

    /// - Initialize the nodes list.
    mNodeList.mNodes      = mNodes;
    mNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    mName                 = "nominal";
    mMaxConductivity      = 20.0;
    mExpansionScaleFactor = 0.5;
    mConfigData           = new GunnsFluidLiquidWaterSensorConfigData(mName,
                                                                      &mNodeList,
                                                                      mMaxConductivity,
                                                                      mExpansionScaleFactor);

    /// - Define the nominal input data.
    mMalfBlockageFlag     = false;
    mMalfBlockageValue    = 0.3;
    mInputData            = new GunnsFluidLiquidWaterSensorInputData(mMalfBlockageFlag,
                                                                     mMalfBlockageValue);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;

    /// - Default construct the nominal test article.
    mArticle              = new FriendlyGunnsFluidLiquidWaterSensor;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLiquidWaterSensor::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput;
    delete mFluidConfig2;
    delete mFluidConfig;
    delete mFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Fluid Liquid Water Sensor link model configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLiquidWaterSensor::testConfigAndInput()
{
    UT_RESULT_FIRST;

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
    GunnsFluidLiquidWaterSensorConfigData defaultConfig;
    CPPUNIT_ASSERT(0 == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0 == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultConfig.mExpansionScaleFactor,
                                 0.0);

    /// @test    Input data default construction.
    GunnsFluidLiquidWaterSensorInputData   defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 defaultInput.mMalfBlockageValue,
                                 0.0);

    /// @test    Configuration data copy construction.
    GunnsFluidLiquidWaterSensorConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxConductivity,
                                 copyConfig.mMaxConductivity,
                                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExpansionScaleFactor,
                                 copyConfig.mExpansionScaleFactor,
                                 0.0);

    /// @test    Input data copy construction.
    GunnsFluidLiquidWaterSensorInputData   copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,
                                 copyInput.mMalfBlockageValue,
                                 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Liquid Water Sensor link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLiquidWaterSensor::testDefaultConstruction()
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
    CPPUNIT_ASSERT(!mArticle->mLiquidPresent);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidLiquidWaterSensor* article = new GunnsFluidLiquidWaterSensor();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Liquid Water Sensor link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLiquidWaterSensor::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidLiquidWaterSensor article;
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
    CPPUNIT_ASSERT(!mArticle->mLiquidPresent);
    CPPUNIT_ASSERT(0.0 == mArticle->mRelativeHumidity);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Verify restartModel functionality
    mArticle->mEffectiveConductivity = 1.0;
    mArticle->mSystemConductance     = 1.0;

    mArticle->restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mArticle->mEffectiveConductivity,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mArticle->mSystemConductance,DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Liquid Water Sensor link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLiquidWaterSensor::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Test the getter methods.
    mArticle->mLiquidPresent = true;
    CPPUNIT_ASSERT(mArticle->getLiquidPresent());
    mArticle->mRelativeHumidity = 0.8;
    CPPUNIT_ASSERT(0.8 == mArticle->getRelativeHumidity());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Liquid Water Sensor link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLiquidWaterSensor::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nothing (this derived class does not add or override any modifiers).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Liquid Water Sensor link model update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLiquidWaterSensor::testUpdateState()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Update state sensor truth values when liquid should be present.
    mArticle->updateState(0.0);
    CPPUNIT_ASSERT(mArticle->mLiquidPresent);
    CPPUNIT_ASSERT(mArticle->mRelativeHumidity      > 1.0);
    CPPUNIT_ASSERT(mArticle->mEffectiveConductivity > 0.0);

    /// @test    Verify the output when liquid should not be present.
    double partialPressures[2] = {100.0, 0.0};
    mNodes[0].getContent()->edit(300.0, partialPressures);

    mArticle->updateState(0.0);
    CPPUNIT_ASSERT(!mArticle->mLiquidPresent);
    CPPUNIT_ASSERT(mArticle->mRelativeHumidity      < 1.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Liquid Water Sensor link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLiquidWaterSensor::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidLiquidWaterSensor article;

    /// @test    Initialization exception invalid config data:no name.
    mConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mName = mName;

    /// @test    Initialization exception on H2O not present in the network.
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, mPort1),
                         TsInitializationException);

    UT_PASS_LAST;
}
