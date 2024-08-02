/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (core/GunnsBasicLink.o)
     (simulation/hs/TsHsMsg.o)
     (software/exceptions/TsInitializationException.o)
     (aspects/electrical/SolarArray/SolarSection.o)
    )
PROGRAMMERS
 :
    (
    (Nicholas Kaufmann) (L-3 Comm) (9/2013) (Initial)
    )
 **/

#include "aspects/electrical/SolarArray/SolarArray.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"
#include <string.h>
#include <cmath>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    SolarArray Config Data
/// @param[in] name                  (--) Link name
/// @param[in] nodes                 (--) Network nodes array
/// @param[in] numSections           (--) Number of sections the array is split into
/// @param[in] numString             (--) the number of strings per section
/// @param[in] numCells;                   (--) number of cells that this solar string has
/// @param[in]  blockingDiodeVoltageDrop; (V) the voltage drop across the diode at end of string
/// @param[in]  bipassDiodeVoltageDrop;   (V) the voltage drop across each bipass diode.
/// @param[in]  bipassDiodeInterval;         (--) the number of cells per ever one bipass diode
/// @param[in]  vocRef;                   (V) the open circuit voltage reference value.
/// @param[in]  vmpRef;                   (V) the voltage max power reference value.
/// @param[in]  vocTempCoefficient;       (--) the open circuit voltage coefficient. see page (XYZ) of (XYZ) document
/// @param[in]  iscRef;                   (amp) the source current reference value.
/// @param[in]  impRef;                   (amp) the max power current reference value.
/// @param[in]  iscTempCoefficient;       (--) the source current temperature coefficient. see page (XYZ) of (XYZ) document
/// @param[in]  isat;                     (amp) the saturation current of the cell. aka "Dark Current"
/// @param[in]  temperatureRef;           (K)  the temperature at which the reference values were recorded.
/// @param[in]  cellDegradation;          (--) the percent amount to degrade cell by. This accounts for age, damage, etc
/// @param[in]  rs;                       (ohm) the cell series resistance
/// @param[in]  rsh;                      (ohm) the cell shunt resistance
/// @param[in]  vCrit;                    (--) a value multiplier for voc, after Vcrit*mVoc, model  apply damping to dV
/// @param[in]  backSideIscReduction;     (--) The percentage Isc drops by when the cell is back lit onl. Ranges from 0 to 1
/// @details  Constructs the SolarArray Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarArrayConfigData::SolarArrayConfigData(const std::string name,
        GunnsNodeList *nodes,
        const int numSections                 ,
        const int numStrings                  ,
        const int numCells                    ,
        const double blockingDiodeVoltageDrop ,
        const double bipassDiodeVoltageDrop   ,
        const int bipassDiodeInterval         ,
        const double vocRef                   ,
        const double vmpRef                   ,
        const double vocTempCoeff             ,
        const double iscRef                   ,
        const double impRef                   ,
        const double iscTempCoeff             ,
        const double iSat                     ,
        const double temperatureRef           ,
        const double cellDegradation          ,
        const double seriesResistance         ,
        const double shuntResistance          ,
        const double vCrit                    ,
        const double backSideIscReduction     ,
        const double cellEfficiency           ,
        const double cellArea)
     :
    GunnsBasicLinkConfigData(name, nodes),
    mNumSections(numSections),
    mSectionArea(0.0),
    mNumStrings(numStrings),
    mNumCells(numCells),
    mBlockingDiodeVoltageDrop(blockingDiodeVoltageDrop),
    mBipassDiodeVoltageDrop(bipassDiodeVoltageDrop),
    mBipassDiodeInterval(bipassDiodeInterval),
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
    mBackSideIscReduction(backSideIscReduction),
    mCellEfficiency(cellEfficiency),
    mCellArea(cellArea),
    mSectionConfigData(SolarSectionConfigData(mNumStrings, SolarStringConfigData(mNumCells,
                                                              mBlockingDiodeVoltageDrop ,
                                                              mBipassDiodeVoltageDrop ,
                                                              mBipassDiodeInterval ,
                                                              PVCellCompanionModelConfigData(mVocRef ,
                                                                                             mVmpRef ,
                                                                                             mVocTempCoefficient ,
                                                                                             mIscRef ,
                                                                                             mImpRef ,
                                                                                             mIscTempCoefficient ,
                                                                                             mIsat ,
                                                                                             mTemperatureRef ,
                                                                                             mCellDegradation,
                                                                                             mRs,
                                                                                             mRsh,
                                                                                             mVCrit ,
                                                                                             mBackSideIscReduction,
                                                                                             mCellEfficiency,
                                                                                             mCellArea))))
{
     //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarArrayConfigData::SolarArrayConfigData(const SolarArrayConfigData& that):

    GunnsBasicLinkConfigData(that.mName,that.mNodeList),
    mNumSections(that.mNumSections),
    mSectionArea(that.mSectionArea),
    mNumStrings(that.mNumStrings),
    mNumCells(that.mNumCells),
    mBlockingDiodeVoltageDrop(that.mBlockingDiodeVoltageDrop),
    mBipassDiodeVoltageDrop(that.mBipassDiodeVoltageDrop),
    mBipassDiodeInterval(that.mBipassDiodeInterval),
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
    mCellArea(that.mCellArea),
    mSectionConfigData(SolarSectionConfigData(that.mNumStrings, SolarStringConfigData(that.mNumCells,
                                                              that.mBlockingDiodeVoltageDrop ,
                                                              that.mBipassDiodeVoltageDrop ,
                                                              that.mBipassDiodeInterval ,
                                                              PVCellCompanionModelConfigData(that.mVocRef ,
                                                                                             that.mVmpRef ,
                                                                                             that.mVocTempCoefficient ,
                                                                                             that.mIscRef ,
                                                                                             that.mImpRef ,
                                                                                             that.mIscTempCoefficient ,
                                                                                             that.mIsat ,
                                                                                             that.mTemperatureRef ,
                                                                                             that.mCellDegradation,
                                                                                             that.mRs,
                                                                                             that.mRsh,
                                                                                             that.mVCrit ,
                                                                                             that.mBackSideIscReduction))))
{
    //Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SolarArray Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarArrayConfigData::~SolarArrayConfigData()
{
    //Nothing to do
}

//////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag                      (--)  flag to enable/disable the malfunction
/// @param[in] malfBlockageValue                     (--)  amount of blockage to add to the network link
/// @param[in] mIsShadowed;                          (--) whether this section has a line of site to the sun or not */
/// @param[in] mIsFailed;                            (--) Whether this string is failed or not.    */
/// @param[in] mNumFailedCells;                      (--) the number of failed cells in the string */
/// @param[in] mCellPowerMalfunctionIsEnabled;       (--) bool represents initial state of cell power malfunction.                                */
/// @param[in] mCellDegradationMalfunctionIsEnabled; (--) bool represents the inital state of the cell degradation malfunction                    */
/// @param[in] mCellDegradationOverrideValue;        (--) a double value which overrides default cell degradation when assoicated boolean is true */
/// @details  Constructs the SolarArray Input data
//////////////////////////////////////////////////////////////////////////////////////////////
SolarArrayInputData::SolarArrayInputData(const bool   malfBlockageFlag,
                                         const double malfBlockageValue,
                                         const bool isShadowed,
                                         const bool isFailed,
                                         const int numFailedCells,
                                         const bool cellPowerMalfIsEnabled,
                                         const bool cellDegradationMalfIsEnabled,
                                         const double cellDegradationOverrideValue,
                                         const bool isInLeo)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue),
    mIsShadowed(isShadowed),
    mIsFailed(isFailed),
    mNumFailedCells(numFailedCells),
    mCellPowerMalfunctionIsEnabled(cellPowerMalfIsEnabled),
    mCellDegradationMalfunctionIsEnabled(cellDegradationMalfIsEnabled),
    mCellDegradationOverrideValue(cellDegradationOverrideValue),
    mIsInLeo(isInLeo),
    mSectionInputData(SolarSectionInputData(mIsShadowed,SolarStringInputData(mIsFailed,
                                                                          mNumFailedCells,PVCellCompanionModelInputData(mCellPowerMalfunctionIsEnabled,
                                                                                                                       mCellDegradationMalfunctionIsEnabled,
                                                                                                                       mCellDegradationOverrideValue))))
 {
    //Nothing to do
 }

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
SolarArrayInputData::SolarArrayInputData(const SolarArrayInputData& that)
:
    GunnsBasicLinkInputData(that.mMalfBlockageFlag, that.mMalfBlockageValue),
    mIsShadowed(that.mIsShadowed),
    mIsFailed(that.mIsFailed),
    mNumFailedCells(that.mNumFailedCells),
    mCellPowerMalfunctionIsEnabled(that.mCellPowerMalfunctionIsEnabled),
    mCellDegradationMalfunctionIsEnabled(that.mCellDegradationMalfunctionIsEnabled),
    mCellDegradationOverrideValue(that.mCellDegradationOverrideValue),
    mIsInLeo(that.mIsInLeo),
    mSectionInputData(SolarSectionInputData(that.mIsShadowed,SolarStringInputData(that.mIsFailed,
                                                                                  that.mNumFailedCells,PVCellCompanionModelInputData(that.mCellPowerMalfunctionIsEnabled,
                                                                                                                                     that.mCellDegradationMalfunctionIsEnabled,
                                                                                                                                     that.mCellDegradationOverrideValue))))

{
            //Nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SolarArray Input Data Object
///////////////////////////////////////////////////////////////////////////////////////////////
SolarArrayInputData::~SolarArrayInputData()
{
    //Nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the SolarArray
///////////////////////////////////////////////////////////////////////////////////////////////////
SolarArray::SolarArray()
:
GunnsBasicLink(NUM_PORTS),
mSections(0),
mSectionArea(1.0),
mIsInLeo(true),
mMinimumSolarPower(5000.0),
mSunAngleScalar(1.0),
mLocalSunAngleScalar(1.0),
mLocalSunAngleScalarFlag(false),
mReferenceInsolation(1361.0),
mNumSections(0),
mArrayCurrent(45.0),
mArrayVoltage(180.0),
mIsMinorStep(false),
mIeq(0),
mGeq(0),
mMaxPossibleVoltage(0.0),
mMaxPossibleCurrent(0.0),
mFakeOrbitFlag(false),
mFakeOrbitTime(0.0),
mFakeOrbitTimeSet(0.0),
mFakeOrbitTimeSetFlag(false),
mFakeSunIntensity(1.0),
mFakeSunAngle(1.57),
mFakeSunAngleRate(0.0),
mPercentInsolation(100.0)
{
    //Nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the SolarArray
///////////////////////////////////////////////////////////////////////////////////////////////////
SolarArray::~SolarArray()
{
    cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   configData   (--) Reference to Link Config Data
/// @param[in]   inputData    (--) Reference to Link Input Data
/// @param[in]   networkLinks (--) Reference to the Network Link Vector
/// @param[in]     port0      (--) Port 0 Mapping
/// @param[in]     port1      (--) Port 1 Mapping
/// @throws   TsInitializationException
/// @details  Initializes the Solar Array Object
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::initialize(const SolarArrayConfigData &configData,
                            const SolarArrayInputData &inputData,
                            std::vector<GunnsBasicLink*> &networkLinks,
                            const int  port0,
                            const int  port1)
{

    int ports[NUM_PORTS] = {port0,port1};
    /// - Initialize the parent class.
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Initialize class attributes.
    validate(configData, inputData);
    mNumSections          = configData.mNumSections;
    allocateArrays();
    initializeSections(configData.mSectionConfigData, inputData.mSectionInputData);

    mAdmittanceMatrix[0] = 0.0;
    mAdmittanceMatrix[1] = 0.0;
    mAdmittanceMatrix[2] = 0.0;
    mAdmittanceMatrix[3] = 0.0;

    mSourceVector[0] = 0.0;
    mSourceVector[1] = 0.0;

    /// - Warn that this link is deprecated due to obsolescence by GunnsElectPvArray.
    GUNNS_WARNING("this link is deprecated!  It is obsoleted by GunnsElectPvArray.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Allocate arrays based on the number of sections
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::allocateArrays()
{

    TS_NEW_CLASS_ARRAY_EXT(mSections, static_cast<int>(mNumSections), SolarSection, (), std::string(mName) + ".mSections");

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes the sections with config and input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::initializeSections(const SolarSectionConfigData& cd, const SolarSectionInputData &id)
{

    for (int section = 0; section < mNumSections; section++){
        mSections[section].initialize(cd,id);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes allocated memory objects
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::cleanup()
{
   TS_DELETE_ARRAY(mSections);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt  (s)  model iteration period
/// @details  Method for updating the link for the network
/////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::step(const double dt)
{
    if (!mIsInLeo){
        updateNonLeoArray();
    }

    // pass down the sun angle scalar from epsOverrides
    for(int i=0; i < mNumSections; i++) {
        if(mLocalSunAngleScalarFlag) {
            mSections[i].setSunAngleScalar(mLocalSunAngleScalar);
        } else {
            mSections[i].setSunAngleScalar(mSunAngleScalar);
        }
    }

    // fake the sun angle and intensity values coming from environment
    if (mFakeOrbitFlag) {
        fakeOrbit(dt);
    } else {
        if(mSections[0].getFakeOrbitFlag()) {          // if the flag is set for section 1, clear them all out
            for(int i = 0; i < mNumSections; i++) {
                mSections[i].setFakeOrbitFlag(false);
            }
        }
    }

    //Zero values
    mGeq = 0.0;
    mIeq = 0.0;
    mMaxPossibleVoltage = 0.0;
    mMaxPossibleCurrent = 0.0;           // sum up the Isc values,  which will give the max current for existing conditions
    double lMaxPossibleCurrent = 0.0;    // sum up the IscReference values, which will give max current at ideal conditions

    double Isection = 0.0;
    double Vsection = 0.0;
    double Voc      = 0.0;
    double ThisMaxV = 0.0;

    //Loop over each section that the array is divided into.
    for (int sect = 0; sect < mNumSections; sect++){

        //We'd like to keep track of the maximum voltage and current possible, assuming all strings, cells, and sections, at max.
        //Note, do not trust this number if any string or cell malfunctions are present. It is only accurate with default conditions when everything
        // is working properly. This value is mainly for debugging purposes and should not be keyed off of.
        Voc          =  mSections[sect].getRefCellVoc();
        ThisMaxV     =  static_cast<double>(mSections[sect].getNumCells()) * Voc;
        if(ThisMaxV > mMaxPossibleVoltage){
            mMaxPossibleVoltage = ThisMaxV;
        }

        mMaxPossibleCurrent         = mMaxPossibleCurrent + static_cast<double>(mSections[sect].getNumStrings()) * mSections[sect].getStringIsc();
        lMaxPossibleCurrent += static_cast<double>(mSections[sect].getNumStrings()) * mSections[sect].getStringIscRef();

        //Solve the section current, (the section voltage is the same as the array voltage)
        //To do this note that GeqArray = GeqSec1 + GeqSec2 + GeqSec3 + ....   Therefore, the individual
        // contribution of a section can be solved from the total array voltage and the section's Geq and Ieq.
        Isection     = -(mArrayVoltage * mSections[sect].getConductanceStamp() - mSections[sect].getSourceVectorStamp());
        Vsection     =  mArrayVoltage;

        //Update each section. We pass the latest values for section voltage and current to the section so it can
        //update the location that the array is on the I-V curve. Doing this process repeatedly will cause our I and V
        // to move towards the correct location on the I-V curve as the load on the array changes.
        mSections[sect].update(mIsMinorStep,Isection,Vsection);

        //Add source vector contribution to the whole. After the sections get a chance to update, we want to use
        // the latest information to plug back into the source vector for the gunns solver
        mIeq = mIeq + mSections[sect].getSourceVectorStamp();

        //Add  conductance contribution to the whole. This follows the same logic as the source vector building.
        mGeq = mGeq + mSections[sect].getConductanceStamp();
    }

    //Setup other parts of the matrix. Gunns expects a matrix of the form [2x2] * [2x1] = [2x1]
    // {G}*{V} = {I} , after gunns solves, we know V, we can extract the flow in computeflows.
    setupSourceVector();
    setupAdmittanceMatrix();

    //calculate these here in step so that if a regulator link needs to access this information during step via pointer, it will be up
    //to date. Otherwise, if calculated in compute flows only, the link accessing via pointer will be off by one step.
    mArrayVoltage = getDeltaPotential(1,0);
    mArrayCurrent = -((mArrayVoltage * mAdmittanceMatrix[0]) + mSourceVector[0]);

    mArrayVoltage = std::min(mMaxPossibleVoltage, mArrayVoltage);     // keep the array voltage from being more than the max the array can generate
    mArrayCurrent = std::max(0.0, mArrayCurrent);                     // keep the array putting out current only

    mPercentInsolation = 0.0;
    if (mIsInLeo) {
        if (lMaxPossibleCurrent >= DBL_EPSILON){
            mPercentInsolation = (mArrayCurrent / lMaxPossibleCurrent) * 100.0;
        }
    } else if (mReferenceInsolation > 0.0) {
        /// - In non-LEO mode, compute percent insolation based on actual interface with the solar
        ///   environment model considering facing, occultation & ambient solar flux.
        for (int sect = 0; sect < mNumSections; sect++) {
            mPercentInsolation += mSections[sect].mSunIntensity
                                * mSections[sect].mSolarPowerOnSection
                                * sin(mSections[sect].mSunAngleFromHorizontal)
                                / mSectionArea;
        }
        mPercentInsolation *= 100.0 / (mReferenceInsolation * mNumSections);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt  (s)  model iteration period
/// @details  Method for computing flows
/////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::computeFlows(const double dt __attribute__((unused)))
{
    /// Do KCL over the loop. Assume I flows left to right.
    /// We know Voltage drop, Geq, and Ieq.
    //////////////////////////////////////////////////
    ///                    Geq                      //
    ///               |--/\/\/\--|                  //
    ///  (p0)   V+ ---|          |--- V-    (p1)    //
    ///               |---(=>)---|                  //
    ///                    Ieq                      //
    //////////////////////////////////////////////////
    mArrayVoltage = getDeltaPotential(1,0);
    mArrayCurrent = -((mArrayVoltage * mAdmittanceMatrix[0]) + mSourceVector[0]);

//    if(mArrayVoltage > mMaxPossibleVoltage){
//        mArrayVoltage = mMaxPossibleVoltage;
//        mArrayCurrent = 0.0;
//    }
//
//    if(mArrayCurrent > mMaxPossibleCurrent){
//        mArrayVoltage = 0.0;
//        mArrayCurrent = mMaxPossibleCurrent;
//        }

}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt         (s)  model iteration period
/// @param[in]   minorStep  (--) current minor step inside a major step
/// @details  Method for updating the link for the network - non-linear link
/////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::minorStep(const double dt, const int minorStep __attribute__((unused)))
{
    mIsMinorStep = true;
    computeFlows(dt);
    step(dt);
    mIsMinorStep = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Validates the initialized parameters passed in through config and input data
/////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::validate(const SolarArrayConfigData& config, const SolarArrayInputData& input)
{
    if (config.mNumSections <= 0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                "mNumSections must be greater than zero");
    }

    /// - Cell area and efficiency are only used when not in LEO.  They are stored in the reference
    ///   cell object, but we validate the value passed to the ref cell here since only this class
    ///   knows whether we're in LEO.
    if (!input.mIsInLeo) {
        if ((config.mCellEfficiency < 0) || (config.mCellEfficiency > 1)){
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mCellEfficiency must be between 0 and 1.0");
        }
        if (config.mCellArea <= 0){
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mCellArea must be greater than 0");
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Populates the admittance matrix with the latest value of Geq
/////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::setupAdmittanceMatrix()
{
    if(mAdmittanceMatrix[0] != mGeq){
    mAdmittanceMatrix[0] = mGeq;
    mAdmittanceMatrix[1] = -mAdmittanceMatrix[0];
    mAdmittanceMatrix[2] = -mAdmittanceMatrix[0];
    mAdmittanceMatrix[3] = mAdmittanceMatrix[0];
    mAdmittanceUpdate = true;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Populates the source vector with the latest Ieq
/////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::setupSourceVector()
{
    /// With mSourceVector[0] as negative, index 1 will be positive. This means that current
    /// flowing from 0 to 1 will be positive.
    mSourceVector[0] = -mIeq;
    mSourceVector[1] = -mSourceVector[0];
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  convergedStep  (--)  number of times the GUNNS network has converged in the current major step
/// @param[in]  absoluteStep   (--)  current minor step inside the major step
/// @details  Returns TRUE if the network needs re-solving for a trip condition
/////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult SolarArray::confirmSolutionAcceptable(
        const int convergedStep __attribute__((unused)), const int absoluteStep __attribute__((unused))) {

   GunnsBasicLink::SolutionResult result;
   result = GunnsBasicLink::CONFIRM;
   return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns if the link is non-linear
/// @return bool
////////////////////////////////// ///////////////////////////////////////////////////////////////
bool SolarArray::isNonLinear() {
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Resets the last minor step
/// @return bool
////////////////////////////////// ///////////////////////////////////////////////////////////////
bool SolarArray::resetLastMinorStep(
        int convergedStep __attribute__((unused)), int absoluteStep __attribute__((unused)))
{
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns if the link admittance matrix has been updated
/// @return bool
/////////////////////////////////////////////////////////////////////////////////////////////////
bool SolarArray::needAdmittanceUpdate() {
    return mAdmittanceUpdate;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  function to override the sun angle and intensity given by environment.  If a flag
///           is set this function will run a timer to set the sun intensity at 1.0 for 60 minutes,
///           then 0.0 for 30 minutes.
/////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::fakeOrbit(const double dt) {
    // set the time from the overrides, if applicable
    if(mFakeOrbitTimeSetFlag) {
        // if someone enters a number larger than one orbit, put them at the right point for that orbit
        while (mFakeOrbitTimeSet > 90.0) {
            mFakeOrbitTimeSet -= 90.0;
        }
        // be on the lookout for negative time values
        mFakeOrbitTime = std::max(0.0, mFakeOrbitTimeSet);
    }

    // increment the timer
    double lTimeInSec = mFakeOrbitTime * 60.0;

    if(!mIsMinorStep) {
        lTimeInSec += dt;
    }

    mFakeOrbitTime = lTimeInSec / 60.0;

    // fake the solar intensity, with a ramp down before eclipse and ramp up before insolation
    if (mFakeOrbitTime > 0.0 && mFakeOrbitTime <= 59.7) {
        mFakeSunIntensity = 1.0;   // day time
    } else if (mFakeOrbitTime > 59.7 && mFakeOrbitTime <= 60.0) {
        // ramp down to zero
        mFakeSunIntensity -= 0.0111;
    } else if (mFakeOrbitTime > 60.0 && mFakeOrbitTime <= 89.7) {
        mFakeSunIntensity = 0.0;  // night time
    } else if (mFakeOrbitTime > 89.7 && mFakeOrbitTime <= 90.0) {
        mFakeSunIntensity += 0.0111;
    } else {
        mFakeOrbitTime = 0.0;  // reset the orbit time
    }

    // now change the angle as needed
    if(!mIsMinorStep) {
        mFakeSunAngle += (mFakeSunAngleRate * dt);
    }

    // bound fake angle to +/- 180 degrees
    if (mFakeSunAngle > 3.14) {
        mFakeSunAngle = -3.139;
    }

    if(mFakeSunAngle < -3.14) {
        mFakeSunAngle = 3.139;
    }

    // if the sun angle is negative the array is backlit
    bool lBackLit = false;
    if(mFakeSunAngle < 0.0) {
        lBackLit = true;
    }

    // now pass down the overridden parameters to all the sections
    for(int i = 0; i < mNumSections; i++) {
        mSections[i].setFakeOrbitParms(mFakeOrbitFlag, mFakeSunAngle, mFakeSunIntensity, lBackLit);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Function to dynamically calculate the cell reference current so that the
///           cells produce current that is actually a function of the solar flux
///           magnitude, cell area, and cell performance.
/////////////////////////////////////////////////////////////////////////////////////////////////
void SolarArray::updateNonLeoArray()
{
    for(int secNum = 0; secNum < mNumSections; secNum++) {
        double solarPowerPerUnitArea =  mSections[secNum].mSolarPowerOnSection / mSectionArea;
        for(int stringNum = 0; stringNum < mSections[secNum].getNumStrings(); stringNum++) {

            if(mSections[secNum].mSolarPowerOnSection < mMinimumSolarPower){
                mSections[secNum].mStrings[stringNum].mRefCell.setIscRef(0.0);
            }else{
                // Cell reference current calculation
                double refCellEfficiency = mSections[secNum].mStrings[stringNum].mRefCell.getCellEfficiency();
                double refCellVoc = mSections[secNum].mStrings[stringNum].mRefCell.getVoc();
                double refCellArea = mSections[secNum].mStrings[stringNum].mRefCell.getCellArea();
                double iscRef = refCellEfficiency * solarPowerPerUnitArea * refCellArea / refCellVoc;
                mSections[secNum].mStrings[stringNum].mRefCell.setIscRef(iscRef);
            }
        }
    }
}
