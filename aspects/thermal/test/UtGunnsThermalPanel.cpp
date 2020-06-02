/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/thermal/GunnsThermalPanel.o))
***************************************************************************************************/
#include "UtGunnsThermalPanel.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <iostream>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermalPanel class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalPanel::UtGunnsThermalPanel()
    :
    tName(),
    tArticle1(),
    tArticle3(),
    tConfigData1(),
    tConfigData3(),
    tInputData(),
    tFlux(),
    tDefaultDemand(),
    tTuningScalar(),
    tIncidentHeatFluxPerArea(),
    tAbsorptivity(),
    tSurfaceArea(),
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
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsThermalPanel class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalPanel::~UtGunnsThermalPanel()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPanel::tearDown()
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
void UtGunnsThermalPanel::setUp()
{
    /// - Declare the GunnsThermalPanel specific test data.
    tName = "Test GunnsThermalPanel";
    tTol = 1.0e-08;
    tTimeStep = 0.1;

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
    tFluxDistributionFractions3.push_back(0.4);
    tFluxDistributionFractions3.push_back(0.1);
    tFluxDistributionFractions3.push_back(0.5);

    /// - Create two vectors of port numbers.
    tPorts1.clear();
    tPorts1.push_back(0);
    tPorts3.clear();
    tPorts3.push_back(2);
    tPorts3.push_back(1);
    tPorts3.push_back(3);

    /// - Declare base-class test data
    tTuningScalar  =  0.91;
    tDefaultDemand = 35.0;

    /// - Declare GunnsThermalPanel specific test data
    tIncidentHeatFluxPerArea = 21.1;   // (W/m2)
    tAbsorptivity =  0.293; // (--)
    tSurfaceArea  =  2.5;   // (m2)

    /// - Set test flux value.
    tFlux = tIncidentHeatFluxPerArea * tAbsorptivity * tSurfaceArea * tTuningScalar;

    /// - Define single-port configuration data.
    tConfigData1 = new GunnsThermalPanelConfigData(tName, &tNodeList, tTuningScalar, &tFluxDistributionFractions1, tAbsorptivity, tSurfaceArea);

    /// - Define triple-port configuration data.
    tConfigData3 = new GunnsThermalPanelConfigData(tName, &tNodeList, tTuningScalar, &tFluxDistributionFractions3, tAbsorptivity, tSurfaceArea);

    /// - Define nominal input data.
    tInputData = new GunnsThermalPanelInputData(tMalfBlockageFlag, tMalfBlockageValue, tDefaultDemand);

    /// - Create the test articles.
    tArticle1 = new FriendlyGunnsThermalPanel;
    tArticle3 = new FriendlyGunnsThermalPanel;

    /// - Initialize the test articles.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->initialize(*tConfigData1, *tInputData, tLinks, &tPorts1) );
    CPPUNIT_ASSERT_NO_THROW( tArticle3->initialize(*tConfigData3, *tInputData, tLinks, &tPorts3) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPanel::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsThermalPanel ... 01: testConfig ..........................";

    /// @test  Nominal config construction of single-port source
    CPPUNIT_ASSERT(tName == tConfigData1->mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, tConfigData1->cTuningScalar);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity, tConfigData1->cAbsorptivity);
    CPPUNIT_ASSERT_EQUAL(tSurfaceArea,  tConfigData1->cSurfaceArea);
    CPPUNIT_ASSERT_EQUAL(1.0, tConfigData1->cFluxDistributionFractions.at(0) );

    /// @test  Nominal config construction of triple-port source
    CPPUNIT_ASSERT(tName == tConfigData3->mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, tConfigData3->cTuningScalar);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity, tConfigData3->cAbsorptivity);
    CPPUNIT_ASSERT_EQUAL(tSurfaceArea,  tConfigData3->cSurfaceArea);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(0), tConfigData3->cFluxDistributionFractions.at(0), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(1), tConfigData3->cFluxDistributionFractions.at(1), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(2), tConfigData3->cFluxDistributionFractions.at(2), tTol);

    /// @test  Check default config construction
    GunnsThermalPanelConfigData defaultConfig;
    CPPUNIT_ASSERT("unnamed panel" == defaultConfig.mName);
    CPPUNIT_ASSERT_EQUAL(1.0, defaultConfig.cTuningScalar);
    CPPUNIT_ASSERT_EQUAL(0.5, defaultConfig.cAbsorptivity);
    CPPUNIT_ASSERT_EQUAL(1.0, defaultConfig.cSurfaceArea);
    CPPUNIT_ASSERT( defaultConfig.cFluxDistributionFractions.empty() );

    /// @test  Check copy config construction of single-port source
    GunnsThermalPanelConfigData copyConfig1(*tConfigData1);
    CPPUNIT_ASSERT(tName  == copyConfig1.mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, copyConfig1.cTuningScalar);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity, copyConfig1.cAbsorptivity);
    CPPUNIT_ASSERT_EQUAL(tSurfaceArea, copyConfig1.cSurfaceArea);
    CPPUNIT_ASSERT_EQUAL(1.0, copyConfig1.cFluxDistributionFractions.at(0));

    /// @test  Check copy config construction of triple-port source
    GunnsThermalPanelConfigData copyConfig3(*tConfigData3);
    CPPUNIT_ASSERT(tName == copyConfig3.mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, copyConfig3.cTuningScalar);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity, copyConfig3.cAbsorptivity);
    CPPUNIT_ASSERT_EQUAL(tSurfaceArea, copyConfig3.cSurfaceArea);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(0), copyConfig3.cFluxDistributionFractions.at(0), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(1), copyConfig3.cFluxDistributionFractions.at(1), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(2), copyConfig3.cFluxDistributionFractions.at(2), tTol);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPanel::testInput()
{
    std::cout << "\n UtGunnsThermalPanel ... 02: testInput ...........................";

    /// @test  Check nominal input construction
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tDefaultDemand     == tInputData->iDemandedFlux);

    /// @test  Check default input construction
    GunnsThermalPanelInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iDemandedFlux);

    /// @test  Check copy input construction
    GunnsThermalPanelInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(tInputData->iDemandedFlux, copyInput.iDemandedFlux);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPanel::testDefaultConstruction()
{
    std::cout << "\n UtGunnsThermalPanel ... 03: testDefaultConstruction .............";

    /// - create non-initialized, default test article
    FriendlyGunnsThermalPanel article;

    /// @test  default-constructed article
    CPPUNIT_ASSERT_EQUAL(0.0, article.mDemandedFlux);
    CPPUNIT_ASSERT_EQUAL(1.0, article.mTuningScalar);
    CPPUNIT_ASSERT(0 == article.mFluxDistributionFractions);
    CPPUNIT_ASSERT(0.0 == article.mIncidentHeatFluxPerArea);
    CPPUNIT_ASSERT(0.0 == article.mAbsorptivity);
    CPPUNIT_ASSERT(0.0 == article.mSurfaceArea);

    /// @test  Init flag
    CPPUNIT_ASSERT(!article.mInitFlag);

    /// - check new/delete for code coverage
    GunnsThermalPanel* pArticle = new GunnsThermalPanel();
    delete pArticle;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPanel::testNominalInitialization()
{
    std::cout << "\n UtGunnsThermalPanel ... 04: testNominalInitialization ...........";

    /// @test  Number of ports initialization
    CPPUNIT_ASSERT_EQUAL(1, tArticle1->mNumPorts);
    CPPUNIT_ASSERT_EQUAL(3, tArticle3->mNumPorts);

    /// @test  Demanded flux initialization
    CPPUNIT_ASSERT_EQUAL(tDefaultDemand, tArticle1->mDemandedFlux);
    CPPUNIT_ASSERT_EQUAL(tDefaultDemand, tArticle3->mDemandedFlux);

    /// @test  Tuning scalar initialization
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, tArticle1->mTuningScalar);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, tArticle3->mTuningScalar);

    /// @test  Absorptivity initialization
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity, tArticle1->mAbsorptivity);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity, tArticle3->mAbsorptivity);

    /// @test  SurfaceArea initialization
    CPPUNIT_ASSERT_EQUAL(tSurfaceArea, tArticle1->mSurfaceArea);
    CPPUNIT_ASSERT_EQUAL(tSurfaceArea, tArticle3->mSurfaceArea);

    /// @test  IncidentHeatFluxPerArea should be initialized to a default value derived from the
    ///        other configuration data.
    tIncidentHeatFluxPerArea = tDefaultDemand / (tAbsorptivity * tSurfaceArea);
    CPPUNIT_ASSERT_EQUAL(tIncidentHeatFluxPerArea, tArticle1->mIncidentHeatFluxPerArea);
    CPPUNIT_ASSERT_EQUAL(tIncidentHeatFluxPerArea, tArticle3->mIncidentHeatFluxPerArea);

    /// @test  Flux-distribution-fractions array initialization
    CPPUNIT_ASSERT_EQUAL(tFluxDistributionFractions1.at(0), tArticle1->mFluxDistributionFractions[0]);
    CPPUNIT_ASSERT_EQUAL(tFluxDistributionFractions3.at(0), tArticle3->mFluxDistributionFractions[0]);
    CPPUNIT_ASSERT_EQUAL(tFluxDistributionFractions3.at(1), tArticle3->mFluxDistributionFractions[1]);
    CPPUNIT_ASSERT_EQUAL(tFluxDistributionFractions3.at(2), tArticle3->mFluxDistributionFractions[2]);

    /// @test  Init flag
    CPPUNIT_ASSERT(tArticle1->mInitFlag);
    CPPUNIT_ASSERT(tArticle3->mInitFlag);

    /// - Construct an un-initialized test article.
    FriendlyGunnsThermalPanel article;

    /// @test  Shouldn't initialize() with a single-member ports array and a triple-member flux-distribution vector
    CPPUNIT_ASSERT_THROW( article.initialize(*tConfigData3, *tInputData, tLinks, &tPorts1), TsInitializationException );
    /// @test  Shouldn't initialize() with a triple-member ports array and a single-member flux-distribution vector
    CPPUNIT_ASSERT_THROW( article.initialize(*tConfigData1, *tInputData, tLinks, &tPorts3), TsInitializationException );

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that bad data produces the appropriate exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPanel::testValidation()
{
    std::cout << "\n UtGunnsThermalPanel ... 05: testValidation ......................";

    /// @test  No exception should be thrown on good config data.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->validate(*tConfigData1) );

    /// @test  range of absorptivity validation
    tConfigData1->cAbsorptivity = -1;
    CPPUNIT_ASSERT_THROW(tArticle1->validate(*tConfigData1), TsInitializationException);
    tConfigData1->cAbsorptivity = 2.0;
    CPPUNIT_ASSERT_THROW(tArticle1->validate(*tConfigData1), TsInitializationException);

    /// - Reset cAbsorptivity and test again.
    tConfigData1->cAbsorptivity = tAbsorptivity;

    /// @test range of surface area validation
    tConfigData1->cSurfaceArea = -0.256;
    CPPUNIT_ASSERT_THROW(tArticle1->validate(*tConfigData1), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for updateState().
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPanel::testUpdateState()
{
    std::cout << "\n UtGunnsThermalPanel ... 06: testUpdateState .....................";

    /// - Set mIncidentHeatFluxPerArea manually, to simulate it being set by the SimBus.
    tArticle1->mIncidentHeatFluxPerArea = tIncidentHeatFluxPerArea;
    tArticle3->mIncidentHeatFluxPerArea = tIncidentHeatFluxPerArea;

    /// - Call updateState() method.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->updateState(tTimeStep) );
    CPPUNIT_ASSERT_NO_THROW( tArticle3->updateState(tTimeStep) );

    /// - After updateState(), mDemandedFlux should be set, but not affected by any blockage value.
    ///   Blockage is not applied until step(), in the parent-class.
    /// @test  mDemandedFlux was appropriately set
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mSourceFlux not correct",
            tAbsorptivity * tSurfaceArea * tIncidentHeatFluxPerArea,
            tArticle1->mDemandedFlux, tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mSourceFlux not correct",
            tAbsorptivity * tSurfaceArea * tIncidentHeatFluxPerArea,
            tArticle3->mDemandedFlux, tTol);

    /// @test  mFlux should not be set until step()
    CPPUNIT_ASSERT_MESSAGE("mFlux nonzero before step()", 0 == tArticle1->mFlux);
    CPPUNIT_ASSERT_MESSAGE("mFlux nonzero before step()", 0 == tArticle3->mFlux);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalPanel::testStep()
{
    std::cout << "\n UtGunnsThermalPanel ... 07: testStep ............................";

    /// - Manually set mIncidentHeatFluxPerArea, to simulate it being set by the SimBus.
    tArticle1->mIncidentHeatFluxPerArea = tIncidentHeatFluxPerArea;

    /// - Step the article.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->step(tTimeStep) );

    /// @test  mIncidentHeatFluxPerArea should be unchanged.
    CPPUNIT_ASSERT_EQUAL(tIncidentHeatFluxPerArea, tArticle1->mIncidentHeatFluxPerArea);

    /// @test  mFlux should be scaled by the tuning scalar only.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux, tArticle1->mFlux, tTol);

    /// - Give the source a blockage malfunction.
    tArticle1->mMalfBlockageFlag = true;
    tArticle1->mMalfBlockageValue = tMalfBlockageValue;

    /// - Step the article again, this time with blockage active.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->step(tTimeStep) );

    /// @test  mIncidentHeatFluxPerArea should be unchanged.
    CPPUNIT_ASSERT_EQUAL(tIncidentHeatFluxPerArea, tArticle1->mIncidentHeatFluxPerArea);

    /// @test  mFlux should be scaled by the tuning scalar and affected by the blockage.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux * (1.0-tMalfBlockageValue), tArticle1->mFlux, tTol);

    /// - Give the source an override malfunction.
    tArticle1->mMalfFluxOverrideFlag = true;
    tArticle1->mMalfFluxOverrideValue = tMalfFluxOverrideValue;

    /// - Step the article again, this time with override active.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->step(tTimeStep) );

    /// @test  mFlux should be overriden.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMalfFluxOverrideValue, tArticle1->mFlux, tTol);

    std::cout << "... Pass";
}
