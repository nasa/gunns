# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#===============================================================================================
# SIM_test Basic Network Testing Constants.
#===============================================================================================
# Nominal Values & tolerances for initial & final Node potentials
basicInitP      = [ 119.989,   109.081 ]
basicFinalP     = [ 119.989,   109.081 ]
basicTolP       = 0.001

# Nominal values # tolerances for conductor flux (amps)
flux            = 1.09081e-4
tolFlux         = 1.0e-8

#--- define basic network node name dictionary
basicNodeNames  = { "Node 0":0,    \
                    "Node 1":1     \
                  }
