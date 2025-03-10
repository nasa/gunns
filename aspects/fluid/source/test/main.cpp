////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_GUNNS_FLUID_SOURCE   Source Unit tests
/// @ingroup   UT_GUNNS_FLUID
/// @copyright Copyright 2021 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test classes for GUNNS fluid source link models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsFluidAdsorber.hh"
#include "UtGunnsFluidAdsorptionCompound.hh"
#include "UtGunnsFluidEvaporation.hh"
#include "UtGunnsFluidHotAdsorber.hh"
#include "UtGunnsFluidHeater.hh"
#include "UtGunnsFluidHotReactor.hh"
#include "UtGunnsFluidFireSource.hh"
#include "UtGunnsFluidMetabolic.hh"
#include "UtGunnsFluidMetabolic2.hh"
#include "UtGunnsFluidMetabolic3.hh"
#include "UtGunnsFluidMultiAdsorber.hh"
#include "UtGunnsFluidMultiSeparator.hh"
#include "UtGunnsFluidReactor.hh"
#include "UtGunnsFluidPhaseChangeSource.hh"
#include "UtGunnsFluidSeparatorGas.hh"
#include "UtGunnsFluidSeparatorLiquid.hh"
#include "UtGunnsFluidSimpleH2Redox.hh"
#include "UtGunnsFluidSublimator.hh"
#include "UtGunnsGasDisplacementPump.hh"
#include "UtGunnsLiquidDisplacementPump.hh"
#include "UtGunnsFluidSimpleH2Redox.hh"
#include "UtGunnsFluidSourceBoundary.hh"
#include "UtGunnsFluidSorptionBed.hh"
#include "UtGunnsFluidSelectiveMembrane.hh"

////////////////////////////////////////////////////////////////////////////////////////////
/// @param    int     --  not used
/// @param    char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for Plumbing Reactor unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////
int main(int, char**) {
    CppUnit::TextTestRunner runner;

    runner.addTest(UtGunnsFluidAdsorber::suite());
    runner.addTest(UtGunnsFluidAdsorptionCompound::suite());
    runner.addTest(UtGunnsFluidEvaporation::suite());
    runner.addTest(UtGunnsFluidHotAdsorber::suite());
    runner.addTest(UtGunnsFluidReactor::suite());
    runner.addTest(UtGunnsFluidHotReactor::suite());
    runner.addTest(UtGunnsFluidFireSource::suite());
    runner.addTest(UtGunnsFluidMetabolic::suite());
    runner.addTest(UtGunnsFluidMetabolic2::suite());
    runner.addTest(UtGunnsFluidMetabolic3::suite());
    runner.addTest(UtGunnsFluidMultiAdsorber::suite());
    runner.addTest(UtGunnsFluidMultiSeparator::suite());
    runner.addTest(UtGunnsFluidPhaseChangeSource::suite());
    runner.addTest(UtGunnsFluidSeparatorGas::suite());
    runner.addTest(UtGunnsFluidSeparatorLiquid::suite());
    runner.addTest(UtGunnsFluidSimpleH2Redox::suite());
    runner.addTest(UtGunnsFluidSublimator::suite());
    runner.addTest(UtGunnsFluidHeater::suite());
    runner.addTest(UtGunnsGasDisplacementPump::suite());
    runner.addTest(UtGunnsLiquidDisplacementPump::suite());
    runner.addTest(UtGunnsFluidSourceBoundary::suite());
    runner.addTest(UtGunnsFluidSorptionBed::suite());
    runner.addTest(UtGunnsFluidSelectiveMembrane::suite());

    runner.run();

    return 0;
}
