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
- (TBD)

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
        double* mFluidColumn;       /**< (m)    trick_chkpnt_io(**) Height and orientation vector of the fluid column in its local reference frame. */
        double* mAcceleration;      /**< (m/s2) trick_chkpnt_io(**) Pointer to the acceleration vector. */
        double* mRotationDcm;       /**< (1)    trick_chkpnt_io(**) Pointer to the rotation direction cosine matrix. */
        bool    mTransposeRotation; /**< (1)    trick_chkpnt_io(**) Reverse the frame transformation direction. */
        /// @brief  Default constructs this GUNNS Fluid Pressure Head Spotter configuration data.
        GunnsFluidPressureHeadConfigData(const std::string& name,
                                         const double*      fluidColumn       = 0,
                                         const double*      acceleration      = 0,
                                         const double*      rotationDcm       = 0,
                                         const bool         transposeRotation = false);
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
//        int mLinkPort;   /**< (--) trick_chkpnt_io(**) Which of the link's port nodes to monitor. */
        /// @brief  Default constructs this GUNNS Fluid Pressure Head Spotter input data.
        GunnsFluidPressureHeadInputData(const int linkPort = 0);
        /// @brief  Default destructs this GUNNS Fluid Pressure Head Spotter input data.
        virtual ~GunnsFluidPressureHeadInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidPressureHeadInputData(const GunnsFluidPressureHeadInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPressureHeadInputData& operator =(const GunnsFluidPressureHeadInputData& that);
};

// Forward-declare referenced types.
struct GunnsNodeList;
class GunnsFluidLink;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Head Spotter Class.
///
/// @details  This spotter is used to peek at the non-sense-able fluid parameters of a node, such as
///           volume, total mass, partial masses & moles.  This spotter attaches to a link and looks
///           at the node attached at the designated link's port.  This allows the spotter to keep
///           monitoring the link's attached node when the link changes nodes in the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPressureHead : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidPressureHead);
    public:
        /// @brief  Default Constructor
        GunnsFluidPressureHead(const GunnsFluidLink& link);
        /// @brief   Default destructor.
        virtual     ~GunnsFluidPressureHead();
        /// @brief   Initializes the GUNNS Fluid Pressure Head Spotter with configuration and
        ///          input data.
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        /// @brief   Steps the GUNNS Fluid Pressure Head Spotter prior to the GUNNS solver step.
        virtual void stepPreSolver(const double dt);
        /// @brief   Steps the GUNNS Fluid Pressure Head Spotter after the GUNNS solver step.
        virtual void stepPostSolver(const double dt);
//        /// @brief   Gets the total mass of the node contents.
//        double       getMass() const;
//        /// @brief   Gets the mass fraction of the specified constituent index in the node contents.
//        double       getMassFraction(const int index) const;
//        /// @brief   Gets the mole fraction of the specified constituent index in the node contents.
//        double       getMoleFraction(const int index) const;
//        /// @brief   Gets the total volume of the node.
//        double       getVolume() const;

    protected:
        GunnsFluidPotential* mPotentialLink; /**< ** (1) trick_chkpnt_io(**) Pointer to the fluid potential link. */
        GunnsFluidAccum*     mAccumLink;     /**< ** (1) trick_chkpnt_io(**) Pointer to the fluid accumulator link. */
        double  mFluidColumn[3];    /**< (m)    trick_chkpnt_io(**) Orientation unit vector of the fluid column in the structure reference frame. */
        double  mColumnLength;      /**< (m)    trick_chkpnt_io(**) Length of the fluid column. */
        double* mAcceleration;      /**< (m/s2) trick_chkpnt_io(**) Pointer to the acceleration vector. */
        double* mRotationDcm;       /**< (1)    trick_chkpnt_io(**) Pointer to the rotation direction cosine matrix. */
        bool    mTransposeRotation; /**< (1)    trick_chkpnt_io(**) Reverse the frame transformation direction. */
        double  mPressureHead;      /**< (kPa)  trick_chkpnt_io(**) Output pressure head value. */
//        const GunnsNodeList&  mNodeList;             /**< *o (--) trick_chkpnt_io(**) Reference to the network node list. */
//        int                   mLinkPort;             /**<    (--)                     Which of the link's port nodes to monitor. */
//        int                   mNumFluidConstituents; /**< *o (--) trick_chkpnt_io(**) Number of fluid constituents in the network. */
//        double                mNodeMass;             /**< *o (kg) trick_chkpnt_io(**) Total mass of the node contents. */
//        double*               mNodeMassFractions;    /**< *o (--) trick_chkpnt_io(**) Mass fractions of the node contents. */
//        double*               mNodeMoleFractions;    /**< *o (--) trick_chkpnt_io(**) Mole fractions of the node contents. */
//        double*               mNodeTcMoleFractions;  /**< *o (--) trick_chkpnt_io(**) Mole fractions of the node trace compounds contents. */
//        double                mNodeVolume;           /**< *o (m3) trick_chkpnt_io(**) Total volume of the node. */
        /// @brief   Validates the supplied configuration data.
        const GunnsFluidPressureHeadConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        /// @brief   Validates the supplied input data.
        const GunnsFluidPressureHeadInputData*  validateInput (const GunnsNetworkSpotterInputData* input);

    private:
        /// @brief   Deletes dynamic memory.
        void         cleanup();
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidPressureHead(const GunnsFluidPressureHead& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPressureHead& operator =(const GunnsFluidPressureHead& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (kg) Total mass of all fluid in the node contents.
///
/// @details  Returns the total mass of all fluid in the node contents.
////////////////////////////////////////////////////////////////////////////////////////////////////
//inline double GunnsFluidPressureHead::getMass() const
//{
//    return mNodeMass;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  index (--) Index of the fluid constituent to get the mass fraction of.
///
/// @returns  double (--) Mass fraction (0-1) of the specified constituent in the node contents.
///
/// @details  Returns the mass fraction (0-1) of the specified fluid constituent in the node
///           contents.  The given index is limited to the valid range of fluid constituents.
////////////////////////////////////////////////////////////////////////////////////////////////////
//inline double GunnsFluidPressureHead::getMassFraction(const int index) const
//{
//    return mNodeMassFractions[MsMath::limitRange(0, index, mNumFluidConstituents-1)];
//}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  index (--) Index of the fluid constituent to get the mole fraction of.
///
/// @returns  double (--) Mole fraction (0-1) of the specified constituent in the node contents.
///
/// @details  Returns the mole fraction (0-1) of the specified fluid constituent in the node
///           contents.  The given index is limited to the valid range of fluid constituents.
////////////////////////////////////////////////////////////////////////////////////////////////////
//inline double GunnsFluidPressureHead::getMoleFraction(const int index) const
//{
//    return mNodeMoleFractions[MsMath::limitRange(0, index, mNumFluidConstituents-1)];
//}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (m3) Total volume of the node.
///
/// @details  Returns the total volume of the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
//inline double GunnsFluidPressureHead::getVolume() const
//{
//    return mNodeVolume;
//}

#endif
