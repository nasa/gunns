/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
    (aspects/electrical/SolarArray/SolarSection.o)
    (software/exceptions/TsInitializationException.o)
    )
***************************************************************************************************/
#include "UtSolarSection.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtSolarSection class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSolarSection::UtSolarSection()
    :
    mTestObj(0),
    mSectionCD(0),
    mSectionID(0),
    mStringCD(0),
    mStringID(0),
    mRefCellCD(0),
    mRefCellID(0),
    mNumStrings(0),
    mTemperature(0.0),
    mSunAngleFromHorizontal(0.0),
    mSunIntensity(0.0),
    mIsShadowed(false),
    mIsMinor(false),
    mNumCells(0),
    mTolerance(0.0)
    {

    }

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtSolarSection class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSolarSection::~UtSolarSection() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::tearDown() {
    /// - Deletes for news in setUp
    delete mTestObj;
    delete mSectionID;
    delete mSectionCD;
    delete mStringCD;
    delete mStringID;
    delete mRefCellCD;
    delete mRefCellID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::setUp() {

    mNumStrings = 42;
    mTemperature = 300.0;
    mSunAngleFromHorizontal = 1.0;
    mSunIntensity = 1.0;
    mIsShadowed = false;
    mNumCells   = 200;
    mTolerance = 0.00001;

    mRefCellCD = new PVCellCompanionModelConfigData(0.6152, 0.4996,-0.00191, 2.6355, 2.445, 0.001424, 1.0e-10, 300.0, 0.0, 0.017, 200.0, 0.75, 0.30);
    mRefCellID = new PVCellCompanionModelInputData(false,false,0.0);

    mStringCD = new SolarStringConfigData(mNumCells, 0.07, 0.05, 8, *mRefCellCD);
    mStringID = new SolarStringInputData(false, 0, *mRefCellID);

    mSectionCD = new SolarSectionConfigData(mNumStrings, *mStringCD);
    mSectionID = new SolarSectionInputData(mIsShadowed, *mStringID);

    mTestObj  = new FriendlySolarSection();

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testDefaultConstructor(){
    std::cout << "\n =============== UtSolarSection ===============";
    std::cout << "\n UtSolarSection: testDefaultConstructor .................";

      /// - Use the default from setup
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(0),mTestObj->mNumStrings);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0,mTestObj->mTemperature,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.57,mTestObj->mSunAngleFromHorizontal,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,mTestObj->mSunIntensity,mTolerance);
    CPPUNIT_ASSERT_EQUAL(false,mTestObj->mZPlusLOSIsClear);
    CPPUNIT_ASSERT_EQUAL(true,mTestObj->mZMinusLOSIsClear);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(0),mTestObj->mNumFailedStrings);
    CPPUNIT_ASSERT_EQUAL(false,mTestObj->mIsShadowed);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mGeq,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mIeq,mTolerance);

     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test default config data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testDefaultConfigData(){
    std::cout << "\n UtSolarSection: testDefaultConfigData ..................";
    delete mSectionCD;
    mSectionCD = new SolarSectionConfigData();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(0),mSectionCD->mNumStrings);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test custom config data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testCustomConfigData(){
    std::cout << "\n UtSolarSection: testCustomConfigData ...................";

      /// - Use the default from setup
     CPPUNIT_ASSERT_EQUAL(mNumStrings,mSectionCD->mNumStrings);

     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test default input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testDefaultInputData(){
    std::cout << "\n UtSolarSection: testDefaultInputData ...................";
    delete mSectionID;
    mSectionID = new SolarSectionInputData();
    CPPUNIT_ASSERT_EQUAL(false,mSectionID->mIsShadowed);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test custom input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testCustomInputData(){
    std::cout << "\n UtSolarSection: testCustomInputData ....................";
    delete mSectionID;
    mSectionID = new SolarSectionInputData(true,*mStringID);
    CPPUNIT_ASSERT_EQUAL(true,mSectionID->mIsShadowed);
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test copy constructors
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testCopyConstructors(){
    std::cout << "\n UtSolarSection: testCopyConstructors....................";

    SolarSectionConfigData cd = SolarSectionConfigData(*mSectionCD);
    SolarSectionInputData id = SolarSectionInputData(*mSectionID);

      /// - Use the default from setup
    CPPUNIT_ASSERT_EQUAL(cd.mNumStrings,mSectionCD->mNumStrings);
    CPPUNIT_ASSERT_EQUAL(cd.mSolarStringConfigData.mNumCells,mSectionCD->mSolarStringConfigData.mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mSolarStringConfigData.mRefCellConfigData.mRsh,mSectionCD->mSolarStringConfigData.mRefCellConfigData.mRsh,0.000001);


    CPPUNIT_ASSERT_EQUAL(id.mIsShadowed,mSectionID->mIsShadowed);
    CPPUNIT_ASSERT_EQUAL(id.mSolarStringInputData.mIsFailed,mSectionID->mSolarStringInputData.mIsFailed);
    CPPUNIT_ASSERT_EQUAL(id.mSolarStringInputData.mRefCellInputData.mCellDegradationMalfunctionIsEnabled,mSectionID->mSolarStringInputData.mRefCellInputData.mCellDegradationMalfunctionIsEnabled);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test nominal initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testNominalInitialization(){
    std::cout << "\n UtSolarSection: testNominalInitialization ..............";
    //Initialize the test object with CD ID from setup method
    mTestObj->initialize(*mSectionCD,*mSectionID);
    CPPUNIT_ASSERT_EQUAL(mNumStrings,mTestObj->mNumStrings);
    CPPUNIT_ASSERT_EQUAL(mIsShadowed,mTestObj->mIsShadowed);
    for (int i = 0 ; i < mNumStrings; i ++){
        CPPUNIT_ASSERT_EQUAL(mNumCells,mTestObj->mStrings[i].getNumCells());
    }

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test nominal initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testInitializationWithLessThanOneString(){
    std::cout << "\n UtSolarSection: testLessThanOneInitialization...........";
    mSectionCD->mNumStrings = 0;
    //Initialize the test object with CD ID from setup method
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mSectionCD,*mSectionID),TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test nominal initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testConductanceIsZeroIfSectionShadowed(){
    std::cout << "\n UtSolarSection: testConductanceIsZeroIfSectionShadowed .";
    //Initialize the test object with CD ID from setup method
    mTestObj->initialize(*mSectionCD,*mSectionID);
    mTestObj->mZPlusLOSIsClear = false;
    mTestObj->mZMinusLOSIsClear = false;
    mTestObj->update(mIsMinor, 20.0, 100.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->getConductanceStamp(),mTolerance);

     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test source vector is zero when the section is shadowed
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testSourceVectorIsZeroIfSectionShadowed(){
    std::cout << "\n UtSolarSection: testSourceVectorIsZeroIfSectionShadowed ";
    //Initialize the test object with CD ID from setup method
    mTestObj->initialize(*mSectionCD,*mSectionID);
    mTestObj->mZPlusLOSIsClear = false;
    mTestObj->mZMinusLOSIsClear = false;
    mTestObj->update(mIsMinor, 20.0, 100.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->getSourceVectorStamp(),mTolerance);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the total number of failedcells is accurate during update
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testUpdateMethodCountsNumberOfFailedCells(){
    std::cout << "\n UtSolarSection: testUpdateMethodCountsNumberOfFailedCells .....";
    //Initialize the test object with CD ID from setup method
    mTestObj->initialize(*mSectionCD,*mSectionID);
    delete mSectionID;
    mSectionID = new SolarSectionInputData(mIsShadowed, SolarStringInputData(true, 0, *mRefCellID));
    mTestObj->update(mIsMinor, 20.0, 100.0);
    //All will be failed
    CPPUNIT_ASSERT_EQUAL(mNumStrings,mTestObj->getNumFailedStrings());

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test update sums up the geq for multiple sections
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testUpdateSumsUpGeqForSection(){
    std::cout << "\n UtSolarSection: testUpdateSumsUpGeqForSection ..........";

    //Initialize the test object with CD ID from setup method
    mTestObj->initialize(*mSectionCD,*mSectionID);
    mTestObj->setSunIntensity(1.0);
    mTestObj->mTemperature = 300;
    mTestObj->mSunAngleFromHorizontal = 1.0;
    mTestObj->update(mIsMinor, 20.0, 100.0);
    double expected = 7.1222254;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,mTestObj->getConductanceStamp(),mTolerance);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test nominal initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testUpdateSumsUpIeqForSection(){
    std::cout << "\n UtSolarSection: testUpdateSumsUpIeqForSection ..........";
    //Initialize the test object with CD ID from setup method
    mTestObj->initialize(*mSectionCD,*mSectionID);
    mTestObj->mSunAngleFromHorizontal = 1.0;
    mTestObj->mTemperature = 300;
    mTestObj->mSunIntensity = 1.0;
    mTestObj->update(mIsMinor, 20.0, 100.0);
    double expected = 879.9689478;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,mTestObj->getSourceVectorStamp(),mTolerance);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test shadowed section when both z are blocked
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testSectionShadowedIfBothZAreBlocked(){
    std::cout << "\n UtSolarSection: testShadowedSection ....................";
    //Initialize the test object with CD ID from setup method
    mTestObj->initialize(*mSectionCD,*mSectionID);
    mTestObj->mSunAngleFromHorizontal = 1.0; // 57.3 degrees
    mTestObj->mTemperature = 300;
    mTestObj->mZPlusLOSIsClear = false;
    mTestObj->mZMinusLOSIsClear = false;
    mTestObj->mSunIntensity = 1.0;
    mTestObj->update(mIsMinor, 20.0, 100.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->getSourceVectorStamp(),mTolerance);
    CPPUNIT_ASSERT_EQUAL(true,mTestObj->mIsShadowed);
    CPPUNIT_ASSERT_EQUAL(false,mTestObj->mIsBackLit);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test section is backlit when z+ is blocks but z- isnt
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarSection::testSectionBacklitIfZMinusBlockedButZPlusIsnt(){
    std::cout << "\n UtSolarSection: testBackLitConditions ..................";
    //Initialize the test object with CD ID from setup method
    mTestObj->initialize(*mSectionCD,*mSectionID);
    mTestObj->mSunAngleFromHorizontal = 1.0; // 57.3 degrees
    mTestObj->mTemperature = 300;
    mTestObj->mZPlusLOSIsClear = true;
    mTestObj->mZMinusLOSIsClear = false;
    mTestObj->mSunIntensity = 1.0;
    mTestObj->update(mIsMinor, 20.0, 100.0);
    mTestObj->update(mIsMinor, 20.0, 100.0);
    double expected = 604.827244;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,mTestObj->getSourceVectorStamp(),mTolerance);
    CPPUNIT_ASSERT_EQUAL(false,mTestObj->mIsShadowed);
    CPPUNIT_ASSERT_EQUAL(true,mTestObj->mIsBackLit);

     std::cout << "... Pass";
}



