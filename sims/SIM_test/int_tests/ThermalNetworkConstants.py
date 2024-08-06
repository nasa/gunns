# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#===============================================================================================
# SIM_test Thermal Network Testing Constants.
#===============================================================================================
# Nominal Values & tolerances for initial & final Node potentials
thermalFinalP     = [ 295.0, 295.0, 0.7, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ]
thermalTolP       = 0.0001

# Nominal values # tolerances for link flux (Watts)
fluxConductor     = -0.3
fluxDemand        = -0.7
tolFlux         = 1.0e-5

#--- define thermal network node name dictionary
thermalNodeNames  = { "Node 0":0,    \
                      "Node 1":1,    \
                      "Node 2":2,    \
                      "Node 3":3,    \
                      "Node 4":4,    \
                      "Node 5":5,    \
                      "Node 6":6,    \
                      "Node 7":7,    \
                      "Node 8":8,    \
                      "Node 9":9,    \
                      "Node 10":10,    \
                      "Node 11":11,    \
                      "Node 12":12,    \
                      "Node 13":13    \
                    }
