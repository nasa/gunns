#ifndef UtGunnsThermalSource_EXISTS
#define UtGunnsThermalSource_EXISTS
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_THERMAL_SOURCE   GunnsThermalSource Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GunnsThermalSource
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "aspects/thermal/GunnsThermalSource.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Inherit from GunnsThermalSource and befriend UtGunnsThermalSource.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsThermalSource : public GunnsThermalSource
{
    public:
        FriendlyGunnsThermalSource();
        virtual ~FriendlyGunnsThermalSource();
        friend class UtGunnsThermalSource;
};
inline FriendlyGunnsThermalSource::FriendlyGunnsThermalSource() : GunnsThermalSource() {};
inline FriendlyGunnsThermalSource::~FriendlyGunnsThermalSource() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  GunnsThermalSource  unit tests.
///
/// @details  This class provides unit tests for GunnsThermalSource within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsThermalSource: public CppUnit::TestFixture
{
    public:
        /// @brief  Default Constructor.
        UtGunnsThermalSource();
        /// @brief  Default Destructor.
        virtual ~UtGunnsThermalSource();

        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Tests config data.
        void testConfig();
        /// @brief  Tests overrideFluxDistributionFractions() method.
        void testOverrideFluxDistributionFractions();
        /// @brief  Tests input data.
        void testInput();
        /// @brief  Tests default construction.
        void testDefaultConstruction();
        /// @brief  Tests initialize() method.
        void testInitialization();
        /// @brief  Tests buildSourceVector() method.
        void testBuildSourceVector();
        /// @brief  Test the computeFlows() method.
        void testComputeFlows();
        /// @brief  Test the step() method.
        void testStep();
        /// @brief  Tests appropriate exceptions are thrown during validation.
        void testValidation();
        /// @brief  Test that the port rules check returns the correct booleans.
        void testSpecificPortRules();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsThermalSource);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testOverrideFluxDistributionFractions);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testBuildSourceVector);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testValidation);
        CPPUNIT_TEST(testSpecificPortRules);
        CPPUNIT_TEST_SUITE_END();

        std::string  tName;  /**< (--) Source name */
        FriendlyGunnsThermalSource*   tArticle1;    /**< (--) Test Article, single-port  */
        FriendlyGunnsThermalSource*   tArticle3;    /**< (--) Test Article, triple-port  */
        GunnsThermalSourceConfigData* tConfigData1; /**< (--) Pointer to nominal configuration data */
        GunnsThermalSourceConfigData* tConfigData3; /**< (--) Pointer to nominal configuration data */
        GunnsThermalSourceInputData*  tInputData;   /**< (--) Pointer to nominal Input data */

        double tFlux;          /**<  (W) Flux value for testing */
        double tDefaultDemand; /**<  (W) Link's default heat flux value */
        double tTuningScalar;  /**<  (--) Source's electrical-to-thermal tuning scalar */

        /// @brief  Defines the fraction of flux that is distributed to each port
        std::vector<double> tFluxDistributionFractions1; /**< (--) vector of size 1 */
        std::vector<double> tFluxDistributionFractions3; /**< (--) vector of size 3 */

        double tPort0Temperature;   /**< (K) temperature of node at port 0 */
        double tPort1Temperature;   /**< (K) temperature of node at port 1 */
        double tPort2Temperature;   /**< (K) temperature of node at port 2 */
        double tAvgPortTemperature; /**< (K) Average temperature of ports */

        std::vector<int> tPorts1; /**< (--) Vector of port numbers for the single-port source */
        std::vector<int> tPorts3; /**< (--) Vector of port numbers for the triple-port source */

        bool    tMalfFluxOverrideFlag;  /**< (--) Flux override malfunction boolean */
        double  tMalfFluxOverrideValue; /**<  (W) Flux override malfunction wattage value */
        bool    tMalfBlockageFlag;  /**< (--) Blockage malfunction boolean */
        double  tMalfBlockageValue; /**< (--) Blockage malfunction fractional value (0-1) */

        GunnsBasicNode tNodes[6]; /**< (--) Network Nodes */
        GunnsNodeList  tNodeList; /**< (--) Node List */
        std::vector<GunnsBasicLink*> tLinks; /**< (--) Network Links */

        double tTimeStep;  /**< (s)  Nominal time step */
        double tTol;       /**< (--) Nominal tolerance for comparison of expected/returned values */

        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtGunnsThermalSource(const UtGunnsThermalSource& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        UtGunnsThermalSource& operator =(const UtGunnsThermalSource& that);
};
///@}

#endif   // UtGunnsThermalSource_EXISTS
