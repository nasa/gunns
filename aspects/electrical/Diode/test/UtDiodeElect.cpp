/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
    (aspects/electrical/Diode/DiodeElect.o)
    (core/GunnsBasicLink.o)
    (software/exceptions/TsInitializationException.o)
    )
***************************************************************************************************/
#include "UtDiodeElect.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtDiodeElect class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtDiodeElect::UtDiodeElect()
    :
mLinks(),
mName(""),
mNodes(),
mNodeList(),
mPort0(0),
mPort1(0),
mMinorTimeStep(1),
mConfigData(0),
mInputData(0),
mTestArticle(0),
mTol(0.00001),
mNominalReverseResistance(100000000.0),
mNominalForwardResistance(0.01),
mNominalMalfBlockageFlag(false),
mNominalMalfBlockageValue(0.0),
mCustomReverseResistance(200000000.0),
mCustomForwardResistance(0.02),
mCustomMalfBlockageFlag(false),
mCustomMalfBlockageValue(0.1),
mStubbTimeStep(0.0),
mStubbMinorTimeStep(0.0)
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtDiodeElect class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtDiodeElect::~UtDiodeElect()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::setUp()
{
    mName = "Known_Link";

    /// - Initialize the nodes list.
    mNodeList.mNodes      = mNodes;
    mNodeList.mNumNodes   = N_NODES;


    /// - Define the nominal configuration data.
    mConfigData           = new DiodeElectConfigData(mName,
                                                     &mNodeList,
                                                     mNominalReverseResistance,
                                                     mNominalForwardResistance);

    /// - Define the nominal input data.
    mInputData            = new DiodeElectInputData(mNominalMalfBlockageFlag,mNominalMalfBlockageValue);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;


    /// - Default construct the nominal test article.
    mTestArticle          = new FriendlyDiode;

}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::tearDown()
{
    /// - Deletes for news in setUp
   delete mTestArticle;
   delete mInputData;
   delete mConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of Diode link model configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::testConfigAndInput()
{
    /// @test    Configuration nominal construction.
    CPPUNIT_ASSERT(mName  == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNominalReverseResistance,
                                 mConfigData->mDiodeReverseResistance,
                                 mTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNominalForwardResistance,
                                 mConfigData->mDiodeForwardResistance,
                                 mTol);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mNominalMalfBlockageFlag  == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(mNominalMalfBlockageValue == mInputData->mMalfBlockageValue);

    /// @test    Configuration data default construction.
    DiodeElectConfigData defaultConfig;
    CPPUNIT_ASSERT("" == defaultConfig.mName);
    CPPUNIT_ASSERT(0  == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNominalReverseResistance,
                                 defaultConfig.mDiodeReverseResistance,
                                 mTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNominalForwardResistance,
                                 defaultConfig.mDiodeForwardResistance,
                                 mTol);

    /// @test    Input data default construction.
    DiodeElectInputData defaultInput;
    CPPUNIT_ASSERT(false   == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);

    /// @test    Configuration data copy construction.
    DiodeElectConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mName  == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNominalReverseResistance,
                                 copyConfig.mDiodeReverseResistance,
                                     mTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNominalForwardResistance,
                                 copyConfig.mDiodeForwardResistance,
                                 mTol);


    /// @test    Input data copy construction.
    DiodeElectInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mNominalMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(mNominalMalfBlockageValue == copyInput.mMalfBlockageValue);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsFluidLink class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::testDefaultConstruction()
{
    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT("" == mTestArticle->mName);
    CPPUNIT_ASSERT(0  == mTestArticle->mNodes);

    /// @test    Default construction state data.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mTestArticle->mConductance,mTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mTestArticle->mActiveConductance,mTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mTestArticle->mCurrent,mTol);
    CPPUNIT_ASSERT_EQUAL(false, mTestArticle->mDiodeReverseBias);
    CPPUNIT_ASSERT_EQUAL(true, mTestArticle->mDiodePreviousReverseBias);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mTestArticle->mDiodeReverseResistance,mTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mTestArticle->mDiodeForwardResistance,mTol);


    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!mTestArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    DiodeElect* testArticle = new DiodeElect();
    delete testArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Diode link model nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::testNominalInitialization()
{

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyDiode article;
    CPPUNIT_ASSERT_NO_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1));

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(mName == article.mName);
    CPPUNIT_ASSERT(mNodes == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNominalReverseResistance,
                                 article.mDiodeReverseResistance,
                                 mTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNominalForwardResistance,
                                 article.mDiodeForwardResistance,
                                 mTol);
    CPPUNIT_ASSERT_EQUAL(false, article.mDiodeReverseBias);
    CPPUNIT_ASSERT_EQUAL(true,  article.mDiodePreviousReverseBias);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(article.mInitFlag);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests diode uses reverse resistance when cathode vltg > than anode vltg
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::testDiodeShouldUseReverseResistanceWhenInReverseBias(){

    // Do this assignment for clarity
    int anode   = mPort0;
    int cathode = mPort1;

    //Cathode voltage greater than anode voltage
    double anodeVltg   = 12.0;
    double cathodeVltg = 25.0;

    // Initialize default constructed test article with nominal initialization data.
    FriendlyDiode article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    article.mAdmittanceUpdate = true;

    article.mPotentialVector[anode] =   anodeVltg;
    article.mPotentialVector[cathode] = cathodeVltg;

    //Must call the model similar to how it is called in gunns for nonlinear links.
    article.minorStep     (mStubbTimeStep, mMinorTimeStep);
    article.confirmSolutionAcceptable(mMinorTimeStep,mMinorTimeStep);
    article.minorStep     (mStubbTimeStep, mMinorTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mAdmittanceMatrix[0],mTol);
    article.minorStep(mStubbTimeStep, mMinorTimeStep);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests diode uses forward resistance when cathode vltg < than anode vltg
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::testDiodeShouldUseForwardResistanceWhenInForwardBias(){

    // Do this assignment for clarity
    int anode   = mPort0;
    int cathode = mPort1;

    //Cathode voltage less than anode voltage
    double anodeVltg   = 12.0;
    double cathodeVltg = 5.0;

    // Initialize default constructed test article with nominal initialization data.
    FriendlyDiode article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    article.mAdmittanceUpdate = true;

    article.mPotentialVector[anode] =   anodeVltg;
    article.mPotentialVector[cathode] = cathodeVltg;

    //Must call the model similar to how it is called in gunns for nonlinear links.
    article.minorStep     (mStubbTimeStep, mMinorTimeStep);
    article.confirmSolutionAcceptable(mMinorTimeStep,mMinorTimeStep);
    article.minorStep     (mStubbTimeStep, mMinorTimeStep);

    //With the voltages given, the model should be in reverse bias and using reverse resistance.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article.mDiodeForwardResistance,(1/article.mAdmittanceMatrix[0]),mTol);
    article.minorStep(mStubbTimeStep, mMinorTimeStep);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests case where voltage shows forward bias but model is in reverse bias.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::testDiscrepancyBetweenForwardBiasAndDeltaPotential(){

    // Do this assignment for clarity
    int anode   = mPort0;
    int cathode = mPort1;

    // Initialize default constructed test article with nominal initialization data.
    FriendlyDiode article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    article.mAdmittanceUpdate = true;

    //We setup of the model state to be reverse biased
    article.mDiodeReverseBias = true;

    //However, based on network solution, sometimes there can be a discrepancy.
    //Cathode voltage less than anode voltage so the model should be forward bias but we
    //know it is not based on above.
    double anodeVltg   = 12.0;
    double cathodeVltg = 5.0;

    article.mPotentialVector[anode] =   anodeVltg;
    article.mPotentialVector[cathode] = cathodeVltg;


    //Must call the model similar to how it is called in gunns for nonlinear links.
    article.minorStep     (mStubbTimeStep, mMinorTimeStep);
    //With the voltages given, the model should pick up the discrepancy, and correct itself.
    //It should return Acceptable = false, and then recalculate step using the correct reverse bias
    CPPUNIT_ASSERT(article.confirmSolutionAcceptable(mMinorTimeStep,mMinorTimeStep) == false);
    CPPUNIT_ASSERT(article.mDiodeReverseBias == false);
    //Run step again after making correction for discrepancy.
    article.minorStep     (mStubbTimeStep, mMinorTimeStep);
    //Results as expected
    CPPUNIT_ASSERT(article.confirmSolutionAcceptable(mMinorTimeStep,mMinorTimeStep) == true);
    CPPUNIT_ASSERT(article.mDiodeReverseBias == false);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests case where voltage shows reverse bias but model is in forward bias.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::testDiscrepancyBetweenReverseBiasAndDeltaPotential(){

    // Do this assignment for clarity
    int anode   = mPort0;
    int cathode = mPort1;

    // Initialize default constructed test article with nominal initialization data.
    FriendlyDiode article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    article.mAdmittanceUpdate = true;

    //We set the model state to be reverse biased.
    article.mDiodeReverseBias = false;

    //However, based on network solution, sometimes there can be a discrepancy.
    //Cathode voltage greater than anode voltage so the model should be reverse bias but we
    //know it is not based on above.
    double anodeVltg   = 5.0;
    double cathodeVltg = 32.0;

    article.mPotentialVector[anode] =   anodeVltg;
    article.mPotentialVector[cathode] = cathodeVltg;


    //Must call the model similar to how it is called in gunns for nonlinear links.
    article.minorStep     (mStubbTimeStep, mMinorTimeStep);
    //With the voltages given, the model should pick up the discrepancy, and correct itself.
    //It should return Acceptable = false, and then recalculate step using the correct reverse bias
    CPPUNIT_ASSERT(article.confirmSolutionAcceptable(mMinorTimeStep,mMinorTimeStep) == false);
    CPPUNIT_ASSERT(article.mDiodeReverseBias == true);
    //Run step again after making corrections for discrepancy
    article.minorStep     (mStubbTimeStep, mMinorTimeStep);
    //Results as expected
    CPPUNIT_ASSERT(article.confirmSolutionAcceptable(mMinorTimeStep,mMinorTimeStep) == true);
    CPPUNIT_ASSERT(article.mDiodeReverseBias == true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests initialization throws exceptions
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::testInitializeThrowsExceptionWhenGivenLessThanZeroForwardResistance()
{

    double badResistance = -20.0;
    DiodeElectConfigData* badConfigData = new DiodeElectConfigData(mName,
                                                                  &mNodeList,
                                                                  mNominalReverseResistance,
                                                                  badResistance);

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyDiode article;
    CPPUNIT_ASSERT_THROW(article.initialize(*badConfigData, *mInputData, mLinks, mPort0, mPort1),TsInitializationException);

    delete badConfigData;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests initialization throws exceptions
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::testInitializeThrowsExceptionWhenGivenLessThanZeroReverseResistance()
{

    double badResistance = -20.0;
    DiodeElectConfigData* badConfigData = new DiodeElectConfigData(mName,
                                                                  &mNodeList,
                                                                  badResistance,
                                                                  mNominalForwardResistance);

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyDiode article;
    CPPUNIT_ASSERT_THROW(article.initialize(*badConfigData, *mInputData, mLinks, mPort0, mPort1),TsInitializationException);

    delete badConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests initialization throws exceptions
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::testInitializeThrowsExceptionWhenRevResistanceLessThanForwResistance(){
    double badForwardResistance = 100.0;
    double badReverseResistance = 5.0;
    DiodeElectConfigData* badConfigData = new DiodeElectConfigData(mName,
                                                                  &mNodeList,
                                                                  badReverseResistance,
                                                                  badForwardResistance);

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyDiode article;
    CPPUNIT_ASSERT_THROW(article.initialize(*badConfigData, *mInputData, mLinks, mPort0, mPort1),TsInitializationException);

    delete badConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests initialization throws exceptions
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::testTheDiodeIsANonLinearLink(){
    //This test here mainly for code coverage
    CPPUNIT_ASSERT(true == mTestArticle->isNonLinear());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the compute flow method calculates correctly. potential[0]- potential[1]
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtDiodeElect::testTheComputeFlowMethodCalculatesCorrectly(){

    // Do this assignment for clarity
    int anode   = mPort0;
    int cathode = mPort1;

    // Initialize default constructed test article with nominal initialization data.
    FriendlyDiode article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    article.mAdmittanceUpdate = true;

    //We set the model state to be reverse biased.
    article.mDiodeReverseBias = false;

    //However, based on network solution, sometimes there can be a discrepancy.
    //Cathode voltage greater than anode voltage so the model should be reverse bias but we
    //know it is not based on above.
    double anodeVltg       = 15.0;
    article.mPotentialVector[anode] =   anodeVltg;
    double cathodeVltg     = 10.0;
    article.mPotentialVector[cathode] = cathodeVltg;
    double randomTestValue = 5.0;
    article.mAdmittanceMatrix[0] = randomTestValue;
    double expectedPotential = anodeVltg - cathodeVltg;
    double expectedCurrent   = expectedPotential * article.mAdmittanceMatrix[0];

    article.computeFlows(mStubbTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPotential,article.mPotentialDrop,mTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCurrent,article.mCurrent,mTol);
}
