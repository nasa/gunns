#ifndef EPS_CONVERTER_EXISTS_HH
#define EPS_CONVERTER_EXISTS_HH
/********************************************************
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_ISS_EPS_CONVERTER_UNIT_TEST    Generic Converter Unit Test
/// @ingroup  UT_ISS_EPS
/// -
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
LIBRARY DEPENDENCY:
 (
 (UT_Converter.o)
 )
/// @details  Unit Tests for the generic Converter
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////
*********************************************************/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/electrical/Converter/Converter.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from Converter and befriend Converter.
/// -
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyConverter: public Converter {
 public:
    FriendlyConverter();
    virtual ~FriendlyConverter();
    friend class UtConverter;
};

inline FriendlyConverter::FriendlyConverter()
    :
    Converter() {
};

inline FriendlyConverter::~FriendlyConverter() {
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Converter unit tests.
//// -
/// @details  This class provides the unit tests for the converter within the CPPunit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtConverter: public CppUnit::TestFixture {
 private:

    CPPUNIT_TEST_SUITE(UtConverter);
    //===========================================
    CPPUNIT_TEST(testConfig);
    CPPUNIT_TEST(testInput);
    CPPUNIT_TEST(testDefaultConstruction);
    CPPUNIT_TEST(testNominalInitialization);
    CPPUNIT_TEST(testStandbyPowerException);
    CPPUNIT_TEST(testInputUnderVoltageException);
    CPPUNIT_TEST(testRegulatedVoltageException);
    CPPUNIT_TEST(testEfficiencyException);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testModifiers);
    CPPUNIT_TEST(testUpdate);
    CPPUNIT_TEST(testConversion);
    CPPUNIT_TEST(testConverterOff);
    CPPUNIT_TEST(testInputUnderVoltageTrip);
    CPPUNIT_TEST(testInputOverVoltageTrip);
    CPPUNIT_TEST(testOutputOverCurrentTrip);
    CPPUNIT_TEST(testOutputOverVoltageTrip);
    CPPUNIT_TEST(testBadEfficiency);
    CPPUNIT_TEST(testResetTrips);
    CPPUNIT_TEST_SUITE_END();

    /// --     Pointer to nominal configuration data
    ConverterConfigData* mConfigData;
    /// --     Pointer to nominal input data
    ConverterInputData*  mInputData;
    /// --     Test Article
    FriendlyConverter* mArticle;


    double mStandbyPower;
    double mInputVoltage;
    double mRegulatedVoltage;
    double mEfficiency;
    double mOutputOverCurrentLimit;
    double mOutputOverVoltageLimit;
    bool mOutOverCurrentTripActive;
    bool mOutOverVoltageTripActive;
    double mInputOverVoltageLimit;
    double mInputUnderVoltageLimit;
    bool mInOverVoltageTripActive;
    bool mInUnderVoltageTripActive;
    double mTolerance;

    UtConverter(const UtConverter& that);
    UtConverter operator =(const UtConverter& that);

 public:

    UtConverter();
    virtual ~UtConverter();

    void setUp();
    void tearDown();
    void testConfig();
    void testInput();
    void testDefaultConstruction();
    void testNominalInitialization();
    void testStandbyPowerException();
    void testInputUnderVoltageException();
    void testRegulatedVoltageException();
    void testEfficiencyException();
    void testAccessors();
    void testModifiers();
    void testUpdate();
    void testConversion();
    void testConverterOff();
    void testInputUnderVoltageTrip();
    void testInputOverVoltageTrip();
    void testOutputOverCurrentTrip();
    void testOutputOverVoltageTrip();
    void testBadEfficiency();
    void testResetTrips();
};

/// @}

#endif

