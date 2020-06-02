/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
    (aspects/electrical/SolarArray/SolarString.o)
    (software/exceptions/TsInitializationException.o)
    )
***************************************************************************************************/
#include "UtSolarString.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtSolarString class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSolarString::UtSolarString()
    :
    mTestObj(0),
    mStringCD(0),
    mStringID(0),
    mStringBadBlockDiodeCD(0),
    mStringBadBipassDiodeCD(0),
    mCellCD(0),
    mCellID(0),
    mBipassDiodeVoltageDrop(0),
    mBipassDiodeInterval(0),
    mBlockingDiodeVoltageDrop(0),
    mIsFailed(false),
    mNumCells(0),
    mNumFailedCells(0),
    mVoltage(0),
    mCurrent(0),
    mTolerance(0),
    mIsMinorStep(0),
    mStringCurrent(0),
    mStringVoltage(0),
    mSunAngle(0),
    mTemperature(0),
    mSunIntensity(0),
    mThisBackSideIsLit(false)
    {

    }

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtSolarString class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSolarString::~UtSolarString() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::tearDown() {
    /// - Deletes for news in setUp
    delete mTestObj;
    delete mStringBadBlockDiodeCD;
    delete mStringBadBipassDiodeCD;
    delete mStringID;
    delete mStringCD;
    delete mCellCD;
    delete mCellID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::setUp() {
    mNumCells                 = 200;
    mNumFailedCells           = 0;
    mBlockingDiodeVoltageDrop = 0.07;
    mBipassDiodeVoltageDrop   = 0.05;
    mBipassDiodeInterval      = 8;
    mVoltage                  = 0.0;
    mCurrent                  = 0.0;
    mIsFailed                 = false;
    mTolerance                = 0.00001;

    mIsMinorStep = false;
    mStringCurrent = 20;
    mStringVoltage = 200;
    mSunAngle      = 0.9;
    mTemperature   = 300;
    mSunIntensity  = 1.0;
    mThisBackSideIsLit = false;

    mCellCD = new PVCellCompanionModelConfigData(0.6152, 0.4996, -0.00191, 2.6355, 2.445, 0.001424, 1.0e-10, 300.0, 0.0, 0.017, 200.0, 0.75, 0.30);
    mCellID = new PVCellCompanionModelInputData(false,false,0.0);

    mStringCD = new SolarStringConfigData(mNumCells, mBlockingDiodeVoltageDrop, mBipassDiodeVoltageDrop, mBipassDiodeInterval, *mCellCD);
    mStringID = new SolarStringInputData(mIsFailed, mNumFailedCells, *mCellID);

    mStringBadBlockDiodeCD = new SolarStringConfigData(mNumCells, -0.1, mBipassDiodeVoltageDrop, mBipassDiodeInterval, *mCellCD);
    mStringBadBipassDiodeCD = new SolarStringConfigData(mNumCells, mBlockingDiodeVoltageDrop, -0.1, mBipassDiodeInterval, *mCellCD);

    mTestObj  = new FriendlySolarString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testDefaultConstructor(){
    std::cout << "\n =============== UtSolarString ===============";
    std::cout << "\n UtSolarString: testDefaultConstructor .................................................";

      /// - Use the default from setup
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mBipassDiodeVoltageDrop,mTolerance);
     CPPUNIT_ASSERT_EQUAL(static_cast<int>(0),mTestObj->mBipassDiodeInterval);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mBlockingDiodeVoltageDrop,mTolerance);
     CPPUNIT_ASSERT_EQUAL(false,mTestObj->mMalfFailString);
     CPPUNIT_ASSERT_EQUAL(static_cast<int>(0),mTestObj->mNumCells);
     CPPUNIT_ASSERT_EQUAL(static_cast<int>(0),mTestObj->mNumFailedCells);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mVoltage,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mCurrent,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mGeq,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mIeq,mTolerance);

     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test default config data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testDefaultConfigData(){
    std::cout << "\n UtSolarString: testDefaultConfigData ..................................................";
    delete mStringCD;
    mStringCD = new SolarStringConfigData();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(0),mStringCD->mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mStringCD->mBlockingDiodeVoltageDrop,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mStringCD->mBipassDiodeVoltageDrop,mTolerance);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(0),mStringCD->mBipassDiodeInterval);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test custom config data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testCustomConfigData(){
    std::cout << "\n UtSolarString: testCustomConfigData ...................................................";

      /// - Use the default from setup
    CPPUNIT_ASSERT_EQUAL(mNumCells,mStringCD->mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mBlockingDiodeVoltageDrop,mStringCD->mBlockingDiodeVoltageDrop,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mBipassDiodeVoltageDrop,mStringCD->mBipassDiodeVoltageDrop,mTolerance);
    CPPUNIT_ASSERT_EQUAL(mBipassDiodeInterval,mStringCD->mBipassDiodeInterval);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test copy constructors
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testCopyConstructors(){
    std::cout << "\n UtSolarString: testCopyConstructors....................................................";

    SolarStringConfigData cd = SolarStringConfigData(*mStringCD);
    SolarStringInputData id = SolarStringInputData(*mStringID);

      /// - Use the default from setup
    CPPUNIT_ASSERT_EQUAL(cd.mNumCells,mStringCD->mNumCells);
    CPPUNIT_ASSERT_EQUAL(cd.mBipassDiodeInterval,mStringCD->mBipassDiodeInterval);
    CPPUNIT_ASSERT_EQUAL(cd.mBipassDiodeVoltageDrop,mStringCD->mBipassDiodeVoltageDrop);
    CPPUNIT_ASSERT_EQUAL(cd.mBlockingDiodeVoltageDrop,mStringCD->mBlockingDiodeVoltageDrop);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mRefCellConfigData.mRsh,mStringCD->mRefCellConfigData.mRsh,0.00001);

    CPPUNIT_ASSERT_EQUAL(id.mIsFailed,mStringID->mIsFailed);
    CPPUNIT_ASSERT_EQUAL(id.mNumFailedCells,mStringID->mNumFailedCells);
    CPPUNIT_ASSERT_EQUAL(id.mRefCellInputData.mCellPowerMalfunctionIsEnabled,mStringID->mRefCellInputData.mCellPowerMalfunctionIsEnabled);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test default input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testDefaultInputData(){
    std::cout << "\n UtSolarString: testDefaultInputData ...................................................";
    delete mStringID;
    mStringID = new SolarStringInputData();
    CPPUNIT_ASSERT_EQUAL(false,mStringID->mIsFailed);
    CPPUNIT_ASSERT_EQUAL(mNumFailedCells,mStringID->mNumFailedCells);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test custom input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testCustomInputData(){
    std::cout << "\n UtSolarString: testCustomInputData ....................................................";
    delete mStringID;
    mStringID = new SolarStringInputData(true, 2,mCellID);
    CPPUNIT_ASSERT_EQUAL(true,mStringID->mIsFailed);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(2),mStringID->mNumFailedCells);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test nominal initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testNominalInitialization(){
    std::cout << "\n UtSolarString: testNominalInitialization ..............................................";
    //Initialize the test object with CD ID from setup method
    mTestObj->initialize(*mStringCD,*mStringID);
    CPPUNIT_ASSERT_EQUAL(mTestObj->mNumCells,mStringCD->mNumCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTestObj->mBlockingDiodeVoltageDrop,mStringCD->mBlockingDiodeVoltageDrop,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTestObj->mBipassDiodeVoltageDrop,mStringCD->mBipassDiodeVoltageDrop,mTolerance);
    CPPUNIT_ASSERT_EQUAL(mTestObj->mBipassDiodeInterval,mStringCD->mBipassDiodeInterval);
    CPPUNIT_ASSERT_EQUAL(mTestObj->mMalfFailString,mStringID->mIsFailed);
    CPPUNIT_ASSERT_EQUAL(mTestObj->mNumFailedCells,mStringID->mNumFailedCells);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test nominal initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testInitializationWithLessThanOneNumCells(){
    std::cout << "\n UtSolarString: testLessThanOneInitialization ..........................................";
    //Initialize the test object with CD ID from setup method
    mStringCD->mNumCells = 0;

    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mStringCD,*mStringID),TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero cells
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testInitializationWithLessThanZeroBlockDiodeVDropThrowsInitException(){
    std::cout << "\n UtSolarString: testInitializationWithLessThanZeroBlockDiodeVDropThrowsInitException ...";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mStringBadBlockDiodeCD,*mStringID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero diode drop
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testInitializationWithLessThanZeroBipassDiodeVDropThrowsInitException(){
    std::cout << "\n UtSolarString: testInitializationWithLessThanZeroBipassDiodeVDropThrowsInitException...";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mStringBadBipassDiodeCD,*mStringID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that the total voltage is numCells*refCellVoltage - DiodeDrop
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testVoltageIsTotalVMinusDiodeLossDividedByActiveCellsTimesIsFailed(){
    std::cout << "\n UtSolarString: testVoltageIsTotalVMinusDiodeLossDividedByActiveCellsTimesIsFailed .....";
    //Give update method a reference voltage, and use num cells from setup.
    mTestObj->initialize(*mStringCD,*mStringID);
    mTestObj->update(mIsMinorStep,mStringCurrent,mStringVoltage,mSunAngle,mTemperature,mSunIntensity, mThisBackSideIsLit);

    // CellV =  (TotalVoltage - DiodeLoss) / TotalActiveCells * isFailed
    // double vRefCell = ( stringVoltage * static_cast<int> (!mIsFailed) - mBlockingDiodeVoltageDrop - mNumFailedCells * mBipassDiodeVoltageDrop / mBipassDiodeInterval) / (mNumCells - mNumFailedCells);
    double expected = 200.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,mTestObj->mVoltage,mTolerance);
     std::cout << "... Pass";

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that the voltage is zero when the string is failed
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testVoltageIsZeroWhenStringIsFailed(){
    std::cout << "\n UtSolarString: testVoltageIsZeroWhenStringIsFailed ....................................";
    //Give update method a reference voltage, and use num cells from setup.
    mTestObj->initialize(*mStringCD,*mStringID);
    //Test the malfunction is set by the setter method.
    mTestObj->setMalfFailString(true);
    CPPUNIT_ASSERT(true  == mTestObj->mMalfFailString);
    mTestObj->update(mIsMinorStep,mStringCurrent,mStringVoltage,mSunAngle,mTemperature,mSunIntensity, mThisBackSideIsLit);
    mTestObj->update(mIsMinorStep,mStringCurrent,mStringVoltage,mSunAngle,mTemperature,mSunIntensity, mThisBackSideIsLit);
    double expected = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,mTestObj->mIeq,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,mTestObj->mGeq,mTolerance);
    //Test the malfunction is reset by the setter method.
    mTestObj->setMalfFailString();
    CPPUNIT_ASSERT(false == mTestObj->mMalfFailString);
     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that the voltage is zero when the string is failed
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testCurrentIsZeroWhenStringIsFailed(){
    std::cout << "\n UtSolarString: testCurrentIsZeroWhenStringIsFailed ....................................";
    //Give update method a reference voltage, and use num cells from setup.
    mTestObj->initialize(*mStringCD,*mStringID);
    mTestObj->mMalfFailString = true;
    mTestObj->update(mIsMinorStep,mStringCurrent,mStringVoltage,mSunAngle,mTemperature,mSunIntensity, mThisBackSideIsLit);
    double expected = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,mTestObj->mIeq,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,mTestObj->mGeq,mTolerance);
     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that the voltage is zero when the string is failed
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testStringSourceVectorEqualsCellSourceVector(){
    std::cout << "\n UtSolarString: testStringSourceVectorEqualsCellSourceVector ...........................";
    //Give update method a reference voltage, and use num cells from setup.
    mTestObj->initialize(*mStringCD,*mStringID);
    mTestObj->update(mIsMinorStep,mStringCurrent,mStringVoltage,mSunAngle,mTemperature,mSunIntensity, mThisBackSideIsLit);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTestObj->mRefCell.getConductanceStamp() / mNumCells, mTestObj->getConductanceStamp(),mTolerance);
     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that the voltage is zero when the string is failed
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testStringConductanceEqualsCellConductanceDividedByNumActiveCells(){
    std::cout << "\n UtSolarString: testStringConductanceEqualsCellConductanceDividedByNumActiveCells ......";
    //Give update method a reference voltage, and use num cells from setup.
    mTestObj->initialize(*mStringCD,*mStringID);
    mTestObj->mNumFailedCells = 5;
    mTestObj->update(mIsMinorStep,mStringCurrent,mStringVoltage,mSunAngle,mTemperature,mSunIntensity, mThisBackSideIsLit);
    double expected = mTestObj->mRefCell.getConductanceStamp() / (mTestObj->mNumCells - mTestObj->mNumFailedCells);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mTestObj->getConductanceStamp(),mTolerance);
     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that the voltage is zero when the string is failed
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testStringConductanceEqualsZeroIfNumFailedCellsEqualsNumCells(){
    std::cout << "\n UtSolarString: testStringConductanceEqualsZeroIfNumFailedCellsEqualsNumCells ..........";
    //Give update method a reference voltage, and use num cells from setup.
    mTestObj->initialize(*mStringCD,*mStringID);
    mTestObj->mNumFailedCells = mTestObj->mNumCells;
    mTestObj->update(mIsMinorStep,mStringCurrent,mStringVoltage,mSunAngle,mTemperature,mSunIntensity, mThisBackSideIsLit);
    double expected = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mTestObj->getConductanceStamp(),mTolerance);
     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the voltage is zero if diode voltage drop would drop it below zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testVoltageIsZeroIfDiodeVoltageDropWouldBringItLessThanZero(){
    std::cout << "\n UtSolarString: testVoltageIsZeroIfDiodeVoltageDropWouldBringItLessThanZero ............";
    //Give update method a reference voltage, and use num cells from setup.
    mTestObj->initialize(*mStringCD,*mStringID);
    mTestObj->mBlockingDiodeVoltageDrop = 1000.0;
    mTestObj->update(mIsMinorStep,mStringCurrent,mStringVoltage,mSunAngle,mTemperature,mSunIntensity, mThisBackSideIsLit);
    mTestObj->update(mIsMinorStep,mStringCurrent,mStringVoltage,mSunAngle,mTemperature,mSunIntensity, mThisBackSideIsLit);
    mTestObj->update(mIsMinorStep,mStringCurrent,mStringVoltage,mSunAngle,mTemperature,mSunIntensity, mThisBackSideIsLit);
    double expected = 0.51742905;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,mTestObj->mRefCell.getV(),mTolerance);
     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the voltage is zero if diode voltage drop would drop it below zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolarString::testUpdateWithLessThanOneNumCells(){
    std::cout << "\n UtSolarString: testVoltageIsZeroIfDiodeVoltageDropWouldBringItLessThanZero ............";
    //Give update method a reference voltage, and use num cells from setup.
    mTestObj->initialize(*mStringCD,*mStringID);
    mTestObj->mNumCells = 0;
    mTestObj->update(mIsMinorStep,mStringCurrent,mStringVoltage,mSunAngle,mTemperature,mSunIntensity, mThisBackSideIsLit);
    double expected = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mTestObj->mGeq, mTolerance);
     std::cout << "... Pass";
}

