/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
     (GunnsThermalSource.o)
*/

#include "GunnsThermalMultiPanel.hh"
#include "aspects/thermal/PtcsMacros.hh"
#include "math/MsMath.hh" //needed for MsMath::isInRange() in validate()

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                      (--) Name of given ThermalSource object.
/// @param[in] nodes                     (--) Pointer to Gunns nodes to which object connects.
/// @param[in] tuningScalar              (--) Source's electrical-to-thermal tuning scalar.
/// @param[in] fluxDistributionFractions (--) Pointer to vector of fractions that describe the flux distribution to each port.
/// @param[in] absorptivity0             (--) Absorptivity (0-1) of exterior panel to 0th radiant source.
/// @param[in] absorptivity1             (--) Absorptivity (0-1) of exterior panel to 1st radiant source.
/// @param[in] absorptivity2             (--) Absorptivity (0-1) of exterior panel to 2nd radiant source.
/// @param[in] absorptivity3             (--) Absorptivity (0-1) of exterior panel to 3rd radiant source.
/// @param[in] absorptivity4             (--) Absorptivity (0-1) of exterior panel to 4th radiant source.
/// @param[in] surfaceArea               (m2) Surface area of panel object.
///
/// @details  Constructs the Thermal Multi-Panel Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalMultiPanelConfigData::GunnsThermalMultiPanelConfigData(
        const std::string&   name,
        GunnsNodeList*       nodes,
        const double         tuningScalar,
        std::vector<double>* fluxDistributionFractions,
        const double         absorptivity0,
        const double         absorptivity1,
        const double         absorptivity2,
        const double         absorptivity3,
        const double         absorptivity4,
        const double         surfaceArea)
    :
    GunnsThermalSourceConfigData(name, nodes, tuningScalar, fluxDistributionFractions),
    cAbsorptivity(),
    cSurfaceArea(surfaceArea)
{
    cAbsorptivity[0] = absorptivity0;
    cAbsorptivity[1] = absorptivity1;
    cAbsorptivity[2] = absorptivity2;
    cAbsorptivity[3] = absorptivity3;
    cAbsorptivity[4] = absorptivity4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Thermal Multi-Panel Config Data Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalMultiPanelConfigData::~GunnsThermalMultiPanelConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag      (--)   Blockage malfunction flag
/// @param[in] malfBlockageValue     (--)   Blockage malfunction fractional value (0-1)
/// @param[in] demandedFlux          (W)    (not used)
/// @param[in] malfFluxOverrideFlag  (--)   Override flux malfunction flag
/// @param[in] malfFluxOverrideValue (W)    Override flux malfunction value
/// @param[in] viewScalar0           (--)   Initial view scalar (0-1) to account for occultation or facing to 0th radiant source.
/// @param[in] viewScalar1           (--)   Initial view scalar (0-1) to account for occultation or facing to 1st radiant source.
/// @param[in] viewScalar2           (--)   Initial view scalar (0-1) to account for occultation or facing to 2nd radiant source.
/// @param[in] viewScalar3           (--)   Initial view scalar (0-1) to account for occultation or facing to 3rd radiant source.
/// @param[in] viewScalar4           (--)   Initial view scalar (0-1) to account for occultation or facing to 4th radiant source.
/// @param[in] incidentFlux0         (W/m2) Initial flux of 0th radiant source.
/// @param[in] incidentFlux1         (W/m2) Initial flux of 1st radiant source.
/// @param[in] incidentFlux2         (W/m2) Initial flux of 2nd radiant source.
/// @param[in] incidentFlux3         (W/m2) Initial flux of 3rd radiant source.
/// @param[in] incidentFlux4         (W/m2) Initial flux of 4th radiant source.
///
/// @details  Default constructs this Thermal Multi-Panel input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalMultiPanelInputData::GunnsThermalMultiPanelInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue,
        const double demandedFlux,
        const bool   malfFluxOverrideFlag,
        const double malfFluxOverrideValue,
        const double viewScalar0,
        const double viewScalar1,
        const double viewScalar2,
        const double viewScalar3,
        const double viewScalar4,
        const double incidentFlux0,
        const double incidentFlux1,
        const double incidentFlux2,
        const double incidentFlux3,
        const double incidentFlux4)
    :
    GunnsThermalSourceInputData(malfBlockageFlag,
                                malfBlockageValue,
                                demandedFlux,
                                malfFluxOverrideFlag,
                                malfFluxOverrideValue),
    iViewScalar(),
    iIncidentFlux()
{
    iViewScalar[0]   = viewScalar0;
    iViewScalar[1]   = viewScalar1;
    iViewScalar[2]   = viewScalar2;
    iViewScalar[3]   = viewScalar3;
    iViewScalar[4]   = viewScalar4;
    iIncidentFlux[0] = incidentFlux0;
    iIncidentFlux[1] = incidentFlux1;
    iIncidentFlux[2] = incidentFlux2;
    iIncidentFlux[3] = incidentFlux3;
    iIncidentFlux[4] = incidentFlux4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Thermal Multi-Panel input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalMultiPanelInputData::~GunnsThermalMultiPanelInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Thermal Multi-Panel Object with default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalMultiPanel::GunnsThermalMultiPanel()
    :
    GunnsThermalSource(),
    mAbsorptivity(),
    mSurfaceArea(0.0),
    mViewScalar(),
    mIncidentFlux()
{
    for (int i = 0; i < 5; ++i) {
        mAbsorptivity[i] = 0.0;
        mViewScalar[i]   = 0.0;
        mIncidentFlux[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Thermal Multi-Panel Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalMultiPanel::~GunnsThermalMultiPanel()
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
void GunnsThermalMultiPanel::initialize(const GunnsThermalMultiPanelConfigData& configData,
                                        const GunnsThermalMultiPanelInputData&  inputData,
                                        std::vector<GunnsBasicLink*>&           networkLinks,
                                        std::vector<int>*                       portsVector)
{
    /// - Calls ThermalSource::initialize to set up ports.
    GunnsThermalSource::initialize(configData, inputData, networkLinks, portsVector);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize from config & input data.
    for (int i = 0; i < 5; ++i) {
        mAbsorptivity[i] = configData.cAbsorptivity[i];
        mViewScalar[i]   = inputData.iViewScalar[i];
        mIncidentFlux[i] = inputData.iIncidentFlux[i];
    }
    mSurfaceArea = configData.cSurfaceArea;

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--) Configuration data.
/// @param[in]      inputData   (--) Input data.
///
/// @throw    TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Thermal Multi-Panel link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalMultiPanel::validate(const GunnsThermalMultiPanelConfigData& configData,
                                      const GunnsThermalMultiPanelInputData&  inputData) const
{
    /// - Throw an exception if any absorptivity < 0 or > 1.
    for (int i = 0; i < 5; ++i) {
        TS_PTCS_IF_ERREX(!MsMath::isInRange(0.0, configData.cAbsorptivity[i], 1.0),
                TsInitializationException,
                "invalid config data", "An absorptivity value is out of range (0-1).");
    }

    /// - Throw an exception if surface area < 0.
    TS_PTCS_IF_ERREX(configData.cSurfaceArea < 0.0, TsInitializationException,
            "invalid config data", "SurfaceArea less than zero.");

    /// - Throw an exception if any view scalar < 0 or > 1.
    for (int i = 0; i < 5; ++i) {
        TS_PTCS_IF_ERREX(!MsMath::isInRange(0.0, inputData.iViewScalar[i], 1.0),
                TsInitializationException,
                "invalid input data", "A view scalar value is out of range (0-1).");
    }

    /// - Throw an exception if any incident flux < 0.
    for (int i = 0; i < 5; ++i) {
        TS_PTCS_IF_ERREX(inputData.iIncidentFlux[i] < 0.0,
                TsInitializationException,
                "invalid input data", "An incident flux is less than zero..");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalMultiPanel::restartModel()
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
///           The flux is calculated by multiplying the incident fluxes and view scalar, set by the
///           SimBus, times the optical absorptivity config data for each radiant source, summing
///           all of those radiant source absorbed flux per area, and then multiplying the whole
///           thing by panel surface area.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalMultiPanel::updateState(const double dt)
{
    /// - Call parent class updateState().
    GunnsThermalSource::updateState(dt);

    /// - Sum the absorbed flux per unit area from each radiant source.
    double fluxPerArea = 0.0;
    for (int i = 0; i < 5; ++i) {
        fluxPerArea += mAbsorptivity[i] * mViewScalar[i] * mIncidentFlux[i];
    }

    /// - Find the total flux.
    mDemandedFlux = fluxPerArea * mSurfaceArea;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   index  (--)  Array index for absorptivity
///
/// @return      double (--)  Current value of absorptivity with a specified array index.
///
/// @details  Returns the absorptivity with a specified array index.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsThermalMultiPanel::getAbsorptivity(const int index) const
{
    if (MsMath::isInRange(0, index, 4)) {
        return mAbsorptivity[index];
    } else {
        TS_PTCS_WARNING ("Array index out of bounds. First element returned.");
        return mAbsorptivity[0];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   index  (--)  Array index for mViewScalar
///
/// @return      double (--)  Current value of mViewScalar with a specified array index.
///
/// @details  Returns the mViewScalar with a specified array index.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsThermalMultiPanel::getViewScalar(const int index) const
{
    if (MsMath::isInRange(0, index, 4)) {
        return mViewScalar[index];
    } else {
        TS_PTCS_WARNING ("Array index out of bounds. First element returned.");
        return mViewScalar[0];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   index  (--)   Array index for mIncidentFlux
///
/// @return      double (W/m2) Current value of mIncidentFlux with a specified array index.
///
/// @details  Returns the mIncidentFlux with a specified array index.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsThermalMultiPanel::getIncidentFlux(const int index) const
{
    if (MsMath::isInRange(0, index, 4)) {
        return mIncidentFlux[index];
    } else {
        TS_PTCS_WARNING ("Array index out of bounds. First element returned.");
        return mIncidentFlux[0];
    }
}
