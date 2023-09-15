/**
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"
#include "UtGunnsElectDistributedIf.hh"

/// @details  Test identification number.
int UtGunnsElectDistributedIf::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectDistributedIf class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectDistributedIf::UtGunnsElectDistributedIf()
    :
    tLinks(),
    tNodes(),
    tNodeList(),
    tNodeNetCapDp(0),
    tPort0(0),
    tName(""),
    tConfigData(0),
    tInputData(0),
    tArticle(0),
    tInterface(0),
    tVoltageSource(0),
    tPowerLoad(0),
    tIsPairPrimary(false),
    tConductance(0.0),
    tNetCapDvThresh(0.0),
    tForceDemandMode(false),
    tForceSupplyMode(false),
    tSupply()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectDistributedIf class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectDistributedIf::~UtGunnsElectDistributedIf()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::setUp()
{
    tName = "tArticle";
    tLinks.clear();

    /// - Define the nominal port mapping of the test article to node 0.
    tPort0 = 0;

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    /// - Initialize the nodes.
    tNodes[0].initialize("tNodes_0", 119.0);
    tNodes[0].prepareForStart();
    tNodeNetCapDp = new double[N_NODES - 1];
    tNodes[0].setNetCapDeltaPotential(tNodeNetCapDp);

    tNodes[1].initialize("tNodes_0", 120.0);
    tNodes[1].prepareForStart();

    /// - Define the nominal configuration data.
    tIsPairPrimary  = true;
    tConductance    = 500.0;
    tNetCapDvThresh = 1.0e-10;
    tConfigData     = new GunnsElectDistributedIfConfigData(tName, &tNodeList, tIsPairPrimary,
                                                            tConductance, tNetCapDvThresh);
    tConfigData->addSupply(&tSupply);

    /// - Define the nominal input data.
    tForceDemandMode = false;
    tForceSupplyMode = false;
    tInputData       = new GunnsElectDistributedIfInputData(tForceDemandMode, tForceSupplyMode);

    /// - Default construct the nominal test article.
    tArticle       = new FriendlyGunnsElectDistributedIf;
    tInterface     = static_cast<FriendlyGunnsElectDistributed2WayBus*>(&tArticle->mInterface);
    tVoltageSource = static_cast<FriendlyGunnsElectConverterOutput*>(&tArticle->mVoltageSource);
    tPowerLoad     = static_cast<FriendlyGunnsElectConverterInput*>(&tArticle->mPowerLoad);

    /// - Initialize the local network voltage supply link, connected to node 1.
    GunnsElectConverterOutputConfigData supplyConfig("tSupply", &tNodeList,
                                                     GunnsElectConverterOutput::VOLTAGE, 1000.0, 1.0);
    GunnsElectConverterOutputInputData supplyInput(false, 0.0, true, 120.0, 0.0, 120.0);
    tSupply.initialize(supplyConfig, supplyInput, tLinks, 1);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete [] tNodeNetCapDp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Electrical Distributed Interface Link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration nominal construction and addSupply method
    CPPUNIT_ASSERT(tName           == tConfigData->mName);
    CPPUNIT_ASSERT(&tNodeList      == tConfigData->mNodeList);
    CPPUNIT_ASSERT(tIsPairPrimary  == tConfigData->mIsPairPrimary);
    CPPUNIT_ASSERT(tConductance    == tConfigData->mConductance);
    CPPUNIT_ASSERT(tNetCapDvThresh == tConfigData->mNetCapDvThreshold);
    CPPUNIT_ASSERT(1               == tConfigData->mSupplies.size());
    CPPUNIT_ASSERT(&tSupply        == tConfigData->mSupplies.at(0));

    /// @test    Configuration data default construction.
    GunnsElectDistributedIfConfigData defaultConfig;
    CPPUNIT_ASSERT(""    == defaultConfig.mName);
    CPPUNIT_ASSERT(0     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(false == defaultConfig.mIsPairPrimary);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mConductance);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mNetCapDvThreshold);
    CPPUNIT_ASSERT(0     == defaultConfig.mSupplies.size());

    /// @test    Configuration data copy construction.
    GunnsElectDistributedIfConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tName           == copyConfig.mName);
    CPPUNIT_ASSERT(&tNodeList      == copyConfig.mNodeList);
    CPPUNIT_ASSERT(tIsPairPrimary  == copyConfig.mIsPairPrimary);
    CPPUNIT_ASSERT(tConductance    == copyConfig.mConductance);
    CPPUNIT_ASSERT(tNetCapDvThresh == copyConfig.mNetCapDvThreshold);
    CPPUNIT_ASSERT(1               == copyConfig.mSupplies.size());
    CPPUNIT_ASSERT(&tSupply        == copyConfig.mSupplies.at(0));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Electrical Distributed Interface Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    GunnsElectDistributedIfInputData normalInput(true, true);
    CPPUNIT_ASSERT(false == normalInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == normalInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(true  == normalInput.mForceDemandMode);
    CPPUNIT_ASSERT(true  == normalInput.mForceSupplyMode);

    /// @test    Input data default construction.
    GunnsElectDistributedIfInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(false == defaultInput.mForceDemandMode);
    CPPUNIT_ASSERT(false == defaultInput.mForceSupplyMode);

    /// @test    Input data copy construction.
    GunnsElectDistributedIfInputData copyInput(normalInput);
    CPPUNIT_ASSERT(false == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(true  == copyInput.mForceDemandMode);
    CPPUNIT_ASSERT(true  == copyInput.mForceSupplyMode);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsElectConverterInput class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::testConstruction()
{
    UT_RESULT;

    /// @test    Default construction.
    CPPUNIT_ASSERT(""    == tArticle->mName);
    CPPUNIT_ASSERT(false == tInterface->mInData.mDemandMode);
    CPPUNIT_ASSERT(false == tArticle->mMalfPowerLoad);
    CPPUNIT_ASSERT(false == tArticle->mMalfVoltageSource);
    CPPUNIT_ASSERT(false == tArticle->mPowerLoad.mMalfBlockageFlag);
    CPPUNIT_ASSERT(false == tArticle->mVoltageSource.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mNetCapDvThreshold);
    CPPUNIT_ASSERT(0     == tArticle->mSupplies.size());
    CPPUNIT_ASSERT(0     == tArticle->mNumSupplies);
    CPPUNIT_ASSERT(0     == tArticle->mSupplyMonitorIndex);
    CPPUNIT_ASSERT(0     == tArticle->mSupplyMonitor);

    /// @test    New/delete for code coverage.
    GunnsElectDistributedIf* testArticle = new GunnsElectDistributedIf();
    delete testArticle;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed Interface Link nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test    Nominal config data.
    CPPUNIT_ASSERT(tIsPairPrimary == tInterface->mIsPairMaster);
    CPPUNIT_ASSERT(tConductance   == tVoltageSource->mOutputConductance);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(GunnsElectDistributed2WayBus::NONE == tInterface->mForcedRole);

    /// @test    Initialization of child objects.
    CPPUNIT_ASSERT(0                         == tInterface->mNotifications.size());
    CPPUNIT_ASSERT(tName + ".mVoltageSource" == tVoltageSource->mName);
    CPPUNIT_ASSERT(true                      == tVoltageSource->mInitFlag);
    CPPUNIT_ASSERT(tName + ".mPowerLoad"     == tPowerLoad->mName);
    CPPUNIT_ASSERT(true                      == tPowerLoad->mInitFlag);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(false           == tArticle->mMalfPowerLoad);
    CPPUNIT_ASSERT(false           == tArticle->mMalfVoltageSource);
    CPPUNIT_ASSERT(tNetCapDvThresh == tArticle->mNetCapDvThreshold);
    CPPUNIT_ASSERT(1               == tArticle->mNumSupplies);
    CPPUNIT_ASSERT(1               == tArticle->mSupplies.size());
    CPPUNIT_ASSERT(false           == tArticle->mSupplies.at(0).mSupplyData->mAvailable);
    CPPUNIT_ASSERT(0.0             == tArticle->mSupplies.at(0).mSupplyData->mMaximumVoltage);
    CPPUNIT_ASSERT(&tSupply        == tArticle->mSupplies.at(0).mLink);
    CPPUNIT_ASSERT(0.0             == tArticle->mSupplies.at(0).mNetCapDV);
    CPPUNIT_ASSERT(0               == tArticle->mSupplyMonitorIndex);
    CPPUNIT_ASSERT(0               == tArticle->mSupplyMonitor->mSupplyData->mAvailable);
    CPPUNIT_ASSERT(0.0             == tArticle->mSupplyMonitor->mSupplyData->mMaximumVoltage);
    CPPUNIT_ASSERT(&tSupply        == tArticle->mSupplyMonitor->mLink);
    CPPUNIT_ASSERT(0.0             == tArticle->mSupplyMonitor->mNetCapDV);

    /// @test    Child links added to the network links vector.
    CPPUNIT_ASSERT(4              == tLinks.size());
    CPPUNIT_ASSERT(&tSupply       == tLinks.at(0));
    CPPUNIT_ASSERT(tArticle       == tLinks.at(1));
    CPPUNIT_ASSERT(tPowerLoad     == tLinks.at(2));
    CPPUNIT_ASSERT(tVoltageSource == tLinks.at(3));

    /// @test    Re-init with forcing interface to Demand mode.
    tInputData->mForceDemandMode = true;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));
    CPPUNIT_ASSERT(GunnsElectDistributed2WayBus::DEMAND == tInterface->mForcedRole);
    CPPUNIT_ASSERT(1        == tArticle->mNumSupplies);
    CPPUNIT_ASSERT(1        == tArticle->mSupplies.size());

    /// @test    Re-init with forcing interface to Supply mode.
    tInputData->mForceSupplyMode = true;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));
    CPPUNIT_ASSERT(GunnsElectDistributed2WayBus::SUPPLY == tInterface->mForcedRole);
    CPPUNIT_ASSERT(1        == tArticle->mNumSupplies);
    CPPUNIT_ASSERT(1        == tArticle->mSupplies.size());

    CPPUNIT_ASSERT(true == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed Interface Link nominal initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::testInitializationErrors()
{
    UT_RESULT;

    /// @test    Exception thrown for empty link name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Exception thrown for attempting to connect to the Ground node.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, 2), TsInitializationException);

    /// @test    Exception thrown on bad net cap DV threshold.
    tConfigData->mNetCapDvThreshold = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0), TsInitializationException);
    tConfigData->mNetCapDvThreshold = tNetCapDvThresh;

    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed Interface Link restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::testRestart()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test    Restart method clears non-config and non-checkpointed data.
    tVoltageSource->mReverseBiasState = true;
    tArticle->restart();
    CPPUNIT_ASSERT(false == tVoltageSource->mReverseBiasState);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed Interface Link step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::testStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// - Drive the inerface's in & out data, in Supply mode.
    tInterface->mOutData.mFrameCount   = 43;
    tInterface->mInData.mDemandMode    = true;
    tInterface->mInData.mDemandPower   = 100.0;
    tInterface->mInData.mFrameCount    = 44;
    tInterface->mInData.mFrameLoopback = 42;
    tInterface->mInData.mSupplyVoltage = 115.0;
    tInterface->mFramesSinceFlip       = 99;

    /// - Drive the local voltage supply link's output as if it's enabled and controlling.
    tSupply.mRegulatorType        = GunnsElectConverterOutput::VOLTAGE;
    tSupply.mEnabled              = true;
    tSupply.mOutputConductance    = 100.0;
    tSupply.mOutputPowerAvailable = true;
    tSupply.mSetpoint             = 125.0;
    tSupply.mSourceVoltage        = 125.0;

    /// - Drive the article's node's network capacitance delta-potential array as if from a previous
    ///   capacitance request, and indicating conductance to the supply link.
    tNodeNetCapDp[0] = 0.0;
    tNodeNetCapDp[1] = 1.0;

    /// @test    step function updates the interface, supply monitor, voltage source input, and
    ///          calls minorStep.
    tArticle->mAdmittanceUpdate = true;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(true  == tInterface->mSupplyDatas.at(0)->mAvailable);
    CPPUNIT_ASSERT(125.0 == tInterface->mSupplyDatas.at(0)->mMaximumVoltage);
    CPPUNIT_ASSERT(true  == tArticle->mSupplyMonitor->mSupplyData->mAvailable);
    CPPUNIT_ASSERT(125.0 == tArticle->mSupplyMonitor->mSupplyData->mMaximumVoltage);
    CPPUNIT_ASSERT(false == tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT(44    == tInterface->mOutData.mFrameCount);
    CPPUNIT_ASSERT(false == tInterface->mOutData.mDemandMode);
    CPPUNIT_ASSERT(115.0 == tVoltageSource->mInputVoltage);
    CPPUNIT_ASSERT(1.0   == tNodes[0].getNetworkCapacitanceRequest());
    CPPUNIT_ASSERT(tNodes[0].getPotential() == tInterface->mOutData.mSupplyVoltage);

    /// @test    interface update when the voltage source child link has input power not valid.
    tVoltageSource->mInputPowerValid = false;
    tArticle->step(0.0);
    CPPUNIT_ASSERT(0.0 == tInterface->mOutData.mDemandPower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Electrical Distributed Interface Link minorStep method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::testMinorStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test    Minor step in Supply role.
    tInterface->mOutData.mDemandMode = false;
    tInterface->mInData.mDemandPower = 10.0;
    tVoltageSource->setEnabled(true);
    tPowerLoad->setEnabled(false);
    tArticle->mAdmittanceMatrix[0] = 1.0;
    tArticle->mSourceVector[0]     = 1.0;
    tArticle->mAdmittanceUpdate    = true;
    tArticle->minorStep(0.0, 1);
    CPPUNIT_ASSERT(false == tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT(0.0   == tArticle->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourceVector[0]);
    CPPUNIT_ASSERT(false == tVoltageSource->mEnabled);
    CPPUNIT_ASSERT(true  == tPowerLoad->mEnabled);
    CPPUNIT_ASSERT(10.0  == tPowerLoad->mInputPower);
    const double iuvLimit = 0.9 * tNodes[tPort0].getPotential();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(iuvLimit, tPowerLoad->getInputUnderVoltageTrip()->getLimit(), DBL_EPSILON);

    /// @test    Minor step in Supply role, with the internal power load link failed.
    tArticle->mMalfPowerLoad = true;
    tArticle->minorStep(0.0, 1);
    CPPUNIT_ASSERT(false == tPowerLoad->mEnabled);

    /// @test    Minor step in Demand role.
    tInterface->mOutData.mDemandMode   = true;
    tVoltageSource->setEnabled(false);
    tPowerLoad->setEnabled(true);
    tArticle->mAdmittanceMatrix[0] = 1.0;
    tArticle->mSourceVector[0]     = 1.0;
    tArticle->mAdmittanceUpdate    = true;
    tArticle->minorStep(0.0, 1);
    CPPUNIT_ASSERT(false == tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT(0.0   == tArticle->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourceVector[0]);
    CPPUNIT_ASSERT(true  == tVoltageSource->mEnabled);
    CPPUNIT_ASSERT(false == tPowerLoad->mEnabled);
    CPPUNIT_ASSERT(0.0   == tPowerLoad->mInputPower);
    CPPUNIT_ASSERT(0.0   == tPowerLoad->getInputUnderVoltageTrip()->getLimit());

    /// @test    Minor step in Demand role, with the internal voltage source link failed.
    tArticle->mMalfVoltageSource = true;
    tArticle->minorStep(0.0, 1);
    CPPUNIT_ASSERT(false == tVoltageSource->mEnabled);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Electrical Distributed Interface Link getter and setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::testAccessors()
{
    UT_RESULT;

    /// @test    Link is non-linear.
    CPPUNIT_ASSERT(true == tArticle->isNonLinear());

    /// @test    No exception from updateSupplyMonitor with zero vector size.
    CPPUNIT_ASSERT_NO_THROW(tArticle->updateSupplyMonitor());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Electrical Distributed Interface Link confirmSolutionAcceptable method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::testConfirmSolutionAcceptable()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// - Set up the interface and supply link such that the interface will switch from Supply to
    ///   Demand role due to the local voltage supply having a lower control voltage than the
    ///   incoming supply voltage from the remote side.
    tInterface->mOutData.mDemandMode   = false;
    tInterface->mOutData.mFrameCount   = 43;
    tInterface->mInData.mDemandMode    = true;
    tInterface->mInData.mDemandPower   = 100.0;
    tInterface->mInData.mFrameCount    = 44;
    tInterface->mInData.mFrameLoopback = 42;
    tInterface->mInData.mSupplyVoltage = 115.0;
    tInterface->mFramesSinceFlip       = 99;
    tSupply.mRegulatorType             = GunnsElectConverterOutput::VOLTAGE;
    tSupply.mEnabled                   = true;
    tSupply.mOutputConductance         = 100.0;
    tSupply.mOutputPowerAvailable      = true;
    tSupply.mSetpoint                  = 105.0;
    tNodeNetCapDp[0]                   = 0.0;
    tNodeNetCapDp[1]                   = 1.0;

    /// @test    the interface is updated in confirmSolutionAcceptable, and causes the link to
    ///          reject the solution when the interface changes from Supply to Demand role.
    GunnsBasicLink::SolutionResult result = tArticle->confirmSolutionAcceptable(0, 0);
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT == result);
    CPPUNIT_ASSERT(true                   == tInterface->mOutData.mDemandMode);
    CPPUNIT_ASSERT(43                     == tInterface->mOutData.mFrameCount);

    /// @test    the link confirms the solution when there is no change to the interface role.
    result = tArticle->confirmSolutionAcceptable(0, 0);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == result);
    CPPUNIT_ASSERT(true                    == tInterface->mOutData.mDemandMode);
    CPPUNIT_ASSERT(43                      == tInterface->mOutData.mFrameCount);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeFlows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributedIf::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    tInterface->mOutData.mDemandMode    = false;
    tInterface->mOutData.mFrameCount    = 43;
    tInterface->mInData.mDemandMode     = true;
    tInterface->mInData.mDemandPower    = 100.0;
    tInterface->mInData.mFrameCount     = 44;
    tInterface->mInData.mFrameLoopback  = 42;
    tInterface->mInData.mSupplyVoltage  = 115.0;
    tInterface->mFramesSinceFlip        = 99;
    tNodeNetCapDp[0]                    = 0.0;
    tNodeNetCapDp[1]                    = 1.0;
    tArticle->mPotentialVector[0]       = 99.0;
    tVoltageSource->mPotentialVector[0] = 99.0;
    tPowerLoad->mPotentialVector[0]     = 99.0;
    tSupply.mSourceVoltage              = 99.0;

    /// @test    computeFlows updates the interface and calls the child links.
    tArticle->computeFlows(0.0);
    CPPUNIT_ASSERT( 99.0 == tInterface->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT( 43   == tInterface->mOutData.mFrameCount);
    CPPUNIT_ASSERT(-99.0 == tVoltageSource->mPotentialDrop);
    CPPUNIT_ASSERT( 99.0 == tPowerLoad->mPotentialDrop);

    UT_PASS_FINAL;
}
