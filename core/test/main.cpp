/**
@defgroup  UT_GUNNS   GUNNS Unit Tests
@ingroup   UT_TS_MODELS

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details   This is the unit test main for the General-Use Nodal Network Solver (GUNNS) system.

@{

LIBRARY DEPENDENCY:
(
)

*/

#include <cppunit/ui/text/TestRunner.h>

#include "UtGunns.hh"
#include "UtGunnsBasicNode.hh"
#include "UtGunnsBasicLink.hh"
#include "UtGunnsBasicFlowOrchestrator.hh"
#include "UtGunnsBasicConductor.hh"
#include "UtGunnsBasicPotential.hh"
#include "UtGunnsBasicCapacitor.hh"
#include "UtGunnsBasicSource.hh"
#include "UtGunnsBasicSocket.hh"
#include "UtGunnsBasicJumper.hh"
#include "UtGunnsBasicJumperPlug.hh"
#include "UtGunnsBasicExternalSupply.hh"
#include "UtGunnsBasicExternalDemand.hh"
#include "UtGunnsBasicFlowController.hh"
#include "UtGunnsBasicIslandAnalyzer.hh"
#include "UtGunnsDistributed2WayBusBase.hh"
#include "UtGunnsFluidUtils.hh"
#include "UtGunnsFluidNode.hh"
#include "UtGunnsFluidLink.hh"
#include "UtGunnsFluidFlowOrchestrator.hh"
#include "UtGunnsFluidConductor.hh"
#include "UtGunnsFluidPotential.hh"
#include "UtGunnsFluidCapacitor.hh"
#include "UtGunnsFluidSource.hh"
#include "UtGunnsFluidSocket.hh"
#include "UtGunnsFluidJumper.hh"
#include "UtGunnsFluidJumperPlug.hh"
#include "UtGunnsFluidExternalSupply.hh"
#include "UtGunnsFluidExternalDemand.hh"
#include "UtGunnsFluidDistributedIf.hh"
#include "UtGunnsFluidDistributed2WayBus.hh"
#include "UtGunnsFluidShadow.hh"
#include "UtGunnsFluidFlowController.hh"
#include "UtGunnsFluidIslandAnalyzer.hh"
#include "UtGunnsNetworkSpotter.hh"
#include "UtGunnsMinorStepLog.hh"
#include "UtGunnsFluidFlowIntegrator.hh"
#include "UtGunnsFluidVolumeMonitor.hh"
#include "UtGunnsSensorAnalogWrapper.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for Gunns unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    CppUnit::TextTestRunner runner;

    runner.addTest( UtGunns::suite() );
    runner.addTest( UtGunnsBasicNode::suite() );
    runner.addTest( UtGunnsBasicLink::suite() );
    runner.addTest( UtGunnsBasicFlowOrchestrator::suite() );
    runner.addTest( UtGunnsBasicConductor::suite() );
    runner.addTest( UtGunnsBasicPotential::suite() );
    runner.addTest( UtGunnsBasicCapacitor::suite() );
    runner.addTest( UtGunnsBasicSource::suite() );
    runner.addTest( UtGunnsBasicSocket::suite() );
    runner.addTest( UtGunnsBasicJumper::suite() );
    runner.addTest( UtGunnsBasicJumperPlug::suite() );
    runner.addTest( UtGunnsBasicExternalSupply::suite() );
    runner.addTest( UtGunnsBasicExternalDemand::suite() );
    runner.addTest( UtGunnsBasicFlowController::suite());
    runner.addTest( UtGunnsBasicIslandAnalyzer::suite() );
    runner.addTest( UtGunnsDistributed2WayBusBase::suite() );
    runner.addTest( UtGunnsFluidUtils::suite() );
    runner.addTest( UtGunnsFluidNode::suite() );
    runner.addTest( UtGunnsFluidLink::suite() );
    runner.addTest( UtGunnsFluidFlowOrchestrator::suite() );
    runner.addTest( UtGunnsFluidConductor::suite() );
    runner.addTest( UtGunnsFluidPotential::suite() );
    runner.addTest( UtGunnsFluidCapacitor::suite() );
    runner.addTest( UtGunnsFluidSource::suite() );
    runner.addTest( UtGunnsFluidSocket::suite() );
    runner.addTest( UtGunnsFluidJumper::suite() );
    runner.addTest( UtGunnsFluidJumperPlug::suite() );
    runner.addTest( UtGunnsFluidExternalSupply::suite() );
    runner.addTest( UtGunnsFluidExternalDemand::suite() );
    runner.addTest( UtGunnsFluidDistributed2WayBus::suite() );
    runner.addTest( UtGunnsFluidDistributedIf::suite() );
    runner.addTest( UtGunnsFluidShadow::suite() );
    runner.addTest( UtGunnsFluidFlowController::suite());
    runner.addTest( UtGunnsFluidIslandAnalyzer::suite() );
    runner.addTest( UtGunnsNetworkSpotter::suite() );
    runner.addTest( UtGunnsMinorStepLog::suite() );
    runner.addTest( UtGunnsFluidFlowIntegrator::suite() );
    runner.addTest( UtGunnsFluidVolumeMonitor::suite() );
    runner.addTest( UtGunnsSensorAnalogWrapper::suite() );

    runner.run();

    return 0;
}

/// @}
