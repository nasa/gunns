/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/potential/GunnsGasFan.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsGasFan.hh"

/// @details  Acts as a redundant test on UnitConversion class.
const double UtGunnsGasFan::PI = 3.14159265358979;

/// @details  Test identification number.
int UtGunnsGasFan::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Gas Fan link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsGasFan::UtGunnsGasFan()
    :
    CppUnit::TestFixture(),
    tTypes(),
    tFractions(),
    tFluidProperties(0),
    tFluidConfig(0),
    tFluidInput0(0),
    tFluidInput1(0),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tMaxConductivity(0.0),
    tExpansionScaleFactor(0.0),
    tReferenceDensity(0.0),
    tReferenceSpeed(0.0),
    tReferenceCoeff0(0.0),
    tReferenceCoeff1(0.0),
    tReferenceCoeff2(0.0),
    tReferenceCoeff3(0.0),
    tReferenceCoeff4(0.0),
    tReferenceCoeff5(0.0),
    tBestEfficiency(0.0),
    tReferenceQBep(0.0),
    tFilterGain(0.0),
    tDriveRatio(0.0),
    tThermalLength(0.0),
    tThermalDiameter(0.0),
    tSurfaceRoughness(0.0),
    tCheckValveActive(false),
    tConfigData(0),
    tBlockageFlag(false),
    tBlockage(0.0),
    tSourcePressure(0.0),
    tMotorSpeed(0.0),
    tWallTemperature(0.0),
    tInputData(0),
    tArticle(0),
    tReferenceQ(0.0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Gas Fan link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsGasFan::~UtGunnsGasFan()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFan::setUp()
{
    /// - Define nominal port fluids.
    tFluidProperties      = new DefinedFluidProperties();
    tTypes[0]             = FluidProperties::GUNNS_N2;
    tTypes[1]             = FluidProperties::GUNNS_O2;
    tFractions[0]         = 0.5;
    tFractions[1]         = 0.5;
    tFluidConfig          = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tFluidInput0          = new PolyFluidInputData(283.0,
                                                   109.0,
                                                     0.0,
                                                     0.0,
                                                     tFractions);
    tFluidInput1          = new PolyFluidInputData(283.0,
                                                   111.0,
                                                     0.0,
                                                     0.0,
                                                     tFractions);

    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", tFluidConfig);
    tNodes[1].initialize("UtNode2", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput0);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    /// - Initialize the nodes list.
    tNodeList.mNodes      = tNodes;
    tNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data, roughly resembling an ISS IMV fan.  The reference
    ///   curve has a real root at q = 0.0904418832603137 m3/s, from a free online root-finding site
    ///   http://xrjunque.nom.es/precis/rootfinder.aspx
    tReferenceQ           = 0.0904418832603137;
    tName                 = "nominal";
    tMaxConductivity      = 0.0063;
    tExpansionScaleFactor = 0.5;
    tReferenceDensity     = 1.19;
    tReferenceSpeed       = 8350.0;
    tReferenceCoeff0      = 0.357;
    tReferenceCoeff1      =-24.6528;
    tReferenceCoeff2      = 1167.09;
    tReferenceCoeff3      =-21093.2;
    tReferenceCoeff4      = 168250.0;
    tReferenceCoeff5      =-549729.0;
    tBestEfficiency       = 0.420264;
    tReferenceQBep        = 0.064;
    tFilterGain           = 0.5;
    tDriveRatio           = 0.5;
    tThermalLength        = 0.1;
    tThermalDiameter      = 0.1;
    tSurfaceRoughness     = 2.1336E-6;
    tCheckValveActive     = false;
    tConfigData           = new GunnsGasFanConfigData(tName,
                                                      &tNodeList,
                                                      tMaxConductivity,
                                                      tExpansionScaleFactor,
                                                      tReferenceDensity,
                                                      tReferenceSpeed,
                                                      tReferenceCoeff0,
                                                      tReferenceCoeff1,
                                                      tReferenceCoeff2,
                                                      tReferenceCoeff3,
                                                      tReferenceCoeff4,
                                                      tReferenceCoeff5,
                                                      tBestEfficiency,
                                                      tReferenceQBep,
                                                      tFilterGain,
                                                      tDriveRatio,
                                                      tThermalLength,
                                                      tThermalDiameter,
                                                      tSurfaceRoughness,
                                                      tCheckValveActive);

    /// - Define the nominal input data
    tBlockageFlag         = true;
    tBlockage             = 0.1;
    tSourcePressure       = 1000.0;
    tMotorSpeed           = 3000.0;
    tWallTemperature      = 300.0;
    tInputData            = new GunnsGasFanInputData(tBlockageFlag,
                                                     tBlockage,
                                                     tSourcePressure,
                                                     tMotorSpeed,
                                                     tWallTemperature);

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 1;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsGasFan;

    /// - Define the nominal time step.
    tTimeStep             = 0.1;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFan::tearDown()
{
    /// - Deletes in reverse order of news in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput1;
    delete tFluidInput0;
    delete tFluidConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the construction of GUNNS Gas Fan link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFan::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                 == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity      == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tReferenceDensity     == tConfigData->mReferenceDensity);
    CPPUNIT_ASSERT(tReferenceSpeed       == tConfigData->mReferenceSpeed);
    CPPUNIT_ASSERT(tReferenceCoeff0      == tConfigData->mReferenceCoeff0);
    CPPUNIT_ASSERT(tReferenceCoeff1      == tConfigData->mReferenceCoeff1);
    CPPUNIT_ASSERT(tReferenceCoeff2      == tConfigData->mReferenceCoeff2);
    CPPUNIT_ASSERT(tReferenceCoeff3      == tConfigData->mReferenceCoeff3);
    CPPUNIT_ASSERT(tReferenceCoeff4      == tConfigData->mReferenceCoeff4);
    CPPUNIT_ASSERT(tReferenceCoeff5      == tConfigData->mReferenceCoeff5);
    CPPUNIT_ASSERT(tBestEfficiency       == tConfigData->mBestEfficiency);
    CPPUNIT_ASSERT(tReferenceQBep        == tConfigData->mReferenceQBep);
    CPPUNIT_ASSERT(tFilterGain           == tConfigData->mFilterGain);
    CPPUNIT_ASSERT(tDriveRatio           == tConfigData->mDriveRatio);
    CPPUNIT_ASSERT(tThermalLength        == tConfigData->mThermalLength);
    CPPUNIT_ASSERT(tThermalDiameter      == tConfigData->mThermalDiameter);
    CPPUNIT_ASSERT(tSurfaceRoughness     == tConfigData->mSurfaceRoughness);
    CPPUNIT_ASSERT(tCheckValveActive     == tConfigData->mCheckValveActive);

    /// @test    Configuration data default construction.
    GunnsGasFanConfigData defaultConfig;
    CPPUNIT_ASSERT(0                     == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mReferenceDensity);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mReferenceSpeed);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mReferenceCoeff0);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mReferenceCoeff1);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mReferenceCoeff2);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mReferenceCoeff3);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mReferenceCoeff4);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mReferenceCoeff5);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mBestEfficiency);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mReferenceQBep);
    CPPUNIT_ASSERT(0.5                   == defaultConfig.mFilterGain);
    CPPUNIT_ASSERT(1.0                   == defaultConfig.mDriveRatio);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mThermalLength);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mThermalDiameter);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mSurfaceRoughness);
    CPPUNIT_ASSERT(false                 == defaultConfig.mCheckValveActive);


    /// @test    Configuration data copy construction.
    GunnsGasFanConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tName                 == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity      == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tReferenceDensity     == copyConfig.mReferenceDensity);
    CPPUNIT_ASSERT(tReferenceSpeed       == copyConfig.mReferenceSpeed);
    CPPUNIT_ASSERT(tReferenceCoeff0      == copyConfig.mReferenceCoeff0);
    CPPUNIT_ASSERT(tReferenceCoeff1      == copyConfig.mReferenceCoeff1);
    CPPUNIT_ASSERT(tReferenceCoeff2      == copyConfig.mReferenceCoeff2);
    CPPUNIT_ASSERT(tReferenceCoeff3      == copyConfig.mReferenceCoeff3);
    CPPUNIT_ASSERT(tReferenceCoeff4      == copyConfig.mReferenceCoeff4);
    CPPUNIT_ASSERT(tReferenceCoeff5      == copyConfig.mReferenceCoeff5);
    CPPUNIT_ASSERT(tBestEfficiency       == copyConfig.mBestEfficiency);
    CPPUNIT_ASSERT(tReferenceQBep        == copyConfig.mReferenceQBep);
    CPPUNIT_ASSERT(tFilterGain           == copyConfig.mFilterGain);
    CPPUNIT_ASSERT(tDriveRatio           == copyConfig.mDriveRatio);
    CPPUNIT_ASSERT(tThermalLength        == copyConfig.mThermalLength);
    CPPUNIT_ASSERT(tThermalDiameter      == copyConfig.mThermalDiameter);
    CPPUNIT_ASSERT(tSurfaceRoughness     == copyConfig.mSurfaceRoughness);
    CPPUNIT_ASSERT(tCheckValveActive     == copyConfig.mCheckValveActive);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Gas Fan link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFan::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tBlockageFlag    == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage        == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tSourcePressure  == tInputData->mSourcePressure);
    CPPUNIT_ASSERT(tMotorSpeed      == tInputData->mMotorSpeed);
    CPPUNIT_ASSERT(tWallTemperature == tInputData->mWallTemperature);

    /// @test    Input data default construction.
    GunnsGasFanInputData defaultInput;
    CPPUNIT_ASSERT(false            == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0              == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0              == defaultInput.mSourcePressure);
    CPPUNIT_ASSERT(0.0              == defaultInput.mMotorSpeed);
    CPPUNIT_ASSERT(0.0              == defaultInput.mWallTemperature);

    /// @test    Input data copy construction.
    GunnsGasFanInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tBlockageFlag    == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage        == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tSourcePressure  == copyInput.mSourcePressure);
    CPPUNIT_ASSERT(tMotorSpeed      == copyInput.mMotorSpeed);
    CPPUNIT_ASSERT(tWallTemperature == copyInput.mWallTemperature);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Fan link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFan::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction of class & base class attributes.
    CPPUNIT_ASSERT(0.0 == tArticle->mSourcePressure);
    CPPUNIT_ASSERT(0.0 == tArticle->mReferenceDensity);
    CPPUNIT_ASSERT(0.0 == tArticle->mReferenceSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mReferenceCoeffs[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mReferenceCoeffs[1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mReferenceCoeffs[2]);
    CPPUNIT_ASSERT(0.0 == tArticle->mReferenceCoeffs[3]);
    CPPUNIT_ASSERT(0.0 == tArticle->mReferenceCoeffs[4]);
    CPPUNIT_ASSERT(0.0 == tArticle->mReferenceCoeffs[5]);
    CPPUNIT_ASSERT(0.0 == tArticle->mReferenceQBep);
    CPPUNIT_ASSERT(0.0 == tArticle->mFilterGain);
    CPPUNIT_ASSERT(0.0 == tArticle->mDriveRatio);
    CPPUNIT_ASSERT(0.0 == tArticle->mThermalDiameter);
    CPPUNIT_ASSERT(0.0 == tArticle->mThermalSurfaceArea);
    CPPUNIT_ASSERT(0.0 == tArticle->mThermalROverD);
    CPPUNIT_ASSERT(0.0 == tArticle->mReferenceQ);
    CPPUNIT_ASSERT(0.0 == tArticle->mReferencePowerBep);
    CPPUNIT_ASSERT(0.0 == tArticle->mMotorSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mWallTemperature);
    CPPUNIT_ASSERT(0.0 == tArticle->mWallHeatFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerTorque);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerPower);
    CPPUNIT_ASSERT(0.0 == tArticle->mSystemConstant);
    CPPUNIT_ASSERT(0.0 == tArticle->mAffinityCoeffs[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAffinityCoeffs[1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAffinityCoeffs[2]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAffinityCoeffs[3]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAffinityCoeffs[4]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAffinityCoeffs[5]);
    CPPUNIT_ASSERT(0.0 == tArticle->mSourceQ);
    CPPUNIT_ASSERT(false == tArticle->mCheckValveActive);
    CPPUNIT_ASSERT(0.0 == tArticle->mCheckValvePosition);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsGasFan* article = new GunnsGasFan();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Fan link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFan::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsGasFan article;

    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test    Base class initialization.
    CPPUNIT_ASSERT(tSourcePressure    == article.mSourcePressure);

    /// @test    Terms initialized from configuration data.
    CPPUNIT_ASSERT(tName              == article.mName);
    CPPUNIT_ASSERT(&tNodes[0]         == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]         == article.mNodes[1]);
    CPPUNIT_ASSERT(tMaxConductivity   == article.mMaxConductivity);
    CPPUNIT_ASSERT(tReferenceDensity  == article.mReferenceDensity);
    CPPUNIT_ASSERT(tReferenceSpeed    == article.mReferenceSpeed);
    CPPUNIT_ASSERT(tReferenceQBep     == article.mReferenceQBep);
    CPPUNIT_ASSERT(tFilterGain        == article.mFilterGain);
    CPPUNIT_ASSERT(tDriveRatio        == article.mDriveRatio);
    CPPUNIT_ASSERT(tThermalDiameter   == article.mThermalDiameter);
    CPPUNIT_ASSERT(tCheckValveActive   == article.mCheckValveActive);

    const double expectedArea   = tThermalLength * PI * tThermalDiameter;
    const double expectedROverD = tSurfaceRoughness / tThermalDiameter;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedArea,   article.mThermalSurfaceArea, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedROverD, article.mThermalROverD,      DBL_EPSILON);

    const double pressureBep = tReferenceCoeff0
                             + tReferenceCoeff1 * tReferenceQBep
                             + tReferenceCoeff2 * tReferenceQBep * tReferenceQBep
                             + tReferenceCoeff3 * tReferenceQBep * tReferenceQBep * tReferenceQBep
                             + tReferenceCoeff4 * tReferenceQBep * tReferenceQBep * tReferenceQBep * tReferenceQBep
                             + tReferenceCoeff5 * tReferenceQBep * tReferenceQBep * tReferenceQBep * tReferenceQBep * tReferenceQBep;
    const double expectedNs = tReferenceSpeed * UtGunnsGasFan::PI / 30.0 * sqrt(tReferenceQBep)
                            * pow(0.001 * tReferenceDensity / pressureBep, 0.75);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNs,     article.mSpecificSpeed,      1E-5);

    const double frac      = (expectedNs - 0.2) / (5.0 - 0.2);
    const double expPowCoeff0 =  0.42 + frac * ( 2.0  - 0.42);
    const double expPowCoeff1 =  0.69 + frac * (-5.98 - 0.69);
    const double expPowCoeff2 = -0.11 + frac * ( 8.78 + 0.11);
    const double expPowCoeff3 =  0.0  + frac * (-3.8  - 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expPowCoeff0,     article.mPowerCoeffs[0],     1E-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expPowCoeff1,     article.mPowerCoeffs[1],     1E-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expPowCoeff2,     article.mPowerCoeffs[2],     1E-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expPowCoeff3,     article.mPowerCoeffs[3],     1E-5);

    const double expectedPbep = 1000.0 * pressureBep * tReferenceQBep / tBestEfficiency;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPbep,   article.mReferencePowerBep,  1E-4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tReferenceQ,    article.mReferenceQ,         FLT_EPSILON);

    /// @test    Terms initialized from input data.
    CPPUNIT_ASSERT(tMotorSpeed        == article.mMotorSpeed);
    CPPUNIT_ASSERT(tWallTemperature   == article.mWallTemperature);

    /// @test    Initialized state data.
    const double expectedSysG = tReferenceQ / sqrt(tReferenceCoeff0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSysG, article.mSystemConstant, FLT_EPSILON);
    CPPUNIT_ASSERT(0.0 == article.mWallHeatFlux);
    CPPUNIT_ASSERT(0.0 == article.mImpellerTorque);
    CPPUNIT_ASSERT(0.0 == article.mImpellerSpeed);
    CPPUNIT_ASSERT(0.0 == article.mImpellerPower);
    CPPUNIT_ASSERT(0.0 == article.mAffinityCoeffs[0]);
    CPPUNIT_ASSERT(0.0 == article.mAffinityCoeffs[1]);
    CPPUNIT_ASSERT(0.0 == article.mAffinityCoeffs[2]);
    CPPUNIT_ASSERT(0.0 == article.mAffinityCoeffs[3]);
    CPPUNIT_ASSERT(0.0 == article.mAffinityCoeffs[4]);
    CPPUNIT_ASSERT(0.0 == article.mAffinityCoeffs[5]);
    CPPUNIT_ASSERT(0.0 == article.mSourceQ);
    CPPUNIT_ASSERT(0.0 == article.mCheckValvePosition);

    /// @test    Internal fluid initialization.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getOutflow()->getTemperature(),
                                 article.getInternalFluid()->getTemperature(), DBL_EPSILON);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// @test    Initialize with reference coeffs turned off to test curve generation
    tConfigData->mReferenceCoeff1 = 0.0;
    tConfigData->mReferenceCoeff1 = tConfigData->mReferenceCoeff2 = tConfigData->mReferenceCoeff3 =
            tConfigData->mReferenceCoeff4 = tConfigData->mReferenceCoeff5 = 0.0;
    tConfigData->mReferenceCoeff0 = 0.262665;

    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));
    CPPUNIT_ASSERT(article.mInitFlag);
    const double expCurveCoeff0 =  (1.09 + frac * (1.69   - 1.09))*(0.262665 / pow(tReferenceQBep, 0.0));
    const double expCurveCoeff1 =  (0.33 + frac * (-5.45  - 0.33))*(0.262665 / pow(tReferenceQBep, 1.0));
    const double expCurveCoeff2 = (-0.59 + frac * (9.62   + 0.59))*(0.262665 / pow(tReferenceQBep, 2.0));
    const double expCurveCoeff3 = (-0.39 + frac * (-4.88  + 0.39))*(0.262665 / pow(tReferenceQBep, 3.0));
    const double expCurveCoeff4 =  (1.32 + frac * (0.022  - 1.32))*(0.262665 / pow(tReferenceQBep, 4.0));
    const double expCurveCoeff5 = (-0.76 + frac * (-0.013 + 0.76))*(0.262665 / pow(tReferenceQBep, 5.0));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expCurveCoeff0, article.mReferenceCoeffs[0], 1E-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expCurveCoeff1, article.mReferenceCoeffs[1], 1E-4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expCurveCoeff2, article.mReferenceCoeffs[2], 1E-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expCurveCoeff3, article.mReferenceCoeffs[3], 1E-2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expCurveCoeff4, article.mReferenceCoeffs[4], 1E-1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expCurveCoeff5, article.mReferenceCoeffs[5], 1E-1);

    /// - Initialize another test article with initialization data to exercise alternate logic paths
    ///   in the initialize method.
    FriendlyGunnsGasFan article2;

    tConfigData->mThermalLength = 0.0;
    CPPUNIT_ASSERT_NO_THROW(article2.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article2.mThermalSurfaceArea, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article2.mThermalROverD,      DBL_EPSILON);
    CPPUNIT_ASSERT(article2.mInitFlag);

    // - Verify restartModel Functionality
    article2.mImpellerSpeed      = 1.0;
    article2.mImpellerPower      = 1.0;
    article2.mAffinityCoeffs[0]  = 1.0;
    article2.mAffinityCoeffs[1]  = 1.0;
    article2.mAffinityCoeffs[2]  = 1.0;
    article2.mAffinityCoeffs[3]  = 1.0;
    article2.mAffinityCoeffs[4]  = 1.0;
    article2.mAffinityCoeffs[5]  = 1.0;

    article2.restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article2.mImpellerSpeed,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article2.mImpellerPower,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article2.mAffinityCoeffs[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article2.mAffinityCoeffs[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article2.mAffinityCoeffs[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article2.mAffinityCoeffs[3], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article2.mAffinityCoeffs[4], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article2.mAffinityCoeffs[5], DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Fan link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFan::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    FriendlyGunnsGasFan article;

    /// @test    Initialization exception (from the base class) on no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: max conductivity < 0.
    tConfigData->mMaxConductivity = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxConductivity = tMaxConductivity;

    /// @test    Initialization exception on invalid config data: expansion scale factor < 0.
    tConfigData->mExpansionScaleFactor = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mExpansionScaleFactor = tExpansionScaleFactor;

    /// @test    Initialization exception on invalid config data: no reference curve root.
    tConfigData->mReferenceCoeff0 = 1.0;
    tConfigData->mReferenceCoeff1 = 0.0;
    tConfigData->mReferenceCoeff2 = 0.0;
    tConfigData->mReferenceCoeff3 = 0.0;
    tConfigData->mReferenceCoeff4 = 0.0;
    tConfigData->mReferenceCoeff5 = 0.0;
    tConfigData->mBestEfficiency  = 0.0;
    tConfigData->mReferenceQBep   = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mReferenceCoeff0 = tReferenceCoeff0;
    tConfigData->mReferenceCoeff1 = tReferenceCoeff1;
    tConfigData->mReferenceCoeff2 = tReferenceCoeff2;
    tConfigData->mReferenceCoeff3 = tReferenceCoeff3;
    tConfigData->mReferenceCoeff4 = tReferenceCoeff4;
    tConfigData->mReferenceCoeff5 = tReferenceCoeff5;
    tConfigData->mBestEfficiency  = tBestEfficiency;
    tConfigData->mReferenceQBep   = tReferenceQBep;

    /// @test    Initialization exception on invalid config data: reference density too small.
    tConfigData->mReferenceDensity = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mReferenceDensity = tReferenceDensity;

    /// @test    Initialization exception on invalid config data: reference speed too small.
    tConfigData->mReferenceSpeed = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mReferenceSpeed = tReferenceSpeed;

    /// @test    Initialization exception on invalid config data: drive ratio too small.
    tConfigData->mDriveRatio = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mDriveRatio = tDriveRatio;

    /// @test    Initialization exception on invalid config data: negative dead-head pressure.
    tConfigData->mReferenceCoeff0 =-1.0;
    tConfigData->mReferenceCoeff1 = 1.0;
    tConfigData->mReferenceCoeff2 = 0.0;
    tConfigData->mReferenceCoeff3 = 0.0;
    tConfigData->mReferenceCoeff4 = 0.0;
    tConfigData->mReferenceCoeff5 = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mReferenceCoeff0 = tReferenceCoeff0;
    tConfigData->mReferenceCoeff1 = tReferenceCoeff1;
    tConfigData->mReferenceCoeff2 = tReferenceCoeff2;
    tConfigData->mReferenceCoeff3 = tReferenceCoeff3;
    tConfigData->mReferenceCoeff4 = tReferenceCoeff4;
    tConfigData->mReferenceCoeff5 = tReferenceCoeff5;

    /// @test    Initialization exception on invalid config data: mReferenceQ < mReferenceQBep.
    tConfigData->mBestEfficiency  = 0.55;
    tConfigData->mReferenceQBep   = 1.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    /// @test    Initialization exception on invalid config data: best efficiency > 1.
    tConfigData->mBestEfficiency  = 1.01;
    tConfigData->mReferenceQBep   = 0.064;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mBestEfficiency  = 0.0;
    tConfigData->mReferenceQBep   = 0.0;

    /// @test    Initialization exception on invalid input data: mBlockage < 0.
    tInputData->mMalfBlockageValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mMalfBlockageValue = tBlockage;

    /// @test    Initialization exception on invalid input data: motor speed < 0.
    tInputData->mMotorSpeed = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mMotorSpeed = tMotorSpeed;

    /// @test    Initialization exception on invalid input data: wall temperature < 0.
    tInputData->mWallTemperature = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Fan link model getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFan::testAccessors()
{
    UT_RESULT;

    /// @test    The getWallHeatFlux method.
    tArticle->mWallHeatFlux = 1.0;
    CPPUNIT_ASSERT( 1.0 == tArticle->getWallHeatFlux());

    /// @test    The getImpellerTorque method.
    tArticle->mImpellerTorque = -0.1;
    CPPUNIT_ASSERT(-0.1 == tArticle->getImpellerTorque());

    /// @test    The getImpellerSpeed method.
    tArticle->mImpellerSpeed = 10.0;
    CPPUNIT_ASSERT(10.0 == tArticle->getImpellerSpeed());

    /// @test    The getImpellerPower method.
    tArticle->mImpellerPower = 5.0;
    CPPUNIT_ASSERT( 5.0 == tArticle->getImpellerPower());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Fan link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFan::testModifiers()
{
    UT_RESULT;

    /// @test    The thermal surface area setter with good value.
    tArticle->setThermalSurfaceArea(0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, tArticle->mThermalSurfaceArea, 0.0);

    /// @test    The thermal surface area setter with out of range value.
    tArticle->setThermalSurfaceArea(-0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mThermalSurfaceArea, 0.0);

    /// @test    The wall temperature setter with good value.
    tArticle->setWallTemperature(280.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(280.0, tArticle->mWallTemperature, 0.0);

    /// @test    The wall temperature setter with out of range value.
    tArticle->setWallTemperature(-0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mWallTemperature, 0.0);

    // @test     The motor speed setter with good value.
    tArticle->mMotorSpeed = 0.0;
    tArticle->setMotorSpeed(1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,tArticle->mMotorSpeed,0.0);

    /// @test    The wall temperature setter with out of range value.
    tArticle->setCheckValveFlag(true);
    CPPUNIT_ASSERT(tArticle->mCheckValveActive);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Fan link model update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFan::testUpdateState()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Zero source pressure when drive ratio and impeller speed are zero.
    const double startGsys = tArticle->mSystemConstant;
    tArticle->mDriveRatio = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mSourcePressure);

    /// @test    Outputs when check valve is enabled, and pressure differential is negative
    tArticle->mCheckValveActive = true;
    tArticle->updateState(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mCheckValvePosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mEffectiveConductivity, DBL_EPSILON);
    tArticle->mCheckValveActive = false;

    /// @test    Outputs under normal running condition.  Expected Q (flow rate) value is derived
    ///          from the expected affinity coefficients, expected system constant, using the root
    ///          finder at: https://www.wolframalpha.com.
    tArticle->mDriveRatio     = tDriveRatio;
    tArticle->mVolFlowRate    = 0.06;  // (m3/s), ~140 cfm
    tArticle->mSourcePressure = 0.25;  // (kPa)
    tArticle->updateState(tTimeStep);

    double expectedImpellerSpeed = tMotorSpeed / tDriveRatio;
    double sourceDensity         = tNodes[0].getOutflow()->getDensity();
    double expectedDensityFactor = sourceDensity / tReferenceDensity;
    double expectedSpeedFactor   = expectedImpellerSpeed / tReferenceSpeed;
    double expectedCoeff0        = tReferenceCoeff0 * expectedDensityFactor *
                                   expectedSpeedFactor * expectedSpeedFactor;
    double expectedCoeff2        = tReferenceCoeff2 * expectedDensityFactor;
    double expectedGsys          = std::max(0.06, tReferenceQ * expectedSpeedFactor * 0.0001)
                                 / sqrt(std::min(expectedCoeff0, 0.25));
    double expectedSystemConst   = tFilterGain * expectedGsys + (1.0 - tFilterGain) * startGsys;
    double expectedSourceQ       = 0.051262960458395;
    double expectedSourceP       = tArticle->mAffinityCoeffs[0]
                                 + tArticle->mAffinityCoeffs[1] * pow(expectedSourceQ, 1.0)
                                 + tArticle->mAffinityCoeffs[2] * pow(expectedSourceQ, 2.0)
                                 + tArticle->mAffinityCoeffs[3] * pow(expectedSourceQ, 3.0)
                                 + tArticle->mAffinityCoeffs[4] * pow(expectedSourceQ, 4.0)
                                 + tArticle->mAffinityCoeffs[5] * pow(expectedSourceQ, 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImpellerSpeed, tArticle->mImpellerSpeed,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCoeff0,        tArticle->mAffinityCoeffs[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCoeff2,        tArticle->mAffinityCoeffs[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSystemConst,   tArticle->mSystemConstant,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSourceQ,       tArticle->mSourceQ,           FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSourceP,       tArticle->mSourcePressure,    0.000001);

    /// @test    Outputs at free-flow condition (max flow rate, zero pressure).
    tArticle->mFilterGain     = 0.0;
    tArticle->mSystemConstant = 0.0;
    tArticle->updateState(tTimeStep);
    expectedSourceQ           = tReferenceQ * expectedSpeedFactor;
    expectedSourceP           = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSourceQ,       tArticle->mSourceQ,           FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSourceP,       tArticle->mSourcePressure,    0.000001);

    /// @test    Outputs at dead-head condition (zero flow rate, max pressure).
    tArticle->mFilterGain     = 1.0;
    tArticle->mVolFlowRate    = 0.0;
    tArticle->mSourcePressure = 100.0;
    tArticle->updateState(tTimeStep);
    expectedSourceQ           = 0.0;
    expectedSourceP           = expectedCoeff0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSourceQ,       tArticle->mSourceQ,           0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSourceP,       tArticle->mSourcePressure,    0.01);

    /// @test    Outputs when fluid density near zero.
    tNodes[0].getContent()->setPressure(FLT_EPSILON * 0.1);
    tNodes[0].resetFlows();
    tArticle->mVolFlowRate    = 0.06;
    tArticle->updateState(tTimeStep);
    expectedSourceP           = 0.0;
    CPPUNIT_ASSERT(expectedSourceP == tArticle->mSourcePressure);

    /// @test   Outflow condition variance leading to 0.0 mCheckValvePosition
    tNodes[0].getContent()->setPressure(10.0);
    tNodes[1].getContent()->setPressure(200.0);
    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[0].getContent()->setPressure(109.0);
    tNodes[1].getContent()->setPressure(111.0);
    tArticle->mCheckValveActive = true;
    tArticle->updateState(tTimeStep);

    CPPUNIT_ASSERT(0.0 == tArticle->mCheckValvePosition);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Impeller link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFan::testUpdateFluid()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test   Update fluid with flowrate too small.
    tArticle->updateFluid(tTimeStep, 0.5 * DBL_EPSILON);
    CPPUNIT_ASSERT(0.0 == tArticle->mWallHeatFlux);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getOutflow()->getTemperature(),
                                 tArticle->mInternalFluid->getTemperature(), DBL_EPSILON);

    /// @test   Update fluid with nominal time step, flowrate and temperature.
    tArticle->updateFluid(tTimeStep, 0.01);
    CPPUNIT_ASSERT(tArticle->mWallHeatFlux < 0.0);
    CPPUNIT_ASSERT(tArticle->mInternalFluid->getTemperature() > 283.0);

    /// @test   Impeller power & torque at speed.
    tArticle->mVolFlowRate    = 0.06;
    tArticle->mSourcePressure = 0.25;
    tArticle->mMotorSpeed     = tMotorSpeed;
    /// - Source pressure converted from kPa to Pa to relate power in Watts.
    double expectedPower      = 1000.0 * 0.06 * 0.25;
    /// - Motor speed converted from rpm to r/s to relate torque in N*m.
    double expectedTorque     = -expectedPower / tMotorSpeed * 60.0 / 2.0 / PI;
    tArticle->updateFluid(tTimeStep, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,  tArticle->mImpellerPower,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTorque, tArticle->mImpellerTorque, DBL_EPSILON);

    /// @test   Impeller torque with zero drive ratio.
    tArticle->mDriveRatio     = 0.0;
    expectedTorque            = 0.0;
    tArticle->updateFluid(tTimeStep, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTorque, tArticle->mImpellerTorque, DBL_EPSILON);

    /// @test   Impeller power & torque at zero speed.
    tArticle->mMotorSpeed     = 0.0;
    tArticle->mSourcePressure = 0.0;
    expectedPower             = 0.0;
    tArticle->updateFluid(tTimeStep, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,  tArticle->mImpellerPower,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTorque, tArticle->mImpellerTorque, DBL_EPSILON);

    /// @test   Impeller shaft power for configured specific speed.
    FriendlyGunnsGasFan article2;
    tBestEfficiency              = 0.420264;
    tReferenceQBep               = 0.064;
    tConfigData->mBestEfficiency = tBestEfficiency;
    tConfigData->mReferenceQBep  = tReferenceQBep;
    article2.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tMotorSpeed = 9000.0;
    article2.mVolFlowRate    = 0.06;
    article2.mSourcePressure = 0.25;
    article2.mImpellerSpeed  = tMotorSpeed;
    article2.updateFluid(tTimeStep, 0.01);

    const double speedRatio    = tMotorSpeed / tReferenceSpeed;
    const double affinityQ     = tReferenceQBep * speedRatio;
    const double densityFactor = tNodes[0].getOutflow()->getDensity() / tReferenceDensity;
    const double affinityP     = speedRatio * speedRatio * speedRatio
                               * article2.mReferencePowerBep * densityFactor;
    const double QQbep         = article2.mVolFlowRate / affinityQ;
    expectedPower              = affinityP * (article2.mPowerCoeffs[0]
                                            + article2.mPowerCoeffs[1] * QQbep
                                            + article2.mPowerCoeffs[2] * QQbep * QQbep
                                            + article2.mPowerCoeffs[3] * QQbep * QQbep * QQbep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,  article2.mImpellerPower,   DBL_EPSILON);

    UT_PASS_LAST;
}
