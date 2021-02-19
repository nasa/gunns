/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Three Way Check Valve.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
    ()

PROGRAMMERS:
   ((Chris Brownschidle) (CACI) (2017-09) (Initial version))
**************************************************************************************************/

#include "GunnsFluid3WayCheckValve.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                   (--)  Link name.
/// @param[in] nodes                  (--)  Network nodes array.
/// @param[in] positionPathB          (--)  Valve position is relative to flow path B (false = A).
/// @param[in] maxConductivityA;      (m2)  Path A maximum flow conductivity.
/// @param[in] maxConductivityB;      (m2)  Path B maximum flow conductivity.
/// @param[in] expansionScaleFactorA; (--)  Path A thermal expansion scale factor.
/// @param[in] expansionScaleFactorB; (--)  Path B thermal expansion scale factor.
/// @param[in] thermalLengthA;        (m)   Path A thermal convection length.
/// @param[in] thermalLengthB;        (m)   Path B thermal convection length.
/// @param[in] thermalDiameterA;      (m)   Path A thermal convection diameter.
/// @param[in] thermalDiameterB;      (m)   Path B thermal convection diameter.
/// @param[in] surfaceRoughnessA;     (m)   Path A thermal surface roughness.
/// @param[in] rateLimit              (1/s) Valve position rate limit.
/// @param[in] openPressureA          (kPa) Delta pressure at which path A is fully opened.
/// @param[in] openPressureB          (kPa) Delta pressure at which path B is fully opened.
/// @param[in] minPressureDiff        (kPa) Delta pressure range at which valve is half opened.
///
/// @details  Default GUNNS Three Way Check Valve Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WayCheckValveConfigData::GunnsFluid3WayCheckValveConfigData(const std::string& name,
                                                             GunnsNodeList*     nodes,
                                                             const bool         positionPathB,
                                                             const double       maxConductivityA,
                                                             const double       maxConductivityB,
                                                             const double       expansionScaleFactorA,
                                                             const double       expansionScaleFactorB,
                                                             const double       thermalLengthA,
                                                             const double       thermalLengthB,
                                                             const double       thermalDiameterA,
                                                             const double       thermalDiameterB,
                                                             const double       surfaceRoughnessA,
                                                             const double       surfaceRoughnessB,
                                                             const double       rateLimit,
                                                             const double       openPresureA,
                                                             const double       openPresureB,
                                                             const double       minPressureDiff)
    :
    GunnsFluid3WayValveConfigData(name, nodes, positionPathB, maxConductivityA, maxConductivityB,
                                  expansionScaleFactorA, expansionScaleFactorB, thermalLengthA,
                                  thermalLengthB, thermalDiameterA, thermalDiameterB,
                                  surfaceRoughnessA, surfaceRoughnessB),
    mRateLimit                   (rateLimit),
    mOpenPressureA               (openPresureA),
    mOpenPressureB               (openPresureB),
    mMinPressureDiff             (minPressureDiff)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy.
///
/// @details  Copy constructs this Gunns Three Way Check Valve configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WayCheckValveConfigData::GunnsFluid3WayCheckValveConfigData(
        const GunnsFluid3WayCheckValveConfigData& that)
    :
    GunnsFluid3WayValveConfigData(that),
    mRateLimit                   (that.mRateLimit),
    mOpenPressureA               (that.mOpenPressureA),
    mOpenPressureB               (that.mOpenPressureB),
    mMinPressureDiff             (that.mMinPressureDiff)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Three Way Check Valve Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WayCheckValveConfigData::~GunnsFluid3WayCheckValveConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] position         (--) (0-1) Valve position relative to config data path.
/// @param[in] wallTemperatureA (K)  Path A wall temperature for thermal convection.
/// @param[in] wallTemperatureB (K)  Path B wall temperature for thermal convection.
/// @param[in] malfStuckFlag    (--) Malfunction flag indicating valve is stuck at current position.
/// @param[in] malfFailToFlag   (--) Malfunction flag indicating valve will fail to malfFailToValue.
/// @param[in] malfFailToValue  (--) Value to fail position to.
///
/// @details  Default GUNNS Three Way Check Valve Input Data Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WayCheckValveInputData::GunnsFluid3WayCheckValveInputData(const double position,
                                                                     const double wallTemperatureA,
                                                                     const double wallTemperatureB,
                                                                     const bool   malfStuckFlag,
                                                                     const bool   malfFailToFlag,
                                                                     const double malfFailToValue)
    :
    GunnsFluid3WayValveInputData(position, wallTemperatureA, wallTemperatureB),
    mMalfStuckFlag              (malfStuckFlag),
    mMalfFailToFlag             (malfFailToFlag),
    mMalfFailToValue            (malfFailToValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy.
///
/// @details  Copy constructs this Three Way Check Valve input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WayCheckValveInputData::GunnsFluid3WayCheckValveInputData(const GunnsFluid3WayCheckValveInputData& that)
    :
    GunnsFluid3WayValveInputData(that),
    mMalfStuckFlag              (that.mMalfStuckFlag),
    mMalfFailToFlag             (that.mMalfFailToFlag),
    mMalfFailToValue            (that.mMalfFailToValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Three Way Check Valve Input Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WayCheckValveInputData::~GunnsFluid3WayCheckValveInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Gunns Three Way Check Valve Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WayCheckValve::GunnsFluid3WayCheckValve()
    :
    GunnsFluid3WayValve(),
    mMalfStuckFlag(false),
    mMalfFailToFlag(false),
    mMalfFailToValue(0.0),
    mRateLimit(0.0),
    mOpenPressureA(0.0),
    mOpenPressureB(0.0),
    mMinPressureDiff(0.0)
{
    // - nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Three Way Check Valve
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WayCheckValve::~GunnsFluid3WayCheckValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) GUNNS Three Way Check Valve Config Data
/// @param[in]     inputData    (--) GUNNS Three Way Check Valve Input Data
/// @param[in,out] networkLinks (--) Network links
/// @param[in]     port0        (--) Network port 0
/// @param[in]     port1        (--) Network port 1
/// @param[in]     port2        (--) Network port 2
///
/// @throws   TsInitializationException
///
/// @details  This method initializes this Three Way Check Valve with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WayCheckValve::initialize(const GunnsFluid3WayCheckValveConfigData& configData,
                                     const GunnsFluid3WayCheckValveInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        networkLinks,
                                     const int                            port0,
                                     const int                            port1,
                                     const int                            port2)
{
    /// - Initialize the parent class.
    GunnsFluid3WayValve::initialize(configData, inputData, networkLinks, port0, port1, port2);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize with configuration data.
    mRateLimit       = configData.mRateLimit;
    mOpenPressureA   = configData.mOpenPressureA;
    mOpenPressureB   = configData.mOpenPressureB;
    mMinPressureDiff = configData.mMinPressureDiff;

    /// - Initialize with input data.
    mMalfStuckFlag   = inputData.mMalfStuckFlag;
    mMalfFailToFlag  = inputData.mMalfFailToFlag;
    mMalfFailToValue = inputData.mMalfFailToValue;

    /// - Validate and set init flag on successful validation.
    validate();
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Three Way Check Valve initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WayCheckValve::validate() const
{
    /// - Throw an exception if rate limit < 0.
    if (mRateLimit < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Rate limit < 0.");
    }

    /// - Throw an exception if open pressure less than or equal to min pressure.
    if ((mOpenPressureA - mMinPressureDiff) <= DBL_EPSILON or (mOpenPressureB - mMinPressureDiff) <= DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Open pressure < min pressure difference.");
    }

    /// - Throw an exception if fail to position malfunction value is out of range 0 to 1.
    if (!MsMath::isInRange(0.0, mMalfFailToValue, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Fail to position position malfunction value out of range 0 to 1.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WayCheckValve::restartModel()
{
    /// - Reset the base class.
    GunnsFluid3WayValve::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (not used)
///
/// @details  Updates the state of the Three Way Check Valve
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WayCheckValve::updateState(const double dt)
{
    /// - Calculate valve position
    /// - Handle stuck malfunction
    if (!mMalfStuckFlag) {
        if (mMalfFailToFlag) {
            /// - Handle fail to position malfunction with range limiting.
            mPosition = MsMath::limitRange(0.0, mMalfFailToValue, 1.0);
        } else{
            const double previousPosition = mPosition;
            mPosition = calculateValvePosition();

            /// - Apply range and rate limiting to the computed position.
            const double maxDelta = mRateLimit * dt;
            mPosition = MsMath::limitRange(std::max(0.0, previousPosition - maxDelta),
                                         mPosition,
                                         std::min(1.0, previousPosition + maxDelta));
        }
    }

    /// - Update base class
    GunnsFluid3WayValve::updateState(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns    double (--) Calculated valve position.
///
/// @details    Calculates valve position based on potential drop across path A and B.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluid3WayCheckValve::calculateValvePosition()
{
    double pressureA = mPathA.getPotentialDrop(), pressureB = mPathB.getPotentialDrop();
    double position = mPosition;

    /// - Calculate valve position based on pressure differential across path A and B
    if(pressureA < 0.0 and pressureB < 0.0){
        /// - If both pressure drops are negative (backflow), maintain current position
        return mPosition;
    } else if(std::abs(pressureA - pressureB) < mMinPressureDiff){
        /// - If the pressure differential is within +- mMinPressureDiff, the valve is stuck in the
        ///   halfway position.
        position = 0.5;
    } else if(pressureA > pressureB){
        /// - Greater potential drop across A, position greater than 0.5
        position = MsMath::limitRange(0.5,
                                    0.5 * (1 + (pressureA - pressureB - mMinPressureDiff)
                                             / (mOpenPressureA - mMinPressureDiff)),
                                    1.0);
    } else if(pressureB > pressureA){
        /// - Greater potential drop across B, position less than 0.5
        position = MsMath::limitRange(0.0,
                                    0.5 * (1 - (pressureB - pressureA - mMinPressureDiff)
                                             / (mOpenPressureA - mMinPressureDiff)),
                                    0.5);
    }

    /// - If Path B corresponds to the valve being "open" (position = 1), this logic needs to be
    ///   reversed
    if(mPositionPathB){
        position = 1.0 - position;
    }

    return position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
///
/// @details  Sets the stuck malf flag to given the value.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WayCheckValve::setMalfStuck(const bool flag)
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
void GunnsFluid3WayCheckValve::setMalfFailTo(const bool flag, const double value)
{
    mMalfFailToFlag  = flag;
    mMalfFailToValue = value;
}
