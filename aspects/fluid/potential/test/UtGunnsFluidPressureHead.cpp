/**
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsFluidPressureHead.o))
*/

#include "software/exceptions/TsInitializationException.hh"
#include "UtGunnsFluidPressureHead.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsFluidPressureHead::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidPressureHead class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidPressureHead::UtGunnsFluidPressureHead()
    :
    tTypes(),
    tFractions(),
    tFluidProperties(),
    tFluidConfig(),
    tFluidInput0(),
    tFluidInput1(),
    tLinks(),
    tNodes(),
    tNodeList(),
    tArticle(tPotentialLink),
    tName(""),
    tConfig(),
    tInput(),
    tFluidColumn(),
    tAcceleration(),
    tReverseAcceleration(false),
    tRotationDcm(),
    tTransposeRotation(false),
    tConductorLink(),
    tPotentialLink(),
    tAccumLink(),
    tTimeStep()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidPressureHead class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidPressureHead::~UtGunnsFluidPressureHead()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureHead::tearDown()
{
    /// - Deletes for news in setUp
    delete tConfig;
    delete tInput;
    delete tFluidInput1;
    delete tFluidInput0;
    delete tFluidConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureHead::setUp()
{
    /// - Spotter configuration & input data.
    tName                = "tArticle";
    tFluidColumn[0]      = 0.0;
    tFluidColumn[1]      = 0.0;
    tFluidColumn[2]      = 1.0;
    tAcceleration[0]     = 0.0;
    tAcceleration[1]     = 0.0;
    tAcceleration[2]     = 9.81;
    tReverseAcceleration = false;
    tRotationDcm[0]      = 1.0;
    tRotationDcm[1]      = 0.0;
    tRotationDcm[2]      = 0.0;
    tRotationDcm[3]      = 0.0;
    tRotationDcm[4]      = 1.0;
    tRotationDcm[5]      = 0.0;
    tRotationDcm[6]      = 0.0;
    tRotationDcm[7]      = 0.0;
    tRotationDcm[8]      = 1.0;
    tTransposeRotation   = false;
    tConfig = new GunnsFluidPressureHeadConfigData(tName,
                                                   tFluidColumn[0],
                                                   tFluidColumn[1],
                                                   tFluidColumn[2],
                                                   tAcceleration,
                                                   tReverseAcceleration,
                                                   tRotationDcm,
                                                   tTransposeRotation);
    tInput  = new GunnsFluidPressureHeadInputData;

    /// - Set up fluid states.
    tFluidProperties      = new DefinedFluidProperties();
    tTypes[0]             = FluidProperties::GUNNS_WATER;
    tFractions[0]         = 1.0;
    tFluidConfig          = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tFluidInput0          = new PolyFluidInputData(300.0,
                                                     1.0e2,
                                                     0.0,
                                                     0.0,
                                                     tFractions);
    tFluidInput1          = new PolyFluidInputData(300.0,
                                                     1.0e4,
                                                     0.0,
                                                     0.0,
                                                     tFractions);

    /// - Initialize the nodes.
    tNodes[0].initialize("tNodes_0", tFluidConfig);
    tNodes[1].initialize("tNodes_1", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput0);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    /// - Initialize the nodes list.
    tNodeList.mNodes      = tNodes;
    tNodeList.mNumNodes   = N_NODES;

    /// - Initialize the links with nominal config & input data.
    GunnsFluidPotentialConfigData potentialConfig("tPotentialLink", &tNodeList, 1.0, 0.0);
    GunnsFluidPotentialInputData  potentialInput;
    tPotentialLink.initialize(potentialConfig, potentialInput, tLinks, 0, 1);

    GunnsFluidAccumConfigData accumConfig("tAccumLink", &tNodeList, 1.0, 0.1, 1.0, 5.0, 10.0,
                                          0.016, 5.0, 273.15, 373.15, 1.0e6, 0.0, 0.001);
    GunnsFluidAccumInputData accumInput(false, 0.0, 0.5, tFluidInput1);
    tAccumLink.initialize(accumConfig, accumInput, tLinks, 2, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsFluidPressureHeadConfigData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureHead::testConfig()
{
    UT_RESULT_FIRST;

    /// @test default config data construction.
    GunnsFluidPressureHeadConfigData defaultConfig("");
    CPPUNIT_ASSERT(""    == defaultConfig.mName);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mFluidColumn[0]);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mFluidColumn[1]);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mFluidColumn[2]);
    CPPUNIT_ASSERT(0     == defaultConfig.mAcceleration);
    CPPUNIT_ASSERT(false == defaultConfig.mReverseAcceleration);
    CPPUNIT_ASSERT(0     == defaultConfig.mRotationDcm);
    CPPUNIT_ASSERT(false == defaultConfig.mTransposeRotation);

    /// @test nominal config data construction.
    tFluidColumn[0]      = 1.0;
    tFluidColumn[1]      = 2.0;
    tFluidColumn[2]      = 3.0;
    tAcceleration[0]     = 4.0;
    tAcceleration[1]     = 5.0;
    tAcceleration[2]     = 6.0;
    tReverseAcceleration = true;
    tTransposeRotation   = true;
    GunnsFluidPressureHeadConfigData nominalConfig(tName,
                                                   tFluidColumn[0],
                                                   tFluidColumn[1],
                                                   tFluidColumn[2],
                                                   tAcceleration,
                                                   tReverseAcceleration,
                                                   tRotationDcm,
                                                   tTransposeRotation);
    CPPUNIT_ASSERT(tName                == nominalConfig.mName);
    CPPUNIT_ASSERT(tFluidColumn[0]      == nominalConfig.mFluidColumn[0]);
    CPPUNIT_ASSERT(tFluidColumn[1]      == nominalConfig.mFluidColumn[1]);
    CPPUNIT_ASSERT(tFluidColumn[2]      == nominalConfig.mFluidColumn[2]);
    CPPUNIT_ASSERT(tAcceleration[0]     == nominalConfig.mAcceleration[0]);
    CPPUNIT_ASSERT(tAcceleration[1]     == nominalConfig.mAcceleration[1]);
    CPPUNIT_ASSERT(tAcceleration[2]     == nominalConfig.mAcceleration[2]);
    CPPUNIT_ASSERT(tReverseAcceleration == nominalConfig.mReverseAcceleration);
    CPPUNIT_ASSERT(tRotationDcm[0]      == nominalConfig.mRotationDcm[0]);
    CPPUNIT_ASSERT(tRotationDcm[1]      == nominalConfig.mRotationDcm[1]);
    CPPUNIT_ASSERT(tRotationDcm[2]      == nominalConfig.mRotationDcm[2]);
    CPPUNIT_ASSERT(tRotationDcm[3]      == nominalConfig.mRotationDcm[3]);
    CPPUNIT_ASSERT(tRotationDcm[4]      == nominalConfig.mRotationDcm[4]);
    CPPUNIT_ASSERT(tRotationDcm[5]      == nominalConfig.mRotationDcm[5]);
    CPPUNIT_ASSERT(tRotationDcm[6]      == nominalConfig.mRotationDcm[6]);
    CPPUNIT_ASSERT(tRotationDcm[7]      == nominalConfig.mRotationDcm[7]);
    CPPUNIT_ASSERT(tRotationDcm[8]      == nominalConfig.mRotationDcm[8]);
    CPPUNIT_ASSERT(tTransposeRotation   == nominalConfig.mTransposeRotation);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsFluidPressureHeadInputData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureHead::testInput()
{
    UT_RESULT;

    /// @test default input data construction.
    CPPUNIT_ASSERT_NO_THROW(GunnsFluidPressureHeadInputData defaultInput);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsFluidPressureHead class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureHead::testDefaultConstruction()
{
    UT_RESULT;

    /// @test constructed values.
    CPPUNIT_ASSERT(""              == tArticle.mName);
    CPPUNIT_ASSERT(&tPotentialLink == tArticle.mLink);
    CPPUNIT_ASSERT(0               == tArticle.mPotentialLink);
    CPPUNIT_ASSERT(0               == tArticle.mAccumLink);
    CPPUNIT_ASSERT(0.0             == tArticle.mFluidColumn[0]);
    CPPUNIT_ASSERT(0.0             == tArticle.mFluidColumn[1]);
    CPPUNIT_ASSERT(0.0             == tArticle.mFluidColumn[2]);
    CPPUNIT_ASSERT(0               == tArticle.mAcceleration);
    CPPUNIT_ASSERT(false           == tArticle.mReverseAcceleration);
    CPPUNIT_ASSERT(0               == tArticle.mRotationDcm);
    CPPUNIT_ASSERT(false           == tArticle.mTransposeRotation);
    CPPUNIT_ASSERT(0.0             == tArticle.mPressureHead);
    CPPUNIT_ASSERT(false           == tArticle.mInitFlag);

    /// @test new/delete for code coverage.
    GunnsFluidPressureHead* article2 = new GunnsFluidPressureHead(tPotentialLink);
    delete article2;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsFluidPressureHead class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureHead::testInitialize()
{
    UT_RESULT;

    /// @test nominal initialization with a potential link.
    tReverseAcceleration = true;
    tTransposeRotation   = true;
    tConfig->mReverseAcceleration = tReverseAcceleration;
    tConfig->mTransposeRotation   = tTransposeRotation;
    tArticle.initialize(tConfig, tInput);

    CPPUNIT_ASSERT(tName                == tArticle.mName);
    CPPUNIT_ASSERT(&tPotentialLink      == tArticle.mPotentialLink);
    CPPUNIT_ASSERT(0                    == tArticle.mAccumLink);
    CPPUNIT_ASSERT(tFluidColumn[0]      == tArticle.mFluidColumn[0]);
    CPPUNIT_ASSERT(tFluidColumn[1]      == tArticle.mFluidColumn[1]);
    CPPUNIT_ASSERT(tFluidColumn[2]      == tArticle.mFluidColumn[2]);
    CPPUNIT_ASSERT(0                    != tArticle.mAcceleration);
    CPPUNIT_ASSERT(tAcceleration[0]     == tArticle.mAcceleration[0]);
    CPPUNIT_ASSERT(tAcceleration[1]     == tArticle.mAcceleration[1]);
    CPPUNIT_ASSERT(tAcceleration[2]     == tArticle.mAcceleration[2]);
    CPPUNIT_ASSERT(tReverseAcceleration == tArticle.mReverseAcceleration);
    CPPUNIT_ASSERT(0                    != tArticle.mRotationDcm);
    CPPUNIT_ASSERT(tRotationDcm[0]      == tArticle.mRotationDcm[0]);
    CPPUNIT_ASSERT(tRotationDcm[1]      == tArticle.mRotationDcm[1]);
    CPPUNIT_ASSERT(tRotationDcm[2]      == tArticle.mRotationDcm[2]);
    CPPUNIT_ASSERT(tRotationDcm[3]      == tArticle.mRotationDcm[3]);
    CPPUNIT_ASSERT(tRotationDcm[4]      == tArticle.mRotationDcm[4]);
    CPPUNIT_ASSERT(tRotationDcm[5]      == tArticle.mRotationDcm[5]);
    CPPUNIT_ASSERT(tRotationDcm[6]      == tArticle.mRotationDcm[6]);
    CPPUNIT_ASSERT(tRotationDcm[7]      == tArticle.mRotationDcm[7]);
    CPPUNIT_ASSERT(tRotationDcm[8]      == tArticle.mRotationDcm[8]);
    CPPUNIT_ASSERT(tTransposeRotation   == tArticle.mTransposeRotation);
    CPPUNIT_ASSERT(0.0                  == tArticle.mPressureHead);
    CPPUNIT_ASSERT(true                 == tArticle.mInitFlag);

    /// @test nominal initialization with an accumulator link
    FriendlyGunnsFluidPressureHead article2(tAccumLink);
    article2.initialize(tConfig, tInput);

    CPPUNIT_ASSERT(tName                == article2.mName);
    CPPUNIT_ASSERT(0                    == article2.mPotentialLink);
    CPPUNIT_ASSERT(&tAccumLink          == article2.mAccumLink);
    CPPUNIT_ASSERT(tFluidColumn[0]      == article2.mFluidColumn[0]);
    CPPUNIT_ASSERT(tFluidColumn[1]      == article2.mFluidColumn[1]);
    CPPUNIT_ASSERT(tFluidColumn[2]      == article2.mFluidColumn[2]);
    CPPUNIT_ASSERT(0                    != article2.mAcceleration);
    CPPUNIT_ASSERT(tAcceleration[0]     == article2.mAcceleration[0]);
    CPPUNIT_ASSERT(tAcceleration[1]     == article2.mAcceleration[1]);
    CPPUNIT_ASSERT(tAcceleration[2]     == article2.mAcceleration[2]);
    CPPUNIT_ASSERT(tReverseAcceleration == article2.mReverseAcceleration);
    CPPUNIT_ASSERT(0                    != article2.mRotationDcm);
    CPPUNIT_ASSERT(tRotationDcm[0]      == article2.mRotationDcm[0]);
    CPPUNIT_ASSERT(tRotationDcm[1]      == article2.mRotationDcm[1]);
    CPPUNIT_ASSERT(tRotationDcm[2]      == article2.mRotationDcm[2]);
    CPPUNIT_ASSERT(tRotationDcm[3]      == article2.mRotationDcm[3]);
    CPPUNIT_ASSERT(tRotationDcm[4]      == article2.mRotationDcm[4]);
    CPPUNIT_ASSERT(tRotationDcm[5]      == article2.mRotationDcm[5]);
    CPPUNIT_ASSERT(tRotationDcm[6]      == article2.mRotationDcm[6]);
    CPPUNIT_ASSERT(tRotationDcm[7]      == article2.mRotationDcm[7]);
    CPPUNIT_ASSERT(tRotationDcm[8]      == article2.mRotationDcm[8]);
    CPPUNIT_ASSERT(tTransposeRotation   == article2.mTransposeRotation);
    CPPUNIT_ASSERT(0.0                  == article2.mPressureHead);
    CPPUNIT_ASSERT(true                 == article2.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsFluidPressureHead with errors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureHead::testInitializeExceptions()
{
    UT_RESULT;

    /// @test exception thrown from missing name.
    tConfig->mName = "";
    CPPUNIT_ASSERT_THROW(tArticle.initialize(tConfig, tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tConfig->mName = tName;

    /// @test exception thrown from null config data.
    const GunnsFluidPressureHeadConfigData* nullConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(nullConfig, tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// @test exception thrown from null input data.
    const GunnsFluidPressureHeadInputData* nullInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(tConfig, nullInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// @test exception thrown on bad config data pointer type.
    BadGunnsFluidPressureHeadConfigData* badConfig = new BadGunnsFluidPressureHeadConfigData(tName);
    CPPUNIT_ASSERT_THROW(tArticle.initialize(badConfig, tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badConfig;

    /// @test exception thrown on bad input data pointer type.
    BadGunnsFluidPressureHeadInputData* badInput = new BadGunnsFluidPressureHeadInputData();
    CPPUNIT_ASSERT_THROW(tArticle.initialize(tConfig, badInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badInput;

    /// @test exception thrown on invalid link type.
    FriendlyGunnsFluidPressureHead article2(tConductorLink);
    CPPUNIT_ASSERT_THROW(article2.initialize(tConfig, tInput), TsInitializationException);

    /// @test exception thrown on missing acceleration vector.
    tConfig->mAcceleration = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(tConfig, tInput), TsInitializationException);
    tConfig->mAcceleration = tAcceleration;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsFluidPressureHead class, when
///           connected to a fluid potential link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureHead::testPreSolverPotential()
{
    UT_RESULT;

    tArticle.initialize(tConfig, tInput);

    /// @test with no rotation DCM.
    const double expectedP = tAcceleration[2] * tFluidColumn[2] * tNodes[0].getContent()->getDensity()
                           / 1000.0;
    tArticle.mRotationDcm = 0;
    tArticle.stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle.mPressureHead,             1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tPotentialLink.getSourcePressure(), 1.0e-14);

    /// @test with rotation (perpendicular).
    tRotationDcm[0] = 0.0;
    tRotationDcm[1] = 1.0;
    tRotationDcm[2] = 0.0;
    tRotationDcm[3] = 0.0;
    tRotationDcm[4] = 0.0;
    tRotationDcm[5] = 1.0;
    tRotationDcm[6] = 1.0;
    tRotationDcm[7] = 0.0;
    tRotationDcm[8] = 0.0;
    tArticle.mRotationDcm = tRotationDcm;
    tArticle.stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle.mPressureHead,             1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPotentialLink.getSourcePressure(), 1.0e-14);

    /// @test with rotation (aligned).
    tAcceleration[0] = 9.81;
    tAcceleration[1] = 0.0;
    tAcceleration[2] = 0.0;
    tArticle.stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle.mPressureHead,             1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tPotentialLink.getSourcePressure(), 1.0e-14);

    /// @test with transposed rotation.
    tAcceleration[0] = 0.0;
    tAcceleration[1] = 9.81;
    tAcceleration[2] = 0.0;
    tArticle.mTransposeRotation = true;
    tArticle.stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle.mPressureHead,             1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tPotentialLink.getSourcePressure(), 1.0e-14);

    /// @test with reversed acceleration.
    double accel[3] = {0.0, 0.0, -9.81};
    double col[3]   = {0.0, 0.0,  1.0};
    tArticle.mReverseAcceleration = true;
    tArticle.stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedP, tArticle.mPressureHead,             1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedP, tPotentialLink.getSourcePressure(), 1.0e-14);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsFluidPressureHead class, when
///           connected to a fluid accumulator link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureHead::testPreSolverAccum()
{
    UT_RESULT;

    FriendlyGunnsFluidPressureHeadAccum* accum = static_cast<FriendlyGunnsFluidPressureHeadAccum*>(&tAccumLink);
    tArticle.mLink = &tAccumLink;
    tArticle.initialize(tConfig, tInput);

    /// @test with no rotation DCM.
    const double expectedP = tAcceleration[2] * tFluidColumn[2] * tNodes[1].getContent()->getDensity()
                           * 0.5 / 1000.0;
    tArticle.mRotationDcm = 0;
    tArticle.stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle.mPressureHead,    1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, accum->mAccelPressureHead, 1.0e-14);

    /// @test with rotation (perpendicular).
    tRotationDcm[0] = 0.0;
    tRotationDcm[1] = 1.0;
    tRotationDcm[2] = 0.0;
    tRotationDcm[3] = 0.0;
    tRotationDcm[4] = 0.0;
    tRotationDcm[5] = 1.0;
    tRotationDcm[6] = 1.0;
    tRotationDcm[7] = 0.0;
    tRotationDcm[8] = 0.0;
    tArticle.mRotationDcm = tRotationDcm;
    tArticle.stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle.mPressureHead,    1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, accum->mAccelPressureHead, 1.0e-14);

    /// @test with rotation (aligned).
    tAcceleration[0] = 9.81;
    tAcceleration[1] = 0.0;
    tAcceleration[2] = 0.0;
    tArticle.stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle.mPressureHead,    1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, accum->mAccelPressureHead, 1.0e-14);

    /// @test with transposed rotation.
    tAcceleration[0] = 0.0;
    tAcceleration[1] = 9.81;
    tAcceleration[2] = 0.0;
    tArticle.mTransposeRotation = true;
    tArticle.stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle.mPressureHead,    1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, accum->mAccelPressureHead, 1.0e-14);

    /// @test with reversed acceleration.
    double accel[3] = {0.0, 0.0, -9.81};
    double col[3]   = {0.0, 0.0,  1.0};
    tArticle.mReverseAcceleration = true;
    tArticle.stepPreSolver(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedP, tArticle.mPressureHead,    1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedP, accum->mAccelPressureHead, 1.0e-14);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsFluidPressureHead class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureHead::testPostSolver()
{
    UT_RESULT;

    tArticle.initialize(tConfig, tInput);
    CPPUNIT_ASSERT_NO_THROW(tArticle.stepPostSolver(tTimeStep));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the setter & getter methods of the GunnsFluidPressureHead class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidPressureHead::testAccessors()
{
    UT_RESULT;

    tArticle.initialize(tConfig, tInput);

    /// @test getPressureHead method.
    tArticle.mPressureHead = 42.0;
    CPPUNIT_ASSERT(42.0 == tArticle.getPressureHead());

    UT_PASS_FINAL;

}
