#ifndef GunnsFluidValve_EXISTS
#define GunnsFluidValve_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_VALVE    Valve Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Valve link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidValve.o))

PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Update) (2011-09))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidValveConfigData : public GunnsFluidConductorConfigData
{
    public:
        double mThermalLength;        /**< (m) trick_chkpnt_io(**) Tube length for thermal convection                 */
        double mThermalDiameter;      /**< (m) trick_chkpnt_io(**) Tube inner diameter for thermal convection         */
        double mSurfaceRoughness;     /**< (m) trick_chkpnt_io(**) Tube wall surface roughness for thermal convection */
        /// @brief    Default constructs this Valve configuration data.
        GunnsFluidValveConfigData(const std::string& name                 = "",
                                  GunnsNodeList*     nodes                = 0,
                                  const double       maxConductivity      = 0.0,
                                  const double       expansionScaleFactor = 0.0,
                                  const double       thermalLength        = 0.0,
                                  const double       thermalDiameter      = 0.0,
                                  const double       surfaceRoughness     = 0.0);
        /// @brief    Copy constructs this Valve configuration data.
        GunnsFluidValveConfigData(const GunnsFluidValveConfigData& that);
        /// @brief    Default destructs this Valve configuration data.
        virtual ~GunnsFluidValveConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidValveConfigData& operator =(const GunnsFluidValveConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Base Valve Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Valve link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidValveInputData : public GunnsFluidConductorInputData
{
    public:
        double        mPosition;           /**< (--)     Fractional position of this Valve            */
        bool          mMalfLeakThruFlag;   /**< (--)     Leak through rate malfunction flag           */
        double        mMalfLeakThruValue;  /**< (kg/s)   Leak through rate malfunction value          */
        double        mWallTemperature;    /**< (K)      Tube wall temperature for thermal convection */
         /// @brief    Default constructs this Valve input data.
        GunnsFluidValveInputData(const bool   malfBlockageFlag  = false,
                                 const double malfBlockageValue = 0.0,
                                 const double position          = 0.0,
                                 const bool   malfLeakThruFlag  = false,
                                 const double malfLeakThruValue = 0.0,
                                 const double wallTemperature   = 0.0);
        /// @brief    Copy constructs this Valve input data.
        GunnsFluidValveInputData(const GunnsFluidValveInputData& that);
        /// @brief    Default destructs this Valve input data.
        virtual ~GunnsFluidValveInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidValveInputData& operator =(const GunnsFluidValveInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve Model
///
/// @details  The GUNNS Fluid Valve link model is the base class from which valves derive. A valve
///           is distinguished by a dynamic position that determines the effective conductivity.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidValve : public  GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidValve);
    public:
        /// @brief    Enumeration of the states of a Valve.
        enum State { CLOSED, OPENING, TRANSITIONING, CLOSING, OPEN };

        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool        mMalfLeakThruFlag;      /**< (--)   Malfunction leak thru rate flag                      */
        double      mMalfLeakThruValue;     /**< (kg/s) Malfunction leak thru rate value                     */
        /// @}

        /// @brief    Default constructs this Valve.
        GunnsFluidValve();
        /// @brief    Default destructs this Valve.
        virtual ~GunnsFluidValve();
        /// @brief    Initializes this Valve with configuration and input data.
        void initialize(const GunnsFluidValveConfigData&   configData,
                        const GunnsFluidValveInputData&    inputData,
                        std::vector<GunnsBasicLink*>&      networkLinks,
                        const int                          port0,
                        const int                          port1);
        /// @brief  Returns the heat flux from the fluid to the tube wall.
        double getWallHeatFlux() const;
        /// @brief    Gets the valve position of this Valve.
        double getPosition() const;
        /// @brief    Sets the valve position on this Valve.
        void setPosition(const double position);
        /// @brief    Sets the leak thru malfunction controls.
        void setMalfLeakThru(const bool flag = false, const double value = 0.0);
        /// @brief    Sets the thermal surface area of this Valve.
        void   setThermalSurfaceArea(const double value);
        /// @brief    Sets the wall temperature of this Valve.
        void   setWallTemperature(const double value);
    protected:
        double      mThermalDiameter;       /**< (m)    trick_chkpnt_io(**) Tube inner diameter for thermal convection           */
        double      mThermalSurfaceArea;    /**< (m2)   trick_chkpnt_io(**) Tube inner surface area for thermal convection       */
        double      mThermalROverD;         /**< (--)   trick_chkpnt_io(**) Tube surface roughness over diameter for convection  */
        double      mWallTemperature;       /**< (K)                        Tube wall temperature for thermal convection         */
        double      mWallHeatFlux;          /**< (W)                        Convection heat flux from the fluid to the tube wall */
        double      mPosition;              /**< (--)                       Fractional position of this Valve                    */
        double      mPreviousLeakRate;      /**< (kg/s)                     Previous leak thru rate value                        */
        double      mLeakConductivity;      /**< (m2)                       Conductivity equivalent to the leak                  */
        /// @brief    Validates the initialization of this Valve.
        void validate(const GunnsFluidValveInputData& inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Updates the state of this Base Valve.
        virtual void updateState(const double dt);
        /// @brief    Updates the internal fluid of this valve.
        virtual void updateFluid(const double dt, const double flowRate);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidValve(const GunnsFluidValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidValve& operator =(const GunnsFluidValve&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   -- Valve position.
///
/// @details  Gets the fractional valve position of this GUNNS Fluid Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidValve::getPosition() const
{
    return mPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    position   --  Input commanded position.
///
/// @return   void
///
/// @details  Sets the fractional valve position of this GUNNS Fluid Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidValve::setPosition(const double position)
{
    mPosition = position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) The convection heat flux from the fluid to the tube wall.
///
/// @details  Getter method that returns the wall heat flux.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidValve::getWallHeatFlux() const
{
    return mWallHeatFlux;
}

#endif

