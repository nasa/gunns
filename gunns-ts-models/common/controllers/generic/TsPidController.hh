#ifndef TsPidController_EXISTS
#define TsPidController_EXISTS

/**
@file
@brief    Generic PID Controller Model declarations.

@defgroup TSM_CONTROLLER_GENERIC  Generic Controllers
@ingroup  TSM_CONTROLLER
@defgroup TSM_CONTROLLER_GENERIC_PID  Proportional Integral Derivative Controller
@ingroup  TSM_CONTROLLER_GENERIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:  (Provides the classes for a generic PID Controller Model.)

@details
REFERENCE:
 - ()

ASSUMPTIONS AND LIMITATIONS:
 - ()

LIBRARY DEPENDENCY:
 - (TsPidController.o)

PROGRAMMERS:
 - ((Jason Harvey) (CACI) (Initial) (January 2019))
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PID Controller Model Configuration Data.
///
/// @details  This class provides a data structure for the PID Controller config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsPidControllerConfigData  {
    public:
        double mGainP;      /**< (--)  trick_chkpnt_io(**) Proportional gain. */
        double mGainI;      /**< (1/s) trick_chkpnt_io(**) Integral gain. */
        double mGainD;      /**< (s)   trick_chkpnt_io(**) Derivative gain. */
        double mInterval;   /**< (s)   trick_chkpnt_io(**) Control update interval. */
        double mLimitLow;   /**< (--)  trick_chkpnt_io(**) Lower limit on output value. */
        double mLimitHigh;  /**< (--)  trick_chkpnt_io(**) Upper limit on output value. */
        /// @brief  Default constructs this PID Controller configuration data.
        TsPidControllerConfigData(const double gainP     = 0.0,
                                  const double gainI     = 0.0,
                                  const double gainD     = 0.0,
                                  const double interval  = 0.0,
                                  const double limitLow  = 0.0,
                                  const double limitHigh = 0.0);
        /// @brief  Default destructs this PID Controller configuration data.
        virtual ~TsPidControllerConfigData();
        /// @brief  Copy constructs this PID Controller configuration data.
        TsPidControllerConfigData(const TsPidControllerConfigData& that);
        /// @brief  Assignment operator for this PID Controller configuration data.
        TsPidControllerConfigData& operator=(const TsPidControllerConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PID Controller Model Input Data.
///
/// @details  This class provides a data structure for the PID Controller input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsPidControllerInputData {
    public:
        double mInput;      /**< (--)  trick_chkpnt_io(**) Initial input process variable. */
        double mSetpoint;   /**< (--)  trick_chkpnt_io(**) Initial setpoint value to control to. */
        double mTimer;      /**< (s)   trick_chkpnt_io(**) Initial timer for control update interval. */
        double mIntegral;   /**< (s)   trick_chkpnt_io(**) Initial controller integral value. */
        double mOutput;     /**< (--)  trick_chkpnt_io(**) Initial output control value. */
        /// @brief  Default constructs this PID Controller input data.
        TsPidControllerInputData(const double input      = 0.0,
                                 const double setpoint   = 0.0,
                                 const double timer      = 0.0,
                                 const double integral   = 0.0,
                                 const double output     = 0.0);
        /// @brief  Default destructs this PID Controller input data.
        virtual ~TsPidControllerInputData();
        /// @brief  Copy constructs this PID Controller input data.
        TsPidControllerInputData(const TsPidControllerInputData& that);
        /// @brief  Assignment operator for this PID Controller input data.
        TsPidControllerInputData& operator=(const TsPidControllerInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PID Controller Model.
///
/// @details  Models a Proportional Integral Derivative controller.  The controller updates at a
///           configured interval that can differ from the execution call period.  The output value
///           is limited to a configurable range.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsPidController  {
    TS_MAKE_SIM_COMPATIBLE(TsPidController);
    public:
        /// @brief  Default constructs this PID Controller.
        TsPidController();
        /// @brief  Default destructs this PID Controller.
        virtual ~TsPidController();
        /// @brief  Initializes this PID Controller.
        void   initialize(const TsPidControllerConfigData &configData,
                          const TsPidControllerInputData  &inputData,
                          const std::string               &name);
        /// @brief  Updates the PID Controller.
        double update(const double dt);
        /// @brief  Updates the PID Controller with input argument.
        double update(const double dt, const double input);
        /// @brief  Sets the setpoint value to control the input to.
        void   setSetpoint(const double setpoint);
        /// @brief  Sets the input process variable to control.
        void   setInput(const double input);
        /// @brief  Returns the computed controller output.
        double getOutput() const;
        /// @brief  Returns true if this PID Controller has been properly initialized.
        bool   isInitialized() const;

    protected:
        double      mGainP;      /**< (--)  trick_chkpnt_io(**) Proportional gain. */
        double      mGainI;      /**< (1/s) trick_chkpnt_io(**) Integral gain. */
        double      mGainD;      /**< (s)   trick_chkpnt_io(**) Derivative gain. */
        double      mInterval;   /**< (s)   trick_chkpnt_io(**) Control update interval. */
        double      mLimitLow;   /**< (--)  trick_chkpnt_io(**) Lower limit on output value. */
        double      mLimitHigh;  /**< (--)  trick_chkpnt_io(**) Upper limit on output value. */
        double      mInput;      /**< (--)                      Input process variable to control. */
        double      mSetpoint;   /**< (--)                      Setpoint value to control to. */
        double      mTimer;      /**< (s)                       Timer for control update interval. */
        double      mIntegral;   /**< (s)                       Controller integral value. */
        double      mOutput;     /**< (--)                      Output control value. */
        double      mError;      /**< (--)                      Controller error value. */
        double      mDerivative; /**< (1/s)                     Controller derivative value. */
        std::string mName;       /**< (--)  trick_chkpnt_io(**) Name of the instance for messages. */
        bool        mInitFlag;   /**< (--)  trick_chkpnt_io(**) Object initialization flag. */
        /// @brief  Validates a PID controller configuration data.
        void validate(const TsPidControllerConfigData &configData) const;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        TsPidController(const TsPidController&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        TsPidController& operator= (const TsPidController&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] setpoint (--) The value to control the input to.
///
/// @details  Sets the mSetpoint attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPidController::setSetpoint(const double setpoint)
{
    mSetpoint = setpoint;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] input (--) The input value to be controlled.
///
/// @details  Sets the mInput attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsPidController::setInput(const double input)
{
    mInput = input;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (--) Controller output.
///
/// @details  Returns the mOutput attribute value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsPidController::getOutput() const
{
    return mOutput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  (--)  True if the model initialized properly.
///
/// @details  Returns true if the model initialized properly.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsPidController::isInitialized() const
{
    return mInitFlag;
}

#endif
