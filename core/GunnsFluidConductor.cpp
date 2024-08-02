/**
@file
@brief    GUNNS Fluid Conductor Link implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Fluid Conductor.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
    (
     (GunnsFluidLink.o)
    )

PROGRAMMERS:
   ((Jason Harvey) (L3) (2011-02) (Design))
*/
#include "GunnsFluidConductor.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--) Link name
/// @param[in] nodes                (--) Network nodes array
/// @param[in] maxConductivity      (m2) Maximum possible effective conductivity of the link
/// @param[in] expansionScaleFactor (--) Scale factor for isentropic cooling across the link (0-1)
/// @param[in] pressureExponent     (--) Exponent on the flow equation pressure term
///
/// @details  Default GUNNS Fluid Conductor Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidConductorConfigData::GunnsFluidConductorConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       maxConductivity,
        const double       expansionScaleFactor,
        const double       pressureExponent)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mMaxConductivity(maxConductivity),
    mExpansionScaleFactor(expansionScaleFactor),
    mPressureExponent(pressureExponent)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy
///
/// @details  Copy constructs this Gunns Fluid Conductor configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidConductorConfigData::GunnsFluidConductorConfigData(
        const GunnsFluidConductorConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mMaxConductivity(that.mMaxConductivity),
    mExpansionScaleFactor(that.mExpansionScaleFactor),
    mPressureExponent(that.mPressureExponent)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Conductor Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidConductorConfigData::~GunnsFluidConductorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
///
/// @details  Default GUNNS Fluid Conductor Input Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidConductorInputData::GunnsFluidConductorInputData(const bool   malfBlockageFlag,
                                                           const double malfBlockageValue)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy
///
/// @details  Copy constructs this Fluid Conductor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidConductorInputData::GunnsFluidConductorInputData(const GunnsFluidConductorInputData& that)
    :
    GunnsFluidLinkInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Conductor Input Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidConductorInputData::~GunnsFluidConductorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Gunns Fluid Conductor Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidConductor::GunnsFluidConductor()
    :
    GunnsFluidLink(NPORTS),
    mEffectiveConductivity(0.0),
    mMaxConductivity(0.0),
    mSystemConductance(0.0),
    mExpansionScaleFactor(0.0),
    mPressureExponent(0.0),
    mTuneMode(GunnsFluidUtils::OFF),
    mTuneMassFlow(0.0),
    mTuneVolFlow(0.0),
    mTuneDeltaT(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Fluid Conductor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidConductor::~GunnsFluidConductor()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) GUNNS Fluid Conductor Config Data
/// @param[in]     inputData    (--) GUNNS Fluid Conductor Input Data
/// @param[in,out] networkLinks (--) Network links
/// @param[in]     port0        (--) Network port 0
/// @param[in]     port1        (--) Network port 1
///
/// @throws   TsInitializationException
///
/// @details  This method initializes this Fluid Conductor with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidConductor::initialize(const GunnsFluidConductorConfigData& configData,
                                     const GunnsFluidConductorInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        networkLinks,
                                     const int                            port0,
                                     const int                            port1)
{
    /// - Initialize the parent class.
    int ports[2] = {port0, port1};
    GunnsFluidLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize class attributes.
    mMaxConductivity       = configData.mMaxConductivity;
    mEffectiveConductivity = mMaxConductivity;
    mExpansionScaleFactor  = configData.mExpansionScaleFactor;
    mPressureExponent      = configData.mPressureExponent;
    mSystemConductance     = 0.0;
    mTuneMode              = GunnsFluidUtils::OFF;
    mTuneMassFlow          = 0.0;
    mTuneVolFlow           = 0.0;
    mTuneDeltaT            = 0.0;

    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Conductor initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidConductor::validate() const
{
    /// - Issue an error on max conductivity < 0.
    if (mMaxConductivity < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has max conductivity < 0.");
    }

    /// - Issue an error on expansion scale factor < 0 or > 1.
    if (mExpansionScaleFactor < 0.0 || mExpansionScaleFactor > 1.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has expansion scale factor < 0 or > 1.");
    }

    /// - Issue an error on pressure exponent < 0.5 or > 1.
    if (mPressureExponent < 0.5 || mPressureExponent > 1.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has pressure exponent < 0.5 or > 1.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidConductor::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mEffectiveConductivity = 0.0;
    mSystemConductance     = 0.0;
    mTuneMode              = GunnsFluidUtils::OFF;
    mTuneMassFlow          = 0.0;
    mTuneVolFlow           = 0.0;
    mTuneDeltaT            = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Calls step on the fluid conductor to update the state and compute a new conductivity
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidConductor::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Default the effective conductivity to the maximum, then call the virtual updateState
    ///   method so a derived model can calculate a new conductivity.
    mEffectiveConductivity = mMaxConductivity;
    updateState(dt);

    /// - Handle requests to automatically tune link parameters.
    switch (mTuneMode) {
        case (GunnsFluidUtils::MASS) :
            tuneFlow(mTuneMassFlow);
            break;

        case (GunnsFluidUtils::TRUE_VOL) :
            if (mTuneVolFlow >= 0.0) {
                tuneFlow(mTuneVolFlow * mNodes[0]->getOutflow()->getDensity());
            } else {
                tuneFlow(mTuneVolFlow * mNodes[1]->getOutflow()->getDensity());
            }
            break;

        case (GunnsFluidUtils::STD_VOL) :
            tuneFlow(mTuneVolFlow * UnitConversion::STD_DENSITY_AIR);
            break;

        case (GunnsFluidUtils::DELTA_T) :
            mExpansionScaleFactor = GunnsFluidUtils::predictExpansionScaleFactor(mTuneDeltaT,
                                            mNodes[0]->getOutflow(),
                                            mNodes[1]->getOutflow());
            mTuneMode = GunnsFluidUtils::OFF;
            break;

        default :
            mTuneMode = GunnsFluidUtils::OFF;
            break;   // mTuneMode = OFF, do nothing
    }

    /// - Set the Link Effective Conductance based on the effective conductivity and the blockage
    ///   fraction.
    if (mMalfBlockageFlag) {
        mEffectiveConductivity *= (1.0 - mMalfBlockageValue);
    }

    mSystemConductance = MsMath::limitRange(0.0, linearizeConductance(), mConductanceLimit);
    buildConductance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Applies a linearization of the fluid momentum equation to create the non-linear
///           relationship between flow and pressure over multiple network steps.
///
/// @returns  double  (kg*mol/kPa/s)  Linearized molar conductance.
///
/// @note     This method is virtual for the derived classes needing a different linearization or
///           none at all.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidConductor::linearizeConductance()
{
    return GunnsFluidUtils::computeAdmittance(mEffectiveConductivity,
                                              mMinLinearizationPotential,
                                              mNodes[0]->getOutflow(),
                                              mNodes[1]->getOutflow(),
                                              mPressureExponent);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the Admittance Matrix for the link
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidConductor::buildConductance()
{
    if (fabs(mAdmittanceMatrix[0] - mSystemConductance) > 0.0) {
        mAdmittanceMatrix[0]   =  mSystemConductance;
        mAdmittanceMatrix[1]   = -mAdmittanceMatrix[0];
        mAdmittanceMatrix[2]   = -mAdmittanceMatrix[0];
        mAdmittanceMatrix[3]   =  mAdmittanceMatrix[0];
        mAdmittanceUpdate = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Computes the flow rate across this link and schedules flow demand from source nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidConductor::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = getDeltaPotential();

    /// - Compute molar flow rate.
    computeFlux();

    /// - Set port flow directions and schedule flow from source nodes.
    if (mFlux > DBL_EPSILON) {
        mPortDirections[0] = SOURCE;
        mPortDirections[1] = SINK;
        mNodes[0]->scheduleOutflux(mFlux);
    } else if (mFlux < -DBL_EPSILON) {
        mPortDirections[0] = SINK;
        mPortDirections[1] = SOURCE;
        mNodes[1]->scheduleOutflux(-mFlux);
    } else {
        mPortDirections[0] = NONE;
        mPortDirections[1] = NONE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Updates final flow properties and transports fluid from source to sink nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidConductor::transportFlows(const double dt)
{
    /// - Convert the molar flow rate to mass rate using the molecular weight of the source node.
    int    sourcePort    = determineSourcePort(mFlux, 0, 1);
    double sourceDensity =         mNodes[sourcePort]->getOutflow()->getDensity();
    mFlowRate            = mFlux * mNodes[sourcePort]->getOutflow()->getMWeight();

    /// - Calculate true volumetric flow rate from the mass flow rate, using the density of the
    ///   source node.
    if (sourceDensity > DBL_EPSILON) {
        mVolFlowRate = mFlowRate / sourceDensity;
    } else {
        mVolFlowRate = 0.0;
    }

    /// - Calculate hydraulic power.
    computePower();

    /// - Isentropic gas expansion cooling across a link.  We only do this for links that define an
    ///   internal fluid.  We store the expanded gas temperature in the internal fluid before giving
    ///   it to the derived model to add its contributions.
    if (mInternalFluid) {

        if (mFlowRate > m100EpsilonLimit) {

            /// - Copy the source node contents into the internal fluid, but only if it isn't the
            ///   network's vacuum node.  If it is, then leave the internal fluid alone and let the
            ///   derived class own it.
            if (0 == sourcePort) {
                mInternalFluid->setState(mNodes[0]->getOutflow());
            }
            mInternalFluid->setTemperature(
                    GunnsFluidUtils::computeIsentropicTemperature(mExpansionScaleFactor,
                                                                  mPotentialVector[0],
                                                                  mPotentialVector[1],
                                                                  mInternalFluid));

        } else if (mFlowRate < -m100EpsilonLimit) {

            if (1 == sourcePort) {
                mInternalFluid->setState(mNodes[1]->getOutflow());
            }
            mInternalFluid->setTemperature(
                    GunnsFluidUtils::computeIsentropicTemperature(mExpansionScaleFactor,
                                                                  mPotentialVector[1],
                                                                  mPotentialVector[0],
                                                                  mInternalFluid));
        }

        /// - If the derived model has declared that it modifies the fluid passing through it (by
        ///   instantiating mInternalFluid), then call the derived model to do the update.
        updateFluid(dt, mFlowRate);
    }

    /// - Call the base class to transport the fluid between the nodes.  The false bool argument
    ///   indicates that this class cannot force a mixture change in the supplying node.
    transportFluid(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the molar flux rate through the link.  Units:
///           Flux (kgmol/s) = Admittance (kgmol/kPa/s) * dP (kPa)
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidConductor::computeFlux()
{
    const double hiP = std::max(mPotentialVector[0], mPotentialVector[1]);
    if (fabs(mPotentialDrop) < (hiP * m100EpsilonLimit)) {
        /// - Zero flux if dP is too low.  This eliminates most mass loss/creation due to rounding
        ///   error in the solver.
        mFlux = 0.0;
    } else {
        mFlux = mPotentialDrop * mAdmittanceMatrix[0];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flowRate (kg/s) Desired flow rate to tune the link to
///
/// @details  This method sets the link conductivity such that it will create the desired flow rate
///           at the current network conditions, and resets the blockage malfunction if it was
///           active, so that the blockage will not interfere with the desired rate.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidConductor::tuneFlow(const double flowRate) {
    mMaxConductivity = GunnsFluidUtils::predictConductivity(flowRate,
                                                            mMinLinearizationPotential,
                                                            mNodes[0]->getOutflow(),
                                                            mNodes[1]->getOutflow(),
                                                            mPressureExponent);
    mEffectiveConductivity = mMaxConductivity;
    mMalfBlockageFlag      = false;
    mTuneMode = GunnsFluidUtils::OFF;
}
