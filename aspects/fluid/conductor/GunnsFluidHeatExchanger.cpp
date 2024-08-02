/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (Classes for the GUNNS Fluid Heat Exchanger link model.)

 ASSUMPTIONS AND LIMITATIONS:
 ((SI units only))

 REFERENCE:
 (A Transient Thermal Model of the Portable Life Support System AIAA-94-4622, Heat Exchanger pg 5.)

 LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o))

 PROGRAMMERS:
 (((Chuck Sorensen) (LZT) (Jan 2011) (Initial Prototype)))
 **************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "GunnsFluidHeatExchanger.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                  (--)  Name of object.
/// @param[in]  nodes                 (--)  Pointer to nodes.
/// @param[in]  maxConductivity       (m2)  Max conductivity.
/// @param[in]  expansionScaleFactor  (--)  Scale factor for isentropic gas cooling.
/// @param[in]  numSegs               (--)  Number of segments.
///
/// @details  Default constructs this GUNNS Fluid Heat Exchanger link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeatExchangerConfigData::GunnsFluidHeatExchangerConfigData(
                                                            const std::string& name,
                                                            GunnsNodeList*     nodes,
                                                            const double       maxConductivity,
                                                            const double       expansionScaleFactor,
                                                            const int          numSegs)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mNumSegs(numSegs)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Heat Exchanger link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeatExchangerConfigData::GunnsFluidHeatExchangerConfigData(
                                                      const GunnsFluidHeatExchangerConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mNumSegs(that.mNumSegs)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Heat Exchanger link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeatExchangerConfigData::~GunnsFluidHeatExchangerConfigData()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag           (--)   Blockage malfunction flag.
/// @param[in]  malfBlockageValue          (--)   Blockage malfunction fractional value (0-1).
/// @param[in]  heatTransferCoefficient    (W/K)  Default heat transfer coefficient (W/K).
/// @param[in]  initialSegmentTemperature  (K)    Initial value for segment temperatures.
///
/// @details  Default constructs this GUNNS Fluid Heat Exchanger link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeatExchangerInputData::GunnsFluidHeatExchangerInputData(
                                                             const bool   malfBlockageFlag,
                                                             const double malfBlockageValue,
                                                             const double heatTransferCoefficient,
                                                             const double initialSegmentTemperature)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mHeatTransferCoefficient(heatTransferCoefficient),
    mInitialSegmentTemperature(initialSegmentTemperature),
    mSegmentHtcOverrides(0),
    mTemperatureOverride(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Heat Exchanger link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeatExchangerInputData::GunnsFluidHeatExchangerInputData(
                                                       const GunnsFluidHeatExchangerInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mHeatTransferCoefficient(that.mHeatTransferCoefficient),
    mInitialSegmentTemperature(that.mInitialSegmentTemperature),
    mSegmentHtcOverrides(that.mSegmentHtcOverrides),
    mTemperatureOverride(that.mTemperatureOverride)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Heat Exchanger link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeatExchangerInputData::~GunnsFluidHeatExchangerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Heat Exchanger link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeatExchanger::GunnsFluidHeatExchanger()
    :
    GunnsFluidConductor(),
    mMalfHxDegradeFlag(false),
    mMalfHxDegradeValue(0.0),
    mMalfSegDegradeFlag(0),
    mMalfSegDegradeValue(0),
    mNumSegs(0),
    mSegHtcDefault(0),
    mSegHtc(0),
    mSegTemperature(0),
    mSegEnergyGain(0),
    mTotalEnergyGain(0.0),
    mDeltaTemperature(0.0),
    mTemperatureOverride(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Heat Exchanger link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHeatExchanger::~GunnsFluidHeatExchanger()
{
    TS_DELETE_ARRAY(mSegEnergyGain);
    TS_DELETE_ARRAY(mSegTemperature);
    TS_DELETE_ARRAY(mSegHtc);
    TS_DELETE_ARRAY(mSegHtcDefault);
    TS_DELETE_ARRAY(mMalfSegDegradeValue);
    TS_DELETE_ARRAY(mMalfSegDegradeFlag);
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
/// @details  Initializes this GUNNS Fluid Heat Exchanger link model with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeatExchanger::initialize(const GunnsFluidHeatExchangerConfigData& configData,
                                         const GunnsFluidHeatExchangerInputData&  inputData,
                                         std::vector<GunnsBasicLink*>&            links,
                                         const int                                port0,
                                         const int                                port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize with configuration data.
    mNumSegs = configData.mNumSegs;

    /// - Delete any old arrays and allocate new ones.
    TS_DELETE_ARRAY(mSegEnergyGain);
    TS_DELETE_ARRAY(mSegTemperature);
    TS_DELETE_ARRAY(mSegHtc);
    TS_DELETE_ARRAY(mSegHtcDefault);
    TS_DELETE_ARRAY(mMalfSegDegradeValue);
    TS_DELETE_ARRAY(mMalfSegDegradeFlag);
    TS_NEW_PRIM_ARRAY_EXT(mMalfSegDegradeFlag,  mNumSegs, bool,   std::string(mName) + ".mMalfSegDegradeFlag");
    TS_NEW_PRIM_ARRAY_EXT(mMalfSegDegradeValue, mNumSegs, double, std::string(mName) + ".mMalfSegDegradeValue");
    TS_NEW_PRIM_ARRAY_EXT(mSegHtcDefault,       mNumSegs, double, std::string(mName) + ".mSegHtcDefault");
    TS_NEW_PRIM_ARRAY_EXT(mSegHtc,              mNumSegs, double, std::string(mName) + ".mSegHtc");
    TS_NEW_PRIM_ARRAY_EXT(mSegTemperature,      mNumSegs, double, std::string(mName) + ".mSegTemperature");
    TS_NEW_PRIM_ARRAY_EXT(mSegEnergyGain,       mNumSegs, double, std::string(mName) + ".mSegEnergyGain");

    /// - Initialize from input data.
    mTemperatureOverride = inputData.mTemperatureOverride;

    for(int i = 0; i < mNumSegs; ++i) {
        mSegTemperature[i] = inputData.mInitialSegmentTemperature;

        /// - Segment heat transfer coefficients are initialized as follows:  if the optional input
        ///   data overrides array is supplied and the array value is not negative, the array's
        ///   value is used.  Otherwise, the default value is used.
        if (inputData.mSegmentHtcOverrides and inputData.mSegmentHtcOverrides[i] >= 0.0) {
            mSegHtcDefault[i] = inputData.mSegmentHtcOverrides[i];
        } else {
            mSegHtcDefault[i] = inputData.mHeatTransferCoefficient / mNumSegs;
        }
        /// - Initialize computed segment heat transfer coefficients.
        mSegHtc[i]              = mSegHtcDefault[i];

        /// - Initialize remaining attributes.
        mMalfSegDegradeFlag[i]  = false;
        mMalfSegDegradeValue[i] = 0.0;
        mSegEnergyGain[i]       = 0.0;
    }
    mTotalEnergyGain  = 0.0;
    mDeltaTemperature = 0.0;
    mDeltaTemperature = 0.0;

    /// - Initialize malfunctions
    mMalfHxDegradeFlag = false;
    mMalfHxDegradeValue = 0.0;

    /// - Create the internal fluid.
    createInternalFluid();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Configuration data.
/// @param[in]  inputData   (--)  Input data.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates this this GUNNS Fluid Heat Exchanger link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeatExchanger::validate(const GunnsFluidHeatExchangerConfigData& configData,
                                       const GunnsFluidHeatExchangerInputData&  inputData) const
{
    /// - Throw an exception if number of segments < 1.
    if(configData.mNumSegs < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Number of segments < 1.");
    }

    /// - Throw an exception if default heat transfer coefficient < FLT_EPSILON.
    if (inputData.mHeatTransferCoefficient < static_cast<double>(FLT_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Default heat transfer coefficient < FLT_EPSILON.");
    }

    /// - Throw an exception if default initial segment temperature < FLT_EPSILON.
    if(inputData.mInitialSegmentTemperature < static_cast<double>(FLT_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Default initial segment temperature < FLT_EPSILON.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeatExchanger::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mTotalEnergyGain  = 0.0;
    mDeltaTemperature = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt        (s)     Time step.
/// @param[in]  flowRate  (kg/s)  Mass flow rate.
///
/// @return   void
///
/// @details  Updates the internal fluid of this this GUNNS Fluid Pipe link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeatExchanger::updateFluid(const double dt, const double flowRate)
{
    /// - Compute heat transfer coefficient.
    computeHeatTransferCoefficient();

    /// - For each segment compute the energy gain loss and update the fluid temperature.
    updateSegments(dt, flowRate);

    /// - Compute the total temperature change across the heat exchanger
    mDeltaTemperature = mInternalFluid->getTemperature() - mNodes[0]->getOutflow()->getTemperature();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt        (s)     Time step.
/// @param[in]  flowRate  (kg/s)  Mass flow rate.
///
/// @return   void
///
/// @details  Updates the fluid temperature and stores the computed segment energy gain.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeatExchanger::updateSegments(const double dt, const double flowRate) {

    mTotalEnergyGain = 0.0;

    /// - Skip if mass flow rate or time step are too small.
    if (fabs(flowRate) > DBL_EPSILON && dt > DBL_EPSILON) {

        /// - Set up indexing based on flow direction.
        int start    = 0;
        int end      = mNumSegs;
        int inc      = 1;

        if (flowRate < 0.0) {
            start    = mNumSegs - 1;
            end      = -1;
            inc      = -1;
        }

        /// - Apply temperature override on the internal fluid before the first segment.
        applyTemperatureOverride();

        /// - Perform the heat transfer in each segment and add the calculated heat flux to the
        ///   total.  Then apply the temperature override to the segment exit temperature.  The
        ///   internal fluid takes the fluid exit temperature of each segment and is re-used as the
        ///   inlet temperature of the next segment.
        for (int i = start; i != end; i += inc) {
            mSegEnergyGain[i] = GunnsFluidUtils::computeConvectiveHeatFlux(mInternalFluid,
                                                                           flowRate,
                                                                           mSegHtc[i],
                                                                           mSegTemperature[i]);
            mTotalEnergyGain += mSegEnergyGain[i];
            applyTemperatureOverride();
        }

    } else {
        for (int i = 0; i < mNumSegs; ++i) {
            mSegEnergyGain[i] = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Applies the override temperature value (if it is above absolute zero) to the link's
///           internal fluid.  If the override temperature value is zero, nothing is done.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeatExchanger::applyTemperatureOverride() {
    if (static_cast<double>(FLT_EPSILON) < mTemperatureOverride) {
        mInternalFluid->setTemperature(mTemperatureOverride);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @details  Calculates the segment heat transfer coefficients. This method is virtual and can be
///           overridden by derived classes.  Degrade malfunctions scale the nominal coefficient.
///           The segment degrade malfunction takes precedence over the overall degrade in each
///           segment.  The degraded coefficient is limited between zero and its default value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeatExchanger::computeHeatTransferCoefficient()
{
    for (int i = 0; i < mNumSegs; i++) {

        mSegHtc[i] = mSegHtcDefault[i];
        if (mMalfSegDegradeFlag[i]) {
            mSegHtc[i] *= mMalfSegDegradeValue[i];
        } else if (mMalfHxDegradeFlag) {
            mSegHtc[i] *= mMalfHxDegradeValue;
        }

        mSegHtc[i] = MsMath::limitRange(0.0, mSegHtc[i], mSegHtcDefault[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
/// @param[in] value (--) HX heat transfer degradation malfunction value
///
/// @details  Sets the malf parameters to given the values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeatExchanger::setMalfHxDegrade(const bool flag, const double value)
{
    mMalfHxDegradeFlag  = flag;
    mMalfHxDegradeValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] segment (--) The segment number to apply the malfunction to
/// @param[in] flag    (--) Malfunction activation flag, true activates
/// @param[in] value   (--) HX heat transfer degradation malfunction value
///
/// @details  Sets the malf parameters to given the values.  Calling this method with default
///           arguments resets the malfunction.  A valid segment number must be supplied.  If the
///           segment number is not valid then the method issues a H&S warning and ignores the
///           request.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeatExchanger::setMalfSegDegrade(const int segment, const bool flag, const double value)
{
    if (MsMath::isInRange(0, segment, mNumSegs-1)) {
        mMalfSegDegradeFlag[segment]  = flag;
        mMalfSegDegradeValue[segment] = value;
    } else {
        GUNNS_WARNING("ignored invalid segment " << segment << ".");
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (--) HX heat transfer degradation malfunction value
///
/// @details  Sets the fluid temperature override.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHeatExchanger::setTemperatureOverride(double value)
{
    mTemperatureOverride = std::max(0.0, value);
}
