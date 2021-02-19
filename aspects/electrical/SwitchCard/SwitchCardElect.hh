#ifndef SwitchCardElect_EXISTS
#define SwitchCardElect_EXISTS

#define MAXNUMSWITCHES 30

/// @defgroup  TSM_GUNNS_ELECTRICAL_SWITCH_CARD_LINK   SwitchCardElect Model
/// @ingroup   TSM_GUNNS_ELECTRICAL_SWITCH_CARD
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @{
/*********************** TRICK HEADER *************************************************************

 PURPOSE:
 (The SwitchCardElect class is the electrical aspect of a generic 'box of switches' to distribute
  power.  It can have a configurable number of switches, with varying trip set-points.)

 REQUIREMENTS:
 (none)

 REFERENCE:
 (none)

 ASSUMPTIONS AND LIMITATIONS:
 (none)

 LIBRARY DEPENDENCY:
 (
 (SwitchCardElect.o)
 )

 PROGRAMMERS:
 (
 (Minh Tran) (L-3) (Initial Prototype)
 (Philip Trottman) (USA) (Updates for ISS RPCM) (Feb 2012)
 )
 **************************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////
/// There are two type of switches: TwoPortSwitch and LoadSwitch
/// TwoPortSwitch: both side of switch are nodes in the network.
/// LoadSwitch: only one side of the switch is a node.  The other side is connected to a userload.
///  DCSU/MBSU: has all TwoPortSwitches
///  RPCM:
///    - RPCM has all switches that power useLoads (LoadSwitch)
///    - RPCM has some switches that power userLoad (LoadSwitch) and some switches that power other RPCM (TwoPortSwitch)
///    -
/// All switches(1...n) are connected to the common bus (PORT0)
/// mNumPorts = mNumTwoPortSwitches + 1
/// PORT0: common bus node .
/// PORT1: second node of TwoPortSwitch 1
/// PORT2: second node of TwoPortSwitch 2
/// ...
/// PORT(NumTwoPortSwitches): second node of TwoPortSwitch (NumTwoPortSwitches)
/// ...
/// BusVoltage is the common Bus(PORT0) voltage.
/// ...
/// Switch current flow
///     TwoPortSwitch current:
///           positive : from common bus to the other port of switch
///           negative : to common bus from the other port of switch
///     LoadSwitch current:  always positive.
/////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include "core/GunnsBasicLink.hh"
#include "aspects/electrical/UserLoad/UserLoadBase.hh"
#include "aspects/electrical/Switch/Switch.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "common/sensors/SensorAnalog.hh"
#include "aspects/electrical/IPS/GunnsElectIps.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    SwitchCardElect Config Data
/// @details  The  purpose of this class is to provide a data structure for the SwitchCardElect
///           Config data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class SwitchCardElectConfigData : public GunnsBasicLinkConfigData {
  public:

    //std::string mSwitchName;                                      /**< (--) trick_chkpnt_io(**) name of this instance*/
    int mNumSwitches;                                       /**< (--) trick_chkpnt_io(**) Number of Switches total */
    int mNumTwoPortSwitches;                                /**< (--) trick_chkpnt_io(**) Number of switches that feed something other than userLoads */
    double mDefaultPowerSupplyLoad;                         /**< (--) trick_chkpnt_io(**) Load for power supply of the device itself */
    SwitchConfigData *mSwitchConfigData;                    /**< (--) trick_chkpnt_io(**) Switches/Switch Config Data */
    int* mPorts;                                            /**< (--) trick_chkpnt_io(**) Mapping for two port switches */
    const SensorAnalogConfigData* mInputVoltSensorConfigData;      /**< (--) trick_chkpnt_io(**) input voltage sensor config data*/
    const SensorAnalogConfigData* mInputCurrentSensorConfigData;   /**< (--) trick_chkpnt_io(**) input current sensor config data*/
    const SensorAnalogConfigData* mSwitchVoltSensorConfigData;     /**< (--) trick_chkpnt_io(**) voltage sensor config data for individual switches */
    const SensorAnalogConfigData* mSwitchCurrentSensorConfigData;  /**< (--) trick_chkpnt_io(**) current sensor config data for individual switches */
    double mUvTripVoltage;                                  /**< (V)  trick_chkpnt_io(**) minimum voltage for undervolt trip (if applicable) */
    double mOvTripVoltage;                                  /**< (V)  trick_chkpnt_io(**) maximum voltage for over-volt switch trip (if enabled) */
    const GunnsElectIps* mPowerSupply;                      /**< (--)  trick_chkpnt_io(**) pointer for instance of Internal Power Supply class (for this switchcard) */

    /// @brief Default SwitchCardElect Configuration Data Constructor
    SwitchCardElectConfigData(const std::string name  = "Unnamed Link",
            GunnsNodeList* nodes  = 0,
            const int numSwitches = 0,
            const int numTwoPortSwitches = 0,
            const double powerSupplyLoad = 0.0,
            const SensorAnalogConfigData* inputVoltSensorConfigData = 0,
            const SensorAnalogConfigData* inputCurrentSensorConfigData = 0,
            const SensorAnalogConfigData* switchVoltSensorConfigData = 0,
            const SensorAnalogConfigData* switchCurrentSensorConfigData = 0,
            const double uvTripVoltage = 0.0,
            const double ovTripVoltage = 0.0,
            const GunnsElectIps* powerSupply = 0);

    /// @brief Default SwitchCardElect Configuration Data Destructor
    virtual ~SwitchCardElectConfigData();

    /// @brief SwitchCardElect Configuration Data Initialize
    void initData(const std::string name,
            const int numSwitches,
            const int numTwoPortSwitches,
            const double powerSupplyLoad);


    /// @brief Frees dynamic memory.
    void cleanup();

  private:
    /// @brief  Operator = is not available since declared private and not implemented.
    SwitchCardElectConfigData& operator =(const SwitchCardElectConfigData&);

    /// @brief Copy constructor not available since declared private and not implemented.
    SwitchCardElectConfigData(const SwitchCardElectConfigData&);
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Input Data
/// @details  The  purpose of this class is to provide a data structure for the SwitchCardElect
///           input data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class SwitchCardElectInputData:public GunnsBasicLinkInputData {
  public:
    int mNumSwitches;                                            /**< (--)  trick_chkpnt_io(**) Number of Switches total */
    SwitchInputData *mSwitchInputData;                           /**< (--) trick_chkpnt_io(**) Switch Input Data */
    const SensorAnalogInputData* mInputVoltSensorInputData;      /**< (--) trick_chkpnt_io(**) input voltage sensor input data */
    const SensorAnalogInputData* mInputCurrentSensorInputData;   /**< (--) trick_chkpnt_io(**) input current sensor input data */
    const SensorAnalogInputData* mSwitchVoltSensorInputData;     /**< (--) trick_chkpnt_io(**) voltage sensor input data for all switches */
    const SensorAnalogInputData* mSwitchCurrentSensorInputData;  /**< (--) trick_chkpnt_io(**) current sensor input data for all switches */
    double mMinInputVoltage;                                     /**< (V)   trick_chkpnt_io(**) minimum voltage for switchcard power supply */
    double mMaxConductance;                                      /**< (--)   trick_chkpnt_io(**) maximum conductance through the switchcard */

    /// @brief Default SwitchCardElect Input Data Constructor
    SwitchCardElectInputData(const bool   malfBlockageFlag  = false,
            const double malfBlockageValue = 0.0,
            const int numSwitches = 1,
            const SensorAnalogInputData* inputVoltSensorInputData = 0,
            const SensorAnalogInputData* inputCurrentSensorInputData = 0,
            const SensorAnalogInputData* switchVoltSensorInputData = 0,
            const SensorAnalogInputData* switchCurrentSensorInputData = 0,
            const double minInputVoltage = 70.0,
            const double maxConductance = 1.3E5);      // equivalent of 1 foot of 0/0 gauge wire, valid for short circuit

    /// @brief Default SwitchCardElect Input DataDestructor
    virtual ~SwitchCardElectInputData();

    /// @brief SwitchCardElect Input Data Initialize
    void init(const int numSwitches,
            const double minInputVoltage,
            const double maxConductance);

    /// @brief Copy constructs Load Switch Input Data.
    SwitchCardElectInputData(const SwitchCardElectInputData& that);

    /// @brief Frees dynamic memory.
    void cleanup();

  private:
    static std::size_t mCounter; /**< ** (--) trick_chkpnt_io(**) Instance count for unique naming */
    /// @brief  Operator = is not available since declared private and not implemented.
    SwitchCardElectInputData& operator =(const SwitchCardElectInputData&);
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Load Switches Elect class
/// @details  The  purpose of this class is to provide a data structure for the SwitchCardElect
/////////////////////////////////////////////////////////////////////////////////////////////////
class SwitchCardElect: public GunnsBasicLink {
    TS_MAKE_SIM_COMPATIBLE(SwitchCardElect);

  public:
    /// @name    Embedded objects.
    /// @{
    /// @details Embedded objects are public to allow access from the Trick events processor.
    Switch *mSwitch;                          /**< (--) trick_chkpnt_io(**) Switch instances */
    UserLoadBase **mLoad;                     /**< (--) trick_chkpnt_io(**) Pointer to userLoads connected to switches */
    SensorAnalog mInputVoltSensor;            /**< (--)                     input voltage sensor object*/
    SensorAnalog mInputCurrentSensor;         /**< (--)                     input current sensor object*/
    SensorAnalog *mSwitchVoltSensors;         /**< (--) trick_chkpnt_io(**) individual switch volt sensor objects*/
    SensorAnalog *mSwitchCurrentSensors;      /**< (--) trick_chkpnt_io(**) individual switch current sensor objects*/
    GunnsElectIps *mPowerSupply;              /**< (--) trick_chkpnt_io(**) pointer to power supply instance, if applicable */

    bool mMagicPowerFlag;                     /**< (--) flag to override voltage available */
    double mMagicPowerValue;                  /**< (V)  value to override input voltage to */
    double mMagicPowerAdmittance;             /**< (1/ohm)  value to set port 0 admittance to when magic power is active */
    bool mMagicPowerFlagResetFromOverride;    /**< (--) flag reset the override voltage available flag*/
    /// @}

    /// @name    Malfunction terms.
    /// @{
    /// @details Malfunction targets are public to allow access from the Trick events processor.
    bool mMalfPowerSupplyFailFlag;           /**< (--) flag for power supply failure malfunction*/
    bool mMalfBusFault;                      /**< (--) Malf Bus Fault */
    double mMalfBusFaultValue;               /**< (amp) Malf Bus Fault Value */
    bool mLastBusFault;                      /**< (--) Malf Bus Fault */
    bool mMalfADCFailHigh;                   /**< (--) Malf to fail Analog/Digital converter with all sensed values high */
    bool mMalfADCFailLow;                    /**< (--) Malf to fail Analog/Digital converter with all sensed values low  */

    /// @}

    /// @brief Default SwitchCardElect Constructor, deprecated due to obsolescence, includes non-generic features and needs a redesign.
    SwitchCardElect() __attribute__ ((deprecated));

    /// @brief Default SwitchCardElect Destructor
    virtual ~SwitchCardElect();

    /// @brief Initialize method as Gunns network links
    virtual void initialize(SwitchCardElectConfigData& configData,
                            SwitchCardElectInputData& inputData,
                            std::vector<GunnsBasicLink*>& networkLinks,
                            UserLoadBase ** loads,
                            const int*  ports);

    /// @brief Step method for updating the link
    void step(const double dt);

    /// @brief Minor step method for non-linear
    void minorStep(const double dt, const int minorStep);

    /// @brief Method for computing the flows across the Link
    void computeFlows(const double dt);

    /// @brief Returns TRUE if the link is non-linear
    bool isNonLinear();

    /// @brief Returns TRUE if admittance matrix needs updating
    bool needAdmittanceUpdate();

    /// @brief Method to clear an input under-volt trip flag
    void clearUndervoltTrip();

    /// @brief Method for calculating trip staging:
    /// @note  Return DELAY if not this switch's turn to trip.  If it is this switch's turn return
    ///        REJECT if switch has tripped open, CONFIRM if trip condition has been resolved in a
    ///        previous step (lower priority switch has already tripped, eliminating the trip
    ///        condition at this level).  Return CONFIRM if it is past our turn.
    GunnsBasicLink::SolutionResult confirmSolutionAcceptable(const int convergedStep, const int absoluteStep);

    void setSwitchCommandedClosed(const int switchNumber, const bool switchCommandedClosed);
    void setAllSwitchCommandedClosed(const bool *switchCommandedClosed);

    inline double getBusVoltage() const {
        return mBusVoltage;
    }

//////////////////////////////////////////////////////////////////////////////
/// @brief  a function to return the sensed current value for any switch
/// @param[in]   int   num   Which switch to get the value for
/// @return      double      the sensed current out for the chosen switch
//////////////////////////////////////////////////////////////////////////////
    inline double getSwitchCurrentSensed(const int num){
        if(num < 0 || num > mNumSwitches) {
            return 0.0;
        } else {
            return mTlmSensedSwitchCurrent[num];
        }

    }

    //////////////////////////////////////////////////////////////////////////////
    /// @brief  a function to return the actual current value for any switch
    /// @param[in]   int   num   Which switch to get the value for
    /// @return      double      the actual current out for the chosen switch
    //////////////////////////////////////////////////////////////////////////////
    inline double getSwitchCurrentActual(const int num) const {
        if(num < 0 || num > mNumSwitches) {
            return 0.0;
        } else {
            return mSwitchCurrent[num];
        }
    }

    inline bool getSwitchIsClosed(const int whichSwitch) const {
        return mTlmActualSwitchPosition[whichSwitch];
    }

    inline double* getSwitchOutputVolt() const{
        return mTlmSwitchVolts;
    }
    inline bool* getUserLoadPowerValid() const{
        return mUserLoadPowerValid;
    }

    /// @brief Sets and resets the power supply fail malfunction.
    void setMalfPowerSupplyFail(const bool flag = false);

    double getAdmittanceValue(const int value) const {
        return mAdmittanceMatrix[value];
    }

    void catchDebug();


  protected:
    /// @brief  Allocate arrays based on the number of switches
    void allocateArrays();

    /// @brief  Deletes allocated memory objects
    void cleanup();

    /// @brief Virtual method for derived links to perform their restart functions.
    virtual void restartModel();

    bool mVerbose;                            /**< (--) flag for reporting extra debug data */
    double mDeltaTime;                        /**< (--) hold delta time from step function for use in confirmSolutionAcceptable */
    double mMinVoltage;                       /**< (V)  minimum input voltage for the device to turn on */
    double mLoadChangeTolerance;              /**< (--) threshold for how much the conductance of a switch load has to change before bothering to re-calculate the GUNNS network */
    int mNumSwitches;                         /**< *o (--) trick_chkpnt_io(**) Number of switch instances to create */
    int mNumTwoPortSwitches;                  /**< (--) trick_chkpnt_io(**) Number of switches that feed other SwitchCards */
    int mNumLoadSwitches;                     /**< (--) trick_chkpnt_io(**) Number of switches that feed userLoads */
    double mBusVoltage;                       /**< (V)  Bus Voltage */
    double *mTwoPortSwitchActiveConductance;  /**< (--) trick_chkpnt_io(**) Total conductance of all two port switches */
    double mTotalCPowerLoadsPower;            /**< (W)  Total Power of all Constant Power Loads */
    double mInputCurrent;                     /**< (amp) Input current into the switchCard */
    double mInputVoltage;                     /**< (V)  input voltage, highest value from all inputs */
    double mInputPowerUsed;                   /**< (W) Sum of all power loads */
    double mDefaultPowerSupplyLoad;           /**< (W) trick_chkpnt_io(**) Watt load for device power supply */
    double mPowerSupplyLoad;                  /**< (W) Watt load for device power supply */
    double mPowerSupplyConductance;           /**< (ohm) Resistance value to apply power supply load on the network */
    double mResLoadsConductance;              /**< (--) Total Conductance of resistive Loads */
    double mCPowerLoadsConductance;           /**< (--) Total Conductance of constant power loads */
    double mActiveResLoadsConductance;        /**< (--) Active conductance of Resistive Loads */
    double mActiveCPowerLoadsConductance;     /**< (--) Active conductance of Constant Power Loads */
    double mMaxConductance;                   /**< (--) Maximum allowed conductance */
    double mMinConductance;                   /**< (--) trick_chkpnt_io(**) Minimum allowed conductance */
    double mMaxResistance;                    /**< (ohm) trick_chkpnt_io(**) inverse of mMinConductance, used for sanity check on resistive user loads */
    bool mIsMinorStep;                        /**< (--) Flag for whether this is a major or minor step */
    bool mAnySwitchTripped;                   /**< (--) Flag that a switch in the box is tripped */
    bool mAnySwitchWaitingToTrip;             /**< (--) Flag that a switch needs to trip but can't yet due to trip priority */
    int mCurrentMinorStep;                    /**< (--) Current minor step */
    bool mNonLinear;                          /**< (--) Flag for whether this is a non-linear link */
    bool mUndervoltFdirActive;                /**< (--) Flag for input under-voltage FDIR, opens switches when input < 105 volts */
    double mUvTripVoltage;                    /**< (V)  trick_chkpnt_io(**) minimum voltage for under-volt trip (for ISS RPCMs) */
    bool mUndervoltTrip;                      /**< (--) Flag for whether an under-volt trip is occurring */
    bool mFdirTrip;                           /**< (--) Flag for ISS DCSU RBI 1 FDIR trip has occurred */
    bool mClearUndervoltTrip;                 /**< (--) Flag to clear an input under-volt trip flag */
    bool mOvervoltFdirActive[MAXNUMSWITCHES];  /**< (--) Flag for switch over-voltage FDIR, when flag true, opens the switch when input > specified value */
    double mOvTripVoltage;                    /**< (V)  trick_chkpnt_io(**) voltage level to open the switches */
    bool mOvervoltTrip[MAXNUMSWITCHES];       /**< (--) Flag for whether an over-volt trip is occurring */
    bool mClearOvervoltTrip;                  /**< (--) Flag to clear an switch over-volt flag */

    /// @brief  Data going out of the model to the signal aspect
    bool mTlmPowerSupplyValid;                       /**< (--) Flag for whether the power supply is functioning*/
    bool mTlmActualSwitchPosition[MAXNUMSWITCHES];   /**< (--) array for switch position telemetry to signal aspect*/
    bool mTlmSensedSwitchPosition[MAXNUMSWITCHES];   /**< (--) array for sensed switch position telemetry, if implemented*/
    bool mTlmPosTrip[MAXNUMSWITCHES];                /**< (--) array for switch positive trip telemetry to signal aspect*/
    bool mTlmNegTrip[MAXNUMSWITCHES];                /**< (--) array for switch negative trip telemetry to signal aspect*/
    bool mTlmUndervoltTrip[MAXNUMSWITCHES];          /**< (--) array for switch under-volt trip telemetry to signal aspect*/
    bool mTlmOvervoltTrip[MAXNUMSWITCHES];           /**< (--) array for switch over-volt trip telemetry to signal aspect */
    double mTlmSensedBusVolt;                        /**< (V)  switchCard input voltage telemetry to signal aspect*/
    double mTlmSensedInputCurrent;                   /**< (amp) switchCard input current telemetry to signal aspect*/
    double mTlmSensedSwitchVolts[MAXNUMSWITCHES];    /**< (V)   Individual Switch sensed output voltages*/
    double mTlmSensedSwitchCurrent[MAXNUMSWITCHES];  /**< (amp) Individual Switch sensed current values */
    double mSwitchCurrent[MAXNUMSWITCHES];           /**< (amp) Individual Switch actual current values */

    double *mTlmSwitchVolts;    /**< (--) trick_chkpnt_io(**) Individual Switch output voltages*/
    double *mTlmSwitchCurrent;  /**< (--) trick_chkpnt_io(**) Individual Switch current values*/
    double *mTlmSwitchPower;    /**< (--) trick_chkpnt_io(**) Individual Switch Power values*/
    bool *mUserLoadPowerValid;  /**< (--) trick_chkpnt_io(**) UserLoadPowerValid*/

    /// @brief  Data going out to thermal aspect
    double mThermWasteHeat;              /**< (--) parasitic/waste power heat value*/
    double mThermalTuning;               /**< (--) tuning factor for waste heat that comes from bus faults */

    SwitchCardElect(const SwitchCardElect& that);
    SwitchCardElect& operator =(const SwitchCardElect& that);
};
/// @}
#endif
