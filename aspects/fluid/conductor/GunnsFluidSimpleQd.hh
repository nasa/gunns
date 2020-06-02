#ifndef GunnsFluidSimpleQd_EXISTS
#define GunnsFluidSimpleQd_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_SIMPLE_QD    Simple Quick-Disconnect Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Simple Quick-Disconnect model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidSimpleQd.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Initial) (2012-08))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Simple Quick-Disconnect Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Gunns Simple
///           Quick-Disconnect configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSimpleQdConfigData : public GunnsFluidConductorConfigData
{
    public:
        double mThermalLength;        /**< (m) trick_chkpnt_io(**) Tube length for thermal convection                 */
        double mThermalDiameter;      /**< (m) trick_chkpnt_io(**) Tube inner diameter for thermal convection         */
        double mSurfaceRoughness;     /**< (m) trick_chkpnt_io(**) Tube wall surface roughness for thermal convection */
        /// @brief    Default constructs this Simple QD configuration data.
        GunnsFluidSimpleQdConfigData(const std::string& name                 = "",
                                     GunnsNodeList*     nodes                = 0,
                                     const double       maxConductivity      = 0.0,
                                     const double       expansionScaleFactor = 0.0,
                                     const double       thermalLength        = 0.0,
                                     const double       thermalDiameter      = 0.0,
                                     const double       surfaceRoughness     = 0.0);
        /// @brief    Copy constructs this Simple QD configuration data.
        GunnsFluidSimpleQdConfigData(const GunnsFluidSimpleQdConfigData& that);
        /// @brief    Default destructs this Simple QD configuration data.
        virtual ~GunnsFluidSimpleQdConfigData();

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSimpleQdConfigData& operator =(const GunnsFluidSimpleQdConfigData&);
};

// Forward declaring the input data class, see below for the actual class definition.  We had to
// define the input data class after the main link class so that the input data class could use the
// State enum type.
class GunnsFluidSimpleQdInputData;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsFluidSimpleQd Model
///
/// @details  The GUNNS Simple Quick-Disconnect link is a simplified functional model of a hose with
///           self-sealing quick-disconnects at one or both ends.  The simple QD has two states of
///           connection - either CONNECTED or DISCONNECTED.  When connected, the hose is open to
///           flow between the nodes.  When disconnected, the hose is sealed and cannot flow.  The
///           simple QD also can model convective heat transfer between the hose wall and the fluid;
///           this is optional, and the thermalLength or thermaDiameter configuration parameters can
///           be left zero to disable this effect.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSimpleQd : public GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSimpleQd);
    public:
        /// @brief    Enumeration of the possible states of a Simple QD.
        enum State {
            DISCONNECTED = 0,
            CONNECTED    = 1
        };
        /// @brief    Default constructs this Simple QD.
        GunnsFluidSimpleQd();
        /// @brief    Default destructs this Simple QD.
        virtual ~GunnsFluidSimpleQd();
        /// @brief    Initializes this Simple QD with configuration and initialization data.
        void initialize(const GunnsFluidSimpleQdConfigData& configData,
                        const GunnsFluidSimpleQdInputData&  inputData,
                        std::vector<GunnsBasicLink*>&       networkLinks,
                        const int                           port0,
                        const int                           port1);
        /// @brief    Returns the heat flux from the fluid to the tube wall.
        double getWallHeatFlux() const;
        /// @brief    Returns the state of this Simple QD.
        State  getState() const;
        /// @brief    Sets the state of this Simple QD.
        void   setState(const State newState);
        /// @brief    Sets the thermal surface area of this Simple QD.
        void   setThermalSurfaceArea(const double value);
        /// @brief    Sets the wall temperature of this Simple QD.
        void   setWallTemperature(const double value);

    protected:
        double mThermalDiameter;    /**< (m)  trick_chkpnt_io(**) Tube inner diameter for thermal convection          */
        double mThermalSurfaceArea; /**< (m2) trick_chkpnt_io(**) Tube inner surface area for thermal convection      */
        double mThermalROverD;      /**< (--) trick_chkpnt_io(**) Tube surface roughness over diameter for convection */
        int    mState;              /**< (--)                     Connection state of the QD, controlled by user      */
        double mWallTemperature;    /**< (K)                      Tube wall temperature for thermal convection        */
        double mWallHeatFlux;       /**< (W)                      Convection heat flux from the fluid to the tube wall*/
        /// @brief    Validates the initialization of this Simple QD.
        void validate(const GunnsFluidSimpleQdInputData& inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Updates the state of this Simple QD.
        virtual void updateState(const double dt);
        /// @brief    Updates the internal fluid of this Simple QD.
        virtual void updateFluid(const double dt, const double flowRate);

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSimpleQd(const GunnsFluidSimpleQd& );
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSimpleQd& operator =(const GunnsFluidSimpleQd&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsFluidSimpleQd Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Gunns Simple
///           Quick-Disconnect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSimpleQdInputData : public GunnsFluidConductorInputData
{
    public:
        GunnsFluidSimpleQd::State mState;  /**< (--) Initial connection state of the QD           */
        double mWallTemperature;           /**< (K)  Tube wall temperature for thermal convection */
        /// @brief    Default constructs this Simple QD input data.
        GunnsFluidSimpleQdInputData(const bool   malfBlockageFlag         = false,
                                    const double malfBlockageValue        = 0.0,
                                    const GunnsFluidSimpleQd::State state = GunnsFluidSimpleQd::DISCONNECTED,
                                    const double wallTemperature          = 0.0);
        /// @brief    Copy constructs this Simple QD input data.
        GunnsFluidSimpleQdInputData(const GunnsFluidSimpleQdInputData& that);
        /// @brief    Default destructs this Simple QD input data.
        virtual ~GunnsFluidSimpleQdInputData();

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSimpleQdInputData& operator =(const GunnsFluidSimpleQdInputData&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) The convection heat flux from the fluid to the tube wall.
///
/// @details  Getter method that returns the wall heat flux.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSimpleQd::getWallHeatFlux() const
{
    return mWallHeatFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsFluidSimpleQd::State (--) Connection state.
///
/// @details  Getter method that returns connection state.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsFluidSimpleQd::State GunnsFluidSimpleQd::getState() const
{
    return static_cast<GunnsFluidSimpleQd::State>(mState);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] newState   (--)    New State.
///
/// @returns  void
///
/// @details  Sets the state of this this GUNNS Simple Quick-Disconnect link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSimpleQd::setState(const State newState)
{
    mState = static_cast<int>(newState);
}

#endif
