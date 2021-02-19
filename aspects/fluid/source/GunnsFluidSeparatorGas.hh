#ifndef GunnsFluidSeparatorGas_EXISTS
#define GunnsFluidSeparatorGas_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_SOURCE_SEPARATOR_GAS  Separator Gas Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Separator Gas link model.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidSeparatorGas.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (2011-11) (Initial)
   (Jason Harvey)      (L-3 Communications) (2012-12) (Base on conductor with extra source))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Separator Gas Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Separator Gas link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSeparatorGasConfigData : public GunnsFluidConductorConfigData
{
    public:
        FluidProperties::FluidType mGasType;              /**< (--)             trick_chkpnt_io(**) Gas phase fluid type */
        double                     mMassExponent;         /**< (--)             trick_chkpnt_io(**) Mass factor exponent */
        double                     mMaxLiquidMass;        /**< (kg)             trick_chkpnt_io(**) Liquid mass capacity */
        double                     mReferenceSpeed;       /**< (revolution/min) trick_chkpnt_io(**) Reference speed */
        double                     mReferencePressure;    /**< (kPa)            trick_chkpnt_io(**) Reference pressure */
        double                     mReferenceRemovalRate; /**< (kg/s)           trick_chkpnt_io(**) Reference liquid removal rate */
        /// @brief    Default constructs this Separator Gas model configuration data with arguments.
        GunnsFluidSeparatorGasConfigData(
                const std::string&               name                 = "",
                GunnsNodeList*                   nodes                = 0,
                const double                     maxConductivity      = 0.0,
                const FluidProperties::FluidType gasType              = FluidProperties::NO_FLUID,
                const double                     massExponent         = 0.0,
                const double                     maxLiquidMass        = 0.0,
                const double                     referenceSpeed       = 0.0,
                const double                     referencePressure    = 0.0,
                const double                     referenceRemovalRate = 0.0);
        /// @brief    Default destructs this Separator Gas model configuration data.
        virtual ~GunnsFluidSeparatorGasConfigData();
        /// @brief    Copy constructs this Separator Gas model configuration data.
        GunnsFluidSeparatorGasConfigData(const GunnsFluidSeparatorGasConfigData& that);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSeparatorGasConfigData& operator =(const GunnsFluidSeparatorGasConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Separator Gas  Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Separator Gas link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSeparatorGasInputData : public GunnsFluidConductorInputData
{
    public:
        double mSeparatorSpeed;     /**< (revolution/min) Separator speed (from simbus) */
        double mTransferFlowRate;   /**< (kg/s)           Mass flow rate to liquid network (from simbus) */
        double mLiquidMass;         /**< (kg)             Mass of liquid in separator */
        /// @brief    Default constructs this Separator Gas model input data with arguments.
        GunnsFluidSeparatorGasInputData(const bool   malfBlockageFlag    = false,
                                        const double malfBlockageValue   = 0.0,
                                        const double separatorSpeed      = 0.0,
                                        const double transferFlowRate    = 0.0,
                                        const double liquidMass          = 0.0);
        /// @brief    Copy constructs this Separator Gas model input data.
        GunnsFluidSeparatorGasInputData(const GunnsFluidSeparatorGasInputData& that);
        /// @brief    Default destructs this Separator Gas model input data.
        ~GunnsFluidSeparatorGasInputData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSeparatorGasInputData& operator=(const GunnsFluidSeparatorGasInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Separator Gas Model
///
/// @details  The GUNNS Fluid Separator Gas link model provides the gas network side link for a gas-
///           liquid separator.  It works in conjunction with a GunnsFluidSeparatorLiquid link in
///           the same or an external network.  The separator is assumed to use a centrifugal drum
///           that pressurizes the separated liquid.  This link removes condensed operating fluid at
///           a rate proportional to the drum spin speed, and limited to the maximum available
///           condensed fluid passing through the link.  The actual fluid in the network remains
///           gas phase and is treated as condensed by this link if relative humidity is > 1.  This
///           mass of separated liquid is added to the mass contained in the drum.
///
///           The liquid pressure is proportional to drum speed and exponentially proportional to
///           liquid mass in the drum.  This link sends the liquid pressure & temperature to the
///           liquid-side link, and receives a liquid transfer demand mass flow rate.  The
///           transferred liquid mass is removed from the drum.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSeparatorGas : public GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSeparatorGas);
    public:
        /// @brief    Default constructs this Separator Gas model.
        GunnsFluidSeparatorGas();
        /// @brief    Default destructs this Separator Gas model.
        virtual ~GunnsFluidSeparatorGas();
        /// @brief    Initializes this Separator Gas model with configuration and input data.
        void initialize(const GunnsFluidSeparatorGasConfigData& configData,
                        const GunnsFluidSeparatorGasInputData&  inputData,
                        std::vector<GunnsBasicLink*>&           networkLinks,
                        const int                               port0,
                        const int                               port1);
        /// @brief    Updates the internal fluid of this Separator Gas model.
        virtual void updateFluid(const double dt, const double flowrate);
        /// @brief    Special processing of data outputs from the model after to the network update
        virtual void processOutputs();
        /// @brief    Return temperature of liquid
        double getTransferTemperature() const;
        /// @brief    Return outgoing liquid rate
        double getTransferFlowRate() const;
        /// @brief    Return outgoing liquid pressure
        double getTransferPressure() const;
        /// @brief Return liquid mass in separator
        double getLiquidMass() const;
        /// @brief Return true if liquid is present in exit gas stream
        bool getLiquidOverflow() const;
    protected:
        FluidProperties::FluidType mGasType; /**< *o (--)                 trick_chkpnt_io(**) Separated gas phase fluid type */
        double     mMassExponent;            /**<    (--)                 trick_chkpnt_io(**) Mass factor exponent in pump curve */
        double     mMaxLiquidMass;           /**<    (kg)                 trick_chkpnt_io(**) Liquid mass capacity */
        double     mPowerCurveCoefficient;   /**<    (kPa*min/revolution) trick_chkpnt_io(**) Power curve coefficient */
        double     mRemovalRateCoefficient;  /**<    (kg*min/revolution)  trick_chkpnt_io(**) Liquid separation coefficient */
        int        mGasIndex;                /**< *o (--)                 trick_chkpnt_io(**) Index of separated gas */
        double     mSeparatorSpeed;          /**<    (revolution/min)                         Separator speed (from simbus) */
        double     mTransferFlowRate;        /**<    (kg/s)                                   Outgoing liquid rate (from simbus) */
        double     mLiquidMass;              /**<    (kg)                                     Liquid mass in separator */
        double     mLiquidMassError;         /**<    (kg)                 trick_chkpnt_io(**) Accumulated mass created on liquid side */
        double     mLiquidDeltaP;            /**<    (kPa)                trick_chkpnt_io(**) Delta pressure on liquid */
        double     mRelativeHumidity;        /**<    (--)                 trick_chkpnt_io(**) Relative humidity of the gas mixture. */
        double     mSeparationRate;          /**<    (kg/s)               trick_chkpnt_io(**) Liquid separation rate */
        PolyFluid* mSeparatedLiquid;         /**<    (--)                 trick_chkpnt_io(**) Separated liquid fluid object. */
        double     mTransferTemperature;     /**<    (K)                                      Temperature of liquid (to simbus) */
        double     mTransferPressure;        /**<    (kPa)                                    Outgoing liquid pressure (to simbus)*/
        bool       mLiquidOverflow;          /**<    (--)                                     Liquid present in exit gas stream (to simbus) */
        /// @brief    Validates the initialization inputs of this Separator Gas model.
        void validate(const GunnsFluidSeparatorGasConfigData& configData,
                      const GunnsFluidSeparatorGasInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Derives the initialization data of this Separator Gas model.
        void derive(const GunnsFluidSeparatorGasConfigData& configData,
                    const GunnsFluidSeparatorGasInputData&  inputData);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSeparatorGas(const GunnsFluidSeparatorGas&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSeparatorGas& operator=(const GunnsFluidSeparatorGas&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return     double (K) The temperature.
///
/// @details    Getter method that returns the temperature of liquid.
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSeparatorGas::getTransferTemperature() const
{
    return mTransferTemperature;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return     double (kg/s) The outgoing liquid rate.
///
/// @details    Getter method that returns the outgoing liquid rate.
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSeparatorGas::getTransferFlowRate() const
{
    return mTransferFlowRate;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return     double (kPa) The outgoing liquid pressure.
///
/// @details    Getter method that returns the outgoing liquid pressure.
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSeparatorGas::getTransferPressure() const
{
    return mTransferPressure;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return     double (kg) Liquid mass in separator.
///
/// @details    Getter method that returns the liquid mass in separator.
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSeparatorGas::getLiquidMass() const
{
    return mLiquidMass;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return     bool -- Liquid Overflow flag.
///
/// @details    Getter method that returns true if liquid is present in exit gas stream.
////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsFluidSeparatorGas::getLiquidOverflow() const
{
    return mLiquidOverflow;
}

#endif
