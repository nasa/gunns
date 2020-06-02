#ifndef GunnsBasicSource_EXISTS
#define GunnsBasicSource_EXISTS

/**
@file
@brief    GUNNS Basic Source Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_BASIC_SOURCE    GUNNS Basic Source Link
@ingroup   TSM_GUNNS_CORE_LINK_BASIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Basic Source Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- (
   (GunnsBasicSource.o)
  )

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
/// @brief    Basic Source Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Source
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicSourceConfigData : public GunnsBasicLinkConfigData
{
    public:
        /// @brief Default constructs this Basic Source configuration data.
        GunnsBasicSourceConfigData(const std::string& name  = "",
                                   GunnsNodeList*     nodes = 0);

        /// @brief Default destructs this Basic Source configuration data.
        virtual ~GunnsBasicSourceConfigData();

        /// @brief Copy constructs this Basic Source configuration data.
        GunnsBasicSourceConfigData(const GunnsBasicSourceConfigData& that);
    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicSourceConfigData& operator =(const GunnsBasicSourceConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Source Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Source
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicSourceInputData : public GunnsBasicLinkInputData
{
    public:
        double mSourceFlux;                    /**< (--) Initial demanded flux of the link */

        /// @brief    Default constructs this Basic Source input data.
        GunnsBasicSourceInputData(const bool   malfBlockageFlag  = false,
                                  const double malfBlockageValue = 0.0,
                                  const double sourceFlux        = 0.0);

        /// @brief    Default destructs this Basic Source input data.
        virtual ~GunnsBasicSourceInputData();

        /// @brief    Copy constructs this Basic Source input data.
        GunnsBasicSourceInputData(const GunnsBasicSourceInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicSourceInputData& operator =(const GunnsBasicSourceInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basic Source
///
/// @details This class creates a fixed flux between two nodes.  One of the nodes may be
///          GROUND, or both may be normal network nodes.  Flow direction is defined positive
///          from Port0 to Port1, so it is best to use Port0 as the GROUND node.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicSource : public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsBasicSource);

    public:
        /// @brief Default Constructor
        GunnsBasicSource();

        /// @brief Default Destructor
        virtual ~GunnsBasicSource();

        /// @brief Initializes the link
        void initialize(const GunnsBasicSourceConfigData& configData,
                        const GunnsBasicSourceInputData&  inputData,
                        std::vector<GunnsBasicLink*>&     networkLinks,
                        const int                         port0,
                        const int                         port1);

        /// @brief Updates the link during a frame
        virtual void step(const double dt);

        /// @brief Calculates flow across the link
        virtual void computeFlows(const double);

        /// @brief Sets the flux demand
        void setFluxDemand(const double toFlux);

        /// @brief Gets the Source Flux
        double getFluxDemand() const;

    protected:
        double mSourceFlux;               /**< (--) Demanded generic flux through the link */

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Builds the source vector terms of the links contribution to the network
        virtual void buildSource();

        /// @brief Validates the link
        void validate() const;

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};

        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsBasicSource(const GunnsBasicSource& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicSource& operator =(const GunnsBasicSource& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] toFlux (--) Requested Flux Demand
///
/// @details Sets the Flux Demand
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicSource::setFluxDemand(const double toFlux)
{
    mSourceFlux = toFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return double (--) Returns the source flux
///
/// @details Gets the Flux Demand
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicSource::getFluxDemand() const
{
    return mSourceFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Builds the source vector terms of the links contribution to the network.  This sign
///          convention creates positive flow from port 0 to port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicSource::buildSource()
{
    mSourceVector[0] = -mFlux;
    mSourceVector[1] =  mFlux;
}

#endif
