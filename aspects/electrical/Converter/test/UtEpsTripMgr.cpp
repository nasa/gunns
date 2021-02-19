/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
 (aspects/electrical/Converter/EpsTripMgr.o)
 )
**************************************************************************************************/

#include <math.h>

#include "UtEpsTripMgr.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  UtEpsTripMgr constructor called before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtEpsTripMgr::UtEpsTripMgr()
    :
    tArticle(),
    tLinkName(""),
    tTolerance(0.0),
    tTripPriority(0),
    tTripOccured(false),
    tTimeToTrip(false)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief UtEpsTripMgr destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtEpsTripMgr::~UtEpsTripMgr() {

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Sets up default data to be used for the unit test cases.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsTripMgr::setUp() {
    tTripPriority       = 3;
    tLinkName           = "TestEpsTripMgrLink";
    tTolerance          = 1.0e-08;
    tTimeToTrip         = false;
    tTripOccured        = false;

    tArticle = new FriendlyEpsTripMgr();

    try {
        /// - Initialize the Links. catch any exceptions and terminate.
        tArticle->initialize(tLinkName, tTripPriority);
    } catch (TsInitializationException &e) {
        std::cout << "UtEpsTripMgr setup - Initialization exception caught -- setup failed." << std::endl;
    } catch (...) {
        std::cout << "UtEpsTripMgr setup - unknown exception caught -- setup failed." << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Deleted all allocated objects after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsTripMgr::tearDown() {
    delete tArticle;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Test configuration
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsTripMgr::testInitialization() {
    std::cout << "\n -----------------------------------------------------------------";
    std::cout << "\n UtEpsTripMgr: testInitialization ........................";

    /// - Check nominal construction
    CPPUNIT_ASSERT(tLinkName       == tArticle->mName);
    CPPUNIT_ASSERT(tTripPriority   == tArticle->mTripPriority);
    CPPUNIT_ASSERT(tTripOccured    == tArticle->mTripOccured);
    CPPUNIT_ASSERT(tTimeToTrip     == tArticle->mTimeToTrip);
    CPPUNIT_ASSERT(false           == tArticle->mWaitToTrip);

    /// - Check default construction
    FriendlyEpsTripMgr defaultConfig;
    CPPUNIT_ASSERT(0   == defaultConfig.mTripPriority);
    CPPUNIT_ASSERT(0   == defaultConfig.mTripOccured);
    CPPUNIT_ASSERT(0   == defaultConfig.mTimeToTrip);
    CPPUNIT_ASSERT(0   == defaultConfig.mWaitToTrip);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsTripMgr::testDefaultConstruction() {
    std::cout << "\n UtEpsTripMgr: testDefaultConstruction ...........";

    /// @test the default constructed values
    FriendlyEpsTripMgr defaultArticle;
    CPPUNIT_ASSERT(false        == defaultArticle.mTripOccured);
    CPPUNIT_ASSERT(0            == defaultArticle.mTripPriority);
    CPPUNIT_ASSERT(false        == defaultArticle.mWaitToTrip);
    CPPUNIT_ASSERT(false          == defaultArticle.mTimeToTrip);

    /// @test new/delete for code coverage
    EpsTripMgr* article = new EpsTripMgr();
    delete article;

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests nominal initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsTripMgr::testNominalInitialization() {
    std::cout << "\n UtEpsTripMgr: testNominalInitialization .........";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyEpsTripMgr article;
    tTripPriority = 1;
    article.initialize(tLinkName, tTripPriority);
    CPPUNIT_ASSERT(false        == article.mTripOccured);
    CPPUNIT_ASSERT_EQUAL(1, article.mTripPriority);
    CPPUNIT_ASSERT(false        == article.mWaitToTrip);
    CPPUNIT_ASSERT(false        == article.mTimeToTrip);
    CPPUNIT_ASSERT(tLinkName    == article.mName);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests exception due to out of bound trip priority
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsTripMgr::testTripPriorityException() {
    std::cout << "\n UtEpsTripMgr: testTripPriorityException .........";

    /// @test initialization exception due to trip priority <= 0
    tTripPriority = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tLinkName, tTripPriority), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests exception due to out of bound efficiency value of the converter.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsTripMgr::testNameNullException() {
    std::cout << "\n UtEpsTripMgr: testNameNullException ...........";

    tLinkName = "";

    /// @test initialization exception due to efficiency < 0
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tLinkName, tTripPriority), TsInitializationException);
    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests link's compute flow routine.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsTripMgr::testcomputeTripState() {
    std::cout << "\n UtEpsTripMgr: testcomputeTripState ...................";

    int tripped = false;

    tArticle->computeTripState(tripped);
    /// - all the trips are off during initialization
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTripOccured);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mWaitToTrip);

    tripped = true;
    tArticle->computeTripState(tripped);
    /// - all the trip will be off and result will be confirm
    CPPUNIT_ASSERT_EQUAL(true, tArticle->mTripOccured);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mWaitToTrip);

    tripped = true;
    tArticle->mTimeToTrip = true;
    tArticle->computeTripState(tripped);
    /// - all the trip will be off and result will be confirm
    CPPUNIT_ASSERT_EQUAL(true, tArticle->mTripOccured);
    CPPUNIT_ASSERT_EQUAL(true, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mWaitToTrip);

    tripped = false;
    tArticle->mTimeToTrip = true;
    tArticle->mWaitToTrip = true;
    tArticle->computeTripState(tripped);
    /// - all the trip will be off and result will be confirm
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTripOccured);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mWaitToTrip);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the link step routin.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsTripMgr::testVerifyTimeToTrip() {
    std::cout << "\n UtEpsTripMgr: testVerifyTimeToTrip ..........................";

    int convergedStep = 1;
    GunnsBasicLink::SolutionResult result = tArticle->verifyTimeToTrip(convergedStep);

    /// - all the trip will be off and result will be confirm
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mWaitToTrip);
    CPPUNIT_ASSERT_EQUAL(3, tArticle->mTripPriority);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::CONFIRM, result);

    tArticle->mWaitToTrip = true;
    convergedStep = 2;
    result = tArticle->verifyTimeToTrip(convergedStep);
    /// - wait to trip is true, priority low - result will be delay
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mWaitToTrip);
    CPPUNIT_ASSERT_EQUAL(3, tArticle->mTripPriority);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::DELAY, result);

    convergedStep = 3;
    result = tArticle->verifyTimeToTrip(convergedStep);
    /// - wait to trip is true, priority reached - result will be reject
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mWaitToTrip);
    CPPUNIT_ASSERT_EQUAL(3, tArticle->mTripPriority);
    CPPUNIT_ASSERT_EQUAL(true, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::REJECT, result);

    tArticle->mWaitToTrip = false;
    convergedStep = 3;
    result = tArticle->verifyTimeToTrip(convergedStep);
    /// - wait to trip is false, priority reached - result will be confirm
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mWaitToTrip);
    CPPUNIT_ASSERT_EQUAL(3, tArticle->mTripPriority);
    CPPUNIT_ASSERT_EQUAL(true, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::CONFIRM, result);


    std::cout << "... Pass";
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests confirm solution when time to trip is false
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsTripMgr::testResetTrips() {
    std::cout << "\n UtEpsTripMgr: testResetTrips ..................";

    /// - reset trips set all flags to false
    tArticle->mTripOccured = true;
    tArticle->mWaitToTrip = true;

    tArticle->resetTrips();
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTripOccured);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mWaitToTrip);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests confirm solution when time to trip is false
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtEpsTripMgr::testTripTiming() {
    std::cout << "\n UtEpsTripMgr: testTripTiming ..................";

    int tripped = true;
    int convergedStep = 1;
    GunnsBasicLink::SolutionResult result;

    tArticle->computeTripState(tripped);
    result = tArticle->verifyTimeToTrip(convergedStep);

    /// - all the trip will be off and result will be confirm
    CPPUNIT_ASSERT_EQUAL(true, tArticle->mTripOccured);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mWaitToTrip);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::DELAY, result);

    tripped = true;
    convergedStep = 2;

    tArticle->computeTripState(tripped);
    result = tArticle->verifyTimeToTrip(convergedStep);

    /// - all the trip will be off and result will be confirm
    CPPUNIT_ASSERT_EQUAL(true, tArticle->mTripOccured);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mWaitToTrip);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::DELAY, result);

    tripped = true;
    convergedStep = 3;

    tArticle->computeTripState(tripped);
    result = tArticle->verifyTimeToTrip(convergedStep);

    /// - all the trip will be off and result will be confirm
    CPPUNIT_ASSERT_EQUAL(true, tArticle->mTripOccured);
    CPPUNIT_ASSERT_EQUAL(true, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mWaitToTrip);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::REJECT, result);

    tripped = false;
    convergedStep = 2;

    tArticle->computeTripState(tripped);
    result = tArticle->verifyTimeToTrip(convergedStep);

    /// - all the trip will be off and result will be confirm
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTripOccured);
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mTimeToTrip);
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mWaitToTrip);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::CONFIRM, result);


    std::cout << "... Pass";
}

