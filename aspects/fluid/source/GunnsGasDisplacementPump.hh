#ifndef GunnsGasDisplacementPump_EXISTS
#define GunnsGasDisplacementPump_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_SOURCE_GAS_DISPLACEMENT_PUMP  Gas Displacement Pump Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Gas Displacement Pump link model.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsGasDisplacementPump.o))

 PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Initial) (2012-07))

@{
*/

#include "core/GunnsFluidSource.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gas Displacement Pump Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Gas
///           Displacement Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsGasDisplacementPumpConfigData : public GunnsFluidSourceConfigData
{
    public:
        double mCycleVolume;          /**< (m3) trick_chkpnt_io(**) Volume of fluid displaced per cycle               */
        double mDriveRatio;           /**< (--) trick_chkpnt_io(**) Gear ratio of motor to impeller speed             */
        double mThermalLength;        /**< (m)  trick_chkpnt_io(**) Impeller length for thermal convection            */
        double mThermalDiameter;      /**< (m)  trick_chkpnt_io(**) Impeller inner diameter for thermal convection    */
        double mSurfaceRoughness;     /**< (m)  trick_chkpnt_io(**) Impeller surface roughness for thermal convection */
        bool   mCheckValveActive;     /**< (--) trick_chkpnt_io(**) Flag indicating check valve functionality is active     */
        /// @brief    Default constructs this Gas Displacement Pump model configuration data with
        ///           arguments.
        GunnsGasDisplacementPumpConfigData(const std::string& name             = "",
                                           GunnsNodeList*     nodes            = 0,
                                           const double       cycleVolume      = 0.0,
                                           const double       driveRatio       = 1.0,
                                           const double       thermalLength    = 0.0,
                                           const double       thermalDiameter  = 0.0,
                                           const double       surfaceRoughness = 0.0,
                                           const bool         checkValveActive = false);
        /// @brief    Default destructs this Gas Displacement Pump model configuration data.
        virtual ~GunnsGasDisplacementPumpConfigData();
        /// @brief    Copy constructs this Gas Displacement Pump model configuration data.
        GunnsGasDisplacementPumpConfigData(const GunnsGasDisplacementPumpConfigData& that);

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasDisplacementPumpConfigData& operator =(const GunnsGasDisplacementPumpConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gas Displacement Pump Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Gas
///           Displacement Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsGasDisplacementPumpInputData : public GunnsFluidSourceInputData
{
    public:
        double mMotorSpeed;                      /**< (revolution/min) Initial speed of the motor        */
        double mWallTemperature;                 /**< (K)              Initial impeller wall temperature */
        /// @brief    Default constructs this Gas Displacement Pump model input data with arguments.
        GunnsGasDisplacementPumpInputData(const bool   malfBlockageFlag  = false,
                                          const double malfBlockageValue = 0.0,
                                          const double flowDemand        = 0.0,
                                          const double motorSpeed        = 0.0,
                                          const double wallTemperature   = 0.0);
        /// @brief    Copy constructs this Gas Displacement Pump model input data.
        GunnsGasDisplacementPumpInputData(const GunnsGasDisplacementPumpInputData& that);
        /// @brief    Default destructs this Gas Displacement Pump model input data.
        ~GunnsGasDisplacementPumpInputData();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasDisplacementPumpInputData& operator=(const GunnsGasDisplacementPumpInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gas Displacement Pump Model
///
/// @details
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsGasDisplacementPump : public GunnsFluidSource
{
    TS_MAKE_SIM_COMPATIBLE(GunnsGasDisplacementPump);
    public:
        /// @brief    Default constructs this Gas Displacement Pump model.
        GunnsGasDisplacementPump();
        /// @brief    Default destructs this Gas Displacement Pump model.
        virtual ~GunnsGasDisplacementPump();
        /// @brief    Initializes this Gas Displacement Pump model with configuration and input data.
        void initialize(const GunnsGasDisplacementPumpConfigData& configData,
                        const GunnsGasDisplacementPumpInputData&  inputData,
                        std::vector<GunnsBasicLink*>&           networkLinks,
                        const int                               port0,
                        const int                               port1);
        /// @brief    Updates the state of this Gas Displacement Pump model.
        virtual void updateState(const double dt);
        /// @brief    Updates the internal fluid of this Gas Displacement Pump model.
        virtual void updateFluid(const double dt, const double flowRate);
        /// @brief    Returns the wall heat flux.
        double getWallHeatFlux();
        /// @brief    Returns the impeller fluid torque.
        double getImpellerTorque();
        /// @brief    Returns the impeller speed.
        double getImpellerSpeed();
        /// @brief    Returns the impeller fluid power.
        double getImpellerPower();
        /// @brief    Returns the impeller wall temperature.
        double getWallTemperature();
        /// @brief    Sets the thermal surface area of this Gas Displacement Pump model.
        void   setThermalSurfaceArea(const double value);
        /// @brief    Sets the wall temperature of this Gas Displacement Pump model.
        void   setWallTemperature(const double value);
        /// @brief    Turn on or off the check valve functionality
        void   setCheckValveFlag(const bool value);

    protected:
        double mCycleVolume;        /**< (m3)             trick_chkpnt_io(**) Volume of fluid displaced per cycle            */
        double mDriveRatio;         /**< (--)             trick_chkpnt_io(**) Gear ratio of motor to impeller cycles         */
        double mThermalDiameter;    /**< (m)              trick_chkpnt_io(**) Impeller inner diameter for thermal convection */
        double mThermalSurfaceArea; /**< (m2)             trick_chkpnt_io(**) Impeller surface area for thermal convection   */
        double mThermalROverD;      /**< (--)             trick_chkpnt_io(**) Impeller convection surface roughness/diameter */
        double mMotorSpeed;         /**< (revolution/min)                     Motor rotational speed input from simbus       */
        double mWallTemperature;    /**< (K)                                  Impeller wall temperature input from simbus    */
        double mWallHeatFlux;       /**< (W)                                  Heat flux from fluid to wall output to simbus  */
        double mImpellerTorque;     /**< (N*m)                                Impeller fluid torque opposing rotation to simbus */
        double mImpellerSpeed;      /**< (revolution/min) trick_chkpnt_io(**) Impeller rotational speed                      */
        double mImpellerPower;      /**< (W)              trick_chkpnt_io(**) Power imparted on fluid by the impeller        */
        bool   mCheckValveActive;   /**< (--)             trick_chkpnt_io(**) Flag indicating check valve functionality is active     */
        double mCheckValvePosition; /**< (--)             trick_chkpnt_io(**) Check valve current position                            */
        /// @brief    Validates the initialization inputs of this Gas Displacement Pump model.
        void validate(const GunnsGasDisplacementPumpConfigData& configData,
                      const GunnsGasDisplacementPumpInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Virtual interface for application of cavitation in derived classes.
        virtual void applyCavitation(const double dt);
        /// @brief    Transports flow between nodes specifying source node mixture option.
        virtual void transportFluidMixtureOption();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasDisplacementPump(const GunnsGasDisplacementPump&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsGasDisplacementPump& operator=(const GunnsGasDisplacementPump&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) The heat flux from the fluid to the impeller wall.
///
/// @details  Returns the heat flux from the fluid to the impeller wall.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasDisplacementPump::getWallHeatFlux()
{
    return mWallHeatFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (N*m) The fluid torque opposing impeller rotation.
///
/// @details  Returns the fluid torque opposing impeller rotation.  This is signed relative to the
///           normal rotational direction of the impeller, so will usually be negative.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasDisplacementPump::getImpellerTorque()
{
    return mImpellerTorque;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (revolution/min) The impeller rotational speed.
///
/// @details  Returns the impeller rotational speed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasDisplacementPump::getImpellerSpeed()
{
    return mImpellerSpeed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (K) The impeller wall temperature.
///
/// @details  Returns the impeller wall temperature value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasDisplacementPump::getWallTemperature()
{
    return mWallTemperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) The power imparted on the fluid by the impeller.
///
/// @details  Returns the power imparted on the fluid by the impeller.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsGasDisplacementPump::getImpellerPower()
{
    return mImpellerPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Not used.
///
/// @returns  void
///
/// @details  This provides an interface for derived classes to apply cavitation where needed.  The
///           GunnsGasDisplacementPump does not model cavitation.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsGasDisplacementPump::applyCavitation(const double dt __attribute__((unused)))
{
    //nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Calls the base class to transport the fluid between the nodes. The false bool argument
///           indicates that this class cannot force a mixture change in the supplying node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsGasDisplacementPump::transportFluidMixtureOption()
{
    transportFluid(false);
}

#endif
