/**
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

//#include <iostream>
#include "UtGunnsElectDistributed2WayBus.hh"
#include "strings/UtResult.hh"

/// @details  Test identification number.
int UtGunnsElectDistributed2WayBus::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectDistributed2WayBus class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectDistributed2WayBus::UtGunnsElectDistributed2WayBus()
    :
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectDistributed2WayBus class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectDistributed2WayBus::~UtGunnsElectDistributed2WayBus()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributed2WayBus::setUp()
{
    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsElectDistributed2WayBus;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributed2WayBus::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of the Electrical Distributed 2-Way Bus notification message.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributed2WayBus::testNotificationConstruction()
{
    UT_RESULT_FIRST;

    /// @test Default construction.
    GunnsDistributed2WayBusNotification defaultNotif;
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::NONE == defaultNotif.mLevel);
    CPPUNIT_ASSERT(""                                        == defaultNotif.mMessage);

    /// @test Nominal construction.
    GunnsDistributed2WayBusNotification nominalNotif(GunnsDistributed2WayBusNotification::ERR, "Error!");
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::ERR == nominalNotif.mLevel);
    CPPUNIT_ASSERT("Error!"                                 == nominalNotif.mMessage);

    /// @test Copy construction.
    GunnsDistributed2WayBusNotification copyNotif(nominalNotif);
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::ERR == copyNotif.mLevel);
    CPPUNIT_ASSERT("Error!"                                 == copyNotif.mMessage);

    /// @test Assignment operator.
    defaultNotif = copyNotif;
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::ERR == defaultNotif.mLevel);
    CPPUNIT_ASSERT("Error!"                                 == defaultNotif.mMessage);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsElectDistributed2WayBus class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributed2WayBus::testConstruction()
{
    UT_RESULT;

    /// @test    Default construction.
    CPPUNIT_ASSERT(0                                  == tArticle->mInData.mFrameCount);
    CPPUNIT_ASSERT(0                                  == tArticle->mInData.mFrameLoopback);
    CPPUNIT_ASSERT(false                              == tArticle->mInData.mDemandMode);
    CPPUNIT_ASSERT(0.0                                == tArticle->mInData.mDemandPower);
    CPPUNIT_ASSERT(0.0                                == tArticle->mInData.mSupplyVoltage);
    CPPUNIT_ASSERT(0                                  == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(0                                  == tArticle->mOutData.mFrameLoopback);
    CPPUNIT_ASSERT(false                              == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(0.0                                == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(0.0                                == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(false                              == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(GunnsElectDistributed2WayBus::NONE == tArticle->mForcedRole);
    CPPUNIT_ASSERT(0                                  == tArticle->mSupplyDatas.size());
    CPPUNIT_ASSERT(0                                  == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(0                                  == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(0                                  == tArticle->mNotifications.size());

    /// @test    New/delete for code coverage.
    GunnsElectDistributed2WayBus* testArticle = new GunnsElectDistributed2WayBus();
    delete testArticle;

    /// @test    Interface data functions.
    GunnsElectDistributed2WayBusInterfaceData* ifData1 = new GunnsElectDistributed2WayBusInterfaceData();
    GunnsElectDistributed2WayBusInterfaceData* ifData2 = new GunnsElectDistributed2WayBusInterfaceData();
    ifData1->mDemandMode    = true;
    ifData1->mDemandPower   = 1.0;
    ifData1->mFrameCount    = 42;
    ifData1->mFrameLoopback = 12;
    ifData1->mSupplyVoltage = 120.0;
    *ifData2 = *ifData1;
    CPPUNIT_ASSERT(ifData2->mDemandMode    == ifData1->mDemandMode);
    CPPUNIT_ASSERT(ifData2->mDemandPower   == ifData1->mDemandPower);
    CPPUNIT_ASSERT(ifData2->mFrameCount    == ifData1->mFrameCount);
    CPPUNIT_ASSERT(ifData2->mFrameLoopback == ifData1->mFrameLoopback);
    CPPUNIT_ASSERT(ifData2->mSupplyVoltage == ifData1->mSupplyVoltage);
    CPPUNIT_ASSERT(true                    == ifData2->hasValidData());
    delete ifData2;
    delete ifData1;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed 2-Way Bus Interface initialization without errors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributed2WayBus::testNominalInitialization()
{
    UT_RESULT;

    /// @test    Adding some voltage supply interfaces.
    GunnsElectDistributed2WayBusSupplyData* supply1 = tArticle->createSupplyData();
    GunnsElectDistributed2WayBusSupplyData* supply2 = tArticle->createSupplyData();
    GunnsElectDistributed2WayBusSupplyData* supply3 = tArticle->createSupplyData();

    CPPUNIT_ASSERT(3       == tArticle->mSupplyDatas.size());
    CPPUNIT_ASSERT(supply1 == tArticle->mSupplyDatas.at(0));
    CPPUNIT_ASSERT(supply2 == tArticle->mSupplyDatas.at(1));
    CPPUNIT_ASSERT(supply3 == tArticle->mSupplyDatas.at(2));

    /// @test    Initialize function as primary side.
    const float voltage = 120.0;
    tArticle->initialize(true, voltage);

    CPPUNIT_ASSERT(true    == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(true    == tArticle->mInData.mDemandMode);
    CPPUNIT_ASSERT(0.0     == tArticle->mInData.mDemandPower);
    CPPUNIT_ASSERT(voltage == tArticle->mInData.mSupplyVoltage);
    CPPUNIT_ASSERT(false   == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(0.0     == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(voltage == tArticle->mOutData.mSupplyVoltage);

    /// @test    Initialize function as secondary side.
    FriendlyGunnsElectDistributed2WayBus article2;
    article2.initialize(false, voltage);

    CPPUNIT_ASSERT(false   == article2.mIsPairMaster);
    CPPUNIT_ASSERT(false   == article2.mInData.mDemandMode);
    CPPUNIT_ASSERT(0.0     == article2.mInData.mDemandPower);
    CPPUNIT_ASSERT(voltage == article2.mInData.mSupplyVoltage);
    CPPUNIT_ASSERT(true    == article2.mOutData.mDemandMode);
    CPPUNIT_ASSERT(0.0     == article2.mOutData.mDemandPower);
    CPPUNIT_ASSERT(voltage == article2.mOutData.mSupplyVoltage);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed 2-Way Bus Interface nominal initialization with
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributed2WayBus::testInitializationErrors()
{
    UT_RESULT;

    /// - currently nothing to test

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed 2-Way Bus Interface updateFrameCounts method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributed2WayBus::testUpdateFrameCounts()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    tArticle->initialize(true, 120.0);

    /// @test    updateFrameCounts method.
    tArticle->mOutData.mFrameCount   = 43;
    tArticle->mInData.mFrameCount    = 44;
    tArticle->mInData.mFrameLoopback = 42;
    tArticle->mFramesSinceFlip       = 99;
    tArticle->updateFrameCounts();
    CPPUNIT_ASSERT(44  == tArticle->mOutData.mFrameCount);
    CPPUNIT_ASSERT(100 == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(2   == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(44  == tArticle->mOutData.mFrameLoopback);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed 2-Way Bus Interface update method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributed2WayBus::testUpdate()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    GunnsElectDistributed2WayBusSupplyData* supply1 = tArticle->createSupplyData();
    GunnsElectDistributed2WayBusSupplyData* supply2 = tArticle->createSupplyData();
    tArticle->initialize(true, 120.0);
    tArticle->mOutData.mFrameCount = 43;

    GunnsDistributed2WayBusNotification notif;
    GunnsDistributed2WayBusNotification::NotificationLevel NONE =
            GunnsDistributed2WayBusNotification::NONE;
    GunnsDistributed2WayBusNotification::NotificationLevel INFO =
            GunnsDistributed2WayBusNotification::INFO;

    /// @test    Remain in Supply mode.
    supply1->mAvailable              = true;
    supply1->mMaximumVoltage         = 120.0;
    supply2->mAvailable              = true;
    supply2->mMaximumVoltage         = 105.0;
    tArticle->mInData.mDemandMode    = true;
    tArticle->mInData.mDemandPower   = 100.0;
    tArticle->mInData.mSupplyVoltage = 110.0;
    tArticle->mInData.mFrameCount    = 44;
    tArticle->mInData.mFrameLoopback = 42;
    tArticle->updateFrameCounts();
    tArticle->update(119.0, 1.0);
    tArticle->popNotification(notif);

    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(119.0 == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(NONE  == notif.mLevel);
    CPPUNIT_ASSERT(""    == notif.mMessage);

    /// @test    Switch to Demand mode.
    supply1->mAvailable              = false;
    tArticle->mInData.mFrameCount    = 45;
    tArticle->mInData.mFrameLoopback = 43;
    tArticle->updateFrameCounts();
    tArticle->update(104.0, 1.0);
    tArticle->popNotification(notif);

    CPPUNIT_ASSERT(true  == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(105.0 == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(1.0   == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(0     == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(INFO  == notif.mLevel);
    CPPUNIT_ASSERT(0     == notif.mMessage.rfind("flipping to Demand role", 0));

    /// @test    Remain in Demand mode, even though our local supply has returned, because not
    ///          enough frames have passed since our flip to Demand.
    supply1->mAvailable              = true;
    tArticle->mInData.mFrameCount    = 46;
    tArticle->mInData.mFrameLoopback = 44;
    tArticle->updateFrameCounts();
    tArticle->update(119.0, 1.0);
    tArticle->popNotification(notif);

    tArticle->mInData.mFrameCount    = 47;
    tArticle->mInData.mFrameLoopback = 45;
    tArticle->updateFrameCounts();
    tArticle->update(119.0, 1.0);
    tArticle->popNotification(notif);

    CPPUNIT_ASSERT(true  == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(120.0 == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(1.0   == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(2     == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(NONE  == notif.mLevel);
    CPPUNIT_ASSERT(""    == notif.mMessage);

    /// @test    Switch to Supply mode.
    tArticle->mInData.mFrameCount    = 48;
    tArticle->mInData.mFrameLoopback = 46;
    tArticle->updateFrameCounts();
    tArticle->update(119.0, 1.0);
    tArticle->popNotification(notif);

    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(119.0 == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(0     == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(INFO  == notif.mLevel);
    CPPUNIT_ASSERT(0     == notif.mMessage.rfind("flipping to Supply role", 0));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Electrical Distributed 2-Way Bus Interface update method with forced roles.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributed2WayBus::testUpdateForcedRole()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    GunnsElectDistributed2WayBusSupplyData* supply1 = tArticle->createSupplyData();
    tArticle->initialize(true, 120.0);
    tArticle->mOutData.mFrameCount = 43;

    GunnsDistributed2WayBusNotification notif;
    GunnsDistributed2WayBusNotification::NotificationLevel NONE =
            GunnsDistributed2WayBusNotification::NONE;
    GunnsDistributed2WayBusNotification::NotificationLevel INFO =
            GunnsDistributed2WayBusNotification::INFO;

    /// @test    Remains in Supply when forced.
    tArticle->mInData.mDemandMode    = true;
    tArticle->mInData.mDemandPower   = 100.0;
    tArticle->mInData.mSupplyVoltage = 110.0;
    tArticle->mInData.mFrameCount    = 44;
    tArticle->mInData.mFrameLoopback = 42;
    tArticle->updateFrameCounts();
    tArticle->forceSupplyRole();
    tArticle->update(2.0, 1.0);

    CPPUNIT_ASSERT(false == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(2.0   == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(0     == tArticle->mNotifications.size());

    /// @test    Force to Demand role.
    tArticle->forceDemandRole();
    tArticle->update(2.0, 1.0);

    CPPUNIT_ASSERT(true  == tArticle->mOutData.mDemandMode);
    CPPUNIT_ASSERT(0.0   == tArticle->mOutData.mSupplyVoltage);
    CPPUNIT_ASSERT(1.0   == tArticle->mOutData.mDemandPower);
    CPPUNIT_ASSERT(0     == tArticle->mNotifications.size());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the Electrical Distributed 2-Way Bus Interface getter and setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectDistributed2WayBus::testAccessors()
{
    UT_RESULT;

    /// @test    resetForceRole()
    tArticle->mForcedRole = GunnsElectDistributed2WayBus::SUPPLY;
    tArticle->resetForceRole();
    CPPUNIT_ASSERT(GunnsElectDistributed2WayBus::NONE == tArticle->mForcedRole);

    /// @test    isInDemandRole()
    tArticle->mOutData.mDemandMode = true;
    CPPUNIT_ASSERT(true  == tArticle->isInDemandRole());
    tArticle->mOutData.mDemandMode = false;
    CPPUNIT_ASSERT(false == tArticle->isInDemandRole());

    /// @test    getRemoteLoad()
    tArticle->mInData.mDemandPower = 10.0;
    tArticle->mOutData.mDemandMode = true;
    CPPUNIT_ASSERT(0.0  == tArticle->getRemoteLoad());
    tArticle->mOutData.mDemandMode = false;
    CPPUNIT_ASSERT(10.0 == tArticle->getRemoteLoad());

    /// @test    getRemoteSupply()
    tArticle->mInData.mSupplyVoltage = 100.0;
    CPPUNIT_ASSERT(100.0 == tArticle->getRemoteSupply());

    UT_PASS_LAST;
}
