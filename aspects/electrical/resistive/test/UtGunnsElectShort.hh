#ifndef UtGunnsElectShort_EXISTS
#define UtGunnsElectShort_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_ELECTRICAL_RESISTIVE_SHORT    GUNNS Electrical Short Link Unit Test
/// @ingroup  UT_GUNNS_ELECTRICAL_RESISTIVE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Electrical Short link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "aspects/electrical/resistive/GunnsElectShort.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectShort and befriend UtGunnsElectShort.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectShort : public GunnsElectShort
{
    public:
        FriendlyGunnsElectShort();
        virtual ~FriendlyGunnsElectShort();
        friend class UtGunnsElectShort;
};
inline FriendlyGunnsElectShort::FriendlyGunnsElectShort() : GunnsElectShort() {};
inline FriendlyGunnsElectShort::~FriendlyGunnsElectShort() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Electrical Short link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectShort: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Valve unit test.
        UtGunnsElectShort();
        /// @brief    Default constructs this Check Valve unit test.
        virtual ~UtGunnsElectShort();
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
        /// @brief    Tests step method.
        void testStep();
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsElectShort);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes.
        enum {N_NODES = 3};
        std::vector<GunnsBasicLink*> tLinks;                    /**< (--)  Link vector. */
        std::string                  tName;                     /**< (--)  Nominal name. */
        GunnsBasicNode               tNodes[N_NODES];           /**< (--)  Nominal connected nodes. */
        GunnsNodeList                tNodeList;                 /**< (--)  Network node structure. */
        int                          tPort0;                    /**< (--)  Nominal port 0 assignment. */
        int                          tPort1;                    /**< (--)  Nominal port 1 assignment. */
        GunnsElectShortConfigData*   tConfigData;               /**< (--)  Pointer to nominal configuration data. */
        bool                         tMalfBlockageFlag;         /**< (--)  Nominal input data. */
        double                       tMalfBlockageValue;        /**< (--)  Nominal input data. */
        double                       tMalfMinimumVoltage;       /**< (V)   Nominal input data. */
        bool                         tMalfConstantPowerFlag;    /**< (--)  Nominal input data. */
        double                       tMalfConstantPowerValue;   /**< (W)   Nominal input data. */
        bool                         tMalfConstantCurrentFlag;  /**< (--)  Nominal input data. */
        double                       tMalfConstantCurrentValue; /**< (amp) Nominal input data. */
        bool                         tMalfInitialPowerFlag;     /**< (--)  Nominal input data. */
        double                       tMalfInitialPowerValue;    /**< (W)   Nominal input data. */
        bool                         tMalfInitialCurrentFlag;   /**< (--)  Nominal input data. */
        double                       tMalfInitialCurrentValue;  /**< (amp) Nominal input data. */
        bool                         tMalfResistanceFlag;       /**< (--)  Nominal input data. */
        double                       tMalfResistanceValue;      /**< (ohm) Nominal input data. */
        GunnsElectShortInputData*    tInputData;                /**< (--)  Pointer to nominal input data. */
        FriendlyGunnsElectShort*     tArticle;                  /**< (--)  Pointer to the test article. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectShort(const UtGunnsElectShort&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectShort& operator =(const UtGunnsElectShort&);
};

///@}

#endif
