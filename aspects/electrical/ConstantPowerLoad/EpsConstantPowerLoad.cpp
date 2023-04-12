/**
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (core/GunnsBasicConductor.o)
     (simulation/hs/TsHsMsg.o)
     (software/exceptions/TsInitializationException.o)
    )
*/

#include "EpsConstantPowerLoad.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--) Link name.
/// @param[in] nodes                (--) Network nodes array.
/// @param[in] defaultConductivity  (--) Default conductivity of the link.
/// @param[in] defaultPower         (--) Default power load of the link.
/// @param[in] minimumVoltageLimit  (V)  Minimum voltage limit to act like constant power load.
///
/// @details  Default constructs the constant power load config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoadConfigData::EpsConstantPowerLoadConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       defaultConductivity,
        const double       defaultPower,
        const double       minimumVoltageLimit)
    :
    GunnsBasicConductorConfigData(name, nodes, defaultConductivity),
    mDefaultPower(defaultPower),
    mMinimumVoltageLimit(minimumVoltageLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  Copy constructs the constant power load config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoadConfigData::EpsConstantPowerLoadConfigData(
        const EpsConstantPowerLoadConfigData& that)
    :
    GunnsBasicConductorConfigData(that),
    mDefaultPower(that.mDefaultPower),
    mMinimumVoltageLimit(that.mMinimumVoltageLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the constant power load config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoadConfigData::~EpsConstantPowerLoadConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag.
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1).
///
/// @details  Default constructs the constant power load input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoadInputData::EpsConstantPowerLoadInputData(const bool   malfBlockageFlag,
                                                             const double malfBlockageValue)
    :
    GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  Copy constructs the constant power load input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoadInputData::EpsConstantPowerLoadInputData(
        const EpsConstantPowerLoadInputData& that)
    :
    GunnsBasicConductorInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs the constant power load input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoadInputData::~EpsConstantPowerLoadInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs the constant power load link.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoad::EpsConstantPowerLoad()
    :
    GunnsBasicConductor(),
    mDesiredPower(0.0),
    mPowerDraw(0.0),
    mMinimumVoltageLimit(0.0),
    mBiasPowerLoadValue(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the constant power load link.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoad::~EpsConstantPowerLoad()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data.
/// @param[in]     inputData    (--) Reference to Link Input Data.
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector.
/// @param[in]     port0        (--) Port 0 Mapping.
/// @param[in]     port1        (--) Port 1 Mapping.
///
/// @throws   TsInitializationException
///
/// @details  Initializes the constant power load link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsConstantPowerLoad::initialize(const EpsConstantPowerLoadConfigData& configData,
                                      const EpsConstantPowerLoadInputData&  inputData,
                                      std::vector<GunnsBasicLink*>&         networkLinks,
                                      const int                             port0,
                                      const int                             port1)
{
    /// - Initialize the parent class.
    GunnsBasicConductor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize class attributes.
    mDesiredPower         = configData.mDefaultPower;
    mMinimumVoltageLimit  = configData.mMinimumVoltageLimit;

    validate();

    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates the constant power load link initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsConstantPowerLoad::validate()
{
    /// - Issue an error on power being less than zero.
    if (mDesiredPower < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has power < 0.");
    }

    /// - Issue an error on minimum voltage < 0
    if (mMinimumVoltageLimit < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has minimum voltage limit < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsConstantPowerLoad::restartModel()
{
    /// - Reset the base class.
    GunnsBasicConductor::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mPowerDraw = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the link's effective conductivity during the time step
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsConstantPowerLoad::updateState(const double)
{
    double deltaPotential = getDeltaPotential();
    updateEffectiveConductivity(deltaPotential);
    mPowerDraw = -1.0 * mPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Link time step
///
/// @details  Updates the link during the minor time step
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsConstantPowerLoad::minorStep(const double dt, const int)
{
    computeFlows(dt);
    step(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] power     (--) Link desired power
/// @param[in] potential (--) Supply potential for desired power
///
/// @details  This method can be used to configure the object with one call, rather than calling
///           both setPowerConsumption and setStartingConductivity separately.  This method sets
///           the StartingConductivity based on the power demand and expected nominal potential
///           of the system.
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsConstantPowerLoad::setPowerAndNominalPotential(double power, double potential)
{
    mDesiredPower = power;
    updateEffectiveConductivity(potential);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] potential (V) Voltage across the conductor
///
/// @details  Updates the conductor base class effective conductivity to dissipate the desired
///           power at the given potential.  Effective conductivity G is calculated as G = P/V^2.
///           A user bias to the desired power is applied to this P.  The voltage V uses is limited
///           to above zero, to avoid divide-by-zero, and the higher of the given potential and 99%
///           of the constant power load's minimum voltage limit.  This 99% helps the network
///           converge when the upstream supply is unable to meet the desired power demand at the
///           minimum voltage.  This this 99% limit is occurring, the actual power will be less
///           than desired.  For any given potential negative value, this sets the effective
///           conductivity to zero, so that this link only draws constant power in the forward
///           direction from port 0 to 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsConstantPowerLoad::updateEffectiveConductivity(const double potential)
{
    if (potential >= 0.0) {
        const double limitedPotential = std::max(potential, std::max(0.99 * mMinimumVoltageLimit, DBL_EPSILON));
        mEffectiveConductivity = (mDesiredPower + mBiasPowerLoadValue) / (limitedPotential * limitedPotential);
        mEffectiveConductivity = MsMath::limitRange(0.0, mEffectiveConductivity, mConductanceLimit);
    } else {
        mEffectiveConductivity = 0.0;
    }
}
