/*
@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
 (Provides the class for modeling poly fluids.)

REQUIREMENTS:
 ()

REFERENCE:
 ()

ASSUMPTIONS AND LIMITATIONS:
 ()

 LIBRARY DEPENDENCY:
 (
  (aspects/fluid/fluid/MonoFluid.o)
  (aspects/fluid/fluid/GunnsFluidTraceCompounds.o)
  (math/approximation/LinearFit.o)
 )

 PROGRAMMERS:
 ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

*/

#include <cmath>
#include <cfloat>
#include <cstring>

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "math/approximation/LinearFit.hh"

#include "PolyFluid.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] properties     (--) Pointer to the defined fluid properties.
/// @param[in] types          (--) Pointer to array of constituent types in the PolyFluid.
/// @param[in] nTypes         (--) Number of constituent fluids in the PolyFluid.
/// @param[in] traceCompounds (--) Optional pointer to the trace compounds config data.
///
/// @details  Default constructs this PolyFluid configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
PolyFluidConfigData::PolyFluidConfigData(const DefinedFluidProperties*             properties,
                                         const FluidProperties::FluidType*         types,
                                         const int                                 nTypes,
                                         const GunnsFluidTraceCompoundsConfigData* traceCompounds)
    :
    mProperties(properties),
    mTypes(0),
    mNTypes(nTypes),
    mTraceCompounds(traceCompounds)
{
    if (nTypes > 0 && types != 0) {
        /// - Allocate and initialize the array of fluid types.
        mTypes = new FluidProperties::FluidType[nTypes];
        for (int i = 0; i < nTypes; ++i) {
           mTypes[i] = types[i];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that (--) Source to copy.
///
/// @details  Copy constructs this PolyFluid configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
PolyFluidConfigData::PolyFluidConfigData(const PolyFluidConfigData& that)
    :
    mProperties(that.mProperties),
    mTypes(0),
    mNTypes(that.mNTypes),
    mTraceCompounds(that.mTraceCompounds)
{
    /// - Allocate and initialize the array of fluid types.
    mTypes = new FluidProperties::FluidType[that.mNTypes];
    for (int i = 0; i < that.mNTypes; ++i) {
       mTypes[i] = that.mTypes[i];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this PolyFluid configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
PolyFluidConfigData::~PolyFluidConfigData()
{
    /// - Deallocate the array of fluid types
    delete [] mTypes;
    mTypes = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] temperature    (K)    Temperature of the fluid.
/// @param[in] pressure       (kPa)  Pressure of the fluid.
/// @param[in] flowRate       (kg/s) Mass flow rate of the fluid.
/// @param[in] mass           (kg)   Mass of the fluid.
/// @param[in] massFraction   (--)   Array of mass fractions of constituent fluids in PolyFluid.
/// @param[in] traceCompounds (--)   Optional pointer to the trace compounds input data.
///
/// @note     The array of mass fractions must be of the same length and in the same order as the
///           array of types specified in the configuration data.
///
/// @details  Default constructs this PolyFluid input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
PolyFluidInputData::PolyFluidInputData(const double                             temperature,
                                       const double                             pressure,
                                       const double                             flowRate,
                                       const double                             mass,
                                       double*                                  massFraction,
                                       const GunnsFluidTraceCompoundsInputData* traceCompounds)
    :
    MonoFluidInputData(temperature, pressure, flowRate, mass),
    mMassFraction(massFraction),
    mTraceCompounds(traceCompounds)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that (--) Source to copy.
///
/// @details  Copy constructs this PolyFluid input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
PolyFluidInputData::PolyFluidInputData(const PolyFluidInputData& that)
    :
    MonoFluidInputData(that),
    mMassFraction(that.mMassFraction),
    mTraceCompounds(that.mTraceCompounds)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this PolyFluid input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
PolyFluidInputData::~PolyFluidInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tolerance on sum of fractions for normalizing fractions.
////////////////////////////////////////////////////////////////////////////////////////////////////
const double PolyFluid::FRACTION_TOLERANCE  = 1.0e-04;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Inner limit for various molar flow rate and moles per unit mass parameters.
////////////////////////////////////////////////////////////////////////////////////////////////////
const double PolyFluid::MOLE_INNER_LIMIT  = DBL_EPSILON * DBL_EPSILON;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs a Constituent object.
////////////////////////////////////////////////////////////////////////////////////////////////////
Constituent::Constituent()
    :
    mType(FluidProperties::GUNNS_CO),               //enum value of zero
    mFluid(),
    mMassFraction(0.0),
    mMoleFraction(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs a Constituent object.
////////////////////////////////////////////////////////////////////////////////////////////////////
Constituent::~Constituent()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this PolyFluid with obviously invalid data.
////////////////////////////////////////////////////////////////////////////////////////////////////
PolyFluid::PolyFluid()
    :
    mName(),
    mTemperature(0.0),
    mPressure(0.0),
    mFlowRate(0.0),
    mMass(0.0),
    mMole(0.0),
    mConstituents(0),
    mNConstituents(0),
    mPhase(FluidProperties::NO_PHASE),
    mMWeight(0.0),
    mDensity(0.0),
    mViscosity(0.0),
    mSpecificHeat(0.0),
    mSpecificEnthalpy(0.0),
    mThermalConductivity(0.0),
    mPrandtlNumber(0.0),
    mAdiabaticIndex(0.0),
    mTraceCompounds(0),
    mInitFlag(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    configData   (--)    Configuration data.
/// @param[in]    inputData    (--)    Input data.
///
/// @throws   TsInitializationException
///
/// @details  Constructs and initializes this PolyFluid with input data.
///
/// @note     This method leaves the instance unnamed, so it's not good to use in Trick sims because
///           the Trick dynamic memory allocations will throw error messages.  We're only keeping
///           method because a lot of GUNNS unit tests use it to create test utility fluids.
////////////////////////////////////////////////////////////////////////////////////////////////////
PolyFluid::PolyFluid(const PolyFluidConfigData& configData,
                     const PolyFluidInputData&  inputData)
    :
    mName(),
    mTemperature(inputData.mTemperature),
    mPressure(inputData.mPressure),
    mFlowRate(inputData.mFlowRate),
    mMass(inputData.mMass),
    mMole(0.0),
    mConstituents(0),
    mNConstituents(configData.mNTypes),
    mPhase(FluidProperties::NO_PHASE),
    mMWeight(0.0),
    mDensity(0.0),
    mViscosity(0.0),
    mSpecificHeat(0.0),
    mSpecificEnthalpy(0.0),
    mThermalConductivity(0.0),
    mPrandtlNumber(0.0),
    mAdiabaticIndex(0.0),
    mTraceCompounds(0),
    mInitFlag(false)
{
    initializeName("Unnamed PolyFluid");
    initialize(configData, inputData);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that   (--) Source to copy.
/// @param[in] name   (--) Instance name for messages.
/// @param[in] copyTc (--) Copy the source's trace compounds if true, otherwise don't create a
///                        trace compounds.
///
/// @throws   TsInitializationException
///
/// @details  Copy constructs this PolyFluid.  The extra name argument is needed because Trick
///           requires all dynamic allocations to have a sim-unique name.  Therefore this fluid
///           object must be named when it is constructed.
////////////////////////////////////////////////////////////////////////////////////////////////////
PolyFluid::PolyFluid(const PolyFluid& that, const std::string& name, const bool copyTc)
    :
    mName(),
    mTemperature(that.mTemperature),
    mPressure(that.mPressure),
    mFlowRate(that.mFlowRate),
    mMass(that.mMass),
    mMole(that.mMole),
    mConstituents(0),
    mNConstituents(that.mNConstituents),
    mPhase(that.mPhase),
    mMWeight(that.mMWeight),
    mDensity(that.mDensity),
    mViscosity(that.mViscosity),
    mSpecificHeat(that.mSpecificHeat),
    mSpecificEnthalpy(that.mSpecificEnthalpy),
    mThermalConductivity(that.mThermalConductivity),
    mPrandtlNumber(that.mPrandtlNumber),
    mAdiabaticIndex(that.mAdiabaticIndex),
    mTraceCompounds(0),
    mInitFlag(that.mInitFlag)
{
    /// - Throw an exception if empty name.
    if (name.empty()) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Empty instance name.",
                        TsInitializationException, "Invalid Initialization Data", "PolyFluid");
    }

    /// - Initialize the instance name.
    mName = name;

    MonoFluidInputData input(that.mTemperature, that.mPressure, that.mFlowRate, that.mMass);

    /// - Deallocate the constituent array.
    TS_DELETE_ARRAY(mConstituents);

    /// - Allocate and initialize the constituent array and fluids.
    TS_NEW_CLASS_ARRAY_EXT(mConstituents, mNConstituents, Constituent, (), mName + ".mConstituents");
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mType         = that.mConstituents[i].mType;
        mConstituents[i].mFluid.initialize(that.mConstituents[i].mFluid.getProperties(),
                                            input);
        mConstituents[i].mMassFraction = that.mConstituents[i].mMassFraction;
        mConstituents[i].mMoleFraction = that.mConstituents[i].mMoleFraction;
    }

    /// - Copy the trace compounds if it exists.  This is a deep copy so the new fluid's trace
    ///   compounds has its own memory and reference to this fluid's mMole term.
    if (copyTc and that.getTraceCompounds()) {
        TS_NEW_PRIM_OBJECT_EXT(mTraceCompounds, GunnsFluidTraceCompounds,
                               (*that.getTraceCompounds(), mMole, std::string(mName) + ".mTraceCompounds"),
                               std::string(mName) + ".mTraceCompounds");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
PolyFluid::~PolyFluid()
{
    /// - Deallocate the constituent fluids and array.
    cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Cleans up this PolyFluid.  Deletes the mConstituents array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::cleanup()
{
    /// - Deallocate the trace compounds object.
    TS_DELETE_OBJECT(mTraceCompounds);
    /// - Deallocate the constituent fluids and array.
    TS_DELETE_ARRAY(mConstituents);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    configData   (--)    Configuration data.
/// @param[in]    inputData    (--)    Input data.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this PolyFluid with input data.  This must be called after the
///           initializeName method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::initialize(const PolyFluidConfigData& configData,
                           const PolyFluidInputData&  inputData)
{
    mTemperature   = inputData.mTemperature;
    mPressure      = inputData.mPressure;
    mFlowRate      = inputData.mFlowRate;
    mMass          = inputData.mMass;
    mMole          = 0.0;
    mInitFlag      = false;

    /// - Throw an exception if empty name.
    if (mName.empty()) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Empty instance name.",
                        TsInitializationException, "Invalid Initialization Data", "PolyFluid");
    }

    /// - Throw an exception if null pointer to predefined fluid properties.
    if (0 == configData.mProperties) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Null pointer to predefined fluid properties.",
                        TsInitializationException, "Invalid Configuration Data", mName);
    }

    /// - Throw an exception if number of types < 1.
    if (1 > configData.mNTypes) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Number of types < 1.",
                        TsInitializationException, "Invalid Configuration Data", mName);
    }

    /// - Throw an exception if null pointer to array of types.
    if (0 == configData.mTypes) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Null pointer to constituent types array.",
                        TsInitializationException, "Invalid Configuration Data", mName);
    }

    /// - Throw an exception if null pointer to array of mass fractions.
    if (0 == inputData.mMassFraction) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Null pointer to constituent mass fractions array.",
                        TsInitializationException, "Invalid Input Data", mName);
    }

    /// - Deallocate any constituent fluids and deallocate the constituent array.
    TS_DELETE_ARRAY(mConstituents);
    mNConstituents = 0;

    /// - Allocate and initialize the constituent array and fluids.
    TS_NEW_CLASS_ARRAY_EXT(mConstituents, configData.mNTypes, Constituent, ());
    for (int i = 0; i < configData.mNTypes; ++i) {
        mNConstituents++;
        mConstituents[i].mType         = configData.mTypes[i];
        try {
            mConstituents[i].mFluid.initialize(configData.mProperties->
                    getProperties(mConstituents[i].mType), inputData);
        } catch (...) {
            /// - Catch any exceptions and delete the constituent fluids and array
            TS_DELETE_ARRAY(mConstituents);
            /// - and then re-throw the exception.
            TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Constituent fluid initialization failed.",
                    TsInitializationException, "Invalid Configuration Data", mName);
        }
        mConstituents[i].mMassFraction = inputData.mMassFraction[i];
        mConstituents[i].mMoleFraction = inputData.mMassFraction[i];
    }

    /// - Allocate the trace compounds object if its config data is present.
    if (configData.mTraceCompounds) {
        TS_NEW_PRIM_OBJECT_EXT(mTraceCompounds, GunnsFluidTraceCompounds,
                               (mMole), std::string(mName) + ".mTraceCompounds");
    }

    try {
        /// - Validate the initial data.
        validate();
    } catch (...) {
        /// - Catch any exceptions and delete the constituent fluids and array
        TS_DELETE_ARRAY(mConstituents);
        /// - and then re-throw the exception.
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Validation failed.",
                TsInitializationException, "Invalid Configuration Data", mName);
    }

    /// - Compute the derived data.
    derive();

    /// - Initialize the trace compounds object if it exists.
    if (mTraceCompounds) {
        mTraceCompounds->initialize(configData.mTraceCompounds, inputData.mTraceCompounds,
                                    std::string(mName) + ".mTraceCompounds");
    }

    /// - Set the flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    name (--) Instance name for messages.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this PolyFluid's instance name attribute.  This must be called before the
///           initialize method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::initializeName(const std::string& name)
{
    /// - Throw an exception if empty name.
    if (name.empty()) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Empty instance name.",
                        TsInitializationException, "Invalid Initialization Data", "PolyFluid");
    }

    /// - Initialize the instance name.
    mName = name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates this PolyFluid input data.  The input data temperature and pressure must be
///           >= FLT_EPSILON, but we don't check it here since we rely on the constituent MonoFluid
///           to already have checked it.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::validate()
{
    /// - Reset initialization flag
    mInitFlag = false;

    /// - Check that the mass fractions add up to 1. Normalize if close, otherwise throw exception.
    double one = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        one += mConstituents[i].mMassFraction;
    }
    const double error = fabs(1.0 - one);
    if (error > FRACTION_TOLERANCE) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Mass fractions don't add up to 1.",
                        TsInitializationException, "Invalid Input Data", mName);
    } else if (error > static_cast<double>(FLT_EPSILON)) {
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " normalized mass fractions.";
        hsSendMsg(msg);
        for (int i = 0; i < mNConstituents; ++i) {
            mConstituents[i].mMassFraction  /= one;
        }
    }

    /// - Throw an exception if non-zero fraction constituents don't have the same phase.
    //    The mass fractions add up to 1, so at least one constituent has a non-zero mass fraction.
    //    The defined fluids are either GAS or LIQUID phase, so mPhase will be reset from SOLID.
    mPhase = FluidProperties::NO_PHASE;
    for (int i = 0; i < mNConstituents; ++i) {
        if (fabs(mConstituents[i].mMassFraction) > DBL_EPSILON) {
            if (FluidProperties::NO_PHASE == mPhase) {
                mPhase = mConstituents[i].mFluid.getPhase();
            } else if (mConstituents[i].mFluid.getPhase() != mPhase) {
                TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS",
                                "Non-zero constituent fluids not all the same phase.",
                                TsInitializationException, "Invalid Input Data", mName);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @details  Computes this PolyFluid derived state and properties from validated input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::derive()
{
    /// - Set the constituent fractions and update the constituent states from the composite state.
    double molesPerUnitMass = 0;
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mMoleFraction /= mConstituents[i].mFluid.getMWeight();
        molesPerUnitMass               += mConstituents[i].mMoleFraction;
        mConstituents[i].mFluid.setTemperature(mTemperature);
        mConstituents[i].mFluid.setFlowRate(mConstituents[i].mMassFraction * mFlowRate);
        mConstituents[i].mFluid.setMass(mConstituents[i].mMassFraction * mMass);
    }

    /// - Bound moles/unit mass away from zero
    molesPerUnitMass = MsMath::innerLimit(-DBL_EPSILON, molesPerUnitMass, +DBL_EPSILON);

    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mMoleFraction /= molesPerUnitMass;
        mConstituents[i].mFluid.setPressure(mConstituents[i].mMoleFraction * mPressure);
    }

    /// - Then update the composite properties from the constituent properties.
    mMWeight             = 1.0 / molesPerUnitMass;
    mMole                = mMass / mMWeight;
    mDensity             = 0.0;
    mViscosity           = 0.0;
    mSpecificHeat        = 0.0;
    mThermalConductivity = 0.0;
    mPrandtlNumber       = 0.0;
    mAdiabaticIndex      = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        if (fabs(mConstituents[i].mMoleFraction) > DBL_EPSILON) {
            mDensity             += mConstituents[i].mFluid.getDensity();
            mViscosity           += mConstituents[i].mFluid.getViscosity() *
                                    mConstituents[i].mMoleFraction;
            mSpecificHeat        += mConstituents[i].mFluid.getSpecificHeat() *
                                    mConstituents[i].mMassFraction;
            mThermalConductivity += mConstituents[i].mFluid.getThermalConductivity() *
                                    mConstituents[i].mMassFraction;
            mPrandtlNumber       += mConstituents[i].mFluid.getPrandtlNumber() *
                                    mConstituents[i].mMoleFraction;
            mAdiabaticIndex      += mConstituents[i].mFluid.getAdiabaticIndex()*
                                    mConstituents[i].mMoleFraction;
        }
    }
    mSpecificEnthalpy = mSpecificHeat * mTemperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   index (--) Index of fluid
///
/// @return  (--) Mass fraction of the specified constituent fluid in this PolyFluid
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the mass fraction constituent fluid at the specified index in this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
double PolyFluid::getMassFraction(const int index) const
{
    /// - Throw an exception if fluid index is out of bounds.
    if (index < 0 || index >= mNConstituents) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Invalid constituent index.",
                        TsOutOfBoundsException, "Input Argument Out of Range", mName);
    }

    return mConstituents[index].mMassFraction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   index (--) Index of fluid
///
/// @return  (--) Mole fraction of the specified constituent fluid in this PolyFluid
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the mole fraction constituent fluid at the specified index in this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
double PolyFluid::getMoleFraction(const int index) const
{
    /// - Throw an exception if fluid index is out of bounds.
    if (index < 0 || index >= mNConstituents) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Invalid constituent index.",
                        TsOutOfBoundsException, "Input Argument Out of Range", mName);
    }

    return mConstituents[index].mMoleFraction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature  (K) Temperature
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Sets the temperature (K) of this PolyFluid. The constituent fluid temperatures and
///           pressures are updated from the composite temperature and pressure and then the
///           composite properties are updated from the constituent properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::setTemperature(const double temperature)
{
    /// - Set the temperature of the composite fluid.
    mTemperature         = temperature;

    mDensity             = 0.0;
    mViscosity           = 0.0;
    mSpecificHeat        = 0.0;
    mThermalConductivity = 0.0;
    mPrandtlNumber       = 0.0;
    mAdiabaticIndex      = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        /// - Update the temperature and pressure of the constituent fluids.
        const double moleFraction = mConstituents[i].mMoleFraction;
        mConstituents[i].mFluid.setTemperature(mTemperature);
        mConstituents[i].mFluid.setPressure(mPressure * moleFraction);
        /// - Update the properties of the composite fluid from the constituent fluid properties.
         if (fabs(moleFraction) > DBL_EPSILON) {
             const double massFraction = mConstituents[i].mMassFraction;
             mDensity                 += mConstituents[i].mFluid.getDensity();
             mViscosity               += mConstituents[i].mFluid.getViscosity()     * moleFraction;
             mSpecificHeat            += mConstituents[i].mFluid.getSpecificHeat()  * massFraction;
             mThermalConductivity     += mConstituents[i].mFluid.getThermalConductivity() *
                                         massFraction;
             mPrandtlNumber           += mConstituents[i].mFluid.getPrandtlNumber() * moleFraction;
             mAdiabaticIndex          += mConstituents[i].mFluid.getAdiabaticIndex()* moleFraction;
         }
    }
    mSpecificEnthalpy = mSpecificHeat * mTemperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    pressure  (kPa) Pressure
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Sets the pressure (kPa) of this PolyFluid and updates the density. The constituent
///           fluid states and other PolyFluid properties are not updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::setPressure(const double pressure)
{
    /// - Set the pressure of the composite fluid.
    mPressure  = pressure;

    /// - Update the density of the composite fluid from the constituent fluid densities.
    mDensity   = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        if (fabs(mConstituents[i].mMoleFraction) > DBL_EPSILON) {
            mDensity += mConstituents[i].mFluid.getProperties()->
                        getDensity(mTemperature, mPressure * mConstituents[i].mMoleFraction);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      mass            (kg)   Mass of the PolyFluid
/// @param[in,out]  massFraction    (--)   Array of constituent fluid mass fractions
///
/// @return   void
///
/// @throws   TsOutOfBoundsException
///
/// @note     The composite properties are not updated until a subsequent call to setTemperature.
///
/// @note     The caller's massFraction array is normalized if necessary.
///
/// @note     The constituents with non-zero mass fractions should all have the same phase.
///
/// @note     The input mass fraction array should be of the same length and in the same order as
///           configuration data types array.
///
/// @details  Sets the mass of this PolyFluid and the mass fractions of its constituent fluids. The
///           constituent fluid mass, moles and mole fractions and PolyFluid properties are updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::setMassAndMassFractions(const double mass, double* massFraction)
{
    /// - Check that the mass fractions add up to 1. Normalize if close, otherwise throw exception.
    double one = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        one += massFraction[i];
    }
    const double error = fabs(1.0 - one);
    if (error > FRACTION_TOLERANCE) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Mass fractions don't add up to 1.",
                        TsOutOfBoundsException, "Input Argument Out of Range", mName);
    } else if (error > static_cast<double>(FLT_EPSILON)) {
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " normalized mass fractions.";
        hsSendMsg(msg);
        for (int i = 0; i < mNConstituents; ++i) {
            massFraction[i]  /= one;
        }
    }

    /// - Set the composite mass and constituent mass fractions.
    mMass                   = mass;
    double molesPerUnitMass = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mMassFraction = massFraction[i];
        mConstituents[i].mMoleFraction = mConstituents[i].mMassFraction /
                                         mConstituents[i].mFluid.getMWeight();
        molesPerUnitMass              += mConstituents[i].mMoleFraction;
    }

    /// - Bound moles/unit mass away from zero
    molesPerUnitMass = MsMath::innerLimit(-DBL_EPSILON, molesPerUnitMass, +DBL_EPSILON);

    /// - Then update the composite moles and molecular weight.
    mMWeight          = 1.0 / molesPerUnitMass;
    mMole             = mMass / mMWeight;
    for (int i = 0; i < mNConstituents; ++i) {
        /// - Finally update the constituent mole fractions and fluid quantities.
        mConstituents[i].mMoleFraction /= molesPerUnitMass;
        mConstituents[i].mFluid.setMole(mConstituents[i].mMoleFraction * mMole);
        mConstituents[i].mFluid.setFlowRate(mConstituents[i].mMassFraction * mFlowRate);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      mole            (kg*mol)  Moles of the PolyFluid
/// @param[in,out]  moleFraction    (--)      Array of constituent fluid mole fractions
///
/// @return   void
///
/// @throws   TsOutOfBoundsException
///
/// @note     The composite properties are not updated until a subsequent call to setTemperature.
///
/// @note     The caller's moleFraction array is normalized if necessary.
///
/// @note     The constituents with non-zero mole fractions should all have the same phase.
///
/// @note     The input mole fraction array should be of the same length and in the same order as
///           configuration data types array.
///
/// @details  Sets the moles of this PolyFluid and the mole fractions of its constituent fluids. The
///           constituent fluid mass, moles and mass fractions are updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::setMoleAndMoleFractions(const double mole, double* moleFraction)
{
    /// - Check that the mole fractions add up to 1. Normalize if close, otherwise throw exception.
    double one = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        one += moleFraction[i];
    }
    const double error = fabs(1.0 - one);
    if (error > FRACTION_TOLERANCE) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Mole fractions don't add up to 1.",
                        TsOutOfBoundsException, "Input Argument Out of Range", mName);
    } else if (error > static_cast<double>(FLT_EPSILON)) {
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " normalized mass fractions.";
        hsSendMsg(msg);
        for (int i = 0; i < mNConstituents; ++i) {
            moleFraction[i]  /= one;
        }
    }

    /// - Set the composite moles.
    mMole = mole;

    /// - Set the constituent mole fractions and update the constituent molecular weight.
    mMWeight = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mMoleFraction = moleFraction[i];
        mConstituents[i].mMassFraction   = mConstituents[i].mMoleFraction *
                                           mConstituents[i].mFluid.getMWeight();
        mMWeight                        += mConstituents[i].mMassFraction;
    }

    /// - Bound molecular weight away from zero
    mMWeight = MsMath::innerLimit(-DBL_EPSILON, mMWeight, +DBL_EPSILON);

    /// - Update the mass fractions and composite mass and moles.
    mMass             = mMole * mMWeight;
    for (int i = 0; i < mNConstituents; ++i) {
        /// - Finally update the constituent mass fractions and fluid quantities.
        mConstituents[i].mMassFraction /= mMWeight;
        mConstituents[i].mFluid.setMass(mConstituents[i].mMassFraction * mMass);
        mConstituents[i].mFluid.setFlowRate(mConstituents[i].mMassFraction * mFlowRate);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   index           (--)   Constituent fluid index in composite fluid
/// @param[in]   mass            (kg)   Mass of the constituent fluid.
///
/// @return   void
///
/// @throws   TsOutOfBoundsException
///
/// @note     The other mass properties are not updated until a subsequent call updateMass.
///
/// @details  Sets the mass of the specified constituent fluid in this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::setMass(const int index, const double mass)
{
    /// - Throw an exception if fluid index is out of bounds.
    if (index < 0 || index >= mNConstituents) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Invalid constituent index.",
                        TsOutOfBoundsException, "Input Argument Out of Range", mName);
    }

    mConstituents[index].mFluid.setMass(mass);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Sets the mass and moles of this PolyFluid and the mass and mole fractions of the
///           constituent fluids based on the mass of the constituent fluids.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::updateMass()
{
    /// - First update the total mass from the constituent masses.
    mMass        = 0.0;
    double moles = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        const double constituentMass   = mConstituents[i].mFluid.getMass();
        mMass                         += constituentMass;
        mConstituents[i].mMassFraction = constituentMass;
    }

    /// - Bound mass away from zero
    mMass = MsMath::innerLimit(-DBL_EPSILON, mMass, +DBL_EPSILON);

    /// - Calculate constituent moles from the constituent mass, then normalize the mass fractions.
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mMoleFraction  = mConstituents[i].mMassFraction /
                                          mConstituents[i].mFluid.getMWeight();
        mConstituents[i].mMassFraction /= mMass;
        moles                          += mConstituents[i].mMoleFraction;
    }

    /// - Bound moles away from zero
    moles = MsMath::innerLimit(-DBL_EPSILON, moles, +DBL_EPSILON);

    /// - Next update the constituent mass fractions.
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mMoleFraction   /= moles;
    }

    /// - And finally update the composite moles and molecular weight.
    mMWeight          = mMass / moles;
    mMole             = moles;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Resets (zeros) the state of this PolyFluid.  The mass and moles are not updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::resetState()
{
    /// - Reset the composite state.
    mTemperature = 0.0;
    mPressure    = 0.0;
    mFlowRate    = 0.0;
    mMass        = 0.0;
    mMole        = 0.0;

    /// - Reset the constituent states.
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mFluid.resetState();
        mConstituents[i].mMassFraction = 0.0;
        mConstituents[i].mMoleFraction = 0.0;
    }

    /// - Reset the composite properties.
    mMWeight             = 0.0;
    mDensity             = 0.0;
    mViscosity           = 0.0;
    mSpecificHeat        = 0.0;
    mSpecificEnthalpy    = 0.0;
    mThermalConductivity = 0.0;
    mPrandtlNumber       = 0.0;
    mAdiabaticIndex      = 0.0;

    /// - Reset the trace compounds.
    if (mTraceCompounds) {
        mTraceCompounds->setMasses();
        mTraceCompounds->setMoleFractions();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    src        (--)    Pointer to the fluid to set the state from
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Updates the state of the PolyFluid by copying from the specified PolyFluid state.
///           The mass and moles are not updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::setState(const PolyFluid* src)
{
    /// - Check for constituent consistency between this and source.
    if (mNConstituents != src->mNConstituents) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS",
                        "The source object has different number of constituents.",
                        TsOutOfBoundsException, "Input Argument Out of Range", mName);
    }
    for (int i = 0; i < mNConstituents; ++i) {
        if (mConstituents[i].mType != src->mConstituents[i].mType) {
            TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "The source object has different constituents.",
                            TsOutOfBoundsException, "Input Argument Out of Range", mName);
        }
    }

    /// - Set the composite state (except for mass & moles).
    mTemperature      = src->mTemperature;
    mPressure         = src->mPressure;
    mFlowRate         = src->mFlowRate;

    /// - Set the constituent states.
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mMassFraction = src->mConstituents[i].mMassFraction;
        mConstituents[i].mMoleFraction = src->mConstituents[i].mMoleFraction;
        mConstituents[i].mFluid.setState(&(src->mConstituents[i].mFluid));
    }

    /// - Set the composite properties.
    mMWeight             = src->mMWeight;
    mDensity             = src->mDensity;
    mViscosity           = src->mViscosity;
    mSpecificHeat        = src->mSpecificHeat;
    mSpecificEnthalpy    = src->mSpecificEnthalpy;
    mThermalConductivity = src->mThermalConductivity;
    mPrandtlNumber       = src->mPrandtlNumber;
    mAdiabaticIndex      = src->mAdiabaticIndex;

    /// - Set the trace compounds.
    if (mTraceCompounds) {
        mTraceCompounds->setMoleFractions(src->getTraceCompounds()->getMoleFractions());
    }
 }


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    src         (--)   Pointer to the PolyFluid to add in the state from
/// @param[in]    flowRate    (kg/s) Optional overriding source mass flow rate
///
/// @return   void
///
/// @throws   TsInitializationException, TsOutOfBoundsException
///
/// @details  Updates the state of the PolyFluid by adding in the specified PolyFluid state. The
///           mass and moles are undefined and the pressure, partial pressure and density are
///           approximate.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::addState(const PolyFluid* src, const double flowRate)
{
    /// - Check for constituent consistency between this and source.
    if (mNConstituents != src->mNConstituents) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS",
                        "The source object has different number of constituents.",
                        TsOutOfBoundsException, "Input Argument Out of Range", mName);
    }
    for (int i = 0; i < mNConstituents; ++i) {
        if (mConstituents[i].mType != src->mConstituents[i].mType) {
            TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "The source object has different constituents.",
                            TsOutOfBoundsException, "Input Argument Out of Range", mName);
        }
    }

    /// - First determine the source mass flow rate which may be overridden by a non-zero argument
    ///   value and them compute the combined mass flow rate.
    double destFlowRate = mFlowRate;
    double srcFlowRate;
    if (fabs(flowRate) < DBL_EPSILON) {
        srcFlowRate  = src->mFlowRate;
    } else {
        srcFlowRate  =  flowRate;
    }
    mFlowRate += srcFlowRate;

    /// - Throw an exception if mass flow rate is too small.
    if (fabs(mFlowRate) < DBL_EPSILON) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Combined flow rate magnitude < DBL_EPSILON.",
                        TsOutOfBoundsException, "Input Argument Out of Range", mName);
    }

    /// - Next combine the constituent fluids and compute the combined PolyFluid mole flow rate.
    double moleFlowRate = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mFluid.addState(&(src->mConstituents[i].mFluid),
                        srcFlowRate * src->mConstituents[i].mMassFraction);
        moleFlowRate                  += mConstituents[i].mFluid.getFlowRate() /
                                         mConstituents[i].mFluid.getMWeight();
    }
    /// - Bound mole flow rate away from zero.  Since mass flow rate is limited at DBL_EPSILON, we
    ///   can legitimately have a small mole rate that is <DBL_EPSILON and still valid, so we use a
    ///   smaller limit here.
    moleFlowRate = MsMath::innerLimit(-MOLE_INNER_LIMIT, moleFlowRate, MOLE_INNER_LIMIT);

    /// - And compute the combined constituent mass fractions proportional to the flow rate.
    double molesPerUnitMass = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mMassFraction = mConstituents[i].mFluid.getFlowRate() / mFlowRate;
        mConstituents[i].mMoleFraction = mConstituents[i].mMassFraction /
                                         mConstituents[i].mFluid.getMWeight();
        molesPerUnitMass              += mConstituents[i].mMoleFraction;
    }

    /// - Bound moles/unit mass away from zero.
    molesPerUnitMass = MsMath::innerLimit(-MOLE_INNER_LIMIT, molesPerUnitMass, MOLE_INNER_LIMIT);

    /// - Now update the combined constituent mole fractions and combined composite pressure.
    mPressure    = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mMoleFraction /= molesPerUnitMass;
        mPressure                      += mConstituents[i].mFluid.getPressure() *
                                          mConstituents[i].mMoleFraction;
    }

    /// - And update the composite specific enthalpy and temperature
    mSpecificEnthalpy    = src->getSpecificEnthalpy() * srcFlowRate / mFlowRate +
                           mSpecificEnthalpy * destFlowRate / mFlowRate;
    mTemperature         = computeTemperature(mSpecificEnthalpy);

    /// - Update the remaining composite properties from the constituent properties.
    mMWeight             = mFlowRate / moleFlowRate;
    mDensity             = 0.0;
    mViscosity           = 0.0;
    mSpecificHeat        = 0.0;
    mThermalConductivity = 0.0;
    mPrandtlNumber       = 0.0;
    mAdiabaticIndex      = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mFluid.setTemperature(mTemperature);
        if (fabs(mConstituents[i].mMoleFraction) > DBL_EPSILON) {
            mDensity             += mConstituents[i].mFluid.getDensity();
            mViscosity           += mConstituents[i].mFluid.getViscosity() *
                                    mConstituents[i].mMoleFraction;
            mSpecificHeat        += mConstituents[i].mFluid.getSpecificHeat() *
                                    mConstituents[i].mMassFraction;
            mThermalConductivity += mConstituents[i].mFluid.getThermalConductivity() *
                                    mConstituents[i].mMassFraction;
            mPrandtlNumber       += mConstituents[i].mFluid.getPrandtlNumber() *
                                    mConstituents[i].mMoleFraction;
            mAdiabaticIndex      += mConstituents[i].mFluid.getAdiabaticIndex() *
                                    mConstituents[i].mMoleFraction;
        }
    }

    /// - Mix in the trace compounds.  The mMoles term is temporarily set to the total resulting
    ///   moles of the mixed fluid in order for the trace compounds object to properly update its
    ///   mole fractions.  mMole is then reset to its previous value since this method is not
    ///   supposed to change it.
    GunnsFluidTraceCompounds* srcTraceCompounds = src->getTraceCompounds();
    double                    srcMWeight        = src->getMWeight();
    if (mTraceCompounds and srcTraceCompounds and srcMWeight > DBL_EPSILON) {
        const double savedMole = mMole;
        mMole = moleFlowRate;
        mTraceCompounds->flowIn(*srcTraceCompounds, srcFlowRate / srcMWeight);
        mMole = savedMole;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature  (K)   Temperature
/// @param[in]    pressure     (kPa) Pressure
///
/// @return   void
///
/// @details  Sets the temperature and pressure of this PolyFluid. The constituent fluid
///           temperatures and pressures are updated from the composite temperature and pressure and
///           then the composite properties are updated from the constituent properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::edit(const double temperature, const double pressure)
{
    /// - Set the composite pressure
    mPressure  = fmax(0.0, pressure);

    /// - And let setTemperature do the rest
    setTemperature(fmax(0.0, temperature));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature      (K)   Temperature
/// @param[in]    partialPressure  (kPa) Partial pressure array
///
/// @return   void
///
/// @details  Sets the temperature and partial pressures of this PolyFluid. The constituent fluid
///           temperatures and composite pressure are updated from the composite temperature and
///           partial pressures and then the composite properties are updated from the constituent
///           properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::edit(const double temperature, double* partialPressure)
{
    /// - Set the composite pressure as the sum of the constituent partial pressures
    mPressure = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        mPressure += fmax(0.0, partialPressure[i]);
    }

    /// - Call setTemperature to set the fluid to the new total pressure and temperature.
    setTemperature(fmax(0.0, temperature));

    /// - Return after setting pressure and temperature if pressure is zero to avoid divide-by zero.
    if (mPressure < static_cast<double>(FLT_EPSILON)) {
        return;
    }

    /// - Set the constituent mole fractions as the ratios of partial pressure to total pressure
    double* moleFractions = new double[mNConstituents];

    // Because this block can throw exceptions, do a try/catch to prevent possible memory leaks
    // between the new/delete.
    try {
        for (int i = 0; i < mNConstituents; ++i) {
            moleFractions[i] = fmax(0.0, partialPressure[i]) / mPressure;
        }
        setMoleAndMoleFractions(getMole(), moleFractions);

        /// - And let setTemperature do the rest
        setTemperature(fmax(0.0, temperature));

    } catch (...) {
        TsHsMsg warnMsg(TS_HS_WARNING, "GUNNS");
        warnMsg << mName << " caught exception during edit.";
        hsSendMsg(warnMsg);
    }

    delete [] moleFractions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    specificEnthalpy   (J/kg)     Specific enthalpy to compute temperature at
///
/// @return   (K) The temperature this PolyFluid at the specified specific enthalpy.
///
/// @throws   TsInitializationException
///
/// @details   Returns this PolyFluid temperature corresponding to the specified specific enthalpy.
///            -# The constituent and PolyFluid states are not updated.
///            -# computeTemperature(getSpecificEnthalpy(temperature)) == temperature
///              for energy conservation
////////////////////////////////////////////////////////////////////////////////////////////////////
double PolyFluid::computeTemperature(const double specificEnthalpy) const
{
    /// - Since the specific heat curve fit for each of the constituents is linear, the composite
    ///   temperature is the root of a quadratic equation whose coefficients are the mass
    ///   fraction weighted coefficients of the constituent curve fits and the composite specific
    ///   enthalpy: a * x^2 + b * x - h.
    double a        = 0.0;
    double b        = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        const double massFraction = mConstituents[i].mMassFraction;
        if (fabs(massFraction) > DBL_EPSILON) {
            const LinearFit* cpFit = dynamic_cast<const LinearFit*>(mConstituents[i].mFluid.
                                                                    mProperties->mSpecificHeat);
            a += massFraction * cpFit->getB(); // scale factor in linear curve fit
            b += massFraction * cpFit->getA(); // bias in linear curve fit
        }
    };

    /// - Specific Heat Cp = b + a * T.  Specific Enthalpy h = Cp * T = b * T + a * T^2.
    ///   Quadratic: a * T^2 + b * T - h = 0.  Hence the 'c' term in the quadratic equation = -h,
    ///   so -4ac = +4ah.
    // Protect for square root of negative number
    const double temp = std::max( (b * b + 4.0 * a * specificEnthalpy), 0.0);
    return 0.5 * (-b + sqrt(temp)) / a;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature   (K)    Temperature to compute specific enthalpy at
/// @param[in]    pressure      (kPa)  Pressure to compute specific enthalpy at
///
/// @return   (J/kg) The specific enthalpy this PolyFluid at the specified temperature and pressure.
///
/// @throws   TsInitializationException
///
/// @details  Returns the PolyFluid specific enthalpy (as the mole fraction weighted average of
///           constituent specific enthalpies) computed at the specified temperature and pressure.
///           The constituent and PolyFluid states are not updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
double PolyFluid::computeSpecificEnthalpy(const double temperature, const double pressure) const
{
    /// - Compute composite specific heat as the mass fraction weighted sum of the constituent
    ///   specific heats.
    double specificHeat = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        const double massFraction = mConstituents[i].mMassFraction;
        if (fabs(massFraction) > DBL_EPSILON) {
            specificHeat += mConstituents[i].mFluid.mProperties->mSpecificHeat->get(temperature,
                            pressure) * massFraction;
        }
    }

    /// - Return the composite specific enthalpy at the requested temperature and pressure.
    return specificHeat * temperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature   (K)     Temperature to compute pressure at
/// @param[in]    density       (kg/m3) Density to compute pressure at
///
/// @return   (kPa) The pressure this PolyFluid at the specified temperature and density.
///
/// @note     computePressure(temperature, getDensity(temperature, pressure)) == pressure for mass
///           conservation
///
/// @details  Returns this PolyFluid pressure computed at the specified temperature and density.
///           The constituent and PolyFluid states are not updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
double PolyFluid::computePressure(const double temperature, const double density) const
{
    /// - Compute composite pressure as the sum of the constituent pressures.
    double pressure   = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        const double massFraction = mConstituents[i].mMassFraction;
        if (fabs(massFraction) > DBL_EPSILON) {
            pressure += mConstituents[i].mFluid.computePressure(temperature,
                                                                 density * massFraction);
        }
    }

    return pressure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature   (K)    Temperature to compute density at
/// @param[in]    pressure      (kPa)  Pressure to compute density at
///
/// @return   (kg/m3) The density of this PolyFluid at the specified temperature and pressure.
///
/// @note     computeDensity(temperature, getPressure(temperature, density)) == density for mass
///           conservation
///
/// @details  Returns this PolyFluid density computed at the specified temperature and pressure.
///           The constituent and PolyFluid states are not updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
double PolyFluid::computeDensity(const double temperature, const double pressure) const
{
    /// - Compute composite density as the sum of the constituent densities.
    double density = 0.0;
    for (int i = 0; i < mNConstituents; ++i) {
        const double moleFraction = mConstituents[i].mMoleFraction;
        if (fabs(moleFraction) > DBL_EPSILON) {
            density += mConstituents[i].mFluid.computeDensity(temperature,
                                                               pressure * moleFraction);
        }
    }

    return density;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    type  (--)   Type of constituent fluid
///
/// @return   Index into constituent array corresponding to specified constituent type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the array index in the constituent array corresponding to the specified
///           constituent type.
////////////////////////////////////////////////////////////////////////////////////////////////////
int PolyFluid::find(const FluidProperties::FluidType& type) const
{
    /// - Return the index if found
    for (int i = 0; i < mNConstituents; ++i) {
        if (mConstituents[i].mType == type) {
            return i;
        }
    }

    /// - Otherwise throw an exception
    std::ostringstream msg;
    msg << "An invalid fluid type (" << type << ") was specified.";
    TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", msg.str(),
                    TsOutOfBoundsException, "Input Argument Out of Range", mName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] fluidIndex (--) Reference returned index of the compound in the bulk fluid constituents.
/// @param[out] tcIndex    (--) Reference returned index of the compound in the trace compounds.
/// @param[in]  compound   (--) Pointer to the properties of the chemical compound to find.
///
/// @details  Returns the fluid and trace compound indexes of the given chemical compound in this
///           PolyFluid.  This treats the given compound type as optional and indicates in the
///           returned values if compound is not found.
///
/// @note  The respective indexes for fluid constituents and trace compounds are returned a value of
///        -1 if the compound isn't found in them.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::findCompound(int&                    fluidIndex,
                             int&                    tcIndex,
                             const ChemicalCompound* compound) const
{
    fluidIndex = -1;
    for (int i = 0; i < mNConstituents; ++i) {
        if (mConstituents[i].mType == compound->mFluidType) {
            fluidIndex = i;
            break;
        }
    }

    tcIndex = -1;
    if (mTraceCompounds) {
        tcIndex = mTraceCompounds->findCompound(compound->mType);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   mass  (kg)  Mass of the PolyFluid
///
/// @return   void
///
/// @details  Sets the mass and updates the moles of this PolyFluid and its constituent fluids, and
///           updates the masses of the trace compounds based on the new total moles and their
///           previous mole fractions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyFluid::setMass(const double mass)
{
    /// - Bound molecular weight away from zero
    mMWeight = MsMath::innerLimit(-DBL_EPSILON, mMWeight, +DBL_EPSILON);
    /// - Set the composite mass and update the composite moles.
    mMass = mass;
    mMole = mMass / mMWeight;

    /// - Apportion the composite mass and moles to the constituents according to their mass and
    ///   mole fractions.
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mFluid.setMass(mConstituents[i].mMassFraction * mMass);
    }

    /// - Holding the trace compound mole fractions the same, recompute new compound masses
    ///   from the new total moles.
    if (mTraceCompounds) {
        mTraceCompounds->updateMasses();
    }
}
