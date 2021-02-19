/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsTripLogic.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
/// @return    --  status (always 0)
/// @details  Main for Gunns unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    CppUnit::TextTestRunner runner;

    runner.addTest(UtGunnsTripLogic::suite());
    runner.run();

    return 0;
}

/// @}
