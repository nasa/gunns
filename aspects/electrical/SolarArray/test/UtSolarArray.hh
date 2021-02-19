#ifndef UtSolarArray_EXISTS
#define UtSolarArray_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_SolarArray    Gunns SolarArray Link Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns SolarArray Link
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/SolarArray/SolarArray.hh"
#include "UtSolarSection.hh"
#include "core/GunnsBasicLink.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from SolarArray and befriend UtSolarArray.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlySolarArray : public SolarArray
{
    public:
        FriendlySolarArray();
        virtual ~FriendlySolarArray();
        friend class UtSolarArray;

};
inline FriendlySolarArray::FriendlySolarArray() : SolarArray() {};
inline FriendlySolarArray::~FriendlySolarArray() {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns ric SolarArray Link  unit tests.
////
/// @details  This class provides the unit tests for the SolarArray within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtSolarArray: public CppUnit::TestFixture
{

public:
    /// @brief Nominal constructor
    UtSolarArray();
    /// @brief Nominal destructs
    virtual ~UtSolarArray();
    /// @brief Executes before each test.
    void setUp();
    /// @brief Executes after each test.
    void tearDown();


    void testDefaultConstructor();
    void testDefaultConfigData();
    void testCopyConstructors();
    void testCustomConfigData();
    void testDefaultInputData();
    void testCustomInputData();
    void testNominalInitialization();
    void testInitializeWithLessThanOneSectionThrowsException();
    void testComputeFlowsWithTooSmallVoltage();
    void testComputeFlowsWithTooBigVoltage();
    void testComputeFlowsWithNegativeCurrent();
    void testComputeFlowsWithTooBigCurrent();
    void testMinorStep();
    void testComputeFlows();
    void testStepNonLeo();


private:

    ///@brief Sets up the suite of tests for the SolarArrayric unit testing.
    CPPUNIT_TEST_SUITE(UtSolarArray);
    CPPUNIT_TEST(testDefaultConstructor);
    CPPUNIT_TEST(testDefaultConfigData);
    CPPUNIT_TEST(testCopyConstructors);
    CPPUNIT_TEST(testCustomConfigData);
    CPPUNIT_TEST(testDefaultInputData);
    CPPUNIT_TEST(testCustomInputData);
    CPPUNIT_TEST(testNominalInitialization);
    CPPUNIT_TEST(testInitializeWithLessThanOneSectionThrowsException);
    CPPUNIT_TEST(testComputeFlowsWithTooSmallVoltage);
    CPPUNIT_TEST(testComputeFlowsWithTooBigVoltage);
    CPPUNIT_TEST(testComputeFlowsWithNegativeCurrent);
    CPPUNIT_TEST(testComputeFlowsWithTooBigCurrent);
    CPPUNIT_TEST(testMinorStep);
    CPPUNIT_TEST(testComputeFlows);
    CPPUNIT_TEST(testStepNonLeo);
    CPPUNIT_TEST_SUITE_END();

    /// @brief      Enumeration for the number of nodes and fluid constituents.
    enum {N_NODES = 2};

    /// @brief      Link vector.
    std::vector<GunnsBasicLink*> mLinks;

    /// @brief      Nominal name.
    std::string                  mName;

    /// @brief      Nominal connected nodes.
    GunnsBasicNode               mNodes[N_NODES];

    /// @brief      Network node structure.
    GunnsNodeList                mNodeList;

    /// @brief      Nominal inlet port index.
    int                          mPort0;

    /// @brief      Nominal outlet port index.
    int                          mPort1;

    /// @brief      Minor time step.
    int                          mMinorTimeStep;

    /// @details test article
    FriendlySolarArray   *mTestObj;
    SolarArrayConfigData *mTestCD;
    SolarArrayInputData  *mTestID;
    SolarSectionConfigData  *mSectionCD;
    SolarSectionInputData  *mSectionID;
    PVCellCompanionModelConfigData *mRefCellCD;
    PVCellCompanionModelInputData  *mRefCellID;
    SolarStringConfigData *mStringCD;
    SolarStringInputData  *mStringID;

    int mNumSections;
    double mTolerance;
    bool   mNominalMalfBlockageFlag;
    double mNominalMalfBlockageValue;
    double                       mStubbTimeStep;

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Copy constructor unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    UtSolarArray(const UtSolarArray& that);
    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Assignment operator unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    UtSolarArray& operator =(const UtSolarArray& that);

};

///@}

#endif /*UtSolarArray_EXISTS*/
