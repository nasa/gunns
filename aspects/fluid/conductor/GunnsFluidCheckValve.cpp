/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((aspects/fluid/conductor/GunnsFluidValve.o))
***************************************************************************************************/

#include "math/MsMath.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <algorithm>
#include "GunnsFluidCheckValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      name                 (--)   Name of object.
/// @param[in]      nodes                (--)   Pointer to nodes.
/// @param[in]      maxConductivity      (m2)   Max conductivity.
/// @param[in]      expansionScaleFactor (--)   Scale factor for isentropic gas cooling.
/// @param[in]      thermalLength        (m)    Tube length for thermal convection
/// @param[in]      thermalDiameter      (m)    Tube inner diameter for thermal convection
/// @param[in]      surfaceRoughness     (m)    Tube wall surface roughness for thermal convection
/// @param[in]      rateLimit            (1/s)  Valve position rate limit.
/// @param[in]      closePressure        (kPa)  Delta pressure at which valve is fully closed.
/// @param[in]      openPressure         (kPa)  Delta pressure at which valve is fully opened.
///
/// @details        Default constructs this GUNNS Fluid Check Valve Link Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCheckValveConfigData::GunnsFluidCheckValveConfigData(const std::string& name,
                                                               GunnsNodeList*     nodes,
                                                               const double       maxConductivity,
                                                               const double       expansionScaleFactor,
                                                               const double       thermalLength,
                                                               const double       thermalDiameter,
                                                               const double       surfaceRoughness,
                                                               const double       rateLimit,
                                                               const double       closePressure,
                                                               const double       openPressure)
    :
    GunnsFluidValveConfigData(name,
                             nodes,
                             maxConductivity,
                             expansionScaleFactor,
                             thermalLength,
                             thermalDiameter,
                             surfaceRoughness),
    mRateLimit(rateLimit),
    mClosePressure(closePressure),
    mOpenPressure(openPressure)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that   (--)  Source to copy.
///
/// @details        Copy constructs this GUNNS Fluid Check Valve Link Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCheckValveConfigData::GunnsFluidCheckValveConfigData(const GunnsFluidCheckValveConfigData& that)
    :
    GunnsFluidValveConfigData(that),
    mRateLimit(that.mRateLimit),
    mClosePressure(that.mClosePressure),
    mOpenPressure(that.mOpenPressure)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details        Default destructs this GUNNS Fluid Check Valve Link Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCheckValveConfigData::~GunnsFluidCheckValveConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      malfBlockageFlag   (--)    Blockage malfunction flag.
/// @param[in]      malfBlockageValue  (--)    Blockage malfunction fractional value (0-1).
/// @param[in]      position           (--)    Fractional position.
/// @param[in]      malfLeakThruFlag   (--)    Leak rate malfunction flag.
/// @param[in]      malfLeakThruValue  (kg/s)  Leak rate malfunction value.
/// @param[in]      wallTemperature    (K)     Tube wall temperature for thermal convection
/// @param[in]      malfStuckFlag      (--)    Stuck at current position malfunction flag.
/// @param[in]      malfFailToFlag     (--)    Fail to position malfunction flag.
/// @param[in]      malfFailToValue    (--)    Fail to position malfunction value
////
/// @details        Default constructs this GUNNS Fluid Check Valve Link Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCheckValveInputData::GunnsFluidCheckValveInputData(const bool   malfBlockageFlag,
                                                             const double malfBlockageValue,
                                                             const double position,
                                                             const bool   malfLeakThruFlag,
                                                             const double malfLeakThruValue,
                                                             const double wallTemperature,
                                                             const bool   malfStuckFlag,
                                                             const bool   malfFailToFlag,
                                                             const double malfFailToValue)
    :
    GunnsFluidValveInputData(malfBlockageFlag,
                             malfBlockageValue,
                             position,
                             malfLeakThruFlag,
                             malfLeakThruValue,
                             wallTemperature),
    mMalfStuckFlag(malfStuckFlag),
    mMalfFailToFlag(malfFailToFlag),
    mMalfFailToValue(malfFailToValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that   (--) Source to copy.
///
/// @details        Copy constructs this GUNNS Fluid Check Valve Link Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCheckValveInputData::GunnsFluidCheckValveInputData(const GunnsFluidCheckValveInputData& that)
    :
    GunnsFluidValveInputData(that),
    mMalfStuckFlag(that.mMalfStuckFlag),
    mMalfFailToFlag(that.mMalfFailToFlag),
    mMalfFailToValue(that.mMalfFailToValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details        Default destructs this GUNNS Fluid Check Valve Link Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCheckValveInputData::~GunnsFluidCheckValveInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note           This should be followed by a call to the initialize method before calling an
///                 update method.
///
/// @details        Default constructs this GUNNS Fluid Check Valve Link Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCheckValve::GunnsFluidCheckValve()
    :
    GunnsFluidValve(),
    mMalfStuckFlag(false),
    mMalfFailToFlag(false),
    mMalfFailToValue(0.0),
    mRateLimit(0.0),
    mClosePressure(0.0),
    mOpenPressure(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details        Default destructs this GUNNS Fluid Check Valve Link Model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCheckValve::~GunnsFluidCheckValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
/// @param[in,out]  links        (--)  Link vector.
/// @param[in]      port0        (--)  Nominal inlet port map index.
/// @param[in]      port1        (--)  Nominal outlet port map index.
///
/// @return         void
///
/// @throws         TsInitializationException
///
/// @details        Initializes this GGUNNS Fluid Check Valve Link Model with configuration and
///                 input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCheckValve::initialize(const GunnsFluidCheckValveConfigData&  configData,
                                      const GunnsFluidCheckValveInputData&   inputData,
                                      std::vector<GunnsBasicLink*>&          links,
                                      const int                              port0,
                                      const int                              port1)
{
    /// - First initialize & validate parent.
    GunnsFluidValve::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag          = false;

    /// - Initialize with configuration data.
    mRateLimit         = configData.mRateLimit;
    mOpenPressure      = configData.mOpenPressure;
    mClosePressure     = configData.mClosePressure;

    /// - Initialize with input data.
    mMalfStuckFlag     = inputData.mMalfStuckFlag;
    mMalfFailToFlag    = inputData.mMalfFailToFlag;
    mMalfFailToValue   = inputData.mMalfFailToValue;

    /// - Validate configuration and input data.
    validate();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag          = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return         void
///
/// @throws         TsInitializationException
///
/// @details        Validates this GUNNS Fluid Check Valve Link Model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCheckValve::validate() const
{
    /// - Throw an exception if rate limit < 0.
    if (mRateLimit < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Rate limit < 0.");
    }

    /// - Throw an exception if open pressure less than close pressure.
    if (mOpenPressure < mClosePressure) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Open pressure < close pressure.");
    }

    /// - Throw an exception if fail to position malfunction value is out of range 0 to 1.
    if (!MsMath::isInRange(0.0, mMalfFailToValue, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Fail to position position malfunction value out of range 0 to 1.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCheckValve::restartModel()
{
    /// - Reset the base class.
    GunnsFluidValve::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Time step.
///
/// @return         void
///
/// @details        Updates this GUNNS Fluid Check Valve Link Model state (valve position and
///                 effective conductivity).
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCheckValve::updateState(const double dt)
{
    /// - Handle stuck malfunction
    if (!mMalfStuckFlag) {
        if (mMalfFailToFlag) {
            /// - Handle fail to position malfunction with range limiting.
            mPosition                     = MsMath::limitRange(0.0, mMalfFailToValue, 1.0);
        } else {
            const double previousPosition = mPosition;
            if (mPotentialDrop >= mOpenPressure) {
                /// - The position is fully open (1.0) if delta P across the valve is large enough.
                mPosition                 = 1.0;
            } else if (mPotentialDrop <= mClosePressure) {
                /// - The position is fully closed (0.0) if delta P across the valve is small enough.
                mPosition                 = 0.0;
            } else {
                /// - Otherwise the position transitions (0.0 to 1.0) linearly in delta P.
                mPosition                 = (mPotentialDrop - mClosePressure) / (mOpenPressure - mClosePressure);
            }

            /// - Apply range and rate limiting to the computed position.
            const double maxDelta         = mRateLimit * dt;
            mPosition                     = MsMath::limitRange(std::max(0.0, previousPosition - maxDelta),
                                                             mPosition,
                                                             std::min(1.0, previousPosition + maxDelta));
        }
    }
    /// - Call parent updateState to apply valve malfunctions and update the effective conductivity.
    GunnsFluidValve::updateState(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
///
/// @details  Sets the stuck malf flag to given the value.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCheckValve::setMalfStuck(const bool flag)
{
    mMalfStuckFlag = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
/// @param[in] value (--) Fail to position malfunction value
///
/// @details  Sets the fail to position malf parameters to given the values.  Calling this method
///           with default arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCheckValve::setMalfFailTo(const bool flag, const double value)
{
    mMalfFailToFlag  = flag;
    mMalfFailToValue = value;
}
