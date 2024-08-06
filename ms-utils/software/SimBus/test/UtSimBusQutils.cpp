/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((software/SimBus/SimBusQutils.o) )
***************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

#include "UtSimBusQutils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Sim Bus Queue Utilities unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSimBusQutils::UtSimBusQutils()
    :
    tQueueDouble(0),
    tQueueInt(0),
    tQueueBool(0),
    tQueueSize(0),
    tExtractionSize(0)
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Sim Bus Queue Utilities unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSimBusQutils::~UtSimBusQutils()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimBusQutils::setUp()
{
    /// - Allocate the queue arrays and initialize the values.
    tQueueSize      = 10;
    tExtractionSize = 5;
    tQueueDouble    = new double[tQueueSize];
    tQueueInt       = new int[tQueueSize];
    tQueueBool      = new bool[tQueueSize];

    for (int i = 0; i < tQueueSize; ++i) {
        tQueueDouble[i] = i + 1.1;
        tQueueInt[i]    = i + 1;
        tQueueBool[i]   = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimBusQutils::tearDown()
{
    delete [] tQueueBool;
    delete [] tQueueInt;
    delete [] tQueueDouble;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the getLatest methods
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimBusQutils::testGetLatest()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtSimBusQutils ..... 01: testGetLatest .............................";

    CPPUNIT_ASSERT(tQueueDouble[0] == SimBusQutils::getLatest(tQueueDouble));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the getSum methods
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimBusQutils::testGetSum()
{
    std::cout << "\n UtSimBusQutils ..... 02: testGetSum ................................";

    double expectedSumDouble = 0.0;
    for (int i = 0; i < tExtractionSize; ++i) {
        expectedSumDouble += tQueueDouble[i];
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSumDouble,
                                 SimBusQutils::getSum(tQueueDouble, tExtractionSize),
                                 DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the testGetAverage methods
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimBusQutils::testGetAverage()
{
    std::cout << "\n UtSimBusQutils ..... 03: testGetAverage ............................";

    double expectedAverage = 0.0;
    for (int i = 0; i < tExtractionSize; ++i) {
        expectedAverage += tQueueDouble[i];
    }
    expectedAverage /= tExtractionSize;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAverage,
                                 SimBusQutils::getAverage(tQueueDouble, tExtractionSize),
                                 DBL_EPSILON);

    /// - Test for exception thrown for zero queue size.
    try {
        SimBusQutils::getAverage(tQueueDouble, 0);
    } catch (TsOutOfBoundsException& e) {
        CPPUNIT_ASSERT( true );
    } catch ( ... ) {
        CPPUNIT_ASSERT( false );
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the isPulseHigh methods
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimBusQutils::testIsPulseHigh()
{
    std::cout << "\n UtSimBusQutils ..... 02: testIsPulseHigh ...........................";

    bool expectedBool = false;
    CPPUNIT_ASSERT_EQUAL(expectedBool,
                                 SimBusQutils::isPulseHigh(tQueueBool, tExtractionSize));

    /// - Set just one boolean to true in boolean array
    tQueueBool[tExtractionSize - 1] = true;

    expectedBool = true;
    CPPUNIT_ASSERT_EQUAL(expectedBool,
                                 SimBusQutils::isPulseHigh(tQueueBool, tExtractionSize));

    std::cout << "... Pass";
}
