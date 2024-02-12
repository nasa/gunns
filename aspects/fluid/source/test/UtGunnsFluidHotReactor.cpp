/**
@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidHotReactor.hh"

/// @details  Test identification number.
int UtGunnsFluidHotReactor::TEST_ID = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Hot Reactor link model unit test.
///////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHotReactor::UtGunnsFluidHotReactor()
    :
    mFluidProperties(),
    mTypes(),
    mFractions(),
    mFluidConfig(),
    mFluidInput(),
    mLinks(),
    mName(),
    mNodes(),
    mNodeList(),
    mPort0(0),
    mPort1(0),
    mMaxConductivity(0.0),
    mExpansionScaleFactor(0.0),
    mInput(),
    mOutput(),
    mReactions(),
    mReactionTypes(),
    mCompounds(),
    mCompoundTypes(),
    mThermalLength(0.0),
    mThermalDiameter(0.0),
    mSurfaceRoughness(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mEfficiencyBias(0.0),
    mEfficiencyScaleFactor(0.0),
    mConfigData(),
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mTotalMasses(),
    mWallTemperature(0.0),
    mInputData(),
    mFluidTemperature(0.0),
    mWallHeatFlux(0.0),
    mArticle(),
    mFlowRate(),
    mTimeStep(),
    mTolerance()
{
    // do nothing
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hot Reactor link model unit test.
///////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHotReactor::~UtGunnsFluidHotReactor()
{
    // do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotReactor::setUp()
{
    /// - Define the nominal port fluids.
    mFluidProperties       = new DefinedFluidProperties();
    mTypes[0]              = FluidProperties::GUNNS_N2;  mFractions[0] = 0.750;
    mTypes[1]              = FluidProperties::GUNNS_NH3; mFractions[1] = 0.010;
    mTypes[2]              = FluidProperties::GUNNS_CH4; mFractions[2] = 0.002;
    mTypes[3]              = FluidProperties::GUNNS_H2;  mFractions[3] = 0.000;
    mTypes[4]              = FluidProperties::GUNNS_O2;  mFractions[4] = 0.200;
    mTypes[5]              = FluidProperties::GUNNS_H2O; mFractions[5] = 0.010;
    mTypes[6]              = FluidProperties::GUNNS_CO;  mFractions[6] = 0.008;
    mTypes[7]              = FluidProperties::GUNNS_CO2; mFractions[7] = 0.020;
    mFluidConfig           = new PolyFluidConfigData(mFluidProperties,
                                                     mTypes,
                                                     UtGunnsFluidHotReactor::N_CONSTITUENTS);
    mFluidInput            = new PolyFluidInputData(300.0,
                                                    140000.0,
                                                    1.0,
                                                    1.0,
                                                    mFractions);

    /// - Initialize the nodes.
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, *mFluidInput);
    mNodes[1].getContent()->initialize(*mFluidConfig, *mFluidInput);

    /// - Initialize the nodes list.
    mNodeList.mNodes       = mNodes;
    mNodeList.mNumNodes    = N_NODES;

    /// - Define the nominal port mapping.
    mPort0                 = 0;
    mPort1                 = 1;

    /// - Define the nominal configuration data.
    mName                  = "nominal";
    mMaxConductivity       =  1.5;
    mExpansionScaleFactor  =  0.0;
    mCompoundTypes[0]      = ChemicalCompound::NH42HPO4;   mTotalMasses[0] = 0.5;
    mCompoundTypes[1]      = ChemicalCompound::H3PO4;      mTotalMasses[1] = 0.5;
    mCompoundTypes[2]      = ChemicalCompound::NH3;        mTotalMasses[2] = 0.0;
    mCompoundTypes[3]      = ChemicalCompound::CH4;        mTotalMasses[3] = 0.0;
    mCompoundTypes[4]      = ChemicalCompound::H2;         mTotalMasses[4] = 0.0;
    mCompoundTypes[5]      = ChemicalCompound::O2;         mTotalMasses[5] = 0.0;
    mCompoundTypes[6]      = ChemicalCompound::H2O;        mTotalMasses[6] = 0.0;
    mCompoundTypes[7]      = ChemicalCompound::CO;         mTotalMasses[7] = 0.0;
    mCompoundTypes[8]      = ChemicalCompound::CO2;        mTotalMasses[8] = 0.0;
    mReactionTypes[0]      = ChemicalReaction::NH3_REMOVAL;    ///<  2NH3 + H3PO4 --> (NH4)2(HPO4)
    mReactionTypes[1]      = ChemicalReaction::H2_REMOVAL;     ///<  2H2 + O2 --> 2H2O
    mReactionTypes[2]      = ChemicalReaction::CH4_REMOVAL;    ///<  CH4 + 2O2 --> CO2 + 2H2O
    mReactionTypes[3]      = ChemicalReaction::CO_REMOVAL;     ///<  2CO + O2 --> 2CO2
    mThermalLength         = 1.0;
    mThermalDiameter       = 0.1;
    mSurfaceRoughness      = 1.0e-03;
    mThermalSurfaceArea    = UnitConversion::PI_UTIL * mThermalDiameter * mThermalLength;
    mThermalROverD         = mSurfaceRoughness / mThermalDiameter;
    mEfficiencyBias        = 0.25;
    mEfficiencyScaleFactor = 0.75 / 700.0;
    mConfigData            = new GunnsFluidHotReactorConfigData(mName,
                                                                &mNodeList,
                                                                mMaxConductivity,
                                                                mExpansionScaleFactor,
                                                                &mReactions,
                                                                mReactionTypes,
                                                                UtGunnsFluidHotReactor::N_REACTIONS,
                                                                &mCompounds,
                                                                mCompoundTypes,
                                                                UtGunnsFluidHotReactor::N_COMPOUNDS,
                                                                mThermalLength,
                                                                mThermalDiameter,
                                                                mSurfaceRoughness,
                                                                mEfficiencyBias,
                                                                mEfficiencyScaleFactor);

    /// - Define the nominal input data.
    mMalfBlockageFlag      = false;
    mMalfBlockageValue     = 0.3;
    mWallTemperature       = 700.0;
    mInputData             = new GunnsFluidHotReactorInputData(mMalfBlockageFlag,
                                                               mMalfBlockageValue,
                                                               mTotalMasses,
                                                               mWallTemperature);

    /// - Default construct the nominal test article.
    mFluidTemperature     = 0.5 * (mWallTemperature + mFluidInput->mTemperature);
    mWallHeatFlux         = 0.0;
    mArticle = new FriendlyGunnsFluidHotReactor();

    /// - Define the nominal flow rate, time step and comparison tolerance.
    mFlowRate              = 1.0;
    mTimeStep              = 0.1;
    mTolerance             = 1.0e-06;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotReactor::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput;
    delete mFluidConfig;
    delete mFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Reactor link model construction of configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotReactor::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName                                           == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                                          == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,                    mConfigData->mMaxConductivity,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,               mConfigData->mExpansionScaleFactor,   0.0);
    for (int i = 0; i < N_REACTIONS; i++) {
        ChemicalReaction::Type type = static_cast<ChemicalReaction::Type>(i);
        CPPUNIT_ASSERT(mReactions.getReaction(type)                == mConfigData->mReactions->getReaction(type));
    }
    CPPUNIT_ASSERT(N_REACTIONS                                     == mConfigData->mNReactions);
    for (int i = 0; i < N_COMPOUNDS; i++) {
        ChemicalCompound::Type type = static_cast<ChemicalCompound::Type>(i);
        CPPUNIT_ASSERT(mCompounds.getCompound(type)                == mConfigData->mCompounds->getCompound(type));
    }
    CPPUNIT_ASSERT(N_COMPOUNDS                                     == mConfigData->mNCompounds);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalLength,                      mConfigData->mThermalLength,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,                    mConfigData->mThermalDiameter,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSurfaceRoughness,                   mConfigData->mSurfaceRoughness,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiencyBias,                     mConfigData->mEfficiencyBias,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiencyScaleFactor,              mConfigData->mEfficiencyScaleFactor,  0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag                               == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,                  mInputData->mMalfBlockageValue,       0.0);
    for (int i = 0; i < N_COMPOUNDS; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mTotalMasses[i],                 mInputData->mTotalMasses[i],          mTolerance);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,                    mInputData->mWallTemperature,         0.0);

    /// @test    Configuration data default construction.
     GunnsFluidHotReactorConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                               == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                               == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT(0                                               == defaultConfig.mReactions);
    CPPUNIT_ASSERT(0                                               == defaultConfig.mNReactions);
    CPPUNIT_ASSERT(0                                               == defaultConfig.mCompounds);
    CPPUNIT_ASSERT(0                                               == defaultConfig.mNCompounds);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mThermalLength,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mThermalDiameter,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mSurfaceRoughness,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mEfficiencyBias,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultConfig.mEfficiencyScaleFactor, 0.0);

    /// @test    Input data default construction.
    GunnsFluidHotReactorInputData defaultInput;
    CPPUNIT_ASSERT(                                                  !defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultInput.mMalfBlockageValue,      0.0);
    CPPUNIT_ASSERT(0                                               == defaultInput.mTotalMasses);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                 defaultInput.mWallTemperature,        0.0);

    /// @test    Input data copy construction.
    GunnsFluidHotReactorInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag                   == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,      copyInput.mMalfBlockageValue,         0.0);
    for (int i = 0; i < N_COMPOUNDS; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mTotalMasses[i],     copyInput.mTotalMasses[i],            mTolerance);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mWallTemperature,        copyInput.mWallTemperature,           0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Reactor link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotReactor::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(""              == mArticle->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMaxConductivity,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mExpansionScaleFactor,      0.0);
    CPPUNIT_ASSERT(0               == mArticle->mReactions);
    CPPUNIT_ASSERT(0               == mArticle->mNReactions);
    CPPUNIT_ASSERT(0               == mArticle->mCompounds);
    CPPUNIT_ASSERT(0               == mArticle->mNCompounds);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mThermalDiameter,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mThermalSurfaceArea,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mThermalROverD,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mEfficiencyBias,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mEfficiencyScaleFactor,     0.0);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(!mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMalfBlockageValue,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,           mArticle->mWallTemperature, 0.0);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT(0               == mArticle->mInternalFluid);
    CPPUNIT_ASSERT(0               == mArticle->mReactantsFluid);
    CPPUNIT_ASSERT(0               == mArticle->mProductsFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mFluidTemperature,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mWallHeatFlux,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mReactantsFlowRate,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mProductsFlowRate,          0.0);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidHotReactor* article = new GunnsFluidHotReactor();
    delete article;

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Reactor link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotReactor::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName                                                 == mArticle->mName);
    CPPUNIT_ASSERT(&mNodes[0]                                            == mArticle->mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1]                                            == mArticle->mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,                          mArticle->mMaxConductivity,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,                     mArticle->mExpansionScaleFactor,   0.0);
    CPPUNIT_ASSERT(ChemicalReaction::NH3_REMOVAL                         == mArticle->mReactions[0].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::NH3_REMOVAL) == mArticle->mReactions[0].mReaction);
    CPPUNIT_ASSERT(2                                                     == mArticle->mReactions[0].mReactantIndex);
    CPPUNIT_ASSERT(1                                                     == mArticle->mReactions[0].mReagentIndex);
    CPPUNIT_ASSERT(0                                                     == mArticle->mReactions[0].mProduct1Index);
    CPPUNIT_ASSERT(-1                                                    == mArticle->mReactions[0].mProduct2Index);
    CPPUNIT_ASSERT(ChemicalReaction::H2_REMOVAL                          == mArticle->mReactions[1].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::H2_REMOVAL)  == mArticle->mReactions[1].mReaction);
    CPPUNIT_ASSERT(4                                                     == mArticle->mReactions[1].mReactantIndex);
    CPPUNIT_ASSERT(5                                                     == mArticle->mReactions[1].mReagentIndex);
    CPPUNIT_ASSERT(6                                                     == mArticle->mReactions[1].mProduct1Index);
    CPPUNIT_ASSERT(-1                                                    == mArticle->mReactions[1].mProduct2Index);
    CPPUNIT_ASSERT(ChemicalReaction::CH4_REMOVAL                         == mArticle->mReactions[2].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::CH4_REMOVAL) == mArticle->mReactions[2].mReaction);
    CPPUNIT_ASSERT(3                                                     == mArticle->mReactions[2].mReactantIndex);
    CPPUNIT_ASSERT(5                                                     == mArticle->mReactions[2].mReagentIndex);
    CPPUNIT_ASSERT(8                                                     == mArticle->mReactions[2].mProduct1Index);
    CPPUNIT_ASSERT(6                                                     == mArticle->mReactions[2].mProduct2Index);
    CPPUNIT_ASSERT(ChemicalReaction::CO_REMOVAL                          == mArticle->mReactions[3].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::CO_REMOVAL)  == mArticle->mReactions[3].mReaction);
    CPPUNIT_ASSERT(7                                                     == mArticle->mReactions[3].mReactantIndex);
    CPPUNIT_ASSERT(5                                                     == mArticle->mReactions[3].mReagentIndex);
    CPPUNIT_ASSERT(8                                                     == mArticle->mReactions[3].mProduct1Index);
    CPPUNIT_ASSERT(-1                                                    == mArticle->mReactions[3].mProduct2Index);
    CPPUNIT_ASSERT(N_REACTIONS                                           == mArticle->mNReactions);
    CPPUNIT_ASSERT(N_COMPOUNDS                                           == mArticle->mNCompounds);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiencyBias,                           mArticle->mEfficiencyBias,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiencyScaleFactor,                    mArticle->mEfficiencyScaleFactor,  0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mDiameter,                                 mArticle->mDiameter,               0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSurfaceArea,                              mArticle->mSurfaceArea,            0.0);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mROverD,                                   mArticle->mROverD,                 0.0);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mMalfBlockageFlag                                     == mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,                        mArticle->mMalfBlockageValue,      0.0);
    for (int i = 0; i < N_COMPOUNDS; i++) {
        const ChemicalCompound::Type type = mCompoundTypes[i];
        CPPUNIT_ASSERT(type                                             == mArticle->mCompounds[i].mType);
        CPPUNIT_ASSERT(mCompounds.getCompound(type)                     == mArticle->mCompounds[i].mCompound);
        if (i >= 2) {
            CPPUNIT_ASSERT(i-1                                          == mArticle->mCompounds[i].mIndex);
        } else {
            CPPUNIT_ASSERT(-1                                           == mArticle->mCompounds[i].mIndex);
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mTotalMasses[i],                      mArticle->mCompounds[i].mTotalMass, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                  mArticle->mCompounds[i].mMass,      mTolerance);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,                         mArticle->mWallTemperature,         0.0);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0                                                    != mArticle->mInternalFluid);
    CPPUNIT_ASSERT(0                                                    != mArticle->mReactantsFluid);
    CPPUNIT_ASSERT(0                                                    != mArticle->mProductsFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      mArticle->mFlowRate,                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      mArticle->mReactantsFlowRate,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      mArticle->mProductsFlowRate,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      mArticle->mWallHeatFlux,            mTolerance);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(                                                        mArticle->mInitFlag);

    /// @test    Verify restartModel Functionality
    mArticle->mFluidTemperature  = 1.0;
    mArticle->mReactantsFlowRate = 1.0;
    mArticle->mProductsFlowRate  = 1.0;

    mArticle->restartModel();

    CPPUNIT_ASSERT(0.0 == mArticle->mFluidTemperature);
    CPPUNIT_ASSERT(0.0 == mArticle->mReactantsFlowRate);
    CPPUNIT_ASSERT(0.0 == mArticle->mProductsFlowRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Reactor link model duplicate initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotReactor::testMultipleInitialization()
{
    UT_RESULT;

    /// - Initialize a default test article with off-nominal config and input data
    FriendlyGunnsFluidHotReactor article;
    GunnsFluidHotReactorConfigData configData("off-nominal",
                                              &mNodeList,
                                              mMaxConductivity,
                                              mExpansionScaleFactor,
                                              &mReactions,
                                              mReactionTypes,
                                              UtGunnsFluidHotReactor::N_REACTIONS - 2,
                                              &mCompounds,
                                              mCompoundTypes,
                                              UtGunnsFluidHotReactor::N_COMPOUNDS - 2,
                                              mEfficiencyBias,
                                              mEfficiencyScaleFactor,
                                              mThermalDiameter,
                                              mThermalLength,
                                              mSurfaceRoughness);
    double totalMasses[N_COMPOUNDS - 2];
    totalMasses[0] = 0.1;
    totalMasses[1] = 0.2;
    totalMasses[2] = 0.3;
    totalMasses[3] = 0.4;
    totalMasses[4] = 0.5;
    totalMasses[5] = 0.6;
    totalMasses[6] = 0.7;
    GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag,
                                            mMalfBlockageValue,
                                            totalMasses,
                                            mWallTemperature);
    article.initialize(configData, inputData, mLinks, mPort1, mPort0);

    /// - Initialize test article again with nominal configuration and input data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName                                                 == mArticle->mName);
    CPPUNIT_ASSERT(&mNodes[0]                                            == mArticle->mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1]                                            == mArticle->mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,                          mArticle->mMaxConductivity,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,                     mArticle->mExpansionScaleFactor,   0.0);
    CPPUNIT_ASSERT(ChemicalReaction::NH3_REMOVAL                         == mArticle->mReactions[0].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::NH3_REMOVAL) == mArticle->mReactions[0].mReaction);
    CPPUNIT_ASSERT(2                                                     == mArticle->mReactions[0].mReactantIndex);
    CPPUNIT_ASSERT(1                                                     == mArticle->mReactions[0].mReagentIndex);
    CPPUNIT_ASSERT(0                                                     == mArticle->mReactions[0].mProduct1Index);
    CPPUNIT_ASSERT(-1                                                    == mArticle->mReactions[0].mProduct2Index);
    CPPUNIT_ASSERT(ChemicalReaction::H2_REMOVAL                          == mArticle->mReactions[1].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::H2_REMOVAL)  == mArticle->mReactions[1].mReaction);
    CPPUNIT_ASSERT(4                                                     == mArticle->mReactions[1].mReactantIndex);
    CPPUNIT_ASSERT(5                                                     == mArticle->mReactions[1].mReagentIndex);
    CPPUNIT_ASSERT(6                                                     == mArticle->mReactions[1].mProduct1Index);
    CPPUNIT_ASSERT(-1                                                    == mArticle->mReactions[1].mProduct2Index);
    CPPUNIT_ASSERT(ChemicalReaction::CH4_REMOVAL                         == mArticle->mReactions[2].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::CH4_REMOVAL) == mArticle->mReactions[2].mReaction);
    CPPUNIT_ASSERT(3                                                     == mArticle->mReactions[2].mReactantIndex);
    CPPUNIT_ASSERT(5                                                     == mArticle->mReactions[2].mReagentIndex);
    CPPUNIT_ASSERT(8                                                     == mArticle->mReactions[2].mProduct1Index);
    CPPUNIT_ASSERT(6                                                     == mArticle->mReactions[2].mProduct2Index);
    CPPUNIT_ASSERT(ChemicalReaction::CO_REMOVAL                          == mArticle->mReactions[3].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::CO_REMOVAL)  == mArticle->mReactions[3].mReaction);
    CPPUNIT_ASSERT(7                                                     == mArticle->mReactions[3].mReactantIndex);
    CPPUNIT_ASSERT(5                                                     == mArticle->mReactions[3].mReagentIndex);
    CPPUNIT_ASSERT(8                                                     == mArticle->mReactions[3].mProduct1Index);
    CPPUNIT_ASSERT(-1                                                    == mArticle->mReactions[3].mProduct2Index);
    CPPUNIT_ASSERT(N_REACTIONS                                           == mArticle->mNReactions);
    CPPUNIT_ASSERT(N_COMPOUNDS                                           == mArticle->mNCompounds);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,                          mArticle->mThermalDiameter,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalSurfaceArea,                       mArticle->mThermalSurfaceArea,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalROverD,                            mArticle->mThermalROverD,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiencyBias,                           mArticle->mEfficiencyBias,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiencyScaleFactor,                    mArticle->mEfficiencyScaleFactor,  0.0);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mMalfBlockageFlag                                     == mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,                        mArticle->mMalfBlockageValue,      0.0);
    for (int i = 0; i < N_COMPOUNDS; i++) {
        const ChemicalCompound::Type type = mCompoundTypes[i];
        CPPUNIT_ASSERT(type                                             == mArticle->mCompounds[i].mType);
        CPPUNIT_ASSERT(mCompounds.getCompound(type)                     == mArticle->mCompounds[i].mCompound);
        if (i >= 2) {
            CPPUNIT_ASSERT(i-1                                          == mArticle->mCompounds[i].mIndex);
        } else {
            CPPUNIT_ASSERT(-1                                           == mArticle->mCompounds[i].mIndex);
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mTotalMasses[i],                      mArticle->mCompounds[i].mTotalMass, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                  mArticle->mCompounds[i].mMass,      mTolerance);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,                         mArticle->mWallTemperature,      0.0);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0                                                    != mArticle->mInternalFluid);
    CPPUNIT_ASSERT(0                                                    != mArticle->mReactantsFluid);
    CPPUNIT_ASSERT(0                                                    != mArticle->mProductsFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      mArticle->mFlowRate,                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      mArticle->mReactantsFlowRate,       mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      mArticle->mProductsFlowRate,        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      mArticle->mWallHeatFlux,            mTolerance);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(                                                        mArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Reactor link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotReactor::testAccess()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort1, mPort0);

    /// @test    Nothing (this derived class does not add or override any accessors).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Reactor link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotReactor::testModify()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort1, mPort0);

    /// @test    Nothing (this derived class does not add or override any modifiers).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Reactor link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotReactor::testUpdateFluid()
{
    UT_RESULT;

    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort1, mPort0);
    mArticle->mNReactions           = 1;
    const double maxRate            = mReactions.getReaction(ChemicalReaction::NH3_REMOVAL)->mMaxReactantRate;
    const double efficiency         = mEfficiencyBias + mEfficiencyScaleFactor * mWallTemperature;
    const double moleRatioNH42HPO4  = mReactions.getReaction(ChemicalReaction::NH3_REMOVAL)->mProduct1MoleRatio;
    const double moleRatioH3PO4     = mReactions.getReaction(ChemicalReaction::NH3_REMOVAL)->mReagentMoleRatio;
    const double moleRatioNH3       = mReactions.getReaction(ChemicalReaction::NH3_REMOVAL)->mReactantMoleRatio;
    const double mWeightNH42HPO4    = mCompounds.getCompound(ChemicalCompound::NH42HPO4)-> mMWeight;
    const double mWeightH3PO4       = mCompounds.getCompound(ChemicalCompound::H3PO4)-> mMWeight;
    const double mWeightNH3         = mCompounds.getCompound(ChemicalCompound::NH3)-> mMWeight;
    const int    NH42HPO4           = 0;
    const int    H3PO4              = 1;
    const int    NH3                = 2;
    const int    index              = mNodes[0].getContent()->find(FluidProperties::GUNNS_NH3);

    {
        /// @test     Reaction constrained by reactant unavailability.
        const double initialNH42HPO4Mass          =  0.0;
        const double initialH3PO4Mass             =  100.0;
        const double initialNH3Mass               =  0.0;
        mArticle->mCompounds[NH42HPO4].mTotalMass =  initialNH42HPO4Mass;
        mArticle->mCompounds[H3PO4].mTotalMass    =  initialH3PO4Mass;
        mNodes[0].getOutflow()->setMass(index, initialNH3Mass);
        mNodes[0].getOutflow()->updateMass();
        mArticle->mCompounds[NH42HPO4].mMass      =  0.0;
        mArticle->mCompounds[H3PO4].mMass         =  0.0;
        mArticle->mCompounds[NH3].mMass           =  0.0;
        const double expectedNH42HPO4TotalMass    =  initialNH42HPO4Mass;
        const double expectedH3PO4TotalMass       =  initialH3PO4Mass;
        const double expectedNH3TotalMass         =  initialNH3Mass;
        const double expectedNH42HPO4Mass         =  0.0;
        const double expectedH3PO4Mass            =  0.0;
        const double expectedNH3Mass              =  0.0;
        mArticle->updateFluid(mTimeStep, 0.0);
        const double returnedNH42HPO4TotalMass    =  mArticle->mCompounds[NH42HPO4].mTotalMass;
        const double returnedH3PO4TotalMass       =  mArticle->mCompounds[H3PO4].mTotalMass;
        const double returnedNH3TotalMass         =  mArticle->mCompounds[NH3].mTotalMass;
        const double returnedNH42HPO4Mass         =  mArticle->mCompounds[NH42HPO4].mMass;
        const double returnedH3PO4Mass            =  mArticle->mCompounds[H3PO4].mMass;
        const double returnedNH3Mass              =  mArticle->mCompounds[NH3].mMass;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3Mass,           returnedNH3Mass,           mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3Mass,           returnedNH3Mass,           mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH3PO4Mass,         returnedH3PO4Mass,         mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH42HPO4Mass,      returnedNH42HPO4Mass,      mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3TotalMass,      returnedNH3TotalMass,      mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH3PO4TotalMass,    returnedH3PO4TotalMass,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH42HPO4TotalMass, returnedNH42HPO4TotalMass, mTolerance);
    } {
        /// @test     Reaction constrained by efficiency.
        const double initialNH42HPO4Mass          =  0.0;
        const double initialH3PO4Mass             =  100.0;
        const double initialNH3Mass               =  0.5 * maxRate * mTimeStep;
        mArticle->mCompounds[NH42HPO4].mTotalMass =  initialNH42HPO4Mass;
        mArticle->mCompounds[H3PO4].mTotalMass    =  initialH3PO4Mass;
        mArticle->mCompounds[NH3].mTotalMass      =  initialNH3Mass;
        mArticle->mInternalFluid->resetState();
        mArticle->mInternalFluid->setMass(index, 1.0);
        mArticle->mInternalFluid->updateMass();
        mArticle->mInternalFluid->setTemperature(mWallTemperature);
        mArticle->mFluidTemperature               =  mWallTemperature;
        mArticle->mCompounds[NH42HPO4].mMass      =  0.0;
        mArticle->mCompounds[H3PO4].mMass         =  0.0;
        mArticle->mCompounds[NH3].mMass           =  0.0;
        const double expectedNH3Mass              = -efficiency * initialNH3Mass;
        const double expectedH3PO4Mass            =  expectedNH3Mass / mWeightNH3 *
                                                     moleRatioH3PO4 / moleRatioNH3 *
                                                     mWeightH3PO4;
        const double expectedNH42HPO4Mass         = -expectedNH3Mass / mWeightNH3 *
                                                     moleRatioNH42HPO4 / moleRatioNH3 *
                                                     mWeightNH42HPO4;
        const double expectedNH42HPO4TotalMass    =  expectedNH42HPO4Mass;
        const double expectedH3PO4TotalMass       =  initialH3PO4Mass + expectedH3PO4Mass;
        const double expectedNH3TotalMass         =  initialNH3Mass   + expectedNH3Mass;
        mArticle->mFlowRate                       = 0.5 * maxRate;
        mArticle->updateFluid(mTimeStep, mFlowRate);
        const double returnedNH42HPO4TotalMass    =  mArticle->mCompounds[NH42HPO4].mTotalMass;
        const double returnedH3PO4TotalMass       =  mArticle->mCompounds[H3PO4].mTotalMass;
        const double returnedNH3TotalMass         =  mArticle->mCompounds[NH3].mTotalMass;
        const double returnedNH42HPO4Mass         =  mArticle->mCompounds[NH42HPO4].mMass;
        const double returnedH3PO4Mass            =  mArticle->mCompounds[H3PO4].mMass;
        const double returnedNH3Mass              =  mArticle->mCompounds[NH3].mMass;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3Mass,           returnedNH3Mass,           mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3Mass,           returnedNH3Mass,           mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH3PO4Mass,         returnedH3PO4Mass,         mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH42HPO4Mass,      returnedNH42HPO4Mass,      mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3TotalMass,      returnedNH3TotalMass,      mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH3PO4TotalMass,    returnedH3PO4TotalMass,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH42HPO4TotalMass, returnedNH42HPO4TotalMass, mTolerance);
    } {
        /// @test     Reaction constrained by reagent efficiency override.
        const double initialNH42HPO4Mass          =  0.0;
        const double initialH3PO4Mass             =  100.0;
        const double initialNH3Mass               =  0.5 * maxRate * mTimeStep;
        const double reagEfficiency = 0.01;
        mArticle->mReactions[0].mReagentEfficiency = reagEfficiency;
        const double totalEfficiency               = efficiency * reagEfficiency * initialH3PO4Mass;
        mArticle->mCompounds[NH42HPO4].mTotalMass =  initialNH42HPO4Mass;
        mArticle->mCompounds[H3PO4].mTotalMass    =  initialH3PO4Mass;
        mArticle->mCompounds[NH3].mTotalMass      =  initialNH3Mass;
        mArticle->mInternalFluid->resetState();
        mArticle->mInternalFluid->setMass(index, 1.0);
        mArticle->mInternalFluid->updateMass();
        mArticle->mInternalFluid->setTemperature(mWallTemperature);
        mArticle->mFluidTemperature               =  mWallTemperature;
        mArticle->mCompounds[NH42HPO4].mMass      =  0.0;
        mArticle->mCompounds[H3PO4].mMass         =  0.0;
        mArticle->mCompounds[NH3].mMass           =  0.0;
        const double expectedNH3Mass              = -totalEfficiency * initialNH3Mass;
        const double expectedH3PO4Mass            =  expectedNH3Mass / mWeightNH3 *
                                                     moleRatioH3PO4 / moleRatioNH3 *
                                                     mWeightH3PO4;
        const double expectedNH42HPO4Mass         = -expectedNH3Mass / mWeightNH3 *
                                                     moleRatioNH42HPO4 / moleRatioNH3 *
                                                     mWeightNH42HPO4;
        const double expectedNH42HPO4TotalMass    =  expectedNH42HPO4Mass;
        const double expectedH3PO4TotalMass       =  initialH3PO4Mass + expectedH3PO4Mass;
        const double expectedNH3TotalMass         =  initialNH3Mass   + expectedNH3Mass;
        mArticle->mFlowRate                       = 0.5 * maxRate;
        mArticle->updateFluid(mTimeStep, mFlowRate);
        const double returnedNH42HPO4TotalMass    =  mArticle->mCompounds[NH42HPO4].mTotalMass;
        const double returnedH3PO4TotalMass       =  mArticle->mCompounds[H3PO4].mTotalMass;
        const double returnedNH3TotalMass         =  mArticle->mCompounds[NH3].mTotalMass;
        const double returnedNH42HPO4Mass         =  mArticle->mCompounds[NH42HPO4].mMass;
        const double returnedH3PO4Mass            =  mArticle->mCompounds[H3PO4].mMass;
        const double returnedNH3Mass              =  mArticle->mCompounds[NH3].mMass;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3Mass,           returnedNH3Mass,           mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3Mass,           returnedNH3Mass,           mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH3PO4Mass,         returnedH3PO4Mass,         mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH42HPO4Mass,      returnedNH42HPO4Mass,      mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3TotalMass,      returnedNH3TotalMass,      mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH3PO4TotalMass,    returnedH3PO4TotalMass,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH42HPO4TotalMass, returnedNH42HPO4TotalMass, mTolerance);
        mArticle->mReactions[0].mReagentEfficiency  = 0.0;
    } {
        /// @test     Reaction constrained by reagant availability.
        const double initialNH42HPO4Mass          =  0.0;
        const double initialH3PO4Mass             =  0.00001;
        const double initialNH3Mass               =  0.5 * maxRate * mTimeStep;
        mArticle->mCompounds[NH42HPO4].mTotalMass =  initialNH42HPO4Mass;
        mArticle->mCompounds[H3PO4].mTotalMass    =  initialH3PO4Mass;
        mArticle->mCompounds[NH3].mTotalMass      =  initialNH3Mass;
        mArticle->mInternalFluid->resetState();
        mArticle->mInternalFluid->setMass(index, 1.0);
        mArticle->mInternalFluid->updateMass();
        mArticle->mInternalFluid->setTemperature(mWallTemperature);
        mArticle->mCompounds[NH42HPO4].mMass      =  0.0;
        mArticle->mCompounds[H3PO4].mMass         =  0.0;
        mArticle->mCompounds[NH3].mMass           =  0.0;
        const double expectedH3PO4Mass            = -initialH3PO4Mass;
        const double expectedNH3Mass              =  expectedH3PO4Mass / mWeightH3PO4 *
                                                     moleRatioNH3 / moleRatioH3PO4 *
                                                     mWeightNH3;
        const double expectedNH42HPO4Mass         = -expectedNH3Mass / mWeightNH3 *
                                                     moleRatioNH42HPO4 / moleRatioNH3 *
                                                     mWeightNH42HPO4;
        const double expectedNH42HPO4TotalMass    =  expectedNH42HPO4Mass;
        const double expectedH3PO4TotalMass       =  initialH3PO4Mass + expectedH3PO4Mass;
        const double expectedNH3TotalMass         =  initialNH3Mass   + expectedNH3Mass;
        mArticle->mFlowRate                       = -0.5 * maxRate;
        mArticle->updateFluid(mTimeStep, mFlowRate);
        const double returnedNH42HPO4TotalMass    =  mArticle->mCompounds[NH42HPO4].mTotalMass;
        const double returnedH3PO4TotalMass       =  mArticle->mCompounds[H3PO4].mTotalMass;
        const double returnedNH3TotalMass         =  mArticle->mCompounds[NH3].mTotalMass;
        const double returnedNH42HPO4Mass         =  mArticle->mCompounds[NH42HPO4].mMass;
        const double returnedH3PO4Mass            =  mArticle->mCompounds[H3PO4].mMass;
        const double returnedNH3Mass              =  mArticle->mCompounds[NH3].mMass;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3Mass,           returnedNH3Mass,           mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3Mass,           returnedNH3Mass,           mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH3PO4Mass,         returnedH3PO4Mass,         mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH42HPO4Mass,      returnedNH42HPO4Mass,      mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH3TotalMass,      returnedNH3TotalMass,      mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH3PO4TotalMass,    returnedH3PO4TotalMass,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNH42HPO4TotalMass, returnedNH42HPO4TotalMass, mTolerance);
    } {
        /// @test     Zero incoming flowrate.
        const double initialNH42HPO4Mass          =  0.0;
        const double initialH3PO4Mass             =  100.0;
        const double initialNH3Mass               =  0.5 * maxRate * mTimeStep;
        mArticle->mCompounds[NH42HPO4].mTotalMass =  initialNH42HPO4Mass;
        mArticle->mCompounds[H3PO4].mTotalMass    =  initialH3PO4Mass;
        mArticle->mCompounds[NH3].mTotalMass      =  initialNH3Mass;
        mArticle->mCompounds[NH42HPO4].mMass      =  0.0;
        mArticle->mCompounds[H3PO4].mMass         =  0.0;
        mArticle->mCompounds[NH3].mMass           =  0.0;
        const double expectedNH3Mass              =  mArticle->mInternalFluid->
                                                     getMassFraction(FluidProperties::GUNNS_NH3) *
                                                     mArticle->mInternalFluid->getMass();
        mArticle->mFlowRate                       = 0.0;
        mArticle->mInternalFluid->setTemperature(mWallTemperature);
        mArticle->updateFluid(mTimeStep, mFlowRate);
        const double returnedNH42HPO4TotalMass    =  mArticle->mCompounds[NH42HPO4].mTotalMass;
        const double returnedH3PO4TotalMass       =  mArticle->mCompounds[H3PO4].mTotalMass;
        const double returnedNH3TotalMass         =  mArticle->mCompounds[NH3].mTotalMass;
        const double returnedNH42HPO4Mass         =  mArticle->mCompounds[NH42HPO4].mMass;
        const double returnedH3PO4Mass            =  mArticle->mCompounds[H3PO4].mMass;
        const double returnedNH3Mass              =  mArticle->mCompounds[NH3].mMass;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                       returnedNH3Mass,           mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                       returnedNH3Mass,           mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                       returnedH3PO4Mass,         mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                       returnedNH42HPO4Mass,      mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(initialNH3Mass,            returnedNH3TotalMass,      mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(initialH3PO4Mass,          returnedH3PO4TotalMass,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(initialNH42HPO4Mass,       returnedNH42HPO4TotalMass, mTolerance);
    }  {
        /// @test     Zero timestep.
        const double initialNH42HPO4Mass          =  0.0;
        const double initialH3PO4Mass             =  100.0;
        const double initialNH3Mass               =  0.5 * maxRate * mTimeStep;
        mArticle->mCompounds[NH42HPO4].mTotalMass =  initialNH42HPO4Mass;
        mArticle->mCompounds[H3PO4].mTotalMass    =  initialH3PO4Mass;
        mArticle->mCompounds[NH3].mTotalMass      =  initialNH3Mass;
        mArticle->mCompounds[NH42HPO4].mMass      =  0.0;
        mArticle->mCompounds[H3PO4].mMass         =  0.0;
        mArticle->mCompounds[NH3].mMass           =  0.0;
        const double expectedNH3Mass              =  mArticle->mInternalFluid->
                                                     getMassFraction(FluidProperties::GUNNS_NH3) *
                                                     mArticle->mInternalFluid->getMass();
        mArticle->mFlowRate                       = 1.0;
        mArticle->mInternalFluid->setTemperature(mWallTemperature);
        mArticle->updateFluid(0.0, mFlowRate);
        const double returnedNH42HPO4TotalMass    =  mArticle->mCompounds[NH42HPO4].mTotalMass;
        const double returnedH3PO4TotalMass       =  mArticle->mCompounds[H3PO4].mTotalMass;
        const double returnedNH3TotalMass         =  mArticle->mCompounds[NH3].mTotalMass;
        const double returnedNH42HPO4Mass         =  mArticle->mCompounds[NH42HPO4].mMass;
        const double returnedH3PO4Mass            =  mArticle->mCompounds[H3PO4].mMass;
        const double returnedNH3Mass              =  mArticle->mCompounds[NH3].mMass;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                       returnedNH3Mass,           mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                       returnedNH3Mass,           mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                       returnedH3PO4Mass,         mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                       returnedNH42HPO4Mass,      mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(initialNH3Mass,            returnedNH3TotalMass,      mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(initialH3PO4Mass,          returnedH3PO4TotalMass,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(initialNH42HPO4Mass,       returnedNH42HPO4TotalMass, mTolerance);
    } {
        /// @test     Product 2.
        mArticle->mNReactions = 3;
        mArticle->updateFluid(mTimeStep, mFlowRate);
        const double expected = 0.0;
        const int H2O         = 6;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,
                                     mArticle->mCompounds[H2O].mMass,
                                     mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Hot Reactor link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHotReactor::testInitializationException()
{
    UT_RESULT;

    {
        /// @test    Initialization exception on invalid config data: null pointer to name.
        GunnsFluidHotReactorConfigData configData("",
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    } {
        /// @test    Initialization exception on invalid config data: max conductivity < 0.
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  -DBL_EPSILON,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    }
    {
        /// @test    Initialization exception on invalid config data: expansion scale factor < 0.
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  -DBL_EPSILON,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    } {
        /// @test    Initialization exception on invalid config data: null pointer to compounds.
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  0,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    } {
        /// @test    Initialization exception on invalid config data: no compounds specified.
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  0,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    } {
        /// @test    Initialization exception on invalid config data: undefined compound specified.
        mCompoundTypes[0] = ChemicalCompound::NO_COMPOUND;
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
        mCompoundTypes[0] = ChemicalCompound::ChemicalCompound::NH42HPO4;
    } {
        /// @test    Initialization exception on invalid config data: null pointer to reactions.
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  0,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    } {
        /// @test    Initialization exception on invalid config data: no reactions specified.
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  0,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    } {
        /// @test    Initialization exception on invalid config data: undefined reaction type
        ///          specified.
        mReactionTypes[0] = ChemicalReaction::NO_REACTION;
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
        mReactionTypes[0] = ChemicalReaction::NH3_REMOVAL;
    } {
        /// @test    Initialization exception on invalid config data: undefined reaction reactant.
        mCompoundTypes[0] = ChemicalCompound::NH42HPO4;   mTotalMasses[0] = 0.5;
        mCompoundTypes[1] = ChemicalCompound::H3PO4;      mTotalMasses[1] = 0.5;
        mCompoundTypes[2] = ChemicalCompound::NH3;        mTotalMasses[2] = 0.0;
        mCompoundTypes[3] = ChemicalCompound::CH4;        mTotalMasses[3] = 0.0;
        mCompoundTypes[4] = ChemicalCompound::H2;         mTotalMasses[4] = 0.0;
        mCompoundTypes[5] = ChemicalCompound::O2;         mTotalMasses[5] = 0.0;
        mCompoundTypes[6] = ChemicalCompound::H2O;        mTotalMasses[6] = 0.0;
        mCompoundTypes[7] = ChemicalCompound::CO2;        mTotalMasses[7] = 0.0;
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS-1,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),  TsInitializationException);
    } {
        /// @test    Initialization exception on invalid config data: undefined reaction reagent.
        mCompoundTypes[0] = ChemicalCompound::NH42HPO4;   mTotalMasses[0] = 0.5;
        mCompoundTypes[1] = ChemicalCompound::H3PO4;      mTotalMasses[1] = 0.5;
        mCompoundTypes[2] = ChemicalCompound::NH3;        mTotalMasses[2] = 0.0;
        mCompoundTypes[3] = ChemicalCompound::CH4;        mTotalMasses[3] = 0.0;
        mCompoundTypes[4] = ChemicalCompound::H2;         mTotalMasses[4] = 0.0;
        mCompoundTypes[5] = ChemicalCompound::H2O;        mTotalMasses[5] = 0.0;
        mCompoundTypes[6] = ChemicalCompound::CO;         mTotalMasses[6] = 0.0;
        mCompoundTypes[7] = ChemicalCompound::CO2;        mTotalMasses[7] = 0.0;
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS-1,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    } {
        /// @test    Initialization exception on invalid config data: undefined reaction product 1.
        mCompoundTypes[0] = ChemicalCompound::H3PO4;      mTotalMasses[0] = 0.5;
        mCompoundTypes[1] = ChemicalCompound::NH3;        mTotalMasses[1] = 0.0;
        mCompoundTypes[2] = ChemicalCompound::CH4;        mTotalMasses[2] = 0.0;
        mCompoundTypes[3] = ChemicalCompound::H2;         mTotalMasses[3] = 0.0;
        mCompoundTypes[4] = ChemicalCompound::O2;         mTotalMasses[4] = 0.0;
        mCompoundTypes[5] = ChemicalCompound::H2O;        mTotalMasses[5] = 0.0;
        mCompoundTypes[6] = ChemicalCompound::CO;         mTotalMasses[6] = 0.0;
        mCompoundTypes[7] = ChemicalCompound::CO2;        mTotalMasses[7] = 0.0;
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS-1,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    }  {
        /// @test    Initialization exception on invalid config data: unavailable reaction reactant.
        FluidProperties::FluidType types[N_CONSTITUENTS-1];
        double fractions[N_CONSTITUENTS-1];
        types[0]        = FluidProperties::GUNNS_N2;  fractions[0] = 0.750;
        types[1]        = FluidProperties::GUNNS_NH3; fractions[1] = 0.010;
        types[2]        = FluidProperties::GUNNS_CH4; fractions[2] = 0.002;
        types[3]        = FluidProperties::GUNNS_H2;  fractions[3] = 0.003;
        types[4]        = FluidProperties::GUNNS_O2;  fractions[4] = 0.200;
        types[5]        = FluidProperties::GUNNS_H2O; fractions[5] = 0.010;
        types[6]        = FluidProperties::GUNNS_CO2; fractions[6] = 0.025;
        PolyFluidConfigData fluidConfig(mFluidProperties, types, N_CONSTITUENTS-1);
        PolyFluidInputData  fluidInput(300.0,  140000.0, 1.0, 1.0, fractions);
        GunnsFluidNode  nodes[3];
        nodes[0].getContent()->initializeName("nodes0.mContent");
        nodes[1].getContent()->initializeName("nodes1.mContent");
        nodes[2].getContent()->initializeName("nodes2.mContent");
        nodes[0].getContent()->initialize(fluidConfig, fluidInput);
        nodes[1].getContent()->initialize(fluidConfig, fluidInput);
        nodes[2].getContent()->initialize(fluidConfig, fluidInput);
        mNodeList.mNodes = nodes;
        mCompoundTypes[0] = ChemicalCompound::NH42HPO4;   mTotalMasses[0] = 0.5;
        mCompoundTypes[1] = ChemicalCompound::H3PO4;      mTotalMasses[1] = 0.5;
        mCompoundTypes[2] = ChemicalCompound::NH3;        mTotalMasses[2] = 0.0;
        mCompoundTypes[3] = ChemicalCompound::CH4;        mTotalMasses[3] = 0.0;
        mCompoundTypes[4] = ChemicalCompound::H2;         mTotalMasses[4] = 0.0;
        mCompoundTypes[5] = ChemicalCompound::O2;         mTotalMasses[5] = 0.0;
        mCompoundTypes[6] = ChemicalCompound::H2O;        mTotalMasses[6] = 0.0;
        mCompoundTypes[7] = ChemicalCompound::CO;         mTotalMasses[7] = 0.0;
        mCompoundTypes[8] = ChemicalCompound::CO2;        mTotalMasses[8] = 0.0;
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, *mInputData, mLinks, mPort0, mPort1), TsInitializationException);
    } {
        /// @test    Initialization exception on invalid config data: unavailable reaction reagent.
        FluidProperties::FluidType types[N_CONSTITUENTS-1];
        double fractions[N_CONSTITUENTS-1];
        types[0]        = FluidProperties::GUNNS_N2;  fractions[0] = 0.750;
        types[1]        = FluidProperties::GUNNS_NH3; fractions[1] = 0.010;
        types[2]        = FluidProperties::GUNNS_CH4; fractions[2] = 0.002;
        types[3]        = FluidProperties::GUNNS_H2;  fractions[3] = 0.203;
        types[4]        = FluidProperties::GUNNS_H2O; fractions[4] = 0.010;
        types[5]        = FluidProperties::GUNNS_CO;  fractions[5] = 0.005;
        types[6]        = FluidProperties::GUNNS_CO2; fractions[6] = 0.020;
        PolyFluidConfigData fluidConfig(mFluidProperties, types, N_CONSTITUENTS-1);
        PolyFluidInputData  fluidInput(300.0, 140000.0, 1.0, 1.0, fractions);
        GunnsFluidNode  nodes[3];
        mNodeList.mNodes = nodes;
        nodes[0].getContent()->initializeName("nodes0.mContent");
        nodes[1].getContent()->initializeName("nodes1.mContent");
        nodes[2].getContent()->initializeName("nodes2.mContent");
        nodes[0].getContent()->initialize(fluidConfig, fluidInput);
        nodes[1].getContent()->initialize(fluidConfig, fluidInput);
        nodes[2].getContent()->initialize(fluidConfig, fluidInput);
        GunnsFluidHotReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidHotReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidHotReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness,
                                                  mEfficiencyBias,
                                                  mEfficiencyScaleFactor);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, *mInputData, mLinks, mPort0, mPort1), TsInitializationException);
        mNodeList.mNodes = mNodes;
    } {
        /// @test    Initialization exception on invalid input data: mMalfBlockageValue < 0.
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, -DBL_EPSILON, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    } {
        /// @test    Initialization exception on invalid input data: mMalfBlockageValue > 1.
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, 1.0 + DBL_EPSILON, mTotalMasses, mWallTemperature);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    } {
        /// @test    Initialization exception on invalid input data: mReactorTemperature < 0.
        GunnsFluidHotReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, -0.5 * DBL_EPSILON);
        GunnsFluidHotReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    }

    UT_PASS_LAST;
}
