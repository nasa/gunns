#ifndef UtGunnsBasicFlowOrchestrator_EXISTS
#define UtGunnsBasicFlowOrchestrator_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_BASIC_FLOW_ORCH    GUNNS Basic Flow Orchestrator Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2024 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Basic Flow Orchestrator class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/GunnsBasicFlowOrchestrator.hh"
#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicFlowOrchestrator and befriend UtGunnsBasicFlowOrchestrator.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicFlowOrchestrator : public GunnsBasicFlowOrchestrator
{
    public:
        FriendlyGunnsBasicFlowOrchestrator(const int& numLinks, const int& numNodes) : GunnsBasicFlowOrchestrator(numLinks, numNodes) {};
        virtual ~FriendlyGunnsBasicFlowOrchestrator() {;}
        friend class UtGunnsBasicFlowOrchestrator;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief     GUNNS Basic Flow Orchestrator Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsBasicFlowOrchestrator class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicFlowOrchestrator : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsBasicFlowOrchestrator unit test.
        UtGunnsBasicFlowOrchestrator();
        /// @brief    Default destructs this GunnsBasicFlowOrchestrator unit test.
        virtual ~UtGunnsBasicFlowOrchestrator();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests default constructors.
        void testDefaultConstruction();
        /// @brief    Tests initialization.
        void testInitialize();
        /// @brief    Tests initialization exceptions.
        void testInitializeExceptions();
        /// @brief    Tests update method.
        void testUpdate();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsBasicFlowOrchestrator);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testInitializeExceptions);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST_SUITE_END();

        enum {NUMLINKS = 2, NUMNODES = 3};                        /**< (--) Enumeration of numbers of objects */
        int                                tNumLinks;             /**< (--) Number of links */
        int                                tNumNodes;             /**< (--) Number of nodes */
        FriendlyGunnsBasicFlowOrchestrator tArticle;              /**< (--) Test article */
        GunnsBasicLink**                   tLinksArray;           /**< (--) Array of link pointers */
        GunnsBasicNode**                   tNodesArray;           /**< (--) Array of node pointers */
        GunnsBasicConductor                tConductors[NUMLINKS]; /**< (--) Test referenced network link */
        GunnsBasicNode                     tNodes[NUMNODES];      /**< (--) Test referenced network link */
        int**                              tLinkNodeMaps;         /**< (--) Array of link node maps */
        int*                               tLinkNumPorts;         /**< (--) Array of link number of ports */
        std::string                        tName;                 /**< (--) Instance name */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsBasicFlowOrchestrator(const UtGunnsBasicFlowOrchestrator& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsBasicFlowOrchestrator& operator =(const UtGunnsBasicFlowOrchestrator& that);
};

///@}

#endif
