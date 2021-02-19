#ifndef UtThermalNetwork_EXISTS
#define UtThermalNetwork_EXISTS
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_PTCS_THERMAL_NETWORK    Thermal Network Unit Test
/// @ingroup  UT_PTCS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the ThermalNetwork class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "aspects/thermal/network/ThermalNetwork.hh"
#include "UtThermFileParser.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from ThermalNetwork and befriend UtThermalNetwork.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyThermalNetwork : public ThermalNetwork
{
    public:
        FriendlyThermalNetwork(const std::string name = "");
        virtual ~FriendlyThermalNetwork();
        friend class UtThermalNetwork;
};
inline FriendlyThermalNetwork::FriendlyThermalNetwork(const std::string name) : ThermalNetwork(name) {};
inline FriendlyThermalNetwork::~FriendlyThermalNetwork() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from Gunns and befriend UtThermalNetwork.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsSolver : public Gunns
{
    public:
    FriendlyGunnsSolver();
        virtual ~FriendlyGunnsSolver();
        friend class UtThermalNetwork;
};
inline FriendlyGunnsSolver::FriendlyGunnsSolver() : Gunns() {};
inline FriendlyGunnsSolver::~FriendlyGunnsSolver() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    ThermalNetwork  unit tests.
///
/// @details  Provides the unit tests for the ThermalNetwork within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtThermalNetwork: public CppUnit::TestFixture
{
    public:
        /// @brief  Default Constructor.
        UtThermalNetwork();
        /// @brief  Default Destructor.
        virtual ~UtThermalNetwork();

        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Tests for appropriate configuration data.
        void testConfig();
        /// @brief  Tests for appropriate input data.
        void testInput();
        /// @brief  Tests default construction.
        void testNominalConstruction();
        /// @brief  Tests initialize() method.
        void testInitialize();
        /// @brief  Tests buildConfig() method.
        void testConfigBuild();
        /// @brief  Tests buildConfig() method.
        void testInputBuild();
        /// @brief  Tests buildLinks() method.
        void testLinksBuild();
        /// @brief  Tests validate() method.
        void testValidate();
        /// @brief  Tests setHeaterPowers() method.
        void testSetHeaterPowers();
        /// @brief  Tests update() method.
        void testUpdate();
        /// @brief  Tests access to link members.
        void testAccess();
        /// @brief  Tests the capacitor link group edits.
        void testCapacitanceEdit();
        /// @brief  Tests the network in a super-network.
        void testSuperNetwork();

    private:
        CPPUNIT_TEST_SUITE(UtThermalNetwork);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testNominalConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testConfigBuild);
        CPPUNIT_TEST(testInputBuild);
        CPPUNIT_TEST(testLinksBuild);
        CPPUNIT_TEST(testValidate);
        CPPUNIT_TEST(testSetHeaterPowers);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testAccess);
        CPPUNIT_TEST(testCapacitanceEdit);
        CPPUNIT_TEST(testSuperNetwork);
        CPPUNIT_TEST_SUITE_END();

        FriendlyThermalNetwork*  tArticle; /**< (--)  pointer to test article */
        std::string tName; /**< (--)  name of test object */

        /// @details  Since the test will need access to the parser's data vectors, this class
        ///           contains a FriendlyThermFileParser, which also befriends the
        ///           UtThermalNetwork class. The parserFriendly will be loaded with the same
        ///           thermal config-files as the test article, so that the vectors can be
        ///           accessed and a proper comparison can be tested.
        FriendlyThermFileParser parserFriendly;

        std::string tNodeFile; /**< (--)  name of node config-file to parse */
        std::string tCondFile; /**< (--)  name of conduction config-file to parse */
        std::string tRadFile;  /**< (--)  name of radiation config-file to parse */
        std::string tHtrFile;  /**< (--)  name of heater config-file to parse */
        std::string tPanFile;  /**< (--)  name of panel config-file to parse */
        std::string tEtcFile;  /**< (--)  name of et.cetera config-file to parse */

        std::string tThermInputFile;  /**< (--)  name of thermal input-file to parse */

        GunnsConfigData tNetworkConfig; /**< (--)  Network config data needed for initialization */
        GunnsBasicNode* tNodes;         /**< (--)  network's pointer to BasicNode objects */
        int tNumberNodes;               /**< (--)  nmber of nodes listed in test case */

        /// - Declare the network link arrays.
        GunnsThermalCapacitor* tCapacitanceLinks; /**< (--)  Capacitance link array */
        GunnsBasicConductor*   tConductionLinks;  /**< (--)  Conduction link array  */
        GunnsThermalRadiation* tRadiationLinks;   /**< (--)  Radiation link array   */
        GunnsThermalHeater*    tHeaters;          /**< (--)  Heater link array      */
        GunnsThermalPanel*     tPanels;           /**< (--)  Panel link array       */
        GunnsThermalPotential* tPotentialLinks;   /**< (--)  Potential link array   */
        GunnsThermalSource*    tSources;          /**< (--)  Source link array      */

        /// - Declare the pointers to the Config data arrays
        GunnsThermalCapacitorConfigData** tCapacitanceConfigData; /**< (--) Capacitance config array */
        GunnsBasicConductorConfigData**   tConductionConfigData;  /**< (--) Conduction config array  */
        GunnsThermalRadiationConfigData** tRadiationConfigData;   /**< (--) Radiation config array   */
        GunnsThermalHeaterConfigData**    tHeaterConfigData;      /**< (--) Heater config array      */
        GunnsThermalPanelConfigData**     tPanelConfigData;       /**< (--) Panel config array       */
        GunnsThermalPotentialConfigData** tPotentialConfigData;   /**< (--) Potential config array   */
        GunnsThermalSourceConfigData**    tSourceConfigData;      /**< (--) Source config array      */

        /// - Declare the pointers to the Input data arrays
        GunnsThermalCapacitorInputData** tCapacitanceInputData; /**< (--) Capacitance input array */
        GunnsBasicConductorInputData**   tConductionInputData;  /**< (--) Conduction input array  */
        GunnsThermalRadiationInputData** tRadiationInputData;   /**< (--) Radiation input array   */
        GunnsThermalHeaterInputData**    tHeaterInputData;      /**< (--) Heater input array      */
        GunnsThermalPanelInputData**     tPanelInputData;       /**< (--) Panel input array       */
        GunnsThermalPotentialInputData** tPotentialInputData;   /**< (--) Potential input array   */
        GunnsThermalSourceInputData**    tSourceInputData;      /**< (--) Source input array      */

        double tTimeStep; /**<  (s)   nominal time step */
        double tTolerance; /**< (--)  tolerance for comparison of expected & returned values */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtThermalNetwork(const UtThermalNetwork& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtThermalNetwork& operator =(const UtThermalNetwork& that);
};
///@}
#endif /* UtThermalNetwork_EXISTS */
