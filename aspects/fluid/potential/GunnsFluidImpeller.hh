#ifndef GunnsFluidImpeller_EXISTS
#define GunnsFluidImpeller_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_POTENTIAL_IMPELLER    Impeller Model
@ingroup   TSM_GUNNS_FLUID_POTENTIAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Impeller link model used to model the fluid aspect of a fan or
   pump.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidImpeller.o))

PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Update) (2011-09))

@{
*/

#include "core/GunnsFluidPotential.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Impeller Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Impeller link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidImpellerConfigData : public GunnsFluidPotentialConfigData
{
    public:
        double        mReferenceDensity;  /**< (kg/m3)          trick_chkpnt_io(**) Reference fluid density for this Impeller power curve. */
        double        mReferenceSpeed;    /**< (revolution/min) trick_chkpnt_io(**) Reference speed for this Impeller power curve. */
        double        mThermalLength;     /**< (m)              trick_chkpnt_io(**) Tube length for thermal convection. */
        double        mThermalDiameter;   /**< (m)              trick_chkpnt_io(**) Tube inner diameter for thermal convection. */
        double        mSurfaceRoughness;  /**< (m)              trick_chkpnt_io(**) Tube wall surface roughness for thermal convection. */
        /// @brief    Default constructs this Impeller configuration data.
        GunnsFluidImpellerConfigData(const std::string& name                 = "",
                                     GunnsNodeList*     nodes                = 0,
                                     const double       maxConductivity      = 0.0,
                                     const double       expansionScaleFactor = 0.0,
                                     const double       referenceDensity     = 0.0,
                                     const double       referenceSpeed       = 0.0,
                                     const double       thermalLength        = 0.0,
                                     const double       thermalDiameter      = 0.0,
                                     const double       surfaceRoughness     = 0.0);
        /// @brief    Copy constructs this Impeller configuration data.
        GunnsFluidImpellerConfigData(const GunnsFluidImpellerConfigData& that);
        /// @brief    Default destructs this Impeller configuration data.
        virtual ~GunnsFluidImpellerConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidImpellerConfigData& operator =(const GunnsFluidImpellerConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Impeller Model Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Impeller link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidImpellerInputData : public  GunnsFluidPotentialInputData
{
    public:
        double        mMotorSpeed;        /**< (revolution/min) Initial speed of the motor. */
        double        mWallTemperature;   /**< (K)              Initial impeller wall temperature. */
        /// @brief    Default constructs this Impeller input data.
        GunnsFluidImpellerInputData(const bool   malfBlockageFlag    = false,
                                    const double malfBlockageValue   = 0.0,
                                    const double sourcePressure      = 0.0,
                                    const double motorSpeed          = 0.0,
                                    const double wallTemperature     = 0.0);
        /// @brief    Copy constructs this Impeller input data.
        GunnsFluidImpellerInputData(const GunnsFluidImpellerInputData& that);
        /// @brief    Default destructs this Impeller input data.
        virtual ~GunnsFluidImpellerInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidImpellerInputData& operator =(const GunnsFluidImpellerInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Impeller Model
///
/// @details  The GUNNS Fluid Impeller link model simulates a fan or pump.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidImpeller : public  GunnsFluidPotential
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidImpeller);
    public:
        /// @brief    Default constructs this Impeller, deprecated due to obsolescence by GunnsGasFan.
        GunnsFluidImpeller() __attribute__ ((deprecated));
        /// @brief    Default destructs this Impeller.
        virtual ~GunnsFluidImpeller();
        /// @brief    Initializes this Impeller with configuration and input data.
        void initialize(const GunnsFluidImpellerConfigData& configData,
                        const GunnsFluidImpellerInputData&  inputData,
                        std::vector<GunnsBasicLink*>&       links,
                        const int                           port0,
                        const int                           port1);
        /// @brief    Sets the thermal surface area of this Impeller.
        void   setThermalSurfaceArea(const double value);
        /// @brief    Sets the wall temperature of this Impeller.
        void   setWallTemperature(const double value);
    protected:
        double        mPowerCurveCoefficient;  /**< (kPa*min2/revolution2*m3/kg) trick_chkpnt_io(**) Impeller power curve coefficient. */
        double        mThermalDiameter;        /**< (m)                          trick_chkpnt_io(**) Impeller inner diameter for thermal convection. */
        double        mThermalSurfaceArea;     /**< (m2)                         trick_chkpnt_io(**) Impeller surface area for thermal convection. */
        double        mThermalROverD;          /**< (--)                         trick_chkpnt_io(**) Impeller surface roughness over diameter for convection. */
        double        mMotorSpeed;             /**< (revolution/min)                                 Initial speed of the motor. */
        double        mWallTemperature;        /**< (K)                                              Impeller wall temperature input from simbus. */
        double        mWallHeatFlux;           /**< (W)                                              Heat flux from fluid to wall output to simbus. */
        /// @brief    Validates the initialization of this Impeller.
        void validate(const GunnsFluidImpellerConfigData& configData,
                      const GunnsFluidImpellerInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Updates the state of this Impeller.
        virtual void updateState(const double dt);
        /// @brief    Updates the internal fluid of this Impeller.
        virtual void updateFluid(const double dt, const double flowRate);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidImpeller(const GunnsFluidImpeller&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidImpeller& operator =(const GunnsFluidImpeller&);
};

/// @}

#endif
