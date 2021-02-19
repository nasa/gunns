/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/source/GunnsFluidMetabolic.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidMetabolic.hh"

/// @details  Test identification number.
int UtGunnsFluidMetabolic::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor for the UtGunnsFluidMetabolic test class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidMetabolic::UtGunnsFluidMetabolic()
    :
    mFluidProperties(),
    mTypes(),
    mFractions(),
    mFluidConfig(),
    mFluidInput(),
    mLinks(),
    mName(),
    mNodes(),
    mNodeList(),
    mInput(),
    mOutput(),
    mInitialFlowDemand(),
    mConsumptionRateO2(),
    mProductionRateCO2(),
    mProductionRateH2O(),
    mProductionRateHeat(),
    mSleepScaleFactor(),
    mRecoveryScaleFactor(),
    mExerciseScaleFactor(),
    mConfigData(),
    mNNominal(),
    mNSleep(),
    mNRecovery(),
    mNExercise(),
    mInputData(),
    mArticle(),
    mFlowRate(),
    mTimeStep(),
    mTolerance()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor for the UtGunnsFluidMetabolic test class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidMetabolic::~UtGunnsFluidMetabolic()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic::setUp()
{
    /// - Set up the nodes list
    mNodeList.mNumNodes = 3;
    mNodeList.mNodes    = mNodes;

    /// - Define the nominal atmosphere
    mFluidProperties = new DefinedFluidProperties();
    mTypes[0]        = FluidProperties::GUNNS_N2;  mFractions[0] = 0.750;
    mTypes[1]        = FluidProperties::GUNNS_NH3; mFractions[1] = 0.010;
    mTypes[2]        = FluidProperties::GUNNS_CH4; mFractions[2] = 0.002;
    mTypes[3]        = FluidProperties::GUNNS_H2;  mFractions[3] = 0.003;
    mTypes[4]        = FluidProperties::GUNNS_O2;  mFractions[4] = 0.200;
    mTypes[5]        = FluidProperties::GUNNS_H2O; mFractions[5] = 0.010;
    mTypes[6]        = FluidProperties::GUNNS_CO;  mFractions[6] = 0.005;
    mTypes[7]        = FluidProperties::GUNNS_CO2; mFractions[7] = 0.020;
    mFluidConfig = new PolyFluidConfigData(mFluidProperties,
                                           mTypes,
                                           UtGunnsFluidMetabolic::N_CONSTITUENTS);
    mFluidInput  = new PolyFluidInputData(300.0, 140000.0, 1.0, 1.0, mFractions);

    /// - Initialize the node fluids
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[2].initialize("UtNode3", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, *mFluidInput);
    mNodes[1].getContent()->initialize(*mFluidConfig, *mFluidInput);
    mNodes[2].getContent()->initialize(*mFluidConfig, *mFluidInput);

    /// - Define the nominal metabolic config data
    mName                = "nominal";
    mInitialFlowDemand   = 0.0;
    mConsumptionRateO2   = 0.000010;
    mProductionRateCO2   = 0.000013;
    mProductionRateH2O   = 0.000014;
    mProductionRateHeat  = 100.0;
    mSleepScaleFactor    = 0.60;
    mRecoveryScaleFactor = 1.2;
    mExerciseScaleFactor = 6.0;
    mConfigData          = new GunnsFluidMetabolicConfigData(mName,
                                                        &mNodeList,
                                                        mConsumptionRateO2,
                                                        mProductionRateCO2,
                                                        mProductionRateH2O,
                                                        mProductionRateHeat,
                                                        mSleepScaleFactor,
                                                        mRecoveryScaleFactor,
                                                        mExerciseScaleFactor);

    /// - Define the nominal metabolic input data
    mNNominal            = 3.0;
    mNSleep              = 2.0;
    mNRecovery           = 0.75;
    mNExercise           = 0.25;
    mInputData           = new GunnsFluidMetabolicInputData(false, 0.0, mInitialFlowDemand,
                                                       mNNominal,
                                                       mNSleep,
                                                       mNRecovery,
                                                       mNExercise);

    /// - Create the nominal test article
    mArticle = new FriendlyGunnsFluidMetabolic();

    /// - Define the nominal flow rate, time step and comparison tolerance
    mFlowRate  = 1.0;
    mTimeStep  = 0.1;
    mTolerance = DBL_EPSILON;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic::tearDown()
{
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput;
    delete mFluidConfig;
    delete mFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model construction of config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test config nominal construction
    CPPUNIT_ASSERT(mName  == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConsumptionRateO2,
                                mConfigData->mConsumptionRateO2,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mProductionRateCO2,
                                mConfigData->mProductionRateCO2,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mProductionRateH2O,
                                mConfigData->mProductionRateH2O,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mProductionRateHeat,
                                mConfigData->mProductionRateHeat,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSleepScaleFactor,
                                mConfigData->mSleepScaleFactor,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRecoveryScaleFactor,
                                mConfigData->mRecoveryScaleFactor,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExerciseScaleFactor,
                                mConfigData->mExerciseScaleFactor,
                                mTolerance);

    /// @test of input nominal construction
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialFlowDemand,
                                mInputData->mFlowDemand,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNNominal,
                                mInputData->mNNominal,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNSleep,
                                mInputData->mNSleep,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNRecovery,
                                mInputData->mNRecovery,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNExercise,
                                mInputData->mNExercise,
                                mTolerance);

    /// @test default config construction
    GunnsFluidMetabolicConfigData defaultConfig;
    CPPUNIT_ASSERT(0 == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0 == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                defaultConfig.mConsumptionRateO2,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                defaultConfig.mProductionRateCO2,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                defaultConfig.mProductionRateH2O,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                defaultConfig.mProductionRateHeat,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                defaultConfig.mSleepScaleFactor,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                defaultConfig.mRecoveryScaleFactor,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                defaultConfig.mExerciseScaleFactor,
                                mTolerance);

    /// @test of input default construction
    GunnsFluidMetabolicInputData defaultInput;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                defaultInput.mFlowDemand,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                defaultInput.mNNominal,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                defaultInput.mNSleep,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                defaultInput.mNRecovery,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                defaultInput.mNExercise,
                                mTolerance);

    /// @test config copy construction
    GunnsFluidMetabolicConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mConsumptionRateO2,
                                copyConfig.mConsumptionRateO2,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mProductionRateCO2,
                                copyConfig.mProductionRateCO2,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mProductionRateH2O,
                                copyConfig.mProductionRateH2O,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mProductionRateHeat,
                                copyConfig.mProductionRateHeat,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mSleepScaleFactor,
                                copyConfig.mSleepScaleFactor,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mRecoveryScaleFactor,
                                copyConfig.mRecoveryScaleFactor,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExerciseScaleFactor,
                                copyConfig.mExerciseScaleFactor,
                                mTolerance);

    /// @test input copy construction
    GunnsFluidMetabolicInputData copyInput(*mInputData);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mFlowDemand,
                                copyInput.mFlowDemand,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mNNominal,
                                copyInput.mNNominal,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mNSleep,
                                copyInput.mNSleep ,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mNRecovery,
                                copyInput.mNRecovery,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mNExercise,
                                copyInput.mNExercise,
                                mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic::testDefaultConstruction()
{
    UT_RESULT;

    FriendlyGunnsFluidMetabolic article;
    CPPUNIT_ASSERT("" == article.mName);
    CPPUNIT_ASSERT(0  == article.mInternalFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                article.mFlowDemand,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                article.mFlux,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                article.mFlowRate,
                                mTolerance);
    for (int i = 0; i < GunnsFluidMetabolic::NO_METABOLIC; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                    article.mNCrew[i],
                                    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                    article.mO2ConsumptionRate[i],
                                    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                    article.mCO2ProductionRate[i],
                                    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                    article.mH2OProductionRate[i],
                                    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                   article.mHeatProductionRate[i],
                                   mTolerance);
    }
    CPPUNIT_ASSERT(-1 == article.mO2);
    CPPUNIT_ASSERT(-1 == article.mCO2);
    CPPUNIT_ASSERT(-1 == article.mH2O);
    CPPUNIT_ASSERT(!article.mInitFlag);

    /// @test Destructor for code coverage
    GunnsFluidMetabolic* article2 = new GunnsFluidMetabolic;
    delete article2;

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic::testNominalInitialization()
{
    UT_RESULT;

    mArticle->initialize(*mConfigData, *mInputData, mLinks, 2, 1);

    CPPUNIT_ASSERT(mName == mArticle->mName);
    CPPUNIT_ASSERT(0 != mArticle->mInternalFluid);
    CPPUNIT_ASSERT(&mNodes[2] == mArticle->mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1] == mArticle->mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialFlowDemand,
                                mArticle->mFlowDemand,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                mArticle->mFlux,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                mArticle->mFlowRate,
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNNominal,
                                mArticle->mNCrew[GunnsFluidMetabolic::NOMINAL],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNSleep,
                                mArticle->mNCrew[GunnsFluidMetabolic::SLEEP],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNRecovery,
                                mArticle->mNCrew[GunnsFluidMetabolic::RECOVERY],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNExercise,
                                mArticle->mNCrew[GunnsFluidMetabolic::EXERCISE],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConsumptionRateO2,
                                mArticle->mO2ConsumptionRate[GunnsFluidMetabolic::NOMINAL],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSleepScaleFactor    * mConsumptionRateO2,
                                mArticle->mO2ConsumptionRate[GunnsFluidMetabolic::SLEEP],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRecoveryScaleFactor * mConsumptionRateO2,
                                mArticle->mO2ConsumptionRate[GunnsFluidMetabolic::RECOVERY],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExerciseScaleFactor * mConsumptionRateO2,
                                mArticle->mO2ConsumptionRate[GunnsFluidMetabolic::EXERCISE],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mProductionRateCO2,
                                mArticle->mCO2ProductionRate[GunnsFluidMetabolic::NOMINAL],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSleepScaleFactor    * mProductionRateCO2,
                                mArticle->mCO2ProductionRate[GunnsFluidMetabolic::SLEEP],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRecoveryScaleFactor * mProductionRateCO2,
                                mArticle->mCO2ProductionRate[GunnsFluidMetabolic::RECOVERY],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExerciseScaleFactor * mProductionRateCO2,
                                mArticle->mCO2ProductionRate[GunnsFluidMetabolic::EXERCISE],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mProductionRateH2O,
                                mArticle->mH2OProductionRate[GunnsFluidMetabolic::NOMINAL],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSleepScaleFactor    * mProductionRateH2O,
                                mArticle->mH2OProductionRate[GunnsFluidMetabolic::SLEEP],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRecoveryScaleFactor * mProductionRateH2O,
                                mArticle->mH2OProductionRate[GunnsFluidMetabolic::RECOVERY],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExerciseScaleFactor * mProductionRateH2O,
                                mArticle->mH2OProductionRate[GunnsFluidMetabolic::EXERCISE],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mProductionRateHeat,
                                mArticle->mHeatProductionRate[GunnsFluidMetabolic::NOMINAL],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSleepScaleFactor    * mProductionRateHeat,
                                mArticle->mHeatProductionRate[GunnsFluidMetabolic::SLEEP],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRecoveryScaleFactor * mProductionRateHeat,
                                mArticle->mHeatProductionRate[GunnsFluidMetabolic::RECOVERY],
                                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExerciseScaleFactor * mProductionRateHeat,
                                 mArticle->mHeatProductionRate[GunnsFluidMetabolic::EXERCISE],
                                 mTolerance);
    CPPUNIT_ASSERT(&mNodes[1]                                               == mArticle->mNodes[1]);
    CPPUNIT_ASSERT(mNodes[1].getContent()->find(FluidProperties::GUNNS_O2)  == mArticle->mO2);
    CPPUNIT_ASSERT(mNodes[1].getContent()->find(FluidProperties::GUNNS_CO2) == mArticle->mCO2);
    CPPUNIT_ASSERT(mNodes[1].getContent()->find(FluidProperties::GUNNS_H2O) == mArticle->mH2O);
    CPPUNIT_ASSERT(mArticle->mInitFlag);

    /// @test Verify RestartModel Functionality
    mArticle->mConsumedO2   = 1.0;
    mArticle->mProducedCO2  = 1.0;
    mArticle->mProducedH2O  = 1.0;
    mArticle->mProducedHeat = 1.0;

    mArticle->restartModel();

    CPPUNIT_ASSERT(0.0 == mArticle->mConsumedO2);
    CPPUNIT_ASSERT(0.0 == mArticle->mProducedCO2);
    CPPUNIT_ASSERT(0.0 == mArticle->mProducedH2O);
    CPPUNIT_ASSERT(0.0 == mArticle->mProducedHeat);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model multiple initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic::testMultipleInitialization()
{
    UT_RESULT;

    /// - Initialize a default test article with off-nominal config and input data
    FriendlyGunnsFluidMetabolic article;
    GunnsFluidMetabolicConfigData configData(std::string("off-nominal"),
                                         &mNodeList,
                                         1.1 * mConsumptionRateO2,
                                         1.1 * mProductionRateCO2,
                                         1.1 * mProductionRateH2O,
                                         1.1 * mProductionRateHeat,
                                         1.1 * mSleepScaleFactor,
                                         1.1 * mRecoveryScaleFactor,
                                         1.1 * mExerciseScaleFactor);
    GunnsFluidMetabolicInputData inputData(false, 0.0,
                                      1.1 * mInitialFlowDemand,
                                      1.1 * mNNominal,
                                      1.1 * mNSleep,
                                      1.1 * mNRecovery,
                                      1.1 * mNExercise);
    CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, 1, 2),
                         TsInitializationException);

    /// - Initialize test article again with nominal input data
    article.initialize(*mConfigData, *mInputData, mLinks, 2, 1);
    CPPUNIT_ASSERT(mName == article.mName);
    CPPUNIT_ASSERT(0 != article.mInternalFluid);
    CPPUNIT_ASSERT(&mNodes[2] == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1] == article.mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialFlowDemand,
                                 article.mFlowDemand,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 article.mFlux,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 article.mFlowRate,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNNominal,
                                 article.mNCrew[GunnsFluidMetabolic::NOMINAL],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNSleep,
                                 article.mNCrew[GunnsFluidMetabolic::SLEEP],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNRecovery,
                                 article.mNCrew[GunnsFluidMetabolic::RECOVERY],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNExercise,
                                 article.mNCrew[GunnsFluidMetabolic::EXERCISE],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConsumptionRateO2,
                                 article.mO2ConsumptionRate[GunnsFluidMetabolic::NOMINAL],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSleepScaleFactor    * mConsumptionRateO2,
                                 article.mO2ConsumptionRate[GunnsFluidMetabolic::SLEEP],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRecoveryScaleFactor * mConsumptionRateO2,
                                 article.mO2ConsumptionRate[GunnsFluidMetabolic::RECOVERY],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExerciseScaleFactor * mConsumptionRateO2,
                                 article.mO2ConsumptionRate[GunnsFluidMetabolic::EXERCISE],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mProductionRateCO2,
                                 article.mCO2ProductionRate[GunnsFluidMetabolic::NOMINAL],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSleepScaleFactor    * mProductionRateCO2,
                                 article.mCO2ProductionRate[GunnsFluidMetabolic::SLEEP],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRecoveryScaleFactor * mProductionRateCO2,
                                 article.mCO2ProductionRate[GunnsFluidMetabolic::RECOVERY],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExerciseScaleFactor * mProductionRateCO2,
                                 article.mCO2ProductionRate[GunnsFluidMetabolic::EXERCISE],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mProductionRateH2O,
                                 article.mH2OProductionRate[GunnsFluidMetabolic::NOMINAL],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSleepScaleFactor    * mProductionRateH2O,
                                 article.mH2OProductionRate[GunnsFluidMetabolic::SLEEP],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRecoveryScaleFactor * mProductionRateH2O,
                                 article.mH2OProductionRate[GunnsFluidMetabolic::RECOVERY],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExerciseScaleFactor * mProductionRateH2O,
                                 article.mH2OProductionRate[GunnsFluidMetabolic::EXERCISE],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mProductionRateHeat,
                                 article.mHeatProductionRate[GunnsFluidMetabolic::NOMINAL],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSleepScaleFactor    * mProductionRateHeat,
                                 article.mHeatProductionRate[GunnsFluidMetabolic::SLEEP],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRecoveryScaleFactor * mProductionRateHeat,
                                 article.mHeatProductionRate[GunnsFluidMetabolic::RECOVERY],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExerciseScaleFactor * mProductionRateHeat,
                                 article.mHeatProductionRate[GunnsFluidMetabolic::EXERCISE],
                                 mTolerance);
    CPPUNIT_ASSERT(&mNodes[1]                                               == article.mNodes[1]);
    CPPUNIT_ASSERT(mNodes[1].getContent()->find(FluidProperties::GUNNS_O2)  == article.mO2);
    CPPUNIT_ASSERT(mNodes[1].getContent()->find(FluidProperties::GUNNS_CO2) == article.mCO2);
    CPPUNIT_ASSERT(mNodes[1].getContent()->find(FluidProperties::GUNNS_H2O) == article.mH2O);
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic::testAccess()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, 2, 1);

    /// @test    Test the access methods.
    mArticle->mConsumedO2 = 5.0;
    mArticle->mProducedCO2 = 7.0;
    mArticle->mProducedH2O = 9.0;
    mArticle->mProducedHeat = 11.0;
    mArticle->mNCrew[GunnsFluidMetabolic::EXERCISE] = 2.5;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getConsumedO2(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, mArticle->getProducedCO2(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, mArticle->getProducedH2O(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(11.0, mArticle->getProducedHeat(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, *(mArticle->getNCrew()+GunnsFluidMetabolic::EXERCISE), 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic::testModify()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, 2, 1);

    /// @test transition single crew member nominal -> sleep.
    mArticle->transition(1.0, GunnsFluidMetabolic::NOMINAL, GunnsFluidMetabolic::SLEEP);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNNominal - 1.0,
                                 mArticle->mNCrew[GunnsFluidMetabolic::NOMINAL],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNSleep   + 1.0,
                                 mArticle->mNCrew[GunnsFluidMetabolic::SLEEP],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNRecovery,
                                 mArticle->mNCrew[GunnsFluidMetabolic::RECOVERY],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNExercise,
                                 mArticle->mNCrew[GunnsFluidMetabolic::EXERCISE],
                                 mTolerance);

    /// @test transition single crew member sleep -> nominal.
    mArticle->transition(1.0, GunnsFluidMetabolic::SLEEP, GunnsFluidMetabolic::NOMINAL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNNominal,
                                 mArticle->mNCrew[GunnsFluidMetabolic::NOMINAL],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNSleep,
                                 mArticle->mNCrew[GunnsFluidMetabolic::SLEEP],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNRecovery,
                                 mArticle->mNCrew[GunnsFluidMetabolic::RECOVERY],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNExercise,
                                 mArticle->mNCrew[GunnsFluidMetabolic::EXERCISE],
                                 mTolerance);

    /// @test transition zero crew member sleep -> nominal.
    mArticle->transition(0.0, GunnsFluidMetabolic::NOMINAL, GunnsFluidMetabolic::SLEEP);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNNominal,
                                 mArticle->mNCrew[GunnsFluidMetabolic::NOMINAL],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNSleep,
                                 mArticle->mNCrew[GunnsFluidMetabolic::SLEEP],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNRecovery,
                                 mArticle->mNCrew[GunnsFluidMetabolic::RECOVERY],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNExercise,
                                 mArticle->mNCrew[GunnsFluidMetabolic::EXERCISE],
                                 mTolerance);

    /// @test transition too many crew members exercise -> recovery.
    mArticle->transition(7.0, GunnsFluidMetabolic::EXERCISE, GunnsFluidMetabolic::RECOVERY);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNNominal,
                                 mArticle->mNCrew[GunnsFluidMetabolic::NOMINAL],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNSleep,
                                 mArticle->mNCrew[GunnsFluidMetabolic::SLEEP],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNRecovery + mNExercise,
                                 mArticle->mNCrew[GunnsFluidMetabolic::RECOVERY],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mNCrew[GunnsFluidMetabolic::EXERCISE],
                                 mTolerance);

    /// @test transition fractional crew member recovery -> exercise.
    mArticle->transition(0.75, GunnsFluidMetabolic::RECOVERY, GunnsFluidMetabolic::EXERCISE);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNNominal,
                                 mArticle->mNCrew[GunnsFluidMetabolic::NOMINAL],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNSleep,
                                 mArticle->mNCrew[GunnsFluidMetabolic::SLEEP],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNRecovery + mNExercise - 0.75,
                                 mArticle->mNCrew[GunnsFluidMetabolic::RECOVERY],
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.75,
                                 mArticle->mNCrew[GunnsFluidMetabolic::EXERCISE],
                                 mTolerance);

    /// @test for no change on negative number of crew members transition.
    const double expected = mArticle->mNCrew[GunnsFluidMetabolic::NOMINAL];
    mArticle->transition(-1.0, GunnsFluidMetabolic::NOMINAL, GunnsFluidMetabolic::SLEEP);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,
                                 mArticle->mNCrew[GunnsFluidMetabolic::NOMINAL],
                                 mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic::testUpdateFluid()
{
    UT_RESULT;

    mArticle->initialize(*mConfigData, *mInputData, mLinks, 2, 1);
    mArticle->updateState(mTimeStep);
    mArticle->updateFluid(mTimeStep, mFlowRate);

//////////////////////////////////////// Expected Outputs //////////////////////////////////////////
//                                      O2                 CO2                H2O           Heat
// State   # Crew    SF Mult      Base       Rate     Base      Rate     Base      Rate  Base  Rate
// -------------------------------------------------------------------------------------------------
// Nominal  3.00   1.00 3.00 -0.000010 -0.0000300 0.000013 0.0000390 0.000014 0.0000420 100.0 300.0
// Sleep    2.00   0.60 1.20 -0.000010 -0.0000120 0.000013 0.0000156 0.000014 0.0000168 100.0 120.0
// Recovery 0.75   1.20 0.90 -0.000010 -0.0000090 0.000013 0.0000117 0.000014 0.0000126 100.0  90.0
// Exercise 0.25   6.00 1.50 -0.000010 -0.0000150 0.000013 0.0000195 0.000014 0.0000210 100.0 150.0
// -------------------------------------------------------------------------------------------------
// Total    6                          -0.0000660          0.0000858          0.0000924       660.0
// -------------------------------------------------------------------------------------------------
// Fraction                            -0.58823529         0.76470588         0.82352941
// -------------------------------------------------------------------------------------------------
// Flowrate 0.00011220
////////////////////////////////////////////////////////////////////////////////////////////////////


    // todo: need to manipulate influx in node
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.58823529,
//                                 mArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_O2),
//                                 0.5e-08);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(+0.76470588,
//                                 mArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_CO2),
//                                 0.5e-08);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(+0.82352941,
//                                 mArticle->mInternalFluid->getMassFraction(FluidProperties::GUNNS_H2O),
//                                 0.5e-08);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(+0.00011220,
//                                 mArticle->mFlowDemand,
//                                 0.5e-08);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[1].getContent()->getTemperature(),
//                                 mArticle->mInternalFluid->getTemperature(),
//                                 0.5e-08);

    /// @test    Too low flow demand.
    for (int i = 0; i < GunnsFluidMetabolic::NO_METABOLIC; i++) {
        mArticle->mNCrew[i] = DBL_EPSILON;
    }
    mArticle->updateState(mTimeStep);
    mArticle->updateFluid(mTimeStep, mFlowRate);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mFlowDemand, 0.0);

    /// @test    Zero time step.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, 2, 1);
    mArticle->updateState(0.0);
    mArticle->updateFluid(0.0, mFlowRate);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mFlowDemand, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model specific port mapping rules.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic::testPortMapping()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, 2, 1);

    {
        /// @test    Fail if port 1 is the vacuum boundary node.
        const bool returned = mArticle->checkSpecificPortRules(1,2);
        CPPUNIT_ASSERT(!returned);
    } {
        /// @test    Fail if port 0 is not the vacuum boundary node.
        const bool returned = mArticle->checkSpecificPortRules(0,1);
        CPPUNIT_ASSERT(!returned);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Metabolic link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMetabolic::testInitializationExceptions()
{
    UT_RESULT;
    {
        /// @test for exception on negative O2 consumption rate.
        GunnsFluidMetabolic article;
        mConfigData->mConsumptionRateO2 = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, 1),
                             TsInitializationException);
        mConfigData->mConsumptionRateO2 = mConsumptionRateO2;
    } {
        /// @test for exception on negative CO2 production rate.
        GunnsFluidMetabolic article;
        mConfigData->mProductionRateCO2 = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, 1),
                             TsInitializationException);
        mConfigData->mProductionRateCO2 = mProductionRateCO2;
    } {
        /// @test for exception on negative H2O production rate.
        GunnsFluidMetabolic article;
        mConfigData->mProductionRateH2O = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, 1),
                             TsInitializationException);
        mConfigData->mProductionRateH2O = mProductionRateH2O;
    } {
        /// @test for exception on negative heat production rate.
        GunnsFluidMetabolic article;
        mConfigData->mProductionRateHeat = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, 1),
                             TsInitializationException);
        mConfigData->mProductionRateHeat = mProductionRateHeat;
    } {
        /// @test for exception on sleep scale factor negative.
        GunnsFluidMetabolic article;
        mConfigData->mSleepScaleFactor = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, 1),
                             TsInitializationException);
        mConfigData->mSleepScaleFactor = mSleepScaleFactor;
    } {
        /// @test for exception on sleep scale factor greater then nominal.
        GunnsFluidMetabolic article;
        mConfigData->mSleepScaleFactor = 1.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, 1),
                             TsInitializationException);
        mConfigData->mSleepScaleFactor = mSleepScaleFactor;
    } {
        /// @test for exception on recovery scale factor less than nominal.
        GunnsFluidMetabolic article;
        mConfigData->mRecoveryScaleFactor = 0.9;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, 1),
                             TsInitializationException);
        mConfigData->mRecoveryScaleFactor = mRecoveryScaleFactor;
    } {
        /// @test for exception on exercise scale factor less than recovery.
        GunnsFluidMetabolic article;
        mConfigData->mExerciseScaleFactor = 0.9;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, 1),
                             TsInitializationException);
        mConfigData->mExerciseScaleFactor = mExerciseScaleFactor;
    } {
        /// @test for exception on negative number of crew members in nominal state.
        GunnsFluidMetabolic article;
        mInputData->mNNominal = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, 1),
                             TsInitializationException);
        mInputData->mNNominal = mNNominal;
    }  {
        /// @test for exception on negative number of crew members in sleep state.
        GunnsFluidMetabolic article;
        mInputData->mNSleep = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, 1),
                             TsInitializationException);
        mInputData->mNSleep = mNSleep;
    } {
        /// @test for exception on negative number of crew members in recovery state.
        GunnsFluidMetabolic article;
        mInputData->mNRecovery = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, 1),
                             TsInitializationException);
        mInputData->mNRecovery = mNRecovery;
    } {
        /// @test for exception on negative number of crew members in exercise state.
        GunnsFluidMetabolic article;
        mInputData->mNExercise = -0.1;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, 2, 1),
                             TsInitializationException);
        mInputData->mNExercise = mNExercise;
    }

    UT_PASS_LAST;
}

