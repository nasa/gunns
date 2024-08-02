/**
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
    (software/exceptions/TsInitializationException.o)
    (simulation/hs/TsHsMsg.o)
    )


PROGRAMMERS:
    (
    ((Nicholas Kaufmann) (L-3 Comm) (Oct 2012) (TS21) (Initial Version))
    )
 **/

#include "aspects/electrical/SolarArray/PVCellCompanionModel.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsBasicNode.hh"
#include "math/MsMath.hh"

/// @details -- Max degrade
const double PVCellCompanionModel::mMaxDegradation    = 1.0;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Config Data
/// @param[in] vocRef           (V) reference value of open circuit voltage
/// @param[in] vmpRef           (V) reference value of voltage at max power
/// @param[in] vocTempCoeff     (--) open circuit voltage temperature coeff
/// @param[in] iscRef           (amp) reference value of source current
/// @param[in] impRef           (amp) reference value for current at max power
/// @param[in] iscTempCoeff     (--) source current temperature coeff
/// @param[in] iSat             (--) saturation current of diode in companion network
/// @param[in] temperatureRef   (K) the temperature at which the reference values were recorded
/// @param[in] cellDegradation  (--) the percent amount to degrade cell by. This accounts for age, damage, etc
/// @param[in] seriesResistance (ohm) resistance for series resistor of companion network
/// @param[in] shuntResistance  (ohm) resistance for shunt resistor of companion network
/// @param[in] vCrit            (--) a value multiplier for voc after vCrit*voc, model will apply damping to dV
/// @param[in] backSideIscReduc (--) a percentage value to multiply isc by if back side is lit. ex: .30
/// @param[in] cellEfficiency   (--) The reference cell's max power output to solar power input ratio.
/// @param[in] cellArea         (m2) The reference cell's area.
/// @details  Constructs the Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
PVCellCompanionModelConfigData::PVCellCompanionModelConfigData(const double vocRef               ,
                                                               const double vmpRef               ,
                                                               const double vocTempCoeff         ,
                                                               const double iscRef               ,
                                                               const double impRef               ,
                                                               const double iscTempCoeff         ,
                                                               const double iSat                 ,
                                                               const double temperatureRef       ,
                                                               const double cellDegradation      ,
                                                               const double seriesResistance     ,
                                                               const double shuntResistance      ,
                                                               const double vCrit                ,
                                                               const double backSideIscReduc     ,
                                                               const double cellEfficiency       ,
                                                               const double cellArea
                                                               )
     :
    mVocRef(vocRef),
    mVmpRef(vmpRef),
    mVocTempCoefficient(vocTempCoeff),
    mIscRef(iscRef),
    mImpRef(impRef),
    mIscTempCoefficient(iscTempCoeff),
    mIsat(iSat),
    mTemperatureRef(temperatureRef),
    mCellDegradation(cellDegradation),
    mRs(seriesResistance),
    mRsh(shuntResistance),
    mVCrit(vCrit),
    mBackSideIscReduction(backSideIscReduc),
    mCellEfficiency(cellEfficiency),
    mCellArea(cellArea)
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
PVCellCompanionModelConfigData::PVCellCompanionModelConfigData(const PVCellCompanionModelConfigData& that):

        mVocRef(that.mVocRef),
        mVmpRef(that.mVmpRef),
        mVocTempCoefficient(that.mVocTempCoefficient),
        mIscRef(that.mIscRef),
        mImpRef(that.mImpRef),
        mIscTempCoefficient(that.mIscTempCoefficient),
        mIsat(that.mIsat),
        mTemperatureRef(that.mTemperatureRef),
        mCellDegradation(that.mCellDegradation),
        mRs(that.mRs),
        mRsh(that.mRsh),
        mVCrit(that.mVCrit),
        mBackSideIscReduction(that.mBackSideIscReduction),
        mCellEfficiency(that.mCellEfficiency),
        mCellArea(that.mCellArea)
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
PVCellCompanionModelConfigData::~PVCellCompanionModelConfigData()
{
    //Nothing to do
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Input Data
/// @param[in] cellPowerMalfIsEnabled         (--)  malfunctions the cell power to 0 when true.
/// @param[in] cellDegradationMalfIsEnabled   (--)  bool when true enables degradation override value
/// @param[in] cellDegradationOverrideValue   (--)  a double value which overrides default cell degradation. 0 to 1
/// @details  Constructs the Input data
////////////////////////////////////////////////////////////////////////////////////////////////////
PVCellCompanionModelInputData::PVCellCompanionModelInputData(const bool cellPowerMalfIsEnabled      ,
                                                             const bool cellDegradationMalfIsEnabled,
                                                             const double cellDegradationOverrideValue)
     :
    mCellPowerMalfunctionIsEnabled(cellPowerMalfIsEnabled),
    mCellDegradationMalfunctionIsEnabled(cellDegradationMalfIsEnabled),
    mCellDegradationOverrideValue(cellDegradationOverrideValue)
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
PVCellCompanionModelInputData::PVCellCompanionModelInputData(const PVCellCompanionModelInputData& that):

    mCellPowerMalfunctionIsEnabled(that.mCellPowerMalfunctionIsEnabled),
    mCellDegradationMalfunctionIsEnabled(that.mCellDegradationMalfunctionIsEnabled),
    mCellDegradationOverrideValue(that.mCellDegradationOverrideValue)
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Input Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
PVCellCompanionModelInputData::~PVCellCompanionModelInputData()
{
    //Nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the PVCellCompanionModel
///////////////////////////////////////////////////////////////////////////////////////////////////
PVCellCompanionModel::PVCellCompanionModel():
   mCellPowerMalfunctionIsEnabled(false),
   mCellDegradationMalfunctionIsEnabled(false),
   mCellDegradationOverrideValue(false),
   mVocRef(0.0),
   mVmpRef(0.0),
   mVocTempCoefficient(0.0),
   mIscRef(0.0),
   mImpRef(0.0),
   mIscTempCoefficient(0.0),
   mIsat(0.0),
   mTemperatureRef(0.0),
   mCellDegradation(0.0),
   mRs(0.0),
   mRsh(0.0),
   mVCrit(0.0),
   mBackSideIscReduction(0.0),
   mCellEfficiency(0.0),
   mCellArea(0.0),
   mTemperature(0.0),
   mSunAngle(1.57),
   mSunAngleFromEnv(1.57),
   mSunAngleScalar(1.0),
   mSunIntensity(1.0),
   mIsBackSideLit(false),
   mV(0.0),
   mVlast(0.615),
   mVmp(0.0),
   mVoc(0.0),
   mVd(0.0),
   mI(0.0),
   mIlast(0.0),
   mImp(0.0),
   mIsc(0.0),
   mId(0.0),
   mIl(0.0),
   mIeqCell(0.0),
   mGeqCell(0.0),
   mLambda(0.0)
{
    //Nothing to do
}


PVCellCompanionModel::~PVCellCompanionModel()
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   configData   (--) Reference to Link Config Data
/// @param[in]   inputData    (--) Reference to Link Input Data
/// @throws   TsInitializationException
/// @details  Initializes the object with configuration and input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void PVCellCompanionModel::initialize(const PVCellCompanionModelConfigData& cd, const PVCellCompanionModelInputData& id)
{
    //populate the class attributes with config data
    mVocRef               = cd.mVocRef;
    mVmpRef               = cd.mVmpRef;
    mVocTempCoefficient   = cd.mVocTempCoefficient;
    mIscRef               = cd.mIscRef;
    mImpRef               = cd.mImpRef;
    mIscTempCoefficient   = cd.mIscTempCoefficient;
    mIsat                 = cd.mIsat;
    mTemperatureRef       = cd.mTemperatureRef;
    mCellDegradation      = cd.mCellDegradation;
    mRs                   = cd.mRs;
    mRsh                  = cd.mRsh;
    mVCrit                = cd.mVCrit;
    mBackSideIscReduction = cd.mBackSideIscReduction;
    mCellEfficiency       = cd.mCellEfficiency;
    mCellArea             = cd.mCellArea;

    //populate the class attributes with input data
    mCellPowerMalfunctionIsEnabled       = id.mCellPowerMalfunctionIsEnabled;
    mCellDegradationMalfunctionIsEnabled = id.mCellDegradationMalfunctionIsEnabled;
    mCellDegradationOverrideValue        = id.mCellDegradationOverrideValue;

    mVoc = mVocRef;
    mIsc = mIscRef;

    //Validate the input and configuration values
    validate();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  validates the passed in configuration and input data for Initialize
////////////////////////////////////////////////////////////////////////////////////////////////////
void PVCellCompanionModel::validate()
{
    std::string mName = "PVCellCompanionModel";

    if ((mVocRef < 0)){
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "mVocRef must be greater than or equal to zero");
    }
    if ((mVmpRef < 0)){
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "mVmpRef must be greater than or equal to zero");
    }
    if ((mIscRef < 0)){
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "mIscRef must be greater than or equal to zero");
    }
    if ((mImpRef < 0)){
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "mImpRef must be greater than or equal to zero");
    }
    if ((mIsat < 0)){
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "mIsat must be greater than or equal to zero");
    }
    if ((mRs < 0)){
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "mRs is a resistance. Resistance must be greater than or equal to zero.");
    }
    if ((mRsh < 0)){
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "mRs is a resistance. Resistance must be greater than or equal to zero.");
    }
    if ((mVCrit < 0) || (mVCrit > 1)){
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "mVCrit must be between 0 and 1.0");
    }
    if ((mCellDegradation < 0) || (mCellDegradation > 1)){
               GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                           "mCellDegradation must be between 0 and 1.0");
    }
    if ((mCellDegradationOverrideValue < 0) || (mCellDegradationOverrideValue > 1)){
               GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                           "mCellDegradationOverrideValue must be between 0 and 1.0");
    }
    if ((mBackSideIscReduction < 0) || (mBackSideIscReduction > 1)){
               GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                           "mBackSideIscReduction must be between 0 and 1.0");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   isMinr      (--) whether it is a minor step
/// @param[in]   I           (amp) the present current
/// @param[in]   V           (V) the present voltage
/// @param[in]   sunAngle    (rad) the present sun angle in radians measured from 0 degree horizontal(Z- normal) to line of sight vector in. This is the red Theta in my LosAngleSketch.png helper picture. (B = 90 - A).
/// @param[in]   T           (K) the present Temperature
/// @param[in]   sunInt      (--) the present sun intensity ranges 0 to 1
/// @param[in]   backsideLit (--) whether the back of the array is illuminated by the sun.
/// @details  updates the cell based on new changing parameters
////////////////////////////////////////////////////////////////////////////////////////////////////
void PVCellCompanionModel::update(const bool isMinor, const double V, const double I, const double sunAngle, const double T, const double sunIntensity,const bool backSideLit)
{
    mV = V;
    mI = I;

    mSunAngle = sunAngle;
    mSunAngleFromEnv = sunAngle;
    mTemperature = T;
    mSunIntensity = sunIntensity;
    mIsBackSideLit = backSideLit;
    if (mSunIntensity < 0.0){ mSunIntensity = 0.0;}
    if (mSunIntensity > 1.0){ mSunIntensity = 1.0;}

    mSunAngle = fmod(mSunAngle, 6.283185307);
    // if (mSunAngle < 0.0)        {mSunAngle += 6.283185307;}             //2pi
    if (mSunAngle < 0.0) {
        mSunAngle = fabs(mSunAngle);
    }
    if (mSunAngle < 0.0)        { mSunAngle = 0.0;}                     //0 radians
    if (mSunAngle >  3.1415926535898){ mSunAngle =  3.1415926535898 ; } //pi radians

    if(!isMinor){
        updatePhysicalParameters();
    }

    updateCompanionModel();
    setupConductance();
    setupSourceVector();

    mVlast = mV;
    mIlast = mI;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  updates physical parameters of the cell which change with temperature, environment,
///           and load demand.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PVCellCompanionModel::updatePhysicalParameters()
 {

    //update Isc, source current using Isc temperature relation from reference 2. Isc changes by coefficient * deltaT.
    //Use sin because the angle supplied by environment is measured between LOS and horizontal. We want the perpendicular component of Isc
    double lAngleScalar = pow(sin(mSunAngle), mSunAngleScalar);
    double lTempScalar = mIscTempCoefficient * (mTemperature - mTemperatureRef);
    mIsc = (mIscRef + lTempScalar) * mSunIntensity * lAngleScalar * degrade();

    if(mIsBackSideLit){
        mIsc = mIsc * mBackSideIscReduction;
    }

    //update Voc, open circuit voltage using Voc temperature relation from reference 2. Voc changes by coefficient * deltaT
    mVoc = mVocRef + mVocTempCoefficient * (mTemperature - mTemperatureRef);
    //update the max power using the Isc and Voc temperature coefficients, to scale the values with temperature change.
    mImp = mImpRef + mIscTempCoefficient * (mTemperature - mTemperatureRef);
    mVmp = mVmpRef + mVocTempCoefficient * (mTemperature - mTemperatureRef);


    double mElectronCharge    = 1.6021764e-19; //Well known constant for charge of electron
    double mBoltzmannConstant = 1.3806488e-23; //Well known Boltzmann's constant
    //update Isat , saturation current
    if ((mBoltzmannConstant * mTemperature) > 0) {
        // a simplifying factor to help make the ideal diode equation Thermal Voltage easier to read.
        mLambda = mElectronCharge / (mBoltzmannConstant * mTemperature);
    } else {
        mLambda = 0.0;
    }
    if((exp(mVoc * mLambda) - 1) > 0){
    //This comes from the well known equation for an ideal Diode
    mIsat = mIsc / (exp(mVoc * mLambda) - 1);
    }else{
        mIsat = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  updates the values of this cell based on the representative circuit network
////////////////////////////////////////////////////////////////////////////////////////////////////
void PVCellCompanionModel::updateCompanionModel()
 {
    dampAndBoundIVCurve();
    double denominator = (1 + mIsat * mLambda * mRs * exp(mLambda * (mV + mI * mRs)));

    if (denominator > 0.0) {
        //Derived from implicit derivation of circuit network. See design review documents for details and derivation.
        mGeqCell = -(mIsat * mLambda * exp(mLambda * (mV + mI * mRs))) / denominator;
    } else {
        mGeqCell = 0.0;
    }
    //Ideal diode equation
    mId = mIsat * (exp(mLambda * (mV + mI * mRs)) - 1);
    mIl = mGeqCell * mV;
    //Derived from circuit analysis. See design review documents for details and derivation.
    mIeqCell = mIsc - mId - mIl;
    mGeqCell = fabs(mGeqCell);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  applies damping and ensures values stay within possible limits of the IV curve.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PVCellCompanionModel::dampAndBoundIVCurve()
{
    double filterI = mI;
    double filterV = mV;

    /// - mVCrit is used to switch between the old & new stability filters.  Zero uses the new
    ///   filter, >0 uses the old filter.  The new filter doesn't use this term.
    if (mVCrit > DBL_EPSILON) {

        /// - Original stability filter:  When the candidate voltage is greater than some critical
        ///   voltage limit, we must damp the possible candidate voltage so that our solution will
        ///   converge. This happens in the near vertical regions of the I-V curve. When near this
        ///   region, you must discard the candidate mV , and use the new mV shown in the log
        ///   equation below as your next pass mV.
        if (mV > mVCrit * mVoc) {

            if ((2.0 * mLambda) > 0) {
                //Derived from logarithmic damping scenario. See Logarithmic Damping derivation in design review documentation.
                double logVal = mV - mVlast + 1.0;
                if (logVal > 0) {
                    mV = (log(logVal) - mLambda * mRs * (mI - mIlast)) / (2.0 * mLambda) + mVlast;
                } else {
                    mV = (0.0 - mLambda * mRs * (mI - mIlast)) / (2.0 * mLambda) + mVlast;
                }
            } else {
                mV = mVlast;
            }
        }
    } else {

        /// - New stability filter:  When V > Vmp we're in the unstable "vertical" part of the I-V
        ///   curve.  Predict the point on the cell I-V curve in equilibrium with the vehicle load,
        ///   assuming the load is resistive.  Drive the cell towards that predicted point.  This
        ///   seems to perform better overall than the old filter, but we preserved the old filter
        ///   for backwards compatibility.
        if (mV > mVmp and mLambda > 0.0 and mIsat > 0.0) {

            /// - Assuming the vehicle load is resistive, then its conductance = mI/mV.  Find the
            ///   (V, I) where the cell and load I-V curves intersect.  There's no closed-form
            ///   solution so we have to iterate & solve numerically.
            double Gload = mI/mV;
            if ((mIsc - mVmp * Gload) > -mIsat) {
                filterI = mVmp * Gload;
                double delta     = 0.0;
                double lastDelta = 1.0e6;
                double lastV     = mVmp;
                for (int i=0; i<10; ++i) {
                    filterV = log(1.0 + (mIsc - filterI) / mIsat) / mLambda - filterI * mRs;

                    /// - Limit filterI to within valid bounds to prevent log of a negative number
                    //    on next pass.
                    filterI = MsMath::limitRange(0.0, filterV * Gload, mIsc);

                    /// - Detect a diverging oscillation about mVmp.  This can occur in some cases
                    ///   where the vehicle constant power load is higher than the solar array can
                    ///   provide.  In this case output max power and break out of the filter.
                    if ( (fabs(delta) > fabs(lastDelta)) and (filterV > mVmp) and (lastV < mVmp) ) {
                        filterV = mVmp;
                        filterI = filterV * Gload;
                        break;
                    }
                    lastDelta = delta;
                    delta = filterV - lastV;
                    lastV = filterV;
                }
            }
        }
        mV = filterV;
        mI = filterI;
    }

    //This if block is to stop very small negative current near oscillations around Voc.
    //The only way to get negative current is if we go past mVoc.
    if(mV >= mVoc){
        mV = mVoc;
        mI = 0.0;
    }

    if(mV < 0.0){
       mV = 0.0;
       mI = mIsc;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///@brief sets up conductance and modifies it based on any malfuctions
////////////////////////////////////////////////////////////////////////////////////////////////////
void PVCellCompanionModel::setupConductance()
{
    if (mCellPowerMalfunctionIsEnabled) {
        mGeqCell = 0.0;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
///@brief sets up source vector and modifies it based on any malfunctions
////////////////////////////////////////////////////////////////////////////////////////////////////
void PVCellCompanionModel::setupSourceVector()
{
    if (mCellPowerMalfunctionIsEnabled) {
        mIeqCell = 0.0;
    }

}
////////////////////////////////////////////////////////////////////////////////////////////////////
///@brief applies degrading effects to cell performance.
////////////////////////////////////////////////////////////////////////////////////////////////////
double PVCellCompanionModel::degrade()
{

    std::string mName = "PVCellCompanionModel";

    if ((mCellDegradation > mMaxDegradation) || (mCellDegradation < 0.0)) {
        mCellDegradation = 0.0;
        GUNNS_WARNING("The cell degradation is out of the range   [0.0 : mMaxDegradation]. This is not allowed, setting degradation to 0.0.");
    }

    if (mCellDegradationMalfunctionIsEnabled){
        if ((mCellDegradationOverrideValue >= 0.0) && (mCellDegradationOverrideValue <= mMaxDegradation)) {
         return mMaxDegradation - mCellDegradationOverrideValue;
        } else {
        mCellDegradationOverrideValue = 0.0;
        GUNNS_WARNING("The cell degradation override value is out of the range   [0.0 : mMaxDegradation]. This is not allowed, setting override value to 0.0");
        }
    }

    return mMaxDegradation - mCellDegradation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) Malfunction activation flag, true activates.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PVCellCompanionModel::setMalfCellPower(const bool flag)
{
    mCellPowerMalfunctionIsEnabled = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] value (--) Degrade malfunction amount.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void PVCellCompanionModel::setMalfCellDegrade(const bool flag, const double value)
{
    mCellDegradationMalfunctionIsEnabled = flag;
    mCellDegradationOverrideValue        = value;
}
