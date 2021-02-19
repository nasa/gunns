#ifndef GunnsFluidPhaseChangeSource_EXISTS
#define GunnsFluidPhaseChangeSource_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_SOURCE_PHASE_CHANGE  Phase Change Source Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for the GUNNS Fluid Phase Change Source link model.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ((This link can momentarily fail to conserve mass for gas-to-liquid flow direction when the gas
    node switches between 100% and less than 100% of the phase change gas type mixture.))

 LIBRARY DEPENDENCY:
- ((GunnsFluidPhaseChangeSource.o))

 PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (initial) (2015-01))

@{
*/

#include "core/GunnsFluidSource.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Phase Change Source Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Phase Change Source link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPhaseChangeSourceConfigData : public GunnsFluidLinkConfigData
{
    public:
        FluidProperties::FluidType  mLiquidType; /**< (--) trick_chkpnt_io(**) Fluid type of constituent liquid being changed. */
        FluidProperties::FluidType  mGasType;    /**< (--) trick_chkpnt_io(**) Fluid type of constituent gas being changed. */
        double                      mEfficiency; /**< (--) trick_chkpnt_io(**) Power Efficiency of the modeled device. */
        /// @brief    Default constructs this Phase Change Source configuration data.
        GunnsFluidPhaseChangeSourceConfigData(const std::string&               name       = "",
                                              GunnsNodeList*                   nodes      = 0,
                                              const FluidProperties::FluidType liquidType = FluidProperties::NO_FLUID,
                                              const FluidProperties::FluidType gasType    = FluidProperties::NO_FLUID,
                                              const double                     efficiency = 0.0);
        /// @brief    Default destructs this Phase Change Source configuration data.
        virtual ~GunnsFluidPhaseChangeSourceConfigData();
        /// @brief    Copy constructs this Phase Change Source configuration data.
        GunnsFluidPhaseChangeSourceConfigData(const GunnsFluidPhaseChangeSourceConfigData& that);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidPhaseChangeSourceConfigData& operator =(const GunnsFluidPhaseChangeSourceConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Phase Change Source Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Phase Change Source link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPhaseChangeSourceInputData : public GunnsFluidLinkInputData
{
    public:
        double                      mPowerInput; /**< (W) Initial power input to the link. */
        /// @brief    Default constructs this Phase Change Source input data with arguments.
        GunnsFluidPhaseChangeSourceInputData(const bool   malfBlockageFlag  = false,
                                             const double malfBlockageValue = 0.0,
                                             const double powerInput        = 0.0);
        /// @brief    Copy constructs this Phase Change Source input data.
        GunnsFluidPhaseChangeSourceInputData(const GunnsFluidPhaseChangeSourceInputData& that);
        /// @brief    Default destructs this Phase Change Source input data.
        ~GunnsFluidPhaseChangeSourceInputData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidPhaseChangeSourceInputData& operator=(const GunnsFluidPhaseChangeSourceInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Phase Change Source
///
/// @details  Class for modeling a GUNNS Fluid Phase Change Source link model.  This link converts
///           gas to liquid, and vice-versa, given an input power or heat.  It computes the mass
///           change rate based on heat to move the fluid to saturation temperature at the exit
///           node's pressure plus the heat of phase change (heat of vaporization).  Input power is
///           scaled by an efficiency to model the conversion of electrical input power to useful
///           fluid heat, as in a liquefier equipment, etc.  Liquid is always on the Port 0 node,
///           and Port 1 is always on a gas node.  Positive power input converts liquid to gas and
///           a flow from Port 0 to Port 1.  Negative power input causes the opposite direction of
///           phase conversion & flow.  Efficiency can be positive or negative to correlate the
///           desired flow direction to the desired sign of the input power.
///
/// @note     This link assumes the destination node fluid is saturated.
///
/// @note     This link assumes the liquid node is 100% of the phase change liquid type, since in
///           general, GUNNS does not support liquid mixtures.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPhaseChangeSource : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidPhaseChangeSource);
    public:
        /// @brief    Default constructs this Phase Change Source.
        GunnsFluidPhaseChangeSource();
        /// @brief    Default destructs this Phase Change Source.
        virtual ~GunnsFluidPhaseChangeSource();
        /// @brief    Initializes this Phase Change Source with configuration and input data.
        void initialize(const GunnsFluidPhaseChangeSourceConfigData& configData,
                        const GunnsFluidPhaseChangeSourceInputData&  inputData,
                        std::vector<GunnsBasicLink*>&                networkLinks,
                        const int                                    port0,
                        const int                                    port1);
        /// @brief    Updates the link's contributions to the network system of equations.
        virtual void step(const double dt);
        /// @brief    Computes the flows across the link.
        virtual void computeFlows(const double dt);
        /// @brief    Transports the flows across the link.
        virtual void transportFlows(const double dt);
        /// @brief    Sets the input power of this GunnsFluidPhaseChangeSource.
        void         setPowerInput(const double power);
        /// @brief Moves the fluid from one port to the other
        virtual void transportFluid(const bool forcedOutflow,
                                    const int  fromPort = 0,
                                    const int  toPort   = 1);

    protected:
        FluidProperties::FluidType mLiquidType;  /**< *o (--)   trick_chkpnt_io(**) Fluid type of constituent liquid being changed. */
        FluidProperties::FluidType mGasType;     /**< *o (--)   trick_chkpnt_io(**) Fluid type of constituent gas being changed. */
        double                     mEfficiency;  /**<    (--)   trick_chkpnt_io(**) Power efficiency of the modeled device. */
        int                        mLiquidIndex; /**< *o (--)   trick_chkpnt_io(**) Index of constituent liquid being changed. */
        int                        mGasIndex;    /**< *o (--)   trick_chkpnt_io(**) Index of constituent gas being changed. */
        double                     mPowerInput;  /**<    (W)                        Power input to the link. */
        PolyFluid*                 mLiquidFluid; /**<    (--)   trick_chkpnt_io(**) Pointer to an internal fluid for transport to/from the liquid node. */
        PolyFluid*                 mGasFluid;    /**<    (--)   trick_chkpnt_io(**) Pointer to an internal fluid for transport to/from the gas node. */
        double                     mTsat;        /**<    (K)    trick_chkpnt_io(**) Exit side saturation temperature. */
        double                     mDh;          /**<    (J/kg) trick_chkpnt_io(**) Change in enthalpy to heat the inlet side to the exit saturation point. */
        double                     mL;           /**<    (J/kg) trick_chkpnt_io(**) Exit side heat of vaporization. */
        /// @brief    Validates the initialization inputs of this Phase Change Source.
        void validate(const GunnsFluidPhaseChangeSourceConfigData& configData) const;
        /// @brief Checks for valid implementation-specific port node assignment
        virtual bool checkSpecificPortRules(const int port, const int node) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidPhaseChangeSource(const GunnsFluidPhaseChangeSource&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidPhaseChangeSource& operator=(const GunnsFluidPhaseChangeSource&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  power  (W)  Input power to the link.
///
/// @return   void
///
/// @details  Sets the input power value of this GunnsFluidPhaseChangeSource to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidPhaseChangeSource::setPowerInput(const double power)
{
    mPowerInput = power;
}

#endif
