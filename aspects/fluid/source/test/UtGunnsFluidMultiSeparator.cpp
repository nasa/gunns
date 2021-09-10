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
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::setUp()
{
    tLinkName           = "Test Fluid Selective Membrane";
    tNodeList.mNumNodes = 5;
    tNodeList.mNodes    = tNodes;
    tPort0              = 0;
    tPort1              = 4; // Ground
    tPort2              = 1;
    tPort3              = 2;
    tTimeStep           = 0.1;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[4];
    types[0]     = FluidProperties::GUNNS_N2;
    types[1]     = FluidProperties::GUNNS_WATER;
    types[2]     = FluidProperties::GUNNS_H2O;
    types[3]     = FluidProperties::GUNNS_H2;
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 4);

    fractions = new double[4];
    fractions[0] = 0.99;
    fractions[1] = 0.0;
    fractions[2] = 0.009;
    fractions[3] = 0.001;
    tFluidInput0 = new PolyFluidInputData(294.261,                  //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions

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
                                          fractions);               //massFractions

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
                                          fractions);               //massFractions

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
                                          fractions);               //massFractions

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
    CPPUNIT_ASSERT(0   == tArticle->mSepIndex);
    CPPUNIT_ASSERT(0   == tArticle->mSepFraction);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductance);
    CPPUNIT_ASSERT(0.0 == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0   == tArticle->mSepBufferThru);
    CPPUNIT_ASSERT(0   == tArticle->mSepBufferExit);
    CPPUNIT_ASSERT(0   == tArticle->mSepFluid);

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
    CPPUNIT_ASSERT(                      article.mSepIndex);
    CPPUNIT_ASSERT(2                  == article.mSepIndex[0]); //GUNNS_H2O
    CPPUNIT_ASSERT(3                  == article.mSepIndex[1]); //GUNNS_H2
    CPPUNIT_ASSERT(                      article.mSepFraction);
    CPPUNIT_ASSERT(tFluidFractions[0] == article.mSepFraction[0]);
    CPPUNIT_ASSERT(tFluidFractions[1] == article.mSepFraction[1]);
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

    /// @test for exception on max conductivity < 0.0.
    tConfigData->mMaxConductance = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mMaxConductance = tMaxConductance;

    /// @test for exception on empty separation types vector.
    std::vector<FluidProperties::FluidType> emptyTypes;
    tConfigData->mFluidTypes.clear();
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    tConfigData->mFluidTypes = tFluidTypes;

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
    const double expectedFlux   = expectedA * (p0 - p1);
    const double expectedMdot   = expectedFlux * expectedMwIn;
    const double expectedQ      = expectedMdot / expectedRhoIn;
    const double expectedPwr    = -expectedQ * expectedDp * 1000.0;
    const double xH2O           = tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_H2O);
    const double xH2            = tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_H2);
    const double expectedSepH2O = expectedFlux * xH2O * tFluidFractions[0];
    const double expectedSepH2  = expectedFlux * xH2  * tFluidFractions[1];
    tArticle->mSepBufferExit[0] = expectedSepH2O;
    tArticle->mSepBufferExit[1] = expectedSepH2;

    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp,     tArticle->mPotentialDrop,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux,   tArticle->mFlux,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot,   tArticle->mFlowRate,         FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,      tArticle->mVolFlowRate,      FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPwr,    tArticle->mPower,            0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSepH2O, tArticle->mSepBufferThru[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSepH2,  tArticle->mSepBufferThru[1], DBL_EPSILON);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[2]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[3]);
    //TODO complete checks

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

    //TODO setup inputs and check outputs
    tArticle->transportFlows(tTimeStep);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test setter & getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidMultiSeparator::testAccessMethods()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    const int array[] = {tPort0, tPort1, tPort2, tPort3};
    std::vector<int> ports (array, array + sizeof(array) / sizeof(int));
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

    UT_PASS_LAST;
}
