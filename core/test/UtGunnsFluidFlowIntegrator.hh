#ifndef UtGunnsFluidFlowIntegrator_EXISTS
#define UtGunnsFluidFlowIntegrator_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_FLOW_INTEGRATOR    Gunns Fluid Flow Integrator Spotter Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Flow Integrator Spotter class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/GunnsFluidFlowIntegrator.hh"
#include "core/GunnsFluidNode.hh"
#include "core/GunnsFluidConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidFlowIntegrator and befriend UtGunnsFluidFlowIntegrator.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidFlowIntegrator : public GunnsFluidFlowIntegrator
{
    public:
        FriendlyGunnsFluidFlowIntegrator(const GunnsFluidLink& link)
            : GunnsFluidFlowIntegrator(link) {};
        virtual ~FriendlyGunnsFluidFlowIntegrator() {;}
        friend class UtGunnsFluidFlowIntegrator;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterConfigData.
///
/// @details  Derives from GunnsNetworkSpotterConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsFluidFlowIntegratorConfigData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsFluidFlowIntegratorConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        BadGunnsFluidFlowIntegratorConfigData(const std::string& name) : GunnsNetworkSpotterConfigData(name) {}
        virtual ~BadGunnsFluidFlowIntegratorConfigData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterInputData.
///
/// @details  Derives from GunnsNetworkSpotterInputData and is used to test that a dynamic_cast of
///           this type to the GunnsFluidFlowIntegratorInputData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsFluidFlowIntegratorInputData : public GunnsNetworkSpotterInputData
{
    public:
        BadGunnsFluidFlowIntegratorInputData() {}
        virtual ~BadGunnsFluidFlowIntegratorInputData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Flow Integrator Spotter Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsFluidFlowIntegrator class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidFlowIntegrator : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsFluidFlowIntegrator unit test.
        UtGunnsFluidFlowIntegrator();
        /// @brief    Default destructs this GunnsFluidFlowIntegrator unit test.
        virtual ~UtGunnsFluidFlowIntegrator();
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
        /// @brief    Tests the stepPreSolver method.
        void testPreSolver();
        /// @brief    Tests the stepPostSolver method.
        void testPostSolver();
        /// @brief    Tests the setter and getter methods.
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidFlowIntegrator);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testPreSolver);
        CPPUNIT_TEST(testPostSolver);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsFluidFlowIntegrator   tArticle;              /**< (--) Test article */
        GunnsFluidNode                     tNodes[3];             /**< (--) Network nodes */
        GunnsNodeList                      tNodeList;             /**< (--) Test node list */
        std::string                        tName;                 /**< (--) Instance name */
        GunnsFluidFlowIntegratorConfigData tConfig;               /**< (--) Nominal config data */
        GunnsFluidFlowIntegratorInputData  tInput;                /**< (--) Nominal input data */
        DefinedFluidProperties*            tFluidProperties;      /**< (--) Pre-defined fluid properties */
        PolyFluidConfigData*               tFluidConfig;          /**< (--) Fluid config data */
        PolyFluidInputData*                tFluidInput1;          /**< (--) Fluid 1 input data */
        PolyFluidInputData*                tFluidInput2;          /**< (--) Fluid 2 input data */
        double                             tMassFractions0[2];    /**< (--) Fluid mass fractions for node 0. */
        double                             tMassFractions1[2];    /**< (--) Fluid mass fractions for node 1. */
        std::vector<GunnsBasicLink*>       tLinks;                /**< (--) Test basic link vector. Needed to initalize the test link.*/
        GunnsFluidConductor                tConductorLink;        /**< (--) Test conductor link */
        GunnsFluidConductorConfigData*     tConductorLinkConfig;  /**< (--) Test conductor link */
        GunnsFluidConductorInputData*      tConductorLinkInput;   /**< (--) Test conductor link */
        double                             tMaxConductivity;      /**< (--) Max conductivity for test conductor link */
        double                             tExpansionScaleFactor; /**< (--) Expansion scale factor for test conductor link */
        int                                tPort0;                /**< (--) Port 0 for link initialization */
        int                                tPort1;                /**< (--) Port 1 for link initialization */
        double                             tTimeStep;             /**< (--) Time step size for this test */
        double                             tTolerance;            /**< (--) Comparison tolerance for floating pt tests */


        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidFlowIntegrator(const UtGunnsFluidFlowIntegrator& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidFlowIntegrator& operator =(const UtGunnsFluidFlowIntegrator& that);
};

///@}

#endif
