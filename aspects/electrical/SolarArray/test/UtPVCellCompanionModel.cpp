/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
    (aspects/electrical/SolarArray/PVCellCompanionModel.o)
    (software/exceptions/TsInitializationException.o)
    )
***************************************************************************************************/
#include "UtPVCellCompanionModel.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <math.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtPVCellCompanionModel class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtPVCellCompanionModel::UtPVCellCompanionModel()
    :
    mTestObj(0),
    mCellCD(0),
    mCellID(0),
    mCellBadVocCD(0),
    mCellBadVmpCD(0),
    mCellBadIscCD(0),
    mCellBadImpCD(0),
    mCellBadIsatCD(0),
    mCellBadDegradationCD(0),
    mCellBadRsCD(0),
    mCellBadRshCD(0),
    mCellBadVCritCD(0),
    mCellBadBackSideCD(0),
    mCellBadDegradationValueLowID(0),
    mCellBadDegradationValueHighID(0),
    mVocRef(0),
    mVmpRef(0),
    mVocTempCoefficient(0),
    mIscRef(0),
    mImpRef(0),
    mIscTempCoefficient(0),
    mIsat(0),
    mTemperatureRef(0),
    mCellDegradation(0),
    mRs(0),
    mRsh(0),
    mVCrit(0),
    mBackSideRedux(0.0),
    mCellPowerMalfunctionIsEnabled(0),
    mCellDegradationMalfunctionIsEnabled(0),
    mCellDegradationOverrideValue(0),
    mTolerance(0.0),
    mThisV(0.0),
    mThisI(0.0),
    mThisSunAng(0.0),
    mThisTemp(0.0),
    mThisSunInt(0.0),
    mThisBackSideIsLit(0)



{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtPVCellCompanionModel class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtPVCellCompanionModel::~UtPVCellCompanionModel() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::tearDown() {
    /// - Deletes for news in setUp
    delete mTestObj;
    delete mCellID;
    delete mCellCD;
    delete mCellBadVocCD;
    delete mCellBadVmpCD;
    delete mCellBadIscCD;
    delete mCellBadImpCD;
    delete mCellBadIsatCD;
    delete mCellBadDegradationCD;
    delete mCellBadRsCD;
    delete mCellBadRshCD;
    delete mCellBadVCritCD;
    delete mCellBadBackSideCD;
    delete mCellBadDegradationValueLowID;
    delete mCellBadDegradationValueHighID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::setUp() {
    mVocRef = 0.6152;
    mVmpRef = 0.4996;
    mVocTempCoefficient = -0.00191;
    mIscRef = 2.6355;
    mImpRef = 2.445;
    mIscTempCoefficient = 0.001424;
    mIsat = 1.0e-10;
    mTemperatureRef = 300.0;
//    mCellArea = 16.0;
    mCellDegradation = 0.0;
//    mCurrentPerSquareCm = 0.164719;
    mRs = 0.017;
    mRsh = 200.0;
    mVCrit = 0.75;
    mBackSideRedux = 0.30;

    mCellPowerMalfunctionIsEnabled = false;
    mCellDegradationMalfunctionIsEnabled = false;
    mCellDegradationOverrideValue = 0.98;

    mTolerance   = 0.00001;
    mThisV       = 0.4;
    mThisI       = 1.0;
    mThisSunAng  = 0.80;
    mThisTemp    = 250.0;
    mThisSunInt  = 0.80;

    mThisBackSideIsLit = false;

    mCellCD = new PVCellCompanionModelConfigData(mVocRef,mVmpRef,mVocTempCoefficient,mIscRef,mImpRef,mIscTempCoefficient,mIsat,mTemperatureRef,mCellDegradation,mRs,mRsh,mVCrit,mBackSideRedux);
    mCellID = new PVCellCompanionModelInputData(mCellPowerMalfunctionIsEnabled,mCellDegradationMalfunctionIsEnabled,mCellDegradationOverrideValue);

    mCellBadVocCD         = new PVCellCompanionModelConfigData( -1    ,mVmpRef,mVocTempCoefficient,mIscRef,mImpRef,mIscTempCoefficient,mIsat,mTemperatureRef,mCellDegradation,mRs,mRsh, mVCrit, mBackSideRedux);
    mCellBadVmpCD         = new PVCellCompanionModelConfigData(mVocRef, -1    ,mVocTempCoefficient,mIscRef,mImpRef,mIscTempCoefficient,mIsat,mTemperatureRef,mCellDegradation,mRs,mRsh, mVCrit, mBackSideRedux);
    mCellBadIscCD         = new PVCellCompanionModelConfigData(mVocRef,mVmpRef,mVocTempCoefficient, -1    ,mImpRef,mIscTempCoefficient,mIsat,mTemperatureRef,mCellDegradation,mRs,mRsh, mVCrit, mBackSideRedux);
    mCellBadImpCD         = new PVCellCompanionModelConfigData(mVocRef,mVmpRef,mVocTempCoefficient,mIscRef, -1    ,mIscTempCoefficient,mIsat,mTemperatureRef,mCellDegradation,mRs,mRsh, mVCrit, mBackSideRedux);
    mCellBadIsatCD        = new PVCellCompanionModelConfigData(mVocRef,mVmpRef,mVocTempCoefficient,mIscRef,mImpRef,mIscTempCoefficient, -1  ,mTemperatureRef,mCellDegradation,mRs,mRsh, mVCrit, mBackSideRedux);
    mCellBadDegradationCD = new PVCellCompanionModelConfigData(mVocRef,mVmpRef,mVocTempCoefficient,mIscRef,mImpRef,mIscTempCoefficient,mIsat,mTemperatureRef, -1     ,mRs,mRsh, mVCrit, mBackSideRedux);
    mCellBadRsCD          = new PVCellCompanionModelConfigData(mVocRef,mVmpRef,mVocTempCoefficient,mIscRef,mImpRef,mIscTempCoefficient,mIsat,mTemperatureRef,mCellDegradation, -1,mRsh, mVCrit, mBackSideRedux);
    mCellBadRshCD         = new PVCellCompanionModelConfigData(mVocRef,mVmpRef,mVocTempCoefficient,mIscRef,mImpRef,mIscTempCoefficient,mIsat,mTemperatureRef,mCellDegradation,mRs, -1 , mVCrit, mBackSideRedux);
    mCellBadVCritCD       = new PVCellCompanionModelConfigData(mVocRef,mVmpRef,mVocTempCoefficient,mIscRef,mImpRef,mIscTempCoefficient,mIsat,mTemperatureRef,mCellDegradation,mRs,mRsh, -1, mBackSideRedux);
    mCellBadBackSideCD    = new PVCellCompanionModelConfigData(mVocRef,mVmpRef,mVocTempCoefficient,mIscRef,mImpRef,mIscTempCoefficient,mIsat,mTemperatureRef,mCellDegradation,mRs,mRsh, mVCrit, -1);

    mCellBadDegradationValueLowID  = new PVCellCompanionModelInputData(false,false,-1.0);
    mCellBadDegradationValueHighID = new PVCellCompanionModelInputData(false,false, 2.0);

    mTestObj  = new FriendlyPVCellCompanionModel();

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testDefaultConstructor(){
    std::cout << "\n =============== UtPVCellCompanionModel ===============";
    std::cout << "\n UtPVCellCompanionModel: testDefaultConstructor ..............................................";

      /// - Use the default from setup
     CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,mTestObj->mMaxDegradation,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mVocRef,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mVmpRef,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mVocTempCoefficient,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mIscRef,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mImpRef,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mIscTempCoefficient,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mIsat,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mTemperatureRef,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mCellDegradation,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mRs,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mRsh,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mVCrit,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mBackSideIscReduction,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mTemperature,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(1.57,mTestObj->mSunAngle,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,mTestObj->mSunIntensity,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mIsBackSideLit,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mV,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.615,mTestObj->mVlast,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mVmp,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mVoc,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mVd,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mI,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mIlast,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mImp,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mIsc,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mId,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mIl,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mIeqCell,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mLambda,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mCellPowerMalfunctionIsEnabled,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mCellPowerMalfunctionIsEnabled,mTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mTestObj->mCellPowerMalfunctionIsEnabled,mTolerance);

     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test copy constructors
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testCopyConstructors(){
    std::cout << "\n UtPVCellCompanionModel: testCopyConstructors.................................................";

    PVCellCompanionModelConfigData cd = PVCellCompanionModelConfigData(*mCellCD);
    PVCellCompanionModelInputData id = PVCellCompanionModelInputData(*mCellID);

      /// - Use the default from setup
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mVocRef,mCellCD->mVocRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mVmpRef,mCellCD->mVmpRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mVocTempCoefficient,mCellCD->mVocTempCoefficient,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mIscRef,mCellCD->mIscRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mImpRef,mCellCD->mImpRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mIscTempCoefficient,mCellCD->mIscTempCoefficient,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mIsat,mCellCD->mIsat,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mTemperatureRef,mCellCD->mTemperatureRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mCellDegradation,mCellCD->mCellDegradation,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mRs,mCellCD->mRs,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mRsh,mCellCD->mRsh,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mVCrit,mCellCD->mVCrit,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cd.mBackSideIscReduction,mCellCD->mBackSideIscReduction,mTolerance);
    CPPUNIT_ASSERT_EQUAL(id.mCellDegradationMalfunctionIsEnabled,mCellID->mCellDegradationMalfunctionIsEnabled);
    CPPUNIT_ASSERT_EQUAL(id.mCellPowerMalfunctionIsEnabled,mCellID->mCellPowerMalfunctionIsEnabled);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(id.mCellDegradationOverrideValue,mCellID->mCellDegradationOverrideValue,mTolerance);


     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test default config data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testDefaultConfigData(){
    std::cout << "\n UtPVCellCompanionModel: testDefaultConfigData ...............................................";
    delete mCellCD;
    mCellCD = new PVCellCompanionModelConfigData();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mVocRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mVmpRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mVocTempCoefficient,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mIscRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mImpRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mIscTempCoefficient,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mIsat,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mTemperatureRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mCellDegradation,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mRs,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mRsh,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mVCrit,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,mCellCD->mBackSideIscReduction,mTolerance);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test custom config data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testCustomConfigData(){
    std::cout << "\n UtPVCellCompanionModel: testCustomConfigData ................................................";

      /// - Use the default from setup
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mVocRef,mCellCD->mVocRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mVmpRef,mCellCD->mVmpRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mVocTempCoefficient,mCellCD->mVocTempCoefficient,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mIscRef,mCellCD->mIscRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mImpRef,mCellCD->mImpRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mIscTempCoefficient,mCellCD->mIscTempCoefficient,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mIsat,mCellCD->mIsat,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperatureRef,mCellCD->mTemperatureRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCellDegradation,mCellCD->mCellDegradation,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRs,mCellCD->mRs,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRsh,mCellCD->mRsh,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mVCrit,mCellCD->mVCrit,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mBackSideRedux,mCellCD->mBackSideIscReduction,mTolerance);

     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test default input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testDefaultInputData(){
    std::cout << "\n UtPVCellCompanionModel: testDefaultInputData ................................................";
    delete mCellID;
    mCellID = new PVCellCompanionModelInputData();
    CPPUNIT_ASSERT_EQUAL(false,mCellID->mCellDegradationMalfunctionIsEnabled);
    CPPUNIT_ASSERT_EQUAL(false,mCellID->mCellPowerMalfunctionIsEnabled);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,mCellID->mCellDegradationOverrideValue,mTolerance);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test custom input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testCustomInputData(){
    std::cout << "\n UtPVCellCompanionModel: testCustomInputData .................................................";
    delete mCellID;
    mCellID = new PVCellCompanionModelInputData(true,true,mCellDegradationOverrideValue);
    CPPUNIT_ASSERT_EQUAL(true,mCellID->mCellDegradationMalfunctionIsEnabled);
    CPPUNIT_ASSERT_EQUAL(true,mCellID->mCellPowerMalfunctionIsEnabled);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCellDegradationOverrideValue,mCellID->mCellDegradationOverrideValue,mTolerance);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test nominal initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testNominalInitialization(){
    std::cout << "\n UtPVCellCompanionModel: testNominalInitialization ...........................................";
    //Initialize the test object with CD ID from setup method
    mTestObj->initialize(*mCellCD,*mCellID);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mVocRef,mTestObj->mVocRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mVmpRef,mTestObj->mVmpRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mVocTempCoefficient,mTestObj->mVocTempCoefficient,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mIscRef,mTestObj->mIscRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mImpRef,mTestObj->mImpRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mIscTempCoefficient,mTestObj->mIscTempCoefficient,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mIsat,mTestObj->mIsat,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mTemperatureRef,mTestObj->mTemperatureRef,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCellDegradation,mTestObj->mCellDegradation,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRs,mTestObj->mRs,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mVCrit,mTestObj->mVCrit,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mBackSideRedux,mTestObj->mBackSideIscReduction,mTolerance);

    CPPUNIT_ASSERT_EQUAL(false,mTestObj->mCellDegradationMalfunctionIsEnabled);
    CPPUNIT_ASSERT_EQUAL(false,mTestObj->mCellPowerMalfunctionIsEnabled);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCellDegradationOverrideValue,mTestObj->mCellDegradationOverrideValue,mTolerance);

     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero value
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testInitializationWithLessThanZeroVocThrowsInitException(){
    std::cout << "\n UtPVCellCompanionModel: testInitializationWithLessThanZeroVocThrowsInitException ............";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mCellBadVocCD, *mCellID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero value
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testInitializationWithLessThanZeroVmpThrowsInitException(){
    std::cout << "\n UtPVCellCompanionModel: testInitializationWithLessThanZeroVmpThrowsInitException ............";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mCellBadVmpCD, *mCellID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero value
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testInitializationWithLessThanZeroIscThrowsInitException(){
    std::cout << "\n UtPVCellCompanionModel: testInitializationWithLessThanZeroIscThrowsInitException ............";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mCellBadIscCD, *mCellID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero value
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testInitializationWithLessThanZeroImpThrowsInitException(){
    std::cout << "\n UtPVCellCompanionModel: testInitializationWithLessThanZeroImpThrowsInitException ............";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mCellBadImpCD, *mCellID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero value
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testInitializationWithLessThanZeroIsatThrowsInitException(){
    std::cout << "\n UtPVCellCompanionModel: testInitializationWithLessThanZeroIsatThrowsInitException ...........";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mCellBadIsatCD, *mCellID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero value
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testInitializationWithLessThanZeroAgeThrowsInitException(){
    std::cout << "\n UtPVCellCompanionModel: testInitializationWithLessThanZeroAgeThrowsInitException ............";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mCellBadDegradationCD, *mCellID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero value
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testInitializationWithLessThanZeroRsThrowsInitException(){
    std::cout << "\n UtPVCellCompanionModel: testInitializationWithLessThanZeroRsThrowsInitException .............";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mCellBadRsCD, *mCellID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero value
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testInitializationWithLessThanZeroRshThrowsInitException(){
    std::cout << "\n UtPVCellCompanionModel: testInitializationWithLessThanZeroRshThrowsInitException ............";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mCellBadRshCD, *mCellID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero value
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testInitializationWithLessThanZeroVCritThrowsInitException(){
    std::cout << "\n UtPVCellCompanionModel: testInitializationWithLessThanZeroVCritThrowsInitException ..........";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mCellBadVCritCD, *mCellID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero value
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testInitializationWithLessThanZeroDegradationThrowsInitException(){
    std::cout << "\n UtPVCellCompanionModel: testInitializationWithLessThanZeroDegradationThrowsInitException ....";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mCellCD, *mCellBadDegradationValueLowID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero value
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testInitializationWithGreaterThanOneDegradationThrowsInitException(){
    std::cout << "\n UtPVCellCompanionModel: testInitializationWithGreaterThanOneDegradationThrowsInitException ..";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mCellCD, *mCellBadDegradationValueHighID),TsInitializationException);
     std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test bad initialization with less than zero isc redux
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testInitializationWithGreaterThanOneBackSideReduxThrowsInitException(){
    std::cout << "\n UtPVCellCompanionModel: testInitializationWithLessThanZeroIscRedux ..........................";
    //Initialize the test object with bad CD from setup
    CPPUNIT_ASSERT_THROW(mTestObj->initialize(*mCellBadBackSideCD, *mCellID),TsInitializationException);
     std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test degrade with nominal operating values
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testNominalDegrade(){
    std::cout << "\n UtPVCellCompanionModel: testNominalDegrade ..................................................";
    //Initialize the test object with CD ID from setup method
    mTestObj->initialize(*mCellCD,*mCellID);
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);
    double expected = 1.47161297911492;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 1.0
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mTestObj->mIsc,mTolerance);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test degrade with default cell degradation out of range 0 to 1
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testDegradeWithCellDegradationOutOfRangeGoesBackToZero(){
    std::cout << "\n UtPVCellCompanionModel: testDegradeWithCellDegradationOutOfRangeGoesBackToZero ..............";
    mTestObj->initialize(*mCellCD,*mCellID);
    mTestObj->mCellDegradation = 100.0; //This is out of range
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);
    double expected = 1.47161297911492;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 1.0
    //Results will be the same since we clip the degradation back to 0.0
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mTestObj->mIsc,mTolerance);
    std::cout << "... Pass";

}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test degrade with malfunction will take higher precedence than default degradation
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testDegradeWithMalfunctionEnabledOverridesCellDegradation(){
    std::cout << "\n UtPVCellCompanionModel: testDegradeWithMalfunctionEnabledOverridesCellDegradation ...........";
    mTestObj->initialize(*mCellCD,*mCellID);
    mTestObj->mCellDegradation = 0.7; //This is out of range
    mTestObj->mCellDegradationMalfunctionIsEnabled = true;
    mTestObj->mCellDegradationOverrideValue = 0.0;
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);
    //Results would normally be mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 0.3, bt since we overrid, it will be same.
    double expected = 1.47161297911492;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 1.0
    //Results will be the same since we clip the degradation back to 0.0
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mTestObj->mIsc,mTolerance);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test degrade with malfunction degradation value out of range 0 to 1
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testDegradeWithMalfunctionOutOfRangeGoesBackToZero(){
    std::cout << "\n UtPVCellCompanionModel: testDegradeWithMalfunctionOutOfRangeGoesBackToZero ..................";
    mTestObj->initialize(*mCellCD,*mCellID);
    //Test the malfunction is set by the setter method.
    mTestObj->setMalfCellDegrade(true, 100.0);
    CPPUNIT_ASSERT(true  == mTestObj->mCellDegradationMalfunctionIsEnabled);
    CPPUNIT_ASSERT(100.0 == mTestObj->mCellDegradationOverrideValue);
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);
    //Results would normally be mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 0.3, bt since we overrid, it will be same.
    double expected = 1.47161297911492;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 1.0
    //Results will be the same since we clip the override degradation back to 0.0
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mTestObj->mIsc,mTolerance);
    //Test the malfunction is reset by the setter method.
    mTestObj->setMalfCellDegrade();
    CPPUNIT_ASSERT(false == mTestObj->mCellDegradationMalfunctionIsEnabled);
    CPPUNIT_ASSERT(0.0   == mTestObj->mCellDegradationOverrideValue);
    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test degrade with malfunction degradation value out of range 0 to 1
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testNominalUpdate(){
    std::cout << "\n UtPVCellCompanionModel: testNominalUpdate ...................................................";
    mTestObj->initialize(*mCellCD,*mCellID);
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);

    double expectedGeq = 7.96275654e-05;
    double expectedIeq = 1.47164400790396;
    double expectedIl  = -3.185106163e-05;
    double expectedId  = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGeq, mTestObj->mGeqCell,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIeq, mTestObj->mIeqCell,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIl, mTestObj->mIl,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedId, mTestObj->mId,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4, mTestObj->mV,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mTestObj->mI,mTolerance);
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The update function should bound extremely high values of V and cast them out
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testUpdateWithVPastVoc(){
    std::cout << "\n UtPVCellCompanionModel: testUpdateWithVPastVoc ..............................................";
    mTestObj->initialize(*mCellCD,*mCellID);
    mThisV = 200.0;
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);

    double expectedGeq = 13.12008;
    double expectedIeq = 9.81421;
    double expectedIl  = -8.706387;
    double expectedId  = 0.363791;
    double expectedV   = 0.663592;
    double expectedI   = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGeq, mTestObj->mGeqCell,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIeq, mTestObj->mIeqCell,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIl, mTestObj->mIl,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedId, mTestObj->mId,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, mTestObj->mV,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, mTestObj->mI,mTolerance);
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The update function should bound V values less than zero and cast them out
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testUpdateWithVLessThanZero(){
    std::cout << "\n UtPVCellCompanionModel: testUpdateWithVLessThanZero .........................................";
    mTestObj->initialize(*mCellCD,*mCellID);
    mThisV = -200.0;
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);

    double expectedGeq = 9.6499751e-13;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 0.0
    double expectedIeq = 1.471612979114922;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 0.0
    double expectedIl  = 0.0;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 0.0
    double expectedId  = 0.0;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 0.0
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGeq, mTestObj->mGeqCell,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIeq, mTestObj->mIeqCell,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIl, mTestObj->mIl,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedId, mTestObj->mId,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mTestObj->mV,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIeq, mTestObj->mI,mTolerance);
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The update function should handle divide by zero values gracefully
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testDivideByZeroes(){
    std::cout << "\n UtPVCellCompanionModel: testDivideByZeroes ..................................................";
    mTestObj->initialize(*mCellCD,*mCellID);

    //Try to give horrible horrible values and make sure no exceptions are thrown.
    mThisI = -10.0;
    mTestObj->mRs = 0.0;
    mTestObj->mV = 0.0;
    mTestObj->mVlast = 0.0;
    mTestObj->mVCrit = -10.0;
    mTestObj->mVoc = -10.0;
    mTestObj->mTemperature = 0.0;
    mTestObj->mVocRef = -1000.0;
    mTestObj->update(mIsMinor, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    double expectedGeq = 0.0;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 0.0
    double expectedIeq = 0.0;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 0.0
    double expectedIl  = 0.0;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 0.0
    double expectedId  = 0.;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 0.0
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGeq, mTestObj->mGeqCell,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIeq, mTestObj->mIeqCell,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIl, mTestObj->mIl,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedId, mTestObj->mId,mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mTestObj->mI,mTolerance);
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test degrade with malfunction degradation value out of range 0 to 1
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testUpdateHandlesBadSunIntensityNicely(){
    std::cout << "\n UtPVCellCompanionModel: testUpdateHandlesBadSunIntensityNicely ..............................";
    mTestObj->initialize(*mCellCD,*mCellID);
    mThisSunInt = -20.0;
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);
    //Results would normally be mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 0.3, bt since we give less than zero
    // sun intensity, whole result will go to zero.
    double expected = 0.0;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(0.8) * 0.0
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mTestObj->mIsc,mTolerance);
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test degrade with malfunction degradation value out of range 0 to 1
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testUpdateHandlesBadSunAngleNicely(){
    std::cout << "\n UtPVCellCompanionModel: testUpdateHandlesBadSunIntensityNicely ..............................";
    //Verify behavior of fmod
    double result = -2;
    double cirlce = 360;
    result = fmod(result,360);
    if(result < 0.0){result += 360;}
    CPPUNIT_ASSERT_DOUBLES_EQUAL(358.0,result,mTolerance);

    result = 258;
    cirlce = 360;
    result = fmod(result,360);
    if(result < 0.0){result += 360;}
    CPPUNIT_ASSERT_DOUBLES_EQUAL(258.0,result,mTolerance);


    //There arent really any bad angles, we just expect it to work and be happy. However, since
    // the angle is out of the range (its not between 0 and pi), this means the vector is on the backside
    mTestObj->initialize(*mCellCD,*mCellID);
    mThisSunAng = -2000.0;
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);
    double expected = 1.90792019788986;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mTestObj->mIsc,mTolerance);
    mThisSunAng = 2000.0;
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);
    expected = 1.90792024093944;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mTestObj->mIsc,mTolerance);
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  During a power malf geq and ieq are zero
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testThatCellPowerMalfCausesZeroIeqAndGeq(){
    std::cout << "\n UtPVCellCompanionModel: testThatCellPowerMalfCausesZeroIeqAndGeq ............................";
    mTestObj->initialize(*mCellCD,*mCellID);
    //Test the malfunction is set by the setter method
    mTestObj->setMalfCellPower(true);
    CPPUNIT_ASSERT(true  == mTestObj->mCellPowerMalfunctionIsEnabled);
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);
    double expectedIeq = 0.0;
    double expectedGeq = 0.0;
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGeq, mTestObj->getConductanceStamp(),mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIeq, mTestObj->getSourceVectorStamp(),mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.3738, mTestObj->getImp(),mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5951, mTestObj->getVmp(),mTolerance);
    //Test the malfunction is reset by the setter method
    mTestObj->setMalfCellPower();
    CPPUNIT_ASSERT(false == mTestObj->mCellPowerMalfunctionIsEnabled);
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test Isc is reduced by percentage when backside lit
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtPVCellCompanionModel::testIscReducedByPercentageWhenBackSideLit(){
    std::cout << "\n UtPVCellCompanionModel: testUpdateHandlesBadSunIntensityNicely ..............................";
    mTestObj->initialize(*mCellCD,*mCellID);
    mThisSunAng = 1.0;
    mThisBackSideIsLit = false;
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);
    double expected = 1.72622723707431;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(-2000.0) * 1.0
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mTestObj->mIsc,mTolerance);
    mThisBackSideIsLit = true;
    mTestObj->update(mIsMinor,mThisV,mThisI,mThisSunAng,mThisTemp,mThisSunInt,mThisBackSideIsLit);
    expected = expected * 0.30;   //mIsc = (2.6355 + .001424 * -0.5)*0.80 * cos(2000.0) * 1.0 * .30
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mTestObj->mIsc,mTolerance);
    std::cout << "... Pass";
}
