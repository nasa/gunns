/**
@defgroup  UT_UTILITIES_MATH_ELEM_FUNCTIONS Elementary Functions Unit Tests
@ingroup   UT_UTILITIES_MATH

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details   This is the unit test main for the math elementary functions classes.

@{

*/

#include <cppunit/ui/text/TestRunner.h>

#include "UtLambertW.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    CppUnit::TextTestRunner runner;

    runner.addTest( UtLambertW::suite() );

    runner.run();

    return 0;
}

/// @}

