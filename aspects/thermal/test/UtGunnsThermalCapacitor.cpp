/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
    (aspects/thermal/GunnsThermalCapacitor.o)
    (core/Gunns.o)
    )
***************************************************************************************************/
#include "UtGunnsThermalCapacitor.hh"
#include <iostream>

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsThermalCapacitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalCapacitor::UtGunnsThermalCapacitor()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tEditCapacitanceGroup(),
    tDefaultCapacitance(),
    tInitialPotential(),
    tTemperatureOverride(),
    tLargeHeatFlux(),
    tSmallHeatFlux(),
    tName(),
    tMalfBlockageFlag(),
    tMalfBlockageValue(),
    tGunnsSolver(),
    tLinks(),
    tNodes(),
    tNodeList(),
    tPort0(),
    tPort1(),
    tTimeStep(),
    tTolerance()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsThermalCapacitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsThermalCapacitor::~UtGunnsThermalCapacitor()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::setUp()
{
    /// - Declare the standard Gunns link test data
    tMalfBlockageFlag     = false;
    tMalfBlockageValue    = 0.0;
    tPort0                = 0;
    tPort1                = 1;
    tNodeList.mNumNodes   = 2;
    tNodeList.mNodes      = tNodes;
    tTolerance            = 1.0e-08;
    tTimeStep             = 0.1;

    /// - Declare the GunnsThermalCapacitor specific test data
    tName                 = "Test GunnsThermalCapacitor";
    tDefaultCapacitance   = 0.54 * 1.5;  // 0.54 kJ/kg/K * 1.5 kg mass
    tInitialPotential     = 322.0;
    tTemperatureOverride  =  55.0;
    tLargeHeatFlux        =  10.0;
    tSmallHeatFlux        =   5.0;
    tEditCapacitanceGroup =   2;

    /// - Loop through all the nodes.
    for(int i = 0; i < tNodeList.mNumNodes; ++i)
    {
        /// - Call initialize() on every GunnsBasicNode.
        tNodes[i].initialize("Test");
    }

    /// - Tell the solver to initializeNodes().
    tGunnsSolver.initializeNodes(tNodeList);

    /// - Define nominal configuration data
    tConfigData = new GunnsThermalCapacitorConfigData(tName, &tNodeList, tEditCapacitanceGroup);

    /// - Define nominal input data
    tInputData = new GunnsThermalCapacitorInputData(false, tMalfBlockageValue,
            tDefaultCapacitance, tInitialPotential, false, tTemperatureOverride);

    /// - Create new test Article
    tArticle = new FriendlyGunnsThermalCapacitor();

    /// - Initialize the test article.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1 );

    /// - Construct the Gunns network config.
    GunnsConfigData NetworkConfig("Test",  // network name
                                    1.0,  // convergenceTolerance
                                    1.0,  // minLinearizationP
                                     10,  // minorStepLimit
                                     1);  // decompositionLimit

    /// - Initialize the solver after all link objects have been initialized.
    tGunnsSolver.initialize(NetworkConfig, tLinks);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsThermalCapacitor 01: testConfig ..........................";

    /// @test   Nominal config construction
    CPPUNIT_ASSERT(tName                 == tConfigData->mName);
    CPPUNIT_ASSERT(tEditCapacitanceGroup == tConfigData->mEditCapacitanceGroup);

    /// @test   Check default config construction
    GunnsThermalCapacitorConfigData defaultConfig;
    CPPUNIT_ASSERT("unnamed capacitor"   == defaultConfig.mName);
    CPPUNIT_ASSERT(-1                    == defaultConfig.mEditCapacitanceGroup);

    /// @test   Check copy config construction
    GunnsThermalCapacitorConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tName                 == copyConfig.mName);
    CPPUNIT_ASSERT(tEditCapacitanceGroup == copyConfig.mEditCapacitanceGroup);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testInput()
{
    std::cout << "\n UtGunnsThermalCapacitor 02: testInput ...........................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(!tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue  == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tDefaultCapacitance == tInputData->mCapacitance);
    CPPUNIT_ASSERT(tInitialPotential   == tInputData->mPotential);
    CPPUNIT_ASSERT(!tInputData->iTemperatureOverrideFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperatureOverride,  tInputData->iTemperatureOverrideValue,  tTolerance);

    /// - Check default input construction
    GunnsThermalCapacitorInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0  == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0  == defaultInput.mCapacitance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(GunnsThermalCapacitorInputData::DEFAULT_TEMPERATURE,
            defaultInput.mPotential,
            tTolerance);
    CPPUNIT_ASSERT(!defaultInput.iTemperatureOverrideFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(GunnsThermalCapacitorInputData::DEFAULT_TEMPERATURE,
            defaultInput.iTemperatureOverrideValue,
            tTolerance);

    /// - Check copy input construction
    GunnsThermalCapacitorInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mCapacitance       == copyInput.mCapacitance);
    CPPUNIT_ASSERT(tInputData->mPotential         == copyInput.mPotential);
    CPPUNIT_ASSERT(tInputData->iTemperatureOverrideFlag == copyInput.iTemperatureOverrideFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInputData->iTemperatureOverrideValue,
            copyInput.iTemperatureOverrideValue,
            tTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testDefaultConstruction()
{
    std::cout << "\n UtGunnsThermalCapacitor 03: testDefaultConstruction .............";

    /// - Construct an uninitialized test article
    FriendlyGunnsThermalCapacitor article;

    /// @test   Default values on construction
    CPPUNIT_ASSERT(0   == article.mEditCapacitanceGroup);
    CPPUNIT_ASSERT(0.0 == article.mCapacitance);
    CPPUNIT_ASSERT(0.0 == article.mPotentialDrop);

    /// @test   mExternalHeatFlux[] is not null.
    CPPUNIT_ASSERT(0 != tArticle->mExternalHeatFlux);

    /// - Loop through each index of the mExternalHeatFlux[] array.
    for (int i = 0; i < tArticle->NUM_EXT_HEATFLUXES; ++i)
    {
        /// @test   Each index should be cleared.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mExternalHeatFlux[i],  tTolerance);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testInitialization()
{
    std::cout << "\n UtGunnsThermalCapacitor 04: testInitialization ..................";

    /// @test   config and input values
    CPPUNIT_ASSERT(tEditCapacitanceGroup == tArticle->mEditCapacitanceGroup);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tDefaultCapacitance, tArticle->mCapacitance,        tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInitialPotential,   tArticle->mPotentialVector[0], tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMalfBlockageValue,  tArticle->mMalfBlockageValue,  tTolerance);

    /// @test   initial temperature
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInitialPotential, tArticle->mTemperature, tTolerance);

    /// @test   initialization of the temperature override.
    FriendlyGunnsThermalCapacitor article;
    tInputData->iTemperatureOverrideFlag = true;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT(true == article.mOverrideVector[0]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperatureOverride, article.mPotentialVector[0], tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperatureOverride, article.mTemperature,        tTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that bad data produces the appropriate exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testValidation()
{
    std::cout << "\n UtGunnsThermalCapacitor 05: testValidation ......................";

    /// @test  Exception thrown on invalid temperature value.
    tInputData->mPotential = -10;
    CPPUNIT_ASSERT_THROW(tArticle->validate(*tInputData), TsInitializationException);
    tInputData->mPotential = tInitialPotential;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testRestart()
{
    std::cout << "\n UtGunnsThermalCapacitor 06: testRestart .........................";

    /// - initialize with default data
    FriendlyGunnsThermalCapacitor article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1 );

    article.mExternalHeatFlux[0]  = 1.0;
    article.mExternalHeatFlux[14] = 14.0;

    /// @test  Test article and base class attributes reset by the restart.
    article.restart();
    CPPUNIT_ASSERT(0.0 == article.mExternalHeatFlux[0]);
    CPPUNIT_ASSERT(0.0 == article.mExternalHeatFlux[14]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests link update using an updateState() method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testUpdateState()
{
    std::cout << "\n UtGunnsThermalCapacitor 07: testUpdateState .....................";

    testLinkUpdate(&UtGunnsThermalCapacitor::callUpdateState);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the capacitor via a call to updateState().
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::callUpdateState()
{
    tArticle->updateState(tTimeStep);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests link update using a network step() method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testNetworkStep()
{
    std::cout << "\n UtGunnsThermalCapacitor 08: testNetworkStep .....................";

    testLinkUpdate(&UtGunnsThermalCapacitor::callGunnsStep);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Updates the capacitor via a call to Gunns::step().
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::callGunnsStep()
{
    tGunnsSolver.step(tTimeStep);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  pf        (--)  A pointer to a zero-argument void function. Used to represent
///                             one of the two update methods.
/// @details  Tests the update of the capacitor, which can be done via either the updateState()
///           method in the case of a dummy network, or the Gunns.step() method in the case of a
///           regular network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testLinkUpdate(void (UtGunnsThermalCapacitor::*pf) ())
{
    /// - Placeholder for last-step potential
    double tPreviousPotential = tInitialPotential;

    /// - Default net heat flux
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Update the capacitor.
    (this->*pf)();

    /// @test   If no External Heat Fluxes have been set, the sum should be zero.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSumExternalHeatFluxes, tTolerance);

    /// - Zero net heat flux
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - manually set the members of mExternalHeatFlux[], to simulate sim_bus overwrite.
    tArticle->mExternalHeatFlux[0] = -tSmallHeatFlux;
    tArticle->mExternalHeatFlux[1] =  tSmallHeatFlux;

    /// - save previous Potential
    tPreviousPotential = tArticle->getTemperature();

    /// - Update the capacitor to calculate a new Potential[0]
    (this->*pf)();

    /// @test    With equal heat fluxes the temperature should not change.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPreviousPotential, tArticle->mPotentialVector[0], tTolerance);

    /// - Negative net heat flux
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - manually set the members of mExternalHeatFlux[], to simulate sim_bus overwrite.
    tArticle->mExternalHeatFlux[0] = -tLargeHeatFlux;
    tArticle->mExternalHeatFlux[1] =  tSmallHeatFlux;

    /// - save previous Potential
    tPreviousPotential = tArticle->getTemperature();

    /// - Update the capacitor to calculate a new Potential[0]
    (this->*pf)();

    /// @test    With a negative net heat flux, temperature should decrease from tPreviousPotential.
    CPPUNIT_ASSERT_MESSAGE("Net heat flux is negative(-) but potential did not decrease.",
            tArticle->mPotentialVector[0] < tPreviousPotential);

    /// - Positive net heat flux
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - manually set the members of mExternalHeatFlux[], to simulate sim_bus overwrite.
    tArticle->mExternalHeatFlux[0] = -tSmallHeatFlux;
    tArticle->mExternalHeatFlux[1] =  tLargeHeatFlux;

    /// - save previous Potential
    tPreviousPotential = tArticle->getTemperature();

    /// - Update the capacitor to calculate a new Potential[0]
    (this->*pf)();

    /// @test    With a positive net heat flux, temperature should decrease from tPreviousPotential.
    CPPUNIT_ASSERT_MESSAGE("Net heat flux is positive(+) but potential did not increase.",
            tArticle->mPotentialVector[0] > tPreviousPotential);

    /// - Zero Capacitance tests.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Create a new article, and initialize with zero capacitance.
    FriendlyGunnsThermalCapacitor article;
    GunnsThermalCapacitorInputData inputData(false, tMalfBlockageValue, 0.0, tInitialPotential);
    article.initialize(*tConfigData, inputData, tLinks, tPort0, tPort1 );

    /// - manually set the members of mExternalHeatFlux[], to simulate sim_bus overwrite.
    article.mExternalHeatFlux[0] = -tSmallHeatFlux;
    article.mExternalHeatFlux[1] =  tLargeHeatFlux;

    /// - Update the capacitor.
    article.updateState(tTimeStep);

    /// @test   With zero capacitance, the Potential should not update, regardless of the
    ///         net heat flux.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInitialPotential, article.mPotentialVector[0], tTolerance);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Makes sure the net sum of the external heat fluxes is communicated into the link's
///           mSourceVector, which is used as an input to GUNNS.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testBuildCapacitance()
{
    std::cout << "\n UtGunnsThermalCapacitor 09: testBuildCapacitance ................";

    /// - first find what the SourceVector would be without any external heat fluxes.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - Create a default article.
    FriendlyGunnsThermalCapacitor article;
    /// - initialize with default data
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1 );
    /// - step the capacitor
    article.buildCapacitance(tTimeStep);

    /// - save the default value of SourceVector
    const double tPreviousSource = article.mSourceVector[0];

    /// - compare the default value of SourceVector with one with ExternalHeatFluxes.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// - manually set the members of mExternalHeatFlux[], to simulate sim_bus overwrite.
    tArticle->mExternalHeatFlux[0] = -tSmallHeatFlux;
    tArticle->mExternalHeatFlux[1] =  tLargeHeatFlux;

    /// - step() calls both updateState() and buildCapacitance()
    tArticle->step(tTimeStep);

    /// @test   buildCapacitance() stores the net ExternalHeatFluxes into the link's SourceVector
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("ExternalHeatFluxes not loaded into SourceVector.",
         tPreviousSource + tLargeHeatFlux - tSmallHeatFlux, tArticle->mSourceVector[0], tTolerance);

    std::cout << "... Pass";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
///// @details  Tests correct recording of mTemperature from the link's mPotentialVector.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testTemperatureRecord()
{
    std::cout << "\n UtGunnsThermalCapacitor 10: testTemperatureRecord ...............";

    /// @test   mTemperature should be set at initialize()
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mTemperature not recorded after initialize() call.",
            tArticle->mTemperature, tArticle->mPotentialVector[0], tTolerance);

    /// - manually set the members of mExternalHeatFlux[], to simulate sim_bus overwrite.
    tArticle->mExternalHeatFlux[0] = -tSmallHeatFlux;
    tArticle->mExternalHeatFlux[1] =  tLargeHeatFlux;

    /// - Update the capacitor.
    tArticle->updateState(tTimeStep);

    /// @test   mTemperature should be set in updateState(), in case Potential is set manually in a
    ///         dummy network.
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mTemperature not recorded after step() call.",
            tArticle->mTemperature, tArticle->mPotentialVector[0], tTolerance);

    /// - processOutputs() is called by the GUNNS orchestrator. If the ThermalCapacitor is NOT in a
    ///   dummy network, then GUNNS solves for all the potentials in all the nodes. Afterward, it
    ///   calls processOutputs(), so here is a good place to record mTemperature.
    tArticle->processOutputs();

    /// @test   mTemperature should be set in processOutputs()
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("mTemperature not recorded after step() call.",
            tArticle->mTemperature, tArticle->mPotentialVector[0], tTolerance);

    std::cout << "... Pass";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
///// @details  Tests the class setter and getter methods.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testTemperatureOverrride()
{
    std::cout << "\n UtGunnsThermalCapacitor 11: testTemperatureOverride .............";

    /// - save previous Potential
    double tPreviousPotential = tArticle->getTemperature();

    /// - Provide the link a positive net heat flux. Manually set the members of mExternalHeatFlux[],
    ///   to simulate sim_bus overwrite.
    tArticle->mExternalHeatFlux[0] = -tSmallHeatFlux;
    tArticle->mExternalHeatFlux[1] =  tLargeHeatFlux;

    /// - Activate override  and step the network.
    tArticle->setPortOverride(0, tTemperatureOverride);
    tGunnsSolver.step(tTimeStep);

    /// @test   Temperature should be set to the override value.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperatureOverride, tArticle->getTemperature(), tTolerance);

    /// - No matter how many times the network is stepped, the temperature will hold constant.
    tGunnsSolver.step(tTimeStep);
    tGunnsSolver.step(tTimeStep);
    tGunnsSolver.step(tTimeStep);
    tGunnsSolver.step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTemperatureOverride, tArticle->getTemperature(), tTolerance);

    /// - Now de-activate the override malfunction and step the network several times.
    tArticle->resetPortOverride();
    tGunnsSolver.step(tTimeStep);
    tGunnsSolver.step(tTimeStep);
    tGunnsSolver.step(tTimeStep);
    tGunnsSolver.step(tTimeStep);

    /// - Since the malf is off, Gunns is free to operate on the link's temperature. However,
    /// it should start from the malf'd temperature, not the original.
    /// @test   The temperature should still be less than the previous potential.
    CPPUNIT_ASSERT(tArticle->getTemperature() < tPreviousPotential);
    /// @test   But greater than the override temperature, since there is a net (+) heat flux.
    CPPUNIT_ASSERT(tArticle->getTemperature() > tTemperatureOverride);

    std::cout << "... Pass";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
///// @details  Tests the class setter and getter methods.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsThermalCapacitor::testAccessors()
{
    std::cout << "\n UtGunnsThermalCapacitor 12: testAccessors .......................";

    int group = 5;
    double potential = 126.4;
    double override = 156.1;

    /// @test   getEditCapacitanceGroup() accessor.
    tArticle->mEditCapacitanceGroup = group;
    CPPUNIT_ASSERT(group == tArticle->getEditCapacitanceGroup());

    /// @test   getTemperature() accessor.
    tArticle->mPotentialVector[0] = potential;
    CPPUNIT_ASSERT(potential == tArticle->getTemperature());

    /// @test   setExternalHeatFlux with nominal values.
    tArticle->setExternalHeatFlux(0, 2.0);
    CPPUNIT_ASSERT(2.0 == tArticle->mExternalHeatFlux[0]);
    tArticle->setExternalHeatFlux(GunnsThermalCapacitor::NUM_EXT_HEATFLUXES-1, 3.0);
    CPPUNIT_ASSERT(3.0 == tArticle->mExternalHeatFlux[GunnsThermalCapacitor::NUM_EXT_HEATFLUXES-1]);

    /// @test   exception thrown from setExternalHeatFlux given invalid index < minimum allowed.
    CPPUNIT_ASSERT_THROW(tArticle->setExternalHeatFlux(-1, 1.0), TsOutOfBoundsException);

    /// @test   exception thrown from setExternalHeatFlux given invalid index > maximum allowed.
    CPPUNIT_ASSERT_THROW(tArticle->setExternalHeatFlux(GunnsThermalCapacitor::NUM_EXT_HEATFLUXES, 4.0),
                         TsOutOfBoundsException);

    std::cout << "... Pass";
}
