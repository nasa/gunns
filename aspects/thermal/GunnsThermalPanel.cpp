/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
     (GunnsThermalSource.o)
***************************************************************************************************/
#include "GunnsThermalPanel.hh"
#include "aspects/thermal/PtcsMacros.hh"
#include "math/MsMath.hh" //needed for MsMath::isInRange() in validate()
/**************************************************************************************************/
/* ConfigData class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name             (--)   name of given ThermalSource object
/// @param[in]  nodes            (--)   pointer to Gunns nodes to which object connects
/// @param[in]  tuningScalar     (--)   source's electrical-to-thermal tuning scalar
/// @param[in]  fluxDistributionFractions (--)   pointer to vector of fractions that describe the
///                                              flux distribution to each port
/// @param[in]  absorptivity     (--)   panel's optical absorptivity (0-1)
/// @param[in]  area             (m2)   surface area of panel object
///
/// @details  Constructs the ThermalPanel Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPanelConfigData::GunnsThermalPanelConfigData(
        const std::string& name,
        GunnsNodeList* nodes,
        const double tuningScalar,
        std::vector<double>* fluxDistributionFractions,
        const double absorptivity,
        const double area)
    :
    GunnsThermalSourceConfigData(name, nodes, tuningScalar, fluxDistributionFractions),
    cAbsorptivity(absorptivity),
    cSurfaceArea(area)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  (--)  Object to copy
///
/// @details  Copy Constructs the ThermalPanel Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPanelConfigData::GunnsThermalPanelConfigData(const GunnsThermalPanelConfigData& that)
    :
    GunnsThermalSourceConfigData(that),
    cAbsorptivity(that.cAbsorptivity),
    cSurfaceArea(that.cSurfaceArea)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the ThermalPanel Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPanelConfigData::~GunnsThermalPanelConfigData()
{
    // nothing to do
}
/**************************************************************************************************/
/* InputData class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    malfBlockageFlag       (--)   Blockage malfunction flag
/// @param[in]    malfBlockageValue      (--)   Blockage malfunction fractional value (0-1)
/// @param[in]    demandedFlux           (W)    Initial demanded flux of the link
/// @param[in]    malfFluxOverrideFlag   (--)   Override flux malfunction flag
/// @param[in]    malfFluxOverrideValue  (W)    Override flux malfunction value
///
/// @details  Default constructs this ThermalPanel input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPanelInputData::GunnsThermalPanelInputData( const bool   malfBlockageFlag,
                                                        const double malfBlockageValue,
                                                        const double demandedFlux,
                                                        const bool   malfFluxOverrideFlag,
                                                        const double malfFluxOverrideValue )
    :
    GunnsThermalSourceInputData(malfBlockageFlag,
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
/// @details  Copy constructs this ThermalPanel input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPanelInputData::GunnsThermalPanelInputData(const GunnsThermalPanelInputData& that)
    :
    GunnsThermalSourceInputData(that)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this ThermalPanel input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPanelInputData::~GunnsThermalPanelInputData()
{
    // nothing to do
}
/**************************************************************************************************/
/* GunnsThermalPanel class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the ThermalPanel Object with default values
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPanel::GunnsThermalPanel()
    :
    GunnsThermalSource(),
    mIncidentHeatFluxPerArea(0.0),
    mAbsorptivity(0.0),
    mSurfaceArea(0.0)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the ThermalPanel Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPanel::~GunnsThermalPanel()
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   configData   (--)  Link Config Data
/// @param[in]   inputData    (--)  Link Input Data
/// @param[in]   networkLinks (--)  Reference to the Solver Links
/// @param[in]   portsVector  (--)  Vector of port numbers this Panel is linked to
///
/// @throw    TsInitializationException (by base class)
///
/// @details  This initializes the link and sets up its connectivity to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPanel::initialize(const GunnsThermalPanelConfigData& configData,
                                   const GunnsThermalPanelInputData&  inputData,
                                   std::vector<GunnsBasicLink*>&      networkLinks,
                                   std::vector<int>*                  portsVector)
{
    /// - Calls ThermalSource::initialize to set up ports.
    GunnsThermalSource::initialize(configData, inputData, networkLinks, portsVector);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Validate configuration data.
    validate(configData);

    /// - Configure the panel with an optical absorptivity.
    mAbsorptivity = configData.cAbsorptivity;

    /// - Configure the panel with its surface area.
    mSurfaceArea = configData.cSurfaceArea;

    /// - Set mIncidentHeatFluxPerArea, nominally set by the SimBus, to an initial value.
    mIncidentHeatFluxPerArea = mDemandedFlux / (mAbsorptivity * mSurfaceArea);

    /// - Set init flag on successful validation.
    mInitFlag = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--) Configuration data.
///
/// @throw    TsInitializationException
///
/// @details  Validates the initialization of this GUNNS ThermalPanel link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPanel::validate(const GunnsThermalPanelConfigData& configData) const
{
    /// - Throw an exception if absorptivity < 0 or > 1.
    TS_PTCS_IF_ERREX(!MsMath::isInRange(0.0, configData.cAbsorptivity, 1.0), TsInitializationException,
            "invalid config data", "Absorptivity out of range (0-1).");

    /// - Throw an exception if surface area < 0.
    TS_PTCS_IF_ERREX(configData.cSurfaceArea < 0.0, TsInitializationException,
            "invalid config data", "SurfaceArea less than zero.");
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPanel::restartModel()
{
    /// - Reset the base class.
    GunnsThermalSource::restartModel();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt  (s)  Integration time step (not used)
///
/// @details  This updates the link during a calculation frame, creating a fixed flux onto one or
///           more nodes.
///
///           The flux is calculated by multiplying the incident flux, set by the SimBus, times
///           scalars based on panel configuration data (optical absorptivity and surface area).
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPanel::updateState(const double dt)
{
    /// - Call parent class updateState().
    GunnsThermalSource::updateState(dt);

    /// - Set the GUNNS flux demand as the incident flux, set by the SimBus, times scalars
    ///   based on panel configuration data.
    mDemandedFlux = mAbsorptivity * mSurfaceArea * mIncidentHeatFluxPerArea;
}
