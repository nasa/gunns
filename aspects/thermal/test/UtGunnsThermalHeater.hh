#ifndef UtGunnsThermalHeater_EXISTS
#define UtGunnsThermalHeater_EXISTS
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_THERMAL_HEATER    GunnsThermalHeater Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GunnsThermalHeater
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "aspects/thermal/GunnsThermalHeater.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsThermalHeater and befriend UtGunnsThermalHeater.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsThermalHeater : public GunnsThermalHeater
{
    public:
        FriendlyGunnsThermalHeater();
        virtual ~FriendlyGunnsThermalHeater();
        friend class UtGunnsThermalHeater;
};
inline FriendlyGunnsThermalHeater::FriendlyGunnsThermalHeater() : GunnsThermalHeater() {};
inline FriendlyGunnsThermalHeater::~FriendlyGunnsThermalHeater() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsThermalHeater  unit tests.
///
/// @details  This class provides unit tests for GunnsThermalHeater within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsThermalHeater: public CppUnit::TestFixture
{
    public:
        /// @brief    Default Constructor.
        UtGunnsThermalHeater();
        /// @brief    Default Destructor.
        virtual ~UtGunnsThermalHeater();

        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config data.
        void testConfig();
        /// @brief    Tests input data.
        void testInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests getters() and setters().
        void testGettersAndSetters();
        /// @brief    Tests initialize() method.
        void testInitialization();
        /// @brief    Tests updateState() method.
        void testUpdateState();
        /// @brief    Test the step() method.
        void testStep();
        /// @brief    Tests appropriate exceptions are thrown during validation.
        void testValidation();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsThermalHeater);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testGettersAndSetters);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testValidation);
        CPPUNIT_TEST_SUITE_END();

        std::string  tName;  /**< (--) Heater name */
        FriendlyGunnsThermalHeater*   tArticle1;    /**< (--) Test Article, single-port  */
        FriendlyGunnsThermalHeater*   tArticle3;    /**< (--) Test Article, triple-port  */
        GunnsThermalHeaterConfigData* tConfigData1; /**< (--) Pointer to nominal configuration data */
        GunnsThermalHeaterConfigData* tConfigData3; /**< (--) Pointer to nominal configuration data */
        GunnsThermalHeaterInputData*  tInputData;   /**< (--) Pointer to nominal Input data */

        double       tPower;   /**<  (W) A default power value */
        double tDefaultDemand; /**<  (W) Link's default heat flux value */
        double tTuningScalar;  /**<  (--) Heater's electrical-to-thermal tuning scalar */

        /// @brief  Defines the fraction of flux that is distributed to each port
        std::vector<double> tFluxDistributionFractions1; /**< (--) vector of size 1 */
        std::vector<double> tFluxDistributionFractions3; /**< (--) vector of size 3 */

        std::vector<int> tPorts1; /**< (--) Vector of port numbers for the single-port heater */
        std::vector<int> tPorts3; /**< (--) Vector of port numbers for the triple-port heater */

        bool    tMalfFluxOverrideFlag;  /**< (--) Flux override malfunction boolean */
        double  tMalfFluxOverrideValue; /**<  (W) Flux override malfunction wattage value */
        bool    tMalfBlockageFlag;  /**< (--) Blockage malfunction boolean */
        double  tMalfBlockageValue; /**< (--) Blockage malfunction fractional value (0-1) */

        GunnsBasicNode tNodes[6]; /**< (--) Network Nodes */
        GunnsNodeList  tNodeList; /**< (--) Node List */
        std::vector<GunnsBasicLink*> tLinks; /**< (--) Network Links */

        double tTimeStep;  /**< (s)  Nominal time step */
        double tTol;       /**< (--) Nominal tolerance for comparison of expected/returned values */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsThermalHeater(const UtGunnsThermalHeater& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsThermalHeater& operator =(const UtGunnsThermalHeater& that);
};
///@}

#endif   // UtGunnsThermalHeater_EXISTS
