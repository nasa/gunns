///@copyright Copyright 2023 United States Government as represented by the Administrator of the
///           National Aeronautics and Space Administration.  All Rights Reserved.

#include "strings/UtResult.hh"

#include "UtGunnsOptimMonteCarlo.hh"
#include "core/GunnsInfraFunctions.hh"

/// @details  Test identification number.
int UtGunnsOptimMonteCarlo::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name (--) Instance name for H&S messages.
///
/// @details  Default constructs this test article.
////////////////////////////////////////////////////////////////////////////////////////////////////
FriendlyGunnsOptimMonteCarlo::FriendlyGunnsOptimMonteCarlo(const std::string& name)
    :
    GunnsOptimMonteCarlo(name)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this test article.
////////////////////////////////////////////////////////////////////////////////////////////////////
FriendlyGunnsOptimMonteCarlo::~FriendlyGunnsOptimMonteCarlo()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS optimization monte carlo manager class unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsOptimMonteCarlo::UtGunnsOptimMonteCarlo()
    :
    CppUnit::TestFixture(),
    tName(""),
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS optimization monte carlo manager class unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsOptimMonteCarlo::~UtGunnsOptimMonteCarlo()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::setUp()
{
    /// - Default construct the nominal test article.
    tName    = "tArticle";
    tArticle = new FriendlyGunnsOptimMonteCarlo(tName);

    /// - Set verbosity for code coverage.  This will spam the verbose outputs to the console, but
    ///   we aren't actually testing that output for correctness.
    tArticle->setVerbosityLevel(1);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS optimization monte carlo manager class default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::testDefaultConstruction()
{
    UT_RESULT_INITIAL("GUNNS Optimization Classes");

    /// @test Default construction values.
    FriendlyGunnsOptimMonteCarlo* article = new FriendlyGunnsOptimMonteCarlo(tName);

    CPPUNIT_ASSERT(0     == article->mOptimizer);
    CPPUNIT_ASSERT(0     == article->mRunId);
    CPPUNIT_ASSERT(tName == article->mName);
    CPPUNIT_ASSERT(false == article->mIsMaster);
    CPPUNIT_ASSERT(false == article->mIsSlave);
    CPPUNIT_ASSERT(0     == article->mSlaveId);
    CPPUNIT_ASSERT(0.0   == article->mRunIdReturned);
    CPPUNIT_ASSERT(0     == article->mModelStepCount);
    CPPUNIT_ASSERT(0     == article->mVerbosityLevel);
    CPPUNIT_ASSERT(0     == article->mInputs.size());
    CPPUNIT_ASSERT(0     == article->mDrivers.size());
    CPPUNIT_ASSERT(0     == article->mOutputs.size());

    delete article;

    /// @test new/delete for code coverage.
    GunnsOptimMonteCarlo* article2 = new GunnsOptimMonteCarlo();
    delete article2;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS optimization monte carlo manager class initMaster method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::testInitMaster()
{
    UT_RESULT;

    /// @test exception thrown when calling from the Slave role.
    std::cout << std::endl;
    GunnsInfraFunctions::mMcIsSlave = true;
    CPPUNIT_ASSERT_THROW(tArticle->initMaster(), std::runtime_error);

    /// @test nominal initialization with scalar output targets and no optimizer.
    GunnsInfraFunctions::mMcIsSlave = false;
    double outputA = 0.0;
    double outputB = 0.0;
    tArticle->addOutput("outputA", &outputA, 0.0, 1.0);
    tArticle->addOutput("outputB", &outputB, 0.0, 1.0);

    CPPUNIT_ASSERT_NO_THROW(tArticle->initMaster());
    CPPUNIT_ASSERT(false == tArticle->mIsSlave);
    CPPUNIT_ASSERT(true  == tArticle->mIsMaster);
    CPPUNIT_ASSERT(-1    == tArticle->mSlaveId);
    CPPUNIT_ASSERT(-1    == tArticle->mRunId);
    CPPUNIT_ASSERT(0     == tArticle->mOptimizer);

    /// @test nominal initialization with an optimizer and output trajectory data.
    tArticle->addOptimizer(GunnsOptimFactory::TEST);
    GunnsOptimTest* optimizer = dynamic_cast<GunnsOptimTest*>(tArticle->mOptimizer);
    CPPUNIT_ASSERT(0     != optimizer);

    std::string targetsRow0 = "0.0,3.0,12.0";
    tArticle->addOutputDataRow(targetsRow0);

    tArticle->initMaster();
    CPPUNIT_ASSERT(false == tArticle->mIsSlave);
    CPPUNIT_ASSERT(true  == tArticle->mIsMaster);
    CPPUNIT_ASSERT(-1    == tArticle->mSlaveId);
    CPPUNIT_ASSERT(-1    == tArticle->mRunId);
    CPPUNIT_ASSERT(1     == optimizer->mInitCounter);

    /// @test factory creation of a PSO optimizer.
    tArticle->addOptimizer(GunnsOptimFactory::PSO);
    GunnsOptimParticleSwarm* optimizerPso = dynamic_cast<GunnsOptimParticleSwarm*>(tArticle->mOptimizer);
    CPPUNIT_ASSERT(0     != optimizerPso);

    /// @test factory creation of a Gradient Descent optimizer.
    tArticle->addOptimizer(GunnsOptimFactory::GRADIENT_DESCENT);
    GunnsOptimGradientDescent* optimizerGrad = dynamic_cast<GunnsOptimGradientDescent*>(tArticle->mOptimizer);
    CPPUNIT_ASSERT(0     != optimizerGrad);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS optimization monte carlo manager class initSlave method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::testInitSlave()
{
    UT_RESULT;

    /// @test exception thrown when calling from the Master role.
    std::cout << std::endl;
    GunnsInfraFunctions::mMcIsSlave = false;
    CPPUNIT_ASSERT_THROW(tArticle->initSlave(), std::runtime_error);

    /// @test nominal initialization.
    GunnsInfraFunctions::mMcIsSlave = true;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initSlave());
    CPPUNIT_ASSERT(true  == tArticle->mIsSlave);
    CPPUNIT_ASSERT(false == tArticle->mIsMaster);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS optimization monte carlo manager class updateMasterPre method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::testUpdateMasterPre()
{
    UT_RESULT;

    tArticle->addOptimizer(GunnsOptimFactory::TEST);
    GunnsOptimTest* optimizer = dynamic_cast<GunnsOptimTest*>(tArticle->mOptimizer);

    /// @test Run ID incremented and optimizer is updated.  Also test handles null state returned.
    std::cout << std::endl;
    CPPUNIT_ASSERT_NO_THROW(tArticle->updateMasterPre());
    CPPUNIT_ASSERT(1.0  == tArticle->mRunId);
    CPPUNIT_ASSERT(1    == optimizer->mUpdateCounter);

    /// @test addInput doesn't accept inputs in Slave role.
    GunnsInfraFunctions::mMcIsSlave = true;
    double mcInputs[3] = {0.0, 0.0, 0.0};
    tArticle->addInput("mcInputs_0", &mcInputs[0], -1.0, 1.0, 0);
    CPPUNIT_ASSERT(0            == tArticle->mInputs.size());

    /// @test addInput accepts inputs in Master role.
    GunnsInfraFunctions::mMcIsSlave = false;
    tArticle->addInput("mcInputs_0", &mcInputs[0], -1.0, 1.0, 0);
    tArticle->addInput("mcInputs_1", &mcInputs[1], -2.0, 2.0, 0);
    CPPUNIT_ASSERT(2            == tArticle->mInputs.size());
    CPPUNIT_ASSERT("mcInputs_1" == tArticle->mInputs.at(1).mName);
    CPPUNIT_ASSERT(&mcInputs[1] == tArticle->mInputs.at(1).mAddress);
    CPPUNIT_ASSERT(-2.0         == tArticle->mInputs.at(1).mMinimum);
    CPPUNIT_ASSERT( 2.0         == tArticle->mInputs.at(1).mMaximum);
    CPPUNIT_ASSERT(0            == tArticle->mInputs.at(1).mConstraints.size());

    LinearFit fit(0.0, 2.0, 0.0, 0.01);
    GunnsOptimMonteCarloConstraint constraint(&fit, &mcInputs[2]);
    tArticle->addInput("mcInputs_2", &mcInputs[2], -3.0, 3.0, &constraint);
    CPPUNIT_ASSERT(3            == tArticle->mInputs.size());
    CPPUNIT_ASSERT("mcInputs_2" == tArticle->mInputs.at(2).mName);
    CPPUNIT_ASSERT(&mcInputs[2] == tArticle->mInputs.at(2).mAddress);
    CPPUNIT_ASSERT(-3.0         == tArticle->mInputs.at(2).mMinimum);
    CPPUNIT_ASSERT( 3.0         == tArticle->mInputs.at(2).mMaximum);
    CPPUNIT_ASSERT(1            == tArticle->mInputs.at(2).mConstraints.size());

    /// @test MC inputs updated with optimizer state.
    std::vector<double> testState;
    testState.push_back(1.0);
    testState.push_back(2.0);
    testState.push_back(3.0);
    optimizer->mState = &testState;
    CPPUNIT_ASSERT_NO_THROW(tArticle->updateMasterPre());
    CPPUNIT_ASSERT(2.0  == tArticle->mRunId);
    CPPUNIT_ASSERT(2    == optimizer->mUpdateCounter);
    CPPUNIT_ASSERT(1.0  == mcInputs[0]);
    CPPUNIT_ASSERT(2.0  == mcInputs[1]);
    CPPUNIT_ASSERT(3.0  == mcInputs[2]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS optimization monte carlo manager class updateMasterPost method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::testUpdateMasterPost()
{
    UT_RESULT;

    tArticle->addOptimizer(GunnsOptimFactory::TEST);
    GunnsOptimTest* optimizer = dynamic_cast<GunnsOptimTest*>(tArticle->mOptimizer);

    /// @test updateMaterPost throughputs data from the mc_read buffer to the optimizer.
    std::cout << std::endl;
    const double cost          = 10.0;
    const double runIdReturned = 42.0;
    GunnsInfraFunctions::mMcRead.push_back(runIdReturned);
    GunnsInfraFunctions::mMcRead.push_back(cost);
    tArticle->mRunId             = 12.0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->updateMasterPost());
    CPPUNIT_ASSERT(runIdReturned == tArticle->mRunIdReturned);
    CPPUNIT_ASSERT(cost          == optimizer->mCost);
    CPPUNIT_ASSERT(12.0  == optimizer->mRunId);
    CPPUNIT_ASSERT(42.0  == optimizer->mRunIdReturned);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS optimization monte carlo manager class updateMasterShutdown method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::testUpdateMasterShutdown()
{
    UT_RESULT;

    tArticle->addOptimizer(GunnsOptimFactory::TEST);
    GunnsOptimTest* optimizer = dynamic_cast<GunnsOptimTest*>(tArticle->mOptimizer);

    /// @test updateMaterShutdown updates and shuts down the optimizer.
    std::cout << std::endl;
    CPPUNIT_ASSERT_NO_THROW(tArticle->updateMasterShutdown());
    CPPUNIT_ASSERT(1 == optimizer->mUpdateCounter);
    CPPUNIT_ASSERT(1 == optimizer->mShutdownCounter);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS optimization monte carlo manager class updateSlavePost method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::testUpdateSlavePost()
{
    UT_RESULT;

    /// - Add 2 output variables.
    double value1        = 12.0;
    const double target1 = 13.0;
    const double weight1 = 2.0;
    tArticle->addOutput("output", &value1, target1, weight1);

    double value2        = 3.0;
    const double target2 = 1.0;
    const double weight2 = 0.5;
    tArticle->addOutput("output", &value2, target2, weight2);

    /// - Call updateSlavePre for code coverage; it's just an empty placeholder.
    CPPUNIT_ASSERT_NO_THROW(tArticle->updateSlavePre());

    /// @test updateSlavePost computes the total cost function and writes it to mc_write.
    const double expectedCost1 = weight1 * (value1 - target1) * weight1 * (value1 - target1);
    const double expectedCost2 = weight2 * (value2 - target2) * weight2 * (value2 - target2);
    const double expectedTotalCost = expectedCost1 + expectedCost2;

    CPPUNIT_ASSERT_NO_THROW(tArticle->updateSlavePost());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCost1,     tArticle->mOutputs.at(0).mCost,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCost2,     tArticle->mOutputs.at(1).mCost,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTotalCost, GunnsInfraFunctions::mMcWrite.at(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,               GunnsInfraFunctions::mMcWrite.at(1), DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS optimization monte carlo manager class updateSlaveInputs method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::testUpdateSlaveInputs()
{
    UT_RESULT;

    /// @test addDriver and addDriverDataRow for 2 drivers w/ 3 rows.
    double driverA = 0.0;
    double driverB = 0.0;

    tArticle->addDriver(&driverA);
    tArticle->addDriver(&driverB);

    CPPUNIT_ASSERT(2        == tArticle->mDrivers.size());
    CPPUNIT_ASSERT(&driverA == tArticle->mDrivers.at(0).mAddress);
    CPPUNIT_ASSERT(&driverB == tArticle->mDrivers.at(1).mAddress);
    CPPUNIT_ASSERT(0        == tArticle->mDrivers.at(0).mTrajectory.size());
    CPPUNIT_ASSERT(0        == tArticle->mDrivers.at(1).mTrajectory.size());

    /// - Note that the model requires and expects the 0th term in the data list is a time tag, and
    ///   skips it, and the actual driver values are in the 1st and subsequent columns.
    std::string driversRow0 = "0.0,3.0,12.0";
    std::string driversRow1 = "1.0,4.0,11.0";
    std::string driversRow2 = "2.0,5.0,10.0";

    tArticle->addDriverDataRow(driversRow0);
    tArticle->addDriverDataRow(driversRow1);
    tArticle->addDriverDataRow(driversRow2);

    CPPUNIT_ASSERT(3    == tArticle->mDrivers.at(0).mTrajectory.size());
    CPPUNIT_ASSERT(3    == tArticle->mDrivers.at(1).mTrajectory.size());
    CPPUNIT_ASSERT(3.0  == tArticle->mDrivers.at(0).mTrajectory.at(0));
    CPPUNIT_ASSERT(4.0  == tArticle->mDrivers.at(0).mTrajectory.at(1));
    CPPUNIT_ASSERT(5.0  == tArticle->mDrivers.at(0).mTrajectory.at(2));
    CPPUNIT_ASSERT(12.0 == tArticle->mDrivers.at(1).mTrajectory.at(0));
    CPPUNIT_ASSERT(11.0 == tArticle->mDrivers.at(1).mTrajectory.at(1));
    CPPUNIT_ASSERT(10.0 == tArticle->mDrivers.at(1).mTrajectory.at(2));

    /// @test updateSlaveInputs and Outputs for model step counts within range.
    tArticle->mModelStepCount = 0;
    tArticle->updateSlaveInputs();
    CPPUNIT_ASSERT( 3.0  == driverA);
    CPPUNIT_ASSERT(12.0  == driverB);

    tArticle->mModelStepCount = 1;
    tArticle->updateSlaveInputs();
    CPPUNIT_ASSERT( 4.0  == driverA);
    CPPUNIT_ASSERT(11.0  == driverB);

    tArticle->mModelStepCount = 2;
    tArticle->updateSlaveInputs();
    CPPUNIT_ASSERT( 5.0  == driverA);
    CPPUNIT_ASSERT(10.0  == driverB);

    /// @test updateSlaveInputs for model step counts outside the range.
    tArticle->mModelStepCount = 3;
    tArticle->updateSlaveInputs();
    CPPUNIT_ASSERT( 5.0  == driverA);
    CPPUNIT_ASSERT(10.0  == driverB);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS optimization monte carlo manager class addOutputs, addOutputDataRow,
///           and updateSlaveOutputs methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::testUpdateSlaveOutputs()
{
    UT_RESULT;

    /// @test addOutput for 2 outputs.
    double outputA =  4.0;
    double outputB = 11.0;

    tArticle->addOutput("outputA", &outputA, 0.0, 1.0);
    tArticle->addOutput("outputB", &outputB, 0.0, 0.5);

    CPPUNIT_ASSERT(2         == tArticle->mOutputs.size());
    CPPUNIT_ASSERT("outputA" == tArticle->mOutputs.at(0).mName);
    CPPUNIT_ASSERT("outputB" == tArticle->mOutputs.at(1).mName);
    CPPUNIT_ASSERT(&outputA  == tArticle->mOutputs.at(0).mAddress);
    CPPUNIT_ASSERT(&outputB  == tArticle->mOutputs.at(1).mAddress);
    CPPUNIT_ASSERT(0.0       == tArticle->mOutputs.at(0).mCost);
    CPPUNIT_ASSERT(0.0       == tArticle->mOutputs.at(1).mCost);
    CPPUNIT_ASSERT(true      == tArticle->mOutputs.at(0).mIsScalarTarget);
    CPPUNIT_ASSERT(true      == tArticle->mOutputs.at(1).mIsScalarTarget);
    CPPUNIT_ASSERT(0         == tArticle->mOutputs.at(0).mTargetTraj.size());
    CPPUNIT_ASSERT(0         == tArticle->mOutputs.at(1).mTargetTraj.size());
    CPPUNIT_ASSERT(1.0       == tArticle->mOutputs.at(0).mCostWeight);
    CPPUNIT_ASSERT(0.5       == tArticle->mOutputs.at(1).mCostWeight);

    /// @test addOutputDataRow to add rows to the outputs.
    std::string targetsRow0 = "0.0,3.0,12.0";
    std::string targetsRow1 = "1.0,4.0,11.0";
    std::string targetsRow2 = "2.0,5.0,10.0";

    tArticle->addOutputDataRow(targetsRow0);
    tArticle->addOutputDataRow(targetsRow1);
    tArticle->addOutputDataRow(targetsRow2);

    CPPUNIT_ASSERT(false == tArticle->mOutputs.at(0).mIsScalarTarget);
    CPPUNIT_ASSERT(false == tArticle->mOutputs.at(1).mIsScalarTarget);
    CPPUNIT_ASSERT(3     == tArticle->mOutputs.at(0).mTargetTraj.size());
    CPPUNIT_ASSERT(3     == tArticle->mOutputs.at(1).mTargetTraj.size());
    CPPUNIT_ASSERT(3.0   == tArticle->mOutputs.at(0).mTargetTraj.at(0));
    CPPUNIT_ASSERT(4.0   == tArticle->mOutputs.at(0).mTargetTraj.at(1));
    CPPUNIT_ASSERT(5.0   == tArticle->mOutputs.at(0).mTargetTraj.at(2));
    CPPUNIT_ASSERT(12.0  == tArticle->mOutputs.at(1).mTargetTraj.at(0));
    CPPUNIT_ASSERT(11.0  == tArticle->mOutputs.at(1).mTargetTraj.at(1));
    CPPUNIT_ASSERT(10.0  == tArticle->mOutputs.at(1).mTargetTraj.at(2));

    /// @test model step count and output costs updated by updateSlaveOutputs.
    double expectedCostA = 1.0 * ( 4.0 -  3.0) * 1.0 * ( 4.0 -  3.0);
    double expectedCostB = 0.5 * (11.0 - 12.0) * 0.5 * (11.0 - 12.0);
    tArticle->updateSlaveOutputs();
    CPPUNIT_ASSERT(1 == tArticle->mModelStepCount);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCostA, tArticle->mOutputs.at(0).mCost, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCostB, tArticle->mOutputs.at(1).mCost, DBL_EPSILON);

    expectedCostA += 1.0 * ( 4.0 -  4.0) * 1.0 * ( 4.0 -  4.0);
    expectedCostB += 0.5 * (11.0 - 11.0) * 0.5 * (11.0 - 11.0);
    tArticle->updateSlaveOutputs();
    CPPUNIT_ASSERT(2 == tArticle->mModelStepCount);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCostA, tArticle->mOutputs.at(0).mCost, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCostB, tArticle->mOutputs.at(1).mCost, DBL_EPSILON);

    expectedCostA += 1.0 * ( 4.0 -  5.0) * 1.0 * ( 4.0 -  5.0);
    expectedCostB += 0.5 * (11.0 - 10.0) * 0.5 * (11.0 - 10.0);
    tArticle->updateSlaveOutputs();
    CPPUNIT_ASSERT(3 == tArticle->mModelStepCount);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCostA, tArticle->mOutputs.at(0).mCost, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCostB, tArticle->mOutputs.at(1).mCost, DBL_EPSILON);

    /// @test model step counts beyond the output traj data.
    tArticle->updateSlaveOutputs();
    CPPUNIT_ASSERT(4 == tArticle->mModelStepCount);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCostA, tArticle->mOutputs.at(0).mCost, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCostB, tArticle->mOutputs.at(1).mCost, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS optimization monte carlo input variable constraints.  This only tests
///           a few things not already covered by other tests, to complete code coverage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::testConstraints()
{
    UT_RESULT;

    /// @test GunnsOptimMonteCarloConstraint::evaluate, without the optional Y.
    LinearFit linearFit(1.0, 2.0, 0.0, 1.0); // z = 1 + 2x, x: [0-1]
    const double x         = 0.5;
    double       expectedZ = 1.0 + 2.0 * x;
    GunnsOptimMonteCarloConstraint constraintLinear(&linearFit, &x);
    double z = constraintLinear.evaluate();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedZ, z, DBL_EPSILON);

    /// @test GunnsOptimMonteCarloInput::applyConstraints.
    GunnsOptimMonteCarloInput mcInput;
    mcInput.addNewConstraint(&constraintLinear);
    z = 0.0;
    mcInput.applyConstraints(&z);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedZ, z, DBL_EPSILON);

    /// @test GunnsOptimMonteCarloConstraint::evaluate, with the optional Y.
    ProductFit productFit(2.0, 0.0, 1.0, 0.0, 1.0); // z = 2 * x * y, x: [0-1], y: [0-1]
    const double y = 0.1;
    expectedZ      = 2.0 * x * y;
    GunnsOptimMonteCarloConstraint constraintProduct(&productFit, &x, &y);
    z = constraintProduct.evaluate();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedZ, z, DBL_EPSILON);

    /// @test GunnsOptimBase::constrainInputs throws range error for vector size mismatch.
    tArticle->addOptimizer(GunnsOptimFactory::TEST);
    GunnsOptimTest* optimizer = dynamic_cast<GunnsOptimTest*>(tArticle->mOptimizer);

    std::vector<GunnsOptimMonteCarloInput> mcInputs;
    GunnsOptimMonteCarloInput mcInput1;
    mcInputs.push_back(mcInput1);
    optimizer->initialize(&mcInputs);

    std::vector<double> vars;
    vars.push_back(1.0);
    vars.push_back(2.0);
    CPPUNIT_ASSERT_THROW(optimizer->accessConstrainInputs(vars), std::range_error);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS optimization monte carlo manager class access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsOptimMonteCarlo::testAccessors()
{
    UT_RESULT;

    /// @test setVerbosityLevel, called from setup.
    CPPUNIT_ASSERT(1 == tArticle->mVerbosityLevel);

    /// @test optimizer's getNumRuns.
    tArticle->addOptimizer(GunnsOptimFactory::TEST);
    GunnsOptimTest* optimizer = dynamic_cast<GunnsOptimTest*>(tArticle->mOptimizer);
    optimizer->mNumRuns = 5;
    CPPUNIT_ASSERT(5 == tArticle->mOptimizer->getNumRuns());

    /// @test optimizer's setConfigData.
    GunnsOptimTestConfigData* optimConfig = new GunnsOptimTestConfigData();
    tArticle->mOptimizer->setConfigData(optimConfig);
    CPPUNIT_ASSERT(1 == optimizer->mConfigCounter);
    delete optimConfig;

    UT_PASS_LAST;
}

