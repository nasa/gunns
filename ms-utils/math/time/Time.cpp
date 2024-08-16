/************************ TRICK HEADER *****************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Provides time functions )

REFERENCE:
    (none)

ASSUMPTIONS AND LIMITATIONS:
    (none)

PROGRAMMERS:
    ((Dan Jordan) (METECS) (April 2012)
********************************************************************************************/


// TS includes
#include "Time.hh"


double Time::getTimeOfDay()
{
    double seconds, microseconds;
    gettimeofday (&tv, NULL);

    seconds = (double)tv.tv_sec;
    microseconds = (double)tv.tv_usec;

    /* Add seconds and microseconds together for result in seconds */
    double currentWallTime = ( seconds + (microseconds / 1000000.0) );
    return(currentWallTime);

}
