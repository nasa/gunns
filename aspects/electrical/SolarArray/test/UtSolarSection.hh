#ifndef UTSOLAR_SECTION_EXISTS
#define UTSOLAR_SECTION_EXISTS

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "aspects/electrical/SolarArray/SolarSection.hh"


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from SolarSection and befriend FriendlySolarSection.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
///////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlySolarSection : public SolarSection {
    public:
        FriendlySolarSection();
        virtual ~FriendlySolarSection();

        friend class UtSolarSection;
};

inline FriendlySolarSection::FriendlySolarSection() : SolarSection() {}
inline FriendlySolarSection::~FriendlySolarSection() {}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Firmware unit tests.
///
/// @details  This class provides the unit tests for the witchingUnitFirmware with CPPUnit framework.
///////////////////////////////////////////////////////////////////////////////////////////////////
class UtSolarSection : public CppUnit::TestFixture {

    CPPUNIT_TEST_SUITE(UtSolarSection);
    CPPUNIT_TEST(testDefaultConstructor);
    CPPUNIT_TEST(testDefaultConfigData);
    CPPUNIT_TEST(testCopyConstructors);
    CPPUNIT_TEST(testCustomConfigData);
    CPPUNIT_TEST(testDefaultInputData);
    CPPUNIT_TEST(testCustomInputData);
    CPPUNIT_TEST(testNominalInitialization);
    CPPUNIT_TEST(testInitializationWithLessThanOneString);
    CPPUNIT_TEST(testConductanceIsZeroIfSectionShadowed);
    CPPUNIT_TEST(testSourceVectorIsZeroIfSectionShadowed);
    CPPUNIT_TEST(testUpdateSumsUpGeqForSection);
    CPPUNIT_TEST(testUpdateSumsUpIeqForSection);
    CPPUNIT_TEST(testSectionShadowedIfBothZAreBlocked);
    CPPUNIT_TEST(testSectionBacklitIfZMinusBlockedButZPlusIsnt);

    CPPUNIT_TEST_SUITE_END();

public:
    /// @brief default constructor
    UtSolarSection();

    /// @brief default destructor
    virtual ~UtSolarSection();

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
    void testInitializationWithLessThanOneString();
    void testConductanceIsZeroIfSectionShadowed();
    void testSourceVectorIsZeroIfSectionShadowed();
    void testUpdateMethodCountsNumberOfFailedCells();
    void testSectionShadowedIfBothZAreBlocked();
    void testSectionBacklitIfZMinusBlockedButZPlusIsnt();
    void testUpdateSumsUpGeqForSection();
    void testUpdateSumsUpIeqForSection();


    /// @details test article
    FriendlySolarSection   *mTestObj;
    SolarSectionConfigData   *mSectionCD;
    SolarSectionInputData    *mSectionID;
    SolarStringConfigData       *mStringCD;
    SolarStringInputData        *mStringID;
    PVCellCompanionModelConfigData *mRefCellCD;
    PVCellCompanionModelInputData  *mRefCellID;

    int mNumStrings;
    double mTemperature;
    double mSunAngleFromHorizontal;
    double mSunIntensity;
    bool mIsShadowed;

    bool mIsMinor;
    int mNumCells;
    double mTolerance;


private:
    /// @details private copy constructor
    UtSolarSection(const UtSolarSection&);

    /// @details private assignment operator
    UtSolarSection &operator =(const UtSolarSection &n);
};
/// @}
#endif /* UTSOLAR_SECTION_EXISTS */
