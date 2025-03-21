#include "GunnsLosslessSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name  (--) Link name
/// @param[in] nodes (--) Network nodes array
///
/// @details  Constructs the Lossless Source Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSourceConfigData::GunnsLosslessSourceConfigData(
    const std::string& name,
    GunnsNodeList*     nodes,
    TsLinearInterpolator* efficiencyTable
)
    : GunnsBasicSourceConfigData(name, nodes)
    , mEfficiencyTable(efficiencyTable)
{
    // nothing to do
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy Constructs the Lossless Source Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSourceConfigData::GunnsLosslessSourceConfigData(const GunnsLosslessSourceConfigData& that)
    : GunnsBasicSourceConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Lossless Source Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSourceConfigData::~GunnsLosslessSourceConfigData()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
/// @param[in] sourceFlux        (--) Initial demanded flux of the link
///
/// @details  Default constructs this Basic Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSourceInputData::GunnsLosslessSourceInputData(
    const bool   malfBlockageFlag,
    const double malfBlockageValue,
    const double sourceFlux,
    const double efficiency,
    const double referencePower,
    const bool staticEfficiency
)
    : GunnsBasicSourceInputData(malfBlockageFlag, malfBlockageValue, sourceFlux)
    , mEfficiency(efficiency)
    , mReferencePower(referencePower)
    , mStaticEfficiency(staticEfficiency)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Lossless Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSourceInputData::GunnsLosslessSourceInputData(const GunnsLosslessSourceInputData& that)
    : GunnsBasicSourceInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Lossless Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSourceInputData::~GunnsLosslessSourceInputData()
{
// nothing to do
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default constructs link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSource::GunnsLosslessSource()
    : GunnsBasicSource()
    , mEfficiency(1.0)
    , mWastePower(0.0)
{
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Validates the link initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLosslessSource::validate(const GunnsLosslessSourceConfigData &configData, const GunnsLosslessSourceInputData &inputData)
{
    if (inputData.mSourceFlux < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data", "GunnsLosslessSource sourceFlux must be > 0");
    }
    if (inputData.mEfficiency <= 0.0 || inputData.mEfficiency > 1.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data", "GunnsLosslessSource Efficiency must be in range (0.0, 1.0]");
    }
    if (inputData.mReferencePower <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data", "GunnsLosslessSource reference power must be > 0");
    }
    if (!inputData.mStaticEfficiency && !configData.mEfficiencyTable) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data", "GunnsLosslessSource must use static efficiency if no efficiencyTable is provided (mStaticEfficiency = false && mEfficiencyTable = nullptr -> 🙅‍♂️)");
    }

    // Copied from GunnsElectConverterInput
    if (configData.mEfficiencyTable) {
        /// - Issue an error on table limits out of bounds of valid efficiency (DBL_EPSILON-1).
        ///   Check at every 10% power fraction.
        for (int i=0; i<11; ++i) {
            if (not MsMath::isInRange(DBL_EPSILON, configData.mEfficiencyTable->get(0.1 * i), 1.0)) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "some of the efficiency table is not in valid range (DBL_EPSILON-1)");
            }
        }
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Link Config Data
/// @param[in]     inputData    (--) Link Input Data
/// @param[in,out] networkLinks (--) Reference to the Solver Links
/// @param[in]     port0        (--) Port 0 Node Mapping
/// @param[in]     port1        (--) Port 1 Node Mapping
///
/// @throws   TsInitializationException
///
/// @details This initializes the link and sets up its connectivity to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLosslessSource::initialize(
    const GunnsLosslessSourceConfigData& configData,
    const GunnsLosslessSourceInputData&  inputData,
    std::vector<GunnsBasicLink*>&     networkLinks,
    const int                         port0,
    const int                         port1
)
{
  GunnsBasicSource::initialize(configData, inputData, networkLinks, port0, port1);
  this->mSourceFlux = inputData.mSourceFlux;
  this->mEfficiency = inputData.mEfficiency;
  this->mReferencePower = inputData.mReferencePower;
  this->mStaticEfficiency = inputData.mStaticEfficiency;

  this->mEfficiencyTable = configData.mEfficiencyTable;
  this->mInitFlag = true;
}

/// @brief Computes mPower for the aspect-specific implementation
void GunnsLosslessSource::computePower() {
    // mPotentialVector should always be (+)
    double powerOut = mPotentialVector[1]*mSourceVector[1]; // mSourceVector[1] (I_out) should always be (+)
    double powerIn  = mPotentialVector[0]*mSourceVector[0]; // mSourceVector[0] (I_in) should always be (-)
    mPower = powerIn + powerOut;
    mWastePower = mPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Makes calls to accumulate flux in the input & output terms of the receiving and
///           sending nodes, respectively.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLosslessSource::transportFlux() {
    /** Ignore the params because we only go from 0 to 1, but want to override the GunnsBasicLink::transportFlux */
    /**
     * mNodes[0].flux = mNodes[1].flux*mNodes[1].potential/mNodes[0].potential
    */
    // I think I can just use the mSourceVector instead of working from mSourceFlux here
    // double fluxOnInputNode = -1*mSourceFlux*mNodes[1]->getPotential()/mNodes[0]->getPotential();
    double fluxOnInputNode = this->mSourceVector[0];
    if (mSourceFlux > 0.0) {
        mNodes[0]->collectOutflux(std::fabs(fluxOnInputNode));
        mNodes[1]->collectInflux (std::fabs(mSourceFlux));

    } else if (mSourceFlux < 0.0) {
        mNodes[1]->collectOutflux(std::fabs(mSourceFlux));
        mNodes[0]->collectInflux (std::fabs(fluxOnInputNode));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Ignores inputs and calls normal `transportFlux`
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLosslessSource::transportFlux(const int, const int)
{
    this->transportFlux();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Computes the flows across the link
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLosslessSource::computeFlows(const double dt)
{
    mPotentialDrop = getDeltaPotential();
    computePower();
    transportFlux();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Builds the source vector terms of the links contribution to the network.  This sign
///          convention creates positive flow from port 0 to port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLosslessSource::buildSource()
{
  /**
   * Unlike GunnsBasicSource, this link tries to satisfy the eqn
   * Pot_in*Flux_in = Pot_out*Flux_out
   * If the potential on input is 0, we divide by DBL_EPSILON instead
  */

    // First update efficiency if we're supposed to and have a proper eff table 
    this->mOutputPower = this->mSourceFlux * this->mPotentialVector[1];
    this->mEfficiency = this->estimateEfficiencyAtLoad(this->mOutputPower);
    this->mInputPower = this->mOutputPower / this->mEfficiency;
    double inputFlux = this->mInputPower / this->mPotentialVector[0];

    if (mPotentialVector[0] == 0.0) {
      mPotentialVector[0] = __DBL_EPSILON__;
      mOverrideVector[0] = true;
    }
    // Apply mEfficiency to mSourceVector[0]
    // I_in*V_in = I_out*V_out/mEfficiency
    if (mSourceFlux != 0.0) {
        mSourceVector[0] =  -1 * inputFlux;
        mSourceVector[1] = mSourceFlux;
    } else {
        mSourceVector[0] = 0.0;
        mSourceVector[1] = 0.0;
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Dtor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSource::~GunnsLosslessSource()
{
 // nothing to do
}
