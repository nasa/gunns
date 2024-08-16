/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (aspects/thermal/GunnsThermalRadiation.o)
    )
***************************************************************************************************/
#include "UtGunnsThermalRadiation.hh"
#include "software/exceptions/TsInitializationException.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermalRadiation class.
///////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalRadiation::UtGunnsThermalRadiation()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tLinkName(),
    tNodes(),
    tNodeList(),
    tNetworkLinks(),
    tDefaultConductivity(),
    tViewScalar(),
    tBlockage(),
    tPort0(),
    tPort1(),
    tTimeStep(),
    tTolerance()
{
    // Nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsThermalRadiation class.
///////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalRadiation::~UtGunnsThermalRadiation()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalRadiation::tearDown()
{
    /// - Deletes for news in setUp
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalRadiation::setUp()
{
    /// - Declare the standard Gunns Link test data
    tLinkName             = "Test Thermal Radiation";
    tNodeList.mNumNodes   = 2;
    tNodeList.mNodes      = tNodes;
    tDefaultConductivity  = 0.5;
    tViewScalar           = 0.6;
    tBlockage             = 0.2;
    tPort0                = 0;
    tPort1                = 1;

    /// - Define nominal configuration data
    tConfigData = new GunnsThermalRadiationConfigData(tLinkName, &tNodeList,
                                                      tDefaultConductivity);

    /// - Define nominal input data
    tInputData = new GunnsThermalRadiationInputData(true, tBlockage, tViewScalar);

    /// - Initialize the test Article
    tArticle.initialize(*tConfigData, *tInputData, tNetworkLinks, tPort0, tPort1);

    /// - Declare the nominal test data
    tTolerance = 1.0e-08;
    tTimeStep = 0.1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalRadiation::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsThermalRadiation 01: testConfig ..........................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName            == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes               == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tDefaultConductivity == tConfigData->mDefaultConductivity);

    /// - Check default config construction
    GunnsThermalRadiationConfigData defaultConfig;
    CPPUNIT_ASSERT("unnamed radiation"  == defaultConfig.mName);
    CPPUNIT_ASSERT(0                    == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mDefaultConductivity);

    /// - Check copy config construction
    GunnsThermalRadiationConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName            == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes               == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tDefaultConductivity == copyConfig.mDefaultConductivity);

    std::cout << "... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalRadiation::testInput()
{
    std::cout << "\n UtGunnsThermalRadiation 02: testInput............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(tViewScalar == tInputData->iViewScalar);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage == tInputData->mMalfBlockageValue);

    /// - Check default input construction
    GunnsThermalRadiationInputData defaultInput;
    CPPUNIT_ASSERT(1.0 == defaultInput.iViewScalar);
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);

    /// - Check copy input construction
    GunnsThermalRadiationInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->iViewScalar == copyInput.iViewScalar);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalRadiation::testDefaultConstruction()
{
    std::cout << "\n UtGunnsThermalRadiation 03: testDefaultConstruction .............";

    /// - Construct an uninitialized test article
    FriendlyGunnsThermalRadiation article;

    /// @test Default values on construction
    CPPUNIT_ASSERT(0.0 == article.mDefaultConductivity);
    CPPUNIT_ASSERT(1.0 == article.mViewScalar);
    CPPUNIT_ASSERT(0.0 == article.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0 == article.mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == article.mSystemConductance);

    /// @test Dynamic new/delete for code coverage of destructor.
    GunnsThermalRadiation* article2 = new GunnsThermalRadiation;
    delete article2;

    std::cout << "... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for initialization without exceptions.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalRadiation::testInitialization()
{
    std::cout << "\n UtGunnsThermalRadiation 04: testInitialization...................";

    /// @test config data
    CPPUNIT_ASSERT(tDefaultConductivity == tArticle.mDefaultConductivity);
    CPPUNIT_ASSERT(tDefaultConductivity == tArticle.mEffectiveConductivity);

    /// @test init flag
    CPPUNIT_ASSERT(tArticle.mInitFlag);

    /// @test input data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tViewScalar, tArticle.mViewScalar, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tBlockage, tArticle.mMalfBlockageValue, tTolerance);

    std::cout << "... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests restart method.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalRadiation::testRestart()
{
    std::cout << "\n UtGunnsThermalRadiation 05: testRestart..........................";

    tArticle.mEffectiveConductivity = 1.0;
    tArticle.mSystemConductance     = 1.0;

    tArticle.restart();

    CPPUNIT_ASSERT(0.0 == tArticle.mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle.mSystemConductance);

    std::cout << "... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests getter and setter methods.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalRadiation::testAccessors()
{
    std::cout << "\n UtGunnsThermalRadiation 06: testAccessors........................";

    tArticle.setViewScalar(0.5);
    CPPUNIT_ASSERT(0.5 == tArticle.getViewScalar());

    std::cout << "... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the step method
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalRadiation::testUpdateState()
{
    std::cout << "\n UtGunnsThermalRadiation 07: testUpdateState......................";

    /// - Set potentials of vacuum and non-vacuum node equal to check that the first if statement
    ///   is skipped on account of not satisfying the condition argument.
    tArticle.mPotentialVector[0] = 0.8;
    tArticle.mPotentialVector[1] = 0.8;

    /// - Call updateState()
    tArticle.updateState(tTimeStep);

    /// @test mEffectiveConductivity gets set to value held by mDefaultConductivity. Serves to
    /// reinforce that if statement was in fact skipped.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDefaultConductivity, tArticle.mEffectiveConductivity, tTolerance);

    /// - Set potentials of vacuum and non-vacuum node to different values to check that first
    ///   if statement is executed.
    tArticle.mPotentialVector[0] = 0.9;
    tArticle.mPotentialVector[1] = 0.8;

    /// - Call updateState()
    tArticle.step(tTimeStep);

    /// @test System Conductance remains greater than zero
    CPPUNIT_ASSERT(tArticle.mSystemConductance > 0);

    /// - Set potential of non-vacuum node to be such that System Conductance exceeds the
    ///   Conductance limit, and call updateState().
    tArticle.mPotentialVector[0] = 800000;
    tArticle.step(tTimeStep);

    /// @test System Conductance is truncated to the Conductance Limit
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle.mSystemConductance,
            tArticle.mConductanceLimit, tTolerance);

    /// - Set default conductivity to a negative value, such that admittance calculation will
    ///   produce a value less than zero, and call updateState().
    tArticle.mDefaultConductivity = -0.6;
    tArticle.step(tTimeStep);

    /// @test System Conductance is raised to the lower limit, 0.0
    CPPUNIT_ASSERT(0.0 == tArticle.mSystemConductance);

    /// - Set potential of vacuum node, non-vacuum node, default conductivity, and
    ///   BlockageValue to values arbitrarily chosen for the purpose this test.
    tArticle.mPotentialVector[0]  = 240.0;
    tArticle.mPotentialVector[1]  = 2.0;
    tArticle.mDefaultConductivity = 2.5515e-07;
    tArticle.mMalfBlockageValue   = 0.8;
    tArticle.mViewScalar          = 0.75;

    /// - Set blockage flag to active and call updateState().
    tArticle.mMalfBlockageFlag = true;
    tArticle.step(tTimeStep);

    /// @test System Conductance value is in agreement with hand-made calculation
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.533525080, tArticle.mSystemConductance, tTolerance);

    /// - Set the blockage flag to false and call updateState().
    tArticle.mMalfBlockageFlag = false;
    tArticle.step(tTimeStep);

    /// @test System Conductance is in agreement with hand-made calculation when blockage
    /// is not present.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.667625399, tArticle.mSystemConductance, tTolerance);

    std::cout << "... Pass";
}
