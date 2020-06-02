/***************************************** TRICK HEADER ********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   (
    (core/GunnsBasicPotential.o)
   )
**************************************************************************************************/
#include "GunnsThermalPotential.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "aspects/thermal/PtcsMacros.hh"
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    name                 --  Link name
/// @param[in]    nodes                --  Network nodes array
/// @param[in]    defaultConductivity  --  Default conductivity of the link
///
/// @details  Constructs the Thermal Potential Config data
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPotentialConfigData::GunnsThermalPotentialConfigData
                                                           (const std::string& name,
                                                            GunnsNodeList*     nodes,
                                                            const double       defaultConductivity)
    :
    GunnsBasicPotentialConfigData(name, nodes, defaultConductivity)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   that -- Object to copy from
///
/// @details  Copy Constructs the Thermal Potential Config data
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPotentialConfigData::GunnsThermalPotentialConfigData(
        const GunnsThermalPotentialConfigData& that)
    :
    GunnsBasicPotentialConfigData(that)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Thermal Potential Config Data Object
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPotentialConfigData::~GunnsThermalPotentialConfigData()
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    malfBlockageFlag   -- Blockage malfunction flag
/// @param[in]    malfBlockageValue  -- Blockage malfunction fractional value (0-1)
/// @param[in]    sourcePotential    -- Initial potential rise of the link
///
/// @details  Default constructs this Thermal Potential input data.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPotentialInputData::GunnsThermalPotentialInputData(const bool    malfBlockageFlag,
                                                               const double  malfBlockageValue,
                                                               const double  sourcePotential)
    :
    GunnsBasicPotentialInputData(malfBlockageFlag, malfBlockageValue, sourcePotential)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  --  Object to copy
///
/// @details  Copy constructs this Thermal Potential input data.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPotentialInputData::GunnsThermalPotentialInputData
                                (const GunnsThermalPotentialInputData& that)
    :
    GunnsBasicPotentialInputData(that)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Thermal Potential input data.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPotentialInputData::~GunnsThermalPotentialInputData()
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Thermal Potential Object
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPotential::GunnsThermalPotential()
    :
    GunnsBasicPotential(),
    mHeatFluxIntoNode(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Thermal Potential Object
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalPotential::~GunnsThermalPotential()
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   -- Reference to Link Config Data
/// @param[in]     inputData    -- Reference to Link Input Data
/// @param[in,out] networkLinks -- Reference to the Network Link Vector
/// @param[in]     port0        -- Port 0 Mapping
/// @param[in]     port1        -- Port 1 Mapping
///
/// @throws   TsInitializationException
///
/// @details  Initializes the Thermal Potential link with config and input data.
///////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPotential::initialize(const GunnsThermalPotentialConfigData& configData,
                                       const GunnsThermalPotentialInputData&  inputData,
                                       std::vector<GunnsBasicLink*>&          networkLinks,
                                       const int                              port0,
                                       const int                              port1)
{
    /// - Initialize the parent class.
    GunnsBasicPotential::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalPotential::restartModel()
{
    /// - Reset the base class.
    GunnsBasicPotential::restartModel();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port  (--) The port to be assigned
/// @param[in] node  (--) The desired node to assign the port to
///
/// @returns   boolean -- Returns true if all rules checks pass
///
/// @details   Checks the requested port & node arguments for validity against rules that apply to
///            this specific class. These are:
///            -A GunnsThermalPotential must map port 0 to the network's vacuum boundary node.
///            -A GunnsThermalPotential must not map port 1 to the network's vacuum boundary node.
///////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsThermalPotential::checkSpecificPortRules(const int port, const int node) const
{

    /// - Flag is set to true if ports are set up correctly
    bool result = true;

    /// - Report if port 1 is the vacuum boundary node.
    if ( (1 == port) && (node == getGroundNodeIndex()) )
    {
        TS_PTCS_WARNING("Cannot assign port 1 to the boundary node.");

        /// - Invalidate the flag
        result = false;

    }

    /// - Report if port 0 is not the vacuum boundary node.
    else if ( (0 == port) && (node != getGroundNodeIndex()) )
    {
        TS_PTCS_WARNING("Must assign port 0 to the boundary node");

        /// - Invalidate the flag
        result = false;
    }

    /// - return flag, which will be false if the ports are not set up correctly
    return result;
}
