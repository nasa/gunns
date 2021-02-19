#ifndef GunnsElectSelector_EXISTS
#define GunnsElectSelector_EXISTS

/**
@file
@brief     GUNNS Electrical Selector declarations

@defgroup  TSM_GUNNS_ELECTRICAL_SWITCH_SELECTOR    GUNNS Electrical Selector Link
@ingroup   TSM_GUNNS_ELECTRICAL_SWITCH

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Electrical Selector Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsElectSelector.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2016-05) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicConductor.hh"
#include <vector>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Selector Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Electrical
///           Selector link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectSelectorConfigData : public GunnsBasicLinkConfigData
{
    public:
        double                   mDefaultConductance; /**< (1/ohm) trick_chkpnt_io(**) Default conductance of the selector connection. */
        std::vector<std::string> mPortLabels;         /**< (--)    trick_chkpnt_io(**) Optional vector of port labels for runtime display. */
        /// @brief Default constructs this Electrical Selector configuration data.
        GunnsElectSelectorConfigData(const std::string&        name               = "",
                                     GunnsNodeList*            nodes              = 0,
                                     const double              defaultConductance = 0.0,
                                     std::vector<std::string>* portLabels         = 0);
        /// @brief Default destructs this Electrical Selector configuration data.
        virtual ~GunnsElectSelectorConfigData();
        /// @brief Copy constructs this Electrical Selector configuration data.
        GunnsElectSelectorConfigData(const GunnsElectSelectorConfigData& that);

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectSelectorConfigData& operator =(const GunnsElectSelectorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Selector Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Electrical
///           Selector link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectSelectorInputData : public GunnsBasicLinkInputData
{
    public:
        int mSelector;                                   /**< (--) Initial selected connection #. */
        /// @brief    Default constructs this Electrical Selector input data.
        GunnsElectSelectorInputData(const bool   malfBlockageFlag  = false,
                                    const double malfBlockageValue = 0.0,
                                    const int    selector          = 0);
        /// @brief    Default destructs this Electrical Selector input data.
        virtual ~GunnsElectSelectorInputData();
        /// @brief    Copy constructs this Electrical Selector input data.
        GunnsElectSelectorInputData(const GunnsElectSelectorInputData& that);

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsElectSelectorInputData& operator =(const GunnsElectSelectorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Selector Port Label
///
/// @details  The sole purpose of this class is to contain a string holding the label of a port in
///           a GunnsElectSelector link.  Trick can't create dynamic arrays of std::strings, but it
///           can create a dynamic array of class objects (like this) that themselves contain a
///           string.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectSelectorPortLabel
//TODO move this to core/ and rename GunnsNamedObject
{
    public:
        std::string mName;                        /**< (--) trick_chkpnt_io(**) Label for display. */
        /// @brief    Default constructs this Electrical Selector Port Label.
        GunnsElectSelectorPortLabel();
        /// @brief    Default destructs this Electrical Selector Port Label.
        virtual ~GunnsElectSelectorPortLabel();

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        GunnsElectSelectorPortLabel(const GunnsElectSelectorPortLabel& that);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsElectSelectorPortLabel& operator =(const GunnsElectSelectorPortLabel&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Selector Class
///
/// @details  This link creates a conductance from the input port 0 to the output port selected by
///           the user.  The user can change the output port selection in runtime.  The link is
///           dynamically configured with one or more output ports at init time.  The output port
///           numbers range from 1 to the total number of ports - 1.  In runtime, a port selection
///           <= 0 or >= the number of ports turns off the connection, isolating all ports.  The
///           connection is similar in function to the GunnsBasicConductor link.  Current can flow
///           in either direction.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectSelector : public GunnsBasicLink
{
        TS_MAKE_SIM_COMPATIBLE(GunnsElectSelector);

    public:
        /// @brief Default Electrical Selector Constructor.
        GunnsElectSelector();
        /// @brief Default Electrical Selector Destructor.
        virtual ~GunnsElectSelector();
        /// @brief Initialize method.
        void initialize(const GunnsElectSelectorConfigData& configData,
                        const GunnsElectSelectorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&       networkLinks,
                        std::vector<int>*                   portsVector);
        /// @brief Updates the link contributions to the network system of equations.
        virtual void step(const double dt);
        /// @brief Computes the flows across the link and related parameters.
        virtual void computeFlows(const double dt);
        /// @brief Sets the default conductance of the selector connection.
        void   setDefaultConductance(const double conductance);
        /// @brief Returns the default conductance of the selector connection.
        double getDefaultConductance() const;
        /// @brief Sets the selected connection #.
        void   setSelector(const int selector);
        /// @brief Returns the actual connected port #.
        int    getConnectedPort() const;

    protected:
        double                       mDefaultConductance;   /**< (1/ohm)                     Default conductance of the selector connection. */
        //TODO GunnsNamedObject*
        GunnsElectSelectorPortLabel* mPortLabels;           /**< (--)    trick_chkpnt_io(**) Port labels for runtime display. */
        int                          mSelector;             /**< (--)                        Selected connection #. */
        double                       mEffectiveConductance; /**< (1/ohm) trick_chkpnt_io(**) Actual conductance of the selector connection. */
        int                          mLastConnectedPort;    /**< (--)    trick_chkpnt_io(**) Last-pass connected port #. */
        double                       mSystemAdmittance;     /**< (--)    trick_chkpnt_io(**) Limited conductance for the system admittance matrix. */
        double                       mSystemSource;         /**< (--)    trick_chkpnt_io(**) Source flux for the system source vector. */
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Validates the link initialization data.
        void validate(const GunnsElectSelectorConfigData& configData,
                      const GunnsElectSelectorInputData&  inputData,
                      std::vector<int>*                   portsVector) const;
        /// @brief Updates the link's contributions to the system admittance matrix.
        void buildAdmittance();
        /// @brief Updates the thru-flux due to the latest network potentials.
        virtual void computeFlux();
        /// @brief Updates the power dissipated through the connection.
        virtual void computePower();

    private:
        /// @brief Clears the link admittance matrix when the port connection has changed.
        void clearAdmittance();
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectSelector(const GunnsElectSelector& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectSelector& operator =(const GunnsElectSelector& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  conductance  (1/ohm)  The default conductance of the selector connection.
///
/// @details  Sets the default conductance of the selector connection.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectSelector::setDefaultConductance(const double conductance)
{
    mDefaultConductance = conductance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (1/ohm)  The default conductance of the selector connection.
///.
/// @details  Returns the default conductance of the selector connection.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectSelector::getDefaultConductance() const
{
    return mDefaultConductance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  selector  (--)  The desired connection port #.
///
/// @details  Sets the selected connection #.  Note that any value can be stored, but a value < 1 or
///           greater than the highest port # will result in no actual connection.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectSelector::setSelector(const int selector)
{
    mSelector = selector;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the power dissipated through the link connection.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectSelector::computePower()
{
    mPower = -mFlux * mPotentialDrop;
}

#endif
