#ifndef GunnsFluid3WayValve_EXISTS
#define GunnsFluid3WayValve_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_THREE_WAY_VALVE    Three Way Valve Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Class for the GUNNS Three Way Valve.  This consists of two normal valves that share the same exit
   port.  Their positions are coordinated by this parent class to sum to 1.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluid3WayValve.o))

PROGRAMMERS:
- (
   (Mike Moore)   (L3) (2011-02) (Added to GUNNS Fluid Conductors)
   (Jason Harvey) (L3) (2012-12) (Redesign to embed 2 GunnsFluidValve)
  )

@{
*/

#include "aspects/fluid/conductor/GunnsFluidValve.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Three Way Valve Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Three Way Valve
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluid3WayValveConfigData : public GunnsFluidLinkConfigData
{
    public:
        bool   mPositionPathB;         /**< (--) trick_chkpnt_io(**) Valve position is relative to flow path B (false = A) */
        double mMaxConductivityA;      /**< (m2) trick_chkpnt_io(**) Path A maximum flow conductivity                      */
        double mMaxConductivityB;      /**< (m2) trick_chkpnt_io(**) Path B maximum flow conductivity                      */
        double mExpansionScaleFactorA; /**< (--) trick_chkpnt_io(**) Path A thermal expansion scale factor                 */
        double mExpansionScaleFactorB; /**< (--) trick_chkpnt_io(**) Path B thermal expansion scale factor                 */
        double mThermalLengthA;        /**< (m)  trick_chkpnt_io(**) Path A thermal convection length                      */
        double mThermalLengthB;        /**< (m)  trick_chkpnt_io(**) Path B thermal convection length                      */
        double mThermalDiameterA;      /**< (m)  trick_chkpnt_io(**) Path A thermal convection diameter                    */
        double mThermalDiameterB;      /**< (m)  trick_chkpnt_io(**) Path B thermal convection diameter                    */
        double mSurfaceRoughnessA;     /**< (m)  trick_chkpnt_io(**) Path A thermal surface roughness                      */
        double mSurfaceRoughnessB;     /**< (m)  trick_chkpnt_io(**) Path B thermal surface roughness                      */
        double mNonMixingBand;         /**< (--) trick_chkpnt_io(**) Fraction (0-1] of valve range centered on 0.5 in which both paths are shut. */
        /// @brief Default constructs this Three Way Valve configuration data.
        GunnsFluid3WayValveConfigData(const std::string& name                  = "",
                                      GunnsNodeList*     nodes                 = 0,
                                      const bool         positionPathB         = false,
                                      const double       maxConductivityA      = 0.0,
                                      const double       maxConductivityB      = 0.0,
                                      const double       expansionScaleFactorA = 0.0,
                                      const double       expansionScaleFactorB = 0.0,
                                      const double       thermalLengthA        = 0.0,
                                      const double       thermalLengthB        = 0.0,
                                      const double       thermalDiameterA      = 0.0,
                                      const double       thermalDiameterB      = 0.0,
                                      const double       surfaceRoughnessA     = 0.0,
                                      const double       surfaceRoughnessB     = 0.0,
                                      const double       nonMixingBand         = 0.0);
        /// @brief Copy constructs this Three Way Valve configuration data.
        GunnsFluid3WayValveConfigData(const GunnsFluid3WayValveConfigData& that);
        /// @brief Default destructs this Three Way Valve configuration data.
        virtual ~GunnsFluid3WayValveConfigData();
    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluid3WayValveConfigData& operator =(const GunnsFluid3WayValveConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Three Way Valve Input Data
///
/// @details  This provides a data structure for the Three Way Valve input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluid3WayValveInputData : public GunnsFluidLinkInputData
{
    public:
        double mPosition;         /**< (--) (0-1) Valve position relative to config data path */
        double mWallTemperatureA; /**< (K)  Path A wall temperature for thermal convection    */
        double mWallTemperatureB; /**< (K)  Path B wall temperature for thermal convection    */
        /// @brief Default constructs this Three Way Valve input data.
        GunnsFluid3WayValveInputData(const double position         = 0.0,
                                     const double wallTemperatureA = 0.0,
                                     const double wallTemperatureB = 0.0);
        /// @brief Copy constructs this Three Way Valve input data.
        GunnsFluid3WayValveInputData(const GunnsFluid3WayValveInputData& that);
        /// @brief Default destructs this Three Way Valve input data.
        virtual ~GunnsFluid3WayValveInputData();
    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluid3WayValveInputData& operator =(const GunnsFluid3WayValveInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Three Way Valve
///
/// @details  This class models the fluid aspect of a three-way valve.  It has 3 ports - two inputs
///           and an exit.  The link models the two flow paths of the inputs to the exit like a "V"
///           shape, where the tops of the V are the input ports and the vertex is the exit port.
///           Inputs A and B each flow to the exit, but not directly between each other.  The node
///           attached to the exit port is where the A & B flow paths mix.  Flow can go in either
///           direction in each path independently of the other, based on the relative pressures at
///           all three ports.  Flow is defined positive going from an input port to the exit, and
///           negative going from the exit port to an input port.
///
///           This class embeds two normal two-way valves to represent these flow paths.  A single
///           overall 3-way valve position governs the relative positions of both 2-ways.  The 3-way
///           position can be defined relative to either path A or B (default A), and the other
///           path's position is calculated such that (path A position) + (path B position) = 1.
///           Intermediate positions partially open both paths, allowing both paths to flow and mix
///           to or split from the exit.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluid3WayValve : public GunnsFluidLink
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluid3WayValve);

    public:

        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        GunnsFluidValve mPathA;                     /**< (--) Path A valve */
        GunnsFluidValve mPathB;                     /**< (--) Path B valve */
        /// @}

        /// @brief Default constructor.
        GunnsFluid3WayValve();
        /// @brief Default destructor.
        virtual ~GunnsFluid3WayValve();
        /// @brief Initialize method.
        void initialize(const GunnsFluid3WayValveConfigData& configData,
                        const GunnsFluid3WayValveInputData&  inputData,
                        std::vector<GunnsBasicLink*>&        networkLinks,
                        const int                            port0,
                        const int                            port1,
                        const int                            port2);
        /// @brief Steps the 3-way valve.
        virtual void step(const double dt);
        /// @brief Computes flows across the link.
        virtual void computeFlows(const double dt);
        /// @brief Transports flows through the link.
        virtual void transportFlows(const double dt);
        /// @brief Updates the State of the 3-way valve.
        virtual void updateState(const double dt);
        /// @brief Gets the 3-way valve position.
        virtual double getPosition();
        /// @brief Sets the 3-way valve position.
        virtual void setPosition(const double position);
        /// @brief Gets a pointer to the path A valve object.
        GunnsFluidValve* getPathA();
        /// @brief Gets a pointer to the path B valve object.
        GunnsFluidValve* getPathB();

    protected:
        bool            mPositionPathB;             /**< (--) trick_chkpnt_io(**) Valve position is relative to flow path B (false = A) */
        double          mNonMixingBand;             /**< (--) trick_chkpnt_io(**) Fraction (0-1] of valve range centered on 0.5 in which both paths are shut. */
        double          mPosition;                  /**< (--)                     Three way valve position. Initialized, then read from sim-bus */
        /// @brief Validates the initialization of this Gunns Three Way Valve.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 3};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluid3WayValve(const GunnsFluid3WayValve& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluid3WayValve& operator =(const GunnsFluid3WayValve& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  position  (--)  (0-1) 3-way valve fluid position.
///
/// @details  Sets the 3-way valve position.  The given position should be 0-1, 0 representing fully
///           closed and 1 representing fully open of the path given by the config parameter
///           mPositionPathB (false = path A).  The path's 2-way valve positions always sum to 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluid3WayValve::setPosition(const double position)
{
    mPosition = position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  position  (--)  (0-1) 3-way valve fluid position.
///
/// @details  Gets the 3-way valve position.  The position should be 0-1, 0 representing fully
///           closed and 1 representing fully open of the path given by the config parameter
///           mPositionPathB (false = path A).  The path's 2-way valve positions always sum to 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluid3WayValve::getPosition()
{
    return mPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsFluidValve*  (--)  Pointer to the path A valve object.
///
/// @details  Returns a pointer to GunnsFluidValve mPathA.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsFluidValve* GunnsFluid3WayValve::getPathA()
{
    return &mPathA;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsFluidValve*  (--)  Pointer to the path B valve object.
///
/// @details  Returns a pointer to GunnsFluidValve mPathB.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsFluidValve* GunnsFluid3WayValve::getPathB()
{
    return &mPathB;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (not used)
///
/// @details  The path A & B sub-valve links perform their own transprt.  The base class requires
///           this to be overriden, so we leave this empty.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluid3WayValve::transportFlows(const double dt __attribute__((unused)))
{
    // nothing to do
}

#endif
