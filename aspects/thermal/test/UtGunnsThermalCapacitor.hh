#ifndef UtGunnsThermalCapacitor_EXISTS
#define UtGunnsThermalCapacitor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_THERMAL_CAPACITOR  Gunns Thermal Capacitor Unit test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Thermal Capacitor
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "aspects/thermal/GunnsThermalCapacitor.hh"

#include "core/Gunns.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsThermalCapacitor and befriend UtGunnsThermalCapacitor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsThermalCapacitor : public GunnsThermalCapacitor
{
    public:
        FriendlyGunnsThermalCapacitor();
        virtual ~FriendlyGunnsThermalCapacitor();
        friend class UtGunnsThermalCapacitor;
        friend class UtGunnsThermalCapacitorHeatQueues;
};
inline FriendlyGunnsThermalCapacitor::FriendlyGunnsThermalCapacitor() : GunnsThermalCapacitor() {};
inline FriendlyGunnsThermalCapacitor::~FriendlyGunnsThermalCapacitor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Thermal Capacitor  unit tests.
////
/// @details  This class provides unit tests for the Thermal Capacitor within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsThermalCapacitor: public CppUnit::TestFixture
{
    public:
        /// @brief  Default Constructor.
        UtGunnsThermalCapacitor();
        /// @brief  Default Destructor.
        virtual ~UtGunnsThermalCapacitor();
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
        void testInitialization();
        /// @brief  Tests appropriate exceptions are thrown during validation.
        void testValidation();
        /// @brief  Tests restart() method.
        void testRestart();
        /// @brief  Tests link update via the updateState() method, called within BasicLink::step().
        void testUpdateState();
        /// @brief  Tests link update via a call from Gunns::step() method.
        void testNetworkStep();
        /// @brief  Tests buildCapacitance() method, called within BasicCapacitor::step().
        void testBuildCapacitance();
        /// @brief  Tests correct recording of mTemperature from the link's mPotentialVector.
        void testTemperatureRecord();
        /// @brief  Tests correct overriding of capacitor's mPotentialVector.
        void testTemperatureOverrride();
        /// @brief  Tests setter and getter methods. */
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsThermalCapacitor);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testValidation);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testNetworkStep);
        CPPUNIT_TEST(testBuildCapacitance);
        CPPUNIT_TEST(testTemperatureRecord);
        CPPUNIT_TEST(testTemperatureOverrride);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        GunnsThermalCapacitorConfigData* tConfigData; /**< (--)   Pointer to nominal configuration data */
        GunnsThermalCapacitorInputData*  tInputData;  /**< (--)   Pointer to nominal input data */
        FriendlyGunnsThermalCapacitor*   tArticle;    /**< (--)   Test Article */

        int     tEditCapacitanceGroup; /**< (--)   Link edit capacitance group */
        double  tDefaultCapacitance;   /**< (--)   Link Capacitance */
        double  tInitialPotential;     /**< (--)   Link Initial Potential (Temperature) */

        double  tTemperatureOverride; /**< (K)  Override temperature to which the capacitor will be held. */
        double  tLargeHeatFlux;       /**< (W)  Large magnitude heat flux, for use in Link's mExternalHeatFlux[] array */
        double  tSmallHeatFlux;       /**< (W)  Small magnitude heat flux, for use in Link's mExternalHeatFlux[] array */

        std::string  tName;         /**< (--)   Conductor Name */
        bool    tMalfBlockageFlag;  /**< (--)   Blockage malfunction boolean */
        double  tMalfBlockageValue; /**< (--)   Blockage malfunction fractional value (0-1) */

        Gunns tGunnsSolver; /**< (--) Test Network's General-Use Nodal Network Solver (GUNNS) */
        std::vector<GunnsBasicLink*>  tLinks; /**< (--)  Test Network Links */
        GunnsBasicNode tNodes[2];             /**< (--)  Test Network Nodes */
        GunnsNodeList  tNodeList;             /**< (--)  Test Node List */
        int tPort0;                           /**< (--)  Nominal inlet port index */
        int tPort1;                           /**< (--)  Nominal outlet port index */

        double  tTimeStep; /**< (--)   Nominal time step */
        double  tTolerance;/**< (--)   Nominal tolerance for comparison of expected and returned values */

        /// @brief  Tests link update, generic for a solver in both regular or dummy mode.
        void testLinkUpdate(void (UtGunnsThermalCapacitor::*pf) () );
        /// @brief  Updates the capacitor via a call to updateState().
        void callUpdateState();
        /// @brief  Updates the capacitor via a call to Gunns::step().
        void callGunnsStep();

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsThermalCapacitor(const UtGunnsThermalCapacitor& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsThermalCapacitor& operator =(const UtGunnsThermalCapacitor& that);
};

///@}

#endif
