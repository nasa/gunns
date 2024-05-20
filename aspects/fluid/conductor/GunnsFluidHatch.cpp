/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the GUNNS Fluid Hatch link model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   (
    (Conductivity is the hatch cross-sectional area.)
    (Diffusion occurs even with significant mass flow.)
   )

LIBRARY DEPENDENCY:
   ((aspects/fluid/conductor/GunnsFluidValve.o))

PROGRAMMERS:
   ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-11))
*/

#include <cfloat>
#include <math.h>
#include "core/GunnsFluidUtils.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "GunnsFluidHatch.hh"

/// - Delta pressure and temperature below which the diffusion calculation is accurate.
const double GunnsFluidHatch::DIFFUSION_DELTA_PRESS_LIMIT = 0.01;
const double GunnsFluidHatch::DIFFUSION_DELTA_TEMP_LIMIT  = 20.0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                 (--)   Name of object.
/// @param[in]  nodes                (--)   Pointer to nodes.
/// @param[in]  maxConductivity      (m2)   Maximum conductivity.
/// @param[in]  expansionScaleFactor (--)   Scale factor for isentropic gas cooling.
/// @param[in]  thermalLength        (m)    Tube length for thermal convection
/// @param[in]  thermalDiameter      (m)    Tube inner diameter for thermal convection
/// @param[in]  surfaceRoughness     (m)    Tube wall surface roughness for thermal convection
/// @param[in]  length0              (m)    Distance from hatch to center of port 0 node volume.
/// @param[in]  length1              (m)    Distance from hatch to center of port 1 node volume.
///
/// @details  Default constructs this GUNNS Fluid Hatch link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHatchConfigData::GunnsFluidHatchConfigData(const std::string& name,
                                                     GunnsNodeList*     nodes,
                                                     const double       maxConductivity,
                                                     const double       expansionScaleFactor,
                                                     const double       thermalLength,
                                                     const double       thermalDiameter,
                                                     const double       surfaceRoughness,
                                                     const double       length0,
                                                     const double       length1)
    :
    GunnsFluidValveConfigData(name, nodes, maxConductivity, expansionScaleFactor, thermalLength, thermalDiameter, surfaceRoughness),
    mLength0(length0),
    mLength1(length1)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Hatch link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHatchConfigData::GunnsFluidHatchConfigData(const GunnsFluidHatchConfigData& that)
    :
    GunnsFluidValveConfigData(that),
    mLength0(that.mLength0),
    mLength1(that.mLength1)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hatch link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHatchConfigData::~GunnsFluidHatchConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag   (--)    Blockage malfunction flag.
/// @param[in]  malfBlockageValue  (--)    Blockage malfunction fractional value (0-1).
/// @param[in]  position           (--)    Fractional position.
/// @param[in]  malfLeakThruFlag   (--)    Leak through rate malfunction flag.
/// @param[in]  malfLeakThruValue  (kg/s)  Leak through rate malfunction value.
/// @param[in]  wallTemperature    (K)     Tube wall temperature for thermal convection
///
/// @details  Default constructs this GUNNS Fluid Hatch link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHatchInputData::GunnsFluidHatchInputData(const bool   malfBlockageFlag,
                                                   const double malfBlockageValue,
                                                   const double position,
                                                   const bool   malfLeakThruFlag,
                                                   const double malfLeakThruValue,
                                                   const double wallTemperature)
    :
    GunnsFluidValveInputData(malfBlockageFlag,
                             malfBlockageValue,
                             position,
                             malfLeakThruFlag,
                             malfLeakThruValue,
                             wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Hatch link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHatchInputData::GunnsFluidHatchInputData(const GunnsFluidHatchInputData& that)
    :
    GunnsFluidValveInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hatch link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHatchInputData::~GunnsFluidHatchInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Hatch link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHatch::GunnsFluidHatch()
    :
    GunnsFluidValve(),
    mLength0(0.0),
    mLength1(0.0),
    mDiffusiveFlowRate(0.0),
    mDiffusiveFluid(0),
    mConductiveHeatFlux(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hatch link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHatch::~GunnsFluidHatch()
{
    TS_DELETE_OBJECT(mDiffusiveFluid);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
/// @param[in,out]  links        (--)  Link vector.
/// @param[in]      port0        (--)  Nominal inlet port map index.
/// @param[in]      port1        (--)  Nominal outlet port map index.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Hatch link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHatch::initialize(const GunnsFluidHatchConfigData& configData,
                                 const GunnsFluidHatchInputData&  inputData,
                                 std::vector<GunnsBasicLink*>&    links,
                                 const int                        port0,
                                 const int                        port1)
{
    /// - First initialize & validate parent.
    GunnsFluidValve::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag           = false;

    /// - Initialize with configuration data.
    mLength0            = configData.mLength0;
    mLength1            = configData.mLength1;

    /// - Validate configuration and input data.
    validate();

    /// - Need an internal fluid and flowrate for diffusion, but not for normal pressure flows.
    ///   flowrate for hatch as a conductor.  Use parent method to do the work of creating an
    ///   internal fluid, then copy construct a diffusive fluid.  We keep the internal fluid around
    ///   so that the parent class will call our updateFluid method.
    createInternalFluid();
    if (mDiffusiveFluid) {
        mDiffusiveFluid->cleanup();
        TS_DELETE_OBJECT(mDiffusiveFluid);
    }
    TS_NEW_PRIM_OBJECT_EXT(mDiffusiveFluid, PolyFluid,
                           (*mInternalFluid, std::string(mName) + ".mDiffusiveFluid"),
                           std::string(mName) + ".mDiffusiveFluid");
    mDiffusiveFluid->setFlowRate(0.0);
    mDiffusiveFlowRate  = 0.0;
    mConductiveHeatFlux = 0.0;

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag           = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Hatch link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHatch::validate() const
{
    /// - Throw an exception if distance between port node centers < FLT_EPSILON.
    if (mLength0 + mLength1 < static_cast<double>(FLT_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Distance between node volume centers < FLT_EPSILON.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHatch::restartModel()
{
    /// - Reset the base class.
    GunnsFluidValve::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mDiffusiveFlowRate  = 0.0;
    mConductiveHeatFlux = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt        (s)     Time step.
/// @param[in]  flowrate  (kg/s)  Mass flow rate (not used).
///
/// @return   void
///
/// @details  Updates the mass flow rate, internal fluid mole fractions and  node fluid temperatures
///           of this GUNNS Fluid Hatch link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHatch::updateFluid(const double dt, const double flowrate __attribute__((unused)))
{
    /// - Skip if time step is too small or either of the nodes is the network Ground node, since
    ///   molecular diffusion & heat conduction doesn't make sense with a pure vacuum.  Also skip
    ///   when either volume is zero, since there would be zero mass to diffuse.
    mDiffusiveFlowRate  = 0.0;
    mConductiveHeatFlux = 0.0;
    if ( (dt > DBL_EPSILON) and (mNodeMap[0] != getGroundNodeIndex())
                            and (mNodeMap[1] != getGroundNodeIndex())
                            and (mNodes[0]->getVolume() > 0.0)
                            and (mNodes[1]->getVolume() > 0.0) ) {

        /// - Diffusion calculation is only valid when delta pressure and delta temperature are close to 0.0.
        const double deltaTemperature = fabs(mNodes[0]->getOutflow()->getTemperature() - mNodes[1]->getOutflow()->getTemperature());
        if (fabs(mPotentialDrop) < GunnsFluidHatch::DIFFUSION_DELTA_PRESS_LIMIT
            and deltaTemperature < GunnsFluidHatch::DIFFUSION_DELTA_TEMP_LIMIT) {

            /// - Compute diffusive mass flux across hatch and update diffusive fluid and flow rate.
            mDiffusiveFlowRate = GunnsFluidUtils::computeGasDiffusion(mDiffusiveFluid,
                                                                      mEffectiveConductivity,
                                                                      mNodes[0]->getOutflow(),
                                                                      mNodes[1]->getOutflow(),
                                                                      mFlowRate,
                                                                      mLength0,
                                                                      mLength1);
        }

        /// - Compute heat flux across hatch and update node fluid heat fluxes.
        mConductiveHeatFlux = GunnsFluidUtils::computeConductiveHeatFlux(mEffectiveConductivity,
                                                                         mNodes[0]->getOutflow(),
                                                                         mNodes[1]->getOutflow(),
                                                                         mLength0,
                                                                         mLength1);
        mNodes[0]->collectHeatFlux(-mConductiveHeatFlux);
        mNodes[1]->collectHeatFlux(+mConductiveHeatFlux);

    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  forcedOutflow  (--)   not used.
/// @param[in]  fromPort       (--)   not used.
/// @param[in]  toPort         (--)   not used.
///
/// @return   void
///
/// @details  Adds this link's flow to the node's collection terms. Override parent method which
///           did not allow mixture change or multiple transport.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHatch::transportFluid(const bool forcedOutflow __attribute__((unused)),
                                     const int  fromPort      __attribute__((unused)),
                                     const int  toPort        __attribute__((unused)))
{
    /// - Call parent transport fluid once as a conductor with the normal internal fluid and nominal
    ///   pressure flow mass flow rate.
    GunnsFluidLink::transportFluid(false);

    /// - Swap normal internal fluid and mass flow rate for the diffusive parameters.
    PolyFluid* saveInternalFluid = mInternalFluid;
    const double saveFlowRate    = mFlowRate;
    mInternalFluid               = mDiffusiveFluid;
    mFlowRate                    = mDiffusiveFlowRate;

    /// - Call parent transport fluid method again for diffusion.
    GunnsFluidLink::transportFluid(true);

    /// - Swap internal fluid pointer back to null and mass flow rate back to nominal.
    mInternalFluid               = saveInternalFluid;
    mFlowRate                    = saveFlowRate;
}
