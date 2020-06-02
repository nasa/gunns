/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (core/GunnsBasicCapacitor.o)
    )
**************************************************************************************************/
#include "GunnsThermalCapacitor.hh"
#include <cfloat>    //needed for DBL_EPSILON in updateState()
#include "aspects/thermal/PtcsMacros.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

/// @details GUNNS Thermal Capacitor Constants
const double GunnsThermalCapacitorInputData::DEFAULT_TEMPERATURE  =  294.261;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                  --  Link name
/// @param[in]  nodes                 --  Network nodes array
/// @param[in]  editCapacitanceGroup  --  Identifier for capacitance edit grouping
///
/// @details  Constructs the GunnsThermalCapacitor ConfigData
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorConfigData::GunnsThermalCapacitorConfigData(const std::string& name,
                                                                 GunnsNodeList*     nodes,
                                                                 const int          editCapacitanceGroup)
    :
    GunnsBasicCapacitorConfigData(name, nodes),
    mEditCapacitanceGroup(editCapacitanceGroup)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  -- Input Source to Copy
///
/// @details  Copy Constructs the GunnsThermalCapacitor ConfigData
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorConfigData::GunnsThermalCapacitorConfigData(
        const GunnsThermalCapacitorConfigData& that)
    :
    GunnsBasicCapacitorConfigData(that),
    mEditCapacitanceGroup(that.mEditCapacitanceGroup)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the GunnsThermalCapacitor ConfigData object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorConfigData::~GunnsThermalCapacitorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag         (--)  Blockage malfunction flag
/// @param[in] malfBlockageValue        (--)  Blockage malfunction fractional value (0-1)
/// @param[in] capacitance              (J/K) Thermal capacitance of the link
/// @param[in] potential                (K)   Initial potential (temperature) of the capacitive node
/// @param[in] temperatureOverrideFlag  (--)  Flag to activate temperature override
/// @param[in] temperatureOverrideValue (K)   Value at which to override temperature
///
/// @details  Default constructs this GunnsThermalCapacitor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorInputData::GunnsThermalCapacitorInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue,
        const double capacitance,
        const double potential,
        const bool   temperatureOverrideFlag,
        const double tTemperatureOverrideValue)
    :
    GunnsBasicCapacitorInputData(malfBlockageFlag, malfBlockageValue, capacitance, potential),
    iTemperatureOverrideFlag(temperatureOverrideFlag),
    iTemperatureOverrideValue(tTemperatureOverrideValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  --  Object to copy
///
/// @details  Copy constructs this GunnsThermalCapacitor InputData.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorInputData::GunnsThermalCapacitorInputData(
        const GunnsThermalCapacitorInputData& that)
    :
    GunnsBasicCapacitorInputData(that),
    iTemperatureOverrideFlag(that.iTemperatureOverrideFlag),
    iTemperatureOverrideValue(that.iTemperatureOverrideValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GunnsThermalCapacitor InputData.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitorInputData::~GunnsThermalCapacitorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GunnsThermalCapacitor Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitor::GunnsThermalCapacitor()
    :
    GunnsBasicCapacitor(),
    mEditCapacitanceGroup(0),
    mTemperature(0.0),
    mExternalHeatFlux(),
    mSumExternalHeatFluxes(0.0)
{
    /** Initialize every index of the mExternalHeatFlux[] array to zero. */
    for (int i = 0; i < NUM_EXT_HEATFLUXES; ++i)
    {
        mExternalHeatFlux[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the GunnsThermalCapacitor object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalCapacitor::~GunnsThermalCapacitor()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]       configData   -- Reference to Link Config Data
/// @param[in]       inputData    -- Reference to Link Input Data
/// @param[in,out]   networkLinks -- Reference to the Network Link Vector
/// @param[in]       port0        -- Port 0 Mapping
/// @param[in]       port1        -- Port 1 Mapping
///
/// @details  Initializes the GunnsThermalCapacitor
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalCapacitor::initialize(const GunnsThermalCapacitorConfigData& configData,
                                       const GunnsThermalCapacitorInputData&  inputData,
                                       std::vector<GunnsBasicLink*>&          networkLinks,
                                       const int                              port0,
                                       const int                              port1)
{
    GunnsBasicCapacitor::initialize(configData, inputData, networkLinks, port0, port1);

    /** Reset init flag that got set to "true" in parent-class initialize(). */
    mInitFlag = false;

    /// @brief  Call validate() before proceeding.
    validate(inputData);

    /// - Initialize from config data.
    mEditCapacitanceGroup = configData.mEditCapacitanceGroup;

    /// - Initialize from input data.
    if (inputData.iTemperatureOverrideFlag) {
        setPortOverride(0, inputData.iTemperatureOverrideValue);
    }

    /** Set init flag on successful validation. */
    mInitFlag = true;

    /** Initialize mTemperature as the potential at the link's non-vacuum node. This member is
        set so that users of TrickView or the simbus will be able to easily find the temperature
        of the GunnsThermalCapacitor. */
    mTemperature = mPotentialVector[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      inputData   (--) Input data.
///
/// @throw   TsInitializationException
///
/// @details  Validates the GunnsThermalCapacitor's initialization data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalCapacitor::validate(const GunnsThermalCapacitorInputData&  inputData) const
{
    /** Throw exception on potential being less than zero. Temperature, which is potential for a
        thermal network, is on an absolute scale, and should not be less than zero. */
    TS_PTCS_IF_ERREX(inputData.mPotential < 0.0, TsInitializationException,
            "invalid input data", "Input temperature less than zero.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalCapacitor::restartModel()
{
    /// - Reset the base class.
    GunnsBasicCapacitor::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    for (int i = 0; i < NUM_EXT_HEATFLUXES; ++i)
    {
        mExternalHeatFlux[i] = 0.0;
    }
    mSumExternalHeatFluxes = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt   (s)  Integration time step (not used)
///
/// @details  Updates the state of the GunnsThermalCapacitor, called from within
///           GunnsBasicCapacitor::step(). This method finds the sum of the entries in the
///           mExternalHeatFlux array. This sum is then used to update the link's
///           mPotentialVector[0], which is the temperature of the link's non-vacuum node. The
///           buildCapacitance() method in the parent-class computes the link's mSourceVector
///           based on the mPotentialVector, therefore the mSumExternalHeatFluxes is
///           communicated up into the source vector by the parent-class, for use later by GUNNS.
///
///           For the special case where the GunnsThermalCapacitor is in a "dummy" GUNNS
///           network, it must be able to solve for its own potential. When the link is in
///           a standard network, GUNNS will solve for this potential and overwrite the
///           value determined here. The effect of the ExternalHeatFluxes, however, will still be
///           accounted for in the mSourceVector array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalCapacitor::updateState(const double dt)
{
    /** Reset the sum of external heat fluxes to zero. */
    mSumExternalHeatFluxes = 0.0;

    /** Sum up all of the External Heat fluxes in the mExternalHeatFlux[] array. */
    for (int i = 0; i < NUM_EXT_HEATFLUXES; ++i)
    {
        mSumExternalHeatFluxes += mExternalHeatFlux[i];
    }

    /** Divide by zero check. If a capacitance link isolated in a dummy network has zero
        capacitance, then the ExternalHeatFluxes will be ignored, because the temperature would
        instantly shoot to infinity (zero in the denominator). Instead, for mCapacitance == 0,
        the potential will remain at its initial value. */
    if (mCapacitance > DBL_EPSILON and not mOverrideVector[0])
    {
        /** In GunnsBasicCapacitor::buildCapacitance(), which in step() executes next after this
            method, this link's mSourceVector will be calculated based on the value of
            mPotentialVector[0]. Therefore, setting mPotentialVector[0] below will basically have
            the downstream effect of adding mSumExternalHeatFluxes to the link's mSourceVector.
            By doing it this way, there is the additional benefit of the link calculating its own
            potential, which it must do if it is in a stand-alone "dummy" network. */
        mPotentialVector[0] += mSumExternalHeatFluxes * dt / mCapacitance;

        /** Set the potential of the link's node. */
        mNodes[0]->setPotential(mPotentialVector[0]);

        /** Saves mTemperature as the potential at the link's non-vacuum node. This member is set
            so that users of TrickView or the simbus will be able to easily find the temperature
            of the GunnsThermalCapacitor. Necessary here since the potential is being updated. */
        mTemperature = mPotentialVector[0];

        /** If the link is in a regular (not-dummy) Gunns network, then the network will solve
            for the potential of the node and overwrite the value set above. This is desired. */
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Prepares the object for simbus write by updating the mTemperature of the capacitor.
///           The processOutputs() method is the very last call by the Gunns::step() method. It
///           overwrites the virtual method in the BasicLink base-class. When the
///           GunnsThermalCapacitor is in a real GUNNS network that gets solved, the simbus needs
///           an updated temperature that reflects the new value of PotentialVector[0].
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalCapacitor::processOutputs()
{
    /** Saves mTemperature as the potential at the link's non-vacuum node. This member is set
        so that users of TrickView or the simbus will be able to easily find the temperature
        of the GunnsThermalCapacitor. */
    mTemperature = mPotentialVector[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the external heat flux at the given index to the given flux value.  An exception
///           is thrown if the index is out of range.
///
/// @throws   TsOutOfBoundsException
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalCapacitor::setExternalHeatFlux(const unsigned int index, const double flux)
{
    TS_PTCS_IF_ERREX(index >= NUM_EXT_HEATFLUXES, TsOutOfBoundsException,
            "argument out of range", "Input index greater than max allowed.");
    mExternalHeatFlux[index] = flux;
}
