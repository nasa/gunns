/**
@file     GunnsElectPvString2.cpp
@brief    GUNNS Electrical Photovoltaic String Model version 2 implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  (
   (GunnsElectPvString.o)
   (software/exceptions/TsInitializationException.o)
   (software/exceptions/TsOutOfBoundsException.o)
   (math/UnitConversion.o)
   (math/elementary_functions/LambertW.o)
  )
*/

#include "math/MsMath.hh"
#include "GunnsElectPvString2.hh"
#include "core/GunnsBasicNode.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "math/UnitConversion.hh"
#include "math/elementary_functions/LambertW.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Photovoltaic Cell Version 2 Equivalent Circuit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvCellEquivCircuit2::GunnsElectPvCellEquivCircuit2()
    :
    GunnsElectPvCellEquivCircuit()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Cell Version 2 Equivalent Circuit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvCellEquivCircuit2::~GunnsElectPvCellEquivCircuit2()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) The cell configuration data.
/// @param[in] name       (--) Instance name for identification in error messages.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Photovoltaic Cell Version 2 Equivalent Circuit model with the cell
///           configuration data an the instance name.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvCellEquivCircuit2::initialize(const GunnsElectPvCellConfigData* configData, const std::string& name)
{
    mName = name;

    /// - Initialize from config data.
    mVoc         = configData->mOpenCircuitVoltage;
    mIsc         = configData->mShortCircuitCurrent;
    mVmp         = configData->mMppVoltage;
    mImp         = configData->mMppCurrent;
    mPhotoFlux   = configData->mPhotoFlux;
    mIdeality    = configData->mIdeality;
    mTemperature = configData->mRefTemperature;
    mCoeffDVocDT = configData->mTemperatureVoltageCoeff;
    mCoeffDIscDT = configData->mTemperatureCurrentCoeff;
    mSurfaceArea = configData->mSurfaceArea;

    /// - Compute and validate initial state.
    try {
        derive();
        computeEfficiency();
    } catch (TsOutOfBoundsException& e) {
        GUNNS_ERROR(TsInitializationException, "Re-throw",
                    "caught exception from derive().");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the efficiency and fill factor for the cell at current conditions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvCellEquivCircuit2::computeEfficiency()
{
    if (mSurfaceArea > 0.0 and mPhotoFlux > 0.0) {
        mEfficiency = mVmp * mImp / (mSurfaceArea * mPhotoFlux);
    } else {
        mEfficiency = 0.0;
    }

    if (mVoc > 0.0 and mIsc > 0.0) {
        mFillFactor = mVmp * mImp / (mVoc * mIsc);
    } else {
        mFillFactor = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] refCell     (--)   Pointer to the reference cell.
/// @param[in] temperature (K)    Cell temperature.
/// @param[in] photoFlux   (W/m2) Cell absorbed illumination power flux.
/// @param[in] degradation (--)   Fraction of cell degradation.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Computes the equivalent cell parameters as differing from the given reference cell
///           affected by the given temperature and illumination values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvCellEquivCircuit2::update(const GunnsElectPvCellEquivCircuit* refCell, const double temperature,
                                           const double photoFlux, const double degradation)
{
    /// - Start identical to the reference cell.
    static_cast<GunnsElectPvCellEquivCircuit&>(*this) = *refCell;

    /// - Store the given temperature and lighting conditions.  Protection against divide by zero is
    ///   provided by preventing zero reference cell photo flux in its initialization.  Cell
    ///   degradation is modeled as simply reducing the effective flux it can use.
    mTemperature = temperature;
    mPhotoFlux   = photoFlux;
    const double photoRatio = mPhotoFlux * (1.0 - degradation) / refCell->mPhotoFlux;
    if (photoRatio > DBL_EPSILON) {
        if (mTemperature == refCell->mTemperature) {
            /// - If we're at the reference temperature, cell only varies with illumination.
            ///   Currents increase with more light, resistances decrease, and all other parameters
            ///   are unchanged from teh reference cell.
            mIsc = refCell->mIsc * photoRatio;
            mImp = refCell->mImp * photoRatio;
            mRs  = refCell->mRs  / photoRatio;
            mRsh = refCell->mRsh / photoRatio;
            mI0  = refCell->mI0  * photoRatio;
            mIL  = refCell->mIL  * photoRatio;

        } else {
            /// - Away from the reference temperature, Voc and Isc are biased by their temperature
            ///   coefficients, Isc and Imp increase with more light as above, Vmp and Imp change
            ///   by the same ratios as Voc and Isc, and all other cell parameters are re-computed.
            const double dT = mTemperature - refCell->mTemperature;
            const double dVoc = dT * mCoeffDVocDT;
            const double dIsc = dT * mCoeffDIscDT;
            const double tempIsc = refCell->mIsc * photoRatio;
            mVoc = refCell->mVoc + dVoc;
            mIsc = tempIsc + dIsc;
            /// - Extreme temperatures can cause Voc or Isc to reach zero, in which case we zero out
            ///   the cell, as if it has no illumination.
            if (mVoc > DBL_EPSILON and mIsc > DBL_EPSILON) {
                const double tempImp = refCell->mImp * photoRatio;
                mVmp = refCell->mVmp * mVoc / refCell->mVoc;
                mImp = tempImp * mIsc / tempIsc;
                derive();
            } else {
                clear();
            }
        }
    } else {
        clear();
    }
    computeEfficiency();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsOutOfBoundsException
///
/// @details  Computes N*Vt, Rs, Rsh, I0, and IL, given Voc, Isc, Vmp, Imp, T, and ideality.  This
///           method to calculate Rs, Rsh, I0 and IL is from Reference 1.  Results and some
///           intermediate computations are checked for valid ranges and exception is thrown for
///           any out-of-bounds values to avoid later math faults.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvCellEquivCircuit2::derive()
{
    /// - Compute product of diode ideality and thermal voltage.
    mNVt = mIdeality * mBoltzmannOverCharge * mTemperature;

    /// - Compute and range check series resistance.
    const double A = mNVt / mImp;
    const double B = -mVmp * (2.0 * mImp - mIsc)
                   / (mVmp * mIsc + mVoc * (mImp - mIsc));
    const double C = -(2.0 * mVmp - mVoc) / mNVt
                   + (mVmp * mIsc - mVoc * mImp)
                   / (mVmp * mIsc + mVoc * (mImp - mIsc));
    const double D = (mVmp - mVoc) / mNVt;
    if (not MsMath::isInRange(-500.0, C, 500.0)) {
        GUNNS_ERROR(TsOutOfBoundsException, "Operand out of Range",
                    "operand to std::exp not within (-500, 500).");
    }
    const double z = B * std::exp(C);
    double W1z;
    try {
        W1z = LambertW::fastSolveW1(z);
    } catch (TsOutOfBoundsException& e) {
        GUNNS_ERROR(TsOutOfBoundsException, "Re-throw",
                    "caught exception from LambertW::fastSolveW1.");
    }
    mRs = A * (W1z - D - C);
    if (mRs <= 0.0) {
        /// - mRs tends to go negative for large ideality constant.
        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Result",
                    "computed series resistance <= 0.");
    }

    /// - Compute and range check shunt resistance.
    mRsh = (mVmp - mImp * mRs) * (mVmp - mRs * (mIsc - mImp) - mNVt)
         / ((mVmp - mImp * mRs) * (mIsc - mImp) - mNVt * mImp);
    if (mRsh <= 0.0) {
        /// - mRsh tends to go negative for large ideality constant.
        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Result",
                    "computed shunt resistance <= 0.");
    }

    /// - Compute and range check reverse saturation current.
    const double vexp = mVoc / mNVt;
    if (not MsMath::isInRange(-500.0, vexp, 500.0)) {
        GUNNS_ERROR(TsOutOfBoundsException, "Operand out of Range",
                    "operand to std::exp not within (-500, 500).");
    }
    mI0 = ((mRsh + mRs) * mIsc - mVoc) / (mRsh * std::exp(vexp));
    if (mI0 <= 0.0) {
        GUNNS_ERROR(TsOutOfBoundsException, "Failed Initialization",
                    "computed saturation current <= 0.");
    }

    /// - Compute and range check illumination source current.  Error checking for valid range isn't
    ///   needed since this can never be <= 0 because of previous error checks.
    mIL = (mRsh + mRs) * mIsc / mRsh;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage (V) Cell terminal voltage to compute the current for.
///
/// @returns  double (amp) Cell terminal current.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the current leaving the cell at its output terminal, given the cell output
///           terminal voltage, using the Lambert W+0 function.  For given voltages lying at or
///           below zero or at or above open-circuit voltage, return the short-circuit current and
///           zero current, respectively.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsElectPvCellEquivCircuit2::computeCurrent(const double voltage) const
{
    double current;
    if (voltage <= DBL_EPSILON) {
        current = mIsc;
    } else if (voltage < mVoc) {
        const double zexp = voltage / mNVt * (1.0 - mRs/(mRs + mRsh))
                          + (mI0 + mIL) * mRs / mNVt / (1.0 + mRs/mRsh);
        if (not MsMath::isInRange(-500.0, zexp, 500.0)) {
            GUNNS_ERROR(TsOutOfBoundsException, "Operand out of Range",
                        "operand to std::exp not within (-500, 500).");
        }
        const double z = mI0 * mRs / mNVt / (1.0 + mRs/mRsh) * std::exp(zexp);
        try {
            const double W0z = LambertW::fastSolveW0(z);
            current = (mI0 + mIL - voltage / mRsh) / (1.0 + mRs/mRsh) - W0z * mNVt / mRs;
        } catch (TsOutOfBoundsException& e) {
            GUNNS_ERROR(TsOutOfBoundsException, "Re-throw",
                        "caught exception from LambertW::fastSolveW0.");
        }
    } else {
        current = 0.0;
    }
    return current;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  current (amp) Cell terminal current to compute the voltage for.
///
/// @returns  double (V) Cell terminal voltage.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the voltage across the cell at its output terminal, given the cell output
///           terminal current, using the Lambert W+0 function.  For given currents lying at or
///           below zero or at or above the short-circuit current, return the open-circuit voltage
///           and zero voltage, respectively.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsElectPvCellEquivCircuit2::computeVoltage(const double current) const
{
    double voltage;
    if (current <= DBL_EPSILON) {
        voltage = mVoc;
    } else if (current < mIsc) {
        const double zexp = (mI0 + mIL - current) * mRsh / mNVt;
        if (not MsMath::isInRange(-500.0, zexp, 500.0)) {
            GUNNS_ERROR(TsOutOfBoundsException, "Operand out of Range",
                        "operand to std::exp not within (-500, 500).");
        }
        const double z = mI0 * mRsh / mNVt * std::exp(zexp);
        try {
            const double W0z = LambertW::fastSolveW0(z);
            voltage = mRsh * (mI0 + mIL) - current * (mRs + mRsh) - W0z * mNVt;
        } catch (TsOutOfBoundsException& e) {
            GUNNS_ERROR(TsOutOfBoundsException, "Re-throw",
                        "caught exception from LambertW::fastSolveW0.");
        }
    } else {
        voltage = 0.0;
    }
    return voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Photovoltaic String Model version 2.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvString2::GunnsElectPvString2()
    :
    GunnsElectPvString()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Pointer to the string config data.
/// @param[in] inputData  (--) Pointer to this string's input data.
///
/// @details  Default constructs this Photovoltaic String Model version 2 with pointers to its
///           configuration & input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvString2::GunnsElectPvString2(const GunnsElectPvStringConfigData* configData,
                                         const GunnsElectPvStringInputData*  inputData)
    :
    GunnsElectPvString(configData, inputData)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic String Model version 2.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvString2::~GunnsElectPvString2()
{
    TS_DELETE_OBJECT(mEqProps);
    TS_DELETE_OBJECT(mRefCell);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name (--) The instance name for H&S messages.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Photovoltaic String Model version 2 with its instance name and
///           validates its configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString2::initialize(const std::string& name)
{
    /// - Initialize the instance name and throw an exception if it's empty.
    mName = name;
    if (mName.empty()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "empty instance name.");
    }

    /// - Throw an exception on null pointer to config data.
    if (!mConfig) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mConfig is null pointer.");
    }

    /// - Throw an exception on null pointer to input data.
    if (!mInput) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mInput is null pointer.");
    }

    /// - Validate config & input data.
    validate();

    /// - Initialize and validate the reference cell and equivalent circuit properties.
    TS_NEW_CLASS_OBJECT_EXT(mRefCell, GunnsElectPvCellEquivCircuit2, (), mName + ".mRefCell");
    TS_NEW_CLASS_OBJECT_EXT(mEqProps, GunnsElectPvCellEquivCircuit2, (), mName + ".mEqProps");

    mRefCell->initialize(&mConfig->mCellConfig, mName + ".mRefCell");
    mEqProps->initialize(&mConfig->mCellConfig, mName + ".mEqProps");

    /// - Compute and validate the initial state.
    mEqProps->update(mRefCell, mInput->mTemperature, mInput->mPhotoFlux);
    mMpp.clear();
    mTerminal.clear();
    mNumBypassedGroups = 0;
    mNumActiveCells    = 0;
    mShunted           = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this Photovoltaic String Model configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString2::validate() const
{
    /// - Throw an exception on reference cell open-circuit voltage < DBL_EPSILON.
    if (mConfig->mCellConfig.mOpenCircuitVoltage < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "reference cell open-circuit voltage < DBL_EPSILON.");
    }

    /// - Throw an exception on reference cell short-circuit current < DBL_EPSILON.
    if (mConfig->mCellConfig.mShortCircuitCurrent < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "reference cell short-circuit current < DBL_EPSILON.");
    }

    /// - Throw an exception on reference cell maximum power point voltage < DBL_EPSILON.
    if (mConfig->mCellConfig.mMppVoltage < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "reference cell MPP voltage < DBL_EPSILON.");
    }

    /// - Throw an exception on reference cell maximum power point current < DBL_EPSILON.
    if (mConfig->mCellConfig.mMppCurrent < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "reference cell MPP current < DBL_EPSILON.");
    }

    /// - Throw an exception on reference cell absorbed photo flux < DBL_EPSILON.
    if (mConfig->mCellConfig.mPhotoFlux < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "reference cell absorbed photo flux < DBL_EPSILON.");
    }

    /// - Throw an exception on reference cell temperature < DBL_EPSILON.
    if (mConfig->mCellConfig.mRefTemperature < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "reference cell temperature < DBL_EPSILON.");
    }

    /// - Throw an exception on reference cell diode ideality not in range.
    if (not MsMath::isInRange(1.0, mConfig->mCellConfig.mIdeality, 1.5)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "reference cell diode ideality not in [1 - 1.5].");
    }

    /// - Throw an exception on reference cell surface area < 0.0.  Since cell area
    ///   is optional, we allow a value of zero, which disables the efficiency calculations.
    if (mConfig->mCellConfig.mSurfaceArea < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "reference cell surface area < 0.");
    }

    /// - Throw an exception on # cells < 1.
    if (mConfig->mNumCells < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "number of cells < 1.");
    }

    /// - Throw an exception on bypass diode interval < 1.
    if (mConfig->mBypassDiodeInterval < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "bypass diode interval < 1.");
    }

    /// - Throw an exception if bypass diode interval not evenly divisible in total # cells.
    if (0 != mConfig->mNumCells % mConfig->mBypassDiodeInterval) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "bypass diode interval not evenly divisible in number of cells.");
    }

    /// - Throw an exception on photo flux magnitude < 0.
    if (mInput->mPhotoFlux < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "initial photo flux magnitude < 0.");
    }

    /// - Throw an exception on source exposed fraction not in 0-1.
    if (not MsMath::isInRange(0.0, mInput->mSourceExposedFraction, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "initial source exposed fraction not in 0-1.");
    }

    /// - Throw an exception on temperature < 0.
    if (mInput->mTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "initial temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates this Photovoltaic String Model's state, including the equivalent circuit
///           properties, based on the current input conditions such as lighting and temperature.
///           These states do not depend on the current loading of the string.  Effects of loading
///           are calculated in other functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString2::update()
{
    /// - Number of active cells reduced by the bypassed groups.
    updateBypassedGroups();
    mNumActiveCells = mConfig->mNumCells - mNumBypassedGroups * mConfig->mBypassDiodeInterval;
    mSeriesVoltageDrop = mConfig->mBlockingDiodeVoltageDrop
                       + mNumBypassedGroups * mConfig->mBypassDiodeVoltageDrop;

    /// - Skip updating the model, and instead zero the string outputs, if there are no active
    ///   cells or if temperature is near zero.
    if (mNumActiveCells > 0 and mInput->mTemperature > 1.0) {
        if (mMalfDegradeFlag) {
            mEqProps->update(mRefCell, mInput->mTemperature, mInput->mPhotoFlux, MsMath::limitRange(0.0, mMalfDegradeValue, 1.0));
        } else {
            mEqProps->update(mRefCell, mInput->mTemperature, mInput->mPhotoFlux);
        }
        updateMpp();
        mShortCircuitCurrent = mEqProps->mIsc;
        mOpenCircuitVoltage  = std::max(0.0, mEqProps->mVoc * mNumActiveCells - mSeriesVoltageDrop);
    } else {
        mEqProps->clear();
        mMpp.clear();
        mShortCircuitCurrent = 0.0;
        mOpenCircuitVoltage  = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the Maximum Power Point of the string.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString2::updateMpp()
{
    mMpp.mVoltage = std::max(0.0, mEqProps->mVmp * mNumActiveCells - mSeriesVoltageDrop);
    mMpp.mCurrent = mEqProps->mImp;
    mMpp.mPower   = mMpp.mVoltage * mMpp.mCurrent;
    if (mMpp.mVoltage > 0.0) {
        mMpp.mConductance = mMpp.mCurrent / mMpp.mVoltage;
    } else {
        mMpp.mConductance = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  power      (W)   The output power load to apply.
/// @param[in]  shortSide  (--)  True uses the solution on the short-circuit side of maximum power.
///
/// @details  This loads the string at either the MPP, or zero power at Isc or Voc, depending on the
///           the given power output.  This version 2 string model does not have the ability to
///           load the string at arbitrary power loads, only at either MPP or zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString2::loadAtPower(const double power, const bool shortSide)
{
    if (power <= DBL_EPSILON) {
        mTerminal.mPower = 0.0;
        if (shortSide) {
            mTerminal.mVoltage     = 0.0;
            mTerminal.mCurrent     = mShortCircuitCurrent;
            mTerminal.mConductance = mShortCircuitCurrent / DBL_EPSILON;
        } else {
            mTerminal.mVoltage     = mOpenCircuitVoltage;
            mTerminal.mCurrent     = 0.0;
            mTerminal.mConductance = 0.0;
        }
    } else {
        loadAtMpp();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  v1  (V)  The terminal voltage to calculate outputs for.
///
/// @throws   TsOutOfBoundsException
///
/// @details  This loads the string at the given terminal voltage on its I-V curve and computes the
///           resulting terminal output state.  If the given terminal voltage exceeds the limits of
///           the I-V curve, then the terminal output current, power and conductance are zeroed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString2::loadAtVoltage(const double v1)
{
    mTerminal.mVoltage = v1;
    mTerminal.mCurrent = predictCurrentAtVoltage(v1);
    mTerminal.mPower = mTerminal.mVoltage * mTerminal.mCurrent;
    if (mTerminal.mVoltage > 0.0) {
        mTerminal.mConductance = mTerminal.mCurrent / mTerminal.mVoltage;
    } else {
        mTerminal.mConductance = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  g  (1/ohm)  The load conductance to compute the results for.
///
/// @throws   TsOutOfBoundsException
///
/// @details  This loads the string near either the open-circuit or short-circuit points, depending
///           on which side of the MPP the given conductance represents.  The version 2 equivalent
///           cell model has limited ability to load at arbitrary current or power, so we can't load
///           at arbitrary conductance either.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvString2::loadAtConductance(const double g)
{
    if (g < mMpp.mConductance) {
        /// - On the open-circuit side, load at 99.99% of Voc.
        loadAtVoltage(0.9999 * mOpenCircuitVoltage);
    } else {
        /// - If on the short-circuit side or at MPP, load at 0.01% of Voc.
        loadAtVoltage(0.0001 * mOpenCircuitVoltage);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  v1  (V)  The terminal voltage to calculate outputs for.
///
/// @returns  double (amp) String current at the given voltage.
///
/// @throws   TsOutOfBoundsException
///
/// @details This computes the string current at the given string voltage.  This doesn't store the
///          result or actually load the string.  Voltage is given for the entire string, but we can
///          only compute current for an equivalent cell, so we work out what the individual cell
///          voltage is by adding the series diode voltage drops and dividing by the number of
///          active cells.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsElectPvString2::predictCurrentAtVoltage(const double v1) const
{
    double current = 0.0;

    if (mNumActiveCells > 0) {
        const double cellVoltage = (v1 + mSeriesVoltageDrop) / mNumActiveCells;
        current = mEqProps->computeCurrent(cellVoltage);
    }
    return current;
}
