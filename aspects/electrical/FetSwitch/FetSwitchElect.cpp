/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (Purpose of the Class)

 REQUIREMENTS:
 (none)

 REFERENCE:
 (none)

 ASSUMPTIONS AND LIMITATIONS:
 ()

 LIBRARY DEPENDENCY:
 ((core/GunnsBasicLink.o)
  (aspects/electrical/FetSwitch/FetSwitch.o))

 PROGRAMMERS:
 ()
 **************************************************************************************************/
#include "FetSwitchElect.hh"

//===============================================================================================
//
// FetSwitch Configuration Data
//
//===============================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    name                -- Link name
/// @param    nodes               -- Network nodes array
/// @param    fetOnResistance                 -- On Resistance
/// @param    fetOffResistance                -- Fet off equivalent resistance
/// @param    diodeReverseResistance          -- Reverse Diode equivalent resistance
/// @param    diodeForwardResistance          -- Forward Diode equivalent resistance
/// @param    overCurrentProtection           -- Fet over current protection
/// @param    overCurrentLimit                -- Over Curent Limit
///
/// @details  Constructs the FetSwitchElect Configuration data
/////////////////////////////////////////////////////////////////////////////////////////////////
FetSwitchElectConfigData::FetSwitchElectConfigData( const std::string name,
                                                    GunnsNodeList*   nodes,
                                                    const FetSwitchConfigData& config)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mSwitchConfigData (config)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the FetSwitchElect Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
FetSwitchElectConfigData::~FetSwitchElectConfigData()
{
    return;
}

//===============================================================================================
//
// FetSwitch Input Data
//
//===============================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the FetSwitch Input data
//////////////////////////////////////////////////////////////////////////////////////////////
FetSwitchElectInputData::FetSwitchElectInputData(const bool   malfBlockageFlag,
                                                   const double malfBlockageValue,
                                                   const int switchMalf)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue),
    mSwitchMalf(switchMalf)
{
    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the FetSwitch Input Data Object
///////////////////////////////////////////////////////////////////////////////////////////////
FetSwitchElectInputData::~FetSwitchElectInputData()
{
    return;
}

//===============================================================================================
//
// FetSwitch
//
//===============================================================================================
FetSwitchElect::FetSwitchElect():
    GunnsBasicLink(NPORTS),
    mFetPreviousState ( false),
    mDiodePreviousReverseBias ( true)
{
}

FetSwitchElect::~FetSwitchElect() {

}


void FetSwitchElect::initialize(const FetSwitchElectConfigData& configData,
                                 const FetSwitchElectInputData& inputData,
                                 std::vector<GunnsBasicLink*>&   networkLinks,
                                 const int  port0,
                                 const int  port1){
    int* ports = new int[2];
    ports[0] = port0;
    ports[1] = port1;
    GunnsBasicLink::initialize(configData,inputData, networkLinks, ports);
    delete [] ports;

    mSwitch.initialize(configData.mSwitchConfigData, inputData.mSwitchMalf);

    GUNNS_WARNING("this link is deprecated!  It is not complete, use at your own risk.");
}

void FetSwitchElect::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
}

void FetSwitchElect::step(double timeStep) {

    double conductance = 0.0;

    mSwitch.updateSwitchState();

    if (mFetPreviousState != mSwitch.getFetState()) {
        mFetPreviousState = mSwitch.getFetState();
        mAdmittanceUpdate = true;
    }
    if (mDiodePreviousReverseBias != mSwitch.getDiodeReverseBias()) {
        mDiodePreviousReverseBias = mSwitch.getDiodeReverseBias();
        mAdmittanceUpdate = true;
    }

    if (mAdmittanceUpdate) {
        for ( int i = 0; i < mNumPorts * mNumPorts; i++) {
            mAdmittanceMatrix[i] = 0.0;
        }

    conductance = mSwitch.getConductance();

    mAdmittanceMatrix[0 * mNumPorts + 0] = conductance;
    mAdmittanceMatrix[0 * mNumPorts + 1] = -conductance;
    mAdmittanceMatrix[1 * mNumPorts + 0] = -conductance;
    mAdmittanceMatrix[1 * mNumPorts + 1] = conductance;

    }
}

void FetSwitchElect::minorStep(const double timeStep, const int minorStep) {
    computeFlows(timeStep);
    step(timeStep);
}

void FetSwitchElect::computeFlows(double timeStep) {
    double current = 0.0;
    double mUnderVoltageLimit = 98;        ////?????????
    mPotentialDrop = mPotentialVector[0] - mPotentialVector[1];

    if ((mPotentialVector[0] >  mUnderVoltageLimit) && (mPotentialVector[1] >  mUnderVoltageLimit)){
        current = mPotentialDrop * mAdmittanceMatrix[0];}

    mSwitch.updateSwitchFlow(current);

}


bool FetSwitchElect::isNonLinear()
{
    return true;
}


GunnsBasicLink::SolutionResult FetSwitchElect::confirmSolutionAcceptable(const int convergedStep,
                                                                         const int absoluteStep) {
    GunnsBasicLink::SolutionResult result = CONFIRM;

    if (mPotentialVector[0] < mPotentialVector[1] && mSwitch.getDiodeReverseBias()) {
        result = REJECT;
        mSwitch.setDiodeReverseBias (false);
    } else if (mPotentialVector[0] > mPotentialVector[1] && !mSwitch.getDiodeReverseBias()) {
        result = REJECT;
        mSwitch.setDiodeReverseBias (true);
    }

    return result;
}
