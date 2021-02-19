/**
@file
@brief    GUNNS Fluid Flow Controller Model implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o)
    (core/GunnsFluidUtils.o))
*/

#include "math/MsMath.hh"
#include "core/GunnsFluidUtils.hh"
#include "GunnsFluidFlowController.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                   (--) Name of object.
/// @param[in] nodes                  (--) Pointer to nodes.
/// @param[in] maxConductivity        (m2) Max conductivity.
/// @param[in] expansionScaleFactor   (--) Scale factor for isentropic gas cooling.
/// @param[in] filterProportionalGain (--) (0-1) proportional gain for stability filter.
/// @param[in] enableReverseControl   (--) Flag to enable control in reverse flow direction.
///
/// @details  Default constructs this GUNNS Fluid Flow Controller link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowControllerConfigData::GunnsFluidFlowControllerConfigData(const std::string& name,
                                                                       GunnsNodeList*     nodes,
                                                                       const double       maxConductivity,
                                                                       const double       expansionScaleFactor,
                                                                       const double       filterProportionalGain,
                                                                       const bool         enableReverseControl)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mFilterProportionalGain(filterProportionalGain),
    mEnableReverseControl(enableReverseControl)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source object to copy.
///
/// @details  Copy constructs this GUNNS Fluid Flow Controller link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowControllerConfigData::GunnsFluidFlowControllerConfigData(const GunnsFluidFlowControllerConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mFilterProportionalGain(that.mFilterProportionalGain),
    mEnableReverseControl(that.mEnableReverseControl)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Flow Controller link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowControllerConfigData::~GunnsFluidFlowControllerConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--)   Blockage malfunction flag
/// @param[in] malfBlockageValue (--)   Blockage malfunction fractional value (0-1)
/// @param[in] malfFlowRateFlag  (--)   Malfunction flow rate flag.
/// @param[in] malfFlowRateValue (kg/s) Malfunction flow rate value.
/// @param[in] flowRateCommand   (kg/s) Initial commanded flow rate.
///
/// @details  Default constructs this GUNNS Fluid Flow Controller link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowControllerInputData::GunnsFluidFlowControllerInputData(const bool   malfBlockageFlag,
                                                                     const double malfBlockageValue,
                                                                     const bool   malfFlowRateFlag,
                                                                     const double malfFlowRateValue,
                                                                     const double flowRateCommand)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mMalfFlowRateFlag(malfFlowRateFlag),
    mMalfFlowRateValue(malfFlowRateValue),
    mFlowRateCommand(flowRateCommand)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source object to copy.
///
/// @details  Copy constructs this GUNNS Fluid Flow Controller link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowControllerInputData::GunnsFluidFlowControllerInputData(
        const GunnsFluidFlowControllerInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mMalfFlowRateFlag(that.mMalfFlowRateFlag),
    mMalfFlowRateValue(that.mMalfFlowRateValue),
    mFlowRateCommand(that.mFlowRateCommand)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Flow Controller link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowControllerInputData::~GunnsFluidFlowControllerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Flow Controller link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowController::GunnsFluidFlowController()
    :
    GunnsFluidConductor(),
    mMalfFlowRateFlag(false),
    mMalfFlowRateValue(0.0),
    mFilterProportionalGain(0.0),
    mEnableReverseControl(false),
    mFlowRateCommand(0.0),
    mLastConductivity(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Flow Controller link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowController::~GunnsFluidFlowController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
/// @param[in,out]  links       (--)  Link vector.
/// @param[in]      port0       (--)  Nominal inlet port map index.
/// @param[in]      port1       (--)  Nominal outlet port map index.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Flow Controller link model with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFlowController::initialize(const GunnsFluidFlowControllerConfigData& configData,
                                          const GunnsFluidFlowControllerInputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             links,
                                          const int                                 port0,
                                          const int                                 port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag               = false;

    /// - Initialize with config & input data.
    mFilterProportionalGain = configData.mFilterProportionalGain;
    mEnableReverseControl   = configData.mEnableReverseControl;
    mMalfFlowRateFlag       = inputData.mMalfFlowRateFlag;
    mMalfFlowRateValue      = inputData.mMalfFlowRateValue;
    mFlowRateCommand        = inputData.mFlowRateCommand;

    /// - Initialize state data.
    mLastConductivity       = mEffectiveConductivity;

    /// - Validate configuration and input data.
    validate();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag               = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Flow Controller link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFlowController::validate() const
{
    /// - Issue an error on filter gain < 0 or > 1.
    if (!MsMath::isInRange(0.0, mFilterProportionalGain, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Filter gain not within (0-1).");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFlowController::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mLastConductivity = mEffectiveConductivity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Not used.
///
/// @details  Updates this GUNNS Fluid Flow Controller link model conductivity to cause flow through
///           the link that approaches the input commanded rate.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidFlowController::updateState(const double dt __attribute__((unused)))
{
    /// - Flow rate malfunction overrides the normal command input when active.
    double flowCommand = mFlowRateCommand;
    if (mMalfFlowRateFlag) {
        flowCommand = mMalfFlowRateValue;
    }

    /// - The upper limit of maximum conductivity is applied to model the fully opened size of a
    ///   flow control valve.
    mEffectiveConductivity = std::min(mMaxConductivity,
                                      GunnsFluidUtils::predictConductivity(flowCommand,
                                                                           mMinLinearizationPotential,
                                                                           mNodes[0]->getContent(),
                                                                           mNodes[1]->getContent()));

    /// - Back-flow under reverse pressure gradient is either controlled or blocked.
    if (mPotentialVector[1] > mPotentialVector[0] and not mEnableReverseControl) {
        mEffectiveConductivity = 0.0;
    }

    /// - This filter helps reduce noise when this link is in a flow circuit with other dynamic
    ///   valves like pressure regulators.
    mEffectiveConductivity = mLastConductivity + MsMath::limitRange(0.0, mFilterProportionalGain, 1.0)
                           * (mEffectiveConductivity - mLastConductivity);
    if (mEffectiveConductivity < DBL_EPSILON) {
        mEffectiveConductivity = 0.0;
    }
    mLastConductivity = mEffectiveConductivity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag   (--)    Malfunction activation flag.
/// @param[in] value  (kg/s)  Desired mass flow rate.
///
/// @details  Arguments are defaulted to deactivate the malfunction, so you can conveniently turn
///           it off by calling setMalfFlowRate().
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFlowController::setMalfFlowRate(const bool flag, const double value)
{
    mMalfFlowRateFlag  = flag;
    mMalfFlowRateValue = value;
}
