/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (EPS Diode class)

 REQUIREMENTS:
 (none)

 REFERENCE:
 (none)

 ASSUMPTIONS AND LIMITATIONS:
 ()

 LIBRARY DEPENDENCY:
 (
 (DiodeElect.o)
 (software/exceptions/TsConfigurationException.o)
 (software/exceptions/TsInitializationException.o)
 )

 PROGRAMMERS:
 (
 (Minh Tran) (L-3) (July 2011) (Initial Prototype)
 (Nicholas Kaufmann (L-3) (Oct 2011) (Added Config and Input Classes)
 (Mark Wiederholt (Lockhead Martin) (Nov 2014) (Updated comments and error messages)
 )
 **************************************************************************************************/
#include "DiodeElect.hh"
#include <math.h>
#include <cfloat>
#include "software/exceptions/TsConfigurationException.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"


//===============================================================================================
// Diode Configuration Data
//===============================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    name                            -- Link name
/// @param    nodes                           -- Network nodes array
/// @param    diodeReverseResistance          -- Reverse Diode equivalent resistance
/// @param    diodeForwardResistance          -- Forward Diode equivalent resistance
///
/// @details  Constructs the Diode Configuration data
/////////////////////////////////////////////////////////////////////////////////////////////////
DiodeElectConfigData::DiodeElectConfigData(const std::string& name,
                                 GunnsNodeList* nodes,
                                 const double diodeReverseResistance,
                                 const double diodeForwardResistance)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mDiodeReverseResistance(diodeReverseResistance),
    mDiodeForwardResistance(diodeForwardResistance)
{
   return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
///
/// @details  Copy constructs this Diode Elect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DiodeElectConfigData::DiodeElectConfigData(const DiodeElectConfigData& that)
    :
    GunnsBasicLinkConfigData(that),
    mDiodeReverseResistance(that.mDiodeReverseResistance),
    mDiodeForwardResistance(that.mDiodeForwardResistance)
{
    // nothing to do
}



///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Diode Configuration Data Object
///////////////////////////////////////////////////////////////////////////////////////////////
DiodeElectConfigData::~DiodeElectConfigData() {
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    malfBlockageFlag    -- Blockage malfunction flag
/// @param    malfBlockageValue   -- Blockage malfunction fractional value (0-1)
///
/// @details  Default constructs this Diode Elect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DiodeElectInputData::DiodeElectInputData(const bool   malfBlockageFlag,
                                         const double malfBlockageValue,
                                         const bool   initReverseBiased)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue),
    mInitReverseBiased(initReverseBiased)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
///
/// @details  Copy constructs this Diode Elect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DiodeElectInputData::DiodeElectInputData(const DiodeElectInputData& that)
    :
    GunnsBasicLinkInputData(that),
    mInitReverseBiased(that.mInitReverseBiased)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Diode Elect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DiodeElectInputData::~DiodeElectInputData()
{
    // nothing to do
}




//===============================================================================================
// Diode
//===============================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Diode
//////////////////////////////////////////////////////////////////////////////////////////////
DiodeElect::DiodeElect() :
    GunnsBasicLink(NPORTS),
    mConductance(0.0),
    mActiveConductance(0.0),
    mCurrent(0.0),
    mDiodeReverseBias(false),
    mDiodePreviousReverseBias(true),
    mDiodeReverseResistance(0.0),
    mDiodeForwardResistance(0.0),
    mInitReversBiased(false)
{
// Do nothing
}


///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Diode Object
///////////////////////////////////////////////////////////////////////////////////////////////
DiodeElect::~DiodeElect() {
// Do nothing
}


////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector
/// @param[in]     port0        (--) Port 0 Mapping (Anode)
/// @param[in]     port1        (--) Port 1 Mapping (Cathode)
///
/// @throws TsInitializationException
///
/// @details  Initializes the Diode.
////////////////////////////////////////////////////////////////////////////////////////////////
void DiodeElect::initialize(const DiodeElectConfigData& configData,
                       const DiodeElectInputData& inputData,
                       std::vector<GunnsBasicLink*>& networkLinks,
                       const int port0,
                       const int port1)
{
    /// - Initialize the base class.
    int ports[2] = {port0,port1};
    GunnsBasicLink::initialize(configData,inputData,networkLinks, ports);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Throw exception if forward resistance is not greater than zero.
    if (configData.mDiodeForwardResistance <= 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_EPS, " Diode Initialization ", TsInitializationException,
                "Forward Resistance is less than or equal to zero", "DiodeElect");
    }

    /// - Throw exception if reverse resistance is not greater than zero.
    if (configData.mDiodeReverseResistance <= 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_EPS, " Diode Initialization ", TsInitializationException,
                "Reverse Resistance is less than or equal to zero", "DiodeElect");
    }

    /// - Throw exception if reverse resistance is less than forward resistance.
    if (configData.mDiodeReverseResistance < configData.mDiodeForwardResistance){
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_EPS, " Diode Initialization ", TsInitializationException,
                "Forward resistance is greater than Reverse Resistance", "DiodeElect");
    }
    mDiodeForwardResistance   = configData.mDiodeForwardResistance;
    mDiodeReverseResistance   = configData.mDiodeReverseResistance;
    mInitReversBiased         = inputData.mInitReverseBiased;
    mDiodeReverseBias         =  mInitReversBiased;
    mDiodePreviousReverseBias = !mInitReversBiased;

    /// - Warn of deprecation due to obsolescence by GunnsElectRealDiode.
    GUNNS_WARNING("this link is deprecated!  It is obsoleted by GunnsElectRealDiode.");

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DiodeElect::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   timeStep  (s)  time step
///
/// @details  Method for updating the link for the network
/////////////////////////////////////////////////////////////////////////////////////////////////
void DiodeElect::step(double timeStep __attribute__((unused))) {
    double Resistance;

    if (mDiodePreviousReverseBias != mDiodeReverseBias) {
        mDiodePreviousReverseBias = mDiodeReverseBias;
        mAdmittanceUpdate = true;
    }

    if (mAdmittanceUpdate) {
        for (int i = 0; i < mNumPorts * mNumPorts; i++) {
            mAdmittanceMatrix[i] = 0.0;
        }

        if (mDiodeReverseBias) {
            Resistance = mDiodeReverseResistance;
        } else {
            Resistance = mDiodeForwardResistance;
        }

        if (Resistance >= mDiodeReverseResistance){
            mAdmittanceMatrix[0] += 0.0;
            mAdmittanceMatrix[1] += 0.0;
            mAdmittanceMatrix[2] += 0.0;
            mAdmittanceMatrix[3] += 0.0;
        }else{
        mAdmittanceMatrix[0] += 1.0 / Resistance;
        mAdmittanceMatrix[1] += -1.0 / Resistance;
        mAdmittanceMatrix[2] += -1.0 / Resistance;
        mAdmittanceMatrix[3] += 1.0 / Resistance;
        }

    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timeStep  (s)  time step
/// @param[in] minorStep (--) The minor step number that the network is on
///
/// @details  Method for updating the link for the network - non-linear link
/////////////////////////////////////////////////////////////////////////////////////////////////
void DiodeElect::minorStep(double timeStep, const int minorStep __attribute__((unused))) {
    computeFlows(timeStep);
    step(timeStep);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   timeStep  (s)  time step
///
/// @details  Method for computing link flow
/////////////////////////////////////////////////////////////////////////////////////////////////
void DiodeElect::computeFlows(double timeStep __attribute__((unused))) {
    mPotentialDrop = mPotentialVector[0] - mPotentialVector[1];
    mCurrent = mPotentialDrop * mAdmittanceMatrix[0];
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns if the link is non-linear
/////////////////////////////////////////////////////////////////////////////////////////////////
bool DiodeElect::isNonLinear() {
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] minorStep (--) The minor step number that the network is on
///
/// @details  Method for confirming if the voltages is acceptable
/////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult DiodeElect::confirmSolutionAcceptable(
        const int minorStep __attribute__((unused)), const int absoluteStep __attribute__((unused))) {

    bool Acceptable = true;
    double deltaPotential = (mPotentialVector[1] - mPotentialVector[0]);

    /// - Check for disagreement between the actual voltages across the link and the diode bias.
    if (mDiodeReverseBias) {

        /// - Switch from reverse bias to forward bias and resolve the network.
        if (deltaPotential < -DBL_EPSILON) {
            Acceptable = false;
            mDiodeReverseBias = false;
        }

    } else {

        /// - Switch from forward bias to reverse bias and resolve the network.
        if (deltaPotential > DBL_EPSILON) {
            Acceptable = false;
            mDiodeReverseBias = true;
        }
    }

    if (Acceptable){
        return GunnsBasicLink::CONFIRM;
    }else{
        return GunnsBasicLink::REJECT;
    }
}
