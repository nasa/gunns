/**
@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidReactor.hh"

/// @details  Test identification number.
int UtGunnsFluidReactor::TEST_ID = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Reactor link model unit test.
///////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidReactor::UtGunnsFluidReactor()
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
    mReactantEfficiency(0),
    mReagentEfficiency(0),
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
/// @details  Default destructs this GUNNS Fluid Reactor link model unit test.
///////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidReactor::~UtGunnsFluidReactor()
{
    // do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::setUp()
{
    /// - Define the nominal port fluids.
    mFluidProperties      = new DefinedFluidProperties();
    mTypes[0]             = FluidProperties::GUNNS_N2;  mFractions[0] = 0.750;
    mTypes[1]             = FluidProperties::GUNNS_NH3; mFractions[1] = 0.010;
    mTypes[2]             = FluidProperties::GUNNS_CH4; mFractions[2] = 0.002;
    mTypes[3]             = FluidProperties::GUNNS_H2;  mFractions[3] = 0.002;
    mTypes[4]             = FluidProperties::GUNNS_O2;  mFractions[4] = 0.200;
    mTypes[5]             = FluidProperties::GUNNS_H2O; mFractions[5] = 0.010;
    mTypes[6]             = FluidProperties::GUNNS_CO;  mFractions[6] = 0.008;
    mTypes[7]             = FluidProperties::GUNNS_CO2; mFractions[7] = 0.018;
    mFluidConfig          = new PolyFluidConfigData(mFluidProperties,
                                                    mTypes,
                                                    UtGunnsFluidReactor::N_CONSTITUENTS);
    mFluidInput           = new PolyFluidInputData(300.0,
                                                   140000.0,
                                                   1.0,
                                                   1.0,
                                                   mFractions);

    /// - Initialize the nodes.
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, *mFluidInput);
    mNodes[1].getContent()->initialize(*mFluidConfig, *mFluidInput);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Initialize the nodes list.
    mNodeList.mNodes      = mNodes;
    mNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;

    /// - Define the nominal configuration data.
    mName                 = "nominal";
    mMaxConductivity      =  1.5;
    mExpansionScaleFactor =  0.0;
    mCompoundTypes[0]     = ChemicalCompound::NH42HPO4;   mTotalMasses[0] = 0.5;
    mCompoundTypes[1]     = ChemicalCompound::H3PO4;      mTotalMasses[1] = 0.5;
    mCompoundTypes[2]     = ChemicalCompound::NH3;        mTotalMasses[2] = 0.0;
    mCompoundTypes[3]     = ChemicalCompound::CH4;        mTotalMasses[3] = 0.0;
    mCompoundTypes[4]     = ChemicalCompound::H2;         mTotalMasses[4] = 0.0;
    mCompoundTypes[5]     = ChemicalCompound::O2;         mTotalMasses[5] = 0.0;
    mCompoundTypes[6]     = ChemicalCompound::H2O;        mTotalMasses[6] = 0.0;
    mCompoundTypes[7]     = ChemicalCompound::CO;         mTotalMasses[7] = 0.0;
    mCompoundTypes[8]     = ChemicalCompound::CO2;        mTotalMasses[8] = 0.0;
    mReactionTypes[0]     = ChemicalReaction::NH3_REMOVAL;    ///<  2NH3 + H3PO4 --> (NH4)2(HPO4)
    mReactionTypes[1]     = ChemicalReaction::H2_REMOVAL;     ///<  2H2 + O2 --> 2H2O
    mReactionTypes[2]     = ChemicalReaction::SABATIER_RXN;   ///<  CO2 + 4H2 --> 2H2O + CH4
    mReactionTypes[3]     = ChemicalReaction::CO_REMOVAL;     ///<  2CO + O2 --> 2CO2
    mThermalLength        = 1.0;
    mThermalDiameter      = 0.1;
    mSurfaceRoughness     = 1.0e-03;
    mThermalSurfaceArea   = UnitConversion::PI_UTIL * mThermalDiameter * mThermalLength;
    mThermalROverD        = mSurfaceRoughness / mThermalDiameter;
    mReactantEfficiency   = new double[N_REACTIONS];
    mReactantEfficiency[0] = 0.1;
    mReactantEfficiency[1] = 0.2;
    mReactantEfficiency[2] = 0.3;
    mReactantEfficiency[3] = 0.4;
    mReagentEfficiency    = new double[N_REACTIONS];
    mReagentEfficiency[0] = 0.01;
    mReagentEfficiency[1] = 0.4;
    mReagentEfficiency[2] = 0.3;
    mReagentEfficiency[3] = 0.2;
    mConfigData           = new GunnsFluidReactorConfigData(mName,
                                                            &mNodeList,
                                                            mMaxConductivity,
                                                            mExpansionScaleFactor,
                                                            &mReactions,
                                                            mReactionTypes,
                                                            UtGunnsFluidReactor::N_REACTIONS,
                                                            &mCompounds,
                                                            mCompoundTypes,
                                                            UtGunnsFluidReactor::N_COMPOUNDS,
                                                            mThermalLength,
                                                            mThermalDiameter,
                                                            mSurfaceRoughness);

    /// - Define the nominal input data.
    mMalfBlockageFlag     = false;
    mMalfBlockageValue    = 0.3;
    mWallTemperature      = 700.0;
    mInputData            = new GunnsFluidReactorInputData(mMalfBlockageFlag,
                                                           mMalfBlockageValue,
                                                           mTotalMasses,
                                                           mWallTemperature);

    /// - Default construct the nominal test article.
    mFluidTemperature     = 0.5 * (mWallTemperature + mFluidInput->mTemperature);
    mWallHeatFlux         = 0.0;
    mArticle = new FriendlyGunnsFluidReactor();

    /// - Define the nominal flow rate, time step and comparison tolerance.
    mFlowRate             = 1.0;
    mTimeStep             = 0.1;
    mTolerance            = 1.0e-06;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete [] mReagentEfficiency;
    delete [] mReactantEfficiency;
    delete mFluidInput;
    delete mFluidConfig;
    delete mFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Reactor link model construction of configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName                                          == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                                         == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,                   mConfigData->mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,              mConfigData->mExpansionScaleFactor,  0.0);
    for (int i = 0; i < N_REACTIONS; i++) {
        ChemicalReaction::Type type = static_cast<ChemicalReaction::Type>(i);
        CPPUNIT_ASSERT(mReactions.getReaction(type)               == mConfigData->mReactions->getReaction(type));
    }
    CPPUNIT_ASSERT(N_REACTIONS                                    == mConfigData->mNReactions);
    for (int i = 0; i < N_COMPOUNDS; i++) {
        ChemicalCompound::Type type = static_cast<ChemicalCompound::Type>(i);
        CPPUNIT_ASSERT(mCompounds.getCompound(type)               == mConfigData->mCompounds->getCompound(type));
    }
    CPPUNIT_ASSERT(N_COMPOUNDS                                    == mConfigData->mNCompounds);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalLength,                     mConfigData->mThermalLength,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,                   mConfigData->mThermalDiameter,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSurfaceRoughness,                  mConfigData->mSurfaceRoughness,      0.0);
    CPPUNIT_ASSERT(0                                              == mConfigData->mReactantEfficiency);
    CPPUNIT_ASSERT(0                                              == mConfigData->mReagentEfficiency);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag                              == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,                 mInputData->mMalfBlockageValue,      0.0);
    for (int i = 0; i < N_COMPOUNDS; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mTotalMasses[i],                mInputData->mTotalMasses[i],         mTolerance);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,                   mInputData->mWallTemperature,        0.0);

    /// @test    Configuration data default construction.
    GunnsFluidReactorConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                              == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                              == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT(0                                              == defaultConfig.mReactions);
    CPPUNIT_ASSERT(0                                              == defaultConfig.mNReactions);
    CPPUNIT_ASSERT(0                                              == defaultConfig.mCompounds);
    CPPUNIT_ASSERT(0                                              == defaultConfig.mNCompounds);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mThermalLength,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mThermalDiameter,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mSurfaceRoughness,     0.0);
    CPPUNIT_ASSERT(0                                              == defaultConfig.mReactantEfficiency);
    CPPUNIT_ASSERT(0                                              == defaultConfig.mReagentEfficiency);

    /// @test    Input data default construction.
    GunnsFluidReactorInputData defaultInput;
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT(0                                              == defaultInput.mTotalMasses);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mWallTemperature,       0.0);

    /// @test    Input data copy construction.
    GunnsFluidReactorInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag                  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,     copyInput.mMalfBlockageValue,        0.0);
    for (int i = 0; i < N_COMPOUNDS; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mTotalMasses[i],    copyInput.mTotalMasses[i],           mTolerance);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mWallTemperature,       copyInput.mWallTemperature,          0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Reactor link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::testDefaultConstruction()
{
    UT_RESULT;

    FriendlyGunnsFluidReactor article;

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

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(!mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMalfBlockageValue,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mWallTemperature,           0.0);

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

    /// @test    Default constructors for ReactionState class.
    ReactionState* reaction = new ReactionState;

    CPPUNIT_ASSERT(0   == reaction->mType);
    CPPUNIT_ASSERT(0   == reaction->mReaction);
    CPPUNIT_ASSERT(0   == reaction->mReactantIndex);
    CPPUNIT_ASSERT(0   == reaction->mReagentIndex);
    CPPUNIT_ASSERT(0   == reaction->mProduct1Index);
    CPPUNIT_ASSERT(0   == reaction->mProduct2Index);
    CPPUNIT_ASSERT(0.0 == reaction->mReactantEfficiency);
    CPPUNIT_ASSERT(0.0 == reaction->mReagentEfficiency);

    delete reaction;

    /// @test    Default constructors for CompoundState class.
    CompoundState* compund = new CompoundState;

    CPPUNIT_ASSERT(0   == compund->mType);
    CPPUNIT_ASSERT(0   == compund->mCompound);
    CPPUNIT_ASSERT(0   == compund->mIndex);
    CPPUNIT_ASSERT(0.0 == compund->mTotalMass);
    CPPUNIT_ASSERT(0.0 == compund->mMass);

    delete compund;

    UT_PASS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Reactor link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    mConfigData->mReactantEfficiency = mReactantEfficiency;
    mConfigData->mReagentEfficiency  = mReagentEfficiency;
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
    CPPUNIT_ASSERT(mReactantEfficiency[0]                                == mArticle->mReactions[0].mReactantEfficiency);
    CPPUNIT_ASSERT(mReagentEfficiency[0]                                 == mArticle->mReactions[0].mReagentEfficiency);
    CPPUNIT_ASSERT(ChemicalReaction::H2_REMOVAL                          == mArticle->mReactions[1].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::H2_REMOVAL)  == mArticle->mReactions[1].mReaction);
    CPPUNIT_ASSERT(4                                                     == mArticle->mReactions[1].mReactantIndex);
    CPPUNIT_ASSERT(5                                                     == mArticle->mReactions[1].mReagentIndex);
    CPPUNIT_ASSERT(6                                                     == mArticle->mReactions[1].mProduct1Index);
    CPPUNIT_ASSERT(-1                                                    == mArticle->mReactions[1].mProduct2Index);
    CPPUNIT_ASSERT(mReactantEfficiency[1]                                == mArticle->mReactions[1].mReactantEfficiency);
    CPPUNIT_ASSERT(mReagentEfficiency[1]                                 == mArticle->mReactions[1].mReagentEfficiency);
    CPPUNIT_ASSERT(ChemicalReaction::SABATIER_RXN                        == mArticle->mReactions[2].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::SABATIER_RXN) == mArticle->mReactions[2].mReaction);
    CPPUNIT_ASSERT(8                                                     == mArticle->mReactions[2].mReactantIndex);
    CPPUNIT_ASSERT(4                                                     == mArticle->mReactions[2].mReagentIndex);
    CPPUNIT_ASSERT(6                                                     == mArticle->mReactions[2].mProduct1Index);
    CPPUNIT_ASSERT(3                                                     == mArticle->mReactions[2].mProduct2Index);
    CPPUNIT_ASSERT(mReactantEfficiency[2]                                == mArticle->mReactions[2].mReactantEfficiency);
    CPPUNIT_ASSERT(mReagentEfficiency[2]                                 == mArticle->mReactions[2].mReagentEfficiency);
    CPPUNIT_ASSERT(ChemicalReaction::CO_REMOVAL                          == mArticle->mReactions[3].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::CO_REMOVAL)  == mArticle->mReactions[3].mReaction);
    CPPUNIT_ASSERT(7                                                     == mArticle->mReactions[3].mReactantIndex);
    CPPUNIT_ASSERT(5                                                     == mArticle->mReactions[3].mReagentIndex);
    CPPUNIT_ASSERT(8                                                     == mArticle->mReactions[3].mProduct1Index);
    CPPUNIT_ASSERT(-1                                                    == mArticle->mReactions[3].mProduct2Index);
    CPPUNIT_ASSERT(mReactantEfficiency[3]                                == mArticle->mReactions[3].mReactantEfficiency);
    CPPUNIT_ASSERT(mReagentEfficiency[3]                                 == mArticle->mReactions[3].mReagentEfficiency);
    CPPUNIT_ASSERT(N_REACTIONS                                           == mArticle->mNReactions);
    CPPUNIT_ASSERT(N_COMPOUNDS                                           == mArticle->mNCompounds);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,                          mArticle->mThermalDiameter,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalSurfaceArea,                       mArticle->mThermalSurfaceArea,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalROverD,                            mArticle->mThermalROverD,          0.0);

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

    /// @test    Verfiy restartModel Functionality
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
/// @details  Tests for GUNNS Fluid Reactor link model duplicate initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::testMultipleInitialization()
{
    UT_RESULT;

    /// - Initialize a default test article with off-nominal config and input data
    FriendlyGunnsFluidReactor article;
    GunnsFluidReactorConfigData configData(std::string("off-nominal"),
                                         &mNodeList,
                                         mMaxConductivity,
                                         mExpansionScaleFactor,
                                         &mReactions,
                                         mReactionTypes,
                                         UtGunnsFluidReactor::N_REACTIONS - 2,
                                         &mCompounds,
                                         mCompoundTypes,
                                         UtGunnsFluidReactor::N_COMPOUNDS - 2);
    double totalMasses[N_COMPOUNDS - 2];
    totalMasses[0] = 0.1;
    totalMasses[1] = 0.2;
    totalMasses[2] = 0.3;
    totalMasses[3] = 0.4;
    totalMasses[4] = 0.5;
    totalMasses[5] = 0.6;
    totalMasses[6] = 0.7;
    GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, totalMasses);
    article.initialize(configData, inputData, mLinks, mPort1, mPort0);

    /// - Initialize test article again with nominal configuration and input data
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName                                                 == article.mName);
    CPPUNIT_ASSERT(&mNodes[0]                                            == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1]                                            == article.mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,                          article.mMaxConductivity,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,                     article.mExpansionScaleFactor,   0.0);
    CPPUNIT_ASSERT(ChemicalReaction::NH3_REMOVAL                         == article.mReactions[0].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::NH3_REMOVAL) == article.mReactions[0].mReaction);
    CPPUNIT_ASSERT(2                                                     == article.mReactions[0].mReactantIndex);
    CPPUNIT_ASSERT(1                                                     == article.mReactions[0].mReagentIndex);
    CPPUNIT_ASSERT(0                                                     == article.mReactions[0].mProduct1Index);
    CPPUNIT_ASSERT(-1                                                    == article.mReactions[0].mProduct2Index);
    CPPUNIT_ASSERT(ChemicalReaction::H2_REMOVAL                          == article.mReactions[1].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::H2_REMOVAL)  == article.mReactions[1].mReaction);
    CPPUNIT_ASSERT(4                                                     == article.mReactions[1].mReactantIndex);
    CPPUNIT_ASSERT(5                                                     == article.mReactions[1].mReagentIndex);
    CPPUNIT_ASSERT(6                                                     == article.mReactions[1].mProduct1Index);
    CPPUNIT_ASSERT(-1                                                    == article.mReactions[1].mProduct2Index);
    CPPUNIT_ASSERT(ChemicalReaction::SABATIER_RXN                        == article.mReactions[2].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::SABATIER_RXN) == article.mReactions[2].mReaction);
    CPPUNIT_ASSERT(8                                                     == article.mReactions[2].mReactantIndex);
    CPPUNIT_ASSERT(4                                                     == article.mReactions[2].mReagentIndex);
    CPPUNIT_ASSERT(6                                                     == article.mReactions[2].mProduct1Index);
    CPPUNIT_ASSERT(3                                                     == article.mReactions[2].mProduct2Index);
    CPPUNIT_ASSERT(ChemicalReaction::CO_REMOVAL                          == article.mReactions[3].mType);
    CPPUNIT_ASSERT(mReactions.getReaction(ChemicalReaction::CO_REMOVAL)  == article.mReactions[3].mReaction);
    CPPUNIT_ASSERT(7                                                     == article.mReactions[3].mReactantIndex);
    CPPUNIT_ASSERT(5                                                     == article.mReactions[3].mReagentIndex);
    CPPUNIT_ASSERT(8                                                     == article.mReactions[3].mProduct1Index);
    CPPUNIT_ASSERT(-1                                                    == article.mReactions[3].mProduct2Index);
    CPPUNIT_ASSERT(N_REACTIONS                                           == article.mNReactions);
    CPPUNIT_ASSERT(N_COMPOUNDS                                           == article.mNCompounds);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mMalfBlockageFlag                                     == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,                        article.mMalfBlockageValue,      0.0);
    for (int i = 0; i < N_COMPOUNDS; i++) {
        const ChemicalCompound::Type type = mCompoundTypes[i];
        CPPUNIT_ASSERT(type                                             == article.mCompounds[i].mType);
        CPPUNIT_ASSERT(mCompounds.getCompound(type)                     == article.mCompounds[i].mCompound);
        if (i >= 2) {
            CPPUNIT_ASSERT(i-1                                          == article.mCompounds[i].mIndex);
        } else {
            CPPUNIT_ASSERT(-1                                           == article.mCompounds[i].mIndex);
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mTotalMasses[i],                      article.mCompounds[i].mTotalMass, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                  article.mCompounds[i].mMass,      mTolerance);
    }

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0                                                    != article.mInternalFluid);
    CPPUNIT_ASSERT(0                                                    != article.mReactantsFluid);
    CPPUNIT_ASSERT(0                                                    != article.mProductsFluid);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mFlowRate,                mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mReactantsFlowRate,       mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                      article.mProductsFlowRate,        mTolerance);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(                                                        article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Reactor link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::testAccess()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort1, mPort0);

    /// @test    Nothing (this derived class does not add or override any accessors).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Reactor link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::testModify()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort1, mPort0);

    {
        /// @test    The thermal surface area setter with good value.
        const double surfaceArea = 0.1;
        mArticle->setThermalSurfaceArea(surfaceArea);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(surfaceArea, mArticle->mThermalSurfaceArea, 0.0);
    } {
        /// @test    The thermal surface area setter with out of range value.
        const double surfaceArea = -0.1;
        mArticle->setThermalSurfaceArea(surfaceArea);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mThermalSurfaceArea, 0.0);
    } {
        /// @test    The wall temperature setter with good value.
        const double wallTemperature = 280.0;
        mArticle->setWallTemperature(wallTemperature);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(wallTemperature, mArticle->mWallTemperature, 0.0);
    } {
        /// @test    The wall temperature setter with out of range value.
        const double wallTemperature = -0.1;
        mArticle->setWallTemperature(wallTemperature);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mWallTemperature, 0.0);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Reactor link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::testUpdateFluid()
{
    UT_RESULT;

    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort1, mPort0);
    mArticle->mNReactions           = 1;
    const double maxRate            = mReactions.getReaction(ChemicalReaction::NH3_REMOVAL)->mMaxReactantRate;
    const double efficiency         = mReactions.getReaction(ChemicalReaction::NH3_REMOVAL)->mReactantEfficiency;
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
    const double reactEfficiency    = mReactantEfficiency[0];
    const double reagEfficiency     = mReagentEfficiency[0];

    {
        /// @test     Reaction constrained by reactant unavailability.
        const double initialNH42HPO4Mass          =  0.0;
        const double initialH3PO4Mass             =  100.0;
        const double initialNH3Mass               =  0.0;
        mArticle->mCompounds[NH42HPO4].mTotalMass =  initialNH42HPO4Mass;
        mArticle->mCompounds[H3PO4].mTotalMass    =  initialH3PO4Mass;
        mNodes[0].getContent()->setMass(index, initialNH3Mass);
        mNodes[0].getContent()->updateMass();
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
        mArticle->mFlowRate                       =  -0.5 * maxRate;
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
        /// @test     Reaction constrained by reactant efficiency override: same as the constrained
        ///           bu efficiency case above, except we use the reactant efficiency override.
        mArticle->mReactions[0].mReactantEfficiency = reactEfficiency;
        const double initialNH42HPO4Mass          =  0.0;
        const double initialH3PO4Mass             =  100.0;
        const double initialNH3Mass               =  0.5 * maxRate * mTimeStep;
        mArticle->mCompounds[NH42HPO4].mTotalMass =  initialNH42HPO4Mass;
        mArticle->mCompounds[H3PO4].mTotalMass    =  initialH3PO4Mass;
        mArticle->mCompounds[NH3].mTotalMass      =  initialNH3Mass;
        mArticle->mInternalFluid->resetState();
        mArticle->mInternalFluid->setMass(index, 1.0);
        mArticle->mInternalFluid->updateMass();
        mArticle->mCompounds[NH42HPO4].mMass      =  0.0;
        mArticle->mCompounds[H3PO4].mMass         =  0.0;
        mArticle->mCompounds[NH3].mMass           =  0.0;
        const double expectedNH3Mass              = -reactEfficiency * initialNH3Mass;
        const double expectedH3PO4Mass            =  expectedNH3Mass / mWeightNH3 *
                                                     moleRatioH3PO4 / moleRatioNH3 *
                                                     mWeightH3PO4;
        const double expectedNH42HPO4Mass         = -expectedNH3Mass / mWeightNH3 *
                                                     moleRatioNH42HPO4 / moleRatioNH3 *
                                                     mWeightNH42HPO4;
        const double expectedNH42HPO4TotalMass    =  expectedNH42HPO4Mass;
        const double expectedH3PO4TotalMass       =  initialH3PO4Mass + expectedH3PO4Mass;
        const double expectedNH3TotalMass         =  initialNH3Mass   + expectedNH3Mass;
        mArticle->mFlowRate                       =  -0.5 * maxRate;
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
        mArticle->mReactions[0].mReactantEfficiency = 0.0;
    } {
        /// @test     Reaction constrained by reagent efficiency: same as the constrained by
        ///           efficiency case above, except we add the reagent efficiency override.
        const double initialNH42HPO4Mass          =  0.0;
        const double initialH3PO4Mass             =  100.0;
        const double initialNH3Mass               =  0.5 * maxRate * mTimeStep;
        mArticle->mReactions[0].mReagentEfficiency = reagEfficiency;
        const double totalEfficiency               = efficiency * reagEfficiency * initialH3PO4Mass;
        mArticle->mCompounds[NH42HPO4].mTotalMass =  initialNH42HPO4Mass;
        mArticle->mCompounds[H3PO4].mTotalMass    =  initialH3PO4Mass;
        mArticle->mCompounds[NH3].mTotalMass      =  initialNH3Mass;
        mArticle->mInternalFluid->resetState();
        mArticle->mInternalFluid->setMass(index, 1.0);
        mArticle->mInternalFluid->updateMass();
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
        mArticle->mFlowRate                       =  -0.5 * maxRate;
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
/// @details  Tests for GUNNS Fluid Reactor link model compute flows.  Mass balance of products and
///           reactants is tested for in the downstream node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize the test article with 1 chemical reaction of type CH4 REMOVAL.
    mConfigData->mReactionTypes[0] = ChemicalReaction::CH4_REMOVAL; ///<  CH4 + 2O2 --> CO2 + 2H2O
    mConfigData->mReactionTypes[1] = ChemicalReaction::H2_REMOVAL;  ///<  2H2 + O2 --> 2H2O
    mConfigData->mReactionTypes[2] = ChemicalReaction::NH3_REMOVAL; ///<  2NH3 + H3PO4 --> (NH4)2(HPO4)
    mConfigData->mReactionTypes[3] = ChemicalReaction::CO_REMOVAL;  ///<  2CO + O2 --> 2CO2

    mTotalMasses[0] = 0.0;
    mTotalMasses[1] = 0.0;
    mTotalMasses[2] = 0.0;
    mTotalMasses[3] = 0.0;
    mTotalMasses[4] = 0.0;
    mTotalMasses[5] = 0.0;
    mTotalMasses[6] = 0.0;
    mTotalMasses[7] = 0.0;
    mTotalMasses[8] = 0.0;

    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mNReactions    = 1;

    /// - Set up link conduction and node pressures to get a desired flow rate ~ 1.0 for the test.
    mNodes[0].setPotential(200.0);
    mNodes[1].setPotential(100.0);
    mNodes[0].getContent()->updateMass();
    mNodes[1].getContent()->updateMass();
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->mAdmittanceMatrix[0] = 3.5e-04;
    mArticle->mPotentialVector[0]  = 200.0;
    mArticle->mPotentialVector[1]  = 100.0;

    /// - Call the method under test.
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Compute expected results.
    mFlowRate = mArticle->mFlowRate;
    const double maxRate          = mReactions.getReaction(ChemicalReaction::CH4_REMOVAL)->mMaxReactantRate;
    const double efficiency       = mReactions.getReaction(ChemicalReaction::CH4_REMOVAL)->mReactantEfficiency;
    const double moleRatioCO2     = mReactions.getReaction(ChemicalReaction::CH4_REMOVAL)->mProduct1MoleRatio;
    const double moleRatioH2O     = mReactions.getReaction(ChemicalReaction::CH4_REMOVAL)->mProduct2MoleRatio;
    const double moleRatioO2      = mReactions.getReaction(ChemicalReaction::CH4_REMOVAL)->mReagentMoleRatio;
    const double moleRatioCH4     = mReactions.getReaction(ChemicalReaction::CH4_REMOVAL)->mReactantMoleRatio;
    const double mWeightCO2       = mCompounds.getCompound(ChemicalCompound::CO2)-> mMWeight;
    const double mWeightH2O       = mCompounds.getCompound(ChemicalCompound::H2O)-> mMWeight;
    const double mWeightCH4       = mCompounds.getCompound(ChemicalCompound::CH4)-> mMWeight;
    const double mWeightO2        = mCompounds.getCompound(ChemicalCompound::O2) -> mMWeight;

    const int    indexCO2         = mNodes[0].getOutflow()->find(FluidProperties::GUNNS_CO2);
    const int    indexH2O         = mNodes[0].getOutflow()->find(FluidProperties::GUNNS_H2O);
    const int    indexO2          = mNodes[0].getOutflow()->find(FluidProperties::GUNNS_O2);
    const int    indexCH4         = mNodes[0].getOutflow()->find(FluidProperties::GUNNS_CH4);
    const double availableMassCO2 = mNodes[0].getOutflow()->getMassFraction(mNodes[0].getOutflow()->getType(indexCO2)) * fabs(mFlowRate) * mTimeStep;
    const double availableMassH2O = mNodes[0].getOutflow()->getMassFraction(mNodes[0].getOutflow()->getType(indexH2O)) * fabs(mFlowRate) * mTimeStep;
    const double availableMassO2  = mNodes[0].getOutflow()->getMassFraction(mNodes[0].getOutflow()->getType(indexO2))  * fabs(mFlowRate) * mTimeStep;
    const double availableMassCH4 = mNodes[0].getOutflow()->getMassFraction(mNodes[0].getOutflow()->getType(indexCH4)) * fabs(mFlowRate) * mTimeStep;

    const double massRatioCO2     = moleRatioCO2 * mWeightCO2;
    const double massRatioH2O     = moleRatioH2O * mWeightH2O;
    const double massRatioO2      = moleRatioO2  * mWeightO2;
    const double massRatioCH4     = moleRatioCH4 * mWeightCH4;

    // Expect CH4 to be the limiting reactant.
    const double maxReactantMass  = availableMassCH4 * efficiency;
    const double reagentMass      = maxReactantMass * massRatioO2  / massRatioCH4;
    const double reactantMass     = reagentMass     * massRatioCH4 / massRatioO2;
    const double product1Mass     = reagentMass     * massRatioCO2 / massRatioO2;
    const double product2Mass     = reagentMass     * massRatioH2O / massRatioO2;

    const double massCO2          =  product1Mass;
    const double massH2O          =  product2Mass;
    const double massO2           = -reagentMass;
    const double massCH4          = -reactantMass;

    const double totalMassCO2     = availableMassCO2 + product1Mass;
    const double totalMassH2O     = availableMassH2O + product2Mass;
    const double totalMassO2      = availableMassO2  - reagentMass;
    const double totalMassCH4     = availableMassCH4 - reactantMass;

    /// @test  Reaction efficiency.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(efficiency,   mArticle->computeEfficiency(0),     DBL_EPSILON);

    /// @test  Reaction masses.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(massCO2,      mArticle->mCompounds[8].mMass,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(massH2O,      mArticle->mCompounds[6].mMass,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(massO2,       mArticle->mCompounds[5].mMass,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(massCH4,      mArticle->mCompounds[3].mMass,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassCO2, mArticle->mCompounds[8].mTotalMass, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassH2O, mArticle->mCompounds[6].mTotalMass, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassO2,  mArticle->mCompounds[5].mTotalMass, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassCH4, mArticle->mCompounds[3].mTotalMass, DBL_EPSILON);

    /// @test  Zero net reaction mass balance.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mCompounds[8].mMass
                                             + mArticle->mCompounds[6].mMass
                                             + mArticle->mCompounds[5].mMass
                                             + mArticle->mCompounds[3].mMass,      DBL_EPSILON);
    const double reactantsFlowRate = (massCH4 + massO2)  / mTimeStep;
    const double productsFlowRate  = (massCO2 + massH2O) / mTimeStep;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(reactantsFlowRate, mArticle->mReactantsFlowRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(productsFlowRate,  mArticle->mProductsFlowRate,   DBL_EPSILON);

    /// @test  Mass fractions of each product and reactant in the product & reactant fluids.
    const double xCO2 = massRatioCO2 / (massRatioCO2 + massRatioH2O);
    const double xH2O = massRatioH2O / (massRatioCO2 + massRatioH2O);
    const double xO2  = massRatioO2  / (massRatioO2  + massRatioCH4);
    const double xCH4 = massRatioCH4 / (massRatioO2  + massRatioCH4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(xCO2,         mArticle->mProductsFluid->getMassFraction(mArticle->mProductsFluid->getType(indexCO2)),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(xH2O,         mArticle->mProductsFluid->getMassFraction(mArticle->mProductsFluid->getType(indexH2O)),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(xO2,          mArticle->mReactantsFluid->getMassFraction(mArticle->mProductsFluid->getType(indexO2)),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(xCH4,         mArticle->mReactantsFluid->getMassFraction(mArticle->mProductsFluid->getType(indexCH4)), DBL_EPSILON);

    /// @test  Downstream node's inflow has the right mixture, flow rate and temperature.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,          mNodes[1].getInflux(),              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassCO2 / mFlowRate / mTimeStep, mNodes[1].getInflow()->getMassFraction(mNodes[1].getContent()->getType(indexCO2)), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassH2O / mFlowRate / mTimeStep, mNodes[1].getInflow()->getMassFraction(mNodes[1].getContent()->getType(indexH2O)), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassO2  / mFlowRate / mTimeStep, mNodes[1].getInflow()->getMassFraction(mNodes[1].getContent()->getType(indexO2)),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassCH4 / mFlowRate / mTimeStep, mNodes[1].getInflow()->getMassFraction(mNodes[1].getContent()->getType(indexCH4)), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mInternalFluid->getTemperature(), mNodes[1].getInflow()->getTemperature(), FLT_EPSILON);

    mNodes[0].setPotential(100.0);
    mNodes[1].setPotential(200.0);
    mNodes[0].getContent()->updateMass();
    mNodes[1].getContent()->updateMass();
    mNodes[0].resetFlows();
    mNodes[1].resetFlows();
    mArticle->mAdmittanceMatrix[0] = 3.5e-04;
    mArticle->mPotentialVector[0]  = 100.0;
    mArticle->mPotentialVector[1]  = 200.0;

    /// - Call the method under test.
    mArticle->computeFlows(mTimeStep);
    mFlowRate = mArticle->mFlowRate;
    mArticle->transportFlows(mTimeStep);

    CPPUNIT_ASSERT(mFlowRate == mNodes[1].getOutflux());
    CPPUNIT_ASSERT(mFlowRate == mNodes[0].getInflux());
    CPPUNIT_ASSERT(0.0       == mArticle->mSourceVector[0]);
    CPPUNIT_ASSERT(0.0       == mArticle->mSourceVector[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(reactantsFlowRate, mArticle->mReactantsFlowRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(productsFlowRate,  mArticle->mProductsFlowRate,   DBL_EPSILON);

    //TODO check source vector
    //TODO test zero product flow
    //TODO test zero reactant flow

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Reactor link model compute flows.  This tests the outputs of a
///           reaction with differing product and reactant mole numbers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::testComputeFlowsH2Removal()
{
    UT_RESULT;

    /// - Initialize the test article with 1 chemical reaction of type H2 REMOVAL.
    mConfigData->mReactionTypes[0] = ChemicalReaction::H2_REMOVAL;  ///<  2H2 + O2 --> 2H2O
    mConfigData->mReactionTypes[1] = ChemicalReaction::CH4_REMOVAL; ///<  CH4 + 2O2 --> CO2 + 2H2O
    mConfigData->mReactionTypes[2] = ChemicalReaction::NH3_REMOVAL; ///<  2NH3 + H3PO4 --> (NH4)2(HPO4)
    mConfigData->mReactionTypes[3] = ChemicalReaction::CO_REMOVAL;  ///<  2CO + O2 --> 2CO2

    mTotalMasses[0] = 0.0;
    mTotalMasses[1] = 0.0;
    mTotalMasses[2] = 0.0;
    mTotalMasses[3] = 0.0;
    mTotalMasses[4] = 0.0;
    mTotalMasses[5] = 0.0;
    mTotalMasses[6] = 0.0;
    mTotalMasses[7] = 0.0;
    mTotalMasses[8] = 0.0;

    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mNReactions    = 1;

    /// - Set up link conduction and node pressures to get a desired flow rate ~ 1.0 for the test.
    mNodes[0].setPotential(200.0);
    mNodes[1].setPotential(100.0);
    mNodes[0].getContent()->updateMass();
    mNodes[1].getContent()->updateMass();
    mArticle->mAdmittanceMatrix[0] = 3.5e-04;
    mArticle->mPotentialVector[0]  = 200.0;
    mArticle->mPotentialVector[1]  = 100.0;

    /// - Call the method under test.
    mArticle->computeFlows(mTimeStep);
    mArticle->transportFlows(mTimeStep);

    /// - Compute expected results.
    mFlowRate = mArticle->mFlowRate;
    const double maxRate          = mReactions.getReaction(ChemicalReaction::H2_REMOVAL)->mMaxReactantRate;
    const double efficiency       = mReactions.getReaction(ChemicalReaction::H2_REMOVAL)->mReactantEfficiency;
    const double moleRatioH2O     = mReactions.getReaction(ChemicalReaction::H2_REMOVAL)->mProduct1MoleRatio;
    const double moleRatioO2      = mReactions.getReaction(ChemicalReaction::H2_REMOVAL)->mReagentMoleRatio;
    const double moleRatioH2      = mReactions.getReaction(ChemicalReaction::H2_REMOVAL)->mReactantMoleRatio;
    const double mWeightH2O       = mCompounds.getCompound(ChemicalCompound::H2O)-> mMWeight;
    const double mWeightH2        = mCompounds.getCompound(ChemicalCompound::H2) -> mMWeight;
    const double mWeightO2        = mCompounds.getCompound(ChemicalCompound::O2) -> mMWeight;

    const int    indexH2O         = mNodes[0].getOutflow()->find(FluidProperties::GUNNS_H2O);
    const int    indexO2          = mNodes[0].getOutflow()->find(FluidProperties::GUNNS_O2);
    const int    indexH2          = mNodes[0].getOutflow()->find(FluidProperties::GUNNS_H2);
    const double availableMassH2O = mNodes[0].getOutflow()->getMassFraction(mNodes[0].getOutflow()->getType(indexH2O)) * fabs(mFlowRate) * mTimeStep;
    const double availableMassO2  = mNodes[0].getOutflow()->getMassFraction(mNodes[0].getOutflow()->getType(indexO2))  * fabs(mFlowRate) * mTimeStep;
    const double availableMassH2  = mNodes[0].getOutflow()->getMassFraction(mNodes[0].getOutflow()->getType(indexH2))  * fabs(mFlowRate) * mTimeStep;

    const double massRatioH2O     = moleRatioH2O * mWeightH2O;
    const double massRatioO2      = moleRatioO2  * mWeightO2;
    const double massRatioH2      = moleRatioH2  * mWeightH2;

    // Expect H2 to be the limiting reactant, and reactor to only use 0.9999 of it.
    const double maxReactantMass  = availableMassH2 * efficiency * 0.9999;
    const double reagentMass      = maxReactantMass * massRatioO2  / massRatioH2;
    const double reactantMass     = reagentMass     * massRatioH2  / massRatioO2;
    const double product1Mass     = reagentMass     * massRatioH2O / massRatioO2;

    const double massH2O          =  product1Mass;
    const double massO2           = -reagentMass;
    const double massH2           = -reactantMass;

    const double totalMassH2O     = availableMassH2O + product1Mass;
    const double totalMassO2      = availableMassO2  - reagentMass;
    const double totalMassH2      = availableMassH2  - reactantMass;

    /// @test  Reaction efficiency.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(efficiency,   mArticle->computeEfficiency(0),     DBL_EPSILON);

    /// @test  Reaction masses.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(massH2O,      mArticle->mCompounds[6].mMass,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(massO2,       mArticle->mCompounds[5].mMass,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(massH2,       mArticle->mCompounds[4].mMass,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassH2O, mArticle->mCompounds[6].mTotalMass, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassO2,  mArticle->mCompounds[5].mTotalMass, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassH2,  mArticle->mCompounds[4].mTotalMass, DBL_EPSILON);

    /// @test  Zero net reaction mass balance.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          mArticle->mCompounds[6].mMass
                                             + mArticle->mCompounds[5].mMass
                                             + mArticle->mCompounds[4].mMass,      FLT_EPSILON);
    const double reactantsFlowRate = (massH2 + massO2) / mTimeStep;
    const double productsFlowRate  = (massH2O)         / mTimeStep;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(reactantsFlowRate, mArticle->mReactantsFlowRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(productsFlowRate,  mArticle->mProductsFlowRate,   DBL_EPSILON);

    /// @test  Mass fractions of each product and reactant in the product & reactant fluids.
    const double xH2O = 1.0;
    const double xO2  = massRatioO2 / (massRatioO2 + massRatioH2);
    const double xH2  = massRatioH2 / (massRatioO2 + massRatioH2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(xH2O,         mArticle->mProductsFluid->getMassFraction(mArticle->mProductsFluid->getType(indexH2O)), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(xO2,          mArticle->mReactantsFluid->getMassFraction(mArticle->mProductsFluid->getType(indexO2)), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(xH2,          mArticle->mReactantsFluid->getMassFraction(mArticle->mProductsFluid->getType(indexH2)), DBL_EPSILON);

    /// @test  Downstream node's inflow has the right mixture, flow rate and temperature.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,          mNodes[1].getInflux(),              FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassH2O / mFlowRate / mTimeStep, mNodes[1].getInflow()->getMassFraction(mNodes[1].getContent()->getType(indexH2O)), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassO2  / mFlowRate / mTimeStep, mNodes[1].getInflow()->getMassFraction(mNodes[1].getContent()->getType(indexO2)),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassH2  / mFlowRate / mTimeStep, mNodes[1].getInflow()->getMassFraction(mNodes[1].getContent()->getType(indexH2)),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mInternalFluid->getTemperature(), mNodes[1].getInflow()->getTemperature(), FLT_EPSILON);

    //TODO check source vector
    //TODO test zero product flow
    //TODO test zero reactant flow

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for heats of reaction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::testHeatOfReaction()
{
    UT_RESULT;

    /// - Initialize the test article with 1 chemical reaction of type SABATIER_RXN.
    mConfigData->mReactionTypes[0] = ChemicalReaction::SABATIER_RXN; ///< CO2 + 4H2 --> 2H2O + CH4
    mConfigData->mReactionTypes[1] = ChemicalReaction::CH4_REMOVAL;  ///< CH4 + 2O2 --> CO2 + 2H2O
    mConfigData->mReactionTypes[2] = ChemicalReaction::NH3_REMOVAL;  ///< 2NH3 + H3PO4 --> (NH4)2(HPO4)
    mConfigData->mReactionTypes[3] = ChemicalReaction::CO_REMOVAL;   ///< 2CO + O2 --> 2CO2

    mTotalMasses[0] = 0.0;
    mTotalMasses[1] = 0.0;
    mTotalMasses[2] = 0.0;
    mTotalMasses[3] = 0.0;
    mTotalMasses[4] = 0.0;
    mTotalMasses[5] = 0.0;
    mTotalMasses[6] = 0.0;
    mTotalMasses[7] = 0.0;
    mTotalMasses[8] = 0.0;

    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mArticle->mNReactions    = 1;

    /// - Set up link conduction and node pressures to get a desired flow rate ~ 1.0 for the test.
    mNodes[0].setPotential(200.0);
    mNodes[1].setPotential(100.0);
    mNodes[0].getContent()->updateMass();
    mNodes[1].getContent()->updateMass();
    mArticle->mAdmittanceMatrix[0] = 3.5e-04;
    mArticle->mPotentialVector[0]  = 200.0;
    mArticle->mPotentialVector[1]  = 100.0;

    /// - Call the method under test.
    mArticle->computeFlows(mTimeStep);

    /// - Compute expected results.
    mFlowRate = mArticle->mFlowRate;
    const double moleRatioH2      = mReactions.getReaction(ChemicalReaction::SABATIER_RXN)->mReagentMoleRatio;
    const double moleRatioH2O     = mReactions.getReaction(ChemicalReaction::SABATIER_RXN)->mProduct1MoleRatio;
    const double mWeightH2        = mCompounds.getCompound(ChemicalCompound::H2)  -> mMWeight;
    const double mWeightH2O       = mCompounds.getCompound(ChemicalCompound::H2O) -> mMWeight;

    const int    indexH2          = mNodes[0].getContent()->find(FluidProperties::GUNNS_H2);
    const int    indexH2O         = mNodes[0].getContent()->find(FluidProperties::GUNNS_H2O);
    const double availableMassH2  = mNodes[0].getContent()->getMassFraction(mNodes[0].getContent()->getType(indexH2))  * fabs(mFlowRate) * mTimeStep;
    const double availableMassH2O = mNodes[0].getContent()->getMassFraction(mNodes[0].getContent()->getType(indexH2O)) * fabs(mFlowRate) * mTimeStep;

    const double massRatioH2      = moleRatioH2  * mWeightH2;
    const double massRatioH2O     = moleRatioH2O * mWeightH2O;

    // Expect H2 to be the limiting reagent, and reactor to only use 0.9999 of it.
    const double maxReactantMass  = availableMassH2 * 0.9999;
    const double product1Mass     = maxReactantMass * massRatioH2O / massRatioH2;
    const double massH2O          = product1Mass;
    const double totalMassH2O     = availableMassH2O + product1Mass;

    /// @test  Reaction masses.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(massH2O,      mArticle->mCompounds[6].mMass,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassH2O, mArticle->mCompounds[6].mTotalMass, DBL_EPSILON);

    /// @test  Heat of reaction.
    const double heatOfRxn        = mReactions.getReaction(ChemicalReaction::SABATIER_RXN)->mReactionHeat;
    const double heatFactor       = heatOfRxn / moleRatioH2O;
    const double expectedHeat     = heatFactor * 1000000 * massH2O / mWeightH2O;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, mArticle->mProducedHeat,            DBL_EPSILON);

    // TODO internal fluid T
//    const double reactantsFlowRate = (massH2 + massO2) / mTimeStep;
//    const double productsFlowRate  = (massH2O)         / mTimeStep;
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(reactantsFlowRate, mArticle->mReactantsFlowRate,  DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(productsFlowRate,  mArticle->mProductsFlowRate,   DBL_EPSILON);

//    /// @test  Mass fractions of each product and reactant in the product & reactant fluids.
//    const double xH2O = 1.0;
//    const double xO2  = massRatioO2 / (massRatioO2 + massRatioH2);
//    const double xH2  = massRatioH2 / (massRatioO2 + massRatioH2);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(xH2O,         mArticle->mProductsFluid->getMassFraction(mArticle->mProductsFluid->getType(indexH2O)), DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(xO2,          mArticle->mReactantsFluid->getMassFraction(mArticle->mProductsFluid->getType(indexO2)), DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(xH2,          mArticle->mReactantsFluid->getMassFraction(mArticle->mProductsFluid->getType(indexH2)), DBL_EPSILON);
//
//    /// @test  Downstream node's inflow has the right mixture, flow rate and temperature.
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,          mNodes[1].getInflux(),              FLT_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassH2O / mFlowRate / mTimeStep, mNodes[1].getInflow()->getMassFraction(mNodes[1].getContent()->getType(indexH2O)), FLT_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassO2  / mFlowRate / mTimeStep, mNodes[1].getInflow()->getMassFraction(mNodes[1].getContent()->getType(indexO2)),  FLT_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(totalMassH2  / mFlowRate / mTimeStep, mNodes[1].getInflow()->getMassFraction(mNodes[1].getContent()->getType(indexH2)),  FLT_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mInternalFluid->getTemperature(), mNodes[1].getInflow()->getTemperature(), FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Reactor link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidReactor::testInitializationException()
{
    UT_RESULT;

    {
            /// @test    Initialization exception on invalid config data: null pointer to name.
            GunnsFluidReactorConfigData configData("",
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness);
            GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
            GunnsFluidReactor article;
            CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),
                                 TsInitializationException);
        }
    {
            /// @test    Initialization exception on invalid config data: max conductivity < 0.
            GunnsFluidReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  -DBL_EPSILON,
                                                  mExpansionScaleFactor,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness);
            GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
            GunnsFluidReactor article;
            CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),
                                 TsInitializationException);
        }
    {
            /// @test    Initialization exception on invalid config data: expansion scale factor < 0.
            GunnsFluidReactorConfigData configData(mName,
                                                  &mNodeList,
                                                  mMaxConductivity,
                                                  -DBL_EPSILON,
                                                  &mReactions,
                                                  mReactionTypes,
                                                  UtGunnsFluidReactor::N_REACTIONS,
                                                  &mCompounds,
                                                  mCompoundTypes,
                                                  UtGunnsFluidReactor::N_COMPOUNDS,
                                                  mThermalLength,
                                                  mThermalDiameter,
                                                  mSurfaceRoughness);
            GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
            GunnsFluidReactor article;
            CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),
                                 TsInitializationException);
        }
    {
        /// @test    Initialization exception on invalid config data: null pointer to compounds.
        GunnsFluidReactorConfigData configData(mName,
                                              &mNodeList,
                                              mMaxConductivity,
                                              mExpansionScaleFactor,
                                              &mReactions,
                                              mReactionTypes,
                                              UtGunnsFluidReactor::N_REACTIONS,
                                              0,
                                              mCompoundTypes,
                                              UtGunnsFluidReactor::N_COMPOUNDS,
                                              mThermalLength,
                                              mThermalDiameter,
                                              mSurfaceRoughness);
        GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
    }
    {
        /// @test    Initialization exception on invalid config data: no compounds specified.
        GunnsFluidReactorConfigData configData(mName,
                                             &mNodeList,
                                             mMaxConductivity,
                                             mExpansionScaleFactor,
                                             &mReactions,
                                             mReactionTypes,
                                             UtGunnsFluidReactor::N_REACTIONS,
                                             &mCompounds,
                                             mCompoundTypes,
                                             0,
                                             mThermalLength,
                                             mThermalDiameter,
                                             mSurfaceRoughness);
        GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
    }
    {
        /// @test    Initialization exception on invalid config data: undefined compound specified.
        mCompoundTypes[0] = ChemicalCompound::NO_COMPOUND;
        GunnsFluidReactorConfigData configData(mName,
                                             &mNodeList,
                                             mMaxConductivity,
                                             mExpansionScaleFactor,
                                             &mReactions,
                                             mReactionTypes,
                                             UtGunnsFluidReactor::N_REACTIONS,
                                             &mCompounds,
                                             mCompoundTypes,
                                             UtGunnsFluidReactor::N_COMPOUNDS,
                                             mThermalLength,
                                             mThermalDiameter,
                                             mSurfaceRoughness);
        GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
        mCompoundTypes[0] = ChemicalCompound::ChemicalCompound::NH42HPO4;
    }
    {
        /// @test    Initialization exception on invalid config data: null pointer to reactions.
        GunnsFluidReactorConfigData configData(mName,
                                             &mNodeList,
                                             mMaxConductivity,
                                             mExpansionScaleFactor,
                                             0,
                                             mReactionTypes,
                                             UtGunnsFluidReactor::N_REACTIONS,
                                             &mCompounds,
                                             mCompoundTypes,
                                             UtGunnsFluidReactor::N_COMPOUNDS,
                                             mThermalLength,
                                             mThermalDiameter,
                                             mSurfaceRoughness);
        GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
    }
    {
        /// @test    Initialization exception on invalid config data: no reactions specified.
        GunnsFluidReactorConfigData configData(mName,
                                             &mNodeList,
                                             mMaxConductivity,
                                             mExpansionScaleFactor,
                                             &mReactions,
                                             mReactionTypes,
                                             0,
                                             &mCompounds,
                                             mCompoundTypes,
                                             UtGunnsFluidReactor::N_COMPOUNDS,
                                             mThermalLength,
                                             mThermalDiameter,
                                             mSurfaceRoughness);
        GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
    }
    {
        /// @test    Initialization exception on invalid config data: undefined reaction type
        ///          specified.
        mReactionTypes[0] = ChemicalReaction::NO_REACTION;
        GunnsFluidReactorConfigData configData(mName,
                                             &mNodeList,
                                             mMaxConductivity,
                                             mExpansionScaleFactor,
                                             &mReactions,
                                             mReactionTypes,
                                             UtGunnsFluidReactor::N_REACTIONS,
                                             &mCompounds,
                                             mCompoundTypes,
                                             UtGunnsFluidReactor::N_COMPOUNDS,
                                             mThermalLength,
                                             mThermalDiameter,
                                             mSurfaceRoughness);
        GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
        mReactionTypes[0] = ChemicalReaction::NH3_REMOVAL;
    }
    {
        /// @test    Initialization exception on invalid config data: undefined reaction reactant.
        mCompoundTypes[0] = ChemicalCompound::NH42HPO4;   mTotalMasses[0] = 0.5;
        mCompoundTypes[1] = ChemicalCompound::H3PO4;      mTotalMasses[1] = 0.5;
        mCompoundTypes[2] = ChemicalCompound::NH3;        mTotalMasses[2] = 0.0;
        mCompoundTypes[3] = ChemicalCompound::CH4;        mTotalMasses[3] = 0.0;
        mCompoundTypes[4] = ChemicalCompound::H2;         mTotalMasses[4] = 0.0;
        mCompoundTypes[5] = ChemicalCompound::O2;         mTotalMasses[5] = 0.0;
        mCompoundTypes[6] = ChemicalCompound::H2O;        mTotalMasses[6] = 0.0;
        mCompoundTypes[7] = ChemicalCompound::CO2;        mTotalMasses[7] = 0.0;
        GunnsFluidReactorConfigData configData(mName,
                                             &mNodeList,
                                             mMaxConductivity,
                                             mExpansionScaleFactor,
                                             &mReactions,
                                             mReactionTypes,
                                             UtGunnsFluidReactor::N_REACTIONS,
                                             &mCompounds,
                                             mCompoundTypes,
                                             UtGunnsFluidReactor::N_COMPOUNDS-1,
                                             mThermalLength,
                                             mThermalDiameter,
                                             mSurfaceRoughness);
        GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
    }
    {
        /// @test    Initialization exception on invalid config data: undefined reaction reagent.
        mCompoundTypes[0] = ChemicalCompound::NH42HPO4;   mTotalMasses[0] = 0.5;
        mCompoundTypes[1] = ChemicalCompound::H3PO4;      mTotalMasses[1] = 0.5;
        mCompoundTypes[2] = ChemicalCompound::NH3;        mTotalMasses[2] = 0.0;
        mCompoundTypes[3] = ChemicalCompound::CH4;        mTotalMasses[3] = 0.0;
        mCompoundTypes[4] = ChemicalCompound::H2;         mTotalMasses[4] = 0.0;
        mCompoundTypes[5] = ChemicalCompound::H2O;        mTotalMasses[5] = 0.0;
        mCompoundTypes[6] = ChemicalCompound::CO;         mTotalMasses[6] = 0.0;
        mCompoundTypes[7] = ChemicalCompound::CO2;        mTotalMasses[7] = 0.0;
        GunnsFluidReactorConfigData configData(mName,
                                             &mNodeList,
                                             mMaxConductivity,
                                             mExpansionScaleFactor,
                                             &mReactions,
                                             mReactionTypes,
                                             UtGunnsFluidReactor::N_REACTIONS,
                                             &mCompounds,
                                             mCompoundTypes,
                                             UtGunnsFluidReactor::N_COMPOUNDS-1,
                                             mThermalLength,
                                             mThermalDiameter,
                                             mSurfaceRoughness);
        GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
    }
    {
        /// @test    Initialization exception on invalid config data: undefined reaction product 1.
        mCompoundTypes[0] = ChemicalCompound::H3PO4;      mTotalMasses[0] = 0.5;
        mCompoundTypes[1] = ChemicalCompound::NH3;        mTotalMasses[1] = 0.0;
        mCompoundTypes[2] = ChemicalCompound::CH4;        mTotalMasses[2] = 0.0;
        mCompoundTypes[3] = ChemicalCompound::H2;         mTotalMasses[3] = 0.0;
        mCompoundTypes[4] = ChemicalCompound::O2;         mTotalMasses[4] = 0.0;
        mCompoundTypes[5] = ChemicalCompound::H2O;        mTotalMasses[5] = 0.0;
        mCompoundTypes[6] = ChemicalCompound::CO;         mTotalMasses[6] = 0.0;
        mCompoundTypes[7] = ChemicalCompound::CO2;        mTotalMasses[7] = 0.0;
        GunnsFluidReactorConfigData configData(mName,
                                             &mNodeList,
                                             mMaxConductivity,
                                             mExpansionScaleFactor,
                                             &mReactions,
                                             mReactionTypes,
                                             UtGunnsFluidReactor::N_REACTIONS,
                                             &mCompounds,
                                             mCompoundTypes,
                                             UtGunnsFluidReactor::N_COMPOUNDS-1,
                                             mThermalLength,
                                             mThermalDiameter,
                                             mSurfaceRoughness);
        GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, mWallTemperature);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, inputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
    }
    {
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
        GunnsFluidReactorConfigData configData(mName,
                                             &mNodeList,
                                             mMaxConductivity,
                                             mExpansionScaleFactor,
                                             &mReactions,
                                             mReactionTypes,
                                             UtGunnsFluidReactor::N_REACTIONS,
                                             &mCompounds,
                                             mCompoundTypes,
                                             UtGunnsFluidReactor::N_COMPOUNDS,
                                             mThermalLength,
                                             mThermalDiameter,
                                             mSurfaceRoughness);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, *mInputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
    }
    {
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
        GunnsFluidReactorConfigData configData(mName,
                                             &mNodeList,
                                             mMaxConductivity,
                                             mExpansionScaleFactor,
                                             &mReactions,
                                             mReactionTypes,
                                             UtGunnsFluidReactor::N_REACTIONS,
                                             &mCompounds,
                                             mCompoundTypes,
                                             UtGunnsFluidReactor::N_COMPOUNDS,
                                             mThermalLength,
                                             mThermalDiameter,
                                             mSurfaceRoughness);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(configData, *mInputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
        mNodeList.mNodes = mNodes;
    }
    {
        /// @test    Initialization exception on invalid input data: mMalfBlockageValue < 0.
        GunnsFluidReactorInputData inputData(mMalfBlockageFlag, -DBL_EPSILON, mTotalMasses, mWallTemperature);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, inputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
    } {
        /// @test    Initialization exception on invalid input data: mMalfBlockageValue > 1.
        GunnsFluidReactorInputData inputData(mMalfBlockageFlag, 1.0 + DBL_EPSILON, mTotalMasses, mWallTemperature);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, inputData, mLinks, mPort0, mPort1),
                             TsInitializationException);
    } {
        /// @test    Initialization exception on invalid input data: mReactorTemperature < 0.
        GunnsFluidReactorInputData inputData(mMalfBlockageFlag, mMalfBlockageValue, mTotalMasses, -0.5 * DBL_EPSILON);
        GunnsFluidReactor article;
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, inputData, mLinks, mPort0, mPort1), TsInitializationException);
    }

    UT_PASS_LAST;
}
