#ifndef Time_EXISTS
#define Time_EXISTS
/**
@defgroup TSM_UTILITIES_MATH_TIME_TIME Time Computations
@ingroup TSM_UTILITIES_MATH_TIME

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
    (Provides time functions )

REFERENCE:
    (none)

ASSUMPTIONS AND LIMITATIONS:
    (none)

LIBRARY DEPENDENCY:
    ((Time.o))

PROGRAMMERS:
    ((Dan Jordan) (METECS) (April 2012)

@{
*/


// System includes
#include <sys/time.h>
#include <stdio.h>

/// @brief Provides intersect functions for a line segment against primitive 3D shapes
class Time
{
    public:
        /// @brief Get's time of day from system call in seconds
        double getTimeOfDay();

    private:
        timeval tv;

};

/// @}

#endif // Time_EXISTS
