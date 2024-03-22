/*
@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include <iostream>
#include <cmath>
#include <cfloat>

#include "UtGunns.hh"
#include "UtGunnsMinorStepLog.hh"
#include "core/GunnsBasicFlowOrchestrator.hh"

//TODO catch-up for line coverage:
//     - line 591, try to make a link throw during restart
//     - minor step log stuff:
//       - checkSystemConvergence lines 1079, 1087-1088
//       - confirmSolutionAcceptance lines 1154-1155
//     - buildIslands, line 1231
//     - handleDecompose, line 1396
//     - handleSolve, line 1420 (might not be testable)
//     - computeNetworkCapacitance, line 1494

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is a constructor for this dummy link class that configures it for allowing us to
///           manipulate the inputs to the Gunns system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFakeLink::UtGunnsFakeLink(int numPorts, bool isNonLinear, bool failOnStep)
    :
    GunnsBasicLink(numPorts),
    nonLinearFlag(isNonLinear),
    failOnStep(failOnStep),
    delayToAbsoluteStep(0),
    delayToConvergedStep(0),
    callsToRead(0),
    callsToWrite(0),
    callsToMinorStep(0),
    savedPotential()
{
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor for this dummy link class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFakeLink::~UtGunnsFakeLink()
{
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method overrides the GunnsBasicLink method, allowing us to control whether Gunns
///           treats this link as linear or non-linear.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool UtGunnsFakeLink::isNonLinear()
{
    return nonLinearFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method overrides the GunnsBasicLink method, allowing us to control whether the
///           link confirms or rejects the solver solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult UtGunnsFakeLink::confirmSolutionAcceptable(const int convergedStep,
                                                                          const int absoluteStep)
{
    if (failOnStep) {
        return REJECT;
    }
    else if (absoluteStep < delayToAbsoluteStep) {
        return DELAY;
    }
    else if (convergedStep < delayToConvergedStep) {
        return DELAY;
    }
    else {
        return CONFIRM;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method overrides the GunnsBasicLink method, allowing us to control whether the
///           link fails to reset to the last minor step.  It will only fail on minor step 2, for
///           the purposes of these tests.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool UtGunnsFakeLink::resetLastMinorStep(const int convergedStep, const int absoluteStep)
{
    switch (absoluteStep) {
        case 1 :
            savedPotential[0] = mPotentialVector[0];
            savedPotential[1] = mPotentialVector[1];
            return true;

        case 2 :
            return !failOnStep;

        default :
            return true;
    }
}

void UtGunnsFakeLink::minorStep(const double timeStep, const int minorStep)
{
    ++callsToMinorStep;
    mAdmittanceUpdate = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method overrides the GunnsBasicLink method, allowing us to know whether the Gunns
///           test article called this method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFakeLink::processInputs()
{
    ++callsToRead;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method overrides the GunnsBasicLink method, allowing us to know whether the Gunns
///           test article called this method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFakeLink::processOutputs()
{
    ++callsToWrite;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunns class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunns::UtGunns()
    :
    tNetwork(),
    tNetworkConfig(),
    tNetworkName(),
    tDeltaTime(),
    tConvergenceTolerance(),
    tMinLinearizationPotential(),
    tMinorStepLimit(),
    tDecompositionLimit(),
    tBasicNodes(),
    tFluidNodes(),
    tNodeList(),
    tLinks(),
    mFluidProperties(),
    tFluid(),
    tFluidConfig(),
    tFractions(),
    tFluidCapacitor1(),
    tFluidCapacitor1Config(),
    tFluidCapacitor1Input(),
    tFluidConductor1(),
    tFluidConductor1Config(),
    tFluidConductor1Input(),
    tFluidConductor2(),
    tFluidConductor2Config(),
    tFluidConductor2Input(),
    tFluidConductor3(),
    tFluidConductor3Config(),
    tFluidConductor3Input(),
    tConstantLoad1(),
    tConstantLoad1Config(),
    tConstantLoad2(),
    tConstantLoad2Config(),
    tPotential(),
    tPotentialConfig(),
    tConductor1(),
    tConductor1Config(),
    tConductor2(),
    tConductor2Config(),
    tConductor3(),
    tConductor3Config(),
    tConductor4(),
    tConductor4Config(),
    tCapacitor(),
    tCapacitorConfig(),
    tSource(),
    tSourceConfig(),
    tFake1(2, true, false)
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunns class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunns::~UtGunns()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::tearDown()
{
    delete tFluidConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::setUp()
{
    /// - Define nominal configuration data.
    tNetworkName               = "TestNetwork";
    tDeltaTime                 = 0.1;
    tConvergenceTolerance      = 0.1;
    tMinLinearizationPotential = 1.0;
    tMinorStepLimit            = 7;
    tDecompositionLimit        = 5;

    tNetworkConfig.mName                      = tNetworkName;
    tNetworkConfig.mConvergenceTolerance      = tConvergenceTolerance;
    tNetworkConfig.mMinLinearizationPotential = tMinLinearizationPotential;
    tNetworkConfig.mMinorStepLimit            = tMinorStepLimit;
    tNetworkConfig.mDecompositionLimit        = tDecompositionLimit;

    /// - Set up the fluid configuration data.
    static const int numFluidTypes = 3;
    static FluidProperties::FluidType fluidTypesList[numFluidTypes] = {
        FluidProperties::GUNNS_O2,
        FluidProperties::GUNNS_N2,
        FluidProperties::GUNNS_CO2
    };

    tFluidConfig = new PolyFluidConfigData(&mFluidProperties, fluidTypesList, numFluidTypes);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the construction of the GunnsConfigData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testConfigData()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunns ................ 01: testConfigData ........................";

    /// - Check default GunnsConfigData construction.
    GunnsConfigData* tDefaultConfig = new GunnsConfigData;
    CPPUNIT_ASSERT("" == tDefaultConfig->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, tDefaultConfig->mConvergenceTolerance,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, tDefaultConfig->mMinLinearizationPotential, DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL(0, tDefaultConfig->mMinorStepLimit);
    CPPUNIT_ASSERT_EQUAL(0, tDefaultConfig->mDecompositionLimit);
    delete tDefaultConfig;

    /// - Check nominal GunnsConfigData construction.
    GunnsConfigData* tNetConfig = new GunnsConfigData(tNetworkName,
                                                      tConvergenceTolerance,
                                                      tMinLinearizationPotential,
                                                      tMinorStepLimit,
                                                      tDecompositionLimit);

    CPPUNIT_ASSERT(tNetworkName == tNetConfig->mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConvergenceTolerance,
            tNetConfig->mConvergenceTolerance,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinLinearizationPotential,
            tNetConfig->mMinLinearizationPotential, DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL(tMinorStepLimit,     tNetConfig->mMinorStepLimit);
    CPPUNIT_ASSERT_EQUAL(tDecompositionLimit, tNetConfig->mDecompositionLimit);

    /// - Check copy GunnsConfigData construction.
    GunnsConfigData copyConfig(*tNetConfig);
    CPPUNIT_ASSERT(tNetConfig->mName == copyConfig.mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNetConfig->mConvergenceTolerance,
            copyConfig.mConvergenceTolerance,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNetConfig->mMinLinearizationPotential,
            copyConfig.mMinLinearizationPotential, DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL(tNetConfig->mMinorStepLimit,     copyConfig.mMinorStepLimit);
    CPPUNIT_ASSERT_EQUAL(tNetConfig->mDecompositionLimit, copyConfig.mDecompositionLimit);

    delete tNetConfig;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the exception handlers for the Gunns config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testConfigDataExceptions()
{
    std::cout << "\n UtGunns ................ 02: testConfigDataExceptions ..............";

    /// - Check for exception on no network name.
    tNetworkConfig.mName = "";
    CPPUNIT_ASSERT_THROW(tNetwork.initialize(tNetworkConfig, tLinks),
                         TsInitializationException);
    tNetworkConfig.mName = tNetworkName;
    tNetwork.mInitAttempted = false;

    /// - Check for exception on convergence tolerance.
    tNetworkConfig.mConvergenceTolerance = 0.0;
    CPPUNIT_ASSERT_THROW(tNetwork.initialize(tNetworkConfig, tLinks),
                         TsInitializationException);
    tNetworkConfig.mConvergenceTolerance = tConvergenceTolerance;
    tNetwork.mInitAttempted = false;

    /// - Check for exception on minimum linearization potential.
    tNetworkConfig.mMinLinearizationPotential = 0.0;
    CPPUNIT_ASSERT_THROW(tNetwork.initialize(tNetworkConfig, tLinks),
                         TsInitializationException);
    tNetworkConfig.mMinLinearizationPotential = tMinLinearizationPotential;
    tNetwork.mInitAttempted = false;

    /// - Check for exception on number of nodes.
    tNetwork.mNumNodes = 1;
    CPPUNIT_ASSERT_THROW(tNetwork.initialize(tNetworkConfig, tLinks),
                         TsInitializationException);
    tNetwork.mNumNodes = 3;
    tNetwork.mInitAttempted = false;

    /// - Check for exception on minor step limit.
    tNetworkConfig.mMinorStepLimit = 0;
    CPPUNIT_ASSERT_THROW(tNetwork.initialize(tNetworkConfig, tLinks),
                         TsInitializationException);
    tNetworkConfig.mMinorStepLimit = 5;
    tNetwork.mInitAttempted = false;

    /// - Check for exception on decomposition limit.
    tNetworkConfig.mDecompositionLimit = 0;
    CPPUNIT_ASSERT_THROW(tNetwork.initialize(tNetworkConfig, tLinks),
                         TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the exception handlers for trying to add the same link more than
///           once.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testDuplicateLinkException()
{
    std::cout << "\n UtGunns ................ 03: testDuplicateLinkException ............";

    /// - Initialize the basic nodes.
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tBasicNodes;
    tNetwork.initializeNodes(tNodeList);

    /// - Check for exception when trying to add the same link twice.
    tConductor1Config.mName     = "tConductor1";
    tConductor1Config.mNodeList = &tNodeList;
    GunnsBasicConductorInputData tInputData;
    tConductor1.initialize(tConductor1Config, tInputData, tLinks, 0, 1);
    tConductor1.initialize(tConductor1Config, tInputData, tLinks, 0, 1);
    CPPUNIT_ASSERT_THROW(tNetwork.initialize(tNetworkConfig, tLinks),
                         TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the Gunns class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testDefaultConstruction()
{
    std::cout << "\n UtGunns ................ 04: testDefaultConstruction ...............";

    /// - Check default class attributes after construction.
    CPPUNIT_ASSERT(""            == tNetwork.mName);
    CPPUNIT_ASSERT(0             == tNetwork.mNumLinks);
    CPPUNIT_ASSERT(0             == tNetwork.mNumNodes);
    CPPUNIT_ASSERT(0             == tNetwork.mLinks);
    CPPUNIT_ASSERT(0             == tNetwork.mNodes);
    CPPUNIT_ASSERT(0             == tNetwork.mAdmittanceMatrix);
    CPPUNIT_ASSERT(0             == tNetwork.mAdmittanceMatrixIsland);
    CPPUNIT_ASSERT(0             == tNetwork.mSourceVector);
    CPPUNIT_ASSERT(0             == tNetwork.mSourceVectorIsland);
    CPPUNIT_ASSERT(0             == tNetwork.mPotentialVector);
    CPPUNIT_ASSERT(0             == tNetwork.mPotentialVectorIsland);
    CPPUNIT_ASSERT(0             == tNetwork.mMinorPotentialVector);
    CPPUNIT_ASSERT(0             == tNetwork.mMajorPotentialVector);
    CPPUNIT_ASSERT(0             == tNetwork.mSlavePotentialVector);
    CPPUNIT_ASSERT(0             == tNetwork.mNetCapDeltaPotential);
    CPPUNIT_ASSERT(0             == tNetwork.mIslandVectors.size());
    CPPUNIT_ASSERT(0             == tNetwork.mNodeIslandNumbers);
    CPPUNIT_ASSERT(0             == tNetwork.mIslandCount);
    CPPUNIT_ASSERT(0             == tNetwork.mIslandMaxSize);
    CPPUNIT_ASSERT(Gunns::OFF    == tNetwork.mIslandMode);
    CPPUNIT_ASSERT(0             == tNetwork.mSolverCpu);
    CPPUNIT_ASSERT(0             == tNetwork.mSolverGpuDense);
    CPPUNIT_ASSERT(0             == tNetwork.mSolverGpuSparse);
    if (tNetwork.isGpuEnabled()) {
        CPPUNIT_ASSERT(true      == tNetwork.mGpuEnabled);
    } else {
        CPPUNIT_ASSERT(false     == tNetwork.mGpuEnabled);
    }
    CPPUNIT_ASSERT(Gunns::NO_GPU == tNetwork.mGpuMode);
    CPPUNIT_ASSERT(9999999       == tNetwork.mGpuSizeThreshold);
    CPPUNIT_ASSERT(1.0           == tNetwork.mConvergenceTolerance);
    CPPUNIT_ASSERT(0             == tNetwork.mNetworkSize);
    CPPUNIT_ASSERT(1             == tNetwork.mMinorStepLimit);
    CPPUNIT_ASSERT(1             == tNetwork.mDecompositionLimit);
    CPPUNIT_ASSERT(false         == tNetwork.mRebuild);
    CPPUNIT_ASSERT(true          == tNetwork.mLinearNetwork);
    CPPUNIT_ASSERT(Gunns::NORMAL == tNetwork.mSolverMode);
    CPPUNIT_ASSERT(Gunns::RUN    == tNetwork.mRunMode);
    CPPUNIT_ASSERT(false         == tNetwork.mInitAttempted);
    CPPUNIT_ASSERT(false         == tNetwork.mInitFlag);
    CPPUNIT_ASSERT(0             == tNetwork.mLinkAdmittanceMatrices);
    CPPUNIT_ASSERT(0             == tNetwork.mLinkPotentialVectors);
    CPPUNIT_ASSERT(0             == tNetwork.mLinkSourceVectors);
    CPPUNIT_ASSERT(0             == tNetwork.mLinkOverrideVectors);
    CPPUNIT_ASSERT(0             == tNetwork.mLinkNodeMaps);
    CPPUNIT_ASSERT(0             == tNetwork.mLinkNumPorts);
    CPPUNIT_ASSERT(0             == tNetwork.mMajorStepCount);
    CPPUNIT_ASSERT(0             == tNetwork.mConvergenceFailCount);
    CPPUNIT_ASSERT(0             == tNetwork.mLinkResetStepFailCount);
    CPPUNIT_ASSERT(0             == tNetwork.mMinorStepCount);
    CPPUNIT_ASSERT(0             == tNetwork.mMaxMinorStepCount);
    CPPUNIT_ASSERT(0             == tNetwork.mLastMinorStep);
    CPPUNIT_ASSERT(0.0           == tNetwork.mAvgMinorStepCount);
    CPPUNIT_ASSERT(0             == tNetwork.mDecompositionCount);
    CPPUNIT_ASSERT(0             == tNetwork.mMaxDecompositionCount);
    CPPUNIT_ASSERT(0             == tNetwork.mLastDecomposition);
    CPPUNIT_ASSERT(0             == tNetwork.mAvgDecompositionCount);
    CPPUNIT_ASSERT(0.0           == tNetwork.mSolveTime);
    CPPUNIT_ASSERT(0.0           == tNetwork.mStepTime);
    CPPUNIT_ASSERT(false         == tNetwork.mWorstCaseTiming);
    CPPUNIT_ASSERT(0             == tNetwork.mLinksConvergence);
    CPPUNIT_ASSERT(0             == tNetwork.mNodesConvergence);
    CPPUNIT_ASSERT(0             == tNetwork.mDebugSavedSlice);
    CPPUNIT_ASSERT(0             == tNetwork.mDebugSavedNode);
    CPPUNIT_ASSERT(0             == tNetwork.mDebugDesiredSlice);
    CPPUNIT_ASSERT(0             == tNetwork.mDebugDesiredStep);
    CPPUNIT_ASSERT(-1            == tNetwork.mDebugDesiredNode);
    CPPUNIT_ASSERT(false         == tNetwork.mVerbose);
    CPPUNIT_ASSERT(false         == tNetwork.mSorActive);
    CPPUNIT_ASSERT(1.0           == tNetwork.mSorWeight);
    CPPUNIT_ASSERT(100           == tNetwork.mSorMaxIter);
    CPPUNIT_ASSERT(1.0e-12       == tNetwork.mSorTolerance);
    CPPUNIT_ASSERT(-1            == tNetwork.mSorLastIteration);
    CPPUNIT_ASSERT(0             == tNetwork.mSorFailCount);
    CPPUNIT_ASSERT(Gunns::NORMAL == tNetwork.mLastSolverMode);
    CPPUNIT_ASSERT(Gunns::OFF    == tNetwork.mLastIslandMode);
    CPPUNIT_ASSERT(Gunns::RUN    == tNetwork.mLastRunMode);

    /// - Test dynamic allocation of arrays of Gunns.
    int ii;
    Gunns* articles = 0;
    articles = new Gunns[2];
    for (ii=0; ii<2; ii++) {
        new (&articles[ii]) Gunns();
    }
    delete [] articles;
    articles = 0;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the network initialization methods of the Gunns class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testNominalInitialization()
{
    std::cout << "\n UtGunns ................ 05: testNominalInitialization .............";

    /// - Initialize the fluid nodes.
    tFluidNodes[0].initialize("FluidNode1",tFluidConfig,tFluid);
    tFluidNodes[1].initialize("FluidNode2",tFluidConfig,tFluid);
    tFluidNodes[2].initialize("FluidNode3",tFluidConfig,tFluid);
    tFluidNodes[3].initialize("FluidNode4",tFluidConfig);
    tNodeList.mNumNodes = 4;
    tNodeList.mNodes    = tFluidNodes;

    tNetwork.initializeFluidNodes(tNodeList);

    /// - Check Nodes vector.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.32501,
            tNetwork.mNodes[2]->getContent()->getPartialPressure(FluidProperties::GUNNS_O2),
            DBL_EPSILON);
    CPPUNIT_ASSERT(std::string("FluidNode4") == tFluidNodes[3].mName);

    /// - Check vacuum node.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetwork.mNodes[3]->getPotential(), DBL_EPSILON);

    /// - Setup the links config and input data.
    tFluidConductor1Config.mName                  = "tFluidConductor1";
    tFluidConductor1Config.mNodeList              = &tNodeList;
    tFluidConductor1Config.mMaxConductivity       = 1.0;
    tFluidConductor1Config.mExpansionScaleFactor  = 0.1;
    tFluidConductor2Config.mName                  = "tFluidConductor2";
    tFluidConductor2Config.mNodeList              = &tNodeList;
    tFluidConductor2Config.mMaxConductivity       = 2.0;
    tFluidConductor2Config.mExpansionScaleFactor  = 0.2;
    tFluidConductor3Config.mName                  = "tFluidConductor3";
    tFluidConductor3Config.mNodeList              = &tNodeList;
    tFluidConductor3Config.mMaxConductivity       = 3.0;
    tFluidConductor3Config.mExpansionScaleFactor  = 0.3;
    tFluidCapacitor1Config.mName                  = "tFluidCapacitor1";
    tFluidCapacitor1Config.mNodeList              = &tNodeList;
    tFluidCapacitor1Config.mExpansionScaleFactor  = 0.4;
    tFluidConductor1Input = new GunnsFluidConductorInputData(false, 0.0);
    tFluidConductor2Input = new GunnsFluidConductorInputData(true,  0.5);
    tFluidConductor3Input = new GunnsFluidConductorInputData(false, 0.0);
    tFractions[0] = 1.0;
    tFractions[1] = 0.0;
    tFractions[2] = 0.0;
    tFluid = new PolyFluidInputData(283.15,                 //temperature
                                    689.475728,             //pressure
                                    0.0,                    //flowRate
                                    0.0,                    //mass
                                    tFractions);            //massFraction
    tFluidCapacitor1Input = new GunnsFluidCapacitorInputData(false, 0.0, 4.0, tFluid);

    /// - Add the links to the network.
    tFluidConductor1.initialize(tFluidConductor1Config, *tFluidConductor1Input, tLinks, 0, 1);
    tFluidConductor2.initialize(tFluidConductor2Config, *tFluidConductor2Input, tLinks, 1, 2);
    tFluidConductor3.initialize(tFluidConductor3Config, *tFluidConductor3Input, tLinks, 0, 3);
    tFluidCapacitor1.initialize(tFluidCapacitor1Config, *tFluidCapacitor1Input, tLinks, 2, 3);

    delete tFluidConductor1Input;
    delete tFluidConductor2Input;
    delete tFluidConductor3Input;
    delete tFluidCapacitor1Input;
    delete tFluid;

    /// - Check the links vector and link initialization.
    CPPUNIT_ASSERT_EQUAL(2, *(tLinks[1]->getNodeMap()+1));
    CPPUNIT_ASSERT_EQUAL(2, *tLinks[3]->getNodeMap());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tFluidConductor1.mEffectiveConductivity, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3, tFluidConductor3.mExpansionScaleFactor,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tFluidConductor2.mMalfBlockageValue,     DBL_EPSILON);

    /// - Initialize the network with configuration data.
    tNetwork.initialize(tNetworkConfig, tLinks);

    /// - Check configuration data.
    CPPUNIT_ASSERT(tNetwork.mName == tNetworkConfig.mName);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNetworkConfig.mConvergenceTolerance,
            tNetwork.mConvergenceTolerance, 0.0);
    CPPUNIT_ASSERT_EQUAL(tNodeList.mNumNodes - 1, tNetwork.mNetworkSize);
    CPPUNIT_ASSERT_EQUAL(tMinorStepLimit,         tNetwork.mMinorStepLimit);
    CPPUNIT_ASSERT_EQUAL(tDecompositionLimit,     tNetwork.mDecompositionLimit);

    /// - Check initial system of equation values.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,        tNetwork.mSourceVector[2],         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(689.475728, tNetwork.mMinorPotentialVector[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(689.475728, tNetwork.mMajorPotentialVector[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,        tNetwork.mSlavePotentialVector[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,        tNetwork.mNetCapDeltaPotential[8], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.32501,  tNetwork.mPotentialVector[0],      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(689.475728, tNetwork.mPotentialVector[2],      DBL_EPSILON);

    /// - Check initial potential distributed to incident links.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(689.475728, tFluidConductor2.mPotentialVector[1], DBL_EPSILON);

    /// - Check Config data transfer to links.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMinLinearizationPotential,
            tFluidConductor1.mMinLinearizationPotential, DBL_EPSILON);

    /// - Check final init flags for a linear network.
    CPPUNIT_ASSERT(tNetwork.mInitAttempted == true);
    CPPUNIT_ASSERT(tNetwork.mInitFlag      == true);
    CPPUNIT_ASSERT(tNetwork.mLinearNetwork == true);
    CPPUNIT_ASSERT(tNetwork.mRebuild       == true);

    /// - Check remaining state data.
    if (tNetwork.isGpuEnabled()) {
        CPPUNIT_ASSERT(0         != tNetwork.mSolverCpu);
        CPPUNIT_ASSERT(0         != tNetwork.mSolverGpuDense);
        CPPUNIT_ASSERT(0         != tNetwork.mSolverGpuSparse);
    } else {
        CPPUNIT_ASSERT(0         != tNetwork.mSolverCpu);
        CPPUNIT_ASSERT(0         == tNetwork.mSolverGpuDense);
        CPPUNIT_ASSERT(0         == tNetwork.mSolverGpuSparse);
    }
    CPPUNIT_ASSERT(false         == tNetwork.mWorstCaseTiming);
    CPPUNIT_ASSERT(false         == tNetwork.mVerbose);
    CPPUNIT_ASSERT(Gunns::NORMAL == tNetwork.mSolverMode);
    CPPUNIT_ASSERT(Gunns::OFF    == tNetwork.mIslandMode);
    CPPUNIT_ASSERT(Gunns::RUN    == tNetwork.mRunMode);
    CPPUNIT_ASSERT(Gunns::NORMAL == tNetwork.mLastSolverMode);
    CPPUNIT_ASSERT(Gunns::OFF    == tNetwork.mLastIslandMode);
    CPPUNIT_ASSERT(Gunns::RUN    == tNetwork.mLastRunMode);
    CPPUNIT_ASSERT(0             == tNetwork.mLinksConvergence);
    CPPUNIT_ASSERT(0             == tNetwork.mNodesConvergence);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method adds extra tests for initialization of a non-linear network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testNonLinearInitialization()
{
    std::cout << "\n UtGunns ................ 06: testNonLinearInitialization ...........";

    /// - Initialize the basic nodes.
    tBasicNodes[0].initialize("BasicNode1", 1.0);
    tBasicNodes[1].initialize("BasicNode2", 2.0);
    tBasicNodes[2].initialize("BasicNode3", 3.9);
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tBasicNodes;

    tNetwork.initializeNodes(tNodeList);
    CPPUNIT_ASSERT(std::string("BasicNode3") == tBasicNodes[2].mName);

    /// - Initialize some links including a non-linear link.
    tConductor1Config.mName        = "tConductor1";
    tConductor1Config.mNodeList    = &tNodeList;
    tConductor2Config.mName        = "tConductor2";
    tConductor2Config.mNodeList    = &tNodeList;
    tConstantLoad1Config.mName     = "tConstantLoad1";
    tConstantLoad1Config.mNodeList = &tNodeList;
    GunnsBasicConductorInputData tInputData;
    EpsConstantPowerLoadInputData inputData;
    tConductor1.initialize(tConductor1Config, tInputData, tLinks, 0, 1);
    tConductor2.initialize(tConductor2Config, tInputData, tLinks, 0, 1);
    tConstantLoad1.initialize(tConstantLoad1Config, inputData, tLinks, 0, 1);

    /// - Test setting a new flow orchestrator.
    const int newNumLinks = 3;
    const int newNumNodes = 3;
    GunnsBasicFlowOrchestrator newOrch(newNumLinks, newNumNodes);
    tNetwork.setFlowOrchestrator(&newOrch);
    CPPUNIT_ASSERT(&newOrch == tNetwork.mFlowOrchestrator);

    /// - Check for the non-linear network flag being set.
    tNetwork.initialize(tNetworkConfig, tLinks);
    CPPUNIT_ASSERT(tNetwork.mLinearNetwork == false);

    /// - Check allocation and initialization of debug terms.
    CPPUNIT_ASSERT(0.0 == tNetwork.mDebugSavedSlice[1]);
    CPPUNIT_ASSERT(0.0 == tNetwork.mDebugSavedNode[5]);
    CPPUNIT_ASSERT(0   == tNetwork.mDebugDesiredSlice);
    CPPUNIT_ASSERT(0   == tNetwork.mDebugDesiredStep);
    CPPUNIT_ASSERT(-1  == tNetwork.mDebugDesiredNode);

    /// - Check allocation and initialization of convergence terms.
    CPPUNIT_ASSERT(0                       != tNetwork.mLinksConvergence);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tNetwork.mLinksConvergence[0]);
    CPPUNIT_ASSERT(0                       != tNetwork.mNodesConvergence);
    CPPUNIT_ASSERT(0.0                     == tNetwork.mNodesConvergence[1]);

    /// - Check saving initial potential vectors.
    CPPUNIT_ASSERT(tBasicNodes[0].getPotential() == tNetwork.mMinorPotentialVector[0]);
    CPPUNIT_ASSERT(tBasicNodes[1].getPotential() == tNetwork.mMinorPotentialVector[1]);
    CPPUNIT_ASSERT(tBasicNodes[0].getPotential() == tNetwork.mMajorPotentialVector[0]);
    CPPUNIT_ASSERT(tBasicNodes[1].getPotential() == tNetwork.mMajorPotentialVector[1]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method verifies that the initialize method will throw an exception if it is
///           called more than once.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testDuplicateInitException()
{
    std::cout << "\n UtGunns ................ 07: testDuplicateInitException ............";

    /// - Initialize the basic nodes.
    tBasicNodes[0].initialize("BasicNode1");
    tBasicNodes[1].initialize("BasicNode2");
    tBasicNodes[2].initialize("BasicNode3");
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tBasicNodes;
    tNetwork.initializeNodes(tNodeList);

    tConstantLoad1Config.mName     = "tConstantLoad1";
    tConstantLoad1Config.mNodeList = &tNodeList;
    EpsConstantPowerLoadInputData inputData;

    tConstantLoad1.initialize(tConstantLoad1Config, inputData, tLinks, 0, 1);
    tNetwork.initialize(tNetworkConfig, tLinks);

    CPPUNIT_ASSERT_THROW(tNetwork.initialize(tNetworkConfig, tLinks),
            TsInitializationException);
    CPPUNIT_ASSERT(tNetwork.mInitFlag == false);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method verifies that the initialize method will throw an exception if there is
///           a network link that didn't successfully init.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testLinkInitException()
{
    std::cout << "\n UtGunns ................ 08: testLinkInitException .................";

    /// - Initialize the fluid nodes.
    tFluidNodes[0].initialize("FluidNode1",tFluidConfig,tFluid);
    tFluidNodes[1].initialize("FluidNode2",tFluidConfig,tFluid);
    tFluidNodes[2].initialize("FluidNode3",tFluidConfig,tFluid);
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tFluidNodes;
    tNetwork.initializeFluidNodes(tNodeList);

    tFluidConductor1Config.mName                  = "tFluidConductor1";
    tFluidConductor1Config.mNodeList              = &tNodeList;
    tFluidConductor1Config.mMaxConductivity       = 1.0;
    tFluidConductor1Config.mExpansionScaleFactor  = 0.1;
    tFluidConductor1Input = new GunnsFluidConductorInputData(true, -1.0); // Bad value for mBlockage

    CPPUNIT_ASSERT_THROW(
        tFluidConductor1.initialize(tFluidConductor1Config, *tFluidConductor1Input, tLinks, 0, 1),
        TsInitializationException);

    delete tFluidConductor1Input;
    CPPUNIT_ASSERT_THROW(tNetwork.initialize(tNetworkConfig, tLinks), TsInitializationException);
    CPPUNIT_ASSERT(tNetwork.mInitFlag == false);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method verifies that the initialize method will throw an exception if there is
///           a network node that didn't successfully init.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testNodeInitException()
{
    std::cout << "\n UtGunns ................ 09: testNodeInitException .................";

    /// - Initialize the fluid nodes.
    tFluidNodes[0].initialize("FluidNode1",tFluidConfig,tFluid);
    tFluidNodes[1].initialize("FluidNode2",tFluidConfig,tFluid);
    tFluidNodes[2].initialize("FluidNode3",tFluidConfig,tFluid);
    tNodeList.mNumNodes = 4;
    tNodeList.mNodes    = tFluidNodes;
    tNetwork.initializeFluidNodes(tNodeList);

    /// - Verify duplicate attempts to initialie nodes throws exception.
    CPPUNIT_ASSERT_THROW(tNetwork.initializeFluidNodes(tNodeList), TsInitializationException);

    // Must have a non-zero size of tLinks to Pass to the network
    GunnsBasicPotentialConfigData mConfigData("NewLink", &tNodeList, 1.0);
    GunnsBasicPotentialInputData  mInputData(false, 0.0, 1.0);
    GunnsBasicPotential link;
    link.initialize(mConfigData, mInputData, tLinks, 0, 1);

    /// - Verify exception is thrown if the vacuum node hasn't been initialized.
    CPPUNIT_ASSERT_THROW(tNetwork.initialize(tNetworkConfig, tLinks),
            TsInitializationException);
    CPPUNIT_ASSERT(tNetwork.mInitFlag == false);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method verifies that the step method will throw an exception if it is called
///           before successful initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testNonInitException()
{
    std::cout << "\n UtGunns ................ 10: testNonInitException ..................";

    /// - Check for exception when stepped before valid initialization.
    tNetwork.mInitFlag = false;
    CPPUNIT_ASSERT_THROW(tNetwork.step(tDeltaTime), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests a nominal step through a linear network and checks the accuracy of
///           the solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testLinearStep()
{
    std::cout << "\n UtGunns ................ 11: testLinearStep ........................";

    /// - Initialize the basic nodes.
    tBasicNodes[0].initialize("BasicNode1");
    tBasicNodes[1].initialize("BasicNode2");
    tBasicNodes[2].initialize("BasicNode3");
    tBasicNodes[3].initialize("BasicNode4");
    tNodeList.mNumNodes = 4;
    tNodeList.mNodes    = tBasicNodes;
    tNetwork.initializeNodes(tNodeList);

    /// - Set up a simple basic network.
    tPotentialConfig.mName                 = "Potential";
    tPotentialConfig.mNodeList             = &tNodeList;
    tPotentialConfig.mDefaultConductivity  = 1.0E5;
    tConductor1Config.mName                = "Conductor1";
    tConductor1Config.mNodeList            = &tNodeList;
    tConductor1Config.mDefaultConductivity = 0.01;
    tConductor2Config.mName                = "Conductor2";
    tConductor2Config.mNodeList            = &tNodeList;
    tConductor2Config.mDefaultConductivity = 0.001;
    tCapacitorConfig.mName                 = "Capacitor";
    tCapacitorConfig.mNodeList             = &tNodeList;
    tSourceConfig.mName                    = "Capacitor";
    tSourceConfig.mNodeList                = &tNodeList;

    GunnsBasicPotentialInputData tPotentialInput (false, 0.0, -125.0);
    GunnsBasicConductorInputData tConductor1Input(false, 0.0);
    GunnsBasicConductorInputData tConductor2Input(false, 0.0);
    GunnsBasicCapacitorInputData tCapacitorInput (false, 0.0, 1.0, 125.0);
    GunnsBasicSourceInputData    tSourceInput    (false, 0.0, 0.01);

    tPotential .initialize(tPotentialConfig,  tPotentialInput,  tLinks, 0, 3);
    tConductor1.initialize(tConductor1Config, tConductor1Input, tLinks, 0, 1);
    tConductor2.initialize(tConductor2Config, tConductor2Input, tLinks, 1, 2);
    tCapacitor .initialize(tCapacitorConfig,  tCapacitorInput,  tLinks, 1, 3);
    tSource    .initialize(tSourceConfig,     tSourceInput,     tLinks, 2, 3);

    /// - Set up and add a fake link to the network.
    UtGunnsFakeLink          fakeLink(2, false, false);
    GunnsBasicLinkConfigData fakeLinkConfig;
    GunnsBasicLinkInputData  fakeLinkInput;
    fakeLinkConfig.mName     = "tLink";
    fakeLinkConfig.mNodeList = &tNodeList;
    int fakeLinkPorts[2]  = {0, 1};
    fakeLink.initialize(fakeLinkConfig, fakeLinkInput, tLinks, fakeLinkPorts);

    /// - Configure the network minor step log and initialize.
    tNetwork.mStepLog.mInputData.mLogSteps    = 5;
    tNetwork.mStepLog.mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_SNAP;
    tNetwork.initialize(tNetworkConfig, tLinks);

    /// - Check call to links for sim bus writes during network init.
    CPPUNIT_ASSERT(fakeLink.callsToWrite == 1);

    /// - Set the network capacitance request for node 1 in response to unit flux and verify no
    ///   affect on the network solution.
    tBasicNodes[1].setNetworkCapacitanceRequest(1.0);

    /// - Step the network and verify the correct system values.
    tNetwork.step(tDeltaTime);

    /// - Check call to links for sim bus reads & writes.
    CPPUNIT_ASSERT(fakeLink.callsToRead  == 1);
    CPPUNIT_ASSERT(fakeLink.callsToWrite == 2);

    /// - This network should produce the following output of the system state:
    ///
    /// Admittance Matrix, pre-Decomposition:
    ///  1.0000000999999999e+05 -1.0000000000000000e-02  0.0000000000000000e+00
    /// -1.0000000000000000e-02  1.0010999999999999e+01 -1.0000000000000000e-03
    ///  0.0000000000000000e+00 -1.0000000000000000e-03  1.0000000000002221e-03
    /// Source Vector:
    ///  1.2500000000000000e+07
    ///  1.2500000000000000e+03
    /// -1.0000000000000000e-02
    /// Admittance Matrix, post-Decomposition:
    ///  1.0000000999999999e+05 -9.9999990000001005e-08  0.0000000000000000e+00
    /// -9.9999990000001005e-08  1.0010999998999999e+01 -9.9890120877024299e-05
    ///  0.0000000000000000e+00 -9.9890120877024299e-05  9.9990010987912399e-04
    /// Potential Vector:
    ///  1.2499999999990010e+02
    ///  1.2499900099900091e+02
    ///  1.1499900099900080e+02

    /// - Verify the source vector.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.25E7,
            tNetwork.mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.25E3,
            tNetwork.mSourceVector[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0E-2,
            tNetwork.mSourceVector[2], DBL_EPSILON);

    /// - Verify the upper diagonal of the decomposed admittance matrix.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0000000999999999E05,
            tNetwork.mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-9.9999990000001005E-8,
            tNetwork.mAdmittanceMatrix[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mAdmittanceMatrix[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0010999998999999E1,
            tNetwork.mAdmittanceMatrix[4], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-9.9890120877024299E-5,
            tNetwork.mAdmittanceMatrix[5], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.9990010987912399E-4,
            tNetwork.mAdmittanceMatrix[8], DBL_EPSILON);

    /// - Verify the symmetry of the decomposed admittance matrix.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[1],
            tNetwork.mAdmittanceMatrix[3], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[2],
            tNetwork.mAdmittanceMatrix[6], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[5],
            tNetwork.mAdmittanceMatrix[7], 0.0);

    /// - Verify the potential vector solution.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2499999999990010e+02,
            tNetwork.mPotentialVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2499900099900091e+02,
            tNetwork.mPotentialVector[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1499900099900080e+02,
            tNetwork.mPotentialVector[2], DBL_EPSILON);

    /// - Verify potential vector output to the links.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNetwork.mLinkPotentialVectors[1][1],
            tNetwork.mPotentialVector[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNetwork.mPotentialVector[2],
            tNetwork.mLinkPotentialVectors[4][0],0.0);

    /// - Verify outputs to the minor step log.
    FriendlyGunnsMinorStepLog* log = static_cast<FriendlyGunnsMinorStepLog*>(&tNetwork.mStepLog);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[0].mMajorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[0].mMinorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[0].mDecomposition);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[0]        == log->mBuffer[0].mPotentialVector[0]);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[1]        == log->mBuffer[0].mPotentialVector[1]);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[2]        == log->mBuffer[0].mPotentialVector[2]);
    CPPUNIT_ASSERT(0.0                                 == log->mBuffer[0].mNodesConvergence[0]);
    CPPUNIT_ASSERT(0.0                                 == log->mBuffer[0].mNodesConvergence[1]);
    CPPUNIT_ASSERT(0.0                                 == log->mBuffer[0].mNodesConvergence[2]);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM         == log->mBuffer[0].mSolutionResult);
    CPPUNIT_ASSERT(tPotential.getAdmittanceMatrix()[0] == log->mBuffer[0].mLinksData[0].mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(tSource.getSourceVector()[0]        == log->mBuffer[0].mLinksData[4].mSourceVector[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM             == log->mBuffer[0].mLinksData[0].mSolutionResult);

    /// - Step again and verify results of the 2nd major step.
    tNetwork.step(tDeltaTime);

    /// - This network should produce the following output of the system state on the 2nd step:
    ///
    /// Potential Vector:
    ///  1.2499999999980031e+02
    ///  1.2499800299600473e+02
    ///  1.1499800299600462e+02

    /// - Verify the potential vector solution.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2499999999980031e+02,
            tNetwork.mPotentialVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2499800299600473e+02,
            tNetwork.mPotentialVector[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1499800299600462e+02,
            tNetwork.mPotentialVector[2], DBL_EPSILON);

    /// - Verify outputs to the minor step log.
    CPPUNIT_ASSERT(2 == log->mBuffer[1].mMajorStep);
    CPPUNIT_ASSERT(1 == log->mBuffer[1].mMinorStep);
    CPPUNIT_ASSERT(0 == log->mBuffer[1].mDecomposition);

    /// - Step a 3rd time with tConductor2 overriding its port 0 potential, and verify the network
    ///   solution gets this override value and passes it to other incident links on that node.
    tConductor2.mOverrideVector[0] = true;
    tConductor2.mPotentialVector[0] = 125.0;
    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(125.0, tNetwork.mPotentialVector[1],       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(125.0, tConductor2.mPotentialVector[0],    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(125.0, tCapacitor.getPotentialVector()[0], 0.0);

    /// - Step a 4th time to verify the PAUSE run mode.
    tNetwork.setPauseMode();
    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(125.0, tNetwork.mPotentialVector[1],       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(125.0, tConductor2.mPotentialVector[0],    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(125.0, tCapacitor.getPotentialVector()[0], 0.0);
    CPPUNIT_ASSERT(fakeLink.callsToRead  == 3);
    CPPUNIT_ASSERT(fakeLink.callsToWrite == 4);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details This method tests a nominal step through a converging non-linear network and checks the
///          accuracy of the solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testNonLinearStep()
{
    std::cout << "\n UtGunns ................ 12: testNonLinearStep .....................";

    /// - Configure the network minor step log and set up the network.
    tNetwork.mStepLog.mInputData.mLogSteps    = 5;
    tNetwork.mStepLog.mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_SNAP;
    setupNominalNonLinearNetwork(true);

    /// - Verify minor step log init.
    FriendlyGunnsMinorStepLog* log = static_cast<FriendlyGunnsMinorStepLog*>(&tNetwork.mStepLog);
    CPPUNIT_ASSERT(5                         == log->mSize);
    CPPUNIT_ASSERT(5                         == log->mBuffer.size());
    CPPUNIT_ASSERT(GunnsMinorStepLog::PAUSED == log->mState);
    CPPUNIT_ASSERT(true                      == log->mIsRecording);
    CPPUNIT_ASSERT(4                         == log->mHeadIndex);
    CPPUNIT_ASSERT(4                         == log->mBuffer[0].mPotentialVector.size());
    CPPUNIT_ASSERT(4                         == log->mBuffer[4].mPotentialVector.size());
    CPPUNIT_ASSERT(8                         == log->mBuffer[0].mLinksData.size());
    CPPUNIT_ASSERT(8                         == log->mBuffer[4].mLinksData.size());
    CPPUNIT_ASSERT(4                         == log->mBuffer[4].mLinksData[4].mAdmittanceMatrix.size());
    CPPUNIT_ASSERT(2                         == log->mBuffer[4].mLinksData[4].mSourceVector.size());

    /// - Step the network and verify the correct system values.
    tNetwork.step(tDeltaTime);

    /// - This network should produce the following output of the system state on the 5th minor step
    ///   of the 1st major step:
    ///
    /// Admittance Matrix, pre-Decomposition:
    ///  1.0000000000000100e+14 -1.0000000000000000e+00  0.0000000000000000e+00  0.0000000000000000e+00
    /// -1.0000000000000000e+00  1.0460422954127834e+00 -5.7142857142857147e-04  0.0000000000000000e+00
    ///  0.0000000000000000e+00 -5.7142857142857147e-04  6.7142857142857163e-04 -1.0000000000000000e-04
    ///  0.0000000000000000e+00  0.0000000000000000e+00 -1.0000000000000000e-04  2.0000000000000001e-04
    /// Source Vector:
    ///  1.2500000000000000e+16
    ///  0.0000000000000000e+00
    ///  2.4411869147154661e-14
    ///  0.0000000000000000e+00
    /// Admittance Matrix, post-Decomposition:
    ///  1.0000000000000100e+14 -9.9999999999999006e-15  0.0000000000000000e+00  0.0000000000000000e+00
    /// -9.9999999999999006e-15  1.0460422954127300e+00 -5.4627673654735604e-04  0.0000000000000000e+00
    ///  0.0000000000000000e+00 -5.4627673654735604e-04  6.7111641329340224e-04 -1.4900544528372528e-01
    ///  0.0000000000000000e+00  0.0000000000000000e+00 -1.4900544528372528e-01  1.8509945547162749e-04
    /// Potential Vector:
    ///  1.2499999999999994e+02
    ///  1.1955809295177480e+02
    ///  1.0993847627749395e+02
    ///  5.4969238138746974e+01

    /// - Verify the source vector.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2500000000000000e+16,
            tNetwork.mSourceVector[0], DBL_EPSILON * 1.0E-15);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mSourceVector[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mSourceVector[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mSourceVector[3], DBL_EPSILON);

    /// - Verify the upper diagonal of the decomposed admittance matrix.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0000000000000100e+14,
            tNetwork.mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-9.9999999999999006e-15,
            tNetwork.mAdmittanceMatrix[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mAdmittanceMatrix[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mAdmittanceMatrix[3], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.04604229541273e+00,
            tNetwork.mAdmittanceMatrix[5], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-5.4627673654735604e-04,
            tNetwork.mAdmittanceMatrix[6], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mAdmittanceMatrix[7], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.7111641329340224e-04,
            tNetwork.mAdmittanceMatrix[10], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.4900544528372528e-01,
            tNetwork.mAdmittanceMatrix[11], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.8509945547162747e-04,
            tNetwork.mAdmittanceMatrix[15], DBL_EPSILON);

    /// - Verify the symmetry of the decomposed admittance matrix.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[1],
            tNetwork.mAdmittanceMatrix[4], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[2],
            tNetwork.mAdmittanceMatrix[8], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[6],
            tNetwork.mAdmittanceMatrix[9], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[3],
            tNetwork.mAdmittanceMatrix[12], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[7],
            tNetwork.mAdmittanceMatrix[13], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[11],
            tNetwork.mAdmittanceMatrix[14], 0.0);

    /// - Verify the potential vector solution.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2499999999999994e+02,
            tNetwork.mPotentialVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.195580929517748e+02,
            tNetwork.mPotentialVector[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0993847627749395e+02,
            tNetwork.mPotentialVector[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.4969238138746974e+01,
            tNetwork.mPotentialVector[3], DBL_EPSILON);

    /// - Verify the potential vector is saved for next step.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[0],
            tNetwork.mMinorPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[1],
            tNetwork.mMinorPotentialVector[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[2],
            tNetwork.mMinorPotentialVector[2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[3],
            tNetwork.mMinorPotentialVector[3], 0.0);

    /// - Verify minor step iteration & convergence metrics.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.0, tNetwork.mAvgMinorStepCount, DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL( 5, tNetwork.mMinorStepCount);
    CPPUNIT_ASSERT_EQUAL( 5, tNetwork.mDecompositionCount);
    CPPUNIT_ASSERT_EQUAL( 1, tNetwork.mMajorStepCount);
    CPPUNIT_ASSERT_EQUAL( 5, tNetwork.mMaxMinorStepCount);
    CPPUNIT_ASSERT_EQUAL( 0, tNetwork.mConvergenceFailCount);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,                    tNetwork.mNodesConvergence[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.0509654027355282e-03, tNetwork.mNodesConvergence[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.8054854277996810e-03, tNetwork.mNodesConvergence[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.4027427138927351e-03, tNetwork.mNodesConvergence[3], DBL_EPSILON);

    /// - Verify outputs to the minor step log.
    CPPUNIT_ASSERT(1                                   == log->mBuffer[0].mMajorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[1].mMajorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[2].mMajorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[3].mMajorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[4].mMajorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[0].mMinorStep);
    CPPUNIT_ASSERT(2                                   == log->mBuffer[1].mMinorStep);
    CPPUNIT_ASSERT(3                                   == log->mBuffer[2].mMinorStep);
    CPPUNIT_ASSERT(4                                   == log->mBuffer[3].mMinorStep);
    CPPUNIT_ASSERT(5                                   == log->mBuffer[4].mMinorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[0].mDecomposition);
    CPPUNIT_ASSERT(2                                   == log->mBuffer[1].mDecomposition);
    CPPUNIT_ASSERT(3                                   == log->mBuffer[2].mDecomposition);
    CPPUNIT_ASSERT(4                                   == log->mBuffer[3].mDecomposition);
    CPPUNIT_ASSERT(5                                   == log->mBuffer[4].mDecomposition);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[0]        == log->mBuffer[4].mPotentialVector[0]);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[1]        == log->mBuffer[4].mPotentialVector[1]);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[2]        == log->mBuffer[4].mPotentialVector[2]);
    CPPUNIT_ASSERT(tNetwork.mNodesConvergence[0]       == log->mBuffer[4].mNodesConvergence[0]);
    CPPUNIT_ASSERT(tNetwork.mNodesConvergence[1]       == log->mBuffer[4].mNodesConvergence[1]);
    CPPUNIT_ASSERT(tNetwork.mNodesConvergence[2]       == log->mBuffer[4].mNodesConvergence[2]);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM         == log->mBuffer[0].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM         == log->mBuffer[1].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM         == log->mBuffer[2].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM         == log->mBuffer[3].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::SUCCESS         == log->mBuffer[4].mSolutionResult);
    CPPUNIT_ASSERT(tPotential.getAdmittanceMatrix()[0] == log->mBuffer[4].mLinksData[0].mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(tConstantLoad2.getSourceVector()[0] == log->mBuffer[4].mLinksData[6].mSourceVector[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM             == log->mBuffer[4].mLinksData[0].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepLog::RECORDING_SNAP   == log->mState);
    CPPUNIT_ASSERT(true                                == log->mIsRecording);
    CPPUNIT_ASSERT(4                                   == log->mHeadIndex);

    /// - Step again and verify results of the 2nd major step.  This time link tFake1 will delay to
    ///   converged minor step 3 -- this verifies that extra decompositions aren't performed when
    ///   the network has already converged and we are delaying.
    tFake1.delayToConvergedStep = 3;
    tNetwork.step(tDeltaTime);

    /// - This network should produce the following verbose output of the system state on the 1st
    ///   minor step of the 2nd major step:
    ///
    /// Admittance Matrix, pre-Decomposition:
    ///  1.0000000000000100e+14 -1.0000000000000000e+00  0.0000000000000000e+00  0.0000000000000000e+00
    /// -1.0000000000000000e+00  1.0460446161558739e+00 -5.7142857142857147e-04  0.0000000000000000e+00
    ///  0.0000000000000000e+00 -5.7142857142857147e-04  6.7142857142857163e-04 -1.0000000000000000e-04
    ///  0.0000000000000000e+00  0.0000000000000000e+00 -1.0000000000000000e-04  2.0000000000000001e-04
    /// Source Vector:
    ///  1.2500000000000000e+16
    ///  0.0000000000000000e+00
    ///  2.4411245589647365e-14
    ///  0.0000000000000000e+00
    /// Admittance Matrix, post-Decomposition:
    ///  1.0000000000000100e+14 -9.9999999999999006e-15  0.0000000000000000e+00  0.0000000000000000e+00
    /// -9.9999999999999006e-15  1.0460446161558639e+00 -5.4627552458376871e-04  0.0000000000000000e+00
    ///  0.0000000000000000e+00 -5.4627552458376871e-04  6.7111641398595286e-04 -1.4900544512996086e-01
    ///  0.0000000000000000e+00  0.0000000000000000e+00 -1.4900544512996086e-01  1.8509945548700392e-04
    /// Potential Vector:
    ///  1.2499999999999994e+02
    ///  1.1955782756819053e+02
    ///  1.0993823224661188e+02
    ///  5.4969116123305930e+01

    /// - Verify the source vector.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2500000000000000e+16,
            tNetwork.mSourceVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mSourceVector[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mSourceVector[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mSourceVector[3], DBL_EPSILON);

    /// - Verify the upper diagonal of the decomposed admittance matrix.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0000000000000100e+14,
            tNetwork.mAdmittanceMatrix[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-9.9999999999999006e-15,
            tNetwork.mAdmittanceMatrix[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mAdmittanceMatrix[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mAdmittanceMatrix[3], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0460446161558639e+00,
            tNetwork.mAdmittanceMatrix[5], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-5.4627552458376871e-04,
            tNetwork.mAdmittanceMatrix[6], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNetwork.mAdmittanceMatrix[7], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.7111641398595286e-04,
            tNetwork.mAdmittanceMatrix[10], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.4900544512996086e-01,
            tNetwork.mAdmittanceMatrix[11], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.8509945548700392e-04,
            tNetwork.mAdmittanceMatrix[15], DBL_EPSILON);

    /// - Verify the symmetry of the decomposed admittance matrix.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[1],
            tNetwork.mAdmittanceMatrix[4], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[2],
            tNetwork.mAdmittanceMatrix[8], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[6],
            tNetwork.mAdmittanceMatrix[9], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[3],
            tNetwork.mAdmittanceMatrix[12], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[7],
            tNetwork.mAdmittanceMatrix[13], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mAdmittanceMatrix[11],
            tNetwork.mAdmittanceMatrix[14], 0.0);

    /// - Verify the potential vector solution.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2499999999999994e+02,
            tNetwork.mPotentialVector[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1955782756819053e+02,
            tNetwork.mPotentialVector[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0993823224661188e+02,
            tNetwork.mPotentialVector[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.4969116123305930e+01,
            tNetwork.mPotentialVector[3], DBL_EPSILON);

    /// - Verify the potential vector is saved for next step.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[0],
            tNetwork.mMinorPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[1],
            tNetwork.mMinorPotentialVector[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[2],
            tNetwork.mMinorPotentialVector[2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[3],
            tNetwork.mMinorPotentialVector[3], 0.0);

    /// - Verify minor step iteration & convergence metrics.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.0, tNetwork.mAvgMinorStepCount, DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL( 8, tNetwork.mMinorStepCount);
    CPPUNIT_ASSERT_EQUAL( 6, tNetwork.mDecompositionCount);
    CPPUNIT_ASSERT_EQUAL( 2, tNetwork.mMajorStepCount);
    CPPUNIT_ASSERT_EQUAL( 5, tNetwork.mMaxMinorStepCount);
    CPPUNIT_ASSERT_EQUAL( 0, tNetwork.mConvergenceFailCount);

    /// - Verify outputs to the minor step log.
    CPPUNIT_ASSERT(2                                   == log->mBuffer[0].mMajorStep);
    CPPUNIT_ASSERT(2                                   == log->mBuffer[1].mMajorStep);
    CPPUNIT_ASSERT(2                                   == log->mBuffer[2].mMajorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[3].mMajorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[4].mMajorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[0].mMinorStep);
    CPPUNIT_ASSERT(2                                   == log->mBuffer[1].mMinorStep);
    CPPUNIT_ASSERT(3                                   == log->mBuffer[2].mMinorStep);
    CPPUNIT_ASSERT(4                                   == log->mBuffer[3].mMinorStep);
    CPPUNIT_ASSERT(5                                   == log->mBuffer[4].mMinorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[0].mDecomposition);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[1].mDecomposition);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[2].mDecomposition);
    CPPUNIT_ASSERT(4                                   == log->mBuffer[3].mDecomposition);
    CPPUNIT_ASSERT(5                                   == log->mBuffer[4].mDecomposition);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[0]        == log->mBuffer[2].mPotentialVector[0]);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[1]        == log->mBuffer[2].mPotentialVector[1]);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[2]        == log->mBuffer[2].mPotentialVector[2]);
    CPPUNIT_ASSERT(tNetwork.mNodesConvergence[0]       == log->mBuffer[2].mNodesConvergence[0]);
    CPPUNIT_ASSERT(tNetwork.mNodesConvergence[1]       == log->mBuffer[2].mNodesConvergence[1]);
    CPPUNIT_ASSERT(tNetwork.mNodesConvergence[2]       == log->mBuffer[2].mNodesConvergence[2]);
    CPPUNIT_ASSERT(GunnsMinorStepData::DELAY           == log->mBuffer[0].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::DELAY           == log->mBuffer[1].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::SUCCESS         == log->mBuffer[2].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM         == log->mBuffer[3].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::SUCCESS         == log->mBuffer[4].mSolutionResult);
    CPPUNIT_ASSERT(tPotential.getAdmittanceMatrix()[0] == log->mBuffer[2].mLinksData[0].mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(tConstantLoad2.getSourceVector()[0] == log->mBuffer[2].mLinksData[6].mSourceVector[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM             == log->mBuffer[2].mLinksData[0].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepLog::RECORDING_SNAP   == log->mState);
    CPPUNIT_ASSERT(true                                == log->mIsRecording);
    CPPUNIT_ASSERT(2                                   == log->mHeadIndex);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the exception handling of the step() method of the Gunns class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testLinearStepExceptions()
{
    std::cout << "\n UtGunns ................ 13: testLinearStepExceptions ..............";

    /// - Initialize the basic nodes.
    tBasicNodes[0].initialize("BasicNode1");
    tBasicNodes[1].initialize("BasicNode2");
    tBasicNodes[2].initialize("BasicNode3");
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tBasicNodes;
    tNetwork.initializeNodes(tNodeList);

    /// - Create an admittance matrix that will fail in decomposition, then step and verify an
    ///   exception is thrown.
    UtGunnsFakeLink          tLink(2, false, false);
    GunnsBasicLinkConfigData tLinkConfig;
    GunnsBasicLinkInputData  tLinkInput;
    tLinkConfig.mName     = "tLink";
    tLinkConfig.mNodeList = &tNodeList;
    int tLinkPorts[2]  = {0, 1};

    tLink.initialize(tLinkConfig, tLinkInput, tLinks, tLinkPorts);

    /// - Configure the network minor step log and initialize the network.
    tNetwork.mStepLog.mInputData.mLogSteps    = 5;
    tNetwork.mStepLog.mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_SNAP;
    tNetwork.initialize(tNetworkConfig, tLinks);

    /// - Non-positive-definite matrix.
    tLink.mAdmittanceMatrix[0] = 1.0;
    tLink.mAdmittanceMatrix[1] =-1.1;
    tLink.mAdmittanceMatrix[2] =-1.1;
    tLink.mAdmittanceMatrix[3] = 1.0;
    tLink.mAdmittanceUpdate = true;

    /// - Expect a TsOutOfBoundsException for matrix decomposition error.
    CPPUNIT_ASSERT_THROW(tNetwork.step(tDeltaTime), TsNumericalException);

    /// - Verify outputs to the minor step log.
    FriendlyGunnsMinorStepLog* log = static_cast<FriendlyGunnsMinorStepLog*>(&tNetwork.mStepLog);
    CPPUNIT_ASSERT(1                              == log->mBuffer[0].mMajorStep);
    CPPUNIT_ASSERT(1                              == log->mBuffer[0].mMinorStep);
    CPPUNIT_ASSERT(1                              == log->mBuffer[0].mDecomposition);
    CPPUNIT_ASSERT(0.0                            == log->mBuffer[0].mPotentialVector[0]);
    CPPUNIT_ASSERT(0.0                            == log->mBuffer[0].mPotentialVector[1]);
    CPPUNIT_ASSERT(0.0                            == log->mBuffer[0].mNodesConvergence[0]);
    CPPUNIT_ASSERT(0.0                            == log->mBuffer[0].mNodesConvergence[1]);
    CPPUNIT_ASSERT(GunnsMinorStepData::MATH_FAIL  == log->mBuffer[0].mSolutionResult);
    CPPUNIT_ASSERT(tLink.getAdmittanceMatrix()[0] == log->mBuffer[0].mLinksData[0].mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(tLink.getAdmittanceMatrix()[1] == log->mBuffer[0].mLinksData[0].mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT(tLink.getSourceVector()[0]     == log->mBuffer[0].mLinksData[0].mSourceVector[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM        == log->mBuffer[0].mLinksData[0].mSolutionResult);

    /// - Create an system of equations that decomposes fine, but fails in forward/backwards
    ///   substitution, then step and verify an exception is thrown.
    //    TODO I'm not aware of a possible matrix that isn't covered by the decomposition error
    //    handler or the conditionMatrix method.  Need a matrix that after LDU decomposition has
    //    zero on a diagonal.
    /// - Expect a TsNumericalException for forward/backwards substitution error.
//    tLink.mAdmittanceMatrix[0] = 1.0;
//    tLink.mAdmittanceMatrix[1] =-sqrt(0.01+DBL_EPSILON);
//    tLink.mAdmittanceMatrix[2] =-sqrt(0.01+DBL_EPSILON);
//    tLink.mAdmittanceMatrix[3] = 0.01;
//    tLink.mAdmittanceUpdate = true;
//    CPPUNIT_ASSERT_THROW(tNetwork.step(), TsNumericalException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the handling of non-linear networks failing to converge.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testNonConvergence()
{
    std::cout << "\n UtGunns ................ 14: testNonConvergence ....................";

    /// - Configure the network minor step log and set up the network.
    tNetwork.mStepLog.mInputData.mLogSteps    = 10;
    tNetwork.mStepLog.mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_SNAP;
    setupNominalNonLinearNetwork(false);

    /// - Step the network and verify the correct system values.  The network will exceed the
    ///   decomposition limit during the 6th minor frame.
    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT_EQUAL        (1,   tNetwork.mMajorStepCount);
    CPPUNIT_ASSERT_EQUAL        (1,   tNetwork.mConvergenceFailCount);
    CPPUNIT_ASSERT_EQUAL        (6,   tNetwork.mMinorStepCount);
    CPPUNIT_ASSERT_EQUAL        (6,   tNetwork.mMaxMinorStepCount);
    CPPUNIT_ASSERT_EQUAL        (6,   tNetwork.mLastMinorStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, tNetwork.mAvgMinorStepCount,     0.0);
    CPPUNIT_ASSERT_EQUAL        (5,   tNetwork.mDecompositionCount);
    CPPUNIT_ASSERT_EQUAL        (5,   tNetwork.mMaxDecompositionCount);
    CPPUNIT_ASSERT_EQUAL        (5,   tNetwork.mLastDecomposition);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, tNetwork.mAvgDecompositionCount, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5631940186722204e-13, tNetwork.mNodesConvergence[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5277576986464879e+01, tNetwork.mNodesConvergence[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.4048346654220552e+01, tNetwork.mNodesConvergence[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.0241733271102795e+00, tNetwork.mNodesConvergence[3], DBL_EPSILON);

    /// - Verify outputs to the minor step log.
    FriendlyGunnsMinorStepLog* log = static_cast<FriendlyGunnsMinorStepLog*>(&tNetwork.mStepLog);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[5].mMajorStep);
    CPPUNIT_ASSERT(6                                   == log->mBuffer[5].mMinorStep);
    CPPUNIT_ASSERT(5                                   == log->mBuffer[5].mDecomposition);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[0]        == log->mBuffer[5].mPotentialVector[0]);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[1]        == log->mBuffer[5].mPotentialVector[1]);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[2]        == log->mBuffer[5].mPotentialVector[2]);
    CPPUNIT_ASSERT(tNetwork.mNodesConvergence[0]       == log->mBuffer[5].mNodesConvergence[0]);
    CPPUNIT_ASSERT(tNetwork.mNodesConvergence[1]       == log->mBuffer[5].mNodesConvergence[1]);
    CPPUNIT_ASSERT(tNetwork.mNodesConvergence[2]       == log->mBuffer[5].mNodesConvergence[2]);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM         == log->mBuffer[0].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM         == log->mBuffer[1].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM         == log->mBuffer[2].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM         == log->mBuffer[3].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM         == log->mBuffer[4].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::DECOMP_LIMIT    == log->mBuffer[5].mSolutionResult);
    CPPUNIT_ASSERT(tPotential.getAdmittanceMatrix()[0] == log->mBuffer[5].mLinksData[0].mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(tConstantLoad2.getSourceVector()[0] == log->mBuffer[5].mLinksData[6].mSourceVector[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM             == log->mBuffer[5].mLinksData[0].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepLog::RECORDING_SNAP   == log->mState);
    CPPUNIT_ASSERT(true                                == log->mIsRecording);
    CPPUNIT_ASSERT(5                                   == log->mHeadIndex);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the interface to fluid nodes in a fluid network step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testFluidNetworkStep()
{
    std::cout << "\n UtGunns ................ 15: testFluidNetworkStep ..................";

    /// - Initialize the fluid nodes.
    tFluidNodes[0].initialize("FluidNode1",tFluidConfig,tFluid);
    tFluidNodes[1].initialize("FluidNode2",tFluidConfig,tFluid);
    tFluidNodes[2].initialize("FluidNode3",tFluidConfig,tFluid);
    tFluidNodes[3].initialize("FluidNode4",tFluidConfig);
    tNodeList.mNumNodes = 4;
    tNodeList.mNodes    = tFluidNodes;
    tNetwork.initializeFluidNodes(tNodeList);

    /// - Setup the links config and input data.
    tFluidConductor1Config.mName                  = "tFluidConductor1";
    tFluidConductor1Config.mNodeList              = &tNodeList;
    tFluidConductor1Config.mMaxConductivity       = 1.0;
    tFluidConductor1Config.mExpansionScaleFactor  = 0.1;
    tFluidConductor2Config.mName                  = "tFluidConductor2";
    tFluidConductor2Config.mNodeList              = &tNodeList;
    tFluidConductor2Config.mMaxConductivity       = 2.0;
    tFluidConductor2Config.mExpansionScaleFactor  = 0.2;
    tFluidConductor3Config.mName                  = "tFluidConductor3";
    tFluidConductor3Config.mNodeList              = &tNodeList;
    tFluidConductor3Config.mMaxConductivity       = 3.0;
    tFluidConductor3Config.mExpansionScaleFactor  = 0.3;
    tFluidCapacitor1Config.mName                  = "tFluidCapacitor1";
    tFluidCapacitor1Config.mNodeList              = &tNodeList;
    tFluidCapacitor1Config.mExpansionScaleFactor  = 0.4;
    tFluidConductor1Input = new GunnsFluidConductorInputData(false, 0.0);
    tFluidConductor2Input = new GunnsFluidConductorInputData(true,  0.5);
    tFluidConductor3Input = new GunnsFluidConductorInputData(false, 0.0);
    tFractions[0] = 1.0;
    tFractions[1] = 0.0;
    tFractions[2] = 0.0;
    tFluid = new PolyFluidInputData(283.15,                 //temperature
                                    689.475728,             //pressure
                                    0.0,                    //flowRate
                                    0.0,                    //mass
                                    tFractions);            //massFraction
    tFluidCapacitor1Input = new GunnsFluidCapacitorInputData(false, 0.0, 4.0, tFluid);

    /// - Add the links to the network.
    tFluidConductor1.initialize(tFluidConductor1Config, *tFluidConductor1Input, tLinks, 0, 1);
    tFluidConductor2.initialize(tFluidConductor2Config, *tFluidConductor2Input, tLinks, 1, 2);
    tFluidConductor3.initialize(tFluidConductor3Config, *tFluidConductor3Input, tLinks, 0, 3);
    tFluidCapacitor1.initialize(tFluidCapacitor1Config, *tFluidCapacitor1Input, tLinks, 2, 3);

    delete tFluidConductor1Input;
    delete tFluidConductor2Input;
    delete tFluidConductor3Input;
    delete tFluidCapacitor1Input;
    delete tFluid;

    /// - Initialize the network with configuration data.
    tNetwork.initialize(tNetworkConfig, tLinks);

    /// - Step the network and verify the fluid nodes get their integrateFlows method called.
    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.9715166132501332e+02, tFluidNodes[1].mInfluxRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.8377328374256626e+07, tFluidNodes[2].mNetHeatFlux, DBL_EPSILON);

    /// - Step again with the link conductivities shut off and verify the fluid nodes get their
    ///   restFlows method called.
    tFluidConductor1.mMalfBlockageFlag  = true;
    tFluidConductor2.mMalfBlockageFlag  = true;
    tFluidConductor3.mMalfBlockageFlag  = true;
    tFluidConductor1.mMalfBlockageValue = 1.0;
    tFluidConductor2.mMalfBlockageValue = 1.0;
    tFluidConductor3.mMalfBlockageValue = 1.0;
    tNetwork.step(tDeltaTime);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFluidNodes[1].mInfluxRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tFluidNodes[1].mNetHeatFlux, DBL_EPSILON);
    CPPUNIT_ASSERT(2 == tNetwork.mDecompositionCount);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the handshaking with non-linear links for confirming or rejecting
///           the potential solution, and associated outputs.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testSolutionRejection()
{
    std::cout << "\n UtGunns ................ 16: testSolutionRejection .................";

    /// - Initialize the basic nodes.
    tBasicNodes[0].initialize("BasicNode1");
    tBasicNodes[1].initialize("BasicNode2");
    tBasicNodes[2].initialize("BasicNode3");
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tBasicNodes;
    tNetwork.initializeNodes(tNodeList);

    /// - Set up a simple basic network with our fake test links.  tLink2 is set up to reject the
    ///   solution and fail to reset to the last minor step.
    UtGunnsFakeLink          tLink1(2, true, false);
    UtGunnsFakeLink          tLink2(2, true, true);

    GunnsBasicLinkConfigData tLink1Config;
    GunnsBasicLinkConfigData tLink2Config;
    GunnsBasicLinkInputData  tLink1Input;
    GunnsBasicLinkInputData  tLink2Input;

    tLink1Config.mName     = "tLink1";
    tLink2Config.mName     = "tLink2";
    tLink1Config.mNodeList = &tNodeList;
    tLink2Config.mNodeList = &tNodeList;

    int tLink1Map[2] = {0, 1};
    int tLink2Map[2] = {1, 2};

    tLink1.initialize(tLink1Config, tLink1Input, tLinks, tLink1Map);
    tLink2.initialize(tLink2Config, tLink2Input, tLinks, tLink2Map);

    /// - Configure the network minor step log and initialize the network.
    tNetwork.mStepLog.mInputData.mLogSteps    = 3;
    tNetwork.mStepLog.mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_SNAP;
    tNetworkConfig.mMinorStepLimit = 2;
    tNetworkConfig.mConvergenceTolerance = 0.01;
    tNetwork.initialize(tNetworkConfig, tLinks);

    /// - Set the previous potential vectors that the system will return to.
    tNetwork.mMinorPotentialVector[0] = 120.0;
    tNetwork.mMinorPotentialVector[1] =  60.0;
    tNetwork.mMajorPotentialVector[0] = 125.0;
    tNetwork.mMajorPotentialVector[1] =  65.0;

    /// - Step the network and verify the potential vector gets reset because the solution was
    ///   rejected.
    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT(1 == tNetwork.mLinkResetStepFailCount);

    /// - Verify the network reset to the last minor step on the first minor frame.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tLink1.savedPotential[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 60.0, tLink1.savedPotential[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 60.0, tLink2.savedPotential[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, tLink2.savedPotential[1], 0.0);

    /// - Verify the network reset to the last major step at the end.
    CPPUNIT_ASSERT(1 == tNetwork.mConvergenceFailCount);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(125.0, tNetwork.mPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 65.0, tNetwork.mPotentialVector[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(125.0, tLink1.mPotentialVector[0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 65.0, tLink1.mPotentialVector[1],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 65.0, tLink2.mPotentialVector[0],   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, tLink2.mPotentialVector[1],   0.0);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tNetwork.mLinksConvergence[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT  == tNetwork.mLinksConvergence[1]);

    /// - Verify outputs to the minor step log.
    FriendlyGunnsMinorStepLog* log = static_cast<FriendlyGunnsMinorStepLog*>(&tNetwork.mStepLog);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[0].mMajorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[1].mMajorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[0].mMinorStep);
    CPPUNIT_ASSERT(2                                   == log->mBuffer[1].mMinorStep);
    CPPUNIT_ASSERT(1                                   == log->mBuffer[0].mDecomposition);
    CPPUNIT_ASSERT(2                                   == log->mBuffer[1].mDecomposition);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[0]        == log->mBuffer[1].mPotentialVector[0]);
    CPPUNIT_ASSERT(tNetwork.mPotentialVector[1]        == log->mBuffer[1].mPotentialVector[1]);
    CPPUNIT_ASSERT(tNetwork.mNodesConvergence[0]       == log->mBuffer[1].mNodesConvergence[0]);
    CPPUNIT_ASSERT(tNetwork.mNodesConvergence[1]       == log->mBuffer[1].mNodesConvergence[1]);
    CPPUNIT_ASSERT(GunnsMinorStepData::REJECT          == log->mBuffer[0].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::REJECT          == log->mBuffer[1].mSolutionResult);
    CPPUNIT_ASSERT(tLink1.getAdmittanceMatrix()[0]     == log->mBuffer[1].mLinksData[0].mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(tLink1.getSourceVector()[0]         == log->mBuffer[1].mLinksData[0].mSourceVector[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM             == log->mBuffer[0].mLinksData[0].mSolutionResult);
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == log->mBuffer[0].mLinksData[1].mSolutionResult);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM             == log->mBuffer[1].mLinksData[0].mSolutionResult);
    CPPUNIT_ASSERT(GunnsBasicLink::REJECT              == log->mBuffer[1].mLinksData[1].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepLog::RECORDING_SNAP   == log->mState);
    CPPUNIT_ASSERT(true                                == log->mIsRecording);
    CPPUNIT_ASSERT(1                                   == log->mHeadIndex);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the unique functionality of the Gunns DUMMY solver mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testDummyMode()
{
    std::cout << "\n UtGunns ................ 17: testDummyMode .........................";

    /// - Initialize the basic nodes.
    tBasicNodes[0].initialize("BasicNode1");
    tBasicNodes[1].initialize("BasicNode2");
    tNodeList.mNumNodes = 2;
    tNodeList.mNodes    = tBasicNodes;
    tNetwork.initializeNodes(tNodeList);

    /// - Set up a simple basic network.
    tConductor1Config.mName                = "Conductor1";
    tConductor1Config.mNodeList            = &tNodeList;
    tConductor1Config.mDefaultConductivity = 0.01;
    tConductor2Config.mName                = "Conductor2";
    tConductor2Config.mNodeList            = &tNodeList;
    tConductor2Config.mDefaultConductivity = 0.1;

    GunnsBasicConductorInputData tConductor1Input(false, 0.0);
    GunnsBasicConductorInputData tConductor2Input(false, 0.0);

    tConductor1.initialize(tConductor1Config, tConductor1Input, tLinks, 0, 1);
    tConductor2.initialize(tConductor2Config, tConductor2Input, tLinks, 0, 1);

    tNetwork.initialize(tNetworkConfig, tLinks);

    /// - Put the network in DUMMY mode, and set the link's port 0 potential.
    tNetwork.setDummyMode();
    tConductor1.mPotentialVector[0] = 125.0;
    tConductor2.mPotentialVector[0] = 105.0;

    /// - Step the network and verify the links are called and still have their initial potential.
    ///   Expected conductor fluxes are mPotentialVector[0] * mDefaultConductance.
    tNetwork.step(tDeltaTime);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(125.0, tConductor1.mPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(105.0, tConductor2.mPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.250, tConductor1.mFlux,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.50, tConductor2.mFlux,               0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the unique functionality of the Gunns SLAVE solver mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testSlaveMode()
{
    std::cout << "\n UtGunns ................ 18: testSlaveMode .........................";

    /// - Initialize the basic nodes.
    tBasicNodes[0].initialize("BasicNode1");
    tBasicNodes[1].initialize("BasicNode2");
    tNodeList.mNumNodes = 2;
    tNodeList.mNodes    = tBasicNodes;
    tNetwork.initializeNodes(tNodeList);

    /// - Set up a simple basic network.
    tConductor1Config.mName                = "Conductor1";
    tConductor1Config.mNodeList            = &tNodeList;
    tConductor1Config.mDefaultConductivity = 0.01;
    tCapacitorConfig.mName                 = "Capacitor";
    tCapacitorConfig.mNodeList             = &tNodeList;

    GunnsBasicConductorInputData tConductorInput(false, 0.0);
    GunnsBasicCapacitorInputData tCapacitorInput(false, 0.0, 1.0, 125.0);

    tConductor1.initialize(tConductor1Config, tConductorInput, tLinks, 0, 1);
    tCapacitor .initialize(tCapacitorConfig,  tCapacitorInput, tLinks, 0, 1);

    tNetwork.initialize(tNetworkConfig, tLinks);

    /// - Put the network in SLAVE mode and set up the slave potential vector.
    tNetwork.setSlaveMode();
    const double slavePotentials[1] = {100.0};
    tNetwork.setSlavePotentialVector(slavePotentials);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, tNetwork.mSlavePotentialVector[0], 0.0);

    /// - Step the network and verify the solution vector takes the slave values.
    tNetwork.step(tDeltaTime);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, tConductor1.getPotentialVector()[0], 0.0);

    /// - Switch back to NORMAL mode and verify the network can step as normal.
    tNetwork.setNormalMode();
    tNetwork.step(tDeltaTime);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.990009990009990e+01, tConductor1.getPotentialVector()[0],
                                 DBL_EPSILON);

    /// - Verify setSlavePotentials does nothing when not in SLAVE mode.
    tNetwork.mSlavePotentialVector[0] = 0.0;
    tNetwork.setSlavePotentialVector(slavePotentials);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetwork.mSlavePotentialVector[0], 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the handshaking with non-linear links for delaying the potential
///           solution, and associated outputs.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testSolutionDelay()
{
    std::cout << "\n UtGunns ................ 19: testSolutionDelay .....................";

    /// - Configure the network minor step log and set up the network.
    tNetwork.mStepLog.mInputData.mLogSteps    = 20;
    tNetwork.mStepLog.mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_SNAP;
    setupNominalNonLinearNetwork(true);

    /// - Set up tFake1 to delay the solution until minor step 3.  Step the network and verify it
    ///   converges on the 5th minor step even though tFake1 attempted to delay prior to that.
    tFake1.delayToAbsoluteStep = 3;
    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT_EQUAL(5, tNetwork.mMinorStepCount);
    CPPUNIT_ASSERT_EQUAL(5, tNetwork.mDecompositionCount);
    CPPUNIT_ASSERT_EQUAL(4, tFake1.callsToMinorStep);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tNetwork.mLinksConvergence[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM == tNetwork.mLinksConvergence[7]);

    /// - Verify outputs to the minor step log.
    FriendlyGunnsMinorStepLog* log = static_cast<FriendlyGunnsMinorStepLog*>(&tNetwork.mStepLog);
    CPPUNIT_ASSERT(5                           == log->mBuffer[4].mMinorStep);
    CPPUNIT_ASSERT(5                           == log->mBuffer[4].mDecomposition);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM == log->mBuffer[0].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM == log->mBuffer[1].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM == log->mBuffer[2].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::CONFIRM == log->mBuffer[3].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::SUCCESS == log->mBuffer[4].mSolutionResult);

    /// - Now step again, this time tFake1 is set up to delay the solution until the 3rd frame after
    ///   the system converged.
    tFake1.delayToAbsoluteStep  = 0;
    tFake1.delayToConvergedStep = 3;
    tNetwork.step(tDeltaTime);

    /// - Verify that the link minor step method was not called, and decomposition was not performed
    ///   on delayed frames.
    CPPUNIT_ASSERT_EQUAL(8, tNetwork.mMinorStepCount);
    CPPUNIT_ASSERT_EQUAL(6, tNetwork.mDecompositionCount);
    CPPUNIT_ASSERT_EQUAL(4, tFake1.callsToMinorStep);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM     == tNetwork.mLinksConvergence[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM     == tNetwork.mLinksConvergence[7]);
    CPPUNIT_ASSERT(3                           == log->mBuffer[7].mMinorStep);
    CPPUNIT_ASSERT(1                           == log->mBuffer[7].mDecomposition);
    CPPUNIT_ASSERT(GunnsMinorStepData::DELAY   == log->mBuffer[5].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::DELAY   == log->mBuffer[6].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::SUCCESS == log->mBuffer[7].mSolutionResult);

    /// - Now step again, this time tLink1 is set up to delay the solution until the 11th frame
    ///   after the system converged, which is later than the minor step limit.
    tFake1.delayToConvergedStep = 11;
    tNetwork.step(tDeltaTime);

    /// - Verify that the network failed to converge.
    CPPUNIT_ASSERT_EQUAL(1,  tNetwork.mConvergenceFailCount);
    CPPUNIT_ASSERT_EQUAL(18, tNetwork.mMinorStepCount);
    CPPUNIT_ASSERT_EQUAL(7,  tNetwork.mDecompositionCount);
    CPPUNIT_ASSERT_EQUAL(4,  tFake1.callsToMinorStep);
    CPPUNIT_ASSERT(GunnsBasicLink::CONFIRM   == tNetwork.mLinksConvergence[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::DELAY     == tNetwork.mLinksConvergence[7]);
    CPPUNIT_ASSERT(10                        == log->mBuffer[17].mMinorStep);
    CPPUNIT_ASSERT(1                         == log->mBuffer[17].mDecomposition);
    CPPUNIT_ASSERT(GunnsMinorStepData::DELAY == log->mBuffer[15].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::DELAY == log->mBuffer[16].mSolutionResult);
    CPPUNIT_ASSERT(GunnsMinorStepData::DELAY == log->mBuffer[17].mSolutionResult);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the access methods of Gunns.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testAccessMethods()
{
    std::cout << "\n UtGunns ................ 20: testAccessMethods .....................";

    /// - Initialize the basic nodes.
    tBasicNodes[0].initialize("BasicNode1");
    tBasicNodes[1].initialize("BasicNode2");
    tBasicNodes[2].initialize("BasicNode3");
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tBasicNodes;
    tNetwork.initializeNodes(tNodeList);

    /// - Set up a simple basic network.
    tConductor1Config.mName                = "Conductor1";
    tConductor1Config.mNodeList            = &tNodeList;
    tConductor1Config.mDefaultConductivity = 0.01;
    tCapacitorConfig.mName                 = "Capacitor";
    tCapacitorConfig.mNodeList             = &tNodeList;

    GunnsBasicConductorInputData tConductorInput(false, 0.0);
    GunnsBasicCapacitorInputData tCapacitorInput(false, 0.0, 1.0, 125.0);

    tConductor1.initialize(tConductor1Config, tConductorInput, tLinks, 0, 1);
    tCapacitor .initialize(tCapacitorConfig,  tCapacitorInput, tLinks, 0, 1);

    tNetwork.initialize(tNetworkConfig, tLinks);

    /// - Test getter methods
    tNetwork.mAdmittanceMatrix[0] = 5.0;
    tNetwork.mAdmittanceMatrix[1] = 7.0;
    double* actualAdmittanceMatrix = tNetwork.getAdmittanceMatrix();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, actualAdmittanceMatrix[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, actualAdmittanceMatrix[1], 0.0);

    tNetwork.mSourceVector[0] = 5.0;
    tNetwork.mSourceVector[1] = 7.0;
    double* actualSourceVector = tNetwork.getSourceVector();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, actualSourceVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, actualSourceVector[1], 0.0);

    tNetwork.mPotentialVector[0] = 5.0;
    tNetwork.mPotentialVector[1] = 7.0;
    double* actualPotentialVector = tNetwork.getPotentialVector();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, actualPotentialVector[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, actualPotentialVector[1], 0.0);

    tNetwork.mAvgMinorStepCount = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, tNetwork.getAvgMinorStepCount(), 0.0);

    tNetwork.mMinorStepCount = 5;
    CPPUNIT_ASSERT_EQUAL(5, tNetwork.getMinorStepCount());

    tNetwork.mMajorStepCount = 5;
    CPPUNIT_ASSERT_EQUAL(5, tNetwork.getMajorStepCount());

    tNetwork.mMaxMinorStepCount = 5;
    CPPUNIT_ASSERT_EQUAL(5, tNetwork.getMaxMinorStepCount());

    tNetwork.mConvergenceFailCount = 5;
    CPPUNIT_ASSERT_EQUAL(5, tNetwork.getConvergenceFailCount());

    tNetwork.mLinkResetStepFailCount = 5;
    CPPUNIT_ASSERT_EQUAL(5, tNetwork.getLinkResetStepFailCount());

    tNetwork.mDecompositionCount = 5;
    CPPUNIT_ASSERT_EQUAL(5, tNetwork.getDecompositionCount());

    tNetwork.mSolveTime = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tNetwork.getSolveTime(), 0.0);

    tNetwork.mStepTime = 2.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, tNetwork.getStepTime(), 0.0);

    CPPUNIT_ASSERT(2                          == tNetwork.getNumLinks());
    CPPUNIT_ASSERT(2                          == tNetwork.getNetworkSize());
    CPPUNIT_ASSERT(tNetwork.mLinks            == tNetwork.getLinks());
    CPPUNIT_ASSERT(tNetwork.mLinksConvergence == tNetwork.getLinksConvergence());
    CPPUNIT_ASSERT(tNetwork.mNodesConvergence == tNetwork.getNodesConvergence());

    /// - Test setter methods
    tNetwork.setPauseMode();
    CPPUNIT_ASSERT(Gunns::PAUSE == tNetwork.mRunMode);

    tNetwork.setRunMode();
    CPPUNIT_ASSERT(Gunns::RUN == tNetwork.mRunMode);

    tNetwork.setGpuOptions(Gunns::GPU_SPARSE, 10);
    CPPUNIT_ASSERT(10 == tNetwork.mGpuSizeThreshold);
    if (tNetwork.isGpuEnabled()) {
        CPPUNIT_ASSERT(Gunns::GPU_SPARSE == tNetwork.mGpuMode);
    } else {
        CPPUNIT_ASSERT(Gunns::NO_GPU     == tNetwork.mGpuMode);
    }

    tNetwork.setWorstCaseTiming(true);
    CPPUNIT_ASSERT(true == tNetwork.mWorstCaseTiming);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the admittance matrix conditioning.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testMatrixConditioning()
{
    std::cout << "\n UtGunns ................ 21: testMatrixConditioning ................";

    /// verify that a totally isolated node solves to zero potential

    /// verify that two isolated nodes but connected to each other via very small conductivity solve
    /// to zero potential

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the saving of pre-decomposition admittance matrix values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testDebugPreDecomposition()
{
    std::cout << "\n UtGunns ................ 22: testDebugPreDecomposition .............";

    setupNominalNonLinearNetwork(true);

    /// - Set the debug controls to record row 2 on minor step 4.
    tNetwork.mDebugDesiredStep  = 4;
    tNetwork.mDebugDesiredSlice = 2;

    /// - Step the network and verify the correct system values.
    tNetwork.step(tDeltaTime);

    /// - Verify the saved slice, values taken from the pre-Decomposition comments in the
    ///   testNonLinearStep test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0000000000000000e+00, tNetwork.mDebugSavedSlice[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-5.7142857142857147e-04, tNetwork.mDebugSavedSlice[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.7142857142857163e-04, tNetwork.mDebugSavedSlice[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0000000000000000e-04, tNetwork.mDebugSavedSlice[3], DBL_EPSILON);

    /// - Set the debug controls to record the diagonal on minor step 1.
    tNetwork.mDebugDesiredStep  =  1;
    tNetwork.mDebugDesiredSlice = -1;

    /// - Step the network and verify the correct system values.
    tNetwork.step(tDeltaTime);

    /// - Verify the saved slice, values taken from the pre-Decomposition comments in the
    ///   testNonLinearStep test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0000000000000100e+14, tNetwork.mDebugSavedSlice[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0460446161558739e+00, tNetwork.mDebugSavedSlice[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.7142857142857163e-04, tNetwork.mDebugSavedSlice[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0000000000000001e-04, tNetwork.mDebugSavedSlice[3], DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the saving of node minor step potential values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testDebugNode()
{
    std::cout << "\n UtGunns ................ 23: testDebugNode .........................";

    setupNominalNonLinearNetwork(true);

    /// - Set the debug controls to record node 2.
    tNetwork.mDebugDesiredNode = 2;

    /// - Step the network and verify the correct system values.
    tNetwork.step(tDeltaTime);

    /// - Verify the saved node minor step potentials.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,                    tNetwork.mDebugSavedNode[0],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1493724426463116e+02, tNetwork.mDebugSavedNode[1],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1034661406574261e+02, tNetwork.mDebugSavedNode[2],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0997354938683942e+02, tNetwork.mDebugSavedNode[3],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0994128176292175e+02, tNetwork.mDebugSavedNode[4],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0993847627749395e+02, tNetwork.mDebugSavedNode[5],  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    tNetwork.mDebugSavedNode[6],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    tNetwork.mDebugSavedNode[7],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    tNetwork.mDebugSavedNode[8],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    tNetwork.mDebugSavedNode[9],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                    tNetwork.mDebugSavedNode[10], 0.0);

    /// - Force the network to fail to converge and verify recording is paused.
    tNetwork.mConvergenceTolerance = 0.0;
    tNetwork.step(tDeltaTime);
    CPPUNIT_ASSERT(-1 == tNetwork.mDebugDesiredNode);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the formulation of network islands, coordination of island info with
///           the nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testBuildIslands()
{
    std::cout << "\n UtGunns ................ 24: testBuildIslands ......................";

    setupIslandNetwork();

    /// - Step the network and verify the correct island values.
    tNetwork.setIslandMode(Gunns::FIND);
    CPPUNIT_ASSERT_EQUAL(Gunns::FIND, tNetwork.mIslandMode);

    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT_EQUAL(2, tNetwork.mIslandCount);
    CPPUNIT_ASSERT_EQUAL(5, tNetwork.mIslandMaxSize);
    CPPUNIT_ASSERT_EQUAL(0, tNetwork.mNodeIslandNumbers[0]);
    CPPUNIT_ASSERT_EQUAL(0, tNetwork.mNodeIslandNumbers[1]);
    CPPUNIT_ASSERT_EQUAL(0, tNetwork.mNodeIslandNumbers[2]);
    CPPUNIT_ASSERT_EQUAL(0, tNetwork.mNodeIslandNumbers[3]);
    CPPUNIT_ASSERT_EQUAL(4, tNetwork.mNodeIslandNumbers[4]);
    CPPUNIT_ASSERT_EQUAL(0, tNetwork.mNodeIslandNumbers[5]);

    CPPUNIT_ASSERT_EQUAL(0, tNetwork.mIslandVectors[0][0]);
    CPPUNIT_ASSERT_EQUAL(1, tNetwork.mIslandVectors[0][1]);
    CPPUNIT_ASSERT_EQUAL(2, tNetwork.mIslandVectors[0][2]);
    CPPUNIT_ASSERT_EQUAL(3, tNetwork.mIslandVectors[0][3]);
    CPPUNIT_ASSERT_EQUAL(5, tNetwork.mIslandVectors[0][4]);
    CPPUNIT_ASSERT_EQUAL(4, tNetwork.mIslandVectors[4][0]);

    /// - Verify nodes got supplied island vectors.
    CPPUNIT_ASSERT_EQUAL(static_cast< const std::vector<int>* >(&tNetwork.mIslandVectors[0]), tBasicNodes[0].mIslandVector);
    CPPUNIT_ASSERT_EQUAL(static_cast< const std::vector<int>* >(&tNetwork.mIslandVectors[0]), tBasicNodes[1].mIslandVector);
    CPPUNIT_ASSERT_EQUAL(static_cast< const std::vector<int>* >(&tNetwork.mIslandVectors[0]), tBasicNodes[2].mIslandVector);
    CPPUNIT_ASSERT_EQUAL(static_cast< const std::vector<int>* >(&tNetwork.mIslandVectors[0]), tBasicNodes[3].mIslandVector);
    CPPUNIT_ASSERT_EQUAL(static_cast< const std::vector<int>* >(&tNetwork.mIslandVectors[4]), tBasicNodes[4].mIslandVector);
    CPPUNIT_ASSERT_EQUAL(static_cast< const std::vector<int>* >(&tNetwork.mIslandVectors[0]), tBasicNodes[5].mIslandVector);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the solution by matrix islands.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testSolveIslands()
{
    std::cout << "\n UtGunns ................ 25: testSolveIslands ......................";

    setupIslandNetwork();

    /// - Step the network and verify the correct island values in OFF mode.
    tNetwork.setIslandMode(Gunns::OFF);
    CPPUNIT_ASSERT_EQUAL(Gunns::OFF, tNetwork.mIslandMode);

    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT_EQUAL(0,   tNetwork.mIslandCount);
    CPPUNIT_ASSERT_EQUAL(0,   tNetwork.mIslandMaxSize);
    CPPUNIT_ASSERT_EQUAL(0.0, tNetwork.mAdmittanceMatrixIsland[0]);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[0],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[1],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[2],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[3],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0,                tNetwork.mPotentialVector[4],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[5],  1.0E-8);

    /// - Step the network and verify the correct island values in FIND mode.  We have to dither
    ///   one of the conductivities a little to get the network to re-build.
    tNetwork.setIslandMode(Gunns::FIND);
    CPPUNIT_ASSERT_EQUAL(Gunns::FIND, tNetwork.mIslandMode);
    tPotential.mMalfBlockageFlag  = true;
    tPotential.mMalfBlockageValue = 1.0E-14;

    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT_EQUAL(2,   tNetwork.mIslandCount);
    CPPUNIT_ASSERT_EQUAL(5,   tNetwork.mIslandMaxSize);
    CPPUNIT_ASSERT_EQUAL(0.0, tNetwork.mAdmittanceMatrixIsland[0]);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[0],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[1],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[2],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[3],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0,                tNetwork.mPotentialVector[4],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[5],  1.0E-8);

    /// - Step the network and verify the correct island values in SOLVE mode.
    tNetwork.setIslandMode(Gunns::SOLVE);
    CPPUNIT_ASSERT_EQUAL(Gunns::SOLVE, tNetwork.mIslandMode);
    tPotential.mMalfBlockageValue = 1.5E-14;

    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT_EQUAL(2,   tNetwork.mIslandCount);
    CPPUNIT_ASSERT_EQUAL(5,   tNetwork.mIslandMaxSize);
    CPPUNIT_ASSERT_EQUAL(0.0, tNetwork.mAdmittanceMatrixIsland[5*5]);
    CPPUNIT_ASSERT(0.0 < tNetwork.mAdmittanceMatrixIsland[0]);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[0],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[1],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[2],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[3],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0,                tNetwork.mPotentialVector[4],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0,                tNetwork.mPotentialVector[5],  1.0E-8);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the worst-case timing mode when the network converges.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testWorstCaseTimingConverging()
{
    std::cout << "\n UtGunns ................ 26: testWorstCaseTimingConverging .........";

    setupNominalNonLinearNetwork(true);
    tNetwork.mWorstCaseTiming    = true;
    tNetwork.mMinorStepLimit     = 15;
    tNetwork.mDecompositionLimit = 10;

    /// - Step the network and verify the correct system values.
    tNetwork.step(tDeltaTime);

    /// - Verify minor step iteration & convergence metrics for a converging network.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 10.0, tNetwork.mAvgMinorStepCount, DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL(10, tNetwork.mMinorStepCount);
    CPPUNIT_ASSERT_EQUAL( 1, tNetwork.mMajorStepCount);
    CPPUNIT_ASSERT_EQUAL(10, tNetwork.mMaxMinorStepCount);
    CPPUNIT_ASSERT_EQUAL( 0, tNetwork.mConvergenceFailCount);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the worst-case timing mode when the network doesn't converge.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testWorstCaseTimingNonConverging()
{
    std::cout << "\n UtGunns ................ 27: testWorstCaseTimingNonConverging ......";

    setupNominalNonLinearNetwork(false);
    tNetwork.mWorstCaseTiming    = true;

    /// - Step the network and verify the correct system values.
    tNetwork.step(tDeltaTime);

    /// - Verify minor step iteration & convergence metrics for the non-converging network.
    CPPUNIT_ASSERT_EQUAL        (1,   tNetwork.mMajorStepCount);
    CPPUNIT_ASSERT_EQUAL        (1,   tNetwork.mConvergenceFailCount);
    CPPUNIT_ASSERT_EQUAL        (6,   tNetwork.mMinorStepCount);
    CPPUNIT_ASSERT_EQUAL        (6,   tNetwork.mMaxMinorStepCount);
    CPPUNIT_ASSERT_EQUAL        (6,   tNetwork.mLastMinorStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, tNetwork.mAvgMinorStepCount,     0.0);
    CPPUNIT_ASSERT_EQUAL        (5,   tNetwork.mDecompositionCount);
    CPPUNIT_ASSERT_EQUAL        (5,   tNetwork.mMaxDecompositionCount);
    CPPUNIT_ASSERT_EQUAL        (5,   tNetwork.mLastDecomposition);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, tNetwork.mAvgDecompositionCount, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the Gunns restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testRestart()
{
    std::cout << "\n UtGunns ................ 28: testRestart ...........................";

    setupNominalNonLinearNetwork(false);

    /// - Step the network.
    tNetwork.step(tDeltaTime);

    /// - Restart the network and verify restarted state.
    tNetwork.restart();
    CPPUNIT_ASSERT(tNetwork.mRebuild);
    CPPUNIT_ASSERT(tNetwork.mInitFlag);

    /// - Step the network post-restart and verify no exceptions thrown.
    CPPUNIT_ASSERT_NO_THROW(tNetwork.step(tDeltaTime));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the Gunns network capacitance methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testNetworkCapacitance()
{
    std::cout << "\n UtGunns ................ 29: testNetworkCapacitance ................";

    /// - Initialize the basic nodes.
    tBasicNodes[0].initialize("BasicNode1");
    tBasicNodes[1].initialize("BasicNode2");
    tBasicNodes[2].initialize("BasicNode3");
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tBasicNodes;
    tNetwork.initializeNodes(tNodeList);

    /// - Set up a simple basic network.
    tConductor1Config.mName                = "Conductor1";
    tConductor1Config.mNodeList            = &tNodeList;
    tConductor1Config.mDefaultConductivity = 1.0;
    tCapacitorConfig.mName                 = "Capacitor";
    tCapacitorConfig.mNodeList             = &tNodeList;

    GunnsBasicConductorInputData tConductor1Input(false, 0.0);
    GunnsBasicCapacitorInputData tCapacitorInput (false, 0.0, 1.0, 125.0);

    tConductor1.initialize(tConductor1Config, tConductor1Input, tLinks, 0, 1);
    tCapacitor .initialize(tCapacitorConfig,  tCapacitorInput,  tLinks, 1, 2);

    tNetwork.initialize(tNetworkConfig, tLinks);

    /// - Set the network capacitance request for node 0 in response to unit flux.
    tBasicNodes[0].setNetworkCapacitanceRequest(1.0);

    /// - Step the network and verify the correct system values.
    tNetwork.step(tDeltaTime);

    const double expectedCapacitance = tCapacitorInput.mCapacitance
                                     * tConductor1Config.mDefaultConductivity
                                     / (1.0/tDeltaTime + tConductor1Config.mDefaultConductivity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCapacitance, tBasicNodes[0].getNetworkCapacitance(),        FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                 tBasicNodes[0].getNetworkCapacitanceRequest(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                 tBasicNodes[1].getNetworkCapacitance(),        0.0);
    CPPUNIT_ASSERT(tNetwork.mNetCapDeltaPotential[3] == 0.0);
    CPPUNIT_ASSERT(tNetwork.mNetCapDeltaPotential[2] == 0.0);
    CPPUNIT_ASSERT(tNetwork.mNetCapDeltaPotential[1] >  0.0);
    CPPUNIT_ASSERT(tNetwork.mNetCapDeltaPotential[0] >  tNetwork.mNetCapDeltaPotential[1]);
    CPPUNIT_ASSERT(tNetwork.mNetCapDeltaPotential[0] == tBasicNodes[0].getNetCapDeltaPotential()[0]);
    CPPUNIT_ASSERT(tNetwork.mNetCapDeltaPotential[1] == tBasicNodes[0].getNetCapDeltaPotential()[1]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests for the decomposition without errors of a matrix over a range of
///           conductances with differing magnitudes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testSolveIslands2()
{
    std::cout << "\n UtGunns ................ 30: testSolveIslands2 .....................";

    setupIslandNetwork2();

    tNetwork.setIslandMode(Gunns::OFF);
    for (int exp1=-15; exp1<16; ++exp1) {
        tConductor1.setDefaultConductivity(powf(10.0, exp1));
        for (int exp2=-15; exp2<16; ++exp2) {
            tConductor2.setDefaultConductivity(powf(10.0, exp2));
            for (int exp3=-15; exp3<17; ++exp3) {
                tConductor3.setDefaultConductivity(powf(10.0, exp3));
                if (16 == exp3) tConductor3.setDefaultConductivity(0.0);
                CPPUNIT_ASSERT_NO_THROW(tNetwork.step(tDeltaTime));
// for debugging test failure:
//                try {
//                    tNetwork.step(tDeltaTime);
//                } catch (TsNumericalException& e) {
//                    std::cout << std::endl << "  OFF throw: " << exp1 << ", " << exp2 << ", " << exp3;
//                }
            }
        }
    }

    tNetwork.setIslandMode(Gunns::SOLVE);
    for (int exp1=-15; exp1<16; ++exp1) {
        tConductor1.setDefaultConductivity(powf(10.0, exp1));
        for (int exp2=-15; exp2<16; ++exp2) {
            tConductor2.setDefaultConductivity(powf(10.0, exp2));
            for (int exp3=-15; exp3<17; ++exp3) {
                tConductor3.setDefaultConductivity(powf(10.0, exp3));
                if (16 == exp3) tConductor3.setDefaultConductivity(0.0);
                CPPUNIT_ASSERT_NO_THROW(tNetwork.step(tDeltaTime));
// for debugging test failure:
//                try {
//                    tNetwork.step(tDeltaTime);
//                } catch (TsNumericalException& e) {
//                    std::cout << std::endl << "SOLVE throw: " << exp1 << ", " << exp2 << ", " << exp3;
//                }
            }
        }
    }

    /// - This is an example a user found that would fail decomposition in SOLVE mode.
    tConductor1.setDefaultConductivity( 14.0);
    tConductor2.setDefaultConductivity(128.0);
    tConductor3.setDefaultConductivity(  0.0);
    CPPUNIT_ASSERT_NO_THROW(tNetwork.step(tDeltaTime));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Gunns class checkStepInputs method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testCheckStepInputs()
{
    std::cout << "\n UtGunns ................ 31: testCheckStepInputs ...................";

    setupNominalNonLinearNetwork(true);

    tNetwork.mSolverMode       = Gunns::DUMMY;
    tNetwork.mIslandMode       = Gunns::SOLVE;
    tNetwork.mRunMode          = Gunns::PAUSE;
    tNetwork.mGpuMode          = Gunns::GPU_SPARSE;
    tNetwork.mGpuSizeThreshold = 1000;

    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT(Gunns::DUMMY  == tNetwork.mLastSolverMode);
    CPPUNIT_ASSERT(Gunns::SOLVE  == tNetwork.mLastIslandMode);
    CPPUNIT_ASSERT(Gunns::PAUSE  == tNetwork.mLastRunMode);
    CPPUNIT_ASSERT(Gunns::NO_GPU == tNetwork.mGpuMode);

    if (tNetwork.isGpuEnabled()) {
        tNetwork.mGpuMode          = Gunns::GPU_SPARSE;
        tNetwork.mGpuSizeThreshold = 0;
        tNetwork.step(tDeltaTime);
        CPPUNIT_ASSERT(2 == tNetwork.mGpuSizeThreshold);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Gunns class GPU_SPARSE mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testGpuSparse()
{
    std::cout << "\n UtGunns ................ 32: testGpuSparse .........................";

    if (tNetwork.isGpuEnabled()) {
        setupNominalNonLinearNetwork(true);
        CPPUNIT_ASSERT(0 != tNetwork.mSolverCpu);
        CPPUNIT_ASSERT(0 != tNetwork.mSolverGpuDense);
        CPPUNIT_ASSERT(0 != tNetwork.mSolverGpuSparse);

        tNetwork.mGpuMode          = Gunns::GPU_SPARSE;
        tNetwork.mIslandMode       = Gunns::OFF;
        tNetwork.mGpuSizeThreshold = 2;

        tNetwork.step(tDeltaTime);

        /// - Verify the potential vector solution.  These values are copied from testNonLinearStep
        ///   since this test is the same.  We set a slightly larger tolerance since the GPU
        ///   solution is not exactly identical to the CPU solution.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2499999999999994e+02,
                tNetwork.mPotentialVector[0], 100.0*DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.195580929517748e+02,
                tNetwork.mPotentialVector[1], 100.0*DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0993847627749395e+02,
                tNetwork.mPotentialVector[2], 100.0*DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.4969238138746974e+01,
                tNetwork.mPotentialVector[3], 100.0*DBL_EPSILON);

        /// - Verify the potential vector is saved for next step.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[0],
                tNetwork.mMinorPotentialVector[0], 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[1],
                tNetwork.mMinorPotentialVector[1], 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[2],
                tNetwork.mMinorPotentialVector[2], 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[3],
                tNetwork.mMinorPotentialVector[3], 0.0);

        /// - Verify minor step iteration & convergence metrics.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.0, tNetwork.mAvgMinorStepCount, DBL_EPSILON);
        CPPUNIT_ASSERT_EQUAL( 5, tNetwork.mMinorStepCount);
        CPPUNIT_ASSERT_EQUAL( 5, tNetwork.mDecompositionCount);
        CPPUNIT_ASSERT_EQUAL( 1, tNetwork.mMajorStepCount);
        CPPUNIT_ASSERT_EQUAL( 5, tNetwork.mMaxMinorStepCount);
        CPPUNIT_ASSERT_EQUAL( 0, tNetwork.mConvergenceFailCount);

        /// - Verify the second step.
        tNetwork.step(tDeltaTime);

        /// - Verify the potential vector solution.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2499999999999994e+02,
                tNetwork.mPotentialVector[0], 1000.0*DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1955782756819053e+02,
                tNetwork.mPotentialVector[1], 1000.0*DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0993823224661188e+02,
                tNetwork.mPotentialVector[2], 1000.0*DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.4969116123305930e+01,
                tNetwork.mPotentialVector[3], 1000.0*DBL_EPSILON);

        /// - Verify the potential vector is saved for next step.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[0],
                tNetwork.mMinorPotentialVector[0], 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[1],
                tNetwork.mMinorPotentialVector[1], 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[2],
                tNetwork.mMinorPotentialVector[2], 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[3],
                tNetwork.mMinorPotentialVector[3], 0.0);

        /// - Verify minor step iteration & convergence metrics.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 3, tNetwork.mAvgMinorStepCount, DBL_EPSILON);
        CPPUNIT_ASSERT_EQUAL( 6, tNetwork.mMinorStepCount);
        CPPUNIT_ASSERT_EQUAL( 6, tNetwork.mDecompositionCount);
        CPPUNIT_ASSERT_EQUAL( 2, tNetwork.mMajorStepCount);
        CPPUNIT_ASSERT_EQUAL( 5, tNetwork.mMaxMinorStepCount);
        CPPUNIT_ASSERT_EQUAL( 0, tNetwork.mConvergenceFailCount);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Gunns class GPU_DENSE mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testGpuDense()
{
    std::cout << "\n UtGunns ................ 33: testGpuDense ..........................";

    if (tNetwork.isGpuEnabled()) {
        setupNominalNonLinearNetwork(true);
        CPPUNIT_ASSERT(0 != tNetwork.mSolverCpu);
        CPPUNIT_ASSERT(0 != tNetwork.mSolverGpuDense);
        CPPUNIT_ASSERT(0 != tNetwork.mSolverGpuSparse);

        tNetwork.mGpuMode          = Gunns::GPU_DENSE;
        tNetwork.mIslandMode       = Gunns::OFF;
        tNetwork.mGpuSizeThreshold = 2;

        tNetwork.step(tDeltaTime);

        /// - Verify the potential vector solution.  These values are copied from testNonLinearStep
        ///   since this test is the same.  We set a slightly larger tolerance since the GPU
        ///   solution is not exactly identical to the CPU solution.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2499999999999994e+02,
                tNetwork.mPotentialVector[0], 100.0*DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.195580929517748e+02,
                tNetwork.mPotentialVector[1], 100.0*DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0993847627749395e+02,
                tNetwork.mPotentialVector[2], 100.0*DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.4969238138746974e+01,
                tNetwork.mPotentialVector[3], 100.0*DBL_EPSILON);

        /// - Verify the potential vector is saved for next step.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[0],
                tNetwork.mMinorPotentialVector[0], 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[1],
                tNetwork.mMinorPotentialVector[1], 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[2],
                tNetwork.mMinorPotentialVector[2], 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[3],
                tNetwork.mMinorPotentialVector[3], 0.0);

        /// - Verify minor step iteration & convergence metrics.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.0, tNetwork.mAvgMinorStepCount, DBL_EPSILON);
        CPPUNIT_ASSERT_EQUAL( 5, tNetwork.mMinorStepCount);
        CPPUNIT_ASSERT_EQUAL( 5, tNetwork.mDecompositionCount);
        CPPUNIT_ASSERT_EQUAL( 1, tNetwork.mMajorStepCount);
        CPPUNIT_ASSERT_EQUAL( 5, tNetwork.mMaxMinorStepCount);
        CPPUNIT_ASSERT_EQUAL( 0, tNetwork.mConvergenceFailCount);

        /// - Verify the second step.
        tNetwork.step(tDeltaTime);

        /// - Verify the potential vector solution.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2499999999999994e+02,
                tNetwork.mPotentialVector[0], 1000.0*DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1955782756819053e+02,
                tNetwork.mPotentialVector[1], 1000.0*DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0993823224661188e+02,
                tNetwork.mPotentialVector[2], 1000.0*DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.4969116123305930e+01,
                tNetwork.mPotentialVector[3], 1000.0*DBL_EPSILON);

        /// - Verify the potential vector is saved for next step.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[0],
                tNetwork.mMinorPotentialVector[0], 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[1],
                tNetwork.mMinorPotentialVector[1], 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[2],
                tNetwork.mMinorPotentialVector[2], 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( tNetwork.mPotentialVector[3],
                tNetwork.mMinorPotentialVector[3], 0.0);

        /// - Verify minor step iteration & convergence metrics.
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 3, tNetwork.mAvgMinorStepCount, DBL_EPSILON);
        CPPUNIT_ASSERT_EQUAL( 6, tNetwork.mMinorStepCount);
        CPPUNIT_ASSERT_EQUAL( 6, tNetwork.mDecompositionCount);
        CPPUNIT_ASSERT_EQUAL( 2, tNetwork.mMajorStepCount);
        CPPUNIT_ASSERT_EQUAL( 5, tNetwork.mMaxMinorStepCount);
        CPPUNIT_ASSERT_EQUAL( 0, tNetwork.mConvergenceFailCount);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Gunns class GPU_SPARSE mode with islands.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testGpuSparseIslands()
{
    std::cout << "\n UtGunns ................ 34: testGpuSparseIslands ..................";

    setupIslandNetwork();
    if (tNetwork.isGpuEnabled()) {
        CPPUNIT_ASSERT(0 != tNetwork.mSolverCpu);
        CPPUNIT_ASSERT(0 != tNetwork.mSolverGpuDense);
        CPPUNIT_ASSERT(0 != tNetwork.mSolverGpuSparse);
    } else {
        CPPUNIT_ASSERT(0 != tNetwork.mSolverCpu);
        CPPUNIT_ASSERT(0 == tNetwork.mSolverGpuDense);
        CPPUNIT_ASSERT(0 == tNetwork.mSolverGpuSparse);
    }

    tNetwork.mGpuMode          = Gunns::GPU_SPARSE;
    tNetwork.mIslandMode       = Gunns::SOLVE;
    tNetwork.mGpuSizeThreshold = 5;

    /// - Step the network and verify the correct island values.
    tPotential.mMalfBlockageValue = 1.5E-14;

    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT_EQUAL(2,   tNetwork.mIslandCount);
    CPPUNIT_ASSERT_EQUAL(5,   tNetwork.mIslandMaxSize);
    CPPUNIT_ASSERT_EQUAL(0.0, tNetwork.mAdmittanceMatrixIsland[5*5]);
    CPPUNIT_ASSERT(0.0 < tNetwork.mAdmittanceMatrixIsland[0]);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tNetwork.mPotentialVector[0],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tNetwork.mPotentialVector[1],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tNetwork.mPotentialVector[2],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tNetwork.mPotentialVector[3],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, tNetwork.mPotentialVector[4],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tNetwork.mPotentialVector[5],  1.0E-8);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Gunns class GPU_DENSE mode with islands.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::testGpuDenseIslands()
{
    std::cout << "\n UtGunns ................ 35: testGpuDenseIslands ...................";

    setupIslandNetwork();
    if (tNetwork.isGpuEnabled()) {
        CPPUNIT_ASSERT(0 != tNetwork.mSolverCpu);
        CPPUNIT_ASSERT(0 != tNetwork.mSolverGpuDense);
        CPPUNIT_ASSERT(0 != tNetwork.mSolverGpuSparse);
    } else {
        CPPUNIT_ASSERT(0 != tNetwork.mSolverCpu);
        CPPUNIT_ASSERT(0 == tNetwork.mSolverGpuDense);
        CPPUNIT_ASSERT(0 == tNetwork.mSolverGpuSparse);
    }

    tNetwork.mGpuMode          = Gunns::GPU_DENSE;
    tNetwork.mIslandMode       = Gunns::SOLVE;
    tNetwork.mGpuSizeThreshold = 5;

    /// - Step the network and verify the correct island values.
    tPotential.mMalfBlockageValue = 1.5E-14;

    tNetwork.step(tDeltaTime);

    CPPUNIT_ASSERT_EQUAL(2,   tNetwork.mIslandCount);
    CPPUNIT_ASSERT_EQUAL(5,   tNetwork.mIslandMaxSize);
    CPPUNIT_ASSERT_EQUAL(0.0, tNetwork.mAdmittanceMatrixIsland[5*5]);
    CPPUNIT_ASSERT(0.0 < tNetwork.mAdmittanceMatrixIsland[0]);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tNetwork.mPotentialVector[0],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tNetwork.mPotentialVector[1],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tNetwork.mPotentialVector[2],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tNetwork.mPotentialVector[3],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, tNetwork.mPotentialVector[4],  1.0E-8);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, tNetwork.mPotentialVector[5],  1.0E-8);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] converging (--) When true, network set up to be converging, otherwise non-converging.
///
/// @details  Sets up & initializes a nominal non-linear network for several test methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::setupNominalNonLinearNetwork(const bool converging)
{
    /// - Initialize the basic nodes.
    tBasicNodes[0].initialize("BasicNode1");
    tBasicNodes[1].initialize("BasicNode2");
    tBasicNodes[2].initialize("BasicNode3");
    tBasicNodes[3].initialize("BasicNode4");
    tBasicNodes[4].initialize("BasicNode5");
    tNodeList.mNumNodes = 5;
    tNodeList.mNodes    = tBasicNodes;
    tNetwork.initializeNodes(tNodeList);

    /// - Set up a simple basic network.
    tPotentialConfig    .mName                 = "VS1";
    tPotentialConfig    .mNodeList             = &tNodeList;
    tPotentialConfig    .mDefaultConductivity  = 1.0E14;

    tConductor1Config   .mName                 = "R1";
    tConductor1Config   .mNodeList             = &tNodeList;
    tConductor1Config   .mDefaultConductivity  = 1.0;
    tConductor2Config   .mName                 = "R2";
    tConductor2Config   .mNodeList             = &tNodeList;
    tConductor2Config   .mDefaultConductivity  = 1.0/1750.0;
    tConductor3Config   .mName                 = "R3";
    tConductor3Config   .mNodeList             = &tNodeList;
    tConductor3Config   .mDefaultConductivity  = 1.0/10000.0;
    tConductor4Config   .mName                 = "R4";
    tConductor4Config   .mNodeList             = &tNodeList;
    tConductor4Config   .mDefaultConductivity  = 1.0/10000.0;

    tConstantLoad1Config.mName                 = "CP1";
    tConstantLoad1Config.mNodeList             = &tNodeList;
    if (converging) {
        tConstantLoad1Config.mDefaultPower         = 300.0;
        tConstantLoad1Config.mDefaultConductivity  = 0.0192;
    } else {
        tConstantLoad1Config.mDefaultPower         = 3000.0;
        tConstantLoad1Config.mDefaultConductivity  = 0.192;
    }
    tConstantLoad1Config.mMinimumVoltageLimit  = 0.01;
    tConstantLoad2Config.mName                 = "CP2";
    tConstantLoad2Config.mNodeList             = &tNodeList;
    if (converging) {
        tConstantLoad2Config.mDefaultPower         = 350.0;
        tConstantLoad2Config.mDefaultConductivity  = 0.0224;
    } else {
        tConstantLoad2Config.mDefaultPower         = 3500.0;
        tConstantLoad2Config.mDefaultConductivity  = 0.224;
    }
    tConstantLoad2Config.mMinimumVoltageLimit  = 0.01;

    GunnsBasicPotentialInputData    tPotentialInput    (false, 0.0, -125.0);
    GunnsBasicConductorInputData    tConductor1Input   (false, 0.0);
    GunnsBasicConductorInputData    tConductor2Input   (false, 0.0);
    GunnsBasicConductorInputData    tConductor3Input   (false, 0.0);
    GunnsBasicConductorInputData    tConductor4Input   (false, 0.0);
    EpsConstantPowerLoadInputData   tConstantLoad1Input(false, 0.0);
    EpsConstantPowerLoadInputData   tConstantLoad2Input(false, 0.0);

    tPotential    .initialize(tPotentialConfig,     tPotentialInput,     tLinks, 0, 4);
    tConductor1   .initialize(tConductor1Config,    tConductor1Input,    tLinks, 0, 1);
    tConductor2   .initialize(tConductor2Config,    tConductor2Input,    tLinks, 1, 2);
    tConductor3   .initialize(tConductor3Config,    tConductor3Input,    tLinks, 2, 3);
    tConductor4   .initialize(tConductor4Config,    tConductor4Input,    tLinks, 3, 4);
    tConstantLoad1.initialize(tConstantLoad1Config, tConstantLoad1Input, tLinks, 1, 4);
    tConstantLoad2.initialize(tConstantLoad2Config, tConstantLoad2Input, tLinks, 1, 4);

    GunnsBasicLinkConfigData tFake1Config;
    GunnsBasicLinkInputData  tFake1Input;

    tFake1Config.mName     = "tLink1";
    tFake1Config.mNodeList = &tNodeList;

    int tFakeMap[2] = {4, 4};

    tFake1.initialize(tFake1Config, tFake1Input, tLinks, tFakeMap);

    tNetworkConfig.mMinorStepLimit       = 10;
    if (converging) {
        tNetworkConfig.mConvergenceTolerance = 0.01;
    } else {
        tNetworkConfig.mConvergenceTolerance = 0.00001;
    }

    tNetwork.initialize(tNetworkConfig, tLinks);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets up & initializes a basic network with islands in it for testing islands.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::setupIslandNetwork()
{
    /*  A              Node island #  count  island vector 0  island vector 1
     *  [x     2    ]  0              2      0, 1, 2, 3, 5    4
     *  [  x 1     4]  0
     *  [    x 3    ]  0
     *  [      x    ]  0
     *  [        x  ]  4
     *  [          x]  0
     */
    /// - Initialize the basic nodes.
    tBasicNodes[0].initialize("BasicNode0");
    tBasicNodes[1].initialize("BasicNode1");
    tBasicNodes[2].initialize("BasicNode2");
    tBasicNodes[3].initialize("BasicNode3");
    tBasicNodes[4].initialize("BasicNode4");
    tBasicNodes[5].initialize("BasicNode5");
    tBasicNodes[6].initialize("BasicNode6");
    tNodeList.mNumNodes = 7;
    tNodeList.mNodes    = tBasicNodes;
    tNetwork.initializeNodes(tNodeList);

    /// - Set up a simple basic network.
    tConductor1Config   .mName                 = "R1";
    tConductor1Config   .mNodeList             = &tNodeList;
    tConductor1Config   .mDefaultConductivity  = 1.0;
    tConductor2Config   .mName                 = "R2";
    tConductor2Config   .mNodeList             = &tNodeList;
    tConductor2Config   .mDefaultConductivity  = 1.0/1750.0;
    tConductor3Config   .mName                 = "R3";
    tConductor3Config   .mNodeList             = &tNodeList;
    tConductor3Config   .mDefaultConductivity  = 1.0/10000.0;
    tConductor4Config   .mName                 = "R4";
    tConductor4Config   .mNodeList             = &tNodeList;
    tConductor4Config   .mDefaultConductivity  = 1.0/10000.0;
    tPotentialConfig    .mName                 = "VS1";
    tPotentialConfig    .mNodeList             = &tNodeList;
    tPotentialConfig    .mDefaultConductivity  = 1.0E14;

    GunnsBasicConductorInputData    tConductor1Input   (false, 0.0);
    GunnsBasicConductorInputData    tConductor2Input   (false, 0.0);
    GunnsBasicConductorInputData    tConductor3Input   (false, 0.0);
    GunnsBasicConductorInputData    tConductor4Input   (false, 0.0);
    GunnsBasicPotentialInputData    tPotentialInput    (false, 0.0, 120.0);

    tConductor1   .initialize(tConductor1Config,    tConductor1Input,    tLinks, 1, 2);
    tConductor2   .initialize(tConductor2Config,    tConductor2Input,    tLinks, 0, 3);
    tConductor3   .initialize(tConductor3Config,    tConductor3Input,    tLinks, 2, 3);
    tConductor4   .initialize(tConductor4Config,    tConductor4Input,    tLinks, 1, 5);
    tPotential    .initialize(tPotentialConfig,     tPotentialInput,     tLinks, 6, 5);

    tNetworkConfig.mMinorStepLimit       = 10;
    tNetworkConfig.mConvergenceTolerance = 0.01;

    tNetwork.initialize(tNetworkConfig, tLinks);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets up & initializes a basic network with islands in it for testing islands.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunns::setupIslandNetwork2()
{
    /*  A        Node island #  count  island vector
     *  [x 1  ]  0              3      0, 1, 2
     *  [  x 2]  0
     *  [    x]  0
     */
    /// - Initialize the basic nodes.
    tBasicNodes[0].initialize("BasicNode0");
    tBasicNodes[1].initialize("BasicNode1");
    tBasicNodes[2].initialize("BasicNode2");
    tBasicNodes[3].initialize("BasicNode3");
    tNodeList.mNumNodes = 4;
    tNodeList.mNodes    = tBasicNodes;
    tNetwork.initializeNodes(tNodeList);

    /// - Set up a simple basic network.
    tConductor1Config.mName                = "R1";
    tConductor1Config.mNodeList            = &tNodeList;
    tConductor1Config.mDefaultConductivity = 0.0;
    tConductor2Config.mName                = "R2";
    tConductor2Config.mNodeList            = &tNodeList;
    tConductor2Config.mDefaultConductivity = 0.0;
    tConductor3Config.mName                = "R3";
    tConductor3Config.mNodeList            = &tNodeList;
    tConductor3Config.mDefaultConductivity = 0.0;

    GunnsBasicConductorInputData tConductor1Input(false, 0.0);
    GunnsBasicConductorInputData tConductor2Input(false, 0.0);
    GunnsBasicConductorInputData tConductor3Input(false, 0.0);

    tConductor1.initialize(tConductor1Config, tConductor1Input, tLinks, 0, 1);
    tConductor2.initialize(tConductor2Config, tConductor2Input, tLinks, 1, 2);
    tConductor3.initialize(tConductor3Config, tConductor3Input, tLinks, 0, 3);

    tNetworkConfig.mMinorStepLimit       = 10;
    tNetworkConfig.mConvergenceTolerance = 0.01;

    tNetwork.initialize(tNetworkConfig, tLinks);
}
