/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
    (aspects/electrical/SolarArray/SolarString.o)
    (simulation/hs/TsHsMngr.o)
    (software/exceptions/TsInitializationException.o)
    )


PROGRAMMERS:
    (
    ((Nicholas Kaufmann) (L-3 Comm) (Oct 2013) (TS21) (Initial Version))
    )
 **/

#include "aspects/electrical/SolarArray/SolarSection.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsBasicNode.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    SolarSection Config Data
/// @param[in] numStrings        (--) the number of strings for this section
/// @param[in] stringConfigData  (--) string configuration data
/// @details  Constructs the SolarSection Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarSectionConfigData::SolarSectionConfigData(const int numStrings,
                                               const SolarStringConfigData& stringConfigData) :
    mNumStrings(numStrings),
    mSolarStringConfigData(stringConfigData)
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarSectionConfigData::SolarSectionConfigData(const SolarSectionConfigData& that):

    mNumStrings(that.mNumStrings),
    mSolarStringConfigData(that.mSolarStringConfigData)
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SolarSection Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarSectionConfigData::~SolarSectionConfigData()
{
    //Nothing to do
}

//////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] isShadowed     (--)  section not in sunlight if true
/// @param[in] stringInputData  (--) string input data
/// @details  Constructs the SolarSection Input data
//////////////////////////////////////////////////////////////////////////////////////////////
SolarSectionInputData::SolarSectionInputData(const bool isShadowed,
                                             const SolarStringInputData& stringInputData) :

    mIsShadowed(isShadowed),
    mSolarStringInputData(stringInputData)
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarSectionInputData::SolarSectionInputData(const SolarSectionInputData& that):

    mIsShadowed(that.mIsShadowed),
    mSolarStringInputData(that.mSolarStringInputData)
{
   //Nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SolarSection Input Data Object
///////////////////////////////////////////////////////////////////////////////////////////////
SolarSectionInputData::~SolarSectionInputData()
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Creates a default object. Constructor for class
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarSection::SolarSection()
:
mStrings(0),
mSolarPowerOnSection(0.0),
mSunAngleFromHorizontal(1.57),
mSunIntensity(1.0),
mNumStrings(0),
mTemperature(300.0),
mLocalSunAngleScalar(1.0),
mLocalSunAngleScalarFlag(false),
mZPlusLOSIsClear(false),
mZMinusLOSIsClear(true),
mFakeOrbitFlag(false),
mFakeSunIntensity(1.0),
mFakeSunAngle(1.57),
mFakeIsBackLit(false),
mIsShadowed(false),
mIsBackLit(false),
mNumFailedStrings(0),
mNumFailedCells(0),
mGeq(0),
mIeq(0),
mVoltage(0),
mCurrent(0)
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default class destructore
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarSection::~SolarSection()
{
    cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   cd   (--) Reference to section Config Data
/// @param[in]   id   (--) Reference to section Input Data
/// @throws   TsInitializationException
/// @details  Initializes the object with configuration and input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarSection::initialize(const SolarSectionConfigData& cd, const SolarSectionInputData& id)
{

   //populate class attributes with passed in config data
   mNumStrings             = cd.mNumStrings;

   //populate class attributs with passed in input data
   mIsShadowed              = id.mIsShadowed;

   //validates the passed in initialization data.
   validate();

   //allocates the array of strings
   allocateArrays();

  //initialize the newly allocated array
   for (int str = 0 ; str < mNumStrings; str++){
       mStrings[str].initialize(cd.mSolarStringConfigData,id.mSolarStringInputData);
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
/// @details  Validates the input and configuration data that was passed in through initialize.
///////////////////////////////////////////////////////////////////////////////////////////////////
void SolarSection::validate()
{

    std::string mName = "SolarString";

    //Should have a positive value drop
    if (mNumStrings < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "mNumStrings < 1 but must be greater.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   isMinor          (--) Whether this update is occuring during a gunns minor step
/// @param[in]   sectionCurrent   (amp) The total current for this section
/// @param[in]   sectionVoltage   (V)   The total voltage for this section
/// @details  updates the section with the latest phsyical parameters and load requirements
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarSection::update(const bool isMinor, const double sectionCurrent, const double sectionVoltage)
{

    // put a lower bound on sun intensity
    // mSunIntensity = std::max(0.0001, mSunIntensity);

    //The idea is we have to figure out how much voltage and current go to a single string, and to a single cell.
    // Some strings can be failed, therefore not operating.

    mCurrent = sectionCurrent;
    mVoltage = sectionVoltage;

    //Zero out values
    double stringCurrent = 0.0;
    mNumFailedStrings    = 0;
    mNumFailedCells      = 0;
    mGeq                 = 0.0;
    mIeq                 = 0.0;

    //If both sides blocked, then the section is shadowed
    if (!mZPlusLOSIsClear && !mZMinusLOSIsClear)
    {
        mIsShadowed = true;
    }else
    {
        mIsShadowed = false;
    }

    //This code is based on the reference using Z+ normal to the back of the array and the Z- vector normal to the front front conducting side.    }
    //If the back of the array is not blocked but the conducting side is, then it is backlit
    //    if (!mZPlusLOSIsClear && mZMinusLOSIsClear) {
    if (mZPlusLOSIsClear && !mZMinusLOSIsClear) {
        mIsBackLit = true;
    } else {
        mIsBackLit = false;
    }

    //Loop over all the strings
    for (int str = 0; str < mNumStrings; str++){
        //Build up num failed strings and cells
        mNumFailedStrings = mNumFailedStrings + static_cast<int>(mStrings[str].getIsFailed());
        mNumFailedCells   = mNumFailedCells + mStrings[str].getNumFailedCells();

        //Calculate the string current for next pass
        stringCurrent= -(sectionVoltage*mStrings[str].getConductanceStamp() -  mStrings[str].getSourceVectorStamp());

        //Update string with next pass values
        if (mFakeOrbitFlag) {
            mStrings[str].update(isMinor,stringCurrent,sectionVoltage,mFakeSunAngle, mTemperature, mFakeSunIntensity, mFakeIsBackLit);
        } else {
            mStrings[str].update(isMinor,stringCurrent,sectionVoltage,mSunAngleFromHorizontal,mTemperature,mSunIntensity,mIsBackLit);
        }

        //Build up admittance and source vector
        mGeq = mGeq + mStrings[str].getConductanceStamp();
        mIeq = mIeq + mStrings[str].getSourceVectorStamp();
     }

    //Finally, account for shadowing.
    mGeq = mGeq * static_cast<double>(!mIsShadowed);
    mIeq = mIeq * static_cast<double>(!mIsShadowed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   cd   (--) Reference to string Config Data
/// @param[in]   id   (--) Reference to string Input Data
/// @details  Allocates the arrays based on the number of sections
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarSection::allocateArrays()
{

    TS_NEW_CLASS_ARRAY_EXT(mStrings, mNumStrings, SolarString, ());

}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes allocated memory objects
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarSection::cleanup()
{
   TS_DELETE_ARRAY(mStrings);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details for unit testing, function to allow the changing of the amount of sunlight
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarSection::setSunIntensity(const double intensity) {
        mSunIntensity = intensity;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
///@details returns the open circuit voltage of a cell in this section
///@returns (V) the open circuit voltage of a cell in this section
////////////////////////////////////////////////////////////////////////////////////////////////////
double SolarSection::getRefCellVoc(){
  double highestVoc = 0.0;
  double voc = 0.0;
  for (int i = 0; i < mNumStrings; i++){
     voc = mStrings[i].getRefCellVoc();
     if(voc > highestVoc){
         highestVoc = voc;
     }
  }

  return highestVoc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///@details returns the the source current of string 0 on this section
///@returns (amp) the source current of string 0 on this section
////////////////////////////////////////////////////////////////////////////////////////////////////
double SolarSection::getStringIsc(){
    double highestIsc = 0.0;
    double isc = 0.0;
    for ( int i = 0; i < mNumStrings; i++){
        isc = mStrings[i].getIsc();
        if(isc > highestIsc){
            highestIsc = isc;
        }
    }
    return highestIsc;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///@details returns the the source current of string 0 on this section
///@returns (amp) the source current of string 0 on this section
////////////////////////////////////////////////////////////////////////////////////////////////////
double SolarSection::getStringIscRef(){
    double highestIscRef = 0.0;
    double iscRef = 0.0;
    for ( int i = 0; i < mNumStrings; i++){
        iscRef = mStrings[i].getIscRef();
        if(iscRef > highestIscRef){
            highestIscRef = iscRef;
        }
    }
    return highestIscRef;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details set the orbital parameters when they are being overridden in the solar array model
/// @param[in]  flag       bool     Flag for whether the override values are used or value from Environment
/// @param[in]  angle      double   overridden sun angle from horizontal
/// @param[in]  intensity  double   overridden sun intensity
/// @param[in]  backlit    bool     flag for whether the array is backlit, since this comes from environment
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarSection::setFakeOrbitParms(const bool flag, const double angle, const double intensity, const bool backlit) {
   mFakeOrbitFlag = flag;
   mFakeSunAngle  = angle;
   mFakeSunIntensity = intensity;
   mFakeIsBackLit = backlit;

   if (mFakeSunIntensity < 0.1)  {
       mZPlusLOSIsClear = false;
       mZMinusLOSIsClear = false;
   } else {
       mZPlusLOSIsClear = false;
       mZMinusLOSIsClear = true;
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details set the fake orbit flag
/// @param[in]  value       bool     Flag for whether the override values are used or value from Environment
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarSection::setFakeOrbitFlag(const bool value) {
    mFakeOrbitFlag = value;
}
