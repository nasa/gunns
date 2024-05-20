/*
@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   (
    (core/GunnsFluidSource.o)
    (properties/ChemicalReaction.o)
   )
*/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

#include "GunnsFluidReactor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      name                  (--)  Name of object.
/// @param[in,out]  nodes                 (--)  Pointer to nodes.
/// @param[in]      maxConductivity       (m2)  Maximum conductivity.
/// @param[in]      expansionScaleFactor  (--)  Scale factor for isentropic gas cooling.
/// @param[in]      reactions             (--)  Pointer to Defined Chemical Reactions.
/// @param[in]      reactionTypes         (--)  Array of Chemical Reaction Types.
/// @param[in]      nReactions            (--)  Number of Chemical Reactions.
/// @param[in]      compounds             (--)  Pointer to Defined Chemical Compounds.
/// @param[in]      compoundTypes         (--)  Array of Chemical Compound types.
/// @param[in]      nCompounds            (--)  Number of Chemical Compounds.
/// @param[in]      thermalLength         (m)   Tube length for thermal convection.
/// @param[in]      thermalDiameter       (m)   Tube inner diameter for thermal convection.
/// @param[in]      surfaceRoughness      (m)   Tube wall surface roughness for thermal convection.
///
/// @details  Default constructs this GUNNS Fluid Reactor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidReactorConfigData::GunnsFluidReactorConfigData(const std::string&              name,
                                                         GunnsNodeList*                  nodes,
                                                         const double                    maxConductivity,
                                                         const double                    expansionScaleFactor,
                                                         const DefinedChemicalReactions* reactions,
                                                         ChemicalReaction::Type*         reactionTypes,
                                                         const int                       nReactions,
                                                         const DefinedChemicalCompounds* compounds,
                                                         ChemicalCompound::Type*         compoundTypes,
                                                         const int                       nCompounds,
                                                         const double                    thermalLength,
                                                         const double                    thermalDiameter,
                                                         const double                    surfaceRoughness)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mReactions(reactions),
    mReactionTypes(0),
    mNReactions(nReactions),
    mCompounds(compounds),
    mCompoundTypes(0),
    mNCompounds(nCompounds),
    mThermalLength(thermalLength),
    mThermalDiameter(thermalDiameter),
    mSurfaceRoughness(surfaceRoughness),
    mReactantEfficiency(0),
    mReagentEfficiency(0)
{
    /// - Allocate and initialize the array of reaction types.
    if (nReactions > 0 && reactionTypes != 0) {
        TS_NEW_PRIM_ARRAY_EXT(mReactionTypes, nReactions, ChemicalReaction::Type, name + ".mReactionTypes");
        for (int i = 0; i < nReactions; ++i) {
            mReactionTypes[i] = reactionTypes[i];
        }
    }
    /// - Allocate and initialize the array of compound types.
    if (nCompounds > 0 && compoundTypes != 0) {
        TS_NEW_PRIM_ARRAY_EXT(mCompoundTypes, nCompounds, ChemicalCompound::Type, name + ".mCompoundTypes");
        for (int i = 0; i < nCompounds; ++i) {
            mCompoundTypes[i] = compoundTypes[i];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Reactor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidReactorConfigData::~GunnsFluidReactorConfigData()
{
    cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes dynamically allocated arrays.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactorConfigData::cleanup()
{
    /// - Deallocate the array of compound types.
    TS_DELETE_ARRAY(mCompoundTypes);

    /// - Deallocate the array of reaction types.
    TS_DELETE_ARRAY(mReactionTypes);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      malfBlockageFlag   (--)    Blockage malfunction flag.
/// @param[in]      malfBlockageValue  (--)    Blockage malfunction fractional value (0-1).
/// @param[in]      totalMasses        (--)    Array of Chemical Compound total masses.
/// @param[in]      wallTemperature    (K)     Tube wall temperature for thermal convection.
///
/// @details  Default constructs this GUNNS Fluid Reactor link model input data with arguments
///           and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidReactorInputData::GunnsFluidReactorInputData(const bool   malfBlockageFlag,
                                                       const double malfBlockageValue,
                                                       double*      totalMasses,
                                                       const double wallTemperature)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mTotalMasses(totalMasses),
    mWallTemperature(wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that  (--)  Source to copy from.
///
/// @details  Copy constructs this GUNNS Fluid Reactor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidReactorInputData::GunnsFluidReactorInputData(const GunnsFluidReactorInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mTotalMasses(that.mTotalMasses),
    mWallTemperature(that.mWallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Reactor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidReactorInputData::~GunnsFluidReactorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Chemical Reaction State.
////////////////////////////////////////////////////////////////////////////////////////////////////
ReactionState::ReactionState()
    :
    mType(),
    mReaction(0),
    mReactantIndex(0),
    mReagentIndex(0),
    mProduct1Index(0),
    mProduct2Index(0),
    mReactantEfficiency(0.0),
    mReagentEfficiency(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Chemical Reaction State.
////////////////////////////////////////////////////////////////////////////////////////////////////
ReactionState::~ReactionState()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Chemical Compound State.
////////////////////////////////////////////////////////////////////////////////////////////////////
CompoundState::CompoundState()
    :
    mType(),
    mCompound(0),
    mIndex(0),
    mTotalMass(0.0),
    mMass(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Chemical Compound State.
////////////////////////////////////////////////////////////////////////////////////////////////////
CompoundState::~CompoundState()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Reactor link model with obviously invalid data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidReactor::GunnsFluidReactor()
    :
    GunnsFluidConductor(),
    mReactions(0),
    mNReactions(0),
    mCompounds(0),
    mNCompounds(0),
    mThermalDiameter(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mWallTemperature(0.0),
    mFluidTemperature(0.0),
    mWallHeatFlux(0.0),
    mReactantsFlowRate(0.0),
    mProductsFlowRate(0.0),
    mReactantsFluid(0),
    mProductsFluid(0),
    mProducedHeat(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Reactor link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidReactor::~GunnsFluidReactor()
{
    TS_DELETE_OBJECT(mProductsFluid);
    TS_DELETE_OBJECT(mReactantsFluid);
    TS_DELETE_ARRAY(mCompounds);
    TS_DELETE_ARRAY(mReactions);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
/// @param[in,out]  links       (--)  Link vector.
/// @param[in]      port0       (--)  Nominal inlet port map index.
/// @param[in]      port1       (--)  Nominal outlet port map index.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Reactor link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::initialize(      GunnsFluidReactorConfigData& configData,
                                   const GunnsFluidReactorInputData&  inputData,
                                   std::vector<GunnsBasicLink*>&      links,
                                   const int                          port0,
                                   const int                          port1)
{
    /// - First initialize and validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset the initialization status flag.
    mInitFlag   = false;

    /// - Create the internal fluid.
    createInternalFluid();

    /// - Create and initialize the reaction fluids.
    TS_DELETE_OBJECT(mProductsFluid);
    TS_DELETE_OBJECT(mReactantsFluid);
    TS_NEW_PRIM_OBJECT_EXT(mReactantsFluid, PolyFluid,
                           (*mInternalFluid, std::string(mName) + ".mReactantsFluid", false),
                           std::string(mName) + ".mReactantsFluid");
    TS_NEW_PRIM_OBJECT_EXT(mProductsFluid,  PolyFluid,
                           (*mInternalFluid, std::string(mName) + ".mProductsFluid", false),
                           std::string(mName) + ".mProductsFluid");
    mReactantsFluid->setFlowRate(0.0);
    mProductsFluid->setFlowRate(0.0);
    mReactantsFlowRate = 0.0;
    mProductsFlowRate  = 0.0;

    /// - Validate the configuration and input data.
    validate(configData, inputData);

    /// - Initialize from the validated configuration and input data.
    mThermalDiameter    = configData.mThermalDiameter;
    mThermalSurfaceArea = UnitConversion::PI_UTIL * configData.mThermalLength * configData.mThermalDiameter;
    if (mThermalSurfaceArea > DBL_EPSILON) {
        mThermalROverD  = configData.mSurfaceRoughness / mThermalDiameter;
    } else {
        mThermalROverD  = 0.0;
    }
    mWallTemperature    = inputData.mWallTemperature;
    mFluidTemperature   = 0.5 * (mInternalFluid->getTemperature() + inputData.mWallTemperature);

    /// - Initialize the derived attributes from the validated configuration and input data.
    derive(configData, inputData);

    /// - Once we're done with the config data objects, call its cleanup method so it can delete its
    ///   dynamic arrays.  This prevents Trick from checkpointing them.
    configData.cleanup();

    /// - Set the initialization status flag to indicate successful initialization.
    mInitFlag   = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Reactor link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::validate(const GunnsFluidReactorConfigData& configData,
                                 const GunnsFluidReactorInputData&  inputData)
{
    /// - Throw an exception on null pointer to compounds.
    if (0 == configData.mCompounds) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Null pointer to compounds.");
    }

    /// - Throw an exception if no compound types specified.
    if (configData.mNCompounds <= 0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "No compound types specified.");
    }

    /// - Throw an exception on undefined compound type specified.
    for (int i = 0; i < configData.mNCompounds; ++i) {
        if (ChemicalCompound::NO_COMPOUND == configData.mCompoundTypes[i]) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Undefined compound type specified.");
        }
    }

    /// - Throw an exception on null pointer to reactions.
    if (0 == configData.mReactions) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Null pointer to reactions.");
    }

    /// - Throw an exception if no reaction types specified.
    if (configData.mNReactions <= 0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "No reaction types specified.");
    }

    /// - Throw an exception on undefined reaction type specified.
    for (int i = 0; i < configData.mNReactions; ++i) {
        if (ChemicalReaction::NO_REACTION == configData.mReactionTypes[i]) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Undefined reaction type specified.");
        }
    }

    /// - Throw an exception on reaction compounds (reactant, reagent and products) undefined or
    ///   unavailable.
    for (int i = 0; i < configData.mNReactions; ++i) {
        checkCompound(configData, configData.mReactions->getReaction(configData.mReactionTypes[i])->mReactantType);
        checkCompound(configData, configData.mReactions->getReaction(configData.mReactionTypes[i])->mReagentType);
        checkCompound(configData, configData.mReactions->getReaction(configData.mReactionTypes[i])->mProduct1Type);
        if (configData.mReactions->getReaction(configData.mReactionTypes[i])->mProduct2Type !=
            ChemicalCompound::NO_COMPOUND) {
            checkCompound(configData, configData.mReactions->getReaction(configData.mReactionTypes[i])->mProduct2Type);
        }
    }

    /// - Throw an exception if reactor wall temperature < 0.0.
    if (inputData.mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Reactor wall temperature < 0.0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mFluidTemperature  = 0.0;
    mReactantsFlowRate = 0.0;
    mProductsFlowRate  = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      compound    (--)  Compound type.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Checks that the specified chemical compound is defined and available.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::checkCompound(const GunnsFluidReactorConfigData&  configData,
                                      const ChemicalCompound::Type&       compound)
{
    /// - Throw an exception on reaction compound not in compound list.
    bool match = false;
    for (int i = 0; i < configData.mNCompounds; ++i) {
        if (compound == configData.mCompoundTypes[i]) {
            match = true;
            break;
        }
    }
    if (!match) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Reaction uses an undefined compound.");
    }

    /// - Throw an exception on reaction compound expected in atmosphere not found.
    FluidProperties::FluidType fluidType = configData.mCompounds->getCompound(compound)->mFluidType;
    if (fluidType != FluidProperties::NO_FLUID) {
        try {
            mInternalFluid->find(fluidType);
        } catch (TsOutOfBoundsException& e) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Reaction compound expected in atmosphere not found.");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
///
/// @return   void
///
/// @details  Initializes the derived attributes of this GUNNS Fluid Reactor link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::derive(const GunnsFluidReactorConfigData& configData,
                               const GunnsFluidReactorInputData&  inputData)
{
    /// - Initialize the number of reactions and compounds from the configuration data.
    mNReactions = configData.mNReactions;
    mNCompounds = configData.mNCompounds;

    /// - Initialize the compound array for convenience in updateFluid.
    TS_DELETE_ARRAY(mCompounds);
    TS_NEW_CLASS_ARRAY_EXT(mCompounds, mNCompounds, CompoundState, (), std::string(mName) + ".mCompounds");

    for (int i = 0; i < mNCompounds; ++i) {
        mCompounds[i].mType       = configData.mCompoundTypes[i];
        mCompounds[i].mCompound   = const_cast<ChemicalCompound*>(configData.mCompounds->getCompound(mCompounds[i].mType));
        if (FluidProperties::NO_FLUID == mCompounds[i].mCompound->mFluidType) {
            // -1 compound index for compounds not in the atmosphere
            mCompounds[i].mIndex  = -1;
        } else {
            mCompounds[i].mIndex  = mInternalFluid->find(mCompounds[i].mCompound->mFluidType);
        }
        mCompounds[i].mTotalMass  = inputData.mTotalMasses[i];
        mCompounds[i].mMass       = 0.0;
    }

    /// - Initialize the reaction array for convenience in updateFluid.
    TS_DELETE_ARRAY(mReactions);
    TS_NEW_CLASS_ARRAY_EXT(mReactions, mNReactions, ReactionState, (), std::string(mName) + ".mReactions");

    for (int i = 0; i < mNReactions; ++i) {
        mReactions[i].mType          = configData.mReactionTypes[i];
        mReactions[i].mReaction      = const_cast<ChemicalReaction*> (configData.mReactions->getReaction(mReactions[i].mType));
        // -1 reaction compound index for compound not used in the reaction (product 2 is optional)
        mReactions[i].mProduct2Index = -1;
        for (int j = 0; j < mNCompounds; ++j) {
            if (mCompounds[j].mType == mReactions[i].mReaction->mReactantType) {
                 mReactions[i].mReactantIndex = j;
            }
            if (mCompounds[j].mType == mReactions[i].mReaction->mReagentType) {
                mReactions[i].mReagentIndex = j;
            }
            if (mCompounds[j].mType == mReactions[i].mReaction->mProduct1Type) {
                mReactions[i].mProduct1Index = j;
            }
            if (mCompounds[j].mType == mReactions[i].mReaction->mProduct2Type) {
                mReactions[i].mProduct2Index = j;
            }
        }

        /// - Initialize the reaction and reagent efficiency overrides if present in config data.
        if (configData.mReactantEfficiency) {
            mReactions[i].mReactantEfficiency = configData.mReactantEfficiency[i];
        } else {
            mReactions[i].mReactantEfficiency = 0.0;
        }

        if (configData.mReagentEfficiency) {
            mReactions[i].mReagentEfficiency = configData.mReagentEfficiency[i];
        } else {
            mReactions[i].mReagentEfficiency = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt        (s)     Time step.
/// @param[in]      flowrate  (kg/s)  Mass flow rate (not used).
///
/// @return   void
///
/// @details  Perform the chemical reaction and update the Chemical Compounds involved. The reaction
///           is constrained by the available reactant and reagent, and the reactant efficiency. The
///           atmosphere is updated for compounds it contains.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::updateFluid(const double dt, const double flowrate __attribute__((unused)))
{
    /// - Zero the output flow rate and reset the reaction fluid.
    mReactantsFlowRate = 0.0;
    mProductsFlowRate  = 0.0;
    mReactantsFluid->resetState();
    mProductsFluid->resetState();

    /// - Skip reactions if flow rate or time step are negligible.
    if (std::fabs(mFlowRate) > static_cast<double>(FLT_EPSILON) && dt > DBL_EPSILON) {
        /// - Set the compound state from the input atmosphere.
        inputFromAtmosphere(dt);
        /// - Perform the reactions.
        react(dt);
        /// - Set the output atmosphere from the compound state.
        outputToAtmosphere(dt);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Time step.
///
/// @return   void
///
/// @details  Sets the compound states from the input atmosphere.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::inputFromAtmosphere(const double dt)
{
    for (int i = 0; i < mNCompounds; ++i) {
        const int index = mCompounds[i].mIndex;
        if (index != -1) {
            /// - Update total mass for compounds from atmosphere.
            mCompounds[i].mTotalMass = mInternalFluid->getMassFraction(mInternalFluid->getType(index))
                                     * fabs(mFlowRate) * dt;
        }
        /// - Zero out the compound produced/consumed masses.
        mCompounds[i].mMass = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Time step.
///
/// @return   void
///
/// @details  Performs the reactions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::react(const double dt __attribute__((unused)))
{
    /// - Iterate over the reactions.
    mProducedHeat = 0.0;
    for (int i = 0; i < mNReactions; ++i) {

        /// - Get indices for compounds in this reaction.
        const int reactant               = mReactions[i].mReactantIndex;
        const int reagent                = mReactions[i].mReagentIndex;
        const int product1               = mReactions[i].mProduct1Index;

        /// - Get the mole ratios for this reaction.
        const double reactantMoleRatio   = mReactions[i].mReaction->mReactantMoleRatio;
        const double reagentMoleRatio    = mReactions[i].mReaction->mReagentMoleRatio;
        const double product1MoleRatio   = mReactions[i].mReaction->mProduct1MoleRatio;

        /// - Convert the mole ratios to mass ratios.
        const double reactantMassRatio   = reactantMoleRatio * mCompounds[reactant].mCompound->mMWeight;
        const double reagentMassRatio    = reagentMoleRatio  * mCompounds[reagent].mCompound->mMWeight;
        const double product1MassRatio   = product1MoleRatio * mCompounds[product1].mCompound->mMWeight;

        /// - Compute the maximum reactant that the reaction could consume based on its
        ///   availability and reaction efficiency.  Regardless of reaction efficiency, we limit the
        ///   maximum consumption of reaction or reagent to 99.99%, so that no single reaction can
        ///   consume 100% of the incoming flow, which would break the fluid mixing and transport
        ///   interface with the downstream node.
        const double maxReactantMass     = mCompounds[reactant].mTotalMass
                                         * fmin(0.9999, computeEfficiency(i));

        /// - Compute the reagent consumed by the reaction based on its availability and the maximum
        ///   reactant that the reaction could consume.
        const double reagentMass         = fmin(mCompounds[reagent].mTotalMass * 0.9999,
                                                maxReactantMass * reagentMassRatio / reactantMassRatio);

        /// - Compute reactant consumed by the reaction based on reagent consumed by the reaction.
        const double reactantMass        = reagentMass * reactantMassRatio / reagentMassRatio;

        /// - Compute the first product produced by the reaction based on reagent consumed by the
        ///   reaction.
        const double product1Mass        = reagentMass * product1MassRatio / reagentMassRatio;

        /// - Update the mass consumed/produced this cycle and the total available mass.
        mCompounds[reactant].mMass      -= reactantMass;
        mCompounds[reactant].mTotalMass -= reactantMass;
        mCompounds[reagent].mMass       -= reagentMass;
        mCompounds[reagent].mTotalMass  -= reagentMass;
        mCompounds[product1].mMass      += product1Mass;
        mCompounds[product1].mTotalMass += product1Mass;

        /// - Zero total masses when they drop near zero to avoid arithmetic underflow.
        if (mCompounds[reactant].mTotalMass < DBL_EPSILON) {
            mCompounds[reactant].mTotalMass = 0.0;
        }
        if (mCompounds[reagent].mTotalMass < DBL_EPSILON) {
            mCompounds[reagent].mTotalMass = 0.0;
        }

        /// - Calculate the heat produced by the reaction based on the reaction enthalpy
        ///   (mReactionHeat) and the number of produced moles of product 1.
        double heatFactor = mReactions[i].mReaction->mReactionHeat / mReactions[i].mReaction->mProduct1MoleRatio;
        double product1MWeight = mCompounds[product1].mCompound->mMWeight;
        mProducedHeat += heatFactor * UnitConversion::UNIT_PER_MEGA * product1Mass / product1MWeight;

        /// - If there is a 2nd product, compute consumed mass and update.
        const int product2 = mReactions[i].mProduct2Index;
        if (product2 != -1) {
            const double product2MoleRatio   = mReactions[i].mReaction->mProduct2MoleRatio;
            const double product2MassRatio   = product2MoleRatio * mCompounds[product2].mCompound->mMWeight;
            const double product2Mass        = reagentMass * product2MassRatio / reagentMassRatio;
            mCompounds[product2].mMass      += product2Mass;
            mCompounds[product2].mTotalMass += product2Mass;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Time step.
///
/// @return   void
///
/// @details  Sets the reaction fluids (output atmosphere) from the compound states.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::outputToAtmosphere(const double dt)
{
    /// - Skip if all mass fractions negligible.
    bool negligible = true;
    for (int i = 0; i < mNCompounds; ++i) {
        const int index = mCompounds[i].mIndex;
        if (index != -1) {
            if (fabs(mCompounds[i].mMass) > DBL_EPSILON) {
                negligible = false;
            }
        }
    }

    if (!negligible) {
        /// - Set the output atmosphere constituent mass from the compound mass for this cycle.
        for (int i = 0; i < mNCompounds; ++i) {
            const int index = mCompounds[i].mIndex;
            if (index != -1) {
                if (mCompounds[i].mMass > DBL_EPSILON) {
                    mProductsFluid->setMass(index, mCompounds[i].mMass);
                    mProductsFlowRate += mCompounds[i].mMass;
                } else if (mCompounds[i].mMass < -DBL_EPSILON) {
                    mReactantsFluid->setMass(index, mCompounds[i].mMass);
                    mReactantsFlowRate += mCompounds[i].mMass;
                }
            }
        }

        /// - Update the output flow rates.
        mProductsFlowRate  /= dt;
        mReactantsFlowRate /= dt;

        /// - Update output mass, moles & fractions from reaction constituents.
        mProductsFluid->updateMass();
        mReactantsFluid->updateMass();
    }
    /// - add heat from rxn
    addRxnHeat(dt);
    /// - Update fluid temperature.
    updateTemperature(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Time step.
///
/// @return   (--)  Efficiency of the reaction (0-1).
///
/// @note     Derived classes may override, e.g., to model heat of combustion.
///
/// @details  Updates the reaction fluid temperature for thermal convection with the wall.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::updateTemperature(const double dt __attribute__((unused)))
{
    /// - Compute the heat transfer from the fluid to the reactor, and update the internal and
    ///   reaction fluid temperatures.
    mWallHeatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(mInternalFluid,
                                                               mFlowRate,
                                                               mThermalROverD,
                                                               mThermalDiameter,
                                                               mThermalSurfaceArea,
                                                               mWallTemperature);
    mFluidTemperature = mInternalFluid->getTemperature();
    mReactantsFluid->setTemperature(mFluidTemperature);
    mProductsFluid->setTemperature(mFluidTemperature);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Time step.
///
/// @details  Adds or removes the heat from the reactions to the reaction fluid.
///
///           For a reaction: aA + bB -> cC + dD  with dH = h;
///
///           The dH is tied with the chemical equation so if the balance changed, dH will
///           change.
///
///           For example:
///               H2O(l) -> H2O(g) dH = 44 kJ/mole;
///           while
///               2H2O(l) -> 2H2O(g) dH = 88 kJ/mole
///
///           With that information, here are the steps to compute heat generation (unit: J)
///           1. We can use any of the products or reactants to do the computation. [we use the
///           product 1 mole ratio (c)]
///           2. Compute conversion factor using h [kJ/mole] and c [--] cFactor = h/c [unit: kJ/mole]
///           3. Compute heat generation [J] using product C's mass [kg], C's molar weight [g/mole], and cFactor [kJ/mole]
///              Heat[J] = mass_c [kg] * 1000[g/kg] * (1/ molarW_c[g/mole]) * cFactor[kJ/mole] * 1000[J/kJ]
///
///           NOTE: we can also use A, B, or D to do the computation; the steps will be the same,
///           and the result should be the same. Use of product 1 mole ratio was determined to be
///           the best option because not all chem rxns have products 2, 3, etc...
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::addRxnHeat(const double dt)
{
    const double newHeat = mFlowRate * mInternalFluid->getSpecificEnthalpy() + mProducedHeat / dt;
    mInternalFluid->setTemperature(mInternalFluid->computeTemperature( newHeat / mFlowRate ));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      i  (--)  Reaction index.
///
/// @return   (--)  Efficiency of the reaction (0-1).
///
/// @note     Derived classes may override, e.g., to model temperature dependence.
///
/// @details  Computes and returns the reactant efficiency of the specified reaction.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidReactor::computeEfficiency(const int i)
{
    /// - Default is to return the nominal constant efficiency for the reaction.
    double result = mReactions[i].mReaction->mReactantEfficiency;

    /// - Use the link's override value if it is set.
    if (mReactions[i].mReactantEfficiency > DBL_EPSILON) {
        result = mReactions[i].mReactantEfficiency;
    }

    /// - Scale by the optional reagent efficiency times the reagent mass.
    if (mReactions[i].mReagentEfficiency > DBL_EPSILON) {
        result *= mReactions[i].mReagentEfficiency * mCompounds[mReactions[i].mReagentIndex].mTotalMass;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] forcedOutflow (--) Unused in this implementation
/// @param[in] fromPort      (--) The port the fluid is transported out of (defaults to 0)
/// @param[in] toPort        (--) The port the fluid is transported into (default to 1)
///
/// @details  This function adds the link's flows to the node's collection terms.
///
///           This implementation overrides the base class with these major differences:
///           - Conduction flow with mInternalFluid is transported between the nodes before the
///             reaction flows are added to the downstream node.
///           - Adds the reaction flows effect on downstream pressure via the source vector.
///           - Expansion scale factor and the isentropic expansion effect are not used or modeled.
///           - Skips checks for mInternalFluid, since it is always present in this class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::transportFluid(const bool forcedOutflow __attribute__((unused)),
                                       const int  fromPort,
                                       const int  toPort)
{
    if (checkValidPortNumber(toPort) && checkValidPortNumber(fromPort)) {

        if (mFlowRate > m100EpsilonLimit) {

            /// - Move the the conduction flow with mInternalFluid from the upstream node to the
            ///   downstream node.
            mNodes[fromPort]->collectOutflux(mFlowRate);
            mNodes[toPort]->collectInflux(mFlowRate, mInternalFluid);

            /// - Add the reaction flows to the downstream node.
            /// - Add the reaction flows to the downstream source vector so the node pressure will
            ///   respond to the affected total molar flow rate on the next pass.  The source vector
            ///   will be used next pass so there will be a small error in pressure, but mass will
            ///   be conserved and GUNNS washes out pressure errors over time.
            mSourceVector[fromPort] = 0.0;
            mSourceVector[toPort]   = 0.0;
            if (mProductsFlowRate > m100EpsilonLimit) {
                mNodes[toPort]->collectInflux(mProductsFlowRate,  mProductsFluid);
                mSourceVector[toPort] += mProductsFlowRate  / mProductsFluid->getMWeight();
            }
            if (mReactantsFlowRate < -m100EpsilonLimit) {
                mNodes[toPort]->collectInflux(mReactantsFlowRate, mReactantsFluid);
                mSourceVector[toPort] += mReactantsFlowRate / mReactantsFluid->getMWeight();
            }

        } else if(mFlowRate < -m100EpsilonLimit) {

            /// - Move the the conduction flow with mInternalFluid from the upstream node to the
            ///   downstream node.
            mNodes[toPort]->collectOutflux(-mFlowRate);
            mNodes[fromPort]->collectInflux(-mFlowRate, mInternalFluid);

            /// - Add the reaction flows to the downstream node.
            /// - Add the reaction flows to the downstream source vector so the node pressure will
            ///   respond to the affected total molar flow rate on the next pass.  The source vector
            ///   will be used next pass so there will be a small error in pressure, but mass will
            ///   be conserved and GUNNS washes out pressure errors over time.
            mSourceVector[fromPort] = 0.0;
            mSourceVector[toPort]   = 0.0;
            if (mProductsFlowRate > m100EpsilonLimit) {
                mNodes[fromPort]->collectInflux(mProductsFlowRate,  mProductsFluid);
                mSourceVector[fromPort] += mProductsFlowRate  / mProductsFluid->getMWeight();
            }
            if (mReactantsFlowRate < -m100EpsilonLimit) {
                mNodes[fromPort]->collectInflux(mReactantsFlowRate, mReactantsFluid);
                mSourceVector[fromPort] += mReactantsFlowRate / mReactantsFluid->getMWeight();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (m2)    New Thermal Surface Area.
///
/// @returns  void
///
/// @details  Sets the thermal surface area of this this GUNNS Fluid Chemical Reactor model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::setThermalSurfaceArea(const double value)
{
    mThermalSurfaceArea = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (K)    New Wall Temperature.
///
/// @returns  void
///
/// @details  Sets the wall temperature of this this GUNNS Fluid Chemical Reactor model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidReactor::setWallTemperature(const double value)
{
    mWallTemperature = std::max(0.0, value);
}
