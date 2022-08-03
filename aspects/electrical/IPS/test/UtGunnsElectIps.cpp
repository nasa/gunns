/**
@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((GunnsElectIps.o))
*/

#include <iostream>
#include "strings/UtResult.hh"
#include "UtGunnsElectIps.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

/// @details  Test identification number.
int UtGunnsElectIps::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectIps class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectIps::UtGunnsElectIps()
    :
    tLinks(),
    tNodeList(),
    tPorts(),
    tName(""),
    tConfigData(0),
    tInputData(0),
    tArticle(0),
    tBackUpPowerImplemented(false),
    tDefaultPowerConsumedOn(0.0),
    tAuxOnePowerConsumedOn(0.0),
    tAuxTwoPowerConsumedOn(0.0),
    tUnderVoltageLimit(0.0),
    tBackUpVoltageThreshold(0.0),
    tPotentialOnTolerance(0.0),
    tThermalFraction(0.0),
    tBackupVoltageMin(0.0),
    tBackupVoltageMax(0.0),
    tConductanceTolerance(0.0),
    tConvergedFrameToCheckVoltage(0),
    tNumberOfVoltageSwitchesInASolution(0),
    tCommandOnUsed(false),
    tUnselectedInputConductance(0.0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectIps class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectIps::~UtGunnsElectIps()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::setUp()
{
    tName = "tArticle";

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    /// - Define the nominal configuration data.
    tBackUpPowerImplemented             = true;
    tDefaultPowerConsumedOn             = 1.0;
    tAuxOnePowerConsumedOn              = 2.0;
    tAuxTwoPowerConsumedOn              = 3.0;
    tUnderVoltageLimit                  = 4.0;
    tBackUpVoltageThreshold             = 5.0;
    tPotentialOnTolerance               = 6.0;
    tThermalFraction                    = 1.0;
    tBackupVoltageMin                   = 8.0;
    tBackupVoltageMax                   = 9.0;
    tConductanceTolerance               = 10.0;
    tConvergedFrameToCheckVoltage       = 1;
    tNumberOfVoltageSwitchesInASolution = 2;
    tCommandOnUsed                      = true;
    tUnselectedInputConductance         = 11.0;
    tConfigData                         = new GunnsElectIpsConfigData(tName,
                                                                      &tNodeList,
                                                                      tBackUpPowerImplemented,
                                                                      tDefaultPowerConsumedOn,
                                                                      tAuxOnePowerConsumedOn,
                                                                      tAuxTwoPowerConsumedOn,
                                                                      tUnderVoltageLimit,
                                                                      tBackUpVoltageThreshold,
                                                                      tPotentialOnTolerance,
                                                                      tThermalFraction,
                                                                      tBackupVoltageMin,
                                                                      tBackupVoltageMax,
                                                                      tConductanceTolerance,
                                                                      tConvergedFrameToCheckVoltage,
                                                                      tNumberOfVoltageSwitchesInASolution,
                                                                      tCommandOnUsed,
                                                                      tUnselectedInputConductance);

    /// - Define the nominal input data.
    tMalfBlockageFlag  = true;
    tMalfBlockageValue = 1.0;
    tInputData                         = new GunnsElectIpsInputData(tMalfBlockageFlag,
                                                                    tMalfBlockageValue);

    /// - Create a vector of port numbers.
    tPorts.clear();
    tPorts.push_back(0);
    tPorts.push_back(1);

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsElectIps;

    for (int i=0; i<5; ++i) {
        tNodeList.mNodes[i].initialize("tNodes", 100.0);
    }

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::tearDown()
{
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of IPS Link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration nominal construction.
    CPPUNIT_ASSERT(tBackUpPowerImplemented == tConfigData->mBackUpPowerImplemented);
    CPPUNIT_ASSERT_EQUAL(tDefaultPowerConsumedOn,             tConfigData->mDefaultPowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(tAuxOnePowerConsumedOn,              tConfigData->mAuxOnePowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(tAuxTwoPowerConsumedOn,              tConfigData->mAuxTwoPowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(tUnderVoltageLimit,                  tConfigData->mUnderVoltageLimit);
    CPPUNIT_ASSERT_EQUAL(tBackUpVoltageThreshold,             tConfigData->mBackUpVoltageThreshold);
    CPPUNIT_ASSERT_EQUAL(tPotentialOnTolerance,               tConfigData->mPotentialOnTolerance);
    CPPUNIT_ASSERT_EQUAL(tThermalFraction,                    tConfigData->mThermalFraction);
    CPPUNIT_ASSERT_EQUAL(tBackupVoltageMin,                   tConfigData->mBackupVoltageMin);
    CPPUNIT_ASSERT_EQUAL(tBackupVoltageMax,                   tConfigData->mBackupVoltageMax);
    CPPUNIT_ASSERT_EQUAL(tConductanceTolerance,               tConfigData->mConductanceTolerance);
    CPPUNIT_ASSERT_EQUAL(tConvergedFrameToCheckVoltage,       tConfigData->mConvergedFrameToCheckVoltage);
    CPPUNIT_ASSERT_EQUAL(tNumberOfVoltageSwitchesInASolution, tConfigData->mNumberOfVoltageSwitchesInASolution);
    CPPUNIT_ASSERT(tCommandOnUsed == tConfigData->mCommandOnUsed);
    CPPUNIT_ASSERT_EQUAL(tUnselectedInputConductance,         tConfigData->mUnselectedInputConductance);

    /// @test    Configuration default construction.
    GunnsElectIpsConfigData defaultConfig;
    CPPUNIT_ASSERT(false == defaultConfig.mBackUpPowerImplemented);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mDefaultPowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mAuxOnePowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mAuxTwoPowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mUnderVoltageLimit);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mBackUpVoltageThreshold);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mPotentialOnTolerance);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mThermalFraction);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mBackupVoltageMin);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mBackupVoltageMax);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mConductanceTolerance);
    CPPUNIT_ASSERT_EQUAL(0,   defaultConfig.mConvergedFrameToCheckVoltage);
    CPPUNIT_ASSERT_EQUAL(0,   defaultConfig.mNumberOfVoltageSwitchesInASolution);
    CPPUNIT_ASSERT(false == defaultConfig.mCommandOnUsed);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mUnselectedInputConductance);

    /// @test    Configuration copy construction.
    GunnsElectIpsConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tBackUpPowerImplemented == copyConfig.mBackUpPowerImplemented);
    CPPUNIT_ASSERT_EQUAL(tDefaultPowerConsumedOn,             copyConfig.mDefaultPowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(tAuxOnePowerConsumedOn,              copyConfig.mAuxOnePowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(tAuxTwoPowerConsumedOn,              copyConfig.mAuxTwoPowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(tUnderVoltageLimit,                  copyConfig.mUnderVoltageLimit);
    CPPUNIT_ASSERT_EQUAL(tBackUpVoltageThreshold,             copyConfig.mBackUpVoltageThreshold);
    CPPUNIT_ASSERT_EQUAL(tPotentialOnTolerance,               copyConfig.mPotentialOnTolerance);
    CPPUNIT_ASSERT_EQUAL(tThermalFraction,                    copyConfig.mThermalFraction);
    CPPUNIT_ASSERT_EQUAL(tBackupVoltageMin,                   copyConfig.mBackupVoltageMin);
    CPPUNIT_ASSERT_EQUAL(tBackupVoltageMax,                   copyConfig.mBackupVoltageMax);
    CPPUNIT_ASSERT_EQUAL(tConductanceTolerance,               copyConfig.mConductanceTolerance);
    CPPUNIT_ASSERT_EQUAL(tConvergedFrameToCheckVoltage,       copyConfig.mConvergedFrameToCheckVoltage);
    CPPUNIT_ASSERT_EQUAL(tNumberOfVoltageSwitchesInASolution, copyConfig.mNumberOfVoltageSwitchesInASolution);
    CPPUNIT_ASSERT(tCommandOnUsed == copyConfig.mCommandOnUsed);
    CPPUNIT_ASSERT_EQUAL(tUnselectedInputConductance,         copyConfig.mUnselectedInputConductance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of IPS Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testInput()
{
    UT_RESULT;

    /// @test    Input nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageValue, tInputData->mMalfBlockageValue);

    /// @test    Input default construction.
    GunnsElectIpsInputData defaultInput;
    CPPUNIT_ASSERT(false == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.mMalfBlockageValue);

    /// @test    Input copy construction.
    GunnsElectIpsInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageValue, copyInput.mMalfBlockageValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsElectIps class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testConstruction()
{
    UT_RESULT;

    /// @test    Default construction.
    CPPUNIT_ASSERT(0     == tArticle->mMalfPowerInput);
    CPPUNIT_ASSERT(false == tArticle->mMalfAllPowerInputs);
    CPPUNIT_ASSERT(false == tArticle->mMalfBiasPowerConsumedFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfBiasPowerConsumedValue);
    CPPUNIT_ASSERT(0.0   == tArticle->mHeatGeneratedOn);
    CPPUNIT_ASSERT(0.0   == tArticle->mHeatGenerated);
    CPPUNIT_ASSERT(0     == tArticle->mConductance);
    CPPUNIT_ASSERT(0     == tArticle->mActiveConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mPotentialOnTolerance);
    CPPUNIT_ASSERT(0     == tArticle->mOutputPower);
    CPPUNIT_ASSERT(false == tArticle->mPowerValid);
    CPPUNIT_ASSERT(0     == tArticle->mPowerSupplyVoltage);
    CPPUNIT_ASSERT(0     == tArticle->mActivePowerSource);
    CPPUNIT_ASSERT(0     == tArticle->mLastActivePowerSource);
    CPPUNIT_ASSERT(0.0   == tArticle->mDefaultPowerConsumedOn);
    CPPUNIT_ASSERT(0.0   == tArticle->mPowerConsumedOn);
    CPPUNIT_ASSERT(0.0   == tArticle->mUnderVoltageLimit);
    CPPUNIT_ASSERT(0.0   == tArticle->mAuxOnePowerConsumedOn);
    CPPUNIT_ASSERT(0.0   == tArticle->mAuxTwoPowerConsumedOn);
    CPPUNIT_ASSERT(0     == tArticle->mNumberOfPowerSources);
    CPPUNIT_ASSERT(GunnsElectIps::INVALID_SOURCE == tArticle->mBackUpPowerSource);
    CPPUNIT_ASSERT(0.0   == tArticle->mBackUpVoltageThreshold);
    CPPUNIT_ASSERT(0.0   == tArticle->mTotalPowerLoad);
    CPPUNIT_ASSERT(0.0   == tArticle->mBackupVoltageMin);
    CPPUNIT_ASSERT(0.0   == tArticle->mBackupVoltageMax);
    CPPUNIT_ASSERT(false == tArticle->mBackupVoltageInRange);
    CPPUNIT_ASSERT(0.0   == tArticle->mLoadAddedFromInstructor);
    CPPUNIT_ASSERT(0.0   == tArticle->mConductanceTolerance);
    CPPUNIT_ASSERT(0     == tArticle->mNumberOfVoltageSwitchesInASolution);
    CPPUNIT_ASSERT(0     == tArticle->mVoltageSwitches);
    CPPUNIT_ASSERT(false == tArticle->mControlInputPowerInUse);
    CPPUNIT_ASSERT(false == tArticle->mCommandOnUsed);
    CPPUNIT_ASSERT(false == tArticle->mCommandOn);
    CPPUNIT_ASSERT(false == tArticle->mVerbose);
    CPPUNIT_ASSERT(0.0   == tArticle->mThermalFraction);
    CPPUNIT_ASSERT(0     == tArticle->mVoltageSwitches);
    CPPUNIT_ASSERT(0     == tArticle->mInputCurrent);
    CPPUNIT_ASSERT(0     == tArticle->mInputVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mUnselectedInputConductance);

    /// @test    New/delete for code coverage.
    GunnsElectIps* testArticle = new GunnsElectIps();
    delete testArticle;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for IPS Link nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testInitialize()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &tPorts));

    CPPUNIT_ASSERT_EQUAL(2, tArticle->mNumberOfPowerSources);
    CPPUNIT_ASSERT_EQUAL(2, tArticle->mNumPorts);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mPowerSupplyVoltage[0]);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mPowerSupplyVoltage[1]);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mInputCurrent[0]);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mInputCurrent[1]);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mInputVoltage[0]);
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mInputVoltage[1]);

    CPPUNIT_ASSERT_EQUAL(tThermalFraction, tArticle->mThermalFraction);
    CPPUNIT_ASSERT_EQUAL(tDefaultPowerConsumedOn, tArticle->mDefaultPowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(tDefaultPowerConsumedOn, tArticle->mPowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(tUnderVoltageLimit, tArticle->mUnderVoltageLimit);
    CPPUNIT_ASSERT_EQUAL(tAuxOnePowerConsumedOn, tArticle->mAuxOnePowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(tAuxTwoPowerConsumedOn, tArticle->mAuxTwoPowerConsumedOn);
    CPPUNIT_ASSERT_EQUAL(tPotentialOnTolerance, tArticle->mPotentialOnTolerance);
    CPPUNIT_ASSERT_EQUAL(tBackUpVoltageThreshold, tArticle->mBackUpVoltageThreshold);
    CPPUNIT_ASSERT_EQUAL(tBackUpPowerImplemented, tArticle->mBackUpPowerImplemented);
    CPPUNIT_ASSERT_EQUAL(tBackupVoltageMin, tArticle->mBackupVoltageMin);
    CPPUNIT_ASSERT_EQUAL(tBackupVoltageMax, tArticle->mBackupVoltageMax);
    CPPUNIT_ASSERT_EQUAL(tConductanceTolerance, tArticle->mConductanceTolerance);
    CPPUNIT_ASSERT_EQUAL(tNumberOfVoltageSwitchesInASolution, tArticle->mNumberOfVoltageSwitchesInASolution);
    CPPUNIT_ASSERT_EQUAL(tCommandOnUsed, tArticle->mCommandOnUsed);
    CPPUNIT_ASSERT_EQUAL(tUnselectedInputConductance, tArticle->mUnselectedInputConductance);
    CPPUNIT_ASSERT_EQUAL(0, tArticle->mVoltageSwitches);
    CPPUNIT_ASSERT_DOUBLES_EQUAL((tDefaultPowerConsumedOn + tAuxOnePowerConsumedOn + tAuxTwoPowerConsumedOn),
                                 tArticle->mTotalPowerLoad, DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL(1, tArticle->mBackUpPowerSource);
    CPPUNIT_ASSERT(tName == tArticle->mName);
    CPPUNIT_ASSERT(true  == tArticle->mPowerValid);
    CPPUNIT_ASSERT(true  == tArticle->mInitFlag);

    /// @test mBackupPowerImplemented = false.
    FriendlyGunnsElectIps article;
    tConfigData->mBackUpPowerImplemented = false;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*tConfigData, *tInputData, tLinks, &tPorts));
    CPPUNIT_ASSERT_EQUAL(-1, article.mBackUpPowerSource);
    CPPUNIT_ASSERT(tName == article.mName);
    CPPUNIT_ASSERT(true  == article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for IPS Link initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testInitializationErrors()
{
    UT_RESULT;

    /// @test Exception thrown for number of power sources < 1.
    {
        std::vector<int> ports;
        CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    }

    /// @test Exception thrown on number of power sources < 2 with backup power implemented.
    {
        std::vector<int> ports;
        ports.push_back(0);
        CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, &ports), TsInitializationException);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for IPS Link step methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testStep()
{
    UT_RESULT;

    FriendlyGunnsElectIps article;
    GunnsElectIpsConfigData configData(tName, &tNodeList, true, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0,
                                  9.0, 10.0, 1.0e-15, 0, 0, false, 0.001);
    GunnsElectIpsInputData inputData(false, 0.0);

    tPorts.clear();
    tPorts.push_back(1);
    tPorts.push_back(2);
    tPorts.push_back(3);
    article.initialize(configData, inputData, tLinks, &tPorts);

    article.mPotentialVector[0] = 100.0;
    article.mPotentialVector[1] = 120.0;
    article.mPotentialVector[2] = 101.0;

    article.step(0.1);
    double expectedInputPower = 2.0 + 3.0 + 4.0;
    double expectedCurrent    = expectedInputPower / 100.0;
    double expectedAdmittance = expectedCurrent / 100.0;
    CPPUNIT_ASSERT_EQUAL(0, article.mActivePowerSource);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance, article.mConductance[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,              article.mConductance[1],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,              article.mConductance[2],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance, article.mActiveConductance[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,              article.mActiveConductance[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,              article.mActiveConductance[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance, article.mAdmittanceMatrix[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                article.mAdmittanceMatrix[1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                article.mAdmittanceMatrix[2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                article.mAdmittanceMatrix[3],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,              article.mAdmittanceMatrix[4],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                article.mAdmittanceMatrix[5],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                article.mAdmittanceMatrix[6],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                article.mAdmittanceMatrix[7],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,              article.mAdmittanceMatrix[8],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInputPower, article.mTotalPowerLoad,       DBL_EPSILON);

    /// @test    minorStep method, and with a different selected input channel, and with instructor
    ///          override of power load.
    expectedInputPower               = 10.0;
    article.mActivePowerSource       = 2;
    article.mLoadAddedFromInstructor = expectedInputPower;
    article.minorStep(0.1, 1);
    expectedCurrent    = expectedInputPower / 101.0;
    expectedAdmittance = expectedCurrent / 101.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,              article.mConductance[0],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,              article.mConductance[1],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance, article.mConductance[2],       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,              article.mActiveConductance[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,              article.mActiveConductance[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance, article.mActiveConductance[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,              article.mAdmittanceMatrix[0],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                article.mAdmittanceMatrix[1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                article.mAdmittanceMatrix[2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                article.mAdmittanceMatrix[3],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,              article.mAdmittanceMatrix[4],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                article.mAdmittanceMatrix[5],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                article.mAdmittanceMatrix[6],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                article.mAdmittanceMatrix[7],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance, article.mAdmittanceMatrix[8],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInputPower, article.mTotalPowerLoad,       DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for IPS Link setter & getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testSetterGetter()
{
    UT_RESULT;

    FriendlyGunnsElectIps article;
    GunnsElectIpsConfigData configData(tName, &tNodeList, true, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0,
                                  9.0, 10.0, 0.00001);
    GunnsElectIpsConfigData copyConfigData(configData);
    GunnsElectIpsInputData inputData(false, 0.0);

    tPorts.clear();
    tPorts.push_back(1);
    tPorts.push_back(2);
    article.initialize(configData, inputData, tLinks, &tPorts);

    /// @test    isNonLinear method.
    CPPUNIT_ASSERT(true == article.isNonLinear());

    /// @test    setOutputPower method.
    double power = 1.0;
    article.setOutputPower(power);
    CPPUNIT_ASSERT_EQUAL(power, article.mPowerConsumedOn);
    article.setOutputPower(-1.0);
    CPPUNIT_ASSERT_EQUAL(power, article.mPowerConsumedOn);

    /// @test    setUnderVoltageLimit method.
    double voltage = 1.0;
    article.setUnderVoltageLimit(voltage);
    CPPUNIT_ASSERT_EQUAL(voltage, article.mUnderVoltageLimit);
    article.setUnderVoltageLimit(-1.0);
    CPPUNIT_ASSERT_EQUAL(voltage, article.mUnderVoltageLimit);

    /// @test    setOutputPowerAuxiliaryOne method.
    article.setOutputPowerAuxiliaryOne(power);
    CPPUNIT_ASSERT_EQUAL(power, article.mAuxOnePowerConsumedOn);
    article.setOutputPowerAuxiliaryOne(-1.0);
    CPPUNIT_ASSERT_EQUAL(power, article.mAuxOnePowerConsumedOn);

    /// @test    setOutputPowerAuxiliaryTwo method.
    article.setOutputPowerAuxiliaryTwo(power);
    CPPUNIT_ASSERT_EQUAL(power, article.mAuxTwoPowerConsumedOn);
    article.setOutputPowerAuxiliaryTwo(-1.0);
    CPPUNIT_ASSERT_EQUAL(power, article.mAuxTwoPowerConsumedOn);

    /// @test    isPowerSupplyOn method.
    article.mPowerValid = false;
    CPPUNIT_ASSERT(false == article.isPowerSupplyOn());
    article.mPowerValid = true;
    CPPUNIT_ASSERT(true == article.isPowerSupplyOn());

    /// @test    getPotentialValue method.
    /// @note    This method isn't public so it's doubtful this is ever used.
    article.mPotentialOnTolerance = 10.0;
    CPPUNIT_ASSERT_EQUAL(90.0, article.getPotentialValue(100.0));
    CPPUNIT_ASSERT_EQUAL( 0.0, article.getPotentialValue( -1.0));

    /// @test    setMalfPowerInput method.
    article.setMalfPowerInput(0, true);
    CPPUNIT_ASSERT(true  == article.mMalfPowerInput[0]);
    article.setMalfPowerInput(-1, false);
    CPPUNIT_ASSERT(true  == article.mMalfPowerInput[0]);
    article.setMalfPowerInput(3, true);
    CPPUNIT_ASSERT(false == article.mMalfPowerInput[1]);
    article.setMalfPowerInput(0);
    CPPUNIT_ASSERT(false == article.mMalfPowerInput[0]);

    /// @test    setMalfAllPowerInputs method.
    article.setMalfAllPowerInputs(true);
    CPPUNIT_ASSERT(true  == article.mMalfAllPowerInputs);
    article.setMalfAllPowerInputs();
    CPPUNIT_ASSERT(false == article.mMalfAllPowerInputs);

    /// @test    getPsHeatGenerated method.
    article.mHeatGenerated = 10.0;
    CPPUNIT_ASSERT_EQUAL(10.0, article.getPsHeatGenerated());

    /// @test    getOutputPower method.
    article.mPowerConsumedOn = 10.0;
    CPPUNIT_ASSERT_EQUAL(10.0, article.getOutputPower());

    /// @test    commandOn, commandOff, isPowerCommandedOn methods.
    article.mCommandOnUsed = false;
    article.commandOn();
    CPPUNIT_ASSERT(false == article.isPowerCommandedOn());
    article.commandOff();
    CPPUNIT_ASSERT(false == article.isPowerCommandedOn());
    article.mCommandOnUsed = true;
    article.commandOn();
    CPPUNIT_ASSERT(true  == article.isPowerCommandedOn());
    article.commandOff();
    CPPUNIT_ASSERT(false == article.isPowerCommandedOn());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for IPS Link confirmSolutionAcceptable method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps:: testConfirmSolutionAcceptable()
{
    UT_RESULT;

    FriendlyGunnsElectIps article;
    GunnsElectIpsConfigData configData("TEST", &tNodeList, false, 2.0, 3.0, 4.0, 99.0, 6.0, 7.0, 8.0,
                                  9.0,10.0,0.00001, 0, 2);
    GunnsElectIpsInputData inputData(false,0.0);

    tPorts.clear();
    tPorts.push_back(1);
    tPorts.push_back(2);
    tPorts.push_back(3);
    article.initialize(configData, inputData, tLinks, &tPorts);

    /// - Set the verbose flag for code coverage.
    article.mVerbose = true;

    /// @test    confirms a good solution with no input switching.
    article.mPotentialVector[0] = 120.0;
    article.mPotentialVector[1] = 100.0;
    article.mPotentialVector[2] = 101.0;
    article.step(0.1);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == article.confirmSolutionAcceptable(1,1));

    /// @test    rejects when there is a negative input voltage.
    article.mPotentialVector[2] = -100.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT  == article.confirmSolutionAcceptable(1,1));

    /// @test    rejects when it switches inputs.
    article.mPotentialVector[0] =  90.0;
    article.mPotentialVector[2] = 101.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT  == article.confirmSolutionAcceptable(1,1));

    /// @test    rejects when it switches inputs, backup power implemented.
    article.mBackUpPowerImplemented = true;
    article.mPotentialVector[0] = 120.0;
    article.mPotentialVector[2] = 101.0;
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT  == article.confirmSolutionAcceptable(1,1));

    /// @test    doesn't reject when we've reached the number of allowed switch events.
    article.mPotentialVector[0] = 120.0;
    article.mPotentialVector[1] =   0.0;
    article.mPotentialVector[2] =   0.0;
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == article.confirmSolutionAcceptable(1,1));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for IPS Link computeFlows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps:: testComputeFlows()
{
    UT_RESULT;

    FriendlyGunnsElectIps article;
    GunnsElectIpsConfigData configData(tName, &tNodeList, true, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0,
                                  90.0, 130.0, 1.0e-15, 0, 0, false, 0.001);
    GunnsElectIpsInputData inputData(false, 0.0);

    tPorts.clear();
    tPorts.push_back(1);
    tPorts.push_back(2);
    tPorts.push_back(3);
    article.initialize(configData, inputData, tLinks, &tPorts);

    article.mPotentialVector[0] = 100.0;
    article.mPotentialVector[1] = 120.0;
    article.mPotentialVector[2] = 101.0;
    article.mThermalFraction    = 0.1;

    article.step(0.1);
    article.confirmSolutionAcceptable(1, 1);
    article.computeFlows(0.1);

    // check solution result
    double expectedInputPower = 2.0 + 3.0 + 4.0;
    double expectedCurrent = expectedInputPower / 100.0;
    double expectedHeat    = 2.0 * 0.1;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,    article.mFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedInputPower, article.mPower,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat,       article.mHeatGenerated, DBL_EPSILON);

    /// @test    Power consumed bias malfunction.
    double powerBias                      = 5.0;
    article.mMalfBiasPowerConsumedFlag      = true;
    article.mMalfBiasPowerConsumedValue = powerBias;
    expectedInputPower += powerBias;
    article.step(0.1);
    article.confirmSolutionAcceptable(1, 1);
    article.computeFlows(0.1);
    expectedHeat = (2.0 + powerBias) * 0.1;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat,    article.mHeatGenerated, DBL_EPSILON);

    /// @test    Control power in use.
    CPPUNIT_ASSERT(false == article.mControlInputPowerInUse);
    article.calculateFlow(2);
    CPPUNIT_ASSERT(true  == article.mControlInputPowerInUse);

    /// @test    zero heat with power valid off.
    article.mPotentialVector[0] = 0.0;
    article.mPotentialVector[1] = 0.0;
    article.mPotentialVector[2] = 0.0;
    article.step(0.1);
    article.confirmSolutionAcceptable(1, 1);
    article.computeFlows(0.1);
    CPPUNIT_ASSERT_EQUAL(0.0, article.mHeatGenerated);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for IPS Link validate method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testValidate()
{
    UT_RESULT;

    FriendlyGunnsElectIps article;
    GunnsElectIpsConfigData configData(tName, &tNodeList, false, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0,
                                     9.0, 10.0, 0.00001);
    GunnsElectIpsInputData inputData(false, 0.0);

    tPorts.clear();
    tPorts.push_back(1);
    tPorts.push_back(2);
    article.initialize(configData, inputData, tLinks, &tPorts);
    article.mAuxOnePowerConsumedOn = -1.0;
    CPPUNIT_ASSERT_THROW(article.validate(), TsOutOfBoundsException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for IPS Link getVoltagePrimarySourceGreaterThanBackupThreshold method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testGetVoltagePrimarySourceGreaterThanBackupThreshold()
{
    UT_RESULT;

    FriendlyGunnsElectIps article;
    GunnsElectIpsConfigData configData(tName, &tNodeList, true, 2.0, 0.0, 0.0, 65.0, 119.0, 0.5);
    GunnsElectIpsInputData inputData(false, 0.0);

    tPorts.clear();
    tPorts.push_back(1);
    tPorts.push_back(2);
    tPorts.push_back(3);
    article.initialize(configData, inputData, tLinks, &tPorts);

    article.mPotentialVector[0] = 100.0;
    article.mPotentialVector[1] = 120.0;
    article.mPotentialVector[2] = 101.0;
    CPPUNIT_ASSERT_EQUAL(1, article.getVoltagePrimarySourceGreaterThanBackupThreshold());

    article.mPotentialVector[0] = 100.0;
    article.mPotentialVector[1] = 100.0;
    article.mPotentialVector[2] = 101.0;
    CPPUNIT_ASSERT_EQUAL(-1, article.getVoltagePrimarySourceGreaterThanBackupThreshold());

    article.mPotentialVector[0] = 124.0;
    article.mPotentialVector[1] = 143.0;
    article.mPotentialVector[2] = 159.0;
    CPPUNIT_ASSERT_EQUAL(1, article.getVoltagePrimarySourceGreaterThanBackupThreshold());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for IPS Link getVoltagePrimarySourceGreaterUnderVoltageLimit method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testGetVoltagePrimarySourceGreaterUnderVoltageLimit()
{
    UT_RESULT;

    FriendlyGunnsElectIps article;
    GunnsElectIpsConfigData configData(tName, &tNodeList, true, 2.0, 0.0, 0.0, 65.0, 119.0, 0.5);
    GunnsElectIpsInputData inputData(false, 0.0);

    tPorts.clear();
    tPorts.push_back(1);
    tPorts.push_back(2);
    tPorts.push_back(3);
    article.initialize(configData, inputData, tLinks, &tPorts);

    article.mPotentialVector[0] = 100.0;
    article.mPotentialVector[1] =  60.0;
    article.mPotentialVector[2] = 101.0;
    CPPUNIT_ASSERT_EQUAL(2, article.getVoltagePrimarySourceGreaterUnderVoltageLimit());

    article.mPotentialVector[0] = 100.0;
    article.mPotentialVector[1] = 100.0;
    article.mPotentialVector[2] = 100.0;
    CPPUNIT_ASSERT_EQUAL(0, article.getVoltagePrimarySourceGreaterUnderVoltageLimit());

    article.mPotentialVector[0] = 170.0;
    article.mPotentialVector[1] = 143.0;
    article.mPotentialVector[2] = 159.0;
    CPPUNIT_ASSERT_EQUAL(0, article.getVoltagePrimarySourceGreaterUnderVoltageLimit());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for IPS Link testGetVoltageSourceToUseWithBackup method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testGetVoltageSourceToUseWithBackup()
{
    UT_RESULT;

    FriendlyGunnsElectIps article;
    GunnsElectIpsConfigData configData(tName, &tNodeList, true, 2.0, 0.0, 0.0, 65.0, 119.0, 0.5);
    GunnsElectIpsInputData inputData(false, 0.0);

    tPorts.clear();
    tPorts.push_back(1);
    tPorts.push_back(2);
    tPorts.push_back(3);
    article.initialize(configData, inputData, tLinks, &tPorts);

    article.mPotentialVector[0] = 100.0;
    article.mPotentialVector[1] = 60.0;
    article.mPotentialVector[2] = 101.0;
    CPPUNIT_ASSERT_EQUAL(2, article.getVoltageSourceToUseWithBackup());

    article.mPotentialVector[0] = 100.0;
    article.mPotentialVector[1] = 100.0;
    article.mPotentialVector[2] = 100.0;
    CPPUNIT_ASSERT_EQUAL(0, article.getVoltageSourceToUseWithBackup());

    article.mMalfPowerInput[0] = true;
    article.mPotentialVector[0] = 170.0;
    article.mPotentialVector[1] = 143.0;
    article.mPotentialVector[2] = 159.0;
    CPPUNIT_ASSERT_EQUAL(1, article.getVoltageSourceToUseWithBackup());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for IPS Link testGetVoltageSourceToUseWithoutBackup method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testGetVoltageSourceToUseWithoutBackup()
{
    UT_RESULT;

    FriendlyGunnsElectIps article;
    GunnsElectIpsConfigData configData(tName, &tNodeList, true, 2.0, 0.0, 0.0, 65.0, 119.0, 0.5);
    GunnsElectIpsInputData inputData(false, 0.0);

    tPorts.clear();
    tPorts.push_back(1);
    tPorts.push_back(2);
    tPorts.push_back(3);
    article.initialize(configData, inputData, tLinks, &tPorts);

    article.mPotentialVector[0] = 100.0;
    article.mPotentialVector[1] =  60.0;
    article.mPotentialVector[2] = 101.0;
    CPPUNIT_ASSERT_EQUAL(2, article.getVoltageSourceToUseWithoutBackup());

    article.mPotentialVector[0] = 100.0;
    article.mPotentialVector[1] = 100.0;
    article.mPotentialVector[2] = 100.0;
    CPPUNIT_ASSERT_EQUAL(0, article.getVoltageSourceToUseWithoutBackup());

    article.mActivePowerSource = 1;
    article.mPotentialVector[0] = 170.0;
    article.mPotentialVector[1] = 143.0;
    article.mPotentialVector[2] = 159.0;
    CPPUNIT_ASSERT_EQUAL(0, article.getVoltageSourceToUseWithoutBackup());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for IPS Link restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectIps::testRestart()
{
    UT_RESULT;

    FriendlyGunnsElectIps article;
    GunnsElectIpsConfigData configData(tName, &tNodeList);
    GunnsElectIpsInputData inputData;

    tPorts.clear();
    tPorts.push_back(1);
    article.initialize(configData, inputData, tLinks, &tPorts);

    article.mUserPortSelect = 12;
    article.restart();
    CPPUNIT_ASSERT_EQUAL(-1, article.mUserPortSelect);

    UT_PASS_LAST;
}
