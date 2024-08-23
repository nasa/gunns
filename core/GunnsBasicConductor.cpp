/**
@file
@brief    GUNNS Basic Conductor Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling the GUNNS basic conductor.)

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
#include "GunnsBasicConductor.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                (--) Link name
/// @param[in] nodes               (--) Network nodes array
/// @param[in] defaultConductivity (--) Default conductivity of the link
///
/// @details  Constructs the Basic Conductor Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicConductorConfigData::GunnsBasicConductorConfigData(const std::string& name,
                                                             GunnsNodeList*     nodes,
                                                             const double       defaultConductivity)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mDefaultConductivity(defaultConductivity)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from
///
/// @details  Copy Constructs the Basic Conductor Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicConductorConfigData::GunnsBasicConductorConfigData(
        const GunnsBasicConductorConfigData& that)
    :
    GunnsBasicLinkConfigData(that),
    mDefaultConductivity(that.mDefaultConductivity)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Conductor Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicConductorConfigData::~GunnsBasicConductorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
///
/// @details  Default constructs this Basic Conductor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicConductorInputData::GunnsBasicConductorInputData(const bool   malfBlockageFlag,
                                                           const double malfBlockageValue)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Basic Conductor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicConductorInputData::GunnsBasicConductorInputData(const GunnsBasicConductorInputData& that)
    :
    GunnsBasicLinkInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Basic Conductor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicConductorInputData::~GunnsBasicConductorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Basic Conductor Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicConductor::GunnsBasicConductor():
    GunnsBasicLink(NPORTS),
    mEffectiveConductivity(0.0),
    mDefaultConductivity  (0.0),
    mSystemConductance    (0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Conductor Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicConductor::~GunnsBasicConductor()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data
/// @param[in]     inputData    (--) Reference to Link Input Data
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector
/// @param[in]     port0        (--) Port 0 Mapping
/// @param[in]     port1        (--) Port 1 Mapping
///
/// @throws   TsInitializationException
///
/// @details  Initializes the basic conductor with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicConductor::initialize(const GunnsBasicConductorConfigData& configData,
                                     const GunnsBasicConductorInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        networkLinks,
                                     const int                            port0,
                                     const int                            port1)
{
    /// - Initialize the parent class.
    int ports[2] = {port0, port1};
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize class attributes.
    mDefaultConductivity   = configData.mDefaultConductivity;
    mEffectiveConductivity = mDefaultConductivity;
    mSystemConductance     = 0.0;

    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates the link
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicConductor::validate() const
{
    /// - Issue an error on conductivity being less than zero.
    if (mEffectiveConductivity < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has conductivity < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicConductor::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mEffectiveConductivity = 0.0;
    mSystemConductance     = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Updates the basic conductor during the time step
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicConductor::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Call the virtual updateState method so a derived model can calculate a new conductivity.
    /// - Default the effective conductivity, then call the virtual updateState method so a derived
    ///   model can calculate a new conductivity.
    mEffectiveConductivity = mDefaultConductivity;
    updateState(dt);

    /// - Set the link's system conductance based on the effective conductivity and the blockage
    ///   fraction.
    mSystemConductance = mEffectiveConductivity;
    if (mMalfBlockageFlag) {
        mSystemConductance *= (1.0 - mMalfBlockageValue);
    }
    mSystemConductance = MsMath::limitRange(0.0, mSystemConductance, mConductanceLimit);
    buildConductance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Protected method for building the conductance into the system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicConductor::buildConductance()
{
    if (std::fabs(mAdmittanceMatrix[0] - mSystemConductance) > 0.0) {
        mAdmittanceMatrix[0]   =  mSystemConductance;
        mAdmittanceMatrix[1]   = -mAdmittanceMatrix[0];
        mAdmittanceMatrix[2]   = -mAdmittanceMatrix[0];
        mAdmittanceMatrix[3]   =  mAdmittanceMatrix[0];

        mAdmittanceUpdate = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Method for computing the flows across the link
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicConductor::computeFlows(const double dt)
{
    mPotentialDrop = getDeltaPotential();
    computeFlux();
    updateFlux(dt, mFlux);
    computePower();
    transportFlux();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Compute flux across the link, defined positive from port 0 to port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicConductor::computeFlux()
{
    const double hiP = std::max(mPotentialVector[0], mPotentialVector[1]);
    if (std::fabs(mPotentialDrop) < (hiP * m100EpsilonLimit)) {
        /// - Zero flux if dP is too low.  This eliminates most false quantity leak due to rounding
        ///   error in the solver.
        mFlux = 0.0;
    } else {
        mFlux = mPotentialDrop * mAdmittanceMatrix[0];
    }
}
