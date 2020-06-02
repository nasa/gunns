#ifndef UtConverterElect_EXISTS
#define UtConverterElect_EXISTS
/***********************************************************************
 *
@defgroup  UT_TSM_GUNNS_ELECTRICAL_CONVERTER_ELECT_UNIT_TEST  Converter Electrical Unit Test
@ingroup   TSM_GUNNS_ELECTRICAL_CONVERTER_ELECTRICAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details  Unit Tests for the Converter Electrical Model.
 *
 *
LIBRARY DEPENDENCY:
 (
 (UtConverterElect.o)
 )
PROGRAMMERS:
 (
 (Shailaja Janapati) (L-3 Comm) (June 2013) (Initial Prototype)
 )
/// @{
********************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>
#include <string>

#include "aspects/electrical/Converter/ConverterElect.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from ConverterElect and befriend UtConverterElect.
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////

class FriendlyConverterElect: public ConverterElect {
 public:
    FriendlyConverterElect();
    virtual ~FriendlyConverterElect();
    friend class UtConverterElect;
};

inline FriendlyConverterElect::FriendlyConverterElect()
    :
    ConverterElect() {
};

inline FriendlyConverterElect::~FriendlyConverterElect() {
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    ConverterElect unit tests.
/// @details  This class provides the unit tests for the ConverterElect within the CPPunit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtConverterElect: public CppUnit::TestFixture {

public:
    enum Nodes
    {
        Node0 = 0,
        Node1 = 1,
        Node2 = 2,
        VACUUM = 3,
        N_NODES = 4
    };

    UtConverterElect();
    virtual ~UtConverterElect();

    void setUp();
    void tearDown();
    void initPotentialVectorValues();
    void testConfig();
    void testInput();
    void testDefaultConstruction();
    void testNominalInitialization();
    void testConductanceException();
    void testTripPriorityException();
    void testStandbyPowerException();
    void testInputUnderVoltageException();
    void testRegulatedVoltageException();
    void testEfficiencylessThanZeroException();
    void testEfficiencyGreaterThanOneException();
    void testAccessors();
    void testModifiers();
    void stepTheModel();
    void testStep();
    void testBuildConductance();
    void testBuildAdmittance();
    void testComputeFlow();
    void testConfirmSolutionAcceptable();
    void testConverterOff();
    void testOutputOverCurrentTrip();
    void testOutputOverVoltageTrip();
    void testBadEfficiency();
    void testBiasOutputLoad();
    void testInhibitOutOverCurrent();
    void testMalfunctions();

 private:

    CPPUNIT_TEST_SUITE(UtConverterElect);
    CPPUNIT_TEST(testConfig);
    CPPUNIT_TEST(testInput);
    CPPUNIT_TEST(testDefaultConstruction);
    CPPUNIT_TEST(testNominalInitialization);
    CPPUNIT_TEST(testConductanceException);
    CPPUNIT_TEST(testTripPriorityException);
    CPPUNIT_TEST(testStandbyPowerException);
    CPPUNIT_TEST(testInputUnderVoltageException);
    CPPUNIT_TEST(testRegulatedVoltageException);
    CPPUNIT_TEST(testEfficiencylessThanZeroException);
    CPPUNIT_TEST(testEfficiencyGreaterThanOneException);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testModifiers);
    CPPUNIT_TEST(testStep);
    CPPUNIT_TEST(testBuildConductance);
    CPPUNIT_TEST(testBuildAdmittance);
    CPPUNIT_TEST(testComputeFlow);
    CPPUNIT_TEST(testConfirmSolutionAcceptable);
    CPPUNIT_TEST(testConverterOff);
    CPPUNIT_TEST(testOutputOverCurrentTrip);
    CPPUNIT_TEST(testOutputOverVoltageTrip);
    CPPUNIT_TEST(testBadEfficiency);
    CPPUNIT_TEST(testBiasOutputLoad);
    CPPUNIT_TEST(testInhibitOutOverCurrent);
    CPPUNIT_TEST(testMalfunctions);
    CPPUNIT_TEST_SUITE_END();


    /// -- Pointer to nominal configuration data
    ConverterElectConfigData *tConfigData;

    /// -- Pointer to nominal input data
    ConverterElectInputData *tInputData;

    Converter *tConverter;

    /// -- Test Article
    FriendlyConverterElect *tArticle;

    // Config Data
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

    int tPort0;
    int tPort1;

    /// (s) Nominal time step
    double tTimeStep;

    /// -- Nominal tolerance for comparison of expected and returned values
    double tTolerance;

    double tConverterOnConductance;
    double tConverterOffConductance;
    double tStandbyPower;
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
    SensorAnalogInputData* tOutVoltageSensorInput;
    SensorAnalogInputData* tOutCurrentSensorInput;
    bool tMalfOpOverCurrentFlag;
    bool tMalfOpOverVoltageFlag;
    bool tMalfRegulatedVoltageFlag;

    UtConverterElect(const UtConverterElect& that);
    UtConverterElect operator =(const UtConverterElect& that);

};

/// @}

#endif
