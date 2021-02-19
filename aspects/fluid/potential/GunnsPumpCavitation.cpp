/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Class for the GUNNS Pump Cavitation model, used to model cavitation effects in liquid pumps.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((common/sensors/TsNoise.o))

PROGRAMMERS:
   ((Jason Harvey) (L-3 Communications) (Install) (2012-07))
***************************************************************************************************/

#include "GunnsPumpCavitation.hh"
#include "common/sensors/TsNoise.hh"
#include "core/GunnsFluidUtils.hh"
#include "core/GunnsBasicNode.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this Pump Cavitation object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsPumpCavitation::GunnsPumpCavitation()
    :
    mMalfFullCavitationFlag(false),
    mMalfFullCavitationDuration(0.0),
    mMalfPartialCavitationFlag(false),
    mMalfPartialCavitationDuration(0.0),
    mMalfPartialCavitationAmplitude(0.0),
    mAutoCavitationDuration(0.0),
    mAutoCavitationAmplitude(0.0),
    mEnableAutoCavitation(false),
    mCavitationElapsedTime(0.0),
    mInletVaporPressure(0.0),
    mDuration(0.0),
    mCavitationFraction(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Pump Cavitation object.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsPumpCavitation::~GunnsPumpCavitation()
{
    // nothing to be done in this deconstructor
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] autoCavitationDuration  (s)  Duration of auto-cavitation effect.
/// @param[in] autoCavitationAmplitude (--) Amplitude of auto-cavitation noise.
/// @param[in] enableAutoCavitation    (--) Enables the automatic cavitation effect.
///
/// @returns  void
///
/// @details  Initializes this Pump Cavitation object.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsPumpCavitation::initialize(const double autoCavitationDuration,
                                     const double autoCavitationAmplitude,
                                     const bool   enableAutoCavitation)
{
    /// - Initialize from arguments.
    mAutoCavitationDuration         = autoCavitationDuration;
    mAutoCavitationAmplitude        = autoCavitationAmplitude;
    mEnableAutoCavitation           = enableAutoCavitation;

    /// - Initialize remaining state data.
    mMalfFullCavitationFlag         = false;
    mMalfFullCavitationDuration     = 0.0;
    mMalfPartialCavitationFlag      = false;
    mMalfPartialCavitationDuration  = 0.0;
    mMalfPartialCavitationAmplitude = 0.0;
    mCavitationElapsedTime          = 0.0;
    mInletVaporPressure             = 0.0;
    mDuration                       = 0.0;
    mCavitationFraction             = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] pumpSource (--) Reference to the pump flow source parameter to apply cavitation.
/// @param[in]     dt         (s)  Time step.
/// @param[in]     inletNode  (--) Pointer to the node at the pump flow inlet.
/// @param[in]     liquidType (--) Liquid fluid type to use for cavitation check.
///
/// @returns  void
///
/// @details  Calculates the effect of cavitation on the pump source parameter.  Models two types of
///           cavitation: partial and full.  Full cavitation is triggered by malfunction or low
///           inlet pressure (auto-cavitation).  Partial cavitation is triggered by malfunction and
///           also occurs during full cavitation.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsPumpCavitation::update(double&                           pumpSource,
                                 const double                      dt,
                                 GunnsBasicNode*                   inletNode,
                                 const FluidProperties::FluidType& liquidType)
{
    /// - First update the inlet vapor pressure.
    computeVaporPressure(liquidType, inletNode);

    /// - Determine what type of cavitation is occurring and its duration depending on whether it is
    ///   a malf or automatic.  The duration keeps the last type that occurred.
    mCavitationElapsedTime += dt;
    bool   fullCavitation   = false;
    double partAmplitude    = 0.0;
    if (mMalfFullCavitationFlag) {
        fullCavitation = true;
        partAmplitude  = mMalfPartialCavitationAmplitude;
        mDuration      = mMalfFullCavitationDuration;

    } else if (mEnableAutoCavitation and
               inletNode->getContent()->getPressure() < mInletVaporPressure) {
        fullCavitation = true;
        partAmplitude  = mAutoCavitationAmplitude;
        mDuration      = mAutoCavitationDuration;

    } else if (mMalfPartialCavitationFlag) {
        partAmplitude  = mMalfPartialCavitationAmplitude;
        mDuration      = mMalfPartialCavitationDuration;

    } else {
        mCavitationElapsedTime = 0.0;
    }

    /// - Cavitation Fraction represents the fraction of pump source lost due to gas bubbles trapped
    ///   at the pump inlet.  Ramp this up & down to simulate accumulation or dissipation of the
    ///   bubbles over the desired duration.
    mDuration = std::max(mDuration, DBL_EPSILON);
    if (fullCavitation) {
        mCavitationFraction += 2.0 * sqrt(std::max(mCavitationFraction, 0.01)) * dt/mDuration;
    } else {
        mCavitationFraction -= 2.0 * sqrt(1.0 - std::min(mCavitationFraction, 0.99)) * dt/mDuration;
    }
    mCavitationFraction = MsMath::limitRange(0.0, mCavitationFraction, 1.0);
    pumpSource *= 1.0 - mCavitationFraction;

    /// - Partial cavitation simulates gas bubbles arriving at the inlet creating turbulence and
    ///   vibrations in the pump, modeled by adding random noise to the pump source.  It washes out
    ///   over time to simulate the pump gradually clearing the bubbles downstream.  We skip
    ///   processing the random noise generator if the total noise contribution is zero anyway, and
    ///   we don't let noise create negative pump source.
    double noiseFactor = 0.0;
    if (fullCavitation or mMalfPartialCavitationFlag) {
        noiseFactor = partAmplitude * std::max(0.0, 1.0 - mCavitationElapsedTime / mDuration);
    }
    if (noiseFactor > 0.0) {
        pumpSource = std::max(0.0, pumpSource + noiseFactor * TsNoise::getNoise());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] inletNode  (--) Pointer to the node at the pump flow inlet.
/// @param[in] liquidType (--) Liquid fluid type to use for cavitation check.
///
/// @returns  void
///
/// @details  Calculate the vapor pressure at the pump inlet based on liquid type and inlet
///           temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsPumpCavitation::computeVaporPressure(const FluidProperties::FluidType& liquidType,
                                               GunnsBasicNode*                   inletNode)
{
    mInletVaporPressure = inletNode->getFluidConfig()->mProperties->getProperties(liquidType)->getSaturationPressure(
                          inletNode->getContent()->getTemperature());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag     (--) Malfunction activation flag, true activates
/// @param[in] duration (s)  Duration of full cavitation malf effect
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsPumpCavitation::setMalfFullCavitation(const bool flag, const double duration)
{
    mMalfFullCavitationFlag     = flag;
    mMalfFullCavitationDuration = duration;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag      (--) Malfunction activation flag, true activates
/// @param[in] duration  (s)  Duration of partial cavitation wash-out
/// @param[in] amplitude (--) Amplitude of partial cavitation noise
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsPumpCavitation::setMalfPartialCavitation(const bool   flag,
                                                   const double duration,
                                                   const double amplitude)
{
    mMalfPartialCavitationFlag      = flag;
    mMalfPartialCavitationDuration  = duration;
    mMalfPartialCavitationAmplitude = amplitude;
}
