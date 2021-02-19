/**
@file
@brief    GUNNS Basic Flow Controller Model implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsBasicConductor.o))
*/

#include "GunnsBasicFlowController.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--) Name of object.
/// @param[in] nodes                (--) Pointer to nodes.
/// @param[in] defaultConductivity  (--) Default maximum conductivity.
/// @param[in] nonLinear            (--) Flag to operate in non-linear network mode.
/// @param[in] enableReverseControl (--) Flag to enable control in reverse flow direction.
///
/// @details  Default constructs this GUNNS Basic Flow Controller link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicFlowControllerConfigData::GunnsBasicFlowControllerConfigData(const std::string& name,
                                                                       GunnsNodeList*     nodes,
                                                                       const double       defaultConductivity,
                                                                       const bool         nonLinear,
                                                                       const bool         enableReverseControl)
    :
    GunnsBasicConductorConfigData(name, nodes, defaultConductivity),
    mNonLinear(nonLinear),
    mEnableReverseControl(enableReverseControl)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source object to copy.
///
/// @details  Copy constructs this GUNNS Basic Flow Controller link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicFlowControllerConfigData::GunnsBasicFlowControllerConfigData(const GunnsBasicFlowControllerConfigData& that)
    :
    GunnsBasicConductorConfigData(that),
    mNonLinear(that.mNonLinear),
    mEnableReverseControl(that.mEnableReverseControl)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Basic Flow Controller link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicFlowControllerConfigData::~GunnsBasicFlowControllerConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
/// @param[in] malfFluxFlag      (--) Malfunction flux rate flag.
/// @param[in] malfFluxValue     (--) Malfunction flux rate value.
/// @param[in] fluxCommand       (--) Initial commanded flux.
///
/// @details  Default constructs this GUNNS Basic Flow Controller link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicFlowControllerInputData::GunnsBasicFlowControllerInputData(const bool   malfBlockageFlag,
                                                                     const double malfBlockageValue,
                                                                     const bool   malfFluxFlag,
                                                                     const double malfFluxValue,
                                                                     const double fluxCommand)
    :
    GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue),
    mMalfFluxFlag(malfFluxFlag),
    mMalfFluxValue(malfFluxValue),
    mFluxCommand(fluxCommand)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source object to copy.
///
/// @details  Copy constructs this GUNNS Basic Flow Controller link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicFlowControllerInputData::GunnsBasicFlowControllerInputData(
        const GunnsBasicFlowControllerInputData& that)
    :
    GunnsBasicConductorInputData(that),
    mMalfFluxFlag(that.mMalfFluxFlag),
    mMalfFluxValue(that.mMalfFluxValue),
    mFluxCommand(that.mFluxCommand)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Basic Flow Controller link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicFlowControllerInputData::~GunnsBasicFlowControllerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Basic Flow Controller link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicFlowController::GunnsBasicFlowController()
    :
    GunnsBasicConductor(),
    mMalfFluxFlag(false),
    mMalfFluxValue(0.0),
    mNonLinear(false),
    mEnableReverseControl(false),
    mFluxCommand(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Basic Flow Controller link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicFlowController::~GunnsBasicFlowController()
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
/// @details  Initializes this GUNNS Basic Flow Controller link model with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicFlowController::initialize(const GunnsBasicFlowControllerConfigData& configData,
                                          const GunnsBasicFlowControllerInputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             links,
                                          const int                                 port0,
                                          const int                                 port1)
{
    /// - First initialize & validate parent.
    GunnsBasicConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag             = false;

    /// - Initialize with config & input data.
    mNonLinear            = configData.mNonLinear;
    mEnableReverseControl = configData.mEnableReverseControl;
    mMalfFluxFlag         = inputData.mMalfFluxFlag;
    mMalfFluxValue        = inputData.mMalfFluxValue;
    mFluxCommand          = inputData.mFluxCommand;

    /// - Validate configuration and input data.
    validate();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag             = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Basic Flow Controller link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicFlowController::validate() const
{
    /// - There is currently nothing to validate.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicFlowController::restartModel()
{
    /// - Reset the base class.
    GunnsBasicConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Execution time step.
/// @param[in] minorStep (--) Not used.
///
/// @details  Updates the link during the minor time step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicFlowController::minorStep(const double dt, const int minorStep  __attribute__((unused)))
{
    step(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Not used.
///
/// @details  Updates this GUNNS Basic Flow Controller link model conductivity to cause flow through
///           the link that approaches the input commanded rate.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicFlowController::updateState(const double dt __attribute__((unused)))
{
    /// - Flow rate malfunction overrides the normal command input when active.
    double fluxCommand = mFluxCommand;
    if (mMalfFluxFlag) {
        fluxCommand = mMalfFluxValue;
    }

    /// - The upper limit of maximum conductivity is applied to model the fully opened size of a
    ///   flow control valve.
    const double dP = std::max(DBL_EPSILON, fabs(mPotentialVector[0] - mPotentialVector[1]));
    mEffectiveConductivity = std::min(mDefaultConductivity, fabs(fluxCommand) / dP);

    /// - Back-flow under reverse potential gradient is either controlled or blocked.
    if (mPotentialVector[1] > mPotentialVector[0] and not mEnableReverseControl) {
        mEffectiveConductivity = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag   (--)  Malfunction activation flag.
/// @param[in] value  (--)  Desired mass flux.
///
/// @details  Arguments are defaulted to deactivate the malfunction, so you can conveniently turn
///           it off by calling setMalfFlux().
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicFlowController::setMalfFlux(const bool flag, const double value)
{
    mMalfFluxFlag  = flag;
    mMalfFluxValue = value;
}
