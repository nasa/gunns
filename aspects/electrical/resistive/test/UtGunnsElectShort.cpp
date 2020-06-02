/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((aspects/electrical/Switch/GunnsElectShort.o))
*/

#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"
#include "UtGunnsElectShort.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Electrical Short link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectShort::UtGunnsElectShort()
    :
    CppUnit::TestFixture(),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tMalfMinimumVoltage(0.0),
    tMalfConstantPowerFlag(false),
    tMalfConstantPowerValue(0.0),
    tMalfConstantCurrentFlag(false),
    tMalfConstantCurrentValue(0.0),
    tMalfInitialPowerFlag(false),
    tMalfInitialPowerValue(0.0),
    tMalfInitialCurrentFlag(false),
    tMalfInitialCurrentValue(0.0),
    tMalfResistanceFlag(false),
    tMalfResistanceValue(0.0),
    tInputData(0),
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical Short link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectShort::~UtGunnsElectShort()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsElectShort::setUp()
{
    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", 200.0);
    tNodes[1].initialize("UtNode2", 100.0);
    tNodes[2].initialize("UtNode3",   0.0);

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    /// - Define the nominal configuration data.
    tName               = "nominal";
    tConfigData         = new GunnsElectShortConfigData(tName,
                                                        &tNodeList);

    /// - Define the nominal input data.
    tMalfBlockageFlag         = true;
    tMalfBlockageValue        = 0.3;
    tMalfMinimumVoltage       = 3.0;
    tMalfConstantPowerFlag    = true;
    tMalfConstantPowerValue   = 2.0;
    tMalfConstantCurrentFlag  = true;
    tMalfConstantCurrentValue = 1.0;
    tMalfInitialPowerFlag     = true;
    tMalfInitialPowerValue    = 4.0;
    tMalfInitialCurrentFlag   = true;
    tMalfInitialCurrentValue  = 5.0;
    tMalfResistanceFlag       = true;
    tMalfResistanceValue      = 10.0;
    tInputData                = new GunnsElectShortInputData(tMalfBlockageFlag,
                                                             tMalfBlockageValue,
                                                             tMalfMinimumVoltage,
                                                             tMalfConstantPowerFlag,
                                                             tMalfConstantPowerValue,
                                                             tMalfConstantCurrentFlag,
                                                             tMalfConstantCurrentValue,
                                                             tMalfInitialPowerFlag,
                                                             tMalfInitialPowerValue,
                                                             tMalfInitialCurrentFlag,
                                                             tMalfInitialCurrentValue,
                                                             tMalfResistanceFlag,
                                                             tMalfResistanceValue);

    /// - Define the nominal port mapping.
    tPort0 = 0;
    tPort1 = 1;

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsElectShort;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectShort::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Short link model construction of configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectShort::testConfig()
{
    std::cout << "\n ----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsElectShort ..... 01: testConfig ............................";

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                          == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                         == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_EQUAL(0.0,                        tConfigData->mDefaultConductivity);

    /// @test    Configuration data default construction.
    GunnsElectShortConfigData defaultConfig;
    CPPUNIT_ASSERT(0                              == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                              == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_EQUAL(0.0,                        defaultConfig.mDefaultConductivity);

    /// @test    Configuration data copy construction.
    GunnsElectShortConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tConfigData->mName             == copyConfig.mName);
    CPPUNIT_ASSERT(tConfigData->mNodeList->mNodes == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_EQUAL(0.0,                        copyConfig.mDefaultConductivity);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Short link model construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectShort::testInput()
{
    std::cout << "\n UtGunnsElectShort ..... 02: testInput .............................";

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageFlag,         tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageValue,        tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(tMalfMinimumVoltage,       tInputData->mShort.mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantPowerFlag,    tInputData->mShort.mMalfConstantPowerFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantPowerValue,   tInputData->mShort.mMalfConstantPowerValue);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantCurrentFlag,  tInputData->mShort.mMalfConstantCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantCurrentValue, tInputData->mShort.mMalfConstantCurrentValue);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialPowerFlag,     tInputData->mShort.mMalfInitialPowerFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialPowerValue,    tInputData->mShort.mMalfInitialPowerValue);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialCurrentFlag,   tInputData->mShort.mMalfInitialCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialCurrentValue,  tInputData->mShort.mMalfInitialCurrentValue);
    CPPUNIT_ASSERT_EQUAL(tMalfResistanceFlag,       tInputData->mShort.mMalfResistanceFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfResistanceValue,      tInputData->mShort.mMalfResistanceValue);

    /// @test    Input data default construction.
    GunnsElectShortInputData defaultInput;
    CPPUNIT_ASSERT_EQUAL(false,                     defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mShort.mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(false,                     defaultInput.mShort.mMalfConstantPowerFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mShort.mMalfConstantPowerValue);
    CPPUNIT_ASSERT_EQUAL(false,                     defaultInput.mShort.mMalfConstantCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mShort.mMalfConstantCurrentValue);
    CPPUNIT_ASSERT_EQUAL(false,                     defaultInput.mShort.mMalfInitialPowerFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mShort.mMalfInitialPowerValue);
    CPPUNIT_ASSERT_EQUAL(false,                     defaultInput.mShort.mMalfInitialCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mShort.mMalfInitialCurrentValue);
    CPPUNIT_ASSERT_EQUAL(false,                     defaultInput.mShort.mMalfResistanceFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,                       defaultInput.mShort.mMalfResistanceValue);

    /// @test    Input data copy construction.
    GunnsElectShortInputData copyInput(*tInputData);
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageFlag,         copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageValue,        copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(tMalfMinimumVoltage,       copyInput.mShort.mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantPowerFlag,    copyInput.mShort.mMalfConstantPowerFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantPowerValue,   copyInput.mShort.mMalfConstantPowerValue);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantCurrentFlag,  copyInput.mShort.mMalfConstantCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantCurrentValue, copyInput.mShort.mMalfConstantCurrentValue);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialPowerFlag,     copyInput.mShort.mMalfInitialPowerFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialPowerValue,    copyInput.mShort.mMalfInitialPowerValue);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialCurrentFlag,   copyInput.mShort.mMalfInitialCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialCurrentValue,  copyInput.mShort.mMalfInitialCurrentValue);
    CPPUNIT_ASSERT_EQUAL(tMalfResistanceFlag,       copyInput.mShort.mMalfResistanceFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfResistanceValue,      copyInput.mShort.mMalfResistanceValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Short link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectShort::testDefaultConstruction()
{
    std::cout << "\n UtGunnsElectShort ..... 03: testDefaultConstruction ...............";

    /// @test    Default of attributes.
    CPPUNIT_ASSERT(""        == tArticle->mName);
    CPPUNIT_ASSERT(0         == tArticle->mNodes);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mDefaultConductivity);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mShort.mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mShort.mMalfConstantPowerFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mShort.mMalfConstantPowerValue);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsElectShort* article = new GunnsElectShort();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Short link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectShort::testNominalInitialization()
{
    std::cout << "\n UtGunnsElectShort ..... 04: testNominalInitialization .............";

    /// - Initialize the test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                         == tArticle->mName);
    CPPUNIT_ASSERT(&tNodes[0]                    == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]                    == tArticle->mNodes[1]);
    CPPUNIT_ASSERT_EQUAL(0.0,                       tArticle->mDefaultConductivity);
    CPPUNIT_ASSERT_EQUAL(tMalfMinimumVoltage,       tArticle->mShort.mMalfMinimumVoltage);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantPowerFlag,    tArticle->mShort.mMalfConstantPowerFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantPowerValue,   tArticle->mShort.mMalfConstantPowerValue);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantCurrentFlag,  tArticle->mShort.mMalfConstantCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfConstantCurrentValue, tArticle->mShort.mMalfConstantCurrentValue);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialPowerFlag,     tArticle->mShort.mMalfInitialPowerFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialPowerValue,    tArticle->mShort.mMalfInitialPowerValue);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialCurrentFlag,   tArticle->mShort.mMalfInitialCurrentFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfInitialCurrentValue,  tArticle->mShort.mMalfInitialCurrentValue);
    CPPUNIT_ASSERT_EQUAL(tMalfResistanceFlag,       tArticle->mShort.mMalfResistanceFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfResistanceValue,      tArticle->mShort.mMalfResistanceValue);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Short link model step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectShort::testStep()
{
    std::cout << "\n UtGunnsElectShort ..... 05: testStep ..............................";

    /// - Initialize the test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    {
        /// @test step method in a non-linear malf mode.
        tArticle->mPotentialVector[tPort0] = tNodes[tPort0].getPotential();
        tArticle->mPotentialVector[tPort1] = tNodes[tPort1].getPotential();
        tArticle->step(0.0);
        const double voltage = tNodes[tPort0].getPotential() - tNodes[tPort1].getPotential();
        const double expectedG = (1.0 - tMalfBlockageValue) * tMalfConstantPowerValue / voltage / voltage;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_EQUAL(true, tArticle->isNonLinear());
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(1, 1));
    } {
        /// @test minorStep method in a linear malf mode.
        tArticle->mShort.setMalfConstantPower();
        tArticle->mShort.setMalfConstantCurrent();
        tArticle->mShort.setMalfInitialPower();
        tArticle->mShort.setMalfInitialCurrent();
        tArticle->minorStep(0.0, 2);
        const double expectedG = (1.0 - tMalfBlockageValue) * 1.0 / tMalfResistanceValue;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_EQUAL(false, tArticle->isNonLinear());
        CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tArticle->confirmSolutionAcceptable(2, 2));
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Short link model restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectShort::testRestart()
{
    std::cout << "\n UtGunnsElectShort ..... 06: testRestart ...........................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test reset of non-checkpointed & non-config data.
    tArticle->mEffectiveConductivity = 1.0;
    tArticle->restart();
    CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mEffectiveConductivity);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Short link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectShort::testInitializationExceptions()
{
    std::cout << "\n UtGunnsElectShort ..... 07: testInitializationExceptions ..........";

    /// - There is currently nothing to test.

    std::cout << "... Pass";
}
