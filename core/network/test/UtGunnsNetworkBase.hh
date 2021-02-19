#ifndef UtGunnsNetworkBase_EXISTS
#define UtGunnsNetworkBase_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_NETWORK_BASE   GUNNS Network Base Class Unit Tests
/// @ingroup  UT_GUNNS_NETWORK
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS network base class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/network/GunnsNetworkBase.hh"
#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicLink and befriend UtGunnsNetworkBase.
///
/// @details  This sets up a special link that supports unit testing of GunnsNetworkBase.  It keeps
///           track of when the test article updates the link (and by extension, the network solver)
///           and also throws exceptions to test exception handling.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsNetworkBaseLink : public GunnsBasicLink
{
    public:
        friend class UtGunnsNetworkBase;
        int mInitCount;
        int mRestartCount;
        int mStepCount;
        FriendlyGunnsNetworkBaseLink();
        virtual ~FriendlyGunnsNetworkBaseLink();
        void initialize(const GunnsBasicLinkConfigData& configData,
                        const GunnsBasicLinkInputData&  inputData,
                        std::vector<GunnsBasicLink*>&   networkLinks,
                        const int                       port0,
                        const int                       port1);
        virtual void step(const double dt);
        virtual void restartModel();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsNetworkBase and befriend UtGunnsNetworkBase.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsNetworkBase : public GunnsNetworkBase
{
    public:
        friend class UtGunnsNetworkBase;
        friend class UtGunnsBasicSuperNetwork;
        enum Nodes {
            NODE_0  = 0,
            NODE_1  = 1,
            NODE_2  = 2,
            GROUND  = 3,
            N_NODES = 4
        };
        GunnsBasicNode               netNodes[N_NODES];
        FriendlyGunnsNetworkBaseLink mLink;
        int                          mInitNodesCount;
        int                          mInitNetworkCount;
        FriendlyGunnsNetworkBase(const std::string& name);
        virtual ~FriendlyGunnsNetworkBase();
        virtual void initNodes(const std::string& name);
        virtual void initNetwork();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS network base class unit tests.
///
/// @details  This class provides the unit tests for the GUNNS network base class
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsNetworkBase: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GUNNS network base class unit test.
        UtGunnsNetworkBase();
        /// @brief    Default destructs this GUNNS network base class unit test.
        virtual ~UtGunnsNetworkBase();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests sub-network setup methods.
        void testSubNetworkSetup();
        /// @brief    Tests initialize method when a sub-network.
        void testInitSubNetwork();
        /// @brief    Tests initialize method when a standalone network.
        void testInitStandalone();
        /// @brief    Tests restart method when a sub-network.
        void testRestartSubNetwork();
        /// @brief    Tests restart method when a standalone network.
        void testRestartStandalone();
        /// @brief    Tests update method when a sub-network.
        void testUpdateSubNetwork();
        /// @brief    Tests update method when a standalone network.
        void testUpdateStandalone();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsNetworkBase);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testSubNetworkSetup);
        CPPUNIT_TEST(testInitSubNetwork);
        CPPUNIT_TEST(testInitStandalone);
        CPPUNIT_TEST(testRestartSubNetwork);
        CPPUNIT_TEST(testRestartStandalone);
        CPPUNIT_TEST(testUpdateSubNetwork);
        CPPUNIT_TEST(testUpdateStandalone);
        CPPUNIT_TEST_SUITE_END();
        std::string               tName;    /**< (--) Nominal name. */
        FriendlyGunnsNetworkBase* tArticle; /**< (--) Pointer to the article under test. */
        static int                TEST_ID;  /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsNetworkBase(const UtGunnsNetworkBase&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsNetworkBase& operator =(const UtGunnsNetworkBase&);
};

///@}

#endif
