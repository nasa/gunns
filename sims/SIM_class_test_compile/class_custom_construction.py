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
# This is a dictionary of extra code needed by the gen_sm.py script to construct some classes
# in the Trick sim object.  This defines arguments to custom constructors, include statements
# for those included types, etc.
class_custom_construction = {}

#
type = 'GunnsDynSingleRigidStateDeriv'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "aspects/dynamics/GunnsDynSingleRigidStateDeriv.hh"\n'
class_custom_construction[type]['declare']   = '        GunnsDynRotationalState       state;     /**< (1) argument to code under test */\n' \
                                             + '        GunnsDynSingleRigidMassProps  massProps; /**< (1) argument to code under test */\n' \
                                             + '        GunnsDynSingleRigidStateDeriv code;      /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(state, massProps)\n'

#
type = 'GunnsFluidPressureHead'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "aspects/fluid/potential/GunnsFluidPressureHead.hh"\n' \
                                             + '##include "core/GunnsFluidPotential.hh"\n'
class_custom_construction[type]['declare']   = '        GunnsFluidPotential    link; /**< (1) argument to code under test */\n' \
                                             + '        GunnsFluidPressureHead code; /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(link)\n'

#
type = 'GunnsThermalCapacitorHeatQueues'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "aspects/thermal/GunnsThermalCapacitorHeatQueues.hh"\n'
class_custom_construction[type]['declare']   = '        GunnsThermalCapacitor           link; /**< (1) argument to code under test */\n' \
                                             + '        GunnsThermalCapacitorHeatQueues code; /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(link)\n'

#
type = 'GunnsBasicFlowOrchestrator'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "core/GunnsBasicFlowOrchestrator.hh"\n'
class_custom_construction[type]['declare']   = '        GunnsBasicFlowOrchestrator code; /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(2, 2)\n'

#
type = 'GunnsBasicIslandAnalyzer'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "core/GunnsBasicIslandAnalyzer.hh"\n' \
                                             + '##include "core/GunnsBasicNode.hh"\n'
class_custom_construction[type]['declare']   = '        GunnsNodeList            nodes; /**< (1) argument to code under test */\n' \
                                             + '        GunnsBasicIslandAnalyzer code;  /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(nodes)\n'

#
type = 'GunnsFluidFlowIntegrator'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "core/GunnsFluidFlowIntegrator.hh"\n' \
                                             + '##include "core/GunnsFluidConductor.hh"\n'
class_custom_construction[type]['declare']   = '        GunnsFluidConductor      link; /**< (1) argument to code under test */\n' \
                                             + '        GunnsFluidFlowIntegrator code; /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(link)\n'

#
type = 'GunnsFluidFlowOrchestrator'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "core/GunnsFluidFlowOrchestrator.hh"\n'
class_custom_construction[type]['declare']   = '        GunnsFluidFlowOrchestrator code; /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(2, 2)\n'

#
type = 'GunnsFluidIslandAnalyzer'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "core/GunnsFluidIslandAnalyzer.hh"\n' \
                                             + '##include "core/GunnsBasicNode.hh"\n'
class_custom_construction[type]['declare']   = '        GunnsNodeList            nodes; /**< (1) argument to code under test */\n' \
                                             + '        GunnsFluidIslandAnalyzer code;  /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(nodes)\n'

#
type = 'GunnsFluidVolumeMonitor'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "core/GunnsFluidVolumeMonitor.hh"\n' \
                                             + '##include "core/GunnsFluidCapacitor.hh"\n'
class_custom_construction[type]['declare']   = '        GunnsFluidCapacitor     link;  /**< (1) argument to code under test */\n' \
                                             + '        GunnsNodeList           nodes; /**< (1) argument to code under test */\n' \
                                             + '        GunnsFluidVolumeMonitor code;  /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(nodes, link)\n'

#
type = 'GunnsBasicSuperNetwork'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "core/network/GunnsBasicSuperNetwork.hh"\n'
class_custom_construction[type]['declare']   = '        GunnsBasicSuperNetwork code; /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code("code")\n'

#
type = 'GunnsFluidSuperNetwork'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "core/network/GunnsFluidSuperNetwork.hh"\n'
class_custom_construction[type]['declare']   = '        GunnsFluidSuperNetwork code; /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code("code")\n'

# GunnsMultiLinkConnector needs a GunnsNetworkBase*, but GunnsNetworkBase is abstract, so 
# we must define a network type just for this test.  Don't want to use any other networks in the
# gunns repo because they will include other classes that could satisfy missing library dependencies
# in GunnsMultiLinkConnector, which would degrade this test.
type = 'GunnsMultiLinkConnector'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "core/network/GunnsMultiLinkConnector.hh"\n' \
                                             + 'class TestNetwork : public GunnsNetworkBase\n' \
                                             + '{\n' \
                                             + '    public:\n' \
                                             + '        virtual void initNodes(const std::string& name) {;}\n' \
                                             + '        virtual void initNetwork() {;}\n' \
                                             + '        TestNetwork() : GunnsNetworkBase("test", 0, 0) {;};\n' \
                                             + '        virtual ~TestNetwork() {;};\n' \
                                             + '};\n'
class_custom_construction[type]['declare']   = '        TestNetwork             network; /**< (1) argument to code under test */\n' \
                                             + '        GunnsMultiLinkConnector code;    /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(&network)\n'

# This is here because class name doesn't match the file name.
type = 'tinyxml'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "ms-utils/parsing/tinyxml/tinyxml.hh"\n'
class_custom_construction[type]['declare']   = '        TiXmlDocument code; /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = ''

# This is here because class name doesn't match the file name.
type = 'tinystr'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "ms-utils/parsing/tinyxml/tinystr.hh"\n'
class_custom_construction[type]['declare']   = '        TiXmlString code; /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = ''

#
type = 'TsHsConsolePlugin'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "ms-utils/simulation/hs/TsHsConsolePlugin.hh"\n'
class_custom_construction[type]['declare']   = '        TsHsConsolePlugin code; /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(0)\n'

#
type = 'TsHsMsg'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "ms-utils/simulation/hs/TsHsMsg.hh"\n'
class_custom_construction[type]['declare']   = '        TsHsMsg code; /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(TS_HS_DEBUG, "subsys")\n'

#
type = 'TsHsTextPlugin'
class_custom_construction[type] = {}
class_custom_construction[type]['include']   = '##include "ms-utils/simulation/hs/TsHsTextPlugin.hh"\n'
class_custom_construction[type]['declare']   = '        TsHsTextPlugin code; /**< (1) code under test */\n'
class_custom_construction[type]['construct'] = '            :\n' \
                                             + '            code(0)\n'
