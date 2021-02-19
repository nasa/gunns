/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the GUNNS Fluid Separator Gas link model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o))

PROGRAMMERS:
   ((Kenneth McMurtrie) (Tietronix Software) (2011-11) (Initial)
    (Jason Harvey)      (L-3 Communications) (2012-12) (Base on conductor with extra source))
**************************************************************************************************/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

#include "GunnsFluidSeparatorGas.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                 (--)             Name of object.
/// @param[in,out] nodes                (--)             Pointer to nodes.
/// @param[in]     maxConductivity      (m2)             Maximum effective conductivity of the link.
/// @param[in]     gasType              (--)             Gas phase fluid type of separated constituent.
/// @param[in]     massExponent         (--)             Mass factor exponent in separator power curve.
/// @param[in]     maxLiquidMass        (kg)             Liquid mass capacity of separator.
/// @param[in]     referenceSpeed       (revolution/min) Reference speed for separator power curve.
/// @param[in]     referencePressure    (kPa)            Reference pressure for separator power curve.
/// @param[in]     referenceRemovalRate (kg/s)           Reference liquid removal rate.
///
/// @details  Default constructs this GUNNS Fluid Separator Gas link model configuration data with
///           arguments and obviously invalid default values.  The conductor's expansion scale
///           factor is deliberately configured to zero, as we do not want that effect in the
///           separator.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorGasConfigData::GunnsFluidSeparatorGasConfigData(
                                              const std::string&               name,
                                              GunnsNodeList*                   nodes,
                                              const double                     maxConductivity,
                                              const FluidProperties::FluidType gasType,
                                              const double                     massExponent,
                                              const double                     maxLiquidMass,
                                              const double                     referenceSpeed,
                                              const double                     referencePressure,
                                              const double                     referenceRemovalRate)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, 0.0),
    mGasType(gasType),
    mMassExponent(massExponent),
    mMaxLiquidMass(maxLiquidMass),
    mReferenceSpeed(referenceSpeed),
    mReferencePressure(referencePressure),
    mReferenceRemovalRate(referenceRemovalRate)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Fluid Separator Gas link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorGasConfigData::GunnsFluidSeparatorGasConfigData(
                                                       const GunnsFluidSeparatorGasConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mGasType(that.mGasType),
    mMassExponent(that.mMassExponent),
    mMaxLiquidMass(that.mMaxLiquidMass),
    mReferenceSpeed(that.mReferenceSpeed),
    mReferencePressure(that.mReferencePressure),
    mReferenceRemovalRate(that.mReferenceRemovalRate)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Separator Gas link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorGasConfigData::~GunnsFluidSeparatorGasConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--)             Blockage malfunction flag.
/// @param[in] malfBlockageValue (--)             Blockage malfunction fractional value (0-1).
/// @param[in] separatorSpeed    (revolution/min) Separator speed.
/// @param[in] transferFlowRate  (kg/s)           Flow rate of outgoing liquid.
/// @param[in] liquidMass        (kg)             Mass of liquid in separator.
///
/// @details  Default constructs this GUNNS Fluid Separator Gas link model input data with
///           arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorGasInputData::GunnsFluidSeparatorGasInputData(const bool   malfBlockageFlag,
                                                                 const double malfBlockageValue,
                                                                 const double separatorSpeed,
                                                                 const double transferFlowRate,
                                                                 const double liquidMass)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mSeparatorSpeed(separatorSpeed),
    mTransferFlowRate(transferFlowRate),
    mLiquidMass(liquidMass)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Fluid Separator Gas link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorGasInputData::GunnsFluidSeparatorGasInputData(
                                                        const GunnsFluidSeparatorGasInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mSeparatorSpeed(that.mSeparatorSpeed),
    mTransferFlowRate(that.mTransferFlowRate),
    mLiquidMass(that.mLiquidMass)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Separator Gas link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorGasInputData::~GunnsFluidSeparatorGasInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Separator Gas link model with obviously invalid
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorGas::GunnsFluidSeparatorGas()
    :
    GunnsFluidConductor(),
    mGasType(FluidProperties::NO_FLUID),
    mMassExponent(0.0),
    mMaxLiquidMass(0.0),
    mPowerCurveCoefficient(0.0),
    mRemovalRateCoefficient(0.0),
    mGasIndex(-1),
    mSeparatorSpeed(0.0),
    mTransferFlowRate(0.0),
    mLiquidMass(0.0),
    mLiquidMassError(0.0),
    mLiquidDeltaP(0.0),
    mRelativeHumidity(0.0),
    mSeparationRate(0.0),
    mSeparatedLiquid(0),
    mTransferTemperature(0.0),
    mTransferPressure(0.0),
    mLiquidOverflow(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Separator Gas link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorGas::~GunnsFluidSeparatorGas()
{
    TS_DELETE_OBJECT(mSeparatedLiquid);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData (--) Configuration data.
/// @param[in]     inputData  (--) Input data.
/// @param[in,out] links      (--) Link vector.
/// @param[in]     port0      (--) Nominal inlet port map index.
/// @param[in]     port1      (--) Nominal outlet port map index (vacuum node).
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Separator Gas link model with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSeparatorGas::initialize(const GunnsFluidSeparatorGasConfigData& configData,
                                        const GunnsFluidSeparatorGasInputData&  inputData,
                                        std::vector<GunnsBasicLink*>&           links,
                                        const int                               port0,
                                        const int                               port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize derived attributes from configuration and input data.
    derive(configData, inputData);

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   --  Configuration data.
/// @param[in]      inputData    --  Input data.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Separator Gas link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSeparatorGas::validate(const GunnsFluidSeparatorGasConfigData& configData,
                                      const GunnsFluidSeparatorGasInputData&  inputData) const
{
    /// - Throw an exception on gas type not in PolyFluid.
    try {
        mNodes[0]->getContent()->find(configData.mGasType);
    } catch (const TsOutOfBoundsException& e){
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Gas type not in fluid.");
    }

    /// - Throw an exception on mass exponent not (0.1 - 10)
    if (!MsMath::isInRange(0.1, configData.mMassExponent, 10.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Liquid mass exponent not in (0.1 to 10.0).");
    }

    /// - Throw an exception on liquid mass capacity < FLT_EPSILON.
    if (configData.mMaxLiquidMass < FLT_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Liquid mass capacity < FLT_EPSILON.");
    }

    /// - Throw an exception on reference speed < FLT_EPSILON.
    if (configData.mReferenceSpeed < FLT_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference speed < FLT_EPSILON.");
    }

    /// - Throw an exception on reference pressure < FLT_EPSILON.
    if (configData.mReferencePressure < FLT_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference pressure < FLT_EPSILON.");
    }

    /// - Throw an exception on reference liquid removal rate < FLT_EPSILON.
    if (configData.mReferenceRemovalRate < FLT_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Reference liquid removal rate < FLT_EPSILON.");
    }

    /// - Throw an exception on separator speed < 0.
    if (inputData.mSeparatorSpeed < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Separator speed < 0.");
    }

    /// - Throw an exception on mass of liquid in separator < 0.
    if (inputData.mLiquidMass < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Mass of liquid in separator < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSeparatorGas::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mLiquidMassError  = 0.0;
    mLiquidDeltaP     = 0.0;
    mRelativeHumidity = 0.0;
    mSeparationRate   = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   --  Configuration data.
/// @param[in]      inputData    --  Input data.
///
/// @return   void
///
/// @details  Initializes the attributes of this GUNNS Fluid Separator Gas link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSeparatorGas::derive(const GunnsFluidSeparatorGasConfigData& configData,
                                    const GunnsFluidSeparatorGasInputData&  inputData)
{
    /// - Initialize from configuration data.  Divide by zero is protected by the validate method.
    mGasType                 = configData.mGasType;
    mMassExponent            = configData.mMassExponent;
    mMaxLiquidMass           = configData.mMaxLiquidMass;
    mPowerCurveCoefficient   = configData.mReferencePressure / configData.mReferenceSpeed
                             / pow(mMaxLiquidMass, mMassExponent);
    mRemovalRateCoefficient  = configData.mReferenceRemovalRate / configData.mReferenceSpeed;
    mGasIndex                = mNodes[0]->getContent()->find(configData.mGasType);

    /// - Initialize from input data.
    mSeparatorSpeed          = inputData.mSeparatorSpeed;
    mTransferFlowRate        = inputData.mTransferFlowRate;
    mLiquidMass              = inputData.mLiquidMass;
    mLiquidMassError         = 0.0;

    /// - Initialize state.
    mTransferTemperature     = mNodes[0]->getContent()->getTemperature();
    mTransferPressure        = mNodes[0]->getContent()->getPressure();
    mLiquidOverflow          = false;

    /// - Create the internal fluid object.  This is the fluid traveling thru the normal conductive
    ///   path and contains the gas to be separated.
    createInternalFluid();
    mRelativeHumidity        = GunnsFluidUtils::computeRelativeHumidityH2O(mInternalFluid);

    /// - Create the fluid object that represents the separated liquid as 100% separated gas.
    TS_DELETE_OBJECT(mSeparatedLiquid);
    TS_NEW_PRIM_OBJECT_EXT(mSeparatedLiquid, PolyFluid,
                           (*mInternalFluid, std::string(mName) + ".mSeparatedLiquid", false),
                           std::string(mName) + ".mSeparatedLiquid");

    double fractions[FluidProperties::NO_FLUID] = {0.0};
    fractions[mGasIndex]     = 1.0;
    mSeparatedLiquid->setPressure(mTransferPressure);
    mSeparatedLiquid->setFlowRate(0.0);
    mSeparatedLiquid->setMassAndMassFractions(0.0, fractions);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt         (s)    Time step.
/// @param[in]    flowrate   (kg/s) Mass flow rate (not used).
///
/// @return   void
///
/// @details  Updates the atmosphere for the extracted liquid, the liquid mass within the separator,
///           and liquid source pressure head created by the separator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSeparatorGas::updateFluid(const double dt, const double flowrate __attribute__((unused)))
{
    /// - Determine if there is condensate flowing past the separator.  Right now we only support
    ///   water.  Separation will only occur for forward flow direction.
    mRelativeHumidity = GunnsFluidUtils::computeRelativeHumidityH2O(mInternalFluid);
    const bool noCondensate = (mFlowRate < m100EpsilonLimit) or
                              (mRelativeHumidity < 1.0);

    /// - Remove liquid mass transferred from the separator to the liquid network.  As a
    ///   simplification, do not allow backflow into the separator drum from the liquid network.
    mLiquidMass -= std::max(0.0, mTransferFlowRate * dt);

    /// - If the liquid side is depressurized and the separator drum is empty of liquid, then gas
    ///   would flow from the gas side to the liquid side; however we don't bother to simulate this.
    ///   Instead, we cancel the flow demand and liquid mass is created in the liquid side (mass not
    ///   conserved).  We record the created mass as mLiquidMassError.
    if (mLiquidMass < 0.0) {
        mLiquidMassError -= mLiquidMass;
        mLiquidMass = 0.0;
    }

    /// - Skip separation logic if no time step or condensate.
    if (dt < DBL_EPSILON or noCondensate) {
        mSeparationRate = 0.0;
        mLiquidOverflow = false;

    } else {
        /// - Compute the available mass of operating fluid in the stream.
        const double availableMass = mInternalFluid->getMassFraction(mGasIndex) * fabs(mFlowRate) * dt;

        /// - The condensed mass as a function of separator speed.
        const double condensedMass = fmin(availableMass, mRemovalRateCoefficient * mSeparatorSpeed * dt);

        /// - The separated mass limited by capacity in the separator.
        const double separatedMass = fmin(condensedMass, mMaxLiquidMass - mLiquidMass);

        /// - Indicate if not all the condensate was removed from the gas stream.  This can be used
        ///   for output to a liquid detection sensor signal aspect.
        mLiquidOverflow = condensedMass > separatedMass;

        /// - Store the separated mass in the separator drum.
        mLiquidMass    += separatedMass;

        /// - Compute separated liquid mass flow rate.
        mSeparationRate = separatedMass / dt;
    }

    /// - Compute separator delta pressure on liquid as function of speed and mass.
    mLiquidDeltaP = mPowerCurveCoefficient * mSeparatorSpeed * pow(mLiquidMass, mMassExponent);

    if (mSeparationRate > m100EpsilonLimit) {

        /// - Update the separation fluid temperature.
        mSeparatedLiquid->setTemperature(mInternalFluid->getTemperature());

        /// - Remove the separated mass from the downstream node, and update link source vector so
        ///   the pressure solution will reflect the removed mass next pass.  Since the pressure
        ///   lags a cycle there will be a small pressure error, but mass will be conserved and
        ///   GUNNS washes out pressure errors over time.
        mNodes[1]->collectInflux(-mSeparationRate, mSeparatedLiquid);
        mSourceVector[0] = 0.0;
        mSourceVector[1] = -mSeparationRate / mSeparatedLiquid->getMWeight();

    } else {
        mSourceVector[0] = 0.0;
        mSourceVector[1] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @details  Prepares outputs to the liquid separator aspect.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSeparatorGas::processOutputs()
{
    mTransferTemperature = mNodes[0]->getContent()->getTemperature();
    mTransferPressure    = mNodes[0]->getPotential() + mLiquidDeltaP;
}
