#ifndef GunnsFluidFireSource_EXISTS
#define GunnsFluidFireSource_EXISTS

/**
@file
@brief    GUNNS Fluid Fire Source Link declarations

@defgroup  TSM_GUNNS_FLUID_SOURCE_FIRE    GUNNS Fluid Fire Source Link
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Fire Source link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidFireSource.o))

PROGRAMMERS:
- ((Collin Blake) (CACI) (2018-08) (Initial))

@{
*/

#include "core/GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Fire Source Configuration Data
///
/// @details  This class provides a data structure for the Fluid Fire Source configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidFireSourceConfigData : public GunnsFluidLinkConfigData
{
    public:
        double  mFireO2ConsumpRate;      /**< (kg/J) trick_chkpnt_io(**) O2 mass consumption per Joule of fire energy output. */
        double  mFireCO2ProductRate;     /**< (kg/J) trick_chkpnt_io(**) CO2 mass production per Joule of fire energy output. */
        double  mFireH2OProductRate;     /**< (kg/J) trick_chkpnt_io(**) H2O mass production per Joule of fire energy output. */
        double  mFireMinReqO2;           /**< (kPa)  trick_chkpnt_io(**) Minimum partial pressure of O2 to sustain fire. */
        double* mFireTraceCompoundRates; /**< (kg/J) trick_chkpnt_io(**) Array of trace compound mass production per Joule of fire energy output. */
        /// @brief Default constructs this Fluid Fire Source configuration data.
        GunnsFluidFireSourceConfigData(const std::string& name                   = "",
                                       GunnsNodeList*     nodes                  = 0,
                                       const double       fireO2ConsumpRate      = 0.0,
                                       const double       fireCO2ProductRate     = 0.0,
                                       const double       fireH2OProductRate     = 0.0,
                                       const double       fireMinReqO2           = 0.0,
                                       double*            fireTraceCompoundRates = 0);
        /// @brief Default destructs this Fluid Fire Source configuration data.
        virtual ~GunnsFluidFireSourceConfigData();
        /// @brief Copy constructs this Fluid Fire Source configuration data.
        GunnsFluidFireSourceConfigData(const GunnsFluidFireSourceConfigData& that);

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidFireSourceConfigData& operator =(const GunnsFluidFireSourceConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Fire Source Input Data
///
/// @details  This class provides a data structure for the Fluid Fire Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidFireSourceInputData : public GunnsFluidLinkInputData
{
    public:
        bool    mMalfFireFlag;       /**< (--) Initial fire malfunction activation flag. */
        double  mMalfFireHeat;       /**< (W)  Initial fire malfunction generated heat value. */
        /// @brief    Default constructs this Fluid Fire Source input data.
        GunnsFluidFireSourceInputData(const bool   malfBlockageFlag  = false,
                                      const double malfBlockageValue = 0.0,
                                      const bool   malfFireFlag      = false,
                                      const double malfFireHeat      = 0.0);
        /// @brief    Default destructs this Fluid Fire Source input data.
        virtual ~GunnsFluidFireSourceInputData();
        /// @brief    Copy constructs this Fluid Fire Source input data.
        GunnsFluidFireSourceInputData(const GunnsFluidFireSourceInputData& that);

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidFireSourceInputData& operator =(const GunnsFluidFireSourceInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Fire Source class
///
/// @details  This link forces fluid into and out of a single node it accordance with a fire.
///           It consumes O2 from the node and produces CO2, H2O and optional trace compounds.
///
///           This link is similar to GunnsFluidSource except for these key differences:
///           - This link is a 1-port link.
///           - It modifies three specific bulk fluid compounds (GUNNS_H2O, GUNNS_CO2, GUNNS_O2),
///             and these must be in the network.
///           - It generates heat specified by the user and scales the fluid consumption and
///             production rates in proportion to heat supplied.
///           - The internal fluid can contain trace compounds, making this link a source of trace
///             compounds to/from the node in proportion to heat supplied.
///
///           The fire is automatically extinguished, and the malfunction flag deactivated, when O2
///           in the node drops below a configured minimum.
///
///           This link doesn't use or set the mVolFlowRate and mPotentialDrop terms from the base
///           class.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidFireSource : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidFireSource);

    public:
        bool    mMalfFireFlag;           /**< (--)                       Fire malfunction activation flag. */
        double  mMalfFireHeat;           /**< (W)                        Fire malfunction generated heat value. */
        /// @brief Default Constructor.
        GunnsFluidFireSource();
        /// @brief Default Destructor.
        virtual ~GunnsFluidFireSource();
        /// @brief Initialize
        void initialize(const GunnsFluidFireSourceConfigData& configData,
                        const GunnsFluidFireSourceInputData&  inputData,
                        std::vector<GunnsBasicLink*>&         networkLinks,
                        const int                             port0);
        /// @brief Step method for updating the link.
        virtual void step(const double dt);
        /// @brief Method for computing the flows across the link.
        virtual void computeFlows(const double dt);
        /// @brief Method for transporting the flows across the link.
        virtual void transportFlows(const double dt);
        /// @brief Sets and resets the fire malfunction.
        void         setMalfFire(const bool flag = false, const double value = 0.0);
        /// @brief Moves the internal fluid to or from the attached node.
        virtual void transportFluid(const bool forcedOutflow,
                                    const int  fromPort = 0,
                                    const int  toPort   = 1);
        /// @brief Method to calculate the fluid to be added to the Network.
        virtual void updateFluid(const double dt, const double flowRate);

    protected:
        double  mFireO2ConsumpRate;      /**< (kg/J) trick_chkpnt_io(**) Rate of consumption of O2 per Watt. */
        double  mFireCO2ProductRate;     /**< (kg/J) trick_chkpnt_io(**) Rate of production of CO2 per Watt. */
        double  mFireH2OProductRate;     /**< (kg/J) trick_chkpnt_io(**) Rate of production of H2O per Watt. */
        int     miO2;                    /**< (--)   trick_chkpnt_io(**) Index of O2 in bulk fluid. */
        int     miCO2;                   /**< (--)   trick_chkpnt_io(**) Index of CO2 in bulk fluid. */
        int     miH2O;                   /**< (--)   trick_chkpnt_io(**) Index of H2O in bulk fluid. */
        double  mFlowO2;                 /**< (kg/s) trick_chkpnt_io(**) Consumption mass flow rate of O2. */
        double  mFlowCO2;                /**< (kg/s) trick_chkpnt_io(**) Production mass flow rate of CO2. */
        double  mFlowH2O;                /**< (kg/s) trick_chkpnt_io(**) Production mass flow rate of H2O. */
        double* mTraceCompoundRates;     /**< (--)   trick_chkpnt_io(**) Array of rates of production of trace compounds per Watt. */
        double* mTraceCompoundMassRates; /**< (--)   trick_chkpnt_io(**) Array of mass flow rates of trace compounds. */
        double  mFireMinReqO2;           /**< (kPa)  trick_chkpnt_io(**) Minimum required O2 content for fire. */
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Builds the source vector terms of the links contribution to the network.
        virtual void buildSource();
        /// @brief Validates the initialization of this GUNNS Fluid Fire Source.
        void         validate(const GunnsFluidFireSourceConfigData& configData) const;
        /// @brief Computes mPower for the aspect-specific implementation.
        virtual void computePower();
        /// @brief Generates all fluid constituent mass flows to be added/subtracted from node.
        void         calculateFlowRate();
        /// @brief Sets all the bulk fluid mass flow rates for the fluid transfer.
        void         setBulkMasses();
        /// @brief Sets all Trace Compounds mass flow rates for the fluid transfer.
        void         setTCMasses();
        /// @brief Sets all mass flow rates to zero.
        void         zeroGenValues();
        /// @brief Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 1};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidFireSource(const GunnsFluidFireSource& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidFireSource& operator =(const GunnsFluidFireSource& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Fire activation flag.
/// @param[in] value (W)  Fire heat output value.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidFireSource::setMalfFire(const bool flag, const double value)
{
    mMalfFireFlag = flag;
    mMalfFireHeat = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Builds the source vector terms of the link's contribution to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidFireSource::buildSource()
{
    mSourceVector[0] = mFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  For this link, mPower is just the heat output of the fire malfunction when active.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidFireSource::computePower()
{
    if (mMalfFireFlag){
        mPower = mMalfFireHeat;
    } else {
        mPower = 0.0;
    }
}

#endif
