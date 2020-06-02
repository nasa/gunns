#ifndef GunnsLiquidCentrifugalPump_EXISTS
#define GunnsLiquidCentrifugalPump_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_POTENTIAL_LIQUID_CENTRIFUGAL_PUMP Liquid Centrifugal Pump Model
@ingroup   TSM_GUNNS_FLUID_POTENTIAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Liquid Centrifugal Pump link model used to model the fluid aspect of a
   liquid pump.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsLiquidCentrifugalPump.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Install) (2012-07))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "aspects/fluid/potential/GunnsGasFan.hh"
#include "aspects/fluid/potential/GunnsPumpCavitation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Liquid Centrifugal Pump Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Liquid
///           Centrifugal Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsLiquidCentrifugalPumpConfigData : public GunnsGasFanConfigData
{
    public:
        FluidProperties::FluidType mLiquidType; /**< (--) trick_chkpnt_io(**) Type of the operating liquid       */
        double mAutoCavitationDuration;         /**< (s)  trick_chkpnt_io(**) Duration of auto-cavitation effect */
        double mAutoCavitationAmplitude;        /**< (--) trick_chkpnt_io(**) Amplitude (0-1) of auto-cavitation noise */
        /// @brief  Default constructs this Liquid Centrifugal Pump configuration data.
        GunnsLiquidCentrifugalPumpConfigData(
                const std::string& name                     = "",
                GunnsNodeList*     nodes                    = 0,
                const double       maxConductivity          = 0.0,
                const double       expansionScaleFactor     = 0.0,
                const double       referenceDensity         = 0.0,
                const double       referenceSpeed           = 0.0,
                const double       referenceCoeff0          = 0.0,
                const double       referenceCoeff1          = 0.0,
                const double       referenceCoeff2          = 0.0,
                const double       referenceCoeff3          = 0.0,
                const double       referenceCoeff4          = 0.0,
                const double       referenceCoeff5          = 0.0,
                const double       bestEfficiency           = 0.0,
                const double       referenceQBep            = 0.0,
                const double       filterGain               = 0.5,
                const double       driveRatio               = 1.0,
                const double       thermalLength            = 0.0,
                const double       thermalDiameter          = 0.0,
                const double       surfaceRoughness         = 0.0,
                const bool         checkValveActive         = false,
                const FluidProperties::FluidType liquidType = FluidProperties::NO_FLUID,
                const double       autoCavitationDuration   = 0.0,
                const double       autoCavitationAmplitude  = 0.0);
        /// @brief  Copy constructs this Liquid Centrifugal Pump configuration data.
        GunnsLiquidCentrifugalPumpConfigData(const GunnsLiquidCentrifugalPumpConfigData& that);
        /// @brief  Default destructs this Liquid Centrifugal Pump configuration data.
        virtual ~GunnsLiquidCentrifugalPumpConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsLiquidCentrifugalPumpConfigData& operator =(const GunnsLiquidCentrifugalPumpConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Liquid Centrifugal Pump Model Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Liquid
///           Centrifugal Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsLiquidCentrifugalPumpInputData : public GunnsGasFanInputData
{
    public:
        bool mEnableAutoCavitation;              /**< (--) Enables the automatic cavitation model */
        /// @brief  Default constructs this Liquid Centrifugal Pump input data.
        GunnsLiquidCentrifugalPumpInputData(const bool   malfBlockageFlag     = false,
                                            const double malfBlockageValue    = 0.0,
                                            const double sourcePressure       = 0.0,
                                            const double motorSpeed           = 0.0,
                                            const double wallTemperature      = 0.0,
                                            const bool   enableAutoCavitation = false);
        /// @brief  Copy constructs this Liquid Centrifugal Pump input data.
        GunnsLiquidCentrifugalPumpInputData(const GunnsLiquidCentrifugalPumpInputData& that);
        /// @brief  Default destructs this Liquid Centrifugal Pump input data.
        virtual ~GunnsLiquidCentrifugalPumpInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsLiquidCentrifugalPumpInputData& operator =(const GunnsLiquidCentrifugalPumpInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Liquid Centrifugal Pump Model
///
/// @details  The GUNNS Liquid Centrifugal Pump link model simulates all axial, radial, or mixed-
///           flow liquid ducted centrifugal pumps.  It is identical to GunnsGasFan, but adds a
///           model of liquid cavitation.  This class should NOT be used for positive displacement
///           (constant-volume) pumps such as pistons, etc.  For those types of pumps, use the
///           GunnsLiquidDisplacementPump class instead.
///
///           This class inherits GunnsGasFan and has all of that class's features.  This class adds
///           a model of partial and full liquid cavitation as a function of inlet pressure, and
///           malfunctions to force cavitation.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsLiquidCentrifugalPump : public GunnsGasFan
{
    TS_MAKE_SIM_COMPATIBLE(GunnsLiquidCentrifugalPump);
    public:

        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        GunnsPumpCavitation        mCavitation; /**< (--)  Cavitation calculations object */
        /// @}

        /// @brief  Default constructs this Liquid Centrifugal Pump.
        GunnsLiquidCentrifugalPump();
        /// @brief  Default destructs this Liquid Centrifugal Pump.
        virtual ~GunnsLiquidCentrifugalPump();
        /// @brief  Initializes this Liquid Centrifugal Pump with configuration and input data.
        void initialize(const GunnsLiquidCentrifugalPumpConfigData&  configData,
                        const GunnsLiquidCentrifugalPumpInputData&   inputData,
                        std::vector<GunnsBasicLink*>& links,
                        const int                     port0,
                        const int                     port1);

    protected:
        FluidProperties::FluidType mLiquidType; /**< *o (--) trick_chkpnt_io(**) Type of the operating liquid   */
        /// @brief  Validates the initialization of this Liquid Centrifugal Pump.
        void         validate(const GunnsLiquidCentrifugalPumpConfigData& configData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief  Updates the state of this Liquid Centrifugal Pump.
        virtual void updateState(const double dt);

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsLiquidCentrifugalPump(const GunnsLiquidCentrifugalPump&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsLiquidCentrifugalPump& operator =(const GunnsLiquidCentrifugalPump&);
};

/// @}

#endif
