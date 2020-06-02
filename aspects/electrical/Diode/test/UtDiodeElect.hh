#ifndef UtDiodeElect_EXISTS
#define UtDiodeElect_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_DIODE    Gunns Diode Link Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Diode Link
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/Diode/DiodeElect.hh"
#include "core/GunnsBasicLink.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from Diode and befriend UtDiodeElect.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyDiode : public DiodeElect
{
    public:
        FriendlyDiode();
        virtual ~FriendlyDiode();
        friend class UtDiodeElect;
};
inline FriendlyDiode::FriendlyDiode() : DiodeElect() {};
inline FriendlyDiode::~FriendlyDiode() {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Electric Diode Link  unit tests.
////
/// @details  This class provides the unit tests for the DiodeElect within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtDiodeElect: public CppUnit::TestFixture
{

public:
    /// @brief Nominal constructor
    UtDiodeElect();
    /// @brief Nominal destructs
    virtual ~UtDiodeElect();
    /// @brief Executes before each test.
    void setUp();
    /// @brief Executes after each test.
    void tearDown();
    /// @brief    Tests config and input data.
    void testConfigAndInput();
    /// @brief    Tests default construction.
    void testDefaultConstruction();
    /// @brief    Tests the initialization method with nominal values.
    void testNominalInitialization();
    /// @brief    Tests that the diode uses the proper resistance.
    void testDiodeShouldUseReverseResistanceWhenInReverseBias();
    /// @brief    Tests that the diode uses the proper resistance.
    void testDiodeShouldUseForwardResistanceWhenInForwardBias();
    /// @brief    Tests the diode indicates bias correctly based on actual deltaPotential.
    void testDiscrepancyBetweenForwardBiasAndDeltaPotential();
    /// @brief    Tests the diode indicates bias correctly based on actual deltaPotential.
    void testDiscrepancyBetweenReverseBiasAndDeltaPotential();
    /// @brief    Tests that initialize method will check incoming data.
    void testInitializeThrowsExceptionWhenGivenLessThanZeroForwardResistance();
    /// @brief    Tests that initialize method will check incoming data.
    void testInitializeThrowsExceptionWhenGivenLessThanZeroReverseResistance();
    /// @brief    Tests that initialize method will check incoming data.
    void testInitializeThrowsExceptionWhenRevResistanceLessThanForwResistance();
    /// @brief    Tests the nonlinear member method.
    void testTheDiodeIsANonLinearLink();
    /// @brief    Tests the compute flow method behaves correctly.
    void testTheComputeFlowMethodCalculatesCorrectly();

private:

    ///@brief Sets up the suite of tests for the DiodeElectric unit testing.
    CPPUNIT_TEST_SUITE(UtDiodeElect);
    CPPUNIT_TEST(testConfigAndInput);
    CPPUNIT_TEST(testDefaultConstruction);
    CPPUNIT_TEST(testNominalInitialization);
    CPPUNIT_TEST(testDiodeShouldUseReverseResistanceWhenInReverseBias);
    CPPUNIT_TEST(testDiodeShouldUseForwardResistanceWhenInForwardBias);
    CPPUNIT_TEST(testDiscrepancyBetweenForwardBiasAndDeltaPotential);
    CPPUNIT_TEST(testDiscrepancyBetweenReverseBiasAndDeltaPotential);
    CPPUNIT_TEST(testInitializeThrowsExceptionWhenGivenLessThanZeroForwardResistance);
    CPPUNIT_TEST(testInitializeThrowsExceptionWhenGivenLessThanZeroReverseResistance);
    CPPUNIT_TEST(testInitializeThrowsExceptionWhenRevResistanceLessThanForwResistance);
    CPPUNIT_TEST(testTheDiodeIsANonLinearLink);
    CPPUNIT_TEST(testTheComputeFlowMethodCalculatesCorrectly);
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

    ///@brief       Pointer to nominal configuration data
    DiodeElectConfigData*        mConfigData;

    ///@brief       Pointer to nominal input data
    DiodeElectInputData*         mInputData;

    ///@brief        Pointer to Test Article
    FriendlyDiode*               mTestArticle;

    ///@brief        Tolerance for double assert checks.
    double                       mTol;

    ///@brief        Nominal reverse resistance when diode is reverse biased.
    double                       mNominalReverseResistance;

    ///@brief        Nominal forward resistance when diode is forward biased.
    double                       mNominalForwardResistance;

    ///@brief        Nominal malfunction blockage flag

    bool                         mNominalMalfBlockageFlag;

    ///@brief        Nominal malfunction blockage value ranging from 0 to 1.
    double                       mNominalMalfBlockageValue;

    ///@brief        User supplied reverse resistance when diode is reverse biased.
    double                       mCustomReverseResistance;

    ///@brief        User supplied forward resistance when diode is forward biased.
    double                       mCustomForwardResistance;

    ///@brief        User supplied malfunction blockage flag
    bool                         mCustomMalfBlockageFlag;

    ///@brief        User supplied malfunction blockage value ranging from 0 to 1.
    double                       mCustomMalfBlockageValue;

    ///@brief        A stubb time step used by the gunns network but not needed here.
    double                       mStubbTimeStep;

    ///@brief        A stubb minor time step used by the gunns network but not needed here.
    int                       mStubbMinorTimeStep;

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Copy constructor unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    UtDiodeElect(const UtDiodeElect& that);
    ////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Assignment operator unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////
    UtDiodeElect& operator =(const UtDiodeElect& that);

};

///@}

#endif /*UtDiodeElect_EXISTS*/
