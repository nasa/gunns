/**
@file
@brief    GUNNS Electrical User Load Base Spotter implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsNetworkSpotter.o))
*/

#include "GunnsElectUserLoad.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name               (--)  Instance name for self-identification in messages.
/// @param[in]  underVoltageLimit  (V)   Low voltage limit for operation.
/// @param[in]  fuseCurrentLimit   (amp) Current above which the fuse blows.
///
/// @details  Default constructs this GUNNS Electrical User Load Base Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadConfigData::GunnsElectUserLoadConfigData(const std::string& name,
                                                           const double       underVoltageLimit,
                                                           const double       fuseCurrentLimit)
    :
    GunnsNetworkSpotterConfigData(name),
    mUnderVoltageLimit(underVoltageLimit),
    mFuseCurrentLimit(fuseCurrentLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical User Load Base Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadConfigData::~GunnsElectUserLoadConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  initialMode     (--)  Initial mode.
/// @param[in]  initialVoltage  (V)   Initial input voltage.
///
/// @details  Default constructs this GUNNS Electrical User Load Base Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadInputData::GunnsElectUserLoadInputData(const int    initialMode,
                                                         const double initialVoltage)
    :
    GunnsNetworkSpotterInputData(),
    mInitialMode(initialMode),
    mInitialVoltage(initialVoltage)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical User Load Base Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadInputData::~GunnsElectUserLoadInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default constructs this GUNNS Electrical User Load Base Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoad::GunnsElectUserLoad()
    :
    GunnsNetworkSpotter(),
    mSupplyVoltage(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical User Load Base Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoad::~GunnsElectUserLoad()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  This method does nothing because this spotter has no function prior to the network
///           solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoad::stepPreSolver(const double dt __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  This method does nothing because this spotter has no function after the network
///           solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoad::stepPostSolver(const double dt __attribute__((unused)))
{
    // nothing to do
}
