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
    (software/exceptions/TsInitializationException.0)
    (../SensorVlvOpenClose.o)
 )

 PROGRAMMERS:
 (
  ((Chuck Sorensen) (LZT) (Dec 5, 2011) (TS21) (initial))
 )
 **************************************************************************************************/

#include "UtSensorVlvOpenClose.hh"
#include "software/exceptions/TsInitializationException.hh"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSensorVlvOpenClose::UtSensorVlvOpenClose() :
    mSensors(),
    mConfig(),
    mInput(),
    mOpenConfig(),
    mCloseConfig() {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSensorVlvOpenClose::~UtSensorVlvOpenClose() {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorVlvOpenClose::setUp() {



}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorVlvOpenClose::tearDown() {
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  a test
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorVlvOpenClose::testInitialization() {
    std::cout << "\n-------------------------------------------------------------------------------";
    std::cout << "\nTesting valve open close sensors";
    std::cout << "\nTest initialize                ";


    CPPUNIT_ASSERT_NO_THROW(mSensors.initialize(mConfig, mInput, "foo"));

    std::cout << " Pass";

}

void UtSensorVlvOpenClose::testUpdate() {
    std::cout << "\nTest update                    ";

    CPPUNIT_ASSERT_NO_THROW(mSensors.initialize(mConfig, mInput, "foo"));

    mSensors.setPowerFlag(true);
    mSensors.setTruthInput(0.0);
    CPPUNIT_ASSERT_NO_THROW(mSensors.update(0.1));
    CPPUNIT_ASSERT(mSensors.isClosed() );
    CPPUNIT_ASSERT(!mSensors.isOpen() );

    mSensors.setTruthInput(0.5);
    CPPUNIT_ASSERT_NO_THROW(mSensors.update(0.1));
    CPPUNIT_ASSERT(!mSensors.isClosed() );
    CPPUNIT_ASSERT(!mSensors.isOpen() );

    mSensors.setTruthInput(1.0);
    CPPUNIT_ASSERT_NO_THROW(mSensors.update(0.1));
    CPPUNIT_ASSERT(!mSensors.isClosed() );
    CPPUNIT_ASSERT(mSensors.isOpen() );

    std::cout << " Pass";

}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  test constructor with config parms
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorVlvOpenClose::testConfigConstructor() {
    std::cout << "\nTest Config constructor        ";

    SensorBooleanAiConfigData open(true, 1.5, 0.5);
    SensorBooleanAiConfigData close(true, 2.0, 2.5);
    SensorVlvOpenCloseConfigData cfg(open, close);

    CPPUNIT_ASSERT(open.mOffValue == cfg.mOpen.mOffValue);
    CPPUNIT_ASSERT(close.mOffValue == cfg.mClose.mOffValue);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(open.mTarget,  cfg.mOpen.mTarget,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(close.mTarget, cfg.mClose.mTarget, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(open.mTolerance,  cfg.mOpen.mTolerance,  0.0000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(close.mTolerance, cfg.mClose.mTolerance, 0.0000001);

    std::cout << " Pass";

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  test accessors
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSensorVlvOpenClose::testAccessors() {
    std::cout << "\nTest accessors                 ";

    CPPUNIT_ASSERT_NO_THROW(mSensors.initialize(mConfig, mInput, "foo"));

    CPPUNIT_ASSERT_NO_THROW(mSensors.sense(0.1, true, 0.0));
    CPPUNIT_ASSERT(mSensors.isClosed() );
    CPPUNIT_ASSERT(!mSensors.isOpen() );

    CPPUNIT_ASSERT_NO_THROW(mSensors.sense(0.1, true, 0.5));
    CPPUNIT_ASSERT(!mSensors.isClosed() );
    CPPUNIT_ASSERT(!mSensors.isOpen() );

    CPPUNIT_ASSERT_NO_THROW(mSensors.sense(0.1, true, 1.0));
    CPPUNIT_ASSERT(!mSensors.isClosed() );
    CPPUNIT_ASSERT(mSensors.isOpen() );

    std::cout << " Pass";
    std::cout << "\n-------------------------------------------------------------------------------\n";

}


