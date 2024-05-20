#ifndef GunnsFluidHatch_EXISTS
#define GunnsFluidHatch_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_HATCH    Hatch Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Hatch link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (
   (Conductivity is the hatch cross-sectional area.)
   (Diffusion occurs even with significant mass flow.)
  )

LIBRARY DEPENDENCY:
- ((GunnsFluidHatch.o))

PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-11))

@{
*/

#include "aspects/fluid/conductor/GunnsFluidValve.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hatch Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Hatch link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHatchConfigData : public GunnsFluidValveConfigData
{
    public:
        double   mLength0;           /**< (m) trick_chkpnt_io(**) Distance from hatch to center of port 0 node volume */
        double   mLength1;           /**< (m) trick_chkpnt_io(**) Distance from hatch to center of port 1 node volume */
        /// @brief    Default constructs this Hatch configuration data.
        GunnsFluidHatchConfigData(const std::string& name                 = "",
                                  GunnsNodeList*     nodes                = 0,
                                  const double       maxConductivity      = 0.0,
                                  const double       expansionScaleFactor = 0.0,
                                  const double       thermalLength        = 0.0,
                                  const double       thermalDiameter      = 0.0,
                                  const double       surfaceRoughness     = 0.0,
                                  const double       length0              = 0.0,
                                  const double       length1              = 0.0);
        /// @brief    Copy constructs this Hatch configuration data.
        GunnsFluidHatchConfigData(const GunnsFluidHatchConfigData& that);
        /// @brief    Default destructs this Hatch configuration data.
        virtual ~GunnsFluidHatchConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHatchConfigData& operator =(const GunnsFluidHatchConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Base Hatch Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Hatch link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHatchInputData : public GunnsFluidValveInputData
{
    public:
         /// @brief    Default constructs this Hatch input data.
        GunnsFluidHatchInputData(const bool   malfBlockageFlag  = false,
                                 const double malfBlockageValue = 0.0,
                                 const double position          = 0.0,
                                 const bool   malfLeakThruFlag  = false,
                                 const double malfLeakThruValue = 0.0,
                                 const double wallTemperature   = 0.0);
        /// @brief    Copy constructs this Hatch input data.
        GunnsFluidHatchInputData(const GunnsFluidHatchInputData& that);
        /// @brief    Default destructs this Hatch input data.
        virtual ~GunnsFluidHatchInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHatchInputData& operator =(const GunnsFluidHatchInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hatch Model
///
/// @details  The GUNNS Fluid Hatch link model simulates a hatch between cabins. It acts as a valve
///           with the addition of gas diffusion and thermal conduction modeling across its large
///           cross-sectional area between its large port node volumes.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHatch : public  GunnsFluidValve
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidHatch);
    public:
        /// @brief    Default constructs this Hatch.
        GunnsFluidHatch();
        /// @brief    Default destructs this Hatch.
        virtual ~GunnsFluidHatch();
        /// @brief    Initializes this Hatch with configuration and input data.
        void initialize(const GunnsFluidHatchConfigData&   configData,
                        const GunnsFluidHatchInputData&    inputData,
                        std::vector<GunnsBasicLink*>&      networkLinks,
                        const int                          port0,
                        const int                          port1);
    protected:
        double     mLength0;            /**< (m)    trick_chkpnt_io(**) Distance from hatch to center port 0 node volume */
        double     mLength1;            /**< (m)    trick_chkpnt_io(**) Distance from hatch to center port 1 node volume */
        double     mDiffusiveFlowRate;  /**< (kg/s) trick_chkpnt_io(**) Mass flow rate for diffusion */
        PolyFluid* mDiffusiveFluid;     /**< (--)   trick_chkpnt_io(**) Pointer to internal fluid for diffusion */
        double     mConductiveHeatFlux; /**< (W)    trick_chkpnt_io(**) Heat flux conducted across the hatch */

        static const double DIFFUSION_DELTA_PRESS_LIMIT;  /**< (kPa) Delta pressure below which diffusion is allowed to be calculated. */
        static const double DIFFUSION_DELTA_TEMP_LIMIT;   /**< (K)   Delta temperature below which diffusion is allowed to be calculated. */

        /// @brief    Validates the initialization of this Hatch.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Updates the internal fluid of this Hatch.
        virtual void updateFluid(const double dt, const double flowrate);
        /// @brief    Moves the fluid from one port to the other.
        virtual void  transportFluid(const bool forcedOutflow, const int fromPort = 0, const int toPort = 1);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHatch(const GunnsFluidHatch&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHatch& operator =(const GunnsFluidHatch&);
};

/// @}

#endif
