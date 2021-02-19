/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/source/GunnsFluidSimpleH2Redox.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidSimpleH2Redox.hh"

/// @details  Test identification number.
int UtGunnsFluidSimpleH2Redox::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Simple H2 Redox link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSimpleH2Redox::UtGunnsFluidSimpleH2Redox()
    :
    CppUnit::TestFixture(),
    mFluidProperties(0),
    mTypes(),
//    mFractions(),
    mFluidConfig(0),
    mFluidInput0(0),
    mFluidInput1(0),
    mFluidInput2(0),
    mLinks(),
    mName(""),
    mNodes(),
    mNodeList(),
    mPort0(0),
    mPort1(0),
    mPort2(0),
    mH2Coefficient(0.0),
    mMinInletPressure(0.0),
    mNominalStackVoltage(0.0),
    mConfigData(0),
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mCurrent(0.0),
    mTrippedOff(false),
    mInputData(0),
//    mFluidTemperature(0.0),
//    mWallHeatFlux(0.0),
//    mMass(0.0),
//    mSorbtionFlowRate(0.0),
    mArticle(0),
//    mFlowRate(0.0),
    mTimeStep(0.0)
//    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Simple H2 Redox link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSimpleH2Redox::~UtGunnsFluidSimpleH2Redox()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidSimpleH2Redox::setUp()
{
//    /// - Define the nominal port fluids.
//    mFluidProperties      = new DefinedFluidProperties();
//    mTypes[0]             = FluidProperties::GUNNS_H2;
//    mTypes[1]             = FluidProperties::GUNNS_O2;
//    mTypes[2]             = FluidProperties::GUNNS_CO2;
//    mTypes[3]             = FluidProperties::GUNNS_WATER;
//    mFluidConfig          = new PolyFluidConfigData(mFluidProperties, mTypes, N_FLUIDS);
//
//    double fractions0[]   = {0.9, 0.0, 0.1, 0.0};
//    mFluidInput0          = new PolyFluidInputData( 294.261,
//                                                   2000.0,
//                                                      0.0,
//                                                      0.0,
//                                                   fractions0);
//
//    double fractions1[]   = {0.0, 0.9, 0.1, 0.0};
//    mFluidInput1          = new PolyFluidInputData( 294.261,
//                                                   2000.0,
//                                                      0.0,
//                                                      0.0,
//                                                   fractions1);
//
//    double fractions2[]   = {0.0, 0.0, 0.0, 1.0};
//    mFluidInput2          = new PolyFluidInputData( 294.261,
//                                                   2000.0,
//                                                      0.0,
//                                                      0.0,
//                                                   fractions2);
//
//    /// - Initialize the nodes.
//    mNodes[0].initialize("UtNode0", mFluidConfig);
//    mNodes[1].initialize("UtNode1", mFluidConfig);
//    mNodes[2].initialize("UtNode2", mFluidConfig);
//    mNodes[0].getContent()->initialize(*mFluidConfig, *mFluidInput0);
//    mNodes[1].getContent()->initialize(*mFluidConfig, *mFluidInput1);
//    mNodes[2].getContent()->initialize(*mFluidConfig, *mFluidInput2);
//
//    /// - Initialize the nodes list.
//    mNodeList.mNodes      = mNodes;
//    mNodeList.mNumNodes   = N_NODES;
//
//    /// - Define the nominal configuration data.
//    mName                 = "nominal";
//    mH2Coefficient        = 0.0001;
//    mMinInletPressure     = 1500.0;
//    mNominalStackVoltage  = 300.0;
//    mConfigData           = new GunnsFluidSimpleH2RedoxConfigData(mName,
//                                                                  &mNodeList,
//                                                                  mH2Coefficient,
//                                                                  mMinInletPressure,
//                                                                  mNominalStackVoltage);
//
//    /// - Define the nominal input data.
//    mMalfBlockageFlag     = false;
//    mMalfBlockageValue    = 0.3;
//    mCurrent              = 0.5;
//    mTrippedOff           = false;
//    mInputData            = new GunnsFluidSimpleH2RedoxInputData(mMalfBlockageFlag,
//                                                                 mMalfBlockageValue,
//                                                                 mCurrent,
//                                                                 mTrippedOff);
//
//    /// - Define the nominal port mapping.
//    mPort0                = 0;
//    mPort1                = 1;
//    mPort2                = 2;
//
//    /// - Default construct the nominal test article.
//    mArticle              = new FriendlyGunnsFluidSimpleH2Redox;
//
//    /// - Define the nominal time step.
//    mTimeStep             = 0.1;
//
//    /// - Increment the test identification number.
//    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::tearDown()
{
//    /// - Deletes for news (in reverse order) in setUp.
//    delete mArticle;
//    delete mInputData;
//    delete mConfigData;
//    delete mFluidInput2;
//    delete mFluidInput1;
//    delete mFluidInput0;
//    delete mFluidConfig;
//    delete mFluidProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model construction of configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testConfigAndInput()
{
//    UT_RESULT_FIRST;
//
//    /// @test    Configuration data nominal construction.
//    CPPUNIT_ASSERT(mName                                          == mConfigData->mName);
//    CPPUNIT_ASSERT(mNodes                                         == mConfigData->mNodeList->mNodes);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(mH2Coefficient,                     mConfigData->mH2Coefficient,       0.0);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMinInletPressure,                  mConfigData->mMinInletPressure,    0.0);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNominalStackVoltage,               mConfigData->mNominalStackVoltage, 0.0);
//
//    /// @test    Input data nominal construction.
//    CPPUNIT_ASSERT(mMalfBlockageFlag                              == mInputData->mMalfBlockageFlag);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,                 mInputData->mMalfBlockageValue,    0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCurrent,                           mInputData->mCurrent,              0.0);
//    CPPUNIT_ASSERT(mTrippedOff                                    == mInputData->mTrippedOff);
//
//    /// @test    Configuration data default construction.
//    GunnsFluidSimpleH2RedoxConfigData defaultConfig;
//    CPPUNIT_ASSERT(0                                              == defaultConfig.mName.size());
//    CPPUNIT_ASSERT(0                                              == defaultConfig.mNodeList);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mH2Coefficient,       0.0);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mMinInletPressure,    0.0);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mNominalStackVoltage, 0.0);
//
//    /// @test    Input data default construction.
//    GunnsFluidSimpleH2RedoxInputData   defaultInput;
//    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfBlockageFlag);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfBlockageValue,    0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mCurrent,              0.0);
//    CPPUNIT_ASSERT(                                                 !defaultInput.mTrippedOff);
//
//    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testDefaultConstruction()
{
//    UT_RESULT;
//
//    /// @test    Default construction configuration data.
//    CPPUNIT_ASSERT(""                        == mArticle->mName);
//    CPPUNIT_ASSERT(0                         == mArticle->mNodes);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mH2Coefficient,       0.0);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mMinInletPressure,    0.0);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mNominalStackVoltage, 0.0);
//
//    /// @test    Default construction input data.
//    CPPUNIT_ASSERT(                            !mArticle->mMalfBlockageFlag);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mMalfBlockageValue,   0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mCurrent,             0.0);
//    CPPUNIT_ASSERT(                            !mArticle->mTrippedOff);
//
//    /// @test    Default construction state data.
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mOutputStackVoltage,   0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mH2MassRate,           0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mO2MassRate,           0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mH2OMassRate,          0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mH2MoleRate,           0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mO2MoleRate,           0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mH2OMoleRate,          0.0);
//    CPPUNIT_ASSERT(0                        == mArticle->mH2Fluid);
//    CPPUNIT_ASSERT(0                        == mArticle->mO2Fluid);
//    CPPUNIT_ASSERT(0                        == mArticle->mH2OFluid);
//
//    /// @test    Default construction initialization flag.
//    CPPUNIT_ASSERT(!mArticle->mInitFlag);
//
//    /// @test    New/delete for code coverage.
//    GunnsFluidSimpleH2Redox* article = new GunnsFluidSimpleH2Redox();
//    delete article;
//
//    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testNominalInitialization()
{
//    UT_RESULT;
//
//    /// - Initialize default constructed test article with nominal initialization data.
//    FriendlyGunnsFluidSimpleH2Redox article;
//    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2);
//
//    /// @test    Nominal configuration data.
//    CPPUNIT_ASSERT(mName                                      == article.mName);
//    CPPUNIT_ASSERT(&mNodes[mPort0]                            == article.mNodes[0]);
//    CPPUNIT_ASSERT(&mNodes[mPort1]                            == article.mNodes[1]);
//    CPPUNIT_ASSERT(&mNodes[mPort2]                            == article.mNodes[2]);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(mH2Coefficient,                 article.mH2Coefficient,       0.0);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMinInletPressure,              article.mMinInletPressure,    0.0);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNominalStackVoltage,           article.mNominalStackVoltage, 0.0);
//
//    /// @test    Nominal input data.
//    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag              == article.mMalfBlockageFlag);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue, article.mMalfBlockageValue,   0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCurrent,                       article.mCurrent,             0.0);
//    CPPUNIT_ASSERT(mTrippedOff                                == article.mTrippedOff);
//
//    /// @test    Nominal state data.
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mOutputStackVoltage,  0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mH2MassRate,          0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mO2MassRate,          0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mH2OMassRate,         0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mH2MoleRate,          0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mO2MoleRate,          0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mH2MoleRate,          0.0);
//    CPPUNIT_ASSERT(0                                          != article.mH2Fluid);
//    CPPUNIT_ASSERT(0                                          != article.mO2Fluid);
//    CPPUNIT_ASSERT(0                                          != article.mH2OFluid);
//
//    /// @test    Nominal initialization flag.
//    CPPUNIT_ASSERT(article.mInitFlag);
//
//    /// @test    Nominal initialization with fluid objects already allocated.
//    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2);
//
//    /// @test    Nominal configuration data.
//    CPPUNIT_ASSERT(mName                                      == article.mName);
//    CPPUNIT_ASSERT(&mNodes[mPort0]                            == article.mNodes[0]);
//    CPPUNIT_ASSERT(&mNodes[mPort1]                            == article.mNodes[1]);
//    CPPUNIT_ASSERT(&mNodes[mPort2]                            == article.mNodes[2]);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(mH2Coefficient,                 article.mH2Coefficient,       0.0);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMinInletPressure,              article.mMinInletPressure,    0.0);
////    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNominalStackVoltage,           article.mNominalStackVoltage, 0.0);
//
//    /// @test    Nominal input data.
//    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag              == article.mMalfBlockageFlag);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue, article.mMalfBlockageValue,   0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCurrent,                       article.mCurrent,             0.0);
//    CPPUNIT_ASSERT(mTrippedOff                                == article.mTrippedOff);
//
//    /// @test    Nominal state data.
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mOutputStackVoltage,  0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mH2MassRate,          0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mO2MassRate,          0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mH2OMassRate,         0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mH2MoleRate,          0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mO2MoleRate,          0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                            article.mH2MoleRate,          0.0);
//    CPPUNIT_ASSERT(0                                          != article.mH2Fluid);
//    CPPUNIT_ASSERT(0                                          != article.mO2Fluid);
//    CPPUNIT_ASSERT(0                                          != article.mH2OFluid);
//
//    /// @test    Nominal initialization flag.
//    CPPUNIT_ASSERT(article.mInitFlag);
//
//    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testAccessors()
{
//    UT_RESULT;
//
//    /// @test     isInitialized before initialization.
//    CPPUNIT_ASSERT(                       !mArticle->isInitialized());
//
//    /// @test     getOutputStackVoltage before initialization.
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,      mArticle->getOutputStackVoltage(), 0.0);
//
//    /// - Initialize default constructed test article with nominal initialization data.
//    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2);
//
//    /// @test     isInitialized after initialization.
//    CPPUNIT_ASSERT(                        mArticle->isInitialized());
//
//    /// @test     getOutputStackVoltage after initialization.
//    const double expected         = mNominalStackVoltage;
//    mArticle->mOutputStackVoltage = expected;
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getOutputStackVoltage(), 0.0);
//
//    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testModifiers()
{
//    UT_RESULT;
//
//    /// - Initialize default test article with nominal initialization data.
//    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2);
//
//    /// @test    setCurrent with good value.
//    const double current = 0.1;
//    mArticle->setCurrent(current);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(current, mArticle->mCurrent, 0.0);
//
//    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testRestart()
{
//    UT_RESULT;
//
//    /// - Initialize default test article with nominal initialization data.
//    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2);
//
//    /// @test    the state attributes are reset on restart.
//    mArticle->mOutputStackVoltage = mNominalStackVoltage;
//    mArticle->mH2MassRate         = 1.0;
//    mArticle->mO2MassRate         = 1.0;
//    mArticle->mH2OMassRate        = 1.0;
//    mArticle->mH2MoleRate         = 1.0;
//    mArticle->mO2MoleRate         = 1.0;
//    mArticle->mH2OMoleRate        = 1.0;
//    mArticle->mPower              = 1.0;
//    mArticle->restart();
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNominalStackVoltage, mArticle->mOutputStackVoltage, 0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                  mArticle->mH2MassRate,         0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                  mArticle->mO2MassRate,         0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                  mArticle->mH2OMassRate,        0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                  mArticle->mH2MoleRate,         0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                  mArticle->mO2MoleRate,         0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                  mArticle->mH2OMoleRate,        0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                  mArticle->mPower,              0.0);
//
//    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testStep()
{
//    UT_RESULT;
//
//    /// - Initialize default test article with nominal initialization data.
//    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2);
//    mArticle->mPotentialVector[0] = mFluidInput0->mPressure;
//    mArticle->mPotentialVector[1] = mFluidInput1->mPressure;
//    mArticle->mPotentialVector[2] = mFluidInput2->mPressure;
//
//    /// - Step the link.
//    mArticle->step(mTimeStep);
//
//    /// @test    Nominal outputs for forward (fuel-cell) reaction.
//    bool   expectedTrip    = false;
//    double expectedH2mass  = -mH2Coefficient * mCurrent;
//    double expectedH2mole  =  expectedH2mass  / 2.01588;    // MW of H2
//    double expectedO2mole  =  expectedH2mole  * 0.5;
//    double expectedH2Omole = -expectedH2mole  * 1.0;
//    double expectedO2mass  =  expectedO2mole  * 31.9988;    // MW of O2
//    double expectedH2Omass =  expectedH2Omole * 18.0153;    // MW of H2O
//    double expectedVolts   =  mNominalStackVoltage;
//
//    CPPUNIT_ASSERT_EQUAL        (expectedTrip,    mArticle->mTrippedOff);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2mass,  mArticle->mH2MassRate,         DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2mass,  mArticle->mO2MassRate,         DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2Omass, mArticle->mH2OMassRate,        DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2mole,  mArticle->mH2MoleRate,         DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2mole,  mArticle->mO2MoleRate,         DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2Omole, mArticle->mH2OMoleRate,        DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolts,   mArticle->mOutputStackVoltage, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2mole,  mArticle->mSourceVector[0],    DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2mole,  mArticle->mSourceVector[1],    DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2Omole, mArticle->mSourceVector[2],    DBL_EPSILON);
//
//    /// @test    Nominal outputs for reverse (electrolysis) reaction.
//
//    /// @test  port re-mapping
//
//    /// @test trip off for inlet pressure forward
//    /// @test trip off for inlet pressure reverse
//
//    /// @test h2 rate < min
//
//
//    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testUpdateFluidNominal()
{
//    UT_RESULT;
//
//    {
//        /// @test     Nominal initialization data.
//        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//        const double expectedMass     = mNodes[0].getContent()->getMassFraction(mGasIndex) * mEfficiency * mFlowRate * mTimeStep;
//        const double expectedAdsorbed = mArticle->mAdsorbedMass + expectedMass;
//        const double expectedRate     = -expectedMass / mTimeStep;
//        mArticle->mFlowRate           = mFlowRate;
//        mArticle->updateFluid(mTimeStep, 0.0);
//        CPPUNIT_ASSERT(Math::isInRange(mFluidInput0->mTemperature, mArticle->mFluidTemperature, mWallTemperature));
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed,            mArticle->mAdsorbedMass,                    mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,                mArticle->mMass,                            mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,                mArticle->mSorbtionFlowRate,                mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFluidTemperature, mArticle->mInternalFluid->getTemperature(), mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFluidTemperature, mArticle->mSorbtionFluid->getTemperature(), mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,                mArticle->mSorbtionFluid->getMass(),        mTolerance);
//    } {
//        /// @test     Efficiency constraint.
//        mConfigData->mEfficiency = 0.01;
//        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//        const double expectedMass     = mNodes[0].getContent()->getMassFraction(mGasIndex) * 0.01 * mFlowRate * mTimeStep;;
//        const double expectedAdsorbed = mArticle->mAdsorbedMass + expectedMass;
//        const double expectedRate     = -expectedMass / mTimeStep;
//        mArticle->mFlowRate           = mFlowRate;
//        mArticle->updateFluid(mTimeStep, 0.0);
//        CPPUNIT_ASSERT(Math::isInRange(mFluidInput0->mTemperature, mArticle->mFluidTemperature, mWallTemperature));
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed,            mArticle->mAdsorbedMass,                    mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,                mArticle->mMass,                            mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,                mArticle->mSorbtionFlowRate,                mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFluidTemperature, mArticle->mInternalFluid->getTemperature(), mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFluidTemperature, mArticle->mSorbtionFluid->getTemperature(), mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,                mArticle->mSorbtionFluid->getMass(),        mTolerance);
//        mConfigData->mEfficiency      = mEfficiency;
//    } {
//        /// @test     Max removal rate constraint.
//        mConfigData->mMaxAdsorbtionRate = 0.001;
//        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//        const double expectedRate     = -0.001;
//        const double expectedMass     = -expectedRate * mTimeStep;;
//        const double expectedAdsorbed = mArticle->mAdsorbedMass + expectedMass;
//        mArticle->mFlowRate           = mFlowRate;
//        mArticle->updateFluid(mTimeStep, 0.0);
//        CPPUNIT_ASSERT(Math::isInRange(mFluidInput0->mTemperature, mArticle->mFluidTemperature, mWallTemperature));
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed,            mArticle->mAdsorbedMass,                    mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,                mArticle->mMass,                            mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,                mArticle->mSorbtionFlowRate,                mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFluidTemperature, mArticle->mInternalFluid->getTemperature(), mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFluidTemperature, mArticle->mSorbtionFluid->getTemperature(), mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,                mArticle->mSorbtionFluid->getMass(),        mTolerance);
//        mConfigData->mMaxAdsorbtionRate = mMaxAdsorbtionRate;
//    } {
//        /// @test     Capacity constraint.
//        mInputData->mAdsorbedMass     = mConfigData->mMaxAdsorbedMass - 0.001;
//        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//        const double expectedMass     = 0.001;
//        const double expectedAdsorbed = mConfigData->mMaxAdsorbedMass;
//        const double expectedRate     = -0.001 / mTimeStep;
//        mArticle->mFlowRate           = mFlowRate;
//        mArticle->updateFluid(mTimeStep, 0.0);
//        CPPUNIT_ASSERT(Math::isInRange(mFluidInput0->mTemperature, mArticle->mFluidTemperature, mWallTemperature));
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed,            mArticle->mAdsorbedMass,                    mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,                mArticle->mMass,                            mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,                mArticle->mSorbtionFlowRate,                mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFluidTemperature, mArticle->mInternalFluid->getTemperature(), mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFluidTemperature, mArticle->mSorbtionFluid->getTemperature(), mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,                mArticle->mSorbtionFluid->getMass(),        mTolerance);
//        mInputData->mAdsorbedMass     = mAdsorbedMass;
//    } {
//        /// @test     Nominal initialization data with reverse flow.
//        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//        const double expectedMass     = mNodes[0].getContent()->getMassFraction(mGasIndex) * mEfficiency * mFlowRate * mTimeStep;
//        const double expectedAdsorbed = mArticle->mAdsorbedMass + expectedMass;
//        const double expectedRate     = -expectedMass / mTimeStep;
//        mArticle->mFlowRate           = -mFlowRate;
//        mArticle->updateFluid(mTimeStep, 0.0);
//        CPPUNIT_ASSERT(Math::isInRange(mFluidInput0->mTemperature, mArticle->mFluidTemperature, mWallTemperature));
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed,            mArticle->mAdsorbedMass,                    mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,                mArticle->mMass,                            mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,                mArticle->mSorbtionFlowRate,                mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFluidTemperature, mArticle->mInternalFluid->getTemperature(), mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFluidTemperature, mArticle->mSorbtionFluid->getTemperature(), mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,                mArticle->mSorbtionFluid->getMass(),        mTolerance);
//    } {
//        /// @test    Low flow-rate adsorbtion - temperatures should approach wall temperature.
//        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//        const double flowRate         = 1.0E-13;
//        mArticle->mFlowRate           = flowRate;
//        mArticle->updateFluid(mTimeStep, 0.0);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(mAdsorbedMass,               mArticle->mAdsorbedMass,                    mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mMass,                            mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                         mArticle->mSorbtionFlowRate,                mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,            mArticle->mInternalFluid->getTemperature(), mTolerance);
//    }
//
//    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model update fluid with no flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testUpdateFluidOffNominal()
{
//    UT_RESULT;
//
//    /// @test    No flow adsorbtion.
//    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//    mArticle->mFlowRate                          = 0.0;
//    mArticle->updateFluid(mTimeStep, 1.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mAdsorbedMass,    mArticle->mAdsorbedMass,     mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mMass,             mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mSorbtionFlowRate, mTolerance);
//
//    /// @test    Zero time step adsorbtion.
//    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//    mArticle->mFlowRate                          = mFlowRate;
//    mArticle->updateFluid(0.0, 2.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mAdsorbedMass,    mArticle->mAdsorbedMass,     mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mMass,             mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mSorbtionFlowRate, mTolerance);
//
//    /// @test    Too low flow adsorbtion.
//    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//    mArticle->mFlowRate                          = -DBL_EPSILON;
//    mArticle->updateFluid(mTimeStep, 3.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mAdsorbedMass,    mArticle->mAdsorbedMass,     mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mMass,             mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mSorbtionFlowRate, mTolerance);
//
//    const double expectedMass                    = -mDesorbtionRate * mTimeStep;
//    const double expectedAdsorbed                =  mAdsorbedMass - mDesorbtionRate * mTimeStep;
//    const double expectedRate                    = -expectedMass / mTimeStep;
//
//    /// @test    No flow desorbtion.
//    mInputData->mDesorbtionCycle                 = true;
//    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//    mArticle->mFlowRate                          =  0.0;
//    mArticle->updateFluid(mTimeStep, 1.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed, mArticle->mAdsorbedMass,     mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     mArticle->mMass,             mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,     mArticle->mSorbtionFlowRate, mTolerance);
//
//    /// @test    Zero time step desorbtion.
//    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//    mArticle->mFlowRate                          = mFlowRate;
//    mArticle->updateFluid(0.0, 2.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mAdsorbedMass,    mArticle->mAdsorbedMass,     mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mMass,             mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              mArticle->mSorbtionFlowRate, mTolerance);
//
//    /// @test    Too low flow desorbtion.
//    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//    mArticle->mFlowRate                          = -DBL_EPSILON;
//    mArticle->updateFluid(mTimeStep, 3.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed, mArticle->mAdsorbedMass,     mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     mArticle->mMass,             mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,     mArticle->mSorbtionFlowRate, mTolerance);
//    mInputData->mDesorbtionCycle                 = false;
//
//    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model update fluid in desorbtion cycle.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testUpdateFluidDesorbtion()
{
//    UT_RESULT;
//
//    {
//        /// @test    Nominal.
//        const double expectedMass     = -mDesorbtionRate * mTimeStep;
//        const double expectedAdsorbed =  mAdsorbedMass - mDesorbtionRate * mTimeStep;
//        const double expectedRate     =  -expectedMass / mTimeStep;
//        mInputData->mDesorbtionCycle  = true;
//        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//        mArticle->mFlowRate           =  mFlowRate;
//        mArticle->updateFluid(mTimeStep, 0.0);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed, mArticle->mAdsorbedMass,     mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     mArticle->mMass,             mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,     mArticle->mSorbtionFlowRate, mTolerance);
//    } {
//        /// @test    Availability constraint.
//        const double expectedMass     = -0.0001;
//        const double expectedAdsorbed =  0.0;
//        const double expectedRate     = -expectedMass / mTimeStep;
//        mInputData->mAdsorbedMass     = -expectedMass;
//        mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
//        mArticle->mFlowRate           =  mFlowRate;
//        mArticle->updateFluid(mTimeStep, 0.0);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdsorbed, mArticle->mAdsorbedMass,     mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     mArticle->mMass,             mTolerance);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate,     mArticle->mSorbtionFlowRate, mTolerance);
//    }
//
//    mInputData->mAdsorbedMass         = mAdsorbedMass;
//    mInputData->mDesorbtionCycle      = false;
//
//    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model specific port mapping rules.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testPortMapping()
{
//    UT_RESULT;
//
//    /// - Initialize default test article with nominal initialization data.
//    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2);
//
//    /// @test    Fail if port 2 is not liquid phase.
//    CPPUNIT_ASSERT(!mArticle->checkSpecificPortRules(2,0));
//
//    /// @test    Fail if port 1 is not gas phase.
//    CPPUNIT_ASSERT(!mArticle->checkSpecificPortRules(1,2));
//
//    /// @test    Fail if port 0 is not gas phase.
//    CPPUNIT_ASSERT(!mArticle->checkSpecificPortRules(0,2));
//
//    /// @test    Fail if port 0 is the vacuum boundary node.
//    CPPUNIT_ASSERT(!mArticle->checkSpecificPortRules(0,3));
//
//    /// @test    Fail if port 1 is the vacuum boundary node.
//    CPPUNIT_ASSERT(!mArticle->checkSpecificPortRules(1,3));
//
//    /// @test    Fail if port 2 is the vacuum boundary node.
//    CPPUNIT_ASSERT(!mArticle->checkSpecificPortRules(2,3));
//
//    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Simple H2 Redox link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSimpleH2Redox::testInitializationExceptions()
{
//    UT_RESULT;
//
//    /// - Default construct a test article.
//    GunnsFluidSimpleH2Redox article;
//
//    /// @test    Initialization exception on invalid config data: no name.
//    mConfigData->mName = "";
//    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2),
//                         TsInitializationException);
//    mConfigData->mName = mName;
//
//    /// @test    Initialization exception on invalid config data: mH2Coefficient < DBL_EPSILON.
//    mConfigData->mH2Coefficient = 0.0;
//    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2),
//                         TsInitializationException);
//    mConfigData->mH2Coefficient = mH2Coefficient;
//
//    /// @test    Initialization exception on invalid config data: mMinInletPressure < DBL_EPSILON.
//    mConfigData->mMinInletPressure = 0.0;
//    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2),
//                         TsInitializationException);
//    mConfigData->mMinInletPressure = mMinInletPressure;
//
//    /// @test    Initialization exception on invalid config data: mNominalStackVoltage < DBL_EPSILON.
//    mConfigData->mNominalStackVoltage = 0.0;
//    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1, mPort2),
//                         TsInitializationException);
//    mConfigData->mNominalStackVoltage = mNominalStackVoltage;
//
//    /// @test    Initialization exception on invalid config data: H2 not in the network.
//    //TODO
////    mTypes[0]             = FluidProperties::GUNNS_N2;
////    PolyFluidConfigData* fluidConfig = new PolyFluidConfigData(mFluidProperties, mTypes, N_FLUIDS);
////
////    double fractions0[]   = {0.9, 0.0, 0.1, 0.0};
////    PolyFluidInputData fluidInput0   = new PolyFluidInputData( 294.261,
////                                                   2000.0,
////                                                      0.0,
////                                                      0.0,
////                                                   fractions0);
////    delete fluidInput0;
////    delete fluidConfig;
////
////    mTypes[0]             = FluidProperties::GUNNS_H2;
//    /// @test    Initialization exception on invalid config data: O2 not in the network.
//    /// @test    Initialization exception on invalid config data: WATER not in the network.
//
//    UT_PASS_LAST;
}
