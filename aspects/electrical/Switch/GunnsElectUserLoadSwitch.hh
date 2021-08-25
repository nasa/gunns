#ifndef GunnsElectUserLoadSwitch_EXISTS
#define GunnsElectUserLoadSwitch_EXISTS

/**
@file
@brief    GUNNS Electrical User Load Switch Link declarations

@defgroup  TSM_GUNNS_ELECTRICAL_SWITCH_USER_LOAD    GUNNS Electrical User Load Switch Link
@ingroup   TSM_GUNNS_ELECTRICAL_SWITCH

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Electrical User Load Switch Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsElectUserLoadSwitch.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-05) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicConductor.hh"
#include "GunnsElectSwitchUtil.hh"
#include "aspects/electrical/resistive/GunnsShortUtil.hh"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical User Load Switch Configuration Data
///
/// @details  This provides a data structure for the Electrical User Load Switch configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectUserLoadSwitchConfigData : public GunnsBasicConductorConfigData
{
    public:
        GunnsElectSwitchUtilConfigData mSwitch;         /**< (--) trick_chkpnt_io(**) The switch config data. */
        double                         mLoadsPowerRefV; /**< (V)  trick_chkpnt_io(**) Optional reference voltage for user loads power. */
        /// @brief Default constructs this Electrical User Load Switch configuration data.
        GunnsElectUserLoadSwitchConfigData(const std::string& name,
                                           GunnsNodeList*     nodes,
                                           const double       switchResistance,
                                           const int          switchTripPriority);
        /// @brief Default destructs this Electrical User Load Switch configuration data.
        virtual ~GunnsElectUserLoadSwitchConfigData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectUserLoadSwitchConfigData(const GunnsElectUserLoadSwitchConfigData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectUserLoadSwitchConfigData& operator =(const GunnsElectUserLoadSwitchConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical User Load Switch Input Data
///
/// @details  This provides a data structure for the Electrical User Load Switch input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectUserLoadSwitchInputData : public GunnsBasicConductorInputData
{
    public:
        GunnsElectSwitchUtilInputData mSwitch;               /**< (--) trick_chkpnt_io(**) The switch input data. */
        GunnsShortUtilInputData       mShort;                /**< (--) trick_chkpnt_io(**) The electrical short model input data. */
        bool                          mLoadsOverrideActive;  /**< (--) trick_chkpnt_io(**) Initial state of the loads voltage override mode. */
        double                        mLoadsOverrideVoltage; /**< (V)  trick_chkpnt_io(**) Initial loads voltage override voltage. */
        /// @brief    Default constructs this Electrical User Load Switch input data.
        GunnsElectUserLoadSwitchInputData(const bool   malfBlockageFlag,
                                          const double malfBlockageValue,
                                          const bool   switchIsClosed,
                                          const double switchPosTripLimit,
                                          const double switchNegTripLimit,
                                          const bool   loadsOverrideActive  = false,
                                          const double loadsOverrideVoltage = 0.0);
        /// @brief    Default destructs this Electrical User Load Switch input data.
        virtual ~GunnsElectUserLoadSwitchInputData();

    private:
        /// @brief    Copy constructor unavailable since declared private and not implemented.
        GunnsElectUserLoadSwitchInputData(const GunnsElectUserLoadSwitchInputData& that);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsElectUserLoadSwitchInputData& operator =(const GunnsElectUserLoadSwitchInputData&);
};

// Forward declare pointer types.
class GunnsElectUserLoad;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical User Load Switch Class
///
/// @details  This conductive link includes a switch that powers zero or more user loads.  The user
///           loads are optional; when there are none, this reduces to something similar to the
///           SwitchElect link.  The user load objects exist elsewhere; this link only has a vector
///           of pointers to them.  Voltage drop across the switch's internal resistance is modeled,
///           and affects the input voltage to the loads.
///
///           This includes an electrical short model.  The short is parallel with the user loads,
///           between the switch output and Ground.  This is meant to model a short-circuit in a
///           load downstream of the switch.  A short in the switch itself can be modeled with
///           malfunctions and overrides in the switch model.
///
///           This link has a 'loads voltage override' mode, where the voltage between the output of
///           the switch and the input to the loads and downstream node can be overridden.  When
///           this mode is active, this link's switch is opened to disconnect the Port 0 and 1 nodes
///           and the Port 1 voltage is driven to the override voltage value with a potential source
///           effect from Ground to port 1.  The override voltage is also given as the input voltage
///           to the user loads.  In this mode, since there is no longer a conductance between Ports
///           0 and 1, the mMalfBlockage malfunction and the short model have no effect.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectUserLoadSwitch : public GunnsBasicConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectUserLoadSwitch);

    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        GunnsElectSwitchUtil mSwitch;                           /**< (--) The switch object. */
        GunnsShortUtil       mShort;                            /**< (--) The electrical short model. */
        /// @}
        /// @brief Default Electrical User Load Switch Constructor.
        GunnsElectUserLoadSwitch();
        /// @brief Default Electrical User Load Switch Destructor.
        virtual ~GunnsElectUserLoadSwitch();
        /// @brief Initialize method
        void initialize(const GunnsElectUserLoadSwitchConfigData& configData,
                        const GunnsElectUserLoadSwitchInputData&  inputData,
                        std::vector<GunnsBasicLink*>&             networkLinks,
                        const int                                 port0,
                        const int                                 port1);
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
        double                           mLoadsPowerRefV;       /**<    (V)     trick_chkpnt_io(**) Optional reference voltage for user loads power. */
        double                           mLoadsVoltage;         /**<    (V)     trick_chkpnt_io(**) Outlet voltage of switch, input to loads. */
        double                           mLoadsPower;           /**<    (W)     trick_chkpnt_io(**) Total power consumed by user loads. */
        bool                             mLoadsOverrideActive;  /**<    (--)                        Load voltage override mode is active. */
        double                           mLoadsOverrideVoltage; /**<    (V)                         Load voltage override voltage. */
        std::vector<GunnsElectUserLoad*> mUserLoads;            /**< ** (--)    trick_chkpnt_io(**) The user loads on the switch. */
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
        GunnsElectUserLoadSwitch(const GunnsElectUserLoadSwitch& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectUserLoadSwitch& operator =(const GunnsElectUserLoadSwitch& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Returns whether the link is currently non-linear.
///
/// @details  This returns true when the loads override mode is inactive, so that the switch can do
///           trip logic.  This returns false when the loads override mode is active since the
///           switch is unused.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectUserLoadSwitch::isNonLinear()
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
inline void GunnsElectUserLoadSwitch::minorStep(const double dt,
                                                const int    minorStep __attribute__((unused)))
{
    GunnsBasicConductor::step(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (V)  Total power consumed by the user loads.
///
/// @details  Returns the value of the mLoadsPower attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectUserLoadSwitch::getLoadsPower() const
{
    return mLoadsPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] active  (--) Override active flag.
/// @param[in] voltage (V)  Override voltage.
///
/// @details  Sets attributes mLoadsOverrideActive and mLoadsOverrideVoltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectUserLoadSwitch::setLoadsOverride(const bool active, const double voltage)
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
inline GunnsElectUserLoad* GunnsElectUserLoadSwitch::getUserLoad(const unsigned int index)
{
    if (index < mUserLoads.size()) {
        return mUserLoads.at(index);
    }
    return 0;
}

#endif
