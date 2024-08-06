#ifndef GunnsFluidPhaseChangeConductor_EXISTS
#define GunnsFluidPhaseChangeConductor_EXISTS

/**
@file
@brief    GUNNS Fluid Phase Change Conductor Model declarations

@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_PHASE_CHANGE    GUNNS Fluid Phase Change Conductor Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Phase Change Conductor link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidPhaseChangeConductor.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (2016-04))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Phase Change Conductor Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Phase Change Conductor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPhaseChangeConductorConfigData : public GunnsFluidConductorConfigData
{
    public:
        FluidProperties::FluidType mLiquidType; /**< (--) trick_chkpnt_io(**) Fluid type of constituent liquid being changed. */
        FluidProperties::FluidType mGasType;    /**< (--) trick_chkpnt_io(**) Fluid type of constituent gas being changed. */
        double                     mEfficiency; /**< (--) trick_chkpnt_io(**) Optional phase change heat efficiency. */
        /// @brief  Default constructs this Phase Change Conductor configuration data.
        GunnsFluidPhaseChangeConductorConfigData(
                const std::string&               name            = "",
                GunnsNodeList*                   nodes           = 0,
                const double                     maxConductivity = 0.0,
                const FluidProperties::FluidType liquidType      = FluidProperties::NO_FLUID,
                const FluidProperties::FluidType gasType         = FluidProperties::NO_FLUID,
                const double                     efficiency      = 0.0);
        /// @brief  Copy constructs this Phase Change Conductor configuration data.
        GunnsFluidPhaseChangeConductorConfigData(const GunnsFluidPhaseChangeConductorConfigData& that);
        /// @brief  Default destructs this Phase Change Conductor configuration data.
        virtual ~GunnsFluidPhaseChangeConductorConfigData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPhaseChangeConductorConfigData& operator =(const GunnsFluidPhaseChangeConductorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Phase Change Conductor Model Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Phase Change Conductor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPhaseChangeConductorInputData : public GunnsFluidConductorInputData
{
    public:
        double mWallTemperature;           /**< (K) trick_chkpnt_io(**) Initial wall temperature. */
        /// @brief  Default constructs this Phase Change Conductor input data.
        GunnsFluidPhaseChangeConductorInputData(const bool   malfBlockageFlag  = false,
                                                const double malfBlockageValue = 0.0,
                                                const double wallTemperature   = 0.0);
        /// @brief  Copy constructs this Phase Change Conductor input data.
        GunnsFluidPhaseChangeConductorInputData(const GunnsFluidPhaseChangeConductorInputData& that);
        /// @brief  Default destructs this Phase Change Conductor input data.
        virtual ~GunnsFluidPhaseChangeConductorInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPhaseChangeConductorInputData& operator =(const GunnsFluidPhaseChangeConductorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Phase Change Conductor Model
///
/// @details  The GUNNS Fluid Phase Change Conductor link is just like a regular GunnsFluidConductor
///           except it connects between liquid & gas phase nodes, and transforms the liquid fluid
///           type to or from its gas phase based on the flow direction.
///
///           The fluids in the connected liquid & gas nodes should be 100% of the specified liquid
///           & gas fluid type, respectively.  This link can only transport the given fluid compound
///           between the nodes, and other compounds present in a mixture will have their mass
///           replaced by the given compound type.
///
///           Port 0 of this link must be connected to the liquid node, and Port 1 to the gas node.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPhaseChangeConductor : public  GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidPhaseChangeConductor);
    public:
        /// @brief  Default constructs this Phase Change Conductor.
        GunnsFluidPhaseChangeConductor();
        /// @brief  Default destructs this Phase Change Conductor.
        virtual ~GunnsFluidPhaseChangeConductor();
        /// @brief  Initializes this Phase Change Conductor with configuration and input data.
        void initialize(const GunnsFluidPhaseChangeConductorConfigData& configData,
                        const GunnsFluidPhaseChangeConductorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&                   networkLinks,
                        const int                                       port0,
                        const int                                       port1);
        /// @brief  Moves the fluid from one port to the other.
        virtual void transportFluid(const bool forcedOutflow,
                                    const int  fromPort = 0,
                                    const int  toPort   = 1);
        /// @brief  Returns the heat flux from the fluid to the wall.
        double getWallHeatFlux() const;
        /// @brief  Sets the wall temperature.
        void   setWallTemperature(const double temperature);

    protected:
        FluidProperties::FluidType mLiquidType;      /**< *o (--) trick_chkpnt_io(**) Fluid type of constituent liquid being changed. */
        FluidProperties::FluidType mGasType;         /**< *o (--) trick_chkpnt_io(**) Fluid type of constituent gas being changed. */
        PolyFluid*                 mLiquidFluid;     /**<    (--) trick_chkpnt_io(**) Pointer to an internal fluid for transport to/from the liquid node. */
        PolyFluid*                 mGasFluid;        /**<    (--) trick_chkpnt_io(**) Pointer to an internal fluid for transport to/from the gas node. */
        double                     mEfficiency;      /**<    (--) trick_chkpnt_io(**) Optional phase change heat efficiency. */
        double                     mWallHeatFlux;    /**<    (W)                      Phase change heat flux from fluid to wall. */
        double                     mWallTemperature; /**<    (W)                      Wall temperature. */

        /// @brief  Validates the initialization inputs of this Phase Change Conductor.
        void validate(const GunnsFluidPhaseChangeConductorConfigData& configData,
                      const GunnsFluidPhaseChangeConductorInputData&  inputData) const;
        /// @brief  Checks for valid implementation-specific port node assignment
        virtual bool checkSpecificPortRules(const int port, const int node) const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidPhaseChangeConductor(const GunnsFluidPhaseChangeConductor&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPhaseChangeConductor& operator =(const GunnsFluidPhaseChangeConductor&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (W)  Heat flux from the fluid to the wall.
///
/// @details  Returns the value of mWallHeatFlux.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidPhaseChangeConductor::getWallHeatFlux() const
{
    return mWallHeatFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  temperature  (K)  The temperature value to use.
///
/// @details  Sets mWallTemperature to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidPhaseChangeConductor::setWallTemperature(const double temperature)
{
    mWallTemperature = temperature;
}

#endif
