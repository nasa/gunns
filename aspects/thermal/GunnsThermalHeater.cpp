/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   (
    (GunnsThermalSource.o)
   )
***************************************************************************************************/
#include "GunnsThermalHeater.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "aspects/thermal/PtcsMacros.hh"
/**************************************************************************************************/
/* ConfigData class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name             (--)   name of given ThermalHeater object
/// @param[in]  nodes            (--)   pointer to Gunns nodes to which object connects
/// @param[in]  tuningScalar     (--)   heater's electrical-to-thermal tuning scalar
/// @param[in]  fluxDistributionFractions (--)   pointer to vector of fractions that describe the
///                                              flux distribution to each port
///
/// @details  Constructs this GunnsThermalHeater ConfigData
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalHeaterConfigData::GunnsThermalHeaterConfigData(
        const std::string& name,
        GunnsNodeList* nodes,
        const double tuningScalar,
        std::vector<double>* fluxDistributionFractions)
    :
    GunnsThermalSourceConfigData(name, nodes, tuningScalar, fluxDistributionFractions)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  (--)   input source to copy
///
/// @details  Copy Constructs this GunnsThermalHeater ConfigData
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalHeaterConfigData::GunnsThermalHeaterConfigData(const GunnsThermalHeaterConfigData& that)
    :
    GunnsThermalSourceConfigData(that)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs this GunnsThermalHeater ConfigData Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalHeaterConfigData::~GunnsThermalHeaterConfigData()
{
    // nothing to do
}

/**************************************************************************************************/
/* InputData class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    malfBlockageFlag      (--)   Blockage malfunction flag
/// @param[in]    malfBlockageValue     (--)   Blockage malfunction fractional value (0-1)
/// @param[in]    demandedFlux          (W)    Initial demanded flux of the link
/// @param[in]    malfFluxOverrideFlag  (--)   Heat flux override malfunction flag
/// @param[in]    malfFluxOverrideValue (W)    Heat flux override malfunction value
///
/// @details  Default constructs this GunnsThermalHeater InputData.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalHeaterInputData::GunnsThermalHeaterInputData(const bool   malfBlockageFlag,
                                                         const double malfBlockageValue,
                                                         const double demandedFlux,
                                                         const bool   malfFluxOverrideFlag,
                                                         const double malfFluxOverrideValue)
    :
    GunnsThermalSourceInputData(
            malfBlockageFlag,
            malfBlockageValue,
            demandedFlux,
            malfFluxOverrideFlag,
            malfFluxOverrideValue)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  (--)  Object to copy
///
/// @details  Copy constructs this GunnsThermalHeater InputData.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalHeaterInputData::GunnsThermalHeaterInputData(const GunnsThermalHeaterInputData& that)
    :
    GunnsThermalSourceInputData(that)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GunnsThermalHeater InputData.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalHeaterInputData::~GunnsThermalHeaterInputData()
{
    // nothing to do
}

/**************************************************************************************************/
/* GunnsThermalHeater class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructs the GunnsThermalHeater with default values
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalHeater::GunnsThermalHeater()
    :
    GunnsThermalSource(),
    mPowerElectrical(0.0)    //  (W)  actual electrical power of the heater
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructs the GunnsThermalHeater
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalHeater::~GunnsThermalHeater()
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]        configData   (--)  Source configuration data
/// @param[in]        inputData    (--)  Source input data
/// @param[in,out]    networkLinks (--)  Network link vector
/// @param[in]        portsVector  (--)  Vector of port numbers this Heater is linked to
///
/// @throw    TsInitializationException by base class initialize() or validate()
///
/// @details  This initializes the GunnsThermalHeater and sets up its connectivity to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalHeater::initialize(const GunnsThermalHeaterConfigData& configData,
                                    const GunnsThermalHeaterInputData&  inputData,
                                    std::vector<GunnsBasicLink*>&       networkLinks,
                                    std::vector<int>*                   portsVector)
{
    /// - calls ThermalSource::initialize to setup ports
    GunnsThermalSource::initialize(configData, inputData, networkLinks, portsVector);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Set init flag on successful validation.
    mInitFlag = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalHeater::restartModel()
{
    /// - Reset the base class.
    GunnsThermalSource::restartModel();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt  (s)  Integration time step [not used]
///
/// @details  Updates the state of the GunnsThermalHeater. If this heater is attached to a heater
///           control card, the network's HeaterCardInterface sets the electrical power using the
///           setPowerElectrical() method. Otherwise, the power should be written by the sim_bus.
///
///           The main purpose of the method is simply to set the link's flux to be equal to the
///           electric power in Watts times a electrical-to-thermal tuning scalar. The flux
///           demand is the actual heat rate in Watts that will be added to the thermal system.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalHeater::updateState(const double dt)
{
    /// - Call parent class updateState().
    GunnsThermalSource::updateState(dt);

    /// - Set mDemandedFlux as input electrical power. mPowerElectrical
    ///   should either be set by the simbus or a HeaterCardInterface object.
    mDemandedFlux = mPowerElectrical;
}
