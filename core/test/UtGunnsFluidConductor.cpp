/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "UtGunnsFluidConductor.hh"
#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidConductor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidConductor::UtGunnsFluidConductor()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mMaxConductivity(),
    mExpansionScaleFactor(),
    mPressureExponent(),
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
    mFluidInput2(),
    fractions()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidConductor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidConductor::~UtGunnsFluidConductor()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidConductor::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput2;
    delete mFluidInput1;
    delete mFluidConfig;
    delete [] fractions;
    delete mFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidConductor::setUp()
{
    mLinkName             = "Test Fluid Conductor";
    mNodeList.mNumNodes   = 3;
    mNodeList.mNodes      = mNodes;
    mMaxConductivity      = 0.5;
    mExpansionScaleFactor = 0.4;
    mPressureExponent     = 0.5;
    mPort0                = 0;
    mPort1                = 1;
    mTimeStep             = 0.1;
    mTolerance            = 0.01;

    /// - Setup some fluid nodes
    mFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    fractions = new double[2];
    fractions[0] = 0.5;
    fractions[1] = 0.5;
    mFluidConfig = new PolyFluidConfigData(mFluidProperties, types, 2);
    mFluidInput1 = new PolyFluidInputData(290.0,                    //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions
    mFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          689.475,                  //pressure
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
    mConfigData = new GunnsFluidConductorConfigData(
            mLinkName, &mNodeList, mMaxConductivity, mExpansionScaleFactor, mPressureExponent);

    /// - Define nominal input data
    mInputData = new GunnsFluidConductorInputData(true, 0.5);

    mArticle = new FriendlyGunnsFluidConductor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidConductor::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidConductor .. 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName             == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(mMaxConductivity      == mConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(mExpansionScaleFactor == mConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(mPressureExponent     == mConfigData->mPressureExponent);

    /// - Check default config construction
    GunnsFluidConductorConfigData defaultConfig;
    CPPUNIT_ASSERT(""   == defaultConfig.mName);
    CPPUNIT_ASSERT(0    == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0  == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0  == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0.5  == defaultConfig.mPressureExponent);

    /// - Check copy config construction
    GunnsFluidConductorConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName             == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes                == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(mMaxConductivity      == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(mExpansionScaleFactor == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(mPressureExponent     == copyConfig.mPressureExponent);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidConductor::testInput()
{
    std::cout << "\n UtGunnsFluidConductor .. 02: testInput .............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, mInputData->mMalfBlockageValue, 0.0);

    /// - Check default config construction
    GunnsFluidConductorInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mMalfBlockageValue, 0.0);

    /// - Check copy config construction
    GunnsFluidConductorInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidConductor::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidConductor .. 03: testDefaultConstruction ...............";

    /// @test config data
    CPPUNIT_ASSERT(0.0                  == mArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0                  == mArticle->mMaxConductivity);
    CPPUNIT_ASSERT(0.0                  == mArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0                  == mArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(0.0                  == mArticle->mPressureExponent);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT(0.0                  == mArticle->mTuneMassFlow);
    CPPUNIT_ASSERT(0.0                  == mArticle->mTuneVolFlow);
    CPPUNIT_ASSERT(0.0                  == mArticle->mTuneDeltaT);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidConductor* article = new GunnsFluidConductor();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidConductor::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidConductor .. 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidConductor article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test config and input data
    CPPUNIT_ASSERT(mMaxConductivity      == article.mMaxConductivity);
    CPPUNIT_ASSERT(mMaxConductivity      == article.mEffectiveConductivity);
    CPPUNIT_ASSERT(mExpansionScaleFactor == article.mExpansionScaleFactor);
    CPPUNIT_ASSERT(mPressureExponent     == article.mPressureExponent);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF  == article.mTuneMode);
    CPPUNIT_ASSERT(0.0                   == article.mTuneMassFlow);
    CPPUNIT_ASSERT(0.0                   == article.mTuneVolFlow);
    CPPUNIT_ASSERT(0.0                   == article.mTuneDeltaT);

    /// - Verify the parent method is called
    CPPUNIT_ASSERT(mLinkName == article.getName());
    CPPUNIT_ASSERT(mInputData->mMalfBlockageValue == article.mMalfBlockageValue);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidConductor::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidConductor .. 05: testInitializationExceptions ..........";

    /// @test for exception on conductivity < 0
    mConfigData->mMaxConductivity = -0.4;
    CPPUNIT_ASSERT_THROW(mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mMaxConductivity = mMaxConductivity;

    /// @test for exception on expansion scale factor < 0
    mConfigData->mExpansionScaleFactor = -0.3;
    CPPUNIT_ASSERT_THROW(mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    /// @test for exception on expansion scale factor > 1
    mConfigData->mExpansionScaleFactor = 1.1;
    CPPUNIT_ASSERT_THROW(mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mExpansionScaleFactor = 0.0;

    /// @test for exception on pressure exponent < 0.5
    mConfigData->mPressureExponent = 0.4;
    CPPUNIT_ASSERT_THROW(mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    /// @test for exception on pressure exponent > 1
    mConfigData->mPressureExponent = 1.1;
    CPPUNIT_ASSERT_THROW(mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mPressureExponent = 0.5;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidConductor::testStep()
{
    std::cout << "\n UtGunnsFluidConductor .. 06: testStep ..............................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);

    /// - Conductor should always have a positive admittance and zero potential
    CPPUNIT_ASSERT(mArticle->mAdmittanceMatrix[0] > 0.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * 0.5, mArticle->mEffectiveConductivity, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mSourceVector[0], mTolerance);

    /// - Step again with a modified max conductivity and verify the effective conductivity responds.
    mArticle->mMaxConductivity = mMaxConductivity * 2.0;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity, mArticle->mEffectiveConductivity, mTolerance);
    CPPUNIT_ASSERT(mArticle->mConductanceLimit >= mArticle->linearizeConductance());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidConductor::testComputeFlows()
{
    std::cout << "\n UtGunnsFluidConductor .. 07: testComputeFlows ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Confirm correct source port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[0]);

    /// - Confirm correct source port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == mArticle->mPortDirections[1]);

    mArticle->transportFlows(mTimeStep);

    /// - Conductance Link should have zero flow rate because the potential vector is zero
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mFlowRate, mTolerance);

    /// - Confirm mVolFlowRate is zero with a zero potential vector (TransportFlows)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mVolFlowRate, mTolerance);

    /// - Set up positive flow (0-->1) potential vector
    mArticle->mPotentialVector[0] = 0.8;
    mArticle->computeFlows(mTimeStep);

    /// - Molar flux should be greater than zero because the port 0 potential vector is positive
    CPPUNIT_ASSERT( mArticle->mFlux > 0.0);

    /// - Confirm correct source port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == mArticle->mPortDirections[0]);

    /// - Confirm correct source port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == mArticle->mPortDirections[1]);

    /// - Nodal outflux scheduling should be equal to step molar flux for source node
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getScheduledOutflux(),mArticle->mFlux,DBL_EPSILON);

    mArticle->transportFlows(mTimeStep);

    /// - Confirm correct source port selection with Postive potential Vector (TransportFlows)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, mArticle->determineSourcePort(mArticle->mFlux, 0, 1), DBL_EPSILON);

    /// - Conductance Link should have positive flow rate because the port 0 potential vector is positive
    CPPUNIT_ASSERT(mArticle->mFlowRate > 0.0);

    mArticle->mPotentialVector[0] = -0.6;
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

    /// - Confirm correct source port selection with negative potential Vector (TrasportFlows)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, mArticle->determineSourcePort(mArticle->mFlux, 0, 1),
                                                            DBL_EPSILON);

    /// - Conductance Link should have negative flow rate because the port 0 potential vector is negative
    CPPUNIT_ASSERT(mArticle->mFlowRate < 0.0);

    /// - Verify volumetric flow rate and hydraulic power
    const double expectedVolFlow = mArticle->mFlowRate / mNodes[1].getOutflow()->getDensity();
    const double expectedPower = -1000.0 * expectedVolFlow *
            (mArticle->mPotentialVector[0] - mArticle->mPotentialVector[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolFlow, mArticle->mVolFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,   mArticle->mPower,       DBL_EPSILON);

    /// - Verify zero flow when delta-pressure is very small.
    mArticle->mPotentialVector[0] = 1.0;
    mArticle->mPotentialVector[1] = 1.0 + 10.0*DBL_EPSILON;
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    CPPUNIT_ASSERT(0.0 == mArticle->mFlux);
    CPPUNIT_ASSERT(0.0 == mArticle->mFlowRate);
    CPPUNIT_ASSERT(0.0 == mArticle->mVolFlowRate);

    /// - Verify zero flow when all pressures are zero.
    mArticle->mPotentialVector[0] = 0.0;
    mArticle->mPotentialVector[1] = 0.0;
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    CPPUNIT_ASSERT(0.0 == mArticle->mFlux);
    CPPUNIT_ASSERT(0.0 == mArticle->mFlowRate);
    CPPUNIT_ASSERT(0.0 == mArticle->mVolFlowRate);

    /// - Verify volumetric flow rate when the source density is zero
    mArticle->mPotentialVector[0] = -0.6;
    mNodes[1].resetContentState();
    mNodes[1].resetFlows();
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);
    CPPUNIT_ASSERT(0.0 == mArticle->mVolFlowRate);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidConductor::testComputeFlowsWithInternalFluid()
{
    std::cout << "\n UtGunnsFluidConductor .. 08: testComputeFlowsWithInternalFluid .....";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->createInternalFluid();
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Conductance Link should have zero flow rate because the potential vector is zero
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mFlowRate, mTolerance);

    mArticle->mPotentialVector[0] = 0.8;
    mArticle->computeFlows(mTimeStep);
    mNodes[0].getContent()->setTemperature(10.0);
    mNodes[0].getContent()->setPressure(10.0);
    mNodes[0].resetFlows();
    mNodes[0].getContent()->setTemperature(290.0);
    mNodes[0].getContent()->setPressure(700.728);
    /// - Check to ensure that content and outflow conditions are different
    CPPUNIT_ASSERT(mNodes[0].getOutflow()->getPressure()!= mNodes[0].getContent()->getPressure());
    mArticle->mExpansionScaleFactor  = 0.0;
    mArticle->transportFlows(mTimeStep);

    /// - Internal Fluid should be at same pressure as outflow with zero Expansion Scale Factor
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getOutflow()->getPressure(),mArticle->getInternalFluid()->
                                                 getPressure(),mTolerance);

   /// - Internal Fluid should be at same temperature as outflow with zero Expansion Scale Factor
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getOutflow()->getTemperature(),mArticle->
                                                 getInternalFluid()->getTemperature(),mTolerance);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Conductance Link should have positive flow rate because the port 0 potential vector is positive
    CPPUNIT_ASSERT(mArticle->mFlowRate > 0.0);

    mArticle->mPotentialVector[0] = -0.6;
    mArticle->computeFlows(mTimeStep);
    mNodes[1].getContent()->setTemperature(20.0);
    mNodes[1].getContent()->setPressure(20.0);
    mNodes[1].resetFlows();
    mNodes[1].getContent()->setTemperature(283.15);
    mNodes[1].getContent()->setPressure(689.475);

    /// - Check to ensure that content and outflow conditions are different
    CPPUNIT_ASSERT(mNodes[1].getOutflow()->getPressure() !=
                                                 mNodes[1].getContent()-> getPressure());

    mArticle->transportFlows(mTimeStep);

    /// - Internal Fluid should be at same pressure as outflow with zero Expansion Scale Factor
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[1].getOutflow()->getPressure(),mArticle->getInternalFluid()->
                                                getPressure(),mTolerance);

   /// - Internal Fluid should be at same temperature as outflow with zero Expansion Scale Factor
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[1].getOutflow()->getTemperature(),mArticle->
                                                getInternalFluid()->getTemperature(),mTolerance);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Conductance Link should have negative flow rate because the port 0 potential vector is negative
    CPPUNIT_ASSERT(mArticle->mFlowRate < 0.0);

    /// - Test a case where the inlet node is vacuum on port 0 and verify it does not affect the
    ///   internal fluid  //TODO
//    mArticle->mUserPortSelect     = 0;
//    mArticle->mUserPortSelectNode = 1;
//    mArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
//    mArticle->step(mTimeStep);
//
//    mArticle->mUserPortSelect     = 1;
//    mArticle->mUserPortSelectNode = 0;
//    mArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
//    mArticle->step(mTimeStep);
//
//    mArticle->mPotentialVector[0] =  0.0;
//    mArticle->mPotentialVector[1] = -0.6;
//    mArticle->computeFlows(mTimeStep);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test tuning the link conductivity and expansion scale factor.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidConductor::testTuning()
{
    std::cout << "\n UtGunnsFluidConductor .. 09: testTuning ............................";

    /// - Initialize default test article with nominal initialization data
    mInputData->mMalfBlockageFlag = false;
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mNodes[0].setPotential(mNodes[0].getContent()->getPressure());
    mNodes[1].setPotential(mNodes[1].getContent()->getPressure());
    mArticle->mPotentialVector[0] = mNodes[0].getPotential();
    mArticle->mPotentialVector[1] = mNodes[1].getPotential();
    double newT = GunnsFluidUtils::computeIsentropicTemperature(0.4, mNodes[0].getPotential(),
                                                                     mNodes[1].getPotential(),
                                                                     mNodes[0].getContent());
    mNodes[1].getContent()->setTemperature(newT);

    /// - Step once to get the default mdot and dT
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    double defaultConductivity = mArticle->mEffectiveConductivity;
    double defaultScaleFactor  = mArticle->mExpansionScaleFactor;

    /// - Set the controls to tune the conductivity and expansion scale factor to half their default
    ///   values.

    /// - Tune the link to half its default mass flow rate.
    const double defaultMdot = mArticle->mFlowRate;
    mArticle->mTuneMassFlow = fabs(0.5 * defaultMdot);
    mArticle->mTuneMode = GunnsFluidUtils::MASS;
    mArticle->step(mTimeStep);

    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * defaultConductivity, mArticle->mEffectiveConductivity,
                                                            FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * defaultConductivity, mArticle->mMaxConductivity,
                                                            FLT_EPSILON);

    /// - Tune the link to double its default true volumetric flow rate for positive flow.
    double defaultVdot = mArticle->mFlowRate / mNodes[0].getContent()->getDensity();
    mArticle->mTuneVolFlow = 2.0 * defaultVdot;
    mArticle->mTuneMode = GunnsFluidUtils::TRUE_VOL;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0 * defaultConductivity, mArticle->mEffectiveConductivity,
                                                           FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0 * defaultConductivity, mArticle->mMaxConductivity,
                                                            FLT_EPSILON);

    /// - Set up the nodes to create backflow and re-adjust the link to the new pressures.
    mNodes[0].setPotential(675.0);
    mArticle->mPotentialVector[0] = 675.0;
    mArticle->mMaxConductivity = mMaxConductivity;
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Tune the link to half its default true volumetric flow rate for negative flow.
    defaultVdot = mArticle->mFlowRate / mNodes[1].getContent()->getDensity();

    mArticle->mTuneVolFlow = 0.5 * defaultVdot;
    mArticle->mTuneMode = GunnsFluidUtils::TRUE_VOL;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * defaultConductivity, mArticle->mEffectiveConductivity,
                                                            FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * defaultConductivity, mArticle->mMaxConductivity,
                                                            FLT_EPSILON);

    /// - Restore the nodes to their original state and re-adjust the link again.
    mNodes[0].setPotential(700.728);
    mArticle->mPotentialVector[0] = 700.728;
    mArticle->mMaxConductivity = mMaxConductivity;
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Tune the link to double its default standard volumetric flow rate for positive flow, using
    ///   1.225 kg/m3 for the standard air density.
    const double defaultVdotStd = mArticle->mFlowRate / UnitConversion::STD_DENSITY_AIR;    //1.225
    mArticle->mTuneVolFlow = 2.0 * defaultVdotStd;
    mArticle->mTuneMode = GunnsFluidUtils::STD_VOL;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0 * defaultConductivity, mArticle->mEffectiveConductivity,
                                                            FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0 * defaultConductivity, mArticle->mMaxConductivity,
                                                           FLT_EPSILON);

    /// - Tune the link to half its default expansion scale factor.
    const double defaultdT = mArticle->mNodes[1]->getContent()->getTemperature() -
                             mArticle->mNodes[0]->getContent()->getTemperature();
    mArticle->mTuneDeltaT = 0.5 * defaultdT;
    mArticle->mTuneMode = GunnsFluidUtils::DELTA_T;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT(GunnsFluidUtils::OFF == mArticle->mTuneMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * defaultScaleFactor,  mArticle->mExpansionScaleFactor,
                                                            FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidConductor::testAccessMethods()
{
    std::cout << "\n UtGunnsFluidConductor .. 10: testAccessMethods .....................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);

    /// - Test Access Methods
    mArticle->mEffectiveConductivity = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getEffectiveConductivity(), 0.0);

    std::cout << "... Pass";
}
