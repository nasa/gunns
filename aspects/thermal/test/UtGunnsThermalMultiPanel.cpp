/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/thermal/GunnsThermalMultiPanel.o))
***************************************************************************************************/
#include "UtGunnsThermalMultiPanel.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <iostream>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermalMultiPanel class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalMultiPanel::UtGunnsThermalMultiPanel()
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
    tIncidentFlux(),
    tViewScalar(),
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
    for (int i = 0; i < 5; ++i) {
        tIncidentFlux[i] = 0.0;
        tViewScalar[i]   = 0.0;
        tAbsorptivity[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsThermalMultiPanel class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalMultiPanel::~UtGunnsThermalMultiPanel()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalMultiPanel::tearDown()
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
void UtGunnsThermalMultiPanel::setUp()
{
    /// - Declare the GunnsThermalMultiPanel specific test data.
    tName = "Test GunnsThermalMultiPanel";
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

    /// - Declare GunnsThermalMultiPanel specific test data
    tIncidentFlux[0] = 21.1;   // (W/m2)
    tIncidentFlux[1] = 16.0;   // (W/m2)
    tIncidentFlux[2] =  5.3;   // (W/m2)
    tIncidentFlux[3] =  0.1;   // (W/m2)
    tIncidentFlux[4] =  4.2;   // (W/m2)
    tAbsorptivity[0] =  0.293; // (--)
    tAbsorptivity[1] =  1.0;   // (--)
    tAbsorptivity[2] =  0.1;   // (--)
    tAbsorptivity[3] =  0.5;   // (--)
    tAbsorptivity[4] =  0.9;   // (--)
    tViewScalar[0]   =  1.0;   // (--)
    tViewScalar[1]   =  0.1;   // (--)
    tViewScalar[2]   =  0.3;   // (--)
    tViewScalar[3]   =  0.5;   // (--)
    tViewScalar[4]   =  0.7;   // (--)
    tSurfaceArea     =  2.5;   // (m2)

    /// - Set test flux value.
    tFlux = tIncidentFlux[0] * tAbsorptivity[0] * tViewScalar[0] * tSurfaceArea * tTuningScalar
          + tIncidentFlux[1] * tAbsorptivity[1] * tViewScalar[1] * tSurfaceArea * tTuningScalar
          + tIncidentFlux[2] * tAbsorptivity[2] * tViewScalar[2] * tSurfaceArea * tTuningScalar
          + tIncidentFlux[3] * tAbsorptivity[3] * tViewScalar[3] * tSurfaceArea * tTuningScalar
          + tIncidentFlux[4] * tAbsorptivity[4] * tViewScalar[4] * tSurfaceArea * tTuningScalar;

    /// - Define single-port configuration data.
    tConfigData1 = new GunnsThermalMultiPanelConfigData(tName,
                                                        &tNodeList,
                                                        tTuningScalar,
                                                        &tFluxDistributionFractions1,
                                                        tAbsorptivity[0],
                                                        tAbsorptivity[1],
                                                        tAbsorptivity[2],
                                                        tAbsorptivity[3],
                                                        tAbsorptivity[4],
                                                        tSurfaceArea);

    /// - Define triple-port configuration data and in-line definition of absorptivity array.
    tConfigData3 = new GunnsThermalMultiPanelConfigData(tName,
                                                        &tNodeList,
                                                        tTuningScalar,
                                                        &tFluxDistributionFractions3,
                                                        tAbsorptivity[0],
                                                        tAbsorptivity[1],
                                                        tAbsorptivity[2],
                                                        tAbsorptivity[3],
                                                        tAbsorptivity[4],
                                                        tSurfaceArea);

    /// - Define nominal input data.
    tInputData = new GunnsThermalMultiPanelInputData(tMalfBlockageFlag,
                                                     tMalfBlockageValue,
                                                     tDefaultDemand,
                                                     false,
                                                     0.0,
                                                     tViewScalar[0],
                                                     tViewScalar[1],
                                                     tViewScalar[2],
                                                     tViewScalar[3],
                                                     tViewScalar[4],
                                                     tIncidentFlux[0],
                                                     tIncidentFlux[1],
                                                     tIncidentFlux[2],
                                                     tIncidentFlux[3],
                                                     tIncidentFlux[4]);

    /// - Create the test articles.
    tArticle1 = new FriendlyGunnsThermalMultiPanel;
    tArticle3 = new FriendlyGunnsThermalMultiPanel;

    /// - Initialize the test articles.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->initialize(*tConfigData1, *tInputData, tLinks, &tPorts1) );
    CPPUNIT_ASSERT_NO_THROW( tArticle3->initialize(*tConfigData3, *tInputData, tLinks, &tPorts3) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalMultiPanel::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsThermalMultiPanel . 01: testConfig .......................";

    /// @test  Nominal config construction of single-port source
    CPPUNIT_ASSERT(tName == tConfigData1->mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar,    tConfigData1->cTuningScalar);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[0], tConfigData1->cAbsorptivity[0]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[1], tConfigData1->cAbsorptivity[1]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[2], tConfigData1->cAbsorptivity[2]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[3], tConfigData1->cAbsorptivity[3]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[4], tConfigData1->cAbsorptivity[4]);
    CPPUNIT_ASSERT_EQUAL(tSurfaceArea,     tConfigData1->cSurfaceArea);
    CPPUNIT_ASSERT_EQUAL(1.0,              tConfigData1->cFluxDistributionFractions.at(0) );

    /// @test  Nominal config construction of triple-port source
    CPPUNIT_ASSERT(tName == tConfigData3->mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar,    tConfigData3->cTuningScalar);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[0], tConfigData3->cAbsorptivity[0]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[1], tConfigData3->cAbsorptivity[1]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[2], tConfigData3->cAbsorptivity[2]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[3], tConfigData3->cAbsorptivity[3]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[4], tConfigData3->cAbsorptivity[4]);
    CPPUNIT_ASSERT_EQUAL(tSurfaceArea,     tConfigData3->cSurfaceArea);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(0), tConfigData3->cFluxDistributionFractions.at(0), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(1), tConfigData3->cFluxDistributionFractions.at(1), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(2), tConfigData3->cFluxDistributionFractions.at(2), tTol);

    /// @test  Check default config construction
    GunnsThermalMultiPanelConfigData defaultConfig;
    CPPUNIT_ASSERT("" == defaultConfig.mName);
    CPPUNIT_ASSERT_EQUAL(1.0, defaultConfig.cTuningScalar);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.cAbsorptivity[0]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.cAbsorptivity[1]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.cAbsorptivity[2]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.cAbsorptivity[3]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.cAbsorptivity[4]);
    CPPUNIT_ASSERT_EQUAL(1.0, defaultConfig.cSurfaceArea);
    CPPUNIT_ASSERT( defaultConfig.cFluxDistributionFractions.empty() );

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalMultiPanel::testInput()
{
    std::cout << "\n UtGunnsThermalMultiPanel . 02: testInput ........................";

    /// @test  Check nominal input construction
    CPPUNIT_ASSERT(tMalfBlockageFlag    == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue   == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tDefaultDemand       == tInputData->iDemandedFlux);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[0],   tInputData->iViewScalar[0]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[1],   tInputData->iViewScalar[1]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[2],   tInputData->iViewScalar[2]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[3],   tInputData->iViewScalar[3]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[4],   tInputData->iViewScalar[4]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[0], tInputData->iIncidentFlux[0]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[1], tInputData->iIncidentFlux[1]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[2], tInputData->iIncidentFlux[2]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[3], tInputData->iIncidentFlux[3]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[4], tInputData->iIncidentFlux[4]);

    /// @test  Check default input construction
    GunnsThermalMultiPanelInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iDemandedFlux);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iViewScalar[0]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iViewScalar[1]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iViewScalar[2]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iViewScalar[3]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iViewScalar[4]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iIncidentFlux[0]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iIncidentFlux[1]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iIncidentFlux[2]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iIncidentFlux[3]);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iIncidentFlux[4]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalMultiPanel::testDefaultConstruction()
{
    std::cout << "\n UtGunnsThermalMultiPanel . 03: testDefaultConstruction ..........";

    /// - create non-initialized, default test article
    FriendlyGunnsThermalMultiPanel article;

    /// @test  default-constructed article
    CPPUNIT_ASSERT_EQUAL(0.0, article.mDemandedFlux);
    CPPUNIT_ASSERT_EQUAL(1.0, article.mTuningScalar);
    CPPUNIT_ASSERT(0 == article.mFluxDistributionFractions);
    CPPUNIT_ASSERT(0.0 == article.mIncidentFlux[0]);
    CPPUNIT_ASSERT(0.0 == article.mIncidentFlux[1]);
    CPPUNIT_ASSERT(0.0 == article.mIncidentFlux[2]);
    CPPUNIT_ASSERT(0.0 == article.mIncidentFlux[3]);
    CPPUNIT_ASSERT(0.0 == article.mIncidentFlux[4]);
    CPPUNIT_ASSERT(0.0 == article.mAbsorptivity[0]);
    CPPUNIT_ASSERT(0.0 == article.mAbsorptivity[1]);
    CPPUNIT_ASSERT(0.0 == article.mAbsorptivity[2]);
    CPPUNIT_ASSERT(0.0 == article.mAbsorptivity[3]);
    CPPUNIT_ASSERT(0.0 == article.mAbsorptivity[4]);
    CPPUNIT_ASSERT(0.0 == article.mViewScalar[0]);
    CPPUNIT_ASSERT(0.0 == article.mViewScalar[1]);
    CPPUNIT_ASSERT(0.0 == article.mViewScalar[2]);
    CPPUNIT_ASSERT(0.0 == article.mViewScalar[3]);
    CPPUNIT_ASSERT(0.0 == article.mViewScalar[4]);
    CPPUNIT_ASSERT(0.0 == article.mSurfaceArea);

    /// @test  Init flag
    CPPUNIT_ASSERT(!article.mInitFlag);

    /// - check new/delete for code coverage
    GunnsThermalMultiPanel* pArticle = new GunnsThermalMultiPanel();
    delete pArticle;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalMultiPanel::testNominalInitialization()
{
    std::cout << "\n UtGunnsThermalMultiPanel . 04: testNominalInitialization ........";

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
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[0], tArticle1->mAbsorptivity[0]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[1], tArticle1->mAbsorptivity[1]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[2], tArticle1->mAbsorptivity[2]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[3], tArticle1->mAbsorptivity[3]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[4], tArticle1->mAbsorptivity[4]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[0], tArticle3->mAbsorptivity[0]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[1], tArticle3->mAbsorptivity[1]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[2], tArticle3->mAbsorptivity[2]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[3], tArticle3->mAbsorptivity[3]);
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[4], tArticle3->mAbsorptivity[4]);

    /// @test  View scalar initialization
    CPPUNIT_ASSERT_EQUAL(tViewScalar[0], tArticle1->mViewScalar[0]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[1], tArticle1->mViewScalar[1]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[2], tArticle1->mViewScalar[2]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[3], tArticle1->mViewScalar[3]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[4], tArticle1->mViewScalar[4]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[0], tArticle3->mViewScalar[0]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[1], tArticle3->mViewScalar[1]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[2], tArticle3->mViewScalar[2]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[3], tArticle3->mViewScalar[3]);
    CPPUNIT_ASSERT_EQUAL(tViewScalar[4], tArticle3->mViewScalar[4]);

    /// @test  SurfaceArea initialization
    CPPUNIT_ASSERT_EQUAL(tSurfaceArea, tArticle1->mSurfaceArea);
    CPPUNIT_ASSERT_EQUAL(tSurfaceArea, tArticle3->mSurfaceArea);

    /// @test  IncidentHeatFluxPerArea initialization
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[0], tArticle1->mIncidentFlux[0]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[1], tArticle1->mIncidentFlux[1]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[2], tArticle1->mIncidentFlux[2]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[3], tArticle1->mIncidentFlux[3]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[4], tArticle1->mIncidentFlux[4]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[0], tArticle3->mIncidentFlux[0]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[1], tArticle3->mIncidentFlux[1]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[2], tArticle3->mIncidentFlux[2]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[3], tArticle3->mIncidentFlux[3]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[4], tArticle3->mIncidentFlux[4]);

    /// @test  Flux-distribution-fractions array initialization
    CPPUNIT_ASSERT_EQUAL(tFluxDistributionFractions1.at(0), tArticle1->mFluxDistributionFractions[0]);
    CPPUNIT_ASSERT_EQUAL(tFluxDistributionFractions3.at(0), tArticle3->mFluxDistributionFractions[0]);
    CPPUNIT_ASSERT_EQUAL(tFluxDistributionFractions3.at(1), tArticle3->mFluxDistributionFractions[1]);
    CPPUNIT_ASSERT_EQUAL(tFluxDistributionFractions3.at(2), tArticle3->mFluxDistributionFractions[2]);

    /// @test  Init flag
    CPPUNIT_ASSERT(tArticle1->mInitFlag);
    CPPUNIT_ASSERT(tArticle3->mInitFlag);

    /// - Construct an un-initialized test article.
    FriendlyGunnsThermalMultiPanel article;

    /// @test  Shouldn't initialize() with a single-member ports array and a triple-member flux-distribution vector
    CPPUNIT_ASSERT_THROW( article.initialize(*tConfigData3, *tInputData, tLinks, &tPorts1), TsInitializationException );
    /// @test  Shouldn't initialize() with a triple-member ports array and a single-member flux-distribution vector
    CPPUNIT_ASSERT_THROW( article.initialize(*tConfigData1, *tInputData, tLinks, &tPorts3), TsInitializationException );

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that bad data produces the appropriate exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalMultiPanel::testValidation()
{
    std::cout << "\n UtGunnsThermalMultiPanel . 05: testValidation ...................";

    /// @test  No exception should be thrown on good config & input data.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->validate(*tConfigData1, *tInputData) );

    /// @test  range of absorptivity validation
    for (int i = 0; i < 5; ++i) {
        tConfigData1->cAbsorptivity[i] = -0.1;
        CPPUNIT_ASSERT_THROW(tArticle1->validate(*tConfigData1, *tInputData), TsInitializationException);
        tConfigData1->cAbsorptivity[i] =  1.1;
        CPPUNIT_ASSERT_THROW(tArticle1->validate(*tConfigData1, *tInputData), TsInitializationException);
        tConfigData1->cAbsorptivity[i] = tAbsorptivity[i];
    }

    /// @test range of surface area validation
    tConfigData1->cSurfaceArea = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle1->validate(*tConfigData1, *tInputData), TsInitializationException);
    tConfigData1->cSurfaceArea = tSurfaceArea;

    /// @test  range of view scalar validation
    for (int i = 0; i < 5; ++i) {
        tInputData->iViewScalar[i] = -0.1;
        CPPUNIT_ASSERT_THROW(tArticle1->validate(*tConfigData1, *tInputData), TsInitializationException);
        tInputData->iViewScalar[i] =  1.1;
        CPPUNIT_ASSERT_THROW(tArticle1->validate(*tConfigData1, *tInputData), TsInitializationException);
        tInputData->iViewScalar[i] = tViewScalar[i];
    }

    /// @test  range of incident flux validation
    for (int i = 0; i < 5; ++i) {
        tInputData->iIncidentFlux[i] = -0.1;
        CPPUNIT_ASSERT_THROW(tArticle1->validate(*tConfigData1, *tInputData), TsInitializationException);
        tInputData->iIncidentFlux[i] =  tIncidentFlux[i];
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for updateState().
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalMultiPanel::testUpdateState()
{
    std::cout << "\n UtGunnsThermalMultiPanel . 06: testUpdateState ..................";

    /// - Set mIncidentHeatFluxPerArea manually, to simulate it being set by the SimBus.
    setupIncidentFluxes();

    /// - Call updateState() method.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->updateState(tTimeStep) );
    CPPUNIT_ASSERT_NO_THROW( tArticle3->updateState(tTimeStep) );

    /// - After updateState(), mDemandedFlux should be set, but not affected by any blockage value.
    ///   Blockage is not applied until step(), in the parent-class.
    /// @test  mDemandedFlux was appropriately set
    const double expectedFlux = tAbsorptivity[0] * tSurfaceArea * tIncidentFlux[0] * tViewScalar[0]
                              + tAbsorptivity[1] * tSurfaceArea * tIncidentFlux[1] * tViewScalar[1]
                              + tAbsorptivity[2] * tSurfaceArea * tIncidentFlux[2] * tViewScalar[2]
                              + tAbsorptivity[3] * tSurfaceArea * tIncidentFlux[3] * tViewScalar[3]
                              + tAbsorptivity[4] * tSurfaceArea * tIncidentFlux[4] * tViewScalar[4];
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mSourceFlux not correct",
            expectedFlux, tArticle1->mDemandedFlux, tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mSourceFlux not correct",
            expectedFlux, tArticle3->mDemandedFlux, tTol);

    /// @test  mFlux should not be set until step()
    CPPUNIT_ASSERT_MESSAGE("mFlux nonzero before step()", 0 == tArticle1->mFlux);
    CPPUNIT_ASSERT_MESSAGE("mFlux nonzero before step()", 0 == tArticle3->mFlux);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalMultiPanel::testStep()
{
    std::cout << "\n UtGunnsThermalMultiPanel . 07: testStep .........................";

    /// - Manually set mIncidentHeatFluxPerArea, to simulate it being set by the SimBus.
    setupIncidentFluxes();

    /// - Step the article.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->step(tTimeStep) );

    /// @test  mIncidentHeatFluxPerArea should be unchanged.
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[0], tArticle1->mIncidentFlux[0]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[1], tArticle1->mIncidentFlux[1]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[2], tArticle1->mIncidentFlux[2]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[3], tArticle1->mIncidentFlux[3]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[4], tArticle1->mIncidentFlux[4]);

    /// @test  mFlux should be scaled by the tuning scalar only.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux, tArticle1->mFlux, tTol);

    /// - Give the source a blockage malfunction.
    tArticle1->mMalfBlockageFlag = true;
    tArticle1->mMalfBlockageValue = tMalfBlockageValue;

    /// - Step the article again, this time with blockage active.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->step(tTimeStep) );

    /// @test  mIncidentHeatFluxPerArea should be unchanged.
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[0], tArticle1->mIncidentFlux[0]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[1], tArticle1->mIncidentFlux[1]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[2], tArticle1->mIncidentFlux[2]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[3], tArticle1->mIncidentFlux[3]);
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[4], tArticle1->mIncidentFlux[4]);

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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalMultiPanel::testRestart()
{
    std::cout << "\n UtGunnsThermalMultiPanel . 08: testRestart ......................";

    /// - Manually set mIncidentHeatFluxPerArea, to simulate it being set by the SimBus.
    setupIncidentFluxes();

    /// - Step the article to set stuff.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->step(tTimeStep) );

    /// @test  the restart method.
    CPPUNIT_ASSERT_NO_THROW( tArticle1->restart() );
    // (currently no values are reset to check.)

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the accessor methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalMultiPanel::testAccessors()
{
    std::cout << "\n UtGunnsThermalMultiPanel . 09: testAccessors ....................";

    /// - Manually set mIncidentHeatFluxPerArea, to simulate it being set by the SimBus.
    setupIncidentFluxes();

    /// - @test getAbsorptivity method.
    for (int i = 0; i < 5; i++) {
        CPPUNIT_ASSERT_EQUAL(tAbsorptivity[i], tArticle1->getAbsorptivity(i));
    }
    /// - @test getAbsorptivity method with out of bounds index.
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[0], tArticle1->getAbsorptivity(5));
    CPPUNIT_ASSERT_EQUAL(tAbsorptivity[0], tArticle1->getAbsorptivity(-1));

    /// - @test getSurfaceArea method.
    CPPUNIT_ASSERT_EQUAL(tSurfaceArea, tArticle1->getSurfaceArea());

    /// - @test getViewScalar method.
    for (int i = 0; i < 5; i++) {
        CPPUNIT_ASSERT_EQUAL(tViewScalar[i], tArticle1->getViewScalar(i));
    }
    /// - @test getViewScalar method with out of bounds index.
    CPPUNIT_ASSERT_EQUAL(tViewScalar[0], tArticle1->getViewScalar(5));
    CPPUNIT_ASSERT_EQUAL(tViewScalar[0], tArticle1->getViewScalar(-1));

    /// - @test getIncidentFlux method.
    for (int i = 0; i < 5; i++) {
        CPPUNIT_ASSERT_EQUAL(tIncidentFlux[i], tArticle1->getIncidentFlux(i));
    }
    /// - @test getIncidentFlux method with out of bounds index.
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[0], tArticle1->getIncidentFlux(5));
    CPPUNIT_ASSERT_EQUAL(tIncidentFlux[0], tArticle1->getIncidentFlux(-1));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the test articles' incident fluxes to test values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalMultiPanel::setupIncidentFluxes()
{
    tArticle1->mIncidentFlux[0] = tIncidentFlux[0];
    tArticle1->mIncidentFlux[1] = tIncidentFlux[1];
    tArticle1->mIncidentFlux[2] = tIncidentFlux[2];
    tArticle1->mIncidentFlux[3] = tIncidentFlux[3];
    tArticle1->mIncidentFlux[4] = tIncidentFlux[4];
    tArticle3->mIncidentFlux[0] = tIncidentFlux[0];
    tArticle3->mIncidentFlux[1] = tIncidentFlux[1];
    tArticle3->mIncidentFlux[2] = tIncidentFlux[2];
    tArticle3->mIncidentFlux[3] = tIncidentFlux[3];
    tArticle3->mIncidentFlux[4] = tIncidentFlux[4];
}
