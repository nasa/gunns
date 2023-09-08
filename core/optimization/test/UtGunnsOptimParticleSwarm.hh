#ifndef UtGunnsOptimParticleSwarm_EXISTS
#define UtGunnsOptimParticleSwarm_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_OPTIM_PSO   GUNNS Particle Swarm Optimization Unit Tests
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

#include "core/optimization/GunnsOptimParticleSwarm.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsOptimParticleSwarm and befriend UtGunnsOptimParticleSwarm.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsOptimParticleSwarm : public GunnsOptimParticleSwarm
{
    public:
        friend class UtGunnsOptimParticleSwarm;
        FriendlyGunnsOptimParticleSwarm() {;}
        virtual ~FriendlyGunnsOptimParticleSwarm() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsOptimBaseConfigData.
///
/// @details  Derives from GunnsOptimBaseConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsOptimParticleSwarmConfigData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsOptimParticleSwarmConfig : public GunnsOptimBaseConfigData
{
    public:
        BadGunnsOptimParticleSwarmConfig() {;}
        virtual ~BadGunnsOptimParticleSwarmConfig() {;};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS particle swarm optimization unit tests.
///
/// @details  This class provides the unit tests for the GUNNS particle swarm optimization
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsOptimParticleSwarm: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GUNNS particle swarm optimization unit test.
        UtGunnsOptimParticleSwarm();
        /// @brief    Default destructs this GUNNS particle swarm optimization unit test.
        virtual ~UtGunnsOptimParticleSwarm();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests the GunnsOptimParticleSwarmState structure.
        void testParticleState();
        /// @brief    Tests the GunnsOptimParticleSwarmParticle structure.
        void testParticle();
        /// @brief    Tests the GunnsOptimParticleSwarmConfigData class.
        void testConfigData();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests the initialize method with the min/max corners initial swarm state option.
        void testInitializeMinMax();
        /// @brief    Tests the initialize method with the random initial swarm state option.
        void testInitializeRandom();
        /// @brief    Tests the initialize method with errors.
        void testInitErrors();
        /// @brief    Tests the update method.
        void testUpdate();
        /// @brief    Tests the propagateSwarm method.
        void testPropagateSwarm();
        /// @brief    Tests the assignCost method.
        void testAssignCost();
        /// @brief    Tests the shutdown method.
        void testShutdown();
        /// @brief    Tests the initialize method with the file read initial swarm state option.
        void testInitializeFile();
        /// @brief    Tests the initialize method with the continuous file read initial swarm state option.
        void testInitializeContinuous();
        /// @brief    Tests access methods.
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsOptimParticleSwarm);
        CPPUNIT_TEST(testParticleState);
        CPPUNIT_TEST(testParticle);
        CPPUNIT_TEST(testConfigData);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitializeMinMax);
        CPPUNIT_TEST(testInitializeRandom);
        CPPUNIT_TEST(testInitErrors);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testPropagateSwarm);
        CPPUNIT_TEST(testAssignCost);
        CPPUNIT_TEST(testShutdown);
        CPPUNIT_TEST(testInitializeFile);
        CPPUNIT_TEST(testInitializeContinuous);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();
        std::string                            tName;       /**< (--) Nominal name. */
        FriendlyGunnsOptimParticleSwarm*       tArticle;    /**< (--) Pointer to the article under test. */
        GunnsOptimParticleSwarmConfigData*     tConfigData; /**< (--) Pointer to the configuration data. */
        double                                 tMcInput1;   /**< (--) 1st Monte Carlo inptu variable. */
        double                                 tMcInput2;   /**< (--) 2nd Monte Carlo inptu variable. */
        std::vector<GunnsOptimMonteCarloInput> tMcInputs;   /**< (--) Monte Carlo input variables. */
        static int                             TEST_ID;     /**< (--) Test identification number. */
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtGunnsOptimParticleSwarm(const UtGunnsOptimParticleSwarm&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        UtGunnsOptimParticleSwarm& operator =(const UtGunnsOptimParticleSwarm&);
};

///@}

#endif
