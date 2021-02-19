#ifndef GunnsFluidCheckValve_EXISTS
#define GunnsFluidCheckValve_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_CHECK_VALVE    Check Valve Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Check Valve link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidCheckValve.o))

PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Update) (2011-09))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

#include "GunnsFluidValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Check Valve Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Check Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidCheckValveConfigData : public GunnsFluidValveConfigData
{
    public:
        double        mRateLimit;          /**< (1/s)  trick_chkpnt_io(**) Position rate limit for this Check Valve. */
        double        mClosePressure;      /**< (kPa)  trick_chkpnt_io(**) Delta pressure at which this Check Valve is fully closed. */
        double        mOpenPressure;       /**< (kPa)  trick_chkpnt_io(**) Delta pressure at which this Check Valve is fully opened. */
        /// @brief    Default constructs this Check Valve configuration data.
        GunnsFluidCheckValveConfigData(const std::string&        name                 = "",
                                       GunnsNodeList*            nodes                = 0,
                                       const double              maxConductivity      = 0.0,
                                       const double              expansionScaleFactor = 0.0,
                                       const double              thermalLength        = 0.0,
                                       const double              thermalDiameter      = 0.0,
                                       const double              surfaceRoughness     = 0.0,
                                       const double              rateLimit            = 0.0,
                                       const double              closePressure        = 0.0,
                                       const double              openPressure         = 0.0);
        /// @brief    Copy constructs this Check Valve configuration data.
        GunnsFluidCheckValveConfigData(const GunnsFluidCheckValveConfigData& that);
        /// @brief    Default destructs this Check Valve configuration data.
        virtual ~GunnsFluidCheckValveConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidCheckValveConfigData& operator =(const GunnsFluidCheckValveConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Check Valve Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Check Valve link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidCheckValveInputData : public GunnsFluidValveInputData
{
    public:
        bool          mMalfStuckFlag;   /**< (--)   Stuck at current position malfunction flag. */
        bool          mMalfFailToFlag;  /**< (--)   Fail to position malfunction flag. */
        double        mMalfFailToValue; /**< (--)   Fail to position malfunction value. */
        /// @brief    Default constructs this Check Valve input data.
        GunnsFluidCheckValveInputData(const bool   malfBlockageFlag  = false,
                                      const double malfBlockageValue = 0.0,
                                      const double position          = 0.0,
                                      const bool   malfLeakThruFlag  = false,
                                      const double malfLeakThruValue = 0.0,
                                      const double wallTemperature   = 0.0,
                                      const bool   malfStuckFlag     = false,
                                      const bool   malfFailToFlag    = false,
                                      const double malfFailToValue   = 0.0);
        /// @brief    Copy constructs this Check Valve input data.
        GunnsFluidCheckValveInputData(const GunnsFluidCheckValveInputData& that);
        /// @brief    Default destructs this Check Valve input data.
        virtual ~GunnsFluidCheckValveInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidCheckValveInputData& operator =(const GunnsFluidCheckValveInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Check Valve Link Model
///
/// @details  The GUNNS Fluid Check Valve link model restricts flow to a single direction.
///           This valve transitions between fully open and fully closed based on the delta pressure
///           across the valve. The open pressure must be no less than the close pressure. There is
///           no external (automatic or manual) control.
///           \verbatim
///                                valve position
///                                    1 -|         -----------------
///                                       |        /
///                                       |       /
///                                       |      /
///                                       |     /
///                                       |    /
///                                       |   /
///                                       |  /
///                                       | /
///                                       |/
///                                       /
///                                      /|
///                                     / |
///                                    /  |
///                                   /   |
///            ======================|----------|-------------------- delta pressure
///                    mClosePressure            mOpenPressure
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidCheckValve : public GunnsFluidValve
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidCheckValve);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool          mMalfStuckFlag;      /**< (--)   Stuck at current position malfunction flag. */
        bool          mMalfFailToFlag;     /**< (--)   Fail to position malfunction flag. */
        double        mMalfFailToValue;    /**< (--)   Fail to position malfunction value. */
        /// @}

        /// @brief    Default constructs this Check Valve.
        GunnsFluidCheckValve();
        /// @brief    Default destructs this Check Valve.
        virtual ~GunnsFluidCheckValve();
        /// @brief    Initializes this Check Valve with configuration and input data.
        void initialize(const GunnsFluidCheckValveConfigData& configData,
                        const GunnsFluidCheckValveInputData&  inputData,
                        std::vector<GunnsBasicLink*>&         networkLinks,
                        const int                             port0,
                        const int                             port1);
        /// @brief Sets and resets the stuck malfunction
        void setMalfStuck(const bool flag = false);
        /// @brief Sets and resets the fail to position malfunction
        void setMalfFailTo(const bool flag = false, const double value = 0.0);

    protected:
        double        mRateLimit;          /**< (1/s)  trick_chkpnt_io(**) Position rate limit for this Check Valve. */
        double        mClosePressure;      /**< (kPa)  trick_chkpnt_io(**) Delta pressure at which this Check Valve is fully closed. */
        double        mOpenPressure;       /**< (kPa)  trick_chkpnt_io(**) Delta pressure at which this Check Valve is fully opened. */
        /// @brief    Validates the initialization of this Check Valve.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Updates the state of this Check Valve.
        virtual void updateState(const double dt);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidCheckValve(const GunnsFluidCheckValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidCheckValve& operator =(const GunnsFluidCheckValve&);
};

/// @}

#endif
