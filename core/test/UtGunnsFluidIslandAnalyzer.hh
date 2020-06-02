#ifndef UtGunnsFluidIslandAnalyzer_EXISTS
#define UtGunnsFluidIslandAnalyzer_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_ISLAND_ANALYZER    Gunns Fluid Island Analyzer Spotter Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Island Analyzer Spotter class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/GunnsFluidIslandAnalyzer.hh"
#include "core/GunnsFluidNode.hh"
#include "aspects/fluid/fluid/GunnsFluidTraceCompounds.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidIslandAnalyzer and befriend UtGunnsFluidIslandAnalyzer.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidIslandAnalyzer : public GunnsFluidIslandAnalyzer
{
    public:
        FriendlyGunnsFluidIslandAnalyzer(GunnsNodeList& nodeList) : GunnsFluidIslandAnalyzer(nodeList) {};
        virtual ~FriendlyGunnsFluidIslandAnalyzer() {;}
        friend class UtGunnsFluidIslandAnalyzer;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterConfigData.
///
/// @details  Derives from GunnsNetworkSpotterConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsFluidIslandAnalyzerConfigData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsFluidIslandAnalyzerConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        BadGunnsFluidIslandAnalyzerConfigData(const std::string& name) : GunnsNetworkSpotterConfigData(name) {}
        virtual ~BadGunnsFluidIslandAnalyzerConfigData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterInputData.
///
/// @details  Derives from GunnsNetworkSpotterInputData and is used to test that a dynamic_cast of
///           this type to the GunnsFluidIslandAnalyzerInputData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsFluidIslandAnalyzerInputData : public GunnsNetworkSpotterInputData
{
    public:
        BadGunnsFluidIslandAnalyzerInputData() {}
        virtual ~BadGunnsFluidIslandAnalyzerInputData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Island Analyzer Spotter Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsFluidIslandAnalyzer class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidIslandAnalyzer : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsFluidIslandAnalyzer unit test.
        UtGunnsFluidIslandAnalyzer();
        /// @brief    Default destructs this GunnsFluidIslandAnalyzer unit test.
        virtual ~UtGunnsFluidIslandAnalyzer();
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
        CPPUNIT_TEST_SUITE(UtGunnsFluidIslandAnalyzer);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testPreSolver);
        CPPUNIT_TEST(testPostSolver);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsFluidIslandAnalyzer    tArticle;         /**< (--) Test article */
        GunnsFluidNode                      tNodes[3];        /**< (--) Network nodes */
        GunnsNodeList                       tNodeList;        /**< (--) Test referenced network node list */
        std::string                         tName;            /**< (--) Instance name */
        GunnsFluidIslandAnalyzerConfigData  tConfig;          /**< (--) Nominal config data */
        GunnsFluidIslandAnalyzerInputData   tInput;           /**< (--) Nominal input data */
        DefinedFluidProperties*             tFluidProperties; /**< (--) Pre-defined fluid properties */
        PolyFluidConfigData*                tFluidConfig;     /**< (--) Fluid config data */
        PolyFluidInputData*                 tFluidInput1;     /**< (--) Fluid 1 input data */
        PolyFluidInputData*                 tFluidInput2;     /**< (--) Fluid 2 input data */
        GunnsFluidTraceCompoundsInputData*  tTcInput1;        /**< (--) Pointer to nominal trace compounds input data. */
        GunnsFluidTraceCompoundsInputData*  tTcInput2;        /**< (--) Pointer to nominal trace compounds input data. */
        GunnsFluidTraceCompoundsConfigData* tTcConfig;        /**< (--) Pointer to nominal trace compounds config data. */
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidIslandAnalyzer(const UtGunnsFluidIslandAnalyzer& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidIslandAnalyzer& operator =(const UtGunnsFluidIslandAnalyzer& that);
};

///@}

#endif
