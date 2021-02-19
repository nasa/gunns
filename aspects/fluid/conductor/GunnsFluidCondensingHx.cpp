/**
@file
@brief    GUNNS Fluid Condensing Heat Exchanger implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
((core/GunnsFluidConductor.o))
*/

#include "software/exceptions/TsOutOfBoundsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "GunnsFluidCondensingHx.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name             (--)        Name of object.
/// @param[in]  nodes            (--)        Pointer to nodes.
/// @param[in]  maxConductivity  (m2)        Max conductivity.
/// @param[in]  numSegments      (--)        Number of heat exchanger segments.
/// @param[in]  htcCoeff0        (W/K)       Heat transfer 0th order coefficient.
/// @param[in]  htcCoeff1        (W*s/K/kg)  Heat transfer 1st order coefficient.
/// @param[in]  htcExponent      (--)        Heat transfer coefficient exponent.
/// @param[in]  htcLimit         (W/K)       Heat transfer coefficient limit.
///
/// @details  Default constructs this GUNNS Fluid Condensing Heat Exchanger configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxConfigData::GunnsFluidCondensingHxConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       maxConductivity,
        const int          numSegments,
        const double       htcCoeff0,
        const double       htcCoeff1,
        const double       htcExponent,
        const double       htcLimit)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, 0.0),
    mNumSegments(numSegments),
    mHtcCoeff0(htcCoeff0),
    mHtcCoeff1(htcCoeff1),
    mHtcExponent(htcExponent),
    mHtcLimit(htcLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Condensing Heat Exchanger configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxConfigData::GunnsFluidCondensingHxConfigData(
        const GunnsFluidCondensingHxConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mNumSegments(that.mNumSegments),
    mHtcCoeff0(that.mHtcCoeff0),
    mHtcCoeff1(that.mHtcCoeff1),
    mHtcExponent(that.mHtcExponent),
    mHtcLimit(that.mHtcLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Condensing Heat Exchanger configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxConfigData::~GunnsFluidCondensingHxConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag   (--)  Initial blockage malfunction flag.
/// @param[in]  malfBlockageValue  (--)  Initial blockage malfunction fractional value (0-1).
/// @param[in]  wallTemperature    (K)   Initial wall temperature all segments.
///
/// @details  Default constructs this GUNNS Fluid Condensing Heat Exchanger input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxInputData::GunnsFluidCondensingHxInputData(const bool   malfBlockageFlag,
                                                                 const double malfBlockageValue,
                                                                 const double wallTemperature)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mWallTemperature (wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Condensing Heat Exchanger input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxInputData::GunnsFluidCondensingHxInputData(
        const GunnsFluidCondensingHxInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mWallTemperature (that.mWallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Condensing Heat Exchanger input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxInputData::~GunnsFluidCondensingHxInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Condensing Heat Exchanger.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHx::GunnsFluidCondensingHx()
    :
    GunnsFluidConductor(),
    mMalfHxDegradeFlag(false),
    mMalfHxDegradeValue(0.0),
    mNumSegments(0),
    mHtcCoeff0(0.0),
    mHtcCoeff1(0.0),
    mHtcExponent(0.0),
    mHtcLimit(0.0),
    mSegmentTemperature(0),
    mSegmentHtc(0.0),
    mSensibleHeat(0.0),
    mLatentHeat(0.0),
    mTotalHeat(0.0),
    mCondensationRate(0.0),
    mCondensateFluid(0),
    mSegmentHeat(0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Condensing Heat Exchanger.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHx::~GunnsFluidCondensingHx()
{
    TS_DELETE_OBJECT(mCondensateFluid);
    TS_DELETE_ARRAY(mSegmentHeat);
    TS_DELETE_ARRAY(mSegmentTemperature);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
/// @param[in,out]  links        (--)  Link vector.
/// @param[in]      port0        (--)  Nominal inlet port map index.
/// @param[in]      port1        (--)  Nominal outlet port map index.
///
/// @callgraph
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Condensing Heat Exchanger with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHx::initialize(const GunnsFluidCondensingHxConfigData& configData,
                                        const GunnsFluidCondensingHxInputData&  inputData,
                                        std::vector<GunnsBasicLink*>&           links,
                                        const int                               port0,
                                        const int                               port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag              = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize with configuration data.
    mNumSegments           = configData.mNumSegments;
    mHtcCoeff0             = configData.mHtcCoeff0;
    mHtcCoeff1             = configData.mHtcCoeff1;
    mHtcExponent           = configData.mHtcExponent;
    mHtcLimit              = configData.mHtcLimit;

    /// - Create the internal fluid object.  This is the fluid traveling thru the normal conductive
    ///   path and contains the gas to be condensed.
    createInternalFluid();

    /// - Delete old pointers and allocate new ones.
    TS_DELETE_OBJECT     (mCondensateFluid);
    TS_DELETE_ARRAY      (mSegmentHeat);
    TS_DELETE_ARRAY      (mSegmentTemperature);
    TS_NEW_PRIM_ARRAY_EXT(mSegmentTemperature, mNumSegments, double, std::string(mName) + ".mSegmentTemperature");
    TS_NEW_PRIM_ARRAY_EXT(mSegmentHeat,        mNumSegments, double, std::string(mName) + ".mSegmentHeat");
    TS_NEW_PRIM_OBJECT_EXT(mCondensateFluid, PolyFluid,
                           (*mInternalFluid, std::string(mName) + ".mCondensateFluid", false),
                           std::string(mName) + ".mCondensateFluid");

    /// - Initialize input data.
    for (int i=0; i<mNumSegments; ++i) {
        mSegmentTemperature[i] = inputData.mWallTemperature;
    }

    /// - Initialize Malfunctions.
    mMalfHxDegradeFlag     = false;
    mMalfHxDegradeValue    = 0.0;

    /// - Initialize state data.
    mSegmentHtc            = 0.0;
    mSensibleHeat          = 0.0;
    mLatentHeat            = 0.0;
    mTotalHeat             = 0.0;
    mCondensationRate      = 0.0;
    double fractions[FluidProperties::NO_FLUID] = {0.0};
    const int gasIndex     = mNodes[0]->getOutflow()->find(FluidProperties::GUNNS_H2O);
    fractions[gasIndex]    = 1.0;
    mCondensateFluid->setPressure(1.0);   //not used
    mCondensateFluid->setFlowRate(0.0);   //not used
    mCondensateFluid->setMassAndMassFractions(0.0, fractions);

    /// - Initialize outputs.
    for (int i=0; i<mNumSegments; ++i) {
        mSegmentHeat[i]    = 0.0;
    }

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag              = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Configuration data.
/// @param[in]  inputData   (--)  Input data.
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Condensing Heat Exchanger initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHx::validate(const GunnsFluidCondensingHxConfigData& configData,
                                      const GunnsFluidCondensingHxInputData&  inputData) const
{
    /// - Throw an exception on H2O vapor not in the network.
    try {
        mNodes[0]->getOutflow()->find(FluidProperties::GUNNS_H2O);
    } catch (const TsOutOfBoundsException& e){
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "H2O vapor not in fluid.");
    }

    /// - Throw an exception on HX number of segments < 1.
    if (configData.mNumSegments < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "HX number of segments < 1.");
    }

    /// - Throw an exception if the HTC exponent not in (0.05, 20).
    if (!MsMath::isInRange(0.05, configData.mHtcExponent, 20.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                "HTC exponent not in (0.05, 20).");
    }

    /// - Throw an exception if the HTC upper limit < FLT_EPSILON.
    if (configData.mHtcLimit < FLT_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                "HTC upper limit < FLT_EPSILON.");
    }

    /// - Throw an exception on default wall temperature < 0.
    if (inputData.mWallTemperature < FLT_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Default wall temperature < FLT_EPSILON.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHx::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mSegmentHtc       = 0.0;
    mSensibleHeat     = 0.0;
    mLatentHeat       = 0.0;
    mTotalHeat        = 0.0;
    mCondensationRate = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt        (s)     Time step.
/// @param[in]  flowRate  (kg/s)  Not used.
///
/// @callgraph
///
/// @details  Updates the internal fluid of the link, and calls models for the heat exchanger,
///           slurper bar, and water separator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHx::updateFluid(const double dt       __attribute__((unused)),
                                         const double flowRate __attribute__((unused)))
{
    mInternalFluid->setFlowRate(fabs(mFlowRate));

    /// - Update the hardware models.
    computeHeatTransferCoefficient();
    degradeHeatTransferCoefficient();
    updateHeatExchanger(dt);

    /// - Update link source vector so the pressure solution will reflect the removed condensate
    ///   next pass.  Since the pressure lags a cycle there will be a small pressure error, but
    ///   mass will be conserved and GUNNS washes out pressure errors over time.
    if (fabs(mCondensationRate) > m100EpsilonLimit) {
        mSourceVector[0] = 0.0;
        mSourceVector[1] = -mCondensationRate / mCondensateFluid->getMWeight();
    } else {
        mSourceVector[0] = 0.0;
        mSourceVector[1] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Calculates the nominal heat transfer coefficient of each segment.  The overall
///           coefficient is divided equally among the segments, and is limited from going negative.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHx::computeHeatTransferCoefficient()
{
    const double fabsMdot = std::min(10.0, fabs(mFlowRate));
    double htc = mHtcCoeff0;
    if (fabsMdot > FLT_EPSILON) {
        htc += mHtcCoeff1 * powf(fabsMdot, MsMath::limitRange(0.05, mHtcExponent, 20.0));
    }
    mSegmentHtc = MsMath::limitRange(0.0, htc, mHtcLimit) / mNumSegments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Applies a degradation scale factor malfunction to the heat transfer coefficient.  This
///           malf can be used to raise or lower the effective HTC.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHx::degradeHeatTransferCoefficient()
{
    if (mMalfHxDegradeFlag) {
        mSegmentHtc *= std::max(0.0, mMalfHxDegradeValue);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt        (s)     Time step.
///
/// @details  Models heat transfer and water condensation in the heat exchanger segments.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHx::updateHeatExchanger(const double dt)
{
    mSensibleHeat     = 0.0;
    mLatentHeat       = 0.0;
    mCondensationRate = 0.0;
    for (int i = 0; i < mNumSegments; ++i) {
        mSegmentHeat[i] = 0.0;
    }

    /// - Skip if mass flow rate is too small.  Condensing heat exchangers are specific applications
    ///   designed for only one flow direction, so for reverse flow we don't do anything.
    if (mFlowRate > DBL_EPSILON) {

        const FluidProperties* propertiesH2O = mNodes[0]->getOutflow()->getProperties(FluidProperties::GUNNS_H2O);

        /// - Perform the heat transfer in each segment and add the segment contributions to the
        ///   total.  The internal fluid takes the fluid exit temperature of each segment and is
        ///   reused as the inlet temperature of the next segment.
        for (int i = 0; i < mNumSegments; ++i) {

            /// - Segment inlet fluid properties.
            const double mDot  = mInternalFluid->getFlowRate();
            const double ppH2O = mInternalFluid->getPartialPressure(FluidProperties::GUNNS_H2O);
            const double cpIn  = mInternalFluid->getSpecificHeat();
            const double tIn   = mInternalFluid->getTemperature();
            const double tDew  = propertiesH2O->getSaturationTemperature(ppH2O);

            /// - Sensible heat needed to cool fluid down to the dewpoint, limited to zero in case
            ///   the inlet fluid is already colder than dewpoint.  In GUNNS, specific heat varies
            ///   linearly with T.  Account for this by using the average of the inlet & dewpoint
            ///   specific heats.
            const double cpDew =
                mInternalFluid->computeSpecificEnthalpy(tDew, mInternalFluid->getPressure()) / tIn;
            const double cpAvg = 0.5 * (cpIn + cpDew);
            const double qDew  = std::max(0.0, (tIn - tDew) * cpAvg * mDot);

            /// - Initial calculation of outlet fluid temperature and sensible heat flux, assuming
            ///   no condensation.  This is similar to the method used by the normal heat exchanger.
            ///   As a precaution, limit heat transfer coefficient to that which would produce an
            ///   argument of 100 to the exp function below.
            double limitHtc  = std::min(mSegmentHtc, 100.0 * mDot * cpAvg);
            double htcMdotCp = limitHtc / (mDot * cpAvg);
            double tOut      = mSegmentTemperature[i] + exp(-htcMdotCp) * (tIn - mSegmentTemperature[i]);
            double qSensible = limitHtc * (tIn - tOut) / htcMdotCp;
            mSegmentHeat[i]  = 0.0;

            /// - If there is enough heat to cause condensation, proceed with condensation
            ///   calculations.
            if (qSensible > qDew and ppH2O > DBL_EPSILON) {

                /// - Segment wall temperature is limited to be above freezing and a warning message
                ///   is issued as it indicates a network setup or tuning problem.
                const double tSeg = MsMath::limitRange(273.15, mSegmentTemperature[i], 572.999);
                if (mSegmentTemperature[i] < 273.15 or mSegmentTemperature[i] > 572.999) {
                    GUNNS_WARNING("limited HX segment " << i << " temperature to valid range.");
                }

                /// - Iterate calculation of the segment exit temperature, improving the estimate of
                ///   average water properties over the temperature drop range each iteration.  This
                ///   converges in about 4 iterations.
                double deltaHvap  = 0.0;
                double cpLatent   = 0.0;
                double tCondense  = tSeg;
                for (int j=0; j<4; ++j) {

                    /// - Average condensation temperature between inlet & exit of the segment.  The
                    ///   inlet temperature (or temperature at start of condensation) is tDew, known
                    ///   for this segment.  The estimate of the exit temperature is tCondense,
                    ///   improved each iteration.
                    const double tAvg       = 0.5 * (tDew + tCondense);

                    /// - Slope of saturation pressure about the average condensation temperature.
                    const double dT         = 0.001;
                    const double dppSatdT   = (propertiesH2O->getSaturationPressure(tAvg)
                                             - propertiesH2O->getSaturationPressure(tAvg - dT)) / dT;

                    /// - Condensation rate to lower saturation pressure by dT.
                    const double condenseDt = mInternalFluid->getMassFraction(FluidProperties::GUNNS_H2O)
                                            * dppSatdT / ppH2O;

                    /// - Heat of vaporization at average condensation temperature, converted to
                    ///   J/kg.
                    deltaHvap               = propertiesH2O->getHeatOfVaporization(tAvg)
                                            * UnitConversion::UNIT_PER_KILO;

                    /// - Equivalent specific heat for condensation (latent heat).
                    cpLatent                = condenseDt * deltaHvap;

                    /// - Calculate exit temperature including condensation, starting at the
                    ///   dewpoint and using a specific heat that takes into account both the
                    ///   specific heat of the non-condensing gas, and the latent heat of
                    ///   condensation.
                    limitHtc                = std::min(mSegmentHtc, 100.0 * mDot * (cpLatent + cpDew));
                    htcMdotCp               = limitHtc / (mDot * (cpLatent + cpDew));
                    tCondense               = tSeg + exp(-htcMdotCp) * (tDew - tSeg);
                }

                /// - Now that the segment exit temperature is known, calculate the heats.
                const double qCondense      = limitHtc * (tDew - tCondense) / htcMdotCp;
                const double qLatent        = qCondense  / (1.0 + cpDew / cpLatent);
                qSensible                   = qDew + qCondense - qLatent;

                /// - Update actual condensation rate and remove it from the internal fluid.
                const double condensationRate = qLatent / deltaHvap;
                mCondensationRate += condensationRate;
                mInternalFluid->addState(mCondensateFluid, -condensationRate);
                mInternalFluid->setPressure(mNodes[0]->getPotential());

                /// - Add this segment's latent heat to the totals.
                mLatentHeat     += qLatent;
                mSegmentHeat[i] += qLatent;

                /// - When condensing, segment always exits at 100% relative humidity, so force the
                ///   exit temperature accordingly to match the water vapor we condensed out.
                ///   Ideally tCondense would already match this value but there are still some
                ///   error sources -- though it usually agrees within about 1 Kelvin now.  The
                ///   first setTemperature call is to cause the partial pressures to be updated with
                ///   the new H2O mass for the subsequent dewpoint calculation.
                mInternalFluid->setTemperature(tOut);
                tOut = propertiesH2O->getSaturationTemperature(
                        mInternalFluid->getPartialPressure(FluidProperties::GUNNS_H2O));
            }

            /// - Add this segment's sensible heat to the totals.
            mSensibleHeat   += qSensible;
            mSegmentHeat[i] += qSensible;

            /// - Set the segment fluid exit temperature.
            mInternalFluid->setTemperature(tOut);
        }
    }

    /// - Total heat flux from the fluid to the wall for the entire HX.
    mTotalHeat = mSensibleHeat + mLatentHeat;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  forcedOutflow  (--)  Not used.
/// @param[in]  fromPort       (--)  Not used.
/// @param[in]  toPort         (--)  Not used.
///
/// @details  This overrides the GunnsFluidLink implementation.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHx::transportFluid(const bool forcedOutflow __attribute__((unused)),
                                            const int  fromPort      __attribute__((unused)),
                                            const int  toPort        __attribute__((unused)))
{
    if (mFlowRate > m100EpsilonLimit) {

        /// - Forward flow removes total flow rate from port 0 and adds the total flow rate minus
        ///   the condensation rate to port 1, with the internal fluid properties.
        mNodes[0]->collectOutflux(mFlowRate);
        mNodes[1]->collectInflux (mInternalFluid->getFlowRate(), mInternalFluid);

    } else if(mFlowRate < -m100EpsilonLimit) {

        /// - Reverse flow removes total flow rate from port 1 and adds it to port 0, with no
        ///   condensation.
        mNodes[1]->collectOutflux(-mFlowRate);
        mNodes[0]->collectInflux (-mFlowRate, mInternalFluid);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
/// @param[in] value (--) HX heat transfer degradation malfunction value
///
/// @details  Sets the malf parameters to given the values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHx::setMalfHxDegrade(const bool flag, const double value)
{
    mMalfHxDegradeFlag  = flag;
    mMalfHxDegradeValue = value;
}
