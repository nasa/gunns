/**
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
**/

#include "UtGunnsEpsConstantPowerLoad.hh"

#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--) Link name
/// @param[in] nodes                (--) Network nodes array
/// @param[in] defaultConductivity  (--) Default conductivity of the link
/// @param[in] defaultPower         (--) Default power load of the link
/// @param[in] minimumVoltageLimit  (V)  Minimum voltage limit to act like constant power load
/// @details  Constructs the Basic Constant Load Config data
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
    mMinimumVoltageLimit(minimumVoltageLimit) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
/// @details  Copy Constructs the Basic Constant Load Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoadConfigData::EpsConstantPowerLoadConfigData(
        const EpsConstantPowerLoadConfigData& that)
    :
    GunnsBasicConductorConfigData(that),
    mDefaultPower(that.mDefaultPower),
    mMinimumVoltageLimit(that.mMinimumVoltageLimit) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Constant Load Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoadConfigData::~EpsConstantPowerLoadConfigData() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
/// @details  Default constructs this Basic Constant Load input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoadInputData::EpsConstantPowerLoadInputData(const bool   malfBlockageFlag,
                                                                 const double malfBlockageValue)
    :
    GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
/// @details  Copy constructs this Basic Constant Load input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoadInputData::EpsConstantPowerLoadInputData(
        const EpsConstantPowerLoadInputData& that)
    :
    GunnsBasicConductorInputData(that) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Basic Constant Load input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoadInputData::~EpsConstantPowerLoadInputData() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Basic Constant Load Object
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoad::EpsConstantPowerLoad()
    :
    GunnsBasicConductor(),
    mDesiredPower(0.0),
    mPowerDraw(0.0),
    mMinimumVoltageLimit(0.0),
    mBiasPowerLoadValue(0.0) {
    // nothing to do here
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Constant Load Object
////////////////////////////////////////////////////////////////////////////////////////////////////
EpsConstantPowerLoad::~EpsConstantPowerLoad() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data
/// @param[in]     inputData    (--) Reference to Link Input Data
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector
/// @param[in]     port0        (--) Port 0 Mapping
/// @param[in]     port1        (--) Port 1 Mapping
/// @throws   TsInitializationException
/// @details  Initializes the Basic Constant Load Object
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsConstantPowerLoad::initialize(const EpsConstantPowerLoadConfigData& configData,
                                        const EpsConstantPowerLoadInputData&  inputData,
                                        std::vector<GunnsBasicLink*>&           networkLinks,
                                        const int                               port0,
                                        const int                               port1) {
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
/// @details  Validates the Basic Constant Load Object
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsConstantPowerLoad::validate() {
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
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the link's effective conductivity during the time step
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsConstantPowerLoad::updateState(const double) {
    double deltaPotential = getDeltaPotential();

    if (deltaPotential > mMinimumVoltageLimit) {
        if (deltaPotential > 0.0) {
            mEffectiveConductivity = (mDesiredPower + mBiasPowerLoadValue) / (deltaPotential * deltaPotential);
        }
    } else {
        mEffectiveConductivity = 0.0;
    }

    mEffectiveConductivity = MsMath::limitRange
            (1.0/mConductanceLimit, mEffectiveConductivity, mConductanceLimit);
    mPowerDraw = -1.0 * mPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Link time step
/// @details  Updates the link during the minor time step
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsConstantPowerLoad::minorStep(const double dt, const int) {
    computeFlows(dt);
    step(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] power     (--) Link desired power
/// @param[in] potential (--) Supply potential for desired power
/// @details  This method can be used to configure the object with one call, rather than calling
///           both setPowerConsumption and setStartingConductivity separately.  This method sets
///           the StartingConductivity based on the power demand and expected nominal potential
///           of the system.
////////////////////////////////////////////////////////////////////////////////////////////////////
void EpsConstantPowerLoad::setPowerAndNominalPotential(double power, double potential) {
    mDesiredPower = power;
    if (potential > mMinimumVoltageLimit) {
        mEffectiveConductivity = (mDesiredPower + mBiasPowerLoadValue) / (potential * potential);
    } else {
        mEffectiveConductivity = 0.0;
    }
}
