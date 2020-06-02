/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
    (aspects/electrical/SolarArray/SolarArray.o)
    (core/GunnsBasicLink.o)
    (software/exceptions/TsConfigurationException.o)
    )
***************************************************************************************************/
#include "UtSolarArray.hh"
#include "software/exceptions/TsConfigurationException.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtSolarArray class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSolarArray::UtSolarArray()
    :
mLinks(),
mName(""),
mNodes(),
mNodeList(),
mPort0(0),
mPort1(0),
mMinorTimeStep(1),
mTestObj(0),
mTestCD(0),
mTestID(0),
mSectionCD(0),
mSectionID(0),
mRefCellCD(0),
mRefCellID(0),
mStringCD(0),
mStringID(0),
mNumSections(0),
mTolerance(0.000001),
mNominalMalfBlockageFlag(false),
mNominalMalfBlockageValue(0.0),
mStubbTimeStep(0.0)
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtSolarArray class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSolarArray::~UtSolarArray()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::setUp()
{
    mName = "Solar Array";
    mNumSections = 8;


    /// - Initialize the nodes list.
    mNodeList.mNodes      = mNodes;
    mNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    mTestCD           = new SolarArrayConfigData(mName, &mNodeList, 8, 10,200, 0.07, 0.05, 8,0.6152, 0.4996,-0.00191, 2.6355, 2.445, 0.001424, 1.0e-10, 300.0, 0.0, 0.017, 200.0, 0.75,0.30 );
    /// - Define the nominal input data.
    mTestID           = new SolarArrayInputData(mNominalMalfBlockageFlag,mNominalMalfBlockageValue,false, false, 0,false,false,0.0);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;


    /// - Default construct the nominal test article.
    mTestObj  = new FriendlySolarArray();

}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::tearDown()
{
    /// - Deletes for news in setUp
    delete mTestObj;
    delete mTestID;
    delete mTestCD;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testDefaultConstructor(){
    std::cout << "\n =============== UtSolarArray ===============";
    std::cout << "\n UtSolarArray: testDefaultConstructor .......";
    delete mTestObj;
    mTestObj = new FriendlySolarArray();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(0),mTestObj->mNumSections);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(45.0,mTestObj->mArrayCurrent,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(180.0,mTestObj->mArrayVoltage,mTolerance);
    CPPUNIT_ASSERT_EQUAL(false,mTestObj->mIsMinorStep);
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test default config data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testDefaultConfigData(){
    std::cout << "\n UtSolarArray: testDefaultConfigData ........";
    delete mTestCD;
    mTestCD = new SolarArrayConfigData();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(0),mTestCD->mNumSections);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test custom config data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testCustomConfigData(){
    std::cout << "\n UtSolarArray: testCustomConfigData .........";

      /// - Use the default from setup
    CPPUNIT_ASSERT_EQUAL(mNumSections,mTestCD->mNumSections);

     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test default input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testDefaultInputData(){
    std::cout << "\n UtSolarArray: testDefaultInputData .........";
    delete mTestID;
    mTestID = new SolarArrayInputData();
    CPPUNIT_ASSERT_EQUAL(false,mTestID->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestID->mMalfBlockageValue,mTolerance);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test custom input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testCustomInputData(){
    std::cout << "\n UtSolarArray: testCustomInputData ..........";
    delete mTestID;
    mTestID = new SolarArrayInputData(true,5.0,false, false, 0,false,false,0.0);
    CPPUNIT_ASSERT_EQUAL(true,mTestID->mMalfBlockageFlag);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0,mTestID->mMalfBlockageValue,mTolerance);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test nominal initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testNominalInitialization(){
    std::cout << "\n UtSolarArray: testNominalInitialization ....";
    /// - Initialize default constructed test article with nominal initialization data.
    mTestObj->initialize(*mTestCD,*mTestID,mLinks,mPort0,mPort1);

    /// @test    Nominal state data.
     CPPUNIT_ASSERT_EQUAL(mNumSections , mTestObj->mNumSections);

    for (int i = 0; i < mNumSections; i ++){
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(0),mTestObj->mSections[i].getNumFailedStrings());
    }

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(mTestObj->mInitFlag);
     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test nominal initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testInitializeWithLessThanOneSectionThrowsException(){
    std::cout << "\n UtSolarArray: testBadInitialize ............";
    mTestCD->mNumSections = 0;
    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mTestCD,*mTestID,mLinks,mPort0,mPort1),TsInitializationException);
    std::cout << "... Pass";

}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test minor step
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testMinorStep(){
    std::cout << "\n UtSolarArray: testMinorStep ................";
    /// - Initialize default constructed test article with nominal initialization data.
    mTestObj->initialize(*mTestCD,*mTestID,mLinks,mPort0,mPort1);
    mTestObj->minorStep(0.0,false);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,mTestObj->mAdmittanceMatrix[0],mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-210.84,mTestObj->mSourceVector[0],mTolerance);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test copy constructors
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testCopyConstructors(){
    std::cout << "\n UtSolarArray: testCopyConstructors..........";

    SolarArrayConfigData cd = SolarArrayConfigData(*mTestCD);
    SolarArrayInputData id = SolarArrayInputData(*mTestID);

      /// - Use the default from setup
    CPPUNIT_ASSERT_EQUAL(cd.mNumSections,mTestCD->mNumSections);
    CPPUNIT_ASSERT_EQUAL(cd.mSectionConfigData.mNumStrings,mTestCD->mSectionConfigData.mNumStrings);
    CPPUNIT_ASSERT_EQUAL(cd.mSectionConfigData.mSolarStringConfigData.mNumCells,mTestCD->mSectionConfigData.mSolarStringConfigData.mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mSectionConfigData.mSolarStringConfigData.mRefCellConfigData.mRsh,mTestCD->mSectionConfigData.mSolarStringConfigData.mRefCellConfigData.mRsh,0.000001);

    CPPUNIT_ASSERT_EQUAL(id.mMalfBlockageFlag,mTestID->mMalfBlockageFlag);
    CPPUNIT_ASSERT_EQUAL(id.mSectionInputData.mIsShadowed,mTestID->mSectionInputData.mIsShadowed);
    CPPUNIT_ASSERT_EQUAL(id.mSectionInputData.mSolarStringInputData.mIsFailed,mTestID->mSectionInputData.mSolarStringInputData.mIsFailed);
    CPPUNIT_ASSERT_EQUAL(id.mSectionInputData.mSolarStringInputData.mRefCellInputData.mCellDegradationMalfunctionIsEnabled,mTestID->mSectionInputData.mSolarStringInputData.mRefCellInputData.mCellDegradationMalfunctionIsEnabled);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test compute flows
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testComputeFlows(){
    std::cout << "\n UtSolarArray: testComputeFlows..............";
    /// - Initialize default constructed test article with nominal initialization data.
    mTestObj->initialize(*mTestCD,*mTestID,mLinks,mPort0,mPort1);
    mTestObj->mPotentialVector[1] = 100;
    mTestObj->mPotentialVector[0] = 50;
    mTestObj->mSourceVector[0] = -480;
    mTestObj->mSourceVector[1] = 480;
    mTestObj->mAdmittanceMatrix[0] = 10;
    mTestObj->step(0.0);
    mTestObj->computeFlows(0.0);
    mTestObj->step(0.0);
    mTestObj->computeFlows(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(50.0,mTestObj->mArrayVoltage,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(209.6398387,mTestObj->mArrayCurrent,mTolerance);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test compute flows with Negative current sets i to zero and v to max
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testComputeFlowsWithNegativeCurrent(){
    std::cout << "\n UtSolarArray: testComputeFlows..............";
    /// - Initialize default constructed test article with nominal initialization data.
    mTestObj->initialize(*mTestCD,*mTestID,mLinks,mPort0,mPort1);
    mTestObj->step(0);
    mTestObj->mPotentialVector[1] = 100;
    mTestObj->mPotentialVector[0] = 50;
    mTestObj->mSourceVector[0] = -1000;
    mTestObj->mSourceVector[1] = 1000;
    mTestObj->mAdmittanceMatrix[0] = 10;
    mTestObj->computeFlows(0.0);
    mTestObj->step(0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(50.0,mTestObj->mArrayVoltage,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(209.6398387,mTestObj->mArrayCurrent,mTolerance);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test compute flows with huge current sets i to max and v to zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testComputeFlowsWithTooBigCurrent(){
    std::cout << "\n UtSolarArray: testComputeFlows..............";
    /// - Initialize default constructed test article with nominal initialization data.
    mTestObj->initialize(*mTestCD,*mTestID,mLinks,mPort0,mPort1);
    mTestObj->step(0);
    mTestObj->mPotentialVector[1] = 100;
    mTestObj->mPotentialVector[0] = 50;
    mTestObj->mSourceVector[0] = -480;
    mTestObj->mSourceVector[1] = 10;
    mTestObj->mAdmittanceMatrix[0] = 10;
    mTestObj->computeFlows(0.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(50.0,mTestObj->mArrayVoltage,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-20.0,mTestObj->mArrayCurrent,mTolerance);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test compute flows with huge voltage sets v to max and I to zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testComputeFlowsWithTooBigVoltage(){
    std::cout << "\n UtSolarArray: testComputeFlows..............";
    // this functionality was commented out of the array model.
//    /// - Initialize default constructed test article with nominal initialization data.
//    mTestObj->initialize(*mTestCD,*mTestID,mLinks,mPort0,mPort1);
//    mTestObj->mArrayVoltage = 100;
//    mTestObj->step(0);
//    mTestObj->mPotentialVector[1] = 1000;
//    mTestObj->mPotentialVector[0] = 50;
//    mTestObj->mSourceVector[0] = -480;
//    mTestObj->mSourceVector[1] = 480;
//    mTestObj->mAdmittanceMatrix[0] = 10;
//    mTestObj->computeFlows(0.0);
//    mTestObj->step(0);
//    mTestObj->computeFlows(0.0);
//
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(123.04,mTestObj->mArrayVoltage,mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mArrayCurrent,mTolerance);

     std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test compute flows with Negative voltage should set v to zero and I to max.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testComputeFlowsWithTooSmallVoltage(){
    std::cout << "\n UtSolarArray: testComputeFlows..............";
    // this functionality was commented out of the array model.
//    /// - Initialize default constructed test article with nominal initialization data.
//    mTestObj->initialize(*mTestCD,*mTestID,mLinks,mPort0,mPort1);
//    mTestObj->step(0);
//    mTestObj->mPotentialVector[1] = 100;
//    mTestObj->mPotentialVector[0] = 5000;
//    mTestObj->mSourceVector[0] = -480;
//    mTestObj->mSourceVector[1] = 480;
//    mTestObj->mAdmittanceMatrix[0] = 10;
//    mTestObj->computeFlows(0.0);
//
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mArrayVoltage,mTolerance);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(210.84,mTestObj->mArrayCurrent,mTolerance);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for the step method in non-LEO mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarArray::testStepNonLeo(){
    std::cout << "\n UtSolarArray: testStepNonLeo................";

    mTestObj->mIsInLeo     = false;
    mTestObj->mSectionArea = 10.0;
    mTestCD->mCellArea     = 1.0;   // doesn't matter for this test, just need > 0.
    mTestObj->initialize(*mTestCD,*mTestID,mLinks,mPort0,mPort1);

    /// - Set up non-LEO environment interfaces to sections.  This should result in 5% insolation.
    for (int i=0; i<mTestObj->mNumSections; ++i) {
        mTestObj->mSections[i].mSolarPowerOnSection    = 1361.0 * mTestObj->mSectionArea;
        mTestObj->mSections[i].mSunAngleFromHorizontal = 0.5235987756; // pi/6 rad, 30 deg.
        mTestObj->mSections[i].mSunIntensity           = 0.1;
    }

    /// - Test nominal percent insolation.
    mTestObj->step(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mTestObj->mPercentInsolation, FLT_EPSILON);

    /// - Test percent insolation when reference value is zero.
    mTestObj->mReferenceInsolation = 0.0;
    mTestObj->step(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mTestObj->mPercentInsolation, FLT_EPSILON);

    std::cout << "... Pass";
}
