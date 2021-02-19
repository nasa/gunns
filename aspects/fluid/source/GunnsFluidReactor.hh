#ifndef GunnsFluidReactor_EXISTS
#define GunnsFluidReactor_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_SOURCE_REACTOR  Chemical Reactor Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling a chemical reactor.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- (
    (Reactions are performed serially. If a compound is involved in multiple reactions, the order
     in which the reactions are performed may affect the aggregate result.)
  )

 LIBRARY DEPENDENCY:
- ((GunnsFluidReactor.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (virtual methods) (2012-06))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "properties/ChemicalReaction.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Chemical Reactor Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the
///           GUNNS Fluid Reactor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidReactorConfigData : public GunnsFluidConductorConfigData
{
    public:
        const DefinedChemicalReactions* mReactions;          /**< (--)   trick_chkpnt_io(**) Pointer to the defined Chemical Reactions. */
        ChemicalReaction::Type*         mReactionTypes;      /**< (--)   trick_chkpnt_io(**) Array of Chemical Reaction types. */
        const int                       mNReactions;         /**< (--)   trick_chkpnt_io(**) Number of Chemical Reaction types. */
        const DefinedChemicalCompounds* mCompounds;          /**< (--)   trick_chkpnt_io(**) Pointer to the defined Chemical Compounds. */
        ChemicalCompound::Type*         mCompoundTypes;      /**< (--)   trick_chkpnt_io(**) Array of Chemical Compound types. */
        const int                       mNCompounds;         /**< (--)   trick_chkpnt_io(**) Number of Chemical Compound types. */
        double                          mThermalLength;      /**< (m)    trick_chkpnt_io(**) Tube length for thermal convection. */
        double                          mThermalDiameter;    /**< (m)    trick_chkpnt_io(**) Tube inner diameter for thermal convection. */
        double                          mSurfaceRoughness;   /**< (m)    trick_chkpnt_io(**) Tube wall surface roughness for thermal convection. */
        double*                         mReactantEfficiency; /**< (--)   trick_chkpnt_io(**) Optional array of reaction reactant efficiency overrides. */
        double*                         mReagentEfficiency;  /**< (1/kg) trick_chkpnt_io(**) Optional array of reaction reagent efficiency per unit mass. */
        /// @brief    Default constructs this Chemical Reactor configuration data.
        GunnsFluidReactorConfigData(const std::string&              name                 = "",
                                    GunnsNodeList*                  nodes                = 0,
                                    const double                    maxConductivity      = 0.0,
                                    const double                    expansionScaleFactor = 0.0,
                                    const DefinedChemicalReactions* reactions            = 0,
                                    ChemicalReaction::Type*         reactionTypes        = 0,
                                    const int                       nReactions           = 0,
                                    const DefinedChemicalCompounds* compounds            = 0,
                                    ChemicalCompound::Type*         compoundTypes        = 0,
                                    const int                       nCompounds           = 0,
                                    const double                    thermalLength        = 0.0,
                                    const double                    thermalDiameter      = 0.0,
                                    const double                    surfaceRoughness     = 0.0);
        /// @brief    Default destructs this Chemical Reactor configuration data.
        virtual ~GunnsFluidReactorConfigData();
        /// @brief    Frees dynamic memory.
        void cleanup();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidReactorConfigData& operator =(const GunnsFluidReactorConfigData&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidReactorConfigData(const GunnsFluidReactorConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Chemical Reactor Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the
///           GUNNS Fluid Reactor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidReactorInputData : public GunnsFluidConductorInputData
{
    public:
        /// @brief    Default constructs this Chemical Reactor input data with arguments.
        GunnsFluidReactorInputData(const bool   malfBlockageFlag  = false,
                                   const double malfBlockageValue = 0.0,
                                   double*      totalMasses       = 0,
                                   const double wallTemperature   = 0.0);
        /// @brief    Copy constructs this Chemical Reactor input data.
        GunnsFluidReactorInputData(const GunnsFluidReactorInputData& that);
        /// @brief    Default destructs this Chemical Reactor input data.
        ~GunnsFluidReactorInputData();
        /// --       Array of Chemical Compound total masses not taken from atmosphere.
        double*      mTotalMasses;      /**< (kg) Array of Chemical Compound total masses not taken from atmosphere. */
        double       mWallTemperature;  /**< (K)  Tube wall temperature for thermal convection. */
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidReactorInputData& operator=(const GunnsFluidReactorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    A typedef for a class containing the Chemical Reaction data that is mapped by type.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ReactionState {
    TS_MAKE_SIM_COMPATIBLE(ReactionState);
    public:
        ChemicalReaction::Type  mType;               /**< *o (--) trick_chkpnt_io(**) Type of this Chemical Reaction. */
        const ChemicalReaction* mReaction;           /**< *o (--) trick_chkpnt_io(**) Pointer to the properties for this Chemical Reaction. */
        int                     mReactantIndex;      /**< *o (--) trick_chkpnt_io(**) Index of this Chemical Reaction reactant in Chemical Compound array. */
        int                     mReagentIndex;       /**< *o (--) trick_chkpnt_io(**) Index of this Chemical Reaction reagent in Chemical Compound array. */
        int                     mProduct1Index;      /**< *o (--) trick_chkpnt_io(**) Index of this Chemical Reaction product 1 in Chemical Compound array. */
        int                     mProduct2Index;      /**< *o (--) trick_chkpnt_io(**) Index of Chemical Reaction product 2 in Chemical Compound array. */
        double                  mReactantEfficiency; /**<    (--) trick_chkpnt_io(**) Optional override of reactant efficiency for this Chemical Reaction. */
        double                  mReagentEfficiency;  /**<    (--) trick_chkpnt_io(**) Optional override of reagent efficiency for this Chemical Reaction. */
        /// @brief  Default constructs this Chemical Reaction State.
        ReactionState();
        /// @brief  Default destructs this Chemical Reaction State.
        ~ReactionState();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        ReactionState(const ReactionState&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        ReactionState& operator=(const ReactionState&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    A typedef for a struct containing the Chemical Compound data that is mapped by type.
////////////////////////////////////////////////////////////////////////////////////////////////////
class CompoundState {
    TS_MAKE_SIM_COMPATIBLE(CompoundState);
    public:
        ChemicalCompound::Type  mType;      /**< *o (--) trick_chkpnt_io(**) Type of this Chemical Compound. */
        const ChemicalCompound* mCompound;  /**< *o (--) trick_chkpnt_io(**) Pointer to properties for this Chemical Compound. */
        int                     mIndex;     /**< *o (--) trick_chkpnt_io(**) Constituent index of this Chemical Compound or -1 if not in atmosphere. */
        double                  mTotalMass; /**<    (kg)                     Total mass of this Chemical Compound. */
        double                  mMass;      /**<    (kg)                     Mass of this Chemical Compound produced or consumed this cycle. */
        /// @brief  Default constructs this Chemical Compound State.
        CompoundState();
        /// @brief  Default destructs this Chemical Compound State.
        ~CompoundState();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        CompoundState(const CompoundState&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        CompoundState& operator=(const CompoundState&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Chemical Reactor
///
/// @details  Class for modeling a GUNNS Fluid Reactor link model. A chemical reactor is a
///           conductive link masquerading as a source link that modifies the input fluid
///           constituents by consuming reactants and reagents and producing products.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidReactor : public GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidReactor);
    public:
        /// @brief    Default constructs this Chemical Reactor.
        GunnsFluidReactor();
        /// @brief    Default destructs this Chemical Reactor.
        virtual ~GunnsFluidReactor();
        /// @brief    Initializes this Chemical Reactor with configuration and input data.
        void initialize(      GunnsFluidReactorConfigData& configData,
                        const GunnsFluidReactorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&      networkLinks,
                        const int                          port0,
                        const int                          port1);
        /// @brief Moves the fluid from one port to the other
        virtual void     transportFluid(const bool forcedOutflow,
                                        const int  fromPort = 0,
                                        const int  toPort   = 1);
        /// @brief    Sets the thermal surface area of this Chemical Reactor.
        void   setThermalSurfaceArea(const double value);
        /// @brief    Sets the wall temperature of this Chemical Reactor.
        void   setWallTemperature(const double value);

    protected:
        ReactionState* mReactions;          /**<    (--)   trick_chkpnt_io(**) Array of Chemical Reactions data for this Chemical Reactor. */
        int            mNReactions;         /**< *o (--)   trick_chkpnt_io(**) Number of Chemical Reactions for this Chemical Reactor. */
        CompoundState* mCompounds;          /**<    (--)   trick_chkpnt_io(**) Array of Chemical Compounds data for this Chemical Reactor. */
        int            mNCompounds;         /**< *o (--)   trick_chkpnt_io(**) Number of Chemical Compounds for this Chemical Reactor. */
        double         mThermalDiameter;    /**<    (m)    trick_chkpnt_io(**) Tube inner diameter for thermal convection. */
        double         mThermalSurfaceArea; /**<    (m2)   trick_chkpnt_io(**) Tube inner surface area for thermal convection. */
        double         mThermalROverD;      /**<    (--)   trick_chkpnt_io(**) Tube surface roughness over diameter for thermal convection. */
        double         mWallTemperature;    /**<    (K)                        Tube wall temperature for thermal convection  (simbus input from thermal aspect). */
        double         mFluidTemperature;   /**<    (K)    trick_chkpnt_io(**) Temperature of the fluid in the reactor. */
        double         mWallHeatFlux;       /**<    (W)                        Convection heat flow from the fluid to the tube wall (simbus output to thermal aspect). */
        double         mReactantsFlowRate;  /**<    (kg/s) trick_chkpnt_io(**) Total mass rate of consumed fluid constituents. */
        double         mProductsFlowRate;   /**<    (kg/s) trick_chkpnt_io(**) Total mass rate of produced fluid constituents. */
        PolyFluid*     mReactantsFluid;     /**<    (--)   trick_chkpnt_io(**) Pointer to an internal fluid for the consumed fluid constituents. */
        PolyFluid*     mProductsFluid;      /**<    (--)   trick_chkpnt_io(**) Pointer to an internal fluid for the produced fluid constituents. */
        double         mProducedHeat;       /**<    (J)    trick_chkpnt_io(**) Total heat produced from all reactions. */
        /// @brief    Validates the initialization inputs of this Chemical Reactor.
        void validate(const GunnsFluidReactorConfigData& configData,
                      const GunnsFluidReactorInputData&  inputData);
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Validates the reaction compounds for this Chemical Reactor.
        void checkCompound(const GunnsFluidReactorConfigData& configData,
                           const ChemicalCompound::Type&      compound);
        /// @brief    Initializes the derived attributes of this Chemical Reactor.
        void derive(const GunnsFluidReactorConfigData& configData,
                    const GunnsFluidReactorInputData&  inputData);
        /// @brief    Updates the internal fluid.
        virtual void updateFluid(const double dt, const double flowrate);
        /// @brief    Updates the Chemical Compound total masses from the input atmosphere.
        void inputFromAtmosphere(const double dt);
        /// @brief    Performs the reactions.
        void react(const double dt);
        /// @brief    Updates the internal fluid from the Chemical Compound masses.
        void outputToAtmosphere(const double dt);
        /// @brief    Updates the fluid temperature.
        virtual void updateTemperature(const double dt);
        /// @brief    Adds/removes reaction heat from the reactant fluid.
        virtual void addRxnHeat(const double dt);
        /// @brief    Computes the reaction efficiency.
        virtual double computeEfficiency(const int i);

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidReactor(const GunnsFluidReactor&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidReactor& operator=(const GunnsFluidReactor&);
};

/// @}

#endif
