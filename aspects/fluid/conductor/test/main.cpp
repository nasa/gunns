////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_GUNNS_FLUID_CONDUCTOR   Conductor Unit tests
/// @ingroup   UT_GUNNS_FLUID
/// @copyright Copyright 2021 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test classes for GUNNS fluid conductor link models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsFluidCheckValve.hh"
#include "UtGunnsFluidHatch.hh"
#include "UtGunnsFluidHeatExchanger.hh"
#include "UtGunnsFluidHxDynHtc.hh"
#include "UtGunnsFluidCondensingHx.hh"
#include "UtGunnsFluidCondensingHxSeparator.hh"
#include "UtGunnsFluidLeak.hh"
#include "UtGunnsFluidPipe.hh"
#include "UtGunnsFluidPressureSensitiveValve.hh"
#include "UtGunnsFluidRegulatorValve.hh"
#include "UtGunnsFluidReliefValve.hh"
#include "UtGunnsFluidSensor.hh"
#include "UtGunnsFluidValve.hh"
#include "UtGunnsFluid3WayValve.hh"
#include "UtGunnsFluid3WayCheckValve.hh"
#include "UtGunnsFluidBalancedPrv.hh"
#include "UtGunnsFluidLiquidWaterSensor.hh"
#include "UtGunnsFluidEqConductor.hh"
#include "UtGunnsFluidPhaseChangeConductor.hh"
#include "UtGunnsFluidTypeChangeConductor.hh"
#include "UtGunnsFluidSimpleRocket.hh"
#include "UtGunnsFluidSimpleQd.hh"
#include "UtGunnsGasTurbine.hh"
#include "UtGunnsDriveShaftSpotter.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param     int     --  not used
/// @param     char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for GUNNS fluid conductor link model unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int, char**)
{
    CppUnit::TextTestRunner runner;

    runner.addTest(UtGunnsFluidSensor::suite());
    runner.addTest(UtGunnsFluidLiquidWaterSensor::suite());
    runner.addTest(UtGunnsFluidLeak::suite());
    runner.addTest(UtGunnsFluidValve::suite());
    runner.addTest(UtGunnsFluidHatch::suite());
    runner.addTest(UtGunnsFluidCheckValve::suite());
    runner.addTest(UtGunnsFluidPressureSensitiveValve::suite());
    runner.addTest(UtGunnsFluidBalancedPrv::suite());
    runner.addTest(UtGunnsFluidRegulatorValve::suite());
    runner.addTest(UtGunnsFluidReliefValve::suite());
    runner.addTest(UtGunnsFluidPipe::suite());
    runner.addTest(UtGunnsFluidHeatExchanger::suite());
    runner.addTest(UtGunnsFluidHxDynHtc::suite());
    runner.addTest(UtGunnsFluidCondensingHx::suite());
    runner.addTest(UtGunnsFluidCondensingHxSeparator::suite());
    runner.addTest(UtGunnsFluid3WayValve::suite());
    runner.addTest(UtGunnsFluid3WayCheckValve::suite());
    runner.addTest(UtGunnsFluidEqConductor::suite());
    runner.addTest(UtGunnsFluidPhaseChangeConductor::suite());
    runner.addTest(UtGunnsFluidTypeChangeConductor::suite());
    runner.addTest(UtGunnsFluidSimpleRocket::suite());
    runner.addTest(UtGunnsDriveShaftSpotter::suite());
    runner.addTest(UtGunnsGasTurbine::suite());
    runner.addTest(UtGunnsFluidSimpleQd::suite());

    runner.run();

    return 0;
}
