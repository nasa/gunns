/**
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "UtGunnsFluidMultiSeparator.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsFluidMultiSeparator::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidMultiSeparator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidMultiSeparator::UtGunnsFluidMultiSeparator()
    :
    tLinkName(),
    tMaxConductance(),
    tFluidTypes(),
    tFluidPorts(),
    tTcTypes(),
    tTcPorts(),
    tConfigData(),
    tMalfBlockageFlag(),
    tMalfBlockageValue(),
    tFluidFractions(),
    tTcFractions(),
    tInputData(),
    tArticle(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tPort1(),
    tPort2(),
    tPort3(),
    tTimeStep(),
    tFluidProperties(),
    tTcConfig(),
    tFluidConfig(),
    tFluidInput0(),
    tFluidInput1(),
    tFluidInput2(),
    tFluidInput3(),
    fractions()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidMultiSeparator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidMultiSeparator::~UtGunnsFluidMultiSeparator()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput3;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tFluidInput0;
    delete tFluidConfig;
    delete [] fractions;
    delete tFluidProperties;
    delete tTcConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::setUp()
{
    tLinkName           = "Test Fluid Multi-Separator";
    tNodeList.mNumNodes = 5;
    tNodeList.mNodes    = tNodes;
    tPort0              = 0;
    tPort1              = 4; // Ground
    tPort2              = 1;
    tPort3              = 2;
    tTimeStep           = 0.1;

    /// - Setup some fluid nodes
    ChemicalCompound::Type tcTypes[2] = {ChemicalCompound::CO2, ChemicalCompound::CH4};
    tTcConfig = new GunnsFluidTraceCompoundsConfigData(tcTypes, 2, "tTcConfig");

    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[4];
    types[0]     = FluidProperties::GUNNS_N2;
    types[1]     = FluidProperties::GUNNS_WATER;
    types[2]     = FluidProperties::GUNNS_H2O;
    types[3]     = FluidProperties::GUNNS_H2;
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 4, tTcConfig);

    fractions = new double[4];
    fractions[0] = 0.99;
    fractions[1] = 0.0;
    fractions[2] = 0.009;
    fractions[3] = 0.001;
    double tcX[2] = {1.0e-6, 1.0e-7};
    GunnsFluidTraceCompoundsInputData tcInput(tcX);
    tFluidInput0 = new PolyFluidInputData(294.261,                  //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions,                //mass fractions
                                          &tcInput);                //TC mole fractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig, tFluidInput0);
    tNodes[0].resetFlows();

    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 1.0;
    fractions[3] = 0.0;
    tFluidInput1 = new PolyFluidInputData(294.261,                  //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //mass fractions

    tNodes[1].initialize("UtTestNode1", tFluidConfig, tFluidInput1);
    tNodes[1].resetFlows();

    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 0.0;
    fractions[3] = 1.0;
    tFluidInput2 = new PolyFluidInputData(294.261,                  //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //mass fractions

    tNodes[2].initialize("UtTestNode2", tFluidConfig, tFluidInput2);
    tNodes[2].resetFlows();

    fractions[0] = 0.0;
    fractions[1] = 1.0;
    fractions[2] = 0.0;
    fractions[3] = 0.0;
    tFluidInput3 = new PolyFluidInputData(294.261,                  //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //mass fractions

    tNodes[3].initialize("UtTestNode3", tFluidConfig, tFluidInput3);
    tNodes[3].resetFlows();

    tNodes[4].initialize("UtTestNode4", tFluidConfig, 0); // Ground node
    tNodes[4].resetFlows();

    /// - Define nominal configuration data
    tMaxConductance = 1E-04;
    tFluidTypes.push_back(FluidProperties::GUNNS_H2O);
    tFluidTypes.push_back(FluidProperties::GUNNS_H2);
    tFluidPorts.push_back(2);
    tFluidPorts.push_back(3);
    tTcTypes.push_back(ChemicalCompound::CO2);
    tTcTypes.push_back(ChemicalCompound::CH4);
    tTcPorts.push_back(2);
    tTcPorts.push_back(3);
    tConfigData = new GunnsFluidMultiSeparatorConfigData(tLinkName,
                                                         &tNodeList,
                                                         tMaxConductance,
                                                         &tFluidTypes,
                                                         &tFluidPorts,
                                                         &tTcTypes,
                                                         &tTcPorts);

    /// - Define nominal input data
    tMalfBlockageFlag  = false;
    tMalfBlockageValue = 0.1;
    tFluidFractions.push_back(0.5);
    tFluidFractions.push_back(1.0);
    tTcFractions.push_back(0.1);
    tTcFractions.push_back(0.2);
    tInputData = new GunnsFluidMultiSeparatorInputData(tMalfBlockageFlag,
                                                       tMalfBlockageValue,
                                                       &tFluidFractions,
                                                       &tTcFractions);
    /// - Create the nominal test article
    tArticle = new FriendlyGunnsFluidMultiSeparator;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::testConfig()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName         == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes            == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductance   == tConfigData->mMaxConductance);
    CPPUNIT_ASSERT(tFluidTypes.at(0) == tConfigData->mFluidTypes.at(0));
    CPPUNIT_ASSERT(tFluidTypes.at(1) == tConfigData->mFluidTypes.at(1));
    CPPUNIT_ASSERT(tFluidPorts.at(0) == tConfigData->mFluidPorts.at(0));
    CPPUNIT_ASSERT(tFluidPorts.at(1) == tConfigData->mFluidPorts.at(1));
    CPPUNIT_ASSERT(tTcTypes.at(0)    == tConfigData->mTcTypes.at(0));
    CPPUNIT_ASSERT(tTcTypes.at(1)    == tConfigData->mTcTypes.at(1));
    CPPUNIT_ASSERT(tTcPorts.at(0)    == tConfigData->mTcPorts.at(0));
    CPPUNIT_ASSERT(tTcPorts.at(1)    == tConfigData->mTcPorts.at(1));

    /// - Check default config construction
    GunnsFluidMultiSeparatorConfigData defaultConfig;
    CPPUNIT_ASSERT(""                == defaultConfig.mName);
    CPPUNIT_ASSERT(0                 == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0                 == defaultConfig.mMaxConductance);
    CPPUNIT_ASSERT(0                 == defaultConfig.mFluidTypes.size());
    CPPUNIT_ASSERT(0                 == defaultConfig.mFluidPorts.size());
    CPPUNIT_ASSERT(0                 == defaultConfig.mTcTypes.size());
    CPPUNIT_ASSERT(0                 == defaultConfig.mTcPorts.size());

    /// - Check copy config construction
    GunnsFluidMultiSeparatorConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName         == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes            == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductance   == copyConfig.mMaxConductance);
    CPPUNIT_ASSERT(tFluidTypes.at(0) == copyConfig.mFluidTypes.at(0));
    CPPUNIT_ASSERT(tFluidTypes.at(1) == copyConfig.mFluidTypes.at(1));
    CPPUNIT_ASSERT(tFluidPorts.at(0) == copyConfig.mFluidPorts.at(0));
    CPPUNIT_ASSERT(tFluidPorts.at(1) == copyConfig.mFluidPorts.at(1));
    CPPUNIT_ASSERT(tTcTypes.at(0)    == copyConfig.mTcTypes.at(0));
    CPPUNIT_ASSERT(tTcTypes.at(1)    == copyConfig.mTcTypes.at(1));
    CPPUNIT_ASSERT(tTcPorts.at(0)    == copyConfig.mTcPorts.at(0));
    CPPUNIT_ASSERT(tTcPorts.at(1)    == copyConfig.mTcPorts.at(1));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::testInput()
{
    UT_RESULT;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tMalfBlockageFlag     == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue    == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tFluidFractions.at(0) == tInputData->mFluidFractions.at(0));
    CPPUNIT_ASSERT(tFluidFractions.at(1) == tInputData->mFluidFractions.at(1));
    CPPUNIT_ASSERT(tTcFractions.at(0)    == tInputData->mTcFractions.at(0));
    CPPUNIT_ASSERT(tTcFractions.at(1)    == tInputData->mTcFractions.at(1));

    /// - Check default config construction
    GunnsFluidMultiSeparatorInputData defaultInput;
    CPPUNIT_ASSERT(false                 == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                   == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0                     == defaultInput.mFluidFractions.size());
    CPPUNIT_ASSERT(0                     == defaultInput.mTcFractions.size());

    /// - Check copy config construction
    GunnsFluidMultiSeparatorInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag     == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue    == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tFluidFractions.at(0) == copyInput.mFluidFractions.at(0));
    CPPUNIT_ASSERT(tFluidFractions.at(1) == copyInput.mFluidFractions.at(1));
    CPPUNIT_ASSERT(tTcFractions.at(0)    == copyInput.mTcFractions.at(0));
    CPPUNIT_ASSERT(tTcFractions.at(1)    == copyInput.mTcFractions.at(1));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::testDefaultConstruction()
{
    UT_RESULT;

    /// @test proper default construction of class member data
    CPPUNIT_ASSERT(0.0 == tArticle->mMaxConductance);
    CPPUNIT_ASSERT(0   == tArticle->mNumSepTypes);
    CPPUNIT_ASSERT(0   == tArticle->mNumTcTypes);
    CPPUNIT_ASSERT(0   == tArticle->mSepIndex);
    CPPUNIT_ASSERT(0   == tArticle->mTcIndex);
    CPPUNIT_ASSERT(0   == tArticle->mSepPort);
    CPPUNIT_ASSERT(0   == tArticle->mTcPort);
    CPPUNIT_ASSERT(0   == tArticle->mSepFraction);
    CPPUNIT_ASSERT(0   == tArticle->mTcFraction);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductance);
    CPPUNIT_ASSERT(0.0 == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0   == tArticle->mSepBufferThru);
    CPPUNIT_ASSERT(0   == tArticle->mSepBufferExit);
    CPPUNIT_ASSERT(0   == tArticle->mSepFluid);
    CPPUNIT_ASSERT(0   == tArticle->mWorkTcMassFlowRates);
    CPPUNIT_ASSERT(0   == tArticle->mWorkMoleFractions);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidMultiSeparator* article = new GunnsFluidMultiSeparator();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::testNominalInitialization()
{
    UT_RESULT;

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidMultiSeparator article;
    const int array[] = {tPort0, tPort1, tPort2, tPort3};
    std::vector<int> ports (array, array + sizeof(array) / sizeof(int));
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, &ports));

    /// - Verify the parent method is called
    CPPUNIT_ASSERT(tLinkName          == article.getName());
    CPPUNIT_ASSERT(tMalfBlockageValue == article.mMalfBlockageValue);

    /// - Verify member variables are properly set
    CPPUNIT_ASSERT(tMaxConductance    == article.mMaxConductance);
    CPPUNIT_ASSERT(2                  == article.mNumSepTypes);
    CPPUNIT_ASSERT(2                  == article.mNumTcTypes);
    CPPUNIT_ASSERT(                      article.mSepIndex);
    CPPUNIT_ASSERT(2                  == article.mSepIndex[0]); //GUNNS_H2O
    CPPUNIT_ASSERT(3                  == article.mSepIndex[1]); //GUNNS_H2
    CPPUNIT_ASSERT(                      article.mTcIndex);
    CPPUNIT_ASSERT(0                  == article.mTcIndex[0]);  //CO2
    CPPUNIT_ASSERT(1                  == article.mTcIndex[1]);  //CH4
    CPPUNIT_ASSERT(                      article.mSepPort);
    CPPUNIT_ASSERT(2                  == article.mSepPort[0]);  //GUNNS_H2O
    CPPUNIT_ASSERT(3                  == article.mSepPort[1]);  //GUNNS_H2
    CPPUNIT_ASSERT(                      article.mTcPort);
    CPPUNIT_ASSERT(2                  == article.mTcPort[0]);   //CO2
    CPPUNIT_ASSERT(3                  == article.mTcPort[1]);   //CH4
    CPPUNIT_ASSERT(                      article.mSepFraction);
    CPPUNIT_ASSERT(tFluidFractions[0] == article.mSepFraction[0]);
    CPPUNIT_ASSERT(tFluidFractions[1] == article.mSepFraction[1]);
    CPPUNIT_ASSERT(                      article.mTcFraction);
    CPPUNIT_ASSERT(tTcFractions[0]    == article.mTcFraction[0]);
    CPPUNIT_ASSERT(tTcFractions[1]    == article.mTcFraction[1]);
    CPPUNIT_ASSERT(0.0                == article.mEffectiveConductance);
    CPPUNIT_ASSERT(0.0                == article.mSystemConductance);
    CPPUNIT_ASSERT(                      article.mSepBufferThru);
    CPPUNIT_ASSERT(0.0                == article.mSepBufferThru[0]);
    CPPUNIT_ASSERT(0.0                == article.mSepBufferThru[1]);
    CPPUNIT_ASSERT(                      article.mSepBufferExit);
    CPPUNIT_ASSERT(0.0                == article.mSepBufferExit[0]);
    CPPUNIT_ASSERT(0.0                == article.mSepBufferExit[1]);
    CPPUNIT_ASSERT(                      article.mInternalFluid);
    CPPUNIT_ASSERT(                      article.mSepFluid);
    CPPUNIT_ASSERT(1.0                == article.mSepFluid[0].getMassFraction(FluidProperties::GUNNS_H2O));
    CPPUNIT_ASSERT(1.0                == article.mSepFluid[1].getMassFraction(FluidProperties::GUNNS_H2));
    CPPUNIT_ASSERT(                      article.mWorkTcMassFlowRates);
    CPPUNIT_ASSERT(0.0                == article.mWorkTcMassFlowRates[0]);
    CPPUNIT_ASSERT(0.0                == article.mWorkTcMassFlowRates[1]);
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
void UtGunnsFluidMultiSeparator::testInitializationExceptions()
{
    UT_RESULT;

    /// @test for exception on # ports < 3.
    const int array[] = {tPort0, tPort1};
    std::vector<int> ports (array, array + sizeof(array) / sizeof(int));
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    ports.push_back(tPort2);
    ports.push_back(tPort3);

    /// @test for exception on max conductivity < 0.0.
    tConfigData->mMaxConductance = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mMaxConductance = tMaxConductance;

    /// @test for exception on empty separation and TC types vectors.
    tConfigData->mFluidTypes.clear();
    tConfigData->mTcTypes.clear();
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mFluidTypes = tFluidTypes;
    tConfigData->mTcTypes    = tTcTypes;

    /// @test for exception on size mismatch between types and port assignments.
    tConfigData->mFluidPorts.clear();
    tConfigData->mFluidPorts.push_back(2);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mFluidPorts = tFluidPorts;

    /// @test for exception on size mismatch between types and fractions.
    tInputData->mFluidFractions.clear();
    tInputData->mFluidFractions.push_back(1.0);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tInputData->mFluidFractions = tFluidFractions;

    /// @test for exception on duplicated separation types.
    tConfigData->mFluidTypes.clear();
    tConfigData->mFluidTypes.push_back(FluidProperties::GUNNS_H2O);
    tConfigData->mFluidTypes.push_back(FluidProperties::GUNNS_H2O);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mFluidTypes = tFluidTypes;

    /// @test for exceptions on separation port assignments out of bounds.
    tConfigData->mFluidPorts.clear();
    tConfigData->mFluidPorts.push_back(1);
    tConfigData->mFluidPorts.push_back(4);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mFluidPorts.at(0) = 2;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mFluidPorts = tFluidPorts;

    /// @test for exception on a separation fluid type not in the network.
    tConfigData->mFluidTypes.at(0) = FluidProperties::GUNNS_CO2;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mFluidTypes.at(0) = FluidProperties::GUNNS_H2O;

    /// @test for exception on a separation mass fraction < 0.
    tInputData->mFluidFractions.at(0) = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);

    /// @test for exception on a separation mass fraction > 1.
    tInputData->mFluidFractions.at(0) = 1.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tInputData->mFluidFractions.at(0) = 0.5;

    /// @test for exception on separation fluid type is not gas.
    tConfigData->mFluidTypes.at(0) = FluidProperties::GUNNS_WATER;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mFluidTypes.at(0) = FluidProperties::GUNNS_H2O;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests initialization errors associated with trace compounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::testInitializationExceptionsTc()
{
    UT_RESULT;

    const int array[] = {tPort0, tPort1, tPort2, tPort3};
    std::vector<int> ports (array, array + sizeof(array) / sizeof(int));

    /// @test for exception on size mismatch between TC types and port assignments.
    tConfigData->mTcPorts.clear();
    tConfigData->mTcPorts.push_back(2);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mTcPorts = tTcPorts;

    /// @test for exception on size mismatch between TC types and fractions.
    tInputData->mTcFractions.clear();
    tInputData->mTcFractions.push_back(1.0);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tInputData->mTcFractions = tTcFractions;

    /// @test for exception on duplicated TC types.
    tConfigData->mTcTypes.clear();
    tConfigData->mTcTypes.push_back(ChemicalCompound::CO2);
    tConfigData->mTcTypes.push_back(ChemicalCompound::CO2);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mTcTypes = tTcTypes;

    /// @test for exceptions on TC port assignments out of bounds.
    tConfigData->mTcPorts.clear();
    tConfigData->mTcPorts.push_back(1);
    tConfigData->mTcPorts.push_back(4);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mTcPorts.at(0) = 2;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mTcPorts = tTcPorts;

    /// @test for exception on a TC type not in the network.
    tConfigData->mTcTypes.at(0) = ChemicalCompound::NH3;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mTcTypes.at(0) = tTcTypes.at(0);

    /// @test for exception on a TC mass fraction < 0.
    tInputData->mTcFractions.at(0) = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);

    /// @test for exception on a TC mass fraction > 1.
    tInputData->mTcFractions.at(0) = 1.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tInputData->mTcFractions.at(0) = 0.5;

    /// @test for exception on no TC's in the network.
    tFluidConfig->mTraceCompounds = 0;
    GunnsFluidNode nodes[5];
    double massfractions[4] = {1.0, 0.0, 0.0, 0.0};
    PolyFluidInputData fluidInput(294.261,                  //temperature
                                  101.325,                  //pressure
                                  0.0,                      //flowRate
                                  0.0,                      //mass
                                  massfractions);           //massFractions
    nodes[0].initialize("UtTestNode0", tFluidConfig, &fluidInput);
    nodes[1].initialize("UtTestNode1", tFluidConfig, &fluidInput);
    nodes[2].initialize("UtTestNode2", tFluidConfig, &fluidInput);
    nodes[3].initialize("UtTestNode3", tFluidConfig, &fluidInput);
    nodes[4].initialize("UtTestNode4", tFluidConfig, 0);
    tConfigData->mNodeList->mNodes = nodes;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);

    /// @test getTcFraction when there are no TC's in the network.  This isn't in the validate
    ///       functions, but tested here for convenience.
    tConfigData->mTcTypes.clear();
    tConfigData->mTcPorts.clear();
    tInputData->mTcFractions.clear();
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports));
    CPPUNIT_ASSERT_THROW(tArticle->getTcFraction(ChemicalCompound::CO2), TsOutOfBoundsException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    const int array[] = {tPort0, tPort1, tPort2, tPort3};
    std::vector<int> ports (array, array + sizeof(array) / sizeof(int));
    tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports);

    /// - Set some stuff in the base class that should be reset on restart.
    tArticle->mPower               = 1.0;

    /// @test restart resets terms
    tArticle->restart();

    CPPUNIT_ASSERT(0.0 == tArticle->mPower);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    const int array[] = {tPort0, tPort1, tPort2, tPort3};
    std::vector<int> ports (array, array + sizeof(array) / sizeof(int));
    tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports);

    /// - Set up last-pass potentials.
    double p0 = 101.325; // inlet
    double p1 = 0.0;     // exit to Ground
    double p2 = 101.325; // H2O separation exit
    double p3 = 101.325; // H2 separation exit
    tNodes[tPort0].setPotential(p0);
    tNodes[tPort1].setPotential(p1);
    tNodes[tPort2].setPotential(p2);
    tNodes[tPort3].setPotential(p3);
    tNodes[tPort0].updateMass();
    tNodes[tPort1].updateMass();
    tNodes[tPort2].updateMass();
    tNodes[tPort3].updateMass();
    tArticle->mPotentialVector[0] = p0;
    tArticle->mPotentialVector[1] = p1;
    tArticle->mPotentialVector[2] = p2;
    tArticle->mPotentialVector[3] = p3;
    tArticle->setMalfBlockage(true, 0.1);
    const double expectedG = tMaxConductance * 0.9;
    //printf("\n%e %e\n", tNodes[tPort0].getContent()->getDensity(), tNodes[tPort0].getContent()->getMWeight());
    const double expectedRhoIn =  1.13976; // from print statement above
    const double expectedMwIn  = 27.521;   // from print statement above
    const double expectedA     = expectedG * sqrt(1000.0 * 0.5 * expectedRhoIn / (p0 - p1))
                               / expectedMwIn;
    const double flux = expectedA * (p0 - p1);
    const double mdot = flux * expectedMwIn;
    const double sepH2O = flux * 0.009 * tFluidFractions[0] / 18.0153; // mass fraction and MW of H2O;
    const double sepH2  = flux * 0.001 * tFluidFractions[1] / 2.01588; // mass fraction and MW of H2;
    tArticle->mSepBufferThru[0] = sepH2O;
    tArticle->mSepBufferThru[1] = sepH2;
    const double expectedW0 = 0.0;
    const double expectedW1 = -tArticle->mSepBufferThru[0] - tArticle->mSepBufferThru[1];
    const double expectedW2 = tArticle->mSepBufferThru[0];
    const double expectedW3 = tArticle->mSepBufferThru[1];

    tArticle->step(tTimeStep);

    /// @test Conductance and admittance matrix.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mEffectiveConductance,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mSystemConductance,     FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0],   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[1],   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[2],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[3],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[4],   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[5],   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[6],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[7],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[8],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[9],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[10],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[11],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[12],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[13],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[14],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[15],  0.0);

    /// @test Source vector.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW0, tArticle->mSourceVector[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW1, tArticle->mSourceVector[1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW2, tArticle->mSourceVector[2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW3, tArticle->mSourceVector[3],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sepH2O,     tArticle->mSepBufferExit[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sepH2,      tArticle->mSepBufferExit[1], DBL_EPSILON);

    /// @test User port map controls to swap ports 0 & 1 to set up reverse flow.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::GROUND;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(4                     == tArticle->mNodeMap[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::READY == tArticle->mUserPortSetControl);

    // @test Reverse through-flow direction.
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSelectNode = 0;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;
    tArticle->mPotentialVector[0] = p1;
    tArticle->mPotentialVector[1] = p0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0                     == tArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::READY == tArticle->mUserPortSetControl);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mEffectiveConductance,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mSystemConductance,     FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0],   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[1],   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[2],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[3],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[4],   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[5],   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[6],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[7],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[8],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[9],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[10],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[11],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[12],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[13],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[14],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[15],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW1, tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW0, tArticle->mSourceVector[1],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW2, tArticle->mSourceVector[2],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW3, tArticle->mSourceVector[3],       DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with liquid flow-thru.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    const int array[] = {tPort0, tPort1, tPort2, tPort3};
    std::vector<int> ports (array, array + sizeof(array) / sizeof(int));
    tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports);

    /// - Set up potential vector and system of equations for forward flow.
    double p0 = 101.325; // inlet
    double p1 = 0.0;     // exit to Ground
    double p2 = 101.325; // H2O separation exit
    double p3 = 101.325; // H2 separation exit
    tNodes[tPort0].setPotential(p0);
    tNodes[tPort1].setPotential(p1);
    tNodes[tPort2].setPotential(p2);
    tNodes[tPort3].setPotential(p3);
    tNodes[tPort0].updateMass();
    tNodes[tPort1].updateMass();
    tNodes[tPort2].updateMass();
    tNodes[tPort3].updateMass();
    tArticle->mPotentialVector[0] = p0;
    tArticle->mPotentialVector[1] = p1;
    tArticle->mPotentialVector[2] = p2;
    tArticle->mPotentialVector[3] = p3;
    tArticle->setMalfBlockage(true, 0.1);
    const double expectedG = tMaxConductance * 0.9;
    //printf("\n%e %e\n", tNodes[tPort0].getContent()->getDensity(), tNodes[tPort0].getContent()->getMWeight());
    const double expectedRhoIn =  1.13976; // from print statement above
    const double expectedMwIn  = 27.521;   // from print statement above
    const double expectedA     = expectedG * sqrt(1000.0 * 0.5 * expectedRhoIn / (p0 - p1))
                               / expectedMwIn;
    tArticle->mAdmittanceMatrix[0] =  expectedA;
    tArticle->mAdmittanceMatrix[1] = -expectedA;
    tArticle->mAdmittanceMatrix[4] = -expectedA;
    tArticle->mAdmittanceMatrix[5] =  expectedA;

    const double expectedDp     = p0 - p1;
    const double expectedFlux   = expectedA * expectedDp;
    const double expectedMdot   = expectedFlux * expectedMwIn;
    const double expectedQ      = expectedMdot / expectedRhoIn;
    const double expectedPwr    = -expectedQ * expectedDp * 1000.0;
    const double xH2O           = tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_H2O);
    const double xH2            = tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_H2);
    const double expectedSepH2O = expectedFlux * xH2O * tFluidFractions[0];
    const double expectedSepH2  = expectedFlux * xH2  * tFluidFractions[1];
    tArticle->mSepBufferExit[0] = expectedSepH2O;
    tArticle->mSepBufferExit[1] = expectedSepH2;

    /// @test forward bulk flow.
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp,     tArticle->mPotentialDrop,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,   tArticle->mFlux,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot,   tArticle->mFlowRate,             FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,      tArticle->mVolFlowRate,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPwr,    tArticle->mPower,                0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSepH2O, tArticle->mSepBufferThru[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSepH2,  tArticle->mSepBufferThru[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,   tNodes[0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[2]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[3]);

    tNodes[4].setPotential(p0);
    tNodes[4].updateMass();
    tArticle->mPotentialVector[1] = p0;
    tNodes[0].resetFlows();

    /// @test zero bulk flow.
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPotentialDrop,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlux,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlowRate,             FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mVolFlowRate,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPower,                0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSepBufferThru[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSepBufferThru[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[4].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE   == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE   == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[2]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[3]);

    /// - Swap ports 0 & 1 nodes and set up reverse bulk flow direction.
    /// @test reverse bulk flow.
    CPPUNIT_ASSERT(tArticle->setPort(0, 4));
    CPPUNIT_ASSERT(tArticle->setPort(1, 0));
    tArticle->mPotentialVector[0] = p1;
    tArticle->mPotentialVector[1] = p0;
    tArticle->mSepBufferExit[1]   = 0.0;
    tArticle->setSeparationFraction(FluidProperties::GUNNS_H2, 0.0);
    tNodes[0].resetFlows();

    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedDp,    tArticle->mPotentialDrop,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFlux,  tArticle->mFlux,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedMdot,  tArticle->mFlowRate,             FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedQ,     tArticle->mVolFlowRate,          FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPwr,    tArticle->mPower,                0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSepH2O, tArticle->mSepBufferThru[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mSepBufferThru[1],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,   tNodes[0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[2]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE   == tArticle->mPortDirections[3]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for flow transport between nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::testTransportFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    const int array[] = {tPort0, tPort1, tPort2, tPort3};
    std::vector<int> ports (array, array + sizeof(array) / sizeof(int));
    tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports);

    double p0 = 101.325; // inlet
    double p1 = 0.0;     // exit to Ground
    double p2 = 101.325; // H2O separation exit
    double p3 = 101.325; // H2 separation exit
    tNodes[tPort0].setPotential(p0);
    tNodes[tPort1].setPotential(p1);
    tNodes[tPort2].setPotential(p2);
    tNodes[tPort3].setPotential(p3);
    tNodes[tPort0].updateMass();
    tNodes[tPort1].updateMass();
    tNodes[tPort2].updateMass();
    tNodes[tPort3].updateMass();
    tArticle->mPotentialVector[0] = p0;
    tArticle->mPotentialVector[1] = p1;
    tArticle->mPotentialVector[2] = p2;
    tArticle->mPotentialVector[3] = p3;
    tArticle->setMalfBlockage(true, 0.1);
    const double expectedG = tMaxConductance * 0.9;
    const double expectedRhoIn =  1.13976; // from testComputeFlows()
    const double expectedMwIn  = 27.521;   // from testComputeFlows()
    const double expectedDp    = p0 - p1;
    const double expectedA     = expectedG * sqrt(1000.0 * 0.5 * expectedRhoIn / expectedDp)
                               / expectedMwIn;
    tArticle->mAdmittanceMatrix[0] =  expectedA;
    tArticle->mAdmittanceMatrix[1] = -expectedA;
    tArticle->mAdmittanceMatrix[4] = -expectedA;
    tArticle->mAdmittanceMatrix[5] =  expectedA;
    const double expectedFlux   = expectedA * expectedDp;
    const double expectedMdot   = expectedFlux * expectedMwIn;
    const double expectedQ      = expectedMdot / expectedRhoIn;
    const double expectedPwr    = -expectedQ * expectedDp * 1000.0;
    const double xH2O           = tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_H2O);
    const double xH2            = tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_H2);
    const double xCO2           = tNodes[0].getContent()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO2);
    const double xCH4           = tNodes[0].getContent()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CH4);
    const double expectedSepH2O = expectedFlux * xH2O * tFluidFractions[0];
    const double expectedSepH2  = expectedFlux * xH2  * tFluidFractions[1];
    const double expectedSepCO2 = expectedFlux * xCO2 * tTcFractions[0];
    const double expectedSepCH4 = expectedFlux * xCH4 * tTcFractions[1];
    tArticle->mSepBufferExit[0] = expectedSepH2O;
    tArticle->mSepBufferExit[1] = expectedSepH2;
    const double expectedMdotH2O = expectedSepH2O * 18.0153; // MW of H2O
    const double expectedMdotH2  = expectedSepH2  * 2.01588; // MW of H2
    const double expectedMdotCO2 = expectedSepCO2 * 44.0095; // MW of CO2
    const double expectedMdotCH4 = expectedSepCH4 * 16.0425; // MW of CH4
    const double expectedMdotDown = expectedMdot - expectedMdotH2O - expectedMdotH2;
    const double expectedFluxDown = expectedFlux - expectedSepH2O - expectedSepH2;

    /// - Compute expected mole fraction of bulk flow into downstream node.
    double expectedX[4] = {
            tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_N2),
            tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_WATER),
            tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_H2O),
            tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_H2)
    };
    expectedX[2] *= (1.0 - tFluidFractions[0]);  // removed fluids
    expectedX[3] *= (1.0 - tFluidFractions[1]);
    /// - Normalize:
    const double sumX = expectedX[0] + expectedX[1] + expectedX[2] + expectedX[3];
    expectedX[0] /= sumX;
    expectedX[1] /= sumX;
    expectedX[2] /= sumX;
    expectedX[3] /= sumX;

    /// - Compute expected mass & mole fraction of trace compounds into downstream node.
    const double expectedMassCO2 = (expectedFlux * xCO2 - expectedSepCO2) * 44.0095;
    const double expectedMassCH4 = (expectedFlux * xCH4 - expectedSepCH4) * 16.0425;
    const double expectedMoleCO2 = expectedMassCO2 / 44.0095;
    const double expectedMoleCH4 = expectedMassCH4 / 16.0425;
    const double expectedXCO2    = expectedMoleCO2 / expectedFluxDown;
    const double expectedXCH4    = expectedMoleCH4 / expectedFluxDown;

    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    const double actualMassCO2 = tNodes[4].getInflow()->getTraceCompounds()->getMass(ChemicalCompound::CO2);
    const double actualMassCH4 = tNodes[4].getInflow()->getTraceCompounds()->getMass(ChemicalCompound::CH4);
    const double actualXCO2    = tNodes[4].getInflow()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO2);
    const double actualXCH4    = tNodes[4].getInflow()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CH4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFluxDown, tArticle->mInternalFluid->getMole(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassCO2,  actualMassCO2,                       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassCH4,  actualMassCH4,                       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedXCO2,     actualXCO2,                          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedXCH4,     actualXCH4,                          DBL_EPSILON);

    FriendlyGunnsFluidMultiSeparatorNode* node1 = static_cast<FriendlyGunnsFluidMultiSeparatorNode*>(&tNodes[1]);
    FriendlyGunnsFluidMultiSeparatorNode* node2 = static_cast<FriendlyGunnsFluidMultiSeparatorNode*>(&tNodes[2]);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdotH2O,  tNodes[1].getInflux(),                     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdotH2,   tNodes[2].getInflux(),                     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdotCO2,  node1->mTcInflow.mState[0],                DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              node1->mTcInflow.mState[1],                DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              node2->mTcInflow.mState[0],                DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdotCH4,  node2->mTcInflow.mState[1],                DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedX[0],     tNodes[4].getInflow()->getMoleFraction(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedX[1],     tNodes[4].getInflow()->getMoleFraction(1), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedX[2],     tNodes[4].getInflow()->getMoleFraction(2), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedX[3],     tNodes[4].getInflow()->getMoleFraction(3), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot,     tNodes[0].getOutflux(),                    FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdotDown, tNodes[4].getInflux(),                     FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(), tNodes[1].getInflow()->getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(), tNodes[2].getInflow()->getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(), tNodes[4].getInflow()->getTemperature(), FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test setter & getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::testAccessMethods()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.  Change it to only have
    ///   one trace compound so we can test for a trace compound that is in the network but not
    ///   separated by this link.
    const int array[] = {tPort0, tPort1, tPort2, tPort3};
    std::vector<int> ports (array, array + sizeof(array) / sizeof(int));
    tConfigData->mTcTypes.clear();
    tConfigData->mTcTypes.push_back(ChemicalCompound::CO2);
    tConfigData->mTcPorts.clear();
    tConfigData->mTcPorts.push_back(2);
    tInputData->mTcFractions.clear();
    tInputData->mTcFractions.push_back(0.5);
    tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports);

    /// @test setSeparationFraction with good fluid type, and that it is limited to (0-1).
    tArticle->setSeparationFraction(FluidProperties::GUNNS_H2, 1.1);
    CPPUNIT_ASSERT(1.0 == tArticle->mSepFraction[1]);
    tArticle->setSeparationFraction(FluidProperties::GUNNS_H2O, -0.1);
    CPPUNIT_ASSERT(0.0 == tArticle->mSepFraction[0]);

    /// @test setSeparationFraction with bad fluid type.
    CPPUNIT_ASSERT_THROW(tArticle->setSeparationFraction(FluidProperties::GUNNS_CO2, 0.5), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(tArticle->setSeparationFraction(FluidProperties::GUNNS_N2,  0.5), TsOutOfBoundsException);

    /// @test getSeparationFraction with good fluid type.
    CPPUNIT_ASSERT(1.0 == tArticle->getSeparationFraction(FluidProperties::GUNNS_H2));
    CPPUNIT_ASSERT(0.0 == tArticle->getSeparationFraction(FluidProperties::GUNNS_H2O));

    /// @test getSeparationFraction with bad fluid type.
    CPPUNIT_ASSERT_THROW(tArticle->getSeparationFraction(FluidProperties::GUNNS_CO2), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(tArticle->getSeparationFraction(FluidProperties::GUNNS_N2),  TsOutOfBoundsException);

    /// @test setTcFraction with good type, and that it is limited to (0-1).
    tArticle->setTcFraction(ChemicalCompound::CO2, -0.1);
    CPPUNIT_ASSERT(0.0 == tArticle->mTcFraction[0]);
    tArticle->setTcFraction(ChemicalCompound::CO2, 1.1);
    CPPUNIT_ASSERT(1.0 == tArticle->mTcFraction[0]);

    /// @test setTcFraction with bad TC type (not in network).
    CPPUNIT_ASSERT_THROW(tArticle->setTcFraction(ChemicalCompound::NH3, 0.5), TsOutOfBoundsException);

    /// @test getTcFraction with good type.
    CPPUNIT_ASSERT(1.0 == tArticle->getTcFraction(ChemicalCompound::CO2));

    /// @test getTcFraction with bad TC type (not separated by this link).
    CPPUNIT_ASSERT_THROW(tArticle->getTcFraction(ChemicalCompound::CH4), TsOutOfBoundsException);

    UT_PASS_LAST;
}
