#ifndef UtGunnsThermalPanel_EXISTS
#define UtGunnsThermalPanel_EXISTS
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_THERMAL_PANEL    Gunns ThermalPanel Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns ThermalPanel
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "aspects/thermal/GunnsThermalPanel.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Inherit from GunnsThermalPanel and befriend UtGunnsThermalPanel.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsThermalPanel : public GunnsThermalPanel
{
    public:
        FriendlyGunnsThermalPanel();
        virtual ~FriendlyGunnsThermalPanel();
        friend class UtGunnsThermalPanel;
};
inline FriendlyGunnsThermalPanel::FriendlyGunnsThermalPanel() : GunnsThermalPanel() {};
inline FriendlyGunnsThermalPanel::~FriendlyGunnsThermalPanel() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Gunns ThermalPanel  unit tests.
////
/// @details  This class provides the unit tests for the ThermalPanel within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsThermalPanel: public CppUnit::TestFixture
{
    public:
        /// @brief  Default Constructor.
        UtGunnsThermalPanel();
        /// @brief  Default Destructor.
        virtual ~UtGunnsThermalPanel();

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

    private:
        CPPUNIT_TEST_SUITE(UtGunnsThermalPanel);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testValidation);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST_SUITE_END();

        std::string  tName;  /**< (--) Thermal panel link name */
        FriendlyGunnsThermalPanel*   tArticle1;    /**< (--) Test Article, single-port  */
        FriendlyGunnsThermalPanel*   tArticle3;    /**< (--) Test Article, triple-port  */
        GunnsThermalPanelConfigData* tConfigData1; /**< (--) Pointer to nominal configuration data */
        GunnsThermalPanelConfigData* tConfigData3; /**< (--) Pointer to nominal configuration data */
        GunnsThermalPanelInputData*  tInputData;   /**< (--) Pointer to nominal Input data */

        double tFlux;          /**<  (W) Flux value for testing */
        double tDefaultDemand; /**<  (W) Link's default heat flux value */
        double tTuningScalar;  /**<  (--) Source's electrical-to-thermal tuning scalar */

        double tIncidentHeatFluxPerArea; /**< (W/m2) test environmental incident flux */
        double tAbsorptivity;            /**<  (--)  test panel absorptivity (0-1) */
        double tSurfaceArea;             /**<  (m2)  test panel surface area */

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

        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtGunnsThermalPanel(const UtGunnsThermalPanel& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        UtGunnsThermalPanel& operator =(const UtGunnsThermalPanel& that);
};
///@}

#endif   // UtGunnsThermalPanel_EXISTS
