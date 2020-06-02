#ifndef GunnsFluidPipe_EXISTS
#define GunnsFluidPipe_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_PIPE    Pipe Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Pipe link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidPipe.o))

PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pipe Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Pipe link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPipeConfigData : public GunnsFluidConductorConfigData
{
    public:
        double        mThermalLength;       /**< (m) trick_chkpnt_io(**) Tube length for thermal convection.                 */
        double        mThermalDiameter;     /**< (m) trick_chkpnt_io(**) Tube inner diameter for thermal convection.         */
        double        mSurfaceRoughness;    /**< (m) trick_chkpnt_io(**) Tube wall surface roughness for thermal convection. */
        /// @brief    Default constructs this Pipe configuration data.
        GunnsFluidPipeConfigData(const std::string& name                 = "",
                                 GunnsNodeList*     nodes                = 0,
                                 const double       maxConductivity      = 0.0,
                                 const double       expansionScaleFactor = 0.0,
                                 const double       thermalLength        = 0.0,
                                 const double       thermalDiameter      = 0.0,
                                 const double       surfaceRoughness     = 0.0);
        /// @brief    Copy constructs this Pipe configuration data.
        GunnsFluidPipeConfigData(const GunnsFluidPipeConfigData& that);
        /// @brief    Default destructs this Pipe configuration data.
        virtual ~GunnsFluidPipeConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidPipeConfigData& operator =(const GunnsFluidPipeConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pipe Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Pipe input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPipeInputData : public GunnsFluidConductorInputData
{
    public:
        double        mWallTemperature;     /**< (K) Tube wall temperature for thermal convection */
        /// @brief    Default constructs this Pipe input data.
        GunnsFluidPipeInputData(const bool   malfBlockageFlag  = false,
                                const double malfBlockageValue = 0.0,
                                const double wallTemperature   = 0.0);
        /// @brief    Copy constructs this Pipe input data.
        GunnsFluidPipeInputData(const GunnsFluidPipeInputData& that);
        /// @brief    Default destructs this Pipe input data.
        virtual ~GunnsFluidPipeInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidPipeInputData& operator =(const GunnsFluidPipeInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pipe Link Model
///
/// @details  The GUNNS Fluid Pipe link model is a pipe with heat transfer between the fluid and
///           the walls of the pipe.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPipe : public GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidPipe);
    public:
        /// @brief    Default constructs this Pipe.
        GunnsFluidPipe();
        /// @brief    Default destructs this Pipe.
        virtual ~GunnsFluidPipe();
        /// @brief    Initializes this Pipe with configuration and initialization data.
        void initialize(const GunnsFluidPipeConfigData& configData,
                        const GunnsFluidPipeInputData&  inputData,
                        std::vector<GunnsBasicLink*>&   networkLinks,
                        const int                       port0,
                        const int                       port1);
        /// @brief    Returns the pipe segment heat flux.
        double getPipeHeatFlux() const;
        /// @brief    Sets the thermal surface area of this Pipe.
        void   setThermalSurfaceArea(const double value);
        /// @brief    Sets the wall temperature of this Pipe.
        void   setWallTemperature(const double value);
    protected:
        double        mThermalDiameter;     /**< (m)  trick_chkpnt_io(**) Tube inner diameter for thermal convection           */
        double        mThermalSurfaceArea;  /**< (m2) trick_chkpnt_io(**) Tube inner surface area for thermal convection       */
        double        mThermalROverD;       /**< (--) trick_chkpnt_io(**) Tube surface roughness over diameter for convection  */
        double        mWallTemperature;     /**< (K)                      Tube wall temperature for thermal convection (input from simbus) */
        double        mWallHeatFlux;        /**< (W)                      Convection heat flux from the fluid to the tube wall (output to simbus) */
        /// @brief    Validates the initialization of this Pipe.
        void validate(const GunnsFluidPipeConfigData& configData,
                      const GunnsFluidPipeInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Updates the internal fluid of this Pipe.
        virtual void updateFluid(double dt, double flowRate);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidPipe(const GunnsFluidPipe& );
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidPipe& operator =(const GunnsFluidPipe&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (J/s)  The wall heat flux.
///
/// @details  Gets the wall heat flux of this GUNNS FLuid Pipe Link Model.
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidPipe::getPipeHeatFlux() const
{
    return mWallHeatFlux;
}

#endif

