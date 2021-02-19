/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsMinorStepLogDumper.o))
***************************************************************************************************/

#include "UtGunnsMinorStepLogDumper.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <istream>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsMinorStepLogDumper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsMinorStepLogDumper::UtGunnsMinorStepLogDumper()
    :
    tArticle(tSolver, tLinks),
    tName("test.article"),
    tConfig(tName),
    tLogMode(GunnsMinorStepLogDumper::STANDBY),
    tLogSteps(0),
    tPath(""),
    tInput(),
    tSolver(),
    tLinks(),
    tNetworkSize(0),
    tNumLinks(0),
    tStepLimit(0),
    tLink0(),
    tLink1(),
    tLink2()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsMinorStepLogDumper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsMinorStepLogDumper::~UtGunnsMinorStepLogDumper()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLogDumper::tearDown()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLogDumper::setUp()
{
    tConfig.mName       = tName;
    tLogMode            = GunnsMinorStepLogDumper::AUTO;
    tLogSteps           = 3;
    tPath               = "RUN_test/";
    tInput.mLogMode     = tLogMode;
    tInput.mLogSteps    = tLogSteps;
    tInput.mPath        = tPath;

    /// - Initialize the test solver's minor step log.
    tNetworkSize        = 5;
    tNumLinks           = 3;
    tStepLimit          = 8;
    tSolver.mMinorStepLog.initialize(tNetworkSize, tNumLinks, tStepLimit);

    /// - Initialize the test link vector.
    tLink0.mName        = "tLink0";
    tLink1.mName        = "tLink1";
    tLink2.mName        = "tLink2";
    tLinks.push_back(&tLink0);
    tLinks.push_back(&tLink1);
    tLinks.push_back(&tLink2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsMinorStepLogDumperConfigData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLogDumper::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsMinorStepLogDumper 01: testConfig ...........................";

    /// - Test nominal config data construction.
    CPPUNIT_ASSERT(tName == tConfig.mName);

    /// - Test default config data construction.
    GunnsMinorStepLogDumperConfigData article(tName);
    CPPUNIT_ASSERT(tName == article.mName);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsMinorStepLogDumperInputData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLogDumper::testInput()
{
    std::cout << "\n UtGunnsMinorStepLogDumper 02: testInput ............................";

    /// - Test nominal input data construction.
    CPPUNIT_ASSERT(tLogMode                         == tInput.mLogMode);
    CPPUNIT_ASSERT(tLogSteps                        == tInput.mLogSteps);
    CPPUNIT_ASSERT(tPath                            == tInput.mPath);

    /// - Test default input data construction.
    GunnsMinorStepLogDumperInputData article;
    CPPUNIT_ASSERT(GunnsMinorStepLogDumper::STANDBY == article.mLogMode);
    CPPUNIT_ASSERT(0                                == article.mLogSteps);
    CPPUNIT_ASSERT("./"                             == article.mPath);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsMinorStepLogDumper class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLogDumper::testDefaultConstruction()
{
    std::cout << "\n UtGunnsMinorStepLogDumper 03: testDefaultConstruction ..............";

    /// @test state data
    CPPUNIT_ASSERT(""                               ==  tArticle.mName);
    CPPUNIT_ASSERT(&tSolver.mMinorStepLog           == &tArticle.mSolverLog);
    CPPUNIT_ASSERT(&tLinks                          == &tArticle.mLinks);
    CPPUNIT_ASSERT(tNumLinks                        ==  tArticle.mLinks.size());
    CPPUNIT_ASSERT(GunnsMinorStepLogDumper::STANDBY ==  tArticle.mLogMode);
    CPPUNIT_ASSERT(0                                ==  tArticle.mDefaultLogSteps);
    CPPUNIT_ASSERT(""                               ==  tArticle.mPath);
    CPPUNIT_ASSERT(0                                ==  tArticle.mLogSteps);
    CPPUNIT_ASSERT(0                                ==  tArticle.mQueueSize);
    CPPUNIT_ASSERT(false                            ==  tArticle.mLocked);
    CPPUNIT_ASSERT(0                                ==  tArticle.mFileMessages.size());
    CPPUNIT_ASSERT(""                               ==  tArticle.mLogTableHeader);
    CPPUNIT_ASSERT(""                               ==  tArticle.mLinkNamesTable);

    /// @test init flag
    CPPUNIT_ASSERT(false                            ==  tArticle.mInitFlag);

    /// @test new/delete for code coverage
    GunnsMinorStepLogDumper* article = new GunnsMinorStepLogDumper(tSolver, tLinks);
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize and validation methods of the GunnsMinorStepLogDumper
///           class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLogDumper::testInitialize()
{
    std::cout << "\n UtGunnsMinorStepLogDumper 04: testInitialize .......................";

    tArticle.initialize(&tConfig, &tInput);

    /// @test nominal initialization.
    CPPUNIT_ASSERT(tName                            ==  tArticle.mName);
    CPPUNIT_ASSERT(&tSolver.mMinorStepLog           == &tArticle.mSolverLog);
    CPPUNIT_ASSERT(&tLinks                          == &tArticle.mLinks);
    CPPUNIT_ASSERT(tNumLinks                        ==  tArticle.mLinks.size());
    CPPUNIT_ASSERT(tLogMode                         ==  tArticle.mLogMode);
    CPPUNIT_ASSERT(tLogSteps                        ==  tArticle.mDefaultLogSteps);
    CPPUNIT_ASSERT(tPath                            ==  tArticle.mPath);
    CPPUNIT_ASSERT(tLogSteps                        ==  tArticle.mLogSteps);
    CPPUNIT_ASSERT(0                                ==  tArticle.mQueueSize);
    CPPUNIT_ASSERT(false                            ==  tArticle.mLocked);
    CPPUNIT_ASSERT(0                                ==  tArticle.mFileMessages.size());
    CPPUNIT_ASSERT(""                               ==  tArticle.mLogTableHeader);

    std::ostringstream expectedTable;
    expectedTable  << std::endl << "Link # | Name"   << std::endl
                                << "     0 | tLink0" << std::endl
                                << "     1 | tLink1" << std::endl
                                << "     2 | tLink2" << std::endl;
    CPPUNIT_ASSERT_EQUAL(expectedTable.str(),           tArticle.mLinkNamesTable);

    /// @test init flag
    CPPUNIT_ASSERT(true                             ==  tArticle.mInitFlag);

    /// @test nominal initialization of the solver minor step log.
    CPPUNIT_ASSERT(1                                ==  tSolver.mMinorStepLog.mNumLinkWords);
    CPPUNIT_ASSERT(1                                ==  tSolver.mMinorStepLog.mNumNodeWords);

    /// - Test exception thrown from missing name.
    tConfig.mName = "";
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tConfig.mName = tName;

    /// - Test exception thrown from null config data.
    const GunnsMinorStepLogDumperConfigData* nullConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(nullConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown from null input data.
    const GunnsMinorStepLogDumperInputData* nullInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, nullInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown on bad config data pointer type.
    BadGunnsMinorStepLogDumperConfigData* badConfig = new BadGunnsMinorStepLogDumperConfigData(tName);
    CPPUNIT_ASSERT_THROW(tArticle.initialize(badConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badConfig;

    /// - Test exception thrown on bad input data pointer type.
    BadGunnsMinorStepLogDumperInputData* badInput = new BadGunnsMinorStepLogDumperInputData();
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, badInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badInput;

    /// - Test exception thrown on bad initial log mode.
    tInput.mLogMode = GunnsMinorStepLogDumper::LOGGING;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tInput.mLogMode = tLogMode;

    /// - Test exception thrown on bad initial/default log steps.
    tInput.mLogSteps = -1;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tInput.mLogSteps = tLogSteps;

    /// - Test exceptions thrown on empty or bad path.
    tInput.mPath = "";
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tInput.mPath = tPath;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsMinorStepLogDumper class.
///           That method doesn't actually do anything so there are no asserts, but this is done for
///           complete code coverage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLogDumper::testPreSolver()
{
    std::cout << "\n UtGunnsMinorStepLogDumper 05: testPreSolver ........................";

    tArticle.initialize(&tConfig, &tInput);
    tArticle.stepPreSolver(0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsMinorStepLogDumper class, when
///           a dump is initiated automatically.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLogDumper::testPostSolverAuto()
{
    std::cout << "\n UtGunnsMinorStepLogDumper 06: testPostSolverAuto ...................";

    tArticle.initialize(&tConfig, &tInput);

    /// - First test a case where the logger is in AUTO but the solver hasn't failed to converge.
    tSolver.mMinorStepLog.mMajorStep = 4;
    tSolver.mMinorStepLog.mMinorStep = 1;
    tSolver.mMinorStepLog.mResult    = GunnsMinorStepLog::SUCCESS;

    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(GunnsMinorStepLogDumper::AUTO,    tArticle.mLogMode);
    CPPUNIT_ASSERT_EQUAL(tLogSteps,                        tArticle.mLogSteps);
    CPPUNIT_ASSERT_EQUAL(false,                            tArticle.mLocked);
    CPPUNIT_ASSERT_EQUAL(0,                                tArticle.mQueueSize);
    CPPUNIT_ASSERT_EQUAL(0,                            int(tArticle.mFileMessages.size()));

    /// - Verify the log table header gets initialized on the first pass.
    std::ostringstream expectedHeader;
    expectedHeader << std::endl << "Minor | Nodes:   | Links:"   << std::endl
                                << "Step: |  31----0 |  31----0" << std::endl;
    CPPUNIT_ASSERT_EQUAL(expectedHeader.str(),             tArticle.mLogTableHeader);

    /// - Set up the solver's minor step log to trigger a dump in AUTO mode with some node & link
    ///   bits set.
    tSolver.mMinorStepLog.mMajorStep = 5;
    tSolver.mMinorStepLog.mMinorStep = 9;
    tSolver.mMinorStepLog.mResult    = GunnsMinorStepLog::MINOR;
    tSolver.mMinorStepLog.mNodeBits[0][0].set(0);
    tSolver.mMinorStepLog.mNodeBits[1][0].set(1);
    tSolver.mMinorStepLog.mNodeBits[2][0].set(2);
    tSolver.mMinorStepLog.mNodeBits[3][0].set(3);
    tSolver.mMinorStepLog.mNodeBits[4][0].set(4);
    tSolver.mMinorStepLog.mNodeBits[5][0].set(5);
    tSolver.mMinorStepLog.mNodeBits[6][0].set(6);
    tSolver.mMinorStepLog.mNodeBits[7][0].set(7);
    tSolver.mMinorStepLog.mLinkBits[0][0].set(31);
    tSolver.mMinorStepLog.mLinkBits[1][0].set(30);
    tSolver.mMinorStepLog.mLinkBits[2][0].set(29);
    tSolver.mMinorStepLog.mLinkBits[3][0].set(28);
    tSolver.mMinorStepLog.mLinkBits[4][0].set(27);
    tSolver.mMinorStepLog.mLinkBits[5][0].set(26);
    tSolver.mMinorStepLog.mLinkBits[6][0].set(25);
    tSolver.mMinorStepLog.mLinkBits[7][0].set(24);

    std::ostringstream expectedFileName;
    expectedFileName << tPath << "GUNNS_minor_step_log_test.article_5.txt";

    std::ostringstream expectedData;
    expectedData << std::endl << "GUNNS Minor Step Log for Solver: test.article on Major Step: 5"
                              << ", initiated automatically by solver.";
    expectedData << std::endl << "Minor | Nodes:   | Links:"   << std::endl
                              << "Step: |  31----0 |  31----0" << std::endl;
    expectedData              << "    1 | 00000001 | 80000000" << std::endl
                              << "    2 | 00000002 | 40000000" << std::endl
                              << "    3 | 00000004 | 20000000" << std::endl
                              << "    4 | 00000008 | 10000000" << std::endl
                              << "    5 | 00000010 | 08000000" << std::endl
                              << "    6 | 00000020 | 04000000" << std::endl
                              << "    7 | 00000040 | 02000000" << std::endl
                              << "    8 | 00000080 | 01000000" << std::endl;
    expectedData              << "    9 | ***Solver failed to converge*** and gave up when minor step limit was exceeded." << std::endl;

    /// - Test the log dumper creation of a log with the first message in it, and remaining locked
    ///   because we haven't reached the target number of major steps yet.
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(GunnsMinorStepLogDumper::LOGGING, tArticle.mLogMode);
    CPPUNIT_ASSERT_EQUAL(2,                                tArticle.mLogSteps);
    CPPUNIT_ASSERT_EQUAL(true,                             tArticle.mLocked);
    CPPUNIT_ASSERT_EQUAL(1,                                tArticle.mQueueSize);
    CPPUNIT_ASSERT_EQUAL(1,                            int(tArticle.mFileMessages.size()));
    CPPUNIT_ASSERT_EQUAL(expectedData.str(),               tArticle.mFileMessages.back().mData);
    CPPUNIT_ASSERT_EQUAL(expectedFileName.str(),           tArticle.mFileMessages.back().mFileName);

    /// - Set up the solver's minor step log for the 2nd major step.
    tSolver.mMinorStepLog.mMajorStep++;
    tSolver.mMinorStepLog.mMinorStep = 3;
    tSolver.mMinorStepLog.mResult    = GunnsMinorStepLog::DECOMP;
    tSolver.mMinorStepLog.mNodeBits[0][0].set();
    tSolver.mMinorStepLog.mNodeBits[1][0].set();

    expectedData << std::endl << "GUNNS Minor Step Log for Solver: test.article on Major Step: 6"
                              << ", continuing log...";
    expectedData << std::endl << "Minor | Nodes:   | Links:"   << std::endl
                              << "Step: |  31----0 |  31----0" << std::endl;
    expectedData              << "    1 | ffffffff | 80000000" << std::endl
                              << "    2 | ffffffff | 40000000" << std::endl;
    expectedData              << "    3 | ***Solver failed to converge*** and gave up when decomposition limit was exceeded." << std::endl;

    /// - Test the log dumper continuation of the current log because we haven't reached the target
    ///   number of major steps yet.
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(GunnsMinorStepLogDumper::LOGGING, tArticle.mLogMode);
    CPPUNIT_ASSERT_EQUAL(1,                                tArticle.mLogSteps);
    CPPUNIT_ASSERT_EQUAL(true,                             tArticle.mLocked);
    CPPUNIT_ASSERT_EQUAL(1,                                tArticle.mQueueSize);
    CPPUNIT_ASSERT_EQUAL(1,                            int(tArticle.mFileMessages.size()));
    CPPUNIT_ASSERT_EQUAL(expectedData.str(),               tArticle.mFileMessages.back().mData);
    CPPUNIT_ASSERT_EQUAL(expectedFileName.str(),           tArticle.mFileMessages.back().mFileName);

    /// - Set up the solver's minor step log for the 3rd major step.
    tSolver.mMinorStepLog.mMajorStep++;
    tSolver.mMinorStepLog.mMinorStep = 1;
    tSolver.mMinorStepLog.mResult    = GunnsMinorStepLog::SUCCESS;

    expectedData << std::endl << "GUNNS Minor Step Log for Solver: test.article on Major Step: 7"
                              << ", continuing log...";
    expectedData << std::endl << "Minor | Nodes:   | Links:"   << std::endl
                              << "Step: |  31----0 |  31----0" << std::endl;
    expectedData              << "    1 | Solver successfully finished." << std::endl;

    expectedData << std::endl << "End of log: " << tPath <<  "GUNNS_minor_step_log_test.article_5.txt" << std::endl;

    expectedData << std::endl << "Link # | Name"   << std::endl
                              << "     0 | tLink0" << std::endl
                              << "     1 | tLink1" << std::endl
                              << "     2 | tLink2" << std::endl;

    /// - Test the log dumper finishes the current log.
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(GunnsMinorStepLogDumper::STANDBY, tArticle.mLogMode);
    CPPUNIT_ASSERT_EQUAL(tLogSteps,                        tArticle.mLogSteps);
    CPPUNIT_ASSERT_EQUAL(false,                            tArticle.mLocked);
    CPPUNIT_ASSERT_EQUAL(1,                                tArticle.mQueueSize);
    CPPUNIT_ASSERT_EQUAL(1,                            int(tArticle.mFileMessages.size()));
    CPPUNIT_ASSERT_EQUAL(expectedData.str(),               tArticle.mFileMessages.back().mData);
    CPPUNIT_ASSERT_EQUAL(expectedFileName.str(),           tArticle.mFileMessages.back().mFileName);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsMinorStepLogDumper class, when
///           a dump is initiated by the snap command.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLogDumper::testPostSolverSnap()
{
    std::cout << "\n UtGunnsMinorStepLogDumper 07: testPostSolverSnap ...................";

    tArticle.initialize(&tConfig, &tInput);

    /// - Push an old message onto the file message queue for this test.
    tArticle.mFileMessages.push_back(GunnsMinorStepLogMessage());

    /// - Set up the solver's minor step log to trigger a 1-step dump by the snap command.
    tArticle.mLogSteps               = 1;
    tArticle.mLogMode                = GunnsMinorStepLogDumper::SNAP;
    tSolver.mMinorStepLog.mMajorStep = 0;
    tSolver.mMinorStepLog.mMinorStep = 1;
    tSolver.mMinorStepLog.mResult    = GunnsMinorStepLog::SUCCESS;

    std::ostringstream expectedFileName;
    expectedFileName << tPath << "GUNNS_minor_step_log_test.article_0.txt";

    std::ostringstream expectedData;
    expectedData << std::endl << "GUNNS Minor Step Log for Solver: test.article on Major Step: 0"
                              << ", initiated by snap command.";
    expectedData << std::endl << "Minor | Nodes:   | Links:"   << std::endl
                              << "Step: |  31----0 |  31----0" << std::endl;
    expectedData              << "    1 | Solver successfully finished." << std::endl;

    expectedData << std::endl << "End of log: " << tPath <<  "GUNNS_minor_step_log_test.article_0.txt" << std::endl;

    expectedData << std::endl << "Link # | Name"   << std::endl
                              << "     0 | tLink0" << std::endl
                              << "     1 | tLink1" << std::endl
                              << "     2 | tLink2" << std::endl;

    /// - Test the log dumper finishes the current log.
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(GunnsMinorStepLogDumper::STANDBY, tArticle.mLogMode);
    CPPUNIT_ASSERT_EQUAL(tLogSteps,                        tArticle.mLogSteps);
    CPPUNIT_ASSERT_EQUAL(false,                            tArticle.mLocked);
    CPPUNIT_ASSERT_EQUAL(2,                                tArticle.mQueueSize);
    CPPUNIT_ASSERT_EQUAL(2,                            int(tArticle.mFileMessages.size()));
    CPPUNIT_ASSERT_EQUAL(expectedData.str(),               tArticle.mFileMessages.back().mData);
    CPPUNIT_ASSERT_EQUAL(expectedFileName.str(),           tArticle.mFileMessages.back().mFileName);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsMinorStepLogDumper class, when
///           nothing is happening because it is in standby mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLogDumper::testPostSolverStandby()
{
    std::cout << "\n UtGunnsMinorStepLogDumper 08: testPostSolverStandby ................";

    tArticle.initialize(&tConfig, &tInput);

    /// - Set up the solver's minor step log to remain on standby and verify nothing happens.
    tArticle.mLogSteps               = 1;
    tArticle.mLogMode                = GunnsMinorStepLogDumper::STANDBY;
    tSolver.mMinorStepLog.mMajorStep = 5;
    tSolver.mMinorStepLog.mMinorStep = 9;
    tSolver.mMinorStepLog.mResult    = GunnsMinorStepLog::MINOR;

    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(GunnsMinorStepLogDumper::STANDBY, tArticle.mLogMode);
    CPPUNIT_ASSERT_EQUAL(1,                                tArticle.mLogSteps);
    CPPUNIT_ASSERT_EQUAL(false,                            tArticle.mLocked);
    CPPUNIT_ASSERT_EQUAL(0,                                tArticle.mQueueSize);
    CPPUNIT_ASSERT_EQUAL(0,                            int(tArticle.mFileMessages.size()));

    /// - Test that having # log steps set to zero forces the logger to standby.
    tArticle.mLogSteps               = 0;
    tArticle.mLogMode                = GunnsMinorStepLogDumper::SNAP;

    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(GunnsMinorStepLogDumper::STANDBY, tArticle.mLogMode);
    CPPUNIT_ASSERT_EQUAL(tLogSteps,                        tArticle.mLogSteps);
    CPPUNIT_ASSERT_EQUAL(false,                            tArticle.mLocked);
    CPPUNIT_ASSERT_EQUAL(0,                                tArticle.mQueueSize);
    CPPUNIT_ASSERT_EQUAL(0,                            int(tArticle.mFileMessages.size()));

    /// - Test that setting LOGGING mode directly without a log already having started returns the
    ///   logger to standby.
    tArticle.mLogSteps               = 1;
    tArticle.mLogMode                = GunnsMinorStepLogDumper::LOGGING;

    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(GunnsMinorStepLogDumper::STANDBY, tArticle.mLogMode);
    CPPUNIT_ASSERT_EQUAL(tLogSteps,                        tArticle.mLogSteps);
    CPPUNIT_ASSERT_EQUAL(false,                            tArticle.mLocked);
    CPPUNIT_ASSERT_EQUAL(0,                                tArticle.mQueueSize);
    CPPUNIT_ASSERT_EQUAL(0,                            int(tArticle.mFileMessages.size()));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the updateAsync method of the GunnsMinorStepLogDumper class.  This
///           creates and deletes file: GUNNS_minor_step_log_test.article_0.txt in the test folder.
///           If the test fails then the file may be left in the folder.  It can be deleted, and
///           should not be checked into the CM repository.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsMinorStepLogDumper::testUpdateAsync()
{
    std::cout << "\n UtGunnsMinorStepLogDumper 09: testUpdateAsync ......................";

    /// - Delete the test file if it exists.
    remove("GUNNS_minor_step_log_test.article_0.txt");

    /// - Verify the updateAsync method does nothing when the queue is not locked and has no
    ///   messages in it.
    tArticle.updateAsync();

    CPPUNIT_ASSERT_EQUAL(0,                            int(tArticle.mFileMessages.size()));
    CPPUNIT_ASSERT(0 == fopen("./GUNNS_minor_step_log_test.article_0.txt", "r"));

    /// - Set up the solver's minor step log to trigger a 2-step dump by the snap command.
    tArticle.initialize(&tConfig, &tInput);
    tArticle.mLogSteps               = 2;
    tArticle.mLogMode                = GunnsMinorStepLogDumper::SNAP;
    tArticle.mPath                   = "./";
    tSolver.mMinorStepLog.mMajorStep = 0;
    tSolver.mMinorStepLog.mMinorStep = 1;
    tSolver.mMinorStepLog.mResult    = GunnsMinorStepLog::SUCCESS;

    std::ostringstream expectedData;
    expectedData << std::endl << "GUNNS Minor Step Log for Solver: test.article on Major Step: 0"
                              << ", initiated by snap command.";
    expectedData << std::endl << "Minor | Nodes:   | Links:"   << std::endl
                              << "Step: |  31----0 |  31----0" << std::endl;
    expectedData              << "    1 | Solver successfully finished." << std::endl;

    /// - Initiate the log message but leave the message queue locked because the log isn't finished.
    tArticle.stepPostSolver(0.0);

    /// - Verify the updateAsync method refrains from accessing the message queue when it's locked,
    ///   and doesn't create the file yet.
    tArticle.updateAsync();

    CPPUNIT_ASSERT_EQUAL(true,                             tArticle.mLocked);
    CPPUNIT_ASSERT_EQUAL(1,                                tArticle.mQueueSize);
    CPPUNIT_ASSERT_EQUAL(1,                            int(tArticle.mFileMessages.size()));
    CPPUNIT_ASSERT(0 == fopen("./GUNNS_minor_step_log_test.article_0.txt", "r"));

    /// - Finish creating the log message.
    tSolver.mMinorStepLog.mMajorStep = 1;

    expectedData << std::endl << "GUNNS Minor Step Log for Solver: test.article on Major Step: 1"
                              << ", continuing log...";
    expectedData << std::endl << "Minor | Nodes:   | Links:"   << std::endl
                              << "Step: |  31----0 |  31----0" << std::endl;
    expectedData              << "    1 | Solver successfully finished." << std::endl;

    expectedData << std::endl << "End of log: ./GUNNS_minor_step_log_test.article_0.txt" << std::endl;

    expectedData << std::endl << "Link # | Name"   << std::endl
                              << "     0 | tLink0" << std::endl
                              << "     1 | tLink1" << std::endl
                              << "     2 | tLink2" << std::endl;

    tArticle.stepPostSolver(0.0);

    /// - Once the message has been finished, verify the updateAsync message creates the output file
    ///   and verify the file contents.
    tArticle.updateAsync();

    CPPUNIT_ASSERT_EQUAL(false,                            tArticle.mLocked);
    CPPUNIT_ASSERT_EQUAL(1,                                tArticle.mQueueSize);
    CPPUNIT_ASSERT_EQUAL(0,                            int(tArticle.mFileMessages.size()));

    std::ifstream file("./GUNNS_minor_step_log_test.article_0.txt", std::ifstream::in);
    std::string actualData((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
    file.close();

    CPPUNIT_ASSERT_EQUAL(expectedData.str(),               actualData);

    /// - Delete the test file when we're done.
    remove("GUNNS_minor_step_log_test.article_0.txt");

    std::cout << "... Pass";
}
