/**
@file     GunnsOptimBase.cpp
@brief    GUNNS Optimization Optimizer Base Class implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ()
*/

#include "GunnsOptimBase.hh"
#include "math/MsMath.hh"
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Monte Carlo Optimizer Base configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimBaseConfigData::GunnsOptimBaseConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Monte Carlo Optimizer Base configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimBaseConfigData::~GunnsOptimBaseConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Monte Carlo Optimizer Base class.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimBase::GunnsOptimBase()
    :
    mName(""),
    mInStatesMaster(0),
    mGlobalRunCounter(0),
    mRunCounter(0),
    mEpoch(0),
    mVerbosityLevel(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Monte Carlo Optimizer Base class.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimBase::~GunnsOptimBase()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::range_error
///
/// @details  Unlike the rest of GUNNS, here we don't use the H&S system or TsException types and
///           opt to just throw standard exceptions.  Because this MC stuff could be used to
///           optimize non-GUNNS models, the user might not want to bother setting up the H&S.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimBase::validate()
{
    /// - Throw if the monte carlo variables description is missing.
    if (not mInStatesMaster) {
        throw std::range_error(mName + " mInStatesMaster is null.");
    }

    /// - Throw if number of MC inputs is zero.
    if (mInStatesMaster->size() < 1) {
        throw std::range_error(mName + " # monte carlo variables < 1.");
    }

    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
        /// - Throw if the MC variable range max <= min.
        if (mInStatesMaster->at(i).mMaximum <= mInStatesMaster->at(i).mMinimum) {
            throw std::range_error(mName + " a monte carlo variable has max range <= min range.");
        }

        /// - Throw if the MC variable has null address.
        if (not mInStatesMaster->at(i).mAddress) {
            throw std::range_error(mName + " a monte carlo variable has null address.");
        }

        /// - Throw if the MC variable has empty name.
        if ("" == mInStatesMaster->at(i).mName) {
            throw std::range_error(mName + " a monte carlo variable has empty name.");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (--) Uniform distribution random number in the range [0, 1].
///
/// @details  Returns a uniformly-distributed random number between 0 and 1 inclusive.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsOptimBase::uniformRand() const
{
    return (1.0 * std::rand() / RAND_MAX);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] vec (--) Vector of doubles to be constrained by the MC input constraints.
///
/// @throws   std::range_error
///
/// @details  Applies the MC input variable constraints to the given vector.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimBase::constrainInputs(std::vector<double>& vec)
{
    /// - Throw if the given vector size doens't match the number of MC inputs.
    if (vec.size() != mInStatesMaster->size()) {
        throw std::range_error(mName + " constrainInputs vec argument size mismatch with number of MC inputs.");
    }
    for (unsigned int i=0; i<vec.size(); ++i) {
        mInStatesMaster->at(i).applyConstraints(&vec.at(i));
    }
}
