# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#===============================================================================================
# SIM_test Fluid Network Testing Constants.
#===============================================================================================
# Nominal Values & tolerances for initial & final Node pressures, temperatures, and mass fraction
initP      = [ 101.325,   101.325 ]
initT      = [ 290.0,     290.003 ]
initMfH2   = [   8.0e-6,    0.0   ]
initMfCH4  = [   2.3e-6,    0.0   ]
initMfCO2  = [   7.661e-6,  0.0   ]
initMfH2O  = [   2.367e-6,  0.0   ]
initMfO2   = [   0.239926,  0.0   ]
initMfN2   = [   0.750015,  1.0   ]

finalP     = [ 101.354,   101.354 ]
finalT     = [ 290.0,     290.086 ]
finalMfH2  = [   8.0e-6,    0.0   ]
finalMfCH4 = [   2.3e-6,    0.0   ]
finalMfCO2 = [   7.661e-6,  0.0   ]
finalMfH2O = [   2.367e-6,  0.0   ]
finalMfO2  = [   0.239926,  0.0   ]
finalMfN2  = [   0.750015,  1.0   ]

tolP       = 0.001
tolT       = 0.001
tolMf      = 0.001

# Nominal values # tolerances for initial & final conductor flow rates
initConductor1Mdot  = 0.0
initLeakMdot        = 2.442e-6

finalConductor1Mdot = 0.0
finalLeakMdot       = 2.4425e-6

tolMdot             = 1.0e-7

#--- define fluid network node name dictionary
nodeNames = { "Node 0":0,    \
              "Node 1":1     \
            }
