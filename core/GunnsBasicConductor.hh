#ifndef GunnsBasicConductor_EXISTS
#define GunnsBasicConductor_EXISTS

/**
@file
@brief    GUNNS Basic Conductor Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_BASIC_CONDUCTOR    GUNNS Basic Conductor Link
@ingroup   TSM_GUNNS_CORE_LINK_BASIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Basic Conductor Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsBasicConductor.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2011-02) (Initial Prototype)
-  (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsBasicLink.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Conductor Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Conductor
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicConductorConfigData : public GunnsBasicLinkConfigData
{
    public:
        double mDefaultConductivity;             /**< (--) trick_chkpnt_io(**) Default conductivity of the link */

        /// @brief Default constructs this Basic Conductor configuration data.
        GunnsBasicConductorConfigData(const std::string& name                = "",
                                      GunnsNodeList*     nodes               = 0,
                                      const double       defaultConductivity = 0.0);

        /// @brief Default destructs this Basic Conductor configuration data.
        virtual ~GunnsBasicConductorConfigData();

        /// @brief Copy constructs this Basic Conductor configuration data.
        GunnsBasicConductorConfigData(const GunnsBasicConductorConfigData& that);
    protected:

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicConductorConfigData& operator =(const GunnsBasicConductorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Conductor Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Conductor
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicConductorInputData : public GunnsBasicLinkInputData
{
    public:
        /// @brief    Default constructs this Basic Conductor input data.
        GunnsBasicConductorInputData(const bool   malfBlockageFlag  = false,
                                     const double malfBlockageValue = 0.0);

        /// @brief    Default destructs this Basic Conductor input data.
        virtual ~GunnsBasicConductorInputData();

        /// @brief    Copy constructs this Basic Conductor input data.
        GunnsBasicConductorInputData(const GunnsBasicConductorInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicConductorInputData& operator =(const GunnsBasicConductorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Conductor Class
///
/// @details  The Basic Conductor class is the equivalent of an electrical conduction path
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicConductor : public GunnsBasicLink
{
        TS_MAKE_SIM_COMPATIBLE(GunnsBasicConductor);

    public:
        /// @brief Default Basic Conductor Constructor
        GunnsBasicConductor();

        /// @brief Default Basic Conductor Destructor
        virtual ~GunnsBasicConductor();

        /// @brief Initialize method
        void initialize(const GunnsBasicConductorConfigData& configData,
                        const GunnsBasicConductorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&        networkLinks,
                        const int                            port0,
                        const int                            port1);

        /// @brief step method for updating the link
        virtual void step(const double dt);

        /// @brief Method for computing the flows across the link
        virtual void computeFlows(const double dt);

        /// @brief Sets the default conductivity of the link
        void   setDefaultConductivity(const double conductivity);

        /// @brief Returns the effective conductivity of the link
        double getEffectiveConductivity() const;

    protected:
        double mEffectiveConductivity; /**< (--) trick_chkpnt_io(**) Generic effective conductivity of the link */
        double mDefaultConductivity;   /**< (--) trick_chkpnt_io(**) Default conductivity of the link */
        double mSystemConductance;     /**< (--) trick_chkpnt_io(**) Limited conductance for the system of equations */

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Updates the admittance and source terms of the link
        virtual void buildConductance();

        /// @brief Validates the link
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
        GunnsBasicConductor(const GunnsBasicConductor& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicConductor& operator =(const GunnsBasicConductor& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  conductivity (--) The default conductivity of the Link.
///
/// @details  Sets the default conductivity of the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicConductor::setDefaultConductivity(const double conductivity)
{
    mDefaultConductivity = conductivity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The effective conductivity of the Link
///
/// @details  Returns the effective conductivity of the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicConductor::getEffectiveConductivity() const
{
    return mEffectiveConductivity;
}

#endif
