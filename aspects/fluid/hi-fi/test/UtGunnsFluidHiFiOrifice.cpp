/*
@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/
#include "UtGunnsFluidHiFiOrifice.hh"

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsFluidConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidHiFiOrifice class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHiFiOrifice::UtGunnsFluidHiFiOrifice()
    :
    tLinkName(),
    tCoefficientType(),
    tCoefficientValue(),
    tThroatDiameter(),
    tCriticalReynolds(),
    tExpansionScaleFactor(),
    tFlowTuningFactor(),
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
    tFluidInput1(),
    tFluidInput2(),
    tFluidInput3(),
    fractions()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidHiFiOrifice class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHiFiOrifice::~UtGunnsFluidHiFiOrifice()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput3;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tFluidConfig;
    delete [] fractions;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::setUp()
{
    tLinkName             = "Test Fluid Hi-Fi Orifice";
    tNodeList.mNumNodes   = 5;
    tNodeList.mNodes      = tNodes;
    tPort0                = 0;
    tPort1                = 1;
    tTimeStep             = 0.1;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[3];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    types[2]    = FluidProperties::GUNNS_WATER;
    fractions = new double[3];
    fractions[0] = 0.5;
    fractions[1] = 0.5;
    fractions[2] = 0.0;
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 3);
    tFluidInput1 = new PolyFluidInputData(290.0,                    //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions
    tFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          689.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions
    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig, tFluidInput1);
    tNodes[1].initialize("UtTestNode1", tFluidConfig, tFluidInput2);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 1.0;
    tFluidInput3 = new PolyFluidInputData(283.15,                   //temperature
                                          689.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions

    tNodes[2].initialize("UtTestNode2", tFluidConfig, tFluidInput3);
    tNodes[3].initialize("UtTestNode3", tFluidConfig, tFluidInput3);
    tNodes[4].initialize("UtTestNode4", tFluidConfig, 0);

    tNodes[2].resetFlows();
    tNodes[3].resetFlows();
    tNodes[4].resetContentState();
    tNodes[4].resetFlows();

    /// - Define nominal configuration data
    tCoefficientType        = GunnsFluidHiFiOrificeConfigData::DISCHARGE_COEFF;
    tCoefficientValue       = 0.7;
    tThroatDiameter         = 0.005;
    tCriticalReynolds       = 2300.0;
    tExpansionScaleFactor   = 1.0;
    tFlowTuningFactor       = 1.0;
    tConfigData             = new GunnsFluidHiFiOrificeConfigData(tLinkName,
                                                                  &tNodeList,
                                                                  tCoefficientType,
                                                                  tCoefficientValue,
                                                                  tThroatDiameter,
                                                                  tCriticalReynolds,
                                                                  tExpansionScaleFactor,
                                                                  tFlowTuningFactor);

    /// - Define nominal input data
    tMalfBlockageFlag  = false;
    tMalfBlockageValue = 1.0;
    tInputData         = new GunnsFluidHiFiOrificeInputData(tMalfBlockageFlag,
                                                            tMalfBlockageValue);

    tArticle = new FriendlyGunnsFluidHiFiOrifice;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 01: testConfig ......................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName                                        == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                                           == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tCoefficientType                                 == tConfigData->mCoefficientType);
    CPPUNIT_ASSERT(tCoefficientValue                                == tConfigData->mCoefficientValue);
    CPPUNIT_ASSERT(tThroatDiameter                                  == tConfigData->mThroatDiameter);
    CPPUNIT_ASSERT(tCriticalReynolds                                == tConfigData->mCriticalReynolds);
    CPPUNIT_ASSERT(tExpansionScaleFactor                            == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFlowTuningFactor                                == tConfigData->mFlowTuningFactor);

    /// - Check default config construction
    GunnsFluidHiFiOrificeConfigData defaultConfig;
    CPPUNIT_ASSERT(""                                               == defaultConfig.mName);
    CPPUNIT_ASSERT(0                                                == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(GunnsFluidHiFiOrificeConfigData::DISCHARGE_COEFF == defaultConfig.mCoefficientType);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mCoefficientValue);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mThroatDiameter);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mCriticalReynolds);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(1.0                                              == defaultConfig.mFlowTuningFactor);

    /// - Check copy config construction
    GunnsFluidHiFiOrificeConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName                                        == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                                           == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tCoefficientType                                 == copyConfig.mCoefficientType);
    CPPUNIT_ASSERT(tCoefficientValue                                == copyConfig.mCoefficientValue);
    CPPUNIT_ASSERT(tThroatDiameter                                  == copyConfig.mThroatDiameter);
    CPPUNIT_ASSERT(tCriticalReynolds                                == copyConfig.mCriticalReynolds);
    CPPUNIT_ASSERT(tExpansionScaleFactor                            == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFlowTuningFactor                                == copyConfig.mFlowTuningFactor);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testInput()
{
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 02: testInput .......................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);

    /// - Check default config construction
    GunnsFluidHiFiOrificeInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);

    /// - Check copy config construction
    GunnsFluidHiFiOrificeInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == copyInput.mMalfBlockageValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 03: testDefaultConstruction .........";

    /// @test config data
    CPPUNIT_ASSERT(0.0                               == tArticle->mCdDefault);
    CPPUNIT_ASSERT(0.0                               == tArticle->mThroatDiameter);
    CPPUNIT_ASSERT(0.0                               == tArticle->mReCritical);
    CPPUNIT_ASSERT(0.0                               == tArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(0.0                               == tArticle->mFlowTuningFactor);
    CPPUNIT_ASSERT(0.0                               == tArticle->mThroatArea);
    CPPUNIT_ASSERT(0.0                               == tArticle->mReActual);
    CPPUNIT_ASSERT(0.0                               == tArticle->mCdActual);
    CPPUNIT_ASSERT(0.0                               == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0                               == tArticle->mPressureRatio);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidHiFiOrifice* article = new GunnsFluidHiFiOrifice();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 04: testNominalInitialization .......";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidHiFiOrifice article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test config and input data
    const double expectedThroatArea = UnitConversion::PI_UTIL * tThroatDiameter * tThroatDiameter / 4.0;
    CPPUNIT_ASSERT(tThroatDiameter                   == article.mThroatDiameter);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedThroatArea, article.mThroatArea,         DBL_EPSILON);
    CPPUNIT_ASSERT(tCoefficientValue                 == article.mCdDefault);
    CPPUNIT_ASSERT(tExpansionScaleFactor             == article.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFlowTuningFactor                 == article.mFlowTuningFactor);

    /// @test state data
    CPPUNIT_ASSERT(0.0                               == article.mReActual);
    CPPUNIT_ASSERT(0.0                               == article.mCdActual);
    CPPUNIT_ASSERT(0.0                               == article.mSystemConductance);
    CPPUNIT_ASSERT(0.0                               == article.mPressureRatio);
    CPPUNIT_ASSERT(0                                 != article.mInternalFluid);

    /// - Verify the parent method is called
    CPPUNIT_ASSERT(tLinkName                         == article.getName());
    CPPUNIT_ASSERT(tMalfBlockageValue                == article.mMalfBlockageValue);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests various nominal initialization options.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testInitializationOptions()
{
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 05: testInitializationOptions .......";

    /// - Initialize with flow coefficient type defined as FLOW_FACTOR, and zero throat length.
    const double expectedThroatArea = UnitConversion::PI_UTIL * tThroatDiameter * tThroatDiameter / 4.0;

    FriendlyGunnsFluidHiFiOrifice article;
    tConfigData->mCoefficientType   = GunnsFluidHiFiOrificeConfigData::FLOW_FACTOR;
    tConfigData->mCoefficientValue  = tCoefficientValue * expectedThroatArea * 50911.7;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test config and input data
    CPPUNIT_ASSERT(tThroatDiameter                   == article.mThroatDiameter);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedThroatArea, article.mThroatArea,         DBL_EPSILON);
    CPPUNIT_ASSERT(tCoefficientValue                 == article.mCdDefault);
    CPPUNIT_ASSERT(tExpansionScaleFactor             == article.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFlowTuningFactor                 == article.mFlowTuningFactor);

    /// @test state data
    CPPUNIT_ASSERT(0.0                               == article.mReActual);
    CPPUNIT_ASSERT(0.0                               == article.mCdActual);
    CPPUNIT_ASSERT(0.0                               == article.mSystemConductance);
    CPPUNIT_ASSERT(0.0                               == article.mPressureRatio);

    /// - Verify the parent method is called
    CPPUNIT_ASSERT(tLinkName                         == article.getName());
    CPPUNIT_ASSERT(tMalfBlockageValue                == article.mMalfBlockageValue);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Re-initialize with flow coefficient type defined as FLOW_COEFF.
    tConfigData->mCoefficientType  = GunnsFluidHiFiOrificeConfigData::FLOW_COEFF;
    tConfigData->mCoefficientValue = tCoefficientValue * expectedThroatArea * 50911.7 / 0.865;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCoefficientValue, article.mCdDefault, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 06: testInitializationExceptions ....";

    /// @test for exception on throat diameter = 0
    tConfigData->mThroatDiameter = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mThroatDiameter = tThroatDiameter;

    /// @test for exception on bad coefficient type
    tConfigData->mCoefficientType = static_cast<GunnsFluidHiFiOrificeConfigData::CoeffTypes>(99);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCoefficientType = GunnsFluidHiFiOrificeConfigData::DISCHARGE_COEFF;

    /// @test for exception on Cd flow coefficient > 1
    tConfigData->mCoefficientValue = 1.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCoefficientValue = tCoefficientValue;

    /// @test for exception on Cd = 0
    tConfigData->mCoefficientValue = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCoefficientValue = tCoefficientValue;

    /// @test for exception on critical Re < 0
    tConfigData->mCriticalReynolds = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCriticalReynolds = tCriticalReynolds;

    /// @test for exception on expansion scale factor > 1 or < 0
    tConfigData->mExpansionScaleFactor = 1.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    tConfigData->mExpansionScaleFactor = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mExpansionScaleFactor = tExpansionScaleFactor;

    /// @test for exception on flow tuning factor < 0
    tConfigData->mFlowTuningFactor = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mFlowTuningFactor = tFlowTuningFactor;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testRestart()
{
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 07: testRestart .....................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set some stuff in the article and its base class that should be reset on restart.
    tArticle->mPower                 = 1.0;
    tArticle->mSystemConductance     = 1.0;
    tArticle->mPressureRatio         = 1.0;

    /// @test restart rests terms
    tArticle->restart();

    CPPUNIT_ASSERT(0.0                               == tArticle->mPower);
    CPPUNIT_ASSERT(0.0                               == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0                               == tArticle->mPressureRatio);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method for non-choked gas flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testStepGasNonChoked()
{
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 08: testStepGasNonChoked ............";

    /// - Initialize default test article with nominal initialization data
    tArticle->setMinLinearizationPotential(0.0);
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set up last-pass potential vector for forward flow with low enough dp to be non-choked.
    double p0 = 101000.0;   // (Pa)
    double p1 = 100000.0;
    tNodes[0].setPotential(p0 * UnitConversion::KPA_PER_PA);
    tNodes[1].setPotential(p1 * UnitConversion::KPA_PER_PA);
    tNodes[0].updateMass();
    tNodes[1].updateMass();
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tArticle->mPotentialVector[0] = p0 * UnitConversion::KPA_PER_PA;
    tArticle->mPotentialVector[1] = p1 * UnitConversion::KPA_PER_PA;

    tArticle->step(tTimeStep);

    /// @test step outputs for forward non-choked gas flow
    const double expectedEffArea = UnitConversion::PI_UTIL * tThroatDiameter * tThroatDiameter / 4.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEffArea,                   tArticle->getEffectiveArea(), DBL_EPSILON);
    CPPUNIT_ASSERT(tArticle->mPressureRatio > 1.0);

    const double gamma = tNodes[0].getOutflow()->getAdiabaticIndex();
    const double rho0  = tNodes[0].getOutflow()->getDensity();
    const double flux  = sqrt(2 * p0 * rho0 * gamma/(gamma-1) * (powf(p1/p0, 2/gamma)
                                                               - powf(p1/p0, (gamma+1)/gamma)));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( flux, tArticle->computeSubCriticalGasFlux(gamma, p0, rho0, p1), DBL_EPSILON);
    const double conductivity = tCoefficientValue * flux * UnitConversion::PA_PER_KPA / (p0 - p1);
    const double avgMW = (tNodes[0].getOutflow()->getMWeight() + tNodes[1].getOutflow()->getMWeight()) * 0.5;
    const double expectedG = conductivity * expectedEffArea / avgMW;

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mSystemConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedG, tArticle->mAdmittanceMatrix[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedG, tArticle->mAdmittanceMatrix[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mAdmittanceMatrix[3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[1],       DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL        ( true,      tArticle->mAdmittanceUpdate);

    /// @test ballpark agreement with an equivalent fluid conductor
    GunnsFluidConductor           conductor;
    GunnsFluidConductorConfigData conductorConfig("conductor",
                                                  &tNodeList,
                                                  expectedEffArea * tCoefficientValue,
                                                  tExpansionScaleFactor);
    GunnsFluidConductorInputData  conductorInput(tMalfBlockageFlag,
                                                 tMalfBlockageValue);
    conductor.setMinLinearizationPotential(0.0);
    conductor.initialize(conductorConfig, conductorInput, tLinks, tPort0, tPort1);
    double* conductorP = conductor.getPotentialVector();
    conductorP[0] = p0 * UnitConversion::KPA_PER_PA;
    conductorP[1] = p1 * UnitConversion::KPA_PER_PA;
    conductor.step(tTimeStep);

    double* conductorA = conductor.getAdmittanceMatrix();
    CPPUNIT_ASSERT_DOUBLES_EQUAL( conductorA[0], expectedG,     expectedG);

    /// @test equal node pressures
    p0 = 100000.0;   // (Pa)
    p1 = 100000.0;
    tNodes[0].setPotential(p0 * UnitConversion::KPA_PER_PA);
    tNodes[1].setPotential(p1 * UnitConversion::KPA_PER_PA);
    tNodes[0].updateMass();
    tNodes[1].updateMass();
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tArticle->mPotentialVector[0] = p0 * UnitConversion::KPA_PER_PA;
    tArticle->mPotentialVector[1] = p1 * UnitConversion::KPA_PER_PA;

    tArticle->step(tTimeStep);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method for choked gas flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testStepGasChoked()
{
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 09: testStepGasChoked ...............";

    /// - Initialize default test article with nominal initialization data, and activate the
    ///   blockage malfunction.
    tArticle->setMinLinearizationPotential(0.0);
    tInputData->mMalfBlockageFlag  = true;
    tInputData->mMalfBlockageValue = 0.5;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set up last-pass potential vector for reverse flow with large enough dp to be choked.
    const double p0 = 300000.0;   // (Pa)
    const double p1 = 100000.0;
    tNodes[0].setPotential(p1 * UnitConversion::KPA_PER_PA);
    tNodes[1].setPotential(p0 * UnitConversion::KPA_PER_PA);
    tNodes[0].updateMass();
    tNodes[1].updateMass();
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tArticle->mPotentialVector[0] = p1 * UnitConversion::KPA_PER_PA;
    tArticle->mPotentialVector[1] = p0 * UnitConversion::KPA_PER_PA;

    tArticle->step(tTimeStep);

    /// @test step outputs for reverse choked gas flow
    const double expectedEffArea = 0.5 * UnitConversion::PI_UTIL * tThroatDiameter * tThroatDiameter / 4.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEffArea,               tArticle->getEffectiveArea(), DBL_EPSILON);
    CPPUNIT_ASSERT(tArticle->mPressureRatio < 1.0);

    double gamma = tNodes[1].getOutflow()->getAdiabaticIndex();
    double rho0  = tNodes[1].getOutflow()->getDensity();
    double flux  = sqrt(gamma * p0 * rho0 * powf(2/(gamma+1), (gamma+1)/(gamma-1)));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( flux, tArticle->computeCriticalGasFlux(gamma, p0, rho0), DBL_EPSILON);
    double conductivity = tCoefficientValue * flux * UnitConversion::PA_PER_KPA / (p0 - p1);
    double avgMW = (tNodes[0].getOutflow()->getMWeight() + tNodes[1].getOutflow()->getMWeight()) * 0.5;
    double expectedG = conductivity * expectedEffArea / avgMW;

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mSystemConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedG, tArticle->mAdmittanceMatrix[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedG, tArticle->mAdmittanceMatrix[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mAdmittanceMatrix[3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[1],       DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL        ( true,      tArticle->mAdmittanceUpdate);

    /// @test ballpark agreement with an equivalent fluid conductor
    GunnsFluidConductor           conductor;
    GunnsFluidConductorConfigData conductorConfig("conductor",
                                                  &tNodeList,
                                                  expectedEffArea * tCoefficientValue,
                                                  tExpansionScaleFactor);
    GunnsFluidConductorInputData  conductorInput(true, 0.5);
    conductor.setMinLinearizationPotential(0.0);
    conductor.initialize(conductorConfig, conductorInput, tLinks, tPort0, tPort1);
    double* conductorP = conductor.getPotentialVector();
    conductorP[0] = p1 * UnitConversion::KPA_PER_PA;
    conductorP[1] = p0 * UnitConversion::KPA_PER_PA;
    conductor.step(tTimeStep);

    double* conductorA = conductor.getAdmittanceMatrix();
    CPPUNIT_ASSERT_DOUBLES_EQUAL( conductorA[0], expectedG,     expectedG);

    /// @test flow to port 1 Ground node uses inlet MW.
    tNodes[0].setPotential(p0 * UnitConversion::KPA_PER_PA);
    tNodes[1].setPotential(p1 * UnitConversion::KPA_PER_PA);
    tNodes[0].updateMass();
    tNodes[1].updateMass();
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tArticle->setPort(1, 4);
    tArticle->setPort(0, 0);
    tArticle->mPotentialVector[0] = p0 * UnitConversion::KPA_PER_PA;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->step(tTimeStep);

    gamma = tNodes[0].getOutflow()->getAdiabaticIndex();
    rho0  = tNodes[0].getOutflow()->getDensity();
    flux  = sqrt(gamma * p0 * rho0 * powf(2/(gamma+1), (gamma+1)/(gamma-1)));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flux, tArticle->computeCriticalGasFlux(gamma, p0, rho0), DBL_EPSILON);
    conductivity = tCoefficientValue * flux * UnitConversion::PA_PER_KPA / p0;
    avgMW = tNodes[0].getOutflow()->getMWeight();
    expectedG = conductivity * expectedEffArea / avgMW;

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mSystemConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedG, tArticle->mAdmittanceMatrix[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedG, tArticle->mAdmittanceMatrix[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mAdmittanceMatrix[3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[1],       DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL        ( true,      tArticle->mAdmittanceUpdate);

    /// @test both ports on Ground.
    tArticle->setPort(0, 4);
    tArticle->mPotentialVector[0] = 0.0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->step(tTimeStep));

    /// @test flow to port 0 Ground node uses inlet MW.
    tArticle->setPort(1, 0);
    tArticle->mPotentialVector[1] = p0 * UnitConversion::KPA_PER_PA;
    tArticle->step(tTimeStep);

    gamma = tNodes[0].getOutflow()->getAdiabaticIndex();
    rho0  = tNodes[0].getOutflow()->getDensity();
    flux  = sqrt(gamma * p0 * rho0 * powf(2/(gamma+1), (gamma+1)/(gamma-1)));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(flux, tArticle->computeCriticalGasFlux(gamma, p0, rho0), DBL_EPSILON);
    conductivity = tCoefficientValue * flux * UnitConversion::PA_PER_KPA / p0;
    avgMW = tNodes[0].getOutflow()->getMWeight();
    expectedG = conductivity * expectedEffArea / avgMW;

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mSystemConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedG, tArticle->mAdmittanceMatrix[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedG, tArticle->mAdmittanceMatrix[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mAdmittanceMatrix[3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[1],       DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL        ( true,      tArticle->mAdmittanceUpdate);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method for liquid flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testStepLiquid()
{
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 10: testStepLiquid ..................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->setMinLinearizationPotential(0.0);
    tPort0 = 2;
    tPort1 = 3;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set up last-pass potential vector for forward flow.
    const double p0 = 101000.0;   // (Pa)
    const double p1 = 100000.0;
    tNodes[tPort0].setPotential(p0 * UnitConversion::KPA_PER_PA);
    tNodes[tPort1].setPotential(p1 * UnitConversion::KPA_PER_PA);
    tNodes[tPort0].updateMass();
    tNodes[tPort1].updateMass();
    tNodes[tPort0].resetFlows();
    tNodes[tPort1].resetFlows();
    tArticle->mPotentialVector[0] = p0 * UnitConversion::KPA_PER_PA;
    tArticle->mPotentialVector[1] = p1 * UnitConversion::KPA_PER_PA;

    tArticle->step(tTimeStep);

    /// @test step outputs for forward liquid flow
    const double expectedEffArea = UnitConversion::PI_UTIL * tThroatDiameter * tThroatDiameter / 4.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEffArea,               tArticle->getEffectiveArea(), DBL_EPSILON);
    //CPPUNIT_ASSERT(tArticle->mPressureRatio == 0.0);

    const double rho  = 0.5 * (tNodes[tPort0].getOutflow()->getDensity() + tNodes[tPort1].getOutflow()->getDensity());
    const double dp   = p0 - p1;
    const double flux = sqrt(2 * dp * rho);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( flux, tArticle->computeBernoulliFlux(rho, dp), DBL_EPSILON);
    const double conductivity = tCoefficientValue * flux * UnitConversion::PA_PER_KPA / dp;
    const double avgMW = (tNodes[tPort0].getOutflow()->getMWeight() + tNodes[tPort1].getOutflow()->getMWeight()) * 0.5;
    const double expectedG = conductivity * expectedEffArea / avgMW;

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mSystemConductance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedG, tArticle->mAdmittanceMatrix[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedG, tArticle->mAdmittanceMatrix[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedG, tArticle->mAdmittanceMatrix[3],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[1],       DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL        ( true,      tArticle->mAdmittanceUpdate);

    /// @test ballpark agreement with an equivalent fluid conductor
    GunnsFluidConductor           conductor;
    GunnsFluidConductorConfigData conductorConfig("conductor",
                                                  &tNodeList,
                                                  expectedEffArea * tCoefficientValue,
                                                  tExpansionScaleFactor);
    GunnsFluidConductorInputData  conductorInput(false, 0.0);
    conductor.setMinLinearizationPotential(0.0);
    conductor.initialize(conductorConfig, conductorInput, tLinks, tPort0, tPort1);
    double* conductorP = conductor.getPotentialVector();
    conductorP[0] = p0 * UnitConversion::KPA_PER_PA;
    conductorP[1] = p1 * UnitConversion::KPA_PER_PA;
    conductor.step(tTimeStep);

    double* conductorA = conductor.getAdmittanceMatrix();
    CPPUNIT_ASSERT_DOUBLES_EQUAL( conductorA[0], expectedG,     expectedG);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with no internal fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testComputeFlows()
{
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 11: testComputeFlows ................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

        /// - Confirm correct null port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);
    
    /// - Confirm correct null port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);

    tArticle->transportFlows(tTimeStep);

    /// - Confirm mVolFlowRate is zero with a zero potential vector (TransportFlows)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mVolFlowRate, DBL_EPSILON);

    /// @test zero flow rate because the potential vector is zero
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlowRate, DBL_EPSILON);

    tArticle->mPotentialVector[0] = 0.8;
    tArticle->computeFlows(tTimeStep);

    /// - Confirm correct source port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);
    
    /// - Confirm correct sink port allocation with postive potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[1]);

    /// - Nodal outflux scheduling should be equal to step molar flux for source node
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getScheduledOutflux(),tArticle->mFlux,DBL_EPSILON);

    tArticle->transportFlows(tTimeStep);  
        
    /// - Confirm correct source port selection with Postive potential Vector (TransportFlows)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, tArticle->determineSourcePort(tArticle->mFlux, 0, 1), DBL_EPSILON);

    /// - Confirm mVolFlowRate is zero with a zero potential vector (TransportFlows)
    CPPUNIT_ASSERT(tArticle->mVolFlowRate > 0.0);

    /// @test zero flow rate because the potential vector is zero
    CPPUNIT_ASSERT(tArticle->mFlowRate > 0.0);

    tArticle->mPotentialVector[0] = -0.6;
    tArticle->computeFlows(tTimeStep);

     /// - Molar flux should be less than zero because the port 0 potential vector is negative   
    CPPUNIT_ASSERT( tArticle->mFlux < 0.0);

    /// - Confirm correct sink port allocation with negative potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);
    
    /// - Confirm correct source port allocation with negative potential vector (computeFlows)
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[1]);

    /// - Nodal outflux scheduling should be equal to step molar flux for source node
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getScheduledOutflux(),- tArticle->mFlux,DBL_EPSILON);
    
    tArticle->transportFlows(tTimeStep);

    /// - Confirm correct source port selection with negative potential Vector (TrasportFlows)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, tArticle->determineSourcePort(tArticle->mFlux, 0, 1), 
                                                            DBL_EPSILON);

    /// @test negative flow rate because the port 0 potential vector is negative
    CPPUNIT_ASSERT(tArticle->mFlowRate < 0.0);

    /// - Verify volumetric flow rate and hydraulic power
    const double expectedVolFlow = tArticle->mFlowRate / tNodes[1].getOutflow()->getDensity();
    const double expectedPower = -1000.0 * expectedVolFlow *
            (tArticle->mPotentialVector[0] - tArticle->mPotentialVector[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolFlow, tArticle->mVolFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,   tArticle->mPower,       DBL_EPSILON);

    /// - Verify zero flow when delta-pressure is very small.
    tArticle->mPotentialVector[0] = 1.0;
    tArticle->mPotentialVector[1] = 1.0 + 10.0*DBL_EPSILON;
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tArticle->mFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mFlowRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mVolFlowRate);

    /// - Verify zero flow when all pressures are zero.
    tArticle->mPotentialVector[0] = 0.0;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tArticle->mFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mFlowRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mVolFlowRate);

    /// - Verify volumetric flow rate when the source density is zero
    tArticle->mPotentialVector[0] = -0.6;
    tNodes[1].resetContentState();
    tNodes[1].resetFlows();
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);
    
    CPPUNIT_ASSERT(0.0 == tArticle->mVolFlowRate);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test setter & getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testAccessMethods()
{
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 12: testAccessMethods ...............";

    /// - There are currently no access methods to test.

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests nominal initialization with real-world valve examples.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiOrifice::testInitializationRealValves()
{
    std::cout << "\n UtGunnsFluidHiFiOrifice ...... 13: testInitializationRealValves ....";

    /// - Define flow coefficients and diameters (in inches).  These are real-world valves that were
    ///   used in the HESTIA project:
    tConfigData->mCoefficientType = GunnsFluidHiFiOrificeConfigData::FLOW_COEFF;
    const double exampleCv[]      = {1.1,   0.096, 0.088, 0.165,   0.65,  1.57,  0.28,  1.69};
    const double exampleD[]       = {0.406, 0.074, 0.063, 0.09375, 0.177, 0.25,  0.125, 0.295};
    const double expectedCd[]     = {0.224, 0.588, 0.743, 0.629,   0.696, 0.842, 0.601, 0.651};

    for (int i=0; i<sizeof(exampleCv)/sizeof(double); ++i) {
        tConfigData->mCoefficientValue = exampleCv[i];
        tConfigData->mThroatDiameter   = exampleD[i] * UnitConversion::M_PER_IN;

        FriendlyGunnsFluidHiFiOrifice article;
        CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCd[i], article.mCdDefault, 0.001);
    }

    std::cout << "... Pass";
}
