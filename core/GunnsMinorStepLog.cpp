/**
@file
@brief    GUNNS Solver Minor Step Log implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ()
*/

#include "GunnsMinorStepLog.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the GUNNS Solver Minor Step Log.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLog::GunnsMinorStepLog()
    :
    mMajorStep(0),
    mMinorStep(0),
    mResult(SUCCESS),
    mNodeBits(),
    mLinkBits(),
    mNumLinkWords(0),
    mNumNodeWords(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the GUNNS Solver Minor Step Log.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLog::~GunnsMinorStepLog()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  networkSize  (--)  Number of nodes in the network, not counting the ground node.
/// @param[in]  numLinks     (--)  Number of links in the network.
/// @param[in]  stepLimit    (--)  The network's minor step limit.
///
/// @details  Initializes this GUNNS Solver Minor Step Log.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::initialize(const int networkSize, const int numLinks, const int stepLimit)
{
    /// - Allocate bitset arrays for node & link converging debug flags.  First dimension is the
    ///   minor step, second dimension is the bit set spanning the number of nodes or links.
    mNumNodeWords = 1 + networkSize / mWordSize;
    mNumLinkWords = 1 + numLinks / mWordSize;
    for (int step = 0; step < stepLimit; ++step) {
        {
            std::vector< std::bitset<mWordSize> > s;
            mNodeBits.push_back(s);
        } {
            std::vector< std::bitset<mWordSize> > s;
            mLinkBits.push_back(s);
        }

        for (int word = 0; word < mNumNodeWords; ++word) {
            std::bitset<mWordSize> w;
            mNodeBits[step].push_back(w);
        }

        for (int word = 0; word < mNumLinkWords; ++word) {
            std::bitset<mWordSize> w;
            mLinkBits[step].push_back(w);
        }
    }
}
