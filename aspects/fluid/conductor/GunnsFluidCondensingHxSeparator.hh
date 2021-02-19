#ifndef GunnsFluidCondensingHxSeparator_EXISTS
#define GunnsFluidCondensingHxSeparator_EXISTS

/**
@file
@brief    GUNNS Fluid Condensing Heat Exchanger & Phase Separator declarations

@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_CONDENSING_HX_SEPARATOR Condensing Heat Exchanger & Phase Separator
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Condensing Heat Exchanger & Phase Separator fluid aspect.)

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

LIBRARY DEPENDENCY:
-((GunnsFluidCondensingHxSeparator.o))

PROGRAMMERS:
-((Jason Harvey) (L-3 Communications) (March 2013) (Install)
  (Jason Harvey) (CACI) (December 2018) (Refactor to new base class))

@{
*/

#include "aspects/fluid/conductor/GunnsFluidCondensingHx.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Condensing Heat Exchanger & Separator Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Condensing Heat Exchanger & Separator link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidCondensingHxSeparatorConfigData: public GunnsFluidCondensingHxConfigData
{
    public:
        double mHxWetHtcFactor;      /**< (1/kg)            trick_chkpnt_io(**) Contribution of condensate to HX heat coefficient   */
        double mHxWetFlowFactor;     /**< (1/kg)            trick_chkpnt_io(**) Contribution of condensate to HX link conductivity  */
        double mHxEvaporationCoeff;  /**< (kg/s/kPa)        trick_chkpnt_io(**) Heat exchanger evaporation coefficient              */
        double mSlurperFlowMassDry;  /**< (kg)              trick_chkpnt_io(**) HX condensate to begin slurper flow from PAUSED_DRY */
        double mSlurperFlowMassWet;  /**< (kg)              trick_chkpnt_io(**) HX condensate to begin slurper flow from PAUSED_WET */
        double mSlurperFlowRateRef;  /**< (kg/s)            trick_chkpnt_io(**) Slurper condensate flow rate at WS reference speed  */
        double mWsMassExponent;      /**< (--)              trick_chkpnt_io(**) Water separator mass factor exponent                */
        double mWsMaxCondensate;     /**< (kg)              trick_chkpnt_io(**) Water separator maximum liquid mass capacity        */
        double mWsReferenceSpeed;    /**< (revolution/min)  trick_chkpnt_io(**) Water separator reference drum speed                */
        double mWsReferencePressure; /**< (kPa)             trick_chkpnt_io(**) Water separator pressure heat at reference speed    */
        double mWsReferenceTorque;   /**< (N*m)             trick_chkpnt_io(**) Water separator motor torque at reference speed     */
        double mWsDriveRatio;        /**< (--)              trick_chkpnt_io(**) Water separator gear ratio of motor to drum speed   */
        double mWsEvaporationCoeff;  /**< (kg/s/kPa)        trick_chkpnt_io(**) Water separator evaporation coefficient             */
        /// @brief  Default constructs this Condensing Heat Exchanger & Separator configuration data.
        GunnsFluidCondensingHxSeparatorConfigData(const std::string& name                = "",
                                                  GunnsNodeList*     nodes               = 0,
                                                  const double       maxConductivity     = 0.0,
                                                  const int          hxNumSegments       = 0,
                                                  const double       hxDryHtc            = 0.0,
                                                  const double       hxWetHtcFactor      = 0.0,
                                                  const double       hxWetFlowFactor     = 0.0,
                                                  const double       hxEvaporationCoeff  = 0.0,
                                                  const double       slurperFlowMassDry  = 0.0,
                                                  const double       slurperFlowMassWet  = 0.0,
                                                  const double       slurperFlowRateRef  = 0.0,
                                                  const double       wsMassExponent      = 0.0,
                                                  const double       wsMaxCondensate     = 0.0,
                                                  const double       wsReferenceSpeed    = 0.0,
                                                  const double       wsReferencePressure = 0.0,
                                                  const double       wsReferenceTorque   = 0.0,
                                                  const double       wsDriveRatio        = 0.0,
                                                  const double       wsEvaporationCoeff  = 0.0);
        /// @brief  Copy constructs this Condensing Heat Exchanger & Separator configuration data.
        GunnsFluidCondensingHxSeparatorConfigData(const GunnsFluidCondensingHxSeparatorConfigData& that);
        /// @brief  Default destructs this Condensing Heat Exchanger & Separator configuration data.
        virtual ~GunnsFluidCondensingHxSeparatorConfigData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidCondensingHxSeparatorConfigData& operator=(const GunnsFluidCondensingHxSeparatorConfigData&);
};

// Forward declaration for class defined below:
class GunnsFluidCondensingHxSeparatorInputData;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Condensing Heat Exchanger & Separator Model
///
/// @details  This is the fluid aspect of a condensing heat exchanger with hydrophilic coated fins,
///           slurper bar, and phase separator package.  This adds the hypdrophilic coating effects,
///           slurper bar and water separator functions to the base HX class.  The model adds the
///           effect of the wetted fin surface on heat transfer coefficient and resistance to the
///           bulk air flow.  The condensation is pooled as liquid water on the HX fins, flows to
///           the phase separator through the slurper bar, and from there is is pumped to the exit
///           liquid water bus.
///
///           The separator works in conjunction with a GunnsFluidSeparatorLiquid link in the same
///           or an external network.  The separator is assumed to use a centrifugal drum that
///           pressurizes the pooled liquid as it spins.  The separator pulls liquid from the heat
///           exchanger via the slurper at a rate proportional to the drum spin speed.  The liquid
///           pressure is proportional to drum speed and exponentially proportional to pooled mass
///           in the drum.  This link sends the liquid pressure & temperature to the liquid bus link
///           and receives a liquid transfer demand mass flow rate.  The transferred liquid mass is
///           removed from the drum.
///
///           The separator interfaces with a TsDcPwmDynMotor just like the GunnsGasFan link,
///           receiving motor speed and returning a torque load.
///
/// @note     All liquid mass is tracked internally and GUNNS_WATER liquid is not used in this link.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidCondensingHxSeparator : public  GunnsFluidCondensingHx
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidCondensingHxSeparator);
    public:
        /// @brief   Enumeration of the Slurper/HX flow states.
        enum SlurperStates {
            PAUSED_DRY = 0,  ///< Flow is paused and must pool to the start_dry mass before flowing.
            PAUSED_WET = 1,  ///< Flow is paused and must pool to the start_wet mass before flowing.
            FLOWING    = 2   ///< Slurper/HX is flowing to WS.
        };

        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool       mMalfHxCondensateFlag;  /**< (--)           HX condensation pool rate override malf flag         */
        double     mMalfHxCondensateValue; /**< (kg/s)         HX condensation pool rate override malf value        */
        /// @}

        /// @brief    Default constructs this Condensing Heat Exchanger & Separator.
        GunnsFluidCondensingHxSeparator();
        /// @brief    Default destructs this Condensing Heat Exchanger & Separator.
        virtual ~GunnsFluidCondensingHxSeparator();
        /// @brief    Initializes this Condensing Heat Exchanger & Separator with configuration and
        ///           input data.
        void initialize(const GunnsFluidCondensingHxSeparatorConfigData& configData,
                        const GunnsFluidCondensingHxSeparatorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&                    networkLinks,
                        const int                                        port0,
                        const int                                        port1);
        /// @brief    Updates the state of the link
        virtual void updateState(const double dt);
        /// @brief    Performs post-solution fluid calculations & transport.
        virtual void updateFluid(const double dt, const double flowRate);
        /// @brief    Special processing of data outputs from the model after to the network update.
        virtual void processOutputs();
        /// @brief    Returns the water separator motor speed.
        double       getWsMotorSpeed() const;
        /// @brief Sets and resets the HX condensation pool rate override malfunction
        void         setMalfHxCondensate(const bool flag = false, const double value = 0.0);

    protected:
        // Config data
        double     mHxWetHtcFactor;      /**<    (W/K/kg)              trick_chkpnt_io(**) Contribution of condensate to HX heat coefficient  */
        double     mHxWetFlowFactor;     /**<    (m2/kg)               trick_chkpnt_io(**) Contribution of condensate to HX link conductivity */
        double     mHxEvaporationCoeff;  /**<    (kg/s/kPa)            trick_chkpnt_io(**) Heat exchanger evaporation coefficient             */
        double     mSlurperFlowMassDry;  /**<    (kg)                  trick_chkpnt_io(**) HX condensate mass to begin slurper flow from dry  */
        double     mSlurperFlowMassWet;  /**<    (kg)                  trick_chkpnt_io(**) HX condensate mass to begin slurper flow from wet  */
        double     mWsMassExponent;      /**<    (--)                  trick_chkpnt_io(**) Water separator mass factor exponent               */
        double     mWsMaxCondensate;     /**<    (kg)                  trick_chkpnt_io(**) Water separator maximum liquid mass capacity       */
        double     mWsDriveRatio;        /**<    (--)                  trick_chkpnt_io(**) Water separator gear ratio of motor to drum speed  */
        double     mWsEvaporationCoeff;  /**<    (kg/s/kPa)            trick_chkpnt_io(**) Water separator evaporation coefficient            */

        // Attributes derived from config data
        double     mSlurperFlowFactor;   /**<    (kg*min/s/revolution) trick_chkpnt_io(**) Slurper flow rate per unit WS speed                */
        double     mWsPowerCurveCoeff;   /**<    (kPa*min/revolution)  trick_chkpnt_io(**) Water separator power curve coefficient            */
        double     mWsTorqueFactor;      /**<    (N*m*min/revolution)  trick_chkpnt_io(**) Water separator motor torque per unit WS speed     */

        // Input data
        double     mWsMotorSpeed;        /**<    (revolution/min)                          Separator speed (input from simbus)                  */
        double     mWsCondensateMass;    /**<    (kg)                                      Mass of condensate in separator                      */
        double     mHxCondensateMass;    /**<    (kg)                                      Mass of condensate in heat exchanger/slurper         */
        double     mTransferFlowRate;    /**<    (kg/s)                                    Mass flow rate to liquid network (input from simbus) */
        SlurperStates mSlurperState;     /**<    (--)                                      Slurper/HX flow state                                */

        // Internal state
        double     mSlurperFlowRate;     /**<    (kg/s)                trick_chkpnt_io(**) Condensate flow rate from HX to WS                   */
        double     mWsDrumSpeed;         /**<    (revolution/min)      trick_chkpnt_io(**) Water separator drum speed                           */
        double     mWsEvaporationRate;   /**<    (kg/s)                trick_chkpnt_io(**) Water separator evaporation rate                     */
        double     mWsDeltaPressure;     /**<    (kPa)                 trick_chkpnt_io(**) Condensate pressure head WS pump                     */

        // Outputs
        double     mTransferTemperature; /**<    (K)                                       Temperature of liquid (output to simbus)             */
        double     mTransferPressure;    /**<    (kPa)                                     Outgoing liquid pressure (output to simbus)          */
        double     mWsMotorTorque;       /**<    (N*m)                                     Water separator drum torque opposing motor           */
        bool       mLiquidOverflow;      /**<    (--)                                      Liquid present in exit gas stream (output to simbus) */
        /// @brief  Validates the initialization of this Condensing Heat Exchanger & Separator.
        void validate(const GunnsFluidCondensingHxSeparatorConfigData& configData,
                      const GunnsFluidCondensingHxSeparatorInputData&  inputData) const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief  Computes the nominal heat transfer coefficient for this Condensing Heat Exchanger.
        virtual void computeHeatTransferCoefficient();
        /// @brief  Updates the heat exchanger properties over the time step.
        virtual void updateHeatExchanger(const double dt);
        /// @brief  Updates the slurper properties over the time step.
        virtual void updateSlurper();
        /// @brief  Updates the water separator properties over the time step.
        virtual void updateWaterSeparator(const double dt);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidCondensingHxSeparator(const GunnsFluidCondensingHxSeparator& );
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidCondensingHxSeparator& operator =(const GunnsFluidCondensingHxSeparator&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Condensing Heat Exchanger & Separator Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Condensing Heat Exchanger & Separator link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidCondensingHxSeparatorInputData : public GunnsFluidCondensingHxInputData
{
    public:
        double  mWsMotorSpeed;       /**< (revolution/min) Initial speed of the water separator motor    */
        double  mWsCondensateMass;   /**< (kg)             Initial mass of condensate in separator       */
        double  mHxCondensateMass;   /**< (kg)             Initial mass of condensate in heat exchanger  */
        double  mTransferFlowRate;   /**< (kg/s)           Initial mass flow rate to liquid network      */
        GunnsFluidCondensingHxSeparator::SlurperStates
                mSlurperState;       /**< (--)             Initial Slurper/HX flow state                 */
        /// @brief  Default constructs this Condensing Heat Exchanger & Separator input data.
        GunnsFluidCondensingHxSeparatorInputData(
                const bool   malfBlockageFlag  = false,
                const double malfBlockageValue = 0.0,
                const double wallTemperature   = 0.0,
                const double wsMotorSpeed      = 0.0,
                const double wsCondensateMass  = 0.0,
                const double hxCondensateMass  = 0.0,
                const double transferFlowRate  = 0.0,
                const GunnsFluidCondensingHxSeparator::SlurperStates slurperState
                                               = GunnsFluidCondensingHxSeparator::PAUSED_DRY);
        /// @brief  Copy constructs this Condensing Heat Exchanger & Separator input data.
        GunnsFluidCondensingHxSeparatorInputData(const GunnsFluidCondensingHxSeparatorInputData& that);
        /// @brief  Default destructs this Condensing Heat Exchanger & Separator input data.
        virtual ~GunnsFluidCondensingHxSeparatorInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidCondensingHxSeparatorInputData& operator=(const GunnsFluidCondensingHxSeparatorInputData&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (revolution/min) Motor speed of the water separator.
///
/// @details  Rate of water removed from the air stream and pooled in the heat exchanger.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidCondensingHxSeparator::getWsMotorSpeed() const
{
    return mWsMotorSpeed;
}

#endif
