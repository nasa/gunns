/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((aspects/electrical/TripLogic/GunnsElectPvRegSensors.o))
*/

#include <iostream>
#include "strings/UtResult.hh"
#include "UtGunnsElectPvRegSensors.hh"

/// @details  Test identification number.
int UtGunnsElectPvRegSensors::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectPvRegSensors class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvRegSensors::UtGunnsElectPvRegSensors()
    :
    tArticle(0),
    tSensorVin(),
    tSensorVout(),
    tSensorIin(),
    tSensorIout()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectPvRegSensors class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvRegSensors::~UtGunnsElectPvRegSensors()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegSensors::setUp()
{
    /// - Create the test article.
    tArticle = new GunnsElectPvRegSensors();

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegSensors::tearDown()
{
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for default construction of a GunnsElectPvRegSensors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegSensors::testConstruction()
{
    UT_RESULT_FIRST;

    /// @test    Nominal construction of a GunnsElectPvRegSensors.
    CPPUNIT_ASSERT(0 == tArticle->mInVoltage);
    CPPUNIT_ASSERT(0 == tArticle->mInCurrent);
    CPPUNIT_ASSERT(0 == tArticle->mOutVoltage);
    CPPUNIT_ASSERT(0 == tArticle->mOutCurrent);

    UT_PASS;
}

#include "core/test/UtGunnsSensorAnalogWrapper.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the sensor connection methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegSensors::testConnectSensors()
{
    UT_RESULT;

    /// @test    Wrapper pointer assignments.
    tArticle->connectInVoltage(&tSensorVin);
    CPPUNIT_ASSERT(&tSensorVin.mSensor == tArticle->mInVoltage);

    tArticle->connectInCurrent(&tSensorIin);
    CPPUNIT_ASSERT(&tSensorIin.mSensor == tArticle->mInCurrent);

    tArticle->connectOutVoltage(&tSensorVout);
    CPPUNIT_ASSERT(&tSensorVout.mSensor == tArticle->mOutVoltage);

    tArticle->connectOutCurrent(&tSensorIout);
    CPPUNIT_ASSERT(&tSensorIout.mSensor == tArticle->mOutCurrent);

    /// @test    Wrapper step flags are set.
    FriendlyGunnsSensorAnalogWrapper* wrapper = static_cast<FriendlyGunnsSensorAnalogWrapper*>(&tSensorVin);
    CPPUNIT_ASSERT(false == wrapper->mStepPreSolverFlag);
    CPPUNIT_ASSERT(true  == wrapper->mStepPostSolverFlag);

    wrapper = static_cast<FriendlyGunnsSensorAnalogWrapper*>(&tSensorVout);
    CPPUNIT_ASSERT(false == wrapper->mStepPreSolverFlag);
    CPPUNIT_ASSERT(true  == wrapper->mStepPostSolverFlag);

    wrapper = static_cast<FriendlyGunnsSensorAnalogWrapper*>(&tSensorIin);
    CPPUNIT_ASSERT(false == wrapper->mStepPreSolverFlag);
    CPPUNIT_ASSERT(true  == wrapper->mStepPostSolverFlag);

    wrapper = static_cast<FriendlyGunnsSensorAnalogWrapper*>(&tSensorIout);
    CPPUNIT_ASSERT(false == wrapper->mStepPreSolverFlag);
    CPPUNIT_ASSERT(true  == wrapper->mStepPostSolverFlag);

    UT_PASS_LAST;
}
