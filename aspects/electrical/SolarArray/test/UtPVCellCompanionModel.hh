#ifndef UTPVCELL_COMPANION_EXISTS
#define UTPVCELL_COMPANION_EXISTS

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "aspects/electrical/SolarArray/PVCellCompanionModel.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from PVCellCompanionModel and befriend FriendlyPVCellCompanionModel.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
///////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyPVCellCompanionModel : public PVCellCompanionModel {
    public:
        FriendlyPVCellCompanionModel();
        virtual ~FriendlyPVCellCompanionModel();

        friend class UtPVCellCompanionModel;
};

inline FriendlyPVCellCompanionModel::FriendlyPVCellCompanionModel() : PVCellCompanionModel() {}
inline FriendlyPVCellCompanionModel::~FriendlyPVCellCompanionModel() {}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Firmware unit tests.
///
/// @details  This class provides the unit tests for the witchingUnitFirmware with CPPUnit framework.
///////////////////////////////////////////////////////////////////////////////////////////////////
class UtPVCellCompanionModel : public CppUnit::TestFixture {

    CPPUNIT_TEST_SUITE(UtPVCellCompanionModel);
    //TEST INITIALIZATION============================================================
    CPPUNIT_TEST(testDefaultConstructor);
    CPPUNIT_TEST(testDefaultConfigData);
    CPPUNIT_TEST(testCopyConstructors);
    CPPUNIT_TEST(testCustomConfigData);
    CPPUNIT_TEST(testDefaultInputData);
    CPPUNIT_TEST(testCustomInputData);
    CPPUNIT_TEST(testNominalInitialization);
    //TEST BAD INITIALIZATION========================================================
    CPPUNIT_TEST(testInitializationWithLessThanZeroVocThrowsInitException);
    CPPUNIT_TEST(testInitializationWithLessThanZeroVmpThrowsInitException);
    CPPUNIT_TEST(testInitializationWithLessThanZeroIscThrowsInitException);
    CPPUNIT_TEST(testInitializationWithLessThanZeroImpThrowsInitException);
    CPPUNIT_TEST(testInitializationWithLessThanZeroIsatThrowsInitException);
    CPPUNIT_TEST(testInitializationWithLessThanZeroAgeThrowsInitException);
    CPPUNIT_TEST(testInitializationWithLessThanZeroRsThrowsInitException);
    CPPUNIT_TEST(testInitializationWithLessThanZeroRshThrowsInitException);
    CPPUNIT_TEST(testInitializationWithLessThanZeroVCritThrowsInitException);
    CPPUNIT_TEST(testInitializationWithGreaterThanOneBackSideReduxThrowsInitException);
    CPPUNIT_TEST(testInitializationWithLessThanZeroDegradationThrowsInitException);
    CPPUNIT_TEST(testInitializationWithGreaterThanOneDegradationThrowsInitException);
  //TEST UPDATE===================================================================
    CPPUNIT_TEST(testNominalUpdate);
    CPPUNIT_TEST(testUpdateHandlesBadSunAngleNicely);
    CPPUNIT_TEST(testUpdateHandlesBadSunIntensityNicely);
    CPPUNIT_TEST(testUpdateWithVPastVoc);
    CPPUNIT_TEST(testUpdateWithVLessThanZero);
    CPPUNIT_TEST(testDivideByZeroes);
    CPPUNIT_TEST(testIscReducedByPercentageWhenBackSideLit);
    //TEST DEGRADE==================================================================
    CPPUNIT_TEST(testNominalDegrade);
    CPPUNIT_TEST(testDegradeWithCellDegradationOutOfRangeGoesBackToZero);
    CPPUNIT_TEST(testDegradeWithMalfunctionEnabledOverridesCellDegradation);
    CPPUNIT_TEST(testDegradeWithMalfunctionOutOfRangeGoesBackToZero);
    //TEST_MALFS====================================================================
    CPPUNIT_TEST(testThatCellPowerMalfCausesZeroIeqAndGeq);
    CPPUNIT_TEST_SUITE_END();



public:
    /// @brief default constructor
    UtPVCellCompanionModel();

    /// @brief default destructor
    virtual ~UtPVCellCompanionModel();

    /// @brief run setup before each test
    void setUp();

    /// @brief run after each test to clean up
    void tearDown();



    //TEST INITIALIZATION============================================================
    void testDefaultConstructor();
    void testDefaultConfigData();
    void testCopyConstructors();
    void testCustomConfigData();
    void testDefaultInputData();
    void testCustomInputData();
    void testNominalInitialization();
    //TEST BAD INITIALIZATION========================================================
    void testInitializationWithLessThanZeroVocThrowsInitException();
    void testInitializationWithLessThanZeroVmpThrowsInitException();
    void testInitializationWithLessThanZeroIscThrowsInitException();
    void testInitializationWithLessThanZeroImpThrowsInitException();
    void testInitializationWithLessThanZeroIsatThrowsInitException();
    void testInitializationWithLessThanZeroAgeThrowsInitException();
    void testInitializationWithLessThanZeroRsThrowsInitException();
    void testInitializationWithLessThanZeroRshThrowsInitException();
    void testInitializationWithLessThanZeroVCritThrowsInitException();
    void testInitializationWithLessThanZeroDegradationThrowsInitException();
    void testInitializationWithGreaterThanOneDegradationThrowsInitException();
    void testInitializationWithGreaterThanOneBackSideReduxThrowsInitException();
    //TEST UPDATE===================================================================
    void testNominalUpdate();
    void testUpdateHandlesBadSunAngleNicely();
    void testUpdateHandlesBadSunIntensityNicely();
    void testUpdateWithVocPastVCrit();
    void testUpdateWithVPastVoc();
    void testUpdateWithVLessThanZero();
    void testDivideByZeroes();
    void testIscReducedByPercentageWhenBackSideLit();
    //TEST DEGRADE==================================================================
    void testNominalDegrade();
    void testDegradeWithCellDegradationOutOfRangeGoesBackToZero();
    void testDegradeWithMalfunctionEnabledOverridesCellDegradation();
    void testDegradeWithMalfunctionOutOfRangeGoesBackToZero();
    //TEST_MALFS====================================================================
    void testThatCellPowerMalfCausesZeroIeqAndGeq();

    /// @details test article
    FriendlyPVCellCompanionModel   *mTestObj;
    PVCellCompanionModelConfigData *mCellCD;
    PVCellCompanionModelInputData  *mCellID;

    PVCellCompanionModelConfigData *mCellBadVocCD;
    PVCellCompanionModelConfigData *mCellBadVmpCD;
    PVCellCompanionModelConfigData *mCellBadIscCD;
    PVCellCompanionModelConfigData *mCellBadImpCD;
    PVCellCompanionModelConfigData *mCellBadIsatCD;
    PVCellCompanionModelConfigData *mCellBadDegradationCD;
    PVCellCompanionModelConfigData *mCellBadRsCD;
    PVCellCompanionModelConfigData *mCellBadRshCD;
    PVCellCompanionModelConfigData *mCellBadVCritCD;
    PVCellCompanionModelConfigData *mCellBadBackSideCD;

    PVCellCompanionModelInputData  *mCellBadDegradationValueLowID;
    PVCellCompanionModelInputData  *mCellBadDegradationValueHighID;

    double mVocRef;
    double mVmpRef;
    double mVocTempCoefficient;
    double mIscRef;
    double mImpRef;
    double mIscTempCoefficient;
    double mIsat;
    double mTemperatureRef;
    double mCellDegradation;
    double mRs;
    double mRsh;
    double mVCrit;
    double mBackSideRedux;

    double mCellPowerMalfunctionIsEnabled;
    double mCellDegradationMalfunctionIsEnabled;
    double mCellDegradationOverrideValue;

    double mTolerance;
    double mThisV;
    double mThisI;
    double mThisSunAng;
    double mThisTemp;
    double mThisSunInt;
    bool   mThisBackSideIsLit;
    static const bool mIsMinor = false;

private:
    /// @details private copy constructor
    UtPVCellCompanionModel(const UtPVCellCompanionModel&);

    /// @details private assignment operator
    UtPVCellCompanionModel &operator =(const UtPVCellCompanionModel &n);
};
/// @}
#endif /* UTPVCELL_COMPANION_EXISTS */
