/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (EPS Switch Elect class)

 REQUIREMENTS:
 (none)

 REFERENCE:
 (none)

 ASSUMPTIONS AND LIMITATIONS:
 (none)

 LIBRARY DEPENDENCY:
 (
 (aspects/electrical/Switch/GunnsElectSwitchUtil.o)
 (core/GunnsBasicConductor.o)
 (SwitchElect.o)
 )

 PROGRAMMERS:
 (
 (Minh Tran) (L-3) (July 2011) (Initial Prototype)
 (Nicholas Kaufmann (L-3) (Oct 2011) (Refactor code to derive from basic conductor)
 (Mark Wiederholt) (Lockheed) (January 2014) (Fixed the trip level checking to work other than one and work with SwitchElectWithSensor)
 )
 **************************************************************************************************/
#include <string>
#include <vector>
#include "SwitchElect.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the SwitchElect Config data
////////////////////////////////////////////////////////////////////////////////////////////////////

SwitchElectConfigData::SwitchElectConfigData(const std::string name,
                                             GunnsNodeList* nodes,
                                             const GunnsElectSwitchUtilConfigData& config)
:GunnsBasicConductorConfigData(name, nodes, (1.0 / config.mDefaultSwitchResistance)),
mSwitchConfigData(config),
mSwitchName(name) {
     // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
/// -
/// @details  Copy constructs this Switch Elect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////

SwitchElectConfigData::SwitchElectConfigData(const SwitchElectConfigData& that)
:GunnsBasicConductorConfigData(that.mName, that.mNodeList, that.mSwitchConfigData.mDefaultSwitchResistance),
mSwitchConfigData(that.mSwitchConfigData),
mSwitchName(that.mSwitchName) {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SwitchElect Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
SwitchElectConfigData::~SwitchElectConfigData() {
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    malfBlockageFlag    -- Blockage malfunction flag
/// @param    malfBlockageValue   -- Blockage malfunction fractional value (0-1)
/// @details  Default constructs this Switch Elect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////

SwitchElectInputData::SwitchElectInputData(const bool malfBlockageFlag,
                                           const double malfBlockageValue,
                                           const GunnsElectSwitchUtilInputData& inputData)
:GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue),
mSwitchInputData(inputData) {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
/// @details  Copy constructs this Switch Elect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////

SwitchElectInputData::SwitchElectInputData(const SwitchElectInputData& that)
:GunnsBasicConductorInputData(that.mMalfBlockageFlag, that.mMalfBlockageValue),
mSwitchInputData(that.mSwitchInputData) {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Switch Elect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
SwitchElectInputData::~SwitchElectInputData() {
    // nothing to do
}

//////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the SwitchElect
//////////////////////////////////////////////////////////////////////////////////////////////
SwitchElect::SwitchElect()
:GunnsBasicConductor(),
mSwitch(),
mCurrent(0.0),
mVoltage(0.0),
mTimeStep(0.0){
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SwitchElect Object
///////////////////////////////////////////////////////////////////////////////////////////////
SwitchElect::~SwitchElect() {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   configData   -- Reference to Link Config Data
/// @param   networkLinks -- Reference to the Network Link Vector
/// @param   port0        -- Port 0 Mapping
/// @param   port1        -- Port 1 Mapping
/// @details  Initializes the SwitchElect
////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchElect::initialize(const SwitchElectConfigData& configData,
        const SwitchElectInputData& inputData,
        std::vector<GunnsBasicLink*>& networkLinks,
        const int port0,
        const int port1) {
    /// - Initialize the parent class.
    GunnsBasicConductor::initialize(configData, inputData, networkLinks, port0, port1);
    mSwitch.initialize(configData.mSwitchConfigData, inputData.mSwitchInputData, configData.mSwitchName);
    mCurrent= 0.0;
    mVoltage= 0.0;
    mTimeStep = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchElect::restartModel()
{
    /// - Reset the base class.
    GunnsBasicConductor::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   timeStep  (s)  time step
/// @details  Method for updating the link for the network
/////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchElect::updateState(const double timeStep __attribute__((unused))) {
    mSwitch.updateSwitchState(true);
    mEffectiveConductivity = mSwitch.getConductance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   timeStep  (s)  time step
/// @details  Method for saving dt
/////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchElect::computeFlows(const double timeStep) {
    mTimeStep = timeStep;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) The # of minor steps since the network last converged
/// @param[in] absoluteStep  (--) The absolute minor step number that the network is on
/// @return   SolutionResult (--) Returns whether the link confirms, rejects,
///                               or delays the solution.
/////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult SwitchElect::confirmSolutionAcceptable(
        const int convergedStep, const int absoluteStep __attribute__((unused))) {
   GunnsBasicLink::SolutionResult result = DELAY;
   // Converged
   if (convergedStep > 0) {

      mPotentialDrop =  getDeltaPotential();
      computeFlux();
      updateFlux(mTimeStep, mFlux); // nothing really done here. Kept the same gunnsBasicConductor
      computePower();
      transportFlux();

      mVoltage = mPotentialVector[0];
      mCurrent = mFlux;
      mSwitch.updateSwitchFlow(mCurrent, mCurrent,  mVoltage,  convergedStep, false);

      if (mSwitch.isWaitingToTrip() ){
         result = GunnsBasicLink::DELAY;
      } else if (mSwitch.isJustTripped()) {
         result = GunnsBasicLink::REJECT;
      } else {
         result = GunnsBasicLink::CONFIRM;
      }

   } else {
      result = GunnsBasicLink::DELAY;
   }
   return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method for reporting whether a switch object is tripped or not
/////////////////////////////////////////////////////////////////////////////////////////////////
bool SwitchElect::isSwitchTripped() {
    return mSwitch.isTripped();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method for reporting whether a switch object is just tripped or not
/////////////////////////////////////////////////////////////////////////////////////////////////
bool SwitchElect::isSwitchJustTripped() {
    return mSwitch.isJustTripped();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method for reporting whether a switch object is tripped in the positive direction
/////////////////////////////////////////////////////////////////////////////////////////////////
bool SwitchElect::isSwitchPosTrip() {
    return mSwitch.isPosTrip();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method for reporting whether a switch object is tripped in the negative direction
/////////////////////////////////////////////////////////////////////////////////////////////////
bool SwitchElect::isSwitchNegTrip() {
    return mSwitch.isNegTrip();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method for reporting whether a switch object is tripped because of low voltage
/////////////////////////////////////////////////////////////////////////////////////////////////
//bool SwitchElect::isSwitchVoltTrip() {
//    return mSwitch.isVoltTrip();
//}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method for reporting whether a switch object is open or closed
/////////////////////////////////////////////////////////////////////////////////////////////////
bool SwitchElect::isSwitchClosed() {
    return mSwitch.isClosed();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method for closing a switch object
/////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchElect::setSwitchCommandedClosed(const bool switchCommandedClosed) {
    mSwitch.setSwitchCommandedClosed(switchCommandedClosed);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method for reseting all switch object trip flags
/////////////////////////////////////////////////////////////////////////////////////////////////
void SwitchElect::setSwitchTripReset() {
    mSwitch.setTripReset();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method for getting the current through the switch object
/////////////////////////////////////////////////////////////////////////////////////////////////
double SwitchElect::getSwitchCurrent() {
    return mSwitch.getCurrent();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method for getting the output voltage of the switch object
/////////////////////////////////////////////////////////////////////////////////////////////////
double SwitchElect::getSwitchOutputVoltage() {
    return mSwitch.getOutputVolts();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method for getting the input voltage of the switch object
/////////////////////////////////////////////////////////////////////////////////////////////////
double SwitchElect::getSwitchInputVoltage() {
    return mSwitch.getInputVolts();
}
