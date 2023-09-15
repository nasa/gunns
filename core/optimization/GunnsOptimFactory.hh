#ifndef GunnsOptimFactory_EXISTS
#define GunnsOptimFactory_EXISTS
/**
@file     GunnsOptimFactory.hh
@brief    GUNNS Optimization Optimizer Factory declarations

@defgroup  TSM_GUNNS_CORE_OPTIM_FACTORY    GUNNS Optimization Optimizer Factory
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Refer to class details below.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsOptimFactory.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-05) (Initial))

@{
 */

// Optimizer type includes
#include "GunnsOptimParticleSwarm.hh"
#include "GunnsOptimGradientDescent.hh"
#include "GunnsOptimTest.hh"

// System includes
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
///@brief    GUNNS Optimizer Factory
///
///@details  This class creates GUNNS optimization optimizer objects. The program using these
///          optimizers, such as a Trick Monte Carlo input file, calls the createOptimizer method
///          with the desired type enumeration, and receives a pointer to the new object.  This
///          creates, owns and deletes the objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsOptimFactory
{
    public:
        /// @brief Enumeration of available optimizer types.
        enum OptimizerType{
            PSO              = 0,  ///< Particle Swarm Optimization
            GRADIENT_DESCENT = 1,  ///< Gradient Descent Optimization
            TEST             = 2,  ///< Optimizer Test
            NONE             = 3,  ///< No optimizer, and number of options
        };
        /// @brief Default constructor.
        GunnsOptimFactory();
        /// @brief Default destructor.
        virtual ~GunnsOptimFactory();
        /// @brief Method to create new optimizer, and return its base class pointer.
        GunnsOptimBase* createOptimizer(const OptimizerType type);

    private:
        std::vector<GunnsOptimBase*> mOptimizers; /**< (--) trick_chkpnt_io(**) Vector containing pointers to all created optimizers, used for clearing memory in deletion. */
};

/// @}

#endif
