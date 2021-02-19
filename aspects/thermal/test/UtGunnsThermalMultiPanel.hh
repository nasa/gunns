#ifndef UtGunnsThermalMultiPanel_EXISTS
#define UtGunnsThermalMultiPanel_EXISTS
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_THERMAL_PANEL    GUNNS Thermal Multi-Panel Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Thermal Multi-Panel
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "aspects/thermal/GunnsThermalMultiPanel.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Inherit from GunnsThermalMultiPanel and befriend UtGunnsThermalMultiPanel.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsThermalMultiPanel : public GunnsThermalMultiPanel
{
    public:
        FriendlyGunnsThermalMultiPanel();
        virtual ~FriendlyGunnsThermalMultiPanel();
        friend class UtGunnsThermalMultiPanel;
};
inline FriendlyGunnsThermalMultiPanel::FriendlyGunnsThermalMultiPanel() : GunnsThermalMultiPanel() {};
inline FriendlyGunnsThermalMultiPanel::~FriendlyGunnsThermalMultiPanel() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   GUNNS Thermal Multi-Panel  unit tests.
////
/// @details  This class provides the unit tests for the Thermal Multi-Panel within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsThermalMultiPanel: public CppUnit::TestFixture
{
    public:
        /// @brief  Default Constructor.
        UtGunnsThermalMultiPanel();
        /// @brief  Default Destructor.
        virtual ~UtGunnsThermalMultiPanel();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Tests ConfigData construction.
        void testConfig();
        /// @brief  Tests InputData construction.
        void testInput();
        /// @brief  Tests default construction.
        void testDefaultConstruction();
        /// @brief  Tests initialize() method.
        void testNominalInitialization();
        /// @brief  Tests appropriate exceptions are thrown during validation.
        void testValidation();
        /// @brief  Tests updateState() method, called within BasicLink::step().
        void testUpdateState();
        /// @brief  Test the step() method.
        void testStep();
        /// @brief  Test the restart() method.
        void testRestart();
        /// @brief    Tests accessor methods.
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsThermalMultiPanel);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testValidation);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        std::string  tName;  /**< (--) Test Article name */
        FriendlyGunnsThermalMultiPanel*   tArticle1;    /**< (--) Test Article, single-port  */
        FriendlyGunnsThermalMultiPanel*   tArticle3;    /**< (--) Test Article, triple-port  */
        GunnsThermalMultiPanelConfigData* tConfigData1; /**< (--) Pointer to nominal configuration data */
        GunnsThermalMultiPanelConfigData* tConfigData3; /**< (--) Pointer to nominal configuration data */
        GunnsThermalMultiPanelInputData*  tInputData;   /**< (--) Pointer to nominal Input data */

        double tFlux;          /**<  (W) Flux value for testing */
        double tDefaultDemand; /**<  (W) Link's default heat flux value */
        double tTuningScalar;  /**<  (--) Source's electrical-to-thermal tuning scalar */

        double tIncidentFlux[5];         /**< (W/m2) test environmental incident flux */
        double tViewScalar[5];           /**< (--)   test view scalar  */
        double tAbsorptivity[5];         /**< (--)   test panel absorptivity (0-1) */
        double tSurfaceArea;             /**< (m2)   test panel surface area */

        /// @brief  Defines the fraction of flux that is distributed to each port
        std::vector<double> tFluxDistributionFractions1; /**< (--) vector of size 1 */
        std::vector<double> tFluxDistributionFractions3; /**< (--) vector of size 3 */

        std::vector<int> tPorts1; /**< (--) Vector of port numbers for the single-port source */
        std::vector<int> tPorts3; /**< (--) Vector of port numbers for the triple-port source */

        bool    tMalfFluxOverrideFlag;  /**< (--) Flux override malfunction boolean */
        double  tMalfFluxOverrideValue; /**<  (W) Flux override malfunction wattage value */
        bool    tMalfBlockageFlag;  /**< (--) Blockage malfunction boolean */
        double  tMalfBlockageValue; /**< (--) Blockage malfunction fractional value (0-1) */

        GunnsBasicNode               tNodes[6]; /**< (--) Network Nodes */
        GunnsNodeList                tNodeList; /**< (--) Node List */
        std::vector<GunnsBasicLink*> tLinks;    /**< (--) Network Links */

        double tTimeStep;  /**< (s)  Nominal time step */
        double tTol;       /**< (--) Nominal tolerance for comparison of expected/returned values */

        /// @brief  Sets test aritcle's incident fluxes to test values.
        void setupIncidentFluxes();

        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtGunnsThermalMultiPanel(const UtGunnsThermalMultiPanel& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        UtGunnsThermalMultiPanel& operator =(const UtGunnsThermalMultiPanel& that);
};
///@}

#endif   // UtGunnsThermalMultiPanel_EXISTS
