/**
@file
@brief    GUNNS Basic Potential Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling the GUNNS basic potential.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
    (
     (GunnsBasicConductor.o)
    )

 PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2011-02) (Initial Prototype))
     (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))
    )
*/
#include "GunnsBasicPotential.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                (--) Link name
/// @param[in] nodes               (--) Network nodes array
/// @param[in] defaultConductivity (--) Default conductivity of the link
///
/// @details  Constructs the Basic Potential Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicPotentialConfigData::GunnsBasicPotentialConfigData(const std::string& name,
                                                             GunnsNodeList*     nodes,
                                                             const double       defaultConductivity)
    :
    GunnsBasicConductorConfigData(name, nodes, defaultConductivity)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from
///
/// @details  Copy Constructs the Basic Potential Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicPotentialConfigData::GunnsBasicPotentialConfigData(
        const GunnsBasicPotentialConfigData& that)
    :
    GunnsBasicConductorConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Potential Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicPotentialConfigData::~GunnsBasicPotentialConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
/// @param[in] sourcePotential   (--) Initial potential rise of the link
///
/// @details  Default constructs this Basic Potential input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicPotentialInputData::GunnsBasicPotentialInputData(const bool   malfBlockageFlag,
                                                           const double malfBlockageValue,
                                                           const double sourcePotential)
    :
    GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue),
    mSourcePotential(sourcePotential)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Basic Potential input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicPotentialInputData::GunnsBasicPotentialInputData(const GunnsBasicPotentialInputData& that)
    :
    GunnsBasicConductorInputData(that),
    mSourcePotential(that.mSourcePotential)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Basic Potential input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicPotentialInputData::~GunnsBasicPotentialInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Basic Potential Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicPotential::GunnsBasicPotential()
    :
    GunnsBasicConductor(),
    mSourcePotential(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Potential Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicPotential::~GunnsBasicPotential()
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
/// @details  Initializes the basic potential link with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicPotential::initialize(const GunnsBasicPotentialConfigData& configData,
                                     const GunnsBasicPotentialInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        networkLinks,
                                     const int                            port0,
                                     const int                            port1)
{
    /// - Initialize the parent class.
    GunnsBasicConductor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize class attributes.
    mSourcePotential = inputData.mSourcePotential;

    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates the the basic potential object
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicPotential::validate() const
{
    /// - There is currently nothing to check.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicPotential::restartModel()
{
    /// - Reset the base class.
    GunnsBasicConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Updates the link during the time step
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicPotential::step(const double dt)
{
    GunnsBasicConductor::step(dt);
    buildPotential();
}
