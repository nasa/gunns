#ifndef UtGunnsOptimMonteCarlo_EXISTS
#define UtGunnsOptimMonteCarlo_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_OPTIM_MONTE_CARLO   GUNNS Optimization Monte Carlo Manager Unit Tests
/// @ingroup  UT_GUNNS_OPTIM
///
/// @copyright Copyright 2023 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS optimization monte carlo manager.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/optimization/GunnsOptimMonteCarlo.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsOptimMonteCarlo and befriend UtGunnsOptimMonteCarlo.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsOptimMonteCarlo : public GunnsOptimMonteCarlo
{
    public:
        friend class UtGunnsOptimMonteCarlo;
        FriendlyGunnsOptimMonteCarlo(const std::string& name = "");
        virtual ~FriendlyGunnsOptimMonteCarlo();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS monte carlo manager unit tests.
///
/// @details  This class provides the unit tests for the GUNNS monte carlo manager
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsOptimMonteCarlo: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GUNNS monte carlo manager unit test.
        UtGunnsOptimMonteCarlo();
        /// @brief    Default destructs this GUNNS monte carlo manager unit test.
        virtual ~UtGunnsOptimMonteCarlo();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests the initMaster method.
        void testInitMaster();
        /// @brief    Tests the initSlave method.
        void testInitSlave();
        /// @brief    Tests the updateMasterPre method.
        void testUpdateMasterPre();
        /// @brief    Tests the updateMasterPost method.
        void testUpdateMasterPost();
        /// @brief    Tests the updateMasterShutdown method.
        void testUpdateMasterShutdown();
        /// @brief    Tests the updateSlavePost method.
        void testUpdateSlavePost();
        /// @brief    Tests the updateSlaveInputs method.
        void testUpdateSlaveInputs();
        /// @brief    Tests the updateSlaveOutputs method.
        void testUpdateSlaveOutputs();
        /// @brief    Tests some features of MC input constraints.
        void testConstraints();
        /// @brief    Tests access methods.
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsOptimMonteCarlo);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitMaster);
        CPPUNIT_TEST(testInitSlave);
        CPPUNIT_TEST(testUpdateMasterPre);
        CPPUNIT_TEST(testUpdateMasterPost);
        CPPUNIT_TEST(testUpdateMasterShutdown);
        CPPUNIT_TEST(testUpdateSlavePost);
        CPPUNIT_TEST(testUpdateSlaveInputs);
        CPPUNIT_TEST(testUpdateSlaveOutputs);
        CPPUNIT_TEST(testConstraints);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();
        std::string                   tName;    /**< (--) Nominal name. */
        FriendlyGunnsOptimMonteCarlo* tArticle; /**< (--) Pointer to the article under test. */
        static int                    TEST_ID;  /**< (--) Test identification number. */
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtGunnsOptimMonteCarlo(const UtGunnsOptimMonteCarlo&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        UtGunnsOptimMonteCarlo& operator =(const UtGunnsOptimMonteCarlo&);
};

///@}

#endif
