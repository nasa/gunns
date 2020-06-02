/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidCondensingHxSeparator.o))
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidCondensingHxSeparator.hh"

/// @details  Test identification number.
int UtGunnsFluidCondensingHxSeparator::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Condensing HX & Separator link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidCondensingHxSeparator::UtGunnsFluidCondensingHxSeparator()
    :
    CppUnit::TestFixture(),
    tTypes(),
    tFractions(),
    tFluidProperties(0),
    tFluidConfig(0),
    tFluidInput(0),
    tFluidInput1(0),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tMaxConductivity(0.0),
    tHxNumSegments(0),
    tHxDryHtc(0.0),
    tHxWetHtcFactor(0.0),
    tHxWetFlowFactor(0.0),
    tHxEvaporationCoeff(0.0),
    tSlurperFlowMassDry(0.0),
    tSlurperFlowMassWet(0.0),
    tSlurperFlowRateRef(0.0),
    tWsMassExponent(0.0),
    tWsMaxCondensate(0.0),
    tWsReferenceSpeed(0.0),
    tWsReferencePressure(0.0),
    tWsReferenceTorque(0.0),
    tWsDriveRatio(0.0),
    tWsEvaporationCoeff(0.0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tWallTemperature(0),
    tWsMotorSpeed(0.0),
    tWsCondensateMass(0.0),
    tHxCondensateMass(0.0),
    tTransferFlowRate(0.0),
    tSlurperState(GunnsFluidCondensingHxSeparator::PAUSED_DRY),
    tInputData(0),
    tArticle(0),
    tTimeStep(0.0)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Condensing HX & Separator link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidCondensingHxSeparator::~UtGunnsFluidCondensingHxSeparator()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.  The default node
///           fluid set up matches CASE 13 from the testHxFlow test below.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::setUp()
{
    /// - Define the nominal port fluids.
    tFluidProperties       = new DefinedFluidProperties();
    tTypes[0]              = FluidProperties::GUNNS_H2O;
    tTypes[1]              = FluidProperties::GUNNS_O2;
    tTypes[2]              = FluidProperties::GUNNS_N2;
    tFractions[0]          = 0.0091755;
    tFractions[1]          = 0.2335363;
    tFractions[2]          = 1.0 - tFractions[0] - tFractions[1];
    tFluidConfig           = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tFluidInput            = new PolyFluidInputData(295.15,
                                                    100.18082,
                                                      0.0,
                                                      0.0,
                                                    tFractions);

    tFluidInput1           = new PolyFluidInputData(280.42778,
                                                    100.04293,
                                                      0.0,
                                                      0.0,
                                                    tFractions);

    /// - Initialize the nodes, with the last node as the ground node.
    tNodes[0].initialize("UtNode1", tFluidConfig);
    tNodes[1].initialize("UtNode2", tFluidConfig);
    tNodes[2].initialize("UtNode3", tFluidConfig);
    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput);
    tNodes[2].resetContentState();

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    /// - Initialize the nodes list.
    tNodeList.mNodes       = tNodes;
    tNodeList.mNumNodes    = N_NODES;

    /// - Define the nominal configuration data.
    tName                  =  "nominal";
    tMaxConductivity       =  0.0120106;
    tHxNumSegments         =  4;
    tHxDryHtc              =  1200.0;
    tHxWetHtcFactor        =  0.05;         //  5% per kg
    tHxWetFlowFactor       =  0.01;         // -1% per kg
    tHxEvaporationCoeff    =  5.7E-5;
    tSlurperFlowMassDry    =  0.68;         //1.5  lbm
    tSlurperFlowMassWet    =  0.07;         //0.15 lbm
    tSlurperFlowRateRef    =  0.00029;      //1.45 kg/hr @ 5000 rpm
    tWsMassExponent        =  0.2;
    tWsMaxCondensate       =  0.1;
    tWsReferenceSpeed      =  5000.0;
    tWsReferencePressure   =  100.0;
    tWsReferenceTorque     = -10.0;
    tWsDriveRatio          =  1.0;
    tWsEvaporationCoeff    =  1.0E-7;
    tConfigData            = new GunnsFluidCondensingHxSeparatorConfigData(tName,
                                                                           &tNodeList,
                                                                           tMaxConductivity,
                                                                           tHxNumSegments,
                                                                           tHxDryHtc,
                                                                           tHxWetHtcFactor,
                                                                           tHxWetFlowFactor,
                                                                           tHxEvaporationCoeff,
                                                                           tSlurperFlowMassDry,
                                                                           tSlurperFlowMassWet,
                                                                           tSlurperFlowRateRef,
                                                                           tWsMassExponent,
                                                                           tWsMaxCondensate,
                                                                           tWsReferenceSpeed,
                                                                           tWsReferencePressure,
                                                                           tWsReferenceTorque,
                                                                           tWsDriveRatio,
                                                                           tWsEvaporationCoeff);

    /// - Define the nominal input data.
    tMalfBlockageFlag      = false;
    tMalfBlockageValue     = 0.0;
    tWallTemperature       = 286.0;
    tWsMotorSpeed          = 5000.0;
    tWsCondensateMass      = 0.0001;
    tHxCondensateMass      = 0.01;
    tTransferFlowRate      = 4.027778E-4;     // 1.45 kg/hr
    tSlurperState          = GunnsFluidCondensingHxSeparator::PAUSED_WET;
    tInputData             = new GunnsFluidCondensingHxSeparatorInputData(tMalfBlockageFlag,
                                                                          tMalfBlockageValue,
                                                                          tWallTemperature,
                                                                          tWsMotorSpeed,
                                                                          tWsCondensateMass,
                                                                          tHxCondensateMass,
                                                                          tTransferFlowRate,
                                                                          tSlurperState);

    /// - Define the nominal port mapping.
    tPort0                 = 0;
    tPort1                 = 1;

    /// - Default construct the nominal test article.
    tArticle               = new FriendlyGunnsFluidCondensingHxSeparator;

    /// - Define the nominal flow rate, time step and comparison tolerance.
    tTimeStep              = 0.1;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput1;
    delete tFluidInput;
    delete tFluidConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Fluid Condensing HX & Separator link model
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes               == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity     == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(0.0                  == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tHxNumSegments       == tConfigData->mNumSegments);
    CPPUNIT_ASSERT(tHxDryHtc            == tConfigData->mHtcCoeff0);
    CPPUNIT_ASSERT(tHxWetHtcFactor      == tConfigData->mHxWetHtcFactor);
    CPPUNIT_ASSERT(tHxWetFlowFactor     == tConfigData->mHxWetFlowFactor);
    CPPUNIT_ASSERT(tHxEvaporationCoeff  == tConfigData->mHxEvaporationCoeff);
    CPPUNIT_ASSERT(tSlurperFlowMassDry  == tConfigData->mSlurperFlowMassDry);
    CPPUNIT_ASSERT(tSlurperFlowMassWet  == tConfigData->mSlurperFlowMassWet);
    CPPUNIT_ASSERT(tSlurperFlowRateRef  == tConfigData->mSlurperFlowRateRef);
    CPPUNIT_ASSERT(tWsMassExponent      == tConfigData->mWsMassExponent);
    CPPUNIT_ASSERT(tWsMaxCondensate     == tConfigData->mWsMaxCondensate);
    CPPUNIT_ASSERT(tWsReferenceSpeed    == tConfigData->mWsReferenceSpeed);
    CPPUNIT_ASSERT(tWsReferencePressure == tConfigData->mWsReferencePressure);
    CPPUNIT_ASSERT(tWsReferenceTorque   == tConfigData->mWsReferenceTorque);
    CPPUNIT_ASSERT(tWsDriveRatio        == tConfigData->mWsDriveRatio);
    CPPUNIT_ASSERT(tWsEvaporationCoeff  == tConfigData->mWsEvaporationCoeff);

    /// @test    Configuration data default construction.
    GunnsFluidCondensingHxSeparatorConfigData defaultConfig;
    CPPUNIT_ASSERT(0                    == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                    == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0                    == defaultConfig.mNumSegments);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mHtcCoeff0);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mHxWetHtcFactor);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mHxWetFlowFactor);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mHxEvaporationCoeff);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mSlurperFlowMassDry);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mSlurperFlowMassWet);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mSlurperFlowRateRef);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mWsMassExponent);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mWsMaxCondensate);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mWsReferenceSpeed);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mWsReferencePressure);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mWsReferenceTorque);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mWsDriveRatio);
    CPPUNIT_ASSERT(0.0                  == defaultConfig.mWsEvaporationCoeff);

    /// @test    Configuration data copy construction.
    GunnsFluidCondensingHxSeparatorConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tName                == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes               == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity     == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                  == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tHxNumSegments       == copyConfig.mNumSegments);
    CPPUNIT_ASSERT(tHxDryHtc            == copyConfig.mHtcCoeff0);
    CPPUNIT_ASSERT(tHxWetHtcFactor      == copyConfig.mHxWetHtcFactor);
    CPPUNIT_ASSERT(tHxWetFlowFactor     == copyConfig.mHxWetFlowFactor);
    CPPUNIT_ASSERT(tHxEvaporationCoeff  == copyConfig.mHxEvaporationCoeff);
    CPPUNIT_ASSERT(tSlurperFlowMassDry  == copyConfig.mSlurperFlowMassDry);
    CPPUNIT_ASSERT(tSlurperFlowMassWet  == copyConfig.mSlurperFlowMassWet);
    CPPUNIT_ASSERT(tSlurperFlowRateRef  == copyConfig.mSlurperFlowRateRef);
    CPPUNIT_ASSERT(tWsMassExponent      == copyConfig.mWsMassExponent);
    CPPUNIT_ASSERT(tWsMaxCondensate     == copyConfig.mWsMaxCondensate);
    CPPUNIT_ASSERT(tWsReferenceSpeed    == copyConfig.mWsReferenceSpeed);
    CPPUNIT_ASSERT(tWsReferencePressure == copyConfig.mWsReferencePressure);
    CPPUNIT_ASSERT(tWsReferenceTorque   == copyConfig.mWsReferenceTorque);
    CPPUNIT_ASSERT(tWsDriveRatio        == copyConfig.mWsDriveRatio);
    CPPUNIT_ASSERT(tWsEvaporationCoeff  == copyConfig.mWsEvaporationCoeff);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of GUNNS Fluid Condensing HX & Separator link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag    == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue   == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tWallTemperature     == tInputData->mWallTemperature);
    CPPUNIT_ASSERT(tWsMotorSpeed        == tInputData->mWsMotorSpeed);
    CPPUNIT_ASSERT(tWsCondensateMass    == tInputData->mWsCondensateMass);
    CPPUNIT_ASSERT(tHxCondensateMass    == tInputData->mHxCondensateMass);
    CPPUNIT_ASSERT(tTransferFlowRate    == tInputData->mTransferFlowRate);
    CPPUNIT_ASSERT(tSlurperState        == tInputData->mSlurperState);

    /// @test    Input data default construction.
    GunnsFluidCondensingHxSeparatorInputData defaultInput;
    CPPUNIT_ASSERT(false                == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                  == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0                    == defaultInput.mWallTemperature);
    CPPUNIT_ASSERT(0.0                  == defaultInput.mWsMotorSpeed);
    CPPUNIT_ASSERT(0.0                  == defaultInput.mWsCondensateMass);
    CPPUNIT_ASSERT(0.0                  == defaultInput.mHxCondensateMass);
    CPPUNIT_ASSERT(0.0                  == defaultInput.mTransferFlowRate);
    CPPUNIT_ASSERT(GunnsFluidCondensingHxSeparator::PAUSED_DRY
                                        == defaultInput.mSlurperState);

    /// @test    Input data copy construction.
    GunnsFluidCondensingHxSeparatorInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag    == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue   == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tWallTemperature     == copyInput.mWallTemperature);
    CPPUNIT_ASSERT(tWsMotorSpeed        == copyInput.mWsMotorSpeed);
    CPPUNIT_ASSERT(tWsCondensateMass    == copyInput.mWsCondensateMass);
    CPPUNIT_ASSERT(tHxCondensateMass    == copyInput.mHxCondensateMass);
    CPPUNIT_ASSERT(tTransferFlowRate    == copyInput.mTransferFlowRate);
    CPPUNIT_ASSERT(tSlurperState        == copyInput.mSlurperState);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(""                   == tArticle->mName);
    CPPUNIT_ASSERT(0                    == tArticle->mNodes);
    CPPUNIT_ASSERT(0.0                  == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(0.0                  == tArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(0                    == tArticle->mNumSegments);
    CPPUNIT_ASSERT(0.0                  == tArticle->mHtcCoeff0);
    CPPUNIT_ASSERT(0.0                  == tArticle->mHxWetHtcFactor);
    CPPUNIT_ASSERT(0.0                  == tArticle->mHxWetFlowFactor);
    CPPUNIT_ASSERT(0.0                  == tArticle->mHxEvaporationCoeff);
    CPPUNIT_ASSERT(0.0                  == tArticle->mSlurperFlowMassDry);
    CPPUNIT_ASSERT(0.0                  == tArticle->mSlurperFlowMassWet);
    CPPUNIT_ASSERT(0.0                  == tArticle->mWsMassExponent);
    CPPUNIT_ASSERT(0.0                  == tArticle->mWsMaxCondensate);
    CPPUNIT_ASSERT(0.0                  == tArticle->mWsDriveRatio);
    CPPUNIT_ASSERT(0.0                  == tArticle->mWsEvaporationCoeff);

    /// @test    Default construction attributes derived from configuration data.
    CPPUNIT_ASSERT(0.0                  == tArticle->mSlurperFlowFactor);
    CPPUNIT_ASSERT(0.0                  == tArticle->mWsPowerCurveCoeff);
    CPPUNIT_ASSERT(0.0                  == tArticle->mWsTorqueFactor);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(0                    == tArticle->mSegmentTemperature);
    CPPUNIT_ASSERT(0.0                  == tArticle->mWsMotorSpeed);
    CPPUNIT_ASSERT(0.0                  == tArticle->mWsCondensateMass);
    CPPUNIT_ASSERT(0.0                  == tArticle->mHxCondensateMass);
    CPPUNIT_ASSERT(0.0                  == tArticle->mTransferFlowRate);
    CPPUNIT_ASSERT(GunnsFluidCondensingHxSeparator::PAUSED_DRY
                                        == tArticle->mSlurperState);

    /// @test    Default construction malfunction data.
    CPPUNIT_ASSERT(false                == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                  == tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT(false                == tArticle->mMalfHxDegradeFlag);
    CPPUNIT_ASSERT(0.0                  == tArticle->mMalfHxDegradeValue);

    /// @test    Default construction internal state data.
    CPPUNIT_ASSERT(0.0                  == tArticle->mSegmentHtc);
    CPPUNIT_ASSERT(0.0                  == tArticle->mSensibleHeat);
    CPPUNIT_ASSERT(0.0                  == tArticle->mLatentHeat);
    CPPUNIT_ASSERT(0.0                  == tArticle->mTotalHeat);
    CPPUNIT_ASSERT(0.0                  == tArticle->mCondensationRate);
    CPPUNIT_ASSERT(0                    == tArticle->mCondensateFluid);
    CPPUNIT_ASSERT(0                    == tArticle->mSlurperFlowRate);
    CPPUNIT_ASSERT(0                    == tArticle->mWsDrumSpeed);
    CPPUNIT_ASSERT(0                    == tArticle->mWsEvaporationRate);
    CPPUNIT_ASSERT(0                    == tArticle->mWsDeltaPressure);

    /// @test    Default construction output data.
    CPPUNIT_ASSERT(0                    == tArticle->mSegmentHeat);
    CPPUNIT_ASSERT(0.0                  == tArticle->mTransferTemperature);
    CPPUNIT_ASSERT(0.0                  == tArticle->mTransferPressure);
    CPPUNIT_ASSERT(0.0                  == tArticle->mWsMotorTorque);
    CPPUNIT_ASSERT(false                == tArticle->mLiquidOverflow);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(false                == tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    UtGunnsFluidCondensingHxSeparator* article = new UtGunnsFluidCondensingHxSeparator();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model nominal initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testInitialization ()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName               == tArticle->mName);
    CPPUNIT_ASSERT(tNodes              == tArticle->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity    == tArticle->mMaxConductivity);
    CPPUNIT_ASSERT(0.0                 == tArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tHxNumSegments      == tArticle->mNumSegments);
    CPPUNIT_ASSERT(tHxDryHtc           == tArticle->mHtcCoeff0);
    CPPUNIT_ASSERT(tHxWetHtcFactor     == tArticle->mHxWetHtcFactor);
    CPPUNIT_ASSERT(tHxWetFlowFactor    == tArticle->mHxWetFlowFactor);
    CPPUNIT_ASSERT(tHxEvaporationCoeff == tArticle->mHxEvaporationCoeff);
    CPPUNIT_ASSERT(tSlurperFlowMassDry == tArticle->mSlurperFlowMassDry);
    CPPUNIT_ASSERT(tSlurperFlowMassWet == tArticle->mSlurperFlowMassWet);
    CPPUNIT_ASSERT(tWsMassExponent     == tArticle->mWsMassExponent);
    CPPUNIT_ASSERT(tWsMaxCondensate    == tArticle->mWsMaxCondensate);
    CPPUNIT_ASSERT(tWsDriveRatio       == tArticle->mWsDriveRatio);
    CPPUNIT_ASSERT(tWsEvaporationCoeff == tArticle->mWsEvaporationCoeff);

    /// @test    Nominal attributes derived from configuration data.
    const double expectedFlowCoeff  = tSlurperFlowRateRef  / tWsReferenceSpeed;
    const double expectedPowerCoeff = tWsReferencePressure / tWsReferenceSpeed
                                    / pow(tWsMaxCondensate, tWsMassExponent);
    const double expectedTorqueFact = tWsReferenceTorque   / tWsReferenceSpeed;
    CPPUNIT_ASSERT(expectedFlowCoeff   == tArticle->mSlurperFlowFactor);
    CPPUNIT_ASSERT(expectedPowerCoeff  == tArticle->mWsPowerCurveCoeff);
    CPPUNIT_ASSERT(expectedTorqueFact  == tArticle->mWsTorqueFactor);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(tMalfBlockageFlag   == tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue  == tArticle->mMalfBlockageValue);
    for (int i=0; i<tHxNumSegments; ++i) {
        CPPUNIT_ASSERT(tWallTemperature == tArticle->mSegmentTemperature[i]);
    }
    CPPUNIT_ASSERT(tWsMotorSpeed       == tArticle->mWsMotorSpeed);
    CPPUNIT_ASSERT(tWsCondensateMass   == tArticle->mWsCondensateMass);
    CPPUNIT_ASSERT(tHxCondensateMass   == tArticle->mHxCondensateMass);
    CPPUNIT_ASSERT(tTransferFlowRate   == tArticle->mTransferFlowRate);
    CPPUNIT_ASSERT(tSlurperState       == tArticle->mSlurperState);

    /// @test    Malfunction initialization.
    CPPUNIT_ASSERT(false               == tArticle->mMalfHxDegradeFlag);
    CPPUNIT_ASSERT(0.0                 == tArticle->mMalfHxDegradeValue);
    CPPUNIT_ASSERT(false               == tArticle->mMalfHxDegradeFlag);
    CPPUNIT_ASSERT(0.0                 == tArticle->mMalfHxDegradeValue);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0.0                 == tArticle->mSegmentHtc);
    CPPUNIT_ASSERT(0.0                 == tArticle->mSensibleHeat);
    CPPUNIT_ASSERT(0.0                 == tArticle->mLatentHeat);
    CPPUNIT_ASSERT(0.0                 == tArticle->mTotalHeat);
    CPPUNIT_ASSERT(0.0                 == tArticle->mCondensationRate);
    CPPUNIT_ASSERT(1.0                 == tArticle->mCondensateFluid->getMassFraction(FluidProperties::GUNNS_H2O));
    CPPUNIT_ASSERT(0.0                 == tArticle->mSlurperFlowRate);
    CPPUNIT_ASSERT(0.0                 == tArticle->mWsDrumSpeed);
    CPPUNIT_ASSERT(0.0                 == tArticle->mWsEvaporationRate);
    CPPUNIT_ASSERT(0.0                 == tArticle->mWsDeltaPressure);

    /// @test    Nominal output data.
    for (int i = 0; i < tHxNumSegments; ++i) {
        CPPUNIT_ASSERT(0.0             == tArticle->mSegmentHeat[i]);
    }
    CPPUNIT_ASSERT(0.0                 == tArticle->mTransferTemperature);
    CPPUNIT_ASSERT(0.0                 == tArticle->mTransferPressure);
    CPPUNIT_ASSERT(0.0                 == tArticle->mWsMotorTorque);
    CPPUNIT_ASSERT(false               == tArticle->mLiquidOverflow);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(true                == tArticle->mInitFlag);

    /// @test    Verify restartModel functionality
    tArticle->mSlurperFlowRate    = 1.0;
    tArticle->mWsDrumSpeed        = 1.0;
    tArticle->mWsEvaporationRate  = 1.0;
    tArticle->mWsDeltaPressure    = 1.0;

    tArticle->restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mSlurperFlowRate,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mWsDrumSpeed,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mWsEvaporationRate,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tArticle->mWsDeltaPressure,DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testRestart ()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set some stuff in the article and its base class that should be reset on restart.
    tArticle->mSegmentHtc            = 1.0;
    tArticle->mWsDeltaPressure       = 1.0;
    tArticle->mEffectiveConductivity = 1.0;

    /// @test restart resets terms
    tArticle->restart();

    CPPUNIT_ASSERT(0.0                               == tArticle->mSegmentHtc);
    CPPUNIT_ASSERT(0.0                               == tArticle->mWsDeltaPressure);
    CPPUNIT_ASSERT(0.0                               == tArticle->mEffectiveConductivity);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testAccessors()
{
    UT_RESULT;

    /// @test    getTotalHeat method.
    tArticle->mTotalHeat = 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1000.0, tArticle->getTotalHeat(),        DBL_EPSILON);

    /// @test    getCondensationRate method.
    tArticle->mCondensationRate = 0.0001;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0001, tArticle->getCondensationRate(), DBL_EPSILON);

    /// @test    getWsMotorSpeed method.
    tArticle->mWsMotorSpeed = 10.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0,   tArticle->getWsMotorSpeed(),     DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testModifiers()
{
    UT_RESULT;

    /// @test    The HX heat transfer degradation malf parameters are set.
    tArticle->setMalfHxDegrade(true, 1.0);
    CPPUNIT_ASSERT(true  == tArticle->mMalfHxDegradeFlag);
    CPPUNIT_ASSERT(1.0   == tArticle->mMalfHxDegradeValue);

    /// @test    The HX heat transfer degradation malf parameters are reset.
    tArticle->setMalfHxDegrade();
    CPPUNIT_ASSERT(false == tArticle->mMalfHxDegradeFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfHxDegradeValue);

    /// @test    The HX ondensation pool rate override malf parameters are set.
    tArticle->setMalfHxCondensate(true, 1.0);
    CPPUNIT_ASSERT(true  == tArticle->mMalfHxCondensateFlag);
    CPPUNIT_ASSERT(1.0   == tArticle->mMalfHxCondensateValue);

    /// @test    The HX ondensation pool rate override malf parameters are reset.
    tArticle->setMalfHxCondensate();
    CPPUNIT_ASSERT(false == tArticle->mMalfHxCondensateFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfHxCondensateValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model updateState method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testStep()
{
    UT_RESULT;

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Nominal updateState outputs.
    tArticle->mWsDriveRatio = 0.5;
    double expectedCond   = tMaxConductivity * (1.0 - tHxWetFlowFactor * tHxCondensateMass);
    double expectedDrum   = tWsMotorSpeed / tArticle->mWsDriveRatio;
    double expectedTorque = expectedDrum * tWsReferenceTorque / tWsReferenceSpeed;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCond,   tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDrum,   tArticle->mWsDrumSpeed,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTorque, tArticle->mWsMotorTorque,         DBL_EPSILON);

    /// @test    Limited conductivity with negative wet flow factor, and disconnected motor drive.
    tArticle->mWsMotorSpeed   *= 2.0;
    tArticle->mWsDriveRatio    = 0.0;
    tArticle->mHxWetFlowFactor = -0.01;
    expectedCond = tMaxConductivity;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCond,   tArticle->mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDrum,   tArticle->mWsDrumSpeed,           DBL_EPSILON);

    /// @test    Limited conductivity to zero.
    tArticle->mHxWetFlowFactor = 1.0E15;
    expectedCond = 0.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCond,   tArticle->mEffectiveConductivity, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model
///           computeHeatTransferCoefficient method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testHtc()
{
    UT_RESULT;

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Test segment heat transfer coefficient with all effects.
    tArticle->mMalfHxDegradeFlag  = true;
    tArticle->mMalfHxDegradeValue = 0.5;
    double expectedHtc = (tHxDryHtc * (1.0 + tHxWetHtcFactor * tHxCondensateMass))
                       * 0.5 / tHxNumSegments;
    tArticle->computeHeatTransferCoefficient();
    tArticle->degradeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegmentHtc, DBL_EPSILON);

    /// @test    Limited to zero.
    tArticle->mMalfHxDegradeFlag  = false;
    tArticle->mHxWetHtcFactor     = -1.0E15;
    expectedHtc = 0.0;
    tArticle->computeHeatTransferCoefficient();
    tArticle->degradeHeatTransferCoefficient();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHtc,   tArticle->mSegmentHtc, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model updateHeatExchanger method
///           in various forward flow conditions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testHxFlow()
{
    UT_RESULT;

    /// - Define test case inputs.  The first 5 tests reference ISS CCAA test data documented in a
    ///   Hamilton Standard memorandum: "SSF Common Cabin Air Assembly Process Flow Diagrams - Lab
    ///   Module", September 27, 1993.  The remaining test are various special conditions to stress
    ///   the code.
    ///
    ///   Case    Reference Case     Comment
    ///    0            3            Nom P, cold & wet, mid flow
    ///    1           13            Nom P,        wet, hi flow
    ///    2           21            Nom P, hot  & dry, hi flow
    ///    3           23            Low P,        wet, hi flow
    ///    4           30            Low P, cold & wet, lo flow
    ///    5                         Zero PPH2O and hotter HX
    ///    6                         Zero PPH2O and HX colder than min dewpoint
    ///    7                         Inlet colder than dewpoint
    ///    8                         Extreme HTC stress test
    ///    9                         HX pool evaporation
    ///    10                        HX colder than H2O saturation pressure T range
    ///    11                        Hot steam colder wall (Sabatier-like pressure & temperature)
    ///    12                        Hot steam warmer wall (Sabatier-like pressure & temperature)
    ///
    const int numTests = 13;
    const double caseInletP[numTests]    = {100.45661, 100.18082, 102.04241,  69.29231,  73.636,   100.45661, 100.45661, 100.18082,  69.29231, 100.45661, 100.45661, 75.0,      75.0};
    const double caseExitP[numTests]     = {100.31872, 100.04293, 101.83557,  69.08547,  73.621,   100.31872, 100.31872, 100.04293,  69.08547, 100.31872, 100.31872, 74.999,    74.999};
    const double caseXh2o[numTests]      = {0.0089373, 0.0091755, 0.0068318, 0.0116574, 0.0093522, 0.0,       0.0,       0.0091755, 0.0116574, 0.0,       0.0089373, 0.65,      0.65};
    const double caseXo2[numTests]       = {0.2335056, 0.2335363, 0.2332341, 0.2338564, 0.2335591, 0.2335056, 0.2335056, 0.2335363, 0.2338564, 0.2335056, 0.2335056, 0.3,       0.3};
    const double caseInletT[numTests]    = {292.92778, 295.15,    301.20556, 295.98333, 293.70556, 292.92778, 292.92778, 284.0,     295.98333, 292.92778, 274.0,     450.0,     450.0};
    const double caseSeg0T[numTests]     = {283.87269, 284.61339, 287.21823, 284.96832, 279.59323, 301.98287, 250.0,     282.0,     284.96832, 301.98287, 250.0,     300.0,     310.0};
    const double caseSeg1T[numTests]     = {282.23354, 282.77467, 284.70081, 282.99479, 279.05186, 303.62202, 250.0,     281.0,     282.99479, 303.62202, 250.0,     295.0,     305.0};
    const double caseSeg2T[numTests]     = {280.59439, 280.93595, 282.18339, 281.02126, 278.51049, 305.26117, 250.0,     280.0,     281.02126, 305.26117, 250.0,     290.0,     300.0};
    const double caseSeg3T[numTests]     = {278.95524, 279.09723, 279.66597, 279.04772, 277.96913, 306.90032, 250.0,     279.0,     279.04772, 306.90032, 250.0,     285.0,     295.0};
    const double caseHxPool[numTests]    = {0.0,       0.0,       0.0,       0.0,       0.0,       0.0,       0.0,       0.0,       0.0,       0.01,      0.0,       0.0,       0.0};

    /// - Define test case expected outputs
    const double caseQtotal[numTests]    = {3053.0,    3340.0,    4403.0,    3545.0,     843.0,   -1435.8,    4704.0,    2205.0,    4000.0,   -2156.3,    4050.0,    15575.0,   15150.0};
    const double caseQlatent[numTests]   = { 999.0,     998.0,     155.0,     999.0,     155.0,    0.0,       0.0,       1141.0,    1275.0,   -720.6,     2000.0,    13500.0,   13225.0};
    const double caseQsensible[numTests] = {2054.0,    2342.0,    4248.0,    2546.0,     688.0,   -1435.8,    4704.0,    1064.0,    2725.0,   -1435.7,    2050.0,    2075.0,    1925.0};
    const double caseCondense[numTests]  = {4.019e-4,  4.019e-4,  6.3e-5,    4.019e-4,  6.3e-5,    0.0,       0.0,       4.6e-4,    5.5e-4,   -2.94e-4,   7.6729e-4, 5.737E-3,  5.62E-3};
    const double caseExitT[numTests]     = {280.20556, 280.42778, 280.92778, 279.87222, 279.15,    306.6,     250.0,     280.0,     279.04772, 306.6,     273.4,     285.5,     295.5};
    const double caseMdot[numTests]      = {0.1550656, 0.153314,  0.203033,  0.1530622, 0.043306,  0.1550656, 0.1602,    0.153314,  0.1530622, 0.1550656, 0.1585788, 0.0088794, 0.0088794};
    const double caseRelHum[numTests]    = {1.0,       1.0,       1.0,       1.0,       1.0,       0.0,       0.0,       1.0,       1.0,       0.0601190, 1.0,       1.0,       1.0};

    for (int test=0; test<numTests; ++test) {

        /// - Set up test initial conditions.
        tFractions[0]       = caseXh2o[test];
        tFractions[1]       = caseXo2[test];
        tFractions[2]       = 1.0 - tFractions[0] - tFractions[1];

        tNodes[0].resetFlows();
        tNodes[1].resetFlows();
        tNodes[0].getContent()->setMassAndMassFractions(0.0, tFractions);
        tNodes[1].getContent()->setMassAndMassFractions(0.0, tFractions);
        tNodes[0].setPotential(caseInletP[test]);
        tNodes[1].setPotential(caseExitP[test]);
        tNodes[0].getContent()->setTemperature(caseInletT[test]);
        tNodes[1].getContent()->setTemperature(caseExitT[test]);
        tNodes[0].resetFlows();
        tNodes[1].resetFlows();

        FriendlyGunnsFluidCondensingHxSeparator* article = new FriendlyGunnsFluidCondensingHxSeparator;

        article->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

        article->mPotentialVector[0] = tNodes[0].getPotential();
        article->mPotentialVector[1] = tNodes[1].getPotential();

        article->mSegmentTemperature[0] = caseSeg0T[test];
        article->mSegmentTemperature[1] = caseSeg1T[test];
        article->mSegmentTemperature[2] = caseSeg2T[test];
        article->mSegmentTemperature[3] = caseSeg3T[test];

        article->mHxCondensateMass = caseHxPool[test];
        article->mSlurperFlowRate  = 0.0001;

        /// - Special for test #8, try an enormous heat transfer coefficient.
        if (8 == test) {
            article->mHtcCoeff0 = 1.0E15;
        }

        /// - Step the link.
        article->step(tTimeStep);
        article->computeFlows(tTimeStep);
        article->transportFlows(tTimeStep);

        /// - Verify test case results.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(caseQtotal[test],
                                     article->mTotalHeat,
                                     fabs(caseQtotal[test]) * 0.1);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(caseQsensible[test],
                                     article->mSensibleHeat,
                                     fabs(caseQsensible[test]) * 0.1);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(caseCondense[test],
                                     article->mCondensationRate,
                                     fabs(caseCondense[test]) * 0.5);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(caseExitT[test],
                                     article->mInternalFluid->getTemperature(),
                                     1.5);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(caseMdot[test],
                                     article->mFlowRate,
                                     caseMdot[test] * 0.075);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(caseRelHum[test],
                                     GunnsFluidUtils::computeRelativeHumidityH2O(article->mInternalFluid),
                                     FLT_EPSILON);

        /// - Verify common results for all cases.
        CPPUNIT_ASSERT_DOUBLES_EQUAL(article->mTotalHeat,
                                     article->mLatentHeat + article->mSensibleHeat,
                                     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(article->mTotalHeat,
                                     article->mSegmentHeat[0] + article->mSegmentHeat[1] +
                                     article->mSegmentHeat[2] + article->mSegmentHeat[3],
                                     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                tNodes[0].getInflux(),      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(article->mFlowRate, tNodes[0].getOutflux(),     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(article->mFlowRate - article->mCondensationRate,
                                                         tNodes[1].getInflux(),      DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                tNodes[1].getOutflux(),     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(std::max(0.0, caseHxPool[test] + tTimeStep * (article->mCondensationRate - 0.0001)),
                                                         article->mHxCondensateMass, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-article->mCondensationRate / 18.0153,   //MW of H2O
                                                         article->mSourceVector[1],  DBL_EPSILON);
        CPPUNIT_ASSERT(0.0 == article->mSourceVector[0]);
        delete article;
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model updateHeatExhanger method
///           in the no-flow and reverse flow cases.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testHxNoFlow()
{
    UT_RESULT;

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set up equal node pressures for no flow.
    tNodes[1].setPotential(tNodes[0].getPotential());
    tArticle->mPotentialVector[0] = tNodes[0].getPotential();
    tArticle->mPotentialVector[1] = tNodes[1].getPotential();

    /// - Step the link.
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

    /// - Verify no-flow results.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlowRate,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mVolFlowRate,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlux,              DBL_EPSILON);
    CPPUNIT_ASSERT(0.0 == tArticle->mSourceVector[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mSourceVector[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getInflux(),        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getOutflux(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[1].getInflux(),        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[1].getOutflux(),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mTotalHeat,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSensibleHeat,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mLatentHeat,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mCondensationRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSegmentHeat[0],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSegmentHeat[1],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSegmentHeat[2],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSegmentHeat[3],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mWsEvaporationRate, DBL_EPSILON);

    /// - Set up node pressures for back flow.
    tNodes[1].setPotential(tNodes[0].getPotential() + 0.1);
    tArticle->mPotentialVector[1] = tNodes[1].getPotential();

    /// - Step the link.
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    /// - Verify back-flow results.
    CPPUNIT_ASSERT(0.0  > tArticle->mFlowRate);
    CPPUNIT_ASSERT(0.0  > tArticle->mVolFlowRate);
    CPPUNIT_ASSERT(0.0  > tArticle->mFlux);
    CPPUNIT_ASSERT(0.0 == tArticle->mSourceVector[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mSourceVector[1]);
    CPPUNIT_ASSERT(0.0  < tNodes[0].getInflux());
    CPPUNIT_ASSERT(0.0 == tNodes[0].getOutflux());
    CPPUNIT_ASSERT(0.0 == tNodes[1].getInflux());
    CPPUNIT_ASSERT(tNodes[0].getInflux() == tNodes[1].getOutflux());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mTotalHeat,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSensibleHeat,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mLatentHeat,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mCondensationRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSegmentHeat[0],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSegmentHeat[1],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSegmentHeat[2],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mSegmentHeat[3],    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mWsEvaporationRate, DBL_EPSILON);

    /// - Verify volumetric flow rate when the source density is zero.
    tNodes[1].resetContentState();
    tNodes[1].resetFlows();
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tArticle->mVolFlowRate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model updateSlurper method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testSlurper()
{
    UT_RESULT;

    /// - Define test case inputs.
    const int numTests = 16;
    const GunnsFluidCondensingHxSeparator::SlurperStates
                 caseStateIn[numTests]   = {GunnsFluidCondensingHxSeparator::FLOWING,
                                            GunnsFluidCondensingHxSeparator::FLOWING,
                                            GunnsFluidCondensingHxSeparator::FLOWING,
                                            GunnsFluidCondensingHxSeparator::FLOWING,
                                            GunnsFluidCondensingHxSeparator::FLOWING,
                                            GunnsFluidCondensingHxSeparator::FLOWING,
                                            GunnsFluidCondensingHxSeparator::PAUSED_WET,
                                            GunnsFluidCondensingHxSeparator::PAUSED_WET,
                                            GunnsFluidCondensingHxSeparator::PAUSED_WET,
                                            GunnsFluidCondensingHxSeparator::PAUSED_WET,
                                            GunnsFluidCondensingHxSeparator::PAUSED_WET,
                                            GunnsFluidCondensingHxSeparator::PAUSED_WET,
                                            GunnsFluidCondensingHxSeparator::PAUSED_DRY,
                                            GunnsFluidCondensingHxSeparator::PAUSED_DRY,
                                            GunnsFluidCondensingHxSeparator::PAUSED_DRY,
                                            GunnsFluidCondensingHxSeparator::PAUSED_DRY
    };
    const double caseCondMass[numTests]  = {0.0, 1.0, 0.0,    0.0, 1.0,   1.0,               0.5, 0.0,               0.5,               0.0,   0.01,              0.01,  1.0,                       0.5, 0.0,   0.5};
    const double caseCondRate[numTests]  = {0.0, 0.0, 0.001,  0.0, 0.001, 0.001,             0.0, 0.0,               0.0,               0.001, 0.0,               0.001, 0.001,                     0.0, 0.001, 0.001};
    const double caseWsSpeed[numTests]   = {0.0, 0.0, 0.0,   50.0, 0.0,   tWsReferenceSpeed, 0.0, tWsReferenceSpeed, tWsReferenceSpeed, 0.0,   tWsReferenceSpeed, 0.0,  (0.5 * tWsReferenceSpeed),  0.0, 0.0,   0.0};

    /// - Define expected test results.
    const GunnsFluidCondensingHxSeparator::SlurperStates
                 caseStateOut[numTests]  = {GunnsFluidCondensingHxSeparator::PAUSED_DRY,
                                            GunnsFluidCondensingHxSeparator::PAUSED_DRY,
                                            GunnsFluidCondensingHxSeparator::PAUSED_WET,
                                            GunnsFluidCondensingHxSeparator::PAUSED_DRY,
                                            GunnsFluidCondensingHxSeparator::PAUSED_WET,
                                            GunnsFluidCondensingHxSeparator::FLOWING,
                                            GunnsFluidCondensingHxSeparator::PAUSED_DRY,
                                            GunnsFluidCondensingHxSeparator::PAUSED_DRY,
                                            GunnsFluidCondensingHxSeparator::FLOWING,
                                            GunnsFluidCondensingHxSeparator::PAUSED_WET,
                                            GunnsFluidCondensingHxSeparator::PAUSED_WET,
                                            GunnsFluidCondensingHxSeparator::PAUSED_WET,
                                            GunnsFluidCondensingHxSeparator::FLOWING,
                                            GunnsFluidCondensingHxSeparator::PAUSED_DRY,
                                            GunnsFluidCondensingHxSeparator::PAUSED_DRY,
                                            GunnsFluidCondensingHxSeparator::PAUSED_DRY
    };
    const double caseSlurpRate[numTests] = {0.0, 0.0, 0.0,   0.0, 0.0, tSlurperFlowRateRef, 0.0, 0.0, tSlurperFlowRateRef, 0.0, 0.0, 0.0, (0.5 * tSlurperFlowRateRef), 0.0, 0.0, 0.0};

    for (int test=0; test<numTests; ++test) {

        /// - Set up test initial conditions.
        FriendlyGunnsFluidCondensingHxSeparator* article = new FriendlyGunnsFluidCondensingHxSeparator;
        article->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

        article->mSlurperState     = caseStateIn[test];
        article->mHxCondensateMass = caseCondMass[test];
        article->mCondensationRate = caseCondRate[test];
        article->mWsDrumSpeed      = caseWsSpeed[test];

        /// - Call the method and verify outputs.
        article->updateSlurper();

        CPPUNIT_ASSERT(caseStateOut[test] == article->mSlurperState);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(caseSlurpRate[test], article->mSlurperFlowRate, DBL_EPSILON);

        delete article;
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model update fluid (same temperature).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testWaterSeparator()
{
    UT_RESULT;

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set up and verify nominal inputs to the updateWaterSeparator method. This is air at normal
    ///   temperature and humidity, with no HX cooling, to exercise evaporation from the WS drum.
    tArticle->mInternalFluid->setState(tNodes[0].getOutflow());
    tArticle->mFlowRate         = 0.1;
    tArticle->mSlurperFlowRate  = 0.0001;
    tArticle->mTransferFlowRate = 0.0004;
    tArticle->mWsCondensateMass = 0.01;
    tArticle->mWsDrumSpeed      = 6000.0;

    const FluidProperties* propertiesH2O = tNodes[0].getOutflow()->getProperties(FluidProperties::GUNNS_H2O);
    double expectedEvapRate = tWsEvaporationCoeff
                            * (propertiesH2O->getSaturationPressure(tNodes[0].getOutflow()->getTemperature())
                            - tNodes[0].getOutflow()->getPartialPressure(FluidProperties::GUNNS_H2O));
    double expectedCondMass = 0.01 + tTimeStep * (0.0001 - expectedEvapRate - 0.0004);
    const double expectedPowerCoeff = tWsReferencePressure / tWsReferenceSpeed
                                    / pow(tWsMaxCondensate, tWsMassExponent);
    double expectedDeltaP   = expectedPowerCoeff * 6000.0 * pow(expectedCondMass, tWsMassExponent);
    bool   expectedOverflow = false;

    tArticle->updateWaterSeparator(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEvapRate, tArticle->mWsEvaporationRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCondMass, tArticle->mWsCondensateMass,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDeltaP,   tArticle->mWsDeltaPressure,   DBL_EPSILON);
    CPPUNIT_ASSERT(expectedOverflow == tArticle->mLiquidOverflow);

    /// - Set up and verify a case with saturated air (no evaporation), negative transfer flow rate,
    ///   and liquid overflow.
    tArticle->mInternalFluid->setTemperature(280.0);
    tArticle->mTransferFlowRate = -0.0001;
    tArticle->mWsCondensateMass =  0.11;
    tArticle->mSlurperFlowRate  = 0.0;

    expectedEvapRate = 0.0;
    expectedCondMass = 0.11;
    expectedOverflow = true;

    tArticle->updateWaterSeparator(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEvapRate, tArticle->mWsEvaporationRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCondMass, tArticle->mWsCondensateMass,  DBL_EPSILON);
    CPPUNIT_ASSERT(expectedOverflow == tArticle->mLiquidOverflow);

    /// - Set up and verify a case with no flow, and drum mass limited to zero.
    tArticle->mFlowRate         = 0.0;
    tArticle->mInternalFluid->setTemperature(300.0);
    tArticle->mWsCondensateMass = -0.00001;

    expectedEvapRate = 0.0;
    expectedCondMass = 0.0;
    expectedOverflow = false;

    tArticle->updateWaterSeparator(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEvapRate, tArticle->mWsEvaporationRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCondMass, tArticle->mWsCondensateMass,  DBL_EPSILON);
    CPPUNIT_ASSERT(expectedOverflow == tArticle->mLiquidOverflow);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model processOutputs method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testProcessOutputs()
{
    UT_RESULT;

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tArticle->mWsDeltaPressure = 1.0;
    tArticle->processOutputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getContent()->getTemperature(),
                                 tArticle->mTransferTemperature, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[1].getPotential() + 1.0,
                                 tArticle->mTransferPressure,    DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testInitializationExceptions()
{
    UT_RESULT;

    /// @test    Initialization exception on invalid config data: wet flow factor < 0.
    tConfigData->mHxWetFlowFactor = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mHxWetFlowFactor = tHxWetFlowFactor;

    /// @test    Initialization exception on invalid config data: slurper dry flow mass < DBL_EPSILON.
    tConfigData->mSlurperFlowMassDry = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mSlurperFlowMassDry = tSlurperFlowMassDry;

    /// @test    Initialization exception on invalid config data: slurper wet flow mass < DBL_EPSILON.
    tConfigData->mSlurperFlowMassWet = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mSlurperFlowMassWet = tSlurperFlowMassWet;

    /// @test    Initialization exception on invalid config data: slurper ref flow rate < DBL_EPSILON.
    tConfigData->mSlurperFlowRateRef = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mSlurperFlowRateRef = tSlurperFlowRateRef;

    /// @test    Initialization exception on invalid config data: mWsMassExponent not (0.1 - 10.0).
    tConfigData->mWsMassExponent = 0.0999;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mWsMassExponent = 10.001;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mWsMassExponent = tWsMassExponent;

    /// @test    Initialization exception on invalid config data: WS max mass < FLT_EPSILON.
    tConfigData->mWsMaxCondensate = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mWsMaxCondensate = tWsMaxCondensate;

    /// @test    Initialization exception on invalid config data: WS max mass < FLT_EPSILON.
    tConfigData->mWsMaxCondensate = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mWsMaxCondensate = tWsMaxCondensate;

    /// @test    Initialization exception on invalid config data: ref speed < FLT_EPSILON.
    tConfigData->mWsReferenceSpeed = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mWsReferenceSpeed = tWsReferenceSpeed;

    /// @test    Initialization exception on invalid config data: ref pressure < FLT_EPSILON.
    tConfigData->mWsReferencePressure = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mWsReferencePressure = tWsReferencePressure;

    /// @test    Initialization exception on invalid config data: ref torque > 0.
    tConfigData->mWsReferenceTorque = DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mWsReferenceTorque = tWsReferenceTorque;

    /// @test    Initialization exception on invalid config data: drive ratio < 0.
    tConfigData->mWsDriveRatio = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mWsDriveRatio = tWsDriveRatio;

    /// @test    Initialization exception on invalid config data: HX evaporation coeff < 0.
    tConfigData->mHxEvaporationCoeff = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mHxEvaporationCoeff = tHxEvaporationCoeff;

    /// @test    Initialization exception on invalid config data: WS evaporation coeff < 0.
    tConfigData->mWsEvaporationCoeff = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mWsEvaporationCoeff = tWsEvaporationCoeff;

    /// @test    Initialization exception on invalid input data: separator speed < 0.
    tInputData->mWsMotorSpeed = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mWsMotorSpeed = tWsMotorSpeed;

    /// @test    Initialization exception on invalid input data: WS mass < 0.
    tInputData->mWsCondensateMass = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mWsCondensateMass = tWsCondensateMass;

    /// @test    Initialization exception on invalid input data: HX mass < 0.
    tInputData->mHxCondensateMass = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mHxCondensateMass = tHxCondensateMass;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Condensing HX & Separator link model HX condensation pool rate
///           override malfunction.  Normally condensation pools in the HX at the rate condensed
///           by air cooling.  This is verified in previous tests.  When the malfunction is active,
///           it pools at the malf rate instead.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidCondensingHxSeparator::testPoolRateOverride()
{
    UT_RESULT;

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set up a case identical to Case 0 from the testHxFlow test above to ensure there is actual
    ///   condensation from the air.
    tFractions[0]       = 0.0089373;
    tFractions[1]       = 0.2335056;
    tFractions[2]       = 1.0 - tFractions[0] - tFractions[1];

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[0].getContent()->setMassAndMassFractions(0.0, tFractions);
    tNodes[1].getContent()->setMassAndMassFractions(0.0, tFractions);
    tNodes[0].setPotential(100.45661);
    tNodes[1].setPotential(100.31872);
    tNodes[0].getContent()->setTemperature(292.92778);
    tNodes[1].getContent()->setTemperature(280.20556);

    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tArticle->mPotentialVector[0] = tNodes[0].getPotential();
    tArticle->mPotentialVector[1] = tNodes[1].getPotential();

    tArticle->mSegmentTemperature[0] = 283.87269;
    tArticle->mSegmentTemperature[1] = 282.23354;
    tArticle->mSegmentTemperature[2] = 280.59439;
    tArticle->mSegmentTemperature[3] = 278.95524;

    tArticle->mHxCondensateMass = 0.0;
    tArticle->mSlurperFlowRate  = 0.0001;

    /// - Activate the override malfunction at a typical 1.45 kg/hr rate.
    tArticle->mMalfHxCondensateFlag  = true;
    tArticle->mMalfHxCondensateValue = 0.0004;

    /// - Step the link.
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    tArticle->transportFlows(tTimeStep);

    /// - Verify HX pools at the malf rate.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tTimeStep * (0.0004 - 0.0001),
                                 tArticle->mHxCondensateMass, DBL_EPSILON);

    UT_PASS_LAST;
}
