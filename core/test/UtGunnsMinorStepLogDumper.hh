#ifndef UtGunnsMinorStepLogDumper_EXISTS
#define UtGunnsMinorStepLogDumper_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_MINOR_STEP_LOG_FILE_DUMP Gunns Solver Minor Step Log File Dump Spotter Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Solver Minor Step Log File Dump Spotter class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/GunnsMinorStepLogDumper.hh"
#include "core/GunnsBasicNode.hh"
#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsMinorStepLogDumper and befriend UtGunnsMinorStepLogDumper.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsMinorStepLogDumper : public GunnsMinorStepLogDumper
{
    public:
        FriendlyGunnsMinorStepLogDumper(Gunns& solver, const std::vector<GunnsBasicLink*>& links)
            : GunnsMinorStepLogDumper(solver, links) {};
        virtual ~FriendlyGunnsMinorStepLogDumper() {;}
        friend class UtGunnsMinorStepLogDumper;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterConfigData.
///
/// @details  Derives from GunnsNetworkSpotterConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsMinorStepLogDumperConfigData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsMinorStepLogDumperConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        BadGunnsMinorStepLogDumperConfigData(const std::string& name) : GunnsNetworkSpotterConfigData(name) {}
        virtual ~BadGunnsMinorStepLogDumperConfigData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterInputData.
///
/// @details  Derives from GunnsNetworkSpotterInputData and is used to test that a dynamic_cast of
///           this type to the GunnsMinorStepLogDumperInputData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsMinorStepLogDumperInputData : public GunnsNetworkSpotterInputData
{
    public:
        BadGunnsMinorStepLogDumperInputData() {}
        virtual ~BadGunnsMinorStepLogDumperInputData() {}
};

class FriendlyGunnsSolver : public Gunns
{
    public:
        friend class UtGunnsMinorStepLogDumper;
};

class FriendlyGunnsLink : public GunnsBasicConductor
{
    public:
        friend class UtGunnsMinorStepLogDumper;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Solver Minor Step Log File Dump Spotter Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsMinorStepLogDumper class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsMinorStepLogDumper : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsMinorStepLogDumper unit test.
        UtGunnsMinorStepLogDumper();
        /// @brief    Default destructs this GunnsMinorStepLogDumper unit test.
        virtual ~UtGunnsMinorStepLogDumper();
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
        /// @brief    Tests the stepPostSolver method in AUTO mode.
        void testPostSolverAuto();
        /// @brief    Tests the stepPostSolver method in SNAP mode.
        void testPostSolverSnap();
        /// @brief    Tests the stepPostSolver method in STANDBY mode.
        void testPostSolverStandby();
        /// @brief    Tests the updateAsync method.
        void testUpdateAsync();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsMinorStepLogDumper);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testPreSolver);
        CPPUNIT_TEST(testPostSolverAuto);
        CPPUNIT_TEST(testPostSolverSnap);
        CPPUNIT_TEST(testPostSolverStandby);
        CPPUNIT_TEST(testUpdateAsync);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsMinorStepLogDumper   tArticle;     /**< (--) Test article */
        std::string                       tName;        /**< (--) Instance name */
        GunnsMinorStepLogDumperConfigData tConfig;      /**< (--) Nominal config data */
        GunnsMinorStepLogDumper::LogModes tLogMode;     /**< (--) Nominal input data */
        int                               tLogSteps;    /**< (--) Nominal input data */
        std::string                       tPath;        /**< (--) Nominal input data */
        GunnsMinorStepLogDumperInputData  tInput;       /**< (--) Nominal input data */
        FriendlyGunnsSolver               tSolver;      /**< (--) Test object for article constructor */
        std::vector<GunnsBasicLink*>      tLinks;       /**< (--) Test object for article constructor */
        int                               tNetworkSize; /**< (--) Init data for test solver's minor step log */
        int                               tNumLinks;    /**< (--) Init data for test solver's minor step log */
        int                               tStepLimit;   /**< (--) Init data for test solver's minor step log */
        FriendlyGunnsLink                 tLink0;       /**< (--) Test link */
        FriendlyGunnsLink                 tLink1;       /**< (--) Test link */
        FriendlyGunnsLink                 tLink2;       /**< (--) Test link */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsMinorStepLogDumper(const UtGunnsMinorStepLogDumper& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsMinorStepLogDumper& operator =(const UtGunnsMinorStepLogDumper& that);
};

///@}

#endif
