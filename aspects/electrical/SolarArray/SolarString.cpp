/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
    (software/exceptions/TsInitializationException.o)
    (simulation/hs/TsHsMngr.o)
    (aspects/electrical/SolarArray/PVCellCompanionModel.o)
    )


PROGRAMMERS:
    (
    ((Nicholas Kaufmann) (L-3 Comm) (Oct 2013) (TS21) (Initial Version))
    )
 **/

#include "SolarString.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsBasicNode.hh"
#include <math.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    SolarString Config Data
/// @param[in] numCells                  (--) number of cells on this string
/// @param[in] blockingDiodeVoltageDrop  (V) the voltage drop across diode at end of string
/// @param[in] bipassDiodeVoltageDrop    (V) the voltage drop across diode every N cells
/// @param[in] bipassDiodeInterval       (--) the number of cells between each bipass diode
/// @param[in] refCellConfigData         (--) photocell configuration data
/// @details  Constructs the SolarString Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarStringConfigData::SolarStringConfigData(const int numCells,
                                             const double blockingDiodeVoltageDrop,
                                             const double bipassDiodeVoltageDrop,
                                             const int bipassDiodeInterval,
                                             const PVCellCompanionModelConfigData& refCellConfigData)
     :
    mNumCells(numCells),
    mBlockingDiodeVoltageDrop(blockingDiodeVoltageDrop),
    mBipassDiodeVoltageDrop(bipassDiodeVoltageDrop),
    mBipassDiodeInterval(bipassDiodeInterval),
    mRefCellConfigData(refCellConfigData)
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarStringConfigData::SolarStringConfigData(const SolarStringConfigData& that):

    mNumCells(that.mNumCells),
    mBlockingDiodeVoltageDrop(that.mBlockingDiodeVoltageDrop),
    mBipassDiodeVoltageDrop(that.mBipassDiodeVoltageDrop),
    mBipassDiodeInterval(that.mBipassDiodeInterval),
    mRefCellConfigData(that.mRefCellConfigData)
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SolarString Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarStringConfigData::~SolarStringConfigData()
{
    //Nothing to do
}

//////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   isFailed         (--)  flag to faile the string if true
/// @param[in]   numFailedCells   (--)  the total number of failed cells on this string
/// @param[in]   refCellInputData (--) reference cell input data
/// @details  Constructs the SolarString Input data
//////////////////////////////////////////////////////////////////////////////////////////////
SolarStringInputData::SolarStringInputData(const bool  isFailed,
                                           const int numFailedCells,
                                           const PVCellCompanionModelInputData& refCellInputData)
    :
    mIsFailed(isFailed),
    mNumFailedCells(numFailedCells),
    mRefCellInputData(refCellInputData)
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarStringInputData::SolarStringInputData(const SolarStringInputData& that):

    mIsFailed(that.mIsFailed),
    mNumFailedCells(that.mNumFailedCells),
    mRefCellInputData(that.mRefCellInputData)
{
    //Nothing to do
}


///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SolarString Input Data Object
///////////////////////////////////////////////////////////////////////////////////////////////
SolarStringInputData::~SolarStringInputData()
{
    //Nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the SolarString
///////////////////////////////////////////////////////////////////////////////////////////////////
SolarString::SolarString()
    :
          mRefCell(),
          mMalfFailString(false),
          mLocalSunAngleScalar(1.0),
          mLocalSunAngleScalarFlag(false),
          mBipassDiodeVoltageDrop(0),
          mBipassDiodeInterval(0),
          mBlockingDiodeVoltageDrop(0),
          mNumCells(0),
          mNumFailedCells(0),
          mVoltage(0),
          mCurrent(0),
          mGeq(0),
          mIeq(0)
{
    //Nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SolarString
///////////////////////////////////////////////////////////////////////////////////////////////////
SolarString::~SolarString()
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   cd   (--) Reference to Link Config Data
/// @param[in]   id   (--) Reference to Link Input Data
/// @throws   TsInitializationException
/// @details  Initializes the object with configuration and input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarString::initialize(const SolarStringConfigData &cd,
                             const SolarStringInputData &id)
{
    /// - populate the class attributes with config and input data.
    mNumCells                 = cd.mNumCells;
    mBlockingDiodeVoltageDrop = cd.mBlockingDiodeVoltageDrop;
    mBipassDiodeVoltageDrop   = cd.mBipassDiodeVoltageDrop;
    mBipassDiodeInterval      = cd.mBipassDiodeInterval;

    mMalfFailString           = id.mIsFailed;
    mNumFailedCells           = id.mNumFailedCells;

    validate();

    //Initialize reference cell
    mRefCell.initialize(cd.mRefCellConfigData,id.mRefCellInputData);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   isMinor       (--) Whether this update is occurring during a gunns minor step
/// @param[in]   stringCurrent (amp) The total current for this string
/// @param[in]   stringVoltage (V)   The total voltage for this string
/// @param[in]   sunAngle;     (r) the angle sun rays make with 0 degree horizontal and the z+ line of site vector. Read from ENV. Value passed through solar section.
/// @param[in]   temperature   (K)  the cell temperature. Read from THERMAL subsystem. Value passed through solar section.
/// @param[in]   sunIntensity  (--) the intensity of the sun as felt by this cell
/// @param[in]   backSideLit   (--) bool true when back side of array is fully lit
/// @details  Calculates a single cell voltage  string voltage based on the present ref cell voltage.
///////////////////////////////////////////////////////////////////////////////////////////////////
void SolarString::update(const bool isMinor,
                         const double stringCurrent,
                         const double stringVoltage,
                         const double sunAngle,
                         const double temperature,
                         const double sunIntensity,
                         const bool   backSideLit)
{

    mVoltage = stringVoltage;
    mCurrent = stringCurrent;

    double vRefCell = 0.0;
    double iRefCell = mCurrent;

    //Dont let there be more failed cells than possible. Assume a single failed cell fails a whole group.
    double actCells = mNumCells - mNumFailedCells * mBipassDiodeInterval;
    if((mNumFailedCells >= mNumCells) || (actCells <= 0)) {
        mNumFailedCells = mNumCells;
    }

    if ((mBipassDiodeInterval > 0) && (mNumCells != mNumFailedCells) && (mNumCells > 0)) {
        vRefCell = (mVoltage +
                  (double)mBlockingDiodeVoltageDrop +
                   (mBipassDiodeVoltageDrop * mNumFailedCells))/actCells;
    } else {
        vRefCell = 0;
    }

    mRefCell.update(isMinor, vRefCell, iRefCell, sunAngle, temperature, sunIntensity,backSideLit);

    //GeqString = GeqCell / NumActiveCells
    if ((mNumCells != mNumFailedCells) && (mNumCells > 0)) {
        mGeq = mRefCell.getConductanceStamp() / static_cast<double>(mNumCells - mNumFailedCells);
    } else {
        mGeq =  0.0;
    }

    mIeq = mRefCell.getSourceVectorStamp();


    //Account for whether it is failed or not.
    mGeq = mGeq * !mMalfFailString;
    mIeq = mIeq * !mMalfFailString;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
/// @details  Validates the input and configuration data that was passed in through initialize.
///////////////////////////////////////////////////////////////////////////////////////////////////
void SolarString::validate()
{

    std::string mName = "SolarString";

    //Should have a positive value drop
    if (mBlockingDiodeVoltageDrop < 0){
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mBlockingDiodeVoltageDrop < 0 but must be greater.");
    }

    //Should have a positive value drop
    if (mBipassDiodeVoltageDrop < 0){
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mBipassDiodeVoltageDrop < 0 but must be greater.");
    }

    //Should have a positive value
    if (mNumCells < 1){
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mNumCells < 1 but must be greater.");
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) Malfunction activation flag, true activates.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarString::setMalfFailString(const bool flag)
{
    mMalfFailString = flag;
}
