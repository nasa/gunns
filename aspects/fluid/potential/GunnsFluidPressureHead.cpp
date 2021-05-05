/**
@file      GunnsFluidPressureHead.cpp
@brief     GUNNS Fluid Pressure Head Spotter implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsNetworkSpotter.o)
   (core/GunnsFluidPotential.hh)
   (aspects/fluid/capacitor/GunnsFluidAccum.hh)
   (simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o)
   (aspects/dynamics/GunnsDynUtils.o))
*/

#include "GunnsFluidPressureHead.hh"
#include "core/GunnsFluidPotential.hh"
#include "aspects/fluid/capacitor/GunnsFluidAccum.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "aspects/dynamics/GunnsDynUtils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--)   Instance name for self-identification in messages.
/// @param[in] fluidColumnX         (m)    X-axis component of the orientation vector of the fluid column in the structure reference frame.
/// @param[in] fluidColumnY         (m)    Y-axis component of the orientation vector of the fluid column in the structure reference frame.
/// @param[in] fluidColumnZ         (m)    Z-axis component of the orientation vector of the fluid column in the structure reference frame.
/// @param[in] acceleration         (m/s2) Pointer to the acceleration vector.
/// @param[in] mReverseAcceleration (--)   Reverse the acceleration vector direction.
/// @param[in] mRotationDcm         (--)   Pointer to the rotation direction cosine matrix.
/// @param[in] mTransposeRotation   (--)   Reverse the frame transformation direction.
///
/// @details  Default constructs this GUNNS Fluid Pressure Head Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureHeadConfigData::GunnsFluidPressureHeadConfigData(const std::string& name,
                                                                   const double       fluidColumnX,
                                                                   const double       fluidColumnY,
                                                                   const double       fluidColumnZ,
                                                                   const double*      acceleration,
                                                                   const bool         reverseAcceleration,
                                                                   const double*      rotationDcm,
                                                                   const bool         transposeRotation)
    :
    GunnsNetworkSpotterConfigData(name),
    mFluidColumn(),
    mAcceleration(acceleration),
    mReverseAcceleration(reverseAcceleration),
    mRotationDcm(rotationDcm),
    mTransposeRotation(transposeRotation)
{
    mFluidColumn[0] = fluidColumnX;
    mFluidColumn[1] = fluidColumnY;
    mFluidColumn[2] = fluidColumnZ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Pressure Head Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureHeadConfigData::~GunnsFluidPressureHeadConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Pressure Head Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureHeadInputData::GunnsFluidPressureHeadInputData()
    :
    GunnsNetworkSpotterInputData()
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
/// @param[in]  link  (--)  Reference to the connected link.
///
/// @details Default constructs this GUNNS Fluid Pressure Head Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPressureHead::GunnsFluidPressureHead(GunnsFluidLink& link)
    :
    mLink(&link),
    mPotentialLink(0),
    mAccumLink(0),
    mFluidColumn(),
    mAcceleration(0),
    mReverseAcceleration(false),
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

    /// - Cast the given fluid link to either a potential or accumulator.
    ///   We'd rather do the cast up in the constructor, then wouldn't need to store mLink.  But
    ///   that would fail because spotters are constructed before links in the network, so the link
    ///   hasn't been constructed yet when our constructor runs.
    mPotentialLink = dynamic_cast<GunnsFluidPotential*>(mLink);
    mAccumLink     = dynamic_cast<GunnsFluidAccum*>(mLink);

    /// - Error out if a valid link was not provided.
    if (not (mPotentialLink or mAccumLink)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "a valid link was not provided.");
    }

    /// - Validate & initialize from config & input data.
    const GunnsFluidPressureHeadConfigData* config = validateConfig(configData);
    validateInput(inputData);

    mFluidColumn[0]      = config->mFluidColumn[0];
    mFluidColumn[1]      = config->mFluidColumn[1];
    mFluidColumn[2]      = config->mFluidColumn[2];
    mAcceleration        = config->mAcceleration;
    mReverseAcceleration = config->mReverseAcceleration;
    mRotationDcm         = config->mRotationDcm;
    mTransposeRotation   = config->mTransposeRotation;

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

    /// - Error out if the acceleration vector is not provided.
    if (not result->mAcceleration) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "acceleration vector pointer is missing.");
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
/// @details  Does the dP = rho*g*h calculation and sends dP to the attached link.  The incoming
///           acceleration vector is rotated from its own reference frame to the link's structure
///           reference frame, if the rotation DCM is provided.  This rotation can be flipped in
///           direction if the supplied DCM is in the opposite direction.  Likewise, the
///           acceleration vector itself can be flipped in direction.  These options accommodate
///           whatever vector & DCM convention the user supplies.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureHead::stepPreSolver(const double dt __attribute__((unused)))
{
    // - Get fluid density from the link.
    double density = 0.0;
    if (mPotentialLink) {
        /// - We could get fancy and use whichever node is at the top of the column wrt/ the
        ///   acceleration direction, but a noisy acceleration combined with the lag in density
        ///   values form last pass would make this unreliable.  So we just use the lower of the
        ///   two densities: the acceleration will make this float to the 'top' node on its own,
        ///   and is much simpler.
        density = std::min(mPotentialLink->getNodeContent(0)->getDensity(),
                           mPotentialLink->getNodeContent(1)->getDensity());
    } else if (mAccumLink) {
        density = mAccumLink->getNodeContent(1)->getDensity();
    } //else we already checked for no link during initialization

    /// - Flip acceleration vector direction if so configured.
    double accelArf[3] = {0.0, 0.0, 0.0}; // Acceleration vector, acceleration reference frame
    if (mReverseAcceleration) {
        GunnsDynUtils::scaleV(accelArf, mAcceleration, -1.0, 3);
    } else {
        GunnsDynUtils::setV(accelArf, mAcceleration, 3);
    }

    /// - Rotate acceleration into the structure reference frame, if the optional DCM is
    ///   provided.  If not provided, we assume acceleration and structure frames coincide.
    ///   By default, DCM defines the rotation from the acceleration to the structure frame.
    ///   If the transpose flag is set, then the DCM is transposed, reversing the rotation.
    double accelSrf[3] = {0.0, 0.0, 0.0}; // Acceleration vector, structure reference frame
    if (mRotationDcm) {
        if (mTransposeRotation) {
            GunnsDynUtils::multiplyMtV(accelSrf, mRotationDcm, accelArf, 3);
        } else {
            GunnsDynUtils::multiplyMV(accelSrf, mRotationDcm, accelArf, 3);
        }
    } else {
        GunnsDynUtils::setV(accelSrf, accelArf, 3);
    }

    /// - For the accumulator, the column height is scaled by the bellows position, assuming
    ///   the height is linear with bellows position.
    double column[3];
    GunnsDynUtils::setV(column, mFluidColumn, 3);
    if (mAccumLink) {
        GunnsDynUtils::scaleV(column, column, mAccumLink->getBellowsPosition(), 3);
    }

    /// - Compute pressure head and send it to the link.
    mPressureHead = density * GunnsDynUtils::dotV(accelSrf, column, 3)
                  * UnitConversion::KILO_PER_UNIT;
    if (mPotentialLink) {
        mPotentialLink->setSourcePressure(mPressureHead);
    } else if (mAccumLink) {
        mAccumLink->setAccelPressureHead(mPressureHead);
    } //else we already checked for no link during initialization
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  This spotter has no post-solver functionality, so this function does nothing.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPressureHead::stepPostSolver(const double dt __attribute__((unused)))
{
    // nothing to do
}
