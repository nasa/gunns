/***************************************** TRICK HEADER ********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling the GUNNS Electrical Resistor.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
    (
     (core/GunnsBasicLink.o)
     (core/GunnsBasicConductor.o)
    )

 PROGRAMMERS:
    (
     (Mike Moore) (L3) (2013-05)
    )
 **************************************************************************************************/
#include "core/GunnsBasicConductor.hh"
#include "aspects/electrical/resistive/GunnsElectricalResistor.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                (--) Link name
/// @param[in] nodes               (--) Network nodes array
/// @param[in] defaultResistance (ohm) Default resistance of the link
///
/// @details  Constructs the Electrical Resistor Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectricalResistorConfigData::GunnsElectricalResistorConfigData(const std::string& name,
                                                             GunnsNodeList*     nodes,
                                                             const double       resistance,
                                                             const double       electricalEfficiency)
    :
    GunnsBasicConductorConfigData(name, nodes, 1.0/resistance),
    mResistance(resistance),
    mElectricalEfficiency(electricalEfficiency)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from
///
/// @details  Copy Constructs the Electrical Resistor Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectricalResistorConfigData::GunnsElectricalResistorConfigData(
        const GunnsElectricalResistorConfigData& that)
    :
    GunnsBasicConductorConfigData(that),
    mResistance(that.mResistance),
    mElectricalEfficiency(that.mElectricalEfficiency)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Electrical Resistor Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectricalResistorConfigData::~GunnsElectricalResistorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
///
/// @details  Default constructs this Electrical Resistor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectricalResistorInputData::GunnsElectricalResistorInputData(const bool   malfBlockageFlag,
                                                           const double malfBlockageValue)
    :
    GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Basic Conductor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectricalResistorInputData::GunnsElectricalResistorInputData(const GunnsElectricalResistorInputData& that)
    :
    GunnsBasicConductorInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Basic Conductor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectricalResistorInputData::~GunnsElectricalResistorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Electrical Resistor Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectricalResistor::GunnsElectricalResistor():
    mResistance              (0.0),
    mVoltageIn               (0.0),
    mVoltageOut              (0.0),
    mVoltageDrop             (0.0),
    mCurrent                 (0.0),
    mElectricalEfficiency    (0.0),
    mWasteHeat               (0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Conductor Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectricalResistor::~GunnsElectricalResistor()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data
/// @param[in]     inputData    (--) Reference to Link Input Data
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector
/// @param[in]     port0        (--) Port 0 Mapping
/// @param[in]     port1        (--) Port 1 Mapping
///
/// @throws   TsInitializationException
///
/// @details  Initializes the basic conductor with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectricalResistor::initialize(const GunnsElectricalResistorConfigData& configData,
                                     const GunnsElectricalResistorInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        networkLinks,
                                     const int                            port0,
                                     const int                            port1)
{
    /// - Initialize the parent class.
    GunnsBasicConductor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize class attributes.
    mResistance          = configData.mResistance;
    mElectricalEfficiency          = configData.mElectricalEfficiency;

    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates the link
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectricalResistor::validate() const
{
    /// - Issue an error on conductivity being less than zero.
    if (mResistance < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    " Link has resistance < 0.");
    }

    /// - Throw an exception if electrical efficiency <= 0 or electrical efficiency >= 1.
    if (!MsMath::isInRange(static_cast<double>(FLT_EPSILON), mElectricalEfficiency, 1.0 - static_cast<double>(FLT_EPSILON))) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Electrical Efficiency outside valid range (0-1).");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectricalResistor::restartModel()
{
    /// - Reset the base class.
    GunnsBasicConductor::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Override updateState so that resistance can be converted to conductivity and updated
///           in the solver.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectricalResistor::updateState(const double dt __attribute__((unused)))
{
    /// - Protect for divide by zero on resistance.
    //TODO this is a temporary fix: we had a rare timing problem where a parallel process would
    //stomp on mResistance in between when we checked for mResistance > DBL_EPSILON and when we
    //divided 1 / mResistance, causing a divide-by-zero.  As a temporary workaround, store
    //mResistance in a different local variable on the stack so it's less likely to get stomped
    //during the division.
    const double temp = mResistance;
    mEffectiveConductivity = 1.0 / std::max(DBL_EPSILON, temp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Override computeFlows so that this link can set its voltage, current, and calculate
///           its electrical waste heat.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectricalResistor::computeFlows(const double dt)
{
    // Set the voltage class members appropriately
    mVoltageIn  = getPotentialVector()[0];
    mVoltageOut = getPotentialVector()[1];
    mPotentialDrop = getDeltaPotential();
    mVoltageDrop = mPotentialDrop;
    // Call basic conductor parent class functions
    computeFlux();
    updateFlux(dt, mFlux);
    computePower();
    transportFlux();
    // Update resistor properties
    mCurrent = getFlux();
    mWasteHeat = calculateWasteHeat();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Calculate waste heat generated by the link
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsElectricalResistor::calculateWasteHeat() const
{
    return getPower() * (1.0 - mElectricalEfficiency);
}
