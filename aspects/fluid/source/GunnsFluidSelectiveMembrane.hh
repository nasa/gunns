#ifndef GunnsFluidSelectiveMembrane_EXISTS
#define GunnsFluidSelectiveMembrane_EXISTS

/**
@file
@brief    GUNNS Selective Membrane Model declarations

@defgroup  TSM_GUNNS_FLUID_SOURCE_SELECTIVE_MEMBRANE  GUNNS Selective Membrane Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling a selective absorption membrane.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidSelectiveMembrane.o))

 PROGRAMMERS:
- ((Mike Moore)   (L-3 Communications) (Initial) (2015-10)
   (Jason Harvey) (L-3 Communications) (Initial) (2015-10))

@{
*/

#include "core/GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Selective Membrane Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Selective
///           Membrane configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSelectiveMembraneConfigData : public GunnsFluidLinkConfigData
{
    public:
        double                     mMaxConductance;  /**< (m2)       trick_chkpnt_io(**) Max conductance of the flow-thru path. */
        double                     mAbsorptionCoeff; /**< (kg/s/kPa) trick_chkpnt_io(**) Absorption coefficient of the membrane. */
        FluidProperties::FluidType mExternalType;    /**< (--)       trick_chkpnt_io(**) Membrane absorbed fluid type in the external vent. */
        FluidProperties::FluidType mInternalType;    /**< (--)       trick_chkpnt_io(**) Membrane absorbed fluid type in the internal bulk flow. */
        /// @brief  Default constructs this Selective Membrane configuration data with arguments.
        GunnsFluidSelectiveMembraneConfigData(
                const std::string&               name            = "",
                GunnsNodeList*                   nodes           = 0,
                const double                     maxConductance  = 0.0,
                const double                     absorptionCoeff = 0.0,
                const FluidProperties::FluidType externalType    = FluidProperties::NO_FLUID,
                const FluidProperties::FluidType internalType    = FluidProperties::NO_FLUID);
        /// @brief  Default destructs this Selective Membrane configuration data.
        virtual ~GunnsFluidSelectiveMembraneConfigData();
        /// @brief  Copy constructs this Selective Membrane configuration data.
        GunnsFluidSelectiveMembraneConfigData(const GunnsFluidSelectiveMembraneConfigData& that);

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSelectiveMembraneConfigData& operator =(const GunnsFluidSelectiveMembraneConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Selective Membrane Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Selective
///           Membrane input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSelectiveMembraneInputData : public GunnsFluidLinkInputData
{
    public:
        /// @brief  Default constructs this Selective Membrane input data with arguments.
        GunnsFluidSelectiveMembraneInputData(const bool   malfBlockageFlag  = false,
                                             const double malfBlockageValue = 0.0);
        /// @brief  Copy constructs this Selective Membrane input data.
        GunnsFluidSelectiveMembraneInputData(const GunnsFluidSelectiveMembraneInputData& that);
        /// @brief  Default destructs this Selective Membrane input data.
        ~GunnsFluidSelectiveMembraneInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSelectiveMembraneInputData& operator=(const GunnsFluidSelectiveMembraneInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Selective Membrane
///
/// @details  This link models a selective membrane absorber tube.  The tube wall is a membrane that
///           absorbs and passes a specific fluid constituent (i.e. water) between the bulk flow
///           thru the tube and an external "vent".  The absorption rate is proportional to a given
///           coefficient and the relative partial pressures of the selectively absorbed fluid
///           between the inside and outside of the tube.  The absorbed membrane flow can go in
///           either direction, either to or from the external vent, depending on the relative
///           partial pressures across the membrane.  The vent is always in the gas phase, and the
///           bulk tube thru-flow can be either gas or liquid.  In the case of liquid tube flow,
///           transport across the membrane undergoes a phase change, with the corresponding heat of
///           phase change being modeled.  This can model things like a Nafion humidifier/de-
///           humidifier, or a water membrane evaporator cooler, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSelectiveMembrane : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSelectiveMembrane);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfMembraneDegradeFlag;  /**< (--) Malfunction flag to activate the membrane degradation. */
        double mMalfMembraneDegradeValue; /**< (--) (0-1) Malfunction fractional value to degrade the membrane. */
        /// @}

        /// @brief  Default constructs this Selective Membrane model.
        GunnsFluidSelectiveMembrane();
        /// @brief  Default destructs this Selective Membrane model.
        virtual ~GunnsFluidSelectiveMembrane();
        /// @brief  Initializes this Selective Membrane model with configuration and input data.
        void initialize(const GunnsFluidSelectiveMembraneConfigData& configData,
                        const GunnsFluidSelectiveMembraneInputData&  inputData,
                        std::vector<GunnsBasicLink*>&                networkLinks,
                        const int                                    port0,
                        const int                                    port1,
                        const int                                    port2);
        /// @brief  Updates the state of this Selective Membrane.
        virtual void step(const double dt);
        /// @brief  Updates between nodes resulting from the network solution.
        virtual void computeFlows(const double dt);
        /// @brief  Transports flows between nodes resulting from the network solution.
        virtual void transportFlows(const double dt);
        /// @brief  Sets and resets the membrane degrade malfunction.
        void setMalfMembraneDegrade(const bool flag = false, const double value = 0.0);

    protected:
        double                     mMaxConductance;        /**<    (m2)           trick_chkpnt_io(**) Max conductance of the flow-thru path. */
        double                     mAbsorptionCoeff;       /**<    (kg/s/kPa)     trick_chkpnt_io(**) Absorption coefficient of the membrane. */
        FluidProperties::FluidType mExternalType;          /**< *o (--)           trick_chkpnt_io(**) Membrane absorbed fluid type in the external vent. */
        FluidProperties::FluidType mInternalType;          /**< *o (--)           trick_chkpnt_io(**) Membrane absorbed fluid type in the internal bulk flow. */
        double                     mEffectiveConductance;  /**< *o (m2)           trick_chkpnt_io(**) Effective conductance of the flow-thru path. */
        double                     mSystemConductance;     /**< *o (kg*mol/s/kPa) trick_chkpnt_io(**) Limited molar conductance of the flow-thru path. */
        double                     mExternalPartialP;      /**< *o (kPa)          trick_chkpnt_io(**) Partial pressure of the absorbed fluid in the external vent. */
        double                     mInternalPartialP[2];   /**< *o (kPa)          trick_chkpnt_io(**) Partial pressure of the absorbed fluid in the internal bulk flow ports. */
        double                     mExternalSaturationP;   /**< *o (kPa)          trick_chkpnt_io(**) Saturation pressure of the absorbed fluid in the external vent. */
        double                     mInternalSaturationP;   /**< *o (kPa)          trick_chkpnt_io(**) Saturation pressure of the absorbed fluid in the internal bulk flow. */
        bool                       mExternalSaturated;     /**< *o (--)           trick_chkpnt_io(**) Absorption fluid in the external vent is saturated or super-saturated. */
        bool                       mInternalSaturated;     /**< *o (--)           trick_chkpnt_io(**) Absorption fluid in the internal bulk flow is saturated or super-saturated. */
        bool                       mPhaseChange;           /**< *o (--)           trick_chkpnt_io(**) Phase change is occurring in flow across the membrane. */
        double                     mMembraneDeltaP[2];     /**< *o (kPa)          trick_chkpnt_io(**) Membrane delta-pressure of the selectively absorbed fluid at the internal bulk flow ports. */
        double                     mMembraneFlowRate;      /**< *o (kg/s)         trick_chkpnt_io(**) Membrane transport rate of the selectively absorbed fluid to the gas vent. */
        double                     mPhaseChangeHeat;       /**< *o (W)            trick_chkpnt_io(**) Heat of phase change from membrane transport. */
        PolyFluid*                 mExternalMembraneFluid; /**< *o (--)           trick_chkpnt_io(**) Membrane absorbed fluid output/input to/from the vent. */
        PolyFluid*                 mInternalMembraneFluid; /**< *o (--)           trick_chkpnt_io(**) Membrane absorbed fluid output/input to/from the downstream bulk flow node. */
        /// @brief  Validates the initialization inputs of this Selective Membrane model.
        void validate(const GunnsFluidSelectiveMembraneConfigData& configData) const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief  Linearizes the fluid conductance for the admittance matrix.
        double       linearizeConductance();
        /// @brief  Updates the admittance matrix terms of the link.
        void         buildAdmittance();
        /// @brief Computes molar flux rate through the flow-thru path of the link.
        void         computeFlux();
        /// @brief Computes hydraulic power gain through the flow-thru path of the link.
        virtual void computePower();
        /// @brief  Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;
        /// @brief  Returns the partial pressure of the given fluid type at the given port.
        double       computePortPartialP(const int port, const FluidProperties::FluidType type) const;

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 3};
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSelectiveMembrane(const GunnsFluidSelectiveMembrane&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSelectiveMembrane& operator=(const GunnsFluidSelectiveMembrane&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Hydraulic power in the link is -volumetric flow * potential drop.  Note that this is
///           defined as power gain, such as from a pump.  Pressure-producing links such as pumps
///           and fans will have positive power gain, and the power in dissipating links such as
///           pipes, etc. will be negative.  Since potential is in units of kPa, we must convert it
///           to Pa to express power in Watts.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSelectiveMembrane::computePower()
{
    mPower = -UnitConversion::PA_PER_KPA * mVolFlowRate * getDeltaPotential();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the Admittance Matrix for the link.   This is the same as a regular fluid
///           conductor - a conductive path between ports 0 and 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSelectiveMembrane::buildAdmittance()
{
    if (fabs(mAdmittanceMatrix[0] - mSystemConductance) > 0.0) {
        mAdmittanceMatrix[0]   =  mSystemConductance;
        mAdmittanceMatrix[1]   = -mAdmittanceMatrix[0];
        mAdmittanceMatrix[3]   = -mAdmittanceMatrix[0];
        mAdmittanceMatrix[4]   =  mAdmittanceMatrix[0];
        mAdmittanceUpdate = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  port  (--)  Link port number to evaluate
/// @param[in]  type  (--)  Fluid type to evaluate
///
/// @returns  double  (kPa)  Partial pressure of the given fluid type at the given port.
///
/// @details  Returns the partial pressure of the given fluid type at the given port.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSelectiveMembrane::computePortPartialP(const int port, const FluidProperties::FluidType type) const
{
    return mPotentialVector[port] * mNodes[port]->getContent()->getMoleFraction(type);
}

#endif
