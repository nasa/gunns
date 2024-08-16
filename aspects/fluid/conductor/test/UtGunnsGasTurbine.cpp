/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsGasTurbine.o))
***************************************************************************************************/

#include "UtGunnsGasTurbine.hh"

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"


/// @details  Acts as a redundant test on UnitConversion class.
const double UtGunnsGasTurbine::PI = 3.14159265358979;

/// @details  Test identification number.
int UtGunnsGasTurbine::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Gas Turbine link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsGasTurbine::UtGunnsGasTurbine()
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
    tReferenceTemp(0.0),
    tReferencePress(0.0),
    tMaxFlowLowSpeed(0.0),
    tLowSpeedCoeff1(0.0),
    tLowSpeedCoeff2(0.0),
    tEffCoeffLowSpeed0(0.0),
    tEffCoeffLowSpeed1(0.0),
    tEffCoeffLowSpeed2(0.0),
    tEffCoeffLowSpeed3(0.0),
    tEffCoeffLowSpeed4(0.0),
    tEffCoeffLowSpeed5(0.0),
    tMaxFlowHighSpeed(0.0),
    tHighSpeedCoeff1(0.0),
    tHighSpeedCoeff2(0.0),
    tEffCoeffHighSpeed0(0.0),
    tEffCoeffHighSpeed1(0.0),
    tEffCoeffHighSpeed2(0.0),
    tEffCoeffHighSpeed3(0.0),
    tEffCoeffHighSpeed4(0.0),
    tEffCoeffHighSpeed5(0.0),
    tCorrectedSpeedLow(0.0),
    tCorrectedSpeedHigh(0.0),
    tMinEffLimitLowSpeed(0.0),
    tMinEffLimitHighSpeed(0.0),
    tMaxEffLimitLowSpeed(0.0),
    tMaxEffLimitHighSpeed(0.0),
    tDriveRatio(0.0),
    tFilterGain(0.0),
    tThermalLength(0.0),
    tThermalDiameter(0.0),
    tSurfaceRoughness(0.0),
    tConfigData(0),
    tBlockageFlag(false),
    tBlockage(0.0),
    tMotorSpeed(0.0),
    tWallTemperature(0.0),
    tInputData(0),
    tArticle(0),
    tChokedMassFlow(0.0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Gas Turbine link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsGasTurbine::~UtGunnsGasTurbine()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasTurbine::setUp()
{
    /// - Define nominal port fluids.
    tFluidProperties      = new DefinedFluidProperties();
    tTypes[0]             = FluidProperties::GUNNS_N2;
    tTypes[1]             = FluidProperties::GUNNS_O2;
    tFractions[0]         = 0.5;
    tFractions[1]         = 0.5;
    tFluidConfig          = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tFluidInput0          = new PolyFluidInputData(283.0,
                                                   150.0,
                                                     0.0,
                                                     0.0,
                                                     tFractions);
    tFluidInput1          = new PolyFluidInputData(283.0,
                                                   100.0,
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

    /// - Define the nominal configuration data, roughly resembling an AGARD two-stage turbine.
    tName                 = "nominal";
    tMaxConductivity      = 0.0063;
    tExpansionScaleFactor = 0.5;
    tReferenceTemp        = 273.0 ;
    tReferencePress       = 101.0 ;
    tMaxFlowLowSpeed      = 105 ;
    tLowSpeedCoeff1       = 2.537 ;
    tLowSpeedCoeff2       = -0.642 ;
    tEffCoeffLowSpeed0    = -16.9164 ;
    tEffCoeffLowSpeed1    = 44.3967 ;
    tEffCoeffLowSpeed2    = -42.75194 ;
    tEffCoeffLowSpeed3    = 19.90958 ;
    tEffCoeffLowSpeed4    = -4.4991 ;
    tEffCoeffLowSpeed5    = 0.3946;
    tMaxFlowHighSpeed     = 105 ;
    tHighSpeedCoeff1      = 2.5 ;
    tHighSpeedCoeff2      = -0.5 ;
    tEffCoeffHighSpeed0   = -1.6559 ;
    tEffCoeffHighSpeed1   = 3.00823 ;
    tEffCoeffHighSpeed2   = -1.321 ;
    tEffCoeffHighSpeed3   = 0.2659 ;
    tEffCoeffHighSpeed4   = -0.02223 ;
    tEffCoeffHighSpeed5   = 0.000347 ;
    tCorrectedSpeedLow    = 0.6*6000;
    tCorrectedSpeedHigh   = 1.2*6000;
    tMinEffLimitLowSpeed  = 1.0;
    tMinEffLimitHighSpeed = 1.0;
    tMaxEffLimitLowSpeed  = 2.8;
    tMaxEffLimitHighSpeed = 5.6;
    tDriveRatio           = 0.5;
    tFilterGain           = 0.5;
    tThermalLength        = 0.1;
    tThermalDiameter      = 0.1;
    tSurfaceRoughness     = 2.1336E-6;
    tConfigData           = new GunnsGasTurbineConfigData(tName,
                                                      &tNodeList,
                                                      tMaxConductivity,
                                                      tExpansionScaleFactor,
                                                      tReferenceTemp,
                                                      tReferencePress,
                                                      tMaxFlowLowSpeed,
                                                      tMaxFlowHighSpeed,
                                                      tLowSpeedCoeff1,
                                                      tHighSpeedCoeff1,
                                                      tLowSpeedCoeff2,
                                                      tHighSpeedCoeff2,
                                                      tEffCoeffLowSpeed0,
                                                      tEffCoeffLowSpeed1,
                                                      tEffCoeffLowSpeed2,
                                                      tEffCoeffLowSpeed3,
                                                      tEffCoeffLowSpeed4,
                                                      tEffCoeffLowSpeed5,
                                                      tEffCoeffHighSpeed0,
                                                      tEffCoeffHighSpeed1,
                                                      tEffCoeffHighSpeed2,
                                                      tEffCoeffHighSpeed3,
                                                      tEffCoeffHighSpeed4,
                                                      tEffCoeffHighSpeed5,
                                                      tMinEffLimitLowSpeed,
                                                      tMinEffLimitLowSpeed,
                                                      tMaxEffLimitLowSpeed,
                                                      tMaxEffLimitHighSpeed,
                                                      tCorrectedSpeedLow,
                                                      tCorrectedSpeedHigh,
                                                      tFilterGain,
                                                      tDriveRatio,
                                                      tThermalLength,
                                                      tThermalDiameter,
                                                      tSurfaceRoughness);
    /// - Define the nominal input data
    tBlockageFlag         = true;
    tBlockage             = 0.1;
    tMotorSpeed           = 3000.0;
    tWallTemperature      = 300.0;
    tInputData            = new GunnsGasTurbineInputData(tBlockageFlag,
                                                         tBlockage,
                                                         tMotorSpeed,
                                                         tWallTemperature);

    /// - Define the nominal port mapping.
    tPort0                = 0;
    tPort1                = 1;

    /// - Default construct the nominal test article.
    tArticle              = new FriendlyGunnsGasTurbine;

    /// - Define the nominal time step.
    tTimeStep             = 0.1;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasTurbine::tearDown()
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
/// @details  Tests the construction of GUNNS Gas Turbine link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasTurbine::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                 == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity      == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tReferenceTemp        == tConfigData->mReferenceTemp);
    CPPUNIT_ASSERT(tReferencePress       == tConfigData->mReferencePress);
    CPPUNIT_ASSERT(tMaxFlowLowSpeed      == tConfigData->mMaxFlowLowSpeed);
    CPPUNIT_ASSERT(tLowSpeedCoeff1       == tConfigData->mCoeffLowSpeed1);
    CPPUNIT_ASSERT(tLowSpeedCoeff2       == tConfigData->mCoeffLowSpeed2);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed0    == tConfigData->mEffCoeffLowSpeed0);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed1    == tConfigData->mEffCoeffLowSpeed1);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed2    == tConfigData->mEffCoeffLowSpeed2);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed3    == tConfigData->mEffCoeffLowSpeed3);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed4    == tConfigData->mEffCoeffLowSpeed4);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed5    == tConfigData->mEffCoeffLowSpeed5);
    CPPUNIT_ASSERT(tMaxFlowHighSpeed     == tConfigData->mMaxFlowHighSpeed);
    CPPUNIT_ASSERT(tHighSpeedCoeff1      == tConfigData->mCoeffHighSpeed1);
    CPPUNIT_ASSERT(tHighSpeedCoeff2      == tConfigData->mCoeffHighSpeed2);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed0   == tConfigData->mEffCoeffHighSpeed0);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed1   == tConfigData->mEffCoeffHighSpeed1);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed2   == tConfigData->mEffCoeffHighSpeed2);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed3   == tConfigData->mEffCoeffHighSpeed3);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed4   == tConfigData->mEffCoeffHighSpeed4);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed5   == tConfigData->mEffCoeffHighSpeed5);
    CPPUNIT_ASSERT(tMinEffLimitLowSpeed  == tConfigData->mMinEffLimLowSpeed);
    CPPUNIT_ASSERT(tMinEffLimitHighSpeed == tConfigData->mMinEffLimHighSpeed);
    CPPUNIT_ASSERT(tMaxEffLimitLowSpeed  == tConfigData->mMaxEffLimLowSpeed);
    CPPUNIT_ASSERT(tMaxEffLimitHighSpeed == tConfigData->mMaxEffLimHighSpeed);
    CPPUNIT_ASSERT(tCorrectedSpeedLow    == tConfigData->mCorrectedSpeedLow);
    CPPUNIT_ASSERT(tCorrectedSpeedHigh   == tConfigData->mCorrectedSpeedHigh);
    CPPUNIT_ASSERT(tDriveRatio           == tConfigData->mDriveRatio);
    CPPUNIT_ASSERT(tThermalLength        == tConfigData->mThermalLength);
    CPPUNIT_ASSERT(tThermalDiameter      == tConfigData->mThermalDiameter);
    CPPUNIT_ASSERT(tSurfaceRoughness     == tConfigData->mSurfaceRoughness);

    /// @test    Configuration data default construction.
    GunnsGasTurbineConfigData defaultConfig;
    CPPUNIT_ASSERT(0                     == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mReferenceTemp);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mReferencePress);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mMaxFlowLowSpeed);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mCoeffLowSpeed1);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mCoeffLowSpeed2);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mEffCoeffLowSpeed0);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mEffCoeffLowSpeed1);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mEffCoeffLowSpeed2);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mEffCoeffLowSpeed3);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mEffCoeffLowSpeed4);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mEffCoeffLowSpeed5);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mMaxFlowHighSpeed);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mCoeffHighSpeed1);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mCoeffHighSpeed2);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mEffCoeffHighSpeed0);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mEffCoeffHighSpeed1);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mEffCoeffHighSpeed2);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mEffCoeffHighSpeed3);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mEffCoeffHighSpeed4);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mEffCoeffHighSpeed5);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mMinEffLimLowSpeed);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mMinEffLimHighSpeed);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mMaxEffLimLowSpeed);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mMaxEffLimHighSpeed);
    CPPUNIT_ASSERT(1.0                   == defaultConfig.mDriveRatio);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mThermalLength);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mThermalDiameter);
    CPPUNIT_ASSERT(0.0                   == defaultConfig.mSurfaceRoughness);

    /// @test    Configuration data copy construction.
    GunnsGasTurbineConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tName                 == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity      == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tReferenceTemp        == copyConfig.mReferenceTemp);
    CPPUNIT_ASSERT(tReferencePress       == copyConfig.mReferencePress);
    CPPUNIT_ASSERT(tMaxFlowLowSpeed      == copyConfig.mMaxFlowLowSpeed);
    CPPUNIT_ASSERT(tLowSpeedCoeff1       == copyConfig.mCoeffLowSpeed1);
    CPPUNIT_ASSERT(tLowSpeedCoeff2       == copyConfig.mCoeffLowSpeed2);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed0    == copyConfig.mEffCoeffLowSpeed0);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed1    == copyConfig.mEffCoeffLowSpeed1);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed2    == copyConfig.mEffCoeffLowSpeed2);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed3    == copyConfig.mEffCoeffLowSpeed3);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed4    == copyConfig.mEffCoeffLowSpeed4);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed5    == copyConfig.mEffCoeffLowSpeed5);
    CPPUNIT_ASSERT(tMaxFlowHighSpeed     == copyConfig.mMaxFlowHighSpeed);
    CPPUNIT_ASSERT(tHighSpeedCoeff1      == copyConfig.mCoeffHighSpeed1);
    CPPUNIT_ASSERT(tHighSpeedCoeff2      == copyConfig.mCoeffHighSpeed2);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed0   == copyConfig.mEffCoeffHighSpeed0);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed1   == copyConfig.mEffCoeffHighSpeed1);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed2   == copyConfig.mEffCoeffHighSpeed2);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed3   == copyConfig.mEffCoeffHighSpeed3);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed4   == copyConfig.mEffCoeffHighSpeed4);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed5   == copyConfig.mEffCoeffHighSpeed5);
    CPPUNIT_ASSERT(tMinEffLimitLowSpeed  == copyConfig.mMinEffLimLowSpeed);
    CPPUNIT_ASSERT(tMinEffLimitHighSpeed == copyConfig.mMinEffLimHighSpeed);
    CPPUNIT_ASSERT(tMaxEffLimitLowSpeed  == copyConfig.mMaxEffLimLowSpeed);
    CPPUNIT_ASSERT(tMaxEffLimitHighSpeed == copyConfig.mMaxEffLimHighSpeed);
    CPPUNIT_ASSERT(tCorrectedSpeedLow    == copyConfig.mCorrectedSpeedLow);
    CPPUNIT_ASSERT(tCorrectedSpeedHigh   == copyConfig.mCorrectedSpeedHigh);
    CPPUNIT_ASSERT(tDriveRatio           == copyConfig.mDriveRatio);
    CPPUNIT_ASSERT(tThermalLength        == copyConfig.mThermalLength);
    CPPUNIT_ASSERT(tThermalDiameter      == copyConfig.mThermalDiameter);
    CPPUNIT_ASSERT(tSurfaceRoughness     == copyConfig.mSurfaceRoughness);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Gas Turbine link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasTurbine::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tBlockageFlag    == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage        == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tMotorSpeed      == tInputData->mMotorSpeed);
    CPPUNIT_ASSERT(tWallTemperature == tInputData->mWallTemperature);

    /// @test    Input data default construction.
    GunnsGasTurbineInputData defaultInput;
    CPPUNIT_ASSERT(false            == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0              == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0              == defaultInput.mMotorSpeed);
    CPPUNIT_ASSERT(0.0              == defaultInput.mWallTemperature);

    /// @test    Input data copy construction.
    GunnsGasTurbineInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tBlockageFlag    == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage        == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tMotorSpeed      == copyInput.mMotorSpeed);
    CPPUNIT_ASSERT(tWallTemperature == copyInput.mWallTemperature);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Turbine link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasTurbine::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction of class & base class attributes.
    CPPUNIT_ASSERT(0.0 == tArticle->mReferenceTemp);
    CPPUNIT_ASSERT(0.0 == tArticle->mReferencePress);
    CPPUNIT_ASSERT(0.0 == tArticle->mCorrectedSpeedLow);
    CPPUNIT_ASSERT(0.0 == tArticle->mCorrectedSpeedHigh);
    CPPUNIT_ASSERT(0.0 == tArticle->mLowSpeedMaxFlow);
    CPPUNIT_ASSERT(0.0 == tArticle->mCoeffLowSpeed1);
    CPPUNIT_ASSERT(0.0 == tArticle->mCoeffHighSpeed1);
    CPPUNIT_ASSERT(0.0 == tArticle->mHighSpeedMaxFlow);
    CPPUNIT_ASSERT(0.0 == tArticle->mCoeffLowSpeed2);
    CPPUNIT_ASSERT(0.0 == tArticle->mCoeffHighSpeed2);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffCoeffLowSpeed[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffCoeffLowSpeed[1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffCoeffLowSpeed[2]);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffCoeffLowSpeed[3]);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffCoeffLowSpeed[4]);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffCoeffLowSpeed[5]);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffCoeffHighSpeed[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffCoeffHighSpeed[1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffCoeffHighSpeed[2]);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffCoeffHighSpeed[3]);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffCoeffHighSpeed[4]);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffCoeffHighSpeed[5]);
    CPPUNIT_ASSERT(0.0 == tArticle->mMinEffLimLowSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mMinEffLimHighSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mMaxEffLimLowSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mMaxEffLimHighSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mEfficiency);
    CPPUNIT_ASSERT(0.0 == tArticle->mFilterGain);
    CPPUNIT_ASSERT(0.0 == tArticle->mDriveRatio);
    CPPUNIT_ASSERT(0.0 == tArticle->mThermalDiameter);
    CPPUNIT_ASSERT(0.0 == tArticle->mThermalSurfaceArea);
    CPPUNIT_ASSERT(0.0 == tArticle->mThermalROverD);
    CPPUNIT_ASSERT(0.0 == tArticle->mMotorSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mWallTemperature);
    CPPUNIT_ASSERT(0.0 == tArticle->mWallHeatFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerTorque);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerSpeed);
    CPPUNIT_ASSERT(0.0 == tArticle->mImpellerPower);
    CPPUNIT_ASSERT(0.0 == tArticle->mPressureDrop);
    CPPUNIT_ASSERT(0.0 == tArticle->mPredictedFlowRate);


    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsGasTurbine* article = new GunnsGasTurbine();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Turbine link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasTurbine::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsGasTurbine article;

    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    /// @test    Terms initialized from configuration data.
    CPPUNIT_ASSERT(tName                == article.mName);
    CPPUNIT_ASSERT(&tNodes[0]           == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]           == article.mNodes[1]);
    CPPUNIT_ASSERT(tMaxConductivity     == article.mMaxConductivity);
    CPPUNIT_ASSERT(tReferenceTemp       == article.mReferenceTemp);
    CPPUNIT_ASSERT(tReferencePress      == article.mReferencePress);
    CPPUNIT_ASSERT(tMaxFlowLowSpeed     == article.mLowSpeedMaxFlow);
    CPPUNIT_ASSERT(tLowSpeedCoeff1      == article.mCoeffLowSpeed1);
    CPPUNIT_ASSERT(tLowSpeedCoeff2      == article.mCoeffLowSpeed2);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed0   == article.mEffCoeffLowSpeed[0]);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed1   == article.mEffCoeffLowSpeed[1]);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed2   == article.mEffCoeffLowSpeed[2]);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed3   == article.mEffCoeffLowSpeed[3]);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed4   == article.mEffCoeffLowSpeed[4]);
    CPPUNIT_ASSERT(tEffCoeffLowSpeed5   == article.mEffCoeffLowSpeed[5]);
    CPPUNIT_ASSERT(tMaxFlowHighSpeed    == article.mHighSpeedMaxFlow);
    CPPUNIT_ASSERT(tHighSpeedCoeff1     == article.mCoeffHighSpeed1);
    CPPUNIT_ASSERT(tHighSpeedCoeff2     == article.mCoeffHighSpeed2);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed0  == article.mEffCoeffHighSpeed[0]);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed1  == article.mEffCoeffHighSpeed[1]);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed2  == article.mEffCoeffHighSpeed[2]);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed3  == article.mEffCoeffHighSpeed[3]);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed4  == article.mEffCoeffHighSpeed[4]);
    CPPUNIT_ASSERT(tEffCoeffHighSpeed5  == article.mEffCoeffHighSpeed[5]);
    CPPUNIT_ASSERT(tMinEffLimitLowSpeed == article.mMinEffLimLowSpeed);
    CPPUNIT_ASSERT(tMinEffLimitHighSpeed== article.mMinEffLimHighSpeed);
    CPPUNIT_ASSERT(tMaxEffLimitLowSpeed == article.mMaxEffLimLowSpeed);
    CPPUNIT_ASSERT(tMaxEffLimitHighSpeed== article.mMaxEffLimHighSpeed);
    CPPUNIT_ASSERT(tCorrectedSpeedLow   == article.mCorrectedSpeedLow);
    CPPUNIT_ASSERT(tCorrectedSpeedHigh  == article.mCorrectedSpeedHigh);
    CPPUNIT_ASSERT(tFilterGain          == article.mFilterGain);
    CPPUNIT_ASSERT(tDriveRatio          == article.mDriveRatio);
    CPPUNIT_ASSERT(tThermalDiameter     == article.mThermalDiameter);

    const double expectedArea   = tThermalLength * PI * tThermalDiameter;
    const double expectedROverD = tSurfaceRoughness / tThermalDiameter;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedArea,   article.mThermalSurfaceArea, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedROverD, article.mThermalROverD,      DBL_EPSILON);

    /// @test    Terms initialized from input data.
    CPPUNIT_ASSERT(tMotorSpeed        == article.mMotorSpeed);
    CPPUNIT_ASSERT(tWallTemperature   == article.mWallTemperature);

    /// @test    Initialized state data.
    CPPUNIT_ASSERT(0.0 == article.mWallHeatFlux);
    CPPUNIT_ASSERT(0.0 == article.mImpellerTorque);
    CPPUNIT_ASSERT(0.0 == article.mImpellerSpeed);
    CPPUNIT_ASSERT(0.0 == article.mImpellerPower);
    CPPUNIT_ASSERT(0.0 == article.mPredictedFlowRate);
    CPPUNIT_ASSERT(0.0 == article.mEfficiency);
    CPPUNIT_ASSERT(0.0 == article.mPressureRatio);
    CPPUNIT_ASSERT(0.0 == article.mPressureDrop);

    /// @test    Internal fluid initialization.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getOutflow()->getTemperature(),
                                 article.getInternalFluid()->getTemperature(), DBL_EPSILON);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Initialize another test article with initialization data to exercise alternate logic paths
    ///   in the initialize method.
    FriendlyGunnsGasTurbine article2;

    tConfigData->mThermalLength = 0.0;
    CPPUNIT_ASSERT_NO_THROW(article2.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article2.mThermalSurfaceArea, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article2.mThermalROverD,      DBL_EPSILON);
    CPPUNIT_ASSERT(article2.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Turbine link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasTurbine::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    FriendlyGunnsGasTurbine article;

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

    /// @test    Initialization exception on invalid config data: reference temperature too small.
    tConfigData->mReferenceTemp = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mReferenceTemp = tReferenceTemp;

    /// @test    Initialization exception on invalid config data: reference pressure too small.
    tConfigData->mReferencePress = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mReferencePress = tReferencePress;

    /// @test    Initialization exception on invalid config data: drive ratio too small.
    tConfigData->mDriveRatio = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mDriveRatio = tDriveRatio;

    /// @test    Initialization exception on invalid config data: corrected speeds too small.
    tConfigData->mCorrectedSpeedLow = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCorrectedSpeedLow = tCorrectedSpeedLow;
    tConfigData->mCorrectedSpeedHigh = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCorrectedSpeedHigh = tCorrectedSpeedHigh;

    /// @test    Initialization exception on invalid config data: corrected speeds equal.
    tConfigData->mCorrectedSpeedLow = tCorrectedSpeedHigh;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCorrectedSpeedLow = tCorrectedSpeedLow;

    /// @test    Initialization exception on invalid config data: max Flow rates too small.
    tConfigData->mMaxFlowLowSpeed = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxFlowLowSpeed = tMaxFlowLowSpeed;
    tConfigData->mMaxFlowHighSpeed = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxFlowHighSpeed = tMaxFlowHighSpeed;

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

    /// @test    Initialization exception on invalid input data: Efficiency limits < 0.
    tConfigData->mMinEffLimLowSpeed = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMinEffLimLowSpeed = tMinEffLimitLowSpeed;
    tConfigData->mMinEffLimHighSpeed = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMinEffLimHighSpeed = tMinEffLimitHighSpeed;
    tConfigData->mMaxEffLimLowSpeed = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxEffLimLowSpeed = tMaxEffLimitLowSpeed;
    tConfigData->mMaxEffLimHighSpeed = DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mMaxEffLimHighSpeed = tMaxEffLimitHighSpeed;

    /// @test    Initialization exception on invalid input data: wall temperature < 0.
    tInputData->mWallTemperature = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Turbine link model Restart Model method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasTurbine::testRestartModel()
{
    UT_RESULT;

    tArticle->restartModel() ;
    CPPUNIT_ASSERT( 0.0 == tArticle->mImpellerSpeed);
    CPPUNIT_ASSERT( 0.0 == tArticle->mImpellerPower);
    CPPUNIT_ASSERT( 0.0 == tArticle->mPressureDrop);
    CPPUNIT_ASSERT( 0.0 == tArticle->mEfficiency);

    UT_PASS ;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Turbine link model getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasTurbine::testAccessors()
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
/// @details  Tests for GUNNS Gas Turbine link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasTurbine::testModifiers()
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

    /// @test    Test interpolate function for calculation accuracy
    double tempFrac   = 0.5 ;
    double tempInterp = tArticle->interpolate(100, 150, tempFrac);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(125.0, tempInterp,0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Turbine link model update state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasTurbine::testUpdateState()
{
    UT_RESULT;

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Outputs under normal running condition.
    tArticle->mDriveRatio   = tDriveRatio;
    tArticle->mPressureDrop = 25 ;
    tArticle->mPotentialVector[0] = tNodes[0].getContent()->getPressure();
    tArticle->mPotentialVector[1] = tNodes[1].getContent()->getPressure();

    double expectedImpellerSpeed = tMotorSpeed / tDriveRatio; // 6000
    double sourceDensity         = tNodes[0].getOutflow()->getDensity();
    double sourceTemp            = tNodes[0].getOutflow()->getTemperature(); // 283
    double sourcePress           = tNodes[0].getOutflow()->getPressure(); // 150
    double expectedCorrectedSpeed= expectedImpellerSpeed/sqrt(sourceTemp/tReferenceTemp); // 5893.04
    tArticle->mCorrectedSpeedHigh   = expectedCorrectedSpeed;

    tArticle->updateState(tTimeStep);

    double expectedPressRatio    = 4.0 / 3.0;
    double expectedFlow          = 77.6087 * 1.45867;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImpellerSpeed, tArticle->mImpellerSpeed,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPressRatio,    tArticle->mPressureRatio,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFlow,          tArticle->mPredictedFlowRate,         1.0);

    /// @test   test drive shaft disconnected for line coverage
    tArticle->mDriveRatio = 0.0;
    tArticle->updateState(tTimeStep);
    tArticle->mDriveRatio = tDriveRatio;

    /// @test   check zero density inlet
    tNodes[0].getOutflow()->setPressure(0);
    tNodes[1].getOutflow()->setPressure(0);
    tArticle->mPressureDrop = 25;
    tArticle->mPotentialVector[0] = tNodes[0].getContent()->getPressure();
    tArticle->mPotentialVector[1] = tNodes[1].getContent()->getPressure();
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT(tArticle->mFlowRate == 0.0);
    tNodes[0].getOutflow()->setPressure(150);
    tNodes[1].getOutflow()->setPressure(100);

    /// @test   check computeFlowRate outflow parameters
    tNodes[0].getContent()->setTemperature(50);
    tNodes[0].getContent()->setPressure(40);
    tNodes[0].resetFlows();
    tNodes[0].getContent()->setTemperature(283.0);
    tNodes[0].getContent()->setPressure(150);
    tArticle->mFlux = 1.0;

    CPPUNIT_ASSERT(tNodes[0].getOutflow()->getPressure()!= tNodes[0].getContent()->getPressure());
    CPPUNIT_ASSERT(tNodes[0].getOutflow()->getTemperature()!=
                                           tNodes[0].getContent()->getTemperature());

    tArticle->computeFlowRate();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(std::max(DBL_EPSILON,tNodes[0].getOutflow()->getPressure()),
            tNodes[0].getOutflow()->getPressure(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(std::max(DBL_EPSILON,tNodes[0].getOutflow()->getTemperature()),
            tNodes[0].getOutflow()->getTemperature(), DBL_EPSILON);

    tNodes[1].getContent()->setTemperature(75);
    tNodes[1].getContent()->setPressure(20);
    tNodes[1].resetFlows();
    tNodes[1].getContent()->setTemperature(283.0);
    tNodes[1].getContent()->setPressure(100);

    CPPUNIT_ASSERT(tNodes[1].getOutflow()->getPressure()!= tNodes[1].getContent()->getPressure());
    CPPUNIT_ASSERT(tNodes[1].getOutflow()->getTemperature()!=
                                           tNodes[1].getContent()->getTemperature());

    tArticle->computeFlowRate();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(std::max(DBL_EPSILON,tNodes[1].getOutflow()->getPressure()),
            tNodes[1].getOutflow()->getPressure(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(std::max(DBL_EPSILON,tNodes[1].getOutflow()->getTemperature()),
            tNodes[1].getOutflow()->getTemperature(), DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Turbine link model update fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasTurbine::testUpdateFluid()
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
    tArticle->mVolFlowRate    = 93.0;
    tArticle->mPressureRatio  = 1.406;
    tArticle->mPressureDrop   = 150 * (1 - 1/tArticle->mPressureRatio);
    tArticle->mMotorSpeed     = tMotorSpeed;
    tArticle->mImpellerSpeed  = tMotorSpeed / tDriveRatio;

    /// - Source pressure converted from kPa to Pa to relate power in Watts.
    double expectedImpellerSpeed = tMotorSpeed / tDriveRatio; // 6000
    double sourceDensity         = tNodes[0].getOutflow()->getDensity();
    double sourceTemp            = tNodes[0].getOutflow()->getTemperature(); // 283
    double sourcePress           = tNodes[0].getOutflow()->getPressure(); // 150
    double expectedCorrectedSpeed= expectedImpellerSpeed / sqrt(sourceTemp / tReferenceTemp) ; // 5893.04
    tArticle->mCorrectedSpeedHigh   = expectedCorrectedSpeed ;

    double expectedPressDrop  = -43.31437 ;

    double efficiency         = 0.63 ;
    double expectedPower      = 1000.0 * 93.0 * expectedPressDrop * efficiency;
    /// - Motor speed converted from rpm to r/s to relate torque in N*m.
    double expectedTorque     = -expectedPower / tMotorSpeed * 60.0 / 2.0 / PI;
    tArticle->updateFluid(tTimeStep, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(efficiency,     tArticle->mEfficiency,     0.1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,  tArticle->mImpellerPower,  1000);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTorque, tArticle->mImpellerTorque, 5);

    /// @test   Impeller torque with zero drive ratio.
    tArticle->mDriveRatio     = 0.0;
    expectedTorque            = 0.0;
    tArticle->updateFluid(tTimeStep, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTorque, tArticle->mImpellerTorque, DBL_EPSILON);

    /// @test   Impeller power & torque at zero speed.
    tArticle->mMotorSpeed     = 0.0;
    expectedPower             = 0.0;
    tArticle->updateFluid(tTimeStep, 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPower,  tArticle->mImpellerPower,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTorque, tArticle->mImpellerTorque, DBL_EPSILON);

    UT_PASS_LAST;
}
