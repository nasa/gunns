#ifndef _USER_LOAD_BASE_HH_
#define _USER_LOAD_BASE_HH_

/*
@defgroup  TSM_GUNNS_ELECTRICAL_USERLOAD  User Load Base
@ingroup   TSM_GUNNS_ELECTRICAL_USERLOADBASE

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (This is the base class for the User Load classes. The Constant Power and Constant resistance User Load
  classes are derived from this base class. Most components that have an electrical aspect will derive from
  either the constant resistance or the constant power user load classes. Components should not derive from the
  UserLoadBase class.
  The voltage value is the input parameter for the step method. The power is calculated based on the voltage for the load.
  For the constant resistance load, the resistance value is used to calculate the power and the current.
  For constant power load the power value is assigned to the variable mActualPower, and the current and resistance
  values are calculated based on the power.

  The mPowerValid flag is set to true if the voltage is a above the underVoltage limit. Power Valid flag is only set if
  the input voltage is above the under voltage limit. If the load is OFF then the power and current is set to zero, and the
  resistance is set to maximum resistance.
  This flag is passed to the ICD so other aspects of the model can grab the value to update the status of their
  electrical aspect.
  The loadID is used to identify the particular user load on the loadSwitchCard.
  The cardID is used to identify a loadSwitchCard in the network
  .
  The user load can operate in LoadON, LoadOFF and LoadSTANDBY mode.
)

 REQUIREMENTS:
 ()

 REFERENCE:
 ()

 ASSUMPTIONS AND LIMITATIONS:
 ()

 LIBRARY DEPENDENCY:
 (
     (aspects/electrical/UserLoad/UserLoadBase.o)
 )

 PROGRAMMERS:
 (
     ((Shailaja Janapati) (L-3) (Initial) (October 2011))
 )
 @{
 */
#include <string>
#include <vector>

// #include "aspects/electrical/UserLoad/UserLoadElect.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"


/// @brief the enum values for load type
typedef enum  {
   CONSTANT_POWER_LOAD = 0,
   RESISTIVE_LOAD = 1
} LOAD_TYPE;


/// @brief  Most user loads will operate in either one of these modes. On/Off/Standby.
///         For loads needing only these 3 modes of operation using the Constant Resistance class
///         or Constant Power class will suffice for their modelling of the electrical aspect.
typedef enum {
    LoadOFF = 0,
    LoadON = 1,
    LoadSTANDBY = 2
} UserLoadMode;



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    aspects/electrical/ User Load class Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the user load
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UserLoadBaseConfigData
{
public:

    /// @brief User Load object name
    std::string mName;      /**< (--) trick_chkpnt_io(**) Load Name */

    /// @brief Type of user load read from config object
    int mUserLoadType;     /**< (--) trick_chkpnt_io(**) load type set to constant resistance or constant power */

    ///@brief under voltage limit value
    double mUnderVoltageLimit; /**< (V)   trick_chkpnt_io(**) under voltage limit */
    double mFuseCurrentLimit;  /**< (amp) trick_chkpnt_io(**) Current above which the fuse blows. */
    /// @brief Default Constructor
    UserLoadBaseConfigData(const std::string &name = "",
            const int loadType = RESISTIVE_LOAD,
            const double underVoltageLimit = 98.0,
            const double fuseCurrentLimit  = 0.0);

    /// @brief Default Destructor
    virtual ~UserLoadBaseConfigData();

    /// @brief Copy constructs this User Load Elect configuration data.
    UserLoadBaseConfigData(const UserLoadBaseConfigData& that);

private:
    /// @brief Assignment operator unavailable since declared private and not implemented.
    UserLoadBaseConfigData& operator =(const UserLoadBaseConfigData&);
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    aspects/electrical/ User Load class Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the user load
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UserLoadBaseInputData
{
public:
    /// @brief Flag to override the user load current value.
    bool mMalfOverrideCurrentFlag; /**< (--)  trick_chkpnt_io(**) override the current (malf) */

    /// @brief The override the current value for the user load. Default value set to zero.
    /// This value will override the actual current value of the user load through a
    /// malfunction.
    double mMalfOverrideCurrentValue;  /**< (amp)  trick_chkpnt_io(**) override the current value */

    /// @brief mode of the load could be OFF, ON, or STANDBY
    int mLoadOperMode;  /**< (--)  trick_chkpnt_io(**) opearion mode */

    double mInitialVoltage; /**< (V) trick_chkpnt_io(**) Initial input voltage to the user load from the power supply. */

    /// @brief Default Constructor
    UserLoadBaseInputData(const bool   lMalfOverrideCurrentFlag  = false,
                          const double lMalfOverrideCurrentValue = 0.0,
                          const int    loadOperMode              = LoadON,
                          const double initialVoltage            = 0.0);

    /// @brief Default Destructor
    virtual ~UserLoadBaseInputData();

    /// @brief Copy constructs this User Load Elect Input data.
    UserLoadBaseInputData(const UserLoadBaseInputData& that);

protected:

private:
    /// @brief Assignment operator unavailable since declared private and not implemented.
    UserLoadBaseInputData& operator =(const UserLoadBaseInputData&);
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    User Load Class of Electrical Subsystem
///
/// @details  The purpose of this class is to model user load of the electrical subsystem. This class is
///           is subclassed depending on the type of User Load.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UserLoadBase
{
    TS_MAKE_SIM_COMPATIBLE (UserLoadBase);

public:
    /// @name    Malfunction terms.
    /// @{
    /// @details Malfunction targets are public to allow access from the Trick events processor.
    bool   mMalfOverrideCurrentFlag;  /**< (--)  Flag to override the current value */
    double mMalfOverrideCurrentValue; /**< (amp) Override current value */
    bool   mMalfOverridePowerFlag;    /**< (--)  Flag to overwrite the power value  */
    double mMalfOverridePower;        /**< (W)   Overwrite power value  */
    bool   mMalfBlowFuse;             /**< (--)  Flag to blow the fuse. */
    /// @}

    /// @brief Flag to overwrite the voltage available. Only use when the RPC at this user load is open yet downstream load needs power
    bool mMagicPowerFlag;                /**< (--) flag to override voltage available */
    /// @brief The overwrite voltage value, set above the under voltage limit
    double mMagicPowerValue;             /**< (V)  value to override input voltage to */

    /// @brief Default Constructor
    UserLoadBase();

    /// @brief Default Destructor
    virtual ~UserLoadBase();

    /// @brief Updates the model
    virtual void step(double voltage);

    /// @brief Checks for blown fuse given the input voltage and returns if the blows.
    bool updateFuse(const double voltage);

    /// @brief get the load name
    const std::string getName();

    /// @brief returns the power valid flag - set by the step method when voltage is above
    /// the under voltage limit.
    bool getPowerValid() const;

    /// @brief returns actual total power of this user load object
    double getPower() const;

    /// @brief returns the equivalent resistance of this user load
    double getResistance() const;

    /// @brief returns calculated current of this user load object
    double getCurrent() const;

    /// @brief returns the ID of the switch card of which this user load is part of
    int getCardId() const;

    /// @brief returns the ID of this user load
    int getLoadSwitchId() const;

    /// @brief returns the load type of this user load (for now constant resistance or constant power)
    int getLoadType() const;

    /// @brief returns the load type of this user load (for now constant resistance or constant power)
    int getLoadOperMode() const;

    // @brief returns the over write flag - set by the input data
    bool getOverrideCurrentFlag() const;

    /// @brief returns whether the fuse is blown.
    bool isFuseBlown() const;

    /// @brief returns the current override value - set from the derived classes input data
    /// This value will override the actual current value of the user load through a malfunction.
    double getOverrideCurrentValue() const;

    /// @brief Sets and resets the current override malfunction.
    void setMalfOverrideCurrent(const bool flag = false, const double value = 0.0);

    /// @brief Sets and resets the power override malfunction.
    void setMalfOverridePower(const bool flag = false, const double value = 0.0);

    /// @brief Sets and resets the fuse blown malfunction.
    void setMalfBlowFuse(const bool flag = false);

    /// @brief Sets the load operating mode of this user load.
    void setLoadOperMode(const UserLoadMode mode);

    /// @brief Resets the fuse state to not blown.
    void resetFuse();

    /// @brief Returns whether this load is initialized.
    bool isInitialized() const;

    /// -- resistance value for the load assigned maximum
    static const double MAXIMUM_RESISTANCE; /**< (ohm)  trick_chkpnt_io(**) const value set later  */
    static const double MINIMUM_RESISTANCE; /**< (ohm)  trick_chkpnt_io(**) const value set later  */
    static const double DEFAULT_RESISTANCE; /**< (ohm)  trick_chkpnt_io(**) const value set later  */

protected:

    /// -- User Load name for error messages
    std::string mNameLoad;       /**< (--) trick_chkpnt_io(**) load name in string form for displays */
    std::string mPrettyNameLoad; /**< (--) trick_chkpnt_io(**) parsed version of load name for displays */

    /// @brief
    int mCardId; /**< (--) ID of the load switch card   */

    /// @brief user load ID
    int mLoadSwitchId; /**< (--) number of the individual load on the switch card  */


    /// @brief current passing through this load
    double mCurrent; /**< (amp) the current calculated for this load   */

    /// @brief total actual power of this user load
    double mActualPower; /**< (W) actual power calculated or derived from the load  */

    /// @brief mode of operation for the load enum values are LoadOFF/LoadON/LoadSTANDBY
    int mLoadOperMode; /**< (--) operating mode for the load  */

    /// @brief set to the enum value of the LOAD_TYPE for constant resistance or constant power
    int mUserLoadType; /**< (--) trick_chkpnt_io(**) enum value  */

    /// @brief Equivalent Resistance for this load
    double mEquivalentResistance; /**< (ohm) the equivalent resistance set during model update.  */

    /// @brief voltage value passed in to the step method. Client sends the voltage value
    double mVoltage; /**< (V) load voltage value  */

    /// @brief under voltage trip limit set point value
    double mUnderVoltageLimit; /**< (V) trick_chkpnt_io(**) minimum voltage at which this load trips */
    double mFuseCurrentLimit;  /**< (amp) trick_chkpnt_io(**) Current above which the fuse blows. */
    bool mFuseIsBlown; /**< (--) Flag for the state of the fuse, true is blown. */

    /// @brief Flag set to indicate the power is valid, above the under voltage limit
    bool mPowerValid; /**< (--) boolean for min voltage value */

    /// @brief Initialization status flag, T = good
    bool mInitFlag;   /**< (--) Load Init Flag */

    /// @brief UserLoadBase Initialization method
    void initialize(const UserLoadBaseConfigData &config,
            const UserLoadBaseInputData &input,
            std::vector<UserLoadBase *>&  networkLoads,
            const int cardId);
    // const int loadSwitchId);

    /// @brief Updates the state
    virtual void updateLoad() = 0;

    /// @brief validate the input data
    void  validate(const UserLoadBaseConfigData &config,
                   const UserLoadBaseInputData &input);

    /// @brief utility function to tokenize a string based on an arbitrary delimiter.
    void tokenize(std::vector<std::string>& theStringVector,
                  const std::string&        theString,
                  const std::string&        theDelimiter);

private:

    /// @brief Copy constructor unavailable since declared private and not implemented.
    UserLoadBase(const UserLoadBase& that);

    /// @brief Assignment operator unavailable since declared private and not implemented.
    UserLoadBase& operator =(const UserLoadBase&);

};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  return the name of the user load
/// @return   std::string
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::string UserLoadBase::getName() {
    return mNameLoad;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  return the power valid flag
/// @return   bool
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool UserLoadBase::getPowerValid() const {
    return mPowerValid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  return the actual power
/// @return   double
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UserLoadBase::getPower() const {
    return mActualPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  return the equivalent resistance
/// @return   double
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UserLoadBase::getResistance() const {
    return mEquivalentResistance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  return the current calculated for the load
/// @return   double
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UserLoadBase::getCurrent() const {
    return mCurrent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  return the load type Constant power or Constant Resistance type.
/// @return   int
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int UserLoadBase::getLoadType() const {
    return mUserLoadType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  return the switch card ID
/// @return   int
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int UserLoadBase::getCardId() const {
    return mCardId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  return the load ID
/// @return   int
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int UserLoadBase::getLoadSwitchId() const {
    return mLoadSwitchId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  return the operating mode OFF-ON-STANDBY
/// @return   int
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int UserLoadBase::getLoadOperMode() const {
    return mLoadOperMode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  return the override current flag
/// @return   bool
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool UserLoadBase::getOverrideCurrentFlag() const {
    return mMalfOverrideCurrentFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  return the override current value (amps)
/// @return  double
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UserLoadBase::getOverrideCurrentValue() const {
    return mMalfOverrideCurrentValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True if the fuse is blown.
///
/// @details  Returns the value of the mFuseIsBlown flag.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool UserLoadBase::isFuseBlown() const {
    return mFuseIsBlown;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    UserLoadMode (--) The desired mode to transition to.
/// @details  Sets the load operating mode to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void UserLoadBase::setLoadOperMode(const UserLoadMode mode) {
    mLoadOperMode = mode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the mFuseIsBlown variable to false.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void UserLoadBase::resetFuse() {
    mFuseIsBlown = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    none
/// @details  Returns mInitFlag
/// @return  bool
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool UserLoadBase::isInitialized() const {
    return mInitFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) The value of the malfunction activation flag.
///
/// @details  Sets mMalfBlowFuse equal to the given flag.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void UserLoadBase::setMalfBlowFuse(const bool flag) {
    mMalfBlowFuse = flag;
}

#endif

/// @}
