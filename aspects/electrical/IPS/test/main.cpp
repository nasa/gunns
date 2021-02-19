#ifndef MAIN_H_
#define MAIN_H_

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsElectIps.hh"

#include <cppunit/extensions/HelperMacros.h>
int main() {

    CppUnit::TextUi::TestRunner runner;

    ///////////////////////////////////////////////////////////
    // List of subsystem functions
    ///////////////////////////////////////////////////////////

    runner.addTest(UtGunnsElectIps::suite());
    runner.run();
    return 0;
}
// @}
#endif
