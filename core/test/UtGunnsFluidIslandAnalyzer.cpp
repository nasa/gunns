/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsFluidIslandAnalyzer.o))
***************************************************************************************************/

#include "UtGunnsFluidIslandAnalyzer.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidIslandAnalyzer class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidIslandAnalyzer::UtGunnsFluidIslandAnalyzer()
    :
    tArticle(tNodeList),
    tNodes(),
    tNodeList(),
    tName("test article"),
    tConfig(tName),
    tInput(),
    tFluidProperties(),
    tFluidConfig(),
    tFluidInput1(),
    tFluidInput2(),
    tTcInput1(),
    tTcInput2(),
    tTcConfig()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidIslandAnalyzer class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidIslandAnalyzer::~UtGunnsFluidIslandAnalyzer()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidIslandAnalyzer::tearDown()
{
    /// - Deletes for news in setUp
    delete tFluidInput2;
    delete tFluidInput1;
    delete tFluidConfig;
    delete tFluidProperties;
    delete tTcConfig;
    delete tTcInput1;
    delete tTcInput2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidIslandAnalyzer::setUp()
{
    tConfig.mName       = tName;
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tNodes;

    /// - Setup some fluid nodes
    double tcFractions1[2] = {1.0e-6, 2.0e-7};
    double tcFractions2[2] = {3.0e-8, 4.0e-5};
    tTcInput1 = new GunnsFluidTraceCompoundsInputData(tcFractions1);
    tTcInput2 = new GunnsFluidTraceCompoundsInputData(tcFractions2);
    tTcConfig = new GunnsFluidTraceCompoundsConfigData(0, 0, "tTcConfig");
    tTcConfig->addCompound(28.0, "CO");
    tTcConfig->addCompound(32.0, "H2O");

    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2];
    types[0] = FluidProperties::GUNNS_N2;
    types[1] = FluidProperties::GUNNS_O2;
    double fractions1[2] = {0.5, 0.5};
    double fractions2[2] = {0.8, 0.2};
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 2, tTcConfig);
    tFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions1,               //massFractions
                                          tTcInput1);               //trace compounds
    tFluidInput2 = new PolyFluidInputData(294.261,                  //temperature
                                          689.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions2,               //massFractions
                                          tTcInput2);               //trace compounds

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig);
    tNodes[1].initialize("UtTestNode1", tFluidConfig);

    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput2);

    tNodes[0].setPotential(tFluidInput1->mPressure);
    tNodes[1].setPotential(tFluidInput2->mPressure);

    tNodes[0].initVolume(1.0);
    tNodes[1].initVolume(2.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsFluidIslandAnalyzerConfigData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidIslandAnalyzer::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidIslandAnalyzer 01: testConfig ..........................";

    /// - Test nominal config data construction.
    CPPUNIT_ASSERT(tName == tConfig.mName);

    /// - Test default config data construction.
    GunnsFluidIslandAnalyzerConfigData article(tName);
    CPPUNIT_ASSERT(tName == article.mName);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsFluidIslandAnalyzerInputData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidIslandAnalyzer::testInput()
{
    std::cout << "\n UtGunnsFluidIslandAnalyzer 02: testInput ...........................";

    /// - Currently nothing to test.

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsFluidIslandAnalyzer class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidIslandAnalyzer::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidIslandAnalyzer 03: testDefaultConstruction .............";

    /// @test state data
    CPPUNIT_ASSERT(""         ==  tArticle.mName);
    CPPUNIT_ASSERT(&tNodeList == &tArticle.mNodeList);
    CPPUNIT_ASSERT(0          ==  tArticle.mAttachedNode);
    CPPUNIT_ASSERT(0          ==  tArticle.mIslandSize);
    CPPUNIT_ASSERT(0          ==  tArticle.mIslandNodes);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mIslandVolume);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mIslandMass);
    CPPUNIT_ASSERT(0          ==  tArticle.mIslandConstituentMass);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mIslandEnergy);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mHiPressure);
    CPPUNIT_ASSERT(0          ==  tArticle.mHiPressureNode);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mLoPressure);
    CPPUNIT_ASSERT(0          ==  tArticle.mLoPressureNode);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mHiTemperature);
    CPPUNIT_ASSERT(0          ==  tArticle.mHiTemperatureNode);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mLoTemperature);
    CPPUNIT_ASSERT(0          ==  tArticle.mLoTemperatureNode);
    CPPUNIT_ASSERT(0          ==  tArticle.mHiMoleFraction);
    CPPUNIT_ASSERT(0          ==  tArticle.mHiMoleFractionNode);
    CPPUNIT_ASSERT(0          ==  tArticle.mLoMoleFraction);
    CPPUNIT_ASSERT(0          ==  tArticle.mLoMoleFractionNode);
    CPPUNIT_ASSERT(0          ==  tArticle.mIslandTcMass);
    CPPUNIT_ASSERT(0          ==  tArticle.mHiTcMoleFraction);
    CPPUNIT_ASSERT(0          ==  tArticle.mHiTcMoleFractionNode);
    CPPUNIT_ASSERT(0          ==  tArticle.mLoTcMoleFraction);
    CPPUNIT_ASSERT(0          ==  tArticle.mLoTcMoleFractionNode);

    /// @test init flag
    CPPUNIT_ASSERT(false      ==  tArticle.mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidIslandAnalyzer* article = new GunnsFluidIslandAnalyzer(tNodeList);
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsFluidIslandAnalyzer class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidIslandAnalyzer::testInitialize()
{
    std::cout << "\n UtGunnsFluidIslandAnalyzer 04: testInitialize ......................";

    tArticle.initialize(&tConfig, &tInput);

    /// - Test nominal initialization.
    CPPUNIT_ASSERT(tName      ==  tArticle.mName);
    CPPUNIT_ASSERT(&tNodeList == &tArticle.mNodeList);
    CPPUNIT_ASSERT(-1         ==  tArticle.mAttachedNode);
    CPPUNIT_ASSERT(0          ==  tArticle.mIslandSize);
    CPPUNIT_ASSERT(false      ==  tArticle.mIslandNodes[0]);
    CPPUNIT_ASSERT(false      ==  tArticle.mIslandNodes[1]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mIslandConstituentMass[0]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mIslandConstituentMass[1]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mIslandEnergy);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mIslandVolume);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mIslandMass);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mHiPressure);
    CPPUNIT_ASSERT(-1         ==  tArticle.mHiPressureNode);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mLoPressure);
    CPPUNIT_ASSERT(-1         ==  tArticle.mLoPressureNode);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mHiTemperature);
    CPPUNIT_ASSERT(-1         ==  tArticle.mHiTemperatureNode);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mLoTemperature);
    CPPUNIT_ASSERT(-1         ==  tArticle.mLoTemperatureNode);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mHiMoleFraction[0]);
    CPPUNIT_ASSERT(-1         ==  tArticle.mHiMoleFractionNode[0]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mLoMoleFraction[0]);
    CPPUNIT_ASSERT(-1         ==  tArticle.mLoMoleFractionNode[0]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mHiMoleFraction[1]);
    CPPUNIT_ASSERT(-1         ==  tArticle.mHiMoleFractionNode[1]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mLoMoleFraction[1]);
    CPPUNIT_ASSERT(-1         ==  tArticle.mLoMoleFractionNode[1]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mIslandTcMass[0]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mIslandTcMass[1]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mHiTcMoleFraction[0]);
    CPPUNIT_ASSERT(-1         ==  tArticle.mHiTcMoleFractionNode[0]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mLoTcMoleFraction[0]);
    CPPUNIT_ASSERT(-1         ==  tArticle.mLoTcMoleFractionNode[0]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mHiTcMoleFraction[1]);
    CPPUNIT_ASSERT(-1         ==  tArticle.mHiTcMoleFractionNode[1]);
    CPPUNIT_ASSERT(0.0        ==  tArticle.mLoTcMoleFraction[1]);
    CPPUNIT_ASSERT(-1         ==  tArticle.mLoTcMoleFractionNode[1]);

    /// @test init flag
    CPPUNIT_ASSERT(true       ==  tArticle.mInitFlag);

    /// - Test exception thrown from missing name.
    tConfig.mName = "";
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tConfig.mName = tName;

    /// - Test exception thrown from null config data.
    const GunnsFluidIslandAnalyzerConfigData* nullConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(nullConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown from null input data.
    const GunnsFluidIslandAnalyzerInputData* nullInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, nullInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown on bad config data pointer type.
    BadGunnsFluidIslandAnalyzerConfigData* badConfig = new BadGunnsFluidIslandAnalyzerConfigData(tName);
    CPPUNIT_ASSERT_THROW(tArticle.initialize(badConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badConfig;

    /// - Test exception thrown on bad input data pointer type.
    BadGunnsFluidIslandAnalyzerInputData* badInput = new BadGunnsFluidIslandAnalyzerInputData();
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, badInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badInput;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsFluidIslandAnalyzer class.
///           That method doesn't actually do anything so there are no asserts, but this is done for
///           complete code coverage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidIslandAnalyzer::testPreSolver()
{
    std::cout << "\n UtGunnsFluidIslandAnalyzer 05: testPreSolver .......................";

    tArticle.initialize(&tConfig, &tInput);
    tArticle.stepPreSolver(0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsFluidIslandAnalyzer class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidIslandAnalyzer::testPostSolver()
{
    std::cout << "\n UtGunnsFluidIslandAnalyzer 06: testPostSolver ......................";

    tArticle.initialize(&tConfig, &tInput);

    /// - Test analysis outputs if no vectors are supplied to the nodes, which should be reset.
    tArticle.mAttachedNode = 0;
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mIslandSize);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.mIslandNodes[0]);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.mIslandNodes[1]);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.mIslandNodes[2]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandVolume);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandMass);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandConstituentMass[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandConstituentMass[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandEnergy);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiPressure);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiPressureNode);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoPressure);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoPressureNode);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiTemperature);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiTemperatureNode);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoTemperature);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoTemperatureNode);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandTcMass[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandTcMass[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiTcMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiTcMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiTcMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiTcMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoTcMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoTcMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoTcMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoTcMoleFractionNode[1]);

    /// - Build an island vector and assign nodes
    std::vector<int> island;
    island.push_back(0);
    island.push_back(1);
    tNodes[0].setIslandVector(&island);
    tNodes[1].setIslandVector(&island);

    /// - Test analysis outputs attached to node 0.
    tArticle.mAttachedNode = 0;
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(2,                                         tArticle.mIslandSize);
    CPPUNIT_ASSERT_EQUAL(true,                                      tArticle.mIslandNodes[0]);
    CPPUNIT_ASSERT_EQUAL(true,                                      tArticle.mIslandNodes[1]);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.mIslandNodes[2]);
    CPPUNIT_ASSERT_EQUAL(3.0,                                       tArticle.mIslandVolume);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getMass() + tNodes[1].getMass(), tArticle.mIslandMass);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getMass()*tNodes[0].getContent()->getMassFraction(0) +
                         tNodes[1].getMass()*tNodes[1].getContent()->getMassFraction(0),
                                                                    tArticle.mIslandConstituentMass[0]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getMass()*tNodes[0].getContent()->getMassFraction(1) +
                         tNodes[1].getMass()*tNodes[1].getContent()->getMassFraction(1),
                                                                    tArticle.mIslandConstituentMass[1]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getMass()*tNodes[0].getContent()->getSpecificEnthalpy() +
                         tNodes[1].getMass()*tNodes[1].getContent()->getSpecificEnthalpy(),
                                                                    tArticle.mIslandEnergy);
    CPPUNIT_ASSERT_EQUAL(tFluidInput1->mPressure,                   tArticle.mHiPressure);
    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mHiPressureNode);
    CPPUNIT_ASSERT_EQUAL(tFluidInput2->mPressure,                   tArticle.mLoPressure);
    CPPUNIT_ASSERT_EQUAL(1,                                         tArticle.mLoPressureNode);
    CPPUNIT_ASSERT_EQUAL(tFluidInput2->mTemperature,                tArticle.mHiTemperature);
    CPPUNIT_ASSERT_EQUAL(1,                                         tArticle.mHiTemperatureNode);
    CPPUNIT_ASSERT_EQUAL(tFluidInput1->mTemperature,                tArticle.mLoTemperature);
    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mLoTemperatureNode);
    CPPUNIT_ASSERT_EQUAL(tNodes[1].getContent()->getMoleFraction(tNodes[1].getContent()->getType(0)),
                                                                    tArticle.mHiMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getContent()->getMoleFraction(tNodes[0].getContent()->getType(1)),
                                                                    tArticle.mHiMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(1,                                         tArticle.mHiMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mHiMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getContent()->getMoleFraction(tNodes[0].getContent()->getType(0)),
                                                                    tArticle.mLoMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(tNodes[1].getContent()->getMoleFraction(tNodes[1].getContent()->getType(1)),
                                                                    tArticle.mLoMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mLoMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(1,                                         tArticle.mLoMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getContent()->getTraceCompounds()->getMasses()[0] +
                         tNodes[1].getContent()->getTraceCompounds()->getMasses()[0],
                                                                    tArticle.mIslandTcMass[0]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getContent()->getTraceCompounds()->getMasses()[1] +
                         tNodes[1].getContent()->getTraceCompounds()->getMasses()[1],
                                                                    tArticle.mIslandTcMass[1]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getContent()->getTraceCompounds()->getMoleFractions()[0],
                                                                    tArticle.mHiTcMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(tNodes[1].getContent()->getTraceCompounds()->getMoleFractions()[1],
                                                                    tArticle.mHiTcMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mHiTcMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(1,                                         tArticle.mHiTcMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(tNodes[1].getContent()->getTraceCompounds()->getMoleFractions()[0],
                                                                    tArticle.mLoTcMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getContent()->getTraceCompounds()->getMoleFractions()[1],
                                                                    tArticle.mLoTcMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(1,                                         tArticle.mLoTcMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mLoTcMoleFractionNode[1]);

    /// - Test analysis outputs attached to node -1, which should be reset because it is invalid.
    tArticle.mAttachedNode = -1;
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mIslandSize);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.mIslandNodes[0]);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.mIslandNodes[1]);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.mIslandNodes[2]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandVolume);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandMass);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandConstituentMass[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandConstituentMass[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandEnergy);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiPressure);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiPressureNode);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoPressure);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoPressureNode);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiTemperature);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiTemperatureNode);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoTemperature);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoTemperatureNode);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandTcMass[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandTcMass[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiTcMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiTcMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiTcMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiTcMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoTcMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoTcMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoTcMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoTcMoleFractionNode[1]);

    /// - Test analysis outputs attached to node 1, which should be identical to node 0.
    tArticle.mAttachedNode = 1;
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(2,                                         tArticle.mIslandSize);
    CPPUNIT_ASSERT_EQUAL(true,                                      tArticle.mIslandNodes[0]);
    CPPUNIT_ASSERT_EQUAL(true,                                      tArticle.mIslandNodes[1]);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.mIslandNodes[2]);
    CPPUNIT_ASSERT_EQUAL(3.0,                                       tArticle.mIslandVolume);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getMass() + tNodes[1].getMass(), tArticle.mIslandMass);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getMass()*tNodes[0].getContent()->getMassFraction(0) +
                         tNodes[1].getMass()*tNodes[1].getContent()->getMassFraction(0),
                                                                    tArticle.mIslandConstituentMass[0]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getMass()*tNodes[0].getContent()->getMassFraction(1) +
                         tNodes[1].getMass()*tNodes[1].getContent()->getMassFraction(1),
                                                                    tArticle.mIslandConstituentMass[1]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getMass()*tNodes[0].getContent()->getSpecificEnthalpy() +
                         tNodes[1].getMass()*tNodes[1].getContent()->getSpecificEnthalpy(),
                                                                    tArticle.mIslandEnergy);
    CPPUNIT_ASSERT_EQUAL(tFluidInput1->mPressure,                   tArticle.mHiPressure);
    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mHiPressureNode);
    CPPUNIT_ASSERT_EQUAL(tFluidInput2->mPressure,                   tArticle.mLoPressure);
    CPPUNIT_ASSERT_EQUAL(1,                                         tArticle.mLoPressureNode);
    CPPUNIT_ASSERT_EQUAL(tFluidInput2->mTemperature,                tArticle.mHiTemperature);
    CPPUNIT_ASSERT_EQUAL(1,                                         tArticle.mHiTemperatureNode);
    CPPUNIT_ASSERT_EQUAL(tFluidInput1->mTemperature,                tArticle.mLoTemperature);
    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mLoTemperatureNode);
    CPPUNIT_ASSERT_EQUAL(tNodes[1].getContent()->getMoleFraction(tNodes[1].getContent()->getType(0)),
                                                                    tArticle.mHiMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getContent()->getMoleFraction(tNodes[0].getContent()->getType(1)),
                                                                    tArticle.mHiMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(1,                                         tArticle.mHiMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mHiMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getContent()->getMoleFraction(tNodes[0].getContent()->getType(0)),
                                                                    tArticle.mLoMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(tNodes[1].getContent()->getMoleFraction(tNodes[1].getContent()->getType(1)),
                                                                    tArticle.mLoMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mLoMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(1,                                         tArticle.mLoMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getContent()->getTraceCompounds()->getMasses()[0] +
                         tNodes[1].getContent()->getTraceCompounds()->getMasses()[0],
                                                                    tArticle.mIslandTcMass[0]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getContent()->getTraceCompounds()->getMasses()[1] +
                         tNodes[1].getContent()->getTraceCompounds()->getMasses()[1],
                                                                    tArticle.mIslandTcMass[1]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getContent()->getTraceCompounds()->getMoleFractions()[0],
                                                                    tArticle.mHiTcMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(tNodes[1].getContent()->getTraceCompounds()->getMoleFractions()[1],
                                                                    tArticle.mHiTcMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mHiTcMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(1,                                         tArticle.mHiTcMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(tNodes[1].getContent()->getTraceCompounds()->getMoleFractions()[0],
                                                                    tArticle.mLoTcMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getContent()->getTraceCompounds()->getMoleFractions()[1],
                                                                    tArticle.mLoTcMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(1,                                         tArticle.mLoTcMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mLoTcMoleFractionNode[1]);

    /// - Test analysis outputs attached to node 2, which should be reset because it is ground.
    tArticle.mAttachedNode = 2;
    tArticle.stepPostSolver(0.0);

    CPPUNIT_ASSERT_EQUAL(0,                                         tArticle.mIslandSize);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.mIslandNodes[0]);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.mIslandNodes[1]);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.mIslandNodes[2]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandVolume);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandMass);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandConstituentMass[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandConstituentMass[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandEnergy);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiPressure);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiPressureNode);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoPressure);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoPressureNode);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiTemperature);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiTemperatureNode);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoTemperature);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoTemperatureNode);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandTcMass[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mIslandTcMass[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiTcMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mHiTcMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiTcMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mHiTcMoleFractionNode[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoTcMoleFraction[0]);
    CPPUNIT_ASSERT_EQUAL(0.0,                                       tArticle.mLoTcMoleFraction[1]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoTcMoleFractionNode[0]);
    CPPUNIT_ASSERT_EQUAL(-1,                                        tArticle.mLoTcMoleFractionNode[1]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the setter & getter methods of the GunnsFluidIslandAnalyzer class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidIslandAnalyzer::testAccessors()
{
    std::cout << "\n UtGunnsFluidIslandAnalyzer 07: testAccessors .......................";

    tArticle.initialize(&tConfig, &tInput);

    /// - Build an island vector and assign nodes
    std::vector<int> island;
    island.push_back(0);
    island.push_back(1);
    tNodes[0].setIslandVector(&island);
    tNodes[1].setIslandVector(&island);

    /// - Test the setter & getter for attached node.
    tArticle.setAttachedNode(1);
    CPPUNIT_ASSERT_EQUAL(1, tArticle.mAttachedNode);
    CPPUNIT_ASSERT_EQUAL(1, tArticle.getAttachedNode());

    /// - Test analysis outputs attached to node 1.
    tArticle.stepPostSolver(0.0);

    /// - Test the getter for island size.
    CPPUNIT_ASSERT_EQUAL(2, tArticle.getIslandSize());

    /// - Test the getter for the island nodes array.
    CPPUNIT_ASSERT_EQUAL(true,                                      tArticle.getIslandNodes()[0]);
    CPPUNIT_ASSERT_EQUAL(true,                                      tArticle.getIslandNodes()[1]);
    CPPUNIT_ASSERT_EQUAL(false,                                     tArticle.getIslandNodes()[2]);

    /// - Test the getter for the island volume.
    CPPUNIT_ASSERT_EQUAL(3.0,                                       tArticle.getIslandVolume());

    /// - Test the getter for the island mass.
    CPPUNIT_ASSERT_EQUAL(tNodes[0].getMass() + tNodes[1].getMass(), tArticle.getIslandMass());

    std::cout << "... Pass";
}
