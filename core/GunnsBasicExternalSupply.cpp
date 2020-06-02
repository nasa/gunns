/**
@file
@brief    GUNNS Basic External Supply Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Basic External Supply)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (
    (GunnsBasicSource.o)
    (software/SimBus/SimBusQutils.o)
   )

 PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2011-02) (Initial Prototype))
    )
*/
#include "GunnsBasicExternalSupply.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name  (--) Link name
/// @param[in] nodes (--) Network nodes array
///
/// @details  Default GUNNS Basic External Supply Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalSupplyConfigData::GunnsBasicExternalSupplyConfigData(const std::string& name,
                                                                       GunnsNodeList*     nodes)
    :
    GunnsBasicSourceConfigData(name, nodes)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  GUNNS Basic External Supply Config Data Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalSupplyConfigData::GunnsBasicExternalSupplyConfigData
        (const GunnsBasicExternalSupplyConfigData& that)
    :
    GunnsBasicSourceConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Basic External Supply Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalSupplyConfigData::~GunnsBasicExternalSupplyConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
/// @param[in] sourceFlux        (--) Initial demanded flux of the link
///
/// @details  Default constructs this Basic External Supply input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalSupplyInputData::GunnsBasicExternalSupplyInputData(const bool   malfBlockageFlag,
                                                                     const double malfBlockageValue,
                                                                     const double sourceFlux)
    :
    GunnsBasicSourceInputData(malfBlockageFlag, malfBlockageValue, sourceFlux)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Basic External Supply input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalSupplyInputData::GunnsBasicExternalSupplyInputData(
        const GunnsBasicExternalSupplyInputData& that)
    :
    GunnsBasicSourceInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Basic External Supply input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalSupplyInputData::~GunnsBasicExternalSupplyInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructs the Link
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalSupply::GunnsBasicExternalSupply()
    :
    GunnsBasicSource(),
    mSupplyPotential(0.0),
    mDemandFlux_queue(0),
    mDemandFlux_queue_size(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructs the Link
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalSupply::~GunnsBasicExternalSupply()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--) GUNNS Basic External Supply Config Data
/// @param[in]      inputData    (--) GUNNS Basic External Supply Input Data
/// @param[in,out]  networkLinks (--) Network links
/// @param[in]      port0        (--) Network port 0
/// @param[in]      port1        (--) Network port 1
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Basic External Supply with configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicExternalSupply::initialize(const GunnsBasicExternalSupplyConfigData& configData,
                                          const GunnsBasicExternalSupplyInputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             networkLinks,
                                          const int                                 port0,
                                          const int                                 port1)
{
    /// - Initialize & validate parent.
    GunnsBasicSource::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Validate initialization.
    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Validates this GUNNS Basic External Supply initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicExternalSupply::validate() const
{
    /// - There is currently nothing to validate.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicExternalSupply::restartModel()
{
    /// - Reset the base class.
    GunnsBasicSource::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned
/// @param[in] node (--) The desired node to assign the port to
///
/// @returns  bool  (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsBasicExternalSupply must not map port 0 to the network ground node.
///           - A GunnsBasicExternalSupply must map port 1 to the network ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsBasicExternalSupply::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 0 is the ground node.
    if ((0 == port) && (node == getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 0 to the boundary node.");
        result = false;
    }

    /// - Fail if port 1 is not the ground node.
    else if ((1 == port) && (node != getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: must assign port 1 to the boundary node.");
        result = false;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Handles data read from the external network's demand link, via the sim bus.  Data is
///          moved from the sim but input queue into the link internal members.  The average of all
///          the queued demanded fluxes since last pass, when integrated over this network's time
///          step, produces the same net flux transfer in out network as the demand network, so
///          quantity is conserved.
///
///          If there is no queue, then it is assumed that the user is setting mSourceFlux directly
///          and we skip updating it here.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicExternalSupply::processInputs()
{
    if (mDemandFlux_queue and mDemandFlux_queue_size > 0) {
        mSourceFlux = SimBusQutils::getAverage(mDemandFlux_queue, mDemandFlux_queue_size);
    }
}
