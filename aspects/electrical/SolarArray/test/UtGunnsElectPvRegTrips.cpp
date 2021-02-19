/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((aspects/electrical/TripLogic/GunnsElectPvRegTrips.o))
*/

#include <iostream>
#include "strings/UtResult.hh"
#include "UtGunnsElectPvRegTrips.hh"

/// @details  Test identification number.
int UtGunnsElectPvRegTrips::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectPvRegTrips class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvRegTrips::UtGunnsElectPvRegTrips()
    :
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectPvRegTrips class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvRegTrips::~UtGunnsElectPvRegTrips()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegTrips::setUp()
{
    /// - Create the test article.
    tArticle = new GunnsElectPvRegTrips();

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegTrips::tearDown()
{
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for default construction of a GunnsElectPvRegTrips.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegTrips::testConstruction()
{
    UT_RESULT_FIRST;

    /// @test    Nominal construction of a GunnsElectPvRegTrips.
    CPPUNIT_ASSERT(false == tArticle->mInOverVoltage.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInOverCurrent.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mOutOverVoltage.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mOutOverCurrent.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mOutUnderVoltage.isTripped());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the isTripped method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegTrips::testIsTripped()
{
    UT_RESULT;

    /// @test    isTripped when all trip logics are tripped.
    tArticle->mInOverVoltage.initialize(0.0, 0, true);
    tArticle->mInOverCurrent.initialize(0.0, 0, true);
    tArticle->mOutOverVoltage.initialize(0.0, 0, true);
    tArticle->mOutOverCurrent.initialize(0.0, 0, true);
    tArticle->mOutUnderVoltage.initialize(0.0, 0, true);
    CPPUNIT_ASSERT(true == tArticle->isTripped());

    /// @test    isTripped when no trip logics are tripped.
    tArticle->mInOverVoltage.initialize(0.0, 0, false);
    tArticle->mInOverCurrent.initialize(0.0, 0, false);
    tArticle->mOutOverVoltage.initialize(0.0, 0, false);
    tArticle->mOutOverCurrent.initialize(0.0, 0, false);
    tArticle->mOutUnderVoltage.initialize(0.0, 0, false);
    CPPUNIT_ASSERT(false == tArticle->isTripped());

    /// @test    isTripped for each single trip logic tripped.
    tArticle->mInOverVoltage.initialize(0.0, 0, true);
    CPPUNIT_ASSERT(true == tArticle->isTripped());

    tArticle->mInOverVoltage.initialize(0.0, 0, false);
    tArticle->mInOverCurrent.initialize(0.0, 0, true);
    CPPUNIT_ASSERT(true == tArticle->isTripped());

    tArticle->mInOverCurrent.initialize(0.0, 0, false);
    tArticle->mOutOverVoltage.initialize(0.0, 0, true);
    CPPUNIT_ASSERT(true == tArticle->isTripped());

    tArticle->mOutOverVoltage.initialize(0.0, 0, false);
    tArticle->mOutOverCurrent.initialize(0.0, 0, true);
    CPPUNIT_ASSERT(true == tArticle->isTripped());

    tArticle->mOutOverCurrent.initialize(0.0, 0, false);
    tArticle->mOutUnderVoltage.initialize(0.0, 0, true);
    CPPUNIT_ASSERT(true == tArticle->isTripped());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the resetTrip method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvRegTrips::testResetTrip()
{
    UT_RESULT;

    tArticle->mInOverVoltage.initialize(0.0, 0, true);
    tArticle->mInOverCurrent.initialize(0.0, 0, true);
    tArticle->mOutOverVoltage.initialize(0.0, 0, true);
    tArticle->mOutOverCurrent.initialize(0.0, 0, true);
    tArticle->mOutUnderVoltage.initialize(0.0, 0, true);
    CPPUNIT_ASSERT(true == tArticle->isTripped());

    /// @test    resetTrips resets all trips.
    tArticle->resetTrips();

    CPPUNIT_ASSERT(false == tArticle->isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInOverVoltage.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mInOverCurrent.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mOutOverVoltage.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mOutOverCurrent.isTripped());
    CPPUNIT_ASSERT(false == tArticle->mOutUnderVoltage.isTripped());

    UT_PASS_LAST;
}
