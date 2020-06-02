#ifndef GunnsFluidSublimator_EXISTS
#define GunnsFluidSublimator_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_SOURCE_SUBLIMATOR  Sublimator Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Sublimator link model.)

REQUIREMENTS:
- (TBD)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (Heat balance is always positive (sublimator always cooling the suit).
- We don't simulate deposition (frosting).  If the heat balance from the thermal aspect ever
  goes negative, our results will be inaccurate.
- The vent node is gas only - we will never flow actual liquid to it, even if we're simulating
  liquid breakthrough to the vent.
- We assume liquid in the sublimator is always just above the freezing point, and we neglect
  the extra heat required to raise or lower it from the feed or structural temperatures.)

 LIBRARY DEPENDENCY:
- ((GunnsFluidSublimator.o))

 PROGRAMMERS:
- ((Jason Harvey) (L3) (Initial) (2012-02))

@{
*/

#include "core/GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sublimator Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Sublimator link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSublimatorConfigData : public GunnsFluidLinkConfigData
{
    public:
        /// @details The sublimator class currently only supports water, so this should only be set
        ///          to GUNNS_H2O until other types are supported.
        FluidProperties::FluidType mGasType; /**< (--)    trick_chkpnt_io(**) Gas phase of the operating fluid */
        /// @details Since we only support water, use 2257.0 kJ/kg for now.
        double mHeatOfVaporization;          /**< (kJ/kg) trick_chkpnt_io(**) Heat of vaporization of the operating fluid */
        /// @details Since we only support water, use 334.0 kJ/kg for now.
        double mHeatOfFusion;                /**< (kJ/kg) trick_chkpnt_io(**) Heat of fusion (melting) of the operating fluid */
        /// @details The freezing point of the operating fluid.  The same value should be used for
        ///          the GunnsThermalPotential's mSourcePotential in the thermal network.  Since we
        ///          only support water, use 273.15 K for now.
        double mTriplePointTemperature;      /**< (K)     trick_chkpnt_io(**) Temperature of the operating fluid triple point */
        double mMaxIceMass;                  /**< (kg)    trick_chkpnt_io(**) Maximum mass of ice the sublimator can form */
        /// @details When feed is first supplied to the empty sublimator, liquid breaks through the
        ///          ice plate at a high rate until ice can form and cover the entire plate area.
        ///          This term configures how much ice is needed before we consider the entire plate
        ///          covered.  A larger fraction results in the breakthrough period lasting longer.
        double mIceCoverageFraction;         /**< (--)    trick_chkpnt_io(**) (0-1) fraction of maximum ice mass for complete plate coverage */
        /// @details Governs the temperature rise of the sublimator metal mass in the thermal
        ///          network in response to heat flux due to sublimation.  The higher this value,
        ///          the nearer the sublimator temperatures will remain to the freezing point.
        double mMaxThermalConductivity;      /**< (J/s/K) trick_chkpnt_io(**) Maximum conductivity of the GunnsThermalPotential */
        /// @details Governs the rate at which liquid will break through the plate when
        ///          there is no ice pack formed.
        double mPlateConductivity;           /**< (m2)    trick_chkpnt_io(**) Effective conductivity of plate with no ice coverage */
        double mThermalConductivityGain;     /**< (--)    trick_chkpnt_io(**) (0-1) Recommend 0.01, thermal conductivity filter gain */
        /// @brief Default constructs this Sublimator model configuration data with arguments.
        GunnsFluidSublimatorConfigData(const std::string& name                    = "",
                                       GunnsNodeList*     nodes                   = 0,
                                       const FluidProperties::FluidType gasType   = FluidProperties::NO_FLUID,
                                       const double       heatOfVaporization      = 0.0,
                                       const double       heatOfFusion            = 0.0,
                                       const double       triplePointTemperature  = 0.0,
                                       const double       maxIceMass              = 0.0,
                                       const double       iceCoverageFraction     = 0.0,
                                       const double       maxThermalConductivity  = 0.0,
                                       const double       plateConductivity       = 0.0,
                                       const double       thermalConductivityGain = 0.0);
        /// @brief Default destructs this Sublimator model configuration data.
        virtual ~GunnsFluidSublimatorConfigData();
        /// @brief Copy constructs this Sublimator model configuration data.
        GunnsFluidSublimatorConfigData(const GunnsFluidSublimatorConfigData& that);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSublimatorConfigData& operator =(const GunnsFluidSublimatorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sublimator Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Sublimator link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSublimatorInputData : public GunnsFluidLinkInputData
{
    public:
        double mIceMass;               /**< (kg)   Initial mass of ice in the sublimator */
        double mStructureTemperature;  /**< (K)    Initial temperature of the sublimator structure */
        bool   mMalfFeedRateBiasFlag;  /**< (--)   Initial feed rate bias malf activation flag*/
        double mMalfFeedRateBiasValue; /**< (kg/s) Initial feed rate bias malf value */
        /// @brief Default constructs this Sublimator model input data with arguments.
        GunnsFluidSublimatorInputData(const double iceMass              = 0.0,
                                      const double structureTemperature = 0.0);
        /// @brief Copy constructs this Sublimator model input data.
        GunnsFluidSublimatorInputData(const GunnsFluidSublimatorInputData& that);
        /// @brief Default destructs this Sublimator model input data.
        ~GunnsFluidSublimatorInputData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSublimatorInputData& operator=(const GunnsFluidSublimatorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sublimator Model
///
/// @details  This is the fluid aspect of a sublimator.  The sublimator takes in liquid coolant from
///           a feed source and exposes it to vacuum in a porous plate.  The liquid begins to boil
///           in the plate due to vacuum, and this pulls enough heat out of the liquid that the
///           remaining liquid begins to freeze, creating an ice layer (called the ice pack) in the
///           plate.  Thereafter, the ice pack sublimates in the vacuum, providing a heat sink for
///           attached heat exchangers.  More feed coolant is pulled in to maintain the ice pack.
///
///           Currently, only water is supported by this model.  The fluid properties needed by this
///           model, notably saturation pressure calculations and latent heats of phase change, are
///           not driven by the PolyFluid class, so they are hard-coded here.
///
///           A sublimator will typically be attached to one or more heat exchangers as their heat
///           sink.  These can be modeled with GunnsFluidHeatExchanger links in a fluid network, but
///           they do not interface directly with this sublimator class - rather they transfer their
///           heat to a GunnsThermalCapacitor in a Gunns thermal network, which models the metal
///           mass of the combined sublimator/HX structure.
///
///           This sublimator class is designed to interface with the GunnsThermalCapacitor as well
///           as a GunnsThermalPotential attached to the same thermal node.  The potential link is
///           used to control the temperature of the thermal node, providing the heat sink for the
///           combined model.
///
///           This sublimator link should be hooked up to a liquid feed node at port 0, and port 1
///           should be hooked up to the gas vent.  Optionally, port 1 can be on the ground node if
///           a perfect vacuum vent is desired.  The link detects liquid pressure from the feed, and
///           if vapor pressure is low enough in the vent, freezing & sublimation occurs, and this
///           link drives the effective conductivity of the thermal potential link to modulate the
///           heat sink effect.  Feed liquid is pulled in from the fluid network as a conductivity,
///           and the ice mass is tracked internally.  Sublimated vapor is output to the vent node
///           as a flux source.  NOTE that the input & output ports have different phases, and they
///           should not be mixed up.  The vent node must be in the gas area of a 2-phase network.
///
///           Failure scenarios include failure to freeze or sublimate due to a saturated vent or
///           insufficient vacuum.  Liquid breakthrough (liquid venting through the plate & flooding
///           the vent) is modeled as a high demand on the feed source, but we don't actually add
///           the liquid to the vent node (Gunns can't support mixed-phases in a node).  Heat from
///           the thermal aspect normally sublimates the ice pack, but if sublimation cannot occur,
///           the heat melts the ice pack instead.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSublimator : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSublimator);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfFeedRateBiasFlag;  /**< (--)   Malfunction flag to activate the feed rate bias */
        double mMalfFeedRateBiasValue; /**< (kg/s) Malfunction value to bias the feed rate */
        /// @}

        /// @brief Default constructs this Sublimator model.
        GunnsFluidSublimator();
        /// @brief Default destructs this Sublimator model.
        virtual ~GunnsFluidSublimator();
        /// @brief Initializes this Sublimator model with configuration and input data.
        void initialize(const GunnsFluidSublimatorConfigData& configData,
                        const GunnsFluidSublimatorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&         networkLinks,
                        const int                             port0,
                        const int                             port1);
        /// @brief Step method for updating the link
        virtual void step(const double dt);
        /// @brief Computes the flows through the link
        virtual void computeFlows(const double dt);
        /// @brief Transports the flows through the link
        virtual void transportFlows(const double dt);
        /// @brief Returns the mass of ice in the sublimator
        double getIceMass() const;
        /// @brief  Sets and resets the feed rate bias malfunction
        void setMalfFeedRateBias(const bool flag = false, const double value = 0.0);
        /// @brief  Sets the Heat Balance that goes to the phase change
        void setHeatBalance(const double balanceOfHeat);

    protected:
        FluidProperties::FluidType mGasType; /**< *o (--)           trick_chkpnt_io(**) Gas phase of the operating fluid */
        double mHeatOfVaporization;          /**<    (kJ/kg)        trick_chkpnt_io(**) Heat of vaporization of the operating fluid */
        double mHeatOfFusion;                /**<    (kJ/kg)        trick_chkpnt_io(**) Heat of fusion (melting) of the operating fluid */
        double mTriplePointTemperature;      /**<    (K)            trick_chkpnt_io(**) Temperature of the operating fluid triple point */
        double mMaxIceMass;                  /**<    (kg)           trick_chkpnt_io(**) Maximum mass of ice the sublimator can form */
        double mIceCoverageFraction;         /**<    (--)           trick_chkpnt_io(**) (0-1) fraction of maximum ice mass for complete plate coverage */
        double mMaxThermalConductivity;      /**<    (J/s/K)        trick_chkpnt_io(**) Maximum conductivity of the GunnsThermalPotential */
        double mPlateConductivity;           /**<    (m2)           trick_chkpnt_io(**) Effective conductivity of plate with no ice coverage */
        double mThermalConductivityGain;     /**<    (--)           trick_chkpnt_io(**) (0-1) thermal conductivity filter gain */
        double mIceMass;                     /**<    (kg)                               Mass of ice in the sublimator */
        double mStructureTemperature;        /**<    (K)                                Temperature of the sublimator structure */
        double mHeatBalance;                 /**<    (J/s)                              Balance of heat going to phase change */
        double mHeatOfSublimation;           /**<    (kJ/kg)        trick_chkpnt_io(**) Heat of sublimation of the operating fluid */
        double mThermalConductivity;         /**<    (J/s/K)                            Conductivity of the GunnsThermalPotential */
        double mVentRelativeHumidity;        /**<    (--)           trick_chkpnt_io(**) Relative humidity of the vapor vent */
        double mSystemConductance;           /**<    (kg*mol/kPa/s) trick_chkpnt_io(**) Limited molar conductance */
        bool   mLiquidBreakthrough;          /**<    (--)           trick_chkpnt_io(**) Liquid breaking through the plate and flooding vent */
        PolyFluid* mVentFluid;               /**<    (--)           trick_chkpnt_io(**) Fluid output to the vents */
        /// @brief Validates the initialization inputs of this Sublimator model.
        void validate(const GunnsFluidSublimatorConfigData& configData,
                      const GunnsFluidSublimatorInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Derives the initialization data of this Sublimator model.
        void derive();
        /// @brief Checks for valid Sublimator-specific port node assignment
        virtual bool checkSpecificPortRules(const int port, const int node) const;

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        ///
        ///          The ports are:
        ///          0 - liquid feed
        ///          1 - vapor vent
        enum {NPORTS = 2};
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSublimator(const GunnsFluidSublimator&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSublimator& operator=(const GunnsFluidSublimator&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg) The mass of ice in the sublimator.
///
/// @details  Getter method returns the mass of ice in the sublimator.
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSublimator::getIceMass() const
{
    return mIceMass;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Setter method for setting the Heat Balance of the sublimator phase change
////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSublimator::setHeatBalance(const double balanceOfHeat)
{
    mHeatBalance = balanceOfHeat;
}

#endif
