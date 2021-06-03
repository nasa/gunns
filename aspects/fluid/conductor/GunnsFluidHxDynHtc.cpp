/**
@file
@brief    GUNNS Heat Exchanger With Dynamic HTC Model implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ()
*/

#include "software/exceptions/TsInitializationException.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "GunnsFluidHxDynHtc.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] coeff0   (W/K)      0th order coefficient.
/// @param[in] coeff1   (W*s/K/kg) 1st order coefficient.
/// @param[in] exponent (--)       Mass flow rate exponent.
/// @param[in] limit    (W/K)      Upper limit.
///
/// @details  Default constructs this Heat Exchanger Segment Dynamic Heat Transfer Coefficient.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHxDynHtcSegment::GunnsFluidHxDynHtcSegment(const double coeff0,
                                                     const double coeff1,
                                                     const double exponent,
                                                     const double limit)
    :
    mCoeff0(coeff0),
    mCoeff1(coeff1),
    mExponent(exponent),
    mLimit(limit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Heat Exchanger Segment Dynamic Heat Transfer Coefficient.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHxDynHtcSegment::~GunnsFluidHxDynHtcSegment()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to be copied from.
///
/// @details  Copy constructs this Heat Exchanger Segment Dynamic Heat Transfer Coefficient.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHxDynHtcSegment::GunnsFluidHxDynHtcSegment(const GunnsFluidHxDynHtcSegment& that)
    :
    mCoeff0(that.mCoeff0),
    mCoeff1(that.mCoeff1),
    mExponent(that.mExponent),
    mLimit(that.mLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] mdot        (kg/s) Mass flow rate.
/// @param[in] degradation (--)   Degradation scalar.
///
/// @returns  double (W/K) Heat transfer coefficient at given conditions.
///
/// @details  Computes and returns a new value of the heat transfer coefficient.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidHxDynHtcSegment::update(const double mdot, const double degradation)
{
    const double fabsMdot = std::min(10.0, fabs(mdot));
    double htc = mCoeff0;
    if (fabsMdot > FLT_EPSILON) {
        htc += mCoeff1 * powf(fabsMdot, MsMath::limitRange(0.05, mExponent, 20.0));
    }
    return MsMath::limitRange(0.0, htc * degradation, mLimit);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--)       Name of object.
/// @param[in] nodes                (--)       Pointer to nodes.
/// @param[in] maxConductivity      (m2)       Max conductivity.
/// @param[in] expansionScaleFactor (--)       Scale factor for isentropic gas cooling.
/// @param[in] numSegs              (--)       Number of segments.
/// @param[in] htcCoeff0            (W/K)      Overall HTC 0th order coefficient.
/// @param[in] htcCoeff1            (W*s/K/kg) Overall HTC 1st order coefficient.
/// @param[in] htcExponent          (--)       HTC mass flow rate exponent.
/// @param[in] htcLimit             (W/K)      Overall HTC upper limit.
///
/// @details Default constructs this GUNNS Fluid Heat Exchanger With Dynamic HTC configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHxDynHtcConfigData::GunnsFluidHxDynHtcConfigData(const std::string& name,
                                                           GunnsNodeList*     nodes,
                                                           const double       maxConductivity,
                                                           const double       expansionScaleFactor,
                                                           const int          numSegs,
                                                           const double       htcCoeff0,
                                                           const double       htcCoeff1,
                                                           const double       htcExponent,
                                                           const double       htcLimit)
    :
    GunnsFluidHeatExchangerConfigData(name, nodes, maxConductivity, expansionScaleFactor, numSegs),
    mHtcCoeff0(htcCoeff0),
    mHtcCoeff1(htcCoeff1),
    mHtcExponent(htcExponent),
    mHtcLimit(htcLimit),
    mSegsHtc()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Heat Exchanger With Dynamic HTC configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHxDynHtcConfigData::GunnsFluidHxDynHtcConfigData(const GunnsFluidHxDynHtcConfigData& that)
    :
    GunnsFluidHeatExchangerConfigData(that),
    mHtcCoeff0(that.mHtcCoeff0),
    mHtcCoeff1(that.mHtcCoeff1),
    mHtcExponent(that.mHtcExponent),
    mHtcLimit(that.mHtcLimit),
    mSegsHtc(that.mSegsHtc)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Heat Exchanger With Dynamic HTC configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHxDynHtcConfigData::~GunnsFluidHxDynHtcConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] coeff0   (W/K)      0th order coefficient.
/// @param[in] coeff1   (W*s/K/kg) 1st order coefficient.
/// @param[in] exponent (--)       Mass flow rate exponent.
/// @param[in] limit    (W/K)      Upper limit.
///
/// @details  Pushes a new heat transfer coefficient object onto the vector with the given values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHxDynHtcConfigData::addSegment(const double coeff0,
                                              const double coeff1,
                                              const double exponent,
                                              const double limit)
{
    mSegsHtc.push_back(GunnsFluidHxDynHtcSegment(coeff0, coeff1, exponent, limit));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag           (--)   Blockage malfunction flag.
/// @param[in]  malfBlockageValue          (--)   Blockage malfunction fractional value (0-1).
/// @param[in]  initialSegmentTemperature  (K)    Initial value for segment temperatures.
///
/// @details  Default constructs this GUNNS Fluid Heat Exchanger With Dynamic HTC input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHxDynHtcInputData::GunnsFluidHxDynHtcInputData(const bool   malfBlockageFlag,
                                                         const double malfBlockageValue,
                                                         const double initialSegmentTemperature)
    :
    GunnsFluidHeatExchangerInputData(malfBlockageFlag, malfBlockageValue, 1.0, initialSegmentTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Heat Exchanger With Dynamic HTC input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHxDynHtcInputData::GunnsFluidHxDynHtcInputData(const GunnsFluidHxDynHtcInputData& that)
    :
        GunnsFluidHeatExchangerInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Heat Exchanger With Dynamic HTC input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHxDynHtcInputData::~GunnsFluidHxDynHtcInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Heat Exchanger With Dynamic HTC.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHxDynHtc::GunnsFluidHxDynHtc()
    :
    GunnsFluidHeatExchanger(),
    mSegsDynHtc(0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Heat Exchanger With Dynamic HTC.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHxDynHtc::~GunnsFluidHxDynHtc()
{
    TS_DELETE_ARRAY(mSegsDynHtc);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
/// @param[in,out]  links       (--)  Link vector.
/// @param[in]      port0       (--)  Nominal inlet port map index.
/// @param[in]      port1       (--)  Nominal outlet port map index.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Heat Exchanger With Dynamic HTC with configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHxDynHtc::initialize(const GunnsFluidHxDynHtcConfigData& configData,
                                    const GunnsFluidHxDynHtcInputData&  inputData,
                                    std::vector<GunnsBasicLink*>&       links,
                                    const int                           port0,
                                    const int                           port1)
{
    /// - First initialize & validate parent.
    GunnsFluidHeatExchanger::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Validate configuration data.
    validate(configData);

    /// - Delete any old arrays and allocate new ones.
    TS_DELETE_ARRAY(mSegsDynHtc);
    TS_NEW_CLASS_ARRAY_EXT(mSegsDynHtc, mNumSegs, GunnsFluidHxDynHtcSegment, (),
                           std::string(mName) + ".mSegsDynHtc");

    /// - Initialize with configuration data.
    for(int i = 0; i < mNumSegs; ++i) {
        if (configData.mSegsHtc.size() > 0) {
            mSegsDynHtc[i] = configData.mSegsHtc[i];
        } else {
            mSegsDynHtc[i].mCoeff0   = configData.mHtcCoeff0 / mNumSegs;
            mSegsDynHtc[i].mCoeff1   = configData.mHtcCoeff1 / mNumSegs;
            mSegsDynHtc[i].mExponent = configData.mHtcExponent;
            mSegsDynHtc[i].mLimit    = configData.mHtcLimit  / mNumSegs;
        }
    }

    /// - Initialize dynamic segment heat transfer coefficients.
    computeHeatTransferCoefficient();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Configuration data.
///
/// @throws   TsInitializationException
///
/// @details  Validates this this GUNNS Fluid Heat Exchanger With Dynamic HTC initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHxDynHtc::validate(const GunnsFluidHxDynHtcConfigData& configData) const
{
    if (configData.mSegsHtc.size() > 0) {
        /// - Throw an exception if size of HTC overrides vector doesn't equal number of segments.
        if (static_cast<int>(configData.mSegsHtc.size()) != mNumSegs) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Segment HTC overrides vector does not match number of segments.");
        }

        for (unsigned int i = 0; i < configData.mSegsHtc.size(); ++i) {
            /// - Throw an exception if a segment HTC exponent not in (0.05, 20).
            if (!MsMath::isInRange(0.05, configData.mSegsHtc[i].mExponent, 20.0)) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "A segment's HTC exponent not in (0.05, 20).");
            }

            /// - Throw an exception if a segment HTC upper limit < FLT_EPSILON.
            if (configData.mSegsHtc[i].mLimit < FLT_EPSILON) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "A segment's HTC upper limit < FLT_EPSILON.");
            }
        }
    } else {
        /// - Throw an exception if the HTC exponent not in (0.05, 20).
        if (!MsMath::isInRange(0.05, configData.mHtcExponent, 20.0)) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "HTC exponent not in (0.05, 20).");
        }

        /// - Throw an exception if the HTC upper limit < FLT_EPSILON.
        if (configData.mHtcLimit < FLT_EPSILON) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Overall HTC upper limit < FLT_EPSILON.");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Calculates the segment heat transfer coefficients. This method is virtual and can be
///           overridden by derived classes.  Degrade malfunctions scale the nominal coefficient.
///           The segment degrade malfunction takes precedence over the overall degrade in each
///           segment.  The degraded coefficient is limited between zero and its default value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHxDynHtc::computeHeatTransferCoefficient()
{
    for (int i = 0; i < mNumSegs; i++) {

        double degradation = 1.0;
        if (mMalfSegDegradeFlag[i]) {
            degradation = mMalfSegDegradeValue[i];
        } else if (mMalfHxDegradeFlag) {
            degradation = mMalfHxDegradeValue;
        }
        mSegHtc[i] = mSegsDynHtc[i].update(mFlowRate, MsMath::limitRange(0.0, degradation, 1.0));
    }
}
