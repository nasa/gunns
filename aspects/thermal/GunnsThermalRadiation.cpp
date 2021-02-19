/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
  (Models a thermal radiation effect. The basic function of this class is to change how admittance
   is calculated in order to create this thermal radiation effect. The GUNNS link BasicConductor
   serves as the parent class for ThermalRadiation. Due to this, it felt redundant to declare
   thermal-radiation-specific members. To remedy this, members declared in BasicConductor are used
   in ThermalRadiation methods (e.g. conductivity in the context of thermal radiation is the
   radiation coefficient. Instead of declaring members like radiationk, member nomenclature from
   BasicConductor is carried into ThermalRadiation).
  )

REFERENCE:
  ()

ASSUMPTIONS AND LIMITATIONS:
  (GunnsThermalRadiation assumes Radiation link has constant conductivity.)

LIBRARY_DEPENDENCY:
  (
   (core/GunnsBasicConductor.o)
  )

 PROGRAMMERS:
- ((Jason Harvey)      (L3) (2011-02) (Initial Prototype)
   (Kevin Supak)       (L3) (2011-02) (Updated to Coding Standards)
   (Ulisses Hernandez) (L3) (2012-01) (Updates, unit-test and code review)
  )
**************************************************************************************************/
#include "GunnsThermalRadiation.hh"
#include "math/MsMath.hh" // Needed for pow and DBL_EPSILON

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                (--) Link name
/// @param[in] nodes               (--) Network nodes array
/// @param[in] defaultConductivity (W/K) Default conductivity of the link
///
/// @details  Constructs the Thermal Radiation Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalRadiationConfigData::GunnsThermalRadiationConfigData
                                (const std::string& name, GunnsNodeList* nodes,
                                 const double defaultConductivity)
    :
    GunnsBasicConductorConfigData(name, nodes, defaultConductivity)
{
    // Nothing to do
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from
///
/// @details  Copy Constructs the Thermal Radiation Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalRadiationConfigData::GunnsThermalRadiationConfigData
                                (const GunnsThermalRadiationConfigData& that)
    :
    GunnsBasicConductorConfigData(that)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Destructs the Thermal Radiation Config Data
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalRadiationConfigData::~GunnsThermalRadiationConfigData()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] viewScalar        (--) scales effective conductivity based on changing view to space
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
///
/// @details  Default Constructs this Thermal Radiation input data.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalRadiationInputData::GunnsThermalRadiationInputData(const bool   malfBlockageFlag,
                                                               const double malfBlockageValue,
                                                               const double viewScalar)
    :
    GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue),
    iViewScalar(viewScalar)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Thermal Radiation input data.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalRadiationInputData::GunnsThermalRadiationInputData
                               (const GunnsThermalRadiationInputData& that)
    :
    GunnsBasicConductorInputData(that),
    iViewScalar(that.iViewScalar)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Destructs this Thermal Radiation Input Data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalRadiationInputData::~GunnsThermalRadiationInputData()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Thermal Radiation Effect
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalRadiation::GunnsThermalRadiation():
    GunnsBasicConductor(),
    mViewScalar(1.0)
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalRadiation::~GunnsThermalRadiation()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data
/// @param[in]     inputData    (--) Reference to Link Input Data
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector
/// @param[in]     port0        (--) Port 0 Mapping
/// @param[in]     port1        (--) Port 1 Mapping
///
/// @details  Initializes the GunnsThermalRadiation
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalRadiation::initialize(const GunnsThermalRadiationConfigData& configData,
                                       const GunnsThermalRadiationInputData&  inputData,
                                       std::vector<GunnsBasicLink*>&          networkLinks,
                                       const int                              port0,
                                       const int                              port1)
{
    /// - Initialize the parent class.
    GunnsBasicConductor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag that got set to "true" in parent-class initialize().
    mInitFlag = false;

    /// - Configures the radiation link with its viewing scalar.
    mViewScalar = inputData.iViewScalar;

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalRadiation::restartModel()
{
    /// - Reset the base class.
    GunnsBasicConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step
///
/// @details    Method for calculating the admittance of the radiation link and sending it to base
///             class buildConductance method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalRadiation::updateState(const double dt __attribute__((unused)))
{
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Linearize the radiative heat flux equation
///
/// @details  Heat transfer via radiation from hot to cold is computed as
///
///                Q = emissivity * stefan-boltzmann * effectiveArea * (T0^4 - T1^4)
///
///           In order to linearize this radiative heat flux equation, we calculate
///           an admittance A such that:
///
///                Q = A * (T0 - T1)
///
///           Thus:
///
///                A = C * (T0^4 - T1^4) / (T0 - T1)
///
///           Where the products of emissivity, stefan-boltzmann constant, and effective
///           Area have been lumped into one parameter: C, referred to as EffectiveConductivity.
///           Note that entries in the PotentialVector represent temperatures, as this is the
///           Potential analog for the thermal aspect.
////////////////////////////////////////////////////////////////////////////////////////////////////

    /// - Conditional Statement to ensure a division by zero does not take place. DBL_EPSILON is
    ///   used for tolerance comparison.
    if (fabs(mPotentialVector[0] - mPotentialVector[1]) > DBL_EPSILON)
    {
        /// - Admittance is calculated per the above comment block.
        const double T0_4 = mPotentialVector[0] * mPotentialVector[0] * mPotentialVector[0] * mPotentialVector[0];
        const double T1_4 = mPotentialVector[1] * mPotentialVector[1] * mPotentialVector[1] * mPotentialVector[1];
        mEffectiveConductivity = mViewScalar * mDefaultConductivity * (T0_4 - T1_4)
                               / (mPotentialVector[0] - mPotentialVector[1]);
    }
}
