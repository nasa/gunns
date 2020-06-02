/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 ()

 ASSUMPTIONS AND LIMITATIONS:
 ()


 REFERENCE:
 ()

 LIBRARY DEPENDENCY:
 (
    (common/sensors/TsLimitSwitch.o)
 )

 PROGRAMMERS:
 (
  ((Chuck Sorensen) (LZT) (Dec 2, 2011) (TS21) (initial))
 )
 **************************************************************************************************/

#include "UtTsLimitSwitch.hh"
#include "common/sensors/TsLimitSwitch.hh"


/// @brief class for unit testing
class TsLimitSwitchTester : public TsLimitSwitch {
public:
    bool getFailedValue() {return mFailedValue;}
    bool getTrueValue() {return mTrueValue;}
    bool getNotPoweredValue() {return mNotPoweredValue;}
};
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsLimitSwitch::UtTsLimitSwitch() {
    ; // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsLimitSwitch::~UtTsLimitSwitch() {
    ; // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  unit test infrastructure
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsLimitSwitch::setUp() {
    ; // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  unit test infrastructure
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsLimitSwitch::tearDown() {
    ; // nothing to do
}

/// @brief initialization test
void UtTsLimitSwitch::testInit() {
    std::cout << "\n--------------------------------------------------------------------------------";
    std::cout << "\nTsLimitSwitch unit tests";
    std::cout << "\ntest initialization ";
    TsLimitSwitchInputData input;
    TsLimitSwitchConfigData config;
    TsLimitSwitchTester limitSw;

    CPPUNIT_ASSERT(!limitSw.getFailedValue());
    CPPUNIT_ASSERT(!limitSw.getTrueValue());
    CPPUNIT_ASSERT(!limitSw.getNotPoweredValue());
    CPPUNIT_ASSERT(!limitSw.getValue());

    input.mSensedValue = true;
    input.mTrueValue   = true;
    input.mFailedValue = true;
    config.mNotPoweredValue = true;
    CPPUNIT_ASSERT(!limitSw.isInitialized());
    limitSw.initialize(config, input);
    CPPUNIT_ASSERT(limitSw.isInitialized());

    CPPUNIT_ASSERT(limitSw.getFailedValue());
    CPPUNIT_ASSERT(limitSw.getTrueValue());
    CPPUNIT_ASSERT(limitSw.getNotPoweredValue());
    CPPUNIT_ASSERT(limitSw.getValue());

    std::cout << "Pass";
}

/// @brief update test
void UtTsLimitSwitch::testUpdate() {

    std::cout << "\ntest update         ";
    TsLimitSwitchInputData input;
    TsLimitSwitchConfigData config;
    TsLimitSwitch limitSw;

    limitSw.initialize(config, input);

    // nominal update f->t , t->f when power & not failed
    CPPUNIT_ASSERT(!limitSw.getValue());
    limitSw.update(true, true, false);
    CPPUNIT_ASSERT(limitSw.getValue());
    limitSw.update(false, true, false);
    CPPUNIT_ASSERT(!limitSw.getValue());

    limitSw.update(true, true, false);
    CPPUNIT_ASSERT(limitSw.getValue());
    limitSw.update(true, false, false);
    CPPUNIT_ASSERT(!limitSw.getValue());

    input.mTrueValue = false;
    config.mNotPoweredValue = true;
    limitSw.initialize(config, input);
    limitSw.update(false, true, false);
    CPPUNIT_ASSERT(!limitSw.getValue());
    limitSw.update(false, false, false);
    CPPUNIT_ASSERT(limitSw.getValue());

    limitSw.setFailedValue(false);
    limitSw.update(true, true, true);
    CPPUNIT_ASSERT(!limitSw.getValue());
    limitSw.update(false, true, true);
    CPPUNIT_ASSERT(!limitSw.getValue());
    limitSw.setFailedValue(true);
    limitSw.update(true, true, true);
    CPPUNIT_ASSERT(limitSw.getValue());
    limitSw.update(false, true, true);
    CPPUNIT_ASSERT(limitSw.getValue());



    std::cout << "Pass";
    std::cout << "\n--------------------------------------------------------------------------------";
}
