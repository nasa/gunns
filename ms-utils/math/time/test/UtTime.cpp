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

#include "UtTime.hh"

void UtTime::TestGetTimeOfDay()
{

    double timeNow = myTime.getTimeOfDay();
    CPPUNIT_ASSERT(timeNow > 0.0);

}

