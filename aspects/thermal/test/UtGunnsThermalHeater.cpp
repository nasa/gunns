/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/thermal/GunnsThermalHeater.o))
***************************************************************************************************/
#include "UtGunnsThermalHeater.hh"
#include <iostream>
#include "software/exceptions/TsInitializationException.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermalHeater class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalHeater::UtGunnsThermalHeater()
    :
    tName(),
    tArticle1(),
    tArticle3(),
    tConfigData1(),
    tConfigData3(),
    tInputData(),
    tPower(),
    tDefaultDemand(),
    tTuningScalar(),
    tFluxDistributionFractions1(),
    tFluxDistributionFractions3(),
    tPorts1(),
    tPorts3(),
    tMalfFluxOverrideFlag(),
    tMalfFluxOverrideValue(),
    tMalfBlockageFlag(),
    tMalfBlockageValue(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tTimeStep(),
    tTol()
{
    // do nothing
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsThermalHeater class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalHeater::~UtGunnsThermalHeater()
{
    // do nothing
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalHeater::tearDown()
{
    /// - Deletes for news in setUp().
    delete tArticle3;
    delete tArticle1;
    delete tInputData;
    delete tConfigData3;
    delete tConfigData1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalHeater::setUp()
{
    /// - Declare the GunnsThermalHeater specific test data.
    tName  = "Test GunnsThermalHeater";
    tTol = 1.0e-08;
    tTimeStep  = 0.1;

    /// - Declare the Gunns link test data.
    tNodeList.mNumNodes = 6;
    tNodeList.mNodes    = tNodes;

    /// - Declare the malfunction data.
    tMalfFluxOverrideFlag  = false;
    tMalfFluxOverrideValue = 655.0;
    tMalfBlockageFlag      = false;
    tMalfBlockageValue     = 0.4;

    /// - Set two vectors of flux-to-port distribution fractions.
    tFluxDistributionFractions1.clear();
    tFluxDistributionFractions1.push_back(1.0);
    tFluxDistributionFractions3.clear();
    tFluxDistributionFractions3.push_back(0.5);
    tFluxDistributionFractions3.push_back(0.2);
    tFluxDistributionFractions3.push_back(0.3);

    /// - Create two vectors of port numbers.
    tPorts1.clear();
    tPorts1.push_back(0);
    tPorts3.clear();
    tPorts3.push_back(1);
    tPorts3.push_back(2);
    tPorts3.push_back(3);

    /// - Set Heater power rating.
    tPower = 112;
    /// - Set Heater tuning scalar.
    tTuningScalar = 0.93;
    /// - Set a default demand.
    tDefaultDemand = 30.0;
    /// - Set a blockage malfunction fractional value (0-1)
    tMalfBlockageValue = 0.45;

    /// - Define single-port configuration data.
    tConfigData1 = new GunnsThermalHeaterConfigData(tName, &tNodeList, tTuningScalar, &tFluxDistributionFractions1);

    /// - Define triple-port configuration data.
    tConfigData3 = new GunnsThermalHeaterConfigData(tName, &tNodeList, tTuningScalar, &tFluxDistributionFractions3);

    /// - Define nominal input data.
    tInputData = new GunnsThermalHeaterInputData(tMalfBlockageFlag, tMalfBlockageValue,
            tDefaultDemand);

    /// - Create the test articles.
    tArticle1 = new FriendlyGunnsThermalHeater;
    tArticle3 = new FriendlyGunnsThermalHeater;

    /// - Initialize the test articles.
    tArticle1->initialize(*tConfigData1, *tInputData, tLinks, &tPorts1);
    tArticle3->initialize(*tConfigData3, *tInputData, tLinks, &tPorts3);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalHeater::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsThermalHeater .. 01: testConfig ..........................";

    /// @test  Nominal config construction of single-port heater
    CPPUNIT_ASSERT(tName == tConfigData1->mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, tConfigData1->cTuningScalar);
    CPPUNIT_ASSERT_EQUAL(1.0, tConfigData1->cFluxDistributionFractions.at(0) );

    /// @test  Nominal config construction of triple-port heater
    CPPUNIT_ASSERT(tName == tConfigData3->mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, tConfigData3->cTuningScalar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(0), tConfigData3->cFluxDistributionFractions.at(0), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(1), tConfigData3->cFluxDistributionFractions.at(1), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(2), tConfigData3->cFluxDistributionFractions.at(2), tTol);

    /// @test  Check default config construction
    GunnsThermalHeaterConfigData defaultConfig;
    CPPUNIT_ASSERT("unnamed heater" == defaultConfig.mName);
    CPPUNIT_ASSERT_EQUAL(1.0, defaultConfig.cTuningScalar);
    CPPUNIT_ASSERT( defaultConfig.cFluxDistributionFractions.empty() );

    /// @test  Check copy config construction of single-port heater
    GunnsThermalHeaterConfigData copyConfig1(*tConfigData1);
    CPPUNIT_ASSERT(tName  == copyConfig1.mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, copyConfig1.cTuningScalar);
    CPPUNIT_ASSERT_EQUAL(1.0, copyConfig1.cFluxDistributionFractions.at(0));

    /// @test  Check copy config construction of triple-port heater
    GunnsThermalHeaterConfigData copyConfig3(*tConfigData3);
    CPPUNIT_ASSERT(tName == copyConfig3.mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, copyConfig3.cTuningScalar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(0), copyConfig3.cFluxDistributionFractions.at(0), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(1), copyConfig3.cFluxDistributionFractions.at(1), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(2), copyConfig3.cFluxDistributionFractions.at(2), tTol);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalHeater::testInput()
{
    std::cout << "\n UtGunnsThermalHeater .. 02: testInput ...........................";

    /// @test  Check nominal input construction
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tDefaultDemand     == tInputData->iDemandedFlux);

    /// @test  Check default input construction
    GunnsThermalHeaterInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iDemandedFlux);

    /// @test  Check copy input construction
    GunnsThermalHeaterInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(tInputData->iDemandedFlux, copyInput.iDemandedFlux);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalHeater::testDefaultConstruction()
{
    std::cout << "\n UtGunnsThermalHeater .. 03: testDefaultConstruction .............";

    /// - Construct an un-initialized test article.
    FriendlyGunnsThermalHeater article;

    /// @test  Default construction data
    CPPUNIT_ASSERT_EQUAL(0.0, article.mDemandedFlux);
    CPPUNIT_ASSERT_EQUAL(0.0, article.mPowerElectrical);
    CPPUNIT_ASSERT_EQUAL(1.0, article.mTuningScalar);
    CPPUNIT_ASSERT(0 == article.mFluxDistributionFractions);

    /// @test  Init flag
    CPPUNIT_ASSERT(!article.mInitFlag);

    /// @test  New/delete for code coverage
    GunnsThermalHeater* article2 = new GunnsThermalHeater();
    delete article2;

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that getters and setters are working correctly.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalHeater::testGettersAndSetters()
{
    std::cout << "\n UtGunnsThermalHeater .. 04: testGettersAndSetters ...............";

    /// @test  Default construction data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle1->mPowerElectrical, tTol);

    /// - Try the electrical power setter.
    tArticle1->setPowerElectrical(tPower);

    /// @test  Power value
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPower, tArticle1->mPowerElectrical, tTol);

    /// @test  mPowerElectrical getter
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPower, tArticle1->getPowerElectrical(), tTol);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization of a GunnsThermalHeater with no Card Name given.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalHeater::testInitialization()
{
    std::cout << "\n UtGunnsThermalHeater .. 05: testInitialization ..................";

    /// @test  Number of ports initialization
    CPPUNIT_ASSERT_EQUAL(1, tArticle1->mNumPorts);
    CPPUNIT_ASSERT_EQUAL(3, tArticle3->mNumPorts);

    /// @test  Demanded flux initialization
    CPPUNIT_ASSERT_EQUAL(tDefaultDemand, tArticle1->mDemandedFlux);
    CPPUNIT_ASSERT_EQUAL(tDefaultDemand, tArticle3->mDemandedFlux);

    /// @test  Tuning scalar initialization
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, tArticle1->mTuningScalar);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, tArticle3->mTuningScalar);

    /// @test  Flux-distribution-fractions array initialization
    CPPUNIT_ASSERT_EQUAL(tFluxDistributionFractions1.at(0), tArticle1->mFluxDistributionFractions[0]);
    CPPUNIT_ASSERT_EQUAL(tFluxDistributionFractions3.at(0), tArticle3->mFluxDistributionFractions[0]);
    CPPUNIT_ASSERT_EQUAL(tFluxDistributionFractions3.at(1), tArticle3->mFluxDistributionFractions[1]);
    CPPUNIT_ASSERT_EQUAL(tFluxDistributionFractions3.at(2), tArticle3->mFluxDistributionFractions[2]);

    /// @test  Init flag
    CPPUNIT_ASSERT(tArticle1->mInitFlag);
    CPPUNIT_ASSERT(tArticle3->mInitFlag);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for correct updateState() functionality.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalHeater::testUpdateState()
{
    std::cout << "\n UtGunnsThermalHeater .. 06: testUpdateState .....................";

    /// - Manually set mPowerElectrical to simulate it being set by the sim_bus.
    tArticle1->mPowerElectrical = tPower;

    /// - Update heater state.
    tArticle1->updateState(tTimeStep);

    /// @test  mPowerElectrical should be unchanged.
    CPPUNIT_ASSERT_EQUAL(tPower, tArticle1->mPowerElectrical);

    /// @test  mDemandedFlux should be equivalent to the electrical power
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPower, tArticle1->mDemandedFlux, tTol);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the step() method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalHeater::testStep()
{
    std::cout << "\n UtGunnsThermalHeater .. 07: testStep ............................";

    /// - Manually set mPowerElectrical to simulate it being set by the sim_bus.
    tArticle1->mPowerElectrical = tPower;

    /// - Step() heater.
    tArticle1->step(tTimeStep);

    /// @test  mPowerElectrical should be unchanged.
    CPPUNIT_ASSERT_EQUAL(tPower, tArticle1->mPowerElectrical);

    /// @test  mFlux should be scaled by the tuning scalar only.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPower * tArticle1->mTuningScalar, tArticle1->mFlux, tTol);

    /// - Give the heater a blockage malfunction.
    tArticle1->mPowerElectrical = tPower;
    tArticle1->mMalfBlockageFlag = true;
    tArticle1->mMalfBlockageValue = tMalfBlockageValue;

    /// - Step() heater again, this time with blockage active.
    tArticle1->step(tTimeStep);

    /// @test  mPowerElectrical should be unchanged.
    CPPUNIT_ASSERT_EQUAL(tPower, tArticle1->mPowerElectrical);

    /// @test  mFlux should be scaled by the tuning scalar and affected by the blockage.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPower * tArticle1->mTuningScalar * (1.0-tMalfBlockageValue),
                                 tArticle1->mFlux, tTol);

    /// - Give the heater an override malfunction.
    tArticle1->mMalfFluxOverrideFlag = true;
    tArticle1->mMalfFluxOverrideValue = tMalfFluxOverrideValue;

    /// - Step() heater again, this time with override active.
    tArticle1->step(tTimeStep);

    /// @test  mFlux should be overriden.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMalfFluxOverrideValue, tArticle1->mFlux, tTol);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that bad data produces the appropriate exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalHeater::testValidation()
{
    std::cout << "\n UtGunnsThermalHeater .. 08: testValidation ......................";

    // nothing to test

    std::cout << "... Pass";
}
