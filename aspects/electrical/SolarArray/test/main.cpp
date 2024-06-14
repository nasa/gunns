/// @copyright Copyright 2024 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
#include <cppunit/ui/text/TestRunner.h>

#include "UtSolarString.hh"
#include "UtPVCellCompanionModel.hh"
#include "UtSolarSection.hh"
#include "UtSolarArray.hh"
#include "UtGunnsElectPvString.hh"
#include "UtGunnsElectPvString2.hh"
#include "UtGunnsElectPvSection.hh"
#include "UtGunnsElectPvArray.hh"
#include "UtGunnsElectPvRegTrips.hh"
#include "UtGunnsElectPvRegSensors.hh"
#include "UtGunnsElectPvRegShunt.hh"
#include "UtGunnsElectPvRegConv.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
/// @return    --  status (always 0)
/// @details  Main for Gunns unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    CppUnit::TextTestRunner runner;

    runner.addTest(UtSolarString::suite());
    runner.addTest(UtPVCellCompanionModel::suite());
    runner.addTest(UtSolarSection::suite());
    runner.addTest(UtSolarArray::suite());
    runner.addTest(UtGunnsElectPvString::suite());
    runner.addTest(UtGunnsElectPvString2::suite());
    runner.addTest(UtGunnsElectPvSection::suite());
    runner.addTest(UtGunnsElectPvArray::suite());
    runner.addTest(UtGunnsElectPvRegTrips::suite());
    runner.addTest(UtGunnsElectPvRegSensors::suite());
    runner.addTest(UtGunnsElectPvRegShunt::suite());
    runner.addTest(UtGunnsElectPvRegConv::suite());
    runner.run();

    return 0;
}

/// @}
