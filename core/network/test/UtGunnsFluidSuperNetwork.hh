#ifndef UtGunnsFluidSuperNetwork_EXISTS
#define UtGunnsFluidSuperNetwork_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_NETWORK_FLUID_SUPER   GUNNS Fluid Super Network Class Unit Tests
/// @ingroup  UT_GUNNS_NETWORK
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS fluid super network class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/network/GunnsFluidSuperNetwork.hh"
#include "UtGunnsNetworkBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicSuperNetwork and befriend UtGunnsBasicSuperNetwork.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSuperNetwork : public GunnsFluidSuperNetwork
{
    public:
        friend class UtGunnsFluidSuperNetwork;
        FriendlyGunnsFluidSuperNetwork(const std::string& name) : GunnsFluidSuperNetwork(name) {;}
        virtual ~FriendlyGunnsFluidSuperNetwork() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsNetworkBase and befriend UtGunnsFluidSuperNetwork.
///
/// @details  This sets up a default fluid network to add as sub-networks to the test article.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSubNetwork : public GunnsNetworkBase
{
    public:
        friend class UtGunnsFluidSuperNetwork;
        enum Nodes {
            NODE_0  = 0,
            NODE_1  = 1,
            NODE_2  = 2,
            VACUUM  = 3,
            N_NODES = 4
        };
        PolyFluidConfigData* netInternalFluidConfig;
        GunnsFluidNode       netNodes[N_NODES];
        FriendlyGunnsFluidSubNetwork(const std::string& name);
        virtual ~FriendlyGunnsFluidSubNetwork();
        virtual void initNodes(const std::string& name);
        virtual void initNetwork();
        virtual const PolyFluidConfigData* getFluidConfig() const {return netInternalFluidConfig;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS fluid super network class unit tests.
///
/// @details  This class provides the unit tests for the GUNNS fluid super network class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSuperNetwork: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GUNNS network base class unit test.
        UtGunnsFluidSuperNetwork();
        /// @brief    Default destructs this GUNNS network base class unit test.
        virtual ~UtGunnsFluidSuperNetwork();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests finalizing the sub-networks and super-network nodes.
        void testRegisterSuperNodes();
        /// @brief    Tests initialize method with sub-networks.
        void testInitialize();
        /// @brief    Tests incorrect initialization and error handling.
        void testInitializeErrors();
        /// @brief    Tests sub-networks missing fluid config.
        void testMissingFluidConfig();
        /// @brief    Tests sub-networks having different number of fluid types.
        void testFluidConfigSize();
        /// @brief    Tests sub-networks having different fluid types.
        void testFluidConfigTypes();
        /// @brief    Tests some sub-networks missing trace compounds config.
        void testMissingFluidConfigTc();
        /// @brief    Tests sub-networks having different number of trace compounds.
        void testFluidConfigTcSize();
        /// @brief    Tests sub-networks having different trace compounds.
        void testFluidConfigTcTypes();
        /// @brief    Tests update method error handling.
        void testUpdateErrors();
        /// @brief    Tests update method with no sub-networks.
        void testUpdateEmpty();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidSuperNetwork);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testRegisterSuperNodes);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testInitializeErrors);
        CPPUNIT_TEST(testMissingFluidConfig);
        CPPUNIT_TEST(testFluidConfigSize);
        CPPUNIT_TEST(testFluidConfigTypes);
        CPPUNIT_TEST(testMissingFluidConfigTc);
        CPPUNIT_TEST(testFluidConfigTcSize);
        CPPUNIT_TEST(testFluidConfigTcTypes);
        CPPUNIT_TEST_SUITE_END();
        std::string                     tName;        /**< (--) Nominal name. */
        FriendlyGunnsFluidSuperNetwork* tArticle;     /**< (--) Pointer to the article under test. */
        FriendlyGunnsNetworkBase        tSubNetworkA; /**< (--) Network to be added as a sub-network. */
        FriendlyGunnsNetworkBase        tSubNetworkB; /**< (--) Network to be added as a sub-network. */
        static int                      TEST_ID;      /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSuperNetwork(const UtGunnsFluidSuperNetwork&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSuperNetwork& operator =(const UtGunnsFluidSuperNetwork&);
};

///@}

#endif
