/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/electrical/resistive/GunnsResistorPowerFunction.o))
***************************************************************************************************/

#include "UtGunnsResistorPowerFunction.hh"

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"
#include "core/GunnsFluidNode.hh"

/// @details  Test identification number.
int UtGunnsResistorPowerFunction::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Resistor With Power Function link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsResistorPowerFunction::UtGunnsResistorPowerFunction()
    :
    CppUnit::TestFixture(),
    tLinks(),
    tName(""),
    tNodes(),
    tNodeList(),
    tPort0(0),
    tPort1(0),
    tResistance(0.0),
    tExponent(0.0),
    tUseTangentLine(false),
    tConfigData(0),
    tMalfBlockageFlag(false),
    tMalfBlockageValue(0.0),
    tInputData(0),
    tArticle(0),
    tTimeStep(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Resistor With Power Function link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsResistorPowerFunction::~UtGunnsResistorPowerFunction()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtGunnsResistorPowerFunction::setUp()
{
    /// - Initialize the nodes.
    tNodes[0].initialize("UtNode1", 100.0);
    tNodes[1].initialize("UtNode2", 0.0);

    /// - Initialize the nodes list.
    tNodeList.mNodes    = tNodes;
    tNodeList.mNumNodes = N_NODES;

    /// - Define the nominal configuration data.
    tName               = "nominal";
    tResistance         = 20.0;
    tExponent           = 2.0;
    tUseTangentLine     = true;
    tConfigData         = new GunnsResistorPowerFunctionConfigData(tName,
                                                                   &tNodeList,
                                                                   tResistance,
                                                                   tExponent,
                                                                   tUseTangentLine);

    /// - Define the nominal input data.
    tMalfBlockageFlag   = true;
    tMalfBlockageValue  = 0.3;
    tInputData          = new GunnsResistorPowerFunctionInputData(tMalfBlockageFlag,
                                                                  tMalfBlockageValue);

    /// - Define the nominal port mapping.
    tPort0              = 0;
    tPort1              = 1;

    /// - Default construct the nominal test article.
    tArticle            = new FriendlyGunnsResistorPowerFunction;

    /// - Define the nominal time step.
    tTimeStep           = 0.1;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistorPowerFunction::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Resistor With Power Function link model construction of configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistorPowerFunction::testConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(tName               == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes              == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_EQUAL(tResistance,     tConfigData->mResistance);
    CPPUNIT_ASSERT_EQUAL(tExponent,       tConfigData->mExponent);
    CPPUNIT_ASSERT_EQUAL(tUseTangentLine, tConfigData->mUseTangentLine);

    /// @test    Configuration data default construction.
    GunnsResistorPowerFunctionConfigData defaultConfig;
    CPPUNIT_ASSERT(0         == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0         == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_EQUAL(0.0,   defaultConfig.mResistance);
    CPPUNIT_ASSERT_EQUAL(0.0,   defaultConfig.mExponent);
    CPPUNIT_ASSERT_EQUAL(false, defaultConfig.mUseTangentLine);

    /// @test    Configuration data copy construction.
    GunnsResistorPowerFunctionConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tConfigData->mName               == copyConfig.mName);
    CPPUNIT_ASSERT(tConfigData->mNodeList->mNodes   == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mResistance,     copyConfig.mResistance);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mExponent,       copyConfig.mExponent);
    CPPUNIT_ASSERT_EQUAL(tConfigData->mUseTangentLine, copyConfig.mUseTangentLine);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Resistor With Power Function link model construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistorPowerFunction::testInput()
{
    UT_RESULT;

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(tMalfBlockageFlag      == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageValue, tInputData->mMalfBlockageValue);

    /// @test    Input data default construction.
    GunnsResistorPowerFunctionInputData defaultInput;
    CPPUNIT_ASSERT(false   == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(0.0, defaultInput.mMalfBlockageValue);

    /// @test    Input data copy construction.
    GunnsResistorPowerFunctionInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag      == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Resistor With Power Function link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistorPowerFunction::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default of attributes.
    CPPUNIT_ASSERT(""        == tArticle->mName);
    CPPUNIT_ASSERT(0         == tArticle->mNodes);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mMalfBlockageValue);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mResistance);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mExponent);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mUseTangentLine);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mSystemAdmittance);
    CPPUNIT_ASSERT_EQUAL(0.0,   tArticle->mSystemSource);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsResistorPowerFunction* article = new GunnsResistorPowerFunction();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Resistor With Power Function link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistorPowerFunction::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsResistorPowerFunction article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(tName                  == article.mName);
    CPPUNIT_ASSERT(&tNodes[0]             == article.mNodes[0]);
    CPPUNIT_ASSERT(&tNodes[1]             == article.mNodes[1]);
    CPPUNIT_ASSERT_EQUAL(tResistance,        article.mResistance);
    CPPUNIT_ASSERT_EQUAL(tExponent,          article.mExponent);
    CPPUNIT_ASSERT_EQUAL(tUseTangentLine,    article.mUseTangentLine);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageFlag,  article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(tMalfBlockageValue, article.mMalfBlockageValue);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT_EQUAL(0.0,                article.mSystemAdmittance);
    CPPUNIT_ASSERT_EQUAL(0.0,                article.mSystemSource);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Resistor With Power Function link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistorPowerFunction::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test  getResistance method.
    CPPUNIT_ASSERT_EQUAL(tResistance, tArticle->getResistance());

    /// @test  isNonLinear method when exponent != 1.
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->isNonLinear());

    /// @test  isNonLinear method when exponent = 1.
    tArticle->mExponent = 1.0;
    CPPUNIT_ASSERT_EQUAL(false, tArticle->isNonLinear());

    /// @test  confirmSolutionAcceptable method.
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::CONFIRM,  tArticle->confirmSolutionAcceptable(0, 0));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Resistor With Power Function link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistorPowerFunction::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test  setResistance method.
    tArticle->setResistance(1000.0);
    CPPUNIT_ASSERT_EQUAL(1000.0, tArticle->mResistance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Resistor With Power Function link model step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistorPowerFunction::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Configure minimum linearization P as from the network solver.
    const double minLinP = 0.001;
    tArticle->setMinLinearizationPotential(minLinP);

    tArticle->mPotentialVector[tPort0] = tNodes[tPort0].getPotential();
    tArticle->mPotentialVector[tPort1] = tNodes[tPort1].getPotential();

    /// - Tests using the non-tangent line approximation option:
    tArticle->mUseTangentLine = false;
    {
        /// @test    Nominal potential and resistance within limits, with blockage malfunction.
        tArticle->step(tTimeStep);
        const double dP        = tNodes[0].getPotential() - tNodes[1].getPotential();
        /// - Note that even though tExponent is 2, we can't use sqrt function in this test and
        ///   expect an exact match with the model, because the model uses the powf function and
        ///   sqrt(X) != powf(X, 1/2).
        const double G         = (1.0 - tMalfBlockageValue) / tResistance;
        const double expectedA = powf(dP * G, (1.0 / tExponent)) / dP;
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());

        /// @test    Repeated system admittance.
        tArticle->step(tTimeStep);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(false == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Nominal potential and resistance within limits, without blockage malfunction.
        tArticle->mMalfBlockageFlag = false;
        tArticle->step(tTimeStep);
        const double dP        = tNodes[0].getPotential() - tNodes[1].getPotential();
        const double G         = 1.0 / tResistance;
        const double expectedA = powf(dP * G, (1.0 / tExponent)) / dP;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Potential below minimum linearization.
        tArticle->mPotentialVector[tPort0] = tNodes[tPort1].getPotential() + FLT_EPSILON;
        tArticle->step(tTimeStep);
        const double dP        = minLinP;
        const double G         = 1.0 / tResistance;
        const double expectedA = powf(dP * G, (1.0 / tExponent)) / dP;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Resistance below minimum.
        tArticle->mPotentialVector[tPort0] = tNodes[tPort0].getPotential();
        tArticle->setResistance(0.0);
        tArticle->step(tTimeStep);
        const double dP        = tNodes[0].getPotential() - tNodes[1].getPotential();
        const double G         = GunnsBasicLink::mConductanceLimit;
        const double expectedA = powf(dP * G, (1.0 / tExponent)) / dP;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Resistance above maximum.
        tArticle->setResistance(1.0e100);
        tArticle->step(tTimeStep);
        const double dP        = tNodes[0].getPotential() - tNodes[1].getPotential();
        const double G         = GunnsBasicLink::m100EpsilonLimit;
        const double expectedA = powf(dP * G, (1.0 / tExponent)) / dP;
        CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mAdmittanceMatrix[0]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,       tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    }

    /// - Tests using the tangent-line approximation option:
    tArticle->mUseTangentLine = true;
    {
        /// @test    Nominal potential and resistance within limits, with blockage malfunction.
        tArticle->mMalfBlockageFlag = true;
        tArticle->setResistance(tResistance);
        tArticle->step(tTimeStep);
        const double dP        = tNodes[0].getPotential() - tNodes[1].getPotential();
        const double G         = (1.0 - tMalfBlockageValue) / tResistance;
        const double expectedA = (1.0 / tExponent) * powf(dP * G, (1.0 / tExponent - 1.0));
        const double expectedW = powf(dP * G, (1.0 / tExponent)) - expectedA * dP;
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedW, tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedW, tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Nominal potential and resistance within limits, without blockage malfunction.
        tArticle->mMalfBlockageFlag = false;
        tArticle->step(tTimeStep);
        const double dP        = tNodes[0].getPotential() - tNodes[1].getPotential();
        const double G         = 1.0 / tResistance;
        const double expectedA = (1.0 / tExponent) * powf(dP * G, (1.0 / tExponent - 1.0));
        const double expectedW = powf(dP * G, (1.0 / tExponent)) - expectedA * dP;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Potential below minimum linearization.
        tArticle->mPotentialVector[tPort0] = tNodes[tPort1].getPotential() + FLT_EPSILON;
        tArticle->step(tTimeStep);
        const double dP        = minLinP;
        const double G         = 1.0 / tResistance;
        const double expectedA = (1.0 / tExponent) * powf(dP * G, (1.0 / tExponent - 1.0));
        const double expectedW = powf(dP * G, (1.0 / tExponent)) - expectedA * dP;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Resistance below minimum.
        tArticle->mPotentialVector[tPort0] = tNodes[tPort0].getPotential();
        tArticle->setResistance(0.0);
        tArticle->step(tTimeStep);
        const double dP        = tNodes[0].getPotential() - tNodes[1].getPotential();
        const double G         = GunnsBasicLink::mConductanceLimit;
        const double expectedA = (1.0 / tExponent) * powf(dP * G, (1.0 / tExponent - 1.0));
        const double expectedW = powf(dP * G, (1.0 / tExponent)) - expectedA * dP;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedW, tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    } {
        /// @test    Resistance above maximum.
        tArticle->setResistance(1.0e100);
        tArticle->step(tTimeStep);
        const double dP        = tNodes[0].getPotential() - tNodes[1].getPotential();
        const double G         = GunnsBasicLink::m100EpsilonLimit;
        const double expectedA = (1.0 / tExponent) * powf(dP * G, (1.0 / tExponent - 1.0));
        const double expectedW = powf(dP * G, (1.0 / tExponent)) - expectedA * dP;
        CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mAdmittanceMatrix[0]);
        CPPUNIT_ASSERT_EQUAL(0.0, tArticle->mSourceVector[1]);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Resistor With Power Function link model minorStep method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistorPowerFunction::testMinorStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Configure minimum linearization P as from the network solver.
    const double minLinP = 0.001;
    tArticle->setMinLinearizationPotential(minLinP);

    tArticle->mPotentialVector[tPort0] = tNodes[tPort0].getPotential();
    tArticle->mPotentialVector[tPort1] = tNodes[tPort1].getPotential();

    {
        /// @test    Nominal potential and resistance within limits, with blockage malfunction.
        tArticle->mUseTangentLine = false;
        tArticle->minorStep(tTimeStep, 2);
        const double dP        = tNodes[0].getPotential() - tNodes[1].getPotential();
        const double G         = (1.0 - tMalfBlockageValue) / tResistance;
        const double expectedA = powf(dP * G, (1.0 / tExponent)) / dP;
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-expectedA, tArticle->mAdmittanceMatrix[2], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedA, tArticle->mAdmittanceMatrix[3], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[0],     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,       tArticle->mSourceVector[1],     DBL_EPSILON);
        CPPUNIT_ASSERT(true == tArticle->needAdmittanceUpdate());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Resistor with Power Function link model computeFlows method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistorPowerFunction::testComputeFlows()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tArticle->mPotentialVector[tPort0] = tNodes[tPort0].getPotential();
    tArticle->mPotentialVector[tPort1] = tNodes[tPort1].getPotential();

    /// - Tests using the tangent-line approximation option:
    {
        /// @test    Nominal potential drop.
        tArticle->step(tTimeStep);
        const double dP        = tNodes[0].getPotential() - tNodes[1].getPotential();
        const double expectedI =  dP * tArticle->mAdmittanceMatrix[0] + tArticle->mSourceVector[1];
        const double expectedP = -dP * expectedI;
        tArticle->computeFlows(tTimeStep);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(dP,        tArticle->mPotentialDrop, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mFlux,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mPower,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tNodes[1].getInflux(),    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tNodes[0].getOutflux(),   DBL_EPSILON);
    } {
        /// @test    Zero potential drop.
        tNodes[0].resetFlows();
        tNodes[1].resetFlows();
        tArticle->mPotentialVector[tPort1] = tNodes[tPort0].getPotential();
        tArticle->step(tTimeStep);
        tArticle->computeFlows(tTimeStep);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPotentialDrop, DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlux,          DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPower,         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[1].getInflux(),    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNodes[0].getOutflux(),   DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Resistor With Power Function link model restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistorPowerFunction::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    tArticle->mSystemAdmittance     = 1.0;
    tArticle->mSystemSource         = 1.0;

    tArticle->restart();

    CPPUNIT_ASSERT_EQUAL(tResistance, tArticle->mResistance);
    CPPUNIT_ASSERT_EQUAL(0.0,         tArticle->mSystemAdmittance);
    CPPUNIT_ASSERT_EQUAL(0.0,         tArticle->mSystemSource);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Resistor With Power Function link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsResistorPowerFunction::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsResistorPowerFunction article;

    /// @test    Initialization exception invalid config data: no name.
    tConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    tConfigData->mName = tName;

    /// @test    Initialization exception on invalid config data: flow resistance < DBL_EPSILON.
    tConfigData->mResistance = 0.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    tConfigData->mResistance = tResistance;

    /// @test    Initialization exception on invalid config data: exponent near zero.
    tConfigData->mExponent = FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.isInitialized());
    tConfigData->mExponent = tExponent;

    UT_PASS_LAST;
}
