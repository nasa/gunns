#ifndef GunnsFluidLink_EXISTS
#define GunnsFluidLink_EXISTS

/**
@file
@brief    GUNNS Fluid Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID    GUNNS Fluid Link
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (This is the parent class for GUNNS link objects in fluid networks.  It extents the GunnsBasicLink
   class with fluid attributes and fluid transport methods that all fluid links share.  It is
   intended that all fluid links ultimately derive from this class. As such, this class will not run
   by itself in a Gunns network and should never be directly instantiated.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidLink.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2011-02) (Initial Prototype)
-  (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))

@{
*/

#include "GunnsBasicLink.hh"
#include "GunnsFluidNode.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Link Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Link
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidLinkConfigData : public GunnsBasicLinkConfigData
{
    public:
        /// @brief    Default constructs this Fluid Link configuration data.
        GunnsFluidLinkConfigData(const std::string& name     = "",
                                 GunnsNodeList*     nodeList = 0);

        /// @brief    Default destructs this Fluid Link configuration data.
        virtual ~GunnsFluidLinkConfigData();

        /// @brief    Copy constructs this Fluid Link configuration data.
        GunnsFluidLinkConfigData(const GunnsFluidLinkConfigData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidLinkConfigData& operator =(const GunnsBasicLinkConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Link Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Link
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidLinkInputData : public GunnsBasicLinkInputData
{
    public:
        /// @brief    Default constructs this Fluid Link input data.
        GunnsFluidLinkInputData(const bool   malfBlockageFlag  = false,
                                const double malfBlockageValue = 0.0);

        /// @brief    Default destructs this Fluid Link input data.
        virtual ~GunnsFluidLinkInputData();

        /// @brief    Copy constructs this Fluid Link input data.
        GunnsFluidLinkInputData(const GunnsFluidLinkInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidLinkInputData& operator =(const GunnsFluidLinkInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Link Class
///
/// @details  GunnsFluidLink class inherits from the GunnsBasicLink class and adds the data members
///           needed for a fluid system.  This makes use of the Hydraulic-Electric Analogy, allowing
///           us to simulate both electrical and fluid networks with the same reuse code.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidLink : public GunnsBasicLink
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluidLink);

    public:
        /// @brief Protected Default Destructor because Fluid Link is never directly instantiated
        /// Not protected thanks to Trick10.  TODO resolve this when Trick10 is fixed.
        virtual ~GunnsFluidLink();

        /// @brief Returns the pointer to the link exit fluid
        PolyFluid*       getInternalFluid() const;

        /// @brief Returns the mass flow rate through the link
        double           getFlowRate() const;

        /// @brief Returns the volumetric flow rate through the link
        double           getVolFlowRate() const;

        /// @brief Return the fluid content of the node at the given port index
        const PolyFluid* getNodeContent(const int port = 0) const;

        /// @brief Updates the derived class object internal fluid
        /// TODO 2nd arg is mFlowRate but why pass it?
        virtual void     updateFluid(const double, const double);

        /// @brief Transports the flows through the link between the nodes.
        virtual void     transportFlows(const double dt);

        /// @brief Moves the fluid from one port to the other
        virtual void     transportFluid(const bool forcedOutflow,
                                        const int  fromPort = 0,
                                        const int  toPort   = 1);

    protected:
        /// @details  Pointer to the link internal fluid.  The internal fluid is optional - it is
        ///           only needed when a link needs to change the properties of the fluid passing
        ///           through it, such as a chemical reaction changing the fluid mixture, or a fan
        ///           adding heat.  If needed, this fluid is instantiated by the derived class
        ///           during initialization by calling the the createInternalFluid method.  If
        ///           instantiated, Gunns uses this fluid as the exit fluid properties of the link.
        PolyFluid*          mInternalFluid;   /**< (--)   trick_chkpnt_io(**) Pointer to the link exit fluid */
        double              mFlowRate;        /**< (kg/s)                     Mass flow rate through the link */
        double              mVolFlowRate;     /**< (m3/s)                     True volumetric flow rate through the link */

        /// @brief Protected Constructor because Fluid Link is never directly instantiated
        GunnsFluidLink(const int numPorts);

        /// @brief    Initialize the link with config data and port assignments
        void initialize(const GunnsFluidLinkConfigData& configData,
                        const GunnsFluidLinkInputData&  inputData,
                        std::vector<GunnsBasicLink*>&   links,
                        const int*                      ports);

        /// @brief    Validates the initialization of this Gunns Fluid Link
        void validate() const;

        /// @brief Initializes the internal fluid object with default properties
        void createInternalFluid();

        /// @brief Initializes the internal fluid object with given initial properties
        void createInternalFluid(const PolyFluidInputData& initialFluidState);

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Determines which port supplies valid fluid properties for a flow
        int determineSourcePort(const double flow, const int fromPort, const int toPort) const;

        /// @brief Sets the port node pointer to the node indicated by the port node map
        virtual void updateNodePointer(const int port);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidLink(const GunnsFluidLink& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidLink& operator =(const GunnsFluidLink& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  PolyFluid*  --  Pointer to the internal fluid
///
/// @details Returns the pointer to the internal fluid
////////////////////////////////////////////////////////////////////////////////////////////////////
inline PolyFluid* GunnsFluidLink::getInternalFluid() const
{
    return mInternalFluid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  double  (kg/s)  Mass flow rate through the link
///
/// @details Returns the link mass flow rate
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidLink::getFlowRate() const
{
    return mFlowRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  double  (m3/s)  Volumetric flow rate through the link
///
/// @details Returns the link volumetric flow rate
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidLink::getVolFlowRate() const
{
    return mVolFlowRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Updates the Link Internal Fluid. Although there is no method body they aren't made
///          pure virtual so the class can be unit tested.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidLink::updateFluid(const double, const double)
{
    // nothing to do
}

#endif
