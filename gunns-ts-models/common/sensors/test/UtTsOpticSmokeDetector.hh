#ifndef UtTsOpticSmokeDetector_EXISTS
#define UtTsOpticSmokeDetector_EXISTS

/**
@defgroup  UtTsOpticSmokeDetector Optic Smoke Detector unit test
@ingroup   TSM_OpticSmokeDetector

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
@details
PURPOSE:
 - (Unit test for TsOpticSmokeDectector)

 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ()

 PROGRAMMERS:
 - ((Jose A. Perez) (L3-Comm) (Mar 23, 2012) (TS21) (initial))
@{
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "../TsOpticSmokeDetector.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @reqt Test_TsOpticSmokeDetector REQ-R.TS222-0001 TS21 ECLSS models shall detect simulated fires.
/// @reqt Test_TsOpticSmokeDetector REQ-R.TS222-0002 TS21 ECLSS models shall provide sensor telemetry for the detection of smoke.
///
/// @details
///   This test verifies the ts-model optical smoke detector's functionality.
///
///   The TsOpticSmokeDetector will receive a power command and will start generating
///   obscuration and scatter readings according to the present amount of smoke. The smoke
///   value is set by another higher level class or model. The smoke detector also receives
///   an Active Bit Test command, hence performs a time-based bit test.
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsOpticSmokeDetector and befriend UtUtTsOpticSmokeDetector.
///
/// @details  Class derived from the unit under test. Has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members. Reason of doing this is
///           to have access to the private FSW and malfunction parameters of TsOpticSmokeDetector
///           class.
///////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyTsOpticSmokeDetector: public TsOpticSmokeDetector
{
public:
FriendlyTsOpticSmokeDetector();
virtual ~FriendlyTsOpticSmokeDetector();
friend class UtTsOpticSmokeDetector;
};

inline FriendlyTsOpticSmokeDetector::FriendlyTsOpticSmokeDetector()
    :
    TsOpticSmokeDetector()
{
}

inline FriendlyTsOpticSmokeDetector::~FriendlyTsOpticSmokeDetector()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TsOpticSmokeDetector unit tests.
///
/// @details  This class provides the unit tests for the TsOpticSmokeDetector class with
///           CPPUnit framework.
///////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsOpticSmokeDetector : public CppUnit::TestFixture {

public:
    UtTsOpticSmokeDetector();
    virtual ~UtTsOpticSmokeDetector();

    void setUp();
    void tearDown();

    void DefaultConstructorTest();
    void InitializeMethodTest();
    void ConfigExceptionThrownMinObscurationGreaterThanMaxTest();
    void ConfigExceptionThrownNoNameSpecifiedTest();
    void ConfigExceptionThrownMinScatterGreaterThanMaxTest();
    void ConfigExceptionThrownMinObscurationGreaterThanNominalTest();
    void ConfigExceptionThrownMinScatterGreaterThanNominalTest();
    void ConfigExceptionThrownBitOnMaxScatterValueNotCorrectValueTest();
    void ConfigExceptionThrownBitRecoveryTimeLessThanTolTest();
    void ConfigExceptionThrownSmokeLevelOutOfBoundsTest();
    void ConfigExceptionThrownBitTimerOutOfBoundsTest();
    void InitExceptionThrownCallingUpdatePriorInitTest();
    void RunUpdateNominallyTest();
    void RunUpdateNominallyWithSmokeTest();
    void RunUpdateWithPowerMalfunctionsTest();
    void RunUpdateWithNoPowerTest();
    void RunUpdateWithFailCommandTest();
    void RunUpdateWithFailedObscurationSensorTest();
    void RunUpdateWithFailedScatterSensorTest();
    void RunActiveBitTest();
    void RunActiveBitWithSmokeTest();


protected:

private:
    FriendlyTsOpticSmokeDetector testArticle;
    TsOpticSmokeDetectorConfigData configData;
    TsOpticSmokeDetectorInputData inputData;
    TsOpticSmokeDetector * test;
    TsOpticSmokeDetectorConfigData * testC;
    TsOpticSmokeDetectorInputData *testI;
    float dt;
    std::string                sdName;

    CPPUNIT_TEST_SUITE(UtTsOpticSmokeDetector);

    CPPUNIT_TEST(DefaultConstructorTest);
    CPPUNIT_TEST(InitializeMethodTest);
    CPPUNIT_TEST(ConfigExceptionThrownNoNameSpecifiedTest);
    CPPUNIT_TEST(ConfigExceptionThrownMinObscurationGreaterThanMaxTest);
    CPPUNIT_TEST(ConfigExceptionThrownMinScatterGreaterThanMaxTest);
    CPPUNIT_TEST(ConfigExceptionThrownMinObscurationGreaterThanNominalTest);
    CPPUNIT_TEST(ConfigExceptionThrownMinScatterGreaterThanNominalTest);
    CPPUNIT_TEST(ConfigExceptionThrownBitOnMaxScatterValueNotCorrectValueTest);
    CPPUNIT_TEST(ConfigExceptionThrownBitRecoveryTimeLessThanTolTest);
    CPPUNIT_TEST(ConfigExceptionThrownSmokeLevelOutOfBoundsTest);
    CPPUNIT_TEST(ConfigExceptionThrownBitTimerOutOfBoundsTest);
    CPPUNIT_TEST(InitExceptionThrownCallingUpdatePriorInitTest);
    CPPUNIT_TEST(RunUpdateNominallyTest);
    CPPUNIT_TEST(RunUpdateNominallyWithSmokeTest);
    CPPUNIT_TEST(RunUpdateWithPowerMalfunctionsTest);
    CPPUNIT_TEST(RunUpdateWithNoPowerTest);
    CPPUNIT_TEST(RunUpdateWithFailCommandTest);
    CPPUNIT_TEST(RunUpdateWithFailedObscurationSensorTest);
    CPPUNIT_TEST(RunUpdateWithFailedScatterSensorTest);
    CPPUNIT_TEST(RunActiveBitTest);
    CPPUNIT_TEST(RunActiveBitWithSmokeTest);

    CPPUNIT_TEST_SUITE_END();

    /// @brief keep private, never used
    UtTsOpticSmokeDetector(const UtTsOpticSmokeDetector &rhs);

    /// @brief keep private, never used
    UtTsOpticSmokeDetector& operator= (const UtTsOpticSmokeDetector &rhs);

};


/// @}


#endif //UtTsOpticSmokeDetector_EXISTS
