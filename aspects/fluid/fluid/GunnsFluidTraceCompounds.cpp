/**
@file
@brief    GUNNS Fluid Trace Compounds Model implementation

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((properties/ChemicalCompound.o)
    (simulation/hs/TsHsMsg.o)
    (software/exceptions/TsInitializationException.o)
    (software/exceptions/TsOutOfBoundsException.o))
*/

#include "GunnsFluidTraceCompounds.hh"
#include "core/GunnsBasicNode.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  types      (--)  Pointer to the array of compound types.
/// @param[in]  nTypes     (--)  Number of compound types.
/// @param[in]  name       (--)  Name of this object.
///
/// @throws   TsInitializationException
///
/// @details  Default constructs this GUNNS Fluid Trace Compounds model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTraceCompoundsConfigData::GunnsFluidTraceCompoundsConfigData(
        const ChemicalCompound::Type*   types,
        const int                       nTypes,
        const std::string&              name)
    :
    mNTypes(0),
    mCompounds(),
    mName(name)
{
    /// - Validate the arguments.
    validate(types, nTypes);

    /// - The types array is optional so that this config data can be constructed empty and
    ///   compounds added later.
    if (types) {

        /// - Create a DefineChemicalCompounds to pull data from.
        DefinedChemicalCompounds definedCompounds;

        /// - Create new chemical compound from defined compounds and add it to the vector.
        for(int i = 0; i < nTypes; i++){

            ChemicalCompound* compound =
                    new ChemicalCompound(types[i],
                            definedCompounds.getCompound(types[i])->mName,
                            definedCompounds.getCompound(types[i])->mFluidType,
                            definedCompounds.getCompound(types[i])->mMWeight);
            mCompounds.push_back(compound);
        }
    }

    /// - Update the number of types to match the vector size.
    mNTypes = static_cast<int>(mCompounds.size());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Trace Compounds model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTraceCompoundsConfigData::~GunnsFluidTraceCompoundsConfigData()
{
    for(int i = 0; i < mNTypes; i++){
        delete mCompounds[i];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  types   (--)  Pointer to the array of compound types.
/// @param[in]  nTypes  (--)  Number of compound types.
///
/// @throws   TsInitializationException
///
/// @details  Validates the data provided to the constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompoundsConfigData::validate(const ChemicalCompound::Type* types,
                                                  const int                     nTypes)
{
    /// - Throw an error if this object is unnamed.
    if ("Unnamed GunnsFluidTraceCompoundsConfigData" == mName) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Object is unnamed.");
    }

    if (types) {
        if (nTypes < 1) {
            /// - Throw error if nTypes < 1 when types array was given.
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                        "Number of compound types is less than 1.");
        } else {
            for (int i=0; i<nTypes; ++i) {
                /// - Throw error if any given type isn't in the valid ChemicalCompound::Type range.
                ///   This does not allow NO_COMPOUND.
                const int type = static_cast<int>(types[i]);
                if (type < 0 or type >= static_cast<int>(ChemicalCompound::NO_COMPOUND)) {
                    GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                                "Invalid ChemicalCompound::Type.");
                }
            }
        }
    } else if (0 != nTypes) {
        /// - Throw an error if a non-zero number of types but the types array wasn't given.
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Missing types array when nTypes != 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  molecWeight (1/mol) Molecular weight of compound.
/// @param[in]  name        (--)    Name of compound.
/// @param[in]  fluidType   (--)    Optional fluid properties type associated with this compound.
/// @param[in]  type        (--)    Optional chemical compound type of this compound.
///
/// @throws   TsInitializationException
///
/// @details  Adds a new trace compound at run time, given its custom properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompoundsConfigData::addCompound(const double                     molecWeight,
                                                     const std::string                name,
                                                     const FluidProperties::FluidType fluidType,
                                                     ChemicalCompound::Type           type)
{
    /// - Throw an error if Molecular Weight is <= 0.0.
    if (DBL_EPSILON > molecWeight) {
        std::string errorMsg = "Can't add custom trace compound " + name +
                               ".  Molecular weight must be > 0.0.";
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data", errorMsg);
    }

    /// - Throw an error if name is blank.
    if (name.empty()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Can't add a custom trace compound with an empty name.");
    }

    /// - Create the new chemical compound.
    ChemicalCompound* compound = new ChemicalCompound(type,
                                                      name,
                                                      fluidType,
                                                      molecWeight);

    /// - Add the new compound to the compounds vector.
    mCompounds.push_back(compound);
    mNTypes = static_cast<int>(mCompounds.size());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type  (--)  The type of defined chemical compound to add.
///
/// @throws   TsInitializationException
///
/// @details  Adds a new trace compound at run time, of a defined type.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompoundsConfigData::addCompound(ChemicalCompound::Type type)
{
    /// - Throw an error if NO_COMPOUND.
    if (ChemicalCompound::NO_COMPOUND == type) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Can't add NO_COMPOUND type.");
    }

    /// - Create DefineChemicalCompounds to pull data from.
    const DefinedChemicalCompounds definedCompounds;

    /// - Add the compound.
    addCompound(definedCompounds.getCompound(type)->mMWeight,
                definedCompounds.getCompound(type)->mName,
                definedCompounds.getCompound(type)->mFluidType,
                type);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Trace Compounds model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTraceCompoundsInputData::GunnsFluidTraceCompoundsInputData(double* state)
    :
    mState(state)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Trace Compounds model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTraceCompoundsInputData::~GunnsFluidTraceCompoundsInputData()
{
    // nothing to do
}

/// @details  This is used to catch the use of the default no-argument constructor for an instance
///           during its initialize method call.
const double GunnsFluidTraceCompounds::mNoRef = 0.0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     DO NOT use this constructor unless you know what you're doing.
//
/// @details  TS21 requires a no-argument default constructor for any classes that will be
///           dynamically allocated with their Trick memory macro's.  This particular example is
///           dangerous because no reference to an external fluid moles is provided, which is not
///           how this class is intended to be used.  We initialize the reference to an internal
///           static const which will cause an exception to be thrown if this object is initialized.
///           We would prefer to keep this private and not implemented to prevent users from calling
///           it at all.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTraceCompounds::GunnsFluidTraceCompounds()
    :
    mName(""),
    mConfig(0),
    mMass(0),
    mMoleFraction(0),
    mFluidMoles(mNoRef),
    mInitFlag(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fluidMoles  (kg*mol)  Reference to the fluid moles term in the parent object.
///
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Trace Compounds model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTraceCompounds::GunnsFluidTraceCompounds(const double& fluidMoles)
    :
    mName(""),
    mConfig(0),
    mMass(0),
    mMoleFraction(0),
    mFluidMoles(fluidMoles),
    mInitFlag(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that        (--)      Source to copy.
/// @param[in]  fluidMoles  (kg*mol)  Reference to the fluid moles term in the parent object.
/// @param[in]  name        (--)      Instance name for messages.
///
/// @throws   TsInitializationException
///
/// @details  Copy constructs and initializes this GUNNS Fluid Trace Compounds model.  This is a
///           deep copy.
///
/// @note     The source to copy must have been initialized or this method will throw an exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTraceCompounds::GunnsFluidTraceCompounds(const GunnsFluidTraceCompounds& that,
                                                   const double&                   fluidMoles,
                                                   const std::string&              name)
    :
    mName(""),
    mConfig(0),
    mMass(0),
    mMoleFraction(0),
    mFluidMoles(fluidMoles),
    mInitFlag(false)
{
    GunnsFluidTraceCompoundsInputData input(that.getMoleFractions());
    initialize(that.getConfig(), &input, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Trace Compounds model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTraceCompounds::~GunnsFluidTraceCompounds()
{
    cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes allocated memory.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::cleanup()
{
    TS_DELETE_ARRAY(mMoleFraction);
    TS_DELETE_ARRAY(mMass);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)      Configuration data.
/// @param[in]  inputData   (--)      Input data.
/// @param[in]  name        (--)      Name of the instance for messaging.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Trace Compounds model with configuration and input data.
///
/// @note     Trace compounds and all associated objects are optional in GUNNS networks.  If there
///           are to be no TC's in a network, this method should not be called.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::initialize(const GunnsFluidTraceCompoundsConfigData* configData,
                                          const GunnsFluidTraceCompoundsInputData*  inputData,
                                          const std::string&                        name)
{
    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Have to call delete in case someone calls this method twice during a run.
    cleanup();

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize link name from config data.
    GUNNS_NAME_ERREX("GunnsFluidTraceCompounds", name);

    /// - Initialize from config data.
    mConfig  = configData;

    /// - Allocate dynamic arrays.
    if (mConfig->mNTypes) {
        TS_NEW_PRIM_ARRAY_EXT(mMass,          mConfig->mNTypes, double, name + ".mMass");
        TS_NEW_PRIM_ARRAY_EXT(mMoleFraction,  mConfig->mNTypes, double, name + ".mMoleFraction");
    }

    /// - Initialize state data from input data.  The input data is optional; if it isn't specified,
    ///   then the compound masses and mole fractions are all initialized to zero.
    if (inputData) {
        setMoleFractions(inputData->mState);
        updateMasses();
        limitPositive();
    } else {
        setMoleFractions();
        setMasses();
    }

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Configuration data.
/// @param[in]  inputData   (--)  Input data.
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Trace Compounds model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::validate(const GunnsFluidTraceCompoundsConfigData* configData,
                                        const GunnsFluidTraceCompoundsInputData*  inputData) const
{
    /// - Throw an exception on uninitialized reference mFluidMoles.
    if (&mNoRef == &mFluidMoles) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Uninitialized reference mFluidMoles, wrong constructor used.");
    }

    /// - Throw an exception on NULL config data.
    if (!configData) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Compounds configuration data is missing.");
    }

    /// - Throw an exception if input data is given with mole fractions not defined.
    if (inputData and !inputData->mState) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Initial compound mole fractions (state array) are not given.");
    }

    for (int i = 0; i < configData->mNTypes; ++i) {
        for (int j = i+1; j < configData->mNTypes; ++j) {
            /// - Throw an exception if there are duplicated types in the array.
            if (configData->mCompounds[j]->mType == configData->mCompounds[i]->mType and
                configData->mCompounds[i]->mType != ChemicalCompound::NO_COMPOUND) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "A type is duplicated in the compound types array.");
            }

            /// - Throw an exception if there are duplicated names in the array.
            if (configData->mCompounds[j]->mName == configData->mCompounds[i]->mName) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "A name is duplicated in the compound names array.");
            }
        }
    }

    for (int i = 0; i < configData->mNTypes; ++i) {
        /// - Throw an exception if mole fraction is < 0.
        if (inputData and inputData->mState[i] < 0.0) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                        "An initial compound mole fraction (state) is < 0.");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Calls the updateMoleFractions method to reset the compound masses from their
///           mole fractions relative to the total moles of the parent fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::restart()
{
    updateMasses();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  i  (--)  Index in the compounds array to get the compound type of.
///
/// @returns  ChemicalCompound::Type  (--)  Type of the compound at the specified array index.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the type of the compound at the specified array index.
////////////////////////////////////////////////////////////////////////////////////////////////////
ChemicalCompound::Type GunnsFluidTraceCompounds::getType(const int i) const
{
    /// - Return the compound type if the index is in range
    if (0 <= i && i < mConfig->mNTypes) {
        return mConfig->mCompounds[i]->mType;
    }

    /// - Otherwise throw an exception.
    std::ostringstream msg;
    msg << "An invalid index (" << i << ") was specified.";
    TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_GUNNS, msg.str(),
                    TsOutOfBoundsException, "Input Argument Out of Range", mName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type  (--)  Type of compound to find.
/// @param[in]  name  (--)  Name of compound to find (used for compounds added at run time).
///
/// @return   Index into the compounds array corresponding to the given compound type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the array index in the compounds array corresponding to the given compound
///           type.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsFluidTraceCompounds::find(const ChemicalCompound::Type& type,
                                   const std::string name) const
{
    /// - Return the index if found.
    for (int i = 0; i < mConfig->mNTypes; ++i) {
        if(type != ChemicalCompound::NO_COMPOUND){
            if (mConfig->mCompounds[i]->mType == type) {
                return i;
            }
        } else {
            if (mConfig->mCompounds[i]->mName == name) {
                return i;
            }
        }
    }

    /// - Otherwise throw an exception.
    std::ostringstream msg;
    msg << "An invalid compound type (" << type << ") was specified.";
    TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_GUNNS, msg.str(),
                    TsOutOfBoundsException, "Input Argument Out of Range", mName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  compound  (--)  Type of the chemical compound to find.
///
/// @return   Index into the compounds array corresponding to the given compound type.
///
/// @details  Returns the array index in the compounds array corresponding to the given compound
///           type, or -1 if the given compound isn't found.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsFluidTraceCompounds::findCompound(const ChemicalCompound::Type compound) const
{
    for (int i = 0; i < mConfig->mNTypes; ++i) {
        if (mConfig->mCompounds[i]->mType == compound) {
            return i;
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type  (--)  Chemical compound type to find the mass of.
/// @param[in]  name  (--)  Name of compound to find (used for compounds added at run time).
///
/// @returns  double  (kg)  Mass of the specified compound type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the mass of the specified compound type currently in this Trace Compounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidTraceCompounds::getMass(const ChemicalCompound::Type& type, const std::string name) const
{
    return mMass[find(type, name)];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type  (--)  Chemical compound type to find the mole fraction of.
/// @param[in]  name  (--)  Name of compound to find (used for compounds added at run time).
///
/// @returns  double  (--)  Mole fraction of the specified compound type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the mole fraction of the specified compound type currently in this Trace
///           Compound.  Mole fractions are relative to the total moles in a parent fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidTraceCompounds::getMoleFraction(const ChemicalCompound::Type& type, const std::string name) const
{
    return mMoleFraction[find(type, name)];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  masses  (kg)  An array of given compound masses.
///
/// @details  Sets the entire set of compound masses in this Trace Compound to the given mass array,
///           which is assumed to represent the same compounds and in the same order as this Trace
///           Compound.
///
/// @note     This method does not recompute the mole fractions resulting from the new masses.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::setMasses(const double* masses)
{
    for (int i = 0; i < mConfig->mNTypes; ++i) {
        if (masses) {
            mMass[i] = masses[i];
        } else {
            mMass[i] = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type  (--)  Specifies which compound type the given mass represents.
/// @param[in]  mass  (kg)  The mass of the given compound type.
/// @param[in]  name  (--)  Name of compound to find (used for compounds added at run time).
///
/// @throws   TsOutOfBoundsException
///
/// @details  Sets the mass of the given compound type in this Trace Compounds to the given value.
///
/// @note     This method does not recompute the mole fractions resulting from the new masses.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::setMass(const ChemicalCompound::Type& type, const double mass, const std::string name)
{
    mMass[find(type, name)] = mass;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  index  (--)  Specifies which index in the trace compounds the given mass represents.
/// @param[in]  mass   (kg)  The mass of the given compound type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Sets the mass of the given compound index in this Trace Compounds to the given value.
///
/// @note     This method does not recompute the mole fractions resulting from the new masses.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::setMass(const int index, const double mass)
{
    if (index > -1 and index < mConfig->mNTypes) {
        mMass[index] = mass;
    } else {
        /// - Throw an exception if the given index is out of bounds of the compounds config array.
        std::ostringstream msg;
        msg << "An invalid compound index (" << index << ") was specified.";
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_GUNNS, msg.str(),
                        TsOutOfBoundsException, "Input Argument Out of Range", mName);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  moleFractions  (--)  An array of given compound mole fractions.
///
/// @details  Sets the entire set of compound mole fractions in this Trace Compound to the given
///           mole fraction array, which is assumed to represent the same compounds and in the same
///           order as this Trace Compound.
///
/// @note     This method does not recompute the masses resulting from the new mole fractions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::setMoleFractions(const double* moleFractions)
{
    for (int i = 0; i < mConfig->mNTypes; ++i) {
        if (moleFractions) {
            mMoleFraction[i] = moleFractions[i];
        } else {
            mMoleFraction[i] = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type         (--) Specifies which compound type the given mole fraction represents.
/// @param[in]  moleFraction (--) The mole fraction of the given compound type.
/// @param[in]  name  (--)  Name of compound to find (used for compounds added at run time).
///
/// @throws   TsOutOfBoundsException
///
/// @details  Sets the mole fraction of the given compound type in this Trace Compounds to the given
///           value.
///
/// @note     This method does not recompute the masses resulting from the new mole fractions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::setMoleFraction(const ChemicalCompound::Type& type,
                                               const double                  moleFraction,
                                               const std::string name)
{
    mMoleFraction[find(type, name)] = moleFraction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  index  (--)  Specifies which index the given mole fraction represents.
/// @param[in]  mass   (--)  The mole fraction of the given compound type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Sets the mole fraction of the given compound index in this Trace Compounds to the
///           given value.
///
/// @note     This method does not recompute the masses resulting from the new mole fractions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::setMoleFraction(const int index, const double moleFraction)
{
    if (index > -1 and index < mConfig->mNTypes) {
        mMoleFraction[index] = moleFraction;
    } else {
        /// - Throw an exception if the given index is out of bounds of the compounds config array.
        std::ostringstream msg;
        msg << "An invalid compound index (" << index << ") was specified.";
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_GUNNS, msg.str(),
                        TsOutOfBoundsException, "Input Argument Out of Range", mName);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Recomputes the compound masses in this Trace Compounds from their mole fractions
///           relative to the total moles of the parent fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::updateMasses()
{
    for (int i = 0;  i < mConfig->mNTypes; ++i) {
        mMass[i] = mMoleFraction[i] * mFluidMoles * mConfig->mCompounds[i]->mMWeight;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Recomputes the compound mole fractions in this Trace Compounds from their masses
///           relative to the total moles of the parent fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::updateMoleFractions()
{
    if (mFluidMoles > 0.0) {
        for (int i = 0;  i < mConfig->mNTypes; ++i) {
            mMoleFraction[i] = mMass[i] / mFluidMoles / mConfig->mCompounds[i]->mMWeight;
        }
    }

    /// - To avoid math underflows, zero mass and mole fraction if the mole fraction drops to an
    ///   insignificant level.
    for (int i = 0;  i < mConfig->mNTypes; ++i) {
        if (mMoleFraction[i] < DBL_EPSILON) {
            mMoleFraction[i] = 0.0;
            mMass[i]         = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  source        (--)   The Trace Compounds object that is to be mixed in.
/// @param[in]  totalMolesIn  (mol)  The total number of fluid moles of the incoming parent fluid
///                                  that has flowed in.
///
/// @details  Performs the mixing of another Trace Compounds into this Trace Compounds resulting
///           from the bulk flow of the former's parent fluid into this Trace Compound's parent
///           fluid.  Updates the resulting compound masses and mole fractions.
///
/// @note     This assumes the incoming Trace Compounds has the same compound types and in the same
///           order as our compounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::flowIn(const GunnsFluidTraceCompounds& source,
                                      const double                    totalMolesIn)
{
    double* sourceMoleFractions = source.getMoleFractions();

    for (int i = 0; i < mConfig->mNTypes; ++i) {
        mMass[i] += totalMolesIn * sourceMoleFractions[i] * mConfig->mCompounds[i]->mMWeight;
    }
    updateMoleFractions();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] rates (kg/s) Array of trace compound mass flow rates added.
/// @param[in] dt    (s)    Time step of integration.
///
/// @details   This overloaded version of flowIn input the trace compounds as an array of mass flow
///            rates integrated over a timestep.  This allows negative flow rates in, which are
///            positive flow rates out.  Updates the resulting mole fractions.
///
/// @notes     Resulting masses less than zero are quietly zeroed, and doesn't conserve mass.  The
///            caller should ensure not to remove more mass than the parent fluid has in order to
///            maintain conservation of mass.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::flowIn(const double* rates, const double dt)
{
    for (int i = 0; i < mConfig->mNTypes; ++i) {
        mMass[i] = fmax(0.0, mMass[i] + rates[i] * dt);
    }
    updateMoleFractions();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  totalMolesOut  (mol)  The total number of fluid moles of the parent fluid that has
///                                   flowed out.
///
/// @details  Reduces the trace compound masses in this Trace Compounds resulting from the bulk
///           reduction in total moles of the parent fluid, such as when the parent fluid flows out
///           of a network node.  This does not recompute the mole fractions because a flow out
///           would not change them.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::flowOut(const double totalMolesOut)
{
    if (totalMolesOut > DBL_EPSILON) {

        for (int i = 0; i < mConfig->mNTypes; ++i) {
            mMass[i] -= totalMolesOut * mMoleFraction[i] * mConfig->mCompounds[i]->mMWeight;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Zeroes negative trace compound masses and mole fractions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTraceCompounds::limitPositive()
{
    for (int i = 0; i < mConfig->mNTypes; ++i) {
        mMass[i]  = std::max(0.0, mMass[i]);
    }
    updateMoleFractions();
}
