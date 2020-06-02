#ifndef MAIN_H_
#define MAIN_H_

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

//#include "UT_SwitchCardElect.hh"
#include "UtSwitchCard.hh"
#include "../SwitchCardElect.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    --   none
///
/// @return    --  status (always 0)
///
/// @details  Main for Gunns: Elecrical components unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

    SwitchCardElect lSwitchCardElect;

    //CppUnit::TextUi::TestRunner runner;
    CppUnit::TextTestRunner runner;

    ///////////////////////////////////////////////////////////
    // List of subsystem functions
    ///////////////////////////////////////////////////////////

    runner.addTest(UtSwitchCard::suite());

    runner.run();

    return 0;
}

/// @}
#endif
