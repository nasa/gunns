/*
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "UtGunnsFluidDistributed2WayBus.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidDistributed2WayBus class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidDistributed2WayBus::UtGunnsFluidDistributed2WayBus()
    :
    tArticle(0),
    tIsPairMaster(),
    tNumBulk(0),
    tNumTc(0),
    tNumIfBulk(0),
    tNumIfTc(0),
    tFluidState(),
    tFlowState()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidDistributed2WayBus class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidDistributed2WayBus::~UtGunnsFluidDistributed2WayBus()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributed2WayBus::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributed2WayBus::setUp()
{
    tIsPairMaster = true;
    tNumBulk      = 2;
    tNumTc        = 4;
    tNumIfBulk    = 6;
    tNumIfTc      = 6;
    tArticle      = new FriendlyGunnsFluidDistributed2WayBus;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributed2WayBus::testDefaultConstruction()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidDistributed2WayBus 01: testDefaultConstruction .........";

    /// @test state data
    CPPUNIT_ASSERT(false == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(0     == tArticle->mInData.mFrameCount);
    CPPUNIT_ASSERT(0     == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(0.0   == tArticle->mDemandLimitGain);
    CPPUNIT_ASSERT(0.0   == tArticle->mDemandLimitFlowRate);

    /// @test new/delete for code coverage
    GunnsFluidDistributed2WayBus* article = new GunnsFluidDistributed2WayBus();
    delete article;

    GunnsFluidDistributedMixtureData* mixtureData = new GunnsFluidDistributedMixtureData();
    delete mixtureData;

    GunnsFluidDistributed2WayBusFluidState* fluidData = new GunnsFluidDistributed2WayBusFluidState();
    delete fluidData;

    GunnsFluidDistributed2WayBusFlowState* flowData = new GunnsFluidDistributed2WayBusFlowState();
    delete flowData;

    GunnsFluidDistributed2WayBusInterfaceData* ifData = new GunnsFluidDistributed2WayBusInterfaceData();
    delete ifData;

    /// @test call article class functions directly for code coverage
    double mix[3] = {0.0};
    tFluidState.getMoleFractions(mix, 3);
    tFluidState.getTcMoleFractions(mix, 3);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributed2WayBus::testInitialization()
{
    std::cout << "\n UtGunnsFluidDistributed2WayBus 02: testInitialization ..............";

    /// - Create some friendly type pointers for test visibility.
    FriendlyGunnsFluidDistributed2WayBusData* inData  = static_cast<FriendlyGunnsFluidDistributed2WayBusData*>(&tArticle->mInData);
    FriendlyGunnsFluidDistributed2WayBusData* outData = static_cast<FriendlyGunnsFluidDistributed2WayBusData*>(&tArticle->mOutData);

    /// @test initialize with nominal data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tIsPairMaster, tNumIfBulk, tNumIfTc));
    CPPUNIT_ASSERT(tIsPairMaster == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(0.0           == tArticle->mDemandLimitGain);
    CPPUNIT_ASSERT(0.0           == tArticle->mDemandLimitFlowRate);
    CPPUNIT_ASSERT(tNumIfBulk    == inData->mNumFluid);
    CPPUNIT_ASSERT(tNumIfTc      == inData->mNumTc);
    CPPUNIT_ASSERT(tNumIfBulk    == outData->mNumFluid);
    CPPUNIT_ASSERT(tNumIfTc      == outData->mNumTc);

    /// @test interface data assignment.
    inData->mCapacitance        = 1.0;
    inData->mSource             = 2.0;
    inData->mDemandMode         = true;
    inData->mEnergy             = 42.0;
    inData->mFrameCount         = 11;
    inData->mFrameLoopback      = 2112;
    inData->mMoleFractions[0]   = 0.5;
    inData->mMoleFractions[1]   = 0.49;
    inData->mTcMoleFractions[0] = 0.001;
    inData->mTcMoleFractions[1] = 0.002;
    inData->mTcMoleFractions[2] = 0.003;
    inData->mTcMoleFractions[3] = 0.004;

    *outData = *inData;

    CPPUNIT_ASSERT(outData->mCapacitance        == inData->mCapacitance);
    CPPUNIT_ASSERT(outData->mSource             == inData->mSource);
    CPPUNIT_ASSERT(outData->mDemandMode         == inData->mDemandMode);
    CPPUNIT_ASSERT(outData->mEnergy             == inData->mEnergy);
    CPPUNIT_ASSERT(outData->mFrameCount         == inData->mFrameCount);
    CPPUNIT_ASSERT(outData->mFrameLoopback      == inData->mFrameLoopback);
    CPPUNIT_ASSERT(outData->mMoleFractions[0]   == inData->mMoleFractions[0]);
    CPPUNIT_ASSERT(outData->mMoleFractions[1]   == inData->mMoleFractions[1]);
    CPPUNIT_ASSERT(outData->mTcMoleFractions[0] == inData->mTcMoleFractions[0]);
    CPPUNIT_ASSERT(outData->mTcMoleFractions[1] == inData->mTcMoleFractions[1]);
    CPPUNIT_ASSERT(outData->mTcMoleFractions[2] == inData->mTcMoleFractions[2]);
    CPPUNIT_ASSERT(outData->mTcMoleFractions[3] == inData->mTcMoleFractions[3]);

    /// @test repeated initialize with different array sizes.
    tIsPairMaster = false;
    tNumIfBulk    = 2;
    tNumIfTc      = 1;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tIsPairMaster, tNumIfBulk, tNumIfTc));
    CPPUNIT_ASSERT(tIsPairMaster == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(0.0           == tArticle->mDemandLimitGain);
    CPPUNIT_ASSERT(0.0           == tArticle->mDemandLimitFlowRate);
    CPPUNIT_ASSERT(tNumIfBulk    == inData->mNumFluid);
    CPPUNIT_ASSERT(tNumIfTc      == inData->mNumTc);
    CPPUNIT_ASSERT(tNumIfBulk    == outData->mNumFluid);
    CPPUNIT_ASSERT(tNumIfTc      == outData->mNumTc);

    /// @test initialize with zero array sizes.
    tIsPairMaster = false;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tIsPairMaster, 0, 0));
    CPPUNIT_ASSERT(tIsPairMaster == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(0.0           == tArticle->mDemandLimitGain);
    CPPUNIT_ASSERT(0.0           == tArticle->mDemandLimitFlowRate);
    CPPUNIT_ASSERT(0             == inData->mMoleFractions);
    CPPUNIT_ASSERT(0             == inData->mTcMoleFractions);
    CPPUNIT_ASSERT(0             == inData->mNumFluid);
    CPPUNIT_ASSERT(0             == inData->mNumTc);
    CPPUNIT_ASSERT(0             == outData->mMoleFractions);
    CPPUNIT_ASSERT(0             == outData->mTcMoleFractions);
    CPPUNIT_ASSERT(0             == outData->mNumFluid);
    CPPUNIT_ASSERT(0             == outData->mNumTc);

    /// @test assignment operator for fluid and flow state classes.
    GunnsFluidDistributed2WayBusFluidState* fluidState1 = new GunnsFluidDistributed2WayBusFluidState();
    GunnsFluidDistributed2WayBusFluidState* fluidState2 = new GunnsFluidDistributed2WayBusFluidState();
    fluidState1->mPressure = 1.0;
    fluidState1->mEnergy   = 300.0;
    *fluidState2 = *fluidState1;
    CPPUNIT_ASSERT(fluidState1->mPressure == fluidState2->mPressure);
    CPPUNIT_ASSERT(fluidState1->mEnergy   == fluidState2->mEnergy);
    delete fluidState2;
    delete fluidState1;

    GunnsFluidDistributed2WayBusFlowState* flowState1 = new GunnsFluidDistributed2WayBusFlowState();
    GunnsFluidDistributed2WayBusFlowState* flowState2 = new GunnsFluidDistributed2WayBusFlowState();
    flowState1->mFlowRate = 1.0;
    flowState1->mEnergy   = 300.0;
    *flowState2 = *flowState1;
    CPPUNIT_ASSERT(flowState1->mFlowRate == flowState2->mFlowRate);
    CPPUNIT_ASSERT(flowState1->mEnergy   == flowState2->mEnergy);
    delete flowState2;
    delete flowState1;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the processInputs method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributed2WayBus::testProcessInputs()
{
    std::cout << "\n UtGunnsFluidDistributed2WayBus 03: testProcessInputs ...............";

    /// - Initialize our fluid state objects and the test article.  For this test, we are making the
    ///   local model array sizes larger than the interface, to verify that the get fluid state
    ///   functions clear the unused indexes in the local model mixture arrays.
    tNumBulk = 7;
    tNumTc   = 7;
    tFluidState.initialize(tNumBulk, tNumTc);
    tFlowState .initialize(tNumBulk, tNumTc);
    tArticle->initialize(tIsPairMaster, tNumIfBulk, tNumIfTc);

    /// @test with no incoming data.
    tArticle->mInDataLastDemandMode = true;
    tArticle->processInputs();
    CPPUNIT_ASSERT(true  == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(1     == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(1     == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(0     == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(false == tArticle->mInData.hasValidData());

    double inTcFractions[4] = {1.0e-7, 2.0e-7, 3.0e-7, 4.0e-7};
    double inTcFractionSum  = inTcFractions[0] + inTcFractions[1] + inTcFractions[2] + inTcFractions[3];
    double inFractions[2]   = {0.7, 0.3 - inTcFractionSum};

    /// @test with incoming data, flip to demand mode due to equal capacitances and we are the pair
    ///       master.
    tArticle->mInData.mFrameCount         = 1;
    tArticle->mInData.mDemandMode         = false;
    tArticle->mInData.mCapacitance        = 1.0;
    tArticle->mInData.mSource             = 1.0e5;
    tArticle->mInData.mEnergy             = 3.0e5;
    tArticle->mInData.mMoleFractions[0]   = inFractions[0] / (1.0 + inTcFractionSum);
    tArticle->mInData.mMoleFractions[1]   = inFractions[1] / (1.0 + inTcFractionSum);
    tArticle->mInData.mTcMoleFractions[0] = inTcFractions[0] / (1.0 + inTcFractionSum);
    tArticle->mInData.mTcMoleFractions[1] = inTcFractions[1] / (1.0 + inTcFractionSum);
    tArticle->mInData.mTcMoleFractions[2] = inTcFractions[2] / (1.0 + inTcFractionSum);
    tArticle->mInData.mTcMoleFractions[3] = inTcFractions[3] / (1.0 + inTcFractionSum);
    tArticle->mOutData.mCapacitance       = 1.0;
    tArticle->mInDataLastDemandMode       = false;
    tArticle->mFramesSinceFlip            = 2;
    tArticle->processInputs();
    CPPUNIT_ASSERT(true  == tArticle->isInDemandRole());
    CPPUNIT_ASSERT(false == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(0     == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(2     == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(2     == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(1     == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(true  == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(true  == tArticle->mInData.hasValidData());

    GunnsDistributed2WayBusNotification notif;
    CPPUNIT_ASSERT(0 == tArticle->popNotification(notif));
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::INFO == notif.mLevel);
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("switched to Demand mode.", 0));

    /// @test getFluidState() from the previous call to processInputs().
    CPPUNIT_ASSERT(tArticle->getFluidState(tFluidState));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mSource,             tFluidState.mPressure,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mEnergy,             tFluidState.mEnergy,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mMoleFractions[0],   tFluidState.mMoleFractions[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mMoleFractions[1],   tFluidState.mMoleFractions[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                   tFluidState.mMoleFractions[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                   tFluidState.mMoleFractions[3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                   tFluidState.mMoleFractions[4],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                   tFluidState.mMoleFractions[5],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                   tFluidState.mMoleFractions[6],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mTcMoleFractions[0], tFluidState.mTcMoleFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mTcMoleFractions[1], tFluidState.mTcMoleFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mTcMoleFractions[2], tFluidState.mTcMoleFractions[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mTcMoleFractions[3], tFluidState.mTcMoleFractions[3], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                   tFluidState.mTcMoleFractions[4], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                   tFluidState.mTcMoleFractions[5], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                   tFluidState.mTcMoleFractions[6], DBL_EPSILON);

    /// @test getFlowState() when in the wrong mode.
    CPPUNIT_ASSERT(not tArticle->getFlowState(tFlowState));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFlowState.mFlowRate,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFlowState.mEnergy,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFlowState.mMoleFractions[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFlowState.mMoleFractions[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFlowState.mTcMoleFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFlowState.mTcMoleFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFlowState.mTcMoleFractions[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFlowState.mTcMoleFractions[3], DBL_EPSILON);

    /// @test flip to supply mode when incoming data takes over demand.
    tArticle->mInData.mFrameCount  = 2;
    tArticle->mInData.mDemandMode  = true;
    tArticle->mInData.mCapacitance = 1.0;
    tArticle->mInData.mSource      = -1.0;
    tArticle->mFramesSinceFlip     = 999;
    tArticle->processInputs();
    CPPUNIT_ASSERT(false == tArticle->isInDemandRole());
    CPPUNIT_ASSERT(true  == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(0     == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(3     == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(3     == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(2     == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(0     == tArticle->popNotification(notif));
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::INFO == notif.mLevel);
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("switched to Supply mode.", 0));

    /// @test getFlowState() from the previous call to processInputs().
    CPPUNIT_ASSERT(tArticle->getFlowState(tFlowState));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mSource,             tFlowState.mFlowRate,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mEnergy,             tFlowState.mEnergy,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mMoleFractions[0],   tFlowState.mMoleFractions[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mMoleFractions[1],   tFlowState.mMoleFractions[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mTcMoleFractions[0], tFlowState.mTcMoleFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mTcMoleFractions[1], tFlowState.mTcMoleFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mTcMoleFractions[2], tFlowState.mTcMoleFractions[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInData.mTcMoleFractions[3], tFlowState.mTcMoleFractions[3], DBL_EPSILON);

    /// - Reset our test fluid state for verifying the next part:
    tFluidState.mPressure           = 0.0;
    tFluidState.mEnergy             = 0.0;
    tFluidState.mMoleFractions[0]   = 0.0;
    tFluidState.mMoleFractions[1]   = 0.0;
    tFluidState.mTcMoleFractions[0] = 0.0;
    tFluidState.mTcMoleFractions[1] = 0.0;
    tFluidState.mTcMoleFractions[2] = 0.0;
    tFluidState.mTcMoleFractions[3] = 0.0;

    /// @test getFluidState() when in the wrong mode.
    CPPUNIT_ASSERT(not tArticle->getFluidState(tFluidState));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFluidState.mPressure,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFluidState.mEnergy,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFluidState.mMoleFractions[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFluidState.mMoleFractions[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFluidState.mTcMoleFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFluidState.mTcMoleFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFluidState.mTcMoleFractions[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFluidState.mTcMoleFractions[3], DBL_EPSILON);

    /// @test with both sides in supply mode, remain in supply mode due to equal capacitances and we
    ///       are not the pair master.
    tArticle->mInData.mDemandMode = false;
    tArticle->mInData.mSource     = 1.0e5;
    tArticle->mIsPairMaster       = false;
    tArticle->processInputs();
    CPPUNIT_ASSERT(false == tArticle->isInDemandRole());
    CPPUNIT_ASSERT(false == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);

    /// @test with both sides in supply mode, remain in supply mode due to greater capacitance.
    tArticle->mOutData.mCapacitance = 2.0;
    tArticle->processInputs();
    CPPUNIT_ASSERT(false == tArticle->isInDemandRole());
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);

    /// @test with both sides in supply mode, remain in supply mode due to greater capacitance and
    ///       we are the pair master.
    tArticle->mIsPairMaster = true;
    tArticle->processInputs();
    CPPUNIT_ASSERT(false == tArticle->isInDemandRole());
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);

    /// @test with both sides in supply mode, take demand mode due to lesser capacitance.
    tArticle->mOutData.mCapacitance = 0.5;
    tArticle->processInputs();
    CPPUNIT_ASSERT(true == tArticle->isInDemandRole());
    CPPUNIT_ASSERT(true == tArticle->mOutData.mDemandMode);

    /// @test remain in demand mode when incoming data is supply mode.
    tArticle->processInputs();
    CPPUNIT_ASSERT(true == tArticle->isInDemandRole());
    CPPUNIT_ASSERT(true == tArticle->mOutData.mDemandMode);

    /// @test remain in demand mode when both sides are in demand mode and other side has not
    ///       recently flipped to demand mode -- this allows time for the other side to relinquish
    ///       demand mode after we took it, in the presence of data lag.
    tArticle->mInData.mDemandMode   = true;
    tArticle->mInDataLastDemandMode = true;
    tArticle->processInputs();
    CPPUNIT_ASSERT(true == tArticle->isInDemandRole());
    CPPUNIT_ASSERT(true == tArticle->mOutData.mDemandMode);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the processOutput methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributed2WayBus::testProcessOutputs()
{
    std::cout << "\n UtGunnsFluidDistributed2WayBus 04: testProcessOutputs ..............";

    /// - Initialize our fluid state objects and the test article.
    tFluidState.initialize(tNumBulk, tNumTc);
    tFlowState .initialize(tNumBulk, tNumTc);
    tArticle->initialize(tIsPairMaster, tNumIfBulk, tNumIfTc);

    /// @test setFlowState() in demand mode.
    double outTcFractions[4] = {1.0e-7, 2.0e-7, 3.0e-7, 4.0e-7};
    double outTcFractionSum  = outTcFractions[0] + outTcFractions[1] + outTcFractions[2] + outTcFractions[3];
    double outFractions[2]   = {0.7, 0.3 - outTcFractionSum};

    tFlowState.mEnergy             = 275.0;
    tFlowState.mFlowRate           = 0.001;
    tFlowState.mMoleFractions[0]   = outFractions[0];
    tFlowState.mMoleFractions[1]   = outFractions[1];
    tFlowState.mTcMoleFractions[0] = outTcFractions[0];
    tFlowState.mTcMoleFractions[1] = outTcFractions[1];
    tFlowState.mTcMoleFractions[2] = outTcFractions[2];
    tFlowState.mTcMoleFractions[3] = outTcFractions[3];
    tArticle->mOutData.mDemandMode = true;
    tArticle->setFlowState(tFlowState);
    CPPUNIT_ASSERT(true                           == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(tFlowState.mEnergy             == tArticle->mOutData.mEnergy);
    CPPUNIT_ASSERT(tFlowState.mFlowRate           == tArticle->mOutData.mSource);
    CPPUNIT_ASSERT(tFlowState.mMoleFractions[0]   == tArticle->mOutData.mMoleFractions[0]);
    CPPUNIT_ASSERT(tFlowState.mMoleFractions[1]   == tArticle->mOutData.mMoleFractions[1]);
    CPPUNIT_ASSERT(0.0                            == tArticle->mOutData.mMoleFractions[2]);
    CPPUNIT_ASSERT(0.0                            == tArticle->mOutData.mMoleFractions[3]);
    CPPUNIT_ASSERT(0.0                            == tArticle->mOutData.mMoleFractions[4]);
    CPPUNIT_ASSERT(0.0                            == tArticle->mOutData.mMoleFractions[5]);
    CPPUNIT_ASSERT(tFlowState.mTcMoleFractions[0] == tArticle->mOutData.mTcMoleFractions[0]);
    CPPUNIT_ASSERT(tFlowState.mTcMoleFractions[1] == tArticle->mOutData.mTcMoleFractions[1]);
    CPPUNIT_ASSERT(tFlowState.mTcMoleFractions[2] == tArticle->mOutData.mTcMoleFractions[2]);
    CPPUNIT_ASSERT(tFlowState.mTcMoleFractions[3] == tArticle->mOutData.mTcMoleFractions[3]);
    CPPUNIT_ASSERT(0.0                            == tArticle->mOutData.mTcMoleFractions[4]);
    CPPUNIT_ASSERT(0.0                            == tArticle->mOutData.mTcMoleFractions[5]);

    GunnsDistributed2WayBusNotification notif;
    CPPUNIT_ASSERT(0                                         == tArticle->popNotification(notif));
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::NONE == notif.mLevel);

    /// @test setFluidState() in demand mode results in a warning and doesn't change the out data.
    tArticle->setFluidState(tFluidState);
    CPPUNIT_ASSERT(true                           == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(tFlowState.mEnergy             == tArticle->mOutData.mEnergy);
    CPPUNIT_ASSERT(tFlowState.mFlowRate           == tArticle->mOutData.mSource);
    CPPUNIT_ASSERT(tFlowState.mMoleFractions[0]   == tArticle->mOutData.mMoleFractions[0]);
    CPPUNIT_ASSERT(tFlowState.mMoleFractions[1]   == tArticle->mOutData.mMoleFractions[1]);
    CPPUNIT_ASSERT(0.0                            == tArticle->mOutData.mMoleFractions[2]);
    CPPUNIT_ASSERT(0.0                            == tArticle->mOutData.mMoleFractions[3]);
    CPPUNIT_ASSERT(0.0                            == tArticle->mOutData.mMoleFractions[4]);
    CPPUNIT_ASSERT(0.0                            == tArticle->mOutData.mMoleFractions[5]);
    CPPUNIT_ASSERT(tFlowState.mTcMoleFractions[0] == tArticle->mOutData.mTcMoleFractions[0]);
    CPPUNIT_ASSERT(tFlowState.mTcMoleFractions[1] == tArticle->mOutData.mTcMoleFractions[1]);
    CPPUNIT_ASSERT(tFlowState.mTcMoleFractions[2] == tArticle->mOutData.mTcMoleFractions[2]);
    CPPUNIT_ASSERT(tFlowState.mTcMoleFractions[3] == tArticle->mOutData.mTcMoleFractions[3]);
    CPPUNIT_ASSERT(0.0                            == tArticle->mOutData.mTcMoleFractions[4]);
    CPPUNIT_ASSERT(0.0                            == tArticle->mOutData.mTcMoleFractions[5]);
    CPPUNIT_ASSERT(0                              == tArticle->popNotification(notif));
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::WARN == notif.mLevel);
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("setFluidState was called when in the Demand role.", 0));

    /// @test processOutput() in demand mode.
    const double capacitance = 1.0;
    tArticle->processOutputs(capacitance);
    CPPUNIT_ASSERT(true        == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(capacitance == tArticle->mOutData.mCapacitance);

    /// @test setFluidState in supply mode.
    tFluidState.mEnergy             = 275.0;
    tFluidState.mPressure           = 99999.0;
    tFluidState.mMoleFractions[0]   = outFractions[0];
    tFluidState.mMoleFractions[1]   = outFractions[1];
    tFluidState.mTcMoleFractions[0] = outTcFractions[0];
    tFluidState.mTcMoleFractions[1] = outTcFractions[1];
    tFluidState.mTcMoleFractions[2] = outTcFractions[2];
    tFluidState.mTcMoleFractions[3] = outTcFractions[3];
    tArticle->mOutData.mDemandMode  = false;
    tArticle->setFluidState(tFluidState);
    CPPUNIT_ASSERT(false                           == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(tFluidState.mEnergy             == tArticle->mOutData.mEnergy);
    CPPUNIT_ASSERT(tFluidState.mPressure           == tArticle->mOutData.mSource);
    CPPUNIT_ASSERT(tFluidState.mMoleFractions[0]   == tArticle->mOutData.mMoleFractions[0]);
    CPPUNIT_ASSERT(tFluidState.mMoleFractions[1]   == tArticle->mOutData.mMoleFractions[1]);
    CPPUNIT_ASSERT(tFluidState.mTcMoleFractions[0] == tArticle->mOutData.mTcMoleFractions[0]);
    CPPUNIT_ASSERT(tFluidState.mTcMoleFractions[1] == tArticle->mOutData.mTcMoleFractions[1]);
    CPPUNIT_ASSERT(tFluidState.mTcMoleFractions[2] == tArticle->mOutData.mTcMoleFractions[2]);
    CPPUNIT_ASSERT(tFluidState.mTcMoleFractions[3] == tArticle->mOutData.mTcMoleFractions[3]);
    CPPUNIT_ASSERT(0                               == tArticle->popNotification(notif));
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::NONE == notif.mLevel);

    /// @test setFlowState() in demand mode results in a warning and doesn't change the out data.
    tFlowState.mFlowRate           = 0.0;
    tFlowState.mEnergy             = 0.0;
    tFlowState.mMoleFractions[0]   = 0.0;
    tFlowState.mMoleFractions[1]   = 0.0;
    tFlowState.mTcMoleFractions[0] = 0.0;
    tFlowState.mTcMoleFractions[1] = 0.0;
    tFlowState.mTcMoleFractions[2] = 0.0;
    tFlowState.mTcMoleFractions[3] = 0.0;

    tArticle->setFlowState(tFlowState);
    CPPUNIT_ASSERT(false                           == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(tFluidState.mEnergy             == tArticle->mOutData.mEnergy);
    CPPUNIT_ASSERT(tFluidState.mPressure           == tArticle->mOutData.mSource);
    CPPUNIT_ASSERT(tFluidState.mMoleFractions[0]   == tArticle->mOutData.mMoleFractions[0]);
    CPPUNIT_ASSERT(tFluidState.mMoleFractions[1]   == tArticle->mOutData.mMoleFractions[1]);
    CPPUNIT_ASSERT(tFluidState.mTcMoleFractions[0] == tArticle->mOutData.mTcMoleFractions[0]);
    CPPUNIT_ASSERT(tFluidState.mTcMoleFractions[1] == tArticle->mOutData.mTcMoleFractions[1]);
    CPPUNIT_ASSERT(tFluidState.mTcMoleFractions[2] == tArticle->mOutData.mTcMoleFractions[2]);
    CPPUNIT_ASSERT(tFluidState.mTcMoleFractions[3] == tArticle->mOutData.mTcMoleFractions[3]);
    CPPUNIT_ASSERT(0                               == tArticle->popNotification(notif));
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::WARN == notif.mLevel);
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("setFlowState was called when in the Supply role.", 0));

    /// @test outputs in supply mode, without flipping to demand mode.
    tArticle->mInData.mDemandMode  = true;
    tArticle->mInData.mCapacitance = 1.25; // right at the limit before flipping to demand mode.
    tArticle->processOutputs(capacitance);
    CPPUNIT_ASSERT(false        == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(capacitance  == tArticle->mOutData.mCapacitance);

    /// @test not flipping to demand mode due to low capacitance when frames since flip is small.
    tArticle->mInData.mCapacitance = 1.26; // just over limit to cause flip to demand mode.
    tArticle->mFramesSinceFlip     = 4;
    tArticle->mLoopLatency         = 4;
    tArticle->processOutputs(capacitance);
    CPPUNIT_ASSERT(false        == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(capacitance  == tArticle->mOutData.mCapacitance);

    /// @test flip to demand mode due to low capacitance.
    tArticle->mFramesSinceFlip = 5;
    tArticle->processOutputs(capacitance);
    CPPUNIT_ASSERT(true         == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(capacitance  == tArticle->mOutData.mCapacitance);
    CPPUNIT_ASSERT(0.0          == tArticle->mOutData.mSource);
    CPPUNIT_ASSERT(0            == tArticle->popNotification(notif));
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::INFO == notif.mLevel);
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("switched to Demand mode.", 0));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the computeDemandLimit and getDemandLimitGain methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributed2WayBus::testDemandLimit()
{
    std::cout << "\n UtGunnsFluidDistributed2WayBus 05: testDemandLimit .................";

    tArticle->initialize(tIsPairMaster, tNumIfBulk, tNumIfTc);

    /// @test demand limit and filter gain with normal Cs/Cd and latency.
    tArticle->mOutData.mDemandMode  = true;
    tArticle->mOutData.mCapacitance = 0.5;
    tArticle->mInData.mDemandMode   = false;
    tArticle->mInData.mCapacitance  = 1.0;
    tArticle->mInData.mSource       = 99000.0;
    tArticle->mLoopLatency          = 4;
    const double timestep           = 0.1;
    const double demandSideP        = 100000.0;  // ~1 atm, dP = 1%
    double csOverCd     = 1.25; // default moding capacitance ratio upper limit
    double gainLimit    = 1.5 * powf(0.75, 4); // default demand filter constants A & B
    double expectedGain = gainLimit + (1.0 - gainLimit) * (csOverCd - 1.0) * 4.0;
    double expectedNdot = expectedGain * (demandSideP - tArticle->mInData.mSource) / timestep
                        / (1.0 / tArticle->mOutData.mCapacitance + 1.0 / tArticle->mInData.mCapacitance);
    double actualNdot = tArticle->computeDemandLimit(timestep, demandSideP);
    double actualGain = tArticle->getDemandLimitGain();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNdot, actualNdot, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGain, actualGain, FLT_EPSILON);

    /// @test demand limit and gain with Cs < Cd and loop latency 1.
    tArticle->mOutData.mCapacitance = 1.0;
    tArticle->mInData.mCapacitance  = 0.9;
    tArticle->mLoopLatency          = 1;
    csOverCd     = 1.0; // default moding capacitance ratio lower limit
    gainLimit    = 1.0; // upper limit on gain limit
    expectedGain = gainLimit;
    expectedNdot = expectedGain * (demandSideP - tArticle->mInData.mSource) / timestep
                 / (1.0 / tArticle->mOutData.mCapacitance + 1.0 / tArticle->mInData.mCapacitance);
    actualNdot   = tArticle->computeDemandLimit(timestep, demandSideP);
    actualGain   = tArticle->getDemandLimitGain();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNdot, actualNdot, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGain, actualGain, FLT_EPSILON);

    /// @test limits on latency exponent.
    tArticle->mLoopLatency = 0;
    actualNdot = tArticle->computeDemandLimit(timestep, demandSideP);
    actualGain = tArticle->getDemandLimitGain();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNdot, actualNdot, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGain, actualGain, FLT_EPSILON);

    tArticle->mLoopLatency = 200;
    gainLimit    = 1.5 * powf(0.75, 100);
    expectedGain = gainLimit + (1.0 - gainLimit) * (csOverCd - 1.0) * 4.0;
    expectedNdot = expectedGain * (demandSideP - tArticle->mInData.mSource) / timestep
                 / (1.0 / tArticle->mOutData.mCapacitance + 1.0 / tArticle->mInData.mCapacitance);
    actualNdot   = tArticle->computeDemandLimit(timestep, demandSideP);
    actualGain   = tArticle->getDemandLimitGain();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNdot, actualNdot, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGain, actualGain, FLT_EPSILON);

    /// @test demand limit and gain with 1 < Cs/Cd < 1.25.
    tArticle->mOutData.mCapacitance = 1.0;
    tArticle->mInData.mCapacitance  = 1.15;
    tArticle->mLoopLatency          = 2;
    csOverCd     = 1.15;
    gainLimit    = 1.5 * powf(0.75, 2);
    expectedGain = gainLimit + (1.0 - gainLimit) * (csOverCd - 1.0) * 4.0;
    expectedNdot = expectedGain * (demandSideP - tArticle->mInData.mSource) / timestep
                 / (1.0 / tArticle->mOutData.mCapacitance + 1.0 / tArticle->mInData.mCapacitance);
    actualNdot   = tArticle->computeDemandLimit(timestep, demandSideP);
    actualGain   = tArticle->getDemandLimitGain();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNdot, actualNdot, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGain, actualGain, FLT_EPSILON);

    /// @test incoming capacitance is zero.
    tArticle->mInData.mCapacitance = 0.0;
    expectedGain = 0.0;
    expectedNdot = 0.0;
    actualNdot   = tArticle->computeDemandLimit(timestep, demandSideP);
    actualGain   = tArticle->getDemandLimitGain();
    CPPUNIT_ASSERT(expectedNdot == actualNdot);
    CPPUNIT_ASSERT(expectedGain == actualGain);

    /// @test outgoing capacitance is zero.
    tArticle->mInData.mCapacitance  = 1.0;
    tArticle->mOutData.mCapacitance = 0.0;
    actualNdot = tArticle->computeDemandLimit(timestep, demandSideP);
    actualGain = tArticle->getDemandLimitGain();
    CPPUNIT_ASSERT(expectedNdot == actualNdot);
    CPPUNIT_ASSERT(expectedGain == actualGain);

    /// @test zero timestep.
    tArticle->mOutData.mCapacitance = 1.0;
    actualNdot = tArticle->computeDemandLimit(0.0, demandSideP);
    actualGain = tArticle->getDemandLimitGain();
    CPPUNIT_ASSERT(expectedNdot == actualNdot);
    CPPUNIT_ASSERT(expectedGain == actualGain);

    /// @test calling with our side in the wrong mode (Supply).
    tArticle->mOutData.mDemandMode = false;
    actualNdot = tArticle->computeDemandLimit(timestep, demandSideP);
    actualGain = tArticle->getDemandLimitGain();
    CPPUNIT_ASSERT(expectedNdot == actualNdot);
    CPPUNIT_ASSERT(expectedGain == actualGain);

    /// @test calling with other side in the wrong mode (Demand).
    tArticle->mOutData.mDemandMode = true;
    tArticle->mInData.mDemandMode  = true;
    actualNdot = tArticle->computeDemandLimit(timestep, demandSideP);
    actualGain = tArticle->getDemandLimitGain();
    CPPUNIT_ASSERT(expectedNdot == actualNdot);
    CPPUNIT_ASSERT(expectedGain == actualGain);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the mode forcing flags.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributed2WayBus::testForceModes()
{
    std::cout << "\n UtGunnsFluidDistributed2WayBus 06: testForceModes ..................";

    /// @test Starts in Supply mode after initialization regardless of Demand force flag.
    tArticle->forceDemandRole();
    tArticle->initialize(tIsPairMaster, tNumIfBulk, tNumIfTc);
    CPPUNIT_ASSERT(false == tArticle->isInDemandRole());

    /// @test Flips to Demand mode on input regardless of Demand mode in incoming data.
    tArticle->mInData.mDemandMode = true;
    tArticle->mInData.mFrameCount = 1;
    tArticle->processInputs();
    CPPUNIT_ASSERT(0    == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(1    == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(true == tArticle->isInDemandRole());

    /// @test Flips to Supply mode on input regardless of capacitance.
    tArticle->mInData.mDemandMode = false;
    tArticle->mInData.mFrameCount = 2;
    tArticle->forceSupplyRole();
    tArticle->processInputs();
    CPPUNIT_ASSERT(0     == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(2     == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(false == tArticle->isInDemandRole());

    /// @test Stays in Supply mode on output regardless of new local capacitance.
    tArticle->mLoopLatency         = 2;
    tArticle->mFramesSinceFlip     = 3;
    tArticle->mInData.mCapacitance = 1.0;
    tArticle->processOutputs(0.0);
    CPPUNIT_ASSERT(false == tArticle->isInDemandRole());

    std::cout << "... Pass";
}
