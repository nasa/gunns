/*
@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

#include "UtPolyFluid.hh"

/// @details  Test identification number.
int UtPolyFluid::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid PolyFluid model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtPolyFluid::UtPolyFluid()
    :
    CppUnit::TestFixture(),
    mTemperature(0.0),
    mPressure(0.0),
    mFlowRate(0.0),
    mMass(0.0),
    mMole1(0.0),
    mMole2(0.0),
    mMole3(0.0),
    mType1(),
    mType2(),
    mType3(),
    mMassFraction1(),
    mMassFraction2(),
    mMassFraction3(),
    mMoleFraction1(),
    mMoleFraction2(),
    mMoleFraction3(),
    mProperties(0),
    mConfigData1(0),
    mConfigData2(0),
    mConfigData3(0),
    mInputData1(0),
    mInputData2(0),
    mInputData3(0),
    mArticle1(0),
    mArticle2(0),
    mArticle3(0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid PolyFluid model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtPolyFluid::~UtPolyFluid()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::setUp()
{
    /// - Define common initialization data
    mTemperature      = 300.0;
    mPressure         = 140.0;
    mFlowRate         = 1.0;
    mMass             = 1.0;

    /// - defined fluid properties
    mProperties = new DefinedFluidProperties();

    /// - Define initialization data for multi constituent test article
    mType1[0]         = FluidProperties::GUNNS_N2;  mMassFraction1[0] = 0.750;
    mType1[1]         = FluidProperties::GUNNS_O2;  mMassFraction1[1] = 0.200;
    mType1[2]         = FluidProperties::GUNNS_CO2; mMassFraction1[2] = 0.020;
    mType1[3]         = FluidProperties::GUNNS_CO;  mMassFraction1[3] = 0.005;
    mType1[4]         = FluidProperties::GUNNS_H2O; mMassFraction1[4] = 0.010;
    mType1[5]         = FluidProperties::GUNNS_NH3; mMassFraction1[5] = 0.010;
    mType1[6]         = FluidProperties::GUNNS_H2;  mMassFraction1[6] = 0.005;
    mConfigData1      = new PolyFluidConfigData(mProperties, mType1, UtPolyFluid::NMULTI);
    mInputData1       = new PolyFluidInputData(mTemperature,  mPressure, mFlowRate, mMass,
                                               mMassFraction1);

    /// - Define multi constituent test article
    mArticle1         = new FriendlyPolyFluid(*mConfigData1, *mInputData1);

    /// - Compute mole fractions for multi constituent test article
    double sumMole = 0.0;
    for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
        FluidProperties::FluidType type = mType1[i];
        const double mWeight = mProperties->getProperties(type)->getMWeight();
        mMoleFraction1[i]    = mMassFraction1[i] / mWeight;
        sumMole             += mMoleFraction1[i];
    }
    mMole1            = mMass * sumMole;
    for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
        mMoleFraction1[i]   /= sumMole;
    };

    /// - Define single liquid initialization data
    mType2[0]         = FluidProperties::GUNNS_WATER;  mMassFraction2[0] = 1.000;
    mConfigData2      = new PolyFluidConfigData(mProperties, mType2, UtPolyFluid::NSINGLE);
    mInputData2       = new PolyFluidInputData(mTemperature,  mPressure, mFlowRate, mMass,
                                               mMassFraction2);

    /// - Define single liquid test article
    mArticle2         = new FriendlyPolyFluid(*mConfigData2, *mInputData2);

    /// - Compute mole and mole fractions for single liquid test article
    mMoleFraction2[0] = 1.000;
    mMole2            = mMass /
        mProperties->getProperties(FluidProperties::GUNNS_WATER)->getMWeight();

    /// - Define single non-zero fraction initialization data
    mType3[0]         = FluidProperties::GUNNS_HFE7000; mMassFraction3[0] = 0.000;
    mType3[1]         = FluidProperties::GUNNS_O2;      mMassFraction3[1] = 1.000;
    mConfigData3      = new PolyFluidConfigData(mProperties, mType3, UtPolyFluid::NDUAL);
    mInputData3       = new PolyFluidInputData(mTemperature,  mPressure, mFlowRate, mMass,
                                               mMassFraction3);

    /// - Define single non-zero fraction test article
    mArticle3         = new FriendlyPolyFluid(*mConfigData3, *mInputData3);

    /// - Compute mole fractions for single non-zero fraction test article
    mMoleFraction3[0] = 0.000;
    mMoleFraction3[1] = 1.000;
    mMole3            = mMass /
        mProperties->getProperties(FluidProperties::GUNNS_O2)->getMWeight();

    /// - Define nominal time step and comparison tolerance
    mTimeStep         = 0.1;
    mTolerance        = 1.0e-06;

    /// - Increment the test identification number.
    ++TEST_ID;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::tearDown()
{
    /// - Delete the objects created in setUp
    delete mArticle3;
    delete mInputData3;
    delete mConfigData3;
    delete mArticle2;
    delete mInputData2;
    delete mConfigData2;
    delete mArticle1;
    delete mInputData1;
    delete mConfigData1;
    delete mProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test for multi constituent config construction
    for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
        CPPUNIT_ASSERT(mType1[i] == mConfigData1->mTypes[i]);
    }
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(UtPolyFluid::NMULTI), mConfigData1->mNTypes);

    /// @test for multi constituent input construction
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, mInputData1->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure,    mInputData1->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,    mInputData1->mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMass,        mInputData1->mMass,        mTolerance);
    for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMassFraction1[i], mInputData1->mMassFraction[i], mTolerance);
    }

    /// @test for single liquid config construction
    for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
        CPPUNIT_ASSERT(mType2[i] == mConfigData2->mTypes[i]);
    }
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(UtPolyFluid::NSINGLE), mConfigData2->mNTypes);

    /// @test for single liquid input construction
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, mInputData2->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure,    mInputData2->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,    mInputData2->mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMass,        mInputData2->mMass,        mTolerance);
    for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMassFraction2[i], mInputData2->mMassFraction[i], mTolerance);
    }

    /// @test for single non-zero fraction config construction
    for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
        CPPUNIT_ASSERT(mType3[i] == mConfigData3->mTypes[i]);
    }
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(UtPolyFluid::NDUAL), mConfigData3->mNTypes);

    /// @test for single non-zero fraction input construction
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, mInputData3->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure,    mInputData3->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,    mInputData3->mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMass,        mInputData3->mMass,        mTolerance);
    for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMassFraction3[i], mInputData3->mMassFraction[i], mTolerance);
    }

    /// @test for multi constituent copy config construction
    PolyFluidConfigData   copyConfig1(*mConfigData1);
    for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
        CPPUNIT_ASSERT(mConfigData1->mTypes[i] == copyConfig1.mTypes[i]);
    }
    CPPUNIT_ASSERT_EQUAL(mConfigData1->mNTypes, copyConfig1.mNTypes);

    /// @test for multi constituent copy input construction
    PolyFluidInputData   copyInput1(*mInputData1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData1->mTemperature, copyInput1.mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData1->mPressure,    copyInput1.mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData1->mFlowRate,    copyInput1.mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData1->mMass,        copyInput1.mMass,        mTolerance);
    for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData1->mMassFraction[i],
                                     copyInput1.mMassFraction[i],
                                     mTolerance);
    }

    /// @test for single liquid copy config construction
    PolyFluidConfigData   copyConfig2(*mConfigData2);
    for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
        CPPUNIT_ASSERT(mConfigData2->mTypes[i] == copyConfig2.mTypes[i]);
    }
    CPPUNIT_ASSERT_EQUAL(mConfigData2->mNTypes, copyConfig2.mNTypes);

    /// @test for single liquid  copy input construction
    PolyFluidInputData   copyInput2(*mInputData2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData2->mTemperature, copyInput2.mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData2->mPressure,    copyInput2.mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData2->mFlowRate,    copyInput2.mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData2->mMass,        copyInput2.mMass,        mTolerance);
    for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData2->mMassFraction[i],
                                     copyInput2.mMassFraction[i],
                                     mTolerance);
    }

    /// @test for single non-zero fraction copy config construction
    PolyFluidConfigData   copyConfig3(*mConfigData3);
    for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
        CPPUNIT_ASSERT(mConfigData3->mTypes[i] == copyConfig3.mTypes[i]);
    }
    CPPUNIT_ASSERT_EQUAL(mConfigData3->mNTypes, copyConfig3.mNTypes);

    /// @test for single non-zero fraction copy input construction
    PolyFluidInputData   copyInput3(*mInputData3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData3->mTemperature, copyInput3.mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData3->mPressure,    copyInput3.mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData3->mFlowRate,    copyInput3.mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData3->mMass,        copyInput3.mMass,        mTolerance);
    for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData3->mMassFraction[i],
                                     copyInput3.mMassFraction[i],
                                     mTolerance);
    }

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testNominalConstruction()
{
    UT_RESULT;

    /// @test for composite state attributes
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, mArticle1->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, mArticle2->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, mArticle3->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure,    mArticle1->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure,    mArticle2->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure,    mArticle3->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,    mArticle1->mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,    mArticle2->mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,    mArticle3->mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMass,        mArticle1->mMass,        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMass,        mArticle2->mMass,        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMass,        mArticle3->mMass,        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMole1,       mArticle1->mMole,        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMole2,       mArticle2->mMole,        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMole3,       mArticle3->mMole,        mTolerance);

    /// @test for constituent state attributes
    for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
        CPPUNIT_ASSERT(mType1[i] == mArticle1->mConstituents[i].mType);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMassFraction1[i],
                                     mArticle1->mConstituents[i].mMassFraction,
                                     mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoleFraction1[i],
                                     mArticle1->mConstituents[i].mMoleFraction,
                                     mTolerance);
    }
    for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
        CPPUNIT_ASSERT(mType2[i] == mArticle2->mConstituents[i].mType);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMassFraction2[i],
                                     mArticle2->mConstituents[i].mMassFraction,
                                     mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoleFraction2[i],
                                     mArticle2->mConstituents[i].mMoleFraction,
                                     mTolerance);
    }
    for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
        CPPUNIT_ASSERT(mType3[i] == mArticle3->mConstituents[i].mType);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMassFraction3[i],
                                     mArticle3->mConstituents[i].mMassFraction,
                                     mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoleFraction3[i],
                                     mArticle3->mConstituents[i].mMoleFraction,
                                     mTolerance);
    }

    /// @test for composite state properties
    {
        CPPUNIT_ASSERT(FluidProperties::GAS == mArticle1->mPhase);

        const FluidProperties* properties[NMULTI];
        for (int i = 0; i < NMULTI; i++) {
            properties[i] = mProperties->getProperties(mType1[i]);
        }

        double expected = mMass / mMole1;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle1->mMWeight,          mTolerance);

        // composite density is sum of constituent densities at partial pressure
        expected = 0.0;
        for (int i = 0; i < NMULTI; i++) {
            expected += properties[i]->getDensity(mTemperature, mMoleFraction1[i] * mPressure);
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle1->mDensity,          mTolerance);

        // composite viscosity is mole fraction weighted sum of constituent viscosities at partial
        // pressure
        expected = 0.0;
        for (int i = 0; i < NMULTI; i++) {
            expected += properties[i]->getViscosity(mTemperature,
                mMoleFraction1[i] * mPressure) * mMoleFraction1[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle1->mViscosity,        mTolerance);

        // composite specific heat is mass fraction weighted sum of constituent specific heats at
        // partial pressure
        expected = 0.0;
        for (int i = 0; i < NMULTI; i++) {
            expected += properties[i]->getSpecificHeat(mTemperature,
                mMoleFraction1[i] * mPressure) * mMassFraction1[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle1->mSpecificHeat,     mTolerance);

        // composite specific enthalpy is temperature times composite specific heat
        expected *= mTemperature;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle1->mSpecificEnthalpy, mTolerance);

        // composite adiabatic index is mole fraction weighted sum of constituent adiabatic indices
        // at partial pressure
        expected = 0.0;
        for (int i = 0; i < NMULTI; i++) {
            expected += properties[i]->getAdiabaticIndex(mTemperature,
                mMoleFraction1[i] * mPressure) * mMoleFraction1[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle1->mAdiabaticIndex,   mTolerance);
    } {
        CPPUNIT_ASSERT(FluidProperties::LIQUID == mArticle2->mPhase);

        const FluidProperties* properties =
            mProperties->getProperties(FluidProperties::GUNNS_WATER);

        double expected = properties->getMWeight();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle2->mMWeight,          mTolerance);

        expected = properties->getDensity(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle2->mDensity,          mTolerance);

        expected = properties->getViscosity(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle2->mViscosity,        mTolerance);

        expected = properties->getSpecificHeat(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle2->mSpecificHeat,     mTolerance);

        expected = properties->getSpecificEnthalpy(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle2->mSpecificEnthalpy, mTolerance);

        expected = properties->getAdiabaticIndex(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle2->mAdiabaticIndex,   mTolerance);
    } {
        CPPUNIT_ASSERT(FluidProperties::GAS == mArticle1->mPhase);

        const FluidProperties* properties = mProperties->getProperties(FluidProperties::GUNNS_O2);

        double expected = properties->getMWeight();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle3->mMWeight,          mTolerance);

        expected = properties->getDensity(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle3->mDensity,          mTolerance);

        expected = properties->getViscosity(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle3->mViscosity,        mTolerance);

        expected = properties->getSpecificHeat(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle3->mSpecificHeat,     mTolerance);

        expected = properties->getSpecificEnthalpy(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle3->mSpecificEnthalpy, mTolerance);

        expected = properties->getAdiabaticIndex(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle3->mAdiabaticIndex,   mTolerance);
    }

    /// @test for initialization flag
    CPPUNIT_ASSERT(mArticle1->mInitFlag);
    CPPUNIT_ASSERT(mArticle2->mInitFlag);
    CPPUNIT_ASSERT(mArticle3->mInitFlag);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testDefaultConstruction()
{
    UT_RESULT;

    /// - Default construct a test article
    FriendlyPolyFluid article;

    /// @test for composite state attributes
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mMass,        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mMole,        mTolerance);

    /// @test for constituent attributes
    CPPUNIT_ASSERT(0 == article.mConstituents);
    CPPUNIT_ASSERT_EQUAL(0, article.mNConstituents);

    /// @test for composite properties
    CPPUNIT_ASSERT(FluidProperties::NO_PHASE == article.mPhase);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mMWeight, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mDensity, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mViscosity, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mSpecificHeat, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mSpecificEnthalpy, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mAdiabaticIndex, mTolerance);

    /// @test for initialization flag
    CPPUNIT_ASSERT(!article.mInitFlag);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for copy construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testCopyConstruction()
{
    UT_RESULT;

    {
        /// - Copy multi constituent test article
        FriendlyPolyFluid* article ;
        TS_NEW_CLASS_OBJECT(article, FriendlyPolyFluid, (*mArticle1, "article"));

        /// @test for composite state attributes
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mTemperature, article->mTemperature, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mPressure,    article->mPressure,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mFlowRate,    article->mFlowRate,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mMass,        article->mMass,        mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mMole,        article->mMole,        mTolerance);

        /// @test for constituent state attributes
        CPPUNIT_ASSERT_EQUAL(mArticle1->mNConstituents, article->mNConstituents);
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            CPPUNIT_ASSERT(mArticle1->mConstituents[i].mType == article->mConstituents[i].mType);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mConstituents[i].mMassFraction,
                                         article->mConstituents[i].mMassFraction,
                                         mTolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mConstituents[i].mMoleFraction,
                                         article->mConstituents[i].mMoleFraction,
                                         mTolerance);
        }

        /// @test for composite state properties
        CPPUNIT_ASSERT(mArticle1->mPhase ==  article->mPhase);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mMWeight,
            article->mMWeight,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mDensity,
            article->mDensity,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mViscosity,
            article->mViscosity,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mSpecificHeat,
            article->mSpecificHeat,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mSpecificEnthalpy,
            article->mSpecificEnthalpy,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mAdiabaticIndex,
            article->mAdiabaticIndex,
            mTolerance);

        /// - @test for initialization status flag
        CPPUNIT_ASSERT(mArticle1->mInitFlag == article->mInitFlag);

        TS_DELETE_OBJECT(article);
    } {
        /// - Copy single fluid test article
        FriendlyPolyFluid article(*mArticle2, "article");

        /// @test for composite state attributes
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mTemperature, article.mTemperature, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mPressure,    article.mPressure,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mFlowRate,    article.mFlowRate,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mMass,        article.mMass,        mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mMole,        article.mMole,        mTolerance);

        /// @test for constituent state attributes
        CPPUNIT_ASSERT_EQUAL(mArticle2->mNConstituents, article.mNConstituents);
        for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
            CPPUNIT_ASSERT(mArticle2->mConstituents[i].mType == article.mConstituents[i].mType);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mConstituents[i].mMassFraction,
                                         article.mConstituents[i].mMassFraction,
                                         mTolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mConstituents[i].mMoleFraction,
                                         article.mConstituents[i].mMoleFraction,
                                         mTolerance);
        }

        /// @test for composite state properties
        CPPUNIT_ASSERT(mArticle2->mPhase ==  article.mPhase);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mMWeight,
            article.mMWeight,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mDensity,
            article.mDensity,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mViscosity,
            article.mViscosity,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mSpecificHeat,
            article.mSpecificHeat,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mSpecificEnthalpy,
            article.mSpecificEnthalpy,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mAdiabaticIndex,
            article.mAdiabaticIndex,
            mTolerance);

        /// - @test for initialization status flag
        CPPUNIT_ASSERT(mArticle2->mInitFlag == article.mInitFlag);
    } {
        /// - Copy single non-zero fraction test article
        FriendlyPolyFluid article(*mArticle3, "article");

        /// @test for composite state attributes
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mTemperature, article.mTemperature, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mPressure,    article.mPressure,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mFlowRate,    article.mFlowRate,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mMass,        article.mMass,        mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mMole,        article.mMole,        mTolerance);

        /// @test for constituent state attributes
        CPPUNIT_ASSERT_EQUAL(mArticle3->mNConstituents, article.mNConstituents);
        for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
            CPPUNIT_ASSERT(mArticle3->mConstituents[i].mType == article.mConstituents[i].mType);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mConstituents[i].mMassFraction,
                                         article.mConstituents[i].mMassFraction,
                                         mTolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mConstituents[i].mMoleFraction,
                                         article.mConstituents[i].mMoleFraction,
                                         mTolerance);
        }

        /// @test for composite state properties
        CPPUNIT_ASSERT(mArticle3->mPhase ==  article.mPhase);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mMWeight,
            article.mMWeight,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mDensity,
            article.mDensity,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mViscosity,
            article.mViscosity,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mSpecificHeat,
            article.mSpecificHeat,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mSpecificEnthalpy,
            article.mSpecificEnthalpy,
            mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mAdiabaticIndex,
            article.mAdiabaticIndex,
            mTolerance);

        /// - @test for initialization status flag
        CPPUNIT_ASSERT(mArticle3->mInitFlag == article.mInitFlag);
    } {
        /// - Define nominal trace compounds config data.
        DefinedChemicalCompounds definedCompounds;
        ChemicalCompound::Type types[2] = {ChemicalCompound::CO, ChemicalCompound::H2O};
        GunnsFluidTraceCompoundsConfigData tcConfig(types, 2, "tcConfig");
        PolyFluidConfigData configData(mProperties, mType1, UtPolyFluid::NMULTI, &tcConfig);

        /// - Define nominal trace compounds input data.
        double moleFractions[2] = {1.0e-1, 2.0e-2};
        GunnsFluidTraceCompoundsInputData tcInput(moleFractions);
        PolyFluidInputData inputData(mTemperature,  mPressure, mFlowRate, mMass,
                                     mMassFraction1, &tcInput);

        /// - Default construct and initialize (with nominal data) a test article, with trace
        ///   compounds.
        FriendlyPolyFluid article;
        article.initializeName("article");
        article.initialize(configData, inputData);

        // @test trace compounds in PolyFluid copy construction
        FriendlyPolyFluid copy(article, "copy");
        CPPUNIT_ASSERT(copy.mTraceCompounds);
        CPPUNIT_ASSERT(copy.mTraceCompounds->isInitialized());
        CPPUNIT_ASSERT(&tcConfig == copy.getTraceCompounds()->getConfig());
        CPPUNIT_ASSERT(copy.getTraceCompounds() != article.getTraceCompounds());
        CPPUNIT_ASSERT_EQUAL(copy.mTraceCompounds, copy.getTraceCompounds());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(moleFractions[0], copy.getTraceCompounds()->getMoleFraction(ChemicalCompound::CO),  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(moleFractions[1], copy.getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O), DBL_EPSILON);

        // @test PolyFluid copy construction omitting the trace compounds
        FriendlyPolyFluid copy2(article, "copy2", false);
        CPPUNIT_ASSERT(!copy2.mTraceCompounds);
    }

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testNominalInitialization()
{
    UT_RESULT;
    {
        /// - Default construct and initialize (with nominal data) a test article
        FriendlyPolyFluid article;
        article.initializeName("article");
        article.initialize(*mConfigData1, *mInputData1);

        /// @test for composite state attributes
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, article.mTemperature, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure,    article.mPressure,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,    article.mFlowRate,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMass,        article.mMass,        mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMole1,       article.mMole,        mTolerance);
        CPPUNIT_ASSERT(article.mInitFlag);

        /// @test for constituent state attributes
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            CPPUNIT_ASSERT(mType1[i] == article.mConstituents[i].mType);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mMassFraction1[i],
                                         article.mConstituents[i].mMassFraction,
                                         mTolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoleFraction1[i],
                article.mConstituents[i].mMoleFraction,
                                         mTolerance);
        }

        /// @test for composite state properties
        CPPUNIT_ASSERT(FluidProperties::GAS == article.mPhase);

        const FluidProperties* properties[UtPolyFluid::NMULTI];
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            properties[i] = mProperties->getProperties(mType1[i]);
        }

        double expected = mMass / mMole1;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mMWeight,          mTolerance);

        // composite density is sum of constituent densities at partial pressure
        expected = 0.0;
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            expected += properties[i]->getDensity(mTemperature, mMoleFraction1[i] * mPressure);
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mDensity,          mTolerance);

        // composite viscosity is mole fraction weighted sum of constituent viscosities at partial
        // pressure
        expected = 0.0;
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            expected += properties[i]->getViscosity(mTemperature,
                mMoleFraction1[i] * mPressure) * mMoleFraction1[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mViscosity,        mTolerance);

        // composite specific heat is mass fraction weighted sum of constituent specific heats at
        // partial pressure
        expected = 0.0;
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            expected += properties[i]->getSpecificHeat(mTemperature,
                mMoleFraction1[i] * mPressure) * mMassFraction1[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSpecificHeat,     mTolerance);

        // composite specific enthalpy is temperature times composite specific heat
        expected *= mTemperature;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSpecificEnthalpy, mTolerance);

        // composite adiabatic index is mole fraction weighted sum of constituent adiabatic indices
        // at partial pressure
        expected = 0.0;
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            expected += properties[i]->getAdiabaticIndex(mTemperature,
                mMoleFraction1[i] * mPressure) * mMoleFraction1[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mAdiabaticIndex,   mTolerance);
    } {
        /// - Default construct and initialize (with nominal data) a test article
        FriendlyPolyFluid article;
        article.initializeName("article");
        article.initialize(*mConfigData2, *mInputData2);

        /// @test for composite state attributes
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, article.mTemperature, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure,    article.mPressure,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,    article.mFlowRate,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMass,        article.mMass,        mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMole2,       article.mMole,        mTolerance);
        CPPUNIT_ASSERT(article.mInitFlag);

        /// @test for constituent state attributes
        for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
            CPPUNIT_ASSERT(mType2[i] == article.mConstituents[i].mType);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mMassFraction2[i],
                                         article.mConstituents[i].mMassFraction,
                                         mTolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoleFraction2[i],
                article.mConstituents[i].mMoleFraction,
                                         mTolerance);
        }

        /// @test for composite state properties
        CPPUNIT_ASSERT(FluidProperties::LIQUID == article.mPhase);

        const FluidProperties* properties[UtPolyFluid::NSINGLE];
        for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
            properties[i] = mProperties->getProperties(mType2[i]);
        }

        double expected = mMass / mMole2;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mMWeight,          mTolerance);

        // composite density is sum of constituent densities at partial pressure
        expected = 0.0;
        for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
            expected += properties[i]->getDensity(mTemperature, mMoleFraction2[i] * mPressure);
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mDensity,          mTolerance);

        // composite viscosity is mole fraction weighted sum of constituent viscosities at partial
        // pressure
        expected = 0.0;
        for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
            expected += properties[i]->getViscosity(mTemperature,
                mMoleFraction2[i] * mPressure) * mMoleFraction2[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mViscosity,        mTolerance);

        // composite specific heat is mass fraction weighted sum of constituent specific heats at
        // partial pressure
        expected = 0.0;
        for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
            expected += properties[i]->getSpecificHeat(mTemperature,
                mMoleFraction2[i] * mPressure) * mMassFraction2[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSpecificHeat,     mTolerance);

        // composite specific enthalpy is temperature times composite specific heat
        expected *= mTemperature;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSpecificEnthalpy, mTolerance);

        // composite adiabatic index is mole fraction weighted sum of constituent adiabatic indices
        // at partial pressure
        expected = 0.0;
        for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
            expected += properties[i]->getAdiabaticIndex(mTemperature,
                mMoleFraction2[i] * mPressure) * mMoleFraction2[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mAdiabaticIndex,   mTolerance);
    } {
        /// - Default construct and initialize (with nominal data) a test article
        FriendlyPolyFluid article;
        article.initializeName("article");
        article.initialize(*mConfigData3, *mInputData3);

        /// @test for composite state attributes
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, article.mTemperature, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure,    article.mPressure,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,    article.mFlowRate,    mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMass,        article.mMass,        mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMole3,       article.mMole,        mTolerance);
        CPPUNIT_ASSERT(article.mInitFlag);

        /// @test for constituent state attributes
        for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
            CPPUNIT_ASSERT(mType3[i] == article.mConstituents[i].mType);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mMassFraction3[i],
                                         article.mConstituents[i].mMassFraction,
                                         mTolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoleFraction3[i],
                article.mConstituents[i].mMoleFraction,
                                         mTolerance);
        }

        /// @test for composite state properties
        CPPUNIT_ASSERT(FluidProperties::GAS == article.mPhase);

        const FluidProperties* properties[UtPolyFluid::NDUAL];
        for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
            properties[i] = mProperties->getProperties(mType3[i]);
        }

        double expected = mMass / mMole3;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mMWeight,          mTolerance);

        // composite density is sum of constituent densities at partial pressure
        expected = 0.0;
        for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
            expected += properties[i]->getDensity(mTemperature,
                mMoleFraction3[i] * mPressure) * mMoleFraction3[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mDensity,          mTolerance);

        // composite viscosity is mole fraction weighted sum of constituent viscosities at partial
        // pressure
        expected = 0.0;
        for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
            expected += properties[i]->getViscosity(mTemperature,
                mMoleFraction3[i] * mPressure) * mMoleFraction3[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mViscosity,        mTolerance);

        // composite specific heat is mass fraction weighted sum of constituent specific heats at
        // partial pressure
        expected = 0.0;
        for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
            expected += properties[i]->getSpecificHeat(mTemperature,
                mMoleFraction3[i] * mPressure) * mMassFraction3[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSpecificHeat,     mTolerance);

        // composite specific enthalpy is temperature times composite specific heat
        expected *= mTemperature;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSpecificEnthalpy, mTolerance);

        // composite adiabatic index is mole fraction weighted sum of constituent adiabatic indices
        // at partial pressure
        expected = 0.0;
        for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
            expected += properties[i]->getAdiabaticIndex(mTemperature,
                mMoleFraction3[i] * mPressure) * mMoleFraction3[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mAdiabaticIndex,   mTolerance);
    } {
        /// - Define nominal trace compounds config data.
        DefinedChemicalCompounds definedCompounds;
        ChemicalCompound::Type types[2] = {ChemicalCompound::CO, ChemicalCompound::H2O};
        GunnsFluidTraceCompoundsConfigData tcConfig(types, 2, "tcConfig");
        PolyFluidConfigData configData(mProperties, mType1, UtPolyFluid::NMULTI, &tcConfig);

        /// - Define nominal trace compounds input data.
        double moleFractions[2] = {1.0e-1, 2.0e-2};
        GunnsFluidTraceCompoundsInputData tcInput(moleFractions);
        PolyFluidInputData inputData(mTemperature,  mPressure, mFlowRate, mMass,
                                     mMassFraction1, &tcInput);

        /// - Default construct and initialize (with nominal data) a test article, with trace
        ///   compounds.
        FriendlyPolyFluid article;
        article.initializeName("article");
        article.initialize(configData, inputData);

        // @test trace compounds initialization
        CPPUNIT_ASSERT(article.mTraceCompounds);
        CPPUNIT_ASSERT(article.mTraceCompounds->isInitialized());
        CPPUNIT_ASSERT(&tcConfig == article.getTraceCompounds()->getConfig());
        CPPUNIT_ASSERT_EQUAL(article.mTraceCompounds, article.getTraceCompounds());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(moleFractions[0], article.getTraceCompounds()->getMoleFraction(ChemicalCompound::CO),  DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(moleFractions[1], article.getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O), DBL_EPSILON);
    }

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for composite state accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testCompositeStateAccessors()
{
    UT_RESULT;
    {
        /// @test for is initialized
        CPPUNIT_ASSERT(mArticle1->mInitFlag == mArticle1->isInitialized());

        /// @test for get composite temperature
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mTemperature,
            mArticle1->getTemperature(),
            mTolerance);

        /// @test for get composite pressure
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mPressure,
            mArticle1->getPressure(),
            mTolerance);

        /// @test for get composite flow rate
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mFlowRate,
            mArticle1->getFlowRate(),
            mTolerance);

        /// @test for get composite mole
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mMole,
            mArticle1->getMole(),
            mTolerance);

        /// @test for get composite mass
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle1->mMass, mArticle1->getMass(),
            mTolerance);

        /// - For each constituent type:
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            /// @test for get constituent mole fraction
            double expected = mArticle1->mConstituents[i].mMoleFraction;
            double returned = mArticle1->getMoleFraction(mType1[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

            /// @test for get constituent mole fraction by index
            returned = mArticle1->getMoleFraction(i);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

            /// @test for get constituent mass fraction
            expected = mArticle1->mConstituents[i].mMassFraction;
            returned = mArticle1->getMassFraction(mType1[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

            /// @test for get constituent mass fraction by index
            returned = mArticle1->getMassFraction(i);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

            /// @test for get constituent partial pressure
            expected = mArticle1->getPressure() * mArticle1->mConstituents[i].mMoleFraction;
            returned = mArticle1->getPartialPressure(mType1[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

            /// @test for get pointer to constituent
            const MonoFluid* exp = &(mArticle1->mConstituents[i].mFluid);
            const MonoFluid* ret = mArticle1->getConstituent(mType1[i]);
            CPPUNIT_ASSERT(exp == ret);
        }
    } {
        /// @test for is initialized
        CPPUNIT_ASSERT(mArticle2->mInitFlag == mArticle2->isInitialized());

        /// @test for get composite temperature
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mTemperature,
            mArticle2->getTemperature(),
            mTolerance);

        /// @test for get composite pressure
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mPressure,
            mArticle2->getPressure(),
            mTolerance);

        /// @test for get composite flow rate
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mFlowRate,
            mArticle2->getFlowRate(),
            mTolerance);

        /// @test for get composite mole
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mMole,
            mArticle2->getMole(),
            mTolerance);

        /// @test for get composite mass
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle2->mMass,
            mArticle2->getMass(),
            mTolerance);

        /// - For each constituent type:
        for (int i = 0; i < UtPolyFluid::NSINGLE; i++) {
            /// @test for get constituent mole fraction
            double expected = mArticle2->mConstituents[i].mMoleFraction;
            double returned = mArticle2->getMoleFraction(mType2[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

            /// @test for get constituent mass fraction
            expected = mArticle2->mConstituents[i].mMassFraction;
            returned = mArticle2->getMassFraction(mType2[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

            /// @test for get constituent partial pressure
            expected = mArticle2->getPressure() * mArticle2->mConstituents[i].mMoleFraction;
            returned = mArticle2->getPartialPressure(mType2[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

            /// @test for get pointer to constituent
            const MonoFluid* exp = &(mArticle2->mConstituents[i].mFluid);
            const MonoFluid* ret = mArticle2->getConstituent(mType2[i]);
            CPPUNIT_ASSERT(exp == ret);
        }
    } {
        /// @test for is initialized
        CPPUNIT_ASSERT(mArticle3->mInitFlag == mArticle3->isInitialized());

        /// @test for get composite temperature
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mTemperature,
            mArticle3->getTemperature(),
            mTolerance);

        /// @test for get composite pressure
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mPressure,
            mArticle3->getPressure(),
            mTolerance);

        /// @test for get composite flow rate
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mFlowRate,
            mArticle3->getFlowRate(),
            mTolerance);

        /// @test for get composite mole
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mMole,
            mArticle3->getMole(),
            mTolerance);

        /// @test for get composite mass
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle3->mMass,
            mArticle3->getMass(),
            mTolerance);

        /// - For each constituent type:
        for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
            /// @test for get constituent mole fraction
            double expected = mArticle3->mConstituents[i].mMoleFraction;
            double returned = mArticle3->getMoleFraction(mType3[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

            /// @test for get constituent mass fraction
            expected = mArticle3->mConstituents[i].mMassFraction;
            returned = mArticle3->getMassFraction(mType3[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

            /// @test for get constituent partial pressure
            expected = mArticle3->getPressure() * mArticle3->getMoleFraction(mType3[i]);
            returned = mArticle3->getPartialPressure(mType3[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

            /// @test for get pointer to constituent
            const MonoFluid* exp = &(mArticle3->mConstituents[i].mFluid);
            const MonoFluid* ret = mArticle3->getConstituent(mType3[i]);
            CPPUNIT_ASSERT(exp == ret);
        }

        /// @test for getType given invalid index
        CPPUNIT_ASSERT_EQUAL(FluidProperties::NO_FLUID, mArticle3->getType(-1));
        CPPUNIT_ASSERT_EQUAL(FluidProperties::NO_FLUID, mArticle3->getType(10));

        /// @test for findCompound without trace compounds
        DefinedChemicalCompounds definedCompounds;
        int fluidIndex = 0;
        int tcIndex    = 0;
        mArticle3->findCompound(fluidIndex, tcIndex, definedCompounds.getCompound(ChemicalCompound::O2));
        CPPUNIT_ASSERT_EQUAL( 1, fluidIndex);
        CPPUNIT_ASSERT_EQUAL(-1, tcIndex);
    } {
        /// - Define nominal trace compounds config data.
        DefinedChemicalCompounds definedCompounds;
        ChemicalCompound::Type types[2] = {ChemicalCompound::CO, ChemicalCompound::H2O};
        GunnsFluidTraceCompoundsConfigData tcConfig(types, 2, "tcConfig");
        PolyFluidConfigData configData(mProperties, mType1, UtPolyFluid::NMULTI, &tcConfig);

        /// - Define nominal trace compounds input data.
        double moleFractions[2] = {1.0e-1, 2.0e-2};
        GunnsFluidTraceCompoundsInputData tcInput(moleFractions);
        PolyFluidInputData inputData(mTemperature,  mPressure, mFlowRate, mMass,
                                     mMassFraction1, &tcInput);

        /// - Default construct and initialize (with nominal data) a test article, with trace
        ///   compounds.
        FriendlyPolyFluid article;
        article.initializeName("article");
        article.initialize(configData, inputData);

        /// @test for findCompound with trace compounds
        int fluidIndex = 0;
        int tcIndex    = 0;
        article.findCompound(fluidIndex, tcIndex, definedCompounds.getCompound(ChemicalCompound::H2O));
        CPPUNIT_ASSERT_EQUAL(4, fluidIndex);
        CPPUNIT_ASSERT_EQUAL(1, tcIndex);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for composite property accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testCompositePropertyAccessors()
{
    UT_RESULT;
    {
        /// @test for get composite phase
        CPPUNIT_ASSERT(mArticle1->mPhase == mArticle1->getPhase());

        /// @test for get composite molecular weight
        double expected = mArticle1->mMWeight;
        double returned = mArticle1->getMWeight();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite density
        expected = mArticle1->mDensity;
        returned = mArticle1->getDensity();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite visocsity
        expected = mArticle1->mViscosity;
        returned = mArticle1->getViscosity();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite specific heat
        expected = mArticle1->mSpecificHeat;
        returned = mArticle1->getSpecificHeat();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite specific enthalpy
        expected = mArticle1->mSpecificEnthalpy;
        returned = mArticle1->getSpecificEnthalpy();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite adiabatic index
        expected = mArticle1->mAdiabaticIndex;
        returned = mArticle1->getAdiabaticIndex();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test for get composite phase
        CPPUNIT_ASSERT(mArticle2->mPhase == mArticle2->getPhase());

        /// @test for get composite molecular weight
        double expected = mArticle2->mMWeight;
        double returned = mArticle2->getMWeight();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite density
        expected = mArticle2->mDensity;
        returned = mArticle2->getDensity();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite visocsity
        expected = mArticle2->mViscosity;
        returned = mArticle2->getViscosity();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite specific heat
        expected = mArticle2->mSpecificHeat;
        returned = mArticle2->getSpecificHeat();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite specific enthalpy
        expected = mArticle2->mSpecificEnthalpy;
        returned = mArticle2->getSpecificEnthalpy();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite adiabatic index
        expected = mArticle2->mAdiabaticIndex;
        returned = mArticle2->getAdiabaticIndex();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test for get composite phase
        CPPUNIT_ASSERT(mArticle3->mPhase == mArticle3->getPhase());

        /// @test for get composite molecular weight
        double expected = mArticle3->mMWeight;
        double returned = mArticle3->getMWeight();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite density
        expected = mArticle3->mDensity;
        returned = mArticle3->getDensity();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite visosity
        expected = mArticle3->mViscosity;
        returned = mArticle3->getViscosity();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite specific heat
        expected = mArticle3->mSpecificHeat;
        returned = mArticle3->getSpecificHeat();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite specific enthalpy
        expected = mArticle3->mSpecificEnthalpy;
        returned = mArticle3->getSpecificEnthalpy();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);

        /// @test for get composite adiabatic index
        expected = mArticle3->mAdiabaticIndex;
        returned = mArticle3->getAdiabaticIndex();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for setFlowrate.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testSetFlowrate()
{
    UT_RESULT;

    /// @test for pressure and density
    const double expectedFlowRate = mFlowRate * 2.0;
    {
        mArticle1->setFlowRate(expectedFlowRate);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowRate, mArticle1->getFlowRate(), mTolerance);
    } {
        mArticle2->setFlowRate(expectedFlowRate);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowRate, mArticle2->getFlowRate(), mTolerance);
    } {
        mArticle3->setFlowRate(expectedFlowRate);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowRate, mArticle3->getFlowRate(), mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for setPressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testSetPressure()
{
    UT_RESULT;

    /// @test for pressure and density
    const double expectedPressure = mPressure + 20.0;
    {
        mArticle1->setPressure(expectedPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, mArticle1->getPressure(), mTolerance);
        const FluidProperties* properties[NMULTI];
        double expectedDensity = 0.0;
        for (int i = 0; i < NMULTI; i++) {
            properties[i] = mProperties->getProperties(mType1[i]);
            expectedDensity += properties[i]->getDensity(mTemperature,
                                                         mMoleFraction1[i] * expectedPressure);
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDensity, mArticle1->getDensity(), mTolerance);

    } {
        mArticle2->setPressure(expectedPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, mArticle2->getPressure(), mTolerance);
        const FluidProperties* properties =
            mProperties->getProperties(FluidProperties::GUNNS_WATER);
        const double expectedDensity = properties->getDensity(mTemperature, expectedPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDensity, mArticle2->getDensity(), mTolerance);
    } {
        mArticle3->setPressure(expectedPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, mArticle3->getPressure(), mTolerance);
        const FluidProperties* properties = mProperties->getProperties(FluidProperties::GUNNS_O2);
        const double expectedDensity = properties->getDensity(mTemperature, expectedPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDensity, mArticle3->getDensity(), mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for setMass.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testSetMass()
{
    UT_RESULT;

    /// @test for composite and constituent mass and moles
    const double expectedMass = mMass * 2.0;
    {
        mArticle1->setMass(expectedMass);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, mArticle1->getMass(), mTolerance);
        const double expectedMole = mMole1 * 2.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, mArticle1->getMole(), mTolerance);
    } {
        mArticle2->setMass(expectedMass);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, mArticle2->getMass(), mTolerance);
        const double expectedMole = mMole2 * 2.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, mArticle2->getMole(), mTolerance);
    } {
        mArticle3->setMass(expectedMass);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, mArticle3->getMass(), mTolerance);
        const double expectedMole = mMole3 * 2.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, mArticle3->getMole(), mTolerance);
    } {
        /// @test for mMweight of 0.0
        mArticle1->mMWeight = 0.0;
        mArticle1->setMass(expectedMass);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, mArticle1->getMass(), mTolerance);
        const double expectedMole = expectedMass / DBL_EPSILON;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, mArticle1->getMole(), mTolerance);
    } {
        /// - Define nominal trace compounds config data.
        DefinedChemicalCompounds definedCompounds;
        ChemicalCompound::Type types[2] = {ChemicalCompound::CO, ChemicalCompound::H2O};
        GunnsFluidTraceCompoundsConfigData tcConfig(types, 2, "tcConfig");
        PolyFluidConfigData configData(mProperties, mType1, UtPolyFluid::NMULTI, &tcConfig);

        /// - Define nominal trace compounds input data.
        double moleFractions[2] = {1.0e-1, 2.0e-2};
        GunnsFluidTraceCompoundsInputData tcInput(moleFractions);
        PolyFluidInputData inputData(mTemperature,  mPressure, mFlowRate, mMass,
                                     mMassFraction1, &tcInput);

        /// - Default construct and initialize (with nominal data) a test article, with trace
        ///   compounds.
        FriendlyPolyFluid article;
        article.initializeName("article");
        article.initialize(configData, inputData);

        /// @test for setMass with trace compounds
        double mCO  = article.getTraceCompounds()->getMass(ChemicalCompound::CO);
        double mH2O = article.getTraceCompounds()->getMass(ChemicalCompound::H2O);
        article.setMass(expectedMass);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mCO  * 2.0, article.getTraceCompounds()->getMass(ChemicalCompound::CO), mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mH2O * 2.0, article.getTraceCompounds()->getMass(ChemicalCompound::H2O), mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for setMole.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testSetMole()
{
    UT_RESULT;

    /// @test for composite and constituent mass and moles
    const double expectedMass = mMass * 0.5;
    {
        const double expectedMole = mMole1 * 0.5;
        mArticle1->setMole(expectedMole);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, mArticle1->getMole(), mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, mArticle1->getMass(), mTolerance);
    } {
        const double expectedMole = mMole2 * 0.5;
        mArticle2->setMole(expectedMole);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, mArticle2->getMole(), mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, mArticle2->getMass(), mTolerance);
    } {
        const double expectedMole = mMole3 * 0.5;
        mArticle3->setMole(expectedMole);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, mArticle3->getMole(), mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, mArticle3->getMass(), mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for setMassAndMassFractions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testSetMassAndMassFractions()
{
    UT_RESULT;

    /// @test for composite and constituent mass and moles, mass fractions, mole fractions and
    ///   composite molecular weight
    const double expectedMass = mMass * 0.5;
    {
        double expectedMassFraction[UtPolyFluid::NMULTI];
        expectedMassFraction[6] = 0.750;
        expectedMassFraction[5] = 0.200;
        expectedMassFraction[4] = 0.020;
        expectedMassFraction[3] = 0.010;
        expectedMassFraction[2] = 0.010;
        expectedMassFraction[1] = 0.0099999999;
        expectedMassFraction[0] = 0.0000000001;
        double expectedMoleFraction[UtPolyFluid::NMULTI];
        double expectedMole = 0.0;
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            FluidProperties::FluidType type = mType1[i];
            const double mWeight      = mProperties->getProperties(type)->getMWeight();
            expectedMoleFraction[i]   = expectedMassFraction[i] / mWeight;
            expectedMole             += expectedMoleFraction[i];
        }
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            expectedMoleFraction[i]  /= expectedMole;
        };
        expectedMole                 *= expectedMass;
        const double expectedMWeight = expectedMass / expectedMole;
        double expectedFlowRate[UtPolyFluid::NMULTI];
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            expectedFlowRate[i] = expectedMassFraction[i]; // assuming total flow rate = 1
        };

        mArticle1->setFlowRate(1.0);
        mArticle1->setMassAndMassFractions(expectedMass, expectedMassFraction);

        const double returnedMass = mArticle1->getMass();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, returnedMass, mTolerance);
        const double returnedMole = mArticle1->getMole();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, returnedMole, mTolerance);
        const double returnedMWeight = mArticle1->getMWeight();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMWeight, returnedMWeight, mTolerance);
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            const double returnedMassFraction = mArticle1->getMassFraction(mType1[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassFraction[i], returnedMassFraction, mTolerance);
            const double returnedMoleFraction = mArticle1->getMoleFraction(mType1[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMoleFraction[i], returnedMoleFraction, mTolerance);
            const double returnedFlowRate = mArticle1->getConstituent(mType1[i])->getFlowRate();
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowRate[i], returnedFlowRate, DBL_EPSILON);
        }
    } {
        mArticle2->setMassAndMassFractions(expectedMass, mMassFraction2);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, mArticle2->getMass(), mTolerance);
        const double expectedMole = mMole2 * 0.5;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, mArticle2->getMole(), mTolerance);
        for (int i = 0; i < NSINGLE; i++) {
            const double returnedMassFraction = mArticle2->getMassFraction(mType2[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mMassFraction2[i], returnedMassFraction, mTolerance);
            const double returnedMoleFraction = mArticle2->getMoleFraction(mType2[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoleFraction2[i], returnedMoleFraction, mTolerance);
        }
    } {
        const double expectedMass = mMass * 0.5;
        double expectedMassFraction[UtPolyFluid::NDUAL];
        expectedMassFraction[0] = 1.000;
        expectedMassFraction[1] = 0.000;
        double expectedMoleFraction[UtPolyFluid::NDUAL];
        expectedMoleFraction[0] = 1.000;
        expectedMoleFraction[1] = 0.000;
        const double expectedMWeight =
            mProperties->getProperties(FluidProperties::GUNNS_HFE7000)->getMWeight();
        const double expectedMole    = expectedMass / expectedMWeight;

        mArticle3->setMassAndMassFractions(expectedMass, expectedMassFraction);
        const double returnedMass = mArticle3->getMass();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, returnedMass, mTolerance);

        const double returnedMole = mArticle3->getMole();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, returnedMole, mTolerance);

        const double returnedMWeight = mArticle3->getMWeight();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMWeight, returnedMWeight, mTolerance);

        for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
            const double returnedMassFraction = mArticle3->getMassFraction(mType3[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassFraction[i], returnedMassFraction, mTolerance);

            const double returnedMoleFraction = mArticle3->getMoleFraction(mType3[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMoleFraction[i], returnedMoleFraction, mTolerance);
        }
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for SetMoleAndMoleFractions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testSetMoleAndMoleFractions()
{
    UT_RESULT;

    /// @test for composite and constituent mass and moles, mass fractions, mole fractions and
    ///   composite molecular weight
    {
        const double expectedMole = mMole1 * 2.0;
        double expectedMoleFraction[UtPolyFluid::NMULTI];
        expectedMoleFraction[6] = 0.750;
        expectedMoleFraction[5] = 0.200;
        expectedMoleFraction[4] = 0.020;
        expectedMoleFraction[3] = 0.010;
        expectedMoleFraction[2] = 0.010;
        expectedMoleFraction[1] = 0.0099999999;
        expectedMoleFraction[0] = 0.0000000001;
        double expectedMassFraction[UtPolyFluid::NMULTI];
        double expectedMWeight  = 0.0;
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            FluidProperties::FluidType type = mType1[i];
            const double mWeight            = mProperties->getProperties(type)->getMWeight();
            expectedMassFraction[i]         = expectedMoleFraction[i] * mWeight;
            expectedMWeight                += expectedMassFraction[i];
        }
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            expectedMassFraction[i]  /= expectedMWeight;
        };
        double expectedFlowRate[UtPolyFluid::NMULTI];
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            expectedFlowRate[i] = expectedMassFraction[i]; // assuming total flow rate = 1
        };

        mArticle1->setFlowRate(1.0);
        mArticle1->setMoleAndMoleFractions(expectedMole, expectedMoleFraction);
        const double returnedMole     = mArticle1->getMole();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, returnedMole, mTolerance);
        const double returnedMWeight  = mArticle1->getMWeight();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMWeight, returnedMWeight, mTolerance);
        const double expectedMass     = expectedMole * expectedMWeight;
        const double returnedMass     = mArticle1->getMass();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, returnedMass, mTolerance);
        for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
            const double returnedMoleFraction = mArticle1->getMoleFraction(mType1[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMoleFraction[i], returnedMoleFraction, mTolerance);
            const double returnedMassFraction = mArticle1->getMassFraction(mType1[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassFraction[i], returnedMassFraction, mTolerance);
            const double returnedFlowRate = mArticle1->getConstituent(mType1[i])->getFlowRate();
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlowRate[i], returnedFlowRate, DBL_EPSILON);
        }
    } {
        const double expectedMole = mMole2 * 2.0;
        mArticle2->setMoleAndMoleFractions(expectedMole, mMoleFraction2);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, mArticle2->getMole(), mTolerance);
        const double expectedMass = mMass * 2.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, mArticle2->getMass(), mTolerance);
        for (int i = 0; i < NSINGLE; i++) {
            const double returnedMassFraction = mArticle2->getMassFraction(mType2[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mMassFraction2[i], returnedMassFraction, mTolerance);
            const double returnedMoleFraction = mArticle2->getMoleFraction(mType2[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoleFraction2[i], returnedMoleFraction, mTolerance);
        }
    } {
        const double expectedMole = mMole3 * 0.5;
        double expectedMoleFraction[UtPolyFluid::NDUAL];
        expectedMoleFraction[0] = 1.000;
        expectedMoleFraction[1] = 0.000;
        double expectedMassFraction[UtPolyFluid::NDUAL];
        expectedMassFraction[0] = 1.000;
        expectedMassFraction[1] = 0.000;
        const double expectedMWeight =
            mProperties->getProperties(FluidProperties::GUNNS_HFE7000)->getMWeight();
        const double expectedMass    = expectedMole * expectedMWeight;

        mArticle3->setMoleAndMoleFractions(expectedMole, expectedMoleFraction);

        const double returnedMole = mArticle3->getMole();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMole, returnedMole, mTolerance);
        const double returnedMWeight = mArticle3->getMWeight();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMWeight, returnedMWeight, mTolerance);
        const double returnedMass = mArticle3->getMass();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, returnedMass, mTolerance);
        for (int i = 0; i < UtPolyFluid::NDUAL; i++) {
            const double returnedMoleFraction = mArticle3->getMoleFraction(mType3[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMoleFraction[i], returnedMoleFraction, mTolerance);
            const double returnedMassFraction = mArticle3->getMassFraction(mType3[i]);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassFraction[i], returnedMassFraction, mTolerance);
        }
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for setTemperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testSetTemperature()
{
    UT_RESULT;

    const double expectedTemperature = mTemperature + 10.0;
    {
        mArticle1->setTemperature(expectedTemperature);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTemperature, mArticle1->mTemperature, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure,           mArticle1->mPressure,    mTolerance);

        const FluidProperties* properties[NMULTI];
        for (int i = 0; i < NMULTI; i++) {
            properties[i] = mProperties->getProperties(mType1[i]);
        }

        // composite density is sum of constituent densities at partial pressure
        double expected = 0.0;
        for (int i = 0; i < NMULTI; i++) {
            expected += properties[i]->getDensity(expectedTemperature,
                mMoleFraction1[i] * mPressure);
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle1->mDensity,          mTolerance);

        // composite viscosity is mole fraction weighted sum of constituent viscosities at
        // partial pressure
        expected = 0.0;
        for (int i = 0; i < NMULTI; i++) {
            expected += properties[i]->getViscosity(expectedTemperature,
                mMoleFraction1[i] * mPressure) * mMoleFraction1[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle1->mViscosity,        mTolerance);

        // composite specific heat is mass fraction weighted sum of constituent specific heats at
        // partial pressure
        expected = 0.0;
        for (int i = 0; i < NMULTI; i++) {
            expected += properties[i]->getSpecificHeat(expectedTemperature,
                mMoleFraction1[i] * mPressure) * mMassFraction1[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle1->mSpecificHeat,     mTolerance);

        // composite specific enthalpy is temperature times composite specific heat
        expected *= expectedTemperature;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle1->mSpecificEnthalpy, mTolerance);

        // composite adiabatic index is mole fraction weighted sum of constituent adiabatic
        // indices at partial pressure
        expected = 0.0;
        for (int i = 0; i < NMULTI; i++) {
            expected += properties[i]->getAdiabaticIndex(expectedTemperature,
                mMoleFraction1[i] * mPressure) * mMoleFraction1[i];
        }
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle1->mAdiabaticIndex,   mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for setting the mass of a constituent.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testSetConstituentMass()
{
    UT_RESULT;

    const FluidProperties::FluidType type = FluidProperties::GUNNS_O2;
    const double expectedCompositeMass   = mArticle1->getMass();
    const double expectedCompositeMole   = mArticle1->getMole();
    const double expectedMassFraction    = mArticle1->getMassFraction(type);
    const double expectedMoleFraction    = mArticle1->getMoleFraction(type);
    const double expectedConstituentMass = 3.7;

    mArticle1->setMass(mArticle1->find(type), expectedConstituentMass);

    const double returnedCompositeMass   = mArticle1->getMass();
    const double returnedCompositeMole   = mArticle1->getMole();
    const double returnedMassFraction    = mArticle1->getMassFraction(type);
    const double returnedMoleFraction    = mArticle1->getMoleFraction(type);

    /// @test only constituent mass is changed
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCompositeMass,   returnedCompositeMass,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCompositeMole,   returnedCompositeMole,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassFraction,    returnedMassFraction,     mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMoleFraction,    returnedMoleFraction,     mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for updating mass properties from constituent masses.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testUpdateMass()
{
    UT_RESULT;

    const double expectedCompositeMass = 1.0;
    const double expectedCompositeMole = 1.0 /
        mProperties->getProperties(FluidProperties::GUNNS_HFE7000)->getMWeight();
    const double expectedMassFraction1 = 1.0;
    const double expectedMassFraction2 = 0.0;
    const double expectedMoleFraction1 = 1.0;
    const double expectedMoleFraction2 = 0.0;

    mArticle3->setMass(mArticle3->find(FluidProperties::GUNNS_HFE7000), 1.0);
    mArticle3->setMass(mArticle3->find(FluidProperties::GUNNS_O2),      0.0);
    mArticle3->updateMass();

    const double returnedCompositeMass = mArticle3->getMass();
    const double returnedCompositeMole = mArticle3->getMole();
    const double returnedMassFraction1 = mArticle3->getMassFraction(FluidProperties::GUNNS_HFE7000);
    const double returnedMassFraction2 = mArticle3->getMassFraction(FluidProperties::GUNNS_O2);
    const double returnedMoleFraction1 = mArticle3->getMoleFraction(FluidProperties::GUNNS_HFE7000);
    const double returnedMoleFraction2 = mArticle3->getMoleFraction(FluidProperties::GUNNS_O2);

    /// @test all mass properties are updated
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCompositeMass, returnedCompositeMass, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCompositeMole, returnedCompositeMole, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassFraction1, returnedMassFraction1, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassFraction2, returnedMassFraction2, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMoleFraction1, returnedMoleFraction1, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMoleFraction2, returnedMoleFraction2, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for combining fluids.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testStateUpdaters()
{
    UT_RESULT;

    DefinedChemicalCompounds definedCompounds;
    ChemicalCompound::Type tcTypes[2] = {ChemicalCompound::CO, ChemicalCompound::H2O};
    GunnsFluidTraceCompoundsConfigData tcConfig(tcTypes, 2, "tcConfig");
    double moleFractions[2] = {1.0e-1, 2.0e-2};
    GunnsFluidTraceCompoundsInputData tcInput(moleFractions);

    FluidProperties::FluidType types[2];
    double massFractions[2];
    types[0]    = FluidProperties::GUNNS_CO2; massFractions[0] = 0.75;
    types[1]    = FluidProperties::GUNNS_O2;  massFractions[1] = 0.25;
    PolyFluidConfigData* configData = new PolyFluidConfigData(mProperties, types, 2, &tcConfig);
    PolyFluidInputData* initData  = new PolyFluidInputData(300.0,
        100.0,
        1.0,
        1.0,
        massFractions,
        &tcInput);
    FriendlyPolyFluid* article1 = new FriendlyPolyFluid(*configData, *initData);
    delete initData;

    massFractions[0] = 0.25;
    massFractions[1] = 0.75;
    initData  = new PolyFluidInputData(350.0,
        110000.0,
        2.0,
        1.0,
        massFractions,
        &tcInput);
    FriendlyPolyFluid* article2 = new FriendlyPolyFluid(*configData, *initData);
    delete initData;

    moleFractions[0] = 3.0e-3;
    GunnsFluidTraceCompoundsInputData tcInput3(moleFractions);
    initData  = new PolyFluidInputData(350.0,
        110000.0,
        2.0,
        1.0,
        massFractions,
        &tcInput3);
    FriendlyPolyFluid* article3 = new FriendlyPolyFluid(*configData, *initData);

    article3->resetState();

    /// @test for reset state composite state attributes
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mMass,        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mMole,        mTolerance);

    /// @test for reset state constituent attributes
    for (int i = 0; i < 2; i++) {
        // mass fraction
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mConstituents[i].mMassFraction, mTolerance);
        // mole fraction
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mConstituents[i].mMoleFraction, mTolerance);
    }
    /// @test for reset state composite properties
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mMWeight, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mDensity, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mViscosity, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mSpecificHeat, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mSpecificEnthalpy, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mAdiabaticIndex, mTolerance);

    /// @test for reset state trace compounds
    CPPUNIT_ASSERT_EQUAL(0.0, article3->getTraceCompounds()->getMass(ChemicalCompound::CO));
    CPPUNIT_ASSERT_EQUAL(0.0, article3->getTraceCompounds()->getMass(ChemicalCompound::H2O));
    CPPUNIT_ASSERT_EQUAL(0.0, article3->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO));
    CPPUNIT_ASSERT_EQUAL(0.0, article3->getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O));

    article1->getTraceCompounds()->setMoleFraction(0, 3.0e-3);
    article1->getTraceCompounds()->setMoleFraction(1, 4.0e-4);
    article3->setState(article1);

    /// @test for set state composite state attributes
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mTemperature,      article3->mTemperature,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mPressure,         article3->mPressure,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mFlowRate,         article3->mFlowRate,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mMWeight,          article3->mMWeight,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mDensity,          article3->mDensity,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mViscosity,        article3->mViscosity,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mSpecificHeat,     article3->mSpecificHeat,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mSpecificEnthalpy, article3->mSpecificEnthalpy,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mAdiabaticIndex,   article3->mAdiabaticIndex,
        mTolerance);

    /// @test for set state constituent attributes
    for (int i = 0; i < 2; i++) {
        // mass fraction
        CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mConstituents[i].mMassFraction,
                                     article3->mConstituents[i].mMassFraction,
                                     mTolerance);
        // mole fraction
        CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mConstituents[i].mMoleFraction,
                                     article3->mConstituents[i].mMoleFraction,
                                     mTolerance);
        // fluid attributes
    }

    /// @test for set state composite properties
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mMWeight,          article3->mMWeight,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mDensity,          article3->mDensity,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mViscosity,        article3->mViscosity,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mSpecificHeat,     article3->mSpecificHeat,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mSpecificEnthalpy, article3->mSpecificEnthalpy,
        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mAdiabaticIndex,   article3->mAdiabaticIndex,
        mTolerance);

    /// @test for set state trace compounds
    CPPUNIT_ASSERT_EQUAL(article1->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO),
                         article3->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO));
    CPPUNIT_ASSERT_EQUAL(article1->getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O),
                         article3->getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O));

    // TODO: Computed expected values for add state
    double mCO = article3->getTraceCompounds()->getMass(ChemicalCompound::CO)
               + article2->mFlowRate / article2->getMWeight()
               * article2->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO)
               * definedCompounds.getCompound(ChemicalCompound::CO)->mMWeight;
    double mH2O = article3->getTraceCompounds()->getMass(ChemicalCompound::H2O)
                + article2->mFlowRate / article2->getMWeight()
                * article2->getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O)
                * definedCompounds.getCompound(ChemicalCompound::H2O)->mMWeight;

    /// - Add state without overriding flow rate
    article3->addState(article2);

    //  Parameter               article1       article2      article3
    //  mass flow rate          1.000000       2.000000       3.000000
    //  CO2 mass fraction       0.750000       0.250000       0.416667
    //  O2 mass fraction        0.250000       0.750000       0.583333
    //  CO2 mass flow rate      0.750000       0.500000       1.250000
    //  O2 mass flow rate       0.250000       1.500000       1.750000
    //  CO2 mole flow rate      0.017042       0.011361       0.028403
    //  O2 mole flow rate       0.007813       0.046877       0.054690
    //  mole flow rate          0.024855       0.058238       0.083093
    //  molecular weight       40.234053      34.341873      36.104338
    //  CO2 mole fraction       0.685660       0.195082       0.341823
    //  O2 mole fraction        0.314340       0.804918       0.658177
    //  mass fraction           0.333333       0.666667       1.000000
    //  mole fraction           0.299119       0.700881       1.000000
    //  CO2 pressure        68565.967577   21459.037611   37161.347600
    //  O2 pressure         31434.032423   88540.962389   69505.319067
    //  pressure           100000.000000  110000.000000  107008.807622
    //
    // CO2 molecular weight 44.0095
    // O2 molecular weight  31.9988

    const double tolerance = 1.0e-06;
    double expected;

    /// @test for add state composite state attributes
    //            double expected = TODO;
    //            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article3->mTemperature,      tolerance);

    //            expected = 0.299191 * article1->mPressure + 0.700881 * article2->mPressure;
    //            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article3->mPressure,         tolerance);

    expected = article1->mFlowRate + article2->mFlowRate;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,          article3->mFlowRate,         tolerance);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(36.104338, article3->mMWeight,          tolerance);

    //            expected = 0.333333 * article1->mDensity + 0.666667 * article2->mDensity;
    //            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article3->mDensity,          tolerance);

    //            expected = 0.299191 * article1->mViscosity + 0.700881 * article2->mViscosity;
    //            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article3->mViscosity,        tolerance);

    //            expected = 0.299191 * article1->mSpecificHeat +
    //                       0.700881 * article2->mSpecificHeat;
    //            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article3->mSpecificHeat,     tolerance);

    //            expected = 0.299191 * article1->mSpecificEnthalpy +
    //                       0.700881 * article2->mSpecificEnthalpy;
    //            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article3->mSpecificEnthalpy, tolerance);

    /// CO2 constituent state
    // mass fraction
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.416667, article3->mConstituents[0].mMassFraction, tolerance);
    // mole fraction
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.341823, article3->mConstituents[0].mMoleFraction, tolerance);

    /// O2 constituent state
    // mass fraction
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.583333, article3->mConstituents[1].mMassFraction, tolerance);
    // mole fraction
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.658177, article3->mConstituents[1].mMoleFraction, tolerance);

    /// @test for add state trace compounds
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCO,  article3->getTraceCompounds()->getMass(ChemicalCompound::CO),  tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mH2O, article3->getTraceCompounds()->getMass(ChemicalCompound::H2O), tolerance);

    const double cptm1 = article1->mFlowRate * article1->getSpecificEnthalpy();
    const double cptm2 = article2->mFlowRate * article2->getSpecificEnthalpy();
    const double cptm3 = article3->mFlowRate * article3->getSpecificEnthalpy();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cptm1 + cptm2, cptm3, mTolerance);

    delete article3;
    delete article2;
    delete initData;
    delete article1;
    delete configData;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for combining opposite flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testStateUpdatersOppositeFlows()
{
    UT_RESULT;

    {
        /// - Test articles have +CO2 and -O2 flowrates
        FluidProperties::FluidType types[2];
        double massFractions[2];
        types[0]    = FluidProperties::GUNNS_CO2; massFractions[0] =  1.25;
        types[1]    = FluidProperties::GUNNS_O2;  massFractions[1] = -0.25;
        PolyFluidConfigData* configData  = new PolyFluidConfigData(mProperties, types, 2);
        PolyFluidInputData* initData  = new PolyFluidInputData(300.0,
            100000.0,
            1.0,
            3.0,
            massFractions);
        FriendlyPolyFluid* article1 = new FriendlyPolyFluid(*configData, *initData);

        massFractions[0] =  1.5;
        massFractions[1] = -0.5;
        delete initData;
        initData  = new PolyFluidInputData(350.0,
            110.0,
            3.0,
            1.0,
            massFractions);
        FriendlyPolyFluid* article2 = new FriendlyPolyFluid(*configData, *initData);
        FriendlyPolyFluid* article3 = new FriendlyPolyFluid(*configData, *initData);

        article3->resetState();
        article3->setState(article1);
        /// - Add state with overriding flow rate
        article3->addState(article2, 2.0);

        //  __________________________________________________________________
        // | Parameter            |   article1    |   article2   |   article3 |
        // |----------------------|---------------|--------------|------------|
        // | mass flow rate       |   1.000000    |   2.000000   |   3.000000 |
        // | CO2 mass fraction    |   1.250000    |   1.500000   |   1.416667 |
        // | O2 mass fraction     |  -0.250000    |  -0.500000   |  -0.416667 |
        // | CO2 mass flow rate   |   1.250000    |   3.000000   |   4.250000 |
        // | O2 mass flow rate    |  -0.250000    |  -1.000000   |  -1.250000 |
        // | CO2 mole flow rate   |   0.028403    |   0.068167   |   0.096570 |
        // | O2 mole flow rate    |  -0.007813    |  -0.031251   |  -0.039064 |
        // | mole flow rate       |   0.020590    |   0.036916   |   0.057506 |
        // | molecular weight     |  48.566875    |  54.177156   |  52.168384 |
        // | CO2 mole fraction    |   1.379443    |   1.846550   |   1.679301 |
        // | O2 mole fraction     |  -0.379443    |  -0.846550   |  -0.679301 |
        // | mass fraction        |   0.333333    |   0.666667   |   1.000000 |
        // | mole fraction        |   0.358052    |   0.641948   |   1.000000 |
        // |______________________|_______________|______________|____________|
        //  CO2 molecular weight: 44.009500
        //  O2 molecular weight:  31.998800


        CPPUNIT_ASSERT_DOUBLES_EQUAL(+3.00,
            article3->getFlowRate(), mTolerance);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.416667,
            article3->getMassFraction(types[0]), mTolerance);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.416667,
            article3->getMassFraction(types[1]),  mTolerance);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.679301,
            article3->getMoleFraction(types[0]), mTolerance);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.679301,
            article3->getMoleFraction(types[1]),  mTolerance);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(52.168384, article3->getMWeight(), mTolerance);

        const double cptm1 = article1->mFlowRate * article1->getSpecificEnthalpy();
        const double cptm2 = 2.0 * article2->getSpecificEnthalpy();
        const double cptm3 = article3->mFlowRate * article3->getSpecificEnthalpy();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(cptm1 + cptm2, cptm3, mTolerance);

        delete article3;
        delete article2;
        delete article1;
        delete initData;
        delete configData;
    } {
        /// - Test articles have +CO2 and -O2 flowrates
        FluidProperties::FluidType types[2];
        double massFractions[2];
        types[0]    = FluidProperties::GUNNS_CO2; massFractions[0] =  1.25;
        types[1]    = FluidProperties::GUNNS_O2;  massFractions[1] = -0.25;
        PolyFluidConfigData* configData = new PolyFluidConfigData(mProperties, types, 2);
        PolyFluidInputData*  initData   = new PolyFluidInputData(300.0,
            100.0,
            1.0,
            3.0,
            massFractions);
        FriendlyPolyFluid* article1 = new FriendlyPolyFluid(*configData, *initData);

        massFractions[0] =  1.5;
        massFractions[1] = -0.5;
        delete initData;
        initData  = new PolyFluidInputData(350.0,
            110000.0,
            3.0,
            1.0,
            massFractions);
        FriendlyPolyFluid* article2 = new FriendlyPolyFluid(*configData, *initData);
        FriendlyPolyFluid* article3 = new FriendlyPolyFluid(*configData, *initData);

        article3->resetState();
        article3->setState(article1);
        /// - Add state with overriding flow rate
        article3->addState(article2, 2.0);

        //  __________________________________________________________________
        // | Parameter            |   article1    |   article2   |   article3 |
        // |----------------------|---------------|--------------|------------|
        // | mass flow rate       |   1.000000    |   2.000000   |   3.000000 |
        // | CO2 mass fraction    |   1.250000    |   1.500000   |   1.416667 |
        // | O2 mass fraction     |  -0.250000    |  -0.500000   |  -0.416667 |
        // | CO2 mass flow rate   |   1.250000    |   3.000000   |   4.250000 |
        // | O2 mass flow rate    |  -0.250000    |  -1.000000   |  -1.250000 |
        // | CO2 mole flow rate   |   0.028403    |   0.068167   |   0.096570 |
        // | O2 mole flow rate    |  -0.007813    |  -0.031251   |  -0.039064 |
        // | mole flow rate       |   0.020590    |   0.036916   |   0.057506 |
        // | molecular weight     |  48.566875    |  54.177156   |  52.168384 |
        // | CO2 mole fraction    |   1.379443    |   1.846550   |   1.679301 |
        // | O2 mole fraction     |  -0.379443    |  -0.846550   |  -0.679301 |
        // | mass fraction        |   0.333333    |   0.666667   |   1.000000 |
        // | mole fraction        |   0.358052    |   0.641948   |   1.000000 |
        // |______________________|_______________|______________|____________|
        //  CO2 molecular weight: 44.009500
        //  O2 molecular weight:  31.998800

        CPPUNIT_ASSERT_DOUBLES_EQUAL(+3.00,
            article3->getFlowRate(), mTolerance);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.416667,
            article3->getMassFraction(types[0]), mTolerance);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.416667,
            article3->getMassFraction(types[1]),  mTolerance);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.679301,
            article3->getMoleFraction(types[0]), mTolerance);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.679301,
            article3->getMoleFraction(types[1]),  mTolerance);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(52.168384, article3->getMWeight(), mTolerance);

        const double cptm1 = article1->mFlowRate * article1->getSpecificEnthalpy();
        const double cptm2 = 2.0 * article2->getSpecificEnthalpy();
        const double cptm3 = article3->mFlowRate * article3->getSpecificEnthalpy();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(cptm1 + cptm2, cptm3, mTolerance);

        delete article3;
        delete article2;
        delete article1;
        delete initData;
        delete configData;
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for computes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testComputes()
{
    UT_RESULT;

    {
        /// @test for computeTemperature (note tighter tolerance)
        double expected = mArticle1->getTemperature();
        const double specificEnthalpy = mArticle1->getSpecificEnthalpy();
        mArticle1->setTemperature(expected + 1.0);
        double returned = mArticle1->computeTemperature(specificEnthalpy);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-11);

        /// - Consistency check specificEnthalpy = computeTemperature(computeSpecificEnthalpy(t))
        double returned2 = mArticle1->computeSpecificEnthalpy(returned);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(specificEnthalpy, returned2, 1.0e-08);

        /// @test for computeDensity (note tighter tolerance)
        mArticle1->setTemperature(mTemperature);
        expected = mArticle1->getDensity();
        mArticle1->setPressure(mPressure - 1.0);
        mArticle1->setTemperature(mTemperature - 1.0);
        returned = mArticle1->computeDensity(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-16);

        /// - Consistency check pressure = computePressure(t, computeDensity(t, p))
        returned2 = mArticle1->computePressure(mTemperature, returned);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure, returned2, 1.0e-10);

        /// @test for computeSpecificEnthalpy
        mArticle1->setPressure(mPressure);
        mArticle1->setTemperature(mTemperature);
        expected = mArticle1->getSpecificEnthalpy();
        mArticle1->setPressure(mPressure + 1000.0);
        mArticle1->setTemperature(mTemperature + 10.0);
        returned = mArticle1->getSpecificEnthalpy();
        returned = mArticle1->computeSpecificEnthalpy(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-12);

        /// - Consistency check
        ///   temperature = computeTemperature(computeSpecificEnthalpy(temperature))
        returned2 = mArticle1->computeTemperature(returned);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, returned2, 1.0e-11);

        /// @test for computePressure (note tighter tolerance)
        mArticle1->setPressure(mPressure);
        mArticle1->setTemperature(mTemperature);
        expected = mArticle1->getDensity();
        mArticle1->setPressure(mPressure - 1000.0);
        mArticle1->setTemperature(mTemperature - 10.0);
        returned = mArticle1->computePressure(mTemperature, expected);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure, returned, 1.0e-10);

        // Consistency check density = computeDensity(t, computePressure(t, density))
        returned2 = mArticle1->computeDensity(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned2, 1.0e-12);
    } {
        /// @test for computeTemperature (note tighter tolerance)
        double expected = mArticle2->getTemperature();
        const double specificEnthalpy = mArticle2->getSpecificEnthalpy();
        mArticle2->setTemperature(expected + 10.0);
        double returned = mArticle2->computeTemperature(specificEnthalpy);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);

        /// - Consistency check
        ///   specificEnthalpy = computeTemperature(computeSpecificEnthalpy(t))
        double returned2 = mArticle2->computeSpecificEnthalpy(returned);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(specificEnthalpy, returned2, 1.0e-08);

        /// @test for computeDensity (note tighter tolerance)
        mArticle2->setPressure(1400.0);
        mArticle2->setTemperature(270.0);
        expected = mArticle2->getDensity();
        mArticle2->setPressure(1410.0);
        mArticle2->setTemperature(280.0);
        returned = mArticle2->computeDensity(270.0, 1400.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-16);

        /// - Consistency check pressure = computePressure(t, computeDensity(t, p))
        returned2 = mArticle2->computePressure(270.0, returned);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1400.0, returned2, 1.0e-04);

        /// @test for computeSpecificEnthalpy
        mArticle2->setPressure(1400.0);
        mArticle2->setTemperature(270.0);
        expected = mArticle2->getSpecificEnthalpy();
        mArticle2->setPressure(1410.0);
        mArticle2->setTemperature(280.0);
        returned = mArticle2->getSpecificEnthalpy();
        returned = mArticle2->computeSpecificEnthalpy(270.0, 1400.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-12);

        /// - Consistency check
        ///   temperature = computeTemperature(computeSpecificEnthalpy(temperature))
        returned2 = mArticle2->computeTemperature(returned);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(270.0, returned2, 1.0e-10);

        /// @test for computePressure (note tighter tolerance)
        mArticle2->setPressure(1400.0);
        mArticle2->setTemperature(270.0);
        expected = mArticle2->getDensity();
        mArticle2->setPressure(1410.0);
        mArticle2->setTemperature(280.0);
        returned = mArticle2->computePressure(270.0, expected);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1400.0, returned, 1.0e-04);

        // Consistency check
        ///   density = computeDensity(t, computePressure(t, density))
        returned2 = mArticle2->computeDensity(270.0, 1400.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned2, 1.0e-12);
    } {
        /// @test for computeTemperature (note tighter tolerance)
        double expected = mArticle3->getTemperature();
        const double specificEnthalpy = mArticle3->getSpecificEnthalpy();
        mArticle3->setTemperature(expected + 10.0);
        double returned = mArticle3->computeTemperature(specificEnthalpy);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-12);

        /// - Consistency check
        ///   specificEnthalpy = computeTemperature(computeSpecificEnthalpy(t))
        double returned2 = mArticle3->computeSpecificEnthalpy(returned);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(specificEnthalpy, returned2, 1.0e-09);

        /// @test for computeDensity (note tighter tolerance)
        mArticle3->setTemperature(mTemperature);
        expected = mArticle3->getDensity();
        mArticle3->setPressure(mPressure - 1000.0);
        mArticle3->setTemperature(mTemperature - 10.0);
        returned = mArticle3->computeDensity(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-16);

        /// - Consistency check
        ///   pressure = computePressure(t, computeDensity(t, p))
        returned2 = mArticle3->computePressure(mTemperature, returned);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure, returned2, 1.0e-10);

        /// @test for computeSpecificEnthalpy
        mArticle3->setPressure(mPressure);
        mArticle3->setTemperature(mTemperature);
        expected = mArticle3->getSpecificEnthalpy();
        mArticle3->setPressure(mPressure + 1000.0);
        mArticle3->setTemperature(mTemperature + 10.0);
        returned = mArticle3->getSpecificEnthalpy();
        returned = mArticle3->computeSpecificEnthalpy(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-12);

        /// - Consistency check
        ///   temperature = computeTemperature(computeSpecificEnthalpy(temperature))
        returned2 = mArticle3->computeTemperature(returned);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, returned2, 1.0e-12);

        /// @test for computePressure (note tighter tolerance)
        mArticle3->setPressure(mPressure);
        mArticle3->setTemperature(mTemperature);
        expected = mArticle3->getDensity();
        mArticle3->setPressure(mPressure - 1000.0);
        mArticle3->setTemperature(mTemperature - 10.0);
        returned = mArticle3->computePressure(mTemperature, expected);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure, returned, 1.0e-10);

        // Consistency check
        ///   density = computeDensity(t, computePressure(t, density))
        returned2 = mArticle3->computeDensity(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned2, 1.0e-12);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for set sequence.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testSetSequence()
{
    UT_RESULT;

    double temperature = 283.15;
    double pressure    = 689.475728;
    double flowRate    = 1.0;
    double mass        = 1.0;
    FluidProperties::FluidType types[NMULTI];
    double fraction[NMULTI];
    types[0]    = FluidProperties::GUNNS_N2;  fraction[0] = 1.0;
    types[1]    = FluidProperties::GUNNS_O2;  fraction[1] = 0.0;
    types[2]    = FluidProperties::GUNNS_CO2; fraction[2] = 0.0;
    types[3]    = FluidProperties::GUNNS_CO;  fraction[3] = 0.0;
    types[4]    = FluidProperties::GUNNS_H2O; fraction[4] = 0.0;
    types[5]    = FluidProperties::GUNNS_NH3; fraction[5] = 0.0;
    types[6]    = FluidProperties::GUNNS_H2;  fraction[6] = 0.0;
    PolyFluidConfigData configData(mProperties, types, UtPolyFluid::NMULTI);
    PolyFluidInputData initData(temperature,
        pressure,
        flowRate,
        mass,
        fraction);

    double expectedMW = mProperties->getProperties(FluidProperties::GUNNS_N2)->getMWeight();

    /// - Define nominal test article
    FriendlyPolyFluid article;
    article.initializeName("article");
    article.initialize(configData, initData);
    article.setMassAndMassFractions(0.0, fraction);

    double returnedMW = article.getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW, returnedMW, mTolerance);

    const double expectedPressure = article.getPressure();
    const double expectedDensity  = article.getDensity();
    article.setPressure(expectedPressure);

    returnedMW = article.getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW, returnedMW, mTolerance);

    double returnedPressure = article.getPressure();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, returnedPressure, mTolerance);

    double returnedDensity  = article.getDensity();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDensity,  returnedDensity,  mTolerance);

    article.setTemperature(temperature);

    returnedMW = article.getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW, returnedMW, mTolerance);

    returnedDensity  = article.getDensity();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDensity,  returnedDensity,  mTolerance);

    returnedPressure = article.getPressure();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, returnedPressure, mTolerance);

    article.setMass(1.0);

    returnedMW = article.getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW, returnedMW, mTolerance);

    returnedDensity  = article.getDensity();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDensity,  returnedDensity,  mTolerance);

    returnedPressure = article.getPressure();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, returnedPressure, mTolerance);

    returnedDensity = article.computeDensity(temperature, expectedPressure);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDensity,  returnedDensity,  mTolerance);

    returnedPressure = article.computePressure(temperature, expectedDensity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressure, returnedPressure, mTolerance);

    returnedMW = article.getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMW, returnedMW, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for edit methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testEdits()
{
    UT_RESULT;

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyPolyFluid article;
    article.initializeName("article");
    article.initialize(*mConfigData1, *mInputData1);

    /// @test edit temperature and total pressure
    double temperature = 300.0;
    double pressure    = 100.0;

    article.edit(temperature, pressure);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pressure,    article.getPressure(),    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature, article.getTemperature(), mTolerance);

    /// @test edit temperature and partial pressures
    temperature = 400.0;
    pressure    = 0.0;
    double partialPressures[UtPolyFluid::NMULTI];
    for (int i=0; i<UtPolyFluid::NMULTI; ++i) {
        partialPressures[i] = static_cast<double>(i) + 1.0;
        pressure += partialPressures[i];
    }

    CPPUNIT_ASSERT_NO_THROW(article.edit(temperature, partialPressures));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pressure,    article.getPressure(),    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature, article.getTemperature(), mTolerance);

    for (int i=0; i<UtPolyFluid::NMULTI; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(partialPressures[i],
                                     article.getPartialPressure(article.getType(i)),
                                     mTolerance);
    }

    /// @test all partial pressures sum to zero
    for (int i=0; i<UtPolyFluid::NMULTI; ++i) {
        partialPressures[i] = 0.0;
    }
    CPPUNIT_ASSERT_NO_THROW(article.edit(temperature, partialPressures));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,         article.getPressure(),    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature, article.getTemperature(), mTolerance);

    /// @test edit temperature and partial pressures throws H&S warning on bad partial pressure.
    //TODO this is untestable because we can't make an exception for the try block to catch.

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for compute temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testComputeTemperature()
{
    UT_RESULT;

    double temperature = 283.15;
    double pressure    = 689.475728;
    double flowRate    = 1.0;
    double mass        = 1.0;
    FluidProperties::FluidType types[NMULTI];
    double fraction[NMULTI];
    types[0]    = FluidProperties::GUNNS_N2;  fraction[0] = 0.0;
    types[1]    = FluidProperties::GUNNS_O2;  fraction[1] = 0.5;
    types[2]    = FluidProperties::GUNNS_CO2; fraction[2] = 0.5;
    types[3]    = FluidProperties::GUNNS_CO;  fraction[3] = 0.0;
    types[4]    = FluidProperties::GUNNS_H2O; fraction[4] = 0.0;
    types[5]    = FluidProperties::GUNNS_NH3; fraction[5] = 0.0;
    types[6]    = FluidProperties::GUNNS_H2;  fraction[6] = 0.0;
    PolyFluidConfigData configData(mProperties, types, UtPolyFluid::NMULTI);
    PolyFluidInputData initData(temperature,
        pressure,
        flowRate,
        mass,
        fraction);

    /// - Define nominal test article
    FriendlyPolyFluid article;
    article.initializeName("article");
    article.initialize(configData, initData);

    const double specificEnthalpy = article.getSpecificEnthalpy();
    const double expected         = article.getTemperature();
    article.setTemperature(mTemperature + 10.0);
    const double returnedMass     = article.getMass();
    article.setMass(returnedMass);
    const double returned         = article.computeTemperature(specificEnthalpy);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,  returned,  1.0e-12);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for liquid pressure and density.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testLiquidPressureDensity()
{
    UT_RESULT;

    /// - Define initialization data
    const double temperature = 300.;
    const double pressure    = 670.;
    const double flowRate    = 1.0;
    const double mass        = 1.0;
    FluidProperties::FluidType types[2];
    types[0]                  = FluidProperties::GUNNS_AMMONIA;
    types[1]                  = FluidProperties::GUNNS_WATER;
    PolyFluidConfigData configData(mProperties, types, 2);
    double fraction[2];
    fraction[0]              = 1.0;
    fraction[1]              = 0.0;
    PolyFluidInputData initData(temperature,
        pressure,
        flowRate,
        mass,
        fraction);

    /// - Define test article
    PolyFluid article(configData, initData);

    /// test for setter calling sequence
    fraction[0]              = 1.0;
    fraction[1]              = 0.0;
    article.setMassAndMassFractions(3.0, fraction);
    const double newP = 1110.0;
    article.setPressure(newP);
    const double newT = 220.0;
    article.setTemperature(newT);
    article.setMass(0.2);

    const double density    = article.getDensity();
    const double returnedD  = article.computeDensity(newT, newP);
    const double returnedP  = article.computePressure(newT, returnedD);
    const double returned2D = article.computeDensity(newT, returnedP);
    const double returned2P = article.computePressure(newT, returned2D);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(density,  returnedD,  1.0e-04);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(newP,     returnedP,  1.0e-04);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(density,  returned2D, 1.0e-04);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(newP,     returned2P, 1.0e-04);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for multiple initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testMultipleInitialization()
{
    UT_RESULT;

    FriendlyPolyFluid article;
    FluidProperties::FluidType types[3];
    double massFraction[3];
    types[0]    = FluidProperties::GUNNS_H2O; massFraction[0] = 0.50;
    types[1]    = FluidProperties::GUNNS_CO2; massFraction[1] = 0.25;
    types[2]    = FluidProperties::GUNNS_N2;  massFraction[2] = 0.25;
    PolyFluidConfigData configData(mProperties, types, 3);
    PolyFluidInputData  inputData(mTemperature + 10.0,
        mPressure + 1.0,
        mFlowRate + 1.0,
        mMass + 1.0,
        massFraction);

    article.initializeName("article");
    article.initialize(configData, inputData);
    article.initialize(*mConfigData1, *mInputData1);

    /// @test for composite state attributes
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, article.mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure,    article.mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,    article.mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMass,        article.mMass,        mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMole1,       article.mMole,        mTolerance);
    CPPUNIT_ASSERT(mArticle1->mInitFlag);

    /// @test for constituent state attributes
    for (int i = 0; i < UtPolyFluid::NMULTI; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMassFraction1[i],
                                     article.mConstituents[i].mMassFraction,
                                     mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mMoleFraction1[i],
                                     article.mConstituents[i].mMoleFraction,
                                     mTolerance);
    }

    /// @test for composite state properties
    const FluidProperties* properties[NMULTI];
    for (int i = 0; i < NMULTI; i++) {
        properties[i] = mProperties->getProperties(article.mConstituents[i].mType);
    }

    double expected = mMass / mMole1;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mMWeight,          mTolerance);

    // composite density is sum of constituent densities at partial pressure
    expected = 0.0;
    for (int i = 0; i < NMULTI; i++) {
        expected += properties[i]->getDensity(mTemperature, mMoleFraction1[i] * mPressure);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mDensity,          mTolerance);

    // composite viscosity is mole fraction weighted sum of constituent viscosities at partial
    // pressure
    expected = 0.0;
    for (int i = 0; i < NMULTI; i++) {
        expected += properties[i]->getViscosity(mTemperature, mMoleFraction1[i] * mPressure) *
            mMoleFraction1[i];
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mViscosity,        mTolerance);

    // composite specific heat is mass fraction weighted sum of constituent specific heats at
    // partial pressure
    expected = 0.0;
    for (int i = 0; i < NMULTI; i++) {
        expected += properties[i]->getSpecificHeat(mTemperature, mMoleFraction1[i] * mPressure) *
            mMassFraction1[i];
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSpecificHeat,     mTolerance);

    // composite specific enthalpy is temperature times composite specific heat
    expected *= mTemperature;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mSpecificEnthalpy, mTolerance);

    // composite adiabatic index is mole fraction weighted sum of constituent adiabatic indices at
    // partial pressure
    expected = 0.0;
    for (int i = 0; i < NMULTI; i++) {
        expected += properties[i]->getAdiabaticIndex(mTemperature, mMoleFraction1[i] * mPressure) *
            mMoleFraction1[i];
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, article.mAdiabaticIndex,   mTolerance);

    /// @test for initialization flag
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for fraction normalization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testFractionNormalization()
{
    UT_RESULT;

    FriendlyPolyFluid article;
    FluidProperties::FluidType types[3];
    double fraction[3];
    types[0]    = FluidProperties::GUNNS_H2O;
    types[1]    = FluidProperties::GUNNS_CO2;
    types[2]    = FluidProperties::GUNNS_N2;
    types[0]    = FluidProperties::GUNNS_H2O;
    fraction[0] = 0.50 + 0.5 * PolyFluid::FRACTION_TOLERANCE;
    fraction[1] = 0.25;
    fraction[2] = 0.25;
    PolyFluidConfigData configData(mProperties, types, 3);
    PolyFluidInputData  inputData(mTemperature + 10.0,
        mPressure + 1.0,
        mFlowRate + 1.0,
        mMass + 1.0,
        fraction);

    article.initializeName("article");
    article.initialize(configData, inputData);

    double expectedFraction[3];
    const double sumFractions = 1.0 + 0.5 * PolyFluid::FRACTION_TOLERANCE;
    expectedFraction[0] = fraction[0] / sumFractions;
    expectedFraction[1] = fraction[1] / sumFractions;
    expectedFraction[2] = fraction[2] / sumFractions;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[0],
                                 article.getMassFraction(types[0]),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[1],
                                 article.getMassFraction(types[1]),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[2],
                                 article.getMassFraction(types[2]),
                                 mTolerance);

    article.setMassAndMassFractions(mMass, fraction);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[0],
                                 article.getMassFraction(types[0]),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[1],
                                 article.getMassFraction(types[1]),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[2],
                                 article.getMassFraction(types[2]),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[0], fraction[0], mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[1], fraction[1], mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[2], fraction[2], mTolerance);

    fraction[0] = 0.50 + 0.5 * PolyFluid::FRACTION_TOLERANCE;
    fraction[1] = 0.25;
    fraction[2] = 0.25;
    article.setMoleAndMoleFractions(mMass, fraction);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[0],
                                 article.getMoleFraction(types[0]),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[1],
                                 article.getMoleFraction(types[1]),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[2],
                                 article.getMoleFraction(types[2]),
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[0], fraction[0], mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[1], fraction[1], mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFraction[2], fraction[2], mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for init construction exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testConfigAndInputConstructionExceptions()
{
    UT_RESULT;

    {
        /// @test for config data exception on zero number of types
        PolyFluidConfigData config(mProperties, mType1, 0);
        CPPUNIT_ASSERT_THROW(new PolyFluid(config, *mInputData1), TsInitializationException);
        PolyFluid fluid;
        CPPUNIT_ASSERT_THROW(fluid.initialize(config, *mInputData1), TsInitializationException);
    } {
        /// @test for config data exception on null pointer to types
        PolyFluidConfigData config(mProperties, 0, 1);
        CPPUNIT_ASSERT_THROW(new PolyFluid(config, *mInputData1), TsInitializationException);
        PolyFluid fluid;
        CPPUNIT_ASSERT_THROW(fluid.initialize(config, *mInputData1), TsInitializationException);
    } {
        /// @test for input data exception on null pointer to mass fractions
        PolyFluidInputData input(mTemperature,  mPressure, mFlowRate, mMass, 0);
        CPPUNIT_ASSERT_THROW(new PolyFluid(*mConfigData1, input), TsInitializationException);
        PolyFluid fluid;
        CPPUNIT_ASSERT_THROW(fluid.initialize(*mConfigData1, input), TsInitializationException);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for nominal constructor exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testConstructionExceptions()
{
    UT_RESULT;

    {
        /// @test for exception on different number of constituents in config and input data.
        CPPUNIT_ASSERT_THROW(new PolyFluid(*mConfigData1, *mInputData2), TsInitializationException);
    } {
        /// @test for temperature too small. This exception is actually being thrown from
        ///       constituent MonoFluid before PolyFluid gets a chance
        PolyFluidInputData initData(0.0,
                                    mPressure,
                                    mFlowRate,
                                    mMass,
                                    mMassFraction1);
        CPPUNIT_ASSERT_THROW(new PolyFluid(*mConfigData1, initData), TsInitializationException);
    } {
        /// @test for pressure too small. This exception is actually being thrown from
        ///       constituent MonoFluid before PolyFluid gets a chance
        PolyFluidInputData initData(mTemperature,
                                    0.0,
                                    mFlowRate,
                                    mMass,
                                    mMassFraction1);
        CPPUNIT_ASSERT_THROW(new PolyFluid(*mConfigData1, initData), TsInitializationException);
    } {
        /// @test for mass fractions that don't add up to 1
        double massFraction[7];
        for (int i = 0; i < 7; i++) {
            massFraction[i] = 0.1 * i;
        }
        PolyFluidInputData initData(mTemperature,
                                    mPressure,
                                    mFlowRate,
                                    mMass,
                                    massFraction);
        CPPUNIT_ASSERT_THROW(new PolyFluid(*mConfigData1, initData), TsInitializationException);
    } {
        /// @test for inconsistent phase
        double fraction[2];
        fraction[0] = 0.500;
        fraction[1] = 0.500;
        PolyFluidInputData input(mTemperature,
                                    mPressure,
                                    mFlowRate,
                                    mMass,
                                    fraction);
        CPPUNIT_ASSERT_THROW(new PolyFluid(*mConfigData2, input), TsInitializationException);
    } {
        /// @test for copy constructor called with empty name argument
        FriendlyPolyFluid article;
        article.initializeName("article");
        article.initialize(*mConfigData1, *mInputData1);
        CPPUNIT_ASSERT_THROW(new PolyFluid(article, ""), TsInitializationException);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for build method exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testInitializationExceptions()
{
    UT_RESULT;

    /// - Start with uninitialized test article
    PolyFluid article;

    /// @test for exception on different number of constituents in config and input data.
    article.initializeName("article");
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData1, *mInputData2),
                         TsInitializationException);

    {
        /// @test for temperature too small. This exception is actually being thrown from
        ///       constituent MonoFluid before PolyFluid gets a chance
        PolyFluid article;
        PolyFluidInputData initData(0.0,
                                    mPressure,
                                    mFlowRate,
                                    mMass,
                                    mMassFraction1);
        article.initializeName("article");
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData1, initData),
                             TsInitializationException);
    } {
        /// @test for pressure too small. This exception is actually being thrown from
        ///       constituent MonoFluid before PolyFluid gets a chance
        PolyFluid article;
        PolyFluidInputData initData(mTemperature,
                                    0.0,
                                    mFlowRate,
                                    mMass,
                                    mMassFraction1);
        article.initializeName("article");
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData1, initData),
                             TsInitializationException);
    } {
        /// @test for mass fractions that don't add up to 1
        double massFraction[7];
        for (int i = 0; i < 7; i++) {
            massFraction[i] = 0.1 * i;
        }
        PolyFluid article;
        PolyFluidInputData initData(mTemperature,
                                    mPressure,
                                    mFlowRate,
                                    mMass,
                                    massFraction);
        article.initializeName("article");
        CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData1, initData),
                             TsInitializationException);
    } {
        /// @test for inconsistent phase
        FluidProperties::FluidType type[2];
        double                     fraction[2];
        type[0]         = FluidProperties::GUNNS_WATER;  fraction[0] = 0.5;
        type[1]         = FluidProperties::GUNNS_N2;     fraction[1] = 0.5;
        PolyFluidConfigData config(mProperties, type, 2);
        PolyFluidInputData  input(mTemperature,
                                  mPressure,
                                  mFlowRate,
                                  mMass,
                                  fraction);
        PolyFluid article;
        article.initializeName("article");
        CPPUNIT_ASSERT_THROW(article.initialize(config, input), TsInitializationException);
    } {
        /// @test for null pointer to defined fluid properties
        PolyFluidConfigData configData(0, mType1, UtPolyFluid::NMULTI);
        PolyFluid article;
        article.initializeName("article");
        CPPUNIT_ASSERT_THROW(article.initialize(configData, *mInputData1),
                             TsInitializationException);
    } {
        /// @test for name initialized with empty name argument
        PolyFluid article;
        article.initializeName("article");
        CPPUNIT_ASSERT_THROW(article.initializeName(""), TsInitializationException);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for state accessor method exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testStateAccessorExceptions()
{
    UT_RESULT;

    /// - Default construct an uninitialized test article
    PolyFluid article;

    /// - Set type argument to AR, not a constituent in  test article
    FluidProperties::FluidType type = FluidProperties::GUNNS_AMMONIA;

    /// @test for getMoleFraction invalid type exception
    CPPUNIT_ASSERT_THROW(mArticle1->getMoleFraction(type),    TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle2->getMoleFraction(type),    TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle3->getMoleFraction(type),    TsOutOfBoundsException);

    /// @test for getMoleFraction invalid index exception
    CPPUNIT_ASSERT_THROW(mArticle1->getMoleFraction(-1),      TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle1->getMoleFraction(100),     TsOutOfBoundsException);

    /// @test for getMassFraction invalid type exception
    CPPUNIT_ASSERT_THROW(mArticle1->getMassFraction(type),    TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle2->getMassFraction(type),    TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle3->getMassFraction(type),    TsOutOfBoundsException);

    /// @test for getMassFraction invalid index exception
    CPPUNIT_ASSERT_THROW(mArticle1->getMassFraction(-1),      TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle1->getMassFraction(100),     TsOutOfBoundsException);

    /// @test for getPartialPressure invalid type exception
    CPPUNIT_ASSERT_THROW(mArticle1->getPartialPressure(type), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle2->getPartialPressure(type), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle3->getPartialPressure(type), TsOutOfBoundsException);

    /// @test for getConstituent invalid type exception
    CPPUNIT_ASSERT_THROW(mArticle1->getConstituent(type),     TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle2->getConstituent(type),     TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle3->getConstituent(type),     TsOutOfBoundsException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for state modifier method exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testStateModifierExceptions()
{
    UT_RESULT;

    /// - Default construct an uninitialized test article
    PolyFluid article;

    /// - Set argument to 1.0, a valid value
    const double argument = 1.0;

    /// @test for setMassAndMassFractions mass fraction exception using nominal test article and
    ///       mass fractions that don't add up to 1
    const double mass = 1.0;
    double massFraction[NMULTI];
    for (int i = 0; i < NMULTI; i++) {
        massFraction[i] = 0.1 * i;
    }

    CPPUNIT_ASSERT_THROW(mArticle1->setMassAndMassFractions(mass, massFraction),
        TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle2->setMassAndMassFractions(mass, massFraction),
        TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle3->setMassAndMassFractions(mass, massFraction),
        TsOutOfBoundsException);

    /// @test for setMoleAndMoleFractions mole fraction exception using nominal test article and
    ///       mole fractions that don't add up to 1
    const double mole = 1.0;
    double moleFraction[NMULTI];
    for (int i = 0; i < NMULTI; i++) {
        moleFraction[i] = 0.1 * i;
    }

    CPPUNIT_ASSERT_THROW(mArticle1->setMoleAndMoleFractions(mole, moleFraction),
        TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle2->setMoleAndMoleFractions(mole, moleFraction),
        TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle3->setMoleAndMoleFractions(mole, moleFraction),
        TsOutOfBoundsException);

    /// @test for setMass exception when given invalid constituent index
    CPPUNIT_ASSERT_THROW(mArticle1->setMass(-1, 1.0), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(mArticle1->setMass(10, 1.0), TsOutOfBoundsException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for state updater method exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPolyFluid::testStateUpdaterExceptions()
{
    UT_RESULT;

    PolyFluid article0(*(static_cast<PolyFluid*>(mArticle1)), "article0");
    {
        /// @test for setState constituent consistency exception
        FluidProperties::FluidType types[2];
        double massFractions[2];
        types[0]    = FluidProperties::GUNNS_CO2; massFractions[0] = 0.75;
        types[1]    = FluidProperties::GUNNS_O2;  massFractions[1] = 0.25;
        PolyFluidConfigData* configData = new PolyFluidConfigData(mProperties, types, 2);
        PolyFluidInputData*  initData   = new PolyFluidInputData(300.0,
            100.0,
            30.0,
            1.0,
            massFractions);
        PolyFluid article1(*configData, *initData);
        CPPUNIT_ASSERT_THROW(mArticle1->setState(&article1), TsOutOfBoundsException);
        CPPUNIT_ASSERT_THROW(mArticle2->setState(&article1), TsOutOfBoundsException);
        CPPUNIT_ASSERT_THROW(mArticle3->setState(&article1), TsOutOfBoundsException);
        delete initData;
        delete configData;
    } {
        /// @test for addState constituent consistency exception
        FluidProperties::FluidType types[2];
        double massFractions[2];
        types[0]    = FluidProperties::GUNNS_CO2; massFractions[0] = 0.75;
        types[1]    = FluidProperties::GUNNS_O2;  massFractions[1] = 0.25;
        PolyFluidConfigData* configData = new PolyFluidConfigData(mProperties, types, 2);
        PolyFluidInputData*  initData   = new PolyFluidInputData(300.0,
            100.0,
            30.0,
            1.0,
            massFractions);
        PolyFluid article1(*configData, *initData);
        CPPUNIT_ASSERT_THROW(mArticle1->addState(&article1), TsOutOfBoundsException);
        CPPUNIT_ASSERT_THROW(mArticle2->addState(&article1), TsOutOfBoundsException);
        CPPUNIT_ASSERT_THROW(mArticle3->addState(&article1), TsOutOfBoundsException);
        delete initData;
        delete configData;
    } {
        /// @test for combined flowrate of zero
        {
            PolyFluid article1(*mArticle1, "article1");
            PolyFluid article2(*mArticle1, "article2");
            article2.setFlowRate(-mFlowRate);
            CPPUNIT_ASSERT_THROW(article1.addState(&article2), TsOutOfBoundsException);
        } {
            PolyFluid article1(*mArticle2, "article1");
            PolyFluid article2(*mArticle2, "article2");
            article2.setFlowRate(-mFlowRate);
            CPPUNIT_ASSERT_THROW(article1.addState(&article2), TsOutOfBoundsException);
        } {
            PolyFluid article1(*mArticle3, "article1");
            PolyFluid article2(*mArticle3, "article2");
            article2.setFlowRate(-mFlowRate);
            CPPUNIT_ASSERT_THROW(article1.addState(&article2), TsOutOfBoundsException);
        }
    }

    UT_PASS_LAST;
}
