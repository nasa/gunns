/**
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"
#include "UtGunnsFluidTypeChangeConductor.hh"

/// @details  Test identification number.
int UtGunnsFluidTypeChangeConductor::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidTypeChangeConductor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidTypeChangeConductor::UtGunnsFluidTypeChangeConductor()
    :
    tLinkName(),
    tMaxConductivity(),
    tFluidTypesPort0(),
    tFluidTypesPort1(),
    tConfigData(),
    tMalfBlockageFlag(),
    tMalfBlockageValue(),
    tInputData(),
    tArticle(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tPort1(),
    tTimeStep(),
    tFluidProperties(),
    tFluidConfig(),
    tFluidInput0(),
    tFluidInput1(),
    fractions()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidTypeChangeConductor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidTypeChangeConductor::~UtGunnsFluidTypeChangeConductor()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTypeChangeConductor::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput1;
    delete tFluidInput0;
    delete tFluidConfig;
    delete [] fractions;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTypeChangeConductor::setUp()
{
    tLinkName           = "Test Fluid Type Change Converter";
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tNodes;
    tPort0              = 0;
    tPort1              = 1;
    tTimeStep           = 0.1;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[5];
    types[0]     = FluidProperties::GUNNS_N2;
    types[1]     = FluidProperties::GUNNS_N2_REAL_GAS;
    types[2]     = FluidProperties::GUNNS_H2O;
    types[3]     = FluidProperties::GUNNS_H2;
    types[4]     = FluidProperties::GUNNS_WATER;
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 5);

    fractions = new double[5];
    fractions[0] = 0.8;
    fractions[1] = 0.0;
    fractions[2] = 0.2;
    fractions[3] = 0.0;
    fractions[4] = 0.0;
    tFluidInput0 = new PolyFluidInputData(294.261,                  //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //mass fractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig, tFluidInput0);
    tNodes[0].resetFlows();

    fractions[0] = 0.0;
    fractions[1] = 0.6;
    fractions[2] = 0.0;
    fractions[3] = 0.4;
    fractions[4] = 0.0;
    tFluidInput1 = new PolyFluidInputData(294.261,                  //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //mass fractions

    tNodes[1].initialize("UtTestNode1", tFluidConfig, tFluidInput1);
    tNodes[2].initialize("UtTestNode1", tFluidConfig);              // Ground node
    tNodes[1].resetFlows();
    tNodes[2].resetFlows();

    /// - Define nominal configuration data
    tMaxConductivity = 1E-04;
    tFluidTypesPort0.push_back(FluidProperties::GUNNS_N2);
    tFluidTypesPort0.push_back(FluidProperties::GUNNS_H2O);
    tFluidTypesPort1.push_back(FluidProperties::GUNNS_N2_REAL_GAS);
    tFluidTypesPort1.push_back(FluidProperties::GUNNS_H2);
    tConfigData = new GunnsFluidTypeChangeConductorConfigData(tLinkName,
                                                              &tNodeList,
                                                              tMaxConductivity,
                                                              &tFluidTypesPort0,
                                                              &tFluidTypesPort1);

    /// - Define nominal input data
    tMalfBlockageFlag  = false;
    tMalfBlockageValue = 0.1;
    tInputData = new GunnsFluidTypeChangeConductorInputData(tMalfBlockageFlag,
                                                            tMalfBlockageValue);

    /// - Create the nominal test article
    tArticle = new FriendlyGunnsFluidTypeChangeConductor;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTypeChangeConductor::testConfig()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName              == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                 == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity       == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tFluidTypesPort0.at(0) == tConfigData->mFluidTypesPort0.at(0));
    CPPUNIT_ASSERT(tFluidTypesPort0.at(1) == tConfigData->mFluidTypesPort0.at(1));
    CPPUNIT_ASSERT(tFluidTypesPort1.at(0) == tConfigData->mFluidTypesPort1.at(0));
    CPPUNIT_ASSERT(tFluidTypesPort1.at(1) == tConfigData->mFluidTypesPort1.at(1));

    /// - Check default config construction
    GunnsFluidTypeChangeConductorConfigData defaultConfig;
    CPPUNIT_ASSERT(""                     == defaultConfig.mName);
    CPPUNIT_ASSERT(0                      == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0                      == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0                      == defaultConfig.mFluidTypesPort0.size());
    CPPUNIT_ASSERT(0                      == defaultConfig.mFluidTypesPort1.size());

    /// - Check copy config construction
    GunnsFluidTypeChangeConductorConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName              == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity       == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tFluidTypesPort0.at(0) == copyConfig.mFluidTypesPort0.at(0));
    CPPUNIT_ASSERT(tFluidTypesPort0.at(1) == copyConfig.mFluidTypesPort0.at(1));
    CPPUNIT_ASSERT(tFluidTypesPort1.at(0) == copyConfig.mFluidTypesPort1.at(0));
    CPPUNIT_ASSERT(tFluidTypesPort1.at(1) == copyConfig.mFluidTypesPort1.at(1));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTypeChangeConductor::testInput()
{
    UT_RESULT;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);

    /// - Check default config construction
    GunnsFluidTypeChangeConductorInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);

    /// - Check copy config construction
    GunnsFluidTypeChangeConductorInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == copyInput.mMalfBlockageValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTypeChangeConductor::testDefaultConstruction()
{
    UT_RESULT;

    /// @test proper default construction of class member data
    CPPUNIT_ASSERT(0.0 == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0   == tArticle->mNumTypes);
    CPPUNIT_ASSERT(0   == tArticle->mIndexPort0);
    CPPUNIT_ASSERT(0   == tArticle->mIndexPort1);
    CPPUNIT_ASSERT(0   == tArticle->mWorkMoleFractions);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidTypeChangeConductor* article = new GunnsFluidTypeChangeConductor();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTypeChangeConductor::testNominalInitialization()
{
    UT_RESULT;

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidTypeChangeConductor article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// - Verify the parent method is called
    CPPUNIT_ASSERT(tLinkName          == article.getName());
    CPPUNIT_ASSERT(tMalfBlockageValue == article.mMalfBlockageValue);

    /// - Verify member variables are properly set
    CPPUNIT_ASSERT(tMaxConductivity   == article.mMaxConductivity);
    CPPUNIT_ASSERT(2                  == article.mNumTypes);
    CPPUNIT_ASSERT(                      article.mIndexPort0);
    CPPUNIT_ASSERT(0                  == article.mIndexPort0[0]); //GUNNS_N2
    CPPUNIT_ASSERT(2                  == article.mIndexPort0[1]); //GUNNS_H2O
    CPPUNIT_ASSERT(1                  == article.mIndexPort1[0]); //GUNNS_N2_REAL_GAS
    CPPUNIT_ASSERT(3                  == article.mIndexPort1[1]); //GUNNS_H2
    CPPUNIT_ASSERT(0.0                == article.mSystemConductance);
    CPPUNIT_ASSERT(                      article.mInternalFluid);
    CPPUNIT_ASSERT(                      article.mWorkMoleFractions);
    CPPUNIT_ASSERT(0.0                == article.mWorkMoleFractions[0]);
    CPPUNIT_ASSERT(0.0                == article.mWorkMoleFractions[1]);
    CPPUNIT_ASSERT(0.0                == article.mWorkMoleFractions[2]);
    CPPUNIT_ASSERT(0.0                == article.mWorkMoleFractions[3]);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests some initialization errors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTypeChangeConductor::testInitializationExceptions()
{
    UT_RESULT;

    /// @test for exception on max conductivity < 0.0.
    tConfigData->mMaxConductivity = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mMaxConductivity = tMaxConductivity;

    /// @test for exception on size mismatch between port 0 and port 1 types.
    tConfigData->mFluidTypesPort1.clear();
    tConfigData->mFluidTypesPort1.push_back(FluidProperties::GUNNS_H2);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mFluidTypesPort1 = tFluidTypesPort1;

    /// @test for exception on duplicated port 0 types.
    tConfigData->mFluidTypesPort0.clear();
    tConfigData->mFluidTypesPort0.push_back(FluidProperties::GUNNS_N2);
    tConfigData->mFluidTypesPort0.push_back(FluidProperties::GUNNS_N2);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mFluidTypesPort0 = tFluidTypesPort0;

    /// @test for exception on duplicated port 1 types.
    tConfigData->mFluidTypesPort1.clear();
    tConfigData->mFluidTypesPort1.push_back(FluidProperties::GUNNS_H2);
    tConfigData->mFluidTypesPort1.push_back(FluidProperties::GUNNS_H2);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mFluidTypesPort1 = tFluidTypesPort1;

    /// @test for exception for type in both ports.
    tConfigData->mFluidTypesPort1.at(1) = FluidProperties::GUNNS_N2;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mFluidTypesPort1 = tFluidTypesPort1;

    /// @test for exception on types not in the network.
    tConfigData->mFluidTypesPort0.at(0) = FluidProperties::GUNNS_CO2;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mFluidTypesPort0 = tFluidTypesPort0;
    tConfigData->mFluidTypesPort1.at(1) = FluidProperties::GUNNS_NH3;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mFluidTypesPort1 = tFluidTypesPort1;

    /// @test for exception on type doesn't match phase of the node.
    tConfigData->mFluidTypesPort0.at(0) = FluidProperties::GUNNS_WATER;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mFluidTypesPort0 = tFluidTypesPort0;
    tConfigData->mFluidTypesPort1.at(1) = FluidProperties::GUNNS_WATER;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1), TsInitializationException);
    tConfigData->mFluidTypesPort1 = tFluidTypesPort1;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTypeChangeConductor::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set some stuff in the base class that should be reset on restart.
    tArticle->mPower               = 1.0;

    /// @test restart resets terms
    tArticle->restart();

    CPPUNIT_ASSERT(0.0 == tArticle->mPower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the step method (base class).  Since the fluid type change conductor has no
///           step method, we just test the checkSpecificPortRules function that the base class step
///           method calls.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTypeChangeConductor::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test step with a valid port change to the Ground node.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT(2                     == tArticle->mNodeMap[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::READY == tArticle->mUserPortSetControl);

    /// @test step with an invalid port change to a node due to conversion type phase conflict with
    ///       the node's phase.
    tArticle->mIndexPort0[1]      = 4; // GUNNS_WATER
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::DEFAULT;
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT(2                      == tArticle->mNodeMap[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::FAILED == tArticle->mUserPortSetControl);

    /// @test repeat tests for port 1.
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT(2                     == tArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::READY == tArticle->mUserPortSetControl);

    tArticle->mIndexPort1[0]      = 4; // GUNNS_WATER
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSetControl = GunnsBasicLink::DEFAULT;
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT(2                      == tArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::FAILED == tArticle->mUserPortSetControl);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for flow transport between nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTypeChangeConductor::testTransportFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->setMinLinearizationPotential(1.0e-8);

    /// - Set up forward flow through the link.
    tNodes[0].setPotential(200.0);
    tNodes[1].setPotential(100.0);
    tArticle->mPotentialVector[0] = 200.0;
    tArticle->mPotentialVector[1] = 100.0;
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    /// @test port 1's inflow rate matches the link flow rate and mixture is modified from port 0's
    //        contents.
    CPPUNIT_ASSERT(tArticle->mFlowRate > GunnsBasicLink::m100EpsilonLimit);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlowRate, tNodes[1].getInflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                        tNodes[1].getInflow()->getMoleFraction(0), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMoleFraction(0), tNodes[1].getInflow()->getMoleFraction(1), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                        tNodes[1].getInflow()->getMoleFraction(2), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMoleFraction(2), tNodes[1].getInflow()->getMoleFraction(3), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMoleFraction(4), tNodes[1].getInflow()->getMoleFraction(4), DBL_EPSILON);

    /// - Set up reverse flow through the link.
    tNodes[0].setPotential(100.0);
    tNodes[1].setPotential(200.0);
    tArticle->mPotentialVector[0] = 100.0;
    tArticle->mPotentialVector[1] = 200.0;
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    /// @test port 0's inflow rate matches the link flow rate and mixture is modified from port 1's
    //        contents.
    CPPUNIT_ASSERT(tArticle->mFlowRate < -GunnsBasicLink::m100EpsilonLimit);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-tArticle->mFlowRate, tNodes[0].getInflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getContent()->getMoleFraction(1), tNodes[0].getInflow()->getMoleFraction(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                        tNodes[0].getInflow()->getMoleFraction(1), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getContent()->getMoleFraction(3), tNodes[0].getInflow()->getMoleFraction(2), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                        tNodes[0].getInflow()->getMoleFraction(3), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getContent()->getMoleFraction(4), tNodes[0].getInflow()->getMoleFraction(4), DBL_EPSILON);

    UT_PASS_LAST;
}
