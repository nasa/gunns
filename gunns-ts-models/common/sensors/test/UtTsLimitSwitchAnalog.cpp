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
    (common/sensors/TsLimitSwitchAnalog.o)
    (software/exceptions/TsInitializationException.o)
    (
 )

 PROGRAMMERS:
 (
  ((Chuck Sorensen) (LZT) (Dec 2, 2011) (TS21) (initial))
 )
 **************************************************************************************************/

#include "UtTsLimitSwitchAnalog.hh"
#include "common/sensors/TsLimitSwitchAnalog.hh"
#include "software/exceptions/TsInitializationException.hh"



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsLimitSwitchAnalog::UtTsLimitSwitchAnalog() {
    ; // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsLimitSwitchAnalog::~UtTsLimitSwitchAnalog() {
    ; // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsLimitSwitchAnalog::setUp() {

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsLimitSwitchAnalog::tearDown() {
    ; // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  test update
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsLimitSwitchAnalog::testUpdate() {
    std::cout << "\n--------------------------------------------------------------------------------";
    std::cout << "\nTsLimitSwitchAnalog unit tests";
    std::cout << "\ntest update ";

    TsLimitSwitchAnalogConfigData config;
    TsLimitSwitchAnalogInputData  input;
    TsLimitSwitchAnalog           lsa;

    config.mTargetValue = 1.0;
    config.mTolerance   = 0.01;

    lsa.initialize(config, input);
    config.mTolerance   = -2.0;
    CPPUNIT_ASSERT_THROW(lsa.initialize(config, input), TsInitializationException);
    config.mTolerance   = 0.01;
    CPPUNIT_ASSERT_NO_THROW(lsa.initialize(config, input));

    lsa.update(0.0, true, false);
    CPPUNIT_ASSERT(!lsa.getValue());
    lsa.update(1.0, true, false);
    CPPUNIT_ASSERT(lsa.getValue());

    std::cout << "Pass";
    std::cout << "\n--------------------------------------------------------------------------------";

}
