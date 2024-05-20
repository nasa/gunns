#ifndef GunnsFluidAdsorber_EXISTS
#define GunnsFluidAdsorber_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_SOURCE_ADSORBER  Adsorber Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for the GUNNS Fluid Adsorber link model.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidAdsorber.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Change to conductor with sorbtion fluid) (2012-08))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Adsorber Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Adsorber link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidAdsorberConfigData : public GunnsFluidConductorConfigData
{
    public:
        FluidProperties::FluidType  mGasType;           /**< (--)    trick_chkpnt_io(**) Fluid type of constituent gas being sorbed. */
        double                      mEfficiency;        /**< (--)    trick_chkpnt_io(**) Efficiency of gas adsorbtion from atmosphere. */
        double                      mMaxAdsorbtionRate; /**< (kg/s)  trick_chkpnt_io(**) Maximum mass rate at which gas can be adsorbed from atmosphere. */
        double                      mMaxAdsorbedMass;   /**< (kg)    trick_chkpnt_io(**) Maximum amount of adsorbed mass. */
        double                      mDesorbtionRate;    /**< (kg/s)  trick_chkpnt_io(**) Mass flow rate at which gas can be desorbed to atmosphere. */
        double                      mThermalLength;     /**< (m)     trick_chkpnt_io(**) Tube length for thermal convection. */
        double                      mThermalDiameter;   /**< (m)     trick_chkpnt_io(**) Tube inner diameter for thermal convection. */
        double                      mSurfaceRoughness;  /**< (m)     trick_chkpnt_io(**) Tube wall surface roughness for thermal convection. */
        /// @brief    Default constructs this Adsorber configuration data.
        GunnsFluidAdsorberConfigData(const std::string&               name                 = "",
                                     GunnsNodeList*                   nodes                = 0,
                                     const double                     maxConductivity      = 0.0,
                                     const double                     expansionScaleFactor = 0.0,
                                     const FluidProperties::FluidType gasType              = FluidProperties::NO_FLUID,
                                     const double                     efficiency           = 0.0,
                                     const double                     maxAdsorbtionRate    = 0.0,
                                     const double                     maxAdsorbedMass      = 0.0,
                                     const double                     desorbtionRate       = 0.0,
                                     const double                     thermalLength        = 0.0,
                                     const double                     thermalDiameter      = 0.0,
                                     const double                     surfaceRoughness     = 0.0);
        /// @brief    Default destructs this Adsorber configuration data.
        virtual ~GunnsFluidAdsorberConfigData();
        /// @brief    Copy constructs this Adsorber configuration data.
        GunnsFluidAdsorberConfigData(const GunnsFluidAdsorberConfigData& that);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidAdsorberConfigData& operator =(const GunnsFluidAdsorberConfigData&);
};

// Forward-declare types
class GunnsFluidTraceCompoundsInputData;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Adsorber Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Adsorber link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidAdsorberInputData : public GunnsFluidConductorInputData
{
    public:
        bool                               mDesorbtionCycle;     /**< (--) trick_chkpnt_io(**) Desorbtion cycle flag (simbus input from signal aspect). */
        double                             mAdsorbedMass;        /**< (kg) trick_chkpnt_io(**) Adsorbed mass in the adsorber. */
        double                             mWallTemperature;     /**< (K)  trick_chkpnt_io(**) Tube wall temperature for thermal convection. */
        GunnsFluidTraceCompoundsInputData* mTcEfficiency;        /**< (--) trick_chkpnt_io(**) Adsorption/desorption efficiency for trace compounds. */
        GunnsFluidTraceCompoundsInputData* mTcMaxAdsorbedMass;   /**< (kg) trick_chkpnt_io(**) Maximum amount of adsorbed mass of trace compounds. */
        GunnsFluidTraceCompoundsInputData* mTcAdsorbedMass;      /**< (kg) trick_chkpnt_io(**) Initial amount of adsorbed mass of trace compounds. */
        bool                               mMalfEfficiencyFlag;  /**< (--) trick_chkpnt_io(**) Initial efficiency malfunction flag. */
        double                             mMalfEfficiencyValue; /**< (--) trick_chkpnt_io(**) Initial efficiency malfunction value. */
        /// @brief    Default constructs this Adsorber input data with arguments.
        GunnsFluidAdsorberInputData(const bool                         malfBlockageFlag    = false,
                                    const double                       malfBlockageValue   = 0.0,
                                    const bool                         desorbtionCycle     = false,
                                    const double                       adsorbedMass        = 0,
                                    const double                       wallTemperature     = 0.0,
                                    GunnsFluidTraceCompoundsInputData* tcEfficiency        = 0,
                                    GunnsFluidTraceCompoundsInputData* tcMaxAdsorbedMass   = 0,
                                    GunnsFluidTraceCompoundsInputData* tcAdsorbedMass      = 0,
                                    const bool                         malfEfficiencyFlag  = false,
                                    const double                       malfEfficiencyValue = 0.0);
        /// @brief    Copy constructs this Adsorber input data.
        GunnsFluidAdsorberInputData(const GunnsFluidAdsorberInputData& that);
        /// @brief    Default destructs this Adsorber input data.
        ~GunnsFluidAdsorberInputData();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidAdsorberInputData& operator=(const GunnsFluidAdsorberInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Adsorber
///
/// @details  Class for modeling a GUNNS Fluid Adsorber link model. An adsorber is a
///           conductive link masquerading as a source link that modifies the input fluid
///           by either adsorbing or desorbing a single constituent.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidAdsorber : public GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidAdsorber);
        public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool                       mMalfEfficiencyFlag;  /**< (--) Malfunction flag for overriding the efficiency of gas adsorbtion from atmosphere. */
        double                     mMalfEfficiencyValue; /**< (--) Malfunction value for overriding the efficiency of gas adsorbtion from atmosphere. */
        /// @}
        /// @brief    Default constructs this Adsorber.
        GunnsFluidAdsorber();
        /// @brief    Default destructs this Adsorber.
        virtual ~GunnsFluidAdsorber();
        /// @brief    Initializes this Adsorber with configuration and input data.
        void initialize(const GunnsFluidAdsorberConfigData& configData,
                        const GunnsFluidAdsorberInputData&  inputData,
                        std::vector<GunnsBasicLink*>&       networkLinks,
                        const int                           port0,
                        const int                           port1);
        /// @brief    Updates the internal fluid of this GunnsFluidAdsorber.
        virtual void updateFluid(const double dt, const double flowRate);
        /// @brief    Gets the adsorbed mass.
        double getAdsorbedMass() const;
        /// @brief    Sets the thermal surface area of this GunnsFluidAdsorber.
        void   setThermalSurfaceArea(const double value);
        /// @brief    Sets the wall temperature of this GunnsFluidAdsorber.
        void   setWallTemperature(const double value);
        /// @brief    Sets and resets the efficiency override malfunction.
        void   setMalfEfficiency(const bool flag = false, const double value = 0.0);

    protected:
        FluidProperties::FluidType mGasType;            /**< *o (--)   trick_chkpnt_io(**) Fluid type of constituent gas being sorbed. */
        double                     mEfficiency;         /**<    (--)   trick_chkpnt_io(**) Efficiency of gas adsorbtion from atmosphere. */
        double                     mMaxAdsorbtionRate;  /**<    (kg/s) trick_chkpnt_io(**) Maximum mass rate at which gas can be adsorbed from atmosphere. */
        double                     mMaxAdsorbedMass;    /**<    (kg)   trick_chkpnt_io(**) Maximum amount of adsorbed mass. */
        double                     mDesorbtionRate;     /**<    (kg/s) trick_chkpnt_io(**) Mass rate at which gas can be desorbed to atmosphere. */
        double                     mThermalDiameter;    /**<    (m)    trick_chkpnt_io(**) Tube inner diameter for thermal convection. */
        double                     mThermalSurfaceArea; /**<    (m2)   trick_chkpnt_io(**) Tube inner surface area for thermal convection. */
        double                     mThermalROverD;      /**<    (--)   trick_chkpnt_io(**) Tube surface roughness over diameter for thermal convection. */
        bool                       mDesorbtionCycle;    /**<    (--)                       Desorbtion cycle flag (simbus input from signal aspect). */
        double                     mAdsorbedMass;       /**<    (kg)                       Adsorbed mass in the adsorber. */
        double                     mWallTemperature;    /**<    (K)                        Tube wall temperature for thermal convection  (simbus input from thermal aspect). */
        double                     mFluidTemperature;   /**<    (K)    trick_chkpnt_io(**) Temperature of the fluid in the reactor. */
        double                     mWallHeatFlux;       /**<    (W)                        Convection heat flow from the fluid to the tube wall (simbus output to thermal aspect). */
        double                     mMass;               /**<    (kg)   trick_chkpnt_io(**) Adsorbed mass this cycle. */
        double                     mSorbtionFlowRate;   /**<    (kg/s) trick_chkpnt_io(**) Sorbtion mass flow rate. */
        PolyFluid*                 mSorbtionFluid;      /**<    (--)   trick_chkpnt_io(**) Pointer to another internal fluid for the result of sorbtion. */
        int                        mGasIndex;           /**< *o (--)   trick_chkpnt_io(**) Index of constituent gas being sorbed. */
        double*                    mTcEfficiency;       /**<    (--)   trick_chkpnt_io(**) Adsorption/desorption efficiency for trace compounds. */
        double*                    mTcMaxAdsorbedMass;  /**<    (kg)   trick_chkpnt_io(**) Maximum amount of adsorbed mass of trace compounds. */
        double*                    mTcAdsorbedMass;     /**<    (kg)   trick_chkpnt_io(**) Initial amount of adsorbed mass of trace compounds. */
        /// @brief    Validates the initialization inputs of this Adsorber.
        void validate(const GunnsFluidAdsorberConfigData& configData,
                      const GunnsFluidAdsorberInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Update for adsorbtion.
        virtual void adsorb(const double dt, const double rate, const double availability_efficiency);
        /// @brief    Update for desorbtion.
        virtual void desorb(const double dt, const double rate, const double efficiency);
        /// @brief    Updates the fluid temperature.
        virtual void updateTemperature(const double dt);
        /// @brief    Computes the reaction efficiency.
        virtual double computeEfficiency();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidAdsorber(const GunnsFluidAdsorber&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidAdsorber& operator=(const GunnsFluidAdsorber&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (kg)  Adsorbed mass.
///
/// @details  Returns the accumulated atmosphere constituent mass adsorbed by this GunnsFluidAdsorber.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidAdsorber::getAdsorbedMass() const
{
    return mAdsorbedMass;
}


#endif
