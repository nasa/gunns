/// @copyright Copyright 2024 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
#ifndef MAIN_H_
#define MAIN_H_

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "UtGunnsElectSwitchUtil.hh"
#include "UtGunnsElectSwitchUtil2.hh"
#include "UtGunnsElectSelector.hh"
#include "UtGunnsElectUserLoadSwitch.hh"
#include "UtGunnsElectUserLoadSwitch2.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    --   none
///
/// @return    --  status (always 0)
///
/// @details  Main for Gunns: Electrical components unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

    CppUnit::TextTestRunner runner;

    ///////////////////////////////////////////////////////////
    // List of subsystem functions
    ///////////////////////////////////////////////////////////

    runner.addTest(UtGunnsElectSwitchUtil::suite());
    runner.addTest(UtGunnsElectSwitchUtil2::suite());
    runner.addTest(UtGunnsElectSelector::suite());
    runner.addTest(UtGunnsElectUserLoadSwitch::suite());
    runner.addTest(UtGunnsElectUserLoadSwitch2::suite());

    runner.run();

    return 0;
}

/// @}
#endif
