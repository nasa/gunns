/**
@file     GunnsElectIps.cpp
@brief    GUNNS Electrical Internal Power Supply implementation

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsBasicLink.o)
   (software/exceptions/TsInitializationException.o)
   (software/exceptions/TsOutOfBoundsException.o))
*/

#include "GunnsElectIps.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "software/exceptions/TsHsException.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "simulation/hs/TS_hs_msg_types.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param [in]     name                       (--)    Name of this link
/// @param [in,out] nodes                      (--)    Network list of nodes
/// @param [in]     backuUpPowerImplemented    (--)    Back up power logic is implemented
/// @param [in]     powerConsumedOn            (W)     Power consumed when the box is on
/// @param [in]     auxillaryOnePowerOn        (W)     Power consumed by extra number one load
/// @param [in]     auxillaryTwoPowerOn        (W)     Power consumed by extra number two load
/// @param [in]     underVoltageLimit          (V)     Minimum voltage the IPS will operate
/// @param [in]     backUpVoltageThreshold     (V)     Take over voltage for the back up power
/// @param [in]     potentialOnTolerance       (V)     Voltage tolerance to switch between sources
/// @param [in]     thermalFraction            (--)    Fraction of power load output as waste heat
/// @param [in]     backupVoltageMin           (V)     Backup minimum operating voltage
/// @param [in]     backupVoltageMax           (V)     Backup maximum operating voltage
/// @param [in]     conductanceTolerance       (--)    Conductance tolerance for change
/// @param [in]     unselectedInputConductance (1/ohm) Conductance on un-selected input channels
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectIpsConfigData::GunnsElectIpsConfigData(const std::string& name,
        GunnsNodeList* nodes,
        const bool backuUpPowerImplemented,
        const double powerConsumedOn,
        const double auxillaryOnePowerOn,
        const double auxillaryTwoPowerOn,
        const double underVoltageLimit,
        const double backUpVoltageThreshold,
        const double potentialOnTolerance,
        const double thermalFraction,
        const double backupVoltageMin,
        const double backupVoltageMax,
        const double conductanceTolerance,
        const int convergedFrameToCheckVoltage,
        const int numberOfVoltageSwitchesInASolution,
        const bool commandOnUsed,
        const double unselectedInputConductance)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mBackUpPowerImplemented(backuUpPowerImplemented),
    mDefaultPowerConsumedOn(powerConsumedOn),
    mAuxOnePowerConsumedOn(auxillaryOnePowerOn),
    mAuxTwoPowerConsumedOn(auxillaryTwoPowerOn),
    mUnderVoltageLimit(underVoltageLimit),
    mBackUpVoltageThreshold(backUpVoltageThreshold),
    mPotentialOnTolerance(potentialOnTolerance),
    mThermalFraction(thermalFraction),
    mBackupVoltageMin(backupVoltageMin),
    mBackupVoltageMax(backupVoltageMax),
    mConductanceTolerance(conductanceTolerance),
    mConvergedFrameToCheckVoltage(convergedFrameToCheckVoltage),
    mNumberOfVoltageSwitchesInASolution(numberOfVoltageSwitchesInASolution),
    mCommandOnUsed(commandOnUsed),
    mUnselectedInputConductance(unselectedInputConductance)
{
    // Nothing to do.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param [in]   that  --  Object to copy
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectIpsConfigData::GunnsElectIpsConfigData(const GunnsElectIpsConfigData& that)
    :
    GunnsBasicLinkConfigData(that),
    mBackUpPowerImplemented(that.mBackUpPowerImplemented),
    mDefaultPowerConsumedOn(that.mDefaultPowerConsumedOn),
    mAuxOnePowerConsumedOn(that.mAuxOnePowerConsumedOn),
    mAuxTwoPowerConsumedOn(that.mAuxTwoPowerConsumedOn),
    mUnderVoltageLimit(that.mUnderVoltageLimit),
    mBackUpVoltageThreshold(that.mBackUpVoltageThreshold),
    mPotentialOnTolerance(that.mPotentialOnTolerance),
    mThermalFraction(that.mThermalFraction),
    mBackupVoltageMin(that.mBackupVoltageMin),
    mBackupVoltageMax(that.mBackupVoltageMax),
    mConductanceTolerance(that.mConductanceTolerance),
    mConvergedFrameToCheckVoltage(that.mConvergedFrameToCheckVoltage),
    mNumberOfVoltageSwitchesInASolution(that.mNumberOfVoltageSwitchesInASolution),
    mCommandOnUsed(that.mCommandOnUsed),
    mUnselectedInputConductance(that.mUnselectedInputConductance)
{
    // Nothing to do.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectIpsConfigData::~GunnsElectIpsConfigData()
{
    // Nothing to do.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param [in]   malfBlockageFlag    -- Blockage malfunction flag
/// @param [in]   malfBlockageValue   -- Blockage malfunction fractional value (0-1)
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectIpsInputData::GunnsElectIpsInputData(const bool malfBlockageFlag,
        const double malfBlockageValue) :
        GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue) {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param [in]  that  --  Object to copy
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectIpsInputData::GunnsElectIpsInputData(const GunnsElectIpsInputData& that)
:
        GunnsBasicLinkInputData(that) {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this IPS data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectIpsInputData::~GunnsElectIpsInputData() {
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param [in]   numPorts   --  Number of Nodes to connect this class to.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectIps::GunnsElectIps()
    :
    GunnsBasicLink(1),
    mMalfPowerInput(0),
    mMalfAllPowerInputs(false),
    mMalfBiasPowerConsumedFlag(false),
    mMalfBiasPowerConsumedValue(0.0),
    mBackUpPowerImplemented(false),
    mDefaultPowerConsumedOn(0.0),
    mAuxOnePowerConsumedOn(0.0),
    mAuxTwoPowerConsumedOn(0.0),
    mUnderVoltageLimit(0.0),
    mBackUpVoltageThreshold(0.0),
    mPotentialOnTolerance(0.0),
    mThermalFraction(0.0),
    mBackupVoltageMin(0.0),
    mBackupVoltageMax(0.0),
    mConductanceTolerance(0.0),
    mNumberOfVoltageSwitchesInASolution(0),
    mCommandOnUsed(false),
    mUnselectedInputConductance(0.0),
    mHeatGeneratedOn(0.0),
    mHeatGenerated(0.0),
    mConductance(0),
    mActiveConductance(0),
    mOutputPower(0),
    mPowerValid(false),
    mPowerSupplyVoltage(0),
    mActivePowerSource(0),
    mLastActivePowerSource(0),
    mPowerConsumedOn(0.0),
    mNumberOfPowerSources(0),
    mBackUpPowerSource(INVALID_SOURCE),
    mTotalPowerLoad(0.0),
    mBackupVoltageInRange(false),
    mLoadAddedFromInstructor(0.0),
    mVoltageSwitches(0),
    mControlInputPowerInUse(false),
    mCommandOn(false),
    mVerbose(false),
    mInputCurrent(0),
    mInputVoltage(0)
{
    // nothing to do here
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this IPS object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectIps::~GunnsElectIps() {
    cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::cleanup() {
    TS_DELETE_ARRAY(mInputVoltage);
    TS_DELETE_ARRAY(mInputCurrent);
    TS_DELETE_ARRAY(mActiveConductance);
    TS_DELETE_ARRAY(mConductance);
    TS_DELETE_ARRAY(mOutputPower);
    TS_DELETE_ARRAY(mPowerSupplyVoltage);
    TS_DELETE_ARRAY(mMalfPowerInput);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::allocateArrays() {
    cleanup();
    TS_NEW_PRIM_ARRAY_EXT(mMalfPowerInput, mNumberOfPowerSources, bool,   std::string(mName) + ".mMalfPowerInput");
    TS_NEW_PRIM_ARRAY_EXT(mPowerSupplyVoltage,        mNumberOfPowerSources, double, std::string(mName) + ".mPowerSupplyVoltage");
    TS_NEW_PRIM_ARRAY_EXT(mOutputPower,               mNumberOfPowerSources, double, std::string(mName) + ".mOutputPower");
    TS_NEW_PRIM_ARRAY_EXT(mConductance,               mNumberOfPowerSources, double, std::string(mName) + ".mConductance");
    TS_NEW_PRIM_ARRAY_EXT(mActiveConductance,         mNumberOfPowerSources, double, std::string(mName) + ".mActiveConductance");
    TS_NEW_PRIM_ARRAY_EXT(mInputCurrent,              mNumberOfPowerSources, double, std::string(mName) + ".mInputCurrent");
    TS_NEW_PRIM_ARRAY_EXT(mInputVoltage,              mNumberOfPowerSources, double, std::string(mName) + ".mInputVoltage");
    for (int i = 0; i < mNumberOfPowerSources; i++) {
        mMalfPowerInput[i] = false;
        mPowerSupplyVoltage[i] = 0.0;
        mOutputPower[i] = 0.0;
        mConductance[i] = 0.0;
        mActiveConductance[i] = 0.0;
        mInputCurrent[i] = 0.0;
        mInputVoltage[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Configuration data to be initialized to.
/// @param[in]     inputData    (--) Input data to initialize to.
/// @param[in,out] networkLinks (--) Vector of all the network links
/// @param[in]     portsVector  (--) Vector of port numbers this Source is linked to
///
/// @throws TsInitializationException
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::initialize(GunnsElectIpsConfigData       &configData,
                               const GunnsElectIpsInputData  &inputData,
                               std::vector<GunnsBasicLink*>& networkLinks,
                               std::vector<int>*             portsVector)
{
    /// - set the number of ports
    mNumberOfPowerSources = portsVector->size();
    mNumPorts             = mNumberOfPowerSources;

    if (mNumberOfPowerSources < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Number of power sources < 1.");
    }

    /// - Declare temporary ports array.
    int ports[mNumPorts];

    /// - Set members of ports array.
    for (int i = 0; i < mNumPorts; ++i) {
        ports[i] = portsVector->at(i);
    }

    /// - Call BasicLink::initialize to set up ports.
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    // Initialize the Power Supply voltage values from the PotentialVector gunns which can
    // be initialized to a voltage at startup
    allocateArrays();
    for (int i=0; i < mNumberOfPowerSources;i++ ) {
        mPowerSupplyVoltage[i] = mPotentialVector[i];
        mInputCurrent[i] = 0.0;
        mInputVoltage[i] = mPotentialVector[i];
    }

    mThermalFraction = configData.mThermalFraction;
    mDefaultPowerConsumedOn = configData.mDefaultPowerConsumedOn;
    setOutputPower(configData.mDefaultPowerConsumedOn);
    setUnderVoltageLimit(configData.mUnderVoltageLimit);
    setOutputPowerAuxiliaryOne(configData.mAuxOnePowerConsumedOn);
    setOutputPowerAuxiliaryTwo(configData.mAuxTwoPowerConsumedOn);
    mPotentialOnTolerance = configData.mPotentialOnTolerance;
    mBackUpVoltageThreshold = configData.mBackUpVoltageThreshold;
    mBackUpPowerImplemented = configData.mBackUpPowerImplemented;
    mBackupVoltageMin = configData.mBackupVoltageMin;
    mBackupVoltageMax = configData.mBackupVoltageMax;
    mConductanceTolerance = configData.mConductanceTolerance;
    mNumberOfVoltageSwitchesInASolution = configData.mNumberOfVoltageSwitchesInASolution;
    mCommandOnUsed = configData.mCommandOnUsed;
    mUnselectedInputConductance = configData.mUnselectedInputConductance;
    mVoltageSwitches = 0;
    mTotalPowerLoad = mPowerConsumedOn + mAuxOnePowerConsumedOn + mAuxTwoPowerConsumedOn;

    if (mBackUpPowerImplemented) {
        if (1 == mNumberOfPowerSources) {
            TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_EPS, "Internal Power Supply Initialization",
                    TsInitializationException, "One power source defined as backup source", "GunnsElectIps");
        } else {
            mBackUpPowerSource = mNumberOfPowerSources - 1;
        }
    }
    validate();

    /// - Initialize mPowerValid based on initial node voltages.
    for (int i = 0; i < mNumPorts; ++i) {
        if (mNodes[i]->getPotential() >= mUnderVoltageLimit) {
            mPowerValid = true;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param  [in]  TimeStep  --  Time delta
/// @details  Called in Minor Step 1
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::step(const double TimeStep) {
    (void)TimeStep; // get rid of unused parameter compiler
    updateInputConductance();
    updateAdmittance();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param [in]   TimeStep   --  Time delta
/// @param [in]   minorStep  --  minor step number
/// @details  Called in minor steps greater than 1
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::minorStep(const double TimeStep, const int minorStep) {
    (void)minorStep; // get rid of unused parameter compiler
    step(TimeStep);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Calculates the loaded conductance for the selected input channel from its supply
///           voltage and the desired total input power.  For the unselected input channels, applies
///           the optional unselected input conductance value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::updateInputConductance() {
    /// - Unselected inputs:
    for (int port = 0; port < mNumberOfPowerSources; ++port) {
        mConductance[port] = mUnselectedInputConductance;
    }
    /// - Selected input:
    updatePowerLoad();
    if (mActivePowerSource > -1) {
        const double sourceVoltage = mPotentialVector[mActivePowerSource];
        if (sourceVoltage > DBL_EPSILON) {
            mConductance[mActivePowerSource] = mTotalPowerLoad / (sourceVoltage * sourceVoltage);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return int the index of the voltage source to use.
/// @details  Calculates if there are any voltage sources greater than the backup
///           threshold and which has a higher voltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsElectIps::getVoltagePrimarySourceGreaterThanBackupThreshold() {
    int sourceToUse = INVALID_SOURCE;
    double sourceVoltage = 0.0;
    for (int source = 0; source < mBackUpPowerSource; source++) {
        if (!mMalfPowerInput[source] &&
                !mMalfAllPowerInputs) {
            if ((mCommandOnUsed && mCommandOn) || (!mCommandOnUsed)) {
                if ((mPotentialVector[source] > mBackUpVoltageThreshold)
                        && ((mPotentialVector[source] - sourceVoltage) > mPotentialOnTolerance)) {
                    sourceToUse = source;
                    sourceVoltage = mPotentialVector[source];
                }
            }
        }
    }
    return sourceToUse;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return int the index of the voltage source to use.
/// @details  Find the index of the greatest primary voltage input source greater than the
///           primary minimum voltage
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsElectIps::getVoltagePrimarySourceGreaterUnderVoltageLimit() {
    int sourceToUse = INVALID_SOURCE;
    double sourceVoltage = 0.0;
    for (int source = 0; source < mBackUpPowerSource; source++) {
        if (!mMalfPowerInput[source] &&
                !mMalfAllPowerInputs ) {
            if ((mCommandOnUsed && mCommandOn) || (!mCommandOnUsed)) {
                if (source != mBackUpPowerSource) {
                    if ((mPotentialVector[source] > mUnderVoltageLimit)
                            && ((mPotentialVector[source] - sourceVoltage) > mPotentialOnTolerance)) {
                        sourceToUse = source;
                        sourceVoltage = mPotentialVector[source];
                    }
                }
            }
        }
    }
    if (sourceToUse != INVALID_SOURCE) {
        if (!mMalfPowerInput[mBackUpPowerSource] && !mMalfAllPowerInputs) {
            if ((mCommandOnUsed && mCommandOn) || (!mCommandOnUsed)) {
                if ((mPotentialVector[mBackUpPowerSource] - sourceVoltage) > mPotentialOnTolerance) {
                    sourceToUse = mBackUpPowerSource;
                }
            }
        }
    }
    return sourceToUse;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return int the index of the voltage source to use.
/// @details  Find the index of the backup source and determine if it is with in the correct
///           operating range.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsElectIps::getVoltageBackupWithinRange() {
    int sourceToUse = INVALID_SOURCE;
    if (INVALID_SOURCE != mBackUpPowerSource) {
        if ((!mMalfPowerInput[mBackUpPowerSource]) &&
                !mMalfAllPowerInputs &&
                (MsMath::isInRange(mBackupVoltageMin, mPotentialVector[mBackUpPowerSource], mBackupVoltageMax))) {
            if ((mCommandOnUsed && mCommandOn) || (!mCommandOnUsed)) {
                sourceToUse = mBackUpPowerSource;
            }
        }
    }
    return sourceToUse;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return int the index of the voltage source to use.
/// @details  Figure out which source to use in the correct logic with backup power considered.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsElectIps::getVoltageSourceToUseWithBackup() {
    int sourceToUse = INVALID_SOURCE;
    int firstVoltage = getVoltagePrimarySourceGreaterThanBackupThreshold();

    if (firstVoltage != INVALID_SOURCE) {
        sourceToUse = firstVoltage;
    } else {
        int secondVoltage = getVoltagePrimarySourceGreaterUnderVoltageLimit();
        if (secondVoltage != INVALID_SOURCE) {
            sourceToUse = secondVoltage;
        } else {
            sourceToUse = getVoltageBackupWithinRange();
        }
    }
    return sourceToUse;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return int the index of the voltage source to use.
/// @details  Find the index of the greatest primary voltage input source without back up power
///           considered.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsElectIps::getVoltageSourceToUseWithoutBackup() {
    int sourceToUse = 0;
    double sourceVoltage = 0.0;
    if (INVALID_SOURCE == mActivePowerSource) {
        sourceToUse = INVALID_SOURCE;
    } else {
        sourceToUse = mActivePowerSource;
        if ((mCommandOnUsed && mCommandOn) || (!mCommandOnUsed)) {
            if (!mMalfPowerInput[mActivePowerSource] && !mMalfAllPowerInputs) {
                sourceVoltage = mPotentialVector[mActivePowerSource];
                if (sourceVoltage < mUnderVoltageLimit) {
                    sourceVoltage = 0.0;
                    sourceToUse = INVALID_SOURCE;
                }
            } else {
                sourceVoltage = 0.0;
                sourceToUse = INVALID_SOURCE;
            }
        } else {
            sourceVoltage = 0.0;
            sourceToUse = INVALID_SOURCE;
        }
    }
    if ((mCommandOnUsed && mCommandOn) || (!mCommandOnUsed)) {
        for (int source = 0; source < mNumberOfPowerSources; source++) {
            if (!mMalfPowerInput[source] && !mMalfAllPowerInputs) {
                if (((mPotentialVector[source] - sourceVoltage) > mPotentialOnTolerance)
                        && (mPotentialVector[source] >= mUnderVoltageLimit)) {
                    sourceToUse = source;
                    sourceVoltage = mPotentialVector[source];
                }
            }
        }
    }
    return sourceToUse;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param [in] sourceToUse (--) The index of the voltage source to use.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::calculateFlow(const int sourceToUse) {
    double sourceVoltage = 0.0;
    // Valid Source?
    if (sourceToUse != INVALID_SOURCE) {
        sourceVoltage = mPotentialVector[sourceToUse];
        mPowerValid = sourceVoltage > mUnderVoltageLimit;
        if (sourceVoltage > 0.0) {
            mOutputPower[sourceToUse] = mTotalPowerLoad;
            mPowerSupplyVoltage[sourceToUse] = sourceVoltage;
            mInputCurrent[sourceToUse] = mTotalPowerLoad / sourceVoltage;
            mInputVoltage[sourceToUse] = sourceVoltage;
            mActivePowerSource = sourceToUse;
        }
        // Zero out the conductance/power values for the sources not used
        for (int source = 0; source < mNumberOfPowerSources; source++) {
            if (sourceToUse != source) {
                mOutputPower[source] = 0.0;
                mPowerSupplyVoltage[source] = 0.0;
                mInputCurrent[source] = 0.0;
                //mInputVoltage[source] = 0.0;
            }
        }
        // Set telemetry for control power being used
        if (sourceToUse == mBackUpPowerSource){
            mControlInputPowerInUse = true;
        } else {
            mControlInputPowerInUse = false;
        }
    } else {
        // Invalid Sources, output zero.
        mPowerValid = false;

        if (mVerbose) {
            std::ostringstream msg;
            msg << "reset power valid flag with potential vector [" << mPotentialVector[0];
            for (int i=1; i<mNumberOfPowerSources; ++i) {
                msg << ", " << mPotentialVector[i];
            }
            msg << "] and active source: " << sourceToUse;
            GUNNS_WARNING(msg.str());
        }

        mActivePowerSource = INVALID_SOURCE;
        mControlInputPowerInUse = false;

        for (int source = 0; source < mNumberOfPowerSources; source++) {
            mOutputPower[source] = 0.0;
            mPowerSupplyVoltage[source] = 0.0;
            mInputCurrent[source] = 0.0;
            mInputVoltage[source] = 0.0;
        }
    }

}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::updateAdmittance(){
    // Only change the admittance values if they are different from last time
    for (int port = 0; port < mNumberOfPowerSources; port++) {
        if (isConductanceChanged(mActiveConductance[port], mConductance[port])) {
            mActiveConductance[port] = mConductance[port];
            mAdmittanceMatrix[ port * (mNumberOfPowerSources) + port ] = mActiveConductance[port];
            mAdmittanceUpdate = true;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param  [in]  TimeStep  --  Time delta
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::computeFlows(const double TimeStep) {
    (void)TimeStep; // get rid of unused parameter compiler
    mVoltageSwitches = 0;

    /// - Drive base class flux and power terms.  Total power and current drawn by this IPS from all
    ///   inputs are mPower and mFlux, respectively.
    double totalFlux  = 0.0;
    double totalPower = 0.0;
    for (int i=0; i<mNumPorts; ++i) {
        const double flux = mPotentialVector[i] * mAdmittanceMatrix[i];
        totalFlux  += flux;
        totalPower += flux * mPotentialVector[i];
    }
    mFlux  = totalFlux;
    mPower = totalPower;

    if (mMalfBiasPowerConsumedFlag){
        mPowerConsumedOn = mDefaultPowerConsumedOn + mMalfBiasPowerConsumedValue;
    } else {
        mPowerConsumedOn = mDefaultPowerConsumedOn;
    }

    updatePowerLoad();

    // scale the portion of the power supply load that goes out as heat
    mHeatGeneratedOn = mPowerConsumedOn * mThermalFraction;

    mBackupVoltageInRange = (getVoltageBackupWithinRange() != INVALID_SOURCE) && mBackUpPowerImplemented;

    calculateHeatTransfer();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws TsOutOfBoundsException
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::validate() {
    if ((mPowerConsumedOn < 0.0) ||
            (mAuxOnePowerConsumedOn < 0.0) ||
            (mAuxTwoPowerConsumedOn < 0.0)) {
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_EPS, "Internal Power Supply ",
                TsOutOfBoundsException , " Negative Power values" , "GunnsElectIps");

    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) The # of minor steps since the network last converged
/// @param[in] absoluteStep  (--) The absolute minor step number that the network is on
/// @return SolutionResult (--) Returns whether the link confirms, rejects,
///                             or delays the solution.
///////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult GunnsElectIps::confirmSolutionAcceptable(const int convergedStep,
        const int absoluteStep) {
    GunnsBasicLink::SolutionResult solutionResult = GunnsBasicLink::CONFIRM;
    // Converged
    if (convergedStep > 0) {
        mLastActivePowerSource = mActivePowerSource;
        int sourceToUse = INVALID_SOURCE;
        if (mVoltageSwitches < mNumberOfVoltageSwitchesInASolution) {

            // No back up supply selected pick the source with the highest voltage minus tolerance
            if (!mBackUpPowerImplemented) {
                sourceToUse = getVoltageSourceToUseWithoutBackup();
            } else {
                // back up power implemented.
                sourceToUse = getVoltageSourceToUseWithBackup();
            }
            if (INVALID_SOURCE == mLastActivePowerSource &&
                    sourceToUse !=  INVALID_SOURCE){
                mLastActivePowerSource = mActivePowerSource;
            }
            // Will change compute current, power
            calculateFlow(sourceToUse);
        } else {
            calculateFlow(mActivePowerSource);
        }
        if (mLastActivePowerSource == mActivePowerSource) {
            // check for negative voltage. should never happen.
            for (int source = 0; source < mNumberOfPowerSources; source++) {
                if (0.0 > mPotentialVector[source]) {
                    solutionResult = GunnsBasicLink::REJECT;
                    if (mVerbose) {
                        std::ostringstream msg;
                        msg << "REJECTED the solution on absolute step " << absoluteStep <<
                               " because mPotentialVector[" << source << "] = " << mPotentialVector[source];
                        GUNNS_WARNING(msg.str());
                    }
                }
            }
        } else {
            // Switched Power sources.
            solutionResult = GunnsBasicLink::REJECT;
            mVoltageSwitches++;
            if (mVerbose) {
                std::ostringstream msg;
                msg << "switched inputs and REJECTED the solution on absolute step "
                    << absoluteStep << " with potentialVector [" << mPotentialVector[0];
                for (int i=1; i<mNumberOfPowerSources; ++i) {
                    msg << ", " << mPotentialVector[i];
                }
                msg << "] and new active source " << mActivePowerSource << ".";
                GUNNS_WARNING(msg.str());
            }
        }
    }
    return solutionResult;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @return  double  new voltage value minus the tolerance value.
/// @param [in] voltage (V) the voltage
///////////////////////////////////////////////////////////////////////////////////////////////
double GunnsElectIps::getPotentialValue(const double voltage) const {
    double newVoltage = voltage - mPotentialOnTolerance;
    if (0.0 > newVoltage) {
        newVoltage = 0.0;
    }
    return newVoltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] input (--) Identifies which power supply input to apply the malfunction value to.
/// @param[in] value (--) Malfunction value (true fails the power supply input off).
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default value
///           argument resets the malfunction for the given power input.  The input argument is not
///           optional.
///
/// @note     Attempting to control an input index out of bounds of the power supply input array
///           will cause a H&S warning to be sent and the method will return with no effect.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::setMalfPowerInput(const int input, const bool value)
{
    if (0 <= input and mNumberOfPowerSources > input) {
        mMalfPowerInput[input] = value;
    } else {
        GUNNS_WARNING("ignored invalid power supply index, malfunction state not changed.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) Malfunction activation flag, true activates.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectIps::setMalfAllPowerInputs(const bool flag)
{
    mMalfAllPowerInputs = flag;
}
