#ifndef GunnsFluidCapacitor_EXISTS
#define GunnsFluidCapacitor_EXISTS

/**
@file
@brief    GUNNS Fluid Capacitor Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID_CAPACITOR    GUNNS Fluid Capacitor Link
@ingroup   TSM_GUNNS_CORE_LINK_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Capacitor Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidCapacitor.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2011-02) (Initial Prototype)
-  (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))

@{
*/

#include "GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Capacitor Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Capacitor
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidCapacitorConfigData : public GunnsFluidLinkConfigData
{
    public:
        double mExpansionScaleFactor;   /**< (--) trick_chkpnt_io(**) Scaling for isentropic gas cooling (0-1) */

        /// @brief Default constructs this Fluid Capacitor configuration data.
        GunnsFluidCapacitorConfigData(const std::string& name                 = "",
                                      GunnsNodeList*     nodes                = 0,
                                      const double       expansionScaleFactor = 0.0);

        /// @brief Default destructs this Fluid Capacitor configuration data.
        virtual ~GunnsFluidCapacitorConfigData();

        /// @brief Copy constructs this Fluid Capacitor configuration data.
        GunnsFluidCapacitorConfigData(const GunnsFluidCapacitorConfigData& that);

    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidCapacitorConfigData& operator =(const GunnsFluidCapacitorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Capacitor Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Capacitor
///           input data.  This is used to initialize the capacitive node's initial fluid state.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidCapacitorInputData : public GunnsFluidLinkInputData
{
    public:
        double              mInitialVolume;      /**< (m3) Capacitive node's initial volume */
        PolyFluidInputData* mInitialFluidState;  /**< (--) Capacitive node's initial fluid state */

        /// @brief Default constructs this Fluid Capacitor input data.
        GunnsFluidCapacitorInputData(const bool          malfBlockageFlag  = false,
                                     const double        malfBlockageValue = 0.0,
                                     const double        initialVolume     = 0.0,
                                     PolyFluidInputData* initialFluidState = 0);

        /// @brief Default destructs this Fluid Capacitor input data.
        virtual ~GunnsFluidCapacitorInputData();

        /// @brief Copy constructs this Fluid Capacitor input data.
        GunnsFluidCapacitorInputData(const GunnsFluidCapacitorInputData& that);

    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidCapacitorInputData& operator =(const GunnsFluidCapacitorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Capacitor class
///
/// @details  This class creates fluid capacitance in the node at Port 0, which is used to simulate
///           volume in a fluid network.  This class controls the node volume and initializes the
///           fluid properties & mass within it.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidCapacitor : public GunnsFluidLink
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluidCapacitor);

    public:
        /// @brief Constructs the Fluid Capacitor
        GunnsFluidCapacitor();

        /// @brief Destructs the Fluid Capacitor
        virtual ~GunnsFluidCapacitor();

        /// @brief Initialize Method
        void initialize(const GunnsFluidCapacitorConfigData& configData,
                        const GunnsFluidCapacitorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&        networkLinks,
                        const int                            port0,
                        const int                            port1);

        /// @brief Step through a frame
        virtual void step(const double dt);

        /// @brief Computes the flow across the Capacitor
        virtual void computeFlows(const double dt);

        /// @brief Transports the flows through the link between the nodes
        virtual void transportFlows(const double dt);

        /// @brief Updates the derived class object internal fluid
        virtual void updateFluid(const double, const double);

        /// @brief Returns the volume of the Fluid Capacitor
        virtual double getVolume() const;

        /// @brief Returns the Heat Flux of the Fluid Capacitor
        virtual double getHeatFlux() const;

        /// @brief Returns the flux through the associated node.
        virtual double getFluxThrough() const;

        /// @brief Returns the capacitance of the Fluid Capacitor
        double getCapacitance() const;

        /// @brief Sets the volume edit controls
        void editVolume(const bool flag, const double volume);

    protected:
        double mCapacitance;               /**< (kg*mol/kPa) trick_chkpnt_io(**) Fluid molar capacitance applied to the capacitive node */
        double mExpansionScaleFactor;      /**< (--)         trick_chkpnt_io(**) Scaling for isentropic gas cooling (0-1) */
        bool   mDisablePressureCorrection; /**< (--)                             Disables mass-pressure corrections in the node */
        bool   mEditVolumeFlag;            /**< (--)         trick_chkpnt_io(**) Edit node volume activation flag */
        double mEditVolume;                /**< (m3)         trick_chkpnt_io(**) Edit node volume value */

        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Builds the capacitive admittance matrix for the link
        virtual void buildAdmittanceMatrix(const double dt);

        /// @brief Builds the capacitive source vector for the link
        virtual void buildSourceVector(const double dt);

        /// @brief Initializes the Fluid in the Capacitor
        virtual void initializeFluid(const PolyFluidInputData& initialFluidState);

        /// @brief Validates the initialization of this Gunns Fluid Capacitor
        void validate(const GunnsFluidCapacitorConfigData& configData,
                      const GunnsFluidCapacitorInputData&  inputData) const;

        /// @brief Checks for valid implementation-specific port node assignment
        virtual bool checkSpecificPortRules(const int port, const int node) const;

        /// @brief Returns the undamped heat flux of the Fluid Capacitor.
        virtual double getUndampedHeatFlux() const;

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};

        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidCapacitor(const GunnsFluidCapacitor& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidCapacitor& operator =(const GunnsFluidCapacitor& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details The GunnsFluidCapacitor should never produce a flow rate in the link, as that would
///          represent mass being added or deleted from the network.  Therefore we use this method
///          as a convenient place to force the link fluid flow rate to zero.  Derived classes may
///          have reason to create such a flow, in which case they can override this method.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidCapacitor::updateFluid(const double, const double)
{
    mFlowRate = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (m3) The volume
///
/// @details Method which returns the volume.  Derived classes can override this method to return
///          a dynamic volume that they control, as in the case of an accumulator bottle, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidCapacitor::getVolume() const
{
    return mNodes[0]->getVolume();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (W) The heat flux
///
/// @details Method which returns the heat flux.  Derived classes can override this method to
///          return a dynamic heat flux that they control, for example from a tank wall, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidCapacitor::getHeatFlux() const
{
    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (W) The heat flux
///
/// @details Method which returns the heat flux.  Derived classes can override this method to
///          return a the portion of heat flux that they want to be omitted from the fluid node's
///          thermal damping logic, for example the heat flux from a tank wall, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidCapacitor::getUndampedHeatFlux() const
{
    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (--) Flux through associated node
///
/// @details Method which returns the flux through the associated node.  Derived classes can
/// override this method.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidCapacitor::getFluxThrough() const
{
    return mNodes[0]->getFluxThrough();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return double (kg*mol/kPa) Returns the capacitance
///
/// @details Method which returns the capacitance.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidCapacitor::getCapacitance() const
{
    return mCapacitance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag    (--)  Activation flag for the edit.
/// @param[in]  volume  (--)  New volume the node will take during the edit.
///
/// @details  Sets the volume edit controls to the given values.  The edit will be performed and the
///           edit flag reset during the next link step.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidCapacitor::editVolume(const bool flag, const double volume)
{
    mEditVolume     = volume;
    mEditVolumeFlag = flag;
}

#endif
