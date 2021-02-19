/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 ( The PowerBus class is the electrical aspect of a single input with multiple loads that distributes
   current and power to user defined loads.  It has a configurable number of loads and an no upper
   bound on the number of loads)

 REQUIREMENTS:
 (none)

 REFERENCE:
 (none)

 ASSUMPTIONS AND LIMITATIONS:
 ()

 LIBRARY DEPENDENCY:
  (
  (aspects/electrical/UserLoad/UserLoadBase.o)
  (core/GunnsBasicLink.o)
  (core/GunnsBasicJumperPlug.o)
  )

 PROGRAMMERS:
  (
 (Carlo Bocatto) (L-3) (Initial Prototype) (5/2013)
  )
 **************************************************************************************************/
#include <cfloat>
#include <iostream>
#include "PowerBusElect.hh"
#include "math/MsMath.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsBasicJumperPlug.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    name                           (--) identifier for this instance of the Power Bus
/// @param[in]    nodes                          (--) GUNNS network nodes this Power Bus is connected to
/// @param[in]      numLoads                         (--) total number of loads in the Power Bus
/// @details  Constructs the PowerBusElect Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
PowerBusElectConfigData::PowerBusElectConfigData(const std::string&      name,
                                                 GunnsNodeList           *nodes,
                                                 const int               numLoads,
                                                 GunnsBasicJumperPlug*   plug0,
                                                 const bool              selfSealing0)
:
                                                 GunnsBasicLinkConfigData(name, nodes),
                                                 mNumLoads(numLoads),
                                                 mPorts(),
                                                 mPlug0       (plug0),
                                                 mSelfSealing0(selfSealing0)

{
    //one port due to the link having a single input port and no other port
    int lNumPorts = 1;
    TS_NEW_PRIM_ARRAY_EXT(mPorts, lNumPorts, int, name + ".mPorts");
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the PowerBusElect Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
PowerBusElectConfigData::~PowerBusElectConfigData(){
    cleanup();
}


///////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
/// @details  Copy Construct the PowerBusElect Config Data Object
///////////////////////////////////////////////////////////////////////////////////////////////
PowerBusElectConfigData::PowerBusElectConfigData(const PowerBusElectConfigData& that)
:
                GunnsBasicLinkConfigData(that.mName, that.mNodeList),
                mNumLoads(that.mNumLoads),
                mPorts(),
                mPlug0       (that.mPlug0),
                mSelfSealing0(that.mSelfSealing0)
{
    int numPorts = 1;
    TS_NEW_PRIM_ARRAY_EXT(mPorts, numPorts, int, that.mName + ".mPorts");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes dynamically allocated arrays.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElectConfigData::cleanup()
{
    TS_DELETE_ARRAY(mPorts);
}


//////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   malfBlockageFlag                (--)     flag to enable/disable the malfunction
/// @param[in]   malfBlockageValue               (--)     amount of blockage to add to the network link
/// @param[in]   maxConductance                  (1/ohm)  max conductance value allowed throught the power bus
/// @param[in]   minConductance                  (1/ohm)  min conductance value allowed throughout the power bus
/// @details  Constructs the PowerBusElect Input data
//////////////////////////////////////////////////////////////////////////////////////////////
PowerBusElectInputData::PowerBusElectInputData(const bool   malfBlockageFlag,
                                               const double malfBlockageValue,
                                               const double maxConductance,
                                               const double minConductance,
                                               const int    connection0)
:
        GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue),
        mMaxConductance(maxConductance),
        mMinConductance(minConductance),
        mConnection0(connection0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Power Bus Input Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
PowerBusElectInputData::~PowerBusElectInputData()
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @param  that    (--)  source to copy
/// @details  Copy Construct the PowerBusElect Input Data Object
///////////////////////////////////////////////////////////////////////////////////////////////
PowerBusElectInputData::PowerBusElectInputData(const PowerBusElectInputData& that)

:
                GunnsBasicLinkInputData(that.mMalfBlockageFlag, that.mMalfBlockageValue),
                mMaxConductance(that.mMaxConductance),
                mMinConductance(that.mMinConductance),
                mConnection0(that.mConnection0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the PowerBusElect
///////////////////////////////////////////////////////////////////////////////////////////////////
PowerBusElect::PowerBusElect()
:
                        GunnsBasicLink(1),
                        mLoad(0),
                        mLoadChangeTolerance(DBL_EPSILON),        // by how many mhos a load conductance has to change before it affects the GUNNS network
                        mNumLoads(0),                             // PowerBus should always have at least one load
                        mBusVoltage(0.0),
                        mTotalCPowerLoadsPower(0.0),
                        mFlux(0.0),
                        mResLoadsConductance(0.0),
                        mCPowerLoadsConductance(0.0),
                        mActiveResLoadsConductance(0.0),
                        mActiveCPowerLoadsConductance(0.0),
                        mMaxConductance(0.0),
                        mMinConductance(0.0),
                        mMaxResistance(0.0),
                        mNonLinear(false),
                        mSelfSealing0(false),
                        mSealed(false)
{
    mPlug[0] = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Jumper Object
////////////////////////////////////////////////////////////////////////////////////////////////////
PowerBusElect::~PowerBusElect()
{
    cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   configData   (--) Reference to Link Config Data
/// @param[in]   inputData    (--) Reference to Link Input Data
/// @param[in]   networkLinks (--) Reference to the Network Link Vector
/// @param[in]   loads        (--) Pointer to UserLoads
/// @param[in]   ports        (--) GUNNS network I/O Port Mapping
/// @throw TsInitializationException
/// @details  Initialize the PowerBusElect with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElect::initialize(      PowerBusElectConfigData &configData,
        const PowerBusElectInputData &inputData,
        std::vector<GunnsBasicLink*>&networkLinks,
        UserLoadBase **loads,
        const int *ports)
{
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    validateData(configData,inputData,loads);

    mNumLoads = configData.mNumLoads;
    mMaxConductance = inputData.mMaxConductance;
    mMaxResistance = 1.0/ mMinConductance;

    // be sure and erase anything from a possible previous initialize
    cleanup();

    // allocate memory for structures in Trick
    allocateArrays();

    for (int i=0; i < mNumLoads; i++) {
        //assign Loads to bus
        mLoad[i]=loads[i];
    }

    // initialize the plug data
    mPlug[0]      = configData.mPlug0;
    mSelfSealing0 = configData.mSelfSealing0;

    const int connection = inputData.mConnection0;
    if (mPlug[0]) {
        setPort(0, mPlug[0]->initialize(ports[0], connection));
    }

    mSealed = false;
    updateState();

    //checks whether any load is non linear, then sets variable isNonLinear to true if so.
    determineLinearity();

    /// - Once we're done with the config data object, call its cleanup methods so it
    ///   can delete its dynamic arrays.  This prevents Trick from checkpointing them.
    configData.cleanup();

    //set flag to true upon successful initialization
    mInitFlag = true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
///// @details  Allocate arrays based on the number of switches
//////////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElect::allocateArrays() {
   TS_NEW_PRIM_ARRAY_EXT(mLoad, mNumLoads, UserLoadBase*, std::string(mName) + ".mLoad");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
///// @details  Deletes allocated memory objects
//////////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElect::cleanup() {
    TS_DELETE_ARRAY(mLoad);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElect::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method that determines whether or not the Power Bus is a Linear or Non Linear Link.
/// @return bool
//////////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElect::validateData(const PowerBusElectConfigData &configData,
        const PowerBusElectInputData &inputData,
        UserLoadBase **loads)
{
    //Check Config Data
    //check for valid number of loads
    if ( configData.mNumLoads <=0 ) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Initialize:  Number of Loads must be greater than or equal to 1")
    }

    //Check Input Data
    //check for valid MinConductance
    if(0 >= mMinConductance){
        mMinConductance = 1.0E-8;
    }

    //check to ensure that the Max Conductance > 0
    if (inputData.mMaxConductance <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                "Initialize:  Max Conductance must be greater than 0")
    }

    //check for a null load pointer
    if (0 == loads){
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "Initialize:  Invalid pointer. **Loads is empty. ")
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method that determines whether or not the Power Bus is a Linear or Non Linear Link.
/// @return bool
//////////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElect::determineLinearity(){
    mNonLinear = false;
    for (int i = 0; i < mNumLoads; i++) {
        if(CONSTANT_POWER_LOAD == mLoad[i]->getLoadType()){
            mNonLinear = true;
            break;
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt  (s)  model iteration period
/// @details  Method for updating the link for the network
/////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElect::step(const double dt)
{
    //set mBusVoltage  = to the input voltage at port 0
    mBusVoltage =  mPotentialVector[0];

    // start by zeroing out the admittance matrix for this link object, Load Conductances, and Active Conductances
    mAdmittanceMatrix[0] = 0.0;
    mResLoadsConductance = 0.0;
    mTotalCPowerLoadsPower = 0.0;

    updateState(dt);

    /// - We also force power bus conductivity to zero if port is sealed.  This is
    ///   necessary since the power bus plug bypasses the normal port rule of not allowing multiple ports
    ///   connected to the same node through a socket, and placing a conductivity between the same node corrupts the
    ///   admittance matrix.
    if (!mSealed) {
        double lLoadResitance = 0.0;
        // loop through all the loads to build up the new admittance matrix for this link object
        for (int i = 0; i < mNumLoads; i++) {
            mLoad[i]->step(mBusVoltage);
            lLoadResitance = mLoad[i]->getResistance();
            if (CONSTANT_POWER_LOAD == mLoad[i]->getLoadType()) {      // this is a constant power load
                mTotalCPowerLoadsPower += mLoad[i]->getPower();        // sum up all Constant Power load values
            } else if (MsMath::isInRange(DBL_EPSILON, lLoadResitance, mMaxResistance)) {      // this is a resistive load, ignore value if resistance is invalid
                mResLoadsConductance += (1.0 / lLoadResitance);                           // sum up the load conductance
            }
        }
    }

    checkResLoadConductance();
    // add in the totaled conductance for all resistive loads to the admittance matrix at the input port (0)
    mAdmittanceMatrix[0] += mActiveResLoadsConductance;

    // total up conductance (1 / resistance) of any and all constant-power loads
    if (mBusVoltage > DBL_EPSILON) {
        mCPowerLoadsConductance = mTotalCPowerLoadsPower / (mBusVoltage * mBusVoltage);
    }
    checkCPLoadConductance();

    // add in the totaled conductance for all constant power loads to the admittance matrix at the input port (0)
    mAdmittanceMatrix[0] += mActiveCPowerLoadsConductance;

    //-------------------------------------------------------------------------------------------------------------
    // fix for a potential floating node. Allows a floating node to go to zero when disconnected from the network.
    //-------------------------------------------------------------------------------------------------------------
    mAdmittanceMatrix[0] += DBL_EPSILON / .0000001;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt         (s)  model iteration period
/// @param[in]   minorStep  (--) current minor step inside a major step
/// @details  Method for updating the link for the network - non-linear link
/////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElect::minorStep(const double dt, const int minorStep __attribute__((unused))) {
	step(dt);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Method for computing the flows across the link
////////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElect::computeFlows(const double dt __attribute__((unused))) {
	mBusVoltage = mPotentialVector[0];
    mFlux = mAdmittanceMatrix[0] * mBusVoltage;

    // use the total current to figure out the power used for all the device
    mPower = (mFlux * mBusVoltage);
    mPotentialDrop = mBusVoltage;

    //transport flux method for a one port link
    if (mFlux > 0.0 && mNodeMap[0] != getGroundNodeIndex()) {
    	mNodes[0]->collectOutflux(mFlux);
    } //else if (mNodeMap[0] == getGroundNodeIndex()){
//        std::cout<<"PowerBusElect::computeFlows - node is connected to ground" << std::endl;
 //   }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  Updates the jumper's conductivity based on the plug connection states and whether the
///           plugs are self-sealing.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElect::updateState(const double)
{
    /// - Update the plugs and map ports to the connected nodes if they have changed.
    if (mPlug[0]) {
        int node = mPlug[0]->update(mNodeMap[0]);
        if (node != mNodeMap[0]) {
            setPort(0, node);
        }
    }

    /// - If the plug has a value and is disconnected and self-sealing, then the
    ///   entire powerbus is considered sealed and blocks flow.
    mSealed = (mPlug[0] and mSelfSealing0 and
            mPlug[0]->getNoConnection() == mPlug[0]->getActiveConnection());
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method that ensures Load conductance is within desired bounds.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElect::checkResLoadConductance()
{
    //Ensures that the Load Conductance is set within the specified Values
    if(mResLoadsConductance > mMaxConductance){
        mResLoadsConductance = mMaxConductance;
    }
    else if(mResLoadsConductance < mMinConductance) {
        mResLoadsConductance = 0.0;
    }

    // if there's a big enough change in the resistive loads, flag to recalculate the admittance matrix
    if (fabs(mActiveResLoadsConductance - mResLoadsConductance) > mLoadChangeTolerance) {
        mActiveResLoadsConductance = mResLoadsConductance;
        mAdmittanceUpdate = true;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method that ensures Load conductance is within desired bounds.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void PowerBusElect::checkCPLoadConductance()
{
    //Ensures that the Constant Power Load Conductance is set within the specified Values
    if(mCPowerLoadsConductance > mMaxConductance){
        mCPowerLoadsConductance = mMaxConductance;
    }
    else if(mCPowerLoadsConductance < mMinConductance) {
        mCPowerLoadsConductance = 0.0;
    }

    // if there's a big enough change in the constant power loads, flag to recalculate the admittance matrix
    if (fabs(mActiveCPowerLoadsConductance - mCPowerLoadsConductance) > mLoadChangeTolerance) {
        mActiveCPowerLoadsConductance = mCPowerLoadsConductance;
        mAdmittanceUpdate = true;
    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns True if the link is Non Linear
/// @return   bool
/////////////////////////////////////////////////////////////////////////////////////////////////
bool PowerBusElect::isNonLinear()
{
    return mNonLinear;
}
