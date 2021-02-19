/**
@file
@brief    GUNNS Fluid High-Fidelity Valve Model implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((aspects/fluid/hi-fi/GunnsFluidHiFiOrifice.o))
**************************************************************************************************/

#include "GunnsFluidHiFiValve.hh"

#include "core/GunnsFluidUtils.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                   (--) Name of object.
/// @param[in] nodes                  (--) Pointer to nodes.
/// @param[in] coefficientType        (--) Type of coefficient to be specified.
/// @param[in] coefficientValue       (--) Coefficient value of the specified type.
/// @param[in] throatDiameter         (m)  Throat diameter.
/// @param[in] criticalReynolds       (--) Optional Reynolds number at laminar/turbulent transition.
/// @param[in] expansionScaleFactor   (--) (0-1) Optional scaling for expansion gas cooling.
/// @param[in] flowTuningFactor       (--) Optional factor for flow tuning.
///
/// @details  Default constructs this GUNNS Fluid Hi-Fi Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiValveConfigData::GunnsFluidHiFiValveConfigData(const std::string& name,
                                                             GunnsNodeList*     nodes,
                                                             const CoeffTypes   coefficientType,
                                                             const double       coefficientValue,
                                                             const double       throatDiameter,
                                                             const double       criticalReynolds,
                                                             const double       expansionScaleFactor,
                                                             const double       flowTuningFactor)
    :
    GunnsFluidHiFiOrificeConfigData(name, nodes, coefficientType, coefficientValue, throatDiameter,
                                    criticalReynolds, expansionScaleFactor, flowTuningFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Hi-Fi Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiValveConfigData::GunnsFluidHiFiValveConfigData(const GunnsFluidHiFiValveConfigData& that)
    :
    GunnsFluidHiFiOrificeConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hi-Fi Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiValveConfigData::~GunnsFluidHiFiValveConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag   (--)    Blockage malfunction flag.
/// @param[in]  malfBlockageValue  (--)    Blockage malfunction fractional value (0-1).
/// @param[in]  position           (--)    Fractional position.
/// @param[in]  malfLeakThruFlag   (--)    Leak through rate malfunction flag.
/// @param[in]  malfLeakThruValue  (kg/s)  Leak through rate malfunction value.
///
/// @details  Default constructs this GUNNS Fluid Hi-Fi Valve link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiValveInputData::GunnsFluidHiFiValveInputData(const bool   malfBlockageFlag,
                                                           const double malfBlockageValue,
                                                           const double position,
                                                           const bool   malfLeakThruFlag,
                                                           const double malfLeakThruValue)
    :
    GunnsFluidHiFiOrificeInputData(malfBlockageFlag, malfBlockageValue),
    mPosition(position),
    mMalfLeakThruFlag(malfLeakThruFlag),
    mMalfLeakThruValue(malfLeakThruValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   --  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Hi-Fi Valve link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiValveInputData::GunnsFluidHiFiValveInputData(const GunnsFluidHiFiValveInputData& that)
    :
    GunnsFluidHiFiOrificeInputData(that),
    mPosition(that.mPosition),
    mMalfLeakThruFlag(that.mMalfLeakThruFlag),
    mMalfLeakThruValue(that.mMalfLeakThruValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hi-Fi Valve link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiValveInputData::~GunnsFluidHiFiValveInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Hi-Fi Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiValve::GunnsFluidHiFiValve()
    :
    GunnsFluidHiFiOrifice(),
    mMalfLeakThruFlag(false),
    mMalfLeakThruValue(0.0),
    mPosition(0.0),
    mLeakArea(0.0),
    mLastLeakRate(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hi-Fi Valve link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFiValve::~GunnsFluidHiFiValve()
{
    // nothing to do
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
/// @details  Initializes this GUNNS Fluid Hi-Fi Valve link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiValve::initialize(const GunnsFluidHiFiValveConfigData& configData,
                                     const GunnsFluidHiFiValveInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        links,
                                     const int                            port0,
                                     const int                            port1)
{
    /// - Initialize the parent class.
    GunnsFluidHiFiOrifice::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag             = false;

    /// - Initialize with input data.
    mMalfLeakThruFlag     = inputData.mMalfLeakThruFlag;
    mMalfLeakThruValue    = inputData.mMalfLeakThruValue;
    mPosition             = inputData.mPosition;

    /// - Initialize remaining state data.
    mLeakArea             = 0.0;
    mLastLeakRate         = 0.0;

    /// - Validate the initial state.
    validate();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag             = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Hi-Fi Valve link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiValve::validate() const
{
    /// - Throw an exception if valve position < 0 or valve position > 1 .
    if (!MsMath::isInRange(0.0, mPosition, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Position outside valid range (0-1).");
    }

    /// - Throw an exception if malfunction leak through rate < 0.
    if (mMalfLeakThruValue < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Malfunction leak through rate < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiValve::restartModel()
{
    /// - Reset the base class.
    GunnsFluidHiFiOrifice::restartModel();

    /// - Reset non-config & non-checkpointed class attributes (currently none).
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s)  Time step (not used).
///
/// @return   void
///
/// @note     Derived classes should call this method at the end of their updateState method.
///
/// @details  Processes any malfunctions and updates this GUNNS Fluid Hi-Fi Valve link model state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiValve::updateState(const double dt __attribute__((unused)))
{
    if (mMalfLeakThruFlag) {
        /// - For leak thru malfunction, compute the corresponding leak area once on malfunction
        ///   rate change based on last-pass flow conditions and regime.
        if (mLastLeakRate != mMalfLeakThruValue) {
            mLastLeakRate  = mMalfLeakThruValue;

            /// - Determine flow & fluid conditions.
            int inletPort = 0;
            int exitPort  = 1;
            if (mPotentialVector[1] > mPotentialVector[0]) {
                inletPort = 1;
                exitPort  = 0;
            }
            const double gamma = mNodes[inletPort]->getOutflow()->getAdiabaticIndex();
            const double p0    = UnitConversion::PA_PER_KPA * mPotentialVector[inletPort];
            const double p1    = UnitConversion::PA_PER_KPA * mPotentialVector[exitPort];
            const double rho0  = mNodes[inletPort]->getOutflow()->getDensity();

            /// - Find the valve's mass flux (kg/s/m2) from on last-pass conditions and flow regime.
            double massFlux = 0.0;
            if (FluidProperties::GAS == mNodes[0]->getOutflow()->getPhase()) {
                if (mPressureRatio < 1.0) {                                        // choked gas
                    massFlux = mCdActual * computeCriticalGasFlux(gamma, p0, rho0);
                } else {                                                           // non-choked gas
                    massFlux = mCdActual * computeSubCriticalGasFlux(gamma, p0, rho0, p1);
                }
            } else {                                                               // liquid
                const double rho1   = mNodes[exitPort]->getOutflow()->getDensity();
                const double rhoAvg = 0.5 * (rho0 + rho1);
                const double dp     = p0 - p1;
                massFlux = mCdActual * computeBernoulliFlux(rhoAvg, dp);
            }

            /// - The leak area is the desired initial mass rate divided by the valve's mass flux:
            ///   (m2) = (kg/s) / (kg/s/m2)
            if (massFlux > DBL_EPSILON) {
                mLeakArea = MsMath::limitRange(0.0, mMalfLeakThruValue / massFlux, mThroatArea);
            }
        }
    } else {
        /// - For no active malfunction, zero out leak area.
        mLastLeakRate      = 0.0;
        mLeakArea          = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (m2)  The effective throat area of the valve.
///
/// @details  Getter method that returns the effective throat area including base orifice class
///           effective area, valve position and leak-thru malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidHiFiValve::getEffectiveArea() const
{
    double area = mPosition * GunnsFluidHiFiOrifice::getEffectiveArea();
    if (mMalfLeakThruFlag) {
        area = std::max(area, mLeakArea);
    }
    return area;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag   (--)    Malfunction activation flag.
/// @param[in] value  (kg/s)  Desired initial leak mass flow rate.
///
/// @returns  void
///
/// @details  Arguments are defaulted to deactivate the malfunction, so you can conveniently turn
///           it off by calling setMalfLeakThru().
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFiValve::setMalfLeakThru(const bool flag, const double value)
{
    mMalfLeakThruFlag  = flag;
    mMalfLeakThruValue = value;
}
