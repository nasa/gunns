/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
    (Provides the classes for modeling the GUNNS Electrical User Load for constant resistor.)

 REQUIREMENTS:
    ()

 REFERENCE:
    ()

 ASSUMPTIONS AND LIMITATIONS:
    ()
LIBRARY DEPENDENCY:
    (
     (simulation/hs/TsHsMsg.o)
     (software/exceptions/TsInitializationException.o)
    )

 PROGRAMMERS:
    (
     (Shailaja Janapati) (L3) (2011-10) (Initial Prototype)
    )
 **************************************************************************************************/

#include "aspects/electrical/UserLoad/test/DerivedResistanceLoadExample.hh"
#include <cmath>
#include <cfloat>
#include "aspects/electrical/UserLoad/ResistiveLoad.hh"


//=================================================================================================
//
// BEGIN CLASS DerivedResistanceLoadExampleConfigData
//
//=================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    name                  --    user load name
/// @param    loadType              --    type of user load constant resistance or conatant power
/// @param    underVoltageLimit     --    lower limit for the voltage at which the voltage trips
/// @param    resistance            --    resistance for this user load
///
/// @details  Default User Load constant resistor load Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
DerivedResistanceLoadExampleConfigData::DerivedResistanceLoadExampleConfigData(const std::string& name,
        const int    loadType,
        const double underVoltageLimit,
        const double resistanceNormal,
        const double resistanceStandby,
        const double prllResistance1,
        const double prllResistance2)
:
        ResistiveLoadConfigData(name, loadType, underVoltageLimit,
                resistanceNormal, resistanceStandby),
        mPrllResistance1(prllResistance1),
        mPrllResistance2(prllResistance2)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
///
/// @details  Default User Load constant resistor load Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
DerivedResistanceLoadExampleConfigData::DerivedResistanceLoadExampleConfigData(
        const DerivedResistanceLoadExampleConfigData& that)
:
        ResistiveLoadConfigData(that),
        mPrllResistance1(that.mPrllResistance1),
        mPrllResistance2(that.mPrllResistance2)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default derived resistance load example config data class  Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
DerivedResistanceLoadExampleConfigData::~DerivedResistanceLoadExampleConfigData()
{
    // nothing to do
}


//=================================================================================================
//
// BEGIN CLASS DerivedResistanceLoadExampleInputData
//
//=================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    lOverwriteResistanceFlag    --    Flag to over write the cumulative power of this load
/// @param    lOverwriteResistance              --    Value to assign for the power
/// @details  Default User Load constant resistor load Input Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
DerivedResistanceLoadExampleInputData::DerivedResistanceLoadExampleInputData(const bool lMalfOverrideCurrentFlag,
        const double lMalfOverrideCurrentValue, const int loadOperMode)
:ResistiveLoadInputData(lMalfOverrideCurrentFlag, lMalfOverrideCurrentValue, loadOperMode)
{
    // nothing to do

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    that  --  Object to copy
///
/// @details  Default User Load constant resistor load Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
DerivedResistanceLoadExampleInputData::DerivedResistanceLoadExampleInputData(
        const DerivedResistanceLoadExampleInputData& that)
:ResistiveLoadInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default derived resistance load example input data class  Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
DerivedResistanceLoadExampleInputData::~DerivedResistanceLoadExampleInputData()
{
    // nothing to do
}

//=================================================================================================
//
// BEGIN CLASS DerivedResistanceLoadExample
//
//=================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Default Constant Resistance User Load Electrical Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
DerivedResistanceLoadExample::DerivedResistanceLoadExample()
:ResistiveLoad(),
 mPowerdissipated(0.0),
 mEfficiency(0.8), // 80% efficiency
 mPrllResistance1(0.0),
 mPrllResistance2(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @details  Default User Load constant resistor load Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
DerivedResistanceLoadExample::~DerivedResistanceLoadExample()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    configData    --  Constant Resistance User Load Electric Config Data
/// @param    networkLoads  --  vector of all user loads to which this object will be added.
/// @param    cardID        --  ID of the load switch card
/// @param    loadSwitchId  --  ID of the current user load object
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Constant Resistance User Load Electrical with configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DerivedResistanceLoadExample::initialize(const DerivedResistanceLoadExampleConfigData& configData,
        const DerivedResistanceLoadExampleInputData& inputData,
        std::vector<UserLoadBase*>&        networkLoads,
        const int                           cardId,
        const int                        loadSwitchId)
{
    // Initialize power dissipated
    mPowerdissipated = 0.0;
    mEfficiency = 0.8; // 80 % efficiency is assumed for the load;

    mPrllResistance1 = configData.mPrllResistance1;
    mPrllResistance2 = configData.mPrllResistance2;

    /// - Initialize and validate parent
    ResistiveLoad::initialize(configData, inputData, networkLoads, cardId, loadSwitchId);
}




////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   dt  (s)  Integration time step
///
/// @details Method to update the user load during a time step. For a constant resistance load
/// the input parameter is not "dt" but the voltage to be passed to the system.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
/*void DerivedResistanceLoadExample::step(const double voltage) // maybe power in watts is supplied instead of voltage
{
    // when the voltage is supplied here maybe the limit is different than the other class.
    // like if the mode of Fan is high it user 20W power so calculate the voltage and send the data
    // call the parent step function
    updateLoad();

    ResistiveLoad::step(voltage);
}*/



/////////////////////////////////////////////////////////////////////////////////////////////////
///
///// @details Method: updateLoad - method to be overwritten by the derived classes.
/////////////////////////////////////////////////////////////////////////////////////////////////
void DerivedResistanceLoadExample::updateLoad(){
    //update mResistance based on the parallel resistances
    // Calculate power dissipated based on efficiency and operation mode.
    // uses only two modes ON/OFF and calculates the power dissipated value.
    if (LoadON == mLoadOperMode) {
        // calculate the normal resistance for this load
        if (mPrllResistance1 > 0.0 && mPrllResistance2 > 0.0) {
            mResistanceNormal =  1 / (1/mPrllResistance1 + 1/mPrllResistance2);
        }
       mEquivalentResistance = mResistanceNormal;

       if (mEquivalentResistance > 0.0) {
           mPowerdissipated = (mVoltage * mVoltage) / (mEquivalentResistance * (1.0 - mEfficiency));
       }
    }
}



