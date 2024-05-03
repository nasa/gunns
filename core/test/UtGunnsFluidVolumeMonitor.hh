#ifndef UtGunnsFluidVolumeMonitor_EXISTS
#define UtGunnsFluidVolumeMonitor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_VOLUME_MONITOR    GUNNS Fluid Volume Monitor Spotter Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Volume Monitor Spotter class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/GunnsFluidVolumeMonitor.hh"
#include "core/GunnsFluidNode.hh"
#include "core/GunnsFluidConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidVolumeMonitor and befriend UtGunnsFluidVolumeMonitor.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidVolumeMonitor : public GunnsFluidVolumeMonitor
{
    public:
        FriendlyGunnsFluidVolumeMonitor(const GunnsNodeList& nodeList, const GunnsFluidLink& link)
            : GunnsFluidVolumeMonitor(nodeList, link) {;}
        virtual ~FriendlyGunnsFluidVolumeMonitor() {;}
        friend class UtGunnsFluidVolumeMonitor;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterConfigData.
///
/// @details  Derives from GunnsNetworkSpotterConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsFluidVolumeMonitorConfigData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsFluidVolumeMonitorConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        BadGunnsFluidVolumeMonitorConfigData(const std::string& name) : GunnsNetworkSpotterConfigData(name) {}
        virtual ~BadGunnsFluidVolumeMonitorConfigData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterInputData.
///
/// @details  Derives from GunnsNetworkSpotterInputData and is used to test that a dynamic_cast of
///           this type to the GunnsFluidVolumeMonitorInputData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsFluidVolumeMonitorInputData : public GunnsNetworkSpotterInputData
{
    public:
        BadGunnsFluidVolumeMonitorInputData() {}
        virtual ~BadGunnsFluidVolumeMonitorInputData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Volume Monitor Spotter Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsFluidVolumeMonitor class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidVolumeMonitor : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsFluidVolumeMonitor unit test.
        UtGunnsFluidVolumeMonitor();
        /// @brief    Default destructs this GunnsFluidVolumeMonitor unit test.
        virtual ~UtGunnsFluidVolumeMonitor();
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
        /// @brief    Tests the setter and getter methods.
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidVolumeMonitor);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testInitializeExceptions);
        CPPUNIT_TEST(testPreSolver);
        CPPUNIT_TEST(testPostSolver);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsFluidVolumeMonitor*    tArticle;              /**< (--) Test article */
        GunnsFluidNode                      tNodes[3];             /**< (--) Network nodes */
        GunnsNodeList                       tNodeList;             /**< (--) Test node list */
        std::string                         tName;                 /**< (--) Instance name */
        GunnsFluidVolumeMonitorConfigData*  tConfig;               /**< (--) Nominal config data */
        GunnsFluidVolumeMonitorInputData*   tInput;                /**< (--) Nominal input data */
        int                                 tLinkPort;             /**< (--) Nominal input data */
        DefinedFluidProperties*             tFluidProperties;      /**< (--) Pre-defined fluid properties */
        PolyFluidConfigData*                tFluidConfig;          /**< (--) Fluid config data */
        PolyFluidConfigData*                tFluidConfig0;          /**< (--) Fluid config data */
        PolyFluidInputData*                 tFluidInput;           /**< (--) Fluid input data */
        double                              tMassFractions[2];     /**< (--) Fluid mass fractions. */
        std::vector<GunnsBasicLink*>        tLinks;                /**< (--) Test basic link vector. Needed to initialize the test link.*/
        GunnsFluidConductor                 tConductorLink;        /**< (--) Test conductor link */
        GunnsFluidConductorConfigData*      tConductorLinkConfig;  /**< (--) Test conductor link */
        GunnsFluidConductorInputData*       tConductorLinkInput;   /**< (--) Test conductor link */
        double                              tMaxConductivity;      /**< (--) Max conductivity for test conductor link */
        double                              tExpansionScaleFactor; /**< (--) Expansion scale factor for test conductor link */
        int                                 tPort0;                /**< (--) Port 0 for link initialization */
        int                                 tPort1;                /**< (--) Port 1 for link initialization */
        double                              tTimeStep;             /**< (--) Time step size for this test */
        GunnsFluidTraceCompoundsConfigData* tTcConfig;             /**< (--) Trace Compounds */
        GunnsFluidTraceCompoundsInputData*  tTcInput;              /**< (--) Trace Compounds */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidVolumeMonitor(const UtGunnsFluidVolumeMonitor& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidVolumeMonitor& operator =(const UtGunnsFluidVolumeMonitor& that);
};

///@}

#endif
