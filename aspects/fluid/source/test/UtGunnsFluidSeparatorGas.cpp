/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/source/GunnsFluidSeparatorGas.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidSeparatorGas.hh"

/// @details  Test identification number.
int UtGunnsFluidSeparatorGas::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Separator Gas link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSeparatorGas::UtGunnsFluidSeparatorGas()
    :
    CppUnit::TestFixture(),
    tArticle(0),
    tTimeStep(0.0),
    tFluidProperties(0),
    tFluidConfig(0),
    tFluidInput0(0),
    tFluidInput1(0),
    tLinks(),
    tNodes(),
    tPort0(0),
    tPort1(0),
    tName(""),
    tNodeList(),
    tMaxConductivity(0.0),
    tGasType(FluidProperties::NO_FLUID),
    tMassExponent(0.0),
    tMaxLiquidMass(0.0),
    tReferenceSpeed(0.0),
    tReferencePressure(0.0),
    tReferenceRemovalRate(0.0),
    tPowerCurveCoefficient(0.0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tSeparatorSpeed(0.0),
    tTransferFlowRate(0.0),
    tLiquidMass(0.0),
    tInputData(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Separator Gas link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSeparatorGas::~UtGunnsFluidSeparatorGas()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsFluidSeparatorGas::setUp()
{
    /// - Define the nominal configuration data.
    tName                   = "nominal";
    tNodeList.mNumNodes     = 2;
    tNodeList.mNodes        = tNodes;
    tMaxConductivity        = 0.1;
    tGasType                = FluidProperties::GUNNS_H2O;
    tMassExponent           = 0.5;
    tMaxLiquidMass          = 10.0;
    tReferenceSpeed         = 1000.0;
    tReferencePressure      = 130.0;
    tReferenceRemovalRate   = 0.8;
    tConfigData             = new GunnsFluidSeparatorGasConfigData(tName,
                                                                   &tNodeList,
                                                                   tMaxConductivity,
                                                                   tGasType,
                                                                   tMassExponent,
                                                                   tMaxLiquidMass,
                                                                   tReferenceSpeed,
                                                                   tReferencePressure,
                                                                   tReferenceRemovalRate);
    tPowerCurveCoefficient  = tReferencePressure / tReferenceSpeed
                            / pow(tMaxLiquidMass, tMassExponent);

    /// - Define the nominal input data.
    tMalfBlockageFlag       = false;
    tMalfBlockageValue      = 0.3;
    tSeparatorSpeed         = 1000.0;
    tTransferFlowRate       = 0.3;
    tLiquidMass             = 1.5;
    tInputData              = new GunnsFluidSeparatorGasInputData(tMalfBlockageFlag,
                                                                  tMalfBlockageValue,
                                                                  tSeparatorSpeed,
                                                                  tTransferFlowRate,
                                                                  tLiquidMass);

    /// - Set up other init data.
    tPort0                  = 0;
    tPort1                  = 1;
    tTimeStep               = 0.1;

    /// - Define the nominal port fluids.
    double fractionsWet[2]  = {0.9, 0.1};
    tFluidInput0            = new PolyFluidInputData(283.0,
                                                     109.0,
                                                     0.0,
                                                     0.0,
                                                     fractionsWet);
    double fractionsDry[2]  = {1.0, 0.0};
    tFluidInput1            = new PolyFluidInputData(283.0,
                                                     111.0,
                                                     0.0,
                                                     0.0,
                                                     fractionsDry);

    tFluidProperties        = new DefinedFluidProperties();
    FluidProperties::FluidType types[2] = {FluidProperties::GUNNS_N2, FluidProperties::GUNNS_H2O};
    tFluidConfig            = new PolyFluidConfigData(tFluidProperties, types, 2);

    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", tFluidConfig);
    tNodes[1].initialize("UtNode2", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput0);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    /// - Default construct the nominal test article.
    tArticle                = new FriendlyGunnsFluidSeparatorGas;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::tearDown()
{
    /// - Clear out accumulated flows into the nodes between tests.
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tFluidConfig;
    delete tFluidProperties;
    delete tFluidInput1;
    delete tFluidInput0;
    delete tInputData;
    delete tConfigData;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model construction of configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                              == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                             == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity                   == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tGasType                           == tConfigData->mGasType);
    CPPUNIT_ASSERT(tMassExponent                      == tConfigData->mMassExponent);
    CPPUNIT_ASSERT(tMaxLiquidMass                     == tConfigData->mMaxLiquidMass);
    CPPUNIT_ASSERT(tReferenceSpeed                    == tConfigData->mReferenceSpeed);
    CPPUNIT_ASSERT(tReferencePressure                 == tConfigData->mReferencePressure);
    CPPUNIT_ASSERT(tReferenceRemovalRate              == tConfigData->mReferenceRemovalRate);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag                  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue                 == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tSeparatorSpeed                    == tInputData->mSeparatorSpeed);
    CPPUNIT_ASSERT(tTransferFlowRate                  == tInputData->mTransferFlowRate);
    CPPUNIT_ASSERT(tLiquidMass                        == tInputData->mLiquidMass);

    /// @test    Configuration data default construction.
    GunnsFluidSeparatorGasConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                  == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                  == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID          == defaultConfig.mGasType);
    CPPUNIT_ASSERT(0.0                                == defaultConfig.mMassExponent);
    CPPUNIT_ASSERT(0.0                                == defaultConfig.mMaxLiquidMass);
    CPPUNIT_ASSERT(0.0                                == defaultConfig.mReferenceSpeed);
    CPPUNIT_ASSERT(0.0                                == defaultConfig.mReferencePressure);
    CPPUNIT_ASSERT(0.0                                == defaultConfig.mReferenceRemovalRate);

    /// @test    Input data default construction.
    GunnsFluidSeparatorGasInputData defaultInput;
    CPPUNIT_ASSERT(false                              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                                == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                                == defaultInput.mSeparatorSpeed);
    CPPUNIT_ASSERT(0.0                                == defaultInput.mTransferFlowRate);
    CPPUNIT_ASSERT(0.0                                == defaultInput.mLiquidMass);

    /// @test    Configuration data copy construction.
    GunnsFluidSeparatorGasConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tConfigData->mName                 == copyConfig.mName);
    CPPUNIT_ASSERT(tConfigData->mNodeList->mNodes     == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tConfigData->mMaxConductivity      == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tConfigData->mGasType              == copyConfig.mGasType);
    CPPUNIT_ASSERT(tConfigData->mMassExponent         == copyConfig.mMassExponent);
    CPPUNIT_ASSERT(tConfigData->mMaxLiquidMass        == copyConfig.mMaxLiquidMass);
    CPPUNIT_ASSERT(tConfigData->mReferenceSpeed       == copyConfig.mReferenceSpeed);
    CPPUNIT_ASSERT(tConfigData->mReferencePressure    == copyConfig.mReferencePressure);
    CPPUNIT_ASSERT(tConfigData->mReferenceRemovalRate == copyConfig.mReferenceRemovalRate);

    /// @test    Input data copy construction.
    GunnsFluidSeparatorGasInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue     == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tInputData->mSeparatorSpeed        == copyInput.mSeparatorSpeed);
    CPPUNIT_ASSERT(tInputData->mTransferFlowRate      == copyInput.mTransferFlowRate);
    CPPUNIT_ASSERT(tInputData->mLiquidMass            == copyInput.mLiquidMass);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(""                        == tArticle->mName);
    CPPUNIT_ASSERT(0                         == tArticle->mNodes);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID == tArticle->mGasType);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMassExponent);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMaxLiquidMass);
    CPPUNIT_ASSERT(-1                        == tArticle->mGasIndex);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(false                     == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                       == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                       == tArticle->mSeparatorSpeed);
    CPPUNIT_ASSERT(0.0                       == tArticle->mTransferFlowRate);
    CPPUNIT_ASSERT(0.0                       == tArticle->mLiquidMass);

    /// @test    Default state data.
    CPPUNIT_ASSERT(0.0                       == tArticle->mLiquidMassError);
    CPPUNIT_ASSERT(0.0                       == tArticle->mLiquidDeltaP);
    CPPUNIT_ASSERT(0.0                       == tArticle->mRelativeHumidity);
    CPPUNIT_ASSERT(0.0                       == tArticle->mSeparationRate);
    CPPUNIT_ASSERT(0                         == tArticle->mSeparatedLiquid);
    CPPUNIT_ASSERT(0.0                       == tArticle->mTransferTemperature);
    CPPUNIT_ASSERT(0.0                       == tArticle->mTransferPressure);
    CPPUNIT_ASSERT(0.0                       == tArticle->mPowerCurveCoefficient);
    CPPUNIT_ASSERT(0.0                       == tArticle->mRemovalRateCoefficient);
    CPPUNIT_ASSERT(false                     == tArticle->mLiquidOverflow);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(false                     == tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidSeparatorGas* article = new GunnsFluidSeparatorGas();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                        == tArticle->mName);
    CPPUNIT_ASSERT(&tNodes[tPort0]              == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[tPort1]              == tArticle->mNodes[1]);
    CPPUNIT_ASSERT(FluidProperties::GUNNS_H2O   == tArticle->mGasType);
    CPPUNIT_ASSERT(tMassExponent                == tArticle->mMassExponent);
    CPPUNIT_ASSERT(tMaxLiquidMass               == tArticle->mMaxLiquidMass);
    const double expectedRemovalRateCoeff = tReferenceRemovalRate / tReferenceSpeed;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPowerCurveCoefficient,   tArticle->mPowerCurveCoefficient,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRemovalRateCoeff, tArticle->mRemovalRateCoefficient, DBL_EPSILON);
    CPPUNIT_ASSERT(1                            == tArticle->mGasIndex);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(false                        == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue           == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(tSeparatorSpeed              == tArticle->mSeparatorSpeed);
    CPPUNIT_ASSERT(tTransferFlowRate            == tArticle->mTransferFlowRate);
    CPPUNIT_ASSERT(tLiquidMass                  == tArticle->mLiquidMass);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0.0                          == tArticle->mLiquidMassError);
    CPPUNIT_ASSERT(0.0                          == tArticle->mLiquidDeltaP);
    CPPUNIT_ASSERT(GunnsFluidUtils::computeRelativeHumidityH2O(tArticle->mSeparatedLiquid)
                                                == tArticle->mRelativeHumidity);
    CPPUNIT_ASSERT(0.0                          == tArticle->mSeparationRate);
    CPPUNIT_ASSERT(tFluidInput0->mTemperature   == tArticle->mSeparatedLiquid->getTemperature());
    CPPUNIT_ASSERT(tFluidInput0->mTemperature   == tArticle->mTransferTemperature);
    CPPUNIT_ASSERT(tFluidInput0->mPressure      == tArticle->mTransferPressure);
    CPPUNIT_ASSERT(false                        == tArticle->mLiquidOverflow);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(true                         == tArticle->mInitFlag);

    /// @test    Verify restartModel functionality
    tArticle->mLiquidMassError  = 1.0;
    tArticle->mLiquidDeltaP     = 1.0;
    tArticle->mRelativeHumidity = 1.0;
    tArticle->mSeparationRate   = 1.0;

    tArticle->restartModel();

    CPPUNIT_ASSERT(0.0 == tArticle->mLiquidMassError);
    CPPUNIT_ASSERT(0.0 == tArticle->mLiquidDeltaP);
    CPPUNIT_ASSERT(0.0 == tArticle->mRelativeHumidity);
    CPPUNIT_ASSERT(0.0 == tArticle->mSeparationRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Test all accessor methods.
    tArticle->mTransferTemperature = 77.0;
    tArticle->mTransferFlowRate    = 10.0;
    tArticle->mTransferPressure    = 40.0;
    tArticle->mLiquidMass          = 1.0;
    tArticle->mLiquidOverflow      = true;
    CPPUNIT_ASSERT(77.0 == tArticle->getTransferTemperature());
    CPPUNIT_ASSERT(10.0 == tArticle->getTransferFlowRate());
    CPPUNIT_ASSERT(40.0 == tArticle->getTransferPressure());
    CPPUNIT_ASSERT(1.0  == tArticle->getLiquidMass());
    CPPUNIT_ASSERT(true == tArticle->getLiquidOverflow());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Nothing (this derived class does not add or override any modifiers).

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model update fluid with empty.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testUpdateFluidEmpty()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data and empty of water.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mFlowRate   = 0.0;
    tArticle->mLiquidMass = 0.0;

    tArticle->mInternalFluid->setState(tNodes[0].getOutflow());
    tArticle->updateFluid(tTimeStep, 0.0);

    const double expectedHumidity = GunnsFluidUtils::computeRelativeHumidityH2O(tNodes[0].getContent());
    const double expectedSepRate  = 0.0;
    const double expectedMass     = 0.0;
    const double expectedMassErr  = tTransferFlowRate * tTimeStep;
    const double expectedDeltaP   = 0.0;
    const double expectedSource   = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHumidity, tArticle->mRelativeHumidity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSepRate,  tArticle->mSeparationRate,   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     tArticle->mLiquidMass,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassErr,  tArticle->mLiquidMassError,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDeltaP,   tArticle->mLiquidDeltaP,     DBL_EPSILON);
    CPPUNIT_ASSERT(false == tArticle->mLiquidOverflow);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourceVector[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourceVector[1]);
    CPPUNIT_ASSERT(0.0   == tNodes[1].getInflux());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model update fluid method when there is no
///           gas flow past the separator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testUpdateFluidNoGas()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data and no air flow.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mFlowRate = DBL_EPSILON;

    tArticle->mInternalFluid->setState(tNodes[0].getOutflow());
    tArticle->updateFluid(tTimeStep, 0.0);

    const double expectedSepRate = 0.0;
    const double expectedMass    = tLiquidMass - tTransferFlowRate * tTimeStep;
    const double expectedDeltaP  = tPowerCurveCoefficient * tSeparatorSpeed
                                 * pow(expectedMass, tMassExponent);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSepRate, tArticle->mSeparationRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,    tArticle->mLiquidMass,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDeltaP,  tArticle->mLiquidDeltaP,   DBL_EPSILON);
    CPPUNIT_ASSERT(false == tArticle->mLiquidOverflow);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourceVector[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourceVector[1]);
    CPPUNIT_ASSERT(0.0   == tNodes[1].getInflux());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model update fluid method when the time step
///           is zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testUpdateFluidZeroTimeStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data and zero time step.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mFlowRate = 10.0;
    tTimeStep           = 0.0;

    tArticle->mInternalFluid->setState(tNodes[0].getOutflow());
    tArticle->updateFluid(tTimeStep, 0.0);

    const double expectedSepRate = 0.0;
    const double expectedMass    = tLiquidMass - tTransferFlowRate * tTimeStep;
    const double expectedDeltaP  = tPowerCurveCoefficient * tSeparatorSpeed
                                 * pow(expectedMass, tMassExponent);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSepRate, tArticle->mSeparationRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,    tArticle->mLiquidMass,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDeltaP,  tArticle->mLiquidDeltaP,   DBL_EPSILON);
    CPPUNIT_ASSERT(false == tArticle->mLiquidOverflow);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourceVector[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourceVector[1]);
    CPPUNIT_ASSERT(0.0   == tNodes[1].getInflux());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model update fluid method when the separator
///           has reached its liquid mass limit.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testUpdateFluidFull()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data and full of water.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mFlowRate   = 10.0;
    tArticle->mLiquidMass = tMaxLiquidMass;

    tArticle->mInternalFluid->setState(tNodes[0].getOutflow());
    tArticle->updateFluid(tTimeStep, 0.0);

    const double expectedHumidity = GunnsFluidUtils::computeRelativeHumidityH2O(tNodes[0].getContent());
    const double expectedSepRate  = tTransferFlowRate;
    const double expectedMass     = tMaxLiquidMass;
    const double expectedDeltaP   = tPowerCurveCoefficient * tSeparatorSpeed
                                  * pow(expectedMass, tMassExponent);
    const double expectedSource   = expectedSepRate / 18.0153;     // MW of H2O
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHumidity, tArticle->mRelativeHumidity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSepRate,  tArticle->mSeparationRate,   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     tArticle->mLiquidMass,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDeltaP,   tArticle->mLiquidDeltaP,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->mSourceVector[0],  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedSource,  tArticle->mSourceVector[1],  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedSepRate, tNodes[1].getInflux(),       FLT_EPSILON);
    CPPUNIT_ASSERT(true == tArticle->mLiquidOverflow);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model update fluid with dry air.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testUpdateFluidDryAir()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data and flowing from the dry
    ///   air node.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mFlowRate = 10.0;

    tArticle->mInternalFluid->setState(tNodes[1].getOutflow());
    tArticle->updateFluid(tTimeStep, 0.0);

    const double expectedHumidity = 0.0;
    const double expectedSepRate  = 0.0;
    const double expectedMass     = tLiquidMass - tTransferFlowRate * tTimeStep;
    const double expectedDeltaP   = tPowerCurveCoefficient * tSeparatorSpeed
                                  * pow(expectedMass, tMassExponent);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHumidity, tArticle->mRelativeHumidity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSepRate,  tArticle->mSeparationRate,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     tArticle->mLiquidMass,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDeltaP,   tArticle->mLiquidDeltaP,     DBL_EPSILON);
    CPPUNIT_ASSERT(false == tArticle->mLiquidOverflow);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourceVector[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mSourceVector[1]);
    CPPUNIT_ASSERT(0.0   == tNodes[1].getInflux());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testUpdateFluidNominal()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data and normal flow rates.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mFlowRate         = 0.01;
    tArticle->mLiquidMass       = 1.0;
    tArticle->mTransferFlowRate = 0.001;

    tArticle->mInternalFluid->setState(tNodes[0].getOutflow());
    tArticle->updateFluid(tTimeStep, 0.0);

    const double expectedHumidity = GunnsFluidUtils::computeRelativeHumidityH2O(tNodes[0].getContent());
    const double expectedCondense = tNodes[0].getContent()->getMassFraction(tArticle->mGasIndex)
                                  * tArticle->mFlowRate * tTimeStep;
    const double expectedSepRate  = expectedCondense / tTimeStep;
    const double expectedMass     = 1.0 + expectedCondense - tArticle->mTransferFlowRate * tTimeStep;
    const double expectedDeltaP   = tPowerCurveCoefficient * tSeparatorSpeed
                                  * pow(expectedMass, tMassExponent);
    const double expectedSource   = expectedSepRate / 18.0153;     // MW of H2O
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHumidity, tArticle->mRelativeHumidity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSepRate,  tArticle->mSeparationRate,   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     tArticle->mLiquidMass,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDeltaP,   tArticle->mLiquidDeltaP,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->mSourceVector[0],  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedSource,  tArticle->mSourceVector[1],  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedSepRate, tNodes[1].getInflux(),       FLT_EPSILON);
    CPPUNIT_ASSERT(false == tArticle->mLiquidOverflow);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model process outputs method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testProcessOutputs()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tNodes[0].collectInflux(1.0, tArticle->mInternalFluid);
    tArticle->mLiquidDeltaP = 0.01;

    tArticle->processOutputs();

    const double expectedXferP = tNodes[0].getPotential() + 0.01;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(),
                                 tArticle->mTransferTemperature, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedXferP, tArticle->mTransferPressure, FLT_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model specific port mapping rules.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testPortMapping()
{
    UT_RESULT;

    /// Deleted as the port rules have been removed from the link.

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model update fluid with reverse liquid flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testUpdateFluidReverseFlow()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data and reverse liquid flow
    ///   rate.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mFlowRate         = 0.01;
    tArticle->mLiquidMass       = 1.0;
    tArticle->mTransferFlowRate =-0.001;

    tArticle->mInternalFluid->setState(tNodes[0].getOutflow());
    tArticle->updateFluid(tTimeStep, 0.0);

    const double expectedHumidity = GunnsFluidUtils::computeRelativeHumidityH2O(tNodes[0].getContent());
    const double expectedCondense = tNodes[0].getContent()->getMassFraction(tArticle->mGasIndex)
                                  * tArticle->mFlowRate * tTimeStep;
    const double expectedSepRate  = expectedCondense / tTimeStep;
    const double expectedMass     = 1.0 + expectedCondense;
    const double expectedDeltaP   = tPowerCurveCoefficient * tSeparatorSpeed
                                  * pow(expectedMass, tMassExponent);
    const double expectedSource   = expectedSepRate / 18.0153;     // MW of H2O
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHumidity, tArticle->mRelativeHumidity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSepRate,  tArticle->mSeparationRate,   FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     tArticle->mLiquidMass,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDeltaP,   tArticle->mLiquidDeltaP,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tArticle->mSourceVector[0],  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedSource,  tArticle->mSourceVector[1],  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedSepRate, tNodes[1].getInflux(),       FLT_EPSILON);
    CPPUNIT_ASSERT(false == tArticle->mLiquidOverflow);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Separator Gas link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSeparatorGas::testInitializationExceptions()
{
    UT_RESULT;

    /// @test    Initialization exception on invalid config data: no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: gas type not in PolyFluid.
    tConfigData->mGasType = FluidProperties::GUNNS_CO2;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mGasType = tGasType;

    /// @test    Initialization exception on invalid config data: mass exponent < 0.1.
    tConfigData->mMassExponent = 0.1 - FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    /// @test    Initialization exception on invalid config data: mass exponent > 10.0.
    tConfigData->mMassExponent = 10.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMassExponent = tMassExponent;

    /// @test    Initialization exception on invalid config data: liquid mass capacity < FLT_EPSILON.
    tConfigData->mMaxLiquidMass = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxLiquidMass = tMaxLiquidMass;

    /// @test    Initialization exception on invalid config data: reference speed < FLT_EPSILON.
    tConfigData->mReferenceSpeed = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mReferenceSpeed = tReferenceSpeed;

    /// @test    Initialization exception on invalid config data: reference pressure < FLT_EPSILON.
    tConfigData->mReferencePressure = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mReferencePressure = tReferencePressure;

    /// @test    Initialization exception on invalid config data: reference liquid removal
    ///          rate < FLT_EPSILON.
    tConfigData->mReferenceRemovalRate = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mReferenceRemovalRate = tReferenceRemovalRate;

    /// @test    Initialization exception on invalid input data: mBlockage < 0.
    tInputData->mMalfBlockageValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mMalfBlockageValue = tMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: mBlockage > 1.
    tInputData->mMalfBlockageValue = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mMalfBlockageValue = tMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: separator speed < 0.
    tInputData->mSeparatorSpeed = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mSeparatorSpeed = tSeparatorSpeed;

    /// @test    Initialization exception on invalid input data: mass of liquid in separator < 0.
    tInputData->mLiquidMass = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mLiquidMass = tLiquidMass;

    UT_PASS_LAST;
}
