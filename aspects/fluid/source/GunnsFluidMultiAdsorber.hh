#ifndef GunnsFluidMultiAdsorber_EXISTS
#define GunnsFluidMultiAdsorber_EXISTS

/**
@file
@brief    GUNNS Multiple Constituent Adsorber Model declarations

@defgroup  TSM_GUNNS_FLUID_SOURCE_MULTI_ADSORBER  Multiple Constituent Adsorber Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for the GUNNS Fluid Multiple Constituent Adsorber link model.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidMultiAdsorber.o))

 PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (2018-01))

@{
*/

#include "GunnsFluidAdsorptionCompound.hh"
#include "core/GunnsFluidConductor.hh"
#include "properties/ChemicalCompound.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Multiple Constituent Adsorber Configuration Data
///
/// @details  This provides a data structure for the GUNNS Fluid Multiple Constituent Adsorber link
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidMultiAdsorberConfigData : public GunnsFluidConductorConfigData
{
    public:
        double mThermalLength;    /**< (m) trick_chkpnt_io(**) Tube length for thermal convection. */
        double mThermalDiameter;  /**< (m) trick_chkpnt_io(**) Tube inner diameter for thermal convection. */
        double mSurfaceRoughness; /**< (m) trick_chkpnt_io(**) Tube wall surface roughness for thermal convection. */
        std::vector<GunnsFluidAdsorptionCompound> mCompounds; /**< (--) trick_chkpnt_io(**) Absorption compunds config & input data. */
        /// @brief  Default constructs this Multiple Constituent Adsorber configuration data.
        GunnsFluidMultiAdsorberConfigData(const std::string& name                 = "",
                                          GunnsNodeList*     nodes                = 0,
                                          const double       maxConductivity      = 0.0,
                                          const double       expansionScaleFactor = 0.0,
                                          const double       thermalLength        = 0.0,
                                          const double       thermalDiameter      = 0.0,
                                          const double       surfaceRoughness     = 0.0);
        /// @brief  Default destructs this Multiple Constituent Adsorber configuration data.
        virtual ~GunnsFluidMultiAdsorberConfigData();
        /// @brief  Adds a compound and defines its sorption properties.
        void addCompound(const ChemicalCompound::Type type,
                         const double                 maxAdsorbedMass,
                         const double                 efficiencyCoeff0,
                         const double                 efficiencyCoeff1      = 0.0,
                         const double                 desorbPartialPressure = 0.0,
                         const double                 desorbRateFactor      = 0.0,
                         const double                 heatOfAdsorption      = 0.0,
                         const bool                   taperOffFlag          = false,
                         const ChemicalCompound::Type dependentType         = ChemicalCompound::NO_COMPOUND,
                         const bool                   malfEfficiencyFlag    = false,
                         const double                 malfEfficiencyValue   = 0.0,
                         const double                 adsorbedMass          = 0.0,
                         const double                 breakthroughExp       = 1.0);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidMultiAdsorberConfigData(const GunnsFluidMultiAdsorberConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidMultiAdsorberConfigData& operator =(const GunnsFluidMultiAdsorberConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Multiple Constituent Adsorber Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Multiple Constituent Adsorber link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidMultiAdsorberInputData : public GunnsFluidConductorInputData
{
    public:
        double mWallTemperature; /**< (K) trick_chkpnt_io(**) Initial tube wall temperature for thermal convection. */
        /// @brief  Default constructs this Multiple Constituent Adsorber input data with arguments.
        GunnsFluidMultiAdsorberInputData(const bool   malfBlockageFlag  = false,
                                         const double malfBlockageValue = 0.0,
                                         const double wallTemperature   = 0.0);
        /// @brief  Default destructs this Multiple Constituent Adsorber input data.
        ~GunnsFluidMultiAdsorberInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidMultiAdsorberInputData(const GunnsFluidMultiAdsorberInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidMultiAdsorberInputData& operator=(const GunnsFluidMultiAdsorberInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Multiple Constituent Adsorber
///
/// @details  Class for the GUNNS Fluid Multiple Constituent Adsorber link.
///           This is similar to the GunnsFluidHotAdsorber and GunnsFluidAdsorber, without some of
///           their unneeded features, and adding some additional features.
///           Added features (all optional):
///           - Absorb & desorb multiple constituents.
///           - Adsorption/desorption rate tapers off when approaching mass limits.
///           - Interaction of absorbed constituents on each other's efficiencies.
///           - Switch automatically between adsorption/desorption based on pressure.
///           - Model heat of the sorbing reaction.
///           - Consistent configuration between network fluid types and trace compounds.
///           Removed features:
///           - Max adsorption rate.
///           - Desorption cycle input flag, since we switch directions internally.
///
///           Many of these features are implemented in the GunnsFluidAdsorptionCompound class.
///           Refer to that class for more info.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidMultiAdsorber : public GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidMultiAdsorber);
    public:
        GunnsFluidAdsorptionCompound* mCompounds; /**< (--) The compounds considered by this adsorber. */
        /// @brief  Default constructs this Multiple Constituent Adsorber.
        GunnsFluidMultiAdsorber();
        /// @brief  Default destructs this Multiple Constituent Adsorber.
        virtual ~GunnsFluidMultiAdsorber();
        /// @brief  Initializes this Multiple Constituent Adsorber with configuration and input data.
        void initialize(const GunnsFluidMultiAdsorberConfigData& configData,
                        const GunnsFluidMultiAdsorberInputData&  inputData,
                        std::vector<GunnsBasicLink*>&       networkLinks,
                        const int                           port0,
                        const int                           port1);
        /// @brief  Updates the internal fluid of this GunnsFluidMultiAdsorber.
        virtual void updateFluid(const double dt, const double flowRate);
        /// @brief  Sets the thermal surface area of this GunnsFluidMultiAdsorber.
        void   setThermalSurfaceArea(const double value);
        /// @brief  Sets the wall temperature of this GunnsFluidMultiAdsorber.
        void   setWallTemperature(const double value);

    protected:
        unsigned int mNCompounds;         /**< *o (--) trick_chkpnt_io(**) Number of compounds considered by this adsorber. */
        double       mThermalDiameter;    /**<    (m)  trick_chkpnt_io(**) Tube inner diameter for thermal convection. */
        double       mThermalSurfaceArea; /**<    (m2) trick_chkpnt_io(**) Tube inner surface area for thermal convection. */
        double       mThermalROverD;      /**<    (--) trick_chkpnt_io(**) Tube surface roughness over diameter for thermal convection. */
        double       mWallTemperature;    /**<    (K)                      Tube wall temperature for thermal convection  (simbus input from thermal aspect). */
        double       mWallHeatFlux;       /**<    (W)                      Total convection and sorbing reaction heats to the tube wall (simbus output to thermal aspect). */
        double       mSorptionHeat;       /**<    (W)  trick_chkpnt_io(**) Total heat output (positive is exothermic) by all sorbing reactions included in mWallHeatFlux. */
        PolyFluid*   mSorptionFluid;      /**<    (--) trick_chkpnt_io(**) Pointer to an internal fluid for the result of sorptions. */
        /// @brief  Validates the initialization inputs of this Multiple Constituent Adsorber.
        void validate(const GunnsFluidMultiAdsorberConfigData& configData,
                      const GunnsFluidMultiAdsorberInputData&  inputData) const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief  Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidMultiAdsorber(const GunnsFluidMultiAdsorber&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidMultiAdsorber& operator=(const GunnsFluidMultiAdsorber&);
};

/// @}

#endif
