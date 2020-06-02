#ifndef TsValveController_EXISTS
#define TsValveController_EXISTS

/**
@file
@brief     TS21 Fluid Controller Fluid (Valve, Fan, Pump) Controllers declarations.

@defgroup  TSM_CONTROLLER_FLUID   Fluid (Valve, Fan, Pump) Controllers
@ingroup   TSM_CONTROLLER
@defgroup  TSM_CONTROLLER_FLUID_VALVE_CONTROLLER   Base (Manual) Valve Controller
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE: ((Base class for TS21 Fluid Controller valve controllers with position and malfunction.)

@details
   (The simbus will update the GUNNS fluid valve position with the mFluidPosition attribute from this class.
    Malfunctions support latching to the current position and failing to a specified position.))

 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ((TsValveController.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-10)

@{
*/

#include <string>

#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Valve Controller Model configuration data
///
/// @note:    A derived class may add position scale, bias to support other units or a convert
///           function pointer to support non-linearity between valve cmd/position/conductivity.
///
/// @details  The sole purpose of this class is to provide a data structure for the Valve Controller
///           model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsValveControllerConfigData
{
    public:
        double        mMinCmdPosition;    /**< (--)  trick_chkpnt_io(**) Minimum valid valve position. */
        double        mMaxCmdPosition;    /**< (--)  trick_chkpnt_io(**) Maximum valid valve  position. */
        double        mMinFluidPosition;  /**< (--)  trick_chkpnt_io(**) Minimum valid valve flow area fraction. */
        double        mMaxFluidPosition;  /**< (--)  trick_chkpnt_io(**) Maximum valid valve  flow area fraction. */
        /// @brief    Default constructs this Valve Controller model configuration data.
        TsValveControllerConfigData(const double minCmdPosition   = 0.0,
                                    const double maxCmdPosition   = 0.0,
                                    const double minFluidPosition = 0.0,
                                    const double maxFluidPosition = 0.0);
        /// @brief    Copy constructs this Valve Controller model configuration data.
        TsValveControllerConfigData(const TsValveControllerConfigData& that);
        /// @brief    Default destructs this Valve Controller model configuration data.
        virtual ~TsValveControllerConfigData();
        /// @brief    Assigns this Valve Controller model configuration data.
        TsValveControllerConfigData& operator=(const TsValveControllerConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Valve Controller Model input data
///
/// @details  The sole purpose of this class is to provide a data structure for the Valve Controller
///           model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsValveControllerInputData
{
    public:
        double        mCmdPosition;             /**< (--)  Valve position. */
        bool          mManualPositionFlag;      /**< (--)  Manual override valve position flag. */
        double        mManualPositionValue;     /**< (--)  Manual override valve position value. */
        /// @brief    Default constructs this Valve Controller model input data.
        TsValveControllerInputData(const double cmdPosition            = 0.0,
                                   const bool   manualPositionFlag     = false,
                                   const double manualPositionValue    = 0.0);
        /// @brief    Copy constructs this Valve Controller model input data.
        TsValveControllerInputData(const TsValveControllerInputData& that);
        /// @brief    Default destructs this Valve Controller model input data.
        virtual ~TsValveControllerInputData();
        /// @brief    Assigns this Valve Controller model input data.
        TsValveControllerInputData& operator =(const TsValveControllerInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Valve Controller Model
///
/// @details  Provides the base class for a valve controller with manual position and malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsValveController
{
    TS_MAKE_SIM_COMPATIBLE(TsValveController);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool          mMalfValveStuckFlag;      /**< (--)  Stuck valve malfunction flag. */
        bool          mMalfValveFailToFlag;     /**< (--)  Valve fail-to position malfunction flag. */
        double        mMalfValveFailToValue;    /**< (--)  Valve fail-to position malfunction value. */
        bool          mMalfManualFlag;          /**< (--)  Manual override valve malfunction flag. */
        /// @}
        /// @brief    Default constructs this Valve Controller model.
        TsValveController();
        /// @brief    Default destructs this Valve Controller model.
        virtual ~TsValveController();
        /// @brief    Initializes this Valve Controller model.
        /// @callgraph
        void initialize(const TsValveControllerConfigData& configData,
                        const TsValveControllerInputData&  initData,
                        const std::string&         name);
        /// @brief    Updates this Valve Controller model.
        /// @callgraph
        virtual void update(const double dt);
        /// @brief    Is this valve stuck?
        bool isStuck() const;
        /// @brief    Is this valve against lower limit?
        bool isLowerLimit() const;
        /// @brief    Is this valve against upper limit?
        bool isUpperLimit() const;
        /// @brief    Gets this Valve Controller model valve position.
        double getPosition() const;
        /// @brief    Gets this Valve Controller model valve fluid position.
        double getFluidPosition() const;
        /// @brief    Gets this Valve Controller model minimum valid valve position.
        double getMinPosition() const;
        /// @brief    Gets this Valve Controller model maximum valid valve position.
        double getMaxPosition() const;
        /// @brief    Sets this Valve Controller model valve position.
        void setPosition(const double position);
        /// @brief    Sets this Valve Controller model manual valve position controls.
        void setManualPosition(const bool flag, const double value);
        /// @brief    Is this Valve Controller model initialized?
        bool isInitialized() const ;
    protected:
        std::string   mName;                /**< ** (--) trick_chkpnt_io(**) Object name. */
        double        mMinCmdPosition;      /**<    (--) trick_chkpnt_io(**) Minimum valid valve position. */
        double        mMaxCmdPosition;      /**<    (--) trick_chkpnt_io(**) Maximum valid valve  position. */
        double        mFluidBias;           /**<    (--) trick_chkpnt_io(**) Bias for linear conversion of valve position to valve flow area fraction. */
        double        mFluidScale;          /**<    (--) trick_chkpnt_io(**) Scale factor for linear conversion of valve position to valve flow area fraction. */
        double        mCmdPosition;         /**<    (--)                     Valve position. */
        bool          mManualPositionFlag;  /**<    (--)                     Manual override valve position flag. */
        double        mManualPositionValue; /**<    (--)                     Manual override valve position value. */
        bool          mStuckFlag;           /**<    (--)                     Valve stuck flag. */
        bool          mLowerLimitFlag;      /**<    (--)                     Valve against lower limit flag. */
        bool          mUpperLimitFlag;      /**<    (--)                     Valve against upper limit flag. */
        double        mFluidPosition;       /**<    (--)                     Valve flow area fraction (simbus output to fluid aspect). */
        bool          mInitialized;         /**<    (--) trick_chkpnt_io(**) Initialization complete flag. */
        /// @brief    Validates this Valve Controller model initialization data.
        void validate(const TsValveControllerConfigData& configData,
                      const TsValveControllerInputData&  initData) const;
        /// @brief    Updates this Valve Controller model fractional position.
        void updatePosition(const double position);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsValveController(const TsValveController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsValveController operator= (const TsValveController&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Stuck flag.
///
/// @details  Is this Valve Controller model stuck?
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsValveController::isStuck() const
{
    return mStuckFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Lower limit flag.
///
/// @details  Is this Valve Controller model against lower limit?
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsValveController::isLowerLimit() const
{
    return mLowerLimitFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Upper limit flag.
///
/// @details  Is this Valve Controller model against upper limit?
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsValveController::isUpperLimit() const
{
    return mUpperLimitFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (--)  Valve position.
///
/// @details  Gets the valve position for this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsValveController::getPosition() const
{
    return mCmdPosition;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (--)  Valve fluid position.
///
/// @details  Gets the valve fluid position for this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsValveController::getFluidPosition() const
{
    return mFluidPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (--)  Minimum valve position.
///
/// @details  Gets the minimum valid valve position for this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsValveController::getMinPosition() const
{
    return mMinCmdPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (--)  Maximum valve position.
///
/// @details  Gets the maximum valid valve position for this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsValveController::getMaxPosition() const
{
    return mMaxCmdPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   position  (--)  Valve position.
///
/// @return   void
///
/// @details  Sets the valve position for this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsValveController::setPosition(const double position)
{
    mCmdPosition = position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   flag   (--)  Valve manual control enable flag.
/// @param[in]   value  (--)  Valve manual control position value.
///
/// @return   void
///
/// @details  Sets the valve manual position controls for this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsValveController::setManualPosition(const bool flag, const double value)
{
    mManualPositionFlag  = flag;
    mManualPositionValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  True if initialization completed successfully, false otherwise.
///
/// @details  Gets the initialization complete flag for this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsValveController::isInitialized() const
{
    return mInitialized;
}

#endif
