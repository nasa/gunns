# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#trick setup
trick.sim_services.exec_set_trap_sigfpe(1)
#simControlPanel = trick.SimControlPanel()
#trick.add_external_application(simControlPanel)
#trickView = trick.TrickView()
#trick.add_external_application(trickView)
#trick.real_time_enable()
trick.sim_services.exec_set_terminate_time(12)
trick.exec_set_software_frame(0.0125)
trick.TMM_reduced_checkpoint(False)
trick_mm.mm.set_expanded_arrays(True)
trick_sys.sched.set_enable_freeze(True)
#trick_sys.sched.set_freeze_command(True)

#---------------------------------------------
# Initial setup
#---------------------------------------------
# Override fluid14err volumes and pressures to be small to create a loop where all nodes are
# overflowing.  Circulation is created by turning off the potential sources of all but the pot01
# link.
massOverflow.fluid14err.netInput.vol0.mInitialVolume = 1.0e-4
massOverflow.fluid14err.netInput.vol1.mInitialVolume = 1.0e-4
massOverflow.fluid14err.netInput.vol2.mInitialVolume = 1.0e-4
massOverflow.fluid14err.netInput.vol3.mInitialVolume = 1.0e-4
massOverflow.fluid14err.netInput.fluid0.mPressure = 100.0
massOverflow.fluid14err.netInput.fluid1.mPressure = 100.0
massOverflow.fluid14err.netInput.fluid2.mPressure = 100.0
massOverflow.fluid14err.netInput.fluid3.mPressure = 100.0
massOverflow.fluid14err.netInput.fluid0.mTemperature = 294.261
massOverflow.fluid14err.netInput.fluid1.mTemperature = 294.261
massOverflow.fluid14err.netInput.fluid2.mTemperature = 294.261
massOverflow.fluid14err.netInput.fluid3.mTemperature = 294.261
massOverflow.fluid14err.netInput.pot02.mSourcePressure = 0.0
massOverflow.fluid14err.netInput.pot13.mSourcePressure = 0.0
massOverflow.fluid14err.netInput.pot23.mSourcePressure = 0.0
massOverflow.fluid14err.netConfig.pot01.mMaxConductivity = 0.001
massOverflow.fluid14err.netConfig.pot02.mMaxConductivity = 0.001
massOverflow.fluid14err.netConfig.pot13.mMaxConductivity = 0.001
massOverflow.fluid14err.netConfig.pot23.mMaxConductivity = 0.001

massOverflow.fluid12err.netInput.vol0.mInitialVolume = 1.0
massOverflow.fluid12err.netInput.vol1.mInitialVolume = 1.0e-6
massOverflow.fluid12err.netInput.vol2.mInitialVolume = 1.0e-4
massOverflow.fluid12err.netInput.fluid0.mPressure = 100.0
massOverflow.fluid12err.netInput.fluid1.mPressure = 100.0
massOverflow.fluid12err.netInput.fluid2.mPressure = 100.0
massOverflow.fluid12err.netInput.fluid0.mTemperature = 275.0
massOverflow.fluid12err.netInput.fluid1.mTemperature = 275.0
massOverflow.fluid12err.netInput.fluid2.mTemperature = 275.0

massOverflow.fluid35.netConfig.msorb13des.addCompound(trick.ChemicalCompound.CO2,0.1,0.85,0.0,0.05,0.01,-1.0,True,trick.ChemicalCompound.NO_COMPOUND,True,0.0,0.1,1.0)
massOverflow.fluid35.netConfig.msorb13ad.addCompound(trick.ChemicalCompound.H2O,0.11,0.75,0.0,0.05,0.01,-1.0,True,trick.ChemicalCompound.NO_COMPOUND,False,1.0,0.0,1.0)

massOverflow.fluid48.netConfig.msorb45de.addCompound(trick.ChemicalCompound.CO2,0.1,0.85,0.0,0.05,0.01,-1.0,True,trick.ChemicalCompound.NO_COMPOUND,True,0.0,0.1,1.0)
massOverflow.fluid48.netConfig.msorb12ad.addCompound(trick.ChemicalCompound.H2O,0.11,0.75,0.0,0.05,0.01,-1.0,True,trick.ChemicalCompound.NO_COMPOUND,False,1.0,0.0,1.0)

massOverflow.fluid37.sub14.setHeatBalance(-10000.0)

# Configure sorbant segments in the GunnsFluidCdraAdsorber links
massOverflow.fluid41.cdra13.addSegment(0, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra13.addSegment(1, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra13.addSegment(2, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra13.addSegment(3, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra13.addSegment(4, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra13.addSegment(5, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra13.addSegment(6, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra13.addSegment(7, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra13.addSegment(8, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra13.addSegment(9, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra23.addSegment(0, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra23.addSegment(1, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra23.addSegment(2, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra23.addSegment(3, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra23.addSegment(4, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra23.addSegment(5, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra23.addSegment(6, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra23.addSegment(7, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra23.addSegment(8, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)
massOverflow.fluid41.cdra23.addSegment(9, trick.GunnsFluidCdraAdsorber.ZEO_5A_RK38, 0.0, 0.00216)

# Override fluid45over node 1 volume to be small so it will overflow.
massOverflow.fluid45over.netInput.vol1.mInitialVolume = 1.0e-4

#---------------------------------------------
# T-0 setup (block all flows)
#---------------------------------------------
massOverflow.fluid.netInput.cond01.mMalfBlockageFlag = True
massOverflow.fluid.netInput.cond13.mMalfBlockageFlag = True
massOverflow.fluid.netInput.cond02.mMalfBlockageFlag = True
massOverflow.fluid.netInput.cond23.mMalfBlockageFlag = True

massOverflow.fluid37.netInput.sub14.mMalfBlockageFlag = True
massOverflow.fluid37.netInput.sub14.mMalfBlockageValue = 1.0

trick.add_read(0.0, """massOverflow.fluid6.vlv1.mPathA.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid6.vlv1.mPathA.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid6.vlv1.mPathB.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid6.vlv1.mPathB.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid6.vlv2.mPathA.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid6.vlv2.mPathA.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid6.vlv2.mPathB.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid6.vlv2.mPathB.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid6.vlv3.mPathA.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid6.vlv3.mPathA.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid6.vlv3.mPathB.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid6.vlv3.mPathB.mMalfBlockageValue = 1.0""" )

trick.add_read(0.0, """massOverflow.fluid7.vlv1.mPathA.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid7.vlv1.mPathA.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid7.vlv1.mPathB.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid7.vlv1.mPathB.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid7.vlv2.mPathA.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid7.vlv2.mPathA.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid7.vlv2.mPathB.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid7.vlv2.mPathB.mMalfBlockageValue = 1.0""" )

trick.add_read(0.0, """massOverflow.fluid8.chk01.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid8.chk01.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid8.chk02.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid8.chk02.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid8.htch13.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid8.htch13.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid8.htch23.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid8.htch23.mMalfBlockageValue = 1.0""" )

trick.add_read(0.0, """massOverflow.fluid9.hxc01.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid9.hxc01.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid9.hxc02.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid9.hxc02.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid9.hxs13.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid9.hxs13.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid9.hxs23.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid9.hxs23.mMalfBlockageValue = 1.0""" )

trick.add_read(0.0, """massOverflow.fluid33.liqMembrane.mMalfMembraneDegradeFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid33.liqMembrane.mMalfMembraneDegradeValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid33.gasMembrane.mMalfMembraneDegradeFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid33.gasMembrane.mMalfMembraneDegradeValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid33.liqSource.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid33.liqSource.mMalfBlockageValue = 1.0""" )
trick.add_read(0.0, """massOverflow.fluid33.gasSource.mMalfBlockageFlag = True""" )
trick.add_read(0.0, """massOverflow.fluid33.gasSource.mMalfBlockageValue = 1.0""" )

#---------------------------------------------
# T+1 events (start all flows)
#---------------------------------------------
trick.add_read(1.0, """massOverflow.fluid.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid.cond13.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid.cond02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid.cond23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid2.source13.setFlowDemand(0.1)""" )

trick.add_read(1.0, """massOverflow.fluid3.vlv01.setPosition(1.0)""" )
trick.add_read(1.0, """massOverflow.fluid3.vlv02.setPosition(1.0)""" )
trick.add_read(1.0, """massOverflow.fluid3.pipe13.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid3.pipe23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid4.hx01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid4.hx02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid4.sensor13.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid4.sensor23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid5.qd01.setState(trick.GunnsFluidSimpleQd.CONNECTED)""" )
trick.add_read(1.0, """massOverflow.fluid5.qd02.setState(trick.GunnsFluidSimpleQd.CONNECTED)""" )
trick.add_read(1.0, """massOverflow.fluid5.leak13.setMalfLeakHole(True, 1.0)""" )
trick.add_read(1.0, """massOverflow.fluid5.leak23.setMalfLeakHole(True, 1.0)""" )

trick.add_read(1.0, """massOverflow.fluid6.vlv1.mPathA.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid6.vlv1.mPathB.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid6.vlv2.mPathA.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid6.vlv2.mPathB.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid6.vlv3.mPathA.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid6.vlv3.mPathB.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid7.vlv1.mPathA.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid7.vlv1.mPathB.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid7.vlv2.mPathA.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid7.vlv2.mPathB.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid8.chk01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid8.chk02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid8.chk01.setMalfFailTo(True,1.0)""" )
#trick.add_read(1.0, """massOverflow.fluid8.htch13.mMalfBlockageFlag = False""" )
#trick.add_read(1.0, """massOverflow.fluid8.htch23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid9.hxc01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid9.hxc02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid9.hxs13.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid9.hxs23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid10.pchg02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid10.pchg01.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid11.prv01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid11.prv02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid11.ls13.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid11.ls23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid12.htch01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid12.htch12.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid12err.htch01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid12err.htch12.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid13.reg01.setMalfFailTo(True, 1.0)""" )
trick.add_read(1.0, """massOverflow.fluid13.reg02.setMalfFailTo(True, 1.0)""" )
trick.add_read(1.0, """massOverflow.fluid13.rel13.setMalfFailTo(True, 1.0)""" )
trick.add_read(1.0, """massOverflow.fluid13.rel23.setMalfFailTo(True, 1.0)""" )

trick.add_read(1.0, """massOverflow.fluid14.pot01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid14.pot02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid14.pot13.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid14.pot23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid14err.pot01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid14err.pot02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid14err.pot13.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid14err.pot23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid15.vlv32.setPosition(1.0)""" )

trick.add_read(1.0, """massOverflow.fluid16.src10.setFlowDemand(0.01)""" )

trick.add_read(1.0, """massOverflow.fluid17.pot20.setSourcePressure(0.01)""" )
trick.add_read(1.0, """massOverflow.fluid17.tank1.editPartialPressureRate(trick.FluidProperties.GUNNS_H2O, True, 200.0, 10.0)""" )

trick.add_read(1.0, """massOverflow.fluid18.pot20.setSourcePressure(0.01)""" )
trick.add_read(1.0, """massOverflow.fluid18.bln1.editPartialPressureRate(trick.FluidProperties.GUNNS_H2O, True, 200.0, 10.0)""" )

trick.add_read(1.0, """massOverflow.fluid19.srck01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid19.srck13.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid19.turb02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid19.turb23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid20.hfor01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid20.hfor02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid20.hfval13.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid20.hfval23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid21.cont01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid21.cont02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid21.cont13.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid21.cont23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid22.jump01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid22.sock02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid22.jump13.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid22.sock23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid24.gfan01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid24.gfan12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid24.lcp34.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid24.lcp45.mMalfBlockageFlag = False""" )


trick.add_read(1.0, """massOverflow.fluid23.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid23.cond23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid25.pot30.mMalfBlockageFlag  = False""" )
trick.add_read(1.0, """massOverflow.fluid25.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid25.cond02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid25.sorb13des.mMalfEfficiencyFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid25.sorb23des.mMalfEfficiencyFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid26.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid26.cond34.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid26.cond45.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid26.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid26.evap14.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid26.pot20.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid27.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid27.cond34.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid27.react45.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid27.hreact45.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid27.hreact12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid27.react12.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid28.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid28.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid28.met0.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid28.netInput.met0.mNNominal = 2.0""")

trick.add_read(1.0, """massOverflow.fluid29.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid29.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid29.heat10.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid29.heat21.mMalfBlockageFlag = False""")

trick.add_read(1.0, """massOverflow.fluid30.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid30.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid30.v2met1.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid30.netInput.v2met1.mNNominal = 2.0""")

trick.add_read(1.0, """massOverflow.fluid31.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid31.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid31.fire1.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid32.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid32.cond02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid32.gsep13for.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid32.gsep13back.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid32.gpump23for.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid32.gpump23back.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid33.liqMembrane.mMalfMembraneDegradeFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid33.gasMembrane.mMalfMembraneDegradeFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid33.liqSource.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid33.gasSource.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid34.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid34.lpump12for.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid34.lpump12back.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid35.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid35.cond02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid35.cond23.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid35.msorb13ad.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid35.msorb13des.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid35.msorb13des.mCompounds[0].mMalfEfficiencyFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid35.msorb13des.mCompounds[0].mMalfEfficiencyValue = 1.0""" )

trick.add_read(1.0, """massOverflow.fluid36.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid36.cond34.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid36.cond45.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid36.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid36.pchg14.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid37.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid37.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid37.sub14.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid38.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid38.cond23.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid38.cond02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid38.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid38.sbound1.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid38.sbound2.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid39.cond02.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid39.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid39.reactor.setCurrent(10.0)""" )

trick.add_read(1.0, """massOverflow.fluid40.src01.setFlowDemand(0.0039)""" )
trick.add_read(1.0, """massOverflow.fluid40.cond23.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid40.cond56.mMalfBlockageFlag = False""" )
#trick.add_read(0.9, """massOverflow.fluid40.rca12.mDesorbFlag = False """)
trick.add_read(1.0, """massOverflow.fluid40.rca45.mCompounds[0].mAdsorbedMass = 0.02""" )
trick.add_read(1.0, """massOverflow.fluid40.rca45.mCompounds[1].mAdsorbedMass = 0.01""" )

trick.add_read(1.0, """massOverflow.fluid41.src01.setFlowDemand(0.01)""" )
trick.add_read(1.0, """massOverflow.fluid41.src02.setFlowDemand(0.01)""" )

trick.add_read(1.0, """massOverflow.fluid42.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid42.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid42.lsep1.mMalfBlockageFlag  = False""" )

trick.add_read(1.0, """massOverflow.fluid43.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid43.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid43.v4meta1.mMalfBlockageFlag  = False""" )
trick.add_read(1.0, """massOverflow.fluid43.v4meta1.mRespiration.mBreathsPerMinute = 12.0""" )

trick.add_read(1.0, """massOverflow.fluid44.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid44.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid44.Dhtc13.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid44.Dhtc23.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid45.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid45.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid45.pot20.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid45over.cond01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid45over.cond12.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid45over.pot20.mMalfBlockageFlag = False""" )

trick.add_read(1.0, """massOverflow.fluid46.src01.setFlowDemand(0.0039)""" )
trick.add_read(1.0, """massOverflow.fluid46.cond23.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid46.cond56.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid46.sorb45de.mMalfEfficiencyFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid46.sorb45de.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid46.sorb45de.mMalfBlockageValue = 0.0""" )

trick.add_read(1.0, """massOverflow.fluid47.src01.setFlowDemand(0.0039)""" )
trick.add_read(1.0, """massOverflow.fluid47.cond23.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid47.cond56.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid47.hsorb45de.mMalfEfficiencyFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid47.hsorb45de.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid47.hsorb45de.mMalfBlockageValue = 0.0""" )

trick.add_read(1.0, """massOverflow.fluid48.src01.setFlowDemand(0.0039)""" )
trick.add_read(1.0, """massOverflow.fluid48.src01.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid48.src01.mMalfBlockageValue = 0.0""" )
trick.add_read(1.0, """massOverflow.fluid48.cond23.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid48.cond56.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid48.msorb45de.mCompounds[0].mMalfEfficiencyFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid48.msorb45de.mCompounds[0].mMalfEfficiencyValue = 1.0""" )
trick.add_read(1.0, """massOverflow.fluid48.msorb45de.mMalfBlockageFlag = False""" )
trick.add_read(1.0, """massOverflow.fluid48.msorb45de.mMalfBlockageValue = 0.0""" )


#---------------------------------------------
# T+5  events (mid-test changes)
#---------------------------------------------
trick.add_read(5.0, """massOverflow.fluid15.vlv32.setPosition(0.0)""" )
trick.add_read(6.0, """massOverflow.fluid15.vlv24.setPosition(1.0)""" )

#---------------------------------------------
# T+10 events (stop some flows to allow settling before comparison)
#---------------------------------------------
trick.add_read(10.0, """massOverflow.fluid15.vlv24.setPosition(0.0)""" )

trick.add_read(10.0, """massOverflow.fluid16.src10.setFlowDemand(0.0)""" )

trick.add_read(10.0, """massOverflow.fluid17.pot20.setSourcePressure(0.0)""" )
trick.add_read(10.0, """massOverflow.fluid17.tank1.editPartialPressureRate(trick.FluidProperties.GUNNS_H2O)""" )

trick.add_read(10.0, """massOverflow.fluid18.pot20.setSourcePressure(0.0)""" )
trick.add_read(10.0, """massOverflow.fluid18.bln1.editPartialPressureRate(trick.FluidProperties.GUNNS_H2O)""" )

trick.add_read(10.0, """massOverflow.fluid33.liqMembrane.mMalfMembraneDegradeFlag = True""" )
trick.add_read(10.0, """massOverflow.fluid33.gasMembrane.mMalfMembraneDegradeFlag = True""" )
trick.add_read(10.0, """massOverflow.fluid33.liqSource.mMalfBlockageFlag = True""" )
trick.add_read(10.0, """massOverflow.fluid33.gasSource.mMalfBlockageFlag = True""" )

#---------------------------------------------
# Setup Data Logging
#---------------------------------------------
execfile("Log_setup/Log_setup.py")
log_setup(0.1)

#---------------------------------------------
# Call integration tests
#---------------------------------------------
trick_utest.unit_tests.enable()
trick_utest.unit_tests.set_file_name( "RUN_test/results/SIM_mass_overflow_int_test_results.xml" )
execfile("int_tests/SimTestSuite.py")
