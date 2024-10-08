/*
@file      GunnsFluidRegulatorValve.cpp
@brief     Pressure Regulator Valve Model implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((GunnsFluidPressureSensitiveValve.o))
*/

#include <algorithm>

#include "math/MsMath.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "GunnsFluidRegulatorValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      name                  (--)    Name of object.
/// @param[in]      nodes                 (--)    Pointer to nodes.
/// @param[in]      maxConductivity       (m2)    Max conductivity.
/// @param[in]      expansionScaleFactor  (--)    Scale factor for isentropic gas cooling.
/// @param[in]      rateLimit             (1/s)   Valve position rate limit.
/// @param[in]      thermalLength         (m)     Tube length for thermal convection.
/// @param[in]      thermalDiameter       (m)     Tube inner diameter for thermal convection.
/// @param[in]      surfaceRoughness      (m)     Tube wall surface roughness for thermal convection.
/// @param[in]      reseatPressure        (kPa)   Valve reseat pressure.
/// @param[in]      crackPressure         (kPa)   Valve crack pressure.
/// @param[in]      fullOpenPressure      (kPa)   Valve full open pressure.
/// @param[in]      popPosition           (--)    Valve pop position.
/// @param[in]      popSlopeScale         (kPa)   Valve pop slope scale factor.
/// @param[in]      inletDependencyCoeff0 (--)    0th order coefficient of inlet dependency effect on control pressure.
/// @param[in]      inletDependencyCoeff1 (1/kPa) 1st order coefficient of inlet dependency effect on control pressure.
//
/// @details      Default constructs this GUNNS Fluid Pressure Regulator Valve Link Model
///               configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidRegulatorValveConfigData::GunnsFluidRegulatorValveConfigData(const std::string& name,
                                                                       GunnsNodeList*     nodes,
                                                                       const double       maxConductivity,
                                                                       const double       expansionScaleFactor,
                                                                       const double       rateLimit,
                                                                       const double       thermalLength,
                                                                       const double       thermalDiameter,
                                                                       const double       surfaceRoughness,
                                                                       const double       reseatPressure,
                                                                       const double       crackPressure,
                                                                       const double       fullOpenPressure,
                                                                       const double       popPosition,
                                                                       const double       popSlopeScale,
                                                                       const double       inletDependencyCoeff0,
                                                                       const double       inletDependencyCoeff1)
    :
    GunnsFluidPressureSensitiveValveConfigData(name,
                                               nodes,
                                               maxConductivity,
                                               expansionScaleFactor,
                                               rateLimit,
                                               thermalLength,
                                               thermalDiameter,
                                               surfaceRoughness,
                                               inletDependencyCoeff0,
                                               inletDependencyCoeff1),
    mReseatPressure(reseatPressure),
    mCrackPressure(crackPressure),
    mFullOpenPressure(fullOpenPressure),
    mPopPosition(popPosition),
    mPopSlopeScale(popSlopeScale)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that  (--)  Source to copy.
///
/// @details        Copy constructs this GUNNS Fluid Pressure Regulator Valve Link Model
///                 configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidRegulatorValveConfigData::GunnsFluidRegulatorValveConfigData(const GunnsFluidRegulatorValveConfigData& that)
    :
    GunnsFluidPressureSensitiveValveConfigData(that),
    mReseatPressure(that.mReseatPressure),
    mCrackPressure(that.mCrackPressure),
    mFullOpenPressure(that.mFullOpenPressure),
    mPopPosition(that.mPopPosition),
    mPopSlopeScale(that.mPopSlopeScale)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details        Default destructs this GUNNS Fluid Pressure Regulator Valve Link Model
///                 configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidRegulatorValveConfigData::~GunnsFluidRegulatorValveConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      malfBlockageFlag        (--)    Blockage malfunction flag.
/// @param[in]      malfBlockageValue       (--)    Blockage malfunction fractional value (0-1).
/// @param[in]      position                (--)    Fractional position.
/// @param[in]      malfLeakThruFlag        (--)    Leak through rate malfunction flag.
/// @param[in]      malfLeakThruValue       (kg/s)  Leak through rate malfunction value.
/// @param[in]      malfPressureBiasFlag    (--)    Control pressure bias malfunction flag.
/// @param[in]      malfPressureBiasValue   (kPa)   Control pressure bias malfunction value.
/// @param[in]      setPointPressureBias    (kPa)   Set point pressure bias value.
/// @param[in]      wallTemperature         (K)     Tube wall temperature for thermal convection
/// @param[in]      malfValveStuckFlag      (--)    Stuck valve malfunction flag.
/// @param[in]      malfValveConstantFlag   (--)    Constant valve position malfunction flag.
/// @param[in]      malfValveConstantValue  (--)    Constant valve position malfunction value.
///
/// @details        Default constructs this GUNNS Fluid Pressure Regulator Valve Link Model input
///                 data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidRegulatorValveInputData::GunnsFluidRegulatorValveInputData(const bool   malfBlockageFlag,
                                                                     const double malfBlockageValue,
                                                                     const double position,
                                                                     const bool   malfLeakThruFlag,
                                                                     const double malfLeakThruValue,
                                                                     const bool   malfPressureBiasFlag,
                                                                     const double malfPressureBiasValue,
                                                                     const double setPointPressureBias,
                                                                     const double wallTemperature,
                                                                     const bool   malfValveStuckFlag,
                                                                     const bool   malfValveConstantFlag,
                                                                     const double malfValveConstantValue)
    :
    GunnsFluidPressureSensitiveValveInputData(malfBlockageFlag,
                                              malfBlockageValue,
                                              position,
                                              malfLeakThruFlag,
                                              malfLeakThruValue,
                                              malfPressureBiasFlag,
                                              malfPressureBiasValue,
                                              setPointPressureBias,
                                              wallTemperature,
                                              malfValveStuckFlag,
                                              malfValveConstantFlag,
                                              malfValveConstantValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that   (--)  Source to copy.
///
/// @details        Copy constructs this GUNNS Fluid Pressure Regulator Valve Link Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidRegulatorValveInputData::GunnsFluidRegulatorValveInputData(const GunnsFluidRegulatorValveInputData& that)
    :
    GunnsFluidPressureSensitiveValveInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details        Default destructs this GUNNS Fluid Pressure Regulator Valve link model input
///                 data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidRegulatorValveInputData::~GunnsFluidRegulatorValveInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note           This should be followed by a call to the initialize method before calling an
///                 update method.
///
/// @details        Default constructs this GUNNS Fluid Pressure Regulator Valve Link Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidRegulatorValve::GunnsFluidRegulatorValve()
    :
    GunnsFluidPressureSensitiveValve(),
    mReseatPressure(0.0),
    mCrackPressure(0.0),
    mFullOpenPressure(0.0),
    mPopPosition(0.0),
    mPopSlopeScale(0.0),
    mPopIncM(0.0),
    mPopIncMInv(0.0),
    mPopIncB(0.0),
    mPopIncP(0.0),
    mPopDecM(0.0),
    mPopDecMInv(0.0),
    mPopDecB(0.0),
    mPopDecP(0.0),
    mNomIncM(0.0),
    mNomIncMInv(0.0),
    mNomIncB(0.0),
    mNomDecM(0.0),
    mNomDecMInv(0.0),
    mNomDecB(0.0),
    mPreviousPressure(0.0),
    mState(GunnsFluidValve::OPENING)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details        Default destructs this GUNNS Fluid Pressure Regulator Valve Link Model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidRegulatorValve::~GunnsFluidRegulatorValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData     (--)  Configuration data.
/// @param[in]      inputData      (--)  Input data.
/// @param[in,out]  links          (--)  Link vector.
/// @param[in]      port0          (--)  Nominal inlet port map index.
/// @param[in]      port1          (--)  Nominal outlet port map index.
/// @param[in]      pressurePort0  (--)  Network pressure port 0.
/// @param[in]      pressurePort1  (--)  Network pressure port 1.
///
/// @return         void
///
/// @throws         TsInitializationException
///
/// @details        Initializes this GUNNS Fluid Pressure Regulator Valve Link Model with
///                 configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidRegulatorValve::initialize(const GunnsFluidRegulatorValveConfigData& configData,
                                          const GunnsFluidRegulatorValveInputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             links,
                                          const int                                 port0,
                                          const int                                 port1,
                                          const int                                 pressurePort0,
                                          const int                                 pressurePort1)
{
    /// - First initialize & validate parent.
    GunnsFluidPressureSensitiveValve::initialize(configData,
                                                 inputData,
                                                 links,
                                                 port0,
                                                 port1,
                                                 pressurePort0,
                                                 pressurePort1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Initialize with configuration data.
    mReseatPressure   = configData.mReseatPressure;
    mCrackPressure    = configData.mCrackPressure;
    mFullOpenPressure = configData.mFullOpenPressure;
    mPopPosition      = configData.mPopPosition;
    mPopSlopeScale    = configData.mPopSlopeScale;

    /// - Validate configuration and input data.
    validate();

    /// - Initialize derived attributes from configuration and input data.
    derive();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag         = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return         void
///
/// @throws         TsInitializationException
///
/// @details        Validates this GUNNS Fluid Pressure Regulator Valve Link Model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidRegulatorValve::validate() const
{
    /// - Throw an exception if full open pressure < FLT_EPSILON.
    if (mFullOpenPressure < static_cast<double>(FLT_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Full open pressure < FLT_EPSILON.");
    }

    /// - Throw an exception if crack pressure <= full open pressure.
    if (-static_cast<double>(FLT_EPSILON) < mFullOpenPressure - mCrackPressure) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Crack pressure <= full open pressure.");
    }

    /// - Throw an exception if reseat pressure <= crack pressure.
    if (-static_cast<double>(FLT_EPSILON) < mCrackPressure - mReseatPressure) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Reseat pressure <= crack pressure.");
    }

    /// - Throw an exception if pop position <= 0 or pop position >= 1.
    if (!MsMath::isInRange(static_cast<double>(FLT_EPSILON), mPopPosition, 1.0 - static_cast<double>(FLT_EPSILON))) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Pop position outside valid range (0-1).");
    }

    /// - Throw an exception if pop slope scale factor < 1.
    if (mPopSlopeScale < 1.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Pop slope scale factor  < 1.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidRegulatorValve::restartModel()
{
    /// - Reset the base class.
    GunnsFluidPressureSensitiveValve::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return         void
///
/// @details        Computes the coefficients for the four linear equations for this GUNNS Fluid
///                 Pressure Regulator Valve Link Model and sets the initial valve state.
///                 -# The starting point are the two linear equations for valve position as a
///                    function of outlet pressure that run from crack pressure to full open pressure
///                    for decreasing outlet pressure and from full open pressure to reseat pressure
///                    for increasing outlet pressure.
///                 -# For the initial pop segments between 0 and mInitialPop, the slope is scaled
///                    up by mPopSlopeScale and for the nominal segment between mInitialPop and 1
///                    the slope is correspondingly scaled down.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidRegulatorValve::derive()
{
    // Note that validation guarantees
    //    0 < mFullOpenPressure < mCrackPressure < mReseatPressure
    //    0 < mPopPosition < 1
    //    mPopSlopeScale >= 1.
    // Consequently
    //    mPopDecM < 0
    //    mPopDecP > mFullOpenPressure
    //    mNomDecM < 0
    // and
    //    mPopIncM < 0
    //    mPopIncP > mFullOpenPressure
    //    mNomIncM < 0
    // This precludes division by zero in the following computations.

    /// - Compute slope and intercept for decreasing pressure from crack to pop.

    const double mDec = 1.0 / (mFullOpenPressure - mCrackPressure);
    mPopDecM          = mDec * mPopSlopeScale;
    mPopDecMInv       = 1.0 / mPopDecM;
    mPopDecB          = -mPopDecM * mCrackPressure;
    mPopDecP          = (mPopPosition - mPopDecB) * mPopDecMInv;

    /// - Compute slope and intercept for decreasing pressure from pop to fully open.
    mNomDecM          = (1.0 - mPopPosition) / (mFullOpenPressure - mPopDecP);
    mNomDecMInv       = 1.0 / mNomDecM;
    mNomDecB          = 1.0 - mNomDecM * mFullOpenPressure;

    /// - Compute slope and intercept for increasing pressure from pop to reseat.
    const double mInc = 1.0 / (mFullOpenPressure - mReseatPressure);
    mPopIncM          = mInc * mPopSlopeScale;
    mPopIncMInv       = 1.0 / mPopIncM;
    mPopIncB          = -mPopIncM * mReseatPressure;
    mPopIncP          = (mPopPosition - mPopIncB) * mPopIncMInv;

    /// - Compute slope and intercept for increasing pressure from fully open to pop.
    mNomIncM          = (1.0 - mPopPosition) / (mFullOpenPressure - mPopIncP);
    mNomIncMInv       = 1.0 / mNomIncM;
    mNomIncB          = 1.0 - mNomIncM * mFullOpenPressure;

    /// - Set valve state consistent with valve position.
    if (mPosition <= 0.0) {
        mState = GunnsFluidValve::CLOSED;
    } else if (mPosition >= 1.0) {
        mState = GunnsFluidValve::OPEN;
    } else {
        mState = GunnsFluidValve::OPENING;
    }

    /// - Set previous pressure to initial control pressure.
    mPreviousPressure = mControlPressure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Time step.
///
/// @return         void
///
/// @details        Updates this GUNNS Fluid Pressure Regulator Valve Link Model state (valve state,
///                 valve position, and effective conductivity).
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidRegulatorValve::updateState(const double dt)
{
    /// - Perform state transitions.
    updateValveState();

    /// - Set the new valve position.
    if (!mMalfStuckFlag) {
        if (mMalfFailToFlag) {
            mPosition = MsMath::limitRange(0.0, mMalfFailToValue, 1.0);
        } else {
            updatePosition(dt);
        }
    }

    /// - Save current control pressure as previous for next time.
    mPreviousPressure = mControlPressure;

    /// - Call parent updateState to apply malfunctions and update the effective conductivity.
    GunnsFluidPressureSensitiveValve::updateState(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return         void
///
/// @details        Updates this GUNNS Fluid Pressure Regulator Valve Link Model valve state.
///                 \verbatim
///                  mControlPressure <                             mControlPressure >
///                  mFullOpenPressure           _______________    mFullOpenPressure
///                 __________________________\ |               | ___________________________
///                |                          / |     Open      |                            |
///                |                            |_______________|                            |
///                |                                                                        \|/
///            _________  ____________________\  _______________  /__________________    __________
///           |         | mControlPressure >  / |               | \ mControlPressure <  |          |
///           | Opening | mPreviousPressure     | Transitioning |   mPreviousPressure   | Closing  |
///           |_________| /____________________ |_______________| ____________________\ |__________|
///               /|\     \ mControlPressure <                    mControlPressure >  /      |
///                |        decPressure                           incPressure                |
///                |                             _______________                             |
///                |___________________________ |               | /__________________________|
///                  mControlPressure <         |    Closed     | \ mControlPressure >
///                  mCrackPressure             |_______________|   mReseatPressure
///                 \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidRegulatorValve::updateValveState()
{
    /// - Perform state transitions.
    if (GunnsFluidValve::OPEN == mState) {
        if (mControlPressure > mFullOpenPressure) {
            mState = GunnsFluidValve::CLOSING;
        }
    } else if (GunnsFluidValve::CLOSED == mState) {
        if (mControlPressure < mCrackPressure) {
            mState = GunnsFluidValve::OPENING;
        }
    } else if (GunnsFluidValve::OPENING == mState) {
        if (mControlPressure < mFullOpenPressure) {
            mState = GunnsFluidValve::OPEN;
        } else if (mControlPressure > mPreviousPressure){
            mState = GunnsFluidValve::TRANSITIONING;
        }
    } else if (GunnsFluidValve::CLOSING == mState) {
        if (mControlPressure > mReseatPressure) {
            mState = GunnsFluidValve::CLOSED;
        } else if (mControlPressure < mPreviousPressure){
            mState = GunnsFluidValve::TRANSITIONING;
        }
    }

    /// - Allow OPENING <-> TRANSITIONING <-> CLOSING in a single cycle.
    if (GunnsFluidValve::TRANSITIONING == mState) {
        double incPressure;
        double decPressure;
        if (mPosition <= mPopPosition) {
            incPressure = (mPosition - mPopIncB) * mPopIncMInv;
            decPressure = (mPosition - mPopDecB) * mPopDecMInv;
        } else {
            incPressure = (mPosition - mNomIncB) * mNomIncMInv;
            decPressure = (mPosition - mNomDecB) * mNomDecMInv;
        }
        if (mControlPressure > incPressure) {
            mState = GunnsFluidValve::CLOSING;
        } else if (mControlPressure < decPressure) {
            mState = GunnsFluidValve::OPENING;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)   Time step.
///
/// @return         void
///
/// @details        Updates this GUNNS Fluid Pressure Regulator Valve Link Model valve position.
///                 -# In CLOSED state, position is 0.0.
///                 -# In OPEN state, position is 1.0.
///                 -# In OPENING state, position is updated from appropriate increasing linear
///                    equation evaluated at control pressure.
///                 -# In CLOSING state, position is updated from appropriate decreasing linear
///                    equation evaluated at control pressure.
///                 -# In TRANSITIONING state the position is held at its previous value.
///                 -# For position < mPopPosition, the pop linear equations are used, otherwise
///                    the nominal linear equations are used.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidRegulatorValve::updatePosition(const double dt)
{
    /// - Update position based on state and pressure.
    double position;

    switch(mState) {
        case GunnsFluidValve::CLOSED:
            position      = 0.0;
            break;
        case GunnsFluidValve::OPEN:
            position      = 1.0;
            break;
        case GunnsFluidValve::OPENING:
            if (mControlPressure >= mPopDecP) {
                position  = mPopDecB + mPopDecM * mControlPressure;
            } else {
                position  = mNomDecB + mNomDecM * mControlPressure;
            }
            break;
        case GunnsFluidValve::CLOSING:
            if (mControlPressure >= mPopIncP) {
                position  = mPopIncB + mPopIncM * mControlPressure;
            } else {
                position  = mNomIncB + mNomIncM * mControlPressure;
            }
            break;
        case GunnsFluidValve::TRANSITIONING:
            position      = mPosition;
            break;
        default:
            position      = mPosition;
            break;
    }

    /// - Apply range and rate limiting to the computed position.
    const double maxDelta = mRateLimit * dt;
    mPosition             = MsMath::limitRange(std::max(0.0, mPosition - maxDelta),
                                             position,
                                             std::min(1.0, mPosition + maxDelta));

    /// - Check that the state is consistent with the rate limited position.
    if (mPosition > 0.0 && GunnsFluidValve::CLOSED == mState) {
        mState            = GunnsFluidValve::CLOSING;
    } else if (mPosition < 1.0 && GunnsFluidValve::OPEN == mState) {
        mState            = GunnsFluidValve::OPENING;
    }
}
