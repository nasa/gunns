/**
@file
@brief    GUNNS Fluid Potential Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Fluid Potential)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (
    (GunnsFluidConductor.o)
    (GunnsFluidNode.o)
    (aspects/fluid/fluid/PolyFluid.o)
   )

 PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2011-02) (Initial Prototype))
     (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))
    )
*/
#include "GunnsFluidPotential.hh"
#include "aspects/fluid/fluid/PolyFluid.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--) Link name
/// @param[in] nodes                (--) Network nodes array
/// @param[in] maxConductivity      (m2) Maximum possible effective conductivity of the link
/// @param[in] expansionScaleFactor (--) Scale factor for isentropic cooling across the link (0-1)
///
/// @details  Default GUNNS Fluid Potential Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPotentialConfigData::GunnsFluidPotentialConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       maxConductivity,
        const double       expansionScaleFactor)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  GUNNS Fluid Potential Config Data Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPotentialConfigData::GunnsFluidPotentialConfigData(
        const GunnsFluidPotentialConfigData& that)
    :
    GunnsFluidConductorConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Potential Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPotentialConfigData::~GunnsFluidPotentialConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--)  Blockage malfunction flag
/// @param[in] malfBlockageValue (--)  Blockage malfunction fractional value (0-1)
/// @param[in] sourcePressure    (kPa) Initial pressure rise of the link
///
/// @details  Default GUNNS Fluid Potential Input Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPotentialInputData::GunnsFluidPotentialInputData(const bool   malfBlockageFlag,
                                                           const double malfBlockageValue,
                                                           const double sourcePressure)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mSourcePressure(sourcePressure)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy
///
/// @details  Copy constructs this Fluid Potential input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPotentialInputData::GunnsFluidPotentialInputData(const GunnsFluidPotentialInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mSourcePressure(that.mSourcePressure)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Potential Input Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPotentialInputData::~GunnsFluidPotentialInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Potential Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPotential::GunnsFluidPotential()
    :
    GunnsFluidConductor(),
    mSourcePressure(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Potential Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPotential::~GunnsFluidPotential()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to link Config Data
/// @param[in]     inputData    (--) Reference to link Input Data
/// @param[in,out] networkLinks (--) Network links
/// @param[in]     port0        (--) Network port 0
/// @param[in]     port1        (--) Network port 1
///
/// @throws   TsInitializationException
///
/// @details  This method initializes this Fluid Potential link with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPotential::initialize(const GunnsFluidPotentialConfigData& configData,
                                     const GunnsFluidPotentialInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        networkLinks,
                                     const int                            port0,
                                     const int                            port1)
{
    /// - Initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag
    mInitFlag = false;

    /// - Initialize from config data.
    mSourcePressure = inputData.mSourcePressure;

    /// - Validate initialization.
    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Potential initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPotential::validate() const
{
    /// - There is currently nothing to check.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPotential::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Updates the State of the Link during the step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPotential::step(const double dt)
{
    /// - Step the parent conductor functionality.
    GunnsFluidConductor::step(dt);

    /// - Add the potential contribution.
    buildPotential();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the Admittance and Source terms for the potential source
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPotential::buildPotential()
{
    /// - Build the system source vector contribution.  The source pressure term is defined as the
    ///   pressure rise created by the potential source.  So that a positive source pressure creates
    ///   a flow in the positive direction (port 0 to port 1), we reverse the sign in the source
    ///   vector.
    mSourceVector[0] = -mSourcePressure * mAdmittanceMatrix[0];
    mSourceVector[1] = -mSourceVector[0];
}
