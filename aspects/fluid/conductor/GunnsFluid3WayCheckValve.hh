#ifndef GunnsFluid3WayCheckValve_EXISTS
#define GunnsFluid3WayCheckValve_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_THREE_WAY_CHECK_VALVE    Three Way Check Valve Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Class for the GUNNS Three Check Way Valve.  This consists of a three way valve that calculates
   its own position based on the difference in pressure drop across path A and path B.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (This valve does not prevent back flow across either path. If both paths have a negative pressure
   drop, the valve will just retain its current position.)

LIBRARY DEPENDENCY:
- ((GunnsFluid3WayCheckValve.o))

PROGRAMMERS:
- ((Chris Brownschidle) (CACI) (2017-09) (Initial version))

@{
*/

#include "aspects/fluid/conductor/GunnsFluid3WayValve.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Three Way Check Valve Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Three Way Check Valve
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluid3WayCheckValveConfigData : public GunnsFluid3WayValveConfigData
{
    public:
        double        mRateLimit;          /**< (1/s)  trick_chkpnt_io(**) Position rate limit. */
        double        mOpenPressureA;      /**< (kPa)  trick_chkpnt_io(**) Pressure differential at which path A is fully open. */
        double        mOpenPressureB;      /**< (kPa)  trick_chkpnt_io(**) Pressure differential at which path B is fully open. */
        double        mMinPressureDiff;    /**< (kPa)  trick_chkpnt_io(**) Pressure differential range at which valve is stuck at half open. */
        /// @brief Default constructs this Three Way Check Valve configuration data.
        GunnsFluid3WayCheckValveConfigData(const std::string& name                  = "",
                                           GunnsNodeList*     nodes                 = 0,
                                           const bool         positionPathB         = false,
                                           const double       maxConductivityA      = 0.0,
                                           const double       maxConductivityB      = 0.0,
                                           const double       expansionScaleFactorA = 0.0,
                                           const double       expansionScaleFactorB = 0.0,
                                           const double       thermalLengthA        = 0.0,
                                           const double       thermalLengthB        = 0.0,
                                           const double       thermalDiameterA      = 0.0,
                                           const double       thermalDiameterB      = 0.0,
                                           const double       surfaceRoughnessA     = 0.0,
                                           const double       surfaceRoughnessB     = 0.0,
                                           const double       rateLimit             = 0.0,
                                           const double       openPresureA          = 0.0,
                                           const double       openPresureB          = 0.0,
                                           const double       minPressureDiff       = 0.0);
        /// @brief Copy constructs this Three Way Check Valve configuration data.
        GunnsFluid3WayCheckValveConfigData(const GunnsFluid3WayCheckValveConfigData& that);
        /// @brief Default destructs this Three Way Check Valve configuration data.
        virtual ~GunnsFluid3WayCheckValveConfigData();
    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluid3WayCheckValveConfigData& operator =(const GunnsFluid3WayCheckValveConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Three Way Check Valve Input Data
///
/// @details  This provides a data structure for the Three Way Check Valve input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluid3WayCheckValveInputData : public GunnsFluid3WayValveInputData
{
    public:
    bool          mMalfStuckFlag;   /**< (--)   Stuck at current position malfunction flag. */
    bool          mMalfFailToFlag;  /**< (--)   Fail to position malfunction flag. */
    double        mMalfFailToValue; /**< (--)   Fail to position malfunction value. */
        /// @brief Default constructs this Three Way Check Valve input data.
        GunnsFluid3WayCheckValveInputData(const double position         = 0.0,
                                          const double wallTemperatureA = 0.0,
                                          const double wallTemperatureB = 0.0,
                                          const bool   malfStuckFlag    = false,
                                          const bool   malfFailToFlag   = false,
                                          const double malfFailToValue  = 0.0);
        /// @brief Copy constructs this Three Way Check Valve input data.
        GunnsFluid3WayCheckValveInputData(const GunnsFluid3WayCheckValveInputData& that);
        /// @brief Default destructs this Three Way Check Valve input data.
        virtual ~GunnsFluid3WayCheckValveInputData();
    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluid3WayCheckValveInputData& operator =(const GunnsFluid3WayCheckValveInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Three Way Check Valve
///
/// @details  This class models the fluid aspect of a three-way check valve. It is identical to a
///           normal 3-way valve, but controls its own position based on the difference in pressure
///           drop across path A and B. When this difference is within the minimum pressure
///           differential range, the valve will stay at half open. The position follows this trend:
///           \verbatim
///                                valve position
///                                    1 -|         -----------
///                                       |        /
///                                       |       /
///                                       |      /
///                                       |     /
///                                       |    /
///                                    ___|___/
///                                   /0.5|   mMinPressureDiff
///                                  /    |
///                                 /     |
///                                /      |
///                               /       |
///                              /        |
///           ==================|------------------|------------- delta pressureA - delta pressureB
///                    mOpenPressureB            mOpenPressureA
///           \endverbatim
///          If valve position is relative to flow path B, the above graph is reflected across the
///          y-axis.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluid3WayCheckValve : public GunnsFluid3WayValve
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluid3WayCheckValve);

    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfStuckFlag;      /**< (--)   Stuck at current position malfunction flag. */
        bool   mMalfFailToFlag;     /**< (--)   Fail to position malfunction flag. */
        double mMalfFailToValue;    /**< (--)   Fail to position malfunction value. */
        /// @}

        /// @brief Default Constructor.
        GunnsFluid3WayCheckValve();
        /// @brief Default constructor.
        virtual ~GunnsFluid3WayCheckValve();
        /// @brief Initialize method.
        void initialize(const GunnsFluid3WayCheckValveConfigData& configData,
                        const GunnsFluid3WayCheckValveInputData&  inputData,
                        std::vector<GunnsBasicLink*>&             networkLinks,
                        const int                                 port0,
                        const int                                 port1,
                        const int                                 port2);
        /// @brief Updates the State of the Fluid Conductor.
        virtual void updateState(const double dt);
        /// @brief Sets and resets the stuck malfunction
        void setMalfStuck(const bool flag = false);
        /// @brief Sets and resets the fail to position malfunction
        void setMalfFailTo(const bool flag = false, const double value = 0.0);

    protected:
        double mRateLimit;          /**< (1/s)  trick_chkpnt_io(**) Position rate limit. */
        double mOpenPressureA;      /**< (kPa)  trick_chkpnt_io(**) Pressure differential at which path A is fully open. */
        double mOpenPressureB;      /**< (kPa)  trick_chkpnt_io(**) Pressure differential at which path B is fully open. */
        double mMinPressureDiff;    /**< (kPa)  trick_chkpnt_io(**) Pressure differential range at which valve is stuck at half open. */
        /// @brief Validates the initialization of this Gunns Three Way Check Valve.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @brief Calculates valve position
        double calculateValvePosition();
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluid3WayCheckValve(const GunnsFluid3WayCheckValve& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluid3WayCheckValve& operator =(const GunnsFluid3WayCheckValve& that);
};

/// @}

#endif
