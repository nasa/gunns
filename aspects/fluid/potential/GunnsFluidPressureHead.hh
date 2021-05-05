#ifndef GunnsFluidPressureHead_EXISTS
#define GunnsFluidPressureHead_EXISTS

/**
@file      GunnsFluidPressureHead.hh
@brief     GUNNS Fluid Pressure Head Spotter declarations

@defgroup  TSM_GUNNS_FLUID_POTENTIAL_PRESSURE_HEAD_SPOTTER   GUNNS Fluid Pressure Head Spotter
@ingroup   TSM_GUNNS_FLUID_POTENTIAL

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:  (Provides the classes for the GUNNS Fluid Pressure Head Spotter.
           This spotter is used to calculate the pressure head of a column of fluid in an
           acceleration field, and provide it to a connected GunnsFluidPotential or
           GunnsFluidAccum link.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- ((Assume fluid is incompressible.)
   (For accumulator links we assume column height is linear with bellows position.))

LIBRARY DEPENDENCY:
- ((GunnsFluidPressureHead.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (April 2021) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsNetworkSpotter.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Head Spotter Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Pressure Head Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPressureHeadConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        double        mFluidColumn[3];      /**< (m)    trick_chkpnt_io(**) Height and orientation vector of the fluid column in the structure reference frame. */
        const double* mAcceleration;        /**< (m/s2) trick_chkpnt_io(**) Pointer to the acceleration vector. */
        bool          mReverseAcceleration; /**< (1)    trick_chkpnt_io(**) Reverse the acceleration vector direction. */
        const double* mRotationDcm;         /**< (1)    trick_chkpnt_io(**) Pointer to the rotation direction cosine matrix. */
        bool          mTransposeRotation;   /**< (1)    trick_chkpnt_io(**) Reverse the frame transformation direction. */
        /// @brief  Default constructs this GUNNS Fluid Pressure Head Spotter configuration data.
        GunnsFluidPressureHeadConfigData(const std::string& name,
                                         const double       fluidColumnX        = 0.0,
                                         const double       fluidColumnY        = 0.0,
                                         const double       fluidColumnZ        = 0.0,
                                         const double*      acceleration        = 0,
                                         const bool         reverseAcceleration = false,
                                         const double*      rotationDcm         = 0,
                                         const bool         transposeRotation   = false);
        /// @brief  Default destructs this GUNNS Fluid Pressure Head Spotter configuration data.
        virtual ~GunnsFluidPressureHeadConfigData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidPressureHeadConfigData(const GunnsFluidPressureHeadConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPressureHeadConfigData& operator =(const GunnsFluidPressureHeadConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Head Spotter Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Pressure Head Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPressureHeadInputData : public GunnsNetworkSpotterInputData
{
    public:
        /// @brief  Default constructs this GUNNS Fluid Pressure Head Spotter input data.
        GunnsFluidPressureHeadInputData();
        /// @brief  Default destructs this GUNNS Fluid Pressure Head Spotter input data.
        virtual ~GunnsFluidPressureHeadInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidPressureHeadInputData(const GunnsFluidPressureHeadInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPressureHeadInputData& operator =(const GunnsFluidPressureHeadInputData& that);
};

// Forward-declare referenced types.
class GunnsFluidLink;
class GunnsFluidPotential;
class GunnsFluidAccum;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Head Spotter Class.
///
/// @details  This spotter automates the calculation of the pressure created by a column of fluid
///           under acceleration.  This receives an acceleration vector and optional frame rotation,
///           and outputs the resulting pressure head (delta-pressure) to an attached
///           GunnsFluidPotential or GunnsFluidAccum link.  This spotter is configured with the
///           direction vector of the fluid column in its structural reference frame.
///
///           Both link types define the 'bottom' of their column vector at port 1, and port 0 is
///           towards the 'top'.  By default, the acceleration is defined such that if the vehicle
///           is accelerating towards the direction of the 'top' end of the column, the acceleration
///           vector points in that direction, and this creates a pressure increase at the bottom of
///           the fluid column.
///
///           If the user is stuck with an acceleration vector that is defined opposite of the above
///           they can set the mReverseAcceleration flag to cause us to flip their direction to our
///           convention.  Likewise, the rotation direction cosine matrix (DCM), which normally
///           rotates an acceleration reference frame vector into the structural reference frame,
///           can be made to do the reverse rotation instead.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPressureHead : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidPressureHead);
    public:
        /// @brief  Default Constructor
        GunnsFluidPressureHead(GunnsFluidLink& link);
        /// @brief  Default destructor.
        virtual     ~GunnsFluidPressureHead();
        /// @brief  Initializes the GUNNS Fluid Pressure Head Spotter with configuration and
        ///         input data.
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        /// @brief  Steps the GUNNS Fluid Pressure Head Spotter prior to the GUNNS solver step.
        virtual void stepPreSolver(const double dt);
        /// @brief  Steps the GUNNS Fluid Pressure Head Spotter after the GUNNS solver step.
        virtual void stepPostSolver(const double dt);
        /// @brief  Returns the pressure head value.
        double       getPressureHead() const;

    protected:
        GunnsFluidLink*      mLink;             /**< ** (1)    trick_chkpnt_io(**) Pointer to the link. */
        GunnsFluidPotential* mPotentialLink;    /**< ** (1)    trick_chkpnt_io(**) Pointer to the fluid potential link. */
        GunnsFluidAccum*     mAccumLink;        /**< ** (1)    trick_chkpnt_io(**) Pointer to the fluid accumulator link. */
        double               mFluidColumn[3];      /**< (m)    trick_chkpnt_io(**) Height and orientation of the fluid column in the structure reference frame. */
        const double*        mAcceleration;        /**< (m/s2) trick_chkpnt_io(**) Pointer to the acceleration vector. */
        bool                 mReverseAcceleration; /**< (1)    trick_chkpnt_io(**) Reverse the acceleration vector direction. */
        const double*        mRotationDcm;         /**< (1)    trick_chkpnt_io(**) Pointer to the rotation direction cosine matrix. */
        bool                 mTransposeRotation;   /**< (1)    trick_chkpnt_io(**) Reverse the frame transformation direction. */
        double               mPressureHead;        /**< (kPa)  trick_chkpnt_io(**) Output pressure head. */
        /// @brief  Validates the supplied configuration data.
        const GunnsFluidPressureHeadConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        /// @brief  Validates the supplied input data.
        const GunnsFluidPressureHeadInputData*  validateInput (const GunnsNetworkSpotterInputData* input);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidPressureHead(const GunnsFluidPressureHead& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPressureHead& operator =(const GunnsFluidPressureHead& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (kPa)  Pressure head.
///
/// @details  Returns the value of the mPressureHead attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidPressureHead::getPressureHead() const
{
    return mPressureHead;
}

#endif
