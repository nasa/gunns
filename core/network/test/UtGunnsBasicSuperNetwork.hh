#ifndef UtGunnsBasicSuperNetwork_EXISTS
#define UtGunnsBasicSuperNetwork_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_NETWORK_BASIC_SUPER   GUNNS Basic Super Network Class Unit Tests
/// @ingroup  UT_GUNNS_NETWORK
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS basic super network class.  This also provides coverage of
///           the GUNNS super network base class: GunnsSuperNetworkBase.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/network/GunnsBasicSuperNetwork.hh"
#include "UtGunnsNetworkBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicSuperNetwork and befriend UtGunnsBasicSuperNetwork.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicSuperNetwork : public GunnsBasicSuperNetwork
{
    public:
        friend class UtGunnsBasicSuperNetwork;
        FriendlyGunnsBasicSuperNetwork(const std::string& name) : GunnsBasicSuperNetwork(name) {;}
        virtual ~FriendlyGunnsBasicSuperNetwork() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from FriendlyGunnsBasicSuperNetwork and befriend UtGunnsBasicSuperNetwork.
///
/// @details  This is a testing version of a super-network that implements joint network creation.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicSuperNetworkWithJoints : public FriendlyGunnsBasicSuperNetwork
{
    public:
        friend class UtGunnsBasicSuperNetwork;
        FriendlyGunnsBasicSuperNetworkWithJoints(const std::string& name)
            :
            FriendlyGunnsBasicSuperNetwork(name),
            tType(0) {;}
        virtual ~FriendlyGunnsBasicSuperNetworkWithJoints() {;}

    protected:
        int     tType;   /**< (--) Stored type argument for test. */
        virtual GunnsNetworkBase* createJointNetwork(const int type);
        virtual void registerJoints();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS basic super network class unit tests.
///
/// @details  This class provides the unit tests for the GUNNS basic super network base class within
///           the CPPUnit framework.  This also provides coverage and verification of
///           GunnsSuperNetworkBase.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicSuperNetwork: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GUNNS network base class unit test.
        UtGunnsBasicSuperNetwork();
        /// @brief    Default destructs this GUNNS network base class unit test.
        virtual ~UtGunnsBasicSuperNetwork();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests adding a sub-network.
        void testAddSubNetwork();
        /// @brief    Tests adding a super-network.
        void testAddSuperNetwork();
        /// @brief    Tests finalizing the sub-networks and super-network nodes.
        void testRegisterSuperNodes();
        /// @brief    Tests initialize method with sub-networks.
        void testInitialize();
        /// @brief    Tests incorrect initialization and error handling.
        void testInitializeErrors();
        /// @brief    Tests error handling of initialization with a duplicated sub-network.
        void testInitializeDuplicateSub();
        /// @brief    Tests initialize method with no sub-networks.
        void testInitializeEmpty();
        /// @brief    Tests restart method with sub-networks.
        void testRestart();
        /// @brief    Tests restart method error handling.
        void testRestartErrors();
        /// @brief    Tests restart method with no sub-networks.
        void testRestartEmpty();
        /// @brief    Tests update method with sub-networks.
        void testUpdate();
        /// @brief    Tests update method error handling.
        void testUpdateErrors();
        /// @brief    Tests update method with no sub-networks.
        void testUpdateEmpty();
        /// @brief    Tests joinLocations method.
        void testJoinLocations();
        /// @brief    Tests getJointIndex method.
        void testGetJointIndex();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsBasicSuperNetwork);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testAddSubNetwork);
        CPPUNIT_TEST(testAddSuperNetwork);
        CPPUNIT_TEST(testRegisterSuperNodes);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testInitializeErrors);
        CPPUNIT_TEST(testInitializeDuplicateSub);
        CPPUNIT_TEST(testInitializeEmpty);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testRestartErrors);
        CPPUNIT_TEST(testRestartEmpty);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testUpdateErrors);
        CPPUNIT_TEST(testUpdateEmpty);
        CPPUNIT_TEST(testJoinLocations);
        CPPUNIT_TEST(testGetJointIndex);
        CPPUNIT_TEST_SUITE_END();
        std::string                     tName;        /**< (--) Nominal name. */
        FriendlyGunnsBasicSuperNetwork* tArticle;     /**< (--) Pointer to the article under test. */
        FriendlyGunnsNetworkBase        tSubNetworkA; /**< (--) Network to be added as a sub-network. */
        FriendlyGunnsNetworkBase        tSubNetworkB; /**< (--) Network to be added as a sub-network. */
        static int                      TEST_ID;      /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsBasicSuperNetwork(const UtGunnsBasicSuperNetwork&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsBasicSuperNetwork& operator =(const UtGunnsBasicSuperNetwork&);
};

///@}

#endif
