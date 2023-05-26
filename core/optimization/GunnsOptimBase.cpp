/**
@file     GunnsOptimBase.cpp
@brief    GUNNS Optimization Optimizer Base Class implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ()
*/

/// - GUNNS inlcudes:
#include "math/MsMath.hh"
#include "strings/Strings.hh"

/// - System includes:
#include <cfloat>
#include <iostream> //TODO testing
#include <sstream>
#include <fstream>
#include "GunnsOptimBase.hh"

//TODO
GunnsOptimBaseConfigData::GunnsOptimBaseConfigData()
{
    // nothing to do
}

//TODO
GunnsOptimBaseConfigData::~GunnsOptimBaseConfigData()
{
    // nothing to do
}

//TODO
GunnsOptimBase::GunnsOptimBase()
    :
    mInStatesMaster(0),
    mGlobalRunCounter(0),
    mRunCounter(0),
    mEpoch(0)
{
    // nothing to do
}

//TODO
GunnsOptimBase::~GunnsOptimBase()
{
    // nothing to do
}

//TODO inline
void GunnsOptimBase::setConfigData(const GunnsOptimBaseConfigData* configData)
{
    // nothing to do
}

//TODO delete, dead code but useful?
double GunnsOptimBase::computeVectorMagnitude(const std::vector<double>& vec) const
{
    double rss = 0.0;
    for (unsigned int i=0; i<vec.size(); ++i) {
        rss += vec.at(i) * vec.at(i);
    }
    return sqrt(rss);
}

//TODO delete, dead code but useful?
//TODO does nothing if given vector magnitude < DBL_EPSILON
/// @param[in, out] vec
/// @param[in]      magnitude
void GunnsOptimBase::normalizeVector(std::vector<double>& vec, const double magnitude) const
{
    const double vecMag = computeVectorMagnitude(vec);
    if (vecMag > DBL_EPSILON) {
        const double factor = magnitude / vecMag;
        for (unsigned int i=0; i<vec.size(); ++i) {
            vec.at(i) *= factor;
        }
    }
}
