/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the GUNNS Fluid Leak link model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o))

PROGRAMMERS:
   ((Kenneth McMurtrie) (Tietronix Software) (Add) (2011-10))
**************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "GunnsFluidLeak.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                 --     Name of object.
/// @param[in]  nodes                --     Pointer to nodes.
/// @param[in]  maxConductivity      (m2)   Max conductivity.
/// @param[in]  expansionScaleFactor --     Scale factor for isentropic gas cooling.
///
/// @details  Default constructs this GUNNS Fluid Leak link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLeakConfigData::GunnsFluidLeakConfigData(const std::string& name,
                                                   GunnsNodeList*     nodes,
                                                   const double       maxConductivity,
                                                   const double       expansionScaleFactor)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   --  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Leak link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLeakConfigData::GunnsFluidLeakConfigData(const GunnsFluidLeakConfigData& that)
    :
    GunnsFluidConductorConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Leak link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLeakConfigData::~GunnsFluidLeakConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag   --      Blockage malfunction flag.
/// @param[in]  malfBlockageValue  --      Blockage malfunction fractional value (0-1).
/// @param[in]  malfLeakHoleFlag   --      Leak effective hole area malfunction flag.
/// @param[in]  malfLeakHoleValue  (m2)    Leak effective hole area malfunction value.
/// @param[in]  malfLeakRateFlag   --      Leak rate malfunction flag.
/// @param[in]  malfLeakRateValue  (kg/s)  Leak rate malfunction value.
///
/// @details  Default constructs this GUNNS Fluid Leak link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLeakInputData::GunnsFluidLeakInputData(const bool   malfBlockageFlag,
                                                 const double malfBlockageValue,
                                                 const bool   malfLeakHoleFlag,
                                                 const double malfLeakHoleValue,
                                                 const bool   malfLeakRateFlag,
                                                 const double malfLeakRateValue)
    :
                                 //TODO should we force blockage off in this class?  If so, then we
                                 //should force it off at all times, not just input data?
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mMalfLeakHoleFlag(malfLeakHoleFlag),
    mMalfLeakHoleValue(malfLeakHoleValue),
    mMalfLeakRateFlag(malfLeakRateFlag),
    mMalfLeakRateValue(malfLeakRateValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   --  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Leak link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLeakInputData::GunnsFluidLeakInputData(const GunnsFluidLeakInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mMalfLeakHoleFlag(that.mMalfLeakHoleFlag),
    mMalfLeakHoleValue(that.mMalfLeakHoleValue),
    mMalfLeakRateFlag(that.mMalfLeakRateFlag),
    mMalfLeakRateValue(that.mMalfLeakRateValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Leak link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLeakInputData::~GunnsFluidLeakInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Leak link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLeak::GunnsFluidLeak()
    :
    GunnsFluidConductor(),
    mMalfLeakHoleFlag(false),
    mMalfLeakHoleValue(0.0),
    mMalfLeakRateFlag(false),
    mMalfLeakRateValue(0.0),
    mPreviousLeakHole(0.0),
    mPreviousLeakRate(0.0),
    mLeakConductivity(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Leak link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLeak::~GunnsFluidLeak()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   --  Configuration data.
/// @param[in]      inputData    --  Input data.
/// @param[in,out]  links        --  Link vector.
/// @param[in]      port0        --  Nominal inlet port map index.
/// @param[in]      port1        --  Nominal outlet port map index.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Leak link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLeak::initialize(const GunnsFluidLeakConfigData& configData,
                                const GunnsFluidLeakInputData&  inputData,
                                std::vector<GunnsBasicLink*>&   links,
                                const int                       port0,
                                const int                       port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Initialize with input data.
    mMalfLeakHoleFlag  = inputData.mMalfLeakHoleFlag;
    mMalfLeakHoleValue = inputData.mMalfLeakHoleValue;
    mMalfLeakRateFlag  = inputData.mMalfLeakRateFlag;
    mMalfLeakRateValue = inputData.mMalfLeakRateValue;
    mPreviousLeakHole  = 0.0;
    mPreviousLeakRate  = 0.0;
    mLeakConductivity  = 0.0;

    /// - Validate configuration and input data.
    validate();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag          = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Leak link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLeak::validate() const
{
    /// - Throw an exception if malfunction effective hole area value is non-positive.
    if (mMalfLeakHoleValue < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Malfunction leak hole area < 0.");
    }

    /// - Throw an exception if malfunction leak rate value is non-positive.
    if (mMalfLeakRateValue < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Malfunction leak rate < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLeak::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s)  Time step (not used).
///
/// @return   void
///
/// @details  Processes any malfunctions and updates this GUNNS Fluid Leak link model conductivity.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidLeak::updateState(const double dt __attribute__((unused)))
{
    if (mMalfLeakHoleFlag) {
        /// - For leak effective hole area malfunction, set effective conductivity to the effective
        ///   hole area.
        if (mPreviousLeakHole != mMalfLeakHoleValue) {
            mLeakConductivity  = mMalfLeakHoleValue;
            mPreviousLeakHole  = mMalfLeakHoleValue;
        }
        mEffectiveConductivity = mLeakConductivity;
    } else if (mMalfLeakRateFlag) {
        /// - For leak rate malfunction, compute leak conductivity once on malfunction activation
        ///   and set effective conductivity.
        if (mPreviousLeakRate != mMalfLeakRateValue) {
            updateLeakConductance();
            mPreviousLeakRate  = mMalfLeakRateValue;
        }
        mEffectiveConductivity = mLeakConductivity;
    } else {
        /// - For no active malfunctions, zero out effective conductivity.
        mPreviousLeakRate      = 0.0;
        mPreviousLeakHole      = 0.0;
        mLeakConductivity      = 0.0;
        mEffectiveConductivity = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
/// @param[in] value (m2) Leak effective hole area malfunction value
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLeak::setMalfLeakHole(const bool flag, const double value)
{
    mMalfLeakHoleFlag  = flag;
    mMalfLeakHoleValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--)   Malfunction activation flag, true activates
/// @param[in] value (kg/s) Leak initial rate malfunction value
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLeak::setMalfLeakRate(const bool flag, const double value)
{
    mMalfLeakRateFlag  = flag;
    mMalfLeakRateValue = value;
}
