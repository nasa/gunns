#ifndef GunnsLiquidDisplacementPump_EXISTS
#define GunnsLiquidDisplacementPump_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_SOURCE_LIQUID_DISPLACEMENT_PUMP  Liquid Displacement Pump Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Liquid Displacement Pump link model.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsLiquidDisplacementPump.o))

 PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Initial) (2012-07))

@{
*/

#include "aspects/fluid/source/GunnsGasDisplacementPump.hh"
#include "aspects/fluid/potential/GunnsPumpCavitation.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Liquid Displacement Pump Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Liquid
///           Displacement Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsLiquidDisplacementPumpConfigData : public GunnsGasDisplacementPumpConfigData
{
    public:
        FluidProperties::FluidType mLiquidType;    /**< (--)   trick_chkpnt_io(**) Type of the operating liquid       */
        double mAutoCavitationDuration;            /**< (s)    trick_chkpnt_io(**) Duration of auto-cavitation effect */
        double mAutoCavitationAmplitude;           /**< (m3/s) trick_chkpnt_io(**) Amplitude of auto-cavitation noise */
        /// @brief    Default constructs this Liquid Displacement Pump model configuration data with
        ///           arguments.
        GunnsLiquidDisplacementPumpConfigData(
                const std::string& name                     = "",
                GunnsNodeList*     nodes                    = 0,
                const double       cycleVolume              = 0.0,
                const double       driveRatio               = 1.0,
                const double       thermalLength            = 0.0,
                const double       thermalDiameter          = 0.0,
                const double       surfaceRoughness         = 0.0,
                const bool         checkValveActive         = false,
                const FluidProperties::FluidType liquidType = FluidProperties::NO_FLUID,
                const double       autoCavitationDuration   = 0.0,
                const double       autoCavitationAmplitude  = 0.0);

        /// @brief    Default destructs this Liquid Displacement Pump model configuration data.
        virtual ~GunnsLiquidDisplacementPumpConfigData();
        /// @brief    Copy constructs this Liquid Displacement Pump model configuration data.
        GunnsLiquidDisplacementPumpConfigData(const GunnsLiquidDisplacementPumpConfigData& that);

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsLiquidDisplacementPumpConfigData& operator =(const GunnsLiquidDisplacementPumpConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Liquid Displacement Pump Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Liquid
///           Displacement Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsLiquidDisplacementPumpInputData : public GunnsGasDisplacementPumpInputData
{
    public:
        bool mEnableAutoCavitation;              /**< (--) Enables the automatic cavitation model */
        /// @brief    Default constructs this Liquid Displacement Pump model input data with
        ///           arguments.
        GunnsLiquidDisplacementPumpInputData(const bool   malfBlockageFlag     = false,
                                             const double malfBlockageValue    = 0.0,
                                             const double flowDemand           = 0.0,
                                             const double motorSpeed           = 0.0,
                                             const double wallTemperature      = 0.0,
                                             const bool   enableAutoCavitation = false);
        /// @brief    Copy constructs this Liquid Displacement Pump model input data.
        GunnsLiquidDisplacementPumpInputData(const GunnsLiquidDisplacementPumpInputData& that);
        /// @brief    Default destructs this Liquid Displacement Pump model input data.
        ~GunnsLiquidDisplacementPumpInputData();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsLiquidDisplacementPumpInputData& operator=(const GunnsLiquidDisplacementPumpInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Liquid Displacement Pump Model
///
/// @details
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsLiquidDisplacementPump : public GunnsGasDisplacementPump
{
    TS_MAKE_SIM_COMPATIBLE(GunnsLiquidDisplacementPump);
    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        GunnsPumpCavitation        mCavitation; /**< (--) Cavitation calculations object */
        /// @}

        /// @brief    Default constructs this Liquid Displacement Pump model.
        GunnsLiquidDisplacementPump();
        /// @brief    Default destructs this Liquid Displacement Pump model.
        virtual ~GunnsLiquidDisplacementPump();
        /// @brief    Initializes this Liquid Displacement Pump model with configuration and input
        ///           data.
        void initialize(const GunnsLiquidDisplacementPumpConfigData& configData,
                        const GunnsLiquidDisplacementPumpInputData&  inputData,
                        std::vector<GunnsBasicLink*>&           networkLinks,
                        const int                               port0,
                        const int                               port1);

    protected:
        FluidProperties::FluidType mLiquidType; /**< *o (--) trick_chkpnt_io(**) Type of the operating liquid   */
        /// @brief    Validates the initialization inputs of this Liquid Displacement Pump model.
        void validate(const GunnsLiquidDisplacementPumpConfigData& configData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Updates the cavitation object.
        virtual void applyCavitation(const double dt);

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsLiquidDisplacementPump(const GunnsLiquidDisplacementPump&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsLiquidDisplacementPump& operator=(const GunnsLiquidDisplacementPump&);
};

/// @}

#endif
