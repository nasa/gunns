/**
@file
@brief    GUNNS Electrical Short Utility implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ()
*/

#include "GunnsShortUtil.hh"
#include "math/Math.hh"
#include <cfloat>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfMinimumVoltage       (V)   Initial minimum voltage for all malfunction modes to flow.
/// @param[in] malfConstantPowerFlag    (--)  Initial constant power short malfunction activation flag.
/// @param[in] malfConstantPowerValue   (W)   Initial constant power short malfunction value.
/// @param[in] malfConstantCurrentFlag  (--)  Initial constant current short malfunction activation flag.
/// @param[in] malfConstantCurrentValue (amp) Initial constant current short malfunction value.
/// @param[in] malfInitialPowerFlag     (--)  Initial initial power short malfunction activation flag.
/// @param[in] malfInitialPowerValue    (W)   Initial initial power short malfunction value.
/// @param[in] malfInitialCurrentFlag   (--)  Initial initial current short malfunction activation flag.
/// @param[in] malfInitialCurrentValue  (amp) Initial initial current short malfunction value.
/// @param[in] malfResistanceFlag       (--)  Initial constant resistance short malfunction activation flag.
/// @param[in] malfResistanceValue      (ohm) Initial constant resistance short malfunction value.
///
/// @details  Default constructs this Electrical Short Utility input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsShortUtilInputData::GunnsShortUtilInputData(const double malfMinimumVoltage,
                                                 const bool   malfConstantPowerFlag,
                                                 const double malfConstantPowerValue,
                                                 const bool   malfConstantCurrentFlag,
                                                 const double malfConstantCurrentValue,
                                                 const bool   malfInitialPowerFlag,
                                                 const double malfInitialPowerValue,
                                                 const bool   malfInitialCurrentFlag,
                                                 const double malfInitialCurrentValue,
                                                 const bool   malfResistanceFlag,
                                                 const double malfResistanceValue)
    :
    mMalfMinimumVoltage(malfMinimumVoltage),
    mMalfConstantPowerFlag(malfConstantPowerFlag),
    mMalfConstantPowerValue(malfConstantPowerValue),
    mMalfConstantCurrentFlag(malfConstantCurrentFlag),
    mMalfConstantCurrentValue(malfConstantCurrentValue),
    mMalfInitialPowerFlag(malfInitialPowerFlag),
    mMalfInitialPowerValue(malfInitialPowerValue),
    mMalfInitialCurrentFlag(malfInitialCurrentFlag),
    mMalfInitialCurrentValue(malfInitialCurrentValue),
    mMalfResistanceFlag(malfResistanceFlag),
    mMalfResistanceValue(malfResistanceValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from.
///
/// @details  Copy constructs this Electrical Short Utility input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsShortUtilInputData::GunnsShortUtilInputData(const GunnsShortUtilInputData& that)
    :
    mMalfMinimumVoltage(that.mMalfMinimumVoltage),
    mMalfConstantPowerFlag(that.mMalfConstantPowerFlag),
    mMalfConstantPowerValue(that.mMalfConstantPowerValue),
    mMalfConstantCurrentFlag(that.mMalfConstantCurrentFlag),
    mMalfConstantCurrentValue(that.mMalfConstantCurrentValue),
    mMalfInitialPowerFlag(that.mMalfInitialPowerFlag),
    mMalfInitialPowerValue(that.mMalfInitialPowerValue),
    mMalfInitialCurrentFlag(that.mMalfInitialCurrentFlag),
    mMalfInitialCurrentValue(that.mMalfInitialCurrentValue),
    mMalfResistanceFlag(that.mMalfResistanceFlag),
    mMalfResistanceValue(that.mMalfResistanceValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Electrical Short Utility input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsShortUtilInputData::~GunnsShortUtilInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Electrical Short Utility object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsShortUtil::GunnsShortUtil()
    :
    mMalfMinimumVoltage(0.0),
    mMalfConstantPowerFlag(false),
    mMalfConstantPowerValue(0.0),
    mMalfConstantCurrentFlag(false),
    mMalfConstantCurrentValue(0.0),
    mMalfInitialPowerFlag(false),
    mMalfInitialPowerValue(0.0),
    mMalfInitialCurrentFlag(false),
    mMalfInitialCurrentValue(0.0),
    mMalfResistanceFlag(false),
    mMalfResistanceValue(0.0),
    mShortConductance(0.0),
    mPreviousCurrent(0.0),
    mPreviousPower(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Electrical Short Utility object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsShortUtil::~GunnsShortUtil()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--) Reference to the model input data.
///
/// @details  Initializes the Electrical Short Utility object with input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsShortUtil::initialize(const GunnsShortUtilInputData&  inputData)
{
    /// - Initialize class attributes.
    mMalfMinimumVoltage       = inputData.mMalfMinimumVoltage;
    mMalfConstantPowerFlag    = inputData.mMalfConstantPowerFlag;
    mMalfConstantPowerValue   = inputData.mMalfConstantPowerValue;
    mMalfConstantCurrentFlag  = inputData.mMalfConstantCurrentFlag;
    mMalfConstantCurrentValue = inputData.mMalfConstantCurrentValue;
    mMalfInitialPowerFlag     = inputData.mMalfInitialPowerFlag;
    mMalfInitialPowerValue    = inputData.mMalfInitialPowerValue;
    mMalfInitialCurrentFlag   = inputData.mMalfInitialCurrentFlag;
    mMalfInitialCurrentValue  = inputData.mMalfInitialCurrentValue;
    mMalfResistanceFlag       = inputData.mMalfResistanceFlag;
    mMalfResistanceValue      = inputData.mMalfResistanceValue;
    mShortConductance         = 0.0;
    mPreviousCurrent          = 0.0;
    mPreviousPower            = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage  (V)  Current voltage across the short.
///
/// @returns  double  (1/ohm)  Current conductance of the short.
///
/// @details  Computes and returns the electrical short conductance based on malfunction mode and
///           given voltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsShortUtil::computeShort(const double voltage)
{
    if (mMalfConstantPowerFlag) {
        /// - Conductance of the constant power short is P=IV, I=VG, G=P/V^2.
        mShortConductance = fabs(mMalfConstantPowerValue) / fmax(voltage*voltage, DBL_EPSILON);

    } else if (mMalfConstantCurrentFlag) {
        /// - Conductance of the constant current short is G=I/V.
        mShortConductance = fabs(mMalfConstantCurrentValue) / fmax(fabs(voltage), DBL_EPSILON);

    } else if (mMalfInitialPowerFlag) {
        /// - Conductance of the initial power short is only updated when the desired power
        ///   value changes.
        if (fabs(mMalfInitialPowerValue) != mPreviousPower) {
            mShortConductance = fabs(mMalfInitialPowerValue) / fmax(voltage*voltage, DBL_EPSILON);
        }

    } else if (mMalfInitialCurrentFlag) {
        /// - Conductance of the initial current short is only updated when the desired
        ///   current value changes.
        if (fabs(mMalfInitialCurrentValue) != mPreviousCurrent) {
            mShortConductance = fabs(mMalfInitialCurrentValue) / fmax(fabs(voltage), DBL_EPSILON);
        }

    } else if (mMalfResistanceFlag) {
        /// - Conductance of the resistance short is G=1/R.
        mShortConductance = 1.0 / fmax(mMalfResistanceValue, DBL_EPSILON);

    } else {
        /// - Reset the short conductance when all malfunctions are off.
        mShortConductance = 0.0;
    }

    /// - Update or reset initial power and current latching terms.
    if (mMalfInitialPowerFlag) {
        mPreviousPower = mMalfInitialPowerValue;
    } else {
        mPreviousPower = 0.0;
    }
    if (mMalfInitialCurrentFlag) {
        mPreviousCurrent = mMalfInitialCurrentValue;
    } else {
        mPreviousCurrent = 0.0;
    }

    return mShortConductance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag   (--)  Malfunction activation flag.
/// @param[in]  value  (W)   Malfunction activation value.
/// @param[in]  minV   (V)   Malfunction minimum voltage value.
///
/// @details  Sets the constant power malfunction and minimum voltage controls to the given values.
///           The default values clear the malfunction and zero the minimum voltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsShortUtil::setMalfConstantPower(const bool flag, const double value, const double minV)
{
    mMalfConstantPowerFlag  = flag;
    mMalfConstantPowerValue = value;
    mMalfMinimumVoltage     = minV;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag   (--)   Malfunction activation flag.
/// @param[in]  value  (amp)  Malfunction activation value.
/// @param[in]  minV   (V)    Malfunction minimum voltage value.
///
/// @details  Sets the constant current malfunction and minimum voltage controls to the given values.
///           The default values clear the malfunction and zero the minimum voltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsShortUtil::setMalfConstantCurrent(const bool flag, const double value, const double minV)
{
    mMalfConstantCurrentFlag  = flag;
    mMalfConstantCurrentValue = value;
    mMalfMinimumVoltage       = minV;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag   (--)  Malfunction activation flag.
/// @param[in]  value  (W)   Malfunction activation value.
/// @param[in]  minV   (V)   Malfunction minimum voltage value.
///
/// @details  Sets the initial power malfunction and minimum voltage controls to the given values.
///           The default values clear the malfunction and zero the minimum voltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsShortUtil::setMalfInitialPower(const bool flag, const double value, const double minV)
{
    mMalfInitialPowerFlag  = flag;
    mMalfInitialPowerValue = value;
    mMalfMinimumVoltage    = minV;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag   (--)   Malfunction activation flag.
/// @param[in]  value  (amp)  Malfunction activation value.
/// @param[in]  minV   (V)    Malfunction minimum voltage value.
///
/// @details  Sets the initial current malfunction and minimum voltage controls to the given values.
///           The default values clear the malfunction and zero the minimum voltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsShortUtil::setMalfInitialCurrent(const bool flag, const double value, const double minV)
{
    mMalfInitialCurrentFlag  = flag;
    mMalfInitialCurrentValue = value;
    mMalfMinimumVoltage      = minV;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag   (--)   Malfunction activation flag.
/// @param[in]  value  (ohm)  Malfunction activation value.
/// @param[in]  minV   (V)    Malfunction minimum voltage value.
///
/// @details  Sets the constant resistance malfunction and minimum voltage controls to the given values.
///           The default values clear the malfunction and zero the minimum voltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsShortUtil::setMalfResistance(const bool flag, const double value, const double minV)
{
    mMalfResistanceFlag  = flag;
    mMalfResistanceValue = value;
    mMalfMinimumVoltage  = minV;
}
