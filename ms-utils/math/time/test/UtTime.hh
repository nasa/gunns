/********************************* TRICK HEADER *******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Unit Test Time)

ASSUMPTIONS AND LIMITATIONS:
    ((TBD))

LIBRARY DEPENDENCY:
    (
       (math/time/Time.o)
    )

PROGRAMMERS:
    ((Dan Jordan) (METECS) (April 2012) )
*******************************************************************************/
#ifndef UtTime_EXISTS
#define UtTime_EXISTS

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "math/time/Time.hh"

#include "software/unit_test/TsAssert.hh"

class UtTime: public CppUnit::TestFixture
{

    private:

        CPPUNIT_TEST_SUITE(UtTime);

        CPPUNIT_TEST(TestGetTimeOfDay);

        CPPUNIT_TEST_SUITE_END();

    public:
        Time myTime;
        void TestGetTimeOfDay();
};

#endif
