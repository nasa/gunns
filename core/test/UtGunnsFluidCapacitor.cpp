/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (core/GunnsFluidCapacitor.o)
        (core/GunnsFluidNode.o)
        (aspects/fluid/fluid/PolyFluid.o)
    )
***************************************************************************************************/
#include "UtGunnsFluidCapacitor.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidCapacitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidCapacitor::UtGunnsFluidCapacitor()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mInitialVolume(),
    mExpansionScaleFactor(),
    mNodes(),
    mNodeList(),
    mLinks(),
    mPort0(),
    mPort1(),
    mTimeStep(),
    mTolerance(),
    mFluidProperties(),
    mFluidConfig(),
    mFluidInput1(),
    mFluidInput2(),
    mFractions()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidCapacitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidCapacitor::~UtGunnsFluidCapacitor()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput2;
    delete mFluidInput1;
    delete mFluidConfig;
    delete [] mFractions;
    delete mFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::setUp()
{
    mLinkName             = "Test Fluid Link";
    mNodeList.mNumNodes   = 2;
    mNodeList.mNodes      = mNodes;
    mInitialVolume        = 0.5;
    mExpansionScaleFactor = 0.4;
    mPort0                = 0;
    mPort1                = 1;
    mTimeStep             = 0.1;
    mTolerance            = 0.01;

    /// - Setup some fluid nodes
    mFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    mFractions = new double[2];
    mFractions[0] = 0.5;
    mFractions[1] = 0.5;
    mFluidConfig = new PolyFluidConfigData(mFluidProperties, types, 2);
    mFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          mFractions);              //massFractions
    mFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          689.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          mFractions);              //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    mNodes[0].initialize("Node1", mFluidConfig);
    mNodes[1].initialize("Node2", mFluidConfig);

    mNodes[0].getContent()->initialize(*mFluidConfig, *mFluidInput1);
    mNodes[1].getContent()->initialize(*mFluidConfig, *mFluidInput2);

    /// - Define nominal configuration data
    mConfigData = new GunnsFluidCapacitorConfigData(
            mLinkName, &mNodeList, mExpansionScaleFactor);

    /// - Define nominal input data
    mInputData = new GunnsFluidCapacitorInputData(false, 0.0, mInitialVolume, mFluidInput2);

    mArticle = new FriendlyGunnsFluidCapacitor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidCapacitor .. 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName             == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(mExpansionScaleFactor == mConfigData->mExpansionScaleFactor);

    /// - Check default config construction
    GunnsFluidCapacitorConfigData defaultConfig;
    CPPUNIT_ASSERT(""                    == defaultConfig.mName);
    CPPUNIT_ASSERT(0                     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mExpansionScaleFactor);

    /// - Check copy config construction
    GunnsFluidCapacitorConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName             == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes                == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(mExpansionScaleFactor == copyConfig.mExpansionScaleFactor);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::testInput()
{
    std::cout << "\n UtGunnsFluidCapacitor .. 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(mInitialVolume == mInputData->mInitialVolume);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFluidInput2->mPressure,
                                 mInputData->mInitialFluidState->mPressure, 0.0);

    /// - Check default input data construction
    GunnsFluidCapacitorInputData defaultInput;
    CPPUNIT_ASSERT(0.0            == defaultInput.mInitialVolume);
    CPPUNIT_ASSERT(0              == defaultInput.mInitialFluidState);

    /// - Check input copy construction
    GunnsFluidCapacitorInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInitialVolume == copyInput.mInitialVolume);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(copyInput.mInitialFluidState->mPressure,
                                 mInputData->mInitialFluidState->mPressure, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidCapacitor .. 03: testDefaultConstruction ...............";

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mCapacitance, 0.0);
    CPPUNIT_ASSERT(!mArticle->mDisablePressureCorrection);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidCapacitor* article = new GunnsFluidCapacitor();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidCapacitor .. 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidCapacitor article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test config and input data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialVolume,
            article.mNodes[0]->getVolume(),                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article.mPotentialVector[0],
            article.mNodes[0]->getPotential(),              mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getContent()->getPressure(),
            article.mNodes[0]->getContent()->getPressure(), mTolerance);

    CPPUNIT_ASSERT(mNodes[0].getMass() > 0.0);
    CPPUNIT_ASSERT(!mArticle->mDisablePressureCorrection);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Define nominal trace compounds config data.
    DefinedChemicalCompounds definedCompounds;
    ChemicalCompound::Type tcTypes[2] = {ChemicalCompound::CO2, ChemicalCompound::H2O};
    GunnsFluidTraceCompoundsConfigData tcConfig(tcTypes, 2, "tcConfig");

    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;

    PolyFluidConfigData fluidConfig(mFluidProperties, types, 2, &tcConfig);

    /// - Define nominal trace compounds input data.
    double linkMoleFractions[2] = {1.0e-1, 2.0e-2};
    GunnsFluidTraceCompoundsInputData tcInput(linkMoleFractions);
    double nodeMoleFractions[2] = {0.0, 0.0};
    GunnsFluidTraceCompoundsInputData nodeTcInput(nodeMoleFractions);

    double fractions[2] = {0.5, 0.5};

    PolyFluidInputData fluidInput(300.0, 100.0, 1.0, 1.0, fractions, &tcInput);
    PolyFluidInputData nodeInput (300.0, 100.0, 1.0, 1.0, fractions, &nodeTcInput);

    /// - Setup up some new nodes with trace compounds.
    GunnsFluidNode nodes[2];
    mNodeList.mNodes = nodes;

    nodes[0].initialize("Node1", &fluidConfig);
    nodes[1].initialize("Node2", &fluidConfig);
    nodes[0].getContent()->initialize(fluidConfig, nodeInput);
    nodes[1].getContent()->initialize(fluidConfig, nodeInput);

    /// - Define nominal configuration & input data.
    GunnsFluidCapacitorConfigData configData(mLinkName, &mNodeList, 0.0);
    GunnsFluidCapacitorInputData  inputData(false, 0.0, mInitialVolume, &fluidInput);

    /// - Default construct and initialize (with nominal data) a test article with trace compounds.
    FriendlyGunnsFluidCapacitor article2;
    article2.initialize(configData, inputData, mLinks, mPort0, mPort1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(linkMoleFractions[0], nodes[0].getContent()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO2), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(linkMoleFractions[1], nodes[0].getContent()->getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O), DBL_EPSILON);
    CPPUNIT_ASSERT(article2.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidCapacitor .. 05: testInitializationExceptions ..........";

    /// @test for exception on negative volume
    mInputData->mInitialVolume = -0.5;

    CPPUNIT_ASSERT_THROW(mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    mInputData->mInitialVolume = mInitialVolume;

    /// @test for exception on null initial fluid state
    PolyFluidInputData* nullInputData = 0;
    mInputData->mInitialFluidState = nullInputData;
    CPPUNIT_ASSERT_THROW(mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mInitialFluidState = mFluidInput2;
    delete nullInputData;

    /// @test for exception on expansion scale factor < 0
    mConfigData->mExpansionScaleFactor = -0.3;
    CPPUNIT_ASSERT_THROW(mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    /// @test for exception on expansion scale factor > 1
    mConfigData->mExpansionScaleFactor = 1.1;
    CPPUNIT_ASSERT_THROW(mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::testAccessorsAndSetters()
{
    std::cout << "\n UtGunnsFluidCapacitor .. 06: testAccessorsAndSetters ...............";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialVolume, mArticle->getVolume(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getHeatFlux(), mTolerance);

    mArticle->mCapacitance = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getCapacitance(), 0.0);

    mArticle->editVolume(true, 42.0);
    CPPUNIT_ASSERT(true == mArticle->mEditVolumeFlag);
    CPPUNIT_ASSERT(42.0 == mArticle->mEditVolume);

    mNodes[0].mFluxThrough = 0.3;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3, mArticle->getFluxThrough(), 1.0E-06);

    CPPUNIT_ASSERT(0.0 == mArticle->getUndampedHeatFlux());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::testStep()
{
    std::cout << "\n UtGunnsFluidCapacitor .. 07: testStep ..............................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);

    /// - Capacitor was set up with positive pressure and volume so admittance and source terms
    ///   will be greater than zero
    CPPUNIT_ASSERT(mArticle->mAdmittanceMatrix[0] > 0.0 && mArticle->mSourceVector[0] > 0.0);

    /// - Test a volume edit
    const double density = mNodes[0].getContent()->getDensity();
    const double capacitance = mArticle->mCapacitance;

    mArticle->editVolume(true, 2.0);
    mArticle->step(mTimeStep);

    CPPUNIT_ASSERT(false == mArticle->mEditVolumeFlag);
    CPPUNIT_ASSERT(2.0   == mNodes[0].getVolume());
    CPPUNIT_ASSERT(0.0   == mNodes[0].computeCompression());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(density,           mNodes[0].getContent()->getDensity(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(capacitance * 4.0, mArticle->mCapacitance,               DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::testComputeFlows()
{
    std::cout << "\n UtGunnsFluidCapacitor .. 08: testComputeFlows ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Capacitance links should always have a zero flow rate
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mFlowRate, mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the step method when the dt argument is zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::testZeroDtStep()
{
    std::cout << "\n UtGunnsFluidCapacitor .. 09: testZeroDtStep ........................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->step(mTimeStep);
    mArticle->step(0.0);

    /// - With dt = 0, admittance matrix should be capacitance divided by epsilon.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mCapacitance / DBL_EPSILON,
            mArticle->mAdmittanceMatrix[0], mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the specific port assignment rules
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::testPortRules()
{
    std::cout << "\n UtGunnsFluidCapacitor .. 10: testPortRules .........................";

    /// - Must initialize first!
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Test for failure when assigning port 0 to the boundary node
    CPPUNIT_ASSERT(false == mArticle->checkSpecificPortRules(0, mNodeList.mNumNodes - 1));

    /// - Test for failure when assigning port 1 to other than the boundary node
    CPPUNIT_ASSERT(false == mArticle->checkSpecificPortRules(1, 0));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCapacitor::testRestart()
{
    std::cout << "\n UtGunnsFluidCapacitor .. 11: testRestart ...........................";

    /// - Must initialize first!
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    mArticle->mCapacitance    = 1.0;
    mArticle->mEditVolumeFlag = true;
    mArticle->mEditVolume     = 1.0;

    mArticle->restart();

    CPPUNIT_ASSERT(0.0   == mArticle->mCapacitance);
    CPPUNIT_ASSERT(false == mArticle->mEditVolumeFlag);
    CPPUNIT_ASSERT(0.0   == mArticle->mEditVolume);

    std::cout << "... Pass";
}
