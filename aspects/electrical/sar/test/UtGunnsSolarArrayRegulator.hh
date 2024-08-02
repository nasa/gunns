#ifndef UtGunnsSolarArrayRegulator_EXISTS
#define UtGunnsSolarArrayRegulator_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_ELECTRICAL_SOLAR_ARRAY_REGULATOR   Gunns Solar Array Regulator Unit Test
/// @ingroup  UT_GUNNS_ELECTRICAL
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Solar Array Regulator
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>
#include "aspects/electrical/sar/GunnsSolarArrayRegulator.hh"
#include "core/GunnsBasicNode.hh"
#include "core/GunnsBasicPotential.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsSolarArrayRegulator and befriend UtGunnsSolarArrayRegulator.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsSolarArrayRegulator : public GunnsSolarArrayRegulator
{
    public:
        FriendlyGunnsSolarArrayRegulator();
        virtual ~FriendlyGunnsSolarArrayRegulator();
        friend class UtGunnsSolarArrayRegulator;
};
inline FriendlyGunnsSolarArrayRegulator::FriendlyGunnsSolarArrayRegulator() : GunnsSolarArrayRegulator() {};
inline FriendlyGunnsSolarArrayRegulator::~FriendlyGunnsSolarArrayRegulator() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Solar Array Regulator  unit tests.
////
/// @details  This class provides the unit tests for the Gunns Solar Array Regulator within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsSolarArrayRegulator: public CppUnit::TestFixture
{
    public:
        enum Nodes
        {
            Node0 = 0,
            Node1 = 1,
            VACUUM = 2,
            N_NODES = 3
        };

        UtGunnsSolarArrayRegulator();
        virtual ~UtGunnsSolarArrayRegulator();
        void tearDown();
        void initPotentialVectorValues();
        void setUp();
        void batterySetup();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testStep();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsSolarArrayRegulator(const UtGunnsSolarArrayRegulator& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsSolarArrayRegulator& operator =(const UtGunnsSolarArrayRegulator& that);

        CPPUNIT_TEST_SUITE(UtGunnsSolarArrayRegulator);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testStep);

        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsSolarArrayRegulatorConfigData* tConfigData;

        /// --     Pointer to nominal input data
        GunnsSolarArrayRegulatorInputData*  tInputData;

        /// --     Test Article
        FriendlyGunnsSolarArrayRegulator*   tArticle;

        /// -- Conveter Link Name
        std::string tLinkName;

        /// -- Network Links
        std::vector<GunnsBasicLink*> tLinks;

        /// -- Network Nodes
        GunnsBasicNode tNodes[N_NODES];

        /// -- Network Node List
        GunnsNodeList tNodeList;

        /// -- Nominal inlet port index
        int tPort[4];

        /// (s) Nominal time step
        double tTimeStep;

        /// -- Nominal tolerance for comparison of expected and returned values
        double tTolerance;

        // Config data
        double tConverterOnConductance;
        double tConverterOffConductance;
        double tStandbyPower;
        double tNominalTrickleChargeRate;
        double tRegulatedVoltageLowLimit;
        double tRegulatedVoltageHighLimit;
        SensorAnalogConfigData* tOutVoltageSensorConfig;
        SensorAnalogConfigData* tOutCurrentSensorConfig;
        float  tOutVoltageUpperLimit;
        float  tOutCurrentUpperLimit;
        float  tOutVoltageNoiseScale;
        float  tOutCurrentNoiseScale;
        int    tTripPriority;

        // Input data
        double tmalfBlockageFlag;
        double tmalfBlockageValue;
        double tInputVoltage;
        double tRegulatedVoltage;
        double tEfficiency;
        double tOpOverCurrentLimit;
        double tOutputOverVoltageLimit;
        bool tOpOverCurrentTripActive;
        bool tOpOverVoltageTripActive;
        double tInputOverVoltageLimit;
        double tInputUnderVoltageLimit;
        bool tInOverVoltageTripActive;
        bool tInUnderVoltageTripActive;
        double tKp;
        double tKd;

        SensorAnalogInputData* tOutVoltageSensorInput;
        SensorAnalogInputData* tOutCurrentSensorInput;
        bool tMalfOpOverCurrentFlag;
        bool tMalfOpOverVoltageFlag;
        bool tMalfRegulatedVoltageFlag;

        // A test battery
        GunnsBasicPotentialConfigData* tBatteryConfig;
        GunnsBasicPotentialInputData* tBatteryInput;
        GunnsBasicPotential tBatteryLink;

};

///@}

#endif
