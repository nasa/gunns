///@copyright Copyright 2023 United States Government as represented by the Administrator of the
///           National Aeronautics and Space Administration.  All Rights Reserved.

#include "strings/UtResult.hh"
#include "core/optimization/GunnsOptimMonteCarloTypes.hh"
#include <stdexcept>

#include "UtGunnsOptimGradientDescent.hh"
#include "strings/Strings.hh"

/// @details  Test identification number.
int UtGunnsOptimGradientDescent::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS gradient descent optimization class unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsOptimGradientDescent::UtGunnsOptimGradientDescent()
    :
    CppUnit::TestFixture(),
    tName(""),
    tArticle(0),
    tConfigData(0),
    tInitialState(0),
    tMcInput1(0.0),
    tMcInput2(0.0),
    tMcInputs()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS gradient descent optimization class unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsOptimGradientDescent::~UtGunnsOptimGradientDescent()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimGradientDescent::setUp()
{
    /// - Default construct the nominal test article.
    tName    = "tArticle";
    tArticle = new FriendlyGunnsOptimGradientDescent();

    /// - Set verbosity for code coverage.  This will spam the verbose outputs to the console, but
    ///   we aren't actually testing that output for correctness.
    tArticle->setVerbosityLevel(1);

    tInitialState = new double[2];
    tInitialState[0] = 0.1;
    tInitialState[1] = 0.2;

    /// - Set up nominal config data.
    tConfigData = new GunnsOptimGradientDescentConfigData;
    tConfigData->mNumVars         = 2;
    tConfigData->mMaxEpoch        = 5;
    tConfigData->mPropagationGain = 0.5;
    tConfigData->mInitialState    = tInitialState;

    /// - Set up the MC input variables.
    tMcInputs.push_back(GunnsOptimMonteCarloInput());
    tMcInputs.push_back(GunnsOptimMonteCarloInput());

    tMcInputs.at(0).mName = "tMcInput1";
    tMcInputs.at(0).mAddress = &tMcInput1;
    tMcInputs.at(0).mMinimum = 0.0;
    tMcInputs.at(0).mMaximum = 20.0;
    tMcInputs.at(0).mConstraints.clear();

    tMcInputs.at(1).mName = "tMcInput2";
    tMcInputs.at(1).mAddress = &tMcInput2;
    tMcInputs.at(1).mMinimum = 5.0;
    tMcInputs.at(1).mMaximum = 10.0;
    tMcInputs.at(1).mConstraints.clear();

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimGradientDescent::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tConfigData;
    delete [] tInitialState;
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the GUNNS gradient descent optimization descent state structure.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimGradientDescent::testDescentState()
{
    UT_RESULT_FIRST;

    /// @test default constructor.
    GunnsOptimGradientDescentState defaultState;
    CPPUNIT_ASSERT(0      == defaultState.mState.size());
    CPPUNIT_ASSERT(0.0    == defaultState.mDeltaState);
    CPPUNIT_ASSERT(0.0    == defaultState.mCost);
    CPPUNIT_ASSERT(0.0    == defaultState.mDeltaCost);
    CPPUNIT_ASSERT(0.0    == defaultState.mCostGradient);
    CPPUNIT_ASSERT(-99.99 == defaultState.mRunId);

    /// @test implicit copy constructor and custom assignment operator.
    defaultState.mState.push_back(1.0);
    defaultState.mDeltaState   = 2.0;
    defaultState.mCost         = 3.0;
    defaultState.mDeltaCost    = 4.0;
    defaultState.mCostGradient = 5.0;
    defaultState.mRunId        = 6.0;
    GunnsOptimGradientDescentState copyState(defaultState);
    GunnsOptimGradientDescentState assignState;
    assignState = copyState;
    CPPUNIT_ASSERT(1   == assignState.mState.size());
    CPPUNIT_ASSERT(1.0 == assignState.mState.at(0));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS gradient descent optimization configuration data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimGradientDescent::testConfigData()
{
    UT_RESULT;

    /// @test default constructor.
    GunnsOptimGradientDescentConfigData defaultConfig;
    CPPUNIT_ASSERT(0   == defaultConfig.mNumVars);
    CPPUNIT_ASSERT(0   == defaultConfig.mMaxEpoch);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mPropagationGain);
    CPPUNIT_ASSERT(0   == defaultConfig.mInitialState);

    /// @test assignment operator.
    GunnsOptimGradientDescentConfigData assignConfig;
    assignConfig = *tConfigData;

    CPPUNIT_ASSERT(2                == assignConfig.mNumVars);
    CPPUNIT_ASSERT(5                == assignConfig.mMaxEpoch);
    CPPUNIT_ASSERT(0.5              == assignConfig.mPropagationGain);
    CPPUNIT_ASSERT(tInitialState    == assignConfig.mInitialState);
    CPPUNIT_ASSERT(tInitialState    == assignConfig.mInitialState);
    CPPUNIT_ASSERT(tInitialState[0] == assignConfig.mInitialState[0]);
    CPPUNIT_ASSERT(tInitialState[1] == assignConfig.mInitialState[1]);

    /// @test throw on bad config data type.
    BadGunnsOptimGradientDescentConfig badConfig;
    CPPUNIT_ASSERT_THROW(tArticle->setConfigData(&badConfig), std::runtime_error);
    CPPUNIT_ASSERT(0 == tArticle->mConfigData.mNumVars);

    /// @test add configuration data function.
    CPPUNIT_ASSERT_NO_THROW(tArticle->setConfigData(tConfigData));
    CPPUNIT_ASSERT(2 == tArticle->mConfigData.mNumVars);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS gradient descent optimization class default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimGradientDescent::testDefaultConstruction()
{
    UT_RESULT;

    /// @test Default construction values.
    FriendlyGunnsOptimGradientDescent* article = new FriendlyGunnsOptimGradientDescent();

    CPPUNIT_ASSERT(0                           == article->mConfigData.mNumVars);
    CPPUNIT_ASSERT(0                           == article->mGradients.size());
    CPPUNIT_ASSERT(0                           == article->mActiveState);
    CPPUNIT_ASSERT(0                           == article->mState);
    CPPUNIT_ASSERT("GunnsOptimGradientDescent" == article->mName);
    CPPUNIT_ASSERT(0                           == article->mInStatesMaster);

    delete article;

    /// @test new/delete for code coverage.
    GunnsOptimGradientDescent* article2 = new GunnsOptimGradientDescent();
    delete article2;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS gradient descent optimization class initialize method with nominal
///           initial data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimGradientDescent::testInitialize()
{
    UT_RESULT;

    /// @test initialize outputs.
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(&tMcInputs));

    CPPUNIT_ASSERT(&tMcInputs == tArticle->mInStatesMaster);
    CPPUNIT_ASSERT(4 == tArticle->mGradients.size());
    CPPUNIT_ASSERT(2 == tArticle->mGradients.at(0).mState.size());
    CPPUNIT_ASSERT(2 == tArticle->mGradients.at(3).mState.size());
    CPPUNIT_ASSERT(tArticle->mState);
    CPPUNIT_ASSERT(2 == tArticle->mState->mState.size());
    CPPUNIT_ASSERT(tArticle->mActiveState   == &tArticle->mGradients.at(0));
    CPPUNIT_ASSERT(tInitialState[0]         == tArticle->mState->mState.at(0));
    CPPUNIT_ASSERT(tMcInputs.at(1).mMinimum == tArticle->mState->mState.at(1));
    CPPUNIT_ASSERT(1   == tArticle->mEpoch);
    CPPUNIT_ASSERT(-1  == tArticle->mGlobalRunCounter);
    CPPUNIT_ASSERT(-1  == tArticle->mRunCounter);

    {
        std::string pathFile = "grad_cost_history.csv";
        std::ifstream file (pathFile.c_str(), (std::ifstream::in));
        CPPUNIT_ASSERT(file);
        std::stringstream fString;
        fString << file.rdbuf();
        CPPUNIT_ASSERT("Epoch,Global_Cost \n" == fString.str());
        file.close();
    } {
        std::string pathFile = "grad_gradients_history.csv";
        std::ifstream file (pathFile.c_str(), (std::ifstream::in));
        CPPUNIT_ASSERT(file);
        std::stringstream fString;
        fString << file.rdbuf();
        CPPUNIT_ASSERT("Epoch,Index,Direction,State,DeltaState,Cost,DeltaCost,CostGradient\n" == fString.str());
        file.close();
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS gradient descent optimization class initialization method validation
///           of initial configuration and error handling.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimGradientDescent::testInitErrors()
{
    UT_RESULT;

    //TODO also in PSO test

    /// @test throw on missing MC variables description.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(0), std::range_error);

    /// @test throw on empty MC variables description.
    std::vector<GunnsOptimMonteCarloInput> testMc;
    testMc.clear();
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&testMc), std::range_error);

    /// @test throw on MC variable max less than min range.
    GunnsOptimMonteCarloInput mc1;
    mc1.mMaximum = 0.0;
    mc1.mMinimum = 0.01;
    testMc.push_back(mc1);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&testMc), std::range_error);

    /// @test throw on MC variable has missing address.
    mc1.mMaximum = 1.0;
    mc1.mAddress = 0;
    testMc.clear();
    testMc.push_back(mc1);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&testMc), std::range_error);

    /// @test throw on MC variable has empty name.
    double x = 1.0;
    mc1.mAddress = &x;
    mc1.mName    = "";
    testMc.clear();
    testMc.push_back(mc1);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&testMc), std::range_error);

    /// @test throw on bad number of variables.
    tConfigData->mNumVars = 0;
    tArticle->setConfigData(tConfigData);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::range_error);
    tConfigData->mNumVars = 2;

    /// @test throw on bad number of epochs.
    tConfigData->mMaxEpoch = 0;
    tArticle->setConfigData(tConfigData);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::range_error);
    tConfigData->mMaxEpoch = 5;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS gradient descent optimization class update and propagateSwarm methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimGradientDescent::testUpdate()
{
    UT_RESULT;

    /// - Initialize with nominal config.
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    tArticle->initialize(&tMcInputs);

    /// @test update method with initial global state (global run counter = 0)
    tArticle->update();
    CPPUNIT_ASSERT(1 == tArticle->mEpoch);
    CPPUNIT_ASSERT(0 == tArticle->mGlobalRunCounter);
    CPPUNIT_ASSERT(0 == tArticle->mRunCounter);
    CPPUNIT_ASSERT(tArticle->mState == tArticle->mActiveState);
    CPPUNIT_ASSERT(0 == tArticle->mActiveState->mRunId);

    /// - Store current states for comparison
    double stateVars[2];
    stateVars[0] = MsMath::limitRange(tMcInputs.at(0).mMinimum, tInitialState[0], tMcInputs.at(0).mMaximum);
    stateVars[1] = MsMath::limitRange(tMcInputs.at(1).mMinimum, tInitialState[1], tMcInputs.at(1).mMaximum);

    /// @test update method computing the initial gradients (global run counter = 1), tests setGradientStates()
    double expectedStateVarInc0[2];
    double expectedStateVarDec0[2];
    double expectedStateVarInc1[2];
    double expectedStateVarDec1[2];
    expectedStateVarInc0[0] = MsMath::limitRange(tMcInputs.at(0).mMinimum, 1.001 * stateVars[0], tMcInputs.at(0).mMaximum);
    expectedStateVarInc0[1] = MsMath::limitRange(tMcInputs.at(1).mMinimum, 1.0   * stateVars[1], tMcInputs.at(1).mMaximum);
    expectedStateVarInc1[0] = MsMath::limitRange(tMcInputs.at(0).mMinimum, 1.0   * stateVars[0], tMcInputs.at(0).mMaximum);
    expectedStateVarInc1[1] = MsMath::limitRange(tMcInputs.at(1).mMinimum, 1.001 * stateVars[1], tMcInputs.at(1).mMaximum);
    expectedStateVarDec0[0] = MsMath::limitRange(tMcInputs.at(0).mMinimum, 0.999 * stateVars[0], tMcInputs.at(0).mMaximum);
    expectedStateVarDec0[1] = MsMath::limitRange(tMcInputs.at(1).mMinimum, 1.0   * stateVars[1], tMcInputs.at(1).mMaximum);
    expectedStateVarDec1[0] = MsMath::limitRange(tMcInputs.at(0).mMinimum, 1.0   * stateVars[0], tMcInputs.at(0).mMaximum);
    expectedStateVarDec1[1] = MsMath::limitRange(tMcInputs.at(1).mMinimum, 0.999 * stateVars[1], tMcInputs.at(1).mMaximum);
    double expectedDelStateInc0;
    double expectedDelStateDec0;
    double expectedDelStateInc1;
    double expectedDelStateDec1;
    expectedDelStateInc0 = expectedStateVarInc0[0] - stateVars[0];
    expectedDelStateInc1 = expectedStateVarInc1[1] - stateVars[1];
    expectedDelStateDec0 = expectedStateVarDec0[0] - stateVars[0];
    expectedDelStateDec1 = expectedStateVarDec1[1] - stateVars[1];

    tArticle->update();
    CPPUNIT_ASSERT(1 == tArticle->mEpoch);
    CPPUNIT_ASSERT(1 == tArticle->mGlobalRunCounter);
    CPPUNIT_ASSERT(1 == tArticle->mRunCounter);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStateVarInc0[0], tArticle->mGradients.at(0).mState.at(0), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStateVarInc0[1], tArticle->mGradients.at(0).mState.at(1), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStateVarInc1[0], tArticle->mGradients.at(2).mState.at(0), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStateVarInc1[1], tArticle->mGradients.at(2).mState.at(1), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStateVarDec0[0], tArticle->mGradients.at(1).mState.at(0), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStateVarDec0[1], tArticle->mGradients.at(1).mState.at(1), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStateVarDec1[0], tArticle->mGradients.at(3).mState.at(0), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStateVarDec1[1], tArticle->mGradients.at(3).mState.at(1), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDelStateInc0, tArticle->mGradients.at(0).mDeltaState, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDelStateInc1, tArticle->mGradients.at(2).mDeltaState, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDelStateDec0, tArticle->mGradients.at(1).mDeltaState, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDelStateDec1, tArticle->mGradients.at(3).mDeltaState, FLT_EPSILON);
    CPPUNIT_ASSERT(&tArticle->mGradients.at(0) == tArticle->mActiveState);
    CPPUNIT_ASSERT(1 == tArticle->mActiveState->mRunId);

    // @test update method propagating the gradient states (run counter = 0), tests propagateState()
    tArticle->mRunCounter       = 4; // tArticle->mGradients.size()
    tArticle->mGlobalRunCounter = 4;
    tArticle->mState->mCost     = 10.0;
    tArticle->mGradients.at(0).mDeltaCost = -0.001;
    tArticle->mGradients.at(1).mDeltaCost =  0.0;
    tArticle->mGradients.at(2).mDeltaCost = -0.002;
    tArticle->mGradients.at(3).mDeltaCost = -0.003;
    tArticle->mGradients.at(0).mCostGradient = -10.0;
    tArticle->mGradients.at(1).mCostGradient =  0.0;
    tArticle->mGradients.at(2).mCostGradient =  0.0;
    tArticle->mGradients.at(3).mCostGradient = -20.0;
    const double expectedDstate0 = 10.0 / 10.0;
    const double expectedDstate1 = 10.0 / 20.0;
    const double expectedState0  = tInitialState[0] + expectedDstate0 * tConfigData->mPropagationGain / 2;
    const double expectedState1  = 5.0 + expectedDstate1 * tConfigData->mPropagationGain / 2;
    tArticle->update();
    CPPUNIT_ASSERT(2 == tArticle->mEpoch);
    CPPUNIT_ASSERT(5 == tArticle->mGlobalRunCounter);
    CPPUNIT_ASSERT(0 == tArticle->mRunCounter);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedState0, tArticle->mState->mState.at(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedState1, tArticle->mState->mState.at(1), DBL_EPSILON);
    CPPUNIT_ASSERT(tArticle->mState == tArticle->mActiveState);
    CPPUNIT_ASSERT(5 == tArticle->mActiveState->mRunId);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS gradient descent optimization class update method in the special case
///           state value = 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimGradientDescent::testUpdateZeroState()
{
    UT_RESULT;

    /// @test setGradientStates handling special case of state = 0

    /// - Initialize with nominal config.
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    tArticle->initialize(&tMcInputs);

    /// - initial update call
    tArticle->update();
    CPPUNIT_ASSERT(1 == tArticle->mEpoch);
    CPPUNIT_ASSERT(0 == tArticle->mGlobalRunCounter);
    CPPUNIT_ASSERT(0 == tArticle->mRunCounter);
    CPPUNIT_ASSERT(tArticle->mState == tArticle->mActiveState);
    CPPUNIT_ASSERT(0 == tArticle->mActiveState->mRunId);

    /// - Set zero current state
    tArticle->mState->mState.at(0) = 0.0;

    /// @test update method computing the initial gradients (global run counter = 1), tests
    ///       setGradientStates() for special case of states = 0
    const double stateRange0 = tMcInputs.at(0).mMaximum - tMcInputs.at(0).mMinimum;
    double expectedDelStateInc0 = MsMath::limitRange(tMcInputs.at(0).mMinimum,  0.001 * stateRange0, tMcInputs.at(0).mMaximum);
    double expectedDelStateDec0 = MsMath::limitRange(tMcInputs.at(0).mMinimum, -0.001 * stateRange0, tMcInputs.at(0).mMaximum);

    tArticle->update();
    CPPUNIT_ASSERT(1 == tArticle->mEpoch);
    CPPUNIT_ASSERT(1 == tArticle->mGlobalRunCounter);
    CPPUNIT_ASSERT(1 == tArticle->mRunCounter);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDelStateInc0, tArticle->mGradients.at(0).mState.at(0), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDelStateDec0, tArticle->mGradients.at(1).mState.at(0), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDelStateInc0, tArticle->mGradients.at(0).mDeltaState, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDelStateDec0, tArticle->mGradients.at(1).mDeltaState, FLT_EPSILON);
    CPPUNIT_ASSERT(&tArticle->mGradients.at(0) == tArticle->mActiveState);
    CPPUNIT_ASSERT(1 == tArticle->mActiveState->mRunId);

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS gradient descent optimization class assignCost method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimGradientDescent::testAssignCost()
{
    UT_RESULT;

    /// - Initialize with nominal config.
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    tArticle->initialize(&tMcInputs);

    /// @test assigns cost to matching gradient run ID's.
    tArticle->mState->mCost           = 0.5;
    tArticle->mGradients.at(0).mRunId = 1.0;
    tArticle->mGradients.at(1).mRunId = 2.0;
    tArticle->mGradients.at(2).mRunId = 3.0;
    tArticle->mGradients.at(3).mRunId = 4.0;
    tArticle->mState->mRunId          = 5.0;
    tArticle->mGradients.at(2).mDeltaState = 0.2;
    tArticle->mGradients.at(3).mDeltaState = 0.1;
    const double expectedDcost2    = 17.5 - 0.5;
    const double expectedDcost3    = 42.0 - 0.5;
    const double expectedGradient2 = expectedDcost2 / 0.2;
    const double expectedGradient3 = expectedDcost3 / 0.1;
    CPPUNIT_ASSERT_NO_THROW(tArticle->assignCost(42.0, 0.0, 4.0));
    CPPUNIT_ASSERT(42.0 == tArticle->mGradients.at(3).mCost);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGradient3, tArticle->mGradients.at(3).mCostGradient, FLT_EPSILON);
    CPPUNIT_ASSERT_NO_THROW(tArticle->assignCost(17.5, 0.0, 3.0));
    CPPUNIT_ASSERT(17.5 == tArticle->mGradients.at(2).mCost);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGradient2, tArticle->mGradients.at(2).mCostGradient, FLT_EPSILON);

    /// @test assigns cost to the state.
    CPPUNIT_ASSERT_NO_THROW(tArticle->assignCost(0.1, 0.0, 5.0));
    CPPUNIT_ASSERT(0.1 == tArticle->mState->mCost);

    /// @test throws for no match to run ID's.
    CPPUNIT_ASSERT_THROW(tArticle->assignCost(1.0, 0.0, 6.0), std::runtime_error);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS gradient descent optimization class shutdown method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimGradientDescent::testShutdown()
{
    UT_RESULT;

    /// - Initialize with nominal config.
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    tArticle->initialize(&tMcInputs);

    /// - Load some values into the states for checking the output file.
    tArticle->mState->mState.at(0) =  1.1;
    tArticle->mState->mState.at(1) =  9.9;
    tArticle->mState->mCost        = 0.001;

    /// @test output file from shutdown.
    tArticle->shutdown();
    std::string pathFile = "grad_state.csv";
    std::ifstream file (pathFile.c_str(), (std::ifstream::in));
    CPPUNIT_ASSERT(file);
    std::stringstream fString;
    fString << file.rdbuf();
    std::vector<std::string> fLines = Strings::split(fString.str(), "\n");
    CPPUNIT_ASSERT(3 == fLines.size());
    CPPUNIT_ASSERT("Global_cost,state_0,state_1" == fLines.at(0));
    CPPUNIT_ASSERT("0.001,1.1,9.9"               == fLines.at(1));
    file.close();

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS gradient descent optimization class access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimGradientDescent::testAccessors()
{
    UT_RESULT;

    /// - Set the nominal config.
    tArticle->setConfigData(tConfigData);

    /// @test getNumRuns.
    const unsigned int expectedRuns = (2 * tConfigData->mNumVars + 1) * tConfigData->mMaxEpoch;
    CPPUNIT_ASSERT(expectedRuns == tArticle->getNumRuns());

    /// - Initialize with nominal config.
    std::cout << std::endl;
    tArticle->initialize(&tMcInputs);

    /// @test getState.
    CPPUNIT_ASSERT(&tArticle->mActiveState->mState == tArticle->getState());

    UT_PASS_LAST;
}

