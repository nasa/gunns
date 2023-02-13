#ifndef GunnsElectUserLoadSwitch2_EXISTS
#define GunnsElectUserLoadSwitch2_EXISTS

/**
@file     GunnsElectUserLosdSwitch2.hh
@brief    GUNNS Electrical User Load Switch Variant 2 Link declarations

@defgroup  TSM_GUNNS_ELECTRICAL_SWITCH_USER_LOAD_2    GUNNS Electrical User Load Switch Variant 2 Link
@ingroup   TSM_GUNNS_ELECTRICAL_SWITCH

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Electrical User Load Switch Variant 2 Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsElectUserLoadSwitch2.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-01) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicConductor.hh"
#include "GunnsElectSwitchUtil2.hh"
#include "aspects/electrical/resistive/GunnsShortUtil.hh"
#include "common/sensors/SensorAnalog.hh"
#include <vector>

// Forward-declaration of types.
class GunnsElectUserLoadSwitch2ConfigData;
class GunnsElectUserLoadSwitch2InputData;
class GunnsElectUserLoad;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical User Load Switch Class, Variant 2
//
/// @details  This conductive link includes a switch that powers zero or more user loads.  This is
///           based on the GunnsElectUserLoadSwitch link, with these major differences:
///           - it uses the GunnsElectSwitchUtil2 switch model, which has its own differences from
///             the switch model used in GunnsElectUserLoadSwitch,
///           - it has built-in sensor models for the switch through-current, input & output
///             voltages that feed the trip logic in the switch model.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectUserLoadSwitch2 : public GunnsBasicConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectUserLoadSwitch2);

    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        GunnsElectSwitchUtil2 mSwitch;                           /**< (1) The switch object. */
        SensorAnalog          mCurrentSensor;                    /**< (1) The switch through-current sensor. */
        SensorAnalog          mInputVoltageSensor;               /**< (1) The switch input voltage sensor. */
        SensorAnalog          mOutputVoltageSensor;              /**< (1) The switch output voltage sensor. */
        GunnsShortUtil        mShort;                            /**< (1) The electrical short model. */
        /// @}
        /// @brief Default Electrical User Load Switch Constructor.
        GunnsElectUserLoadSwitch2();
        /// @brief Default Electrical User Load Switch Destructor.
        virtual ~GunnsElectUserLoadSwitch2();
        /// @brief Initialize method
        void initialize(const GunnsElectUserLoadSwitch2ConfigData& configData,
                        const GunnsElectUserLoadSwitch2InputData&  inputData,
                        std::vector<GunnsBasicLink*>&              networkLinks,
                        const int                                  port0,
                        const int                                  port1);
        /// @brief Updates the link contributions to the system of equations.
        virtual void step(const double dt);
        /// @brief Updates the link effective conductance.
        virtual void updateState(const double dt);
        /// @brief Puts the given user load object on the switch.
        void         addUserLoad(GunnsElectUserLoad* load);
        /// @brief Tells the network solver whether this is a non-linear link.
        virtual bool isNonLinear();
        /// @brief Minor Step method for the non-linear link.
        virtual void minorStep(const double dt, const int minorStep);
        /// @brief  Returns whether the link confirms, rejects, or delays the network solution.
        virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                         const int absoluteStep);
        /// @brief  Returns the total power consumed by the user loads.
        double       getLoadsPower() const;
        /// @brief  Sets the loads voltage override.
        void         setLoadsOverride(const bool active = false, const double voltage = 0.0);
        /// @brief  Returns the user load object at the given index in the list of user loads.
        GunnsElectUserLoad* getUserLoad(const unsigned int index);

    protected:
        double                           mLoadsVoltage;         /**<    (V)     trick_chkpnt_io(**) Outlet voltage of switch, input to loads. */
        double                           mLoadsPower;           /**<    (W)     trick_chkpnt_io(**) Total power consumed by user loads. */
        bool                             mLoadsOverrideActive;  /**<    (1)                         Load voltage override mode is active. */
        double                           mLoadsOverrideVoltage; /**<    (V)                         Load voltage override voltage. */
        std::vector<GunnsElectUserLoad*> mUserLoads;            /**< ** (1)     trick_chkpnt_io(**) The user loads on the switch. */
        static const double              mLoadsOverrideG;       /**< ** (1/ohm) trick_chkpnt_io(**) Default conductance of the ideal loads override potential source. */
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;
        /// @brief Updates the admittance matrix of the link.
        virtual void buildConductance();
        /// @brief Updates the source vector of the link.
        virtual void buildSource();
        /// @brief Computes flux across the link.
        virtual void computeFlux();
        /// @brief Computes power for the aspect-specific implementation.
        virtual void computePower();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectUserLoadSwitch2(const GunnsElectUserLoadSwitch2& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectUserLoadSwitch2& operator =(const GunnsElectUserLoadSwitch2& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical User Load Switch Configuration Data
///
/// @details  This provides a data structure for the Electrical User Load Switch configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectUserLoadSwitch2ConfigData : public GunnsBasicConductorConfigData
{
    public:
        GunnsElectSwitchUtil2ConfigData mSwitch;              /**< (1) trick_chkpnt_io(**) The switch config data. */
        SensorAnalogConfigData          mCurrentSensor;       /**< (1) trick_chkpnt_io(**) The switch through-current sensor config data. */
        SensorAnalogConfigData          mInputVoltageSensor;  /**< (1) trick_chkpnt_io(**) The switch input voltage sensor config data. */
        SensorAnalogConfigData          mOutputVoltageSensor; /**< (1) trick_chkpnt_io(**) The switch output voltage sensor config data. */
        /// @brief Default constructs this Electrical User Load Switch configuration data.
        GunnsElectUserLoadSwitch2ConfigData(const std::string& name,
                                            GunnsNodeList*     nodes,
                                            const double       switchResistance,
                                            const int          switchTripPriority,
                                            const float        curentSensorMinRange,
                                            const float        curentSensorMaxRange,
                                            const float        inputVoltageSensorMinRange,
                                            const float        inputVoltageSensorMaxRange,
                                            const float        outputVoltageSensorMinRange,
                                            const float        outputVoltageSensorMaxRange);
        /// @brief Default destructs this Electrical User Load Switch configuration data.
        virtual ~GunnsElectUserLoadSwitch2ConfigData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectUserLoadSwitch2ConfigData(const GunnsElectUserLoadSwitch2ConfigData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectUserLoadSwitch2ConfigData& operator =(const GunnsElectUserLoadSwitch2ConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical User Load Switch Input Data
///
/// @details  This provides a data structure for the Electrical User Load Switch input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectUserLoadSwitch2InputData : public GunnsBasicConductorInputData
{
    public:
        GunnsElectSwitchUtil2InputData mSwitch;               /**< (1) trick_chkpnt_io(**) The switch input data. */
        SensorAnalogInputData          mCurrentSensor;        /**< (1) trick_chkpnt_io(**) The switch through-current sensor input data. */
        SensorAnalogInputData          mInputVoltageSensor;   /**< (1) trick_chkpnt_io(**) The switch input voltage sensor input data. */
        SensorAnalogInputData          mOutputVoltageSensor;  /**< (1) trick_chkpnt_io(**) The switch output voltage sensor input data. */
        GunnsShortUtilInputData        mShort;                /**< (1) trick_chkpnt_io(**) The electrical short model input data. */
        bool                           mLoadsOverrideActive;  /**< (1) trick_chkpnt_io(**) Initial state of the loads voltage override mode. */
        double                         mLoadsOverrideVoltage; /**< (V) trick_chkpnt_io(**) Initial loads voltage override voltage. */
        /// @brief    Default constructs this Electrical User Load Switch input data.
        GunnsElectUserLoadSwitch2InputData(const bool   malfBlockageFlag,
                                           const double malfBlockageValue,
                                           const bool   switchIsClosed,
                                           const double inputUnderVoltageTripLimit,
                                           const double inputUnderVoltageTripReset,
                                           const double inputOverVoltageTripLimit,
                                           const double inputOverVoltageTripReset,
                                           const double switchPosTripLimit,
                                           const double switchNegTripLimit,
                                           const bool   loadsOverrideActive  = false,
                                           const double loadsOverrideVoltage = 0.0);
        /// @brief    Default destructs this Electrical User Load Switch input data.
        virtual ~GunnsElectUserLoadSwitch2InputData();

    private:
        /// @brief    Copy constructor unavailable since declared private and not implemented.
        GunnsElectUserLoadSwitch2InputData(const GunnsElectUserLoadSwitch2InputData& that);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsElectUserLoadSwitch2InputData& operator =(const GunnsElectUserLoadSwitch2InputData&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Returns whether the link is currently non-linear.
///
/// @details  This returns true when the loads override mode is inactive, so that the switch can do
///           trip logic.  This returns false when the loads override mode is active since the
///           switch is unused.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectUserLoadSwitch2::isNonLinear()
{
    return (not mLoadsOverrideActive);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Frame delta time
/// @param[in] minorStep (--) The minor step number that the network is on
///
/// @details  For each network minor step, this simply passes execution on to the base class step
///           method.  This allows constant-power user loads to adjust the link admittance as the
///           network converges on a solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectUserLoadSwitch2::minorStep(const double dt,
                                                 const int    minorStep __attribute__((unused)))
{
    GunnsBasicConductor::step(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (V)  Total power consumed by the user loads.
///
/// @details  Returns the value of the mLoadsPower attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectUserLoadSwitch2::getLoadsPower() const
{
    return mLoadsPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] active  (--) Override active flag.
/// @param[in] voltage (V)  Override voltage.
///
/// @details  Sets attributes mLoadsOverrideActive and mLoadsOverrideVoltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectUserLoadSwitch2::setLoadsOverride(const bool active, const double voltage)
{
    mLoadsOverrideActive  = active;
    mLoadsOverrideVoltage = voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] index  (--)  Index of the desired user load in the list of user loads.
///
/// @returns GunnsElectUserLoad* (--) Pointer to the user load object, or zero.
///
/// @details  Returns the address of the user load object at the given index in the mUserLoads
///           vector, or zero.
///
/// @note  Returns zero if the given index is invalid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsElectUserLoad* GunnsElectUserLoadSwitch2::getUserLoad(const unsigned int index)
{
    if (index < mUserLoads.size()) {
        return mUserLoads.at(index);
    }
    return 0;
}

#endif
