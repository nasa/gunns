/**
@file      GunnsFluidPressureHead.cpp
@brief     GUNNS Fluid Pressure Head Spotter implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsNetworkSpotter.o)
   (core/GunnsFluidNode.o)
   (core/GunnsFluidLink.o)
   (simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o)
   (aspects/dynamics/GunnsDynUtils.o))
*/

#include "GunnsFluidPressureHead.hh"
#include "core/GunnsFluidNode.hh"
#include "core/GunnsFluidLink.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "aspects/dynamics/GunnsDynUtils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name  (--)  Instance name for self-identification in messages.
///
/// @details  Default constructs this GUNNS Fluid Pressure Head Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureHeadConfigData::GunnsFluidPressureHeadConfigData(const std::string& name)
    :
    GunnsNetworkSpotterConfigData(name)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Pressure Head Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureHeadConfigData::~GunnsFluidPressureHeadConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  linkPort (--) Which of the link's port nodes to monitor.
///
/// @details  Default constructs this GUNNS Fluid Pressure Head Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureHeadInputData::GunnsFluidPressureHeadInputData(const int linkPort)
    :
    mLinkPort(linkPort)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Pressure Head Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureHeadInputData::~GunnsFluidPressureHeadInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  link  (--)  Reference to the link of interest.
///
/// @details Default constructs this GUNNS Fluid Pressure Head Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureHead::GunnsFluidPressureHead(const GunnsFluidLink& link)
    :
    mPotentialLink(dynamic_cast<GunnsFluidPotential*>(link)),
    mAccumLink(dynamic_cast<GunnsFluidAccum*>(link)),
    mFluidColumn(),
    mColumnLength(0.0),
    mAcceleration(0),
    mRotationDcm(0),
    mTransposeRotation(false),
    mPressureHead(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Pressure Head Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureHead::~GunnsFluidPressureHead()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
/// @param[in]  inputData   (--)  Instance input data.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Pressure Head Spotter with its configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureHead::initialize(const GunnsNetworkSpotterConfigData* configData,
                                        const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(configData, inputData);

    /// - Reset the init flag.
    mInitFlag = false;

    //TODO validate provided link type
    if (not mPotentialLink or mAccumLink) {
        //TODO throw, no link was provided
    }

    /// - Validate & initialize from config & input data.
    const GunnsFluidPressureHeadConfigData* config = validateInput(configData);
    const GunnsFluidPressureHeadInputData*  input  = validateInput(inputData);
    mLinkPort = input->mLinkPort;

    mColumnLength = std::max(GunnsDynUtils::magV(config->mFluidColumn, 3), DBL_EPSILON);
    GunnsDynUtils::scaleV(mFluidColumn, config->mFluidColumn, 1/mColumnLength, 3);


    /// - Step to initialize the outputs.
    stepPostSolver(0.0);

    /// - Set the init flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
///
/// @returns  GunnsFluidPressureHeadConfigData (--) Type-casted and validated config data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base config data class pointer to this spotter's config data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsFluidPressureHeadConfigData* GunnsFluidPressureHead::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
    const GunnsFluidPressureHeadConfigData* result = dynamic_cast<const GunnsFluidPressureHeadConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--)  Instance input data.
///
/// @returns  GunnsFluidPressureHeadInputData (--) Type-casted and validated input data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base input data class pointer to this spotter's input data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsFluidPressureHeadInputData* GunnsFluidPressureHead::validateInput(const GunnsNetworkSpotterInputData* input)
{
    const GunnsFluidPressureHeadInputData* result = dynamic_cast<const GunnsFluidPressureHeadInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  TODO does the dP = rho*g*h
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureHead::stepPreSolver(const double dt __attribute__((unused)))
{
    //TODO option to flip accel sign as well?

    //TODO hang on: for the accum, height is some function of accum bellows position and isn't fixed
    //     like in the Potential.  So not only is density handled differently, but also height.
    //     Maybe that means these should be separate spotters for Potential vs. Accum?
    //     For accum, user will have to configure ratio of height to bellows position, so
    //     0.5 (m) would give 0.1 (m) high at bellows position = 0.2 (0.5 * 0.2).  Or rather, the
    //     exisitng fluid column config vector is from the bottom of accum outlet to the top center
    //     of the full bellows.

    // accum's connect port 1 to teh liquid outlet.  So the column vector is from the port 1 location
    // to the top center of the full accum (toward port 0).  For consistency, we should then make the
    // Potential's column also from port 1 to port 0, so port 1 is considered teh 'bottom' of the
    // column in the accel field, and port 0 the 'top'.  Then positive dP from rho*g*h is a positive
    // dP rise from port 0 to 1 in the Potential.  In terrestrial 1 g, the sensed 1 g (downward) is
    // consistent with accelerating upwards at 1 g in zero gravity.  So the acceleration vector of
    // terrestrial 1 g is actually upwards.  Since the column vector is defined from bottom to top,
    // this aligns with the accel (1 g), so the dot product of g and h creates the correct sign.

    //TODO get fluid density from the link.
    // - assume incompressible
    double density = 0.0;
    if (mPotentialLink) {
        //TODO get both port rho, take the lower value.
        // we know that the FluidPotential will add our positive value as an increase in P from
        // port 0 to 1.   If we used port 1's rho, and it's a compressible gas, then with enough g
        // we'd increase port 1 rho further, which feeds back into rho*g*h, and might blow up the model.
        // So just in case this is being used on a gas (compressible, we should use the port rho
        // for the 'top' of the column wrt/ accel field.  But we don't know which direction the
        // field is until we do the dot product below.   So maybe move this below there...
        //OR.... we make this link only work with liquids and error out if attached to gas nodes...

        // we could get fancy and use whichever node is at the top of the column wrt/ the acceleration
        // direction, but a noisy acceleration combined with the lag in density value form last pass
        // will make this unreliable.  So we just use the lower of the two densities: the acceleration
        // will make this tend to be the 'top' node on its own, and is much simpler.
        density = std::min(mPotentialLink->getNodeContent(0)->getDensity(),
                           mPotentialLink->getNodeContent(1)->getDensity());
    } else if (mAccumLink) {
        //TODO liquid density can be assumed incompressible, so just use rho of the accum liquid
        density = mPotentialLink->getNodeContent(1)->getDensity();
    } else {
        //TODO throw, there is no link.  Should be caught during init so will be untestable here
        //   unless we hack the link pointers to zero in the UT...
    }
    //rotate accel into structure frame
    //the column is in the 'structure' reference frame
    //the DCM defines rotation from accel_frame -> struct_frame
    // OR, if transpose flag is set, it defines struct_frame -> accel_frame
    //
    double accelSrf[3] = {0.0, 0.0, 0.0};
    // rotation DCM is optional; skip if not provided
    if (mRotationDcm) {
        if (mTransposeRotation) {
            //              T
            // a_lrf = [dcm] * {a_arf}
            GunnsDynUtils::multiplyMtV(accelSrf, mRotationDcm, mAcceleration, 3);
        } else {
            // a_lrf = [dcm] * {a_arf}
            GunnsDynUtils::multiplyMV(accelSrf, mRotationDcm, mAcceleration, 3);
        }
    } else {
        GunnsDynUtils::setV(accelSrf, mAcceleration, 3);
    }
    // g is magnitude of accel
    const double g = std::max(GunnsDynUtils::magV(accelSrf, 3), DBL_EPSILON);
    double accelSrfUnit[3] = {0.0, 0.0, 0.0};
    const double accelSrfUnit = GunnsDynUtils::scaleV(accelSrfUnit, accelSrf, 1/g, 3);

    const double aDotH = GunnsDynUtils::dotV(accelSrfUnit, mFluidColumn, 3);

//    mPressureHead = density * g * mColumnLength * aDotH * UnitConversion::KILO_PER_UNIT;
    // (kPa/m = kN/m3), or pressure per length of fluid column
    const double forceDensity = density * g * aDotH * UnitConversion::KILO_PER_UNIT;

    //get pressure to the link
    if (mPotentialLink) {
        // pressure head (kPa) = force density (kN/m3) * length (m)
        const double pressureHead = forceDensity * mColumnLength;
        mPotentialLink->setSourcePressure(pressureHead);
    } else if (mAccumLink) {
        //TODO problem: we're stomping on old values of the bellows coeffs and this doesn't play
        // nice with the accum's configured values, or other drivers (like evaporation).
        // it would be better if accum had a dedicated input for the forceDensity, which it then
        // applied to its bellows pressure as an addition to the normal coeff1 term, i.e:
        // P = coeff0 + coeff1*(bellows position + forceDensity) + coeff2*(bellows position)^2
        mAccumLink->setSpringCoeffs(0.0, forceDensity, 0.0);
    } //else we already checked for no link
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureHead::stepPostSolver(const double dt __attribute__((unused)))
{
    // nothing to do
}
