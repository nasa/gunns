#ifndef TsLimitSwitchAnalog_EXISTS
#define TsLimitSwitchAnalog_EXISTS

/**
@defgroup  TSM_SENSORS_LimitSwitchAnalog  Limit switch with analog rather than boolean input
@ingroup   TSM_SENSORS

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
 - (Based on analog data, a target value and a tolerance determine if the limit switch is
    at the target value.  Then LimitSwitch the resulting boolean value.)

 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ((TsLimitSwitchAnalog.o))

 PROGRAMMERS:
 - ((Chuck Sorensen) (LZT) (Dec 2011) (TS21) (initial))
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "common/sensors/TsLimitSwitch.hh"

/// @brief ConfigData for model
class TsLimitSwitchAnalogConfigData : public TsLimitSwitchConfigData {
public:
    double mTargetValue;  /**< (--) trick_chkpnt_io(**) target value */
    double mTolerance;    /**< (--) trick_chkpnt_io(**) tolerance    */

    TsLimitSwitchAnalogConfigData();
    virtual ~TsLimitSwitchAnalogConfigData();
protected:
private:
    /// @brief Unimplemented
    TsLimitSwitchAnalogConfigData(const TsLimitSwitchAnalogConfigData &rhs);
    /// @brief Unimplemented
    TsLimitSwitchAnalogConfigData& operator =(const TsLimitSwitchAnalogConfigData &rhs);
};


/// @brief InputData for model
class TsLimitSwitchAnalogInputData : public TsLimitSwitchInputData {
public:
    TsLimitSwitchAnalogInputData(const bool trueValue = false,
                                 const bool sensedValue = false,
                                 const bool failedValue = false);
    virtual ~TsLimitSwitchAnalogInputData();
protected:
private:
    /// @brief Unimplemented
    TsLimitSwitchAnalogInputData(const TsLimitSwitchAnalogInputData &rhs);
    /// @brief Unimplemented
    TsLimitSwitchAnalogInputData& operator =(const TsLimitSwitchAnalogInputData &rhs);
};

/// @brief limit switch with analog input model
class TsLimitSwitchAnalog : public TsLimitSwitch {
    TS_MAKE_SIM_COMPATIBLE(TsLimitSwitchAnalog);
public:

    TsLimitSwitchAnalog(void);
    virtual ~TsLimitSwitchAnalog( void );
    void initialize(const TsLimitSwitchAnalogConfigData &configData,
                    const TsLimitSwitchAnalogInputData  &inputData);
    void update(const double realValue, const bool isPowered, const bool isFailed);
protected:
    double mAnalogValue;    /**< (--) value from update */
    double mTargetValue;    /**< (--) trick_chkpnt_io(**) target value      */
    double mTolerance;      /**< (--) trick_chkpnt_io(**) tolerance         */

private:
    /// @brief keep private, never used
    TsLimitSwitchAnalog(const TsLimitSwitchAnalog &rhs);

    /// @brief keep private, never used
    TsLimitSwitchAnalog& operator= (const TsLimitSwitchAnalog &rhs);

};




/// @}


#endif //TsLimitSwitchAnalog_EXISTS
