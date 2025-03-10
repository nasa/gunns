/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (../GunnsLosslessSource.o)
    )
***************************************************************************************************/
#include "UtGunnsLosslessSource.hh"

int UtGunnsLosslessSource::TEST_ID = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsLosslessSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsLosslessSource::UtGunnsLosslessSource()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mInitialDemand(),
    mNodes(),
    mNodeList(),
    mLinks(),
    mPort0(),
    mPort1(),
    mTimeStep(),
    mTolerance()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsLosslessSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsLosslessSource::~UtGunnsLosslessSource()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mEffTable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::setUp()
{
    mLinkName             = "Test Lossless Source";
    mNodeList.mNumNodes   = 2;
    mNodeList.mNodes      = mNodes;
    mInitialDemand        = 0.5;
    mEfficiency           = 1.0;
    mPort0                = 0;
    mPort1                = 1;

    this->xArr[0] = 1.0;
    this->xArr[1] = 2.0;
    this->yArr[0] = 0.1;
    this->yArr[1] = 0.9;
    mEffTable = new TsLinearInterpolator(xArr, yArr, this->nArr, xArr[0], xArr[1]);

    mDefReferencePower = 1.0;
    mDefStaticEff = true;

    /// - Define nominal configuration data
    mConfigData = new GunnsLosslessSourceConfigData(mLinkName, &mNodeList, this->mEffTable);

    /// - Define nominal input data
    mInputData = new GunnsLosslessSourceInputData(true, 0.5, mInitialDemand, mEfficiency, mDefReferencePower, mDefStaticEff);

    /// - Create the test article
    mArticle = new FriendlyGunnsLosslessSource;

    /// - Declare the nominal test data
    mTolerance = 1.0e-08;
    mTimeStep = 0.1;

    TEST_ID ++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testConfig()
{
    UT_RESULT;
    // std::cout << "\n -----------------------------------------------------------------------------";
    // std::cout << "\n UtGunnsLosslessSource ..... 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes    == mConfigData->mNodeList->mNodes);

    /// - Check default config construction
    GunnsLosslessSourceConfigData defaultConfig;
    CPPUNIT_ASSERT(""        == defaultConfig.mName);
    CPPUNIT_ASSERT(0         == defaultConfig.mNodeList);

    /// - Check copy config construction
    GunnsLosslessSourceConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes    == copyConfig.mNodeList->mNodes);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testInput()
{
    UT_RESULT;

    /// - Check nominal input construction
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5            == mInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(mInitialDemand == mInputData->mSourceFlux);

    /// - Check default input construction
    GunnsLosslessSourceInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0 == defaultInput.mSourceFlux);

    /// - Check copy input construction
    GunnsLosslessSourceInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(mInitialDemand                 == copyInput.mSourceFlux);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testDefaultConstruction()
{
    UT_RESULT;

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mSourceFlux,
                                 mTolerance);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsLosslessSource* article = new GunnsLosslessSource();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testNominalInitialization()
{
    UT_RESULT;

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsLosslessSource article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,            article.mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialDemand, article.mSourceFlux,        0.0);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Initialize the nodes potentials
    mNodes[0].setPotential(100.0);
    mNodes[1].setPotential(  0.0);
    mArticle->mPotentialVector[0] = 100.0;
    mArticle->mPotentialVector[1] =   0.0;

    mArticle->step(mTimeStep);

    /// - Not using the malfunction flag now, may change this in the future
    // /// - during step the source Vector will be equal to the source flux times the blockage malf.
    // CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mSourceFlux * 0.5,
    //                              mArticle->mSourceVector[1], DBL_EPSILON);

    /** This is the change from GunnsBasicSource
     * Potential*flux = "Power"
     * For conservation of power we expect Pot_in*flux_in = Pot_out*flux_out
     * We set flux_out, so solving for flux_in:
     * flux_in = (Pot_out/Pot_in)*flux_out
     */
    double expected_influx = std::fabs(mNodes[1].getPotential()/mNodes[0].getPotential()*mArticle->mSourceFlux);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_influx,
        mArticle->mSourceVector[0],
        DBL_EPSILON
    );


    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Initialize the nodes potentials at something more interesting
    mNodes[0].setPotential(600.0);
    mNodes[1].setPotential(135.0);
    mArticle->mPotentialVector[0] = 600.0;
    mArticle->mPotentialVector[1] = 135.0;

    mArticle->step(mTimeStep);

    expected_influx = std::fabs(mNodes[1].getPotential()/mNodes[0].getPotential()*mArticle->mSourceFlux);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1*expected_influx,
        mArticle->mSourceVector[0],
        DBL_EPSILON
    );

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testLosslessComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    // NOTE_ What should the link do if there's 0V on the output? Seems like there should be 0 current too?
    {
    /// - Test 1: 100V input, 0V output
    /// - Initialize the nodes potentials
    mNodes[0].setPotential(100.0);
    mNodes[1].setPotential(  0.0);
    mArticle->mPotentialVector[0] = 100.0;
    mArticle->mPotentialVector[1] =   0.0;
    mArticle->mMalfBlockageFlag = false;

    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Check potential drop and power across the link is updated
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getPotential() - mNodes[1].getPotential(),
        mArticle->mPotentialDrop,
        DBL_EPSILON
    );

    /// - The power across this link should always be near 0.0
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPower, DBL_EPSILON);

    /// - Check flux is transported to/from the nodes
    double expected_influx = std::fabs(mNodes[1].getPotential()/mNodes[0].getPotential()*mInitialDemand);
    double expected_outflux = std::fabs(mInitialDemand);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_outflux, mNodes[1].getInflux(),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_influx, mNodes[0].getOutflux(), 0.0);
    }

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();


    {
    /// - Test 2: 100V input, 13V output
    /// - Initialize the nodes potentials
    mNodes[0].setPotential(100.0);
    mNodes[1].setPotential(  13.0);
    mArticle->mPotentialVector[0] = 100.0;
    mArticle->mPotentialVector[1] =  13.0;
    mArticle->mMalfBlockageFlag = false;

    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Check potential drop and power across the link is updated
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getPotential() - mNodes[1].getPotential(),
        mArticle->mPotentialDrop,
        DBL_EPSILON
    );

    /// - The power across this link should always be near 0.0
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPower, DBL_EPSILON);

    /**
     * 0.5A*13V = 6.5W
     * 6.5W/100V = 0.0065A
     * mNodes[0].getOutflux() should give us 0.0065
    */

    /// - Check flux is transported to/from the nodes
    double expected_influx = std::fabs(mNodes[1].getPotential()/mNodes[0].getPotential()*mInitialDemand);
    double expected_outflux = std::fabs(mInitialDemand);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_outflux, mNodes[1].getInflux(),  0.0); // Exact Assignment
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_influx, mNodes[0].getOutflux(), DBL_EPSILON); // Calculated Value
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testSetters()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Set the source flux demand using the setter method and verify.
    mArticle->setFluxDemand(1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mArticle->mSourceFlux, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testAccessMethods()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    CPPUNIT_ASSERT(mArticle->isInitialized());
    /// - Get the source flux demand using the getter method and verify.
    mArticle->setFluxDemand(5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getFluxDemand(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mArticle->mEfficiency, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mArticle->getEfficiency(), 0.0);

    UT_PASS;
}

void UtGunnsLosslessSource::testInefficiencyAndWaste()
{
    UT_RESULT;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mInputData->mEfficiency, 0.0);
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    double startEff = this->mArticle->getEfficiency();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, startEff, 0.0);
    double lowerEff = 0.9;
    this->mArticle->setEfficiency(lowerEff);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(lowerEff, this->mArticle->getEfficiency(), 0.0);

    this->mInputData->mEfficiency = lowerEff;
    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    {
    /// - Test 1: 100V input, 10V output
    /// - Initialize the nodes potentials
    /// - Test initialDemand = 0.5
    mNodes[0].setPotential(100.0);
    mNodes[1].setPotential( 10.0);
    mArticle->mPotentialVector[0] = 100.0;
    mArticle->mPotentialVector[1] =  10.0;
    mArticle->mMalfBlockageFlag = false;

    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Check potential drop and power across the link is updated
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getPotential() - mNodes[1].getPotential(),
        mArticle->mPotentialDrop,
        DBL_EPSILON
    );

    /// - The power across this link should be PowerThrough*(1-Efficiency)
    double powerIn = mNodes[0].getPotential() * mArticle->getSourceVector()[0];
    double powerOut = mNodes[1].getPotential() * mArticle->getSourceVector()[1];
    double expectedWastePower = powerIn*(1.0 - lowerEff);

    /// - Check "Power values"
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWastePower, mArticle->mPower, DBL_EPSILON*10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWastePower, powerIn + powerOut, DBL_EPSILON*10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mPower, mArticle->getWastePower(), DBL_EPSILON);
    CPPUNIT_ASSERT_GREATER(0.0, std::fabs(mArticle->mPower));

    /// - Check correct flux is transported to/from the nodes
    /// This uses efficiency term now
    double expected_influx = std::fabs(mNodes[1].getPotential()/mNodes[0].getPotential()) *mInitialDemand/mArticle->getEfficiency();
    double expected_outflux = std::fabs(mInitialDemand);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_outflux, mNodes[1].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_influx, mNodes[0].getOutflux(), DBL_EPSILON);
    }

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();


    { // Now test dynamic efficiency, by set static efficiency = false
        this->mArticle->mStaticEfficiency = false;
        mArticle->step(mTimeStep);
        mArticle->computeFlows(mTimeStep);

        /// - The power across this link should be PowerThrough*(1-Efficiency)
        double powerIn = mNodes[0].getPotential() * mArticle->getSourceVector()[0];
        double powerOut = mNodes[1].getPotential() * mArticle->getSourceVector()[1];
        double expectedEff = mArticle->mEfficiencyTable->get(powerOut);
        double expectedWastePower = powerIn*(1.0 - expectedEff);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEff, mArticle->getEfficiency(), DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWastePower , mArticle->mWastePower, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedWastePower, powerIn+powerOut, DBL_EPSILON);
    }

    UT_PASS;
}