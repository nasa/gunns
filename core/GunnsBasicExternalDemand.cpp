/**
@file
@brief    GUNNS Basic External Demand Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Basic External Demand)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (
    (GunnsBasicPotential.o)
   )

 PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2011-02) (Initial Prototype))
    )
*/
#include "GunnsBasicExternalDemand.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                  (--) Link name
/// @param[in] nodes                 (--) Network nodes array
/// @param[in] defaultConductivity   (--) Default conductivity of the link
/// @param[in] filterMinConductivity (--) Minimum filtered effective conductivity
/// @param[in] filterMinDeltaP       (--) Minimum delta-potential for capacitance estimate
/// @param[in] filterCapacitanceGain (--) Gain for estimated capacitance filter (0-1)
///
/// @details  Default GUNNS Basic External Demand Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalDemandConfigData::GunnsBasicExternalDemandConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       defaultConductivity,
        const double       filterMinConductivity,
        const double       filterMinDeltaP,
        const double       filterCapacitanceGain)
    :
    GunnsBasicPotentialConfigData(name, nodes, defaultConductivity),
    mFilterMinConductivity(filterMinConductivity),
    mFilterMinDeltaP(filterMinDeltaP),
    mFilterCapacitanceGain(filterCapacitanceGain)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  GUNNS Basic External Demand Config Data Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalDemandConfigData::GunnsBasicExternalDemandConfigData(
        const GunnsBasicExternalDemandConfigData& that)
    :
    GunnsBasicPotentialConfigData(that),
    mFilterMinConductivity       (that.mFilterMinConductivity),
    mFilterMinDeltaP             (that.mFilterMinDeltaP),
    mFilterCapacitanceGain       (that.mFilterCapacitanceGain)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Basic External Demand Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalDemandConfigData::~GunnsBasicExternalDemandConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
/// @param[in] sourcePotential   (--) Initial potential rise of the link
///
/// @details  Default constructs this Basic External Demand input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalDemandInputData::GunnsBasicExternalDemandInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue,
        const double sourcePotential)
    :
    GunnsBasicPotentialInputData(malfBlockageFlag, malfBlockageValue, sourcePotential)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Basic External Demand input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalDemandInputData::GunnsBasicExternalDemandInputData(
        const GunnsBasicExternalDemandInputData& that)
    :
    GunnsBasicPotentialInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Basic External Demand input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalDemandInputData::~GunnsBasicExternalDemandInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Basic External Demand Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalDemand::GunnsBasicExternalDemand()
    :
    GunnsBasicPotential(),
    mFilterMinConductivity(0),
    mFilterMinDeltaP(0),
    mAvgDemand(0.0),
    mAvgSupplyP(0.0),
    mAvgSupplyDeltaP(0.0),
    mEstimatedCapacitance(0.0),
    mFilterCapacitanceGain(0.0),
    mSupplyPotential(0.0),
    mDemandFlux(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Basic External Demand Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicExternalDemand::~GunnsBasicExternalDemand()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) GUNNS Basic External Demand Config Data
/// @param[in]     inputData    (--) GUNNS Basic Conductor Input Data
/// @param[in,out] networkLinks (--) Network links
/// @param[in]     port0        (--) Network port 0
/// @param[in]     port1        (--) Network port 1
///
/// @throws   TsInitializationException
///
/// @details  This overloaded method initializes this Basic External Demand link with configuration
///           and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicExternalDemand::initialize(const GunnsBasicExternalDemandConfigData& configData,
                                          const GunnsBasicExternalDemandInputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             networkLinks,
                                          const int                                 port0,
                                          const int                                 port1)
{
    /// - Initialize & validate parent.
    GunnsBasicPotential::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset the init flag.
    mInitFlag = true;

    /// - Initialize from config data.
    mFilterMinConductivity = configData.mFilterMinConductivity;
    mFilterMinDeltaP       = configData.mFilterMinDeltaP;
    mFilterCapacitanceGain = configData.mFilterCapacitanceGain;
    mAvgDemand             = 0.0;
    mAvgSupplyP            = 0.0;
    mAvgSupplyDeltaP       = 0.0;
    mEstimatedCapacitance  = 0.0;

    /// - Validate initialization.
    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Basic External Demand initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicExternalDemand::validate() const
{
    /// - There is currently nothing to check.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicExternalDemand::restartModel()
{
    /// - Reset the base class.
    GunnsBasicPotential::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mAvgSupplyDeltaP = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Updates the State of the Link during the step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicExternalDemand::updateState(const double dt)
{
    ///   Since Aspect Architecture allows up to 1 frame of lag, do a 2-frame moving average of
    ///   our demand and the supply potential.  Cut off average demand below a certain amount to
    ///   avoid dirty zeroes whem mFlux is zero.
    mAvgDemand       = 0.5 * (mAvgDemand + mFlux);
    if (fabs(mAvgDemand) < DBL_EPSILON) {
        mAvgDemand = 0.0;
    }
    mAvgSupplyDeltaP = -mAvgSupplyP;
    mAvgSupplyP      = 0.5 * (mAvgSupplyP + mSourcePotential);
    mAvgSupplyDeltaP += mAvgSupplyP;

    /// - Update our estimate of the supply network's effective capacitance:  C = I dt / dP
    if (fabs(mAvgSupplyDeltaP) > mFilterMinDeltaP) {
        mEstimatedCapacitance = (1.0 - mFilterCapacitanceGain) * mEstimatedCapacitance
                              + mFilterCapacitanceGain * (-mAvgDemand * dt / mAvgSupplyDeltaP);
    }

    /// - Filter our effective conductivity towards the supply capacitance when our demand is
    ///   increasing:  G = C/dt
    if (dt > DBL_EPSILON) {
        mEffectiveConductivity = std::max(mEstimatedCapacitance / dt, mFilterMinConductivity);
    } else {
        mEffectiveConductivity = mFilterMinConductivity;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned
/// @param[in] node (--) The desired node to assign the port to
///
/// @returns  bool  (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsBasicExternalDemand must map port 0 to the network ground node.
///           - A GunnsBasicExternalDemand must not map port 1 to the network ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsBasicExternalDemand::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 1 is the ground node.
    if ((1 == port) && (node == getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 1 to the boundary node.");
        result = false;
    }

    /// - Fail if port 0 is not the ground node.
    else if ((0 == port) && (node != getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: must assign port 0 to the boundary node.");
        result = false;
    }

    return result;
}
