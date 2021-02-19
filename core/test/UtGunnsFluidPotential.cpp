/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (core/GunnsFluidPotential.o)
    )
***************************************************************************************************/
#include "UtGunnsFluidPotential.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidPotential class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidPotential::UtGunnsFluidPotential()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mMaxConductivity(),
    mExpansionScaleFactor(),
    mSourcePressure(),
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
    mFractions()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidPotential class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidPotential::~UtGunnsFluidPotential()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPotential::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput2;
    delete mFluidInput1;
    delete mFluidConfig;
    delete [] mFractions;
    delete mFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPotential::setUp()
{
    mLinkName             = "Test Fluid Potential";
    mNodeList.mNumNodes   = 2;
    mNodeList.mNodes      = mNodes;
    mMaxConductivity      = 0.5;
    mExpansionScaleFactor = 0.0;
    mSourcePressure       = 600.0;
    mPort0                = 0;
    mPort1                = 1;
    mTimeStep             = 0.1;
    mTolerance            = 0.01;

    /// - Setup some fluid nodes
    mFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    mFractions = new double[2];
    mFractions[0] = 0.5;
    mFractions[1] = 0.5;
    mFluidConfig = new PolyFluidConfigData(mFluidProperties, types, 2);
    mFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          mFractions);              //massFractions
    mFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          689.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          mFractions);              //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    mNodes[0].initialize("UtTestNode0", mFluidConfig);
    mNodes[1].initialize("UtTestNode1", mFluidConfig);

    mNodes[0].getContent()->initialize(*mFluidConfig, *mFluidInput1);
    mNodes[1].getContent()->initialize(*mFluidConfig, *mFluidInput2);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Define nominal configuration data
    mConfigData = new GunnsFluidPotentialConfigData(
            mLinkName, &mNodeList, mMaxConductivity, mExpansionScaleFactor);

    /// - Define nominal input data
    mInputData = new GunnsFluidPotentialInputData(true, 0.5, mSourcePressure);

    mArticle = new FriendlyGunnsFluidPotential;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPotential::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidPotential .. 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName             == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(mMaxConductivity      == mConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(mExpansionScaleFactor == mConfigData->mExpansionScaleFactor);

    /// - Check default config construction
    GunnsFluidPotentialConfigData defaultConfig;
    CPPUNIT_ASSERT(""   == defaultConfig.mName);
    CPPUNIT_ASSERT(0    == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0  == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0  == defaultConfig.mExpansionScaleFactor);

    /// - Check copy config construction
    GunnsFluidPotentialConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName             == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes                == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(mMaxConductivity      == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(mExpansionScaleFactor == copyConfig.mExpansionScaleFactor);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPotential::testInput()
{
    std::cout << "\n UtGunnsFluidPotential .. 02: testInput .............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5             == mInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(mSourcePressure == mInputData->mSourcePressure);

    /// - Check default config construction
    GunnsFluidPotentialInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0             == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0             == defaultInput.mSourcePressure);

    /// - Check copy config construction
    GunnsFluidPotentialInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(mSourcePressure                == copyInput.mSourcePressure);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPotential::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidPotential .. 03: testDefaultConstruction ...............";

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mSourcePressure,
                                 mTolerance);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidPotential* article = new GunnsFluidPotential();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPotential::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidPotential .. 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidPotential article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test config and input data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSourcePressure, article.mSourcePressure, mTolerance);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test initialization exceptions
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPotential::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidPotential .. 05: testInitializationExceptions ..........";

    /// - There is currently nothing to check.
    mArticle->validate();

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPotential::testAccessorsAndSetters()
{
    std::cout << "\n UtGunnsFluidPotential .. 06: testAccessorsAndSetters ...............";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    mArticle->setSourcePressure(-0.33);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.33, mArticle->mSourcePressure, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.33, mArticle->getSourcePressure(), 0.0);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPotential::testStep()
{
    std::cout << "\n UtGunnsFluidPotential .. 07: testStep ..............................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);

    /// - Potential was set up with a positive source pressure so the source vector and admittance
    ///   will be greater than zero
    CPPUNIT_ASSERT(mArticle->mAdmittanceMatrix[0] > 0.0 && mArticle->mSourceVector[0] < 0.0);

    /// - Change the max conductivity and verify a new step changes the effective conductivity.
    mArticle->mMaxConductivity = mMaxConductivity * 2.0;
    mArticle->step(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity, mArticle->mEffectiveConductivity, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPotential::testComputeFlows()
{
    std::cout << "\n UtGunnsFluidPotential .. 08: testComputeFlows ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Potential Link should have positive flow rate because the source pressure is positive
    CPPUNIT_ASSERT(mArticle->mFlowRate > 0.0);
    mArticle->mPotentialVector[0] = -800.0;
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Potential Link should have negative flow rate because the port 0 potential vector is negative
    CPPUNIT_ASSERT(mArticle->mFlowRate < 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPotential::testComputeFlowsWithInternalFluid()
{
    std::cout << "\n UtGunnsFluidPotential .. 09: testComputeFlowsWithInternalFluid .....";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->createInternalFluid();
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Potential Link should have positive flow rate because the source pressure is positive
    CPPUNIT_ASSERT(mArticle->mFlowRate > 0.0);

    mArticle->mPotentialVector[0] = -800.0;
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Potential Link should have negative flow rate because the port 0 potential vector is negative
    CPPUNIT_ASSERT(mArticle->mFlowRate < 0.0);

    std::cout << "... Pass";
}
