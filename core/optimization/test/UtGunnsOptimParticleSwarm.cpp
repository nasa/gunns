///@copyright Copyright 2023 United States Government as represented by the Administrator of the
///           National Aeronautics and Space Administration.  All Rights Reserved.

#include "strings/UtResult.hh"
#include "core/optimization/GunnsOptimMonteCarloTypes.hh"
#include <stdexcept>

#include "UtGunnsOptimParticleSwarm.hh"
#include "strings/Strings.hh"

/// @details  Test identification number.
int UtGunnsOptimParticleSwarm::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS particle swarm optimization class unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsOptimParticleSwarm::UtGunnsOptimParticleSwarm()
    :
    CppUnit::TestFixture(),
    tName(""),
    tArticle(0),
    tConfigData(0),
    tMcInput1(0.0),
    tMcInput2(0.0),
    tMcInputs()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS particle swarm optimization class unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsOptimParticleSwarm::~UtGunnsOptimParticleSwarm()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::setUp()
{
    /// - Default construct the nominal test article.
    tName    = "tArticle";
    tArticle = new FriendlyGunnsOptimParticleSwarm();

    /// - Set verbosity for code coverage.  This will spam the verbose outputs to the console, but
    ///   we aren't actually testing that output for correctness.
    tArticle->setVerbosityLevel(1);

    /// - Set up nominal config data.
    tConfigData = new GunnsOptimParticleSwarmConfigData;
    tConfigData->mNumParticles     = 2;
    tConfigData->mMaxEpoch         = 5;
    tConfigData->mInertiaWeight    = 0.5;
    tConfigData->mInertiaWeightEnd = 0.8;
    tConfigData->mCognitiveCoeff   = 2.0;
    tConfigData->mSocialCoeff      = 1.5;
    tConfigData->mMaxVelocity      = 0.2;
    tConfigData->mRandomSeed       = 42;
    tConfigData->mInitDistribution = GunnsOptimParticleSwarmConfigData::MIN_MAX_CORNERS;

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
void UtGunnsOptimParticleSwarm::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tConfigData;
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the GUNNS particle swarm optimization particle state structure.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testParticleState()
{
    UT_RESULT_FIRST;

    /// @test default constructor.
    GunnsOptimParticleSwarmState defaultState;
    CPPUNIT_ASSERT(0   == defaultState.mState.size());
    CPPUNIT_ASSERT(0   == defaultState.mVelocity.size());
    CPPUNIT_ASSERT(0   == defaultState.mAcceleration.size());
    CPPUNIT_ASSERT(0.0 == defaultState.mCost);
    CPPUNIT_ASSERT(0.0 == defaultState.mRunId);

    /// @test implicit copy constructor and custom assignment operator.
    defaultState.mState.push_back(1.0);
    defaultState.mVelocity.push_back(2.0);
    defaultState.mAcceleration.push_back(3.0);
    defaultState.mCost  = 4.0;
    defaultState.mRunId = 5.0;
    GunnsOptimParticleSwarmState copyState(defaultState);
    GunnsOptimParticleSwarmState assignState;
    assignState = copyState;
    CPPUNIT_ASSERT(1   == assignState.mState.size());
    CPPUNIT_ASSERT(1.0 == assignState.mState.at(0));
    CPPUNIT_ASSERT(0   == assignState.mVelocity.size());
    CPPUNIT_ASSERT(0   == assignState.mAcceleration.size());
    CPPUNIT_ASSERT(4.0 == assignState.mCost);
    CPPUNIT_ASSERT(5.0 == assignState.mRunId);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the GUNNS particle swarm optimization particle structure.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testParticle()
{
    UT_RESULT;

    /// @test implicit default constructor.
    GunnsOptimParticleSwarmParticle defaultParticle;
    CPPUNIT_ASSERT(0.0 == defaultParticle.mCurrentState.mCost);
    CPPUNIT_ASSERT(0.0 == defaultParticle.mBestState.mCost);

    /// @test initialize function.
    defaultParticle.initialize(2);

    CPPUNIT_ASSERT(2   == defaultParticle.mCurrentState.mState.size());
    CPPUNIT_ASSERT(2   == defaultParticle.mCurrentState.mVelocity.size());
    CPPUNIT_ASSERT(2   == defaultParticle.mCurrentState.mAcceleration.size());
    CPPUNIT_ASSERT(0.0 == defaultParticle.mCurrentState.mState.at(1));
    CPPUNIT_ASSERT(0.0 == defaultParticle.mCurrentState.mVelocity.at(1));
    CPPUNIT_ASSERT(0.0 == defaultParticle.mCurrentState.mAcceleration.at(1));
    CPPUNIT_ASSERT(0.0 == defaultParticle.mCurrentState.mCost);

    CPPUNIT_ASSERT(2   == defaultParticle.mBestState.mState.size());
    CPPUNIT_ASSERT(0   == defaultParticle.mBestState.mVelocity.size());
    CPPUNIT_ASSERT(0   == defaultParticle.mBestState.mAcceleration.size());
    CPPUNIT_ASSERT(0.0 == defaultParticle.mBestState.mState.at(1));
    CPPUNIT_ASSERT(0.0 == defaultParticle.mBestState.mCost);

    /// @test implicit copy constructor and assignment operator.
    defaultParticle.mCurrentState.mState.at(1)     = 1.0;
    defaultParticle.mBestState.mState.at(1)        = 2.0;
    defaultParticle.mCurrentState.mCost            = 4.0;
    defaultParticle.mCurrentState.mRunId           = 5.0;
    defaultParticle.mBestState.mCost               = 6.0;
    defaultParticle.mBestState.mRunId              = 7.0;

    GunnsOptimParticleSwarmParticle copyParticle(defaultParticle);
    GunnsOptimParticleSwarmParticle assignParticle;
    assignParticle = copyParticle;

    CPPUNIT_ASSERT(2   == assignParticle.mCurrentState.mState.size());
    CPPUNIT_ASSERT(0   == assignParticle.mCurrentState.mVelocity.size());
    CPPUNIT_ASSERT(0   == assignParticle.mCurrentState.mAcceleration.size());
    CPPUNIT_ASSERT(1.0 == assignParticle.mCurrentState.mState.at(1));
    CPPUNIT_ASSERT(4.0 == assignParticle.mCurrentState.mCost);
    CPPUNIT_ASSERT(5.0 == assignParticle.mCurrentState.mRunId);

    CPPUNIT_ASSERT(2   == assignParticle.mBestState.mState.size());
    CPPUNIT_ASSERT(0   == assignParticle.mBestState.mVelocity.size());
    CPPUNIT_ASSERT(0   == assignParticle.mBestState.mAcceleration.size());
    CPPUNIT_ASSERT(2.0 == assignParticle.mBestState.mState.at(1));
    CPPUNIT_ASSERT(6.0 == assignParticle.mBestState.mCost);
    CPPUNIT_ASSERT(7.0 == assignParticle.mBestState.mRunId);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS particle swarm optimization configuration data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testConfigData()
{
    UT_RESULT;

    /// @test default constructor.
    GunnsOptimParticleSwarmConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                         == defaultConfig.mNumParticles);
    CPPUNIT_ASSERT(0                                         == defaultConfig.mMaxEpoch);
    CPPUNIT_ASSERT(0.0                                       == defaultConfig.mInertiaWeight);
    CPPUNIT_ASSERT(0.0                                       == defaultConfig.mInertiaWeightEnd);
    CPPUNIT_ASSERT(0.0                                       == defaultConfig.mCognitiveCoeff);
    CPPUNIT_ASSERT(0.0                                       == defaultConfig.mSocialCoeff);
    CPPUNIT_ASSERT(0.0                                       == defaultConfig.mMaxVelocity);
    CPPUNIT_ASSERT(0                                         == defaultConfig.mRandomSeed);
    CPPUNIT_ASSERT(GunnsOptimParticleSwarmConfigData::RANDOM == defaultConfig.mInitDistribution);

    /// @test assignment operator.
    GunnsOptimParticleSwarmConfigData assignConfig;
    assignConfig = *tConfigData;

    CPPUNIT_ASSERT(2                                                  == assignConfig.mNumParticles);
    CPPUNIT_ASSERT(5                                                  == assignConfig.mMaxEpoch);
    CPPUNIT_ASSERT(0.5                                                == assignConfig.mInertiaWeight);
    CPPUNIT_ASSERT(0.8                                                == assignConfig.mInertiaWeightEnd);
    CPPUNIT_ASSERT(2.0                                                == assignConfig.mCognitiveCoeff);
    CPPUNIT_ASSERT(1.5                                                == assignConfig.mSocialCoeff);
    CPPUNIT_ASSERT(0.2                                                == assignConfig.mMaxVelocity);
    CPPUNIT_ASSERT(42                                                 == assignConfig.mRandomSeed);
    CPPUNIT_ASSERT(GunnsOptimParticleSwarmConfigData::MIN_MAX_CORNERS == assignConfig.mInitDistribution);

    /// @test throw on bad condig data type.
    BadGunnsOptimParticleSwarmConfig badConfig;
    CPPUNIT_ASSERT_THROW(tArticle->setConfigData(&badConfig), std::runtime_error);
    CPPUNIT_ASSERT(0  == tArticle->mConfigData.mNumParticles);

    /// @test add configuration data function.
    CPPUNIT_ASSERT_NO_THROW(tArticle->setConfigData(tConfigData));
    CPPUNIT_ASSERT(2  == tArticle->mConfigData.mNumParticles);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS particle swarm optimization class default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testDefaultConstruction()
{
    UT_RESULT;

    /// @test Default construction values.
    FriendlyGunnsOptimParticleSwarm* article = new FriendlyGunnsOptimParticleSwarm();

    CPPUNIT_ASSERT(0                         == article->mConfigData.mNumParticles);
    CPPUNIT_ASSERT(0                         == article->mParticles.size());
    CPPUNIT_ASSERT(0                         == article->mActiveParticle);
    CPPUNIT_ASSERT(0.0                       == article->mGlobalBestState.mCost);
    CPPUNIT_ASSERT(0                         == article->mMaxVelocity.size());
    CPPUNIT_ASSERT("GunnsOptimParticleSwarm" == article->mName);
    CPPUNIT_ASSERT(0                         == article->mInStatesMaster);

    delete article;

    /// @test new/delete for code coverage.
    GunnsOptimParticleSwarm* article2 = new GunnsOptimParticleSwarm();
    delete article2;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS particle swarm optimization class initialize method using the min/max
///           corners option for the initial swarm state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testInitializeMinMax()
{
    UT_RESULT;

    /// @test initialize outputs.
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(&tMcInputs));

    const double expectedMaxV0 = (tMcInputs.at(0).mMaximum - tMcInputs.at(0).mMinimum)
                               * tConfigData->mMaxVelocity;
    const double expectedMaxV1 = (tMcInputs.at(1).mMaximum - tMcInputs.at(1).mMinimum)
                               * tConfigData->mMaxVelocity;

    CPPUNIT_ASSERT(&tMcInputs == tArticle->mInStatesMaster);
    CPPUNIT_ASSERT(2 == tArticle->mParticles.size());
    CPPUNIT_ASSERT(2 == tArticle->mParticles.at(0).mCurrentState.mState.size());
    CPPUNIT_ASSERT(2 == tArticle->mParticles.at(1).mCurrentState.mState.size());
    CPPUNIT_ASSERT(tArticle->mActiveParticle == &tArticle->mParticles.at(0));
    CPPUNIT_ASSERT(2   == tArticle->mGlobalBestState.mState.size());
    CPPUNIT_ASSERT(0.0 == tArticle->mGlobalBestState.mState.at(0));
    CPPUNIT_ASSERT(0.0 == tArticle->mGlobalBestState.mState.at(1));
    CPPUNIT_ASSERT(2   == tArticle->mMaxVelocity.size());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMaxV0, tArticle->mMaxVelocity.at(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMaxV1, tArticle->mMaxVelocity.at(1), DBL_EPSILON);
    CPPUNIT_ASSERT(1   == tArticle->mEpoch);
    CPPUNIT_ASSERT(-1  == tArticle->mGlobalRunCounter);
    CPPUNIT_ASSERT(-1  == tArticle->mRunCounter);
    CPPUNIT_ASSERT(tMcInputs.at(0).mMinimum == tArticle->mParticles.at(0).mCurrentState.mState.at(0));
    CPPUNIT_ASSERT(tMcInputs.at(1).mMinimum == tArticle->mParticles.at(0).mCurrentState.mState.at(1));
    CPPUNIT_ASSERT(tMcInputs.at(0).mMaximum == tArticle->mParticles.at(1).mCurrentState.mState.at(0));
    CPPUNIT_ASSERT(tMcInputs.at(1).mMaximum == tArticle->mParticles.at(1).mCurrentState.mState.at(1));
    CPPUNIT_ASSERT(MsMath::isInRange(-expectedMaxV0, tArticle->mParticles.at(0).mCurrentState.mVelocity.at(0), expectedMaxV0));
    CPPUNIT_ASSERT(MsMath::isInRange(-expectedMaxV1, tArticle->mParticles.at(0).mCurrentState.mVelocity.at(1), expectedMaxV1));
    CPPUNIT_ASSERT(MsMath::isInRange(-expectedMaxV0, tArticle->mParticles.at(1).mCurrentState.mVelocity.at(0), expectedMaxV0));
    CPPUNIT_ASSERT(MsMath::isInRange(-expectedMaxV1, tArticle->mParticles.at(1).mCurrentState.mVelocity.at(1), expectedMaxV1));
    CPPUNIT_ASSERT(0.0 == tArticle->mParticles.at(0).mCurrentState.mAcceleration.at(0));
    CPPUNIT_ASSERT(0.0 == tArticle->mParticles.at(0).mCurrentState.mAcceleration.at(1));
    CPPUNIT_ASSERT(0.0 == tArticle->mParticles.at(1).mCurrentState.mAcceleration.at(0));
    CPPUNIT_ASSERT(0.0 == tArticle->mParticles.at(1).mCurrentState.mAcceleration.at(1));
    CPPUNIT_ASSERT(DBL_MAX == tArticle->mParticles.at(0).mBestState.mCost);
    CPPUNIT_ASSERT(DBL_MAX == tArticle->mParticles.at(1).mBestState.mCost);

    {
        std::string pathFile = "pso_cost_history.csv";
        std::ifstream file (pathFile.c_str(), (std::ifstream::in));
        CPPUNIT_ASSERT(file);
        std::stringstream fString;
        fString << file.rdbuf();
        CPPUNIT_ASSERT("Epoch,Global_Best_Cost\n" == fString.str());
        file.close();
    } {
        std::string pathFile = "pso_swarm_history.csv";
        std::ifstream file (pathFile.c_str(), (std::ifstream::in));
        CPPUNIT_ASSERT(file);
        std::stringstream fString;
        fString << file.rdbuf();
        CPPUNIT_ASSERT("Epoch,Pos_0_0,Pos_0_1,Cost_0,Pos_1_0,Pos_1_1,Cost_1,Pos_Best_0,Pos_Best_1,Cost_Best\n" == fString.str());
        file.close();
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS particle swarm optimization class initialize method using the random
///           option for the initial swarm state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testInitializeRandom()
{
    UT_RESULT;

    /// @test initialize outputs.
    tConfigData->mInitDistribution = GunnsOptimParticleSwarmConfigData::RANDOM;
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(&tMcInputs));

    const double expectedMaxV0 = (tMcInputs.at(0).mMaximum - tMcInputs.at(0).mMinimum)
                               * tConfigData->mMaxVelocity;
    const double expectedMaxV1 = (tMcInputs.at(1).mMaximum - tMcInputs.at(1).mMinimum)
                               * tConfigData->mMaxVelocity;

    CPPUNIT_ASSERT(&tMcInputs == tArticle->mInStatesMaster);
    CPPUNIT_ASSERT(2 == tArticle->mParticles.size());
    CPPUNIT_ASSERT(2 == tArticle->mParticles.at(0).mCurrentState.mState.size());
    CPPUNIT_ASSERT(2 == tArticle->mParticles.at(1).mCurrentState.mState.size());
    CPPUNIT_ASSERT(tArticle->mActiveParticle == &tArticle->mParticles.at(0));
    CPPUNIT_ASSERT(2   == tArticle->mGlobalBestState.mState.size());
    CPPUNIT_ASSERT(0.0 == tArticle->mGlobalBestState.mState.at(0));
    CPPUNIT_ASSERT(0.0 == tArticle->mGlobalBestState.mState.at(1));
    CPPUNIT_ASSERT(2   == tArticle->mMaxVelocity.size());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMaxV0, tArticle->mMaxVelocity.at(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMaxV1, tArticle->mMaxVelocity.at(1), DBL_EPSILON);
    CPPUNIT_ASSERT(1   == tArticle->mEpoch);
    CPPUNIT_ASSERT(-1  == tArticle->mGlobalRunCounter);
    CPPUNIT_ASSERT(-1  == tArticle->mRunCounter);
    CPPUNIT_ASSERT(MsMath::isInRange(tMcInputs.at(0).mMinimum, tArticle->mParticles.at(0).mCurrentState.mState.at(0), tMcInputs.at(0).mMaximum));
    CPPUNIT_ASSERT(MsMath::isInRange(tMcInputs.at(1).mMinimum, tArticle->mParticles.at(0).mCurrentState.mState.at(1), tMcInputs.at(1).mMaximum));
    CPPUNIT_ASSERT(MsMath::isInRange(tMcInputs.at(0).mMinimum, tArticle->mParticles.at(1).mCurrentState.mState.at(0), tMcInputs.at(0).mMaximum));
    CPPUNIT_ASSERT(MsMath::isInRange(tMcInputs.at(1).mMinimum, tArticle->mParticles.at(1).mCurrentState.mState.at(1), tMcInputs.at(1).mMaximum));
    CPPUNIT_ASSERT(MsMath::isInRange(-expectedMaxV0, tArticle->mParticles.at(0).mCurrentState.mVelocity.at(0), expectedMaxV0));
    CPPUNIT_ASSERT(MsMath::isInRange(-expectedMaxV1, tArticle->mParticles.at(0).mCurrentState.mVelocity.at(1), expectedMaxV1));
    CPPUNIT_ASSERT(MsMath::isInRange(-expectedMaxV0, tArticle->mParticles.at(1).mCurrentState.mVelocity.at(0), expectedMaxV0));
    CPPUNIT_ASSERT(MsMath::isInRange(-expectedMaxV1, tArticle->mParticles.at(1).mCurrentState.mVelocity.at(1), expectedMaxV1));
    CPPUNIT_ASSERT(0.0 == tArticle->mParticles.at(0).mCurrentState.mAcceleration.at(0));
    CPPUNIT_ASSERT(0.0 == tArticle->mParticles.at(0).mCurrentState.mAcceleration.at(1));
    CPPUNIT_ASSERT(0.0 == tArticle->mParticles.at(1).mCurrentState.mAcceleration.at(0));
    CPPUNIT_ASSERT(0.0 == tArticle->mParticles.at(1).mCurrentState.mAcceleration.at(1));
    CPPUNIT_ASSERT(DBL_MAX == tArticle->mParticles.at(0).mBestState.mCost);
    CPPUNIT_ASSERT(DBL_MAX == tArticle->mParticles.at(1).mBestState.mCost);

    {
        std::string pathFile = "pso_cost_history.csv";
        std::ifstream file (pathFile.c_str(), (std::ifstream::in));
        CPPUNIT_ASSERT(file);
        std::stringstream fString;
        fString << file.rdbuf();
        CPPUNIT_ASSERT("Epoch,Global_Best_Cost\n" == fString.str());
        file.close();
    } {
        std::string pathFile = "pso_swarm_history.csv";
        std::ifstream file (pathFile.c_str(), (std::ifstream::in));
        CPPUNIT_ASSERT(file);
        std::stringstream fString;
        fString << file.rdbuf();
        CPPUNIT_ASSERT("Epoch,Pos_0_0,Pos_0_1,Cost_0,Pos_1_0,Pos_1_1,Cost_1,Pos_Best_0,Pos_Best_1,Cost_Best\n" == fString.str());
        file.close();
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS particle swarm optimization class initialization method validation of
///           initial configuration and error handling.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testInitErrors()
{
    UT_RESULT;

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

    /// @test throw on bad number of particles.
    tConfigData->mNumParticles = 0;
    tArticle->setConfigData(tConfigData);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::range_error);
    tConfigData->mNumParticles = 2;

    /// @test throw on bad number of epochs.
    tConfigData->mMaxEpoch = 0;
    tArticle->setConfigData(tConfigData);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::range_error);
    tConfigData->mMaxEpoch = 5;

    /// @test throw on bad starting inertial weight.
    tConfigData->mInertiaWeight = 0.0;
    tArticle->setConfigData(tConfigData);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::range_error);
    tConfigData->mInertiaWeight = 0.5;

    /// @test throw on bad ending inertial weight.
    tConfigData->mInertiaWeightEnd = 0.0;
    tArticle->setConfigData(tConfigData);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::range_error);
    tConfigData->mInertiaWeightEnd = 0.8;

    /// @test throw on bad cognitive coefficient.
    tConfigData->mCognitiveCoeff = 0.0;
    tArticle->setConfigData(tConfigData);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::range_error);
    tConfigData->mCognitiveCoeff = 2.0;

    /// @test throw on bad social coefficient.
    tConfigData->mSocialCoeff = 0.0;
    tArticle->setConfigData(tConfigData);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::range_error);
    tConfigData->mSocialCoeff = 2.0;

    /// @test throw on max velocity scalar out of range.
    tConfigData->mMaxVelocity = 0.0;
    tArticle->setConfigData(tConfigData);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::range_error);
    tConfigData->mMaxVelocity = 1.0001;
    tArticle->setConfigData(tConfigData);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::range_error);
    tConfigData->mMaxVelocity = 1.0;

    /// @test throw on invalid initial state selection.
    tConfigData->mInitDistribution = static_cast<GunnsOptimParticleSwarmConfigData::SwarmDistribution>(999);
    tArticle->setConfigData(tConfigData);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::range_error);
    tConfigData->mInitDistribution = GunnsOptimParticleSwarmConfigData::FILE;

    /// @test throw on missing swarm state file.
    std::string pathFile = "pso_state.csv";
    remove(pathFile.c_str());
    tArticle->setConfigData(tConfigData);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::runtime_error);

    /// @test throw on swarm state file has wrong # rows.
    {
        std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
        file << "Particle cost pos_0 pos_1 vel_0 vel_1 best_cost best_pos_0 best_pos_1" << std::endl;
        file.close();
    }
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::runtime_error);

    /// @test throw on swarm state file has wrong # columns.
    remove(pathFile.c_str());
    {
        std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
        file << "Particle" << std::endl;
        file << "global_best" << std::endl;
        file << "0" << std::endl;
        file << "1" << std::endl;
        file.close();
    }
    CPPUNIT_ASSERT_THROW(tArticle->initialize(&tMcInputs), std::runtime_error);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS particle swarm optimization class update and propagateSwarm methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testUpdate()
{
    UT_RESULT;

    /// - Initialize with nominal config.
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    tArticle->initialize(&tMcInputs);

    /// @test update method with no epoch update
    tArticle->update();
    CPPUNIT_ASSERT(1 == tArticle->mEpoch);
    CPPUNIT_ASSERT(0 == tArticle->mGlobalRunCounter);
    CPPUNIT_ASSERT(0 == tArticle->mRunCounter);
    CPPUNIT_ASSERT(&tArticle->mParticles.at(0) == tArticle->mActiveParticle);
    CPPUNIT_ASSERT(0 == tArticle->mActiveParticle->mCurrentState.mRunId);

    /// @test update method with no epoch update
    tArticle->update();
    CPPUNIT_ASSERT(1 == tArticle->mEpoch);
    CPPUNIT_ASSERT(1 == tArticle->mGlobalRunCounter);
    CPPUNIT_ASSERT(1 == tArticle->mRunCounter);
    CPPUNIT_ASSERT(&tArticle->mParticles.at(1) == tArticle->mActiveParticle);
    CPPUNIT_ASSERT(1 == tArticle->mActiveParticle->mCurrentState.mRunId);

    /// - Store current states for comparison
    double globStateDelta[4];
    double persStateDelta[4];
    double accelBound[4];
    double velPrevious[4];
    double posPrevious[4];
    globStateDelta[0] = -tArticle->mParticles.at(0).mCurrentState.mState.at(0);
    globStateDelta[1] = -tArticle->mParticles.at(0).mCurrentState.mState.at(1);
    globStateDelta[2] = -tArticle->mParticles.at(1).mCurrentState.mState.at(0);
    globStateDelta[3] = -tArticle->mParticles.at(1).mCurrentState.mState.at(1);
    persStateDelta[0] = -tArticle->mParticles.at(0).mCurrentState.mState.at(0);
    persStateDelta[1] = -tArticle->mParticles.at(0).mCurrentState.mState.at(1);
    persStateDelta[2] = -tArticle->mParticles.at(1).mCurrentState.mState.at(0);
    persStateDelta[3] = -tArticle->mParticles.at(1).mCurrentState.mState.at(1);
    accelBound[0] = fabs(persStateDelta[0] * tConfigData->mCognitiveCoeff
                       + globStateDelta[0] * tConfigData->mSocialCoeff);
    accelBound[1] = fabs(persStateDelta[1] * tConfigData->mCognitiveCoeff
                       + globStateDelta[1] * tConfigData->mSocialCoeff);
    accelBound[2] = fabs(persStateDelta[2] * tConfigData->mCognitiveCoeff
                       + globStateDelta[2] * tConfigData->mSocialCoeff);
    accelBound[3] = fabs(persStateDelta[3] * tConfigData->mCognitiveCoeff
                       + globStateDelta[3] * tConfigData->mSocialCoeff);
    velPrevious[0] = tArticle->mParticles.at(0).mCurrentState.mVelocity.at(0);
    velPrevious[1] = tArticle->mParticles.at(0).mCurrentState.mVelocity.at(1);
    velPrevious[2] = tArticle->mParticles.at(1).mCurrentState.mVelocity.at(0);
    velPrevious[3] = tArticle->mParticles.at(1).mCurrentState.mVelocity.at(1);
    posPrevious[0] = tArticle->mParticles.at(0).mCurrentState.mState.at(0);
    posPrevious[1] = tArticle->mParticles.at(0).mCurrentState.mState.at(1);
    posPrevious[2] = tArticle->mParticles.at(1).mCurrentState.mState.at(0);
    posPrevious[3] = tArticle->mParticles.at(1).mCurrentState.mState.at(1);
    const double inertia = tConfigData->mInertiaWeight
                         + (tConfigData->mInertiaWeightEnd - tConfigData->mInertiaWeight)
                         * 2 / tConfigData->mMaxEpoch;

    /// @test update method with the epoch update
    tArticle->update();
    CPPUNIT_ASSERT(2 == tArticle->mEpoch);
    CPPUNIT_ASSERT(2 == tArticle->mGlobalRunCounter);
    CPPUNIT_ASSERT(0 == tArticle->mRunCounter);
    CPPUNIT_ASSERT(&tArticle->mParticles.at(0) == tArticle->mActiveParticle);
    CPPUNIT_ASSERT(2 == tArticle->mActiveParticle->mCurrentState.mRunId);

    /// @test acceleration is within expected range
    CPPUNIT_ASSERT(MsMath::isInRange(-accelBound[0], tArticle->mParticles.at(0).mCurrentState.mAcceleration.at(0), accelBound[0]));
    CPPUNIT_ASSERT(MsMath::isInRange(-accelBound[1], tArticle->mParticles.at(0).mCurrentState.mAcceleration.at(1), accelBound[1]));
    CPPUNIT_ASSERT(MsMath::isInRange(-accelBound[2], tArticle->mParticles.at(1).mCurrentState.mAcceleration.at(0), accelBound[2]));
    CPPUNIT_ASSERT(MsMath::isInRange(-accelBound[3], tArticle->mParticles.at(1).mCurrentState.mAcceleration.at(1), accelBound[3]));

    /// @test velocity is the integral of acceleration and within limits
    const double maxV0 = (tMcInputs.at(0).mMaximum - tMcInputs.at(0).mMinimum)
                       * tConfigData->mMaxVelocity;
    const double maxV1 = (tMcInputs.at(1).mMaximum - tMcInputs.at(1).mMinimum)
                       * tConfigData->mMaxVelocity;
    double expectedVel[4];
    expectedVel[0] = inertia * velPrevious[0] + tArticle->mParticles.at(0).mCurrentState.mAcceleration.at(0);
    expectedVel[1] = inertia * velPrevious[1] + tArticle->mParticles.at(0).mCurrentState.mAcceleration.at(1);
    expectedVel[2] = inertia * velPrevious[2] + tArticle->mParticles.at(1).mCurrentState.mAcceleration.at(0);
    expectedVel[3] = inertia * velPrevious[3] + tArticle->mParticles.at(1).mCurrentState.mAcceleration.at(1);
    expectedVel[0] = MsMath::limitRange(-maxV0, expectedVel[0], maxV0);
    expectedVel[1] = MsMath::limitRange(-maxV1, expectedVel[1], maxV1);
    expectedVel[2] = MsMath::limitRange(-maxV0, expectedVel[2], maxV0);
    expectedVel[3] = MsMath::limitRange(-maxV1, expectedVel[3], maxV1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVel[0], tArticle->mParticles.at(0).mCurrentState.mVelocity.at(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVel[1], tArticle->mParticles.at(0).mCurrentState.mVelocity.at(1), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVel[2], tArticle->mParticles.at(1).mCurrentState.mVelocity.at(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVel[3], tArticle->mParticles.at(1).mCurrentState.mVelocity.at(1), DBL_EPSILON);

    /// @test position is the integral of velocity.  This assumes we won't have a boundary bounce,
    ///       which we'll test later.
    double expectedPos[4];
    expectedPos[0] = posPrevious[0] + expectedVel[0];
    expectedPos[1] = posPrevious[1] + expectedVel[1];
    expectedPos[2] = posPrevious[2] + expectedVel[2];
    expectedPos[3] = posPrevious[3] + expectedVel[3];
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPos[0], tArticle->mParticles.at(0).mCurrentState.mState.at(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPos[1], tArticle->mParticles.at(0).mCurrentState.mState.at(1), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPos[2], tArticle->mParticles.at(1).mCurrentState.mState.at(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPos[3], tArticle->mParticles.at(1).mCurrentState.mState.at(1), DBL_EPSILON);

    /// @test states appended to the output files
    //TODO could use more rigor here, check actual values...
    {
        std::string pathFile = "pso_cost_history.csv";
        std::ifstream file (pathFile.c_str(), (std::ifstream::in));
        CPPUNIT_ASSERT(file);
        std::stringstream fString;
        fString << file.rdbuf();
        std::vector<std::string> fLines = Strings::split(fString.str(), "\n");
        CPPUNIT_ASSERT(3 == fLines.size());
        file.close();
    } {
        std::string pathFile = "pso_swarm_history.csv";
        std::ifstream file (pathFile.c_str(), (std::ifstream::in));
        CPPUNIT_ASSERT(file);
        std::stringstream fString;
        fString << file.rdbuf();
        std::vector<std::string> fLines = Strings::split(fString.str(), "\n");
        CPPUNIT_ASSERT(3 == fLines.size());
        file.close();
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Additional tests for GUNNS particle swarm optimization class propagateSwarm and
///           applyConstraints methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testPropagateSwarm()
{
    UT_RESULT;

    /// - Initialize with nominal config.
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    tArticle->initialize(&tMcInputs);

    /// - Load some costs that will be improved by updateBestStates
    tArticle->mParticles.at(0).mBestState.mCost = 1.0;
    tArticle->mParticles.at(1).mBestState.mCost = 0.1;
    tArticle->mGlobalBestState.mCost            = 0.1;

    tArticle->mParticles.at(0).mCurrentState.mCost = 2.0;
    tArticle->mParticles.at(1).mCurrentState.mCost = 0.01;

    /// @test updateBestStates function called from propagateSwarm
    tArticle->propagateSwarm(0.5);
    CPPUNIT_ASSERT(1.0  == tArticle->mParticles.at(0).mBestState.mCost);
    CPPUNIT_ASSERT(0.01 == tArticle->mParticles.at(1).mBestState.mCost);
    CPPUNIT_ASSERT(0.01 == tArticle->mGlobalBestState.mCost);

    /// @test propagateSwarm applies velocity limits.  We manipulate the accel coefficients to
    ///       force acceleration to zero, set velocities outside the limits and then check that
    ///       limits get applied.
    tArticle->mConfigData.mCognitiveCoeff = 0.0;
    tArticle->mConfigData.mSocialCoeff    = 0.0;
    tArticle->mParticles.at(0).mCurrentState.mVelocity.at(0) = -999.9;
    tArticle->mParticles.at(0).mCurrentState.mVelocity.at(1) =  999.9;
    tArticle->propagateSwarm(1.0);
    const double maxV0 = (tMcInputs.at(0).mMaximum - tMcInputs.at(0).mMinimum)
                       * tConfigData->mMaxVelocity;
    const double maxV1 = (tMcInputs.at(1).mMaximum - tMcInputs.at(1).mMinimum)
                       * tConfigData->mMaxVelocity;
    CPPUNIT_ASSERT(0.0 == tArticle->mParticles.at(0).mCurrentState.mAcceleration.at(0));
    CPPUNIT_ASSERT(0.0 == tArticle->mParticles.at(0).mCurrentState.mAcceleration.at(1));
    CPPUNIT_ASSERT(-maxV0 == tArticle->mParticles.at(0).mCurrentState.mVelocity.at(0));
    CPPUNIT_ASSERT( maxV1 == tArticle->mParticles.at(0).mCurrentState.mVelocity.at(1));

    /// @test propagateSwarm applies position limits.  Same as before, manipulate the state to
    ///       force position to hit its min/max limits, check limits are applied and velocity is
    ///       reversed (bounces).
    tArticle->mParticles.at(0).mCurrentState.mVelocity.at(0) =  maxV0;
    tArticle->mParticles.at(0).mCurrentState.mVelocity.at(1) = -maxV1;
    tArticle->mParticles.at(0).mCurrentState.mState.at(0) = tMcInputs.at(0).mMaximum - maxV0 / 2.0;
    tArticle->mParticles.at(0).mCurrentState.mState.at(1) = tMcInputs.at(1).mMinimum + maxV1 / 2.0;
    tArticle->propagateSwarm(1.0);
    CPPUNIT_ASSERT(tMcInputs.at(0).mMaximum == tArticle->mParticles.at(0).mCurrentState.mState.at(0));
    CPPUNIT_ASSERT(tMcInputs.at(1).mMinimum == tArticle->mParticles.at(0).mCurrentState.mState.at(1));
    CPPUNIT_ASSERT(-maxV0 == tArticle->mParticles.at(0).mCurrentState.mVelocity.at(0));
    CPPUNIT_ASSERT( maxV1 == tArticle->mParticles.at(0).mCurrentState.mVelocity.at(1));

    //TODO
    /// - Create a constraint of input 2 (ranges 5-10) from input 1 (ranges 0-20):
    ///   input2 = 5.0 + 0.25 * input1
//    LinearFit fit(5.0, 0.25, 0.0, 20.0);
//    GunnsOptimMonteCarloConstraint constraint(&fit, &tMcInput1);
//    tMcInputs.at(1).addNewConstraint(&constraint);
//
//    FriendlyGunnsOptimParticleSwarm article2;
//    article2.setConfigData(tConfigData);
//    article2.initialize(&tMcInputs);
//
//    const double expectedState0 = 1.0;
//    const double expectedState1 = 5.0 + 0.25 * expectedState0;
//    article2.mParticles.at(0).mCurrentState.mVelocity.at(0) = 0.0;
//    article2.mParticles.at(0).mCurrentState.mVelocity.at(1) = 0.0;
//    article2.mParticles.at(0).mCurrentState.mState.at(0) = expectedState0;
//    article2.mParticles.at(0).mCurrentState.mState.at(1) = 5.0;
//
//    article2.propagateSwarm(1.0);
//
//    CPPUNIT_ASSERT(expectedState0 == article2.mParticles.at(0).mCurrentState.mState.at(0));
//    CPPUNIT_ASSERT(expectedState1 == article2.mParticles.at(0).mCurrentState.mState.at(1));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS particle swarm optimization class assignCost method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testAssignCost()
{
    UT_RESULT;

    /// - Initialize with nominal config.
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    tArticle->initialize(&tMcInputs);

    /// @test assigns cost to matching particle run ID's.
    tArticle->mParticles.at(0).mCurrentState.mRunId = 3.0;
    tArticle->mParticles.at(1).mCurrentState.mRunId = 4.0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->assignCost(42.0, 0.0, 4.0));
    CPPUNIT_ASSERT(42.0 == tArticle->mParticles.at(1).mCurrentState.mCost);
    CPPUNIT_ASSERT_NO_THROW(tArticle->assignCost(17.5, 0.0, 3.0));
    CPPUNIT_ASSERT(17.5 == tArticle->mParticles.at(0).mCurrentState.mCost);

    /// @test throws for no match to particle run ID's.
    CPPUNIT_ASSERT_THROW(tArticle->assignCost(1.0, 0.0, 5.0), std::runtime_error);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS particle swarm optimization class shutdown method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testShutdown()
{
    UT_RESULT;

    /// - Initialize with nominal config.
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    tArticle->initialize(&tMcInputs);

    /// - Load some values into the states for checking the output file.
    tArticle->mParticles.at(0).mCurrentState.mState.at(0)    =  1.1;
    tArticle->mParticles.at(0).mCurrentState.mState.at(1)    =  6.6;
    tArticle->mParticles.at(1).mCurrentState.mState.at(0)    = 19.9;
    tArticle->mParticles.at(1).mCurrentState.mState.at(1)    =  9.9;
    tArticle->mParticles.at(0).mCurrentState.mVelocity.at(0) =  0.1;
    tArticle->mParticles.at(0).mCurrentState.mVelocity.at(1) =  0.2;
    tArticle->mParticles.at(1).mCurrentState.mVelocity.at(0) = -0.1;
    tArticle->mParticles.at(1).mCurrentState.mVelocity.at(1) = -0.2;
    tArticle->mParticles.at(0).mBestState.mCost              = 21.12;
    tArticle->mParticles.at(1).mBestState.mCost              =  0.001;
    tArticle->mGlobalBestState.mState.at(0)                  = 10.1;
    tArticle->mGlobalBestState.mState.at(1)                  =  7.7;
    tArticle->mGlobalBestState.mCost                         = 0.001;

    /// @test output file from shutdown.
    tArticle->shutdown();
    std::string pathFile = "pso_state.csv";
    std::ifstream file (pathFile.c_str(), (std::ifstream::in));
    CPPUNIT_ASSERT(file);
    std::stringstream fString;
    fString << file.rdbuf();
    std::vector<std::string> fLines = Strings::split(fString.str(), "\n");
    CPPUNIT_ASSERT(5 == fLines.size());
    CPPUNIT_ASSERT("Particle epoch cost pos_0 pos_1 vel_0 vel_1 best_cost best_pos_0 best_pos_1" == fLines.at(0));
    CPPUNIT_ASSERT("global_best 1 0.001 10.1 7.7 0.0 0.0 0.001 10.1 7.7"                     == fLines.at(1));
    CPPUNIT_ASSERT("0 1 0 1.1 6.6 0.1 0.2 21.12 0 0"                                         == fLines.at(2));
    CPPUNIT_ASSERT("1 1 0 19.9 9.9 -0.1 -0.2 0.001 0 0"                                      == fLines.at(3));
    file.close();

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS particle swarm optimization class initialize method using the file
///           read option for the initial swarm state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testInitializeFile()
{
    UT_RESULT;

    /// - Initialize with nominal config and using the file input option for initial swarm state.
    tConfigData->mInitDistribution = GunnsOptimParticleSwarmConfigData::FILE;
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    tArticle->initialize(&tMcInputs);

    /// @test initial states match the pso_state.csv file output from the previous test.
    CPPUNIT_ASSERT(10.1    == tArticle->mGlobalBestState.mState.at(0));
    CPPUNIT_ASSERT( 7.7    == tArticle->mGlobalBestState.mState.at(1));
    CPPUNIT_ASSERT(DBL_MAX == tArticle->mGlobalBestState.mCost);

    CPPUNIT_ASSERT( 1.1    == tArticle->mParticles.at(0).mCurrentState.mState.at(0));
    CPPUNIT_ASSERT( 6.6    == tArticle->mParticles.at(0).mCurrentState.mState.at(1));
    CPPUNIT_ASSERT(DBL_MAX == tArticle->mParticles.at(0).mBestState.mCost);

    CPPUNIT_ASSERT(19.9    == tArticle->mParticles.at(1).mCurrentState.mState.at(0));
    CPPUNIT_ASSERT( 9.9    == tArticle->mParticles.at(1).mCurrentState.mState.at(1));
    CPPUNIT_ASSERT(DBL_MAX == tArticle->mParticles.at(1).mBestState.mCost);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS particle swarm optimization class initialize method using the file
///           read option for the initial (continued) swarm state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testInitializeContinuous()
{
    UT_RESULT;

    /// - Initialize with nominal config and using the file input option for initial swarm state.
    tConfigData->mInitDistribution = GunnsOptimParticleSwarmConfigData::FILE_CONTINUOUS;
    tArticle->setConfigData(tConfigData);
    std::cout << std::endl;
    tArticle->initialize(&tMcInputs);

    /// @test initial states match the pso_state.csv file output from the previous test, and
    ///       includes costs and particle velocities for continuous propagation from the file.
    CPPUNIT_ASSERT(10.1   == tArticle->mGlobalBestState.mState.at(0));
    CPPUNIT_ASSERT( 7.7   == tArticle->mGlobalBestState.mState.at(1));
    CPPUNIT_ASSERT( 0.001 == tArticle->mGlobalBestState.mCost);

    CPPUNIT_ASSERT( 1.1   == tArticle->mParticles.at(0).mCurrentState.mState.at(0));
    CPPUNIT_ASSERT( 6.6   == tArticle->mParticles.at(0).mCurrentState.mState.at(1));
    CPPUNIT_ASSERT(21.12  == tArticle->mParticles.at(0).mBestState.mCost);

    CPPUNIT_ASSERT(19.9   == tArticle->mParticles.at(1).mCurrentState.mState.at(0));
    CPPUNIT_ASSERT( 9.9   == tArticle->mParticles.at(1).mCurrentState.mState.at(1));
    CPPUNIT_ASSERT( 0.001 == tArticle->mParticles.at(1).mBestState.mCost);

    CPPUNIT_ASSERT( 0.1   == tArticle->mParticles.at(0).mCurrentState.mVelocity.at(0));
    CPPUNIT_ASSERT( 0.2   == tArticle->mParticles.at(0).mCurrentState.mVelocity.at(1));
    CPPUNIT_ASSERT(-0.1   == tArticle->mParticles.at(1).mCurrentState.mVelocity.at(0));
    CPPUNIT_ASSERT(-0.2   == tArticle->mParticles.at(1).mCurrentState.mVelocity.at(1));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS particle swarm optimization class access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimParticleSwarm::testAccessors()
{
    UT_RESULT;

    /// - Set the nominal config.
    tArticle->setConfigData(tConfigData);

    /// @test getNumRuns.
    const unsigned int expectedRuns = tConfigData->mNumParticles * tConfigData->mMaxEpoch;
    CPPUNIT_ASSERT(expectedRuns == tArticle->getNumRuns());

    /// - Initialize with nominal config.
    std::cout << std::endl;
    tArticle->initialize(&tMcInputs);

    /// @test getState.
    CPPUNIT_ASSERT(&tArticle->mParticles.at(0).mCurrentState.mState == tArticle->getState());

    UT_PASS_LAST;
}

