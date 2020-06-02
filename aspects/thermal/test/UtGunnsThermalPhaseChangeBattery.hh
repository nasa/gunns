#ifndef UtGunnsThermalPhaseChangeBattery_EXISTS
#define UtGunnsThermalPhaseChangeBattery_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup    UT_GUNNS_THERMAL_PHASE_CHANGE_BATTERY  GUNNS Thermal Phase Change Battery Unit Test
/// @ingroup     UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details     Unit Tests for GUNNS Thermal Phase Change Battery
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "aspects/thermal/GunnsThermalPhaseChangeBattery.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsThermalPhaseChangeBattery and befriend UtGunnsThermalPhaseChangeBattery.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsThermalPhaseChangeBattery : public GunnsThermalPhaseChangeBattery
{
    public:
        FriendlyGunnsThermalPhaseChangeBattery();
        virtual ~FriendlyGunnsThermalPhaseChangeBattery();
        friend class UtGunnsThermalPhaseChangeBattery;
};
inline FriendlyGunnsThermalPhaseChangeBattery::FriendlyGunnsThermalPhaseChangeBattery() : GunnsThermalPhaseChangeBattery() {};
inline FriendlyGunnsThermalPhaseChangeBattery::~FriendlyGunnsThermalPhaseChangeBattery() {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Thermal Phase Change Battery unit tests.
///
/// @details  This class provides unit tests for the Thermal Phase Change Battery within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsThermalPhaseChangeBattery: public CppUnit::TestFixture
{
    public:
        /// @brief    Default Constructor.
        UtGunnsThermalPhaseChangeBattery();
        /// @brief    Default Destructor.
        virtual ~UtGunnsThermalPhaseChangeBattery();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests ConfigData construction.
        void testConfig();
        /// @brief    Tests InputData  construction.
        void testInput();
        /// @brief    Tests Default Construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method without exceptions.
        void testInitialization();
        /// @brief    Tests initialize method with exceptions.
        void testInitializationExceptions();
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests step method.
        void testStep();
        /// @brief    Tests computeFlows method.
        void testComputeFlows();
        /// @brief    Tests the hot phase leak malfunction.
        void testLeakMalf();
        /// @brief    Tests accessor methods.
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsThermalPhaseChangeBattery);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testLeakMalf);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();
        GunnsThermalPhaseChangeBatteryConfigData* tConfigData;             /**< (--)    Pointer to nominal configuration data. */
        GunnsThermalPhaseChangeBatteryInputData*  tInputData;              /**< (--)    Pointer to nominal input data. */
        FriendlyGunnsThermalPhaseChangeBattery*   tArticle;                /**< (--)    Pointer to the test article. */
        std::string                               tName;                   /**< (--)    Link Name. */
        GunnsBasicNode                            tNodes[2];               /**< (--)    Network Nodes. */
        GunnsNodeList                             tNodeList;               /**< (--)    Node List. */
        std::vector<GunnsBasicLink*>              tLinks;                  /**< (--)    Network Links.  */
        double                                    tPhaseChangeTemperature; /**< (K)     Nominal config data. */
        double                                    tPhaseChangeHeat;        /**< (J/g)   Nominal config data. */
        double                                    tHotPhaseSpecificHeat;   /**< (J/g/K) Nominal config data. */
        double                                    tColdPhaseSpecificHeat;  /**< (J/g/K) Nominal config data. */
        double                                    tStructureCapacitance;   /**< (J/K)   Nominal config data. */
        double                                    tMass;                   /**< (kg)    Nominal input data. */
        double                                    tTemperature;            /**< (K)     Nominal input data. */
        double                                    tHotPhaseFraction;       /**< (--)    Nominal input data. */
        bool                                      tMalfHotPhaseLeakFlag;   /**< (--)    Nominal input data. */
        double                                    tMalfHotPhaseLeakRate;   /**< (kg/s)  Nominal input data. */
        int                                       tPort0;                  /**< (--)    Nominal inlet port index. */
        double                                    tTimeStep;               /**< (s)     Nominal time step. */
        static int                                TEST_ID;                 /**< (--)    Test identification number. */
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsThermalPhaseChangeBattery(const UtGunnsThermalPhaseChangeBattery& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsThermalPhaseChangeBattery& operator =(const UtGunnsThermalPhaseChangeBattery& that);
};

///@}

#endif
