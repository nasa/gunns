/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsFluidLink.o))
***************************************************************************************************/

#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsFluidUtils.hh"

#include "GunnsFluidPressureSensitiveValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      name                  (--)   Link name.
/// @param[in]      nodes                 (--)   Network nodes array.
/// @param[in]      maxConductivity       (m2)   Maximum possible effective conductivity of the link.
/// @param[in]      expansionScaleFactor  (--)   Scale factor for isentropic cooling across the link (0-1).
/// @param[in]      rateLimit             (1/s)  Valve fractional position rate limit.
/// @param[in]      thermalLength         (m)    Tube length for thermal convection.
/// @param[in]      thermalDiameter       (m)    Tube inner diameter for thermal convection.
/// @param[in]      surfaceRoughness      (m)    Tube wall surface roughness for thermal convection.
///
/// @details        Default constructs this GUNNS Fluid Pressure Sensitive Valve Link Model
///                 configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureSensitiveValveConfigData::GunnsFluidPressureSensitiveValveConfigData(const std::string& name,
                                                                                       GunnsNodeList*     nodes,
                                                                                       const double       maxConductivity,
                                                                                       const double       expansionScaleFactor,
                                                                                       const double       rateLimit,
                                                                                       const double       thermalLength,
                                                                                       const double       thermalDiameter,
                                                                                       const double       surfaceRoughness)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mMaxConductivity(maxConductivity),
    mExpansionScaleFactor(expansionScaleFactor),
    mRateLimit(rateLimit),
    mThermalLength(thermalLength),
    mThermalDiameter(thermalDiameter),
    mSurfaceRoughness(surfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that  (--)  Input source to copy.
///
/// @details        Copy constructs this GUNNS Fluid Pressure Sensitive Valve Link Model
///                 configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureSensitiveValveConfigData::GunnsFluidPressureSensitiveValveConfigData(const GunnsFluidPressureSensitiveValveConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mMaxConductivity(that.mMaxConductivity),
    mExpansionScaleFactor(that.mExpansionScaleFactor),
    mRateLimit(that.mRateLimit),
    mThermalLength(that.mThermalLength),
    mThermalDiameter(that.mThermalDiameter),
    mSurfaceRoughness(that.mSurfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details        Default destructs this GUNNS Fluid Pressure Sensitive Valve Link Model
///                 configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureSensitiveValveConfigData::~GunnsFluidPressureSensitiveValveConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      malfBlockageFlag        (--)    Blockage malfunction flag.
/// @param[in]      malfBlockageValue       (--)    Blockage malfunction fractional value (0-1).
/// @param[in]      position                (--)    Valve position fractional value (0-1).
/// @param[in]      malfLeakThruFlag        (kg/s)  Leak through rate malfunction flag.
/// @param[in]      malfLeakThruValue       (--)    Leak through rate malfunction value.
/// @param[in]      malfPressureBiasFlag    (--)    Control pressure bias malfunction flag.
/// @param[in]      malfPressureBiasValue   (kPa)   Control pressure bias malfunction value.
/// @param[in]      setPointPressureBias    (kPa)   Set point pressure bias value.
/// @param[in]      wallTemperature         (K)     Tube wall temperature for thermal convection
/// @param[in]      malfStuckFlag           (--)    Stuck at current position malfunction flag.
/// @param[in]      malfFailToFlag          (--)    Fail to position position malfunction flag.
/// @param[in]      malfFailToValue         (--)    Fail to position position malfunction value.
///
/// @details        Default constructs this GUNNS Fluid Pressure Sensitive Valve Link Model input
///                 data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureSensitiveValveInputData::GunnsFluidPressureSensitiveValveInputData(const bool   malfBlockageFlag,
                                                                                     const double malfBlockageValue,
                                                                                     const double position,
                                                                                     const bool   malfLeakThruFlag,
                                                                                     const double malfLeakThruValue,
                                                                                     const bool   malfPressureBiasFlag,
                                                                                     const double malfPressureBiasValue,
                                                                                     const double setPointPressureBias,
                                                                                     const double wallTemperature,
                                                                                     const bool   malfStuckFlag,
                                                                                     const bool   malfFailToFlag,
                                                                                     const double malfFailToValue)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mPosition(position),
    mMalfLeakThruFlag(malfLeakThruFlag),
    mMalfLeakThruValue(malfLeakThruValue),
    mMalfPressureBiasFlag(malfPressureBiasFlag),
    mMalfPressureBiasValue(malfPressureBiasValue),
    mSetPointPressureBias(setPointPressureBias),
    mWallTemperature(wallTemperature),
    mMalfStuckFlag(malfStuckFlag),
    mMalfFailToFlag(malfFailToFlag),
    mMalfFailToValue(malfFailToValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that  (--)  Input source to copy.
///
/// @details        Copy constructs this GUNNS Fluid Pressure Sensitive Valve Link Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureSensitiveValveInputData::GunnsFluidPressureSensitiveValveInputData(const GunnsFluidPressureSensitiveValveInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mPosition(that.mPosition),
    mMalfLeakThruFlag(that.mMalfLeakThruFlag),
    mMalfLeakThruValue(that.mMalfLeakThruValue),
    mMalfPressureBiasFlag(that.mMalfPressureBiasFlag),
    mMalfPressureBiasValue(that.mMalfPressureBiasValue),
    mSetPointPressureBias(that.mSetPointPressureBias),
    mWallTemperature(that.mWallTemperature),
    mMalfStuckFlag(that.mMalfStuckFlag),
    mMalfFailToFlag(that.mMalfFailToFlag),
    mMalfFailToValue(that.mMalfFailToValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details        Default destructs this GUNNS Fluid Pressure Sensitive Valve Link Model input
///                 data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureSensitiveValveInputData::~GunnsFluidPressureSensitiveValveInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note           This should be followed by a call to the initialize method before calling an
///                 update method.
///
/// @details        Default constructs this GUNNS Fluid Pressure Sensitive Valve Link Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureSensitiveValve::GunnsFluidPressureSensitiveValve()
    :
    GunnsFluidLink(NPORTS),
    mMalfLeakThruFlag(false),
    mMalfLeakThruValue(0.0),
    mMalfPressureBiasFlag(false),
    mMalfPressureBiasValue(0.0),
    mMalfStuckFlag(false),
    mMalfFailToFlag(false),
    mMalfFailToValue(0.0),
    mMaxConductivity(0.0),
    mExpansionScaleFactor(0.0),
    mRateLimit(0.0),
    mThermalDiameter(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mPosition(0.0),
    mSetPointPressureBias(0.0),
    mWallTemperature(0.0),
    mPreviousLeakRate(0.0),
    mLeakConductivity(0.0),
    mTuneMode(GunnsFluidUtils::OFF),
    mTuneMassFlow(0.0),
    mTuneVolFlow(0.0),
    mTuneDeltaT(0.0),
    mEffectiveConductivity(0.0),
    mSystemConductance(0.0),
    mControlPressure(0.0),
    mWallHeatFlux(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details        Default destructs this GUNNS Fluid Pressure Sensitive Valve Link Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureSensitiveValve::~GunnsFluidPressureSensitiveValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData     (--)  GUNNS Fluid Pressure Sensitive Valve Config Data.
/// @param[in]      inputData      (--)  GUNNS Fluid Pressure Sensitive Valve Input Data.
/// @param[in,out]  networkLinks   (--)  Network links.
/// @param[in]      port0          (--)  Network port 0.
/// @param[in]      port1          (--)  Network port 1.
/// @param[in]      pressurePort0  (--)  Network pressure port 0.
/// @param[in]      pressurePort1  (--)  Network pressure port 1.
///
/// @return         void
///
/// @throws         TsInitializationException
///
/// @details        Initializes this GUNNS Fluid Pressure Sensitive Valve Link Model with
///                 configuration, input and port data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::initialize(const GunnsFluidPressureSensitiveValveConfigData& configData,
                                                  const GunnsFluidPressureSensitiveValveInputData&  inputData,
                                                  std::vector<GunnsBasicLink*>&                     networkLinks,
                                                  const int                                         port0,
                                                  const int                                         port1,
                                                  const int                                         pressurePort0,
                                                  const int                                         pressurePort1)
{
    /// - First initialize & validate parent.
    int ports[4] = {port0, port1, pressurePort0, pressurePort1};
    GunnsFluidLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Initialize with configuration data.  Since surface area is a product of diameter, checking
    ///   it avoids divide-by-zero on diameter, and it also determines whether thermal convection is
    ///   to be used, which includes instantiating the internal fluid.
    mMaxConductivity          = configData.mMaxConductivity;
    mExpansionScaleFactor     = configData.mExpansionScaleFactor;
    mRateLimit                = configData.mRateLimit;
    mThermalDiameter          = configData.mThermalDiameter;
    mThermalSurfaceArea       = configData.mThermalLength * mThermalDiameter * UnitConversion::PI_UTIL;
    if (mThermalSurfaceArea > DBL_EPSILON) {
        mThermalROverD        = configData.mSurfaceRoughness / mThermalDiameter;
        createInternalFluid();
    } else {
        mThermalROverD        = 0.0;

    }

    /// - Initialize with input data.
    mPosition                 = inputData.mPosition;
    mMalfLeakThruFlag         = inputData.mMalfLeakThruFlag;
    mMalfLeakThruValue        = inputData.mMalfLeakThruValue;
    mMalfPressureBiasFlag     = inputData.mMalfPressureBiasFlag;
    mMalfPressureBiasValue    = inputData.mMalfPressureBiasValue;
    mSetPointPressureBias     = inputData.mSetPointPressureBias;
    mWallTemperature          = inputData.mWallTemperature;
    mMalfStuckFlag            = inputData.mMalfStuckFlag;
    mMalfFailToFlag           = inputData.mMalfFailToFlag;
    mMalfFailToValue          = inputData.mMalfFailToValue;

    /// - Initialize outputs.
    mPreviousLeakRate         = 0.0;
    mLeakConductivity         = 0.0;
    mTuneMode                 = GunnsFluidUtils::OFF;
    mTuneMassFlow             = 0.0;
    mTuneVolFlow              = 0.0;
    mTuneDeltaT               = 0.0;
    mEffectiveConductivity    = mMaxConductivity;
    mSystemConductance        = 0.0;
    mControlPressure          = 0.0;
    mWallHeatFlux             = 0.0;

    /// - Validate configuration and input data.
    validate();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return         void
///
/// @throws         TsInitializationException
///
/// @details        Validates this GUNNS Fluid Pressure Sensitive Valve Link Model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::validate() const
{
    /// - Throw an exception if max conductivity < 0.
    if (mMaxConductivity < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Max conductivity < 0.");
    }

    /// - Throw an exception if expansion scale factor < 0 or > 1.
    if (!MsMath::isInRange(0.0, mExpansionScaleFactor, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Expansion scale factor outside valid range (0-1).");
    }

    /// - Throw an exception if rate limit < 0.
    if (mRateLimit < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Rate limit < 0.");
    }

    /// - Throw an exception if valve position < 0 or > 1 .
    if (!MsMath::isInRange(0.0, mPosition, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Position outside valid range (0-1).");
    }

    /// - Throw an exception if malfunction leak through rate < 0.
    if (mMalfLeakThruValue < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Malfunction leak through rate < 0.");
    }

    /// - Throw an exception if initial wall temperature is negative.
    if (mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Wall temperature < 0.");
    }

    /// - Throw an exception if fail to position malfunction value is out of range 0 to 1.
    if (!MsMath::isInRange(0.0, mMalfFailToValue, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Constant position malfunction value out of range 0 to 1.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::restartModel()
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
    mControlPressure       = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      port  (--)  The port to be assigned (not used).
/// @param[in]      node  (--)  The desired node to assign the port to (not used).
///
/// @return         boolean  (--)  Returns true if uniqueness rules checks pass
///
/// @details        Checks this GUNNS Fluid Pressure Sensitive Valve Link Model for unique node
///                 assignments within the two pairs of ports, but both may map to vacuum node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidPressureSensitiveValve::checkPortDuplication(const int port, const int node) const
{
    bool result = true;

    // mNodeMap is only defined up to validated nodes
    if (getGroundNodeIndex() != node) {
        if        (0 == port && mNodeMap[1] == node) {
            result = false;
        } else if (1 == port && mNodeMap[0] == node) {
            result = false;
        } else if (2 == port && mNodeMap[3] == node) {
            result = false;
        } else if (3 == port && mNodeMap[2] == node) {
            result = false;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Integration time step.
///
/// @return         void
///
/// @details        Updates the state of this GUNNS Fluid Pressure Sensitive Valve Link Model and
///                 computes a new conductivity.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Valve is controlled by delta pressure across the pressure ports.
    mControlPressure      = mPotentialVector[2] - mPotentialVector[3];

    /// - Apply control pressure bias malfunction.
    if (mMalfPressureBiasFlag) {
        mControlPressure += mMalfPressureBiasValue;
    }

    /// - The set point pressure bias is equivalent to an opposite bias on the control pressure.
    mControlPressure     -= mSetPointPressureBias;

    /// - Call the virtual updateState method so a derived model can calculate a new valve position.
    updateState(dt);

    /// - Handle requests to automatically tune link parameters. Note that valve position is not
    ///   considered, so the tuned flow rate is achieved when the valve is full open.
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
    double linkAdmittance = GunnsFluidUtils::computeAdmittance(mEffectiveConductivity,
                                                               mMinLinearizationPotential,
                                                               mNodes[0]->getOutflow(),
                                                               mNodes[1]->getOutflow());
    mSystemConductance = MsMath::limitRange(0.0, linkAdmittance, mConductanceLimit);

    buildConductance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return         void
///
/// @details        Builds the admittance matrix for this GUNNS Fluid Pressure Sensitive Valve Link
///                 Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::buildConductance()
{
    if (fabs(mAdmittanceMatrix[0] - mSystemConductance) > 0.0) {
        mAdmittanceMatrix[0]   =  mSystemConductance;
        mAdmittanceMatrix[1]   = -mSystemConductance;
        mAdmittanceMatrix[2]   =  0.0;
        mAdmittanceMatrix[3]   =  0.0;
        mAdmittanceMatrix[4]   = -mSystemConductance;
        mAdmittanceMatrix[5]   =  mSystemConductance;
        mAdmittanceMatrix[6]   =  0.0;
        mAdmittanceMatrix[7]   =  0.0;
        mAdmittanceMatrix[8]   =  0.0;
        mAdmittanceMatrix[9]   =  0.0;
        mAdmittanceMatrix[10]  =  0.0;
        mAdmittanceMatrix[11]  =  0.0;
        mAdmittanceMatrix[12]  =  0.0;
        mAdmittanceMatrix[13]  =  0.0;
        mAdmittanceMatrix[14]  =  0.0;
        mAdmittanceMatrix[15]  =  0.0;
        mAdmittanceUpdate = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Time step (unused).
///
/// @return         void
///
/// @details        Computes the flow rate across this GUNNS Fluid Pressure Sensitive Valve Link
///                 Model and updates the fluid state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::computeFlows(const double dt __attribute__((unused)))
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
    mPortDirections[2] = NONE;
    mPortDirections[3] = NONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Time step.
///
/// @return         void
///
/// @details        Updates final flow properties across this GUNNS Fluid Pressure Sensitive Valve   
///                 Link and transports fluid from source to sink nodes
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::transportFlows(const double dt)
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

        /// - Perform heat convection between the internal fluid and tube wall.
        mWallHeatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(mInternalFluid,
                                                                   mFlowRate,
                                                                   mThermalROverD,
                                                                   mThermalDiameter,
                                                                   mThermalSurfaceArea,
                                                                   mWallTemperature);

        /// - Call derived update fluid.
        updateFluid(dt, mFlowRate);
    }

    /// - Call the base class to transport the fluid between the nodes.  The false boolean argument
    ///   indicates that this class cannot force a mixture change in the supplying node.
    transportFluid(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Time step (not used).
///
/// @return         void
///
/// @note           Derived classes should call this method at the end of their updateState method.
///
/// @details        Updates the effective conductivity of this GUNNS Fluid Pressure Sensitive Valve
///                 Link Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::updateState(const double dt __attribute__((unused)))
{
    if (mMalfLeakThruFlag) {
        /// - For leak rate malfunction, compute leak conductivity once on malfunction activation
        ///   and set effective conductivity.
        if (mPreviousLeakRate != mMalfLeakThruValue) {
            mLeakConductivity  = GunnsFluidUtils::predictConductivity(mMalfLeakThruValue,
                                                                      mMinLinearizationPotential,
                                                                      mNodes[0]->getOutflow(),
                                                                      mNodes[1]->getOutflow());
            mPreviousLeakRate  = mMalfLeakThruValue;
        }
    } else {
        /// - For no active malfunctions, zero out leak conductivity.
        mPreviousLeakRate      = 0.0;
        mLeakConductivity      = 0.0;
    }

    /// - Effective conductivity is maximum conductivity scaled by position, bounded below by leak
    ///   conductivity.
    mEffectiveConductivity     = std::max(mMaxConductivity * mPosition, mLeakConductivity);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt        (s)     Time step (not used).
/// @param[in]      flowRate  (kg/s)  Mass flow rate (not used).
///
/// @returns        void
///
/// @details        Updates the internal fluid of this this GUNNS Fluid Pressure Sensitive Valve
///                 Link Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::updateFluid(const double dt       __attribute__((unused)),
                                                   const double flowRate __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      flowRate  (kg/s)  Desired flow rate to tune the link to.
///
/// @return         void
///
/// @details        Sets the conductivity of this GUNNS Fluid Pressure Sensitive Valve Link Model
///                 such that it will create the specified flow rate at the current network
///                 conditions. The blockage malfunction is reset if it was active, so that it does
///                 not interfere with the desired rate.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::tuneFlow(const double flowRate) {
    mMaxConductivity       = GunnsFluidUtils::predictConductivity(flowRate,
                                                                  mMinLinearizationPotential,
                                                                  mNodes[0]->getOutflow(),
                                                                  mNodes[1]->getOutflow());
    mEffectiveConductivity = mMaxConductivity;
    mMalfBlockageFlag      = false;
    mTuneMode              = GunnsFluidUtils::OFF;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--)   Malfunction activation flag, true activates
/// @param[in] value (kg/s) Malfunction initial leak thru rate value
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::setMalfLeakThru(const bool flag, const double value)
{
    mMalfLeakThruFlag  = flag;
    mMalfLeakThruValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--)   Malfunction activation flag, true activates
/// @param[in] value (kPa)  Control pressure bias malfunction value
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::setMalfPressureBias(const bool flag, const double value)
{
    mMalfPressureBiasFlag  = flag;
    mMalfPressureBiasValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--)   Malfunction activation flag, true activates
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::setMalfStuck(const bool flag)
{
    mMalfStuckFlag  = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
/// @param[in] value (--) Fail to position position malfunction value
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::setMalfFailTo(const bool flag, const double value)
{
    mMalfFailToFlag  = flag;
    mMalfFailToValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (m2)    New Thermal Surface Area.
///
/// @returns  void
///
/// @details  Sets the thermal surface area of this this GUNNS Fluid Pressure Sensitive Valve link
///           model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::setThermalSurfaceArea(const double value)
{
    mThermalSurfaceArea = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (K)    New Wall Temperature.
///
/// @returns  void
///
/// @details  Sets the wall temperature of this this GUNNS Fluid Pressure Sensitive Valve link
///           model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureSensitiveValve::setWallTemperature(const double value)
{
    mWallTemperature = std::max(0.0, value);
}
