#ifndef GunnsBasicLink_EXISTS
#define GunnsBasicLink_EXISTS

/**
@file
@brief    GUNNS Basic Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_BASIC    GUNNS Basic Link
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (This is the main base class for GUNNS link objects.  It mainly contains utility methods and
  attributes that all link classes share.  As such, this class will not run by itself in a Gunns
  network and should never be directly instantiated.

- All physical models simulated with GUNNS, like valves, tanks, electrical cards, radiator panels,
  etc. are GUNNS links and ultimately derive from this class and its sub-classes.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsBasicLink.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2011-02) (Initial Prototype)
-  (Kevin Supak)  (L-3 Communications) (2011-05) (Updated to Coding Standards))

@{
*/

#include <string>
#include <vector>

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicNode.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Link Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Link
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicLinkConfigData
{
    public:
        std::string    mName;      /**< (--) trick_chkpnt_io(**) Link name for messaging */
        GunnsNodeList* mNodeList;  /**< (--) trick_chkpnt_io(**) Network nodes structure */

        /// @brief Default constructs this Basic Link configuration data.
        GunnsBasicLinkConfigData(const std::string& name     = "",
                                 GunnsNodeList*     nodeList = 0);

        /// @brief Default destructs this Basic Link configuration data.
        virtual ~GunnsBasicLinkConfigData();

        /// @brief Copy constructs this Basic Link configuration data.
        GunnsBasicLinkConfigData(const GunnsBasicLinkConfigData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicLinkConfigData& operator =(const GunnsBasicLinkConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Link Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Link
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicLinkInputData
{
    public:
        int*          mInitialNodeMap;      /**< (--) Initial node map override (optional) */
        bool          mMalfBlockageFlag;    /**< (--) Blockage malfunction activation flag */
        double        mMalfBlockageValue;   /**< (--) Blockage malfunction fractional value (0-1) */

        /// @brief    Default constructs this Basic Link input data.
        GunnsBasicLinkInputData(const bool   malfBlockageFlag  = false,
                                const double malfBlockageValue = 0.0);

        /// @brief    Default destructs this Basic Link input data.
        virtual ~GunnsBasicLinkInputData();

        /// @brief    Copy constructs this Basic Link input data.
        GunnsBasicLinkInputData(const GunnsBasicLinkInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicLinkInputData& operator =(const GunnsBasicLinkInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Basic Link Admittance Map
///
/// @details  This describes the mapping of the link's internal admittance matrix to the network's
///           matrix.  This enables links to define custom mapping of their compressed internal
///           admittance matrix to the network as desired.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicLinkAdmittanceMap
{
    public:
        unsigned int mSize; /**< *o (1)                     The size of the mMap array. */
        int*         mMap;  /**< *o (1) trick_chkpnt_io(**) The mapping of the link's internal admittance matrix to the network matrix. */
        /// @brief Default constructs this link admittance map.
        GunnsBasicLinkAdmittanceMap();
        /// @brief Default destructs this link admittance map.
        virtual ~GunnsBasicLinkAdmittanceMap();
        /// @brief Allocates the map array to the given size.
        void allocateMap(const std::string& name, const unsigned int size);
        /// @brief Deletes the map array.
        void freeMap();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsBasicLinkAdmittanceMap(const GunnsBasicLinkAdmittanceMap&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicLinkAdmittanceMap& operator =(const GunnsBasicLinkAdmittanceMap&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Basic Link
///
/// @details  This is the base link class for any connection between two nodes in a network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicLink
{
        TS_MAKE_SIM_COMPATIBLE(GunnsBasicLink);

    public:
        /// @brief Enumeration of the user port mapping command states.
        enum UserPortControl {
            FAILED  = -1,    ///< Feedback to the user that the previous attempt failed.
            READY   =  0,    ///< Feedback to the user that the link is ready for command.
            EXECUTE =  1,    ///< Attempts to map the given port to the given node.
            GROUND  =  2,    ///< Attempts to map the given port to the ground node.
            DEFAULT =  3     ///< Attempts to map the given port to its default node.
        };

        /// @brief Enumeration of the return values of the confirmSolutionAcceptable link method.
        enum SolutionResult {
            REJECT  =  0,    ///< Link rejects the current minor step solution.
            CONFIRM =  1,    ///< Link accepts the current minor step solution.
            DELAY   =  2     ///< Link delays the check for a later minor step.
        };

        /// @brief Enumeration of flow directions at a port node.
        enum PortDirection {
            NONE   = 0,      ///< No flow to or from this port.
            SOURCE = 1,      ///< Port pulling flow from the node, node is a source.
            SINK   = 2,      ///< Port pushing flow to the node, node is a sink.
            BOTH   = 3       ///< Port flowing both to and from the node, node is a source and sink.
        };

        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool           mMalfBlockageFlag;              /**< (--) Blockage malfunction flag */
        double         mMalfBlockageValue;             /**< (--) Blockage malfunction value (0-1) */
        /// @}

        /// @brief Protected Default Destructor because Basic Link is never directly instantiated
        /// Not protected thanks to Trick10.  TODO resolve this when Trick10 is fixed.
        virtual ~GunnsBasicLink();

        /// @brief Step method for updating the link
        virtual void   step(const double dt);

        /// @brief Minor Step method for non-linear Systems
        virtual void   minorStep(const double dt, const int minorStep);

        /// @brief Computes the flows through the link
        virtual void   computeFlows(const double dt);

        /// @brief Transports the flows through the link between the nodes.
        virtual void   transportFlows(const double dt);

        /// @brief Restarts the link after a checkpoint load
        void           restart();

        /// @brief Returns the object name
        const char*    getName() const;

        /// @brief Returns the Node Mapping
        int*           getNodeMap() const;

        /// @brief Returns the admittance matrix mapping
        GunnsBasicLinkAdmittanceMap* getAdmittanceMap();

        /// @brief Returns the link admittance matrix
        double*        getAdmittanceMatrix() const;

        /// @brief Returns the source vector
        double*        getSourceVector() const;

        /// @brief Returns the potential vector
        double*        getPotentialVector() const;

        /// @brief Returns the potential override flags vector
        bool*          getOverrideVector() const;

        /// @brief Returns the port flow directions
        PortDirection* getPortDirections() const;

        /// @brief Returns the number of ports on the link
        int            getNumberPorts() const;

        /// @brief Returns the potential difference across the link
        virtual double getDeltaPotential(const int toPort = 0, const int fromPort = 1);

        /// @brief Returns the flux through the link
        double         getFlux() const;

        /// @brief Returns the power gain/loss across the link
        double         getPower() const;

        /// @brief Returns the potential drop across the link
        double         getPotentialDrop() const;

        /// @brief Sets the minimum linearization potential
        virtual void   setMinLinearizationPotential(const double minLinearP);

        /// @brief Attaches a port to a node
        bool           setPort(const int port, const int node, const std::string& source = "this",
                               const bool verbose = true);

        /// @brief Sets and resets the blockage malfunction
        void           setMalfBlockage(const bool flag = false, const double value = 0.0);

        /// @brief Sets a port potential override.
        void           setPortOverride(const int port, const double value);

        /// @brief Resets a port potential override.
        void           resetPortOverride(const int port = -1);

        /// @brief Boolean for telling the solver to update the admittance matrix for this link
        virtual bool   needAdmittanceUpdate();

        /// @brief Boolean for telling the solver if it is a non-linear link
        virtual bool   isNonLinear();

        /// @brief Returns the initialization status
        bool           isInitialized() const;

        /// @brief Returns the link's assessment of the solution
        virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                         const int absoluteStep);

        /// @brief Resets the link back to the previous minor step iteration
        virtual bool   resetLastMinorStep(const int convergedStep, const int absoluteStep);

        /// @brief Updates the state of the link
        virtual void   updateState(const double dt);

        /// @brief Updates the flux through the link
        virtual void   updateFlux(const double dt, const double flux);

        /// @brief Special processing of data inputs to the model prior to the network update
        virtual void   processInputs();

        /// @brief Special processing of data outputs from the model after the network update
        virtual void   processOutputs();

        /// @brief Locks the link node map, so that ports will not move for any reason other than at initialization
        void           lockNodeMap();

        /// @brief Unlocks the link node map, so that ports are free to move as normal
        void           unlockNodeMap();

    protected:
        std::string      mName;                      /**< *o (1) trick_chkpnt_io(**) Link object name for error messages */
        GunnsBasicNode** mNodes;                     /**< *o (1) trick_chkpnt_io(**) Array of pointers to the linked nodes */
        GunnsNodeList*   mNodeList;                  /**< *o (1) trick_chkpnt_io(**) Pointer to the network nodes structure */
        double*          mPotentialVector;           /**<    (1) trick_chkpnt_io(**) Generic potential at the link ports */
        double*          mAdmittanceMatrix;          /**<    (1) trick_chkpnt_io(**) Link contribution to admittance matrix */
        double*          mSourceVector;              /**<    (1) trick_chkpnt_io(**) Link contribution to the source vector */
        int*             mDefaultNodeMap;            /**< ** (1) trick_chkpnt_io(**) Default node map restored on restart */
        int*             mNodeMap;                   /**< *o (1) trick_chkpnt_io(**) Node numbers at the link ports */
        GunnsBasicLinkAdmittanceMap mAdmittanceMap;  /**< *o (1)                     Mapping of link to network admittance matrices */
        bool*            mOverrideVector;            /**<    (1) trick_chkpnt_io(**) Port potential override flags */
        PortDirection*   mPortDirections;            /**<    (1) trick_chkpnt_io(**) Flow direction at each port */
        int              mNumPorts;                  /**< *o (1) trick_chkpnt_io(**) Number of ports on the link */
        bool             mAdmittanceUpdate;          /**<    (1) trick_chkpnt_io(**) Link is changing the admittance matrix */
        double           mFlux;                      /**<    (1)                     Generic flux through the link */
        double           mPotentialDrop;             /**<    (1)                     Generic potential drop across the link */
        double           mPower;                     /**<    (W) trick_chkpnt_io(**) Generic power gain/loss across the link */
        double           mMinLinearizationPotential; /**<    (1) trick_chkpnt_io(**) Delta-Potential linearization threshold */
        bool             mInitFlag;                  /**< *o (1) trick_chkpnt_io(**) Link Init Flag */
        bool             mNodeMapLocked;             /**<    (1) trick_chkpnt_io(**) Ports cannot change nodes for any reason */
        int              mUserPortSelect;            /**<    (1) trick_chkpnt_io(**) The user-selected port to re-map */
        int              mUserPortSelectNode;        /**<    (1) trick_chkpnt_io(**) The node to re-map the selected port to */
        UserPortControl  mUserPortSetControl;        /**<    (1) trick_chkpnt_io(**) Command/feedback for the re-map action */
        static const double mConductanceLimit;       /**< ** (1)                     Maximum allowed generic conductance */
        static const double m100EpsilonLimit;        /**< ** (1)                     Limit 100 * DBL_EPSILON for some applications */

        /// @brief Protected Constructor because Basic Link is never directly instantiated
        GunnsBasicLink(const int numPorts);

        /// @brief Initializes this Basic Link with configuration and input data
        void initialize(const GunnsBasicLinkConfigData& configData,
                        const GunnsBasicLinkInputData&  inputData,
                        std::vector<GunnsBasicLink*>&   networkLinks,
                        const int*                      portMap);

        /// @brief Performs functions common to initialization and restart.
        void         initializeRestartCommonFunctions();

        /// @brief Virtual method for derived links to create a custom or default admittance map.
        virtual void createAdmittanceMap();

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Setting of link node pointers
        virtual void registerNodes(const int* portMap, const std::string& source, const bool verbose);

        /// @brief Allocates the matrix, node map and vectors
        void         allocateMatrixAndVectors(const std::string& name);

        /// @brief Virtual method for derived links to allocate a custom admittance matrix.
        virtual void allocateAdmittanceMatrix();

        /// @brief Checks for valid port node assignment
        bool         checkPortRules(const int port, const int node) const;

        /// @brief Checks for duplicate port node assignment
        virtual bool checkPortDuplication(const int port, const int node) const;

        /// @brief Checks for valid implementation-specific port node assignment
        virtual bool checkSpecificPortRules(const int port, const int node) const;

        /// @brief Link config data and port mapping validation
        void         validate(const GunnsBasicLinkConfigData& configData,
                              const GunnsBasicLinkInputData&  inputData,
                              const int* portMap) const;

        /// @brief Validation of mapped port node pointers
        void         validateNodes() const;

        /// @brief Deletion of allocated objects
        void         cleanup();

        /// @brief Updates flux transport terms for display
        virtual void transportFlux(const int fromPort = 0, const int toPort = 1);

        /// @brief Handles user commands to re-map port assignments
        void         processUserPortCommand();

        /// @brief Returns the index of the network's ground node
        int          getGroundNodeIndex() const;

        /// @brief Computes mPower for the aspect-specific implementation
        virtual void computePower();

        /// @brief Determines if the supplied port number is a valid port number for this link
        bool         checkValidPortNumber(const int port) const;

        /// @brief Determines if the supplied node number is a valid node number in the network
        bool         checkValidNodeNumber(const int node) const;

        /// @brief Sets the port node pointer to the node indicated by the port node map
        virtual void updateNodePointer(const int port);

        /// @brief Virtual method for derived links to update a custom or default admittance map.
        virtual void updateAdmittanceMap();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsBasicLink(const GunnsBasicLink& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicLink& operator =(const GunnsBasicLink& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   char* -- The link Name
///
/// @details  Returns the link name as a char*.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* GunnsBasicLink::getName() const
{
    return mName.c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   int* -- Node mapping of the link
///
/// @details  Returns the node mapping of the link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int* GunnsBasicLink::getNodeMap() const
{
    return mNodeMap;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsBasicLinkAdmittanceMap* (--) Pointer to this link's admittance map object.
///
/// @details  Returns the admittance map map object of this link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsBasicLinkAdmittanceMap* GunnsBasicLink::getAdmittanceMap()
{
    return &mAdmittanceMap;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double* -- The admittance matrix of the link
///
/// @details  Returns the admittance matrix of the link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double* GunnsBasicLink::getAdmittanceMatrix() const
{
    return mAdmittanceMatrix;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double* -- The Source Vector of the link
///
/// @details  Returns the Source Vector of the Link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double* GunnsBasicLink::getSourceVector() const
{
    return mSourceVector;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double* -- The potential vector of the link
///
/// @details  Returns the potential vector of the Link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double* GunnsBasicLink::getPotentialVector() const
{
    return mPotentialVector;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool* -- The port potential override flags vector of the Link
///
/// @details  Returns the port potential override flags vector of the Link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool* GunnsBasicLink::getOverrideVector() const
{
    return mOverrideVector;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   PortDirection* -- The port flow direction states the Link
///
/// @details  Returns the port flow direction states the Link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsBasicLink::PortDirection* GunnsBasicLink::getPortDirections() const
{
    return mPortDirections;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   int -- The number of ports on the Link
///
/// @details  Returns the number of ports on the link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsBasicLink::getNumberPorts() const
{
    return mNumPorts;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool -- The initialization flag
///
/// @details  Returns the initialization status (T is good).
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsBasicLink::isInitialized() const
{
    return mInitFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Class attribute resets common to both initialization and restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::initializeRestartCommonFunctions()
{
    mAdmittanceUpdate          = false;
    mPower                     = 0.0;
    mUserPortSelect            = -1;
    mUserPortSelectNode        = -1;
    mUserPortSetControl        = READY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets mNodeMapLocked = true.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::lockNodeMap()
{
    mNodeMapLocked = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets mNodeMapLocked = false.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::unlockNodeMap()
{
    mNodeMapLocked = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The flux through the link.
///
/// @details  Returns the flux through the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicLink::getFlux() const
{
    return mFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (W) The power gain/loss across the link.
///
/// @details  Returns the power gain/loss across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicLink::getPower() const
{
    return mPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The potential drop across the link.
///
/// @details  Returns the potential drop across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicLink::getPotentialDrop() const
{
    return mPotentialDrop;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] minLinearP (--) Minimum Linearization Potential
///
/// @details  Sets the minimum linearization potential across the link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::setMinLinearizationPotential(const double minLinearP)
{
    mMinLinearizationPotential = minLinearP;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The default implementation of link power is -flux * potential drop.  Note that this is
///           equivalent to electrical power gain, as in a current or voltage source.  Other
///           aspects, such as thermal and fluid, should override this method to implement the
///           appropriate equations for their specific types of power.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::computePower()
{
    mPower = -mFlux * getDeltaPotential(0, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Dummy placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::updateState(const double dt __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt   (s)  Integration time step
/// @param[in] flux (--) Generic flux through the link
///
/// @details  Dummy placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::updateFlux(const double dt   __attribute__((unused)),
                                       const double flux __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Frame delta time
///
/// @details  Dummy placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::step(const double dt __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Frame delta time
/// @param[in] minorStep (--) The minor step number that the network is on
///
/// @details  Dummy placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::minorStep(const double dt        __attribute__((unused)),
                                      const int    minorStep __attribute__((unused)))
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Frame delta time
///
/// @details  Dummy placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::computeFlows(const double dt __attribute__((unused)))
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Frame delta time
///
/// @details  Dummy placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::transportFlows(const double dt __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  Returns true if the link is non-linear
///
/// @details  Dummy placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsBasicLink::isNonLinear()
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) The # of minor steps since the network last converged
/// @param[in] absoluteStep  (--) The absolute minor step number that the network is on
///
/// @return SolutionResult (--) Returns whether the link confirms, rejects, or delays the solution.
///
/// @details  Dummy placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsBasicLink::SolutionResult GunnsBasicLink::confirmSolutionAcceptable(
        const int convergedStep __attribute__((unused)),
        const int absoluteStep __attribute__((unused)))
{
    return CONFIRM;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) The # of minor steps since the network last converged
/// @param[in] absoluteStep  (--) The absolute minor step number that the network is on
///
/// @return   bool  --  Returns true if the link successfully returned to the previous minor step
///
/// @details  Dummy placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsBasicLink::resetLastMinorStep(const int convergedStep __attribute__((unused)),
                                               const int absoluteStep __attribute__((unused)))
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned
/// @param[in] node (--) The desired node to assign the port to
///
/// @return   bool  --  Returns true if all link-specific rules checks pass
///
/// @details  Dummy placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsBasicLink::checkSpecificPortRules(const int port __attribute__((unused)),
                                                   const int node __attribute__((unused))) const
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Dummy placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::processInputs()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Dummy placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::processOutputs()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Calls the admittance map function to allocate the map.  This is the default
///           implementation, which allocates the map size as nPorts^2, for the default
///           uncompressed link admittance matrix.  Derived links can override this to size their
///           custom map for their custom admittance matrix as needed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicLink::createAdmittanceMap()
{
    mAdmittanceMap.allocateMap(mName + ".mAdmittanceMap", mNumPorts * mNumPorts);
}

#endif
