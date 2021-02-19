/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/source/GunnsFluidHeater.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidHeater.hh"

/// @details  Test identification number.
int UtGunnsFluidHeater::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Heater link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHeater::UtGunnsFluidHeater()
    :
    CppUnit::TestFixture(),
    tTypes(),
    tFractions(),
    tFluidProperties(0),
    tFluidConfig(0),
    tFluidInput(0),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tHeaterEfficiency(0.0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tHeaterPower(0.0),
    tInputData(0),
    tArticle(0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Heater link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHeater::~UtGunnsFluidHeater()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidHeater::setUp()
{
    /// - Define the nominal port fluids.
    tFluidProperties        = new DefinedFluidProperties();
    tTypes[0]               = FluidProperties::GUNNS_O2;
    tFractions[0]           = 1.0;
    tFluidConfig            = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tFluidInput             = new PolyFluidInputData(283.0,
                                                     109.0,
                                                       0.0,
                                                       0.0,
                                                     tFractions);

    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", tFluidConfig);
    tNodes[1].initialize("UtNode2", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput);

    /// - Initialize the nodes list.
    tNodeList.mNodes        = tNodes;
    tNodeList.mNumNodes     = N_NODES;

    /// - Define the nominal configuration data.
    tName                   = "nominal";
    tHeaterEfficiency       = 0.9;
    tConfigData             = new GunnsFluidHeaterConfigData(tName,
                                                             &tNodeList,
                                                             tHeaterEfficiency);

    /// - Define the nominal input data.
    tMalfBlockageFlag     = false;
    tMalfBlockageValue    =   0.3;
    tHeaterPower          = 100.0;
    tInputData            = new GunnsFluidHeaterInputData(tMalfBlockageFlag,
                                                          tMalfBlockageValue,
                                                          tHeaterPower);

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 1;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsFluidHeater;

    /// - Define the nominal time step.
    tTimeStep             = 1.0;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeater::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput;
    delete tFluidConfig;
    delete tFluidProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heater link model construction of configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeater::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                                == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                               == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tHeaterEfficiency                    == tConfigData->mHeaterEfficiency);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag                    == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue                   == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tHeaterPower                         == tInputData->mHeaterPower);

    /// @test    Configuration data default construction.
    GunnsFluidHeaterConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                    == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                    == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(1.0                                  == defaultConfig.mHeaterEfficiency);

    /// @test    Input data default construction.
    GunnsFluidHeaterInputData defaultInput;
    CPPUNIT_ASSERT(                                       !defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                                  == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                                  == defaultInput.mHeaterPower);

    /// @test    Configuration data copy construction.
    GunnsFluidHeaterConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tConfigData->mName                   == copyConfig.mName);
    CPPUNIT_ASSERT(tConfigData->mNodeList->mNodes       == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tConfigData->mHeaterEfficiency       == copyConfig.mHeaterEfficiency);

    /// @test    Input data copy construction.
    GunnsFluidHeaterInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag        == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue       == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mHeaterPower             == copyInput.mHeaterPower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heater link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeater::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction state data.
    CPPUNIT_ASSERT(0.0                       == tArticle->mHeatFlux);
    CPPUNIT_ASSERT(0.0                       == tArticle->mHeaterPower);
    CPPUNIT_ASSERT(0.0                       == tArticle->mHeaterEfficiency);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidHeater* article = new GunnsFluidHeater();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heater link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeater::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidHeater article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                                == article.mName);
    CPPUNIT_ASSERT(&tNodes[tPort0]                      == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[tPort1]                      == article.mNodes[1]);
    CPPUNIT_ASSERT(tConfigData->mHeaterEfficiency       == article.mHeaterEfficiency);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tInputData->mHeaterPower             == article.mHeaterPower);

    /// @test    Nominal derived data.
    CPPUNIT_ASSERT(0.0                                  == article.mHeatFlux);
    CPPUNIT_ASSERT(tHeaterPower                         == article.mHeaterPower);
    CPPUNIT_ASSERT(tHeaterEfficiency                    == article.mHeaterEfficiency);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heater link model nominal initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeater::testInitializationExceptions()
{
    UT_RESULT;

    /// - Initialize default constructed test article with bad config data.
    FriendlyGunnsFluidHeater article;
    tConfigData->mHeaterEfficiency = -0.1;

    /// @test    Exception thrown on bad heater efficiency.
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
            TsInitializationException);

    tConfigData->mHeaterEfficiency =  1.1;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
            TsInitializationException);

    /// @test    Initialization flag not set.
    CPPUNIT_ASSERT(!article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heater link model step method in nominal operation.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeater::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Outputs of step with blockage malf inactive
    tArticle->step(tTimeStep);

    double expectedFlux = tHeaterPower * tHeaterEfficiency;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mHeatFlux, DBL_EPSILON);

    /// @test    Outputs of step with blockage malf active
    tArticle->mMalfBlockageFlag = true;
    tArticle->step(tTimeStep);

    expectedFlux = tHeaterPower * tHeaterEfficiency * (1.0 - tMalfBlockageValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mHeatFlux, DBL_EPSILON);

    /// @test    Contributions to system are zero'ed every pass
    tArticle->mAdmittanceMatrix[0] = 1.0;
    tArticle->mAdmittanceMatrix[1] = 2.0;
    tArticle->mAdmittanceMatrix[2] = 3.0;
    tArticle->mAdmittanceMatrix[3] = 4.0;
    tArticle->mSourceVector[0]     = 1.0;
    tArticle->mSourceVector[1]     = 2.0;
    tArticle->mAdmittanceUpdate    = true;
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tArticle->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAdmittanceMatrix[2]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT(0.0 == tArticle->mSourceVector[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mSourceVector[1]);
    CPPUNIT_ASSERT(      !tArticle->mAdmittanceUpdate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heater link model computeFlows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeater::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Heat flux transported to/from the nodes.
    tArticle->mHeatFlux = 100.0;
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);

    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-100.0, tNodes[0].mInflowHeatFlux, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 100.0, tNodes[1].mInflowHeatFlux, DBL_EPSILON);

    tArticle->mHeatFlux = -100.0;
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);

    tArticle->mHeatFlux = 0.0;
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heater link model access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeater::testAccessMethods()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Heat flux access method.
    tArticle->mHeatFlux = 50.0;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(50, tArticle->getHeatFlux(), 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heater link model modifier methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeater::testModifierMethods()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Set heater power method.
    tArticle->setHeaterPower(25);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(25, tArticle->mHeaterPower, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Heater link model restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHeater::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Reset of non-config and non-checkpointed attributes (currently there are none).
    tArticle->restart();

    UT_PASS_LAST;
}
