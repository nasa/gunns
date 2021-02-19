/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (See GunnsFluidHeater.hh)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((core/GunnsFluidLink.o))

PROGRAMMERS:
   ((Jason Harvey) (L3) (Initial) (2012-03))
**************************************************************************************************/

#include "GunnsFluidHeater.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                   (--)    Name of object
/// @param[in,out] nodes                  (--)    Pointer to nodes
/// @param[in]     heaterEfficiency       (--)    (0-1) Heater efficiency
///
/// @details  Default constructs this GUNNS Fluid Heater link model configuration data with
///           arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeaterConfigData::GunnsFluidHeaterConfigData(const std::string& name,
                                                       GunnsNodeList*     nodes,
                                                       const double       heaterEfficiency)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mHeaterEfficiency(heaterEfficiency)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that   (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Heater link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeaterConfigData::GunnsFluidHeaterConfigData(const GunnsFluidHeaterConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mHeaterEfficiency(that.mHeaterEfficiency)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Heater link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeaterConfigData::~GunnsFluidHeaterConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    malfBlockageFlag     (--)   Blockage malfunction flag
/// @param[in]    malfBlockageValue    (--)   Blockage malfunction fractional value (0-1)
/// @param[in]    heaterPower          (W)    Initial heater power consumed
///
/// @details  Default constructs this GUNNS Fluid Heater link model input data with
///           arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeaterInputData::GunnsFluidHeaterInputData(const bool   malfBlockageFlag,
                                                     const double malfBlockageValue,
                                                     const double heaterPower)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mHeaterPower           (heaterPower)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  (--)   Source to copy from.
///
/// @details  Copy constructs this GUNNS Fluid Heater link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeaterInputData::GunnsFluidHeaterInputData(const GunnsFluidHeaterInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mHeaterPower           (that.mHeaterPower)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Heater link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeaterInputData::~GunnsFluidHeaterInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Heater link model with obviously invalid
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeater::GunnsFluidHeater()
    :
    GunnsFluidLink         (NPORTS),
    mHeatFlux              (0.0),
    mHeaterPower           (0.0),
    mHeaterEfficiency      (0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Heater link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeater::~GunnsFluidHeater()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
/// @param[in,out]  links        (--)  Link vector.
/// @param[in]      port0        (--)  feedwater supply port map index.
/// @param[in]      port1        (--)  vapor vent port map index.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Heater link model with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeater::initialize(const GunnsFluidHeaterConfigData& configData,
                                  const GunnsFluidHeaterInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&     links,
                                  const int                         port0,
                                  const int                         port1)
{
    /// - First initialize & validate parent.
    int ports[2] = {port0, port1};
    GunnsFluidLink::initialize(configData, inputData, links, ports);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize config & input parameters.
    mHeatFlux         = 0.0;
    mHeaterPower      = inputData.mHeaterPower;
    mHeaterEfficiency = configData.mHeaterEfficiency;

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Heater link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeater::validate(const GunnsFluidHeaterConfigData& configData,
                                const GunnsFluidHeaterInputData&  inputData __attribute__((unused))) const
{
    /// - Throw an exception if heater efficiency not between 0 and 1.
    if (!MsMath::isInRange(0.0, configData.mHeaterEfficiency, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Heater efficiency not between 0 and 1.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeater::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Execution time step (not used).
///
/// @returns  void
///
/// @details  Method to update the fluid heater state during the network step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeater::step(const double dt __attribute__((unused)))
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Calculate actual heat flux from consumed heater power, efficiency, and blockage malf.
    mHeatFlux = mHeaterPower * mHeaterEfficiency;
    if (mMalfBlockageFlag) {
        mHeatFlux *= (1.0 - mMalfBlockageValue);
    }

    /// - Force the contributions to the system of equations to zero to prevent accidentally
    ///   corrupting the network system of equations with values lurking in our admittance matrix &
    ///   source vector.  This link never directly contributes to the network solution.  The
    ///   interaction between this heat and the node pressures is accounted for by any capacitor
    ///   links on the nodes during subsequent passes, using their normal thermal capacitance logic.
    mAdmittanceUpdate    = false;
    mAdmittanceMatrix[0] = 0.0;
    mAdmittanceMatrix[1] = 0.0;
    mAdmittanceMatrix[2] = 0.0;
    mAdmittanceMatrix[3] = 0.0;
    mSourceVector[0]     = 0.0;
    mSourceVector[1]     = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Execution time step (not used).
///
/// @returns  void
///
/// @details  Method to update the link flows during the network step.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidHeater::computeFlows(const double dt __attribute__((unused)))
{
    /// - Port flow direction are always none since this link doesn't depend on any node state.
    mPortDirections[0] = NONE;
    mPortDirections[1] = NONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Execution time step (not used).
///
/// @returns  void
///
/// @details  Method to transport heatflux between the ports.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidHeater::transportFlows(const double dt __attribute__((unused)))
{
    /// - Transport heat flux between the ports.
    mNodes[0]->collectHeatFlux(-mHeatFlux);
    mNodes[1]->collectHeatFlux( mHeatFlux);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] heaterPower (W) Heater Power.
///
/// @returns  void
///
/// @details  Method to set the heater power.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeater::setHeaterPower(const double heaterPower)
{
    mHeaterPower = heaterPower;
}
