/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
(
   (core/GunnsFluidExternalSupply.o)
)
***************************************************************************************************/
#include "UtGunnsFluidExternalSupply.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidExternalSupply class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidExternalSupply::UtGunnsFluidExternalSupply()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tLinkName(),
    tInitialFlowDemand(),
    tConvertToType(),
    tUseNetworkCapacitance(),
    tDemandTemperature(),
    tDemandMassFractions(),
    tDemandTcMoleFractions(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tPort1(),
    tTimeStep(),
    tFluidProperties(),
    tTcProperties(),
    tTcTypes(),
    tFluidTcConfig(),
    tLocalConfig(),
    tExternalConfig(),
    tFluidInput1(),
    tFluidInput2()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidExternalSupply class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidExternalSupply::~UtGunnsFluidExternalSupply()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalSupply::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete [] tDemandTcMoleFractions;
    delete [] tDemandMassFractions;
    delete tExternalConfig;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tLocalConfig;
    delete tFluidProperties;
    delete tFluidTcConfig;
    delete tTcProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalSupply::setUp()
{
    tLinkName              = "Test Fluid External Supply";
    tNodeList.mNumNodes    = 3;
    tNodeList.mNodes       = tNodes;
    tInitialFlowDemand     = 0.5;
    tConvertToType         = FluidProperties::GUNNS_N2;
    tUseNetworkCapacitance = true;
    tDemandTemperature     = 300.0;
    tPort0                 = 0;
    tPort1                 = 2;
    tTimeStep              = 0.1;

    /// - Setup some fluid nodes
    tTcProperties  = new DefinedChemicalCompounds();
    tTcTypes[0]    = ChemicalCompound::CH2O;
    tTcTypes[1]    = ChemicalCompound::C2H6O;
    tTcTypes[2]    = ChemicalCompound::C4H4O;
    tTcTypes[3]    = ChemicalCompound::C8H10;
    tFluidTcConfig = new GunnsFluidTraceCompoundsConfigData(tTcTypes, 4, "tFluidTcConfig");

    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2] = {FluidProperties::GUNNS_N2, FluidProperties::GUNNS_O2};
    tLocalConfig = new PolyFluidConfigData(tFluidProperties, types, 2, tFluidTcConfig);
    double fractions[2] = {0.5, 0.5};
    tFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions
    tFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          689.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions

    FluidProperties::FluidType extTypes[3] = {FluidProperties::GUNNS_O2,
                                              FluidProperties::GUNNS_N2,
                                              FluidProperties::GUNNS_CO2};
    tExternalConfig = new PolyFluidConfigData(tFluidProperties, extTypes, 3);

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tLocalConfig, tFluidInput1);
    tNodes[1].initialize("UtTestNode1", tLocalConfig, tFluidInput2);
    tNodes[2].initialize("UtTestNode2", tLocalConfig, tFluidInput2);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[2].resetFlows();

    tDemandMassFractions    = new double[3];
    tDemandMassFractions[0] = 0.5;
    tDemandMassFractions[1] = 0.3;
    tDemandMassFractions[2] = 0.2;

    tDemandTcMoleFractions    = new double[4];
    tDemandTcMoleFractions[0] = 6.0e-6;
    tDemandTcMoleFractions[1] = 7.0e-7;
    tDemandTcMoleFractions[2] = 8.0e-8;
    tDemandTcMoleFractions[3] = 9.0e-9;

    /// - Define nominal configuration data
    tConfigData = new GunnsFluidExternalSupplyConfigData(tLinkName, &tNodeList, tExternalConfig,
            tConvertToType, tUseNetworkCapacitance);

    /// - Define nominal input data
    tInputData = new GunnsFluidExternalSupplyInputData(true,
                                                       0.5,
                                                       tInitialFlowDemand,
                                                       tDemandTemperature,
                                                       tDemandMassFractions,
                                                       tDemandTcMoleFractions);

    tArticle = new FriendlyGunnsFluidExternalSupply;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalSupply::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidExternalSupply 01: testConfig ..........................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName              == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                 == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tExternalConfig        == tConfigData->mExternalConfig);
    CPPUNIT_ASSERT(tConvertToType         == tConfigData->mConvertToType);
    CPPUNIT_ASSERT(tUseNetworkCapacitance == tConfigData->mUseNetworkCapacitance);

    /// - Check default config construction
    GunnsFluidExternalSupplyConfigData defaultConfig;
    CPPUNIT_ASSERT(""                        == defaultConfig.mName);
    CPPUNIT_ASSERT(0                         == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0                         == defaultConfig.mExternalConfig);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == defaultConfig.mConvertToType);
    CPPUNIT_ASSERT(false                     == defaultConfig.mUseNetworkCapacitance);

    /// - Check copy config construction
    GunnsFluidExternalSupplyConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName              == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tExternalConfig        == copyConfig.mExternalConfig);
    CPPUNIT_ASSERT(tConvertToType         == copyConfig.mConvertToType);
    CPPUNIT_ASSERT(tUseNetworkCapacitance == copyConfig.mUseNetworkCapacitance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalSupply::testInput()
{
    std::cout << "\n UtGunnsFluidExternalSupply 02: testInput ...........................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5                  == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tInitialFlowDemand   == tInputData->mFlowDemand);
    CPPUNIT_ASSERT(tDemandTemperature   == tInputData->mDemandTemperature);
    CPPUNIT_ASSERT(tDemandMassFractions == tInputData->mDemandMassFractions);
    CPPUNIT_ASSERT(0.5                  == tInputData->mDemandMassFractions[0]);
    CPPUNIT_ASSERT(0.3                  == tInputData->mDemandMassFractions[1]);
    CPPUNIT_ASSERT(0.2                  == tInputData->mDemandMassFractions[2]);

    /// - Check default config construction
    GunnsFluidExternalSupplyInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                  == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                  == defaultInput.mFlowDemand);
    CPPUNIT_ASSERT(0.0                  == defaultInput.mDemandTemperature);
    CPPUNIT_ASSERT(0                    == defaultInput.mDemandMassFractions);

    /// - Check copy config construction
    GunnsFluidExternalSupplyInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInitialFlowDemand             == copyInput.mFlowDemand);
    CPPUNIT_ASSERT(tDemandTemperature             == copyInput.mDemandTemperature);
    CPPUNIT_ASSERT(tDemandMassFractions           == copyInput.mDemandMassFractions);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalSupply::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidExternalSupply 03: testDefaultConstruction .............";

    /// @test config data
    CPPUNIT_ASSERT(false == tArticle->mUseNetworkCapacitance);
    CPPUNIT_ASSERT(0.0   == tArticle->mFlowDemand);
    CPPUNIT_ASSERT(0.0   == tArticle->mSupplyCapacitance);
    CPPUNIT_ASSERT(0.0   == tArticle->mSupplyPressure);
    CPPUNIT_ASSERT(0.0   == tArticle->mSupplyTemperature);
    CPPUNIT_ASSERT(0     == tArticle->mSupplyMassFractions);
    CPPUNIT_ASSERT(0.0   == tArticle->mDemandFlux);
    CPPUNIT_ASSERT(0.0   == tArticle->mDemandTemperature);
    CPPUNIT_ASSERT(0     == tArticle->mDemandMassFractions);
    CPPUNIT_ASSERT(0     == tArticle->mTransformMap);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidExternalSupply* article = new GunnsFluidExternalSupply();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalSupply::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidExternalSupply 04: testNominalInitialization ...........";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidExternalSupply article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT_EQUAL(tUseNetworkCapacitance, article.mUseNetworkCapacitance);

    /// - Test internal fluid and transform map.
    CPPUNIT_ASSERT(article.mInternalFluid);
    CPPUNIT_ASSERT_EQUAL(3, article.mTransformMap[0]);
    CPPUNIT_ASSERT_EQUAL(1, article.mTransformMap[1]);
    CPPUNIT_ASSERT_EQUAL(0, article.mTransformMap[2]);
    CPPUNIT_ASSERT_EQUAL(0, article.mTransformMap[3]);

    /// - Test output supply data.
    CPPUNIT_ASSERT(article.mSupplyMassFractions);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                        article.mSupplyCapacitance,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getPressure(),      article.mSupplyPressure,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(),   article.mSupplyTemperature,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMassFraction(0), article.mSupplyMassFractions[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMassFraction(1), article.mSupplyMassFractions[1], 0.0);
    CPPUNIT_ASSERT(tNodes[tPort0].getContent()->getTraceCompounds()->getMoleFractions() == article.mSupplyTcMoleFractions);

    /// - Test input demand data.
    CPPUNIT_ASSERT(article.mDemandMassFractions);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                       article.mDemandFlux,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDemandTemperature,        article.mDemandTemperature,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDemandMassFractions[0],   article.mDemandMassFractions[0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDemandMassFractions[1],   article.mDemandMassFractions[1],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDemandMassFractions[2],   article.mDemandMassFractions[2],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDemandTcMoleFractions[0], article.mDemandTcMoleFractions[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDemandTcMoleFractions[1], article.mDemandTcMoleFractions[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDemandTcMoleFractions[2], article.mDemandTcMoleFractions[2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDemandTcMoleFractions[3], article.mDemandTcMoleFractions[3], 0.0);

    /// - Test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Verify a double init works and init without specifying initial demand terms.
    tInputData->mDemandTemperature     = 0.0;
    tInputData->mDemandMassFractions   = 0;
    tInputData->mDemandTcMoleFractions = 0;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(), article.mDemandTemperature,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,                                      article.mDemandMassFractions[0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mDemandMassFractions[1],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mDemandMassFractions[2],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mDemandTcMoleFractions[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mDemandTcMoleFractions[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mDemandTcMoleFractions[2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mDemandTcMoleFractions[3], 0.0);
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Call the validate method for code coverage, although there is nothing to test.
    article.validate();

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalSupply::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidExternalSupply 05: testInitializationExceptions ........";

    /// - Check port rules - initialize with port 0 set to the ground node and verify the link fails
    ///   to initialize
    tPort0 = 2;
    tPort1 = 1;
    FriendlyGunnsFluidExternalSupply article;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.mInitFlag);

    /// - Initialize with port 1 not set to the ground node and verify the link fails to initialize.
    tPort0 = 0;
    tPort1 = 1;
    FriendlyGunnsFluidExternalSupply article2;
    CPPUNIT_ASSERT_THROW(article2.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article2.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the processInputs and processOutputs methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalSupply::testIoMethods()
{
    std::cout << "\n UtGunnsFluidExternalSupply 06: testIoMethods .......................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set the read data as if the sim bus set it, and verify the read method processes the data.
    tArticle->mDemandFlux = 1.0;
    tArticle->mFlowDemand = 2.0;
    tArticle->mDemandTemperature = 290.0;
    tArticle->mDemandMassFractions[0] = 0.4;  // External O2
    tArticle->mDemandMassFractions[1] = 0.5;  // External N2
    tArticle->mDemandMassFractions[2] = 0.1;  // External CO2

    tArticle->processInputs();

    /// - The CO2 is converted to N2, so the mass fraction of N2 should be 0.5 + 0.1 = 0.6.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, tArticle->mFlux,                              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  2.0, tArticle->mFlowRate,                          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0, tArticle->mInternalFluid->getTemperature(),   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.6, tArticle->mInternalFluid->getMassFraction(0), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.4, tArticle->mInternalFluid->getMassFraction(1), 0.0);

    /// - Verify the write method grabs the supply node properties for write to the sim bus.
    tNodes[0].setNetworkCapacitance(1.0);
    tArticle->processOutputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getNetworkCapacitance(),
            tArticle->mSupplyCapacitance, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getPotential(),
            tArticle->mSupplyPressure, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(),
            tArticle->mSupplyTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMassFraction(0),
            tArticle->mSupplyMassFractions[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMassFraction(1),
            tArticle->mSupplyMassFractions[1], 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalSupply::testStep()
{
    std::cout << "\n UtGunnsFluidExternalSupply 07: testStep ............................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set the read data as if the sim bus set it.
    tArticle->mDemandFlux = 1.0;
    tArticle->mDemandTemperature = 290.0;
    tArticle->mDemandMassFractions[0] = 0.4;  // External O2
    tArticle->mDemandMassFractions[1] = 0.5;  // External N2
    tArticle->mDemandMassFractions[2] = 0.1;  // External CO2

    /// - Verify the link mass flow rate based on flow direction and molecular weight of the source.
    ///   Positive flux goes from the supply node to the external network, so use the molecular
    ///   weight of the supply node.
    double expectedMdot = tArticle->mDemandFlux * tNodes[0].getContent()->getMWeight();
    double expectedFlux = tArticle->mDemandFlux;
    tArticle->processInputs();
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,     DBL_EPSILON);

    /// - Check negative flow rate, where the source fluid is the link's internal fluid.
    tArticle->mDemandFlux = -1.0;
    expectedMdot = tArticle->mDemandFlux * tArticle->mInternalFluid->getMWeight();
    expectedFlux = tArticle->mDemandFlux;
    tArticle->processInputs();
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,     DBL_EPSILON);

    /// - Check flux demand overrides mass demand if both are set.
    tArticle->mFlowDemand = 2.0;
    tArticle->processInputs();
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,     DBL_EPSILON);

    /// - Check positive mass flow when there is no flux demand.
    tArticle->mDemandFlux = 0.0;
    expectedMdot          = tArticle->mFlowDemand;
    expectedFlux          = tArticle->mFlowDemand / tNodes[0].getContent()->getMWeight();
    tArticle->processInputs();
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,     DBL_EPSILON);

    /// - Check negative mass flow demand.
    tArticle->mFlowDemand =-1.0;
    expectedMdot          = tArticle->mFlowDemand;
    expectedFlux          = tArticle->mFlowDemand / tArticle->mInternalFluid->getMWeight();
    double request        = GunnsFluidExternalSupply::mNetworkCapacitanceFlux;
    tArticle->processInputs();
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(request,      tNodes[0].getNetworkCapacitanceRequest(), DBL_EPSILON);

    /// - Check zero demands.
    tArticle->mUseNetworkCapacitance = false;
    tNodes[0].setNetworkCapacitanceRequest(0.0);
    tArticle->mFlowDemand = 0.0;
    expectedMdot          = 0.0;
    expectedFlux          = 0.0;
    request               = 0.0;
    tArticle->processInputs();
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mFlux,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(request,      tNodes[0].getNetworkCapacitanceRequest(), DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalSupply::testComputeFlows()
{
    std::cout << "\n UtGunnsFluidExternalSupply 08: testComputeFlows ....................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set the read data as if the sim bus set it.
    tArticle->mDemandFlux = 1.0;
    tArticle->mDemandTemperature = 290.0;
    tArticle->mDemandMassFractions[0] = 0.4;  // External O2
    tArticle->mDemandMassFractions[1] = 0.5;  // External N2
    tArticle->mDemandMassFractions[2] = 0.1;  // External CO2

    tArticle->processInputs();
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mDemandFlux, tArticle->mFlux, 0.0);

    /// - Verify the computeFlows method outputs
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlux, tNodes[tPort0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tNodes[tPort1].getScheduledOutflux(), DBL_EPSILON);

    /// - Verify the transportFlows method outputs
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(0.0 < tNodes[0].getOutflux());

    /// - Verify the internal fluid reflects the source node properties.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInternalFluid->getMassFraction(0),
                                 tNodes[0].getOutflow()->getMassFraction(0), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInternalFluid->getTemperature(),
                                 tNodes[0].getOutflow()->getTemperature(), 0.0);

    /// - Set up negative demand direction.
    tArticle->mDemandFlux = -1.0;
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[2].resetFlows();

    tArticle->processInputs();
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mDemandFlux, tArticle->mFlux, 0.0);

    /// - Verify the computeFlows method outputs
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-tArticle->mFlux, tNodes[tPort1].getScheduledOutflux(), DBL_EPSILON);

    /// - Verify the transportFlows method outputs
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tNodes[0].getOutflux());
    CPPUNIT_ASSERT(0.0 <  tNodes[0].getInflux());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalSupply::testRestart()
{
    std::cout << "\n UtGunnsFluidExternalSupply 09: testRestart .........................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Test base class attributes are restarted.
    tArticle->mPower = 1.0;
    tArticle->restart();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPower, 0.0);

    std::cout << "... Pass";
}
