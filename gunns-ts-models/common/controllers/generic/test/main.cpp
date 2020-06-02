////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_CONTROLLERGENERIC Unit tests for generic controllers
/// @ingroup   UT_CONTROLLER
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test classes for generic controller models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtTsPidController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     (--)  argument count (unused)
/// @param    argv  char**  (--)  arguments (unused)
///
/// @return   (--)  status (always 0)
///
/// @details  Main for fluid controller unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc __attribute__((unused)), char** argv __attribute__((unused))) {

    CppUnit::TextTestRunner runner;

    runner.addTest( UtTsPidController::suite() );

    runner.run();

    return 0;
}

