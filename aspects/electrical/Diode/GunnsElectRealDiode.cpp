/**
@file
@brief    GUNNS Real Diode Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsBasicPotential.o))
*/

#include "GunnsElectRealDiode.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name               (--)    Link name.
/// @param[in] nodes              (--)    Network nodes array.
/// @param[in] forwardConductance (1/ohm) Diode conductance for forward bias.
/// @param[in] reverseConductance (1/ohm) Diode conductance for reverse bias.
/// @param[in] voltageDrop        (V)     Diode junction voltage drop in forward bias.
///
/// @details  Constructs the Real Diode Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectRealDiodeConfigData::GunnsElectRealDiodeConfigData(const std::string& name,
                                                             GunnsNodeList*     nodes,
                                                             const double       forwardConductance,
                                                             const double       reverseConductance,
                                                             const double       voltageDrop)
    :
    GunnsBasicPotentialConfigData(name, nodes, forwardConductance),
    mReverseConductivity(reverseConductance),
    mVoltageDrop(voltageDrop)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from.
///
/// @details  Copy Constructs the Real Diode Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectRealDiodeConfigData::GunnsElectRealDiodeConfigData(
        const GunnsElectRealDiodeConfigData& that)
    :
    GunnsBasicPotentialConfigData(that),
    mReverseConductivity(that.mReverseConductivity),
    mVoltageDrop(that.mVoltageDrop)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Real Diode Config Data Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectRealDiodeConfigData::~GunnsElectRealDiodeConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag.
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1).
/// @param[in] reverseBias       (--) Initial bias direction.
///
/// @details  Default constructs this Real Diode input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectRealDiodeInputData::GunnsElectRealDiodeInputData(const bool   malfBlockageFlag,
                                                           const double malfBlockageValue,
                                                           const bool   reverseBias)
    :
    GunnsBasicPotentialInputData(malfBlockageFlag, malfBlockageValue, 0.0),
    mReverseBias(reverseBias)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  Copy constructs this Real Diode input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectRealDiodeInputData::GunnsElectRealDiodeInputData(const GunnsElectRealDiodeInputData& that)
    :
    GunnsBasicPotentialInputData(that),
    mReverseBias(that.mReverseBias)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Real Diode input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectRealDiodeInputData::~GunnsElectRealDiodeInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Real Diode Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectRealDiode::GunnsElectRealDiode()
    :
    GunnsBasicPotential(),
    mReverseConductivity(0.0),
    mVoltageDrop(0.0),
    mReverseBias(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Real Diode Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectRealDiode::~GunnsElectRealDiode()
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
/// @details  Initializes the basic potential link with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectRealDiode::initialize(const GunnsElectRealDiodeConfigData& configData,
                                     const GunnsElectRealDiodeInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        networkLinks,
                                     const int                            port0,
                                     const int                            port1)
{
    /// - Initialize the parent class.
    GunnsBasicPotential::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag            = false;

    /// - Initialize from config & input data.
    mReverseConductivity = configData.mReverseConductivity;
    mVoltageDrop         = configData.mVoltageDrop;
    mReverseBias         = inputData.mReverseBias;

    validate();

    /// - Set init flag on successful validation.
    mInitFlag            = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates the the basic potential object.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectRealDiode::validate() const
{
    /// - Issue an error on mReverseConductivity being less than zero.
    if (mReverseConductivity < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has reverse bias conductivity < 0.");
    }

    /// - Issue an error on mVoltageDrop being less than zero.
    if (mVoltageDrop < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has junction voltage drop < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectRealDiode::restartModel()
{
    /// - Reset the base class.
    GunnsBasicPotential::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Not used.
///
/// @details  Updates the effective conductance and source potential of the diode based on the
///           voltage bias direction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectRealDiode::updateState(const double dt __attribute__((unused)))
{
    if (mReverseBias) {
        mEffectiveConductivity = mReverseConductivity;
        mSourcePotential       = 0.0;
    } else {
        mEffectiveConductivity = mDefaultConductivity;
        mSourcePotential       = -mVoltageDrop;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Integration time step.
/// @param[in] minorStep (--) Not used.
///
/// @details  For this link, minor steps are identical to major steps, so this simply calls the base
///           step method implementation.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectRealDiode::minorStep(const double dt, const int minorStep __attribute__((unused)))
{
    step(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) Not used.
/// @param[in] absoluteStep  (--) Not used.
///
/// @returns  SolutionResult  (--)  Whether this link confirms or rejects the network solution.
///
/// @details  Updates the bias direction based on node potentials from the converged network
///           solution.  If the direction flipped then we reject the solution, otherwise we confirm.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult GunnsElectRealDiode::confirmSolutionAcceptable(
        const int convergedStep __attribute__((unused)),
        const int absoluteStep __attribute__((unused)))
{
    if (updateBias()) {
        return REJECT;
    } else {
        return CONFIRM;
    }
}
