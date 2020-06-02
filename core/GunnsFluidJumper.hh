#ifndef GunnsFluidJumper_EXISTS
#define GunnsFluidJumper_EXISTS

/**
@file
@brief    GUNNS Fluid Jumper Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID_JUMPER    GUNNS Fluid Jumper Link
@ingroup   TSM_GUNNS_CORE_LINK_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Jumper Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsFluidJumper.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-08) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsFluidConductor.hh"

class GunnsFluidJumperPlug;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Jumper Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Jumper
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidJumperConfigData : public GunnsFluidConductorConfigData
{
    public:
        GunnsFluidJumperPlug* mPlug0;                 /**< (--) trick_chkpnt_io(**) Pointer to jumper plug for port 0 */
        GunnsFluidJumperPlug* mPlug1;                 /**< (--) trick_chkpnt_io(**) Pointer to Jumper plug for port 1 */
        bool                  mSelfSealing0;          /**< (--) trick_chkpnt_io(**) Plug 0 self-seals when un-plugged */
        bool                  mSelfSealing1;          /**< (--) trick_chkpnt_io(**) Plug 1 self-seals when un-plugged */
        /// @brief Default constructs this Fluid Jumper configuration data.
        GunnsFluidJumperConfigData(const std::string&    name                 = "",
                                   GunnsNodeList*        nodes                = 0,
                                   const double          maxConductivity      = 0.0,
                                   const double          expansionScaleFactor = 0.0,
                                   GunnsFluidJumperPlug* plug0                = 0,
                                   GunnsFluidJumperPlug* plug1                = 0,
                                   const bool            selfSealing0         = false,
                                   const bool            selfSealing1         = false);
        /// @brief Default destructs this Fluid Jumper configuration data.
        virtual ~GunnsFluidJumperConfigData();
        /// @brief Copy constructs this Fluid Jumper configuration data.
        GunnsFluidJumperConfigData(const GunnsFluidJumperConfigData& that);
    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidJumperConfigData& operator =(const GunnsFluidJumperConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Jumper Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Jumper
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidJumperInputData : public GunnsFluidConductorInputData
{
    public:
        bool   mMalfLeakRateFlag;                  /**< (--)   Leak-thru malfunction flag         */
        double mMalfLeakRateValue;                 /**< (kg/s) Leak-thru malfunction value        */
        int    mConnection0;                       /**< (--)   Index of the initial plug 0 socket */
        int    mConnection1;                       /**< (--)   Index of the initial plug 1 socket */
        /// @brief    Default constructs this Fluid Jumper input data.
        GunnsFluidJumperInputData(const bool   malfBlockageFlag  = false,
                                  const double malfBlockageValue = 0.0,
                                  const bool   malfLeakRateFlag  = false,
                                  const double malfLeakRateValue = 0.0,
                                  const int    connection0       = -1,
                                  const int    connection1       = -1);
        /// @brief    Default destructs this Fluid Jumper input data.
        virtual ~GunnsFluidJumperInputData();
        /// @brief    Copy constructs this Fluid Jumper input data.
        GunnsFluidJumperInputData(const GunnsFluidJumperInputData& that);
    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidJumperInputData& operator =(const GunnsFluidJumperInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Jumper Class
///
/// @details  The Fluid Jumper class is a conductor that connects to sockets at one or both ends.
///           The jumper receives controls from the user to connect to or disconnect from various
///           sockets in the network.  The jumper requests connection to or disconnection from the
///           commanded sockets, and then maps itself to the network node that the socket returns
///           access to (see the description of the GunnsFluidSocket and GunnsFluidJumperPlug
///           classes).
///
///           The jumper interfaces with the sockets via its plugs.  It has a plug object at each
///           end.  Note that these plugs are optional - if a plug is not provided to the jumper in
///           its config data (i.e. null pointer), then the jumper has no plug at that end and the
///           end is affixed to the initial node, as with a normal link.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidJumper : public GunnsFluidConductor
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluidJumper);

    public:
        /// @details  Declared public to allow external objects to command the plugs.
        GunnsFluidJumperPlug* mPlug[2];             /**< (--)   trick_chkpnt_io(**) Pointers to jumper plugs          */

        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool                  mMalfLeakRateFlag;    /**< (--)   Leak-thru malfunction flag        */
        double                mMalfLeakRateValue;   /**< (kg/s) Leak-thru malfunction value       */
        /// @}

        /// @brief  Default Fluid Jumper Constructor
        GunnsFluidJumper();
        /// @brief  Default Fluid Jumper Destructor
        virtual ~GunnsFluidJumper();
        /// @brief  Initialize method
        void initialize(const GunnsFluidJumperConfigData& configData,
                        const GunnsFluidJumperInputData&  inputData,
                        std::vector<GunnsBasicLink*>&     networkLinks,
                        const int                         port0,
                        const int                         port1);
        /// @brief  Sets and resets the leak rate malfunction
        void setMalfLeakRate(const bool flag = false, const double value = 0.0);

    protected:
        bool                  mSelfSealing0;        /**< (--)   trick_chkpnt_io(**) Plug 0 self-seals when un-plugged */
        bool                  mSelfSealing1;        /**< (--)   trick_chkpnt_io(**) Plug 1 self-seals when un-plugged */
        double                mPreviousLeakRate;    /**< (kg/s)                     Previous leak malf value          */
        double                mLeakConductivity;    /**< (m2)                       Conductivity of the leak-thru     */
        /// @brief  Validates the Fluid Jumper input data.
        void validate(const GunnsFluidJumperInputData& inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Updates the State of the Fluid Jumper.
        virtual void updateState(const double);
        /// @brief Updates the jumper plugs.
        void updatePlugs();
        /// @brief Checks for duplicate port node assignment.
        virtual bool checkPortDuplication(const int, const int) const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidJumper(const GunnsFluidJumper& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidJumper& operator =(const GunnsFluidJumper& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--) Always returns true.
///
/// @details  Overrides the GunnsFluidLink implementation to allow the jumper link to connect both
///           ports to the same node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsFluidJumper::checkPortDuplication(const int, const int) const
{
    return true;
}

/// @}

#endif
