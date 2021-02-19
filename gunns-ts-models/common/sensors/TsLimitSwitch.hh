#ifndef TsLimitSwitch_EXISTS
#define TsLimitSwitch_EXISTS

/**
@defgroup  TSM_SENSORS_LimitSwitch Limit Switch
@ingroup   TSM_SENSORS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
 - (A LimitSwitch takes inputs of a boolean value, is powered or is failed and produces as boolean
    sensed value. If not powered the sensed value is the not powered value, from config data.  If
    powered and is failed the sensed value is the value of the failed variable.)

 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - (
    (TsLimitSwitch.o)
   )

 PROGRAMMERS:
 - ((Chuck Sorensen) (LZT) (Dec 2011) (TS21) (initial))
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

/// @brief ConfigData for model
class TsLimitSwitchConfigData {
public:
    bool mNotPoweredValue;  /**< (--) trick_chkpnt_io(**) value when not powered */

    TsLimitSwitchConfigData();
    virtual ~TsLimitSwitchConfigData();
protected:
private:
    /// @brief Unimplemented
    TsLimitSwitchConfigData(const TsLimitSwitchConfigData &rhs);
    /// @brief Unimplemented
    TsLimitSwitchConfigData& operator =(const TsLimitSwitchConfigData &rhs);
};


/// @brief InputData for model
class TsLimitSwitchInputData {
public:

    bool mTrueValue;   /**< (--) true value        */
    bool mSensedValue; /**< (--) sensed value      */
    bool mFailedValue; /**< (--) value when failed */

    TsLimitSwitchInputData(const bool trueValue = false,
                           const bool sensedValue = false,
                           const bool failedValue = false);
    virtual ~TsLimitSwitchInputData();
protected:
private:
    /// @brief Unimplemented
    TsLimitSwitchInputData(const TsLimitSwitchInputData &rhs);
    /// @brief Unimplemented
    TsLimitSwitchInputData& operator =(const TsLimitSwitchInputData &rhs);
};

/// @brief A limit switch
class TsLimitSwitch {
    TS_MAKE_SIM_COMPATIBLE(TsLimitSwitch);
public:

    TsLimitSwitch(void);
    virtual ~TsLimitSwitch( void );
    void initialize(const TsLimitSwitchConfigData &configData,
            const TsLimitSwitchInputData &inputData);
    bool getValue() const ;
    bool isInitialized() const ;
    void setFailedValue(const bool fail);
    void update(const bool realValue,
                const bool isPowered,
                const bool isFailied);
protected:
    bool mInitialized;      /**< (--) indicates switch has been initialized */
    bool mTrueValue;        /**< (--) true value                            */
    bool mSensedValue;      /**< (--) sensed value                          */
    bool mFailedValue;      /**< (--) value when failed                     */
    bool mNotPoweredValue;  /**< (--) trick_chkpnt_io(**) value when not powered                */


private:
    /// @brief keep private, never used
    TsLimitSwitch(const TsLimitSwitch &rhs);

    /// @brief keep private, never used
    TsLimitSwitch& operator= (const TsLimitSwitch &rhs);

};
/// @}

/// @brief returns the sensed value
inline bool TsLimitSwitch::getValue() const {
    return mSensedValue;
}

/// @brief returns the initialization flag
inline bool TsLimitSwitch::isInitialized() const {
    return mInitialized;
}

/// @param[in] fail -- fail value
/// @brief set the value to return when failed
inline void TsLimitSwitch::setFailedValue(const bool fail) {
    mFailedValue = fail;
}


#endif //TsLimitSwitch_EXISTS
