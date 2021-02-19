#ifndef UtGunnsMultiLinkConnector_EXISTS
#define UtGunnsMultiLinkConnector_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_MULTI_LINKS_CONNECTOR    GUNNS Multiple Links Connector Spotter Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Multiple Links Connector Spotter class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/network/GunnsMultiLinkConnector.hh"
#include "core/GunnsBasicConductor.hh"
#include "UtGunnsNetworkBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsMultiLinkConnector and befriend UtGunnsMultiLinkConnector.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsMultiLinkConnector : public GunnsMultiLinkConnector
{
    public:
        FriendlyGunnsMultiLinkConnector(GunnsNetworkBase* network, const int jointType = 0)
            : GunnsMultiLinkConnector(network, jointType) {;}
        virtual ~FriendlyGunnsMultiLinkConnector() {;}
        friend class UtGunnsMultiLinkConnector;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterConfigData.
///
/// @details  Derives from GunnsNetworkSpotterConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsMultiLinkConnectorConfigData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsMultiLinkConnectorConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        BadGunnsMultiLinkConnectorConfigData(const std::string& name) : GunnsNetworkSpotterConfigData(name) {}
        virtual ~BadGunnsMultiLinkConnectorConfigData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterInputData.
///
/// @details  Derives from GunnsNetworkSpotterInputData and is used to test that a dynamic_cast of
///           this type to the GunnsMultiLinkConnectorInputData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsMultiLinkConnectorInputData : public GunnsNetworkSpotterInputData
{
    public:
        BadGunnsMultiLinkConnectorInputData() {}
        virtual ~BadGunnsMultiLinkConnectorInputData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Multiple Links Connector Spotter Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsMultiLinkConnector class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsMultiLinkConnector : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsMultiLinkConnector unit test.
        UtGunnsMultiLinkConnector();
        /// @brief    Default destructs this GunnsMultiLinkConnector unit test.
        virtual ~UtGunnsMultiLinkConnector();
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
        /// @brief    Tests initialization exceptions.
        void testInitializeExceptions();
        /// @brief    Tests the stepPreSolver method.
        void testPreSolver();
        /// @brief    Tests the stepPostSolver method.
        void testPostSolver();
        /// @brief    Tests the addConnection method.
        void testAddConnections();
        /// @brief    Tests the connect method.
        void testConnect();
        /// @brief    Tests methods related to the joint network.
        void testJointIndex();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsMultiLinkConnector);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testInitializeExceptions);
        CPPUNIT_TEST(testPreSolver);
        CPPUNIT_TEST(testPostSolver);
        CPPUNIT_TEST(testAddConnections);
        CPPUNIT_TEST(testConnect);
        CPPUNIT_TEST(testJointIndex);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsMultiLinkConnector*   tArticle;              /**< (--) Test article */
        FriendlyGunnsNetworkBase           tNetwork;              /**< (--) Network */
        GunnsBasicNode                     tNodes[3];             /**< (--) Network nodes */
        GunnsNodeList                      tNodeList;             /**< (--) Network node list */
        std::string                        tName;                 /**< (--) Instance name */
        int                                tJointType;            /**< (--) Nominal config data */
        GunnsMultiLinkConnectorConfigData* tConfig;               /**< (--) Nominal config data */
        GunnsBasicConductor                tLink1;                /**< (--) A GUNNS link */
        int                                tPort1;                /**< (--) Nominal input data */
        int                                tNode1;                /**< (--) Nominal input data */
        GunnsBasicConductor                tLink2;                /**< (--) A GUNNS link */
        int                                tPort2;                /**< (--) Nominal input data */
        int                                tNode2;                /**< (--) Nominal input data */
        GunnsMultiLinkConnectorInputData*  tInput1;               /**< (--) Nominal input data */
        GunnsMultiLinkConnectorInputData*  tInput2;               /**< (--) Nominal input data */
        static int                         TEST_ID;               /**< (--) Test identification number. */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsMultiLinkConnector(const UtGunnsMultiLinkConnector& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsMultiLinkConnector& operator =(const UtGunnsMultiLinkConnector& that);
};

///@}

#endif
