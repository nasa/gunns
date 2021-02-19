/**
@file
@brief    GUNNS Resistor With Power Function Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (core/GunnsBasicLink.o)
    )
*/

#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"
#include <cfloat>
#include "GunnsResistorPowerFunction.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name            (--)  Link name.
/// @param[in] nodes           (--)  Network nodes array.
/// @param[in] resistance      (--)  Resistance to flow.
/// @param[in] exponent        (--)  Exponent on the power function.
/// @param[in] useTangentLine  (--)  Flag to enable tangent-line approximation.
///
/// @details  Constructs the Resistor With Power Function Config data.  The base class default
///           conductivity term is not used.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistorPowerFunctionConfigData::GunnsResistorPowerFunctionConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       resistance,
        const double       exponent,
        const bool         useTangentLine)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mResistance(resistance),
    mExponent(exponent),
    mUseTangentLine(useTangentLine)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from.
///
/// @details  Copy Constructs the Resistor With Power Function Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistorPowerFunctionConfigData::GunnsResistorPowerFunctionConfigData(
        const GunnsResistorPowerFunctionConfigData& that)
    :
    GunnsBasicLinkConfigData(that),
    mResistance(that.mResistance),
    mExponent(that.mExponent),
    mUseTangentLine(that.mUseTangentLine)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Resistor With Power Function Config Data Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistorPowerFunctionConfigData::~GunnsResistorPowerFunctionConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag.
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1).
///
/// @details  Default constructs this Resistor With Power Function input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistorPowerFunctionInputData::GunnsResistorPowerFunctionInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from.
///
/// @details  Copy constructs this Resistor With Power Function input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistorPowerFunctionInputData::GunnsResistorPowerFunctionInputData(
        const GunnsResistorPowerFunctionInputData& that)
    :
        GunnsBasicLinkInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Resistor With Power Function input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistorPowerFunctionInputData::~GunnsResistorPowerFunctionInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Resistor With Power Function Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistorPowerFunction::GunnsResistorPowerFunction()
    :
    GunnsBasicLink(NPORTS),
    mResistance(0.0),
    mExponent(0.0),
    mUseTangentLine(false),
    mSystemAdmittance(0.0),
    mSystemSource(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Resistor With Power Function Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsResistorPowerFunction::~GunnsResistorPowerFunction()
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
void GunnsResistorPowerFunction::initialize(const GunnsResistorPowerFunctionConfigData& configData,
                                            const GunnsResistorPowerFunctionInputData&  inputData,
                                            std::vector<GunnsBasicLink*>&               networkLinks,
                                            const int                                   port0,
                                            const int                                   port1)
{
    /// - Initialize the parent class.
    int ports[2] = {port0, port1};
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize class attributes.
    mResistance           = configData.mResistance;
    mExponent             = configData.mExponent;
    mUseTangentLine       = configData.mUseTangentLine;
    mSystemAdmittance     = 0.0;
    mSystemSource         = 0.0;

    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates the link initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistorPowerFunction::validate() const
{
    /// - Issue an error on flow resistance being less than DBL_EPSILON.
    if (mResistance < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "flow resistance < DBL_EPSILON.");
    }

    /// - Issue an error on exponent too close to zero.
    if (fabs(mExponent) < 0.001) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "exponent too near to zero.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistorPowerFunction::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mSystemAdmittance     = 0.0;
    mSystemSource         = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Calls minorStep every pass, since this non-linear link doesn't do anything different
///           between the 1st & subsequent minor steps.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistorPowerFunction::step(const double dt __attribute__((unused)))
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    minorStep(dt, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Not used.
/// @param[in] minorStep (--) Not used.
///
/// @details  Updates this link's contributions to the network system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistorPowerFunction::minorStep(const double dt        __attribute__((unused)),
                                           const int    minorStep __attribute__((unused)))
{
    /// - Link delta-potential is limited to above a minimum for stability in linearization and to
    ///   avoid divide-by-zero below.
    const double dPLimit = std::max(fabs(mPotentialVector[0] - mPotentialVector[1]),
                                    mMinLinearizationPotential);

    /// - Conductance is the inverse of resistance, limited to valid ranges.  Blockage malfunction
    ///   lowers the conductance.
    double gLimit  = std::min(mConductanceLimit,
                              1.0 / std::max(mResistance, 1.0 / mConductanceLimit));
    if (mMalfBlockageFlag) {
        gLimit *= MsMath::limitRange(0.0, (1.0 - mMalfBlockageValue), 1.0);
    }

    /// - Exponent is limited away from zero to avoid instability.
    const double expInv = 1.0 / MsMath::innerLimit(-0.001, mExponent, 0.001);
    const double dPG    = dPLimit * gLimit;

    if (gLimit >= 1.0 / mConductanceLimit) {
        if (mUseTangentLine) {
            /// - Use this tangent-line approximation option with caution.  It takes a lot more
            ///   minor steps to converge, and tends to be unstable when combined with other
            ///   similarly-configured links.  We only included it for completeness.
            ///
            ///     dP = R * i^x,     G = 1/R
            ///     linearize tangent-line to w = A * dP
            ///     (dP*G) = i^x
            ///     i = (dP*G)^(1/x)
            ///     idot = (1/x)*(dP*G)^(1/x - 1) = A
            ///     i = A*dP + w
            ///     w = i - A*dP
            ///
            const double current = powf(dPG, expInv);
            mSystemAdmittance    = expInv * powf(dPG, (expInv - 1.0));
            mSystemSource        = current - mSystemAdmittance * dPLimit;

        } else {
            /// - The is the default, and in our opinion, far better configuration.  The
            ///   linearization passes thru the origin instead of being tangent to the curve
            ///   (similar to GunnsFluidConductor).  This trades accuracy for stability during
            ///   transient events but still converges to the correct solution as the non-linear
            ///   network converges.
            ///
            ///     dP = R * w^x,      G = 1/R
            ///     linearize to w = A * dP
            ///     dP*G = w^x
            ///     (dP*G)^(1/x) = w = A*dP
            ///     A = (dP*G)^(1/x) / dP
            ///
            mSystemAdmittance = powf(dPLimit * gLimit, expInv) / dPLimit;
            mSystemSource     = 0.0;
        }
    } else {
        mSystemAdmittance = 0.0;
        mSystemSource     = 0.0;
    }

    buildAdmittance();
    buildSource();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Method for computing the flows across the link
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistorPowerFunction::computeFlows(const double dt)
{
    mPotentialDrop = getDeltaPotential();
    computeFlux();
    updateFlux(dt, mFlux);
    computePower();
    transportFlux();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the link admittance matrix for inclusion into the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistorPowerFunction::buildAdmittance()
{
    if (fabs(mAdmittanceMatrix[0] - mSystemAdmittance) > 0.0) {
        mAdmittanceMatrix[0]  =  mSystemAdmittance;
        mAdmittanceMatrix[1]  = -mSystemAdmittance;
        mAdmittanceMatrix[2]  = -mSystemAdmittance;
        mAdmittanceMatrix[3]  =  mSystemAdmittance;
        mAdmittanceUpdate     = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the link source vector for inclusion into the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistorPowerFunction::buildSource()
{
    mSourceVector[0] = -mSystemSource;
    mSourceVector[1] =  mSystemSource;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Compute flux across the link, defined positive from port 0 to port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsResistorPowerFunction::computeFlux()
{
    const double hiP = std::max(mPotentialVector[0], mPotentialVector[1]);
    if (fabs(mPotentialDrop) < (hiP * m100EpsilonLimit)) {
        /// - Zero flux if dP is too low.  This eliminates most false quantity leak due to rounding
        ///   error in the solver.
        mFlux = 0.0;
    } else {
        mFlux = mPotentialDrop * mAdmittanceMatrix[0] + mSourceVector[1];
    }
}
