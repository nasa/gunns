#ifndef UtGunnsOptimGradientDescent_EXISTS
#define UtGunnsOptimGradientDescent_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_OPTIM_PSO   GUNNS Gradient Descent Optimization Unit Tests
/// @ingroup  UT_GUNNS_OPTIM
///
/// @copyright Copyright 2023 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS particle swarm optimization.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/optimization/GunnsOptimGradientDescent.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsOptimGradientDescent and befriend UtGunnsOptimGradientDescent.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsOptimGradientDescent : public GunnsOptimGradientDescent
{
    public:
        friend class UtGunnsOptimGradientDescent;
        FriendlyGunnsOptimGradientDescent() {;}
        virtual ~FriendlyGunnsOptimGradientDescent() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsOptimBaseConfigData.
///
/// @details  Derives from GunnsOptimBaseConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsOptimGradientDescentConfigData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsOptimGradientDescentConfig : public GunnsOptimBaseConfigData
{
    public:
        BadGunnsOptimGradientDescentConfig() {;}
        virtual ~BadGunnsOptimGradientDescentConfig() {;};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS gradient descent optimization unit tests.
///
/// @details  This class provides the unit tests for the GUNNS gradient descent optimization
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsOptimGradientDescent: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GUNNS gradient descent optimization unit test.
        UtGunnsOptimGradientDescent();
        /// @brief    Default destructs this GUNNS gradient descent optimization unit test.
        virtual ~UtGunnsOptimGradientDescent();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests the GunnsOptimGradientDescentState structure.
        void testDescentState();
        /// @brief    Tests the GunnsOptimGradientDescentConfigData class.
        void testConfigData();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests the initialize method withi nominal initial data.
        void testInitialize();
        /// @brief    Tests the initialize method with errors.
        void testInitErrors();
        /// @brief    Tests the update method.
        void testUpdate();
        /// @brief    Tests the update method with zero state.
        void testUpdateZeroState();
        /// @brief    Tests the assignCost method.
        void testAssignCost();
        /// @brief    Tests the shutdown method.
        void testShutdown();
        /// @brief    Tests access methods.
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsOptimGradientDescent);
        CPPUNIT_TEST(testDescentState);
        CPPUNIT_TEST(testConfigData);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testInitErrors);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testUpdateZeroState);
        CPPUNIT_TEST(testAssignCost);
        CPPUNIT_TEST(testShutdown);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();
        std::string                            tName;         /**< (--) Nominal name. */
        FriendlyGunnsOptimGradientDescent*     tArticle;      /**< (--) Pointer to the article under test. */
        GunnsOptimGradientDescentConfigData*   tConfigData;   /**< (--) Pointer to the configuration data. */
        double*                                tInitialState; /**< (--) Array of initial state values for config data. */
        double                                 tMcInput1;     /**< (--) 1st Monte Carlo inptu variable. */
        double                                 tMcInput2;     /**< (--) 2nd Monte Carlo inptu variable. */
        std::vector<GunnsOptimMonteCarloInput> tMcInputs;     /**< (--) Monte Carlo input variables. */
        static int                             TEST_ID;       /**< (--) Test identification number. */
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtGunnsOptimGradientDescent(const UtGunnsOptimGradientDescent&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        UtGunnsOptimGradientDescent& operator =(const UtGunnsOptimGradientDescent&);
};

///@}

#endif
