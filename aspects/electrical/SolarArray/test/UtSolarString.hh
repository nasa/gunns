#ifndef UTSOLAR_STRING_EXISTS
#define UTSOLAR_STRING_EXISTS

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "aspects/electrical/SolarArray/SolarString.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from SolarString and befriend FriendlySolarString.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
///////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlySolarString : public SolarString {
    public:
        FriendlySolarString();
        virtual ~FriendlySolarString();

        friend class UtSolarString;
};

inline FriendlySolarString::FriendlySolarString() : SolarString() {}
inline FriendlySolarString::~FriendlySolarString() {}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Firmware unit tests.
///
/// @details  This class provides the unit tests for the witchingUnitFirmware with CPPUnit framework.
///////////////////////////////////////////////////////////////////////////////////////////////////
class UtSolarString : public CppUnit::TestFixture {

    CPPUNIT_TEST_SUITE(UtSolarString);
    //TEST INITIALIZE
    CPPUNIT_TEST(testDefaultConstructor);
    CPPUNIT_TEST(testDefaultConfigData);
    CPPUNIT_TEST(testCopyConstructors);
    CPPUNIT_TEST(testCustomConfigData);
    CPPUNIT_TEST(testDefaultInputData);
    CPPUNIT_TEST(testCustomInputData);
    CPPUNIT_TEST(testNominalInitialization);
    CPPUNIT_TEST(testInitializationWithLessThanOneNumCells);
    CPPUNIT_TEST(testInitializationWithLessThanZeroBlockDiodeVDropThrowsInitException);
    CPPUNIT_TEST(testInitializationWithLessThanZeroBipassDiodeVDropThrowsInitException);
    //TEST UPDATE
    CPPUNIT_TEST(testVoltageIsTotalVMinusDiodeLossDividedByActiveCellsTimesIsFailed);
    CPPUNIT_TEST(testVoltageIsZeroWhenStringIsFailed);
    CPPUNIT_TEST(testCurrentIsZeroWhenStringIsFailed);
    CPPUNIT_TEST(testStringSourceVectorEqualsCellSourceVector);
    CPPUNIT_TEST(testStringConductanceEqualsCellConductanceDividedByNumActiveCells);
    CPPUNIT_TEST(testStringConductanceEqualsZeroIfNumFailedCellsEqualsNumCells);
    CPPUNIT_TEST(testVoltageIsZeroIfDiodeVoltageDropWouldBringItLessThanZero);
    CPPUNIT_TEST(testUpdateWithLessThanOneNumCells);
    CPPUNIT_TEST_SUITE_END();

public:
    /// @brief default constructor
    UtSolarString();

    /// @brief default destructor
    virtual ~UtSolarString();

    /// @brief run setup before each test
    void setUp();

    /// @brief run after each test to clean up
    void tearDown();



    //TEST INITIALIZATION===============================================
    void testDefaultConstructor();
    void testDefaultConfigData();
    void testCopyConstructors();
    void testCustomConfigData();
    void testDefaultInputData();
    void testCustomInputData();
    void testNominalInitialization();
    void testInitializationWithLessThanOneNumCells();
    void testInitializationWithLessThanZeroBlockDiodeVDropThrowsInitException();
    void testInitializationWithLessThanZeroBipassDiodeVDropThrowsInitException();
    //TEST UPDATE
    void testVoltageIsTotalVMinusDiodeLossDividedByActiveCellsTimesIsFailed();
    void testVoltageIsZeroWhenStringIsFailed();
    void testCurrentIsZeroWhenStringIsFailed();
    void testStringSourceVectorEqualsCellSourceVector();
    void testStringConductanceEqualsCellConductanceDividedByNumActiveCells();
    void testStringConductanceEqualsZeroIfNumFailedCellsEqualsNumCells();
    void testVoltageIsZeroIfDiodeVoltageDropWouldBringItLessThanZero();
    void testUpdateWithLessThanOneNumCells();

    /// @details test article
    FriendlySolarString   *mTestObj;
    SolarStringConfigData *mStringCD;
    SolarStringInputData  *mStringID;
    SolarStringConfigData *mStringBadBlockDiodeCD;
    SolarStringConfigData  *mStringBadBipassDiodeCD;
    PVCellCompanionModelConfigData *mCellCD;
    PVCellCompanionModelInputData  *mCellID;

    double mBipassDiodeVoltageDrop;
    int mBipassDiodeInterval;
    double mBlockingDiodeVoltageDrop;
    bool mIsFailed;
    int mNumCells;
    int mNumFailedCells;
    double mVoltage;
    double mCurrent;
    double mTolerance;

    bool   mIsMinorStep;
    double mStringCurrent;
    double mStringVoltage;
    double mSunAngle;
    double mTemperature;
    double mSunIntensity;
    bool  mThisBackSideIsLit;

private:
    /// @details private copy constructor
    UtSolarString(const UtSolarString&);

    /// @details private assignment operator
    UtSolarString &operator =(const UtSolarString &n);
};
/// @}
#endif /* UTSOLAR_STRING_EXISTS */
