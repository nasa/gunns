/**
@file
@brief    GUNNS Fluid External Demand Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Fluid External Demand)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (
    (GunnsFluidPotential.o)
   )

 PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2011-02) (Initial Prototype))
    )
*/
#include "GunnsFluidExternalDemand.hh"
#include "core/GunnsFluidUtils.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                  (--)  Link name
/// @param[in] nodes                 (--)  Network nodes array
/// @param[in] maxConductivity       (m2)  Maximum possible effective conductivity of the link
/// @param[in] expansionScaleFactor  (--)  Isentropic gas expansion scale factor
/// @param[in] filterMinConductivity (m2)  Minimum filtered effective conductivity of the link
/// @param[in] filterMinDeltaP       (kPa) Minimum filtered supply delta pressure
/// @param[in] filterCapacitanceGain (--)  Gain for estimated capacitance filter (0-1)
/// @param[in] externalConfig        (--)  Pointer to the external network fluid config
/// @param[in] convertToType         (--)  Fluid type to convert extra external constituents into
///
/// @details  Default GUNNS Fluid External Demand Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalDemandConfigData::GunnsFluidExternalDemandConfigData(
        const std::string&         name,
        GunnsNodeList*             nodes,
        const double               maxConductivity,
        const double               expansionScaleFactor,
        const double               filterMinConductivity,
        const double               filterMinDeltaP,
        const double               filterCapacitanceGain,
        PolyFluidConfigData*       externalConfig,
        FluidProperties::FluidType convertToType)
    :
    GunnsFluidPotentialConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mFilterMinConductivity(filterMinConductivity),
    mFilterMinDeltaP(filterMinDeltaP),
    mFilterCapacitanceGain(filterCapacitanceGain),
    mExternalConfig(externalConfig),
    mConvertToType(convertToType)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  GUNNS Fluid External Demand Config Data Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalDemandConfigData::GunnsFluidExternalDemandConfigData(
        const GunnsFluidExternalDemandConfigData& that)
    :
    GunnsFluidPotentialConfigData(that),
    mFilterMinConductivity       (that.mFilterMinConductivity),
    mFilterMinDeltaP             (that.mFilterMinDeltaP),
    mFilterCapacitanceGain       (that.mFilterCapacitanceGain),
    mExternalConfig              (that.mExternalConfig),
    mConvertToType               (that.mConvertToType)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid External Demand Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalDemandConfigData::~GunnsFluidExternalDemandConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag      (--)         Blockage malfunction flag
/// @param[in] malfBlockageValue     (--)         Blockage malfunction fractional value (0-1)
/// @param[in] sourcePressure        (kPa)        Initial pressure rise of the link
/// @param[in] supplyCapacitance     (kg*mol/kPa) Initial supply temperature
/// @param[in] supplyTemperature     (K)          Initial supply temperature
/// @param[in] supplyMassFractions   (--)         Initial supply mass fractions
/// @param[in] supplyTcMoleFractions (--)         Initial supply trace compounds mole fractions
///
/// @details  Default GUNNS Fluid External Demand Input Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalDemandInputData::GunnsFluidExternalDemandInputData(
        const bool    malfBlockageFlag,
        const double  malfBlockageValue,
        const double  sourcePressure,
        const double  supplyCapacitance,
        const double  supplyTemperature,
              double* supplyMassFractions,
              double* supplyTcMoleFractions)
    :
    GunnsFluidPotentialInputData(malfBlockageFlag, malfBlockageValue, sourcePressure),
    mSupplyCapacitance    (supplyCapacitance),
    mSupplyTemperature    (supplyTemperature),
    mSupplyMassFractions  (supplyMassFractions),
    mSupplyTcMoleFractions(supplyTcMoleFractions)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy
///
/// @details  Copy constructs this Fluid External Demand input data.
///
/// @note  This is a shallow copy, mSupplyMassFractions will point to the same address as the source
///        object's pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalDemandInputData::GunnsFluidExternalDemandInputData(
        const GunnsFluidExternalDemandInputData& that)
    :
    GunnsFluidPotentialInputData(that),
    mSupplyCapacitance    (that.mSupplyCapacitance),
    mSupplyTemperature    (that.mSupplyTemperature),
    mSupplyMassFractions  (that.mSupplyMassFractions),
    mSupplyTcMoleFractions(that.mSupplyTcMoleFractions)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid External Demand Input Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalDemandInputData::~GunnsFluidExternalDemandInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid External Demand Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalDemand::GunnsFluidExternalDemand()
    :
    GunnsFluidPotential   (),
    mFilterMinConductivity(0),
    mFilterMinDeltaP      (0),
    mTransformMap         (0),
    mAvgDemand            (0.0),
    mAvgSupplyP           (0.0),
    mAvgSupplyDeltaP      (0.0),
    mEstimatedCapacitance (0.0),
    mFilterCapacitanceGain(0.0),
    mSupplyCapacitance    (0.0),
    mSupplyPressure       (0.0),
    mSupplyTemperature    (0.0),
    mSupplyMassFractions  (0),
    mSupplyTcMoleFractions(0),
    mDemandFlux           (0.0),
    mDemandTemperature    (0.0),
    mDemandMassFractions  (0),
    mDemandTcMoleFractions(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid External Demand Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalDemand::~GunnsFluidExternalDemand()
{
    TS_DELETE_ARRAY(mSupplyTcMoleFractions);
    TS_DELETE_ARRAY(mDemandMassFractions);
    TS_DELETE_ARRAY(mSupplyMassFractions);
    {
        delete [] mTransformMap;
        mTransformMap = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to link Config Data
/// @param[in]     inputData    (--) Reference to link Input Data
/// @param[in,out] networkLinks (--) Network links vector
/// @param[in]     port0        (--) Network port 0
/// @param[in]     port1        (--) Network port 1
///
/// @throws   TsInitializationException
///
/// @details  This method initializes this Fluid External Demand link with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalDemand::initialize(const GunnsFluidExternalDemandConfigData& configData,
                                          const GunnsFluidExternalDemandInputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             networkLinks,
                                          const int                                 port0,
                                          const int                                 port1)
{
    /// - Initialize & validate parent.
    GunnsFluidPotential::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag
    mInitFlag = false;

    /// - Initialize from config data.
    mFilterMinConductivity = configData.mFilterMinConductivity;
    mFilterMinDeltaP       = configData.mFilterMinDeltaP;
    mFilterCapacitanceGain = configData.mFilterCapacitanceGain;
    mAvgDemand             = 0.0;
    mAvgSupplyP            = 0.0;
    mAvgSupplyDeltaP       = 0.0;
    mEstimatedCapacitance  = 0.0;

    /// - Allocate memory and build the transform map array.
    delete [] mTransformMap;
    mTransformMap = new int[configData.mExternalConfig->mNTypes + 1];

    GunnsFluidUtils::buildTransformMap(mTransformMap, configData.mExternalConfig,
            mNodes[1]->getFluidConfig(), configData.mConvertToType);

    /// - Allocate memory for the read & write data mass fraction arrays.
    TS_DELETE_ARRAY(mSupplyMassFractions);
    TS_DELETE_ARRAY(mDemandMassFractions);
    TS_NEW_PRIM_ARRAY_EXT(mSupplyMassFractions, configData.mExternalConfig->mNTypes,  double, std::string(mName) + ".mSupplyMassFractions");
    TS_NEW_PRIM_ARRAY_EXT(mDemandMassFractions, mNodes[1]->getFluidConfig()->mNTypes, double, std::string(mName) + ".mDemandMassFractions");

    /// - Allocate memory for the supply trace compounds mole fraction array.  Note this remains
    ///   un-allocated if the network has no trace compounds config or zero compounds in the config.
    ///   The demand trace compounds mole fraction array points directly to the node's array.
    const GunnsFluidTraceCompoundsConfigData* tcConfig = mNodes[1]->getFluidConfig()->mTraceCompounds;
    if (tcConfig) {
        const int nTc = tcConfig->mNTypes;
        if (nTc > 0) {
            TS_DELETE_ARRAY(mSupplyTcMoleFractions);
            TS_NEW_PRIM_ARRAY_EXT(mSupplyTcMoleFractions, nTc, double, std::string(mName) + ".mSupplyTcMoleFractions");
        }
        mDemandTcMoleFractions = mNodes[1]->getContent()->getTraceCompounds()->getMoleFractions();
    }

    /// - Initialize the output demand terms.
    mDemandFlux        = 0.0;
    mDemandTemperature = mNodes[1]->getContent()->getTemperature();
    for (int i=0; i<mNodes[1]->getFluidConfig()->mNTypes; ++i) {
        mDemandMassFractions[i] = mNodes[1]->getContent()->getMassFraction(i);
    }

    /// - Initialize the input supply terms.  Normally these will be overwritten by a supply link
    ///   via simbus, but initializing here allows this link to be run standalone if desired.  We
    ///   use the node's temperature if the input data temperature is not specified, {1.0, 0.0 ...}
    ///   mass fractions if the input data fractions are not specified, and zero trace compounds
    ///   mole fractions if they're not specified.  We use zero supply capacitance if the input data
    ///   capacitance is not specified.
    mSupplyPressure    = mSourcePressure;
    if (inputData.mSupplyCapacitance > DBL_EPSILON) {
        mSupplyCapacitance = inputData.mSupplyCapacitance;
    } else {
        mSupplyCapacitance = 0.0;
    }
    if (inputData.mSupplyTemperature > DBL_EPSILON) {
        mSupplyTemperature = inputData.mSupplyTemperature;
    } else {
        mSupplyTemperature = mNodes[1]->getContent()->getTemperature();
    }
    if (inputData.mSupplyMassFractions) {
        for (int i=0; i<configData.mExternalConfig->mNTypes; ++i) {
            mSupplyMassFractions[i] = inputData.mSupplyMassFractions[i];
        }
    } else {
        mSupplyMassFractions[0] = 1.0;
        for (int i=1; i<configData.mExternalConfig->mNTypes; ++i) {
            mSupplyMassFractions[i] = 0.0;
        }
    }
    if (mSupplyTcMoleFractions) {
        if (inputData.mSupplyTcMoleFractions) {
            for (int i=0; i<tcConfig->mNTypes; ++i) {
                mSupplyTcMoleFractions[i] = inputData.mSupplyTcMoleFractions[i];
            }
        } else {
            for (int i=0; i<tcConfig->mNTypes; ++i) {
                mSupplyTcMoleFractions[i] = 0.0;
            }
        }
    }

    /// - Initialize the link conductivity.
    mEffectiveConductivity = mFilterMinConductivity;
    if (mMalfBlockageFlag) {
        mEffectiveConductivity *= (1.0 - mMalfBlockageValue);
    }

    /// - Validate initialization.
    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Validates this GUNNS Fluid External Demand initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalDemand::validate() const
{
    /// - There is currently nothing to validate.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalDemand::restartModel()
{
    /// - Reset the base class.
    GunnsFluidPotential::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mAvgSupplyDeltaP = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Handles data read from the external network's supply link, via the sim bus.  Data is
///          moved from the sim bus input members into the link internal members.  If the supply
///          network runs at a different rate than this network, we always want the latest supply
///          properties regardless of how often it runs, therefore we don't use a queue in the sim
///          bus.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalDemand::processInputs()
{
    /// - Demand nodes should have no capacitance or mass errors will result, so check for this and
    ///   throw a warning.  This is checked every pass rather than on port changes since volumes may
    ///   change to & from zero during run-time.
    if (mNodes[1]->getVolume() > DBL_EPSILON) {
        GUNNS_WARNING("detected volume in the demand node, mass errors may result.");
    }

    /// - If the link is fully blocked, it should isolate this network from the supply network.
    if (mEffectiveConductivity > DBL_EPSILON) {
        setSourcePressure(mSupplyPressure);
        try {
            GunnsFluidUtils::transformState(mNodes[1]->getContent(), mSupplyPressure,
                    mSupplyTemperature, mSupplyMassFractions, mTransformMap,
                    mSupplyTcMoleFractions);
        } catch (TsOutOfBoundsException &e) {
            GUNNS_WARNING("caught exception from GunnsFluidUtils::transformState.");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Handles data written to the external network's supply link, via the sim bus.  Data is
///          moved from the demand node into the sim bus output members.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalDemand::processOutputs()
{
    mDemandFlux = mFlux;

    /// - Before copying the node's inflow into our demand, first make sure it has valid mass
    ///   fractions.  Sometimes when flow rates are near the lower limit, rates may indicate there
    ///   is flow even though no links moved mass into the node's inflow, leaving the inflow
    ///   fractions zero.
    double sum = 0.0;
    for (int i = 0; i < mNodes[1]->getContent()->getNConstituents(); ++i) {
        mDemandMassFractions[i] = mNodes[1]->getInflow()->getMassFraction(i);
        sum += mDemandMassFractions[i];
    }

    /// - When flux is negative, we're flowing from the demand side to the supply side.  The demand
    ///   node's contents is stuffed with the supply node's contents every pass, so we have to look
    ///   at the inflows into the demand node to get the real properties of the fluid leaving the
    ///   demand network.
    if (mFlowRate < -m100EpsilonLimit && sum > static_cast<double>(FLT_EPSILON)) {
        mDemandTemperature = mNodes[1]->getInflow()->getTemperature();
        mFlowRate          = mNodes[1]->getInflow()->getMWeight() * mFlux;
    }

    /// - When flux is positive, flowing from the supply network to the demand network, the supply
    ///   network doesn't actually need the demand fluid properties, but we need to populate the
    ///   sim bus data with something - so we just repeat the demand node contents back.
    else {
        mDemandTemperature = mNodes[1]->getContent()->getTemperature();
        for (int i = 0; i < mNodes[1]->getContent()->getNConstituents(); ++i) {
            mDemandMassFractions[i] = mNodes[1]->getContent()->getMassFraction(i);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Updates the State of the Link during the step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalDemand::updateState(const double dt)
{
    ///   Since Aspect Architecture allows up to 1 frame of lag, do a 2-frame moving average of
    ///   our demand and the supply pressure.  Cut off average demand below a certain amount to
    ///   avoid dirty zeroes whem mFlux is zero.
    mAvgDemand       = 0.5 * (mAvgDemand + mFlux);
    if (fabs(mAvgDemand) < DBL_EPSILON) {
        mAvgDemand = 0.0;
    }

    mAvgSupplyDeltaP = -mAvgSupplyP;
    mAvgSupplyP      = 0.5 * (mAvgSupplyP + mSourcePressure);
    mAvgSupplyDeltaP += mAvgSupplyP;
    if (fabs(mAvgSupplyP) < DBL_EPSILON) {
        mAvgSupplyP = 0.0;
    }

    /// - Update our estimate of the supply network's effective capacitance:  C = I dt / dP
    if (fabs(mAvgSupplyDeltaP) > mFilterMinDeltaP) {
        mEstimatedCapacitance = (1.0 - mFilterCapacitanceGain) * mEstimatedCapacitance
                              + mFilterCapacitanceGain * (-mAvgDemand * dt / mAvgSupplyDeltaP);
        if (fabs(mEstimatedCapacitance) < DBL_EPSILON) {
            mEstimatedCapacitance = 0.0;
        }
    }

    /// - Filter our effective conductivity towards the supply capacitance when our demand is
    ///   increasing:  G = C/dt.  Prefer the supply network's given capacitance over the internal
    ///   estimated capacitance when it is available.
    if (dt > DBL_EPSILON) {
        if (mSupplyCapacitance > DBL_EPSILON) {
            mEffectiveConductivity = mSupplyCapacitance / dt;
        } else {
            mEffectiveConductivity = std::max(mEstimatedCapacitance / dt, mFilterMinConductivity);
        }
    } else {
        mEffectiveConductivity = mFilterMinConductivity;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (kg*mol/kPa/s)  Linearized molar conductance.
///
/// @details  As the effective conductivity is derived from the estimated supply network's
///           capacitance in updateState, we want this to go straight into the admittance matrix,
///           without the fluid flow linearization that the base class uses.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidExternalDemand::linearizeConductance()
{
    return mEffectiveConductivity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned
/// @param[in] node (--) The desired node to assign the port to
///
/// @returns  bool  (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsFluidExternalDemand must map port 0 to the network vacuum node.
///           - A GunnsFluidExternalDemand must not map port 1 to the network vacuum node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidExternalDemand::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 1 is the vacuum node.
    if ((1 == port) && (node == getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 1 to the boundary node.");
        result = false;
    }

    /// - Fail if port 0 is not the vacuum node.
    else if ((0 == port) && (node != getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: must assign port 0 to the boundary node.");
        result = false;
    }

    return result;
}
