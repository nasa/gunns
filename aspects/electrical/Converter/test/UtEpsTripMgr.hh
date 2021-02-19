#ifndef UtEpsTripMgr_EXISTS
#define UtEpsTripMgr_EXISTS
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
 (UtEpsTripMgr.o)
 )
PROGRAMMERS:
 (
 (Shailaja Janapati) (L-3 Comm) (July 2013) (Initial Prototype)
 )
/// @{
********************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>
#include <string>

#include "aspects/electrical/Converter/EpsTripMgr.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from EpsTripMgr and befriend UtEpsTripMgr.
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////

class FriendlyEpsTripMgr: public EpsTripMgr {
 public:
    FriendlyEpsTripMgr();
    virtual ~FriendlyEpsTripMgr();
    friend class UtEpsTripMgr;
};

inline FriendlyEpsTripMgr::FriendlyEpsTripMgr()
    :
    EpsTripMgr() {
};

inline FriendlyEpsTripMgr::~FriendlyEpsTripMgr() {
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    EpsTripMgr unit tests.
/// @details  This class provides the unit tests for the EpsTripMgr within the CPPunit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtEpsTripMgr: public CppUnit::TestFixture {

public:
    enum Nodes
    {
        Node0 = 0,
        Node1 = 1,
        Node2 = 2,
        VACUUM = 3,
        N_NODES = 4
    };

    UtEpsTripMgr();
    virtual ~UtEpsTripMgr();

    void setUp();
    void tearDown();
    void testInitialization();
    void testDefaultConstruction();
    void testNominalInitialization();
    void testTripPriorityException();
    void testNameNullException();
    void testcomputeTripState();
    void testVerifyTimeToTrip();
    void testResetTrips();
    void testTripTiming();

 private:

    CPPUNIT_TEST_SUITE(UtEpsTripMgr);
    CPPUNIT_TEST(testInitialization);
    CPPUNIT_TEST(testDefaultConstruction);
    CPPUNIT_TEST(testNominalInitialization);
    CPPUNIT_TEST(testTripPriorityException);
    CPPUNIT_TEST(testNameNullException);
    CPPUNIT_TEST(testcomputeTripState);
    CPPUNIT_TEST(testVerifyTimeToTrip);
    CPPUNIT_TEST(testResetTrips);
    CPPUNIT_TEST(testTripTiming);
    CPPUNIT_TEST_SUITE_END();

    /// -- Test Article
    FriendlyEpsTripMgr *tArticle;

    /// -- Converter Link Name
    std::string tLinkName;

    /// -- Nominal tolerance for comparison of expected and returned values
    double tTolerance;
    int    tTripPriority;
    bool   tTripOccured;
    bool   tTimeToTrip;

    UtEpsTripMgr(const UtEpsTripMgr& that);
    UtEpsTripMgr operator =(const UtEpsTripMgr& that);

};

/// @}

#endif
