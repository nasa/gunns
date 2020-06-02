/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (core/GunnsFluidExternalDemand.o)
    )
***************************************************************************************************/
#include "UtGunnsFluidExternalDemand.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidExternalDemand class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidExternalDemand::UtGunnsFluidExternalDemand()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tLinkName(),
    tMaxConductivity(),
    tExpansionScaleFactor(),
    tBlockage(),
    tSourcePressure(),
    tConvertToType(),
    tFilterMinConductivity(),
    tFilterMinDeltaP(),
    tFilterCapacitanceGain(),
    tSupplyCapacitance(),
    tSupplyTemperature(),
    tSupplyMassFractions(),
    tSupplyTcMoleFractions(),
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
    tFluidInput2(),
    tFluidInput3()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidExternalDemand class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidExternalDemand::~UtGunnsFluidExternalDemand()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalDemand::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete [] tSupplyTcMoleFractions;
    delete [] tSupplyMassFractions;
    delete tFluidInput3;
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
void UtGunnsFluidExternalDemand::setUp()
{
    tLinkName              = "Test Fluid Potential";
    tNodeList.mNumNodes    = 3;
    tNodeList.mNodes       = tNodes;
    tMaxConductivity       = 0.1;
    tExpansionScaleFactor  = 0.0;
    tBlockage              = 0.5;
    tSourcePressure        = 600.0;
    tFilterMinConductivity = 1.0;
    tFilterMinDeltaP       = 1.0E-8;
    tFilterCapacitanceGain = 0.1;
    tConvertToType         = FluidProperties::GUNNS_N2;
    tSupplyCapacitance     = 1.0;
    tSupplyTemperature     = 300.0;
    tPort0                 = 2;
    tPort1                 = 0;
    tTimeStep              = 1.0;

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
    double extFractions[3] = {0.2, 0.79, 0.01};
    tFluidInput3 = new PolyFluidInputData(300.0,                    //temperature
                                          689.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          extFractions);            //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tLocalConfig);
    tNodes[1].initialize("UtTestNode1", tLocalConfig);
    tNodes[2].initialize("UtTestNode2", tLocalConfig);

    tNodes[0].getContent()->initialize(*tLocalConfig, *tFluidInput1);
    tNodes[1].getContent()->initialize(*tLocalConfig, *tFluidInput2);
    tNodes[2].getContent()->initialize(*tLocalConfig, *tFluidInput2);

    tNodes[0].setPotential(tFluidInput1->mPressure);
    tNodes[1].setPotential(tFluidInput2->mPressure);

    tSupplyMassFractions    = new double[3];
    tSupplyMassFractions[0] = 0.5;
    tSupplyMassFractions[1] = 0.3;
    tSupplyMassFractions[2] = 0.2;

    tSupplyTcMoleFractions    = new double[4];
    tSupplyTcMoleFractions[0] = 6.0e-6;
    tSupplyTcMoleFractions[1] = 7.0e-7;
    tSupplyTcMoleFractions[2] = 8.0e-8;
    tSupplyTcMoleFractions[3] = 9.0e-9;

    /// - Define nominal configuration data
    tConfigData = new GunnsFluidExternalDemandConfigData(tLinkName, &tNodeList, tMaxConductivity,
            tExpansionScaleFactor, tFilterMinConductivity, tFilterMinDeltaP, tFilterCapacitanceGain,
            tExternalConfig, tConvertToType);

    tInputData = new GunnsFluidExternalDemandInputData(true,
                                                       0.5,
                                                       tSourcePressure,
                                                       tSupplyCapacitance,
                                                       tSupplyTemperature,
                                                       tSupplyMassFractions,
                                                       tSupplyTcMoleFractions);

    tArticle = new FriendlyGunnsFluidExternalDemand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalDemand::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidExternalDemand 01: testConfig ..........................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName              == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                 == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity       == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor  == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFilterMinConductivity == tConfigData->mFilterMinConductivity);
    CPPUNIT_ASSERT(tFilterMinDeltaP       == tConfigData->mFilterMinDeltaP);
    CPPUNIT_ASSERT(tFilterCapacitanceGain == tConfigData->mFilterCapacitanceGain);
    CPPUNIT_ASSERT(tExternalConfig        == tConfigData->mExternalConfig);
    CPPUNIT_ASSERT(tConvertToType         == tConfigData->mConvertToType);

    /// - Check default config construction
    GunnsFluidExternalDemandConfigData defaultConfig;
    CPPUNIT_ASSERT(""     == defaultConfig.mName);
    CPPUNIT_ASSERT(0      == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0    == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0    == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(1.0    == defaultConfig.mFilterMinConductivity);
    CPPUNIT_ASSERT(1.0E-8 == defaultConfig.mFilterMinDeltaP);
    CPPUNIT_ASSERT(0.05   == defaultConfig.mFilterCapacitanceGain);
    CPPUNIT_ASSERT(0      == defaultConfig.mExternalConfig);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == defaultConfig.mConvertToType);

    /// - Check copy config construction
    GunnsFluidExternalDemandConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName              == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity       == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor  == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFilterMinConductivity == copyConfig.mFilterMinConductivity);
    CPPUNIT_ASSERT(tFilterMinDeltaP       == copyConfig.mFilterMinDeltaP);
    CPPUNIT_ASSERT(tFilterCapacitanceGain == copyConfig.mFilterCapacitanceGain);
    CPPUNIT_ASSERT(tExternalConfig        == copyConfig.mExternalConfig);
    CPPUNIT_ASSERT(tConvertToType         == copyConfig.mConvertToType);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalDemand::testInput()
{
    std::cout << "\n UtGunnsFluidExternalDemand 02: testInput ...........................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5                  == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tSourcePressure      == tInputData->mSourcePressure);
    CPPUNIT_ASSERT(tSupplyCapacitance   == tInputData->mSupplyCapacitance);
    CPPUNIT_ASSERT(tSupplyTemperature   == tInputData->mSupplyTemperature);
    CPPUNIT_ASSERT(tSupplyMassFractions == tInputData->mSupplyMassFractions);
    CPPUNIT_ASSERT(0.5                  == tInputData->mSupplyMassFractions[0]);
    CPPUNIT_ASSERT(0.3                  == tInputData->mSupplyMassFractions[1]);
    CPPUNIT_ASSERT(0.2                  == tInputData->mSupplyMassFractions[2]);

    /// - Check default config construction
    GunnsFluidExternalDemandInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                  == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                  == defaultInput.mSourcePressure);
    CPPUNIT_ASSERT(0.0                  == defaultInput.mSupplyCapacitance);
    CPPUNIT_ASSERT(0.0                  == defaultInput.mSupplyTemperature);
    CPPUNIT_ASSERT(0                    == defaultInput.mSupplyMassFractions);

    /// - Check copy config construction
    GunnsFluidExternalDemandInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tSourcePressure                == copyInput.mSourcePressure);
    CPPUNIT_ASSERT(tSupplyCapacitance             == copyInput.mSupplyCapacitance);
    CPPUNIT_ASSERT(tSupplyTemperature             == copyInput.mSupplyTemperature);
    CPPUNIT_ASSERT(tSupplyMassFractions           == copyInput.mSupplyMassFractions);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalDemand::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidExternalDemand 03: testDefaultConstruction .............";

    /// @test config data
    CPPUNIT_ASSERT(0.0 == tArticle->mSourcePressure);
    CPPUNIT_ASSERT(0.0 == tArticle->mDemandFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mDemandTemperature);
    CPPUNIT_ASSERT(0   == tArticle->mDemandMassFractions);
    CPPUNIT_ASSERT(0.0 == tArticle->mSupplyCapacitance);
    CPPUNIT_ASSERT(0.0 == tArticle->mSupplyPressure);
    CPPUNIT_ASSERT(0.0 == tArticle->mSupplyTemperature);
    CPPUNIT_ASSERT(0   == tArticle->mSupplyMassFractions);
    CPPUNIT_ASSERT(0.0 == tArticle->mFilterMinConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mFilterMinDeltaP);
    CPPUNIT_ASSERT(0   == tArticle->mTransformMap);
    CPPUNIT_ASSERT(0.0 == tArticle->mAvgDemand);
    CPPUNIT_ASSERT(0.0 == tArticle->mAvgSupplyP);
    CPPUNIT_ASSERT(0.0 == tArticle->mAvgSupplyDeltaP);
    CPPUNIT_ASSERT(0.0 == tArticle->mEstimatedCapacitance);
    CPPUNIT_ASSERT(0.0 == tArticle->mFilterCapacitanceGain);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidExternalDemand* article = new GunnsFluidExternalDemand();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalDemand::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidExternalDemand 04: testNominalInitialization ...........";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidExternalDemand article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test config and input data
    CPPUNIT_ASSERT(tBlockage              == article.mMalfBlockageValue);
    CPPUNIT_ASSERT(tSourcePressure        == article.mSourcePressure);
    CPPUNIT_ASSERT(tFilterMinConductivity == article.mFilterMinConductivity);
    CPPUNIT_ASSERT(tFilterMinDeltaP       == article.mFilterMinDeltaP);
    CPPUNIT_ASSERT(tFilterCapacitanceGain == article.mFilterCapacitanceGain);

    /// - Test transform map.
    CPPUNIT_ASSERT_EQUAL(3, article.mTransformMap[0]);
    CPPUNIT_ASSERT_EQUAL(1, article.mTransformMap[1]);
    CPPUNIT_ASSERT_EQUAL(0, article.mTransformMap[2]);
    CPPUNIT_ASSERT_EQUAL(0, article.mTransformMap[3]);

    /// - Test output demand data.
    CPPUNIT_ASSERT(article.mDemandMassFractions);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                        article.mDemandFlux,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getContent()->getTemperature(),   article.mDemandTemperature,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getContent()->getMassFraction(0), article.mDemandMassFractions[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getContent()->getMassFraction(1), article.mDemandMassFractions[1], 0.0);
    CPPUNIT_ASSERT(tNodes[tPort1].getContent()->getTraceCompounds()->getMoleFractions() == article.mDemandTcMoleFractions);

    /// - Test input supply data.
    CPPUNIT_ASSERT(article.mDemandMassFractions);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSupplyCapacitance,        article.mSupplyCapacitance,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSourcePressure,           article.mSupplyPressure,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSupplyTemperature,        article.mSupplyTemperature,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSupplyMassFractions[0],   article.mSupplyMassFractions[0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSupplyMassFractions[1],   article.mSupplyMassFractions[1],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSupplyMassFractions[2],   article.mSupplyMassFractions[2],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSupplyTcMoleFractions[0], article.mSupplyTcMoleFractions[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSupplyTcMoleFractions[1], article.mSupplyTcMoleFractions[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSupplyTcMoleFractions[2], article.mSupplyTcMoleFractions[2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tSupplyTcMoleFractions[3], article.mSupplyTcMoleFractions[3], 0.0);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Verify a double init works and init without specifying initial supply terms.
    tInputData->mSupplyCapacitance     = 0.0;
    tInputData->mSupplyTemperature     = 0.0;
    tInputData->mSupplyMassFractions   = 0;
    tInputData->mSupplyTcMoleFractions = 0;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mSupplyCapacitance,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getContent()->getTemperature(), article.mSupplyTemperature,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,                                      article.mSupplyMassFractions[0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mSupplyMassFractions[1],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mSupplyMassFractions[2],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mSupplyTcMoleFractions[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mSupplyTcMoleFractions[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mSupplyTcMoleFractions[2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mSupplyTcMoleFractions[3], 0.0);
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Call the validate method for code coverage, although there is nothing to test.
    article.validate();

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test initialization exceptions
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalDemand::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidExternalDemand 05: testInitializationExceptions ........";

    /// - Check port rules - initialize with port 1 set to the ground node and verify the link fails
    ///   to initialize
    tPort0 = 1;
    tPort1 = 2;
    FriendlyGunnsFluidExternalDemand article;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.mInitFlag);

    /// - Initialize with port 0 not set to the ground node and verify the link fails to initialize.
    tPort0 = 0;
    tPort1 = 1;
    FriendlyGunnsFluidExternalDemand article2;
    CPPUNIT_ASSERT_THROW(article2.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article2.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the processInputs and processOutputs methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalDemand::testIoMethods()
{
    std::cout << "\n UtGunnsFluidExternalDemand 06: testIoMethods .......................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set the read data as if the sim bus set it, and verify the read method processes the data.
    tArticle->mSupplyPressure = 600.0;
    tArticle->mSupplyTemperature = 290.0;
    tArticle->mSupplyMassFractions[0] = 0.4;  // External O2
    tArticle->mSupplyMassFractions[1] = 0.5;  // External N2
    tArticle->mSupplyMassFractions[2] = 0.1;  // External CO2

    tArticle->processInputs();

    /// - The CO2 is converted to N2, so the mass fraction of N2 should be 0.5 + 0.1 = 0.6.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(600.0, tArticle->mSourcePressure, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0, tNodes[0].getContent()->getTemperature(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.6, tNodes[0].getContent()->getMassFraction(0), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.4, tNodes[0].getContent()->getMassFraction(1), 0.0);

    /// - Verify that if the link blockage is fully active, the processInputs method does not
    ///   modify the node contents.
    tNodes[0].getContent()->setTemperature(300.0);
    tArticle->mMalfBlockageFlag  = true;
    tArticle->mMalfBlockageValue = 1.0;
    tArticle->step(0.0);
    tArticle->processInputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, tNodes[0].getContent()->getTemperature(), 0.0);

    /// - Verify the write method grabs the supply node properties for write to the sim bus.  When
    ///   flux is positive, the write method should send the port 1 node content fluid properties.
    tArticle->mFlux     = 1.0;
    tArticle->mFlowRate = tNodes[0].getContent()->getMWeight();
    double expectedMdot = tArticle->mFlowRate;
    tArticle->processOutputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlux, tArticle->mDemandFlux, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot,    tArticle->mFlowRate,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(),
            tArticle->mDemandTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMassFraction(0),
            tArticle->mDemandMassFractions[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMassFraction(1),
            tArticle->mDemandMassFractions[1], 0.0);

    /// - When flux is negative, the write method should send the port 1 node inflow fluid
    ///   properties.
    tArticle->mFlux     = -1.0;
    tArticle->mFlowRate = -1.0;
    expectedMdot        = -tNodes[0].getInflow()->getMWeight();
    tArticle->processOutputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mFlux, tArticle->mDemandFlux, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot,    tArticle->mFlowRate,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getInflow()->getTemperature(),
            tArticle->mDemandTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getInflow()->getMassFraction(0),
            tArticle->mDemandMassFractions[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getInflow()->getMassFraction(1),
            tArticle->mDemandMassFractions[1], 0.0);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalDemand::testStep()
{
    std::cout << "\n UtGunnsFluidExternalDemand 07: testStep ............................";

    /// - Initialize default test article with nominal initialization data.
    tInputData->mMalfBlockageFlag = false;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Make the first step with supply pressure zero.
    tArticle->mSupplyPressure = 0.0;
    tArticle->mSupplyTemperature = 290.0;
    tArticle->mSupplyMassFractions[0] = 0.4;  // External O2
    tArticle->mSupplyMassFractions[1] = 0.5;  // External N2
    tArticle->mSupplyMassFractions[2] = 0.1;  // External CO2

    tArticle->processInputs();
    tArticle->step(tTimeStep);

    /// - Check the filtered link conductivity after the 1st step.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAvgDemand,                          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAvgSupplyDeltaP,                    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAvgSupplyP,                         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mEstimatedCapacitance,               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mEffectiveConductivity,              DBL_EPSILON);

    /// - Step again with an increasing supply pressure.  Set the link's flux to simulate network
    ///   flow.
    tArticle->mSupplyPressure = 100.0;
    tArticle->processInputs();
    tArticle->mFlux = 100.0;
    tArticle->step(tTimeStep);

    /// - Check the filtered link conductivity after the 2nd step.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5*tArticle->mFlux, tArticle->mAvgDemand, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5*100.0, tArticle->mAvgSupplyDeltaP,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5*100.0, tArticle->mAvgSupplyP,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-.1, tArticle->mEstimatedCapacitance,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mEffectiveConductivity,     DBL_EPSILON);

    /// - Check the base class was stepped.
    CPPUNIT_ASSERT(tArticle->mSourceVector[1] > 0.0);

    /// - Step again with dt = 0 and verify the result.
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFilterMinConductivity, tArticle->mEffectiveConductivity, 0.0);

    /// - Step again and verify the supplied network capacitance mode.
    tArticle->mSupplyCapacitance = 1.0;
    tArticle->step(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, tArticle->mEffectiveConductivity,      DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidExternalDemand::testRestart()
{
    std::cout << "\n UtGunnsFluidExternalDemand 08: testRestart .........................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Test article and base class attributes are restarted.
    tArticle->mPower           = 1.0;
    tArticle->mAvgSupplyDeltaP = 2.0;
    tArticle->restart();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPower,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAvgSupplyDeltaP, 0.0);

    std::cout << "... Pass";
}
