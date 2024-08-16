/**
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "UtGunnsFluidSelectiveMembrane.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidSelectiveMembrane class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSelectiveMembrane::UtGunnsFluidSelectiveMembrane()
    :
    tLinkName(),
    tMaxConductance(),
    tAbsorptionCoeff(),
    tExternalType(),
    tInternalType(),
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
    tPort2(),
    tTimeStep(),
    tFluidProperties(),
    tFluidConfig(),
    tFluidInput1(),
    tFluidInput2(),
    fractions()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidSelectiveMembrane class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSelectiveMembrane::~UtGunnsFluidSelectiveMembrane()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tFluidConfig;
    delete [] fractions;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::setUp()
{
    tLinkName           = "Test Fluid Selective Membrane";
    tNodeList.mNumNodes = 6;
    tNodeList.mNodes    = tNodes;
    tMaxConductance     = 1E-04;
    tAbsorptionCoeff    = 1E-06;
    tExternalType       = FluidProperties::GUNNS_H2O;
    tInternalType       = FluidProperties::GUNNS_WATER;
    tPort0              = 0;
    tPort1              = 1;
    tPort2              = 2;
    tTimeStep           = 0.1;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[4];
    types[0]     = FluidProperties::GUNNS_N2;
    types[1]     = FluidProperties::GUNNS_H2O;
    types[2]     = FluidProperties::GUNNS_WATER;
    types[3]     = FluidProperties::GUNNS_AMMONIA;
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 4);

    fractions = new double[4];
    fractions[0] = 0.997;
    fractions[1] = 0.003;
    fractions[2] = 0.0;
    fractions[3] = 0.0;
    tFluidInput1 = new PolyFluidInputData(294.261,                  //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[2].initialize("UtTestNode2", tFluidConfig, tFluidInput1);
    tNodes[3].initialize("UtTestNode3", tFluidConfig, tFluidInput1);
    tNodes[4].initialize("UtTestNode4", tFluidConfig, tFluidInput1);
    tNodes[5].initialize("UtTestNode5", tFluidConfig, tFluidInput1);

    tNodes[2].resetFlows();
    tNodes[3].resetFlows();
    tNodes[4].resetFlows();
    tNodes[5].resetFlows();

    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 1.0;
    tFluidInput2 = new PolyFluidInputData(294.261,                  //temperature
                                          689.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions

    tNodes[0].initialize("UtTestNode0", tFluidConfig, tFluidInput2);
    tNodes[1].initialize("UtTestNode1", tFluidConfig, tFluidInput2);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    /// - Define nominal configuration data
    tConfigData        = new GunnsFluidSelectiveMembraneConfigData(tLinkName,
                                                                   &tNodeList,
                                                                   tMaxConductance,
                                                                   tAbsorptionCoeff,
                                                                   tExternalType,
                                                                   tInternalType);

    /// - Define nominal input data
    tMalfBlockageFlag  = false;
    tMalfBlockageValue = 1.0;
    tInputData         = new GunnsFluidSelectiveMembraneInputData(tMalfBlockageFlag,
                                                                  tMalfBlockageValue);

    tArticle = new FriendlyGunnsFluidSelectiveMembrane;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidSelectiveMembrane .... 001: testConfig ....................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName                 == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                    == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductance           == tConfigData->mMaxConductance);
    CPPUNIT_ASSERT(tAbsorptionCoeff          == tConfigData->mAbsorptionCoeff);
    CPPUNIT_ASSERT(tExternalType             == tConfigData->mExternalType);
    CPPUNIT_ASSERT(tInternalType             == tConfigData->mInternalType);

    /// - Check default config construction
    GunnsFluidSelectiveMembraneConfigData defaultConfig;
    CPPUNIT_ASSERT(""                        == defaultConfig.mName);
    CPPUNIT_ASSERT(0                         == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0                         == defaultConfig.mMaxConductance);
    CPPUNIT_ASSERT(0                         == defaultConfig.mAbsorptionCoeff);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == defaultConfig.mExternalType);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == defaultConfig.mInternalType);

    /// - Check copy config construction
    GunnsFluidSelectiveMembraneConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName                 == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                    == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductance           == copyConfig.mMaxConductance);
    CPPUNIT_ASSERT(tAbsorptionCoeff          == copyConfig.mAbsorptionCoeff);
    CPPUNIT_ASSERT(tExternalType             == copyConfig.mExternalType);
    CPPUNIT_ASSERT(tInternalType             == copyConfig.mInternalType);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::testInput()
{
    std::cout << "\n UtGunnsFluidSelectiveMembrane .... 002: testInput .....................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);

    /// - Check default config construction
    GunnsFluidSelectiveMembraneInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);

    /// - Check copy config construction
    GunnsFluidSelectiveMembraneInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == copyInput.mMalfBlockageValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidSelectiveMembrane .... 003: testDefaultConstruction .......";

    /// @test proper default construction of class member data
    CPPUNIT_ASSERT(false                     == tArticle->mMalfMembraneDegradeFlag);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMalfMembraneDegradeValue);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMaxConductance);
    CPPUNIT_ASSERT(0.0                       == tArticle->mAbsorptionCoeff);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == tArticle->mExternalType);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == tArticle->mInternalType);
    CPPUNIT_ASSERT(0.0                       == tArticle->mEffectiveConductance);
    CPPUNIT_ASSERT(0.0                       == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0                       == tArticle->mExternalPartialP);
    CPPUNIT_ASSERT(0.0                       == tArticle->mInternalPartialP[0]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mInternalPartialP[1]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mExternalSaturationP);
    CPPUNIT_ASSERT(0.0                       == tArticle->mInternalSaturationP);
    CPPUNIT_ASSERT(false                     == tArticle->mExternalSaturated);
    CPPUNIT_ASSERT(false                     == tArticle->mInternalSaturated);
    CPPUNIT_ASSERT(false                     == tArticle->mPhaseChange);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMembraneDeltaP[0]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMembraneDeltaP[1]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMembraneFlowRate);
    CPPUNIT_ASSERT(0.0                       == tArticle->mPhaseChangeHeat);
    CPPUNIT_ASSERT(0                         == tArticle->mInternalMembraneFluid);
    CPPUNIT_ASSERT(0                         == tArticle->mExternalMembraneFluid);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidSelectiveMembrane* article = new GunnsFluidSelectiveMembrane();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidSelectiveMembrane .... 004: testNominalInitialization .....";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidSelectiveMembrane article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// - Verify the parent method is called
    CPPUNIT_ASSERT(tLinkName          == article.getName());
    CPPUNIT_ASSERT(tMalfBlockageValue == article.mMalfBlockageValue);

    /// - Verify member variables are properly set
    CPPUNIT_ASSERT(tMaxConductance    == article.mMaxConductance);
    CPPUNIT_ASSERT(tAbsorptionCoeff   == article.mAbsorptionCoeff);
    CPPUNIT_ASSERT(tInternalType      == article.mInternalType);
    CPPUNIT_ASSERT(tExternalType      == article.mExternalType);
    CPPUNIT_ASSERT(0.0                == article.mEffectiveConductance);
    CPPUNIT_ASSERT(0.0                == article.mSystemConductance);
    CPPUNIT_ASSERT(0.0                == article.mInternalPartialP[0]);
    CPPUNIT_ASSERT(0.0                == article.mInternalPartialP[1]);
    CPPUNIT_ASSERT(0.0                == article.mExternalPartialP);
    CPPUNIT_ASSERT(0.0                == article.mInternalSaturationP);
    CPPUNIT_ASSERT(0.0                == article.mExternalSaturationP);
    CPPUNIT_ASSERT(false              == article.mExternalSaturated);
    CPPUNIT_ASSERT(false              == article.mInternalSaturated);
    CPPUNIT_ASSERT(false              == article.mPhaseChange);
    CPPUNIT_ASSERT(0.0                == article.mMembraneDeltaP[0]);
    CPPUNIT_ASSERT(0.0                == article.mMembraneDeltaP[1]);
    CPPUNIT_ASSERT(0.0                == article.mMembraneFlowRate);
    CPPUNIT_ASSERT(0.0                == article.mPhaseChangeHeat);
    CPPUNIT_ASSERT(1.0                == article.mInternalMembraneFluid->getMassFraction(tInternalType));
    CPPUNIT_ASSERT(1.0                == article.mExternalMembraneFluid->getMassFraction(tExternalType));

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidSelectiveMembrane .... 005: testInitializationExceptions ..";

    /// @test for exception on max conductivity < 0.0.
    tConfigData->mMaxConductance = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mMaxConductance = tMaxConductance;

    /// @test for exception on absorption coeff < 0.0.
    tConfigData->mAbsorptionCoeff = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mAbsorptionCoeff = tAbsorptionCoeff;

    /// @test for exception on external type not in the network.
    tConfigData->mExternalType = FluidProperties::GUNNS_CO2;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mExternalType = tExternalType;

    /// @test for exception on internal type not in the network.
    tConfigData->mInternalType = FluidProperties::GUNNS_CO2;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mExternalType = tInternalType;

    /// @test for exception on gas & liquid types not the same compound.
    tConfigData->mInternalType = FluidProperties::GUNNS_AMMONIA;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mInternalType = tInternalType;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::testRestart()
{
    std::cout << "\n UtGunnsFluidSelectiveMembrane .... 006: testRestart ...................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// - Set some stuff in the article and its base class that should be reset on restart.
    tArticle->mPower               = 1.0;
    tArticle->mInternalPartialP[0] = 1.0;

    /// @test restart resets terms
    tArticle->restart();

    CPPUNIT_ASSERT(0.0 == tArticle->mPower);
    CPPUNIT_ASSERT(0.0 == tArticle->mInternalPartialP[0]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the step method for liquid thru-flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::testStep()
{
    std::cout << "\n UtGunnsFluidSelectiveMembrane .... 007: testStep ......................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// - Set up last-pass potential vector and flows for forward flow and liquid evaporation thru
    ///   the membrane.
    double p0 = 300.0;
    double p1 = 290.0;
    double p2 =   0.1;
    tNodes[tPort0].setPotential(p0);
    tNodes[tPort1].setPotential(p1);
    tNodes[tPort2].setPotential(p2);
    tNodes[tPort0].updateMass();
    tNodes[tPort1].updateMass();
    tNodes[tPort2].updateMass();
    tArticle->mPotentialVector[0] = p0;
    tArticle->mPotentialVector[1] = p1;
    tArticle->mPotentialVector[2] = p2;
    tArticle->mFlowRate           = 1.0;
    tArticle->mFlux               = 1.0 / 18.0153;  // MW of water

    tArticle->step(tTimeStep);

    /// @test Conductance and admittance matrix.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxConductance, tArticle->mEffectiveConductance, DBL_EPSILON);
    CPPUNIT_ASSERT(0.0 < tArticle->mSystemConductance);
    CPPUNIT_ASSERT(tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT_EQUAL(tArticle->mSystemConductance, tArticle->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT_EQUAL(tArticle->mSystemConductance,-tArticle->mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT_EQUAL(tArticle->mSystemConductance,-tArticle->mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT_EQUAL(tArticle->mSystemConductance, tArticle->mAdmittanceMatrix[4]);

    /// @test Absorption pressures, rate, phase change and source vector.
    double expectedPpSource = p0; // because 100% liquid water
    double expectedPpVent   = p2 * tNodes[tPort2].getContent()->getMoleFraction(FluidProperties::GUNNS_H2O);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPpSource, tArticle->mInternalPartialP[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPpVent,   tArticle->mExternalPartialP,    DBL_EPSILON);

    double expectedPsatSource = tFluidProperties->getProperties(FluidProperties::GUNNS_WATER)->
                                getSaturationPressure(tNodes[tPort0].getContent()->getTemperature());
    double expectedPsatVent   = tFluidProperties->getProperties(FluidProperties::GUNNS_WATER)->
                                getSaturationPressure(tNodes[tPort2].getContent()->getTemperature());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPsatSource, tArticle->mInternalSaturationP, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPsatVent,   tArticle->mExternalSaturationP, DBL_EPSILON);
    CPPUNIT_ASSERT( tArticle->mInternalSaturated);
    CPPUNIT_ASSERT(!tArticle->mExternalSaturated);
    CPPUNIT_ASSERT( tArticle->mPhaseChange);

    double expectedDp   = expectedPpSource - expectedPpVent;
    double expectedMdot = expectedDp * tAbsorptionCoeff;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp,   tArticle->mMembraneDeltaP[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mMembraneFlowRate,  DBL_EPSILON);

    double expectedW    = expectedMdot / 18.0153;    // MW of water
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedW,   tArticle->mSourceVector[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mSourceVector[1],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,    tArticle->mSourceVector[2],   DBL_EPSILON);

    double expectedQ    = -expectedMdot * 1000.0
                        * tFluidProperties->getProperties(FluidProperties::GUNNS_WATER)->
                          getHeatOfVaporization(tNodes[tPort0].getContent()->getTemperature());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,    tArticle->mPhaseChangeHeat,   FLT_EPSILON);

    /// @test Membrane flow limited to available mass from upstream flow-thru node.
    tArticle->mFlowRate = expectedMdot;

    tArticle->step(tTimeStep);

    expectedMdot *= 0.99;
    expectedQ    *= 0.99;
    expectedW    *= 0.99;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mMembraneFlowRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedW,   tArticle->mSourceVector[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mSourceVector[1],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,    tArticle->mSourceVector[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,    tArticle->mPhaseChangeHeat,   FLT_EPSILON);

    /// @test Blockage malf and reverse thru-flow.
    p0 = 290.0;
    p1 = 300.0;
    tNodes[tPort0].setPotential(p0);
    tNodes[tPort1].setPotential(p1);
    tNodes[tPort0].updateMass();
    tNodes[tPort1].updateMass();
    tArticle->mPotentialVector[0] = p0;
    tArticle->mPotentialVector[1] = p1;
    tArticle->mFlowRate           = -expectedMdot;
    tArticle->mFlux               = -expectedMdot / 18.0153;  // MW of water
    tArticle->mMalfBlockageFlag   = true;
    tArticle->mMalfBlockageValue  = 0.5;
    double expectedG = 0.5 * tMaxConductance;
    double expectedA = 0.5 * tArticle->mSystemConductance;
    expectedMdot *= 0.99;
    expectedW    *= 0.99;
    expectedQ    *= 0.99;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mEffectiveConductance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mSystemConductance,    DBL_EPSILON);
    CPPUNIT_ASSERT(tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT_EQUAL(tArticle->mSystemConductance, tArticle->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT_EQUAL(tArticle->mSystemConductance,-tArticle->mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT_EQUAL(tArticle->mSystemConductance,-tArticle->mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT_EQUAL(tArticle->mSystemConductance, tArticle->mAdmittanceMatrix[4]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp,   tArticle->mMembraneDeltaP[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mMembraneFlowRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mSourceVector[0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedW,   tArticle->mSourceVector[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedW,   tArticle->mSourceVector[2],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,    tArticle->mPhaseChangeHeat,   FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the step method for gas thru-flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::testStepGas()
{
    std::cout << "\n UtGunnsFluidSelectiveMembrane .... 008: testStepGas ...................";

    /// - Initialize default test article with nominal initialization data.
    tPort0 = 2;
    tPort1 = 3;
    tPort2 = 4;
    tConfigData->mInternalType = FluidProperties::GUNNS_H2O;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// - Set up last-pass potential vector and flows for forward flow.
    double p0 = 300.0;
    double p1 = 290.0;
    double p2 =   0.1;
    tNodes[tPort0].setPotential(p0);
    tNodes[tPort1].setPotential(p1);
    tNodes[tPort2].setPotential(p2);
    tNodes[tPort0].updateMass();
    tNodes[tPort1].updateMass();
    tNodes[tPort2].updateMass();
    tArticle->mPotentialVector[0] = p0;
    tArticle->mPotentialVector[1] = p1;
    tArticle->mPotentialVector[2] = p2;
    tArticle->mFlowRate           = 1.0;
    tArticle->mFlux               = 1.0 / tNodes[tPort0].getContent()->getMWeight();

    tArticle->step(tTimeStep);

    /// @test Conductance and admittance matrix.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMaxConductance, tArticle->mEffectiveConductance, DBL_EPSILON);
    CPPUNIT_ASSERT(0.0 < tArticle->mSystemConductance);
    CPPUNIT_ASSERT(tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT_EQUAL(tArticle->mSystemConductance, tArticle->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT_EQUAL(tArticle->mSystemConductance,-tArticle->mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT_EQUAL(tArticle->mSystemConductance,-tArticle->mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT_EQUAL(tArticle->mSystemConductance, tArticle->mAdmittanceMatrix[4]);

    /// @test Absorption pressures, rate, phase change and source vector.
    double expectedPpSource = p0 * tNodes[tPort0].getContent()->getMoleFraction(FluidProperties::GUNNS_H2O);
    double expectedPpVent   = p2 * tNodes[tPort2].getContent()->getMoleFraction(FluidProperties::GUNNS_H2O);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPpSource, tArticle->mInternalPartialP[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPpVent,   tArticle->mExternalPartialP,    DBL_EPSILON);

    double expectedPsatSource = tFluidProperties->getProperties(FluidProperties::GUNNS_H2O)->
                                getSaturationPressure(tNodes[tPort0].getContent()->getTemperature());
    double expectedPsatVent   = tFluidProperties->getProperties(FluidProperties::GUNNS_H2O)->
                                getSaturationPressure(tNodes[tPort2].getContent()->getTemperature());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPsatSource, tArticle->mInternalSaturationP, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPsatVent,   tArticle->mExternalSaturationP, DBL_EPSILON);
    CPPUNIT_ASSERT(!tArticle->mInternalSaturated);
    CPPUNIT_ASSERT(!tArticle->mExternalSaturated);
    CPPUNIT_ASSERT(!tArticle->mPhaseChange);

    double expectedDp   = expectedPpSource - expectedPpVent;
    double expectedMdot = expectedDp * tAbsorptionCoeff;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp,   tArticle->mMembraneDeltaP[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mMembraneFlowRate,  DBL_EPSILON);

    double expectedW    = expectedMdot / 18.0153;   // MW of water
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedW,   tArticle->mSourceVector[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mSourceVector[1],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,    tArticle->mSourceVector[2],   DBL_EPSILON);

    double expectedQ    = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,    tArticle->mPhaseChangeHeat,   DBL_EPSILON);

    /// - Configure the vent to be super-saturated and test reverse membrane flow, with membrane
    ///   degrade malf.
    p2 = 600.0;
    tNodes[tPort2].setPotential(p2);
    tNodes[tPort2].updateMass();
    tArticle->mPotentialVector[2] = p2;
    tArticle->setMalfMembraneDegrade(true, 0.5);

    tArticle->step(tTimeStep);

    expectedPpSource = p1 * tNodes[tPort1].getContent()->getMoleFraction(FluidProperties::GUNNS_H2O);
    expectedPpVent   = p2 * tNodes[tPort2].getContent()->getMoleFraction(FluidProperties::GUNNS_H2O);
    expectedPsatVent = tFluidProperties->getProperties(FluidProperties::GUNNS_H2O)->
                       getSaturationPressure(tNodes[tPort2].getContent()->getTemperature());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPpVent,     tArticle->mExternalPartialP,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPsatVent,   tArticle->mExternalSaturationP,   DBL_EPSILON);
    CPPUNIT_ASSERT(!tArticle->mInternalSaturated);
    CPPUNIT_ASSERT( tArticle->mExternalSaturated);
    CPPUNIT_ASSERT( tArticle->mPhaseChange);

    expectedDp   = expectedPpSource - expectedPpVent;
    expectedMdot = expectedDp * tAbsorptionCoeff * 0.5;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDp,   tArticle->mMembraneDeltaP[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mMembraneFlowRate,  DBL_EPSILON);

    expectedW    = expectedMdot / 18.0153;   // MW of water
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tArticle->mSourceVector[0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedW,   tArticle->mSourceVector[1],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedW,   tArticle->mSourceVector[2],   DBL_EPSILON);

    expectedQ    = -expectedMdot * 1000.0
                 * tFluidProperties->getProperties(FluidProperties::GUNNS_H2O)->
                   getHeatOfVaporization(tNodes[tPort2].getContent()->getTemperature());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedQ,    tArticle->mPhaseChangeHeat,   DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with liquid flow-thru.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::testComputeFlows()
{
    std::cout << "\n UtGunnsFluidSelectiveMembrane .... 009: testComputeFlows ..............";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// - Set up last-pass potential vector and flows for forward flow and liquid evaporation thru
    ///   the membrane.
    double p0 = 300.0;
    double p1 = 290.0;
    double p2 = 0.1;
    tNodes[tPort0].setPotential(p0);
    tNodes[tPort1].setPotential(p1);
    tNodes[tPort2].setPotential(p2);
    tNodes[tPort0].updateMass();
    tNodes[tPort1].updateMass();
    tNodes[tPort2].updateMass();
    tArticle->mPotentialVector[0] = p0;
    tArticle->mPotentialVector[1] = p1;
    tArticle->mPotentialVector[2] = p2;
    tArticle->mFlowRate           = 1.0;
    tArticle->mFlux               = 1.0 / 18.0153;  // MW of water
    tNodes[tPort0].resetFlows();
    tNodes[tPort1].resetFlows();
    tNodes[tPort2].resetFlows();

    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

    const FluidProperties* extProps     = tNodes[2].getOutflow()->getProperties(tExternalType);
    double tempMembraneFlux             = tArticle->mMembraneFlowRate / extProps->getMWeight();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempMembraneFlux, tNodes[0].getScheduledOutflux() - tArticle->mFlux,DBL_EPSILON);

    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[2]);

    tArticle->transportFlows(tTimeStep);

    /// @test Flow-thru and related output.
    double expectedDrop    = p0 - p1;
    double expectedFlow    = tArticle->mFlux * tNodes[tPort0].getOutflow()->getMWeight();
    double expectedUpFlow  = expectedFlow + tArticle->mMembraneFlowRate;
    double expectedVolFlow = expectedFlow / tNodes[tPort0].getOutflow()->getDensity();
    double expectedPower   = -expectedDrop * expectedVolFlow * 1000.0;  // Pa/kPa
    CPPUNIT_ASSERT(0 < tArticle->mFlux);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDrop,     tArticle->mPotentialDrop,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlow,     tArticle->mFlowRate,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolFlow,  tArticle->mVolFlowRate,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,    tArticle->mPower,                DBL_EPSILON);

    /// @test Flow transport between the nodes.
    tNodes[tPort1].integrateFlows(tTimeStep);
    double expectedDownQ    = tNodes[tPort0].getOutflow()->getSpecificEnthalpy() * expectedFlow
                            + tArticle->mPhaseChangeHeat;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort0].getInflux(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedUpFlow,   tNodes[tPort0].getOutflux(),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlow,     tNodes[tPort1].getInflux(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort1].getOutflux(),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mMembraneFlowRate,
                                                   tNodes[tPort2].getInflux(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort2].getOutflux(),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDownQ,    tNodes[tPort1].getNetHeatFlux(), DBL_EPSILON);

    // @test None 100% fluid in downstream (1) node with forward flow
    fractions[0] = 0.0;
    fractions[1] = 0.5;
    fractions[2] = 0.5;

    tNodes[1].getContent()->setMassAndMassFractions(1.0, fractions);

    tArticle->mFlux = 1.0;
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[1]);

    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 1.0;
    tNodes[1].getContent()->setMassAndMassFractions(1.0, fractions);

    // @test Not 100% fluid in upstream (0) node with forward bulk flow, forward membrane flow
    fractions[0] = 0.0;
    fractions[1] = 0.5;
    fractions[2] = 0.5;
    tNodes[0].getContent()->setMassAndMassFractions(1.0, fractions);

    tArticle->mFlux = 1.0;
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[1]);

    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 1.0;
    tNodes[0].getContent()->setMassAndMassFractions(1.0, fractions);

    /// - Set up for reverse flow-thru.
    p0 = 290.0;
    p1 = 300.0;
    tNodes[tPort0].setPotential(p0);
    tNodes[tPort1].setPotential(p1);
    tNodes[tPort0].updateMass();
    tNodes[tPort1].updateMass();
    tArticle->mPotentialVector[0] = p0;
    tArticle->mPotentialVector[1] = p1;
    tArticle->mFlowRate           = -1.0;
    tArticle->mFlux               = -1.0 / 18.0153;  // MW of water
    tNodes[tPort0].resetFlows();
    tNodes[tPort1].resetFlows();
    tNodes[tPort2].resetFlows();
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

    tempMembraneFlux = tArticle->mMembraneFlowRate / extProps->getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempMembraneFlux,  tNodes[1].getScheduledOutflux() + tArticle->mFlux,DBL_EPSILON);

    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[2]);

    tArticle->transportFlows(tTimeStep);

    /// @test Flow-thru and related output.
    expectedDrop     = p0 - p1;
    expectedFlow     = tArticle->mFlux * tNodes[tPort1].getOutflow()->getMWeight();
    expectedUpFlow   = -expectedFlow + tArticle->mMembraneFlowRate;
    expectedVolFlow  = expectedFlow / tNodes[tPort1].getOutflow()->getDensity();
    expectedPower    = -expectedDrop * expectedVolFlow * 1000.0;  // Pa/kPa
    tempMembraneFlux = tArticle->mMembraneFlowRate / extProps->getMWeight();
    CPPUNIT_ASSERT(0 > tArticle->mFlux);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDrop,     tArticle->mPotentialDrop,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlow,     tArticle->mFlowRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolFlow,  tArticle->mVolFlowRate,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,    tArticle->mPower,            FLT_EPSILON);

    /// @test Flow transport between the nodes.
    tNodes[tPort0].integrateFlows(tTimeStep);
    expectedDownQ    = tNodes[tPort1].getOutflow()->getSpecificEnthalpy() * -expectedFlow
                     + tArticle->mPhaseChangeHeat;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedFlow,    tNodes[tPort0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort1].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedUpFlow,   tNodes[tPort1].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mMembraneFlowRate,
                                                   tNodes[tPort2].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort2].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDownQ,    tNodes[tPort0].getNetHeatFlux(), DBL_EPSILON);

    // @test None 100% fluid in downstream (0) node with reverse flow
    fractions[0] = 0.0;
    fractions[1] = 0.5;
    fractions[2] = 0.5;

    tNodes[0].getContent()->setMassAndMassFractions(1.0, fractions);

    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[1]);

    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 1.0;
    tNodes[0].getContent()->setMassAndMassFractions(1.0, fractions);

    // @test test case with reverse membrane flow where vent node (2) has 100% fluid type
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);
    tArticle->mPotentialVector[0]  =  0.0;
    tArticle->mPotentialVector[1]  = 10.0;
    tArticle->mAdmittanceMatrix[0] =  1.0;
    tArticle->mMembraneFlowRate    = -1.0;

    fractions[0] = 0.0;
    fractions[1] = 1.0;
    fractions[2] = 0.0;
    fractions[3] = 0.0;

    tNodes[2].getContent()->setMassAndMassFractions(1.0, fractions);
    tNodes[2].resetFlows();

    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(0.0 > tArticle->mFlux);

    tempMembraneFlux = tArticle->mMembraneFlowRate / extProps->getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-tempMembraneFlux, tNodes[2].getScheduledOutflux(), DBL_EPSILON);

    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[2]);

    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-tArticle->mMembraneFlowRate, tNodes[2].getOutflux(), DBL_EPSILON);

    // @test Ensure correct outflow temp update for vent node (2)
    tArticle->mFlux = 1.0;
    tArticle->mMembraneFlowRate = 1.0;
    fractions[0] = 0.0;
    fractions[1] = 0.5;
    fractions[2] = 0.5;

    tNodes[1].getContent()->setMassAndMassFractions(1.0, fractions);
    tNodes[1].resetFlows();
    double tempInternalTemp = tNodes[0].getOutflow()->getTemperature();

    tArticle->transportFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tempInternalTemp, tArticle->mInternalMembraneFluid->getTemperature(), DBL_EPSILON);

    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 1.0;
    tNodes[1].getContent()->setMassAndMassFractions(1.0, fractions);
    tNodes[1].resetFlows();

    /// @test no flow with a positive vent flow
    tArticle->mPotentialVector[0]  = 0.0;
    tArticle->mPotentialVector[1]  = 0.0;
    tArticle->mFlux                = 0.0;
    tArticle->mMembraneFlowRate    = 1.0;

    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(tArticle->determineSourcePort(tArticle->mFlux,0,1) == 0.0);

    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[1]);

    /// @test delta partial pressure for different downstream and upstream signs
    tArticle->mPotentialVector[0]  = 1.5;
    tArticle->mPotentialVector[1]  = 0.5;
    tArticle->mPotentialVector[2]  = 1.0;
    tArticle->mFlux                = 1.0;
    tArticle->mMembraneFlowRate    = 1.0;

    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    CPPUNIT_ASSERT(tArticle->mMembraneDeltaP[0] * tArticle->mMembraneDeltaP[1] < 0.0);
    CPPUNIT_ASSERT(tArticle->mMembraneFlowRate == 0.0);

    /// @test Volumetric flow rate when the source density is zero
    tArticle->mPotentialVector[tPort0] = -0.6;
    tNodes[tPort1].resetContentState();
    tNodes[tPort1].resetFlows();
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tArticle->mVolFlowRate);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows with gas flow-thru.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::testComputeFlowsGas()
{
    std::cout << "\n UtGunnsFluidSelectiveMembrane .... 010: testComputeFlowsGas ...........";

    /// - Initialize default test article with nominal initialization data.
    tPort0 = 2;
    tPort1 = 3;
    tPort2 = 4;
    tConfigData->mInternalType = FluidProperties::GUNNS_H2O;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// - Set up last-pass potential vector and flows for forward flow, with the vent super-
    ///   saturated for reverse membrane flow.
    double p0 = 300.0;
    double p1 = 290.0;
    double p2 = 600.0;
    tNodes[tPort0].setPotential(p0);
    tNodes[tPort1].setPotential(p1);
    tNodes[tPort2].setPotential(p2);
    tNodes[tPort0].updateMass();
    tNodes[tPort1].updateMass();
    tNodes[tPort2].updateMass();
    tArticle->mPotentialVector[0] = p0;
    tArticle->mPotentialVector[1] = p1;
    tArticle->mPotentialVector[2] = p2;
    tArticle->mFlowRate           = 1.0;
    tArticle->mFlux               = 1.0 / tNodes[tPort0].getOutflow()->getMWeight();

    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    /// @test Flow-thru and related output.
    double expectedDrop    = p0 - p1;
    double expectedFlow    = tArticle->mFlux * tNodes[tPort0].getOutflow()->getMWeight();
    double expectedVolFlow = expectedFlow / tNodes[tPort0].getOutflow()->getDensity();
    double expectedPower   = -expectedDrop * expectedVolFlow * 1000.0;  // Pa/kPa
    CPPUNIT_ASSERT(0 < tArticle->mFlux);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDrop,     tArticle->mPotentialDrop,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlow,     tArticle->mFlowRate,             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVolFlow,  tArticle->mVolFlowRate,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,    tArticle->mPower,                FLT_EPSILON);

    /// @test Flow transport between the nodes.
    tNodes[tPort1].integrateFlows(tTimeStep);
    double expectedDownFlow = expectedFlow - tArticle->mMembraneFlowRate;
    double ventH = tFluidProperties->getProperties(FluidProperties::GUNNS_H2O)->getSpecificEnthalpy(
                                                   tNodes[tPort2].getOutflow()->getTemperature());
    double expectedDownQ    = tNodes[tPort0].getOutflow()->getSpecificEnthalpy() * expectedFlow
                            - ventH * tArticle->mMembraneFlowRate;
    CPPUNIT_ASSERT(0.0 > tArticle->mMembraneFlowRate);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort0].getInflux(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlow,     tNodes[tPort0].getOutflux(),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDownFlow, tNodes[tPort1].getInflux(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort1].getOutflux(),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mMembraneFlowRate,
                                                   tNodes[tPort2].getInflux(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort2].getOutflux(),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDownQ,    tNodes[tPort1].getNetHeatFlux(), DBL_EPSILON);

    /// - Set up for zero membrane flow rate.
    tArticle->mMembraneFlowRate = 0.0;
    tNodes[tPort0].resetFlows();
    tNodes[tPort1].resetFlows();
    tNodes[tPort2].resetFlows();

    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    /// @test Flow transport between the nodes.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlow,     tNodes[tPort0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlow,     tNodes[tPort1].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort1].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort2].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort2].getOutflux(), DBL_EPSILON);

    /// - Set up for zero flow-thru rate.
    p0 = p1;
    tNodes[tPort0].setPotential(p0);
    tNodes[tPort0].updateMass();
    tArticle->mPotentialVector[0] = p0;
    tNodes[tPort0].resetFlows();
    tNodes[tPort1].resetFlows();
    tNodes[tPort2].resetFlows();

    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    /// @test Flow-thru and related output.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->mFlux,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->mPotentialDrop,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->mFlowRate,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->mVolFlowRate,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->mPower,            FLT_EPSILON);

    /// @test Flow transport between the nodes.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort0].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort1].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort1].getOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort2].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNodes[tPort2].getOutflux(), DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test setter & getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::testAccessMethods()
{
    std::cout << "\n UtGunnsFluidSelectiveMembrane .... 011: testAccessMethods .............";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// - Test Access Methods
    tArticle->setMalfMembraneDegrade(true, 1.0);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mMalfMembraneDegradeFlag);
    CPPUNIT_ASSERT_EQUAL(1.0,   tArticle->mMalfMembraneDegradeValue);

    tArticle->setMalfMembraneDegrade();
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mMalfMembraneDegradeFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfMembraneDegradeValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test port mapping and validity checks for liquid flow-thru links.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSelectiveMembrane::testPortMapping()
{
    std::cout << "\n UtGunnsFluidSelectiveMembrane .... 012: testPortMapping ...............";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// @test An invalid liquid port 0 move to a gas node.
    tArticle->mUserPortSelect     = 0;
    tArticle->mUserPortSelectNode = 3;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::FAILED,          tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(tPort0,                          tArticle->mNodeMap[0]);

    /// @test An invalid liquid port 1 move to a gas node.
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSelectNode = 3;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::FAILED,          tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(tPort1,                          tArticle->mNodeMap[1]);

    /// @test A valid port liquid port 1 move to Ground.
    tArticle->mUserPortSelect     = 1;
    tArticle->mUserPortSelectNode = 5;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::READY,           tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(5,                               tArticle->mNodeMap[1]);

    /// @test An invalid gas port 2 move to a liquid node.
    tArticle->mUserPortSelect     = 2;
    tArticle->mUserPortSelectNode = 1;
    tArticle->mUserPortSetControl = GunnsBasicLink::EXECUTE;

    tArticle->step(tTimeStep);

    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::FAILED,          tArticle->mUserPortSetControl);
    CPPUNIT_ASSERT_EQUAL(tPort0,                          tArticle->mNodeMap[0]);

    std::cout << "... Pass";
    std::cout << "\n -----------------------------------------------------------------------------";
}
