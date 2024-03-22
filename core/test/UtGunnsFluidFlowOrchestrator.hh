#ifndef UtGunnsFluidFlowOrchestrator_EXISTS
#define UtGunnsFluidFlowOrchestrator_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_FLOW_ORCH    GUNNS Fluid Flow Orchestrator Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2024 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Flow Orchestrator class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/GunnsFluidFlowOrchestrator.hh"
#include "core/GunnsFluidSource.hh"
#include "core/GunnsFluidCapacitor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidFlowOrchestrator and befriend UtGunnsFluidFlowOrchestrator.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidFlowOrchestrator : public GunnsFluidFlowOrchestrator
{
    public:
        bool triggerAbort;        /**< (--) Flag to force test article to abort an infinite loop. */
        FriendlyGunnsFluidFlowOrchestrator(const int& numLinks, const int& numNodes)
            : GunnsFluidFlowOrchestrator(numLinks, numNodes), triggerAbort(false) {};
        virtual ~FriendlyGunnsFluidFlowOrchestrator() {;}
        friend class UtGunnsFluidFlowOrchestrator;

    protected:
        /// @brief  This forces the orchestrator to abort to escape an infinite loop.
        virtual bool checkAllNodesComplete() const {
            if (triggerAbort) {
                return false;
            } else {
                return GunnsFluidFlowOrchestrator::checkAllNodesComplete();
            }
        }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief     GUNNS Fluid Flow Orchestrator Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsFluidFlowOrchestrator class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidFlowOrchestrator : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsFluidFlowOrchestrator unit test.
        UtGunnsFluidFlowOrchestrator();
        /// @brief    Default destructs this GunnsFluidFlowOrchestrator unit test.
        virtual ~UtGunnsFluidFlowOrchestrator();
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
        /// @brief    Tests update method with no link flows.
        void testUpdateNoFlow();
        /// @brief    Tests update method with a nominal flow case.
        void testUpdateNominal();
        /// @brief    Tests update method with a overflow loop case.
        void testUpdateOverflowLoop();
        /// @brief    Tests update method with an infinite loop escape case.
        void testUpdateAbort();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidFlowOrchestrator);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testInitializeExceptions);
        CPPUNIT_TEST(testUpdateNoFlow);
        CPPUNIT_TEST(testUpdateNominal);
        CPPUNIT_TEST(testUpdateOverflowLoop);
        CPPUNIT_TEST(testUpdateAbort);
        CPPUNIT_TEST_SUITE_END();

        enum {NUMLINKS = 5, NUMNODES = 4};                   /**< (--) Enumeration of numbers of objects */
        int                                tNumLinks;        /**< (--) Number of links */
        int                                tNumNodes;        /**< (--) Number of nodes */
        FriendlyGunnsFluidFlowOrchestrator tArticle;         /**< (--) Test article */
        GunnsBasicLink**                   tLinksArray;      /**< (--) Array of link pointers */
        GunnsBasicNode**                   tNodesArray;      /**< (--) Array of node pointers */
        GunnsFluidSource                   tSource1;         /**< (--) Test referenced network link */
        GunnsFluidSource                   tSource2;         /**< (--) Test referenced network link */
        GunnsFluidSource                   tSource3;         /**< (--) Test referenced network link */
        GunnsFluidCapacitor                tCapacitor1;      /**< (--) Test referenced network link */
        GunnsFluidCapacitor                tCapacitor2;      /**< (--) Test referenced network link */
        GunnsFluidNode                     tNodes[NUMNODES]; /**< (--) Test referenced network link */
        std::string                        tName;            /**< (--) Instance name */
        GunnsNodeList                      tNetNodeList;     /**< (--) Network nodes list */
        std::vector<GunnsBasicLink*>       tNetLinks;        /**< (--) Network links vector */
        DefinedFluidProperties*            tFluidProperties; /**< (--) Fluid properties */
        PolyFluidConfigData*               tFluidConfig;     /**< (--) Fluid config data */
        PolyFluidInputData*                tFluidInput;      /**< (--) Fluid input data */
        double*                            tFractions;       /**< (--) Fluid mass fractions */
        GunnsFluidSourceConfigData*        tSourceConfig;    /**< (--) Source links config data */
        GunnsFluidSourceInputData*         tSourceInput;     /**< (--) Source links input data */
        GunnsFluidCapacitorConfigData*     tCapacitorConfig; /**< (--) Capacitor links config data */
        GunnsFluidCapacitorInputData*      tCapacitorInput;  /**< (--) Capacitor links input data */
        int**                              tLinkNodeMaps;    /**< (--) Array of link node maps */
        int*                               tLinkNumPorts;    /**< (--) Array of link number of ports */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidFlowOrchestrator(const UtGunnsFluidFlowOrchestrator& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidFlowOrchestrator& operator =(const UtGunnsFluidFlowOrchestrator& that);
};

///@}

#endif
