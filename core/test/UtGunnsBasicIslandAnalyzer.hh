#ifndef UtGunnsBasicIslandAnalyzer_EXISTS
#define UtGunnsBasicIslandAnalyzer_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_BASIC_ISLAND_ANALYZER    Gunns Basic Island Analyzer Spotter Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Basic Island Analyzer Spotter class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/GunnsBasicIslandAnalyzer.hh"
#include "core/GunnsBasicNode.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicIslandAnalyzer and befriend UtGunnsBasicIslandAnalyzer.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicIslandAnalyzer : public GunnsBasicIslandAnalyzer
{
    public:
        FriendlyGunnsBasicIslandAnalyzer(GunnsNodeList& nodeList) : GunnsBasicIslandAnalyzer(nodeList) {};
        virtual ~FriendlyGunnsBasicIslandAnalyzer() {;}
        friend class UtGunnsBasicIslandAnalyzer;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterConfigData.
///
/// @details  Derives from GunnsNetworkSpotterConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsBasicIslandAnalyzerConfigData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsBasicIslandAnalyzerConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        BadGunnsBasicIslandAnalyzerConfigData(const std::string& name) : GunnsNetworkSpotterConfigData(name) {}
        virtual ~BadGunnsBasicIslandAnalyzerConfigData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterInputData.
///
/// @details  Derives from GunnsNetworkSpotterInputData and is used to test that a dynamic_cast of
///           this type to the GunnsBasicIslandAnalyzerInputData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsBasicIslandAnalyzerInputData : public GunnsNetworkSpotterInputData
{
    public:
        BadGunnsBasicIslandAnalyzerInputData() {}
        virtual ~BadGunnsBasicIslandAnalyzerInputData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basic Island Analyzer Spotter Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsBasicIslandAnalyzer class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicIslandAnalyzer : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsBasicIslandAnalyzer unit test.
        UtGunnsBasicIslandAnalyzer();
        /// @brief    Default destructs this GunnsBasicIslandAnalyzer unit test.
        virtual ~UtGunnsBasicIslandAnalyzer();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests the config data class.
        void testConfig();
        /// @brief    Tests the input data class.
        void testInput();
        /// @brief    Tests default constructors.
        void testDefaultConstruction();
        /// @brief    Tests initialization.
        void testInitialize();
        /// @brief    Tests the stepPreSolver method.
        void testPreSolver();
        /// @brief    Tests the stepPostSolver method.
        void testPostSolver();
        /// @brief    Tests the setter and getter methods.
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsBasicIslandAnalyzer);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testPreSolver);
        CPPUNIT_TEST(testPostSolver);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsBasicIslandAnalyzer   tArticle;  /**< (--) Test article */
        GunnsBasicNode                     tNodes[3]; /**< (--) Network nodes */
        GunnsNodeList                      tNodeList; /**< (--) Test referenced network node list */
        std::string                        tName;     /**< (--) Instance name */
        GunnsBasicIslandAnalyzerConfigData tConfig;   /**< (--) Nominal config data */
        GunnsBasicIslandAnalyzerInputData  tInput;    /**< (--) Nominal input data */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsBasicIslandAnalyzer(const UtGunnsBasicIslandAnalyzer& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsBasicIslandAnalyzer& operator =(const UtGunnsBasicIslandAnalyzer& that);
};

///@}

#endif
