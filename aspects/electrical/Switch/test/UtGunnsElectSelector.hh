#ifndef UtGunnsElectSelector_EXISTS
#define UtGunnsElectSelector_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_SWITCH_SELECTOR   Electrical Selector Unit Tests
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Electrical Selector link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/Switch/GunnsElectSelector.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectSelector and befriend UtGunnsElectSelector.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectSelector : public GunnsElectSelector
{
    public:
        FriendlyGunnsElectSelector();
        virtual ~FriendlyGunnsElectSelector();
        friend class UtGunnsElectSelector;
};
inline FriendlyGunnsElectSelector::FriendlyGunnsElectSelector() : GunnsElectSelector() {};
inline FriendlyGunnsElectSelector::~FriendlyGunnsElectSelector() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Electrical Selector link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectSelector: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Valve unit test.
        UtGunnsElectSelector();
        /// @brief    Default constructs this Check Valve unit test.
        virtual ~UtGunnsElectSelector();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config data.
        void testConfig();
        /// @brief    Tests input data.
        void testInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests step method.
        void testStep();
        /// @brief    Tests computeFlows method.
        void testComputeFlows();
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsElectSelector);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes.
        enum {N_NODES = 3};
        std::vector<GunnsBasicLink*>  tLinks;              /**< (--)    Link vector. */
        std::string                   tName;               /**< (--)    Nominal name. */
        GunnsBasicNode                tNodes[N_NODES];     /**< (--)    Nominal connected nodes. */
        GunnsNodeList                 tNodeList;           /**< (--)    Network node structure. */
        std::vector<int>              tPorts;              /**< (--)    Nominal ports assignment. */
        double                        tDefaultConductance; /**< (1/ohm) Nominal default conductance. */
        std::vector<std::string>      tPortLabels;         /**< (--)    Nominal default port labels. */
        GunnsElectSelectorConfigData* tConfigData;         /**< (--)    Pointer to nominal configuration data. */
        bool                          tMalfBlockageFlag;   /**< (--)    Blockage malfunction flag. */
        double                        tMalfBlockageValue;  /**< (--)    Blockage malfunction value. */
        int                           tSelector;           /**< (--)    Nominal selector. */
        GunnsElectSelectorInputData*  tInputData;          /**< (--)    Pointer to nominal input data. */
        FriendlyGunnsElectSelector*   tArticle;            /**< (--)    Pointer to the friendly Valve under test. */
        static int                    TEST_ID;             /**< (--)    Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectSelector(const UtGunnsElectSelector&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectSelector& operator =(const UtGunnsElectSelector&);
};

///@}

#endif
