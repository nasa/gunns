/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (aspects/thermal/GunnsThermalSource.o)
    )
***************************************************************************************************/
#include "UtGunnsThermalSource.hh"
#include <iostream>
#include "software/exceptions/TsInitializationException.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermalSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalSource::UtGunnsThermalSource()
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
    tFluxDistributionFractions1(),
    tFluxDistributionFractions3(),
    tPort0Temperature(),
    tPort1Temperature(),
    tPort2Temperature(),
    tAvgPortTemperature(),
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
/// @details  This is the default destructor for the UtGunnsThermalSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalSource::~UtGunnsThermalSource()
{
    // do nothing
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalSource::tearDown()
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
void UtGunnsThermalSource::setUp()
{
    /// - Declare the GunnsThermalSource specific test data
    tName = "Test GunnsThermalSource";
    tTol = 1.0e-08;
    tTimeStep  = 0.1;

    /// - Set some default node temperatures.
    tPort0Temperature = 124.4;
    tPort1Temperature =  67.1;
    tPort2Temperature = 355.0;
    tAvgPortTemperature = (tPort0Temperature + tPort1Temperature + tPort2Temperature) / 3;

    /// - Set node potential (temperature).
    tNodes[1].setPotential(tPort0Temperature);
    tNodes[2].setPotential(tPort1Temperature);
    tNodes[3].setPotential(tPort2Temperature);

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

    /// - Set Source tuning scalar.
    tTuningScalar = 0.93;
    /// - Set a default demand.
    tDefaultDemand = 30.0;
    /// - Set test flux value.
    tFlux = tDefaultDemand * tTuningScalar;

    /// - Define single-port configuration data.
    tConfigData1 = new GunnsThermalSourceConfigData(tName, &tNodeList, tTuningScalar, &tFluxDistributionFractions1);

    /// - Define triple-port configuration data.
    tConfigData3 = new GunnsThermalSourceConfigData(tName, &tNodeList, tTuningScalar, &tFluxDistributionFractions3);

    /// - Define nominal input data.
    tInputData = new GunnsThermalSourceInputData(tMalfBlockageFlag, tMalfBlockageValue, tDefaultDemand);

    /// - Create the test articles.
    tArticle1 = new FriendlyGunnsThermalSource;
    tArticle3 = new FriendlyGunnsThermalSource;

    /// - Initialize the test articles.
    tArticle1->initialize(*tConfigData1, *tInputData, tLinks, &tPorts1);
    tArticle3->initialize(*tConfigData3, *tInputData, tLinks, &tPorts3);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalSource::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsThermalSource .. 01: testConfig ..........................";

    /// @test  Nominal config construction of single-port source
    CPPUNIT_ASSERT(tName == tConfigData1->mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, tConfigData1->cTuningScalar);
    CPPUNIT_ASSERT_EQUAL(1.0, tConfigData1->cFluxDistributionFractions.at(0) );

    /// @test  Nominal config construction of triple-port source
    CPPUNIT_ASSERT(tName == tConfigData3->mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, tConfigData3->cTuningScalar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(0), tConfigData3->cFluxDistributionFractions.at(0), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(1), tConfigData3->cFluxDistributionFractions.at(1), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(2), tConfigData3->cFluxDistributionFractions.at(2), tTol);

    /// @test  Check default config construction
    GunnsThermalSourceConfigData defaultConfig;
    CPPUNIT_ASSERT("unnamed source" == defaultConfig.mName);
    CPPUNIT_ASSERT_EQUAL(1.0, defaultConfig.cTuningScalar);
    CPPUNIT_ASSERT( defaultConfig.cFluxDistributionFractions.empty() );

    /// @test  Check copy config construction of single-port source
    GunnsThermalSourceConfigData copyConfig1(*tConfigData1);
    CPPUNIT_ASSERT(tName  == copyConfig1.mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, copyConfig1.cTuningScalar);
    CPPUNIT_ASSERT_EQUAL(1.0, copyConfig1.cFluxDistributionFractions.at(0));

    /// @test  Check copy config construction of triple-port source
    GunnsThermalSourceConfigData copyConfig3(*tConfigData3);
    CPPUNIT_ASSERT(tName == copyConfig3.mName);
    CPPUNIT_ASSERT_EQUAL(tTuningScalar, copyConfig3.cTuningScalar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(0), copyConfig3.cFluxDistributionFractions.at(0), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(1), copyConfig3.cFluxDistributionFractions.at(1), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluxDistributionFractions3.at(2), copyConfig3.cFluxDistributionFractions.at(2), tTol);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the overrideFluxDistributionFractions method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalSource::testOverrideFluxDistributionFractions()
{
    std::cout << "\n UtGunnsThermalSource .. 02: testOverrideFluxDistributionFractions";


    // Set up a modified config data point.
    GunnsThermalSourceConfigData* modifiedConfig;
    // Start from tConfigData1.
    modifiedConfig = new GunnsThermalSourceConfigData(*tConfigData1);
    // Set up new flux distribution fractions.
    int modifiedNumFractions = 4;
    double array[modifiedNumFractions];
    array[0] = 0.1;
    array[1] = 0.2;
    array[2] = 0.3;
    array[3] = 0.4;
    // Override flux distribution fractions.
    modifiedConfig->overrideFluxDistributionFractions(array, modifiedNumFractions);

    // @test   Flux distribution fractions number of elements.
    int newSize = modifiedConfig->cFluxDistributionFractions.size();
    CPPUNIT_ASSERT_EQUAL( modifiedNumFractions, newSize);

    // @test   Flux distribution fractions values.
    for (int i = 0; i < modifiedNumFractions; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(modifiedConfig->cFluxDistributionFractions.at(i), array[i], tTol);
    }

    delete (modifiedConfig);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalSource::testInput()
{
    std::cout << "\n UtGunnsThermalSource .. 03: testInput ...........................";

    /// @test  Check nominal input construction
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tDefaultDemand     == tInputData->iDemandedFlux);

    /// @test  Check default input construction
    GunnsThermalSourceInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.iDemandedFlux);

    /// @test  Check copy input construction
    GunnsThermalSourceInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(tInputData->iDemandedFlux, copyInput.iDemandedFlux);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalSource::testDefaultConstruction()
{
    std::cout << "\n UtGunnsThermalSource .. 04: testDefaultConstruction .............";

    /// - Construct an un-initialized test article.
    FriendlyGunnsThermalSource article;

    /// @test  Default construction data
    CPPUNIT_ASSERT_EQUAL(0.0, article.mDemandedFlux);
    CPPUNIT_ASSERT_EQUAL(1.0, article.mTuningScalar);
    CPPUNIT_ASSERT(0 == article.mFluxDistributionFractions);
    CPPUNIT_ASSERT_EQUAL(0.0, article.mAvgPortTemperature);

    /// @test  Init flag
    CPPUNIT_ASSERT(!article.mInitFlag);

    /// @test  New/delete for code coverage
    GunnsThermalSource* pArticle = new GunnsThermalSource();
    delete pArticle;

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization of a GunnsThermalSource with no Card Name given.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalSource::testInitialization()
{
    std::cout << "\n UtGunnsThermalSource .. 05: testInitialization ..................";

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

    /// @test  Avg port temperature
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tAvgPortTemperature, tArticle3->mAvgPortTemperature, tTol);

    /// @test  Temperature override controls
    CPPUNIT_ASSERT_EQUAL(false, tArticle1->mTemperatureOverrideFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle1->mTemperatureOverrideValue);

    /// @test  Init flag
    CPPUNIT_ASSERT(tArticle1->mInitFlag);
    CPPUNIT_ASSERT(tArticle3->mInitFlag);

    /// - Construct an un-initialized test article.
    FriendlyGunnsThermalSource article;

    /// @test  Shouldn't initialize() with a single-member ports array and a triple-member flux-distribution vector
    CPPUNIT_ASSERT_THROW( article.initialize(*tConfigData3, *tInputData, tLinks, &tPorts1), TsInitializationException );
    /// @test  Shouldn't initialize() with a triple-member ports array and a single-member flux-distribution vector
    CPPUNIT_ASSERT_THROW( article.initialize(*tConfigData1, *tInputData, tLinks, &tPorts3), TsInitializationException );

    ///
    /// - Test with modified number of ports and new initial port map.
    ///

    /// - Construct an un-initialized test article.
    FriendlyGunnsThermalSource articleUpdatedNumPorts;

    // Set up a modified config data point.
    GunnsThermalSourceConfigData* modifiedConfig;
    // Start from tConfigData1.
    modifiedConfig = new GunnsThermalSourceConfigData(*tConfigData1);
    // Set up new flux distribution fractions.
    int modifiedNumFractions = 4;
    double fluxArray[modifiedNumFractions];
    fluxArray[0] = 0.1;
    fluxArray[1] = 0.2;
    fluxArray[2] = 0.3;
    fluxArray[3] = 0.4;
    // Override flux distribution fractions.
    modifiedConfig->cNumPortsOverride = modifiedNumFractions;
    modifiedConfig->overrideFluxDistributionFractions(fluxArray, modifiedNumFractions);

    // Set up input data. Not needed for actual test, but will keep the number of ports consistent so
    // array assignments inside GunnsBasicLink::initialize don't go out of bounds during the test.
    GunnsThermalSourceInputData* modifiedInput;
    // Start from tInputData.
    modifiedInput = new GunnsThermalSourceInputData(*tInputData);
    int initialMapArray[modifiedNumFractions];
    initialMapArray[0] = 1;
    initialMapArray[1] = 2;
    initialMapArray[2] = 3;
    initialMapArray[3] = 4;
    modifiedInput->mInitialNodeMap = initialMapArray;

    articleUpdatedNumPorts.initialize(*modifiedConfig, *modifiedInput, tLinks, &tPorts1);

    /// @test  number of ports override.
    CPPUNIT_ASSERT_EQUAL(articleUpdatedNumPorts.mNumPorts, modifiedNumFractions);

    delete(modifiedInput);
    delete(modifiedConfig);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for correct updateState() functionality.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalSource::testBuildSourceVector()
{
    std::cout << "\n UtGunnsThermalSource .. 06: testBuildSourceVector ...............";

    /// - Manually set mFlux for testing purposes.
    tArticle1->mFlux = tFlux;
    tArticle3->mFlux = tFlux;

    /// - Call buildSourceVector().
    tArticle1->buildSourceVector();
    tArticle3->buildSourceVector();

    /// @test  mSourceVector appropriately set for both sources
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux * tFluxDistributionFractions1.at(0), tArticle1->mSourceVector[0], tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux * tFluxDistributionFractions3.at(0), tArticle3->mSourceVector[0], tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux * tFluxDistributionFractions3.at(1), tArticle3->mSourceVector[1], tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux * tFluxDistributionFractions3.at(2), tArticle3->mSourceVector[2], tTol);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the computeFlows() method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalSource::testComputeFlows()
{
    std::cout << "\n UtGunnsThermalSource .. 07: testComputeFlows ....................";

    /// - Manually set mFlux for testing purposes.
    tArticle1->mFlux = tFlux;
    tArticle3->mFlux = tFlux;

    /// - Call computeFlows() on both test sources.
    tArticle1->computeFlows(tTimeStep);
    tArticle3->computeFlows(tTimeStep);

    /// @test  The single-port source's node was given the correct influxRate
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux * tFluxDistributionFractions1[0], tArticle1->mNodes[0]->getInflux(), tTol);

    /// @test  The triple-port source's nodes were given the correct influxRate
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux * tFluxDistributionFractions3[0], tArticle3->mNodes[0]->getInflux(), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux * tFluxDistributionFractions3[1], tArticle3->mNodes[1]->getInflux(), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux * tFluxDistributionFractions3[2], tArticle3->mNodes[2]->getInflux(), tTol);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the step() method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalSource::testStep()
{
    std::cout << "\n UtGunnsThermalSource .. 08: testStep ............................";

    /// - Manually set mDemandedFlux.
    tArticle1->mDemandedFlux = tDefaultDemand;

    /// - Step the article.
    tArticle1->step(tTimeStep);

    /// @test  mDemandedFlux should be unchanged.
    CPPUNIT_ASSERT_EQUAL(tDefaultDemand, tArticle1->mDemandedFlux);

    /// @test  mFlux should be scaled by the tuning scalar only.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux, tArticle1->mFlux, tTol);

    /// - Give the source a blockage malfunction.
    tArticle1->mMalfBlockageFlag = true;
    tArticle1->mMalfBlockageValue = tMalfBlockageValue;

    /// - Step the article again, this time with blockage active.
    tArticle1->step(tTimeStep);

    /// @test  mDemandedFlux should be unchanged.
    CPPUNIT_ASSERT_EQUAL(tDefaultDemand, tArticle1->mDemandedFlux);

    /// @test  mFlux should be scaled by the tuning scalar and affected by the blockage.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFlux * (1.0-tMalfBlockageValue), tArticle1->mFlux, tTol);

    /// @test  The flux override malfunction is set by the access method.
    tArticle1->setMalfFluxOverride(true, tMalfFluxOverrideValue);
    CPPUNIT_ASSERT(true                   == tArticle1->mMalfFluxOverrideFlag);
    CPPUNIT_ASSERT(tMalfFluxOverrideValue == tArticle1->mMalfFluxOverrideValue);

    /// - Step the article again, this time with override active.
    tArticle1->step(tTimeStep);

    /// @test  mFlux should be overriden.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMalfFluxOverrideValue, tArticle1->mFlux, tTol);

    /// @test  The flux override malfunction is reset by the access method.
    tArticle1->setMalfFluxOverride();
    CPPUNIT_ASSERT(false == tArticle1->mMalfFluxOverrideFlag);
    CPPUNIT_ASSERT(0.0   == tArticle1->mMalfFluxOverrideValue);

    /// - Step the triple-port article again.
    tArticle3->mNodes[0]->setPotential(tPort0Temperature);
    tArticle3->mNodes[1]->setPotential(tPort0Temperature);
    tArticle3->mNodes[2]->setPotential(tPort0Temperature);
    tArticle3->step(tTimeStep);

    /// @test  Avg port temperature
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPort0Temperature, tArticle3->mAvgPortTemperature, tTol);

    /// - Turn on the temperature override and turn off the flux override malf.
    tArticle3->mMalfFluxOverrideFlag     = false;
    tArticle3->mTemperatureOverrideFlag  = true;
    tArticle3->mTemperatureOverrideValue = 300.0;

    /// - Step the article again, this time with temperature override active.
    tArticle3->step(tTimeStep);

    /// @test  mPotentialVector and mOverrideVector should be set.
    CPPUNIT_ASSERT_EQUAL(true, tArticle3->mOverrideVector[0]);
    CPPUNIT_ASSERT_EQUAL(true, tArticle3->mOverrideVector[1]);
    CPPUNIT_ASSERT_EQUAL(true, tArticle3->mOverrideVector[2]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, tArticle3->mPotentialVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, tArticle3->mPotentialVector[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, tArticle3->mPotentialVector[2], DBL_EPSILON);

    /// - Step the article again with the temperature override inactive.
    tArticle3->mTemperatureOverrideFlag  = false;
    tArticle3->step(tTimeStep);

    /// @test  mOverrideVector should be reset.
    CPPUNIT_ASSERT_EQUAL(false, tArticle3->mOverrideVector[0]);
    CPPUNIT_ASSERT_EQUAL(false, tArticle3->mOverrideVector[1]);
    CPPUNIT_ASSERT_EQUAL(false, tArticle3->mOverrideVector[2]);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that bad data produces the appropriate exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalSource::testValidation()
{
    std::cout << "\n UtGunnsThermalSource .. 09: testValidation ......................";

    /// - Construct an un-initialized, single port test article.
    FriendlyGunnsThermalSource article;
//    TS_STRDUP(article.mName, tName.c_str());
    article.mName = tName;
    article.mNumPorts = 1;

    /// @test  DemandedFlux less than zero.
    tInputData->iDemandedFlux = -10;
    CPPUNIT_ASSERT_THROW(article.validate(*tConfigData1,*tInputData), TsInitializationException);
    tInputData->iDemandedFlux = tDefaultDemand;

    /// @test  Empty flux-distribution-fraction vector
    tConfigData1->cFluxDistributionFractions.clear();
    CPPUNIT_ASSERT_THROW(article.validate(*tConfigData1,*tInputData), TsInitializationException);

    /// @test  Flux-distribution vector that is not the same size as the number of ports
    ///        (tConfigData3 has a three-member power-fraction vector).
    CPPUNIT_ASSERT_THROW(article.validate(*tConfigData3,*tInputData), TsInitializationException);

    /// - Increase the number of ports.
    article.mNumPorts = 3;
    /// @test  validate() should pass
    CPPUNIT_ASSERT_NO_THROW( article.validate(*tConfigData3,*tInputData) );

    /// @test  flux-distribution vector with a negative value
    tConfigData3->cFluxDistributionFractions.clear();
    tConfigData3->cFluxDistributionFractions.push_back(-0.2);
    tConfigData3->cFluxDistributionFractions.push_back(0.5);
    tConfigData3->cFluxDistributionFractions.push_back(0.7);
    CPPUNIT_ASSERT_THROW(article.validate(*tConfigData3,*tInputData), TsInitializationException);

    /// @test  flux-distribution vector that does not sum to one
    tConfigData3->cFluxDistributionFractions.clear();
    tConfigData3->cFluxDistributionFractions.push_back(0.2);
    tConfigData3->cFluxDistributionFractions.push_back(0.2);
    tConfigData3->cFluxDistributionFractions.push_back(0.2);
    CPPUNIT_ASSERT_THROW(article.validate(*tConfigData3,*tInputData), TsInitializationException);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that the port rules check returns the correct booleans.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalSource::testSpecificPortRules()
{
    std::cout << "\n UtGunnsThermalSource .. 10: testSpecificPortRules................";

    /// @test  With a port mapped to vacuum, checkSpecificPortRules() method should return false.
    CPPUNIT_ASSERT_MESSAGE("Port invalid but checkSpecificPortRules() does not return false.",
            false == tArticle1->checkSpecificPortRules(5, tArticle1->getGroundNodeIndex()) );
    CPPUNIT_ASSERT_MESSAGE("Port invalid but checkSpecificPortRules() does not return false.",
            false == tArticle3->checkSpecificPortRules(5, tArticle3->getGroundNodeIndex()) );

    std::cout << "... Pass\n";
}
