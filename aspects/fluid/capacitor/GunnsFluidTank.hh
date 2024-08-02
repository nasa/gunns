#ifndef GunnsFluidTank_EXISTS
#define GunnsFluidTank_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CAPACITOR_TANK  Tank Model
@ingroup   TSM_GUNNS_FLUID_CAPACITOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Tank link model, an editable fluid capacitor model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidTank.o))

PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Update) (2011-09))

@{
*/

#include "core/GunnsFluidCapacitor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Tank Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Tank link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidTankConfigData : public GunnsFluidCapacitorConfigData
{
    public:
        double mDpdtFilterGain;     /**< (--)       trick_chkpnt_io(**) Gain for the dP/dt filter (0-1) */
        double mThermalDampingMass; /**< (kg)       trick_chkpnt_io(**) Mass of solid in thermal equilibrium with fluid */
        double mEditFluxTarget;     /**< (kg*mol/s) trick_chkpnt_io(**) Target net node flux for edit auto shut-off */
        /// @details  When both mSurfaceArea and mShellRadius are > 0, the tank calculates the heat
        ///           flux from the shell to the fluid using the shell temperature input from the
        ///           simbus.  Otherwise, the tank receives the heat flux from the thermal aspect.
        float  mSurfaceArea;        /**< (m2)       trick_chkpnt_io(**) Inner surface area of tank shell */
        float  mShellRadius;        /**< (m)        trick_chkpnt_io(**) Distance from tank shell to center */

        /// @brief    Default constructs this Tank configuration data.
        GunnsFluidTankConfigData(const std::string& name                 = "",
                                 GunnsNodeList*     nodes                = 0,
                                 const double       expansionScaleFactor = 0.0,
                                 const double       dpdtFilterGain       = 0.0,
                                 const double       thermalDampingMass   = 0.0,
                                 const double       editFluxTarget       = 1.0E-6,
                                 const float        surfaceArea          = 0.0,
                                 const float        shellRadius          = 0.0);

        /// @brief    Copy constructs this Tank configuration data.
        GunnsFluidTankConfigData(const GunnsFluidTankConfigData& that);
        /// @brief    Default destructs this Tank configuration data.
        virtual ~GunnsFluidTankConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidTankConfigData& operator =(const GunnsFluidTankConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Tank Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Tank link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidTankInputData : public GunnsFluidCapacitorInputData
{
    public:
        float mShellTemperature;                      /**< (K)   Initial Shell temperature */
        float mBiasHeatFlux;                          /**< (J/s) User bias heat flux added to fluid */

        /// @brief    Default constructs this Tank input data.
        GunnsFluidTankInputData(const bool          malfBlockageFlag        = false,
                                const double        malfBlockageValue       = 0.0,
                                const double        initialVolume           = 0.0,
                                PolyFluidInputData* initialFluidState       = 0,
                                const float         initialShellTemperature = 0.0,
                                const float         biasHeatFlux            = 0.0);
        /// @brief    Copy constructs this Tank input data.
        GunnsFluidTankInputData(const GunnsFluidTankInputData& that);
        /// @brief    Default destructs this Tank input data.
        virtual ~GunnsFluidTankInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidTankInputData& operator =(const GunnsFluidTankInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Tank Model
///
/// @details  The GUNNS FLuid Tank link model simulates a volume with user capability to edit
///           either the temperature and pressure or partial pressure, or the partial pressure rates
///           to target partial pressures of the contents. This combines the capabilities of the
///           GUNNS Fluid Capacitor and Source Links.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidTank : public  GunnsFluidCapacitor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidTank);
    public:
        /// @brief  Default constructs this Tank.
        GunnsFluidTank();
        /// @brief  Default destructs this Tank.
        virtual ~GunnsFluidTank();
        /// @brief  Initializes this Tank with configuration and input data.
        void initialize(const GunnsFluidTankConfigData&   configData,
                        const GunnsFluidTankInputData&    inputData,
                        std::vector<GunnsBasicLink*>&     networkLinks,
                        const int                         port0,
                        const int                         port1);
        /// @brief  Updates the state of this Tank.
        virtual void   updateState(const double dt);
        /// @brief Computes the flow across this Tank.
        virtual void   computeFlows(const double dt);
        /// @brief  Updates the internal fluid of this Tank.
        virtual void   updateFluid(const double dt, const double);
        /// @brief  Returns the heat flux from the tank shell.
        virtual double getHeatFlux() const;
        /// @brief  Returns the bias heat flux from the tank shell.
        virtual double getBiasHeatFlux() const;
        /// @brief  Returns the delta pressure/delta time.
        double         getDpdt() const;
        /// @brief  Returns the constituent partial pressures.
        double*        getPartialPressure() const;
        /// @brief  Sets the temperature-only edit controls.
        void           editTemperature(const bool   flag        = false,
                                       const double temperature = 0.0);
        /// @brief  Sets the temperature and total pressure edit controls.
        void           editTemperaturePressure(const bool   flag        = false,
                                               const double temperature = 0.0,
                                               const double pressure    = 0.0);
        /// @brief  Sets the temperature and partial pressure edit controls.
        void           editTemperaturePartialPressure(const bool    flag             = false,
                                                      const double  temperature      = 0.0,
                                                      const double* partialPressures = 0);
        /// @brief  Sets an individual constituent's partial pressure rate edit controls.
        void           editPartialPressureRate(const FluidProperties::FluidType type = FluidProperties::NO_FLUID,
                                               const bool   flag                     = false,
                                               const double partialPressure          = 0.0,
                                               const double rate                     = 0.0);
        /// @brief  Special processing of data outputs from the model after the network update
        virtual void   processOutputs();
        /// @brief  Sets bias heat flux term
        void           setBiasHeatFlux(float biasHeatFlux);

    protected:
        int     mNConstituents;                      /**< *o (--)       trick_chkpnt_io(**) Number of constituent fluids */
        bool    mEditTemperatureFlag;                /**<    (--)       trick_chkpnt_io(**) Edit content T flag */
        bool    mEditTemperaturePressureFlag;        /**<    (--)       trick_chkpnt_io(**) Edit content T and P flag */
        bool    mEditTemperaturePartialPressureFlag; /**<    (--)       trick_chkpnt_io(**) Edit content T and PP flag */
        bool*   mEditPartialPressureRateFlag;        /**<    (--)       trick_chkpnt_io(**) Edit content PP rate flags */
        double  mEditTemperatureValue;               /**<    (K)        trick_chkpnt_io(**) Temperature edit target */
        double  mEditPressureValue;                  /**<    (kPa)      trick_chkpnt_io(**) Pressure edit target */
        double* mEditPartialPressureValue;           /**<    (kPa)      trick_chkpnt_io(**) Partial pressure edit targets */
        double* mEditPartialPressureRateValue;       /**<    (kPa/s)    trick_chkpnt_io(**) Partial pressure rate edit values */
        double  mEditFluxTarget;                     /**<    (kg*mol/s) trick_chkpnt_io(**) Target net node flux for edit auto shut-off */
        float   mBiasHeatFlux;                       /**<    (J/s)                          User bias heat flux added to fluid */
        double  mHeatFluxFromShell;                  /**<    (J/s)                          Heat flux from the tank shell into fluid */
        double  mHeatFluxToShell;                    /**<    (J/s)                          Heat flux from fluid to the tank shell */
        double  mTemperature;                        /**<    (K)                            Tank content temperature output to simbus */
        double  mShellTemperature;                   /**<    (K)                            Tank Shell temperature */
        float   mSurfaceArea;                        /**<    (m2)       trick_chkpnt_io(**) Inner surface area of tank shell */
        float   mShellRadius;                        /**<    (m)        trick_chkpnt_io(**) Distance from tank shell to center */
        double  mPreviousPressure;                   /**<    (kPa)                          Previous pressure in the node */
        double  mDpdt;                               /**<    (kPa/s)                        Delta-Pressure / delta-time */
        double  mDpdtFilterGain;                     /**<    (--)       trick_chkpnt_io(**) Gain for the dP/dt filter (0-1) */
        double* mPartialPressure;                    /**<    (kPa)      trick_chkpnt_io(**) Constituent partial pressures */
        double* mMassFraction;                       /**<    (--)       trick_chkpnt_io(**) Constituent mass fractions (0-1) */
        double* mMoleFraction;                       /**<    (--)       trick_chkpnt_io(**) Constituent mole fractions (0-1) */
        /// @brief  Validates config & input data.
        void           validate(const GunnsFluidTankConfigData& configData,
                                const GunnsFluidTankInputData&  inputData);
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void   restartModel();
        /// @brief  Builds the capacitive source vector for the link.
        virtual void   buildSourceVector(const double dt);
        /// @brief  Computes the true delta-pressure / delta-time in the node.
        void           computeDpdt(const double dt);
        /// @brief  Checks the edit temperature value and corrects if necessary.
        void           checkEditTemperatureValue();
        /// @brief  Computes the target total pressure for an edit.
        double         computeEditTargetPressure();
        /// @brief  Calls the appropriate fluid edit method for the edit type.
        void           performFluidEdit();
        /// @brief  Returns the undamped heat flux.
        virtual double getUndampedHeatFlux() const;

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidTank(const GunnsFluidTank&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidTank& operator =(const GunnsFluidTank&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (W) Heat flux.
///
/// @details  This method returns the total heat flux into the fluid from the tank shell and user
///           bias.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidTank::getHeatFlux() const
{
    return mHeatFluxFromShell + static_cast<double>(mBiasHeatFlux);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (W) Bias heat flux.
///
/// @details  This method returns the bias heat flux.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidTank::getBiasHeatFlux() const
{
    return mBiasHeatFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (W) The heat flux.
///
/// @details  This method returns the total heat flux into the fluid from the tank shell, which we
///           want to omit from the fluid node's thermal damping logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidTank::getUndampedHeatFlux() const
{
    return mHeatFluxFromShell;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kPa/s) Delta pressure/Delta time.
///
/// @details  This method returns the change in pressure over time.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidTank::getDpdt() const
{
    return mDpdt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kPa) Partial pressure.
///
/// @details  This method returns the partial pressures of the constituents.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double* GunnsFluidTank::getPartialPressure() const
{
    return mPartialPressure;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  biasHeatFlux   (J/s)   Bias Heat Flux.
///
/// @details  This method sets the bias heat flux.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidTank::setBiasHeatFlux(float biasHeatFlux)
{
    mBiasHeatFlux = biasHeatFlux;
}

#endif
