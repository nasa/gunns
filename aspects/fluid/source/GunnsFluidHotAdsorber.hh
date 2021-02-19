#ifndef GunnsFluidHotAdsorber_EXISTS
#define GunnsFluidHotAdsorber_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_SOURCE_HOT_ADSORBER  Hot Adsorber Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling an adsorber with heat.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidHotAdsorber.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-09))

@{
*/

#include "aspects/fluid/source/GunnsFluidAdsorber.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hot Adsorber Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the
///           GUNNS Hot Fluid Adsorber link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHotAdsorberConfigData : public GunnsFluidAdsorberConfigData
{
    public:
        double        mEfficiencyBias;         /**< (1/K)  trick_chkpnt_io(**) Bias for the dependency of efficiency on temperature. */
        double        mEfficiencyScaleFactor;  /**< (--)   trick_chkpnt_io(**) Scale factor for the dependency of efficiency on temperature. */
        /// @brief    Default constructs this Hot Adsorber configuration data.
        GunnsFluidHotAdsorberConfigData(const std::string&               name                  = "",
                                        GunnsNodeList*                   nodes                 = 0,
                                        const double                     maxConductivity       = 0.0,
                                        const double                     expansionScaleFactor  = 0.0,
                                        const FluidProperties::FluidType gasType               = FluidProperties::NO_FLUID,
                                        const double                     efficiency            = 0.0,
                                        const double                     maxAdsorbtionRate     = 0.0,
                                        const double                     maxAdsorbedMass       = 0.0,
                                        const double                     desorbtionRate        = 0.0,
                                        const double                     thermalLength         = 0.0,
                                        const double                     thermalDiameter       = 0.0,
                                        const double                     surfaceRoughness      = 0.0,
                                        const double                     efficiencyBias        = 0.0,
                                        const double                     efficiencyScaleFactor = 0.0);
        /// @brief    Default destructs this Hot Adsorber configuration data.
        virtual ~GunnsFluidHotAdsorberConfigData();
        /// @brief    Copy constructs this Hot Adsorber configuration data.
        GunnsFluidHotAdsorberConfigData(const GunnsFluidHotAdsorberConfigData& that);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHotAdsorberConfigData& operator =(const GunnsFluidHotAdsorberConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hot Adsorber Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the
///           GUNNS Hot Fluid Adsorber link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHotAdsorberInputData : public GunnsFluidAdsorberInputData
{
    public:
        /// @brief    Default constructs this Hot Adsorber input data with arguments.
        GunnsFluidHotAdsorberInputData(const bool                         malfBlockageFlag    = false,
                                       const double                       malfBlockageValue   = 0.0,
                                       const bool                         desorbtionCycle     = false,
                                       const double                       adsorbedMass        = 0,
                                       const double                       wallTemperature     = 0.0,
                                       GunnsFluidTraceCompoundsInputData* tcEfficiency        = 0,
                                       GunnsFluidTraceCompoundsInputData* tcMaxAdsorbedMass   = 0,
                                       GunnsFluidTraceCompoundsInputData* tcAdsorbedMass      = 0,
                                       const bool                         malfEfficiencyFlag  = false,
                                       const double                       malfEfficiencyValue = 0.0);
        /// @brief    Copy constructs this Hot Adsorber input data.
        GunnsFluidHotAdsorberInputData(const GunnsFluidHotAdsorberInputData& that);
        /// @brief    Default destructs this Hot Adsorber input data.
        ~GunnsFluidHotAdsorberInputData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHotAdsorberInputData& operator=(const GunnsFluidHotAdsorberInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hot Adsorber
///
/// @details  Class for modeling a GUNNS Hot Fluid Adsorber link model. An adsorber is a
///           conductive link masquerading as a source link that modifies the input fluid
///           constituents by consuming (adsorbtion) or producing (desorbtion) a compound in the
///           atmosphere. This adsorber may be heated and the sorbtion efficiency is temperature
///           dependent.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHotAdsorber : public GunnsFluidAdsorber
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidHotAdsorber);
    public:
        /// @brief    Default constructs this Hot Adsorber.
        GunnsFluidHotAdsorber();
        /// @brief    Default destructs this Hot Adsorber.
        virtual ~GunnsFluidHotAdsorber();
        /// @brief    Initializes this Hot Adsorber with configuration and input data.
        void initialize(const GunnsFluidHotAdsorberConfigData& configData,
                        const GunnsFluidHotAdsorberInputData&  inputData,
                        std::vector<GunnsBasicLink*>&          networkLinks,
                        const int                              port0,
                        const int                              port1);
    protected:
        double        mEfficiencyBias;         /**< (1/K)  trick_chkpnt_io(**) Bias for the dependency of efficiency on temperature. */
        double        mEfficiencyScaleFactor;  /**< (--)   trick_chkpnt_io(**) Scale factor for the dependency of efficiency on temperature. */
        /// @brief    Validates the initialization inputs of this Hot Adsorber.
        void validate(const GunnsFluidHotAdsorberConfigData& configData,
                      const GunnsFluidHotAdsorberInputData&  inputData);
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Computes the sorbtion efficiency.
        virtual double computeEfficiency();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHotAdsorber(const GunnsFluidHotAdsorber&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHotAdsorber& operator=(const GunnsFluidHotAdsorber&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Configuration data (unused).
/// @param[in]  inputData   (--)  Input data (unused).
///
/// @details  Validates the initialization of this GUNNS Fluid Hot Adsorber link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidHotAdsorber::validate(const GunnsFluidHotAdsorberConfigData& configData __attribute__((unused)),
                                            const GunnsFluidHotAdsorberInputData&  inputData  __attribute__((unused)))
{
    // nothing to do
}

#endif
