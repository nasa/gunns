/**
@file     GunnsFluidTypeChangeConductor.cpp
@brief    GUNNS Fluid Type Change Conductor Model implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o))
*/

#include "GunnsFluidTypeChangeConductor.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name             (--)  Name of object.
/// @param[in]  nodes            (--)  Pointer to nodes.
/// @param[in]  maxConductivity  (m2)  Max conductivity.
/// @param[in]  fluidTypesPort0  (--)  Fluid types to be converted at the port 0 node.
/// @param[in]  fluidTypesPort1  (--)  Fluid types to be converted at the port 1 node.
///
/// @details  Default constructs this GUNNS Fluid Type Change Conductor link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTypeChangeConductorConfigData::GunnsFluidTypeChangeConductorConfigData(
        const std::string&                       name,
        GunnsNodeList*                           nodes,
        const double                             maxConductivity,
        std::vector<FluidProperties::FluidType>* fluidTypesPort0,
        std::vector<FluidProperties::FluidType>* fluidTypesPort1)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, 0.0),
    mFluidTypesPort0(),
    mFluidTypesPort1()
{
    /// - Clear the vectors.
    mFluidTypesPort0.clear();
    mFluidTypesPort1.clear();

    /// - Check for null pointer and set the vectors.
    if (fluidTypesPort0)
    {
        mFluidTypesPort0 = *fluidTypesPort0;
    }
    if (fluidTypesPort1)
    {
        mFluidTypesPort1 = *fluidTypesPort1;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Type Change Conductor link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTypeChangeConductorConfigData::GunnsFluidTypeChangeConductorConfigData(
        const GunnsFluidTypeChangeConductorConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mFluidTypesPort0(that.mFluidTypesPort0),
    mFluidTypesPort1(that.mFluidTypesPort1)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Type Change Conductor link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTypeChangeConductorConfigData::~GunnsFluidTypeChangeConductorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
///
/// @details  Default constructs this GUNNS Fluid Type Change Conductor link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTypeChangeConductorInputData::GunnsFluidTypeChangeConductorInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Type Change Conductor link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTypeChangeConductorInputData::GunnsFluidTypeChangeConductorInputData(
        const GunnsFluidTypeChangeConductorInputData& that)
    :
    GunnsFluidConductorInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Type Change Conductor link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTypeChangeConductorInputData::~GunnsFluidTypeChangeConductorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Type Change Conductor link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTypeChangeConductor::GunnsFluidTypeChangeConductor()
    :
    GunnsFluidConductor(),
    mNumTypes(0),
    mIndexPort0(0),
    mIndexPort1(0),
    mWorkMoleFractions(0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Type Change Conductor link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidTypeChangeConductor::~GunnsFluidTypeChangeConductor()
{
    /// - Delete the allocated memory.
    TS_DELETE_ARRAY(mWorkMoleFractions);
    TS_DELETE_ARRAY(mIndexPort1);
    TS_DELETE_ARRAY(mIndexPort0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
/// @param[in,out]  links       (--)  Link vector.
/// @param[in]      port0       (--)  Nominal inlet port map index.
/// @param[in]      port1       (--)  Nominal outlet port map index.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Type Change Conductor link with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTypeChangeConductor::initialize(
        const GunnsFluidTypeChangeConductorConfigData& configData,
        const GunnsFluidTypeChangeConductorInputData&  inputData,
        std::vector<GunnsBasicLink*>&                   links,
        const int                                       port0,
        const int                                       port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    validate(configData);

    mNumTypes = configData.mFluidTypesPort0.size();
    if (mNumTypes > 0) {
        TS_NEW_PRIM_ARRAY_EXT(mIndexPort0,        mNumTypes, int,    mName+".mIndexPort0");
        TS_NEW_PRIM_ARRAY_EXT(mIndexPort1,        mNumTypes, int,    mName+".mIndexPort1");

        for (unsigned int i=0; i<mNumTypes; ++i) {
            mIndexPort0[i] = configData.mNodeList->mNodes[0].getContent()->find(configData.mFluidTypesPort0.at(i));
            mIndexPort1[i] = configData.mNodeList->mNodes[0].getContent()->find(configData.mFluidTypesPort1.at(i));
        }
    }

    const int nConstituents = mNodes[0]->getContent()->getNConstituents();
    TS_NEW_PRIM_ARRAY_EXT(mWorkMoleFractions, nConstituents, double, mName+".mWorkMoleFractions");

    for (int i=0; i<nConstituents; ++i) {
        mWorkMoleFractions[i] = 0.0;
    }

    createInternalFluid();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag          = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Configuration data.
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Type Change Conductor link initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTypeChangeConductor::validate(
        const GunnsFluidTypeChangeConductorConfigData& configData) const
{
    /// - Throw an exception on size mismatch between port types.
    const unsigned int numTypes   = configData.mFluidTypesPort0.size();
    if (numTypes != configData.mFluidTypesPort1.size()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mismatch in port 0 and 1 types vector sizes.");
    }

    for (unsigned int i=0; i<numTypes; ++i) {
        /// - Throw an exception for duplicated port 0 types.
        for (unsigned int j=i+1; j<numTypes; ++j) {
            if (configData.mFluidTypesPort0.at(i) == configData.mFluidTypesPort0.at(j)) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "duplicated types for port 0.");
            }
        }

        /// - Throw an exception for duplicated port 1 types.
        for (unsigned int j=i+1; j<numTypes; ++j) {
            if (configData.mFluidTypesPort1.at(i) == configData.mFluidTypesPort1.at(j)) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "duplicated types for port 1.");
            }
        }

        /// - Throw an exception for a type in both lists.
        for (unsigned int j=0; j<numTypes; ++j) {
            if (configData.mFluidTypesPort0.at(i) == configData.mFluidTypesPort1.at(j) or
                configData.mFluidTypesPort1.at(i) == configData.mFluidTypesPort0.at(j)) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "a type is converted in both ports.");
            }
        }

        /// - Throw an exception on fluid type not in the network.
        try {
            configData.mNodeList->mNodes[0].getContent()->find(configData.mFluidTypesPort0.at(i));
            configData.mNodeList->mNodes[0].getContent()->find(configData.mFluidTypesPort1.at(i));
        } catch (const TsOutOfBoundsException& e){
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "a conversion fluid type is not in the network.");
        }

        /// - Throw an exception if the fluid type doesn't match the phase of the node.
        const DefinedFluidProperties* netFluidProperties = mNodes[0]->getFluidConfig()->mProperties;
        {
            const FluidProperties::FluidPhase phase =
                    netFluidProperties->getProperties(configData.mFluidTypesPort0.at(i))->getPhase();
            if (phase != mNodes[0]->getContent()->getPhase()) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "fluid phase mismatch between a port 0 type and its the port 0 node.");
            }
        } {
            const FluidProperties::FluidPhase phase =
                    netFluidProperties->getProperties(configData.mFluidTypesPort1.at(i))->getPhase();
            if (phase != mNodes[1]->getContent()->getPhase()) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "fluid phase mismatch between a port 1 type and its the port 1 node.");
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTypeChangeConductor::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] forcedOutflow (--) not used.
/// @param[in] fromPort      (--) not used.
/// @param[in] toPort        (--) not used.
///
/// @details  The fluid added to the downstream node is the same as the outflow from the upstream
///           node, except we transfer the mixture fractions of the upstream port conversion types
///           to the downstream node's conversion type.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidTypeChangeConductor::transportFluid(const bool forcedOutflow __attribute__((unused)),
                                                   const int  fromPort __attribute__((unused)),
                                                   const int  toPort __attribute__((unused)))
{
    int nConstituents = mNodes[0]->getContent()->getNConstituents();
    if (mFlowRate > m100EpsilonLimit) {

        // set work mole fractions equal to upstream node's outflow fractions
        for (int i=0; i<nConstituents; ++i) {
            mWorkMoleFractions[i] = mNodes[0]->getOutflow()->getMoleFraction(i);
        }

        // loop i over mNumTypes:
        for (unsigned int i=0; i<mNumTypes; ++i) {
            //   set work mole fractions[mIndexPortDown[i]] equal to work mole fractions[mIndexPortUp[i]]
            mWorkMoleFractions[mIndexPort1[i]] = mNodes[0]->getOutflow()->getMoleFraction(mIndexPort0[i]);
            //   zero work mole fractions[mIndexPortUp[i]]
            mWorkMoleFractions[mIndexPort0[i]] = 0.0;
        }

        // Finish up internal fluid with new mixture:
        mInternalFluid->setMoleAndMoleFractions(mNodes[0]->getOutflow()->getMole(), mWorkMoleFractions);
        mInternalFluid->setTemperature(mNodes[0]->getOutflow()->getTemperature());

        // transport the flows
        mNodes[0]->collectOutflux(mFlowRate);
        mNodes[1]->collectInflux(mFlowRate, mInternalFluid);

    } else if (mFlowRate < -m100EpsilonLimit) {
        // Same as above but in other flow direction
        for (int i=0; i<nConstituents; ++i) {
            mWorkMoleFractions[i] = mNodes[1]->getOutflow()->getMoleFraction(i);
        }

        for (unsigned int i=0; i<mNumTypes; ++i) {
            mWorkMoleFractions[mIndexPort0[i]] = mNodes[1]->getOutflow()->getMoleFraction(mIndexPort1[i]);
            mWorkMoleFractions[mIndexPort1[i]] = 0.0;
        }

        mInternalFluid->setMoleAndMoleFractions(mNodes[1]->getOutflow()->getMole(), mWorkMoleFractions);
        mInternalFluid->setTemperature(mNodes[1]->getOutflow()->getTemperature());

        mNodes[1]->collectOutflux(-mFlowRate);
        mNodes[0]->collectInflux(-mFlowRate, mInternalFluid);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned.
/// @param[in] node (--) The desired node to assign the port to.
///
/// @return   bool  (--) Returns true if all rules checks pass.
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsFluidTypeChangeConductor cannot map a port to a non-Ground node whose fluid
///             phase differs from any of the conversion types for this port.
///
/// @note  This won't check during initialization because the conversion types haven't been
///        initialized from config data yet.  So this kind of conflict must also be checked in the
///        validate method.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidTypeChangeConductor::checkSpecificPortRules(const int port, const int node) const
{
    bool                              result             = true;
    GunnsFluidNode*                   nodePtr            = dynamic_cast<GunnsFluidNode*> (mNodeList->mNodes);
    const PolyFluidConfigData*        netFluidConfig     = nodePtr[node].getFluidConfig();
    const DefinedFluidProperties*     netFluidProperties = netFluidConfig->mProperties;
    const FluidProperties::FluidPhase nodePhase          = nodePtr[node].getContent()->getPhase();

    if ( (0 == port) and (node != getGroundNodeIndex()) ) {
        /// - Fail if port 0 is not Ground and fluid phase conflicts with the port 0 conversion
        ///   types.
        for (unsigned int i=0; i<mNumTypes; ++i) {
            const FluidProperties::FluidPhase typePhase =
                    netFluidProperties->getProperties(netFluidConfig->mTypes[mIndexPort0[i]])->getPhase();
            if (typePhase != nodePhase) {
                GUNNS_WARNING("aborted setting port 0: fluid phase of conversion types doesn't match the node's phase.");
                result = false;
            }
        }
    } else if ( (1 == port) and (node != getGroundNodeIndex()) ) {
        /// - Fail if port 1 is not Ground and fluid phase conflicts with the port 1 conversion
        ///   types.
        for (unsigned int i=0; i<mNumTypes; ++i) {
            const FluidProperties::FluidPhase typePhase =
                    netFluidProperties->getProperties(netFluidConfig->mTypes[mIndexPort1[i]])->getPhase();
            if (typePhase != nodePhase) {
                GUNNS_WARNING("aborted setting port 1: fluid phase of conversion types doesn't match the node's phase.");
                result = false;
            }
        }
    }

    return result;
}
