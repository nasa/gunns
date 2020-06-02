/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (aspects/fluid/conductor/GunnsFluid3WayCheckValve.o)
    )
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluid3WayCheckValve.hh"

/// @details  Test identification number.
int UtGunnsFluid3WayCheckValve::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluid3WayCheckValve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluid3WayCheckValve::UtGunnsFluid3WayCheckValve()
    :
    tArticle(),
    tLinkName(),
    tConfigData(),
    tPositionPathB(false),
    tMaxConductivityA(0.0),
    tMaxConductivityB(0.0),
    tExpansionScaleFactorA(0.0),
    tExpansionScaleFactorB(0.0),
    tThermalLengthA(0.0),
    tThermalLengthB(0.0),
    tThermalDiameterA(0.0),
    tThermalDiameterB(0.0),
    tSurfaceRoughnessA(0.0),
    tSurfaceRoughnessB(0.0),
    tRateLimit(0.0),
    tOpenPressureA(0.0),
    tOpenPressureB(0.0),
    tMinPressDiff(0.0),
    tInputData(),
    tPosition(0.0),
    tWallTemperatureA(0.0),
    tWallTemperatureB(0.0),
    tMalfStuckFlag(0.0),
    tMalfFailToFlag(0.0),
    tMalfFailToValue(0.0),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(0),
    tPort1(0),
    tPort2(0),
    tTimeStep(0),
    tFluidProperties(),
    tFluidConfig(),
    tFluidInput1(),
    tFluidInput2(),
    tFluidInput3(),
    tPathA(0),
    tPathB(0)
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluid3WayCheckValve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluid3WayCheckValve::~UtGunnsFluid3WayCheckValve()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WayCheckValve::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tFluidConfig;
    delete tFluidProperties;
    delete tFluidInput3;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WayCheckValve::setUp()
{
    /// - Define nominal configuration data
    tLinkName              = "Test Three Way Check Valve";
    tNodeList.mNumNodes    = 3;
    tNodeList.mNodes       = tNodes;
    tPositionPathB         = false;
    tMaxConductivityA      = 0.6;
    tMaxConductivityB      = 0.5;
    tExpansionScaleFactorA = 0.4;
    tExpansionScaleFactorB = 0.3;
    tThermalLengthA        = 1.0;
    tThermalLengthB        = 1.1;
    tThermalDiameterA      = 0.9;
    tThermalDiameterB      = 0.8;
    tSurfaceRoughnessA     = 1.0E-4;
    tSurfaceRoughnessB     = 2.0E-4;
    tRateLimit             = 10.0;
    tOpenPressureA         = 50.0;
    tOpenPressureB         = 50.0;
    tMinPressDiff          = 5.0;
    tConfigData            = new GunnsFluid3WayCheckValveConfigData(tLinkName,
                                                               &tNodeList,
                                                               tPositionPathB,
                                                               tMaxConductivityA,
                                                               tMaxConductivityB,
                                                               tExpansionScaleFactorA,
                                                               tExpansionScaleFactorB,
                                                               tThermalLengthA,
                                                               tThermalLengthB,
                                                               tThermalDiameterA,
                                                               tThermalDiameterB,
                                                               tSurfaceRoughnessA,
                                                               tSurfaceRoughnessB,
                                                               tRateLimit,
                                                               tOpenPressureA,
                                                               tOpenPressureB,
                                                               tMinPressDiff);

    /// - Set up nominal input data.
    tPosition              = 1.0;
    tWallTemperatureA      = 300.0;
    tWallTemperatureB      = 290.0;
    tMalfStuckFlag         = false;
    tMalfFailToFlag        = false;
    tMalfFailToValue       = 0.0;
    tInputData             = new GunnsFluid3WayCheckValveInputData(tPosition,
                                                              tWallTemperatureA,
                                                              tWallTemperatureB,
                                                              tMalfStuckFlag,
                                                              tMalfFailToFlag,
                                                              tMalfFailToValue);

    /// - Set up other init data.
    tPort0                 = 0;
    tPort1                 = 1;
    tPort2                 = 2;
    tTimeStep              = 0.1;

    /// - Set up fluid nodes
    double fractions1[2]   = {1.0, 0.0};
    tFluidInput1 = new PolyFluidInputData(300.15,                   //temperature
                                          689.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions1);              //massFractions
    double fractions2[2]   = {0.0, 1.0};
    tFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          589.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      // mass
                                          fractions2);              //massFractions
    double fractions3[2]   = {0.5, 0.5};
    tFluidInput3 = new PolyFluidInputData(283.15,                   //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions3);              //massFractions

    /// - Have to initialize the nodes with the fluid config (normally done by GUNNS)
    tFluidProperties       = new DefinedFluidProperties();
    FluidProperties::FluidType types[2] = {FluidProperties::GUNNS_N2, FluidProperties::GUNNS_O2};
    tFluidConfig           = new PolyFluidConfigData(tFluidProperties, types, 2);

    tNodes[0].initialize("UtTestNode0", tFluidConfig);
    tNodes[1].initialize("UtTestNode1", tFluidConfig);
    tNodes[2].initialize("UtTestNode2", tFluidConfig);

    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput2);
    tNodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput3);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[2].resetFlows();

    /// - Create the test article.
    tArticle               = new FriendlyGunnsFluid3WayCheckValve;
    tPathA                 = static_cast<FriendlyGunnsFluidValveFor3WayCheck*>(&tArticle->mPathA);
    tPathB                 = static_cast<FriendlyGunnsFluidValveFor3WayCheck*>(&tArticle->mPathB);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WayCheckValve::testConfig()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName              == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                 == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivityA      == tConfigData->mMaxConductivityA);
    CPPUNIT_ASSERT(tMaxConductivityB      == tConfigData->mMaxConductivityB);
    CPPUNIT_ASSERT(tExpansionScaleFactorA == tConfigData->mExpansionScaleFactorA);
    CPPUNIT_ASSERT(tExpansionScaleFactorB == tConfigData->mExpansionScaleFactorB);
    CPPUNIT_ASSERT(tThermalLengthA        == tConfigData->mThermalLengthA);
    CPPUNIT_ASSERT(tThermalLengthB        == tConfigData->mThermalLengthB);
    CPPUNIT_ASSERT(tThermalDiameterA      == tConfigData->mThermalDiameterA);
    CPPUNIT_ASSERT(tThermalDiameterB      == tConfigData->mThermalDiameterB);
    CPPUNIT_ASSERT(tSurfaceRoughnessA     == tConfigData->mSurfaceRoughnessA);
    CPPUNIT_ASSERT(tSurfaceRoughnessB     == tConfigData->mSurfaceRoughnessB);
    CPPUNIT_ASSERT(tRateLimit             == tConfigData->mRateLimit);
    CPPUNIT_ASSERT(tOpenPressureA         == tConfigData->mOpenPressureA);
    CPPUNIT_ASSERT(tOpenPressureB         == tConfigData->mOpenPressureB);
    CPPUNIT_ASSERT(tMinPressDiff          == tConfigData->mMinPressureDiff);

    /// - Check default config construction
    GunnsFluid3WayCheckValveConfigData defaultConfig;
    CPPUNIT_ASSERT(""  == defaultConfig.mName);
    CPPUNIT_ASSERT(0   == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMaxConductivityA);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMaxConductivityB);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mExpansionScaleFactorA);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mExpansionScaleFactorB);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mThermalLengthA);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mThermalLengthB);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mThermalDiameterA);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mThermalDiameterB);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mSurfaceRoughnessA);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mSurfaceRoughnessB);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mRateLimit);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mOpenPressureA);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mOpenPressureB);
    CPPUNIT_ASSERT(0.0 == defaultConfig.mMinPressureDiff);
    /// - Check copy config construction
    GunnsFluid3WayCheckValveConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName              == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivityA      == copyConfig.mMaxConductivityA);
    CPPUNIT_ASSERT(tMaxConductivityB      == copyConfig.mMaxConductivityB);
    CPPUNIT_ASSERT(tExpansionScaleFactorA == copyConfig.mExpansionScaleFactorA);
    CPPUNIT_ASSERT(tExpansionScaleFactorB == copyConfig.mExpansionScaleFactorB);
    CPPUNIT_ASSERT(tThermalLengthA        == copyConfig.mThermalLengthA);
    CPPUNIT_ASSERT(tThermalLengthB        == copyConfig.mThermalLengthB);
    CPPUNIT_ASSERT(tThermalDiameterA      == copyConfig.mThermalDiameterA);
    CPPUNIT_ASSERT(tThermalDiameterB      == copyConfig.mThermalDiameterB);
    CPPUNIT_ASSERT(tSurfaceRoughnessA     == copyConfig.mSurfaceRoughnessA);
    CPPUNIT_ASSERT(tSurfaceRoughnessB     == copyConfig.mSurfaceRoughnessB);
    CPPUNIT_ASSERT(tRateLimit             == copyConfig.mRateLimit);
    CPPUNIT_ASSERT(tOpenPressureA         == copyConfig.mOpenPressureA);
    CPPUNIT_ASSERT(tOpenPressureB         == copyConfig.mOpenPressureB);
    CPPUNIT_ASSERT(tMinPressDiff          == copyConfig.mMinPressureDiff);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WayCheckValve::testInput()
{
    UT_RESULT;

    /// - Check nominal input data construction
    CPPUNIT_ASSERT(tPosition              == tInputData->mPosition);
    CPPUNIT_ASSERT(tWallTemperatureA      == tInputData->mWallTemperatureA);
    CPPUNIT_ASSERT(tWallTemperatureB      == tInputData->mWallTemperatureB);
    CPPUNIT_ASSERT(tMalfStuckFlag   == tInputData->mMalfStuckFlag);
    CPPUNIT_ASSERT(tMalfFailToFlag  == tInputData->mMalfFailToFlag);
    CPPUNIT_ASSERT(tMalfFailToValue == tInputData->mMalfFailToValue);

    /// - Check default input data construction
    GunnsFluid3WayCheckValveInputData defaultInput;
    CPPUNIT_ASSERT(0.0                    == defaultInput.mPosition);
    CPPUNIT_ASSERT(0.0                    == defaultInput.mWallTemperatureA);
    CPPUNIT_ASSERT(0.0                    == defaultInput.mWallTemperatureB);
    CPPUNIT_ASSERT(false == defaultInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(false == defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(0.0   == defaultInput.mMalfFailToValue);

    /// - Check copy input data construction
    GunnsFluid3WayCheckValveInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tPosition              == copyInput.mPosition);
    CPPUNIT_ASSERT(tWallTemperatureA      == copyInput.mWallTemperatureA);
    CPPUNIT_ASSERT(tWallTemperatureB      == copyInput.mWallTemperatureB);
    CPPUNIT_ASSERT(tMalfStuckFlag   == copyInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(tMalfFailToFlag  == copyInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(tMalfFailToValue == copyInput.mMalfFailToValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WayCheckValve::testDefaultConstruction()
{
    UT_RESULT;

    /// @test config data
    CPPUNIT_ASSERT(tArticle->mMalfStuckFlag   == false);
    CPPUNIT_ASSERT(tArticle->mMalfFailToFlag  == false);
    CPPUNIT_ASSERT(tArticle->mMalfFailToValue == 0.0);
    CPPUNIT_ASSERT(tArticle->mRateLimit       == 0.0);
    CPPUNIT_ASSERT(tArticle->mOpenPressureA   == 0.0);
    CPPUNIT_ASSERT(tArticle->mOpenPressureB   == 0.0);
    CPPUNIT_ASSERT(tArticle->mMinPressureDiff == 0.0);

    /// @test init flag
    CPPUNIT_ASSERT(false                  == tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluid3WayCheckValve* article          =  new GunnsFluid3WayCheckValve();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid 3 Way Check Valve link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WayCheckValve::testAccessors()
{
    UT_RESULT;

    /// @test set stuck malfunction
    tArticle->setMalfStuck(true);
    CPPUNIT_ASSERT(tArticle->mMalfStuckFlag == true);

    /// @test set fail to malfunction
    tArticle->setMalfFailTo(true, 1.0);
    CPPUNIT_ASSERT(tArticle->mMalfFailToFlag  == true);
    CPPUNIT_ASSERT(tArticle->mMalfStuckFlag   == true);
    CPPUNIT_ASSERT(tArticle->mMalfFailToValue == 1.0);

    UT_PASS;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WayCheckValve::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize the test article with nominal data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// @test data initialized correctly
    CPPUNIT_ASSERT(tArticle->mMalfStuckFlag   == tMalfStuckFlag);
    CPPUNIT_ASSERT(tArticle->mMalfFailToFlag  == tMalfFailToFlag);
    CPPUNIT_ASSERT(tArticle->mMalfFailToValue == tMalfFailToValue);
    CPPUNIT_ASSERT(tArticle->mRateLimit       == tRateLimit);
    CPPUNIT_ASSERT(tArticle->mOpenPressureA   == tOpenPressureA);
    CPPUNIT_ASSERT(tArticle->mOpenPressureB   == tOpenPressureB);
    CPPUNIT_ASSERT(tArticle->mMinPressureDiff == tMinPressDiff);

    /// @test init flag.
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WayCheckValve::testInitializationExceptions()
{
    UT_RESULT;

    /// @test for negative rate limit
    tConfigData->mRateLimit = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mRateLimit = tRateLimit;

    /// @test for open pressures less than or equal to min pressure differential
    tConfigData->mOpenPressureA = 0.5 * tConfigData->mMinPressureDiff;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mOpenPressureA = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mOpenPressureA = tOpenPressureA;
    tConfigData->mOpenPressureB= 0.5 * tConfigData->mMinPressureDiff;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mOpenPressureB = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mOpenPressureB = tOpenPressureB;

    /// @test for exception on malf fail to > 1
    tInputData->mMalfFailToValue = 1.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);

    /// @test for exception on malf fail to < 0
    tInputData->mMalfFailToValue = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tInputData->mMalfFailToValue = 0.0;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WayCheckValve::testUpdateState()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// - Set up pressure differential within path A partially open range.
    tPathA->mPotentialDrop = (tOpenPressureA + tMinPressDiff) * 0.5;
    tPathB->mPotentialDrop = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.75, tArticle->mPosition, DBL_EPSILON);

    /// - Set up pressure differential past path A full open range.
    tPathA->mPotentialDrop = tOpenPressureA;
    tPathB->mPotentialDrop = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mPosition, DBL_EPSILON);

    /// - Set up pressure differential within path B partially open range.
    tPathB->mPotentialDrop = (tOpenPressureB + tMinPressDiff) * 0.5;
    tPathA->mPotentialDrop = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25, tArticle->mPosition, DBL_EPSILON);

    /// - Set up pressure differential past path B full open range.
    tPathB->mPotentialDrop = tOpenPressureB;
    tPathA->mPotentialDrop = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPosition, DBL_EPSILON);

    /// - Set Path B to be the path corresponding to the position. This should reverse the position
    tArticle->mPositionPathB = true;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mPosition, DBL_EPSILON);

    /// - Set up pressure differential in minimum range. position should be 0.5
    tPathB->mPotentialDrop = 0.0;
    tPathA->mPotentialDrop = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tArticle->mPosition, DBL_EPSILON);
    tPathB->mPotentialDrop = tMinPressDiff * 0.5;
    tPathA->mPotentialDrop = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tArticle->mPosition, DBL_EPSILON);
    tPathB->mPotentialDrop = tMinPressDiff * -0.5;
    tPathA->mPotentialDrop = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tArticle->mPosition, DBL_EPSILON);

    /// - Set up negative Pressure differential. Valve should hold previous value
    tPathB->mPotentialDrop = -100.0;
    tPathA->mPotentialDrop = -200.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tArticle->mPosition, DBL_EPSILON);

    /// @test fail to malfunction
    tArticle->mMalfFailToFlag = true;
    tArticle->mMalfFailToValue = 0.75;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.75, tArticle->mPosition, DBL_EPSILON);
    tArticle->mMalfFailToFlag = false;

    /// @test stuck malfunction
    tPathB->mPotentialDrop = 0.0;
    tPathA->mPotentialDrop = 0.0;
    tArticle->mMalfStuckFlag = true;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.75, tArticle->mPosition, DBL_EPSILON);

    /// @test Restart model, for code coverage only
    CPPUNIT_ASSERT_NO_THROW(tArticle->restartModel());

    UT_PASS;
}
