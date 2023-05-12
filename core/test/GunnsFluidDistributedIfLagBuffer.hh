#ifndef GunnsFluidDistributedIfLagBuffer_EXISTS
#define GunnsFluidDistributedIfLagBuffer_EXISTS

/**
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE: (This is a ring buffer of GunnsFluidDistributedIfData for both data directions between a
          pair of GunnsFluidDistributedIf links.  This is used to create a desired amount of round-
          trip data lag in the interface, for testing stability, mass conservation, etc.  This
          shouldn't be used in an actual project.

          Use the sim bus to write data to the Head data pointers, and read from the Tail pointers.

          Set mDelayFrames in the input file to dial in the desired total lag.  The total round-trip
          loop lag, as measured by the pair master link, will be 2 * (2 + mDelayFrames) frames.

          The step function should be called before the models that interface with this.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidDistributedIfLagBuffer.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (March 2019) (Initial))

@{
*/

#include <string>
#include "../GunnsFluidDistributedIf.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsFluidDistributedIf Data Lag Buffer
///
/// @details  Refer to class Purpose.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidDistributedIfLagBuffer
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidDistributedIfLagBuffer);
    public:
        unsigned int                 mDelayFrames; /**< (--) Number of frames to delay in each direction */
        GunnsFluidDistributed2WayBusInterfaceData* mHead1;       /**< (--) Head of buffer 1 for writing by the interface. */
        GunnsFluidDistributed2WayBusInterfaceData* mHead2;       /**< (--) Head of buffer 2 for writing by the interface. */
        GunnsFluidDistributed2WayBusInterfaceData* mTail1;       /**< (--) Tail of buffer 1 for reading by the interface. */
        GunnsFluidDistributed2WayBusInterfaceData* mTail2;       /**< (--) Tail of buffer 2 for reading by the interface. */
        /// @brief  Default constructs this GunnsFluidDistributedIfLagBuffer.
        GunnsFluidDistributedIfLagBuffer();
        /// @brief  Default destructs this GunnsFluidDistributedIfLagBuffer.
        virtual ~GunnsFluidDistributedIfLagBuffer();
        /// @brief  Initializes this GunnsFluidDistributedIfLagBuffer.
        void initialize();
        /// @brief  Updates the head and tail indeces for the buffers.
        void step();

    private:
        GunnsFluidDistributed2WayBusInterfaceData mBuffer1[10]; /**< (--) Buffer for direction 1. */
        GunnsFluidDistributed2WayBusInterfaceData mBuffer2[10]; /**< (--) Buffer for direction 2. */
        unsigned int                mHeadIndex;   /**< (--) Index of the head of the buffers. */
        unsigned int                mTailIndex;   /**< (--) Index of the tail of the buffers. */
        /// @brief  Sets the interface Head & Tail data pointers to the correct buffer locations.
        void updatePointers();
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidDistributedIfLagBuffer(const GunnsFluidDistributedIfLagBuffer&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidDistributedIfLagBuffer& operator =(const GunnsFluidDistributedIfLagBuffer&);
};

/// @}

#endif
