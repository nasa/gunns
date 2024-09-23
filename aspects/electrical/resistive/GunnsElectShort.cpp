/**
@file
@brief    GUNNS Electrical Short Link implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ((core/GunnsBasicConductor.o)
     (GunnsShortUtil.o))
*/

#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"
#include <cfloat>
#include "GunnsElectShort.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name   (--)  Link name.
/// @param[in] nodes  (--)  Network nodes array.
///
/// @details  Constructs the Electrical Short Config data.  The base class default conductivity term
///           is zeroed and isn't used.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectShortConfigData::GunnsElectShortConfigData(const std::string& name,
                                                     GunnsNodeList*     nodes)
    :
    GunnsBasicConductorConfigData(name, nodes, 0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from.
///
/// @details  Copy Constructs the Electrical Short Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectShortConfigData::GunnsElectShortConfigData(const GunnsElectShortConfigData& that)
    :
    GunnsBasicConductorConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Electrical Short Config Data Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectShortConfigData::~GunnsElectShortConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag         (--)  Initial blockage malfunction flag.
/// @param[in] malfBlockageValue        (--)  Initial blockage malfunction fractional value (0-1).
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
/// @details  Default constructs this Electrical Short input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectShortInputData::GunnsElectShortInputData(const bool   malfBlockageFlag,
                                                   const double malfBlockageValue,
                                                   const double malfMinimumVoltage,
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
    GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue),
    mShort(malfMinimumVoltage,
           malfConstantPowerFlag,
           malfConstantPowerValue,
           malfConstantCurrentFlag,
           malfConstantCurrentValue,
           malfInitialPowerFlag,
           malfInitialPowerValue,
           malfInitialCurrentFlag,
           malfInitialCurrentValue,
           malfResistanceFlag,
           malfResistanceValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from.
///
/// @details  Copy constructs this Electrical Short input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectShortInputData::GunnsElectShortInputData(
        const GunnsElectShortInputData& that)
    :
    GunnsBasicConductorInputData(that),
    mShort(that.mShort)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Electrical Short input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectShortInputData::~GunnsElectShortInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Electrical Short Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectShort::GunnsElectShort()
    :
    GunnsBasicConductor(),
    mShort()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Electrical Short Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectShort::~GunnsElectShort()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data.
/// @param[in]     inputData    (--) Reference to Link Input Data.
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector.
/// @param[in]     port0        (--) Port 0 Mapping.
/// @param[in]     port1        (--) Port 1 Mapping.
///
/// @throws   TsInitializationException
///
/// @details  Initializes the basic conductor with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectShort::initialize(const GunnsElectShortConfigData& configData,
                                            const GunnsElectShortInputData&  inputData,
                                            std::vector<GunnsBasicLink*>&               networkLinks,
                                            const int                                   port0,
                                            const int                                   port1)
{
    /// - Initialize the parent class.
    GunnsBasicConductor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize class attributes.
    mShort.initialize(inputData.mShort);

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectShort::restartModel()
{
    /// - Reset the base class.
    GunnsBasicConductor::restartModel();

    /// - Reset non-config & non-checkpointed class attributes (there aren't any).
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Integration time step.
/// @param[in] minorStep (--) Not used.
///
/// @details  Updates this link's contributions to the network system of equations in the solver
///           minor steps loop.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectShort::minorStep(const double dt, const int minorStep __attribute__((unused)))
{
    step(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Not used.
/// @param[in] minorStep (--) Not used.
///
/// @details  Sets the link conductance to the electrical short conductance.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectShort::updateState(const double dt __attribute__((unused)))
{
    mEffectiveConductivity = mShort.computeShort(std::fabs(mPotentialVector[0] - mPotentialVector[1]));
}
