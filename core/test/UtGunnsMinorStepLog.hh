#ifndef UtGunnsMinorStepLog_EXISTS
#define UtGunnsMinorStepLog_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_MINOR_STEP_LOG GUNNS Solver Minor Step Log Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2020 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Solver Minor Step Log class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/GunnsMinorStepLog.hh"
#include "core/GunnsBasicNode.hh"
#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsMinorStepLog and befriend UtGunnsMinorStepLog.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsMinorStepLog : public GunnsMinorStepLog
{
    public:
        friend class UtGunnsMinorStepLog;
        friend class UtGunns;
};

class FriendlyGunnsLink : public GunnsBasicConductor
{
    public:
        friend class UtGunnsMinorStepLog;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Solver Minor Step Log Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsMinorStepLog class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsMinorStepLog : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsMinorStepLog unit test.
        UtGunnsMinorStepLog();
        /// @brief    Default destructs this GunnsMinorStepLog unit test.
        virtual ~UtGunnsMinorStepLog();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests the link data class.
        void testLinkData();
        /// @brief    Tests the data class.
        void testData();
        /// @brief    Tests the input data class.
        void testInputData();
        /// @brief    Tests default constructors.
        void testDefaultConstruction();
        /// @brief    Tests initialization.
        void testInitialize();
        /// @brief    Tests the beginMajorStep method.
        void testBeginMajorStep();
        /// @brief    Tests the various record methods.
        void testRecord();
        /// @brief    Tests wrap-around cases in the buffer.
        void testWraparound();
        /// @brief    Tests the updateAsync method.
        void testUpdateAsync();
        /// @brief    Tests the updateFreeze method.
        void testUpdateFreeze();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsMinorStepLog);
        CPPUNIT_TEST(testLinkData);
        CPPUNIT_TEST(testData);
        CPPUNIT_TEST(testInputData);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testBeginMajorStep);
        CPPUNIT_TEST(testRecord);
        CPPUNIT_TEST(testWraparound);
        CPPUNIT_TEST(testUpdateAsync);
        CPPUNIT_TEST(testUpdateFreeze);
        CPPUNIT_TEST_SUITE_END();
        FriendlyGunnsMinorStepLog*              tArticle;     /**< (--) Test article */
        GunnsMinorStepLogInputData::LogCommands tCommand;     /**< (--) Input data */
        unsigned int                            tLogSteps;    /**< (--) Input data */
        std::string                             tPath;        /**< (--) Input data */
        std::string                             tName;        /**< (--) Instance name */
        int                                     tNetworkSize; /**< (--) Init data */
        int                                     tNumLinks;    /**< (--) Init data */
        GunnsBasicLink**                        tLinks;       /**< (--) Init data */
        FriendlyGunnsLink                       tLink0;       /**< (--) Test link */
        FriendlyGunnsLink                       tLink1;       /**< (--) Test link */
        FriendlyGunnsLink                       tLink2;       /**< (--) Test link */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsMinorStepLog(const UtGunnsMinorStepLog& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsMinorStepLog& operator =(const UtGunnsMinorStepLog& that);
};

///@}

#endif
