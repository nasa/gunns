/*
@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "UtGunnsFluidDistributedIf.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidDistributedIf class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidDistributedIf::UtGunnsFluidDistributedIf()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tLinkName(),
    tIsPairMaster(),
    tUseEnthalpy(),
    tDemandOption(),
    tMalfBlockageFlag(),
    tMalfBlockageValue(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tTimeStep(),
    tFluidProperties(),
    tTcProperties(),
    tTcTypes(),
    tFluidTcConfig(),
    tLocalConfig(),
    tFluidInput1(),
    tCapacitorLink()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidDistributedIf class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidDistributedIf::~UtGunnsFluidDistributedIf()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput1;
    delete tLocalConfig;
    delete tFluidProperties;
    delete tFluidTcConfig;
    delete tTcProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::setUp()
{
    tLinkName               = "Test Fluid Distributed Interface";
    tNodeList.mNumNodes     = 2;
    tNodeList.mNodes        = tNodes;
    tIsPairMaster           = true;
    tUseEnthalpy            = true;
    tDemandOption           = true;
    tMalfBlockageFlag       = false;
    tMalfBlockageValue      = 1.0;
    tPort0                  = 0;
    tTimeStep               = 0.1;

    /// - Setup some fluid nodes
    tTcProperties  = new DefinedChemicalCompounds();
    tTcTypes[0]    = ChemicalCompound::CH2O;
    tTcTypes[1]    = ChemicalCompound::C2H6O;
    tTcTypes[2]    = ChemicalCompound::C4H4O;
    tTcTypes[3]    = ChemicalCompound::C8H10;
    tFluidTcConfig = new GunnsFluidTraceCompoundsConfigData(tTcTypes, 4, "tFluidTcConfig");

    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2] = {FluidProperties::GUNNS_N2, FluidProperties::GUNNS_O2};
    tLocalConfig = new PolyFluidConfigData(tFluidProperties, types, 2, tFluidTcConfig);
    double fractions[2] = {0.5, 0.5};
    tFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tLocalConfig, tFluidInput1);
    tNodes[1].initialize("UtTestNode1", tLocalConfig, tFluidInput1);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    /// - Define nominal configuration data
    tConfigData = new GunnsFluidDistributedIfConfigData(tLinkName,
                                                        &tNodeList,
                                                        tIsPairMaster,
                                                        tUseEnthalpy,
                                                        tDemandOption,
                                                        &tCapacitorLink);

    /// - Define nominal input data
    tInputData = new GunnsFluidDistributedIfInputData(tMalfBlockageFlag,
                                                      tMalfBlockageValue);

    tArticle = new FriendlyGunnsFluidDistributedIf;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidDistributedIf 01: testConfig ...........................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName       == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes          == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tIsPairMaster   == tConfigData->mIsPairMaster);
    CPPUNIT_ASSERT(tUseEnthalpy    == tConfigData->mUseEnthalpy);
    CPPUNIT_ASSERT(tDemandOption   == tConfigData->mDemandOption);
    CPPUNIT_ASSERT(&tCapacitorLink == tConfigData->mCapacitorLink);
    /// - Hardcoded values from the Reference: "Distributed Fluid Simulation Interface Standard"
    CPPUNIT_ASSERT(1.25            == tConfigData->mModingCapacitanceRatio);
    CPPUNIT_ASSERT(1.5             == tConfigData->mDemandFilterConstA);
    CPPUNIT_ASSERT(0.75            == tConfigData->mDemandFilterConstB);
    CPPUNIT_ASSERT(false           == tConfigData->mFluidSizesOverride);
    CPPUNIT_ASSERT(0               == tConfigData->mNumFluidOverride);
    CPPUNIT_ASSERT(0               == tConfigData->mNumTcOverride);

    /// - Check default config construction
    GunnsFluidDistributedIfConfigData defaultConfig;
    CPPUNIT_ASSERT(""              == defaultConfig.mName);
    CPPUNIT_ASSERT(0               == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(false           == defaultConfig.mIsPairMaster);
    CPPUNIT_ASSERT(false           == defaultConfig.mUseEnthalpy);
    CPPUNIT_ASSERT(false           == defaultConfig.mDemandOption);
    CPPUNIT_ASSERT(0               == defaultConfig.mCapacitorLink);
    /// - Hardcoded values from the Reference: "Distributed Fluid Simulation Interface Standard"
    CPPUNIT_ASSERT(1.25            == defaultConfig.mModingCapacitanceRatio);
    CPPUNIT_ASSERT(1.5             == defaultConfig.mDemandFilterConstA);
    CPPUNIT_ASSERT(0.75            == defaultConfig.mDemandFilterConstB);
    CPPUNIT_ASSERT(false           == defaultConfig.mFluidSizesOverride);
    CPPUNIT_ASSERT(0               == defaultConfig.mNumFluidOverride);
    CPPUNIT_ASSERT(0               == defaultConfig.mNumTcOverride);

    /// @test overrideInterfaceMixtureSizes function
    defaultConfig.overrideInterfaceMixtureSizes(5, 4);
    CPPUNIT_ASSERT(true            == defaultConfig.mFluidSizesOverride);
    CPPUNIT_ASSERT(5               == defaultConfig.mNumFluidOverride);
    CPPUNIT_ASSERT(4               == defaultConfig.mNumTcOverride);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testInput()
{
    std::cout << "\n UtGunnsFluidDistributedIf 02: testInput ............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);

    /// - Check default input construction
    GunnsFluidDistributedIfInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidDistributedIf 03: testDefaultConstruction ..............";

    /// @test state data
    CPPUNIT_ASSERT(false == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(false == tArticle->mUseEnthalpy);
    CPPUNIT_ASSERT(false == tArticle->mDemandOption);
    CPPUNIT_ASSERT(0     == tArticle->mCapacitorLink);
    CPPUNIT_ASSERT(0.0   == tArticle->mModingCapacitanceRatio);
    CPPUNIT_ASSERT(0.0   == tArticle->mDemandFilterConstA);
    CPPUNIT_ASSERT(0.0   == tArticle->mDemandFilterConstB);
    CPPUNIT_ASSERT(0     == tArticle->mInData.mFrameCount);
    CPPUNIT_ASSERT(0     == tArticle->mInData.mFrameLoopback);
    CPPUNIT_ASSERT(false == tArticle->mInData.mDemandMode);
    CPPUNIT_ASSERT(0.0   == tArticle->mInData.mCapacitance);
    CPPUNIT_ASSERT(0.0   == tArticle->mInData.mSource);
    CPPUNIT_ASSERT(0.0   == tArticle->mInData.mEnergy);
    CPPUNIT_ASSERT(0     == tArticle->mInData.mMoleFractions);
    CPPUNIT_ASSERT(0     == tArticle->mInData.mTcMoleFractions);
    CPPUNIT_ASSERT(false == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(0     == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(0     == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mCapacitance);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mSource);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mEnergy);
    CPPUNIT_ASSERT(0     == tArticle->mOutData.mMoleFractions);
    CPPUNIT_ASSERT(0     == tArticle->mOutData.mTcMoleFractions);
    CPPUNIT_ASSERT(0     == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(0.0   == tArticle->mSupplyVolume);
    CPPUNIT_ASSERT(0.0   == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0   == tArticle->mDemandFlux);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourcePressure);
    CPPUNIT_ASSERT(0.0   == tArticle->mDemandFlux);
    CPPUNIT_ASSERT(0     == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(0.0   == tArticle->mDemandFluxGain);
    CPPUNIT_ASSERT(0.0   == tArticle->mSuppliedCapacitance);
    CPPUNIT_ASSERT(0     == tArticle->mTempMassFractions);
    CPPUNIT_ASSERT(0     == tArticle->mOtherIfs.size());
    CPPUNIT_ASSERT(0.0   == tArticle->mFluidState.getTemperature());

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidDistributedIf* article = new GunnsFluidDistributedIf();
    delete article;

    GunnsFluidDistributedIfData* data = new GunnsFluidDistributedIfData();
    delete data;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidDistributedIf 04: testNominalInitialization ............";

    /// - Initialize a test article with nominal data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0));

    /// @test registering another interface link with the test article.
    GunnsFluidDistributedIf otherLink;
    CPPUNIT_ASSERT_NO_THROW(tArticle->addOtherIf(&otherLink));
    CPPUNIT_ASSERT(1          == tArticle->mOtherIfs.size());
    CPPUNIT_ASSERT(&otherLink == tArticle->mOtherIfs[0]);

    /// @test registering this with itself.
    CPPUNIT_ASSERT_NO_THROW(tArticle->addOtherIf(tArticle));
    CPPUNIT_ASSERT(1          == tArticle->mOtherIfs.size());
    CPPUNIT_ASSERT(&otherLink == tArticle->mOtherIfs[0]);

    /// @test registering a duplicate link.
    CPPUNIT_ASSERT_NO_THROW(tArticle->addOtherIf(&otherLink));
    CPPUNIT_ASSERT(1          == tArticle->mOtherIfs.size());
    CPPUNIT_ASSERT(&otherLink == tArticle->mOtherIfs[0]);

    /// @test config data.
    CPPUNIT_ASSERT(tLinkName       == tArticle->mName);
    CPPUNIT_ASSERT(tNodes          == tArticle->mNodeList->mNodes);
    CPPUNIT_ASSERT(tIsPairMaster   == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(tUseEnthalpy    == tArticle->mUseEnthalpy);
    CPPUNIT_ASSERT(tDemandOption   == tArticle->mDemandOption);
    CPPUNIT_ASSERT(&tCapacitorLink == tArticle->mCapacitorLink);
    /// - Hardcoded values from the Reference: "Distributed Fluid Simulation Interface Standard"
    CPPUNIT_ASSERT(1.25            == tArticle->mModingCapacitanceRatio);
    CPPUNIT_ASSERT(1.5             == tArticle->mDemandFilterConstA);
    CPPUNIT_ASSERT(0.75            == tArticle->mDemandFilterConstB);

    /// @test internal fluid.
    CPPUNIT_ASSERT(tArticle->mInternalFluid);

    /// @test temporary mass fractions array.
    CPPUNIT_ASSERT(tArticle->mTempMassFractions);
    CPPUNIT_ASSERT(0.0 == tArticle->mTempMassFractions[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mTempMassFractions[1]);

    /// @test interface data objects.
    CPPUNIT_ASSERT(0     == tArticle->mInData.mFrameCount);
    CPPUNIT_ASSERT(0     == tArticle->mInData.mFrameLoopback);
    CPPUNIT_ASSERT(false == tArticle->mInData.mDemandMode);
    CPPUNIT_ASSERT(0.0   == tArticle->mInData.mCapacitance);
    CPPUNIT_ASSERT(0.0   == tArticle->mInData.mSource);
    CPPUNIT_ASSERT(0.0   == tArticle->mInData.mEnergy);
    CPPUNIT_ASSERT(tArticle->mInData.mMoleFractions);
    CPPUNIT_ASSERT(0.0   == tArticle->mInData.mMoleFractions[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mInData.mMoleFractions[1]);
    CPPUNIT_ASSERT(tArticle->mInData.mTcMoleFractions);
    CPPUNIT_ASSERT(0.0   == tArticle->mInData.mTcMoleFractions[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mInData.mTcMoleFractions[1]);
    CPPUNIT_ASSERT(0.0   == tArticle->mInData.mTcMoleFractions[2]);
    CPPUNIT_ASSERT(0.0   == tArticle->mInData.mTcMoleFractions[3]);

    CPPUNIT_ASSERT(0     == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(0     == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mCapacitance);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mSource);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mEnergy);
    CPPUNIT_ASSERT(tArticle->mOutData.mMoleFractions);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mMoleFractions[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mMoleFractions[1]);
    CPPUNIT_ASSERT(tArticle->mOutData.mTcMoleFractions);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mTcMoleFractions[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mTcMoleFractions[1]);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mTcMoleFractions[2]);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mTcMoleFractions[3]);

    /// @test remaining state variables.
    CPPUNIT_ASSERT(0.0 == tArticle->mSupplyVolume);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mSourcePressure);
    CPPUNIT_ASSERT(0.0 == tArticle->mDemandFlux);
    CPPUNIT_ASSERT(0   == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(1.0 == tArticle->mDemandFluxGain);
    CPPUNIT_ASSERT(0.0 == tArticle->mSuppliedCapacitance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(700.728, tArticle->mFluidState.getPressure(),      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(283.15,  tArticle->mFluidState.getTemperature(),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,     tArticle->mFluidState.getMassFraction(0), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,     tArticle->mFluidState.getMassFraction(1), DBL_EPSILON);

    /// @test init flag.
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    /// @test init with zero number of trace compounds.
    FriendlyGunnsFluidDistributedIf article;
    tLocalConfig->mTraceCompounds = 0;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0));
    CPPUNIT_ASSERT(article.mInData.mMoleFractions);
    CPPUNIT_ASSERT(0.0 == article.mInData.mMoleFractions[1]);
    CPPUNIT_ASSERT(0   == article.mInData.mTcMoleFractions);

    /// @test init of GunnsFluidDistributedIfData with zero number of fluids.
    GunnsFluidDistributedIfData data;
    data.initialize("data", 0, 4, false, 0, 0);
    CPPUNIT_ASSERT(0 == data.mMoleFractions);
    CPPUNIT_ASSERT(data.mTcMoleFractions);
    CPPUNIT_ASSERT(0.0 == data.mTcMoleFractions[3]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidDistributedIf 05: testInitializationExceptions .........";

    /// @test Check port rules: initialize with port 0 set to the ground node and verify the link
    ///       fails to initialize.
    tPort0 = 1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0),
                         TsInitializationException);
    CPPUNIT_ASSERT(!tArticle->mInitFlag);
    tPort0 = 0;

    /// @test Exception on null capacitor link.
    tConfigData->mCapacitorLink = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0),
                         TsInitializationException);
    CPPUNIT_ASSERT(!tArticle->mInitFlag);
    tConfigData->mCapacitorLink = &tCapacitorLink;

    /// @test Exception on invalid moding capacitance ratio.
    tConfigData->mModingCapacitanceRatio = 1.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0),
                         TsInitializationException);
    tConfigData->mModingCapacitanceRatio = 1.25;

    /// @test Exception on conflicting mode force flags.
    tInputData->mForceDemandMode = true;
    tInputData->mForceSupplyMode = true;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0),
                         TsInitializationException);
    tInputData->mForceDemandMode = false;
    tInputData->mForceSupplyMode = false;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the processInputs method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testProcessInputs()
{
    std::cout << "\n UtGunnsFluidDistributedIf 06: testProcessInputs ....................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test with no incoming data.
    tArticle->mInDataLastDemandMode = true;
    tArticle->processInputs();
    CPPUNIT_ASSERT(true  == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourcePressure);
    CPPUNIT_ASSERT(0.0   == tArticle->mDemandFlux);
    CPPUNIT_ASSERT(1     == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(1     == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(0     == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(false == tArticle->mInData.hasValidData());
    double inFractions[2]   = {0.7, 0.3};
    double inTcFractions[4] = {1.0e-7, 2.0e-7, 3.0e-7, 4.0e-7};
    double inTcFractionSum  = inTcFractions[0] + inTcFractions[1] + inTcFractions[2] + inTcFractions[3];

    /// @test with incoming data, flip to demand mode due to equal capacitances and we are the pair
    ///       master.
    tArticle->mInData.mFrameCount         = 1;
    tArticle->mInData.mDemandMode         = false;
    tArticle->mInData.mCapacitance        = 1.0;
    tArticle->mInData.mSource             = 1.0e5;
    tArticle->mInData.mEnergy             = 3.0e5;
    tArticle->mInData.mMoleFractions[0]   = 0.7 / (1.0 + inTcFractionSum);
    tArticle->mInData.mMoleFractions[1]   = 0.3 / (1.0 + inTcFractionSum);
    tArticle->mInData.mTcMoleFractions[0] = 1.0e-7 / (1.0 + inTcFractionSum);
    tArticle->mInData.mTcMoleFractions[1] = 2.0e-7 / (1.0 + inTcFractionSum);
    tArticle->mInData.mTcMoleFractions[2] = 3.0e-7 / (1.0 + inTcFractionSum);
    tArticle->mInData.mTcMoleFractions[3] = 4.0e-7 / (1.0 + inTcFractionSum);
    tArticle->mOutData.mCapacitance       = 1.0;
    tArticle->mInDataLastDemandMode       = false;
    tArticle->mFramesSinceFlip            = 2;
    tNodes[0].setVolume(1.0);
    tArticle->processInputs();
    CPPUNIT_ASSERT(false == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(1.0   == tArticle->mSupplyVolume);
    CPPUNIT_ASSERT(true  == tCapacitorLink.mEditVolumeFlag);
    CPPUNIT_ASSERT(0.0   == tCapacitorLink.mEditVolume);
    CPPUNIT_ASSERT(0     == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(100.0 == tArticle->mSourcePressure);
    CPPUNIT_ASSERT(0.0   == tArticle->mDemandFlux);
    CPPUNIT_ASSERT(2     == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(2     == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(1     == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(true  == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(true  == tArticle->mInData.hasValidData());

    /// @test inputFluid() from the previous call to processInputs().
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7,    tNodes[0].getContent()->getMoleFraction(0),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3,    tNodes[0].getContent()->getMoleFraction(1),    DBL_EPSILON);
    // from mSource above, converted to kPa
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0,  tNodes[0].getContent()->getPressure(),         DBL_EPSILON);
    // from mEnergy above (J/kg)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0e5,  tNodes[0].getContent()->getSpecificEnthalpy(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0e-7, tNodes[0].getContent()->getTraceCompounds()->getMoleFractions()[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0e-7, tNodes[0].getContent()->getTraceCompounds()->getMoleFractions()[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0e-7, tNodes[0].getContent()->getTraceCompounds()->getMoleFractions()[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0e-7, tNodes[0].getContent()->getTraceCompounds()->getMoleFractions()[3], DBL_EPSILON);

    /// @test fluid state for users is driven by processInputs() in Demand mode.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7,    tArticle->mFluidState.getMoleFraction(0),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3,    tArticle->mFluidState.getMoleFraction(1),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0,  tArticle->mFluidState.getPressure(),         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0e5,  tArticle->mFluidState.getSpecificEnthalpy(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0e-7, tArticle->mFluidState.getTraceCompounds()->getMoleFractions()[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0e-7, tArticle->mFluidState.getTraceCompounds()->getMoleFractions()[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0e-7, tArticle->mFluidState.getTraceCompounds()->getMoleFractions()[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0e-7, tArticle->mFluidState.getTraceCompounds()->getMoleFractions()[3], DBL_EPSILON);

    /// @test flip to supply mode when incoming data takes over demand.
    tArticle->mInData.mFrameCount  = 2;
    tArticle->mInData.mDemandMode  = true;
    tArticle->mInData.mCapacitance = 1.0;
    tArticle->mInData.mSource      = -1.0;
    tArticle->mFramesSinceFlip     = 999;
    tCapacitorLink.editVolume(false, 0.0);
    double expectedFlux = -tArticle->mInData.mSource * (1.0 - inTcFractionSum) / 1000.0;
    tArticle->processInputs();
    CPPUNIT_ASSERT(true   == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(false  == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(true   == tCapacitorLink.mEditVolumeFlag);
    CPPUNIT_ASSERT(1.0    == tCapacitorLink.mEditVolume);
    CPPUNIT_ASSERT(0.0    == tArticle->mSupplyVolume);
    CPPUNIT_ASSERT(0      == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(0.0    == tArticle->mSourcePressure);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlux, tArticle->mDemandFlux, 1.0e-14);
    CPPUNIT_ASSERT(3      == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(3      == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(2      == tArticle->mOutData.mFrameLoopback);

    /// @test inputFluid() from the previous call to processInputs().
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7,    tArticle->mInternalFluid->getMoleFraction(0),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3,    tArticle->mInternalFluid->getMoleFraction(1),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,    tArticle->mInternalFluid->getPressure(),         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0e5,  tArticle->mInternalFluid->getSpecificEnthalpy(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0e-7, tArticle->mInternalFluid->getTraceCompounds()->getMoleFractions()[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0e-7, tArticle->mInternalFluid->getTraceCompounds()->getMoleFractions()[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0e-7, tArticle->mInternalFluid->getTraceCompounds()->getMoleFractions()[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0e-7, tArticle->mInternalFluid->getTraceCompounds()->getMoleFractions()[3], DBL_EPSILON);

    /// @test handles zero incoming bulk fluid mole fractions.
    tArticle->mInData.mMoleFractions[0] = 0.0;
    tArticle->mInData.mMoleFractions[1] = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->processInputs(), TsOutOfBoundsException);
    tArticle->mInData.mMoleFractions[0] = 0.7 / (1.0 + inTcFractionSum);
    tArticle->mInData.mMoleFractions[1] = 0.3 / (1.0 + inTcFractionSum);

    /// @test with both sides in supply mode, remain in supply mode due to equal capacitances and we
    ///       are not the pair master.
    tArticle->mInData.mDemandMode = false;
    tArticle->mInData.mSource     = 1.0e5;
    tArticle->mIsPairMaster       = false;
    tArticle->processInputs();
    CPPUNIT_ASSERT(false == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourcePressure);

    /// @test with both sides in supply mode, remain in supply mode due to greater capacitance.
    tArticle->mOutData.mCapacitance = 2.0;
    tArticle->processInputs();
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);

    /// @test with both sides in supply mode, remain in supply mode due to greater capacitance and
    ///       we are the pair master.
    tArticle->mIsPairMaster = true;
    tArticle->processInputs();
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);

    /// @test with both sides in supply mode, take demand mode due to lesser capacitance.
    tArticle->mOutData.mCapacitance = 0.5;
    tArticle->processInputs();
    CPPUNIT_ASSERT(true  == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(100.0 == tArticle->mSourcePressure);

    /// @test remain in demand mode when incoming data is supply mode.
    tArticle->processInputs();
    CPPUNIT_ASSERT(true == tArticle->mOutData.mDemandMode);

    /// @test remain in demand mode when both sides are in demand mode and other side has not
    ///       recently flipped to demand mode -- this allows time for the other side to relinquish
    ///       demand mode after we took it, in the presence of data lag.
    tArticle->mInData.mDemandMode   = true;
    tArticle->mInDataLastDemandMode = true;
    tArticle->processInputs();
    CPPUNIT_ASSERT(true == tArticle->mOutData.mDemandMode);

    /// @test Input of fluid temperature.
    tArticle->mUseEnthalpy        = false;
    tArticle->mInData.mDemandMode = false;
    tArticle->mInData.mEnergy     = 300.0;
    tArticle->processInputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, tNodes[0].getContent()->getTemperature(), FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the processOutput methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testProcessOutputs()
{
    std::cout << "\n UtGunnsFluidDistributedIf 07: testProcessOutputs ...................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test outputs in demand mode.
    tArticle->mOutData.mDemandMode = true;
    tArticle->mSuppliedCapacitance = 1.0;
    tArticle->mFlux = 0.001;
    tNodes[0].setNetworkCapacitance(2.0);
    double inFractions[2]   = {0.7, 0.3};
    double inTcFractions[4] = {1.0e-7, 2.0e-7, 3.0e-7, 4.0e-7};
    double inTcFractionSum  = inTcFractions[0] + inTcFractions[1] + inTcFractions[2] + inTcFractions[3];
    double expectedSource   = tArticle->mFlux * 1000.0 * (1.0 + inTcFractionSum);
    GunnsFluidTraceCompoundsInputData inTcInput(inTcFractions);
    PolyFluidInputData inFluidInput(275.0, 110.0, 0.0, 0.0, inFractions, &inTcInput);
    PolyFluid inFluid(*tLocalConfig, inFluidInput);
    tNodes[0].collectInflux(0.001, &inFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0e-7, tNodes[0].getInflow()->getTraceCompounds()->getMoleFractions()[0], DBL_EPSILON);
    tArticle->processOutputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,            tArticle->mOutData.mCapacitance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSource, tArticle->mOutData.mSource,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inFluid.getSpecificEnthalpy(), tArticle->mOutData.mEnergy, DBL_EPSILON);
    CPPUNIT_ASSERT(0.7 == tArticle->mTempMassFractions[0]);
    CPPUNIT_ASSERT(0.3 == tArticle->mTempMassFractions[1]);
    {
        const double tempX[2]           = {0.7 / 28.0134, 0.3 / 31.9988};  // MW of N2 & O2
        const double expectedN2Fraction = tempX[0] / (tempX[0] + tempX[1]) / (1.0 + inTcFractionSum);
        const double expectedO2Fraction = tempX[1] / (tempX[0] + tempX[1]) / (1.0 + inTcFractionSum);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedN2Fraction, tArticle->mOutData.mMoleFractions[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2Fraction, tArticle->mOutData.mMoleFractions[1], DBL_EPSILON);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0e-7 / (1.0 + inTcFractionSum), tArticle->mOutData.mTcMoleFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0e-7 / (1.0 + inTcFractionSum), tArticle->mOutData.mTcMoleFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0e-7 / (1.0 + inTcFractionSum), tArticle->mOutData.mTcMoleFractions[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0e-7 / (1.0 + inTcFractionSum), tArticle->mOutData.mTcMoleFractions[3], DBL_EPSILON);

    /// @test output in demand mode when the node inflow has negative mixture fractions.
    tNodes[0].resetFlows();
    inFluid.setMass(0,  0.5);
    inFluid.setMass(1, -0.5);
    inFluid.updateMass();
    tNodes[0].collectInflux(0.001, &inFluid);
    tArticle->processOutputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getSpecificEnthalpy(), tArticle->mOutData.mEnergy, DBL_EPSILON);
    CPPUNIT_ASSERT(0.5 == tArticle->mTempMassFractions[0]);
    CPPUNIT_ASSERT(0.5 == tArticle->mTempMassFractions[1]);

    /// @test output in demand mode when the node has no inflow.
    tArticle->mFlux = 0.0;
    tNodes[0].resetFlows();
    tArticle->processOutputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getSpecificEnthalpy(), tArticle->mOutData.mEnergy, DBL_EPSILON);
    CPPUNIT_ASSERT(0.5 == tArticle->mTempMassFractions[0]);
    CPPUNIT_ASSERT(0.5 == tArticle->mTempMassFractions[1]);

    /// @test outputs in supply mode, without flipping to demand mode.
    tNodes[0].setPotential(tFluidInput1->mPressure);
    tArticle->mInData.mDemandMode  = true;
    tArticle->mInData.mCapacitance = 0.5;
    tArticle->mOutData.mDemandMode = false;
    tArticle->mUseEnthalpy         = false;
    tArticle->mSuppliedCapacitance = 0.0;
    expectedSource                 = tFluidInput1->mPressure * 1000.0;
    tArticle->processOutputs();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,                        tArticle->mOutData.mCapacitance, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSource,             tArticle->mOutData.mSource,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidInput1->mTemperature, tArticle->mOutData.mEnergy,      DBL_EPSILON);
    //temp mass fractions, mole fractions, tc fractions
    CPPUNIT_ASSERT(0.5 == tArticle->mTempMassFractions[0]);
    CPPUNIT_ASSERT(0.5 == tArticle->mTempMassFractions[1]);
    {
        const double tempX[2]           = {0.5 / 28.0134, 0.5 / 31.9988};  // MW of N2 & O2
        const double expectedN2Fraction = tempX[0] / (tempX[0] + tempX[1]);
        const double expectedO2Fraction = tempX[1] / (tempX[0] + tempX[1]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedN2Fraction, tArticle->mOutData.mMoleFractions[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2Fraction, tArticle->mOutData.mMoleFractions[1], DBL_EPSILON);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mOutData.mTcMoleFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mOutData.mTcMoleFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mOutData.mTcMoleFractions[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mOutData.mTcMoleFractions[3], DBL_EPSILON);

    /// @test fluid state for users is driven by processOutputs() in Supply mode.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMoleFraction(0),                         tArticle->mFluidState.getMoleFraction(0),                         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMoleFraction(1),                         tArticle->mFluidState.getMoleFraction(1),                         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getPressure(),                              tArticle->mFluidState.getPressure(),                              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(),                           tArticle->mFluidState.getTemperature(),                           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTraceCompounds()->getMoleFractions()[0], tArticle->mFluidState.getTraceCompounds()->getMoleFractions()[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTraceCompounds()->getMoleFractions()[1], tArticle->mFluidState.getTraceCompounds()->getMoleFractions()[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTraceCompounds()->getMoleFractions()[2], tArticle->mFluidState.getTraceCompounds()->getMoleFractions()[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTraceCompounds()->getMoleFractions()[3], tArticle->mFluidState.getTraceCompounds()->getMoleFractions()[3], DBL_EPSILON);

    /// @test not flipping to demand mode due to low capacitance when frames since flip is small.
    tArticle->mInData.mCapacitance = 4.0;
    tArticle->mFramesSinceFlip     = 4;
    tArticle->mLoopLatency         = 4;
    tArticle->processOutputs();
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSource, tArticle->mOutData.mSource, DBL_EPSILON);

    /// @test flip to demand mode due to low capacitance.
    tArticle->mFramesSinceFlip = 5;
    tArticle->processOutputs();
    CPPUNIT_ASSERT(true == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mOutData.mSource, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Additional tests for the outputCapacitance method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testOutputCapacitance()
{
    std::cout << "\n UtGunnsFluidDistributedIf 08: testOutputCapacitance ................";

    FriendlyGunnsFluidDistributedIf otherIf;
    const double otherSuppliedCap = 0.2;
    int otherNodeMap[1] = {1};
    otherIf.mNodeMap = otherNodeMap;
    const double netCapDp[2]     = {0.5, 0.1};
    const double netCapDpNc[2]   = {0.5, 0.0};
    const double netCapDpZero[2] = {0.0, 1.0};
    tNodes[0].setNetCapDeltaPotential(netCapDpNc);
    tNodes[0].setNetworkCapacitance(1.0);

    /// @test other interface not in demand mode.
    tArticle->addOtherIf(&otherIf);
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
    tArticle->outputCapacitance();
    double expectedC = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedC, tArticle->mOutData.mCapacitance, DBL_EPSILON);

    /// @test other interface in demand mode but no network conductance connection.
    otherIf.mSuppliedCapacitance = otherSuppliedCap;
    CPPUNIT_ASSERT_EQUAL(otherSuppliedCap, otherIf.getSuppliedCapacitance());
    tArticle->outputCapacitance();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedC, tArticle->mOutData.mCapacitance, DBL_EPSILON);

    /// @test other interface in demand mode and connected.
    tNodes[0].setNetCapDeltaPotential(netCapDp);
    tArticle->outputCapacitance();
    expectedC = 1.0 - otherSuppliedCap * netCapDp[1] / netCapDp[0];
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedC, tArticle->mOutData.mCapacitance, DBL_EPSILON);

    /// @test our DP is zero.
    tNodes[0].setNetCapDeltaPotential(netCapDpZero);
    tArticle->outputCapacitance();
    expectedC = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedC, tArticle->mOutData.mCapacitance, DBL_EPSILON);

    otherIf.mNodeMap = 0;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testStep()
{
    std::cout << "\n UtGunnsFluidDistributedIf 09: testStep .............................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test outputs in demand mode, with demand option on.
    tArticle->mOutData.mDemandMode  = true;
    tArticle->mOutData.mCapacitance = 0.5;
    tArticle->mInData.mCapacitance  = 1.0;
    tArticle->mLoopLatency          = 4;
    tArticle->mDemandFlux           = 0.001;
    tArticle->mSourcePressure       = 100.0;
    const double timestep           = 0.1;
    double csOverCd     = 1.25; // default moding capacitance ratio upper limit
    double gainLimit    = 1.5 * powf(0.75, 4); // default demand filter connstants A & B
    double expectedGain = gainLimit + (1.0 - gainLimit) * (csOverCd - 1.0) * 4.0;
    double conductance  = expectedGain * 1.0 / timestep;
    double expectedG    = conductance; // mDemandOption is set in config data
    double expectedW    = expectedG * 100.0 + 0.001;
    double expectedCap  = expectedG * timestep;
    tArticle->step(timestep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGain, tArticle->mDemandFluxGain,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,    tArticle->mEffectiveConductivity,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,    tArticle->mAdmittanceMatrix[0],     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,    tArticle->mSourceVector[0],         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCap,  tArticle->mSuppliedCapacitance,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCap,  tArticle->getSuppliedCapacitance(), DBL_EPSILON);
    CPPUNIT_ASSERT(true   == tArticle->needAdmittanceUpdate());
    CPPUNIT_ASSERT(1.0e-6 == tNodes[0].getNetworkCapacitanceRequest());

    /// @test gain limits in demand mode with Cs < Cd and loop latency 1.
    tArticle->mOutData.mCapacitance = 1.0;
    tArticle->mInData.mCapacitance  = 0.9;
    tArticle->mLoopLatency          = 1;
    csOverCd  = 1.0; // default moding capacitance ratio lower limit
    gainLimit = 1.0; // upper limit on gain limit
    expectedGain = gainLimit;
    conductance  = expectedGain * 0.9 / timestep;
    expectedG    = conductance;
    tArticle->step(timestep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGain, tArticle->mDemandFluxGain,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,    tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// @test limits on latency exponent.
    tArticle->mLoopLatency = 0;
    tArticle->step(timestep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGain, tArticle->mDemandFluxGain,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,    tArticle->mEffectiveConductivity, DBL_EPSILON);

    tArticle->mLoopLatency = 200;
    gainLimit    = 1.5 * powf(0.75, 100);
    expectedGain = gainLimit + (1.0 - gainLimit) * (csOverCd - 1.0) * 4.0;
    conductance  = expectedGain * 0.9 / timestep;
    expectedG    = conductance;
    tArticle->step(timestep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGain, tArticle->mDemandFluxGain,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,    tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// @test demand mode with 1 < Cs/Cd < 1.25.
    tArticle->mOutData.mCapacitance = 1.0;
    tArticle->mInData.mCapacitance  = 1.15;
    tArticle->mLoopLatency          = 2;
    csOverCd     = 1.15;
    gainLimit    = 1.5 * powf(0.75, 2);
    expectedGain = gainLimit + (1.0 - gainLimit) * (csOverCd - 1.0) * 4.0;
    conductance  = expectedGain * 1.15 / timestep;
    expectedG    = conductance;
    tArticle->step(timestep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGain, tArticle->mDemandFluxGain,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,    tArticle->mEffectiveConductivity, DBL_EPSILON);

    /// @test outputs in demand mode with demand option turned off.
    tArticle->mDemandOption         = false;
    tArticle->mOutData.mCapacitance = 0.5;
    tArticle->mInData.mCapacitance  = 1.0;
    tArticle->mLoopLatency          = 4;
    csOverCd     = 1.25;
    gainLimit    = 1.5 * powf(0.75, 4);
    expectedGain = gainLimit + (1.0 - gainLimit) * (csOverCd - 1.0) * 4.0;
    conductance  = expectedGain * 1.0 / timestep;
    expectedG    = 1.0 / (1.0 / conductance + timestep / 0.5);
    expectedW    = expectedG * 100.0 + 0.001;
    expectedCap  = expectedG * timestep;
    tArticle->step(timestep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGain, tArticle->mDemandFluxGain,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,    tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,    tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW,    tArticle->mSourceVector[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCap,  tArticle->mSuppliedCapacitance,   DBL_EPSILON);
    CPPUNIT_ASSERT(true   == tArticle->needAdmittanceUpdate());
    CPPUNIT_ASSERT(1.0e-6 == tNodes[0].getNetworkCapacitanceRequest());

    /// @test 2nd step with same admittance.
    tArticle->step(timestep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,    tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());

    /// @test incoming capacitance is zero.
    tArticle->mInData.mCapacitance = 0.0;
    tArticle->step(timestep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mDemandFluxGain,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSuppliedCapacitance,   DBL_EPSILON);
    CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());

    /// @test outgoing capacitance is zero, and include blockage malfunction.
    tArticle->mInData.mCapacitance  = 1.0;
    tArticle->mOutData.mCapacitance = 0.0;
    tArticle->mMalfBlockageFlag     = true;
    tArticle->mMalfBlockageValue    = 0.5;
    expectedG   = 0.5 * 1.0 * 1.0 / timestep;
    expectedCap = expectedG * timestep;
    tArticle->step(timestep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,         tArticle->mDemandFluxGain,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,   tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG,   tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCap, tArticle->mSuppliedCapacitance,   DBL_EPSILON);
    CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());

    /// @test zero timestep.
    tArticle->step(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSuppliedCapacitance,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());

    /// @test supply mode.
    tArticle->mOutData.mDemandMode = false;
    tArticle->step(timestep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mDemandFluxGain,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAdmittanceMatrix[0],   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSuppliedCapacitance,   DBL_EPSILON);
    CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testComputeFlows()
{
    std::cout << "\n UtGunnsFluidDistributedIf 10: testComputeFlows .....................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test flow into the node.
    tArticle->mPotentialVector[0]  = 100.0;
    tArticle->mAdmittanceMatrix[0] = 1.0e-5;
    tArticle->mSourceVector[0]     = 0.002;
    double expectedW = -100 * 1.0e-5 + 0.002;
    tArticle->computeFlows(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-100.0,    tArticle->mPotentialDrop,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mFlux,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tNodes[0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);

    /// @test flow out of the node, supply mode.
    tNodes[0].resetFlows();
    tArticle->mSourceVector[0]     = 0.0;
    expectedW = -100.0 * 1.0e-5;
    tArticle->computeFlows(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-100.0,     tArticle->mPotentialDrop,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedW, tArticle->mFlux,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,        tNodes[0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK == tArticle->mPortDirections[0]);

    /// @test flow out of the node, demand mode.
    tNodes[0].resetFlows();
    tArticle->mSourceVector[0]     = 0.0;
    tArticle->mOutData.mDemandMode = true;
    expectedW = -100.0 * 1.0e-5;
    tArticle->computeFlows(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-100.0,     tArticle->mPotentialDrop,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedW, tArticle->mFlux,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedW, tNodes[0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tArticle->mPortDirections[0]);

    /// @test zero flow.
    tNodes[0].resetFlows();
    tArticle->mAdmittanceMatrix[0] = 0.0;
    expectedW = 0.0;
    tArticle->computeFlows(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-100.0,    tArticle->mPotentialDrop,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mFlux,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tNodes[0].getScheduledOutflux(), DBL_EPSILON);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE == tArticle->mPortDirections[0]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for transport flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testTransportFlows()
{
    std::cout << "\n UtGunnsFluidDistributedIf 11: testTransportFlows ...................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test forward flow in demand mode.
    tArticle->mOutData.mDemandMode = true;
    double flux                    = 0.001;
    tArticle->mFlux                = flux;
    double expectedMW              = tNodes[0].getContent()->getMWeight();
    double expectedMdot            = flux * expectedMW;
    tArticle->transportFlows(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tNodes[0].getInflux(),               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tNodes[0].getOutflux(),              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW,   tNodes[0].getInflow()->getMWeight(), DBL_EPSILON);

    /// @test reverse flow in demand mode.
    flux            = -0.001;
    tArticle->mFlux = flux;
    expectedMdot    = flux * expectedMW;
    tNodes[0].resetFlows();
    tArticle->transportFlows(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedMdot, tArticle->mFlowRate,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,          tNodes[0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedMdot, tNodes[0].getOutflux(), DBL_EPSILON);

    /// @test zero flow in demand mode.
    flux            = 0.0;
    tArticle->mFlux = flux;
    tNodes[0].resetFlows();
    tArticle->transportFlows(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlowRate,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getOutflux(), DBL_EPSILON);

    /// @test forward flow in supply mode.
    tArticle->mOutData.mDemandMode = false;
    flux                           = 0.001;
    tArticle->mFlux                = flux;
    expectedMW                     = tArticle->mInternalFluid->getMWeight();
    expectedMdot                   = flux * expectedMW;
    tNodes[0].resetFlows();
    tArticle->transportFlows(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tNodes[0].getInflux(),               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tNodes[0].getOutflux(),              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW,   tNodes[0].getInflow()->getMWeight(), DBL_EPSILON);

    /// @test reverse flow in supply mode.
    flux            = -0.001;
    tArticle->mFlux = flux;
    expectedMW      = tNodes[0].getContent()->getMWeight();
    expectedMdot    = flux * expectedMW;
    tNodes[0].resetFlows();
    tArticle->transportFlows(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tArticle->mFlowRate,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMdot, tNodes[0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tNodes[0].getOutflux(), DBL_EPSILON);

    /// @test zero flow in supply mode.
    flux            = 0.0;
    tArticle->mFlux = flux;
    tNodes[0].resetFlows();
    tArticle->transportFlows(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlowRate,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getInflux(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getOutflux(), DBL_EPSILON);


    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testRestart()
{
    std::cout << "\n UtGunnsFluidDistributedIf 12: testRestart ..........................";

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);

    /// @test Resetting of non-config and non-checkpointed data.
    tArticle->mEffectiveConductivity = 1.0;
    tArticle->mSourcePressure        = 1.0;
    tArticle->mDemandFlux            = 1.0;
    tArticle->mLoopLatency           = 1;
    tArticle->mDemandFluxGain        = 0.0;
    tArticle->mSuppliedCapacitance   = 1.0;
    tArticle->mTempMassFractions[0]  = 1.0;

    tArticle->restart();

    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);
    CPPUNIT_ASSERT(0.0 == tArticle->mSourcePressure);
    CPPUNIT_ASSERT(0.0 == tArticle->mDemandFlux);
    CPPUNIT_ASSERT(0   == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(1.0 == tArticle->mDemandFluxGain);
    CPPUNIT_ASSERT(0.0 == tArticle->mSuppliedCapacitance);
    CPPUNIT_ASSERT(0.0 == tArticle->mTempMassFractions[0]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the GunnsFluidDistributedIfData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testData()
{
    std::cout << "\n UtGunnsFluidDistributedIf 13: testData .............................";

    /// @test Default constructor.
    FriendlyGunnsFluidDistributedIfData data;
    CPPUNIT_ASSERT(0     == data.mFrameCount);
    CPPUNIT_ASSERT(0     == data.mFrameLoopback);
    CPPUNIT_ASSERT(false == data.mDemandMode);
    CPPUNIT_ASSERT(0.0   == data.mCapacitance);
    CPPUNIT_ASSERT(0.0   == data.mSource);
    CPPUNIT_ASSERT(0.0   == data.mEnergy);
    CPPUNIT_ASSERT(0     == data.mMoleFractions);
    CPPUNIT_ASSERT(0     == data.mTcMoleFractions);
    CPPUNIT_ASSERT(0     == data.mNumFluid);
    CPPUNIT_ASSERT(0     == data.mNumTc);
    CPPUNIT_ASSERT(0     == data.mNumFluidIf);
    CPPUNIT_ASSERT(0     == data.mNumTcIf);
    CPPUNIT_ASSERT(0     == data.mNumFluidCommon);
    CPPUNIT_ASSERT(0     == data.mNumTcCommon);

    /// @test initialize method.
    CPPUNIT_ASSERT_NO_THROW(data.initialize("data", 3, 2, false, 0, 0));
    CPPUNIT_ASSERT(0     == data.mFrameCount);
    CPPUNIT_ASSERT(0     == data.mFrameLoopback);
    CPPUNIT_ASSERT(false == data.mDemandMode);
    CPPUNIT_ASSERT(0.0   == data.mCapacitance);
    CPPUNIT_ASSERT(0.0   == data.mSource);
    CPPUNIT_ASSERT(0.0   == data.mEnergy);
    CPPUNIT_ASSERT(0     != data.mMoleFractions);
    CPPUNIT_ASSERT(0.0   == data.mMoleFractions[0]);
    CPPUNIT_ASSERT(0.0   == data.mMoleFractions[1]);
    CPPUNIT_ASSERT(0.0   == data.mMoleFractions[2]);
    CPPUNIT_ASSERT(0     != data.mTcMoleFractions);
    CPPUNIT_ASSERT(0.0   == data.mTcMoleFractions[0]);
    CPPUNIT_ASSERT(0.0   == data.mTcMoleFractions[1]);
    CPPUNIT_ASSERT(3     == data.mNumFluid);
    CPPUNIT_ASSERT(2     == data.mNumTc);
    CPPUNIT_ASSERT(3     == data.mNumFluidIf);
    CPPUNIT_ASSERT(2     == data.mNumTcIf);
    CPPUNIT_ASSERT(3     == data.mNumFluidCommon);
    CPPUNIT_ASSERT(2     == data.mNumTcCommon);

    /// @test hasValidData method.
    data.mFrameCount  = 1;
    data.mEnergy      = 1.0;
    data.mSource      = -1.0;
    data.mDemandMode  = true;
    CPPUNIT_ASSERT(true == data.hasValidData());
    data.mMoleFractions[2] = -1.0;
    CPPUNIT_ASSERT(false == data.hasValidData());
    data.mMoleFractions[2] = 0.0;
    data.mTcMoleFractions[1] = -1.0;
    CPPUNIT_ASSERT(false == data.hasValidData());
    data.mTcMoleFractions[1] = 0.0;
    data.mFrameCount  = 0;
    CPPUNIT_ASSERT(false == data.hasValidData());
    data.mFrameCount  = 1;
    data.mCapacitance = -1.0;
    CPPUNIT_ASSERT(false == data.hasValidData());
    data.mCapacitance = 0.0;
    data.mEnergy      = 0.0;
    CPPUNIT_ASSERT(false == data.hasValidData());
    data.mEnergy      = 1.0;
    data.mDemandMode  = false;
    CPPUNIT_ASSERT(false == data.hasValidData());
    data.mSource      = 0.0;
    CPPUNIT_ASSERT(true == data.hasValidData());

    /// @test Assignment operator before initialization.
    data.mFrameCount         = 3;
    data.mFrameLoopback      = 2;
    data.mDemandMode         = true;
    data.mCapacitance        = 1.0;
    data.mSource             = 2.0;
    data.mEnergy             = 3.0;
    data.mMoleFractions[0]   = 0.5;
    data.mMoleFractions[1]   = 0.4;
    data.mMoleFractions[2]   = 0.09;
    data.mTcMoleFractions[0] = 0.009;
    data.mTcMoleFractions[1] = 0.001;

    FriendlyGunnsFluidDistributedIfData data2;
    data2 = data;
    CPPUNIT_ASSERT(3    == data2.mFrameCount);
    CPPUNIT_ASSERT(2    == data2.mFrameLoopback);
    CPPUNIT_ASSERT(true == data2.mDemandMode);
    CPPUNIT_ASSERT(1.0  == data2.mCapacitance);
    CPPUNIT_ASSERT(2.0  == data2.mSource);
    CPPUNIT_ASSERT(3.0  == data2.mEnergy);
    CPPUNIT_ASSERT(0    == data2.mMoleFractions);
    CPPUNIT_ASSERT(0    == data2.mTcMoleFractions);
    CPPUNIT_ASSERT(0    == data2.mNumFluid);
    CPPUNIT_ASSERT(0    == data2.mNumTc);
    CPPUNIT_ASSERT(0    == data2.mNumFluidIf);
    CPPUNIT_ASSERT(0    == data2.mNumTcIf);
    CPPUNIT_ASSERT(0    == data2.mNumFluidCommon);
    CPPUNIT_ASSERT(0    == data2.mNumTcCommon);

    /// @test Assignment operator after initialization.
    FriendlyGunnsFluidDistributedIfData data3;
    CPPUNIT_ASSERT_NO_THROW(data3.initialize("data3", 3, 2, false, 0, 0));
    data3 = data;
    CPPUNIT_ASSERT(3     == data3.mFrameCount);
    CPPUNIT_ASSERT(2     == data3.mFrameLoopback);
    CPPUNIT_ASSERT(true  == data3.mDemandMode);
    CPPUNIT_ASSERT(1.0   == data3.mCapacitance);
    CPPUNIT_ASSERT(2.0   == data3.mSource);
    CPPUNIT_ASSERT(3.0   == data3.mEnergy);
    CPPUNIT_ASSERT(0.5   == data3.mMoleFractions[0]);
    CPPUNIT_ASSERT(0.4   == data3.mMoleFractions[1]);
    CPPUNIT_ASSERT(0.09  == data3.mMoleFractions[2]);
    CPPUNIT_ASSERT(0.009 == data3.mTcMoleFractions[0]);
    CPPUNIT_ASSERT(0.001 == data3.mTcMoleFractions[1]);
    CPPUNIT_ASSERT(3     == data3.mNumFluid);
    CPPUNIT_ASSERT(2     == data3.mNumTc);
    CPPUNIT_ASSERT(3     == data3.mNumFluidIf);
    CPPUNIT_ASSERT(2     == data3.mNumTcIf);
    CPPUNIT_ASSERT(3     == data3.mNumFluidCommon);
    CPPUNIT_ASSERT(2     == data3.mNumTcCommon);

    /// @test initialization with mixture array size overrides.
    FriendlyGunnsFluidDistributedIfData data4, data5;
    CPPUNIT_ASSERT_NO_THROW(data4.initialize("data4", 3, 2, true, 2, 1));
    CPPUNIT_ASSERT_NO_THROW(data5.initialize("data5", 2, 1, true, 3, 2));

    CPPUNIT_ASSERT(0     != data4.mMoleFractions);
    CPPUNIT_ASSERT(0.0   == data4.mMoleFractions[0]);
    CPPUNIT_ASSERT(0.0   == data4.mMoleFractions[1]);
    CPPUNIT_ASSERT(0     != data4.mTcMoleFractions);
    CPPUNIT_ASSERT(0.0   == data4.mTcMoleFractions[0]);
    CPPUNIT_ASSERT(3     == data4.mNumFluid);
    CPPUNIT_ASSERT(2     == data4.mNumTc);
    CPPUNIT_ASSERT(2     == data4.mNumFluidIf);
    CPPUNIT_ASSERT(1     == data4.mNumTcIf);
    CPPUNIT_ASSERT(2     == data4.mNumFluidCommon);
    CPPUNIT_ASSERT(1     == data4.mNumTcCommon);

    CPPUNIT_ASSERT(0     != data5.mMoleFractions);
    CPPUNIT_ASSERT(0.0   == data5.mMoleFractions[0]);
    CPPUNIT_ASSERT(0.0   == data5.mMoleFractions[1]);
    CPPUNIT_ASSERT(0.0   == data5.mMoleFractions[2]);
    CPPUNIT_ASSERT(0     != data5.mTcMoleFractions);
    CPPUNIT_ASSERT(0.0   == data5.mTcMoleFractions[0]);
    CPPUNIT_ASSERT(0.0   == data5.mTcMoleFractions[1]);
    CPPUNIT_ASSERT(2     == data5.mNumFluid);
    CPPUNIT_ASSERT(1     == data5.mNumTc);
    CPPUNIT_ASSERT(3     == data5.mNumFluidIf);
    CPPUNIT_ASSERT(2     == data5.mNumTcIf);
    CPPUNIT_ASSERT(2     == data5.mNumFluidCommon);
    CPPUNIT_ASSERT(1     == data5.mNumTcCommon);

    /// @test setMoleFractions function, interface size = model size.
    double fractions3[3] = {0.2, 0.3, 0.5};
    data.setMoleFractions(fractions3);
    CPPUNIT_ASSERT(fractions3[0] == data.mMoleFractions[0]);
    CPPUNIT_ASSERT(fractions3[1] == data.mMoleFractions[1]);
    CPPUNIT_ASSERT(fractions3[2] == data.mMoleFractions[2]);

    /// @test setMoleFractions function, interface size < model size.
    data4.setMoleFractions(fractions3);
    CPPUNIT_ASSERT(fractions3[0] == data4.mMoleFractions[0]);
    CPPUNIT_ASSERT(fractions3[1] == data4.mMoleFractions[1]);

    /// @test setMoleFractions function, interface size > model size.
    data5.setMoleFractions(fractions3);
    CPPUNIT_ASSERT(fractions3[0] == data5.mMoleFractions[0]);
    CPPUNIT_ASSERT(fractions3[1] == data5.mMoleFractions[1]);
    CPPUNIT_ASSERT(0.0           == data5.mMoleFractions[2]);

    /// @test setTcMoleFractions function, interface size = model size.
    data.setTcMoleFractions(fractions3);
    CPPUNIT_ASSERT(fractions3[0] == data.mTcMoleFractions[0]);
    CPPUNIT_ASSERT(fractions3[1] == data.mTcMoleFractions[1]);

    /// @test setTcMoleFractions function, interface size < model size.
    data4.setTcMoleFractions(fractions3);
    CPPUNIT_ASSERT(fractions3[0] == data4.mTcMoleFractions[0]);

    /// @test setTcMoleFractions function, interface size > model size.
    data5.setTcMoleFractions(fractions3);
    CPPUNIT_ASSERT(fractions3[0] == data5.mTcMoleFractions[0]);
    CPPUNIT_ASSERT(0.0           == data5.mTcMoleFractions[1]);

    /// @test getMoleFractions function, interface size = model size.
    double fractions4[3] = {0.0, 0.0, 0.0};
    data.getMoleFractions(fractions4);
    CPPUNIT_ASSERT(data.mMoleFractions[0] == fractions4[0]);
    CPPUNIT_ASSERT(data.mMoleFractions[1] == fractions4[1]);
    CPPUNIT_ASSERT(data.mMoleFractions[2] == fractions4[2]);

    /// @test getMoleFractions function, interface size < model size.
    data4.getMoleFractions(fractions4);
    CPPUNIT_ASSERT(data4.mMoleFractions[0] == fractions4[0]);
    CPPUNIT_ASSERT(data4.mMoleFractions[1] == fractions4[1]);
    CPPUNIT_ASSERT(0.0                     == fractions4[2]);

    /// @test getMoleFractions function, interface size > model size.
    data5.getMoleFractions(fractions4);
    CPPUNIT_ASSERT(data5.mMoleFractions[0] == fractions4[0]);
    CPPUNIT_ASSERT(data5.mMoleFractions[1] == fractions4[1]);
    CPPUNIT_ASSERT(0.0                     == fractions4[2]);

    /// @test getTcMoleFractions function, interface size = model size.
    data.getTcMoleFractions(fractions4);
    CPPUNIT_ASSERT(data.mTcMoleFractions[0] == fractions4[0]);
    CPPUNIT_ASSERT(data.mTcMoleFractions[1] == fractions4[1]);
    CPPUNIT_ASSERT(0.0                      == fractions4[2]);

    /// @test getTcMoleFractions function, interface size < model size.
    data4.getTcMoleFractions(fractions4);
    CPPUNIT_ASSERT(data4.mTcMoleFractions[0] == fractions4[0]);
    CPPUNIT_ASSERT(0.0                       == fractions4[1]);
    CPPUNIT_ASSERT(0.0                       == fractions4[2]);

    /// @test getTcMoleFractions function, interface size > model size.
    data5.getTcMoleFractions(fractions4);
    CPPUNIT_ASSERT(data5.mTcMoleFractions[0] == fractions4[0]);
    CPPUNIT_ASSERT(0.0                       == fractions4[1]);
    CPPUNIT_ASSERT(0.0                       == fractions4[2]);

    /// @test Data objects are public in GunnsFluidDistributedIf.
    GunnsFluidDistributedIf article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0);
    CPPUNIT_ASSERT(false == article.mInData.hasValidData());
    CPPUNIT_ASSERT(false == article.mOutData.hasValidData());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the mode forcing flags.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidDistributedIf::testForceModes()
{
    std::cout << "\n UtGunnsFluidDistributedIf 14: testForceModes .......................";

    /// @test Starts in Supply mode after initialization regardless of Demand force flag.
    tInputData->mForceDemandMode = true;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0);
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);

    /// @test Flips to Demand mode on input regardless of Demand mode in incoming data.
    tNodes[0].setVolume(1.0);
    tArticle->mInData.mDemandMode = true;
    tArticle->mInData.mFrameCount = 1;
    tArticle->processInputs();
    CPPUNIT_ASSERT(1.0   == tArticle->mSupplyVolume);
    CPPUNIT_ASSERT(true  == tCapacitorLink.mEditVolumeFlag);
    CPPUNIT_ASSERT(0.0   == tCapacitorLink.mEditVolume);
    CPPUNIT_ASSERT(0     == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(1     == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(true  == tArticle->mOutData.mDemandMode);

    /// @test Flips to Supply mode on input regardless of capacitance.
    tArticle->mInData.mDemandMode = false;
    tArticle->mInData.mFrameCount = 2;
    tArticle->mForceDemandMode    = false;
    tArticle->mForceSupplyMode    = true;
    tArticle->processInputs();
    CPPUNIT_ASSERT(0.0   == tArticle->mSupplyVolume);
    CPPUNIT_ASSERT(true  == tCapacitorLink.mEditVolumeFlag);
    CPPUNIT_ASSERT(1.0   == tCapacitorLink.mEditVolume);
    CPPUNIT_ASSERT(0     == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(2     == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);

    /// @test Stays in Supply mode on output regardless of new local capacitance.
    tArticle->mLoopLatency          = 2;
    tArticle->mFramesSinceFlip      = 3;
    tArticle->mOutData.mCapacitance = 0.0;
    tArticle->mInData.mCapacitance  = 1.0;
    tArticle->flipModesOnCapacitance();
    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);

    std::cout << "... Pass";
}
