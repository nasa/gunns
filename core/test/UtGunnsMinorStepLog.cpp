/**
@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "UtGunnsMinorStepLog.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <istream>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsMinorStepLog class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsMinorStepLog::UtGunnsMinorStepLog()
    :
    tArticle(0),
    tCommand(),
    tLogSteps(0),
    tPath(""),
    tName(""),
    tNetworkSize(0),
    tNumLinks(0),
    tLinks(0),
    tLink0(),
    tLink1(),
    tLink2()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsMinorStepLog class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsMinorStepLog::~UtGunnsMinorStepLog()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLog::tearDown()
{
    delete tArticle;
    delete [] tLinks;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLog::setUp()
{
    tName     = "tArticle";
    tCommand  = GunnsMinorStepLogInputData::RECORD_AUTO;
    tLogSteps = 3;
    tPath     = "./";

    /// - Set up network links.
    tLinks       = new GunnsBasicLink*[3];
    tLinks[0]    = &tLink0;
    tLinks[1]    = &tLink1;
    tLinks[2]    = &tLink2;
    tLink0.mName = "tLink0";
    tLink1.mName = "tLink1";
    tLink2.mName = "tLink2";
    tNetworkSize = 5;
    tNumLinks    = 3;

    /// - Create the test article and load nominal input data.
    tArticle = new FriendlyGunnsMinorStepLog;
    tArticle->mInputData.mModeCommand = tCommand;
    tArticle->mInputData.mLogSteps    = tLogSteps;
    tArticle->mInputData.mPath        = tPath;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsMinorStepLogLinkData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLog::testLinkData()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsMinorStepLog .... 01: testLinkData ..........................";

    /// @test nominal construction.
    GunnsMinorStepLinkData nominal(2);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == nominal.mSolutionResult);
    CPPUNIT_ASSERT(4                       == nominal.mAdmittanceMatrix.size());
    CPPUNIT_ASSERT(0.0                     == nominal.mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(0.0                     == nominal.mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT(0.0                     == nominal.mAdmittanceMatrix[2]);
    CPPUNIT_ASSERT(0.0                     == nominal.mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT(2                       == nominal.mSourceVector.size());
    CPPUNIT_ASSERT(0.0                     == nominal.mSourceVector[0]);
    CPPUNIT_ASSERT(0.0                     == nominal.mSourceVector[1]);

    /// @test default construction.
    GunnsMinorStepLinkData def;
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == def.mSolutionResult);
    CPPUNIT_ASSERT(0                       == def.mAdmittanceMatrix.size());
    CPPUNIT_ASSERT(0                       == def.mSourceVector.size());

    /// @test copy construction.
    nominal.mSolutionResult      = GunnsBasicLink::REJECT;
    nominal.mAdmittanceMatrix[3] = 1.0;
    nominal.mSourceVector[1]     = -1.0;
    GunnsMinorStepLinkData copy(nominal);
    CPPUNIT_ASSERT(copy.mSolutionResult      == nominal.mSolutionResult);
    CPPUNIT_ASSERT(copy.mAdmittanceMatrix[0] == nominal.mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(copy.mAdmittanceMatrix[1] == nominal.mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT(copy.mAdmittanceMatrix[2] == nominal.mAdmittanceMatrix[2]);
    CPPUNIT_ASSERT(copy.mAdmittanceMatrix[3] == nominal.mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT(copy.mSourceVector[0]     == nominal.mSourceVector[0]);
    CPPUNIT_ASSERT(copy.mSourceVector[1]     == nominal.mSourceVector[1]);

    /// @test assignment.
    def = copy;
    CPPUNIT_ASSERT(copy.mSolutionResult      == def.mSolutionResult);
    CPPUNIT_ASSERT(copy.mAdmittanceMatrix[0] == def.mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(copy.mAdmittanceMatrix[1] == def.mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT(copy.mAdmittanceMatrix[2] == def.mAdmittanceMatrix[2]);
    CPPUNIT_ASSERT(copy.mAdmittanceMatrix[3] == def.mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT(copy.mSourceVector[0]     == def.mSourceVector[0]);
    CPPUNIT_ASSERT(copy.mSourceVector[1]     == def.mSourceVector[1]);

    /// @test renderSolutionResult.
    nominal.mSolutionResult = GunnsBasicLink::CONFIRM;
    CPPUNIT_ASSERT("CONFIRM" == nominal.renderSolutionResult());
    nominal.mSolutionResult = GunnsBasicLink::REJECT;
    CPPUNIT_ASSERT("REJECT" == nominal.renderSolutionResult());
    nominal.mSolutionResult = GunnsBasicLink::DELAY;
    CPPUNIT_ASSERT("DELAY" == nominal.renderSolutionResult());
    nominal.mSolutionResult = static_cast<GunnsBasicLink::SolutionResult>(42);
    CPPUNIT_ASSERT("invalid" == nominal.renderSolutionResult());

    /// @test new/delete for code coverage
    GunnsMinorStepLinkData* article = new GunnsMinorStepLinkData;
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsMinorStepLogData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLog::testData()
{
    std::cout << "\n UtGunnsMinorStepLog .... 02: testData ..............................";

    /// @test nominal construction.
    GunnsMinorStepData nominal(2, 3, tLinks);
    CPPUNIT_ASSERT(0                           == nominal.mMajorStep);
    CPPUNIT_ASSERT(0                           == nominal.mMinorStep);
    CPPUNIT_ASSERT(0                           == nominal.mDecomposition);
    CPPUNIT_ASSERT(GunnsMinorStepData::SUCCESS == nominal.mSolutionResult);
    CPPUNIT_ASSERT(2                           == nominal.mPotentialVector.size());
    CPPUNIT_ASSERT(0.0                         == nominal.mPotentialVector[0]);
    CPPUNIT_ASSERT(0.0                         == nominal.mPotentialVector[1]);
    CPPUNIT_ASSERT(2                           == nominal.mNodesConvergence.size());
    CPPUNIT_ASSERT(0.0                         == nominal.mNodesConvergence[0]);
    CPPUNIT_ASSERT(0.0                         == nominal.mNodesConvergence[1]);
    CPPUNIT_ASSERT(3                           == nominal.mLinksData.size());

    /// @test default construction.
    GunnsMinorStepData def;
    CPPUNIT_ASSERT(0                           == def.mMajorStep);
    CPPUNIT_ASSERT(0                           == def.mMinorStep);
    CPPUNIT_ASSERT(0                           == def.mDecomposition);
    CPPUNIT_ASSERT(GunnsMinorStepData::SUCCESS == def.mSolutionResult);
    CPPUNIT_ASSERT(0                           == def.mPotentialVector.size());
    CPPUNIT_ASSERT(0                           == def.mNodesConvergence.size());
    CPPUNIT_ASSERT(0                           == def.mLinksData.size());

    /// @test copy construction.
    nominal.mMajorStep                    = 1;
    nominal.mMinorStep                    = 2;
    nominal.mDecomposition                = 3;
    nominal.mSolutionResult               = GunnsMinorStepData::REJECT;
    nominal.mPotentialVector[1]           = 4.0;
    nominal.mNodesConvergence[1]          = -5000.0;
    nominal.mLinksData[2].mSolutionResult = GunnsBasicLink::DELAY;
    GunnsMinorStepData copy(nominal);
    CPPUNIT_ASSERT(nominal.mMajorStep                    == copy.mMajorStep);
    CPPUNIT_ASSERT(nominal.mMinorStep                    == copy.mMinorStep);
    CPPUNIT_ASSERT(nominal.mDecomposition                == copy.mDecomposition);
    CPPUNIT_ASSERT(nominal.mSolutionResult               == copy.mSolutionResult);
    CPPUNIT_ASSERT(nominal.mPotentialVector[1]           == copy.mPotentialVector[1]);
    CPPUNIT_ASSERT(nominal.mNodesConvergence[1]          == copy.mNodesConvergence[1]);
    CPPUNIT_ASSERT(nominal.mLinksData[2].mSolutionResult == copy.mLinksData[2].mSolutionResult);

    /// @test assignment
    def = copy;
    CPPUNIT_ASSERT(nominal.mMajorStep                    == def.mMajorStep);
    CPPUNIT_ASSERT(nominal.mMinorStep                    == def.mMinorStep);
    CPPUNIT_ASSERT(nominal.mDecomposition                == def.mDecomposition);
    CPPUNIT_ASSERT(nominal.mSolutionResult               == def.mSolutionResult);
    CPPUNIT_ASSERT(nominal.mPotentialVector[1]           == def.mPotentialVector[1]);
    CPPUNIT_ASSERT(nominal.mNodesConvergence[1]          == def.mNodesConvergence[1]);
    CPPUNIT_ASSERT(nominal.mLinksData[2].mSolutionResult == def.mLinksData[2].mSolutionResult);

    /// @test renderSolutionResult.
    nominal.mSolutionResult = GunnsMinorStepData::SUCCESS;
    CPPUNIT_ASSERT("SUCCESS" == nominal.renderSolutionResult());
    nominal.mSolutionResult = GunnsMinorStepData::CONFIRM;
    CPPUNIT_ASSERT("CONFIRM" == nominal.renderSolutionResult());
    nominal.mSolutionResult = GunnsMinorStepData::REJECT;
    CPPUNIT_ASSERT("REJECT" == nominal.renderSolutionResult());
    nominal.mSolutionResult = GunnsMinorStepData::DELAY;
    CPPUNIT_ASSERT("DELAY" == nominal.renderSolutionResult());
    nominal.mSolutionResult = GunnsMinorStepData::DECOMP_LIMIT;
    CPPUNIT_ASSERT("DECOMP_LIMIT" == nominal.renderSolutionResult());
    nominal.mSolutionResult = GunnsMinorStepData::MINOR_LIMIT;
    CPPUNIT_ASSERT("MINOR_LIMIT" == nominal.renderSolutionResult());
    nominal.mSolutionResult = GunnsMinorStepData::MATH_FAIL;
    CPPUNIT_ASSERT("MATH_FAIL" == nominal.renderSolutionResult());
    nominal.mSolutionResult = static_cast<GunnsMinorStepData::SolutionResults>(42);
    CPPUNIT_ASSERT("invalid" == nominal.renderSolutionResult());

    /// @test new/delete for code coverage
    GunnsMinorStepData* article = new GunnsMinorStepData;
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsMinorStepLogInputData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLog::testInputData()
{
    std::cout << "\n UtGunnsMinorStepLog .... 03: testInputData .........................";

    /// @test nominal construction.
    GunnsMinorStepLogInputData nominal(GunnsMinorStepLogInputData::SNAP, 10, "./RUN/");
    CPPUNIT_ASSERT(GunnsMinorStepLogInputData::SNAP == nominal.mModeCommand);
    CPPUNIT_ASSERT(10                               == nominal.mLogSteps);
    CPPUNIT_ASSERT("./RUN/"                         == nominal.mPath);

    /// @test default construction.
    GunnsMinorStepLogInputData def;
    CPPUNIT_ASSERT(GunnsMinorStepLogInputData::PAUSE == def.mModeCommand);
    CPPUNIT_ASSERT(0                                 == def.mLogSteps);
    CPPUNIT_ASSERT(""                                == def.mPath);

    /// @test new/delete for code coverage
    GunnsMinorStepLogInputData* article = new GunnsMinorStepLogInputData;
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsMinorStepLog class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLog::testDefaultConstruction()
{
    std::cout << "\n UtGunnsMinorStepLog .... 04: testDefaultConstruction ...............";

    /// @test state data
    FriendlyGunnsMinorStepLog def;
    CPPUNIT_ASSERT(0                         == def.mInputData.mLogSteps);
    CPPUNIT_ASSERT(""                        == def.mName);
    CPPUNIT_ASSERT(GunnsMinorStepLog::PAUSED == def.mState);
    CPPUNIT_ASSERT(false                     == def.mIsRecording);
    CPPUNIT_ASSERT(0                         == def.mBuffer.size());
    CPPUNIT_ASSERT(0                         == def.mSize);
    CPPUNIT_ASSERT(0                         == def.mHeadIndex);
    CPPUNIT_ASSERT(0                         == def.mNumValidSteps);
    CPPUNIT_ASSERT(0                         == def.mLinks);
    CPPUNIT_ASSERT(""                        == def.mLogHeader);
    CPPUNIT_ASSERT(""                        == def.mFilename);

    /// @test new/delete for code coverage
    GunnsMinorStepLog* article = new GunnsMinorStepLog;
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize and validation methods of the GunnsMinorStepLog
///           class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLog::testInitialize()
{
    std::cout << "\n UtGunnsMinorStepLog .... 05: testInitialize ........................";

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName, tNetworkSize, tNumLinks, tLinks));

    /// @test nominal initialization.
    CPPUNIT_ASSERT(tName                     == tArticle->mName);
    CPPUNIT_ASSERT(GunnsMinorStepLog::PAUSED == tArticle->mState);
    CPPUNIT_ASSERT(true                      == tArticle->mIsRecording);
    CPPUNIT_ASSERT(tLogSteps                 == tArticle->mBuffer.size());
    CPPUNIT_ASSERT(tNetworkSize              == tArticle->mBuffer[0].mPotentialVector.size());
    CPPUNIT_ASSERT(tNumLinks                 == tArticle->mBuffer[0].mLinksData.size());
    CPPUNIT_ASSERT(tLogSteps                 == tArticle->mSize);
    CPPUNIT_ASSERT(tLogSteps - 1             == tArticle->mHeadIndex);
    CPPUNIT_ASSERT(0                         == tArticle->mNumValidSteps);
    CPPUNIT_ASSERT(tLinks                    == tArticle->mLinks);
    CPPUNIT_ASSERT(""                        == tArticle->mFilename);

    std::ostringstream expectedLogHeader;
    expectedLogHeader << "Major Step, Minor Step, Decomposition, Solution Result"
            << ", Potential Vector [0], Potential Vector [1], Potential Vector [2]"
            << ", Potential Vector [3], Potential Vector [4]"
            << ", Node Convergence [0], Node Convergence [1], Node Convergence [2]"
            << ", Node Convergence [3], Node Convergence [4]"
            << ", Link tLink0: Solution Result, Admittance Matrix [0], Admittance Matrix [1]"
            << ", Admittance Matrix [2], Admittance Matrix [3], Source Vector [0], Source Vector [1]"
            << ", Link tLink1: Solution Result, Admittance Matrix [0], Admittance Matrix [1]"
            << ", Admittance Matrix [2], Admittance Matrix [3], Source Vector [0], Source Vector [1]"
            << ", Link tLink2: Solution Result, Admittance Matrix [0], Admittance Matrix [1]"
            << ", Admittance Matrix [2], Admittance Matrix [3], Source Vector [0], Source Vector [1]"
            << std::endl;
    CPPUNIT_ASSERT(expectedLogHeader.str() == tArticle->mLogHeader);

    /// - Test exception thrown from missing name.
    CPPUNIT_ASSERT_THROW(tArticle->initialize("", tNetworkSize, tNumLinks, tLinks), TsInitializationException);

    /// - Test exception thrown from network size < 1.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, 0, tNumLinks, tLinks), TsInitializationException);

    /// - Test exception thrown from number of links < 1.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tNetworkSize, 0, tLinks), TsInitializationException);

    /// - Test exception thrown from null links pointer.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tNetworkSize, tNumLinks, 0), TsInitializationException);

    /// - Test exception thrown on initial SNAP command.
    tArticle->mInputData.mModeCommand = GunnsMinorStepLogInputData::SNAP;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tNetworkSize, tNumLinks, tLinks), TsInitializationException);
    tArticle->mInputData.mModeCommand = tCommand;

    /// - Test exception thrown on bad initial/default log steps.
    tArticle->mInputData.mLogSteps = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tNetworkSize, tNumLinks, tLinks), TsInitializationException);
    tArticle->mInputData.mLogSteps = tLogSteps;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsMinorStepLog class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLog::testBeginMajorStep()
{
    std::cout << "\n UtGunnsMinorStepLog .... 06: testBeginMajorStep ....................";

    tArticle->initialize(tName, tNetworkSize, tNumLinks, tLinks);

    /// @test  Transition from PAUSED to RECORDING_AUTO
    tArticle->mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_AUTO;
    tArticle->beginMajorStep();
    CPPUNIT_ASSERT(GunnsMinorStepLogInputData::RECORD_AUTO == tArticle->mInputData.mModeCommand);
    CPPUNIT_ASSERT(GunnsMinorStepLog::RECORDING_AUTO       == tArticle->mState);
    CPPUNIT_ASSERT(true                                    == tArticle->mIsRecording);
    CPPUNIT_ASSERT(""                                      == tArticle->mFilename);

    /// @test  Transition from RECORDING_AUTO to PAUSED
    tArticle->mInputData.mModeCommand = GunnsMinorStepLogInputData::PAUSE;
    tArticle->beginMajorStep();
    CPPUNIT_ASSERT(GunnsMinorStepLogInputData::PAUSE == tArticle->mInputData.mModeCommand);
    CPPUNIT_ASSERT(GunnsMinorStepLog::PAUSED         == tArticle->mState);
    CPPUNIT_ASSERT(false                             == tArticle->mIsRecording);
    CPPUNIT_ASSERT(""                                == tArticle->mFilename);

    /// @test  Transition from PAUSED to RECORDING_SNAP
    tArticle->mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_SNAP;
    tArticle->beginMajorStep();
    CPPUNIT_ASSERT(GunnsMinorStepLogInputData::RECORD_SNAP == tArticle->mInputData.mModeCommand);
    CPPUNIT_ASSERT(GunnsMinorStepLog::RECORDING_SNAP       == tArticle->mState);
    CPPUNIT_ASSERT(true                                    == tArticle->mIsRecording);
    CPPUNIT_ASSERT(""                                      == tArticle->mFilename);

    /// @test  Transition from RECORDING_SNAP to SNAP back to RECORDING_SNAP
    tArticle->mInputData.mModeCommand = GunnsMinorStepLogInputData::SNAP;
    tArticle->beginMajorStep();
    CPPUNIT_ASSERT(GunnsMinorStepLogInputData::RECORD_SNAP == tArticle->mInputData.mModeCommand);
    CPPUNIT_ASSERT(GunnsMinorStepLog::WRITING_FILE         == tArticle->mState);
    CPPUNIT_ASSERT(false                                   == tArticle->mIsRecording);
    CPPUNIT_ASSERT(tPath + tName + "_0.csv"                == tArticle->mFilename);

    /// @test  Nothing happens in WRITING_FILE
    tArticle->mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_AUTO;
    tArticle->beginMajorStep();
    CPPUNIT_ASSERT(GunnsMinorStepLogInputData::RECORD_AUTO == tArticle->mInputData.mModeCommand);
    CPPUNIT_ASSERT(GunnsMinorStepLog::WRITING_FILE         == tArticle->mState);
    CPPUNIT_ASSERT(false                                   == tArticle->mIsRecording);
    CPPUNIT_ASSERT(tPath + tName + "_0.csv"                == tArticle->mFilename);

    /// @test  Transition from RECORDING_SNAP to RECORDING_AUTO
    tArticle->mFilename  = "";
    tArticle->mState     = GunnsMinorStepLog::PAUSED;
    tArticle->beginMajorStep();
    CPPUNIT_ASSERT(GunnsMinorStepLogInputData::RECORD_AUTO == tArticle->mInputData.mModeCommand);
    CPPUNIT_ASSERT(GunnsMinorStepLog::RECORDING_AUTO       == tArticle->mState);
    CPPUNIT_ASSERT(true                                    == tArticle->mIsRecording);
    CPPUNIT_ASSERT(""                                      == tArticle->mFilename);

    /// @test  Transition from RECORDING_AUTO to SNAP back to RECORDING_AUTO
    tArticle->mInputData.mModeCommand = GunnsMinorStepLogInputData::SNAP;
    tArticle->beginMajorStep();
    CPPUNIT_ASSERT(GunnsMinorStepLogInputData::RECORD_AUTO == tArticle->mInputData.mModeCommand);
    CPPUNIT_ASSERT(GunnsMinorStepLog::WRITING_FILE         == tArticle->mState);
    CPPUNIT_ASSERT(false                                   == tArticle->mIsRecording);
    CPPUNIT_ASSERT(tPath + tName + "_0.csv"                == tArticle->mFilename);

    /// @test  Transition from PAUSED to SNAP back to PAUSED
    tArticle->mFilename    = "";
    tArticle->mState       = GunnsMinorStepLog::PAUSED;
    tArticle->mInputData.mModeCommand = GunnsMinorStepLogInputData::SNAP;
    tArticle->beginMajorStep();
    CPPUNIT_ASSERT(GunnsMinorStepLogInputData::PAUSE == tArticle->mInputData.mModeCommand);
    CPPUNIT_ASSERT(GunnsMinorStepLog::WRITING_FILE   == tArticle->mState);
    CPPUNIT_ASSERT(false                             == tArticle->mIsRecording);
    CPPUNIT_ASSERT(tPath + tName + "_0.csv"          == tArticle->mFilename);

    /// @test  Nothing happens for invalid command input
    tArticle->mFilename  = "";
    tArticle->mState     = GunnsMinorStepLog::PAUSED;
    tArticle->mInputData.mModeCommand = static_cast<GunnsMinorStepLogInputData::LogCommands>(16);
    tArticle->beginMajorStep();
    CPPUNIT_ASSERT(16                        == static_cast<int>(tArticle->mInputData.mModeCommand));
    CPPUNIT_ASSERT(GunnsMinorStepLog::PAUSED == tArticle->mState);
    CPPUNIT_ASSERT(false                     == tArticle->mIsRecording);
    CPPUNIT_ASSERT(""                        == tArticle->mFilename);

    /// @test  Transition to PAUSED if buffer size is zero.
    FriendlyGunnsMinorStepLog article2;
    article2.initialize(tName, tNetworkSize, tNumLinks, tLinks);
    article2.mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_AUTO;
    article2.beginMajorStep();
    CPPUNIT_ASSERT(GunnsMinorStepLogInputData::PAUSE == article2.mInputData.mModeCommand);
    CPPUNIT_ASSERT(GunnsMinorStepLog::PAUSED         == article2.mState);
    CPPUNIT_ASSERT(false                             == article2.mIsRecording);
    CPPUNIT_ASSERT(""                                == article2.mFilename);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the record methods of the GunnsMinorStepLog class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLog::testRecord()
{
    std::cout << "\n UtGunnsMinorStepLog .... 07: testRecord ............................";

    tNetworkSize = 2;
    tArticle->initialize(tName, tNetworkSize, tNumLinks, tLinks);

    /// - Override the link's [A] and {w} arrays so we don't have to init the links.
    double admittance[4] = {100.0, 0.0, 0.0, 200.0};
    double source[2]     = {12.6, -12.6};
    tLink0.mAdmittanceMatrix = admittance;
    tLink1.mAdmittanceMatrix = admittance;
    tLink2.mAdmittanceMatrix = admittance;
    tLink0.mSourceVector     = source;
    tLink1.mSourceVector     = source;
    tLink2.mSourceVector     = source;

    /// - Begin the major step in paused mode
    tArticle->mInputData.mModeCommand = GunnsMinorStepLogInputData::PAUSE;
    tArticle->beginMajorStep();
    tArticle->beginMinorStep(1, 1);

    /// @test record functions don't record when recording isn't enabled.
    int index = tLogSteps - 1;
    CPPUNIT_ASSERT(index == tArticle->mHeadIndex);
    double potentials[2] = {200.0, 300.0};
    tArticle->recordPotential(potentials);
    CPPUNIT_ASSERT(0.0 == tArticle->mBuffer[index].mPotentialVector[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mBuffer[index].mPotentialVector[1]);

    double nodesConvergence[2] = {-1.0, 6.2};
    tArticle->recordNodesConvergence(nodesConvergence);
    CPPUNIT_ASSERT(0.0 == tArticle->mBuffer[index].mNodesConvergence[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mBuffer[index].mNodesConvergence[1]);

    tArticle->recordLinkContributions();
    CPPUNIT_ASSERT(0.0 == tArticle->mBuffer[index].mLinksData[0].mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mBuffer[index].mLinksData[0].mSourceVector[0]);

    tArticle->recordLinkResult(0, GunnsBasicLink::DELAY);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->mBuffer[index].mLinksData[0].mSolutionResult);

    tArticle->recordAllLinksResult(GunnsBasicLink::DELAY);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->mBuffer[index].mLinksData[0].mSolutionResult);

    tArticle->recordStepResult(1, GunnsMinorStepData::MATH_FAIL);
    CPPUNIT_ASSERT(0                           == tArticle->mNumValidSteps);
    CPPUNIT_ASSERT(0                           == tArticle->mBuffer[index].mMajorStep);
    CPPUNIT_ASSERT(0                           == tArticle->mBuffer[index].mMinorStep);
    CPPUNIT_ASSERT(0                           == tArticle->mBuffer[index].mDecomposition);
    CPPUNIT_ASSERT(GunnsMinorStepData::SUCCESS == tArticle->mBuffer[index].mSolutionResult);

    /// - Switch to a recording mode.
    tArticle->mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_SNAP;
    tArticle->beginMajorStep();
    tArticle->beginMinorStep(2, 1);

    /// @test record functions when recording is enabled.
    index = 0;
    CPPUNIT_ASSERT(index == tArticle->mHeadIndex);
    tArticle->recordPotential(potentials);
    CPPUNIT_ASSERT(potentials[0] == tArticle->mBuffer[index].mPotentialVector[0]);
    CPPUNIT_ASSERT(potentials[1] == tArticle->mBuffer[index].mPotentialVector[1]);

    tArticle->recordNodesConvergence(nodesConvergence);
    CPPUNIT_ASSERT(nodesConvergence[0] == tArticle->mBuffer[index].mNodesConvergence[0]);
    CPPUNIT_ASSERT(nodesConvergence[1] == tArticle->mBuffer[index].mNodesConvergence[1]);

    tArticle->recordLinkContributions();
    CPPUNIT_ASSERT(admittance[0] == tArticle->mBuffer[index].mLinksData[0].mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(source[0]     == tArticle->mBuffer[index].mLinksData[0].mSourceVector[0]);

    tArticle->recordLinkResult(0, GunnsBasicLink::DELAY);
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY == tArticle->mBuffer[index].mLinksData[0].mSolutionResult);

    tArticle->recordAllLinksResult(GunnsBasicLink::DELAY);
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY == tArticle->mBuffer[index].mLinksData[0].mSolutionResult);
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY == tArticle->mBuffer[index].mLinksData[1].mSolutionResult);
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY == tArticle->mBuffer[index].mLinksData[2].mSolutionResult);

    tArticle->recordStepResult(1, GunnsMinorStepData::MATH_FAIL);
    CPPUNIT_ASSERT(1                             == tArticle->mNumValidSteps);
    CPPUNIT_ASSERT(2                             == tArticle->mBuffer[index].mMajorStep);
    CPPUNIT_ASSERT(1                             == tArticle->mBuffer[index].mMinorStep);
    CPPUNIT_ASSERT(1                             == tArticle->mBuffer[index].mDecomposition);
    CPPUNIT_ASSERT(GunnsMinorStepData::MATH_FAIL == tArticle->mBuffer[index].mSolutionResult);

    /// - Remove our link memory hacks.
    tLink0.mAdmittanceMatrix = 0;
    tLink1.mAdmittanceMatrix = 0;
    tLink2.mAdmittanceMatrix = 0;
    tLink0.mSourceVector     = 0;
    tLink1.mSourceVector     = 0;
    tLink2.mSourceVector     = 0;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the some wrap-around cases in accessing the buffer that aren't
///           covered in the other tests.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLog::testWraparound()
{
    std::cout << "\n UtGunnsMinorStepLog .... 08: testWraparound ........................";

    tArticle->initialize(tName, tNetworkSize, tNumLinks, tLinks);

    /// @test stepping past the buffer size wraps around to start.
    tArticle->mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_SNAP;
    tArticle->beginMajorStep();
    tArticle->beginMinorStep(1, 1);
    CPPUNIT_ASSERT(0 == tArticle->mHeadIndex);
    CPPUNIT_ASSERT(1 == tArticle->mBuffer[0].mMinorStep);
    tArticle->beginMinorStep(1, 2);
    CPPUNIT_ASSERT(1 == tArticle->mHeadIndex);
    CPPUNIT_ASSERT(2 == tArticle->mBuffer[1].mMinorStep);
    tArticle->beginMinorStep(1, 3);
    CPPUNIT_ASSERT(2 == tArticle->mHeadIndex);
    CPPUNIT_ASSERT(3 == tArticle->mBuffer[2].mMinorStep);
    tArticle->beginMinorStep(1, 4);
    CPPUNIT_ASSERT(0 == tArticle->mHeadIndex);
    CPPUNIT_ASSERT(4 == tArticle->mBuffer[0].mMinorStep);

    /// @test headOffset method with offset going past the buffer size wraps around to the start.
    CPPUNIT_ASSERT(2 == tArticle->headOffset(4).mMinorStep);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the updateAsync method of the GunnsMinorStepLog class.  This
///           creates and deletes file: tArticle_*.csv in the test folder.
///           If the test fails then the file may be left in the folder.  It can be deleted, and
///           should not be checked into the CM repository.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLog::testUpdateAsync()
{
    std::cout << "\n UtGunnsMinorStepLog .... 09: testUpdateAsync .......................";

    tArticle->initialize(tName, tNetworkSize, tNumLinks, tLinks);

    /// - Delete the test file if it exists.
    std::string filename = tPath + tName + "_1.csv";
    remove(filename.c_str());

    /// @test  updateAsync does nothing when output filename is not given.
    tArticle->mFilename = "";
    tArticle->mState    = GunnsMinorStepLog::WRITING_FILE;
    tArticle->updateAsync();
    CPPUNIT_ASSERT(0 == fopen(filename.c_str(), "r"));

    /// - Override the link's [A] and {w} arrays so we don't have to init the links.
    double admittance[4] = {100.0, 0.0, 0.0, 200.0};
    double source[2]     = {12.6, -12.6};
    tLink0.mAdmittanceMatrix = admittance;
    tLink1.mAdmittanceMatrix = admittance;
    tLink2.mAdmittanceMatrix = admittance;
    tLink0.mSourceVector     = source;
    tLink1.mSourceVector     = source;
    tLink2.mSourceVector     = source;

    /// - Record some solver data and set expected log file contents.
    tArticle->mState                  = GunnsMinorStepLog::PAUSED;
    tArticle->mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_AUTO;
    std::ostringstream expectedData;
    expectedData << tArticle->mLogHeader;

    const double potential[5]   = {1.0, 2.0, 3.0, 4.0, 5.0};
    const double convergence[5] = {-0.1, -0.2, -0.3, -0.4, -0.5};
    tArticle->beginMajorStep();
    tArticle->beginMinorStep(1, 1);
    tArticle->recordPotential(potential);
    tArticle->recordNodesConvergence(convergence);
    tArticle->recordLinkContributions();
    tArticle->recordAllLinksResult(GunnsBasicLink::CONFIRM);
    tArticle->recordStepResult(1, GunnsMinorStepData::SUCCESS);
    expectedData << "1, 1, 1, SUCCESS, 1, 2, 3, 4, 5, -0.1, -0.2, -0.3, -0.4, -0.5, "
            << "CONFIRM, 100, 0, 0, 200, 12.6, -12.6, CONFIRM, 100, 0, 0, 200, 12.6, -12.6, "
            << "CONFIRM, 100, 0, 0, 200, 12.6, -12.6" <<std::endl;

    tArticle->beginMinorStep(1, 2);
    tArticle->recordPotential(potential);
    tArticle->recordNodesConvergence(convergence);
    tArticle->recordLinkContributions();
    tArticle->recordAllLinksResult(GunnsBasicLink::REJECT);
    tArticle->recordStepResult(2, GunnsMinorStepData::DECOMP_LIMIT);
    tArticle->endMajorStep();
    expectedData << "1, 2, 2, DECOMP_LIMIT, 1, 2, 3, 4, 5, -0.1, -0.2, -0.3, -0.4, -0.5, "
            << "REJECT, 100, 0, 0, 200, 12.6, -12.6, REJECT, 100, 0, 0, 200, 12.6, -12.6, "
            << "REJECT, 100, 0, 0, 200, 12.6, -12.6" <<std::endl;

    /// - Remove our link memory hacks.
    tLink0.mAdmittanceMatrix = 0;
    tLink1.mAdmittanceMatrix = 0;
    tLink2.mAdmittanceMatrix = 0;
    tLink0.mSourceVector     = 0;
    tLink1.mSourceVector     = 0;
    tLink2.mSourceVector     = 0;

    /// @test  When the filename is set, updateAsync creates the output file, verify its contents.
    tArticle->updateAsync();

    std::ifstream file(filename.c_str(), std::ifstream::in);
    std::string actualData((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
    file.close();

    CPPUNIT_ASSERT_EQUAL(expectedData.str(), actualData);

    /// - Delete the test file when we're done.
    remove(filename.c_str());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the updateFreeze method of the GunnsMinorStepLog class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLog::testUpdateFreeze()
{
    std::cout << "\n UtGunnsMinorStepLog .... 10: testUpdateFreeze ......................";

    /// @test  updateFreeze does nothing if object hasn't been initialized.
    tArticle->updateFreeze();
    CPPUNIT_ASSERT(0 == tArticle->mBuffer.size());

    /// - The initialize function already calls updateFreeze, but we need to assert detailed outputs
    ///   and test other logic paths.
    tArticle->initialize(tName, tNetworkSize, tNumLinks, tLinks);

    /// @test  Initial buffer size.
    CPPUNIT_ASSERT(tLogSteps == tArticle->mBuffer.size());
    CPPUNIT_ASSERT_EQUAL(tLogSteps, tArticle->mSize);

    /// @test  Does nothing if input size = 0.
    tArticle->mInputData.mLogSteps = 0;
    tArticle->updateFreeze();
    CPPUNIT_ASSERT(tLogSteps == tArticle->mBuffer.size());
    CPPUNIT_ASSERT_EQUAL(tLogSteps, tArticle->mSize);

    /// @test  Does nothing if input size = current size
    tArticle->mInputData.mLogSteps = tLogSteps;
    tArticle->updateFreeze();
    CPPUNIT_ASSERT(tLogSteps == tArticle->mBuffer.size());
    CPPUNIT_ASSERT_EQUAL(tLogSteps, tArticle->mSize);

    /// @test  new size and logging start on a resize event.
    const unsigned int newSize = tLogSteps + 5;
    tArticle->mHeadIndex     = 2;
    tArticle->mNumValidSteps = 2;
    tArticle->mIsRecording   = false;
    tArticle->mInputData.mLogSteps = newSize;
    tArticle->updateFreeze();
    CPPUNIT_ASSERT(newSize     == tArticle->mSize);
    CPPUNIT_ASSERT(newSize     == tArticle->mBuffer.size());
    CPPUNIT_ASSERT(newSize - 1 == tArticle->mHeadIndex);
    CPPUNIT_ASSERT(0           == tArticle->mNumValidSteps);
    CPPUNIT_ASSERT(true        == tArticle->mIsRecording);

    std::cout << "... Pass";
}
