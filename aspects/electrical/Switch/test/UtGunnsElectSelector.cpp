/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/electrical/Switch/GunnsElectSelector.o))
***************************************************************************************************/

#include "UtGunnsElectSelector.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsElectSelector::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Electrical Selector link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectSelector::UtGunnsElectSelector()
    :
    CppUnit::TestFixture(),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPorts(),
    tDefaultConductance(0.0),
    tPortLabels(),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tSelector(0),
    tInputData(0),
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical Selector link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectSelector::~UtGunnsElectSelector()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsElectSelector::setUp()
{
    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", 200.0);
    tNodes[1].initialize("UtNode2", 100.0);
    tNodes[2].initialize("UtNode2",   0.0);

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    /// - Define the nominal configuration data.
    tName               = "nominal";
    tDefaultConductance = 1.0;
    tPortLabels.push_back("Input");
    tPortLabels.push_back("Output1");
    tPortLabels.push_back("Output2");
    tConfigData         = new GunnsElectSelectorConfigData(tName,
                                                           &tNodeList,
                                                           tDefaultConductance,
                                                           &tPortLabels);

    /// - Define the nominal input data.
    tMalfBlockageFlag   = true;
    tMalfBlockageValue  = 0.3;
    tSelector           = 2;
    tInputData          = new GunnsElectSelectorInputData(tMalfBlockageFlag,
                                                          tMalfBlockageValue,
                                                          tSelector);

    /// - Define the nominal port mapping.
    tPorts.push_back(0);
    tPorts.push_back(1);
    tPorts.push_back(2);

    /// - Default construct the nominal test article.
    tArticle            = new FriendlyGunnsElectSelector;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSelector::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Selector link model construction of configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSelector::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName                   == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                  == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_EQUAL(tDefaultConductance, tConfigData->mDefaultConductance);
    CPPUNIT_ASSERT(tPortLabels[0]          == tConfigData->mPortLabels[0]);
    CPPUNIT_ASSERT(tPortLabels[1]          == tConfigData->mPortLabels[1]);
    CPPUNIT_ASSERT(tPortLabels[2]          == tConfigData->mPortLabels[2]);

    /// @test    Configuration data default construction.
    GunnsElectSelectorConfigData defaultConfig;
    CPPUNIT_ASSERT(0       == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0       == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultConfig.mDefaultConductance);
    CPPUNIT_ASSERT(0       == defaultConfig.mPortLabels.size());

    /// @test    Configuration data copy construction.
    GunnsElectSelectorConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tConfigData->mName                   == copyConfig.mName);
    CPPUNIT_ASSERT(tConfigData->mNodeList->mNodes       == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mDefaultConductance, copyConfig.mDefaultConductance);
    CPPUNIT_ASSERT(tPortLabels[0]                       == copyConfig.mPortLabels[0]);
    CPPUNIT_ASSERT(tPortLabels[1]                       == copyConfig.mPortLabels[1]);
    CPPUNIT_ASSERT(tPortLabels[2]                       == copyConfig.mPortLabels[2]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Selector link model construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSelector::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag      == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageValue, tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(tSelector,          tInputData->mSelector);

    /// @test    Input data default construction.
    GunnsElectSelectorInputData defaultInput;
    CPPUNIT_ASSERT(false   == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(0,   defaultInput.mSelector);

    /// @test    Input data copy construction.
    GunnsElectSelectorInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag      == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(tInputData->mSelector,          copyInput.mSelector);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Selector link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSelector::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default of attributes.
    CPPUNIT_ASSERT(""        == tArticle->mName);
    CPPUNIT_ASSERT(0         == tArticle->mNodes);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mDefaultConductance);
    CPPUNIT_ASSERT(0         == tArticle->mPortLabels);
    CPPUNIT_ASSERT_EQUAL(0,     tArticle->mSelector);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mEffectiveConductance);
    CPPUNIT_ASSERT_EQUAL(0,     tArticle->mLastConnectedPort);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mSystemAdmittance);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mSystemSource);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsElectSelector* article = new GunnsElectSelector();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Selector link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSelector::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsElectSelector article;
    article.initialize(*tConfigData, *tInputData, tLinks, &tPorts);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                   == article.mName);
    CPPUNIT_ASSERT(&tNodes[0]              == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]              == article.mNodes[1]);
    CPPUNIT_ASSERT_EQUAL(tDefaultConductance, article.mDefaultConductance);
    CPPUNIT_ASSERT(tPortLabels[0]          == article.mPortLabels[0].mName);
    CPPUNIT_ASSERT(tPortLabels[1]          == article.mPortLabels[1].mName);
    CPPUNIT_ASSERT(tPortLabels[2]          == article.mPortLabels[2].mName);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageFlag,   article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageValue,  article.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(tSelector,           article.mSelector);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT_EQUAL(0.0,                 article.mEffectiveConductance);
    CPPUNIT_ASSERT_EQUAL(-1,                  article.mLastConnectedPort);
    CPPUNIT_ASSERT_EQUAL(0.0,                 article.mSystemAdmittance);
    CPPUNIT_ASSERT_EQUAL(0.0,                 article.mSystemSource);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Initialize default constructed test article with nominal initialization data and no port
    ///   labels.
    tConfigData->mPortLabels.clear();
    article.initialize(*tConfigData, *tInputData, tLinks, &tPorts);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                   == article.mName);
    CPPUNIT_ASSERT(&tNodes[0]              == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]              == article.mNodes[1]);
    CPPUNIT_ASSERT_EQUAL(tDefaultConductance, article.mDefaultConductance);
    CPPUNIT_ASSERT(0                       == article.mPortLabels);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageFlag,   article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageValue,  article.mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(tSelector,           article.mSelector);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT_EQUAL(0.0,                 article.mEffectiveConductance);
    CPPUNIT_ASSERT_EQUAL(-1,                  article.mLastConnectedPort);
    CPPUNIT_ASSERT_EQUAL(0.0,                 article.mSystemAdmittance);
    CPPUNIT_ASSERT_EQUAL(0.0,                 article.mSystemSource);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Selector link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSelector::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, &tPorts);

    /// @test  getDefaultConductance method.
    CPPUNIT_ASSERT_EQUAL(tDefaultConductance, tArticle->getDefaultConductance());

    /// @test  getConnectedPort method.
    CPPUNIT_ASSERT_EQUAL(tSelector,           tArticle->getConnectedPort());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Selector link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSelector::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, &tPorts);

    /// @test  setResistance method.
    tArticle->setDefaultConductance(2.0);
    CPPUNIT_ASSERT_EQUAL(2.0, tArticle->getDefaultConductance());

    tArticle->setSelector(2);
    CPPUNIT_ASSERT_EQUAL(2, tArticle->getConnectedPort());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Selector link model step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSelector::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, &tPorts);

    tArticle->mPotentialVector[tPorts[0]] = tNodes[tPorts[0]].getPotential();
    tArticle->mPotentialVector[tPorts[1]] = tNodes[tPorts[1]].getPotential();
    tArticle->mPotentialVector[tPorts[2]] = tNodes[tPorts[2]].getPotential();

    {
        /// @test    Nominal potential and resistance within limits, with blockage malfunction.
        tArticle->step(0.0);
        const double expectedA = (1.0 - tMalfBlockageValue) * tDefaultConductance;
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[4], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[5], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[6], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[7], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[8], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[2],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());

        /// @test    Repeated system admittance.
        tArticle->step(0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[4], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[5], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[6], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[7], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[8], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[2],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());

        /// @test    Port selection change from 2 to 1.
        tArticle->setSelector(1);
        tArticle->step(0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[4], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[5], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[6], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[7], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[8], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[2],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());

        /// @test    Port selection change from 1 to 0.
        tArticle->setSelector(0);
        tArticle->step(0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[4], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[5], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[6], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[7], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mAdmittanceMatrix[8], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[2],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Nominal potential and resistance within limits, without blockage malfunction.
        tArticle->setSelector(2);
        tArticle->mMalfBlockageFlag = false;
        tArticle->step(0.0);
        const double expectedA = tDefaultConductance;
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Conductance above maximum.
        tArticle->setDefaultConductance(1.0e100);
        tArticle->step(0.0);
        const double expectedA = GunnsBasicLink::mConductanceLimit;
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Conductance below minimum.
        tArticle->setDefaultConductance(DBL_EPSILON);
        tArticle->step(0.0);
        const double expectedA = 0.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Port selection zero.
        tArticle->setDefaultConductance(tDefaultConductance);
        tArticle->setSelector(0);
        tArticle->step(0.0);
        const double expectedA = 0.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Port selection > max.
        tArticle->setSelector(3);
        tArticle->step(0.0);
        const double expectedA = 0.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Selector link model computeFlows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSelector::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, &tPorts);

    tArticle->mPotentialVector[tPorts[0]] = tNodes[tPorts[0]].getPotential();
    tArticle->mPotentialVector[tPorts[1]] = tNodes[tPorts[1]].getPotential();
    tArticle->mPotentialVector[tPorts[2]] = tNodes[tPorts[2]].getPotential();

    {
        /// @test    Nominal potential drop.
        tArticle->step(0.0);
        const double dP        = tNodes[0].getPotential() - tNodes[2].getPotential();
        const double expectedI =  dP * tArticle->mAdmittanceMatrix[0];
        const double expectedP = -dP * expectedI;
        tArticle->computeFlows(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(dP,        tArticle->mPotentialDrop, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mFlux,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mPower,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tNodes[0].getInflux(),    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tNodes[0].getOutflux(),   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tNodes[1].getInflux(),    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tNodes[1].getOutflux(),   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tNodes[2].getInflux(),    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tNodes[2].getOutflux(),   DBL_EPSILON);
    } {
        /// @test    Zero potential drop.
        tNodes[0].resetFlows();
        tNodes[1].resetFlows();
        tNodes[2].resetFlows();
        tArticle->mPotentialVector[tPorts[2]] = tNodes[tPorts[0]].getPotential();
        tArticle->step(0.0);
        tArticle->computeFlows(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPotentialDrop, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlux,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPower,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getInflux(),    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getOutflux(),   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[1].getInflux(),    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[1].getOutflux(),   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[2].getInflux(),    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[2].getOutflux(),   DBL_EPSILON);
    } {
        /// @test    No connection.
        tArticle->mPotentialVector[tPorts[2]] = tNodes[tPorts[2]].getPotential();
        tArticle->setSelector(0);
        tArticle->step(0.0);
        tArticle->computeFlows(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPotentialDrop, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlux,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPower,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getInflux(),    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getOutflux(),   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[1].getInflux(),    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[1].getOutflux(),   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[2].getInflux(),    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[2].getOutflux(),   DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Selector link model restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSelector::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, &tPorts);

    tArticle->mEffectiveConductance = 1.0;
    tArticle->mLastConnectedPort    = 1;
    tArticle->mSystemAdmittance     = 1.0;
    tArticle->mSystemSource         = 1.0;

    tArticle->restart();

    CPPUNIT_ASSERT_EQUAL(tDefaultConductance, tArticle->mDefaultConductance);
    CPPUNIT_ASSERT_EQUAL(0.0,                 tArticle->mEffectiveConductance);
    CPPUNIT_ASSERT_EQUAL(-1,                  tArticle->mLastConnectedPort);
    CPPUNIT_ASSERT_EQUAL(0.0,                 tArticle->mSystemAdmittance);
    CPPUNIT_ASSERT_EQUAL(0.0,                 tArticle->mSystemSource);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Electrical Selector link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectSelector::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsElectSelector article;

    /// @test    Initialization exception invalid config data: no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, &tPorts),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: default conductance < 0.
    tConfigData->mDefaultConductance = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, &tPorts),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    tConfigData->mDefaultConductance = tDefaultConductance;

    /// @test    Initialization exception on invalid input data: selection < 0.
    tInputData->mSelector = -1;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, &tPorts),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());

    /// @test    Initialization exception on invalid input data: selection > num ports -1.
    tInputData->mSelector = N_NODES;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, &tPorts),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    tInputData->mSelector = tSelector;

    /// @test    Initialization exception on invalid init data, ports vector size < 2.
    std::vector<int> ports;
    ports.push_back(0);
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, &ports),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());

    UT_PASS_LAST;
}
