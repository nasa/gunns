/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsFluidFlowIntegrator.o))
***************************************************************************************************/

#include "UtGunnsFluidFlowIntegrator.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidFlowIntegrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidFlowIntegrator::UtGunnsFluidFlowIntegrator()
    :
    tArticle(tConductorLink),
    tNodes(),
    tNodeList(),
    tName("test article"),
    tConfig(tName),
    tInput(),
    tFluidProperties(),
    tFluidConfig(),
    tFluidInput1(),
    tFluidInput2(),
    tMassFractions0(),
    tMassFractions1(),
    tLinks(),
    tConductorLink(),
    tConductorLinkConfig(),
    tConductorLinkInput(),
    tMaxConductivity(),
    tExpansionScaleFactor(),
    tPort0(),
    tPort1(),
    tTimeStep(),
    tTolerance()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidFlowIntegrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidFlowIntegrator::~UtGunnsFluidFlowIntegrator()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowIntegrator::tearDown()
{
    /// - Deletes for news in setUp
    delete tConductorLinkInput;
    delete tConductorLinkConfig;
    delete tFluidInput1;
    delete tFluidInput2;
    delete tFluidConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowIntegrator::setUp()
{
    /// - Test spotter configuration.
    tConfig.mName       = tName;

    /// - Setup some test fluid nodes.
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2];
    types[0] = FluidProperties::GUNNS_N2;
    types[1] = FluidProperties::GUNNS_O2;
    tMassFractions0[0] = 0.5;
    tMassFractions0[1] = 0.5;
    tMassFractions1[0] = 0.8;
    tMassFractions1[1] = 0.2;
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 2);
    tFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tMassFractions0);         //massFractions
    tFluidInput2 = new PolyFluidInputData(294.261,                  //temperature
                                          689.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tMassFractions1);         //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig);
    tNodes[1].initialize("UtTestNode1", tFluidConfig);

    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput2);

    tNodes[0].setPotential(tFluidInput1->mPressure);
    tNodes[1].setPotential(tFluidInput2->mPressure);

    tNodes[0].initVolume(1.0);
    tNodes[1].initVolume(2.0);

    /// - Test fluid conductor initialization.
    tNodeList.mNumNodes   = 3;
    tNodeList.mNodes      = tNodes;
    tMaxConductivity      = 0.5;
    tExpansionScaleFactor = 0.4;
    tPort0                = 0;
    tPort1                = 1;
    tTimeStep             = 0.1;
    tTolerance            = 1.0E-07;

    /// - Define configuration and input data for the test conductor
    tConductorLinkConfig = new GunnsFluidConductorConfigData(
            "Test Fluid Conductor", &tNodeList, tMaxConductivity, tExpansionScaleFactor);
    tConductorLinkInput  = new GunnsFluidConductorInputData(true, 0.5);

    tConductorLink.initialize(*tConductorLinkConfig, *tConductorLinkInput, tLinks, tPort0, tPort1);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsFluidFlowIntegratorConfigData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowIntegrator::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidFlowIntegrator 01: testConfig ..........................";

    /// - Test nominal config data construction.
    CPPUNIT_ASSERT(tName == tConfig.mName);

    /// - Test default config data construction.
    GunnsFluidFlowIntegratorConfigData article(tName);
    CPPUNIT_ASSERT(tName == article.mName);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsFluidFlowIntegratorInputData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowIntegrator::testInput()
{
    std::cout << "\n UtGunnsFluidFlowIntegrator 02: testInput ...........................";

    /// - Currently nothing to test.

    std::cout << "... Pass";
}

#include <iostream>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsFluidFlowIntegrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowIntegrator::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidFlowIntegrator 03: testDefaultConstruction .............";

    /// @test state data
    CPPUNIT_ASSERT(""          ==  tArticle.mName);
    CPPUNIT_ASSERT(tConductorLink.getName() ==  tArticle.mLink.getName());

    /// @test init flag
    CPPUNIT_ASSERT(false      ==  tArticle.mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidFlowIntegrator* article = new GunnsFluidFlowIntegrator(tConductorLink);
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsFluidFlowIntegrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowIntegrator::testInitialize()
{
    std::cout << "\n UtGunnsFluidFlowIntegrator 04: testInitialize ......................";

    tArticle.initialize(&tConfig, &tInput);

    /// - Test nominal initialization.
    CPPUNIT_ASSERT(tName      ==  tArticle.mName);

    /// @test init flag
    CPPUNIT_ASSERT(true       ==  tArticle.mInitFlag);

    /// @test should have two fluid consituents in this test network.
    CPPUNIT_ASSERT(2          ==  tArticle.mNumFluidConstituents);

    /// - Test exception thrown from missing name.
    tConfig.mName = "";
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tConfig.mName = tName;

    /// - Test exception thrown from null config data.
    const GunnsFluidFlowIntegratorConfigData* nullConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(nullConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown from null input data.
    const GunnsFluidFlowIntegratorInputData* nullInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, nullInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown on bad config data pointer type.
    BadGunnsFluidFlowIntegratorConfigData* badConfig = new BadGunnsFluidFlowIntegratorConfigData(tName);
    CPPUNIT_ASSERT_THROW(tArticle.initialize(badConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badConfig;

    /// - Test exception thrown on bad input data pointer type.
    BadGunnsFluidFlowIntegratorInputData* badInput = new BadGunnsFluidFlowIntegratorInputData();
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, badInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badInput;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsFluidFlowIntegrator class.
///           That method doesn't actually do anything so there are no asserts, but this is done for
///           complete code coverage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowIntegrator::testPreSolver()
{
    std::cout << "\n UtGunnsFluidFlowIntegrator 05: testPreSolver .......................";

    tArticle.initialize(&tConfig, &tInput);
    tArticle.stepPreSolver(tTimeStep);

    for(int fluidIndex = 0; fluidIndex < tFluidConfig->mNTypes; fluidIndex++){
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tMassFractions0[fluidIndex], tArticle.mMassFractionsPort0[fluidIndex], tTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tMassFractions1[fluidIndex], tArticle.mMassFractionsPort1[fluidIndex], tTolerance);
    }

    std::cout << "... Pass";
}

#include <iostream>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsFluidFlowIntegrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowIntegrator::testPostSolver()
{
    std::cout << "\n UtGunnsFluidFlowIntegrator 06: testPostSolver ......................";

    tArticle.initialize(&tConfig, &tInput);
    tArticle.stepPreSolver(tTimeStep);

    /// - Set up the conductor link to compute forward flow.
    double* linkPotentials = tConductorLink.getPotentialVector();
    double* linkAdmittance = tConductorLink.getAdmittanceMatrix();
    linkPotentials[0] = 101.0;
    linkPotentials[1] = 100.0;
    linkAdmittance[0] = 1.0e-6;
    tConductorLink.computeFlows(tTimeStep);
    double actualFlow = tConductorLink.getFlowRate();

    double expectedMasses[2];
    expectedMasses[0] = actualFlow * tTimeStep * tMassFractions0[0];
    expectedMasses[1] = actualFlow * tTimeStep * tMassFractions0[1];

    /// - Verify spotter integrates forward flow.
    tArticle.stepPostSolver(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMasses[0], tArticle.mIntegratedMasses[0], tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMasses[1], tArticle.mIntegratedMasses[1], tTolerance);

    /// - Set up the link for reverse flow and verify spotter outputs.
    linkPotentials[0] = 100.0;
    linkPotentials[1] = 101.0;
    tConductorLink.computeFlows(tTimeStep);
    actualFlow = tConductorLink.getFlowRate();

    expectedMasses[0] += actualFlow * tTimeStep * tMassFractions1[0];
    expectedMasses[1] += actualFlow * tTimeStep * tMassFractions1[1];

    tArticle.stepPostSolver(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMasses[0], tArticle.mIntegratedMasses[0], tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMasses[1], tArticle.mIntegratedMasses[1], tTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the setter & getter methods of the GunnsFluidFlowIntegrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidFlowIntegrator::testAccessors()
{
    std::cout << "\n UtGunnsFluidFlowIntegrator 07: testAccessors .......................";

    tArticle.initialize(&tConfig, &tInput);

    tArticle.mIntegratedMasses[0] = 1.0;
    tArticle.mIntegratedMasses[1] = 2.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle.getIntegratedMass(0), tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, tArticle.getIntegratedMass(1), tTolerance);

    std::cout << "... Pass";
}
