/// @copyright Copyright 2023 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
#ifndef MAIN_H_
#define MAIN_H_

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "UT_Switch.hh"
#include "UtGunnsElectSwitchUtil.hh"
#include "UtGunnsElectSwitchUtil2.hh"
#include "UT_SwitchElect.hh"
#include "UtGunnsElectSelector.hh"
#include "UtGunnsElectUserLoadSwitch.hh"
#include "UtGunnsElectUserLoadSwitch2.hh"
#include "aspects/electrical/Switch/Switch.hh"
#include "aspects/electrical/Switch/SwitchElect.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    --   none
///
/// @return    --  status (always 0)
///
/// @details  Main for Gunns: Electrical components unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

    Switch lSwitch;

    CppUnit::TextTestRunner runner;

    ///////////////////////////////////////////////////////////
    // List of subsystem functions
    ///////////////////////////////////////////////////////////

    runner.addTest(UT_Switch::suite());
    runner.addTest(UtGunnsElectSwitchUtil::suite());
    runner.addTest(UtGunnsElectSwitchUtil2::suite());
    runner.addTest(UT_SwitchElect::suite());
    runner.addTest(UtGunnsElectSelector::suite());
    runner.addTest(UtGunnsElectUserLoadSwitch::suite());
    runner.addTest(UtGunnsElectUserLoadSwitch2::suite());

    runner.run();

    return 0;
}

/// @}
#endif
