/**
@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details   This is the unit test main for the generic converter model
@{
*/

#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsElectConverterInput.hh"
#include "UtGunnsElectConverterOutput.hh"

///////////////////////////////////////////////////////////
// Include all subsystem tests.
///////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    CppUnit::TextTestRunner runner;

    runner.addTest(UtGunnsElectConverterInput::suite());
    runner.addTest(UtGunnsElectConverterOutput::suite());

    runner.run();

    return 0;
}
