/**
@file
@brief    GUNNS Basic Source Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling the GUNNS basic source.)

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
#include "GunnsBasicSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name  (--) Link name
/// @param[in] nodes (--) Network nodes array
///
/// @details  Constructs the Basic Source Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSourceConfigData::GunnsBasicSourceConfigData(const std::string& name,
                                                       GunnsNodeList*     nodes)
    :
    GunnsBasicLinkConfigData(name, nodes)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy Constructs the Basic Source Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSourceConfigData::GunnsBasicSourceConfigData(const GunnsBasicSourceConfigData& that)
    :
    GunnsBasicLinkConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Source Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSourceConfigData::~GunnsBasicSourceConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
/// @param[in] sourceFlux        (--) Initial demanded flux of the link
///
/// @details  Default constructs this Basic Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSourceInputData::GunnsBasicSourceInputData(const bool   malfBlockageFlag,
                                                     const double malfBlockageValue,
                                                     const double sourceFlux)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue),
    mSourceFlux(sourceFlux)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Basic Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSourceInputData::GunnsBasicSourceInputData(const GunnsBasicSourceInputData& that)
    :
    GunnsBasicLinkInputData(that),
    mSourceFlux(that.mSourceFlux)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Basic Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSourceInputData::~GunnsBasicSourceInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructs the Basic Source Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSource::GunnsBasicSource():
    GunnsBasicLink(NPORTS),
    mSourceFlux(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructs the Basic Source Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicSource::~GunnsBasicSource()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Link Config Data
/// @param[in]     inputData    (--) Link Input Data
/// @param[in,out] networkLinks (--) Reference to the Solver Links
/// @param[in]     port0        (--) Port 0 Node Mapping
/// @param[in]     port1        (--) Port 1 Node Mapping
///
/// @throws   TsInitializationException
///
/// @details This initializes the link and sets up its connectivity to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSource::initialize(const GunnsBasicSourceConfigData& configData,
                                  const GunnsBasicSourceInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&     networkLinks,
                                  const int                         port0,
                                  const int                         port1)
{
    /// - Initialize the parent class.
    int ports[2] = {port0, port1};
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize class attributes.
    mSourceFlux = inputData.mSourceFlux;

    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Validates the link initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSource::validate() const
{
    /// - There is currently nothing to check.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSource::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details This updates the link during a calculation frame.
///          This class creates a fixed flux between two nodes.  One of the nodes may be
///          GROUND, or both may be normal network nodes.  Flow direction is defined positive
///          from Port0 to Port1, so it is best to use Port0 as the GROUND node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSource::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    updateState(dt);

    /// - The actual flux through the link will be the desired flux (mSourceFlux), reduced by the
    ///   blockage malfunction, if it is active.
    mFlux = mSourceFlux;
    if (mMalfBlockageFlag) {
        mFlux *= (1.0 - mMalfBlockageValue);
    }
    buildSource();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Computes the flows across the link
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicSource::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = getDeltaPotential();
    computePower();
    transportFlux();
}
