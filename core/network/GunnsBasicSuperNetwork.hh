#ifndef GunnsBasicSuperNetwork_EXISTS
#define GunnsBasicSuperNetwork_EXISTS

/**
@file
@brief     GUNNS Basic Super-Network declarations

@defgroup  TSM_GUNNS_CORE_BASIC_SUPER_NETWORK    GUNNS Basic Super-Network Class
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the GUNNS Basic Super-Network.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsBasicSuperNetwork.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2016-10) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/network/GunnsSuperNetworkBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Basic Super-Network Class.
///
/// @details  This is the super-network for basic (thermal & electrical aspect) networks.  Please
///           see the details for the base class for more info.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicSuperNetwork : public GunnsSuperNetworkBase
{
    TS_MAKE_SIM_COMPATIBLE(GunnsBasicSuperNetwork);
    public:
        GunnsBasicNode* netNodes;                            /**< (--) Network basic nodes array. */
        /// @brief  Default constructor.
        GunnsBasicSuperNetwork(const std::string& name);
        /// @brief  Default destructor.
        virtual ~GunnsBasicSuperNetwork();

    protected:
        /// @brief  Allocates dynamic array of basic nodes for the super-network.
        virtual void allocateNodes();
        /// @brief  Deletes the dynamic array of basic nodes.
        virtual void freeNodes();
        /// @brief  Returns whether the nodes array has been allocated.
        virtual bool isNullNodes() const;
        /// @brief  Initializes the super-network basic Ground node.
        virtual void initGroundNode();
        /// @brief  Initializes the super-network solver.
        virtual void initSolver();
        /// @brief  Creates a derived joint network type.
        virtual GunnsNetworkBase* createJointNetwork(const int type);
        /// @brief  Allocate an array of the derived joint network type.
        virtual void registerJoints();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsBasicSuperNetwork(const GunnsBasicSuperNetwork& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicSuperNetwork& operator =(const GunnsBasicSuperNetwork& that);
};

/// @}

#endif
