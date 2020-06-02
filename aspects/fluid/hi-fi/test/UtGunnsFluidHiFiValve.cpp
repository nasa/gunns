/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (aspects/fluid/hi-fi/GunnsFluidHiFiValve.o)
        (core/GunnsFluidConductor.o)
    )
***************************************************************************************************/
#include "UtGunnsFluidHiFiValve.hh"

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsFluidConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidHiFiValve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHiFiValve::UtGunnsFluidHiFiValve()
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
    tPosition(),
    tMalfLeakThruFlag(),
    tMalfLeakThruValue(),
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
/// @details  This is the default destructor for the UtGunnsFluidHiFiValve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHiFiValve::~UtGunnsFluidHiFiValve()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiValve::tearDown()
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
void UtGunnsFluidHiFiValve::setUp()
{
    tLinkName             = "Test Fluid Hi-Fi Valve";
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

    tNodes[2].resetFlows();
    tNodes[3].resetFlows();

    /// - Define nominal configuration data
    tCoefficientType        = GunnsFluidHiFiValveConfigData::DISCHARGE_COEFF;
    tCoefficientValue       = 0.7;
    tThroatDiameter         = 0.005;
    tCriticalReynolds       = 2300.0;
    tExpansionScaleFactor   = 1.0;
    tFlowTuningFactor       = 1.0;
    tConfigData             = new GunnsFluidHiFiValveConfigData(tLinkName,
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
    tPosition          = 1.0;
    tMalfLeakThruFlag  = false;
    tMalfLeakThruValue = 1.0;
    tInputData         = new GunnsFluidHiFiValveInputData(tMalfBlockageFlag,
                                                          tMalfBlockageValue,
                                                          tPosition,
                                                          tMalfLeakThruFlag,
                                                          tMalfLeakThruValue);

    tArticle = new FriendlyGunnsFluidHiFiValve;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiValve::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidHiFiValve ........ 01: testConfig ......................";

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
    GunnsFluidHiFiValveConfigData defaultConfig;
    CPPUNIT_ASSERT(""                                               == defaultConfig.mName);
    CPPUNIT_ASSERT(0                                                == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(GunnsFluidHiFiValveConfigData::DISCHARGE_COEFF   == defaultConfig.mCoefficientType);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mCoefficientValue);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mThroatDiameter);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mCriticalReynolds);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(1.0                                              == defaultConfig.mFlowTuningFactor);

    /// - Check copy config construction
    GunnsFluidHiFiValveConfigData copyConfig(*tConfigData);
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
void UtGunnsFluidHiFiValve::testInput()
{
    std::cout << "\n UtGunnsFluidHiFiValve ........ 02: testInput .......................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tPosition          == tInputData->mPosition);
    CPPUNIT_ASSERT(tMalfLeakThruFlag  == tInputData->mMalfLeakThruFlag);
    CPPUNIT_ASSERT(tMalfLeakThruValue == tInputData->mMalfLeakThruValue);

    /// - Check default config construction
    GunnsFluidHiFiValveInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                == defaultInput.mPosition);
    CPPUNIT_ASSERT(false              == defaultInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfLeakThruValue);

    /// - Check copy config construction
    GunnsFluidHiFiValveInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tPosition          == copyInput.mPosition);
    CPPUNIT_ASSERT(tMalfLeakThruFlag  == copyInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT(tMalfLeakThruValue == copyInput.mMalfLeakThruValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiValve::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidHiFiValve ........ 03: testDefaultConstruction .........";

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
    CPPUNIT_ASSERT(false                             == tArticle->mMalfLeakThruFlag);
    CPPUNIT_ASSERT(0.0                               == tArticle->mMalfLeakThruValue);
    CPPUNIT_ASSERT(0.0                               == tArticle->mPosition);
    CPPUNIT_ASSERT(0.0                               == tArticle->mLeakArea);
    CPPUNIT_ASSERT(0.0                               == tArticle->mLastLeakRate);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidHiFiValve* article = new GunnsFluidHiFiValve();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiValve::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidHiFiValve ........ 04: testNominalInitialization .......";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidHiFiValve article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test initialization of the base class
    CPPUNIT_ASSERT(tLinkName                         == article.getName());
    CPPUNIT_ASSERT(tMalfBlockageValue                == article.mMalfBlockageValue);
    CPPUNIT_ASSERT(tThroatDiameter                   == article.mThroatDiameter);

    /// @test config and input data
    CPPUNIT_ASSERT(tPosition                         == article.mPosition);
    CPPUNIT_ASSERT(tMalfLeakThruFlag                 == article.mMalfLeakThruFlag);
    CPPUNIT_ASSERT(tMalfLeakThruValue                == article.mMalfLeakThruValue);

    /// @test state data
    CPPUNIT_ASSERT(0.0                               == article.mLeakArea);
    CPPUNIT_ASSERT(0.0                               == article.mLastLeakRate);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiValve::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidHiFiValve ........ 06: testInitializationExceptions ....";

    /// @test for base class exception on throat diameter = 0
    tConfigData->mThroatDiameter = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mThroatDiameter = tThroatDiameter;

    /// @test for exception on bad initial position
    tInputData->mPosition = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mPosition =  1.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mPosition = tPosition;

    /// @test for exception on bad leak thru malf rate
    tInputData->mMalfLeakThruValue = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mMalfLeakThruValue = tMalfLeakThruValue;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiValve::testRestart()
{
    std::cout << "\n UtGunnsFluidHiFiValve ........ 07: testRestart .....................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set some stuff in the article and its base class that should be reset on restart.
    tArticle->mPower                 = 1.0;

    /// @test restart rests terms
    tArticle->restart();

    CPPUNIT_ASSERT(0.0                               == tArticle->mPower);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method for gas flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiValve::testStepGas()
{
    std::cout << "\n UtGunnsFluidHiFiValve ........ 08: testStepGas .....................";

    /// - Initialize default test article with nominal initialization data
    tArticle->setMinLinearizationPotential(0.0);
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set up last-pass conditions for forward, non-choked gas flow.
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
    tArticle->mPressureRatio = 1.01;

    /// - Close the valve and set the leak-thru malfunction.
    double desiredRate = 1.0e-4;
    tArticle->setMalfLeakThru(true, desiredRate);
    tArticle->setPosition(0.0);

    /// @test the actual leak-thru rate derived from the link's conductance.
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT(tArticle->mLeakArea >= 0.0);
    CPPUNIT_ASSERT(tArticle->mLeakArea <= tArticle->mThroatArea);
    double avgMW = (tNodes[0].getOutflow()->getMWeight() + tNodes[1].getOutflow()->getMWeight()) * 0.5;

    // mass rate (kg/s) = conductance (kg*mol/kPa/s) * MW (kg/kg/mol) * dp (kPa)
    double actualRate = tArticle->mSystemConductance * avgMW * (p0 - p1) * UnitConversion::KPA_PER_PA;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(desiredRate, actualRate, DBL_EPSILON);

    /// - Set up last-pass conditions for reverse, choked gas flow.
    p0 = 300000.0;   // (Pa)
    p1 = 100000.0;
    tNodes[0].setPotential(p1 * UnitConversion::KPA_PER_PA);
    tNodes[1].setPotential(p0 * UnitConversion::KPA_PER_PA);
    tNodes[0].updateMass();
    tNodes[1].updateMass();
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tArticle->mPotentialVector[0] = p1 * UnitConversion::KPA_PER_PA;
    tArticle->mPotentialVector[1] = p0 * UnitConversion::KPA_PER_PA;
    tArticle->mPressureRatio = 0.99;

    /// - Close the valve and set the leak-thru malfunction.
    desiredRate = 1.0e-3;
    tArticle->setMalfLeakThru(true, desiredRate);
    tArticle->setPosition(0.0);

    /// @test the actual leak-thru rate derived from the link's conductance.
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT(tArticle->mLeakArea >= 0.0);
    CPPUNIT_ASSERT(tArticle->mLeakArea <= tArticle->mThroatArea);
    avgMW = (tNodes[0].getOutflow()->getMWeight() + tNodes[1].getOutflow()->getMWeight()) * 0.5;

    // mass rate (kg/s) = conductance (kg*mol/kPa/s) * MW (kg/kg/mol) * dp (kPa)
    actualRate = tArticle->mSystemConductance * avgMW * (p0 - p1) * UnitConversion::KPA_PER_PA;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(desiredRate, actualRate, DBL_EPSILON);

    /// @test same conductance on subsequent passes with malf rate unchanged.
    tArticle->step(tTimeStep);
    actualRate = tArticle->mSystemConductance * avgMW * (p0 - p1) * UnitConversion::KPA_PER_PA;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(desiredRate, actualRate, DBL_EPSILON);

    /// @test malfunction off
    tArticle->setMalfLeakThru();
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mLeakArea,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSystemConductance, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method for liquid flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiValve::testStepLiquid()
{
    std::cout << "\n UtGunnsFluidHiFiValve ........ 08: testStepLiquid ..................";

    /// - Initialize default test article with nominal initialization data
    tArticle->setMinLinearizationPotential(0.0);
    tPort0 = 2;
    tPort1 = 3;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set up last-pass conditions for forward liquid flow.
    double p0 = 101000.0;   // (Pa)
    double p1 = 100000.0;
    tNodes[tPort0].setPotential(p0 * UnitConversion::KPA_PER_PA);
    tNodes[tPort1].setPotential(p1 * UnitConversion::KPA_PER_PA);
    tNodes[tPort0].updateMass();
    tNodes[tPort1].updateMass();
    tNodes[tPort0].resetFlows();
    tNodes[tPort1].resetFlows();
    tArticle->mPotentialVector[0] = p0 * UnitConversion::KPA_PER_PA;
    tArticle->mPotentialVector[1] = p1 * UnitConversion::KPA_PER_PA;

    /// - Close the valve and set the leak-thru malfunction.
    double desiredRate = 1.0e-4;
    tArticle->setMalfLeakThru(true, desiredRate);
    tArticle->setPosition(0.0);

    /// @test the actual leak-thru rate derived from the link's conductance.
    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT(tArticle->mLeakArea >= 0.0);
    CPPUNIT_ASSERT(tArticle->mLeakArea <= tArticle->mThroatArea);
    double avgMW = (tNodes[tPort0].getOutflow()->getMWeight() + tNodes[tPort1].getOutflow()->getMWeight()) * 0.5;

    // mass rate (kg/s) = conductance (kg*mol/kPa/s) * MW (kg/kg/mol) * dp (kPa)
    double actualRate = tArticle->mSystemConductance * avgMW * (p0 - p1) * UnitConversion::KPA_PER_PA;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(desiredRate, actualRate, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test setter & getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiValve::testAccessMethods()
{
    std::cout << "\n UtGunnsFluidHiFiValve ........ 09: testAccessMethods ...............";

    /// - All methods are currently accessed & validated thru other tests.

    std::cout << "... Pass";
}
