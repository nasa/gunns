#ifndef GunnsFluidNode_EXISTS
#define GunnsFluidNode_EXISTS

/**
@file
@brief    GUNNS Fluid Node declarations

@defgroup  TSM_GUNNS_CORE_FLUID_NODE    GUNNS Fluid Node
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (The fluid node class inherits from the basic node class and fulfills the same function as the
  basic node class in fluid networks.  In addition to storing the solved network potentials
  (pressure), the nodes also store all fluid mass and energy in the network.  This class makes use
  of the PolyFluid class for storing and calculating fluid properties.  All fluids in Gunns are
  PolyFluid objects, regardless of how many constituents are actually present.

- Fluid nodes can be either capacitive (volume > 0), or non-capacitive (volume = 0).  Fluids are
  transported between nodes by the network links.  The fluid node collects incoming flow properties
  in the mInflow member.  Each pass, the mixed incoming flow from all sources is mixed from mInflow
  into the actual fluid contents of the node, mContent, and all fluid properties in mContent are
  updated.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidNode.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2011-02) (Initial Prototype)
-  (Kevin Supak)  (L-3 Communications) (2011-02) (Updated to Coding Standards)
-  (Eric Ferguson)(L-3 Communications) (2012-02) (Updates and Maintenance))
@{
*/

#include "core/GunnsBasicNode.hh"
#include "aspects/fluid/fluid/PolyFluid.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Gunns Fluid Node class
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidNode : public GunnsBasicNode
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluidNode);

    public:
        /// @brief Default Constructor
        GunnsFluidNode();

        /// @brief Default Destructor
        virtual ~GunnsFluidNode();

        /// @brief Initializes the node
        void           initialize(const std::string&         name              = "",
                                  const PolyFluidConfigData* fluidConfig       = 0,
                                  const PolyFluidInputData*  initialFluidState = 0);

        /// @brief Prepares the node for initial run start
        virtual void   prepareForStart();

        /// @brief Validates the initialization of the Node
        virtual void   validate();

        /// @brief Restarts the node after a checkpoint load
        virtual void   restart();

        /// @brief Initializes the node volume for the beginning of the run
        virtual void   initVolume(const double toVolume);

        /// @brief Sets the node volume for dynamic volume changes in run
        virtual void   setVolume(const double toVolume);

        /// @brief Sets the node potential and fluid content pressure
        virtual void   setPotential(const double potential);

        /// @brief Sets the node expansion scale factor
        virtual void   setExpansionScaleFactor(const double expansionScaleFactor);

        /// @brief Sets the node thermal damping mass
        virtual void   setThermalDampingMass(const double mass);

        /// @brief Sets the portion of the net heat flux not to be included in thermal damping.
        virtual void   setUndampedHeatFlux(const double heatFlux);

        /// @brief Clears out the fluid content properties (empties the node)
        virtual void   resetContentState();

        /// @brief Reset incoming & outgoing flows on the Node
        virtual void   resetFlows();

        /// @brief Computes the thermal capacitance of the node
        virtual double computeThermalCapacitance();

        /// @brief Computes the node compression
        virtual double computeCompression();

        /// @brief Collect fluid mass flow into the node
        virtual void   collectInflux(const double fluxRate, const PolyFluid *fluid = 0);

        /// @brief Collect trace compounds flows into the node, separate from bulk fluid flows.
        void           collectTc(const int tcIndex, const double rate);

        /// @brief Collect heat flux into the node
        virtual void   collectHeatFlux(const double heatFlux);

        /// @brief Integrate flows within the node and updates content fluid properties
        virtual void   integrateFlows(const double dt);

        /// @brief Computes the Node pressure correction to flow out mass error
        virtual double computePressureCorrection();

        /// @brief Resets the fluid mass in the Node based on fluid density and node volume
        virtual void   updateMass();

        /// @brief Gets the volume from the Node
        virtual double getVolume() const;

        /// @brief Gets the fluid mass in the Node
        virtual double getMass() const;

        /// @brief Returns the Node content fluid pointer
        virtual PolyFluid* getContent();

        /// @brief Returns the Node inflow fluid pointer
        virtual PolyFluid* getInflow();

        /// @brief Returns the Node outflow fluid pointer
        virtual PolyFluid* getOutflow();

        /// @brief Gets the net heat flux into the node
        double getNetHeatFlux() const;

        /// @brief Returns the fluid config data pointer
        virtual const PolyFluidConfigData* getFluidConfig() const;

        /// @brief Sets previous equal to the current temperature
        void updatePreviousTemperature();

        /// @brief Sets previous equal to the current pressure
        void updatePreviousPressure();

        /// @brief Returns whether this node is currently overflowing
        virtual bool isOverflowing(const double dt) const;

    protected:
        const PolyFluidConfigData* mFluidConfig;          /**< *o (--)       trick_chkpnt_io(**) Available fluid types in this node */
        int                        mNumFluidTypes;        /**< *o (--)       trick_chkpnt_io(**) The number of fluid types in this node */
        PolyFluid                  mContent;              /**<    (--)                           Fluid contained within this node */
        PolyFluid                  mInflow;               /**<    (--)       trick_chkpnt_io(**) Mixture of all incoming fluid flows */
        PolyFluid                  mOutflow;              /**<    (--)       trick_chkpnt_io(**) Mixture of all outgoing fluid flows */
        GunnsFluidTraceCompoundsInputData mTcInflow;      /**<    (kg/s)     trick_chkpnt_io(**) Mass flow rate of incoming trace compounds */
        double                     mVolume;               /**<    (m3)       trick_chkpnt_io(**) Node volume */
        double                     mPreviousVolume;       /**<    (m3)       trick_chkpnt_io(**) Previous pass value of node volume */
        double                     mThermalCapacitance;   /**<    (kg*mol/K) trick_chkpnt_io(**) Thermal capacitance of the node */
        double                     mCompression;          /**<    (kg*mol)   trick_chkpnt_io(**) Compression effect of changing volume on pressure */
        double                     mInflowHeatFlux;       /**<    (W)        trick_chkpnt_io(**) Collected heat flux into the node */
        double                     mNetHeatFlux;          /**<    (W)        trick_chkpnt_io(**) Net heat flux into the Node */
        double                     mUndampedHeatFlux;     /**<    (W)        trick_chkpnt_io(**) Portion of net heat flux not included in thermal damping */
        double                     mThermalDampingMass;   /**<    (kg)       trick_chkpnt_io(**) Non-fluid equivalent thermal mass for damping dT/dt */
        double                     mThermalDampingHeat;   /**<    (W)        trick_chkpnt_io(**) Heat flux damped by the thermal damping mass */
        double                     mExpansionDeltaT;      /**<    (K)        trick_chkpnt_io(**) Change in temperature due to expansion (gas only) */
        double                     mExpansionScaleFactor; /**<    (--)       trick_chkpnt_io(**) Isentropic expansion effect scale factor (0-1) */
        double                     mPreviousPressure;     /**<    (kPa)                          Previous pass value of node pressure */
        double                     mPreviousTemperature;  /**<    (K)                            Previous pass value of node temperature */
        double                     mMassError;            /**<    (kg)                           Error in node mass between solver & equation of state */
        double                     mPressureCorrection;   /**<    (kPa)                          Filtered pressure correction to wash out mMassError */
        double                     mCorrectGain;          /**<    (--)                           Pressure correction filter gain */
        static const double        mErrorThreshold;       /**< ** (kPa)                          Error threshold for pressure correction */

        /// @brief Calculates mass error in the node
        void   computeMassError();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidNode(const GunnsFluidNode& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidNode& operator =(const GunnsFluidNode& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Resets the node content's fluid properties, equivalent to emptying the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidNode::resetContentState()
{
    mContent.resetState();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] heatRate (W) Node incident Heat Flux
///
/// @details  This function collects pure heat fluxes that aren't associated with a mass flow. An
///           example would be heat flux from a tank wall to the tank fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidNode::collectHeatFlux(const double heatRate)
{
    mInflowHeatFlux += heatRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] heatFlux (W) Portion of the net heat flux not to be included in thermal damping.
///
/// @details  Sets the portion of the net heat flux not to be included in thermal damping.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidNode::setUndampedHeatFlux(const double heatFlux)
{
    mUndampedHeatFlux = heatFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (m3) Node volume
///
/// @details  Returns the volume of the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidNode::getVolume() const
{
    return mVolume;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg) Fluid mass in the node.
///
/// @details  Returns the fluid mass of the Node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidNode::getMass() const
{
    return mContent.getMass();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   PolyFluid* -- Pointer to the fluid content of the node.
///
/// @details  Returns the fluid node content pointer.  Can't const this because Gunns and Links will
///           modify the fluid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline PolyFluid* GunnsFluidNode::getContent()
{
    return &mContent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   PolyFluid* -- Pointer to the node's incoming fluid.
///
/// @details  Returns the fluid node inflow pointer.  Can't const this because Gunns and Links will
///           modify the fluid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline PolyFluid* GunnsFluidNode::getInflow()
{
    return &mInflow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   PolyFluid* -- Pointer to the node's outgoing fluid.
///
/// @details  Returns the fluid node outflow pointer.  Can't const this because Gunns and Links will
///           modify the fluid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline PolyFluid* GunnsFluidNode::getOutflow()
{
    return &mOutflow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (W) The net heat flux into the node.
///
/// @details  Returns the net heat flux into the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidNode::getNetHeatFlux() const
{
    return mNetHeatFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   PolyFluidConfigData* -- Returns a pointer to the node fluid's configuration data
///
/// @details  Returns the fluid config data pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const PolyFluidConfigData* GunnsFluidNode::getFluidConfig() const
{
    return mFluidConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method calculates the discrepancy between the theoretical node mass represented
///           by the fluid properties (density) and the actual mass.  This discrepancy arises from
///           the linearizations of pressure and temperature in the network solution, and it is
///           corrected for later so that mass is conserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidNode::computeMassError()
{
    mMassError = mContent.getMass() - mContent.getDensity() * mVolume;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Resets the node content mass based on the node volume and content density.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidNode::updateMass()
{
    mContent.setMass(mContent.getDensity() * mVolume);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Saves the current fluid content temperature for use in the next pass.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidNode::updatePreviousTemperature()
{
    mPreviousTemperature = mContent.getTemperature();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Saves the current fluid content pressure for use in the next pass.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidNode::updatePreviousPressure()
{
    mPreviousPressure = mContent.getPressure();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @returns  bool  (--)  True if the node is overflowing.
///
/// @details  The node is overflowing if the amount of moles scheduled to flow out this step is
///           greater than the current amount in the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsFluidNode::isOverflowing(const double dt) const
{
    return mScheduledOutflux * dt > mContent.getMole();
}

#endif
