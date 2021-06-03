#!/usr/bin/python
#
# @copyright Copyright 2021 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, June 2021, --, Initial implementation.}
# @revs_end
#
# This is a dictionary of extra code needed to construct some classes in the Trick sim object.
# This will include arguments to custom constructors, include statements for those included
# types, etc.
class_custom_construction = {}

# classes what need this:
# GunnsDynSingleRigidStateDeriv
# GunnsFluidPressureHead
# GunnsThermalCapacitorHeatQueues
# GunnsFluidFLowOrchestrator
# GunnsFluidVolumeMonitor
# GunnsFluidLink
# GunnsFluidIslandAnalyzer
# GunnsBasicIslandAnalyzer
# GunnsFluidFlowIntegrator
# GunnsBasicFLowOrchestrator
# GunnsBasicSuperNetwork
# GunnsFluidSuperNetwork
# GunnsMultiLinkConnector
# tinyxmlparser (different class name, should we handle?)
# TsHsTextPlugin
# TsHsMsg
# TsHsConsolePlugin

#
class_custom_construction['class'] = {}
class_custom_construction['class']['includes'] = '##include...'
class_custom_construction['class']['args']     = '0.0, 1, ...'
