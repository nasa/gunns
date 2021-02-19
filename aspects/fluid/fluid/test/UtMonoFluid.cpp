/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/fluid/MonoFluid.o))
***************************************************************************************************/

#include "properties/FluidProperties.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"

#include "UtMonoFluid.hh"

/// @details  Test identification number.
int UtMonoFluid::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Fitting link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtMonoFluid::UtMonoFluid()
    :
    CppUnit::TestFixture(),
    mType(FluidProperties::NO_FLUID),
    mTemperature(0.0),
    mPressure(0.0),
    mFlowRate(0.0),
    mMass(0.0),
    mMole(0.0),
    mProperties(0),
    mInputData(0),
    mArticle(0),
    mTimeStep(0.0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Fitting link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtMonoFluid::~UtMonoFluid()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::setUp()
{
    /// - Define nominal configuration data
    mType        = FluidProperties::GUNNS_N2;
    mProperties  = new DefinedFluidProperties();
    /// - Define nominal initialization data
    mTemperature =    300.0;
    mPressure    = 100000.0;
    mFlowRate    =     10.0;
    mMass        =      1.0;
    mInputData    = new MonoFluidInputData(mTemperature, mPressure, mFlowRate, mMass);

    /// - Define nominal test article
    mArticle     = new FriendlyMonoFluid(mProperties->getProperties(mType), *mInputData);

    /// - Define nominal derived data
    mMole        = mMass / mProperties->getProperties(mType)->getMWeight();

    /// - Define nominal time step and comparison tolerance
    mTimeStep    = 0.1;
    mTolerance   = 0.001;

    /// - Increment the test identification number.
    ++TEST_ID;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::tearDown()
{
    /// - Delete objects created in setUp
    delete mArticle;
    delete mInputData;
    delete mProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config and init data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testConfigAndInput()
{
    UT_RESULT_INITIAL("GUNNS Fluid Fluids");

    /// @test input nominal construction
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperature, mInputData->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPressure,    mInputData->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlowRate,    mInputData->mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMass,        mInputData->mMass,        mTolerance);

    /// @test input default construction
    MonoFluidInputData* defaultInit = new MonoFluidInputData;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInit->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInit->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInit->mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInit->mMass,        mTolerance);
    delete defaultInit;

    /// @test input copy construction
    MonoFluidInputData copyInit(*mInputData);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mTemperature, copyInit.mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mPressure,    copyInit.mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mFlowRate,    copyInit.mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMass,        copyInit.mMass,        mTolerance);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for construction without exceptions using nominal data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testNominalConstruction()
{
    UT_RESULT;

    /// @test init data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mTemperature, mArticle->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mPressure,    mArticle->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mFlowRate,    mArticle->mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMass,        mArticle->mMass,        mTolerance);

    /// @test derived data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMole,                    mArticle->mMole,        mTolerance);

    /// @test pointer to properties
    CPPUNIT_ASSERT(mProperties->getProperties(mType) == mArticle->mProperties);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testDefaultConstruction()
{
    UT_RESULT;

    /// - Default construct a test article
    FriendlyMonoFluid* empty = new FriendlyMonoFluid();

    /// @test init data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, empty->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, empty->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, empty->mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, empty->mMass,        mTolerance);

    /// @test derived data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, empty->mMole,        mTolerance);

    /// @test pointer to properties
    CPPUNIT_ASSERT(0 == empty->mProperties);

    delete empty;

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for copy construction without exceptions using nominal data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testCopyConstruction()
{
    UT_RESULT;

    /// - Copy construct a test article
    FriendlyMonoFluid* copy = new FriendlyMonoFluid(*mArticle);

    /// @test init data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mTemperature, copy->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mPressure,    copy->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFlowRate,    copy->mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mMass,        copy->mMass,        mTolerance);

    /// @test derived data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mMole,        copy->mMole,        mTolerance);

    /// @test pointer to properties
    CPPUNIT_ASSERT(mArticle->mProperties == copy->mProperties);

    delete copy;

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testNominalInitialization()
{
    UT_RESULT;

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyMonoFluid article;
    article.initialize(mProperties->getProperties(mType), *mInputData);

    /// @test init data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mTemperature, article.mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mPressure,    article.mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mFlowRate,    article.mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMass,        article.mMass,        mTolerance);

    /// @test derived data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMole,                    article.mMole,        mTolerance);

    /// @test pointer to properties
    CPPUNIT_ASSERT(mProperties->getProperties(mType) == article.mProperties);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for multiple initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testMultipleInit()
{
    UT_RESULT;

    /// - Default construct and initialize  a test article and then initialize with nominal data set
    FriendlyMonoFluid article;
    MonoFluidInputData  inputData(mTemperature + 10.0,
                                  mPressure + 1000.0,
                                  mFlowRate + 1.0,
                                  mMass + 1.0);
    article.initialize(mProperties->getProperties(FluidProperties::GUNNS_O2), inputData);
    article.initialize(mProperties->getProperties(mType), *mInputData);

    /// @test init data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mTemperature, article.mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mPressure,    article.mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mFlowRate,    article.mFlowRate,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMass,        article.mMass,        mTolerance);

    /// @test derived data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMole,                    article.mMole,        mTolerance);

    /// @test pointer to properties
    CPPUNIT_ASSERT(mProperties->getProperties(mType) == article.mProperties);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for state accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testStateAccessors()
{
    UT_RESULT;

    /// @test getTemperature
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mTemperature, mArticle->getTemperature(), mTolerance);

    /// @test getPressure
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mPressure,    mArticle->getPressure(),    mTolerance);

    /// @test getFlowRate
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mFlowRate,    mArticle->getFlowRate(),    mTolerance);

    /// @test getMass
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mMass,        mArticle->getMass(),        mTolerance);

    /// @test getMole
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mMole,        mArticle->getMole(),        mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for property accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testPropertyAccessors()
{
    UT_RESULT;

    /// @test getType
    CPPUNIT_ASSERT(mType == mArticle->getType());

    /// @test getPhase
    FluidProperties::FluidPhase expected = mProperties->getProperties(mType)->getPhase();
    CPPUNIT_ASSERT(expected == mArticle->getPhase());

    {
        /// @test getMWeight
        const double expected =
            mProperties->getProperties(mType)->getMWeight();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,
            mArticle->getMWeight(),
            mTolerance);
    } {
        /// @test getDensity
        const double expected =
            mProperties->getProperties(mType)->getDensity(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getDensity(), mTolerance);
    } {
        /// @test getViscosity
        const double expected =
            mProperties->getProperties(mType)->getViscosity(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getViscosity(), mTolerance);
    } {
        /// @test getSpecificHeat
        const double expected =
            mProperties->getProperties(mType)->getSpecificHeat(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getSpecificHeat(), mTolerance);
    } {
        /// @test getSpecificEnthalpy
        const double expected =
            mProperties->getProperties(mType)->getSpecificEnthalpy(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getSpecificEnthalpy(), mTolerance);
    } {
        /// @test getAdiabaticIndex
        const double expected =
            mProperties->getProperties(mType)->getAdiabaticIndex(mTemperature, mPressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->getAdiabaticIndex(), mTolerance);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for state modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testStateModifiers()
{
    UT_RESULT;

    {
        /// @test setTemperature
        const double temperature  = 0.5 * mTemperature;
        mArticle->setTemperature(temperature);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature, mArticle->mTemperature, mTolerance);
        mArticle->mTemperature    = mTemperature;
    } {
        /// @test setPressure
        const double pressure     = 0.5 * mPressure;
        mArticle->setPressure(pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(pressure, mArticle->mPressure, mTolerance);
        mArticle->mPressure       = mPressure;
    } {
        /// @test setFlowRate
        const double flowRate     = 0.5 * mFlowRate;
        mArticle->setFlowRate(flowRate);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(flowRate, mArticle->mFlowRate, mTolerance);
        mArticle->mFlowRate       = mFlowRate;
    } {
        /// @test setMass
        const double mass         = 0.5 * mArticle->mMass;
        mArticle->setMass(mass);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mass, mArticle->mMass, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mMass / mProperties->getProperties(mType)->getMWeight(),
                                    mArticle->mMole, mTolerance);
        mArticle->mMass           = mMass;
        mArticle->mMole           = mMole;
    } {
        /// @test setMole
        const double mole         = 2.0 * mMole;
        mArticle->setMole(mole);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mole, mArticle->mMole, mTolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mMole * mProperties->getProperties(mType)->getMWeight(),
                                     mArticle->mMass, mTolerance);
        mArticle->mMass           = mMass;
        mArticle->mMole           = mMole;
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for state updaters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testStateUpdaters()
{
    UT_RESULT;

    /// - Build test articles
    MonoFluidInputData init1(200.0, 100.0, 9.0, 0.0);
    MonoFluidInputData init2(300.0, 200.0, 1.0, 0.0);
    FriendlyMonoFluid* article1 = new FriendlyMonoFluid(mProperties->getProperties(FluidProperties::GUNNS_N2), init1);
    FriendlyMonoFluid* article2 = new FriendlyMonoFluid(mProperties->getProperties(FluidProperties::GUNNS_N2), init2);
    FriendlyMonoFluid* article3 = new FriendlyMonoFluid(*mArticle);

    /// @test resetState
    article3->resetState();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mFlowRate,    mTolerance);

    /// @test setState
    article3->setState(article1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mTemperature,
        article3->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mPressure,
        article3->mPressure,    mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mFlowRate,
        article3->mFlowRate,    mTolerance);

    /// @test addState
    article3->addState(article2);

    double expected = 0.1 * (9 * article1->mTemperature + article2->mTemperature);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        expected,
        article3->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1 * (9 * article1->mPressure + article2->mPressure),
        article3->mPressure, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article1->mFlowRate + article2->mFlowRate,
        article3->mFlowRate, mTolerance);

    /// @test add state with zero flow rates
    article1->mFlowRate = 0.0;
    article2->mFlowRate = 0.0;
    article3->resetState();
    article3->setState(article1);
    article3->addState(article2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * (article1->mTemperature + article2->mTemperature),
        article3->mTemperature, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5 * (article1->mPressure + article2->mPressure),
        article3->mPressure, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article3->mFlowRate, mTolerance);

    delete article3;
    delete article2;
    delete article1;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for compute methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testComputes()
{
    UT_RESULT;

    {
        /// @test computation of gas temperature for a specified specific enthalpy
        FriendlyMonoFluid article(mProperties->getProperties(FluidProperties::GUNNS_CO2), *mInputData);
        const double specificEnthalpy = article.getSpecificEnthalpy();
        const double expected         = article.getTemperature();
        article.setTemperature(expected + 10.0);
        double returned               = article.computeTemperature(specificEnthalpy);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-08);
    } {
        /// @test computation of liquid temperature for a specified specific enthalpy
        mArticle->setTemperature(200.0);
        const double specificEnthalpy = mArticle->getSpecificEnthalpy();
        const double expected         = mArticle->getTemperature();
        mArticle->setTemperature(expected + 10.0);
        double returned               = mArticle->computeTemperature(specificEnthalpy);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        /// @test computation of gas temperature for a specified specific enthalpy
        FriendlyMonoFluid article(mProperties->getProperties(FluidProperties::GUNNS_O2), *mInputData);
        article.setTemperature(200.0);
        const double specificEnthalpy = article.getSpecificEnthalpy();
        const double expected         = article.getTemperature();
        mArticle->setTemperature(expected + 1000000.0);
        double returned               = article.computeTemperature(specificEnthalpy);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        /// @test computation of liquid specific enthalpy for a specified temperature and pressure
        FriendlyMonoFluid article(mProperties->getProperties(mType), *mInputData);
        const double expected = article.getSpecificEnthalpy();
        mArticle->setTemperature(mInputData->mTemperature - 10.0);
        mArticle->setPressure(mInputData->mPressure - 100.0);
        const double returned = article.computeSpecificEnthalpy(
            mInputData->mTemperature, mInputData->mPressure);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-16);
    } {
        /// @test computation of gas specific enthalpy for a specified temperature and pressure
        FriendlyMonoFluid article(mProperties->getProperties(FluidProperties::GUNNS_O2), *mInputData);
        const double expected = article.getSpecificEnthalpy();
        article.setTemperature(mInputData->mTemperature - 10.0);
        article.setPressure(mInputData->mPressure - 100.0);
        const double returned = article.computeSpecificEnthalpy(
            mInputData->mTemperature, mInputData->mPressure);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-16);
    } {
        /// @test computation of pressure for a specified temperature and density
        const double expected = 100000.0;
        mArticle->setPressure(expected);
        const double density     = mArticle->getDensity();
        const double temperature = mArticle->getTemperature();
        mArticle->setTemperature(temperature + 10.0);
        mArticle->setPressure(expected + 1000.0);
        const double returned = mArticle->computePressure(temperature, density);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        /// @test computation of density for a specified temperature and pressure
        mArticle->setTemperature(mInputData->mTemperature);
        mArticle->setPressure(mInputData->mPressure);
        const double expected = mArticle->getDensity();
        mArticle->setTemperature(mInputData->mTemperature + 10.0);
        mArticle->setPressure(mInputData->mPressure + 100.0);
        const double returned = mArticle->computeDensity(mInputData->mTemperature,
            mInputData->mPressure);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-16);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testConstructionExceptions()
{
    UT_RESULT;

    {
        /// @test constructor for exception on null pointer to properties
        MonoFluidInputData initData(mTemperature, mPressure, mFlowRate, mMass);
        CPPUNIT_ASSERT_THROW(new FriendlyMonoFluid(mProperties->getProperties(FluidProperties::NO_FLUID), initData),
            TsInitializationException);
    } {
        /// @test constructor for exception on temperature too small
        MonoFluidInputData initData(FLT_EPSILON / 2.0, mPressure, mFlowRate, mMass);
        CPPUNIT_ASSERT_THROW(new FriendlyMonoFluid(mProperties->getProperties(mType), initData),
            TsInitializationException);
    } {
        /// @test constructor for exception on pressure too small
        MonoFluidInputData initData(mTemperature, FLT_EPSILON / 2.0, mFlowRate, mMass);
        CPPUNIT_ASSERT_THROW(new FriendlyMonoFluid(mProperties->getProperties(mType), initData),
            TsInitializationException);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMonoFluid::testInitializationExceptions()
{
    UT_RESULT;

    {
        /// @test initialize for exception on null pointer to properties
        MonoFluidInputData initData(mTemperature, mPressure, mFlowRate, mMass);
        FriendlyMonoFluid article;
        CPPUNIT_ASSERT_THROW(article.initialize(mProperties->getProperties(FluidProperties::NO_FLUID), initData),
            TsInitializationException);
    } {
        /// @test initialize for exception on temperature too small
        MonoFluidInputData initData(FLT_EPSILON / 2.0, mPressure, mFlowRate, mMass);
        FriendlyMonoFluid article;
        CPPUNIT_ASSERT_THROW(article.initialize(mProperties->getProperties(mType), initData), TsInitializationException);
    } {
        /// @test initialize for exception on pressure too small
        MonoFluidInputData initData(mTemperature, FLT_EPSILON / 2.0, mFlowRate, mMass);
        FriendlyMonoFluid article;
        CPPUNIT_ASSERT_THROW(article.initialize(mProperties->getProperties(mType), initData), TsInitializationException);
    }

    UT_PASS_LAST;
}
