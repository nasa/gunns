#ifndef GunnsBasicCapacitor_EXISTS
#define GunnsBasicCapacitor_EXISTS

/**
@file
@brief    GUNNS Basic Capacitor Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_BASIC_CAPACITOR    GUNNS Basic Capacitor Link
@ingroup   TSM_GUNNS_CORE_LINK_BASIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Basic Capacitor Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsBasicCapacitor.o))

PROGRAMMERS:
- (
   (Jason Harvey) (L3) (2011-02) (Initial Prototype))
   (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))
  )

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsBasicLink.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Capacitor Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Capacitor
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicCapacitorConfigData : public GunnsBasicLinkConfigData
{
    public:
        /// @brief Default constructs this Basic Capacitor configuration data.
        GunnsBasicCapacitorConfigData(const std::string& name         = "",
                                      GunnsNodeList*     nodes        = 0);

        /// @brief Default destructs this Basic Capacitor configuration data.
        virtual ~GunnsBasicCapacitorConfigData();

        /// @brief Copy constructs this Basic Capacitor configuration data.
        GunnsBasicCapacitorConfigData(const GunnsBasicCapacitorConfigData& that);
    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicCapacitorConfigData& operator =(const GunnsBasicCapacitorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Capacitor Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Capacitor
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicCapacitorInputData : public GunnsBasicLinkInputData
{
    public:
        /// @details  Note that although it is specified in input data, capacitance is really
        ///           treated as config data in that changes to it in run-time are not checkpointed.
        ///           It really belongs in the config data class, but we've left it in input data
        ///           for now because of the re-factoring effort this would impose on the users.
        double mCapacitance;                   /**< (--) Initial capacitance of the link */
        double mPotential;                     /**< (--) Initial potential of the capacitive node */

        /// @brief    Default constructs this Basic Capacitor input data.
        GunnsBasicCapacitorInputData(const bool   malfBlockageFlag  = false,
                                     const double malfBlockageValue = 0.0,
                                     const double capacitance       = 0.0,
                                     const double potential         = 0.0);

        /// @brief    Default destructs this Basic Capacitor input data.
        virtual ~GunnsBasicCapacitorInputData();

        /// @brief    Copy constructs this Basic Capacitor input data.
        GunnsBasicCapacitorInputData(const GunnsBasicCapacitorInputData& that);

    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicCapacitorInputData& operator =(const GunnsBasicCapacitorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Capacitor Class
///
/// @details  The purpose of this class is to model a generic capacitance.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicCapacitor : public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsBasicCapacitor);

    public:
        /// @brief Default Constructor
        GunnsBasicCapacitor();

        /// @brief Default Destructor
        virtual ~GunnsBasicCapacitor();

        /// @brief Capacitor Initialize method
        void initialize(const GunnsBasicCapacitorConfigData& configData,
                        const GunnsBasicCapacitorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&        networkLinks,
                        const int                            port0,
                        const int                            port1);

        /// @brief Step method for the frame
        virtual void step(const double dt);

        /// @brief Method for computing the flows across the link
        virtual void computeFlows(const double dt);

        /// @brief Returns Capacitance
        double getCapacitance() const;

        /// @brief Sets the capacitance edit controls
        void editCapacitance(const bool flag, const double capacitance);

        /// @brief Sets the capacitance directly
        void setCapacitance(const double capacitance);

    protected:
        /// @details  Note that although capacitance comes from input data and can change during
        ///           run-time, we do not checkpoint it - it is treated like config data with
        ///           respect to checkpoints.  This allows the default value to be changed in a new
        ///           delivery and not be wiped out when loading old checkpoints.
        double mCapacitance;         /**< (--) trick_chkpnt_io(**) Generic change in stored quantity per unit change in potential */
        bool   mEditCapacitanceFlag; /**< (--) trick_chkpnt_io(**) Edit capacitance activation flag */
        double mEditCapacitance;     /**< (--) trick_chkpnt_io(**) Edit capacitance value */

        /// @brief Method for telling Gunns to init the potential across the link
        void initPotential(const double toPotential);

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Method for Building the admittance source terms of the link
        virtual void buildCapacitance(const double dt);

        /// @brief Method for Validating the Link
        void validate() const;

        /// @brief Computes flux across the link
        virtual void computeFlux();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};

        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsBasicCapacitor(const GunnsBasicCapacitor& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicCapacitor& operator =(const GunnsBasicCapacitor& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Compute flux across the link, defined positive from port 0 to port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicCapacitor::computeFlux()
{
    mFlux = mPotentialDrop * mAdmittanceMatrix[0] - mSourceVector[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return double (--) The capacitance.
///
/// @details  Returns the capacitance.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicCapacitor::getCapacitance() const
{
    return mCapacitance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag         (--)  Activation flag for the edit.
/// @param[in]  capacitance  (--)  New capacitance the link will take during the edit.
///
/// @details  Sets the capacitance edit controls to the given values.  The edit will be performed
///           and the edit flag reset during the next link step.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicCapacitor::editCapacitance(const bool flag, const double capacitance)
{
    mEditCapacitance     = capacitance;
    mEditCapacitanceFlag = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  capacitance  (--)  New capacitance the link will take during the edit.
///
/// @details  Sets the capacitance to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicCapacitor::setCapacitance(const double capacitance)
{
    mCapacitance = capacitance;
}

#endif
