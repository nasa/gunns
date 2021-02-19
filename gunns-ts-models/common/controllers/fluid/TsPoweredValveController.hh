#ifndef TsPoweredValveController_EXISTS
#define TsPoweredValveController_EXISTS

/**
@file
@brief     TS21 Fluid Controller Powered (Motor or Solenoid) Valve Controller declarations.

@defgroup  TSM_CONTROLLER_FLUID_POWERED_VALVE_CONTROLLER   Powered (Motor or Solenoid) Valve Controller
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Base class for TS21 Fluid Controller powered valve controllers with command, power and malfunction.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ((TsPoweredValveController.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-10)

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

#include "TsValveController.hh"

class TsPoweredValveControllerConfigData;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Powered Valve Controller Model input data
///
/// @details  The sole purpose of this class is to provide a data structure for the Powered Valve
///           Controller model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsPoweredValveControllerInputData : public TsValveControllerInputData
{
    public:
        bool          mSupplyVoltageFlag;      /**< (--)  Supply voltage good flag. */
        bool          mEnabledFlag;            /**< (--)  Controller enabled flag. */
        double        mCmd;                    /**< (--)  Speed command. */
        /// @brief    Default constructs this Powered Valve Controller model input data.
        TsPoweredValveControllerInputData(const double cmdPosition            = 0.0,
                                          const bool   manualPositionFlag     = false,
                                          const double manualPositionValue    = 0.0,
                                          const bool   supplyVoltageFlag      = false,
                                          const bool   enabledFlag            = false,
                                          const double cmd                    = 0.0);
        /// @brief    Copy constructs this Powered Valve Controller model input data.
        TsPoweredValveControllerInputData(const TsPoweredValveControllerInputData& that);
        /// @brief    Default destructs this Powered Valve Controller model input data.
        virtual ~TsPoweredValveControllerInputData();
        /// @brief    Assigns this Powered Valve Controller model input data.
        TsPoweredValveControllerInputData& operator =(const TsPoweredValveControllerInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Powered Valve Controller Model
///
/// @details  Provides the base class for a powered (motor or solenoid) valve controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsPoweredValveController : public TsValveController
{
    TS_MAKE_SIM_COMPATIBLE(TsPoweredValveController);
    public:
        /// @brief    Enumeration of valve position behavior when power is removed.
        enum LatchType {
            LATCHING        = 0,  ///< Position stays at current value.
            NORMALLY_OPEN   = 1,  ///< Position goes open (1.0).
            NORMALLY_CLOSED = 2   ///< Position goes closed (0.0).
        };
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool                 mMalfPowerGoodFlag;       /**< (--)     Ignore supply voltage failure flag. */
        bool                 mMalfPowerFailFlag;       /**< (--)     Force supply voltage failure flag. */
        bool                 mMalfDriveStuckFlag;      /**< (--)     Stuck motor/solenoid malfunction flag. */
        bool                 mMalfDriveFailToFlag;     /**< (--)     Motor/solenoid fail-to position malfunction flag. */
        double               mMalfDriveFailToValue;    /**< (--)     Motor/solenoid fail-to position malfunction value. */
        /// @}
        /// @brief    Default constructs this Powered Valve Controller model.
        TsPoweredValveController();
        /// @brief    Default destructs this Powered Valve Controller model.
        virtual ~TsPoweredValveController();
        /// @brief    Initializes this Powered Valve Controller model.
        /// @callgraph
        void initialize(const TsPoweredValveControllerConfigData& config,
                        const TsPoweredValveControllerInputData&  init,
                        const std::string&                        name);
        /// @brief    Updates the position and power consumption for this Powered Valve Controller model .
        /// @callgraph
        virtual void update(const double dt);
        /// @brief    Updates the position and power consumption for this Powered Valve Controller model.
        void update(const bool   supplyVoltageFlag,
                    const bool   enabledFLag,
                    const double cmd,
                    const double dt);
        /// @brief    Sets the supply voltage flag for this Powered Valve Controller model.
        void setSupplyVoltageFlag(const bool supplyVoltageFlag);
        /// @brief    Sets the enabled flag for this Powered Valve Controller model.
        virtual void setEnabledFlag(const bool enabledFlag);
        /// @brief    Sets the speed command for this Powered Valve Controller model.
        void setCmd(const double cmd);
        /// @brief    Gets the command scale from this Powered Valve Controller model.
        double getCmdScale();
        /// @brief    Gets the supply voltage flag for this Powered Valve Controller model.
        bool getSupplyVoltageFlag() const;
        /// @brief    Gets the power consumed by this Powered Valve Controller model.
        double getPower() const;
        /// @brief    Gets the resistance of this Powered Valve Controller model.
        double getResistance() const;
    protected:
        double               mCmdScale;                /**< (1/s)  trick_chkpnt_io(**) Speed command normalization scale factor. */
        double               mHoldPower;               /**< (W)    trick_chkpnt_io(**) Power consumption when valve is holding. */
        double               mMovePower;               /**< (W)    trick_chkpnt_io(**) Power consumption when valve is moving. */
        double               mStuckPower;              /**< (W)    trick_chkpnt_io(**) Power consumption when valve is stuck. */
        double               mHoldResistance;          /**< (ohm)  trick_chkpnt_io(**) Resistance when valve is holding. */
        double               mMoveResistance;          /**< (ohm)  trick_chkpnt_io(**) Resistance when valve is moving. */
        double               mStuckResistance;         /**< (ohm)  trick_chkpnt_io(**) Resistance when valve is stuck. */
        bool                 mEotInterrupt;            /**< (--)   trick_chkpnt_io(**) End of travel interrupting flag. */
        LatchType            mLatch;                   /**< (--)   trick_chkpnt_io(**) Latch type. */
        bool                 mSupplyVoltageFlag;       /**< (--)   Supply voltage good flag. */
        bool                 mEnabledFlag;             /**< (--)   Controller enabled flag. */
        double               mCmd;                     /**< (--)   Speed command. */
        double               mPower;                   /**< (W)    Power consumption. */
        double               mResistance;              /**< (ohm)  Resistance. */
        static const double  MAX_RESISTANCE;           /**< (ohm)  Maximum resistance. */
        /// @brief    Validates this Powered Valve Controller model initialization data.
        void validate(const TsPoweredValveControllerConfigData& configData,
                      const TsPoweredValveControllerInputData&  initData) const;
        /// @brief    Updates this Powered Valve Controller model power consumption.
        virtual void updatePower();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsPoweredValveController(const TsPoweredValveController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsPoweredValveController& operator= (const TsPoweredValveController&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Powered Valve Controller Model configuration data
///
/// @details  The sole purpose of this class is to provide a data structure for the Powered Valve
///           Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsPoweredValveControllerConfigData : public TsValveControllerConfigData
{
    public:
        double                               mTransitTime;  /**< (s)   trick_chkpnt_io(**) Reference transit time. */
        double                               mRefCmd;       /**< (--)  trick_chkpnt_io(**) Reference speed command. */
        double                               mHoldPower;    /**< (W)   trick_chkpnt_io(**) Power consumption when valve is holding. */
        double                               mMovePower;    /**< (W)   trick_chkpnt_io(**) Power consumption when valve is moving. */
        double                               mStuckPower;   /**< (W)   trick_chkpnt_io(**) Power consumption when valve is stuck. */
        double                               mRefVoltage;   /**< (V)   trick_chkpnt_io(**) Reference voltage for power consumption. */
        bool                                 mEotInterrupt; /**< (--)  trick_chkpnt_io(**) End of travel interrupting flag. */
        TsPoweredValveController::LatchType  mLatch;        /**< (--)  trick_chkpnt_io(**) Latching type. */
        /// @brief    Default constructs this Powered Valve Controller model configuration data.
        TsPoweredValveControllerConfigData(const double                              minCmdPosition   = 0.0,
                                           const double                              maxCmdPosition   = 0.0,
                                           const double                              minFluidPosition = 0.0,
                                           const double                              maxFluidPosition = 0.0,
                                           const double                              transitTime      = 0.0,
                                           const double                              refCmd           = 0.0,
                                           const double                              holdPower        = 0.0,
                                           const double                              movePower        = 0.0,
                                           const double                              stuckPower       = 0.0,
                                           const double                              refVoltage       = 0.0,
                                           const bool                                eotInterrupt     = false,
                                           const TsPoweredValveController::LatchType latch            = TsPoweredValveController::LATCHING);
        /// @brief    Copy constructs this Powered Valve Controller model configuration data.
        TsPoweredValveControllerConfigData(const TsPoweredValveControllerConfigData& that);
        /// @brief    Default destructs this Powered Valve Controller model configuration data.
        virtual ~TsPoweredValveControllerConfigData();
        /// @brief    Assigns this Powered Valve Controller model configuration data.
        TsPoweredValveControllerConfigData& operator=(const TsPoweredValveControllerConfigData& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   supplyVoltageFlag  (--)  Supply voltage flag.
///
/// @return   void
///
/// @details  Sets the supply voltage flag for this Powered Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPoweredValveController::setSupplyVoltageFlag(const bool supplyVoltageFlag)
{
    mSupplyVoltageFlag = supplyVoltageFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   enabledFlag  (--)  Enabled flag.
///
/// @return   void
///
/// @details  Sets the enabled flag for this Powered Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPoweredValveController::setEnabledFlag(const bool enabledFlag)
{
    mEnabledFlag = enabledFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   cmd  (--)  Speed command.
///
/// @return   void
///
/// @details  Sets the speed command for this Powered Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPoweredValveController::setCmd(const double cmd)
{
    mCmd = cmd;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double
///
/// @details  Gets the command scale for this Powered Valve Controller model. Used by firmware
///           controllers.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsPoweredValveController::getCmdScale()
{
    return mCmdScale;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (W) Power consumption.
///
/// @details  Gets the power consumption of this Powered Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsPoweredValveController::getPower() const
{
    return mPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (ohm) Resistance.
///
/// @details  Gets the resistance of this Powered Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsPoweredValveController::getResistance() const
{
    return mResistance;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (--) Supply Voltage Flag.
///
/// @details  Gets the supply voltage flag of this Powered Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsPoweredValveController::getSupplyVoltageFlag() const
{
    return mSupplyVoltageFlag;
}

#endif
