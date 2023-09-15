/*
@file     GunnsOptimFactory.cpp
@brief    GUNNS Optimization Optimizer Factory implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((GunnsOptimParticleSwarm.o)
   (GunnsOptimGradientDescent.o)
   (GunnsOptimTest.o))
*/

#include <iostream>
#include "GunnsOptimFactory.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Defaults constructs this GUNNS Optimizer Factory.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimFactory::GunnsOptimFactory()
    :
    mOptimizers()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Defaults destructs this GUNNS Optimizer Factory.  Deletes the objects we created and
///           cleans up memory.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimFactory::~GunnsOptimFactory()
{
    for (unsigned int i=0; i<mOptimizers.size(); i++){
        delete mOptimizers[i];
        mOptimizers[i] = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type  (--) Enumeration of the desired optimizer type to create.
///
/// @return   GunnsOptimBase* (--) Base class pointer to the created optimizer object.
///
/// @details  Creates a GUNNS optimizer object of type specified by the argument, and returns the
///           base class pointer to the created object.  The available optimizer class types must
///           all derive from GunnsOptimBase.  Returns NULL if the given type argument is NONE or
///           invalid.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimBase* GunnsOptimFactory::createOptimizer(const OptimizerType type)
{
    switch (type) {
        case PSO : {
            GunnsOptimParticleSwarm* newOptimizer = new GunnsOptimParticleSwarm();
            mOptimizers.push_back(newOptimizer);
            return newOptimizer;
            break;
        }
        case GRADIENT_DESCENT : {
            GunnsOptimGradientDescent* newOptimizer = new GunnsOptimGradientDescent();
            mOptimizers.push_back(newOptimizer);
            return newOptimizer;
            break;
        }
        case TEST : {
            GunnsOptimTest* newOptimizer = new GunnsOptimTest();
            mOptimizers.push_back(newOptimizer);
            return newOptimizer;
            break;
        }
        case GunnsOptimFactory::NONE : {
            break;
        }
    }
    return 0;
}
