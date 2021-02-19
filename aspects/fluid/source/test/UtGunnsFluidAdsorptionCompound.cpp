/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/source/GunnsFluidAdsorptionCompound.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"
#include "math/MsMath.hh"
#include "UtGunnsFluidAdsorptionCompound.hh"

/// @details  Test identification number.
int UtGunnsFluidAdsorptionCompound::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Absorbed Compound model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidAdsorptionCompound::UtGunnsFluidAdsorptionCompound()
    :
    CppUnit::TestFixture(),
    tFluidProperties(0),
    tTypes(),
    tFractions(),
    tCompoundProperties(),
    tTcTypes(),
    tFluidTcConfig(),
    tTcFractions(),
    tFluidTcInput(),
    tFluidConfig(0),
    tFluidInput(0),
    tFluid(0),
    tName(""),
    tConfig(),
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Absorbed Compound model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidAdsorptionCompound::~UtGunnsFluidAdsorptionCompound()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidAdsorptionCompound::setUp()
{
    /// - Define the nominal port fluids.
    tFluidProperties    = new DefinedFluidProperties();
    tTypes[0]           = FluidProperties::GUNNS_CO2;
    tTypes[1]           = FluidProperties::GUNNS_H2O;
    tTypes[2]           = FluidProperties::GUNNS_O2;

    tCompoundProperties = new DefinedChemicalCompounds();
    tTcTypes[0]         = ChemicalCompound::CH4;
    tTcTypes[1]         = ChemicalCompound::O2;
    tTcTypes[2]         = ChemicalCompound::C4H10O;
    tFluidTcConfig      = new GunnsFluidTraceCompoundsConfigData(tTcTypes, N_TC, "tFluidTcConfig");

    tFluidConfig        = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS, tFluidTcConfig);

    tFractions[0]       = 0.3;
    tFractions[1]       = 0.7;
    tFractions[2]       = 0.0;
    tTcFractions[0]     = 1.0e-5;
    tTcFractions[1]     = 2.0e-6;
    tTcFractions[2]     = 3.0e-7;
    tFluidTcInput       = new GunnsFluidTraceCompoundsInputData(tTcFractions);

    tFluidInput         = new PolyFluidInputData(283.0,
                                                 109.0,
                                                   0.0,
                                                   1.0,
                                                 tFractions,
                                                 tFluidTcInput);

    tFluid = new PolyFluid(*tFluidConfig, *tFluidInput);

    /// - Define the nominal configuration data.
    tName                          = "tArticle";
    tConfig.mType                  = ChemicalCompound::CO2;
    tConfig.mEfficiencyCoeff0      = 0.5;
    tConfig.mEfficiencyCoeff1      = 0.001;
    tConfig.mMaxAdsorbedMass       = 1.0;
    tConfig.mDesorbPartialPressure = 0.05;
    tConfig.mDesorbRateFactor      = 0.1;
    tConfig.mHeatOfAdsorption      = -100.0;
    tConfig.mTaperOffFlag          = true;
    tConfig.mDependentType         = ChemicalCompound::H2O;
    tConfig.mMalfEfficiencyFlag    = true;
    tConfig.mMalfEfficiencyValue   = 0.2;
    tConfig.mAdsorbedMass          = 0.3;
    tConfig.mBreakthroughExp       = 10.0;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsFluidAdsorptionCompound;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAdsorptionCompound::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tFluid;
    delete tFluidInput;
    delete tFluidTcInput;
    delete tFluidConfig;
    delete tFluidTcConfig;
    delete tCompoundProperties;
    delete tFluidProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Absorbed Compound model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAdsorptionCompound::testDefaultConstruction()
{
    UT_RESULT_FIRST;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(ChemicalCompound::NO_COMPOUND == tArticle->mType);
    CPPUNIT_ASSERT(0.0                           == tArticle->mEfficiencyCoeff0);
    CPPUNIT_ASSERT(0.0                           == tArticle->mEfficiencyCoeff1);
    CPPUNIT_ASSERT(0.0                           == tArticle->mMaxAdsorbedMass);
    CPPUNIT_ASSERT(0.0                           == tArticle->mDesorbPartialPressure);
    CPPUNIT_ASSERT(0.0                           == tArticle->mDesorbRateFactor);
    CPPUNIT_ASSERT(0.0                           == tArticle->mHeatOfAdsorption);
    CPPUNIT_ASSERT(false                         == tArticle->mTaperOffFlag);
    CPPUNIT_ASSERT(ChemicalCompound::NO_COMPOUND == tArticle->mDependentType);
    CPPUNIT_ASSERT(false                         == tArticle->mMalfEfficiencyFlag);
    CPPUNIT_ASSERT(0.0                           == tArticle->mMalfEfficiencyValue);
    CPPUNIT_ASSERT(0.0                           == tArticle->mAdsorbedMass);
    CPPUNIT_ASSERT(0.0                           == tArticle->mFillFraction);
    CPPUNIT_ASSERT(0.0                           == tArticle->mAdsorptionRate);
    CPPUNIT_ASSERT(0.0                           == tArticle->mSorptionHeat);
    CPPUNIT_ASSERT(""                            == tArticle->mName);
    CPPUNIT_ASSERT(false                         == tArticle->mTraceCompound);
    CPPUNIT_ASSERT(-1                            == tArticle->mIndex);
    CPPUNIT_ASSERT(0.0                           == tArticle->mMWeight);
    CPPUNIT_ASSERT(0                             == tArticle->mFluid);
    CPPUNIT_ASSERT(0                             == tArticle->mDependentCompound);

    /// @test    New/delete for code coverage.
    GunnsFluidAdsorptionCompound* article = new GunnsFluidAdsorptionCompound();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Absorbed Compound model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAdsorptionCompound::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    GunnsFluidAdsorptionCompound dependentH2O;
    tArticle->initialize(tName, tConfig, tFluid, &dependentH2O);

    CPPUNIT_ASSERT(tConfig.mType                  == tArticle->mType);
    CPPUNIT_ASSERT(tConfig.mEfficiencyCoeff0      == tArticle->mEfficiencyCoeff0);
    CPPUNIT_ASSERT(tConfig.mEfficiencyCoeff1      == tArticle->mEfficiencyCoeff1);
    CPPUNIT_ASSERT(tConfig.mMaxAdsorbedMass       == tArticle->mMaxAdsorbedMass);
    CPPUNIT_ASSERT(tConfig.mDesorbPartialPressure == tArticle->mDesorbPartialPressure);
    CPPUNIT_ASSERT(tConfig.mDesorbRateFactor      == tArticle->mDesorbRateFactor);
    CPPUNIT_ASSERT(tConfig.mHeatOfAdsorption      == tArticle->mHeatOfAdsorption);
    CPPUNIT_ASSERT(tConfig.mTaperOffFlag          == tArticle->mTaperOffFlag);
    CPPUNIT_ASSERT(tConfig.mDependentType         == tArticle->mDependentType);
    CPPUNIT_ASSERT(tConfig.mMalfEfficiencyFlag    == tArticle->mMalfEfficiencyFlag);
    CPPUNIT_ASSERT(tConfig.mMalfEfficiencyValue   == tArticle->mMalfEfficiencyValue);
    CPPUNIT_ASSERT(tConfig.mAdsorbedMass          == tArticle->mAdsorbedMass);
    CPPUNIT_ASSERT(0.0                            == tArticle->mAdsorptionRate);
    CPPUNIT_ASSERT(0.0                            == tArticle->mSorptionHeat);
    CPPUNIT_ASSERT(tName                          == tArticle->mName);
    CPPUNIT_ASSERT(false                          == tArticle->mTraceCompound);
    CPPUNIT_ASSERT(0                              == tArticle->mIndex);
    CPPUNIT_ASSERT(tFluidProperties->mMWeightCO2  == tArticle->mMWeight);
    CPPUNIT_ASSERT(tFluid                         == tArticle->mFluid);
    CPPUNIT_ASSERT(&dependentH2O                  == tArticle->mDependentCompound);

    const double expectedFill = tConfig.mAdsorbedMass / tConfig.mMaxAdsorbedMass;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFill, tArticle->mFillFraction, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Absorbed Compound model nominal initialization of a trace
///           compound type without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAdsorptionCompound::testNominalInitializationTc()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    GunnsFluidAdsorptionCompound dependentH2O;
    tConfig.mType = ChemicalCompound::C4H10O;
    tArticle->initialize(tName, tConfig, tFluid, &dependentH2O);

    CPPUNIT_ASSERT(tConfig.mType                  == tArticle->mType);
    CPPUNIT_ASSERT(tConfig.mEfficiencyCoeff0      == tArticle->mEfficiencyCoeff0);
    CPPUNIT_ASSERT(tConfig.mEfficiencyCoeff1      == tArticle->mEfficiencyCoeff1);
    CPPUNIT_ASSERT(tConfig.mMaxAdsorbedMass       == tArticle->mMaxAdsorbedMass);
    CPPUNIT_ASSERT(tConfig.mDesorbPartialPressure == tArticle->mDesorbPartialPressure);
    CPPUNIT_ASSERT(tConfig.mDesorbRateFactor      == tArticle->mDesorbRateFactor);
    CPPUNIT_ASSERT(tConfig.mHeatOfAdsorption      == tArticle->mHeatOfAdsorption);
    CPPUNIT_ASSERT(tConfig.mTaperOffFlag          == tArticle->mTaperOffFlag);
    CPPUNIT_ASSERT(tConfig.mDependentType         == tArticle->mDependentType);
    CPPUNIT_ASSERT(tConfig.mMalfEfficiencyFlag    == tArticle->mMalfEfficiencyFlag);
    CPPUNIT_ASSERT(tConfig.mMalfEfficiencyValue   == tArticle->mMalfEfficiencyValue);
    CPPUNIT_ASSERT(tConfig.mAdsorbedMass          == tArticle->mAdsorbedMass);
    CPPUNIT_ASSERT(0.0                            == tArticle->mAdsorptionRate);
    CPPUNIT_ASSERT(0.0                            == tArticle->mSorptionHeat);
    CPPUNIT_ASSERT(tName                          == tArticle->mName);
    CPPUNIT_ASSERT(true                           == tArticle->mTraceCompound);
    CPPUNIT_ASSERT(2                              == tArticle->mIndex);
    const double expectedMW = tCompoundProperties->getCompound(ChemicalCompound::C4H10O)->mMWeight;
    CPPUNIT_ASSERT(expectedMW                     == tArticle->mMWeight);
    CPPUNIT_ASSERT(tFluid                         == tArticle->mFluid);
    CPPUNIT_ASSERT(&dependentH2O                  == tArticle->mDependentCompound);

    const double expectedFill = tConfig.mAdsorbedMass / tConfig.mMaxAdsorbedMass;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFill, tArticle->mFillFraction, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Absorbed Compound model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAdsorptionCompound::testCopyConstruction()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    GunnsFluidAdsorptionCompound dependentH2O;
    tArticle->initialize(tName, tConfig, tFluid, &dependentH2O);

    /// @test    Copy construction configuration data.
    FriendlyGunnsFluidAdsorptionCompound copy(*tArticle);

    CPPUNIT_ASSERT(tConfig.mType                  == copy.mType);
    CPPUNIT_ASSERT(tConfig.mEfficiencyCoeff0      == copy.mEfficiencyCoeff0);
    CPPUNIT_ASSERT(tConfig.mEfficiencyCoeff1      == copy.mEfficiencyCoeff1);
    CPPUNIT_ASSERT(tConfig.mMaxAdsorbedMass       == copy.mMaxAdsorbedMass);
    CPPUNIT_ASSERT(tConfig.mDesorbPartialPressure == copy.mDesorbPartialPressure);
    CPPUNIT_ASSERT(tConfig.mDesorbRateFactor      == copy.mDesorbRateFactor);
    CPPUNIT_ASSERT(tConfig.mHeatOfAdsorption      == copy.mHeatOfAdsorption);
    CPPUNIT_ASSERT(tConfig.mTaperOffFlag          == copy.mTaperOffFlag);
    CPPUNIT_ASSERT(tConfig.mDependentType         == copy.mDependentType);
    CPPUNIT_ASSERT(tConfig.mMalfEfficiencyFlag    == copy.mMalfEfficiencyFlag);
    CPPUNIT_ASSERT(tConfig.mMalfEfficiencyValue   == copy.mMalfEfficiencyValue);
    CPPUNIT_ASSERT(tConfig.mAdsorbedMass          == copy.mAdsorbedMass);
    CPPUNIT_ASSERT(0.0                            == copy.mAdsorptionRate);
    CPPUNIT_ASSERT(0.0                            == copy.mSorptionHeat);
    CPPUNIT_ASSERT(""                             == copy.mName);
    CPPUNIT_ASSERT(false                          == copy.mTraceCompound);
    CPPUNIT_ASSERT(-1                             == copy.mIndex);
    CPPUNIT_ASSERT(0.0                            == copy.mMWeight);
    CPPUNIT_ASSERT(0                              == copy.mFluid);
    CPPUNIT_ASSERT(0                              == copy.mDependentCompound);

    const double expectedFill = tConfig.mAdsorbedMass / tConfig.mMaxAdsorbedMass;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFill, copy.mFillFraction, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Absorbed Compound model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAdsorptionCompound::testAccessors()
{
    UT_RESULT;

    /// @test     isTraceCompound method.
    tArticle->mTraceCompound = true;
    CPPUNIT_ASSERT(true  == tArticle->isTraceCompound());
    tArticle->mTraceCompound = false;
    CPPUNIT_ASSERT(false == tArticle->isTraceCompound());

    /// @test    getIndex method.
    tArticle->mIndex = 2;
    CPPUNIT_ASSERT(2 == tArticle->getIndex());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Absorbed Compound model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAdsorptionCompound::testModifiers()
{
    UT_RESULT;

    /// @test    The efficiency override malfunction is set to given values.
    tArticle->setMalfEfficiency(true, -0.1);
    CPPUNIT_ASSERT(true  == tArticle->mMalfEfficiencyFlag);
    CPPUNIT_ASSERT(-0.1  == tArticle->mMalfEfficiencyValue);

    /// @test    The efficiency override malfunction is reset from default arguments.
    tArticle->setMalfEfficiency();
    CPPUNIT_ASSERT(false == tArticle->mMalfEfficiencyFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfEfficiencyValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Absorbed Compound model sorb method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAdsorptionCompound::testSorb()
{
    UT_RESULT;

    FriendlyGunnsFluidAdsorptionCompound dependentH2O;
    dependentH2O.mFillFraction = 0.5;
    tArticle->initialize(tName, tConfig, tFluid, &dependentH2O);

    {
        /// @test    Adsorption.
        const double dt   = 0.1;
        const double pAvg = 1.0;
        const double tAvg = 283.0;
        const double mdot = 1.0;

        const double expectedRate = (tConfig.mEfficiencyCoeff0 + tConfig.mEfficiencyCoeff1 * tAvg)
                                  * tFractions[0] * mdot * (1 - pow(std::max(0.1,tArticle->mFillFraction),
                                                                     MsMath::limitRange(1.0,tArticle->mBreakthroughExp,100.0)))
                                  * dependentH2O.mFillFraction * tArticle->mMalfEfficiencyValue;
        const double expectedMass = tArticle->mAdsorbedMass + expectedRate * dt;
        const double expectedHeat = -tConfig.mHeatOfAdsorption * expectedRate
                                  * 1000000.0 / tFluidProperties->mMWeightCO2;

        tArticle->sorb(dt, tAvg, pAvg, mdot);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate, tArticle->mAdsorptionRate, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, tArticle->mAdsorbedMass,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, tArticle->mSorptionHeat,   DBL_EPSILON);
    } {
        /// @test    Adsorption limits on breakthrough function.
        const double dt   = 0.1;
        const double pAvg = 1.0;
        const double tAvg = 283.0;
        const double mdot = 1.0;
        tArticle->mFillFraction    = 0.01;
        tArticle->mBreakthroughExp = 100.0;

        const double expectedRate = (tConfig.mEfficiencyCoeff0 + tConfig.mEfficiencyCoeff1 * tAvg)
                                  * tFractions[0] * mdot * (1 - pow(std::max(0.1,tArticle->mFillFraction),
                                                                     MsMath::limitRange(1.0,tArticle->mBreakthroughExp,100.0)))
                                  * dependentH2O.mFillFraction * tArticle->mMalfEfficiencyValue;
        const double expectedMass = tArticle->mAdsorbedMass + expectedRate * dt;
        const double expectedHeat = -tConfig.mHeatOfAdsorption * expectedRate
                                  * 1000000.0 / tFluidProperties->mMWeightCO2;

        tArticle->sorb(dt, tAvg, pAvg, mdot);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate, tArticle->mAdsorptionRate, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, tArticle->mAdsorbedMass,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, tArticle->mSorptionHeat,   DBL_EPSILON);
    } {
        /// @test    Desorption.
        const double dt   = 0.1;
        const double pAvg = 0.01;
        const double tAvg = 283.0;
        const double mdot = -1.0;

        const double expectedPp   = tFluid->getMoleFraction(FluidProperties::GUNNS_CO2) * pAvg;
        const double expectedRate = (tConfig.mEfficiencyCoeff0 + tConfig.mEfficiencyCoeff1 * tAvg)
                                  * (expectedPp - tConfig.mDesorbPartialPressure)
                                  * tConfig.mDesorbRateFactor * tArticle->mFillFraction
                                  * (1.0 - dependentH2O.mFillFraction)
                                  * tArticle->mMalfEfficiencyValue;
        const double expectedMass = tArticle->mAdsorbedMass + expectedRate * dt;
        const double expectedHeat = -tConfig.mHeatOfAdsorption * expectedRate
                                   * 1000000.0 / tFluidProperties->mMWeightCO2;

        tArticle->sorb(dt, tAvg, pAvg, mdot);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate, tArticle->mAdsorptionRate, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, tArticle->mAdsorbedMass,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, tArticle->mSorptionHeat,   DBL_EPSILON);

    } {
        /// @test    Adsorption without taper-off, dependent compound, or malfunction.
        tArticle->mMalfEfficiencyFlag = false;
        tArticle->mTaperOffFlag       = false;
        tArticle->mDependentCompound  = 0;

        const double dt   = 0.1;
        const double pAvg = 1.0;
        const double tAvg = 283.0;
        const double mdot = 1.0;

        const double expectedRate = (tConfig.mEfficiencyCoeff0 + tConfig.mEfficiencyCoeff1 * tAvg)
                                  * tFractions[0] * mdot;
        const double expectedMass = tArticle->mAdsorbedMass + expectedRate * dt;
        const double expectedHeat = -tConfig.mHeatOfAdsorption * expectedRate
                                  * 1000000.0 / tFluidProperties->mMWeightCO2;

        tArticle->sorb(dt, tAvg, pAvg, mdot);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate, tArticle->mAdsorptionRate, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, tArticle->mAdsorbedMass,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, tArticle->mSorptionHeat,   DBL_EPSILON);
    } {
        /// @test    Lower adsorbed mass limit.
        const double dt   = 0.1;
        const double pAvg = 0.01;
        const double tAvg = 283.0;
        const double mdot = -1.0;

        tArticle->mAdsorbedMass = -DBL_EPSILON;
        tArticle->mFillFraction = 0.0;
        tArticle->sorb(dt, tAvg, pAvg, mdot);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAdsorbedMass,   DBL_EPSILON);
    } {
        /// @test    Upper adsorbed mass limit.
        const double dt   = 0.1;
        const double pAvg = 1.0;
        const double tAvg = 283.0;
        const double mdot = 1.0;

        tArticle->mAdsorbedMass = tArticle->mMaxAdsorbedMass;
        tArticle->mFillFraction = 1.0;
        tArticle->sorb(dt, tAvg, pAvg, mdot);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mMaxAdsorbedMass, tArticle->mAdsorbedMass, DBL_EPSILON);
    } {
        /// @test    Zero fluid flow.
        const double dt   = 0.1;
        const double pAvg = 1.0;
        const double tAvg = 283.0;
        const double mdot = 0.0;

        tArticle->mAdsorbedMass = 0.0;
        tArticle->mFillFraction = 0.0;
        tArticle->sorb(dt, tAvg, pAvg, mdot);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAdsorptionRate, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mAdsorbedMass,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSorptionHeat,   DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Absorbed Compound model sorb method for trace compounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAdsorptionCompound::testSorbTc()
{
    UT_RESULT;

    FriendlyGunnsFluidAdsorptionCompound dependentH2O;
    dependentH2O.mFillFraction     = 0.5;
    tConfig.mType                  = ChemicalCompound::CH4;
    tConfig.mEfficiencyCoeff0      = -0.5;
    tConfig.mDesorbPartialPressure = 1.0e-7;
    tConfig.mDesorbRateFactor      = 1.0e-4;
    tArticle->initialize(tName, tConfig, tFluid, &dependentH2O);

    CPPUNIT_ASSERT(0 == tArticle->mIndex);
    {
        /// @test    Adsorption.
        const double dt   = 0.1;
        const double pAvg = 1.0;
        const double tAvg = 600.0;
        const double mdot = 1.0;

        const double expectedMdot = tArticle->mFluid->getTraceCompounds()->getMasses()[tArticle->mIndex];
        const double expectedRate = (tConfig.mEfficiencyCoeff0 + tConfig.mEfficiencyCoeff1 * tAvg)
                                  * expectedMdot * (1 - pow(std::max(0.1,tArticle->mFillFraction),
                                                             MsMath::limitRange(1.0,tArticle->mBreakthroughExp,100.0)))
                                  * dependentH2O.mFillFraction * tArticle->mMalfEfficiencyValue;
        const double expectedMass = tArticle->mAdsorbedMass + expectedRate * dt;
        const double expectedHeat = -tConfig.mHeatOfAdsorption * expectedRate
                                  * 1000000.0 / tFluidProperties->mMWeightCH4;

        tArticle->sorb(dt, tAvg, pAvg, mdot);

        CPPUNIT_ASSERT(0.0 < expectedMdot);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate, tArticle->mAdsorptionRate, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, tArticle->mAdsorbedMass,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, tArticle->mSorptionHeat,   DBL_EPSILON);
    } {
        /// @test    Desorption caused by negative efficiency but positive delta partial-pressure.
        const double dt   = 0.1;
        const double pAvg = 1.0;
        const double tAvg = 283.0;
        const double mdot = 1.0;

        const double expectedRate = 0.0;
        const double expectedMass = tArticle->mAdsorbedMass;
        const double expectedHeat = 0.0;

        tArticle->sorb(dt, tAvg, pAvg, mdot);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate, tArticle->mAdsorptionRate, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, tArticle->mAdsorbedMass,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, tArticle->mSorptionHeat,   DBL_EPSILON);
    } {
        //// @test    Test zero bulk fluid flow.
        const double dt   = 0.1;
        const double pAvg = 1.0;
        const double tAvg = 600.0;
        const double mdot = 0.0;

        tArticle->mFillFraction = 0.5;
        tArticle->mAdsorbedMass = 0.5 * tConfig.mMaxAdsorbedMass;

        const double expectedRate = 0.0;
        const double expectedMass = tArticle->mAdsorbedMass;
        const double expectedHeat = 0.0;

        tArticle->sorb(dt, tAvg, pAvg, mdot);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRate, tArticle->mAdsorptionRate, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, tArticle->mAdsorbedMass,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, tArticle->mSorptionHeat,   DBL_EPSILON);
    }
    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Absorbed Compound model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidAdsorptionCompound::testInitializationExceptions()
{
    UT_RESULT;

    /// @test    Initialization exception on invalid config data: no name.
    CPPUNIT_ASSERT_THROW(tArticle->initialize("", tConfig, tFluid, 0), TsInitializationException);

    /// @test    Initialization exception on invalid config data: invalid compound.
    tConfig.mType = ChemicalCompound::NO_COMPOUND;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tConfig, tFluid, 0), TsInitializationException);

    /// @test    Initialization exception on compound is both fluid type and trace compound.
    tConfig.mType = ChemicalCompound::O2;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tConfig, tFluid, 0), TsInitializationException);

    /// @test    Initialization exception on compound doesn't exist.
    tConfig.mType = ChemicalCompound::LIOH;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tConfig, tFluid, 0), TsInitializationException);
    tConfig.mType = ChemicalCompound::CO2;

    /// @test    Initialization exception on maximum adsorbed mass < DBL_EPSILON.
    tConfig.mMaxAdsorbedMass = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tConfig, tFluid, 0), TsInitializationException);
    tConfig.mMaxAdsorbedMass = 1.0;

    /// @test    Initialization exception on desorb rate factor < 0.
    tConfig.mDesorbRateFactor = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tConfig, tFluid, 0), TsInitializationException);
    tConfig.mDesorbRateFactor = 0.1;

    /// @test    Initialization exception on dependent compound doesn't match given object.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tConfig, tFluid, 0), TsInitializationException);
    GunnsFluidAdsorptionCompound dependentH2O;

    /// @test    Initialization exception on initial adsorbed mass not in limits.
    tConfig.mAdsorbedMass = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tConfig, tFluid, &dependentH2O), TsInitializationException);
    tConfig.mAdsorbedMass = tConfig.mMaxAdsorbedMass + 1.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tConfig, tFluid, &dependentH2O), TsInitializationException);
    tConfig.mAdsorbedMass = 0.3;

    /// @test    Initialization exception on invalid efficiency malf not in limits.
    tConfig.mMalfEfficiencyValue = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tConfig, tFluid, &dependentH2O), TsInitializationException);
    tConfig.mMalfEfficiencyValue = 1.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tConfig, tFluid, &dependentH2O), TsInitializationException);
    tConfig.mMalfEfficiencyValue = 0.5;

    /// @test    Initialization exception on breakthrough exponential not in limits.
    tConfig.mBreakthroughExp = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tConfig, tFluid, &dependentH2O), TsInitializationException);
    tConfig.mBreakthroughExp = 101.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName, tConfig, tFluid, &dependentH2O), TsInitializationException);
    tConfig.mBreakthroughExp = 1.0;

    UT_PASS_LAST;
}
