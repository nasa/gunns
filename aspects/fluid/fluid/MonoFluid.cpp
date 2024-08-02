/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
 (Provides the classes for modeling a single species fluid.)

REQUIREMENTS:
 ()

REFERENCE:
 ()

ASSUMPTIONS AND LIMITATIONS:
 (Verification of FluidProperties ensures returned values are positive so the may be safely used as
  divisors.)

 LIBRARY DEPENDENCY:
 ((properties/FluidProperties.o))

 PROGRAMMERS:
 ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

***************************************************************************************************/

#include <cmath>
#include <cfloat>

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "MonoFluid.hh"

/// @details  The minimum flow rate used by the addState method to allow the flow rate override.
///           Since a PolyFluid's minimum flow rate is DBL_EPSILON, and a constituent MonoFluid in
///           the PolyFluid may have a mass fraction as low as DBL_EPSILON, we must allow
///           constituent flow rates as low as the square of DBL_EPSILON.
const double MonoFluid::mMinFlowRate = DBL_EPSILON * DBL_EPSILON;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      temperature  (K)    Temperature of MonoFluid input data
/// @param[in]      pressure     (kPa)  Pressure of MonoFluid input data
/// @param[in]      flowRate     (kg/s) Mass flow rate of MonoFluid input data
/// @param[in]      mass         (kg)   Mass of MonoFluid input data
///
/// @details  Default constructs this MonoFluid input data with obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
MonoFluidInputData::MonoFluidInputData(const double temperature,
                                       const double pressure,
                                       const double flowRate,
                                       const double mass)
    :
    mTemperature(temperature),
    mPressure(pressure),
    mFlowRate(flowRate),
    mMass(mass)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that  --  Source to copy
///
/// @details  Copy constructs this MonoFluid input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
MonoFluidInputData::MonoFluidInputData(const MonoFluidInputData& that)
    :
    mTemperature(that.mTemperature),
    mPressure(that.mPressure),
    mFlowRate(that.mFlowRate),
    mMass(that.mMass)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this MonoFluid input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
MonoFluidInputData::~MonoFluidInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this MonoFluid with obviously invalid default values. This should
///           be followed by a call to the initialize method before calling an update method.
////////////////////////////////////////////////////////////////////////////////////////////////////
MonoFluid::MonoFluid()
    :
    mTemperature(0.0),
    mPressure(0.0),
    mFlowRate(0.0),
    mMass(0.0),
    mMole(0.0),
    mProperties(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      properties --   Pointer to fluid properties
/// @param[in]      inputData  --   Input data
///
/// @throws   TsInitializationException
///
/// @details  Constructs this MonoFluid with configuration data (properties) and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
MonoFluid::MonoFluid(const FluidProperties* properties, const MonoFluidInputData& inputData)
    :
    mTemperature(inputData.mTemperature),
    mPressure(inputData.mPressure),
    mFlowRate(inputData.mFlowRate),
    mMass(inputData.mMass),
    mMole(0.0),
    mProperties(const_cast<FluidProperties*>(properties))
{
    /// - Validate the initialization.
    validate();

    /// - Compute the derived data.
    derive();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that  --   Source to copy from
///
/// @details  Copy constructs this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
MonoFluid::MonoFluid(const MonoFluid& that)
    :
    mTemperature(that.mTemperature),
    mPressure(that.mPressure),
    mFlowRate(that.mFlowRate),
    mMass(that.mMass),
    mMole(that.mMole),
    mProperties(that.mProperties)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
MonoFluid::~MonoFluid()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      properties  --  Pointer to fluid properties
/// @param[in]      inputData   --  Input data
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this MonoFluid with configuration data (properties) and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void MonoFluid::initialize(const FluidProperties* properties, const MonoFluidInputData& inputData)
{
    mTemperature   = inputData.mTemperature;
    mPressure      = inputData.mPressure;
    mFlowRate      = inputData.mFlowRate;
    mMass          = inputData.mMass;
    mMole          = 0.0;

    mProperties = const_cast<FluidProperties*>(properties);

    /// - Validate the initialization.
    validate();

    /// - Compute the derived data.
    derive();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates this MonoFluid input and configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void MonoFluid::validate()
{
    /// - Throw  exception if null pointer to properties.
     if (0 == mProperties) {
         TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Null pointer to predefined fluid properties.",
                         TsInitializationException, "Invalid Configuration Data", "MonoFluid");
    }

    /// - Throw  exception if temperature too small.
    if (mTemperature < static_cast<double>(FLT_EPSILON)) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Temperature < FLT_EPSILON.",
                        TsInitializationException, "Invalid Input Data", "MonoFluid");
    }

    /// - Throw  exception if pressure too small.
    if (mPressure < static_cast<double>(FLT_EPSILON)) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Pressure < FLT_EPSILON.",
                        TsInitializationException, "Invalid Input Data", "MonoFluid");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @details  Computes this MonoFluid derived state and properties from validated initialization
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void MonoFluid::derive()
{
    /// - Compute the moles.
    mMole        = mMass / mProperties->getMWeight();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      src          --    Pointer to the MonoFluid to add in the state from
/// @param[in]      flowRate    (kg/s) Optional overriding source mass flow rate
///
/// @return   void
///
/// @note     This protected method is intended for use by PolyFluid whose validation includes
///           validation of its constituent MonoFluids, including type consistency.
///
/// @note     The computed temperature and pressure are simply mass flow rate weighted averages
///           which get overwritten by the PolyFluid of which MonoFluid is a constituent.
///
/// @details  Updates the state of this MonoFluid by adding in the specified MonoFluid state.
///           The flow rate is updated, but the mass and moles are not.
////////////////////////////////////////////////////////////////////////////////////////////////////
void MonoFluid::addState(const MonoFluid* src,  const double flowRate)
{
    /// - A non-zero flow rate argument overrides the source flow rate.
    /// - The combined mass flow rate is the sum of the mass flow rates.
    double srcFlowRate = 0.0;

    /// - If the override flow rate is too small, use the rate inside the source fluid.
    if (fabs(flowRate) < mMinFlowRate) {
        srcFlowRate = src->mFlowRate;
    } else {
        srcFlowRate = flowRate;
    }
    const double newFlowRate = mFlowRate + srcFlowRate;

    if (fabs(newFlowRate) > mMinFlowRate) {
        /// - The combined temperature and pressure are the mass flow rate weighted averages.
        const double fraction           = mFlowRate   / newFlowRate;
        const double srcFraction        = srcFlowRate / newFlowRate;
        mTemperature                    = fabs(mTemperature * fraction +
                                               src->mTemperature * srcFraction);
        mPressure                       = fabs(mPressure * fraction +
                                               src->mPressure * srcFraction);
    } else {
        /// - Or 50/50 if the mass flow rate is sufficiently small.
        mPressure                       = 0.5 * (mPressure    + src->mPressure);
        mTemperature                    = 0.5 * (mTemperature + src->mTemperature);
    }
    mFlowRate = newFlowRate;
}
