#ifndef GunnsBasicNode_EXISTS
#define GunnsBasicNode_EXISTS

/**
@file
@brief    GUNNS Basic Node declarations

@defgroup  TSM_GUNNS_CORE_BASIC_NODE    GUNNS Basic Node
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Basic Node Model.  Gunns networks are made up of nodes and links.  Nodes
  are the locations in the network where the network potential (pressure for fluid networks, voltage
  or temperature for basic networks, etc) are solved.  In addition, for fluid networks, nodes are
  also where the fluid mass and its associated properties are collected.

- With one exception, every node in the network has a row in the system of equations.  So in the
  most general form of Gunns, where we are just solving a general [A]{x} = {b} problem, think of the
  nodes as just rows in the system.  The exception is that the last node in the network's node array
  is a zero-potential boundary node.  This represents vacuum for fluid and thermal networks, and
  ground for electrical networks.  This node has no corresponding row in the system.

- This GunnsBasicNodes class provides the base class for fluid nodes and polymorphism for
  simplifying the orchestrator design.  For basic networks, the basic nodes are currently only
  needed to collect fluxes for external network-to-network interfaces.

- Gunns is designed so that most model interaction with Gunns is through the link classes.  As such,
  the nodes are really utility objects for containing the network solution, and the users rarely
  will need to deal directly with the nodes.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsBasicNode.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2011-02) (Initial Prototype)
-  (Eric Ferguson)(L3) (2012-02) (Updates and Maintenance))

@{
*/

#include <string>
#include <cstring>
#include <vector>
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "simulation/hs/TsHsMsg.hh"
//TODO remove this once all links are using the GUNNS macros and no longer need this
#include "software/exceptions/TsHsException.hh"

// Forward declare classes for virtual methods
class PolyFluid;
class PolyFluidConfigData;
class GunnsBasicNode;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Gunns Node List Structure
///
/// @details This structure contains a pointer to the network nodes array, along with the size of
///          the array, for use by network links objects in attaching to the nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsNodeList {
        int             mNumNodes;   /**< (--) trick_chkpnt_io(**) Number of nodes in the array */
        GunnsBasicNode* mNodes;      /**< (--) trick_chkpnt_io(**) The nodes array */

        /// @brief Default Constructor
        GunnsNodeList(int             numNodes = 0,
                      GunnsBasicNode* nodes    = 0);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Gunns Basic Node class
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicNode
{
        TS_MAKE_SIM_COMPATIBLE(GunnsBasicNode);


    public:
        //TODO friend Gunns class, protect functions only intended for Gunns class.
//        friend class Gunns;

        /// @brief Default Constructor
        GunnsBasicNode();

        /// @brief Default Destructor
        virtual ~GunnsBasicNode();

        /// @brief Returns the node object name
        const char*    getName() const;

        /// @brief Returns the basic node influx
        virtual double getInflux() const;

        /// @brief Returns the basic node outflux
        virtual double getOutflux() const;

        /// @brief Returns the basic node net flux
        virtual double getNetFlux() const;

        /// @brief Returns the node potential
        virtual double getPotential() const;

        /// @brief Returns the flux through the node
        double         getFluxThrough() const;

        /// @brief Returns the generic network effective capacitance at this node.
        double         getNetworkCapacitance() const;

        /// @brief Returns the generic network capacitance request flux for this node.
        double         getNetworkCapacitanceRequest() const;

        /// @brief Returns the scheduled outflux for this node.
        double         getScheduledOutflux() const;

        /// @brief Returns the island vector pointer
        const std::vector<int>* getIslandVector() const;

        /// @brief Returns the network capacitance delta-potentials array.
        const double*  getNetCapDeltaPotential() const;

        /// @brief Sets the node potential
        virtual void   setPotential(const double potential);

        /// @brief Sets the generic network effective capacitance at this node.
        void           setNetworkCapacitance(const double capacitance = 0.0);

        /// @brief Sets the generic network capacitance request flux for this node.
        void           setNetworkCapacitanceRequest(const double flux = 1.0);

        /// @brief Sets the node island vector pointer
        void           setIslandVector(const std::vector<int>* island);

        /// @brief Sets the network capacitance delta-potentials array.
        void           setNetCapDeltaPotential(const double* array);

        /// @brief Adds the given value to the scheduled out flux.
        virtual void   scheduleOutflux(const double fluxRate);

        /// @brief Returns whether the node is overflowing.
        virtual bool   isOverflowing(const double dt) const;

        /// @brief Integrate flows within the node
        virtual void   integrateFlows(const double);

        /// @brief Collect the outflows from the node
        virtual void   collectOutflux(const double fluxRate);

        /// @brief Reset Flows on the Node
        virtual void   resetFlows();

        /// @brief Initializes the node with a given name and potential
        void           initialize(const std::string& name = "", const double potential = 0.0);

        /// @brief Returns whether the node has been successfully initialized & validated
        bool           isInitialized() const;

        /// @brief Validates the initialization of this Gunns Basic Node
        virtual void   validate();

        /// @brief Restarts the node after a checkpoint load
        virtual void   restart();

        ////////////////////////////////////////////////////////////////////////////////////////////
        // The following are all dummy placeholder methods to allow polymorphism with the derived
        // GunnsFluidNode class.
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// @brief For fluid networks only, prepares the node for initial run start
        virtual void       prepareForStart();

        /// @brief For fluid networks only, computes the thermal capacitance of the node
        virtual double     computeThermalCapacitance();

        /// @brief For fluid networks only, computes the node compression
        virtual double     computeCompression();

        /// @brief For fluid networks only, sets the node volume for dynamic volume changes in run
        virtual void       setVolume(const double toVolume);

        /// @brief For fluid networks only, initializes the node volume for the beginning of the run
        virtual void       initVolume(const double toVolume);

        /// @brief For fluid networks only, gets the volume from the node
        virtual double     getVolume() const;

        /// @brief For fluid networks only, gets the fluid mass in the node
        virtual double     getMass() const;

        /// @brief For fluid networks only, resets the fluid mass in the node based on fluid density and node volume
        virtual void       updateMass();

        /// @brief For fluid networks only, returns the node content fluid pointer
        virtual PolyFluid* getContent();

        /// @brief For fluid networks only, returns the node inflow fluid pointer
        virtual PolyFluid* getInflow();

        /// @brief For fluid networks only, returns the node outflow fluid pointer
        virtual PolyFluid* getOutflow();

        /// @brief For fluid networks only, collect flow into the node
        virtual void       collectInflux(const double fluxRate, const PolyFluid* fluid = 0);

        /// @brief For fluid networks only, collect heat flux into the node
        virtual void       collectHeatFlux(const double heatFlux);

        /// @brief For fluid networks only, computes the Node pressure correction to flow out mass error
        virtual double     computePressureCorrection();

        /// @brief For fluid networks only, sets the node expansion scale factor
        virtual void       setExpansionScaleFactor(const double expansionScaleFactor);

        /// @brief For fluid networks only, sets the node thermal damping mass
        virtual void       setThermalDampingMass(const double mass);

        /// @brief For fluid networks only, sets the portion of the net heat flux not to be included in thermal damping.
        virtual void       setUndampedHeatFlux(const double heatFlux);

        /// @brief For fluid networks only, clears out the fluid content properties (empties the node)
        virtual void       resetContentState();

        /// @brief For fluid networks only, returns the fluid config data pointer
        virtual const PolyFluidConfigData* getFluidConfig() const;

    protected:
        std::string             mName;                      /**< *o (--) trick_chkpnt_io(**) Node object name for error messages */
        double                  mPotential;                 /**<    (--)                     Generic potential in the node */
        double                  mInfluxRate;                /**<    (--) trick_chkpnt_io(**) Total incident flux rate into the node from all sources */
        double                  mOutfluxRate;               /**<    (--) trick_chkpnt_io(**) Total flux rate out of the node */
        double                  mNetFlux;                   /**<    (--) trick_chkpnt_io(**) Net accumulation rate in the node from all in & out fluxes */
        double                  mFluxThrough;               /**<    (--) trick_chkpnt_io(**) Net flux rate through the node */
        double                  mNetworkCapacitance;        /**<    (--)                     Generic network effective capacitance at this node */
        double                  mNetworkCapacitanceRequest; /**<    (--) trick_chkpnt_io(**) Generic flux requested to be used to calculate network capacitance for this node */
        double                  mScheduledOutflux;          /**<    (--) trick_chkpnt_io(**) Storage of total flux rate value out of the node before actual transport logic */
        const std::vector<int>* mIslandVector;              /**< ** (--) trick_chkpnt_io(**) Pointer to the node's island vector */
        const double*           mNetCapDeltaPotential;      /**< *o (--) trick_chpint_io(**) Network capacitance delta-potentials array */
        bool                    mInitFlag;                  /**< *o (--) trick_chkpnt_io(**) Initialization status flag (T is good) */

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsBasicNode(const GunnsBasicNode& that);

        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicNode& operator =(const GunnsBasicNode& that);
};

//TODO replace all use of these macro's with GunnsMacros.hh and then delete here.
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   exception  Ts*Exception    The exception type (derived from TsException) to throw.
/// @param[in]   subtype    std::string     The subtype description of the exception.
/// @param[in]   cause      std::string     The root cause for this message.
///
/// @details  This macro invokes the TS_HS_EXCEPTION macro to throw an exception and issue a H&S
///           error message consisting of the given exception type, subtype, and cause.  This macro
///           supplies severity = TS_HS_ERROR, subsystem = "GUNNS", and thrower = the object name to
///           TS_HS_EXCEPTION, for consistent error reporting from all Gunns objects.  A char* or
///           const char* is also acceptable as an std:string argument
///
/// @par      Example 1 (static cause):
/// @code
///           #include "software/exceptions/TsInitializationException.hh"
///           ...
///           GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
///                       "All nodes uninitialized or vacuum.");
/// @endcode
///
/// @par      Example 2 (composed cause):
/// @code
///           #include "software/exceptions/TsOutOfBoundsException.hh"
///           ...
///           std::ostringstream msg;
///           msg << "x argument " << x << " outside valid range " << mMinX << " to " << mMaxX;
///           GUNNS_ERROR(TsOutOfBoundsException, "Invalid Input Argument", msg.str());
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_ERROR(exception, subtype, cause) \
{ \
    TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_GUNNS, cause, exception, subtype, mName); \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   text      std::string     The text details of the message.
///
/// @details  This macro creates a TsHsMsg object to log a health & status (H&S) warning message.
///           The message will automatically indicate class, line of code, method, thrower
///           (identified by the instance's mName attribute) and the detailed text supplied in the
///           call.  This macros supplies severity = TS_HS_WARNING and subsystem = "GUNNS", for
///           consistent warning reporting from all Gunns objects.  The text argument can use the
///           << operator.
///
/// @par      Example 1 (static text):
/// @code
///           GUNNS_WARNING("All nodes uninitialized or vacuum.");
/// @endcode
///
/// @par      Example 2 (composed text):
/// @code
///           GUNNS_WARNING("Port setting of " << i << " is illegal.");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_WARNING(text) \
{ \
    TsHsMsg gunnsMsg(TS_HS_WARNING, TS_HS_GUNNS); \
    gunnsMsg << mName << " " << text; \
    hsSendMsg(gunnsMsg); \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   text      std::string     The text details of the message.
///
/// @details  This macro creates a TsHsMsg object to log a health & status (H&S) info message.
///           The message will automatically indicate class, line of code, method, thrower
///           (identified by the instance's mName attribute) and the detailed text supplied in the
///           call.  This macros supplies severity = TS_HS_INFO and subsystem = "GUNNS", for
///           consistent info reporting from all Gunns objects.  The text argument can use the
///           << operator.
///
/// @par      Example 1 (static text):
/// @code
///           GUNNS_INFO("By your command.");
/// @endcode
///
/// @par      Example 2 (composed text):
/// @code
///           GUNNS_INFO("For the " << i << " colonies, toaster!");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_INFO(text) \
{ \
    TsHsMsg gunnsMsg(TS_HS_INFO, TS_HS_GUNNS); \
    gunnsMsg << mName << " " << text; \
    hsSendMsg(gunnsMsg); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro to copy name.
///
/// @param[in]   name       std::string     The given instance name.
///
/// @details  This macro initializes a class std::string attribute mName from the given std::string
///           name argument.
///
/// @par      Example:
/// @code
///           GUNNS_NAME("MyObjectName");
/// @endcode
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_NAME(name) \
{ \
    mName = name; \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro to copy name with H&S error message and exception throw on empty name.
///
/// @param[in]   caller     std::string     The name of the calling class i.e. GunnsBasicNode.
/// @param[in]   name       std::string     The given instance name.
///
/// @details  This macro first checks the std::string name argument. If it is empty, an H&S error
///           message is issued and a TsInitializationException is thrown.  Otherwise it initializes
///           a class std::string attribute mName from the given std::string name argument.
///
/// @par      Example:
/// @code
///           LIBRARY DEPENDENCY:
///               ((simulation/hs/TsHsMsg.o)
///                (software/exceptions/TsInitializtionException.o))
///           ...
///           #include "core/GunnsMacros.hh"
///           #include "software/exceptions/TsInitializationException.hh"
///           ...
///           GUNNS_NAME_ERREX("MyClass", "MyObjectName");
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define GUNNS_NAME_ERREX(caller, name) \
{ \
    if (name.empty()) { \
        TsHsMsg msg(TS_HS_ERROR, TS_HS_GUNNS); \
        msg << "throwing " << "TsInitializationException" << " " << "Invalid Initialization Data" << " - " << "Empty object name."  << '\n' << tsStackTrace(); \
        hsSendMsg(msg); \
        throw TsInitializationException("Invalid Initialization Data", caller, "Empty object name."); \
    } \
    GUNNS_NAME(name); \
}
/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   const char*  -- The Node name
///
/// @details  Returns the Node name as a char*.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* GunnsBasicNode::getName() const
{
    return mName.c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the node potential
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::setPotential(const double potential)
{
    mPotential = potential;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  double  (--)  Generic network effective capacitance at this node
///
/// @details  This method is called by the solver during network solution.  If the network
///           capacitance was requested prior to the solution in which a matrix decomposition is
///           performed, the solver will set this to the computed capacitance.  On any decomposition
///           in which no links requested the value, this is set to zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::setNetworkCapacitance(const double capacitance)
{
    mNetworkCapacitance = capacitance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  double  (--)  Generic flux (> 0) to be used in network capacitance calculation.
///
/// @details  This method is called by links or spotters prior to the network solution to indicate
///           to the solver that a new network capacitance value is desired for this node.  A flux
///           value greater than zero will cause the solver to calculate the network capacitance
///           in response to the flux.  The capacitance value will be set by the solver during the
///           network solution and will be available to links and spotters during computeFlows() and
///           after.  The flux request value is reset when the capacitance is calculated, so the
///           user must continue to call this method each time a capacitance is desired.  The
///           capacitance value persists until the next network solution.  During the link's step
///           method, the capacitance is from the previous network pass.  The capacitance is
///           returned by the getNetworkCapacitance() method.
///
/// @note     Finding the network capacitance is moderately CPU-intensive in the solver, so this
///           should only be used on those nodes for which the value is expressly needed.
///
/// @note     Network capacitances are only supported when the solver is in NORMAL mode and using
///           the Cholesky method.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::setNetworkCapacitanceRequest(const double flux)
{
    mNetworkCapacitanceRequest = flux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (--)  Generic flux to be used to calculate network capacitance.
///
/// @details  Returns the mNetworkCapacitanceRequest flux.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::getNetworkCapacitanceRequest() const
{
    return mNetworkCapacitanceRequest;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (--)  Generic flux scheduled to be removed from the node.
///
/// @details  Returns the mScheduledOutflux value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::getScheduledOutflux() const
{
    return mScheduledOutflux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  island  (--)  Pointer to the solver's island vector this node currently belongs to
///
/// @details  This is only intended to be set by the solver.  The solver provides a vector of all
///           node numbers belonging to the island that this node belongs to.  This can be provided
///           to links incident on this node for their information about other nodes they are
///           indirectly connected to through conductive paths.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::setIslandVector(const std::vector<int>* island)
{
    mIslandVector = island;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  -- The node potential
///
/// @details  Returns the node potential
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::getPotential() const
{
    return mPotential;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  -- The flux through the node
///
/// @details  Returns the flux through the node
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::getFluxThrough() const
{
    return mFluxThrough;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  -- The node total incident influx rate
///
/// @details  Returns the node total incident influx rate
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::getInflux() const
{
    return mInfluxRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  -- The node total incident outflux rate
///
/// @details  Returns the node total incident outflux rate
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::getOutflux() const
{
    return mOutfluxRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  -- The node net incident flux rate
///
/// @details  Returns the node net incident flux rate
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::getNetFlux() const
{
    return mNetFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (--)  Generic network effective capacitance at this node
///
/// @details  This is generally the node's capacitance plus the capacitance of other nodes adjusted
///           by conductive flow paths.  This predicts the response of this node's potential to a
///           unit flux source added to the node in the following network solution.  This can be
///           useful for stabilizing flux sources such as external demands.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::getNetworkCapacitance() const
{
    return mNetworkCapacitance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   std::vector<int>*  --  The island node vector this node belongs to
///
/// @details  Returns the island vector pointer, which is a list of node numbers of the island this
///           node belongs to.  Links can use this for their information about what other nodes they
///           are indirectly connected to through conductive paths.
///
/// @note     This pointer is not guaranteed to not be null, therefore the caller must check for
///           null pointer before dereferencing.
///
/// @note     The contained node numbers are in ascending order.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::vector<int>* GunnsBasicNode::getIslandVector() const
{
    return mIslandVector;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  const double*  (--)  Array of network capacitance delta-potentials.
///
/// @details  Returns the mNetCapDeltaPotential pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const double* GunnsBasicNode::getNetCapDeltaPotential() const
{
    return mNetCapDeltaPotential;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  array  (--)  Pointer to the network capacitance delta-potentials.
///
/// @details  Sets mNetCapDeltaPotential to the given pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::setNetCapDeltaPotential(const double* array)
{
    mNetCapDeltaPotential = array;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fluxRate (--) Node outflux Rate
///
/// @details  Adds the given value to the mScheduledOutflux term.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::scheduleOutflux(const double fluxRate)
{
    mScheduledOutflux += fluxRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fluxRate (--) Node influx Rate
/// @param[in] fluid    (--) Not used
///
/// @details  This function adds an incoming flux to the collection terms for the Node.  When the
///           Node updates, it will zero out the collection terms for next pass.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::collectInflux(const double fluxRate, const PolyFluid* fluid __attribute__((unused)))
{
    mInfluxRate += fluxRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fluxRate (--) Node outflux Rate
///
/// @details  This function adds an outgoing flux to the collection terms for the Node.  When the
///           Node updates, it will zero out the collection terms for next pass.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::collectOutflux(const double fluxRate)
{
    mOutfluxRate += fluxRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This function resets the flows incident on the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::resetFlows()
{
    mInfluxRate  = 0.0;
    mOutfluxRate = 0.0;
    mScheduledOutflux = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   boolean  Returns true if initialization & validation were successful
///
/// @details  Returns whether the node has been successfully initialized & validated.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsBasicNode::isInitialized() const
{
    return mInitFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::prepareForStart()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double Returns zero.
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::computeThermalCapacitance()
{
    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double Returns zero.
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::computeCompression()
{
    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] toVolume (m3) Desired node volume
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::setVolume(const double toVolume __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] toVolume (m3) Desired node volume
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::initVolume(const double toVolume __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double Returns zero.
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::getVolume() const
{
    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double Returns zero.
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::getMass() const
{
    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::updateMass()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   PolyFluid* Returns NULL pointer.
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline PolyFluid* GunnsBasicNode::getContent()
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   PolyFluid* Returns NULL pointer.
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline PolyFluid* GunnsBasicNode::getInflow()
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   PolyFluid* Returns NULL pointer.
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline PolyFluid* GunnsBasicNode::getOutflow()
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] heatFlux (W) Node incident Heat Flux
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::collectHeatFlux(const double heatFlux __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double Returns zero.
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicNode::computePressureCorrection()
{
    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] expansionScaleFactor (--) dT/dP expansion effect scale factor (0-1)
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::setExpansionScaleFactor(const double expansionScaleFactor __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] mass (kg) Non-fluid equivalent thermal mass
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::setThermalDampingMass(const double mass __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] heatFlux (W) Portion of the net heat flux not to be included in thermal damping.
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::setUndampedHeatFlux(const double heatFlux __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicNode::resetContentState()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   PolyFluidConfigData* Returns NULL pointer.
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const PolyFluidConfigData* GunnsBasicNode::getFluidConfig() const
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Unused.
///
/// @details  Dummy placeholder for derived GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsBasicNode::isOverflowing(const double dt __attribute__((unused))) const
{
    return false;
}

#endif
