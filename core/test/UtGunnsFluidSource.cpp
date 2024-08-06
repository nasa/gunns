/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
(
   (core/GunnsFluidSource.o)
)
***************************************************************************************************/
#include "UtGunnsFluidSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSource::UtGunnsFluidSource()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mInitialFlowDemand(),
    mNodes(),
    mNodeList(),
    mLinks(),
    mPort0(),
    mPort1(),
    mTimeStep(),
    mTolerance(),
    mFluidProperties(),
    mFluidConfig(),
    mFluidInput1(),
    mFluidInput2()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSource::~UtGunnsFluidSource()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSource::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput2;
    delete mFluidInput1;
    delete mFluidConfig;
    delete mFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSource::setUp()
{
    mLinkName             = "Test Fluid Source";
    mNodeList.mNumNodes   = 3;
    mNodeList.mNodes      = mNodes;
    mInitialFlowDemand    = 0.5;
    mPort0                = 0;
    mPort1                = 1;
    mTimeStep             = 0.1;
    mTolerance            = 0.01;

    /// - Setup some fluid nodes
    mFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    double fractions[2];
    fractions[0] = 0.5;
    fractions[1] = 0.5;
    mFluidConfig = new PolyFluidConfigData(mFluidProperties, types, 2);
    mFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions
    mFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          689.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    mNodes[0].initialize("UtTestNode0", mFluidConfig);
    mNodes[1].initialize("UtTestNode1", mFluidConfig);

    mNodes[0].getContent()->initialize(*mFluidConfig, *mFluidInput1);
    mNodes[1].getContent()->initialize(*mFluidConfig, *mFluidInput2);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Define nominal configuration data
    mConfigData = new GunnsFluidSourceConfigData(mLinkName, &mNodeList);

    /// - Define default input data
    mInputData = new GunnsFluidSourceInputData(true, 0.5, mInitialFlowDemand);

    mArticle = new FriendlyGunnsFluidSource;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSource::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidSource ..... 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName             == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                == mConfigData->mNodeList->mNodes);

    /// - Check default config construction
    GunnsFluidSourceConfigData defaultConfig;
    CPPUNIT_ASSERT(""   == defaultConfig.mName);
    CPPUNIT_ASSERT(0    == defaultConfig.mNodeList);

    /// - Check copy config construction
    GunnsFluidSourceConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName             == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes                == copyConfig.mNodeList->mNodes);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSource::testInput()
{
    std::cout << "\n UtGunnsFluidSource ..... 02: testInput .............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5                == mInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(mInitialFlowDemand == mInputData->mFlowDemand);

    /// - Check default config construction
    GunnsFluidSourceInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                == defaultInput.mFlowDemand);

    /// - Check copy config construction
    GunnsFluidSourceInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(mInitialFlowDemand             == copyInput.mFlowDemand);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSource::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidSource ..... 03: testDefaultConstruction ...............";

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mFlowDemand,
                                 mTolerance);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidSource* article = new GunnsFluidSource();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSource::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidSource ..... 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidSource article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - There is currently nothing to validate
    article.validate();

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialFlowDemand, article.mFlowDemand, mTolerance);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSource::testAccessorsAndGetters()
{
    std::cout << "\n UtGunnsFluidSource ..... 05: testAccessorsAndGetters ...............";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->createInternalFluid();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialFlowDemand, mArticle->mFlowDemand, mTolerance);

    mArticle->setFlowDemand(0.1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, mArticle->mFlowDemand, mTolerance);

    PolyFluid* testFluid = mNodes[0].getContent();

    mArticle->setFlowState(testFluid);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getContent()->getMass(),
            mArticle->mInternalFluid->getMass(), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSource::testStep()
{
    std::cout << "\n UtGunnsFluidSource ..... 06: testStep ..............................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - during step the flow rate will be equal to the flow demand for a Fluid Source, reduced by
    ///   the blockage malfunction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialFlowDemand * 0.5, mArticle->mFlowRate, mTolerance);

    /// - Step again with the blockage malfunction de-activated.
    mArticle->mMalfBlockageFlag = false;
    mArticle->step(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialFlowDemand, mArticle->mFlowRate, mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSource::testComputeFlows()
{
    std::cout << "\n UtGunnsFluidSource ..... 07: testComputeFlows ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mMalfBlockageFlag = false;
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Molar flux should be greater than zero because the port 0 potential vector is positive
    CPPUNIT_ASSERT( mArticle->mFlux > 0.0);

    /// - Confirm correct source port allocation with postive flow demand (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == mArticle->mPortDirections[0]);

    /// - Confirm correct source port allocation with positive flow demand (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == mArticle->mPortDirections[1]);

    /// - Nodal outflux scheduling should be equal to step molar flux for source node
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getScheduledOutflux(),mArticle->mFlux,DBL_EPSILON);

    mArticle->transportFlows(mTimeStep);

    /// - Confirm correct source port selection with Postive potential Vector (TransportFlows)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, mArticle->determineSourcePort(mArticle->mFlux, 0, 1),
                                                                        DBL_EPSILON);

    /// - Since a basic source is a boundary condition the flow rate will be equal to the flow
    /// - demand
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialFlowDemand, mArticle->mFlowRate, mTolerance);

    /// - Test with a negative demand
    mArticle->setFlowDemand(-1.0);
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Molar flux should be less than zero because the port 0 potential vector is negative
    CPPUNIT_ASSERT( mArticle->mFlux < 0.0);

    /// - Confirm correct source port allocation with negative potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == mArticle->mPortDirections[0]);

    /// - Confirm correct source port allocation with negative potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == mArticle->mPortDirections[1]);

    /// - Nodal outflux scheduling should be equal to step molar flux for source node
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[1].getScheduledOutflux(),- mArticle->mFlux,DBL_EPSILON);

    mArticle->transportFlows(mTimeStep);

    /// - Confirm correct source port selection with Postive potential Vector (TransportFlows)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, mArticle->determineSourcePort(mArticle->mFlux, 0, 1),
                                                                        DBL_EPSILON);

    /// - Since a basic source is a boundary condition the flow rate will be equal to the flow
    /// - demand
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getFlowDemand(), mArticle->mFlowRate, mTolerance);


    /// - Test with a zero demand
    mArticle->setFlowDemand(0.0);
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Molar flux should equal zero because the port 0 potential vector is negative
    CPPUNIT_ASSERT( mArticle->mFlux == 0.0);

   /// - Confirm correct source port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[0]);

    /// - Confirm correct source port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[1]);

    mArticle->transportFlows(mTimeStep);

    /// - Conductance Link should have zero flow rate because the potential vector is zero
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mFlowRate, mTolerance);

    /// - Confirm mVolFlowRate is zero with a zero potential vector (TransportFlows)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mVolFlowRate, mTolerance);

    /// - Since a basic source is a boundary condition the flow rate will be equal to the flow
    /// - demand
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->getFlowDemand(), mArticle->mFlowRate, mTolerance);

    /// - Verify volumetric flow rate and hydraulic power
    const double expectedVolFlow = mArticle->mFlowRate / mNodes[1].getOutflow()->getDensity();
    const double expectedPower = -1000.0 * expectedVolFlow *
            (mArticle->mPotentialVector[0] - mArticle->mPotentialVector[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolFlow, mArticle->mVolFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,   mArticle->mPower,       DBL_EPSILON);

    /// - Verify volumetric flow rate when the source density is zero
    mNodes[0].resetContentState();
    mNodes[0].resetFlows();
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    CPPUNIT_ASSERT(0.0 == mArticle->mVolFlowRate);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSource::testComputeFlowsWithInternalFluid()
{
    std::cout << "\n UtGunnsFluidSource ..... 08: testComputeFlowsWithInternalFluid .....";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->createInternalFluid();
    mArticle->setFlowDemand(-1.0);
    mArticle->mMalfBlockageFlag = false;
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Since a basic source is a boundary condition the flow rate will be equal to the flow
    /// - demand
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, mArticle->mFlowRate, mTolerance);

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->setFlowDemand(-1.0);
    mArticle->mMalfBlockageFlag = false;
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    double fractions[2];
    fractions[0] = 0.1;
    fractions[1] = 0.9;
    mNodes[1].getContent()->initialize(*mFluidConfig,(PolyFluidInputData(283.15,        //temperature
                                          689.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions)));               //massFractions);
    mNodes[1].resetFlows();
    fractions[0] = 0.5;
    fractions[1] = 0.5;
    mNodes[1].getContent()->setMassAndMassFractions(1.0,fractions);

    /// - Check to ensure that content and outflow conditions are different
    CPPUNIT_ASSERT(mNodes[1].getOutflow()->getMWeight() !=
                                                 mNodes[1].getContent()-> getMWeight());

    mArticle->transportFlows(mTimeStep);
    double tempMFlux = mArticle-> mFlowRate / mNodes[1].getOutflow()->getMWeight();

    /// - mFlux should be same as calculated mFlux if mInternalFluid is NULL
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempMFlux,mArticle->mFlux,mTolerance);

    std::cout << "... Pass";
}
