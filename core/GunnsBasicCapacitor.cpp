/**
@file
@brief    GUNNS Basic Capacitor Link implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling the GUNNS basic capacitor.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
    (
     (GunnsBasicLink.o)
    )

 PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2011-02) (Initial Prototype))
     (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))
    )
*/
#include "GunnsBasicCapacitor.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name  (--) Link name
/// @param[in] nodes (--) Network nodes array
///
/// @details  Constructs the Basic Capacitor Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicCapacitorConfigData::GunnsBasicCapacitorConfigData(const std::string& name,
                                                             GunnsNodeList*     nodes)
    :
    GunnsBasicLinkConfigData(name, nodes)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input Source to Copy
///
/// @details  Copy Constructs the Basic Capacitor Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicCapacitorConfigData::GunnsBasicCapacitorConfigData(
        const GunnsBasicCapacitorConfigData& that)
    :
    GunnsBasicLinkConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Capacitor Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicCapacitorConfigData::~GunnsBasicCapacitorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
/// @param[in] capacitance       (--) Initial capacitance of the link
/// @param[in] potential         (--) Initial potential of the capacitive node
///
/// @details  Default constructs this Basic Capacitor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicCapacitorInputData::GunnsBasicCapacitorInputData(const bool   malfBlockageFlag,
                                                           const double malfBlockageValue,
                                                           const double capacitance,
                                                           const double potential)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue),
    mCapacitance(capacitance),
    mPotential(potential)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Basic Capacitor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicCapacitorInputData::GunnsBasicCapacitorInputData(const GunnsBasicCapacitorInputData& that)
    :
    GunnsBasicLinkInputData(that),
    mCapacitance(that.mCapacitance),
    mPotential(that.mPotential)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Basic Capacitor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicCapacitorInputData::~GunnsBasicCapacitorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Gunns Basic Capacitor Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicCapacitor::GunnsBasicCapacitor()
    :
    GunnsBasicLink(NPORTS),
    mCapacitance(0.0),
    mEditCapacitanceFlag(false),
    mEditCapacitance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Capacitor Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicCapacitor::~GunnsBasicCapacitor()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data
/// @param[in]     inputData    (--) Reference to LInk Input Data
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector
/// @param[in]     port0        (--) Port 0 Mapping
/// @param[in]     port1        (--) Port 1 Mapping
///
/// @details  Initializes the Basic Capacitor
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicCapacitor::initialize(const GunnsBasicCapacitorConfigData& configData,
                                     const GunnsBasicCapacitorInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        networkLinks,
                                     const int                            port0,
                                     const int                            port1)
{
    /// - Initialize the parent class.
    int ports[2] = {port0, port1};
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag            = false;

    /// - Initialize class attributes.
    mCapacitance         = inputData.mCapacitance;
    initPotential(inputData.mPotential);
    mEditCapacitanceFlag = false;
    mEditCapacitance     = 0.0;

    validate();

    /// - Set init flag on successful validation.
    mInitFlag            = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates the Basic Capacitor
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicCapacitor::validate() const
{
    /// - Issue an error on capacitance being less than zero.
    if (mCapacitance < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Link has capacitance < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicCapacitor::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.  mCapacitance is treated as config
    ///   data.
    mEditCapacitanceFlag = false;
    mEditCapacitance     = 0.0;
}

//TODO problems with this in a non-linear network:
// - The capacitance effect, added to the network's SOE in step (minor step 1), keeps re-integrating
//   the flow for the whole major step in each minor step network solution.  This makes the capacitor
//   act like it's charging or discharging an entire major step on each minor step, instead of just
//   once.  The capacitance effect, which is time-dependent, includes the integral of its
//   charge/discharge current in the network solution, so each time the network is solved for a minor
//   step, the integrated current gets doubled up again.
// - because of above, nodes don't converge because on the next minor step, needed to check for
//   node convergence by comparing subsequent node potential, the capacitor has re-integrated the
//   full charge/discharge flow to a new potential, instead of keeping the same potential
// - because this link doesn't report non-linear to the solver, solver doesn't call this link's
//   confirmSolutionAcceptable
//   - however, we don't really need this as this link has no reason to reject or delay
// - solver doesn't call this resetLastMinorStep
//   - might not need this either, if we stored our reference t-1 potentials
// - We could do something like only apply the capacitance time effect on minor step 1, and
//   then hold potential constant on subsequent minor steps, either by switching to infinite capacitance
//   or an ideal potential source (these 2 would look similar in the SOE)
//   - however, if the circuit changes due to a link rejecting like a switch tripping or a diode
//     switching bias, the solver only resets potentials to the previous minor step, and not the
//     previous major step.  The saved potential source of the capacitor's minor steps is no longer
//     correct, and any further minor steps are pointless.
//   - but we can't make the solver set potentials back to the major step, only to the previous
//     minor step, which is also invalid.
//   - so we have no way to fix this.
// - we might have to redesign as essentially a battery, as a potential source that integrates the
//   final current to update the potential source next pass.
//   Can compute conductance of the potential source to mimic the
// - It might get really complicated
//   - maybe implement as new GunnsElectCapacitor instead of here, then would always be non-linear

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Method for updating the link for the network
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicCapacitor::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Process a capacitance edit.
    if (mEditCapacitanceFlag) {
        mEditCapacitanceFlag = false;
        setCapacitance(mEditCapacitance);
    }

    /// - Call virtual updateState method so a derived model can calculate a new capacitance.
    updateState(dt);

    /// - Build this link's contributions to the system of equations.
    buildCapacitance(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Method for updating the link flows
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicCapacitor::computeFlows(const double dt)
{
    mPotentialDrop = getDeltaPotential();
    computeFlux();

    /// - Call the physical model so it can update its quantity.
    updateFlux(dt, mFlux);

    computePower();
    transportFlux();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] toPotential (--) Initial potential of the capacitive node
///
/// @details  This method initializes the potential of the node that this capacitive link applies
///           its capacitance to.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicCapacitor::initPotential(const double toPotential)
{
    mPotentialVector[0] = toPotential;
    mNodes[0]->setPotential(toPotential);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Builds the admittance matrix and source vector of the link
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicCapacitor::buildCapacitance(const double dt)
{
    double admittance;

    /// - Calculate the link's contribution to the system admittance matrix.  Protect for divide
    ///   by zero in dt, which will occur when running in Freeze.  In Freeze, dividing
    ///   capacitance by DBL_EPSILON makes the node capacitance in the system of equations
    ///   essentially infinite, which will freeze node potential in place.
    if (dt < DBL_EPSILON) {
        admittance =  mCapacitance / DBL_EPSILON;
    }
    else {
        admittance =  mCapacitance / dt;
    }

    /// - Build the system admittance matrix contribution.
    if (std::fabs(mAdmittanceMatrix[0] -  admittance) > 0.0) {
        mAdmittanceMatrix[0] =  admittance;
        mAdmittanceMatrix[1] = -mAdmittanceMatrix[0];
        mAdmittanceMatrix[2] = -mAdmittanceMatrix[0];
        mAdmittanceMatrix[3] =  mAdmittanceMatrix[0];
        mAdmittanceUpdate = true;
    }

    /// - Calculate the link's contribution to the system source vector.
    mSourceVector[0] = (mPotentialVector[0] - mPotentialVector[1]) * mAdmittanceMatrix[0];
    mSourceVector[1] = -mSourceVector[0];
}
