#ifndef GunnsBasicJumper_EXISTS
#define GunnsBasicJumper_EXISTS

/**
@file
@brief    GUNNS Basic Jumper Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_BASIC_JUMPER    GUNNS Basic Jumper Link
@ingroup   TSM_GUNNS_CORE_LINK_BASIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Basic Jumper Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsBasicJumper.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-08) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsBasicConductor.hh"

class GunnsBasicJumperPlug;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Jumper Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Jumper
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicJumperConfigData : public GunnsBasicConductorConfigData
{
    public:
        GunnsBasicJumperPlug* mPlug0;        /**< (--) trick_chkpnt_io(**) Pointer to jumper plug for port 0 */
        GunnsBasicJumperPlug* mPlug1;        /**< (--) trick_chkpnt_io(**) Pointer to Jumper plug for port 1 */
        bool                  mSelfSealing0; /**< (--) trick_chkpnt_io(**) Plug 0 self-seals when un-plugged */
        bool                  mSelfSealing1; /**< (--) trick_chkpnt_io(**) Plug 1 self-seals when un-plugged */
        /// @brief Default constructs this Basic Jumper configuration data.
        GunnsBasicJumperConfigData(const std::string&    name                = "",
                                   GunnsNodeList*        nodes               = 0,
                                   const double          defaultConductivity = 0.0,
                                   GunnsBasicJumperPlug* plug0               = 0,
                                   GunnsBasicJumperPlug* plug1               = 0,
                                   const bool            selfSealing0        = false,
                                   const bool            selfSealing1        = false);
        /// @brief Default destructs this Basic Jumper configuration data.
        virtual ~GunnsBasicJumperConfigData();
        /// @brief Copy constructs this Basic Jumper configuration data.
        GunnsBasicJumperConfigData(const GunnsBasicJumperConfigData& that);
    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicJumperConfigData& operator =(const GunnsBasicJumperConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Jumper Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Jumper
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicJumperInputData : public GunnsBasicConductorInputData
{
    public:
        int mConnection0;                            /**< (--) Index of the initial plug 0 socket */
        int mConnection1;                            /**< (--) Index of the initial plug 1 socket */
        /// @brief    Default constructs this Basic Jumper input data.
        GunnsBasicJumperInputData(const bool   malfBlockageFlag  = false,
                                  const double malfBlockageValue = 0.0,
                                  const int    connection0       = -1,
                                  const int    connection1       = -1);
        /// @brief    Default destructs this Basic Jumper input data.
        virtual ~GunnsBasicJumperInputData();
        /// @brief    Copy constructs this Basic Jumper input data.
        GunnsBasicJumperInputData(const GunnsBasicJumperInputData& that);
    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicJumperInputData& operator =(const GunnsBasicJumperInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Jumper Class
///
/// @details  The Basic Jumper class is a conductor that connects to sockets at one or both ends.
///           The jumper receives controls from the user to connect to or disconnect from various
///           sockets in the network.  The jumper requests connection to or disconnection from the
///           commanded sockets, and then maps itself to the network node that the socket returns
///           access to (see the description of the GunnsBasicSocket and GunnsBasicJumperPlug
///           classes).
///
///           The jumper interfaces with the sockets via its plugs.  It has a plug object at each
///           end.  Note that these plugs are optional - if a plug is not provided to the jumper in
///           its config data (i.e. null pointer), then the jumper has no plug at that end and the
///           end is affixed to the initial node, as with a normal link.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicJumper : public GunnsBasicConductor
{
        TS_MAKE_SIM_COMPATIBLE(GunnsBasicJumper);

    public:
        /// @brief  Default Basic Jumper Constructor
        GunnsBasicJumper();
        /// @brief  Default Basic Jumper Destructor
        virtual ~GunnsBasicJumper();
        /// @brief  Initialize method
        void initialize(const GunnsBasicJumperConfigData& configData,
                        const GunnsBasicJumperInputData&  inputData,
                        std::vector<GunnsBasicLink*>&     networkLinks,
                        const int                         port0,
                        const int                         port1);
        /// @details  Declared public to allow external objects to command the plugs.
        GunnsBasicJumperPlug* mPlug[2];      /**< (--) trick_chkpnt_io(**) Pointers to jumper plugs          */

    protected:
        bool                  mSelfSealing0; /**< (--) trick_chkpnt_io(**) Plug 0 self-seals when un-plugged */
        bool                  mSelfSealing1; /**< (--) trick_chkpnt_io(**) Plug 1 self-seals when un-plugged */
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Updates the State of the Basic Jumper.
        virtual void updateState(const double);
        /// @brief Updates the jumper plugs.
        void updatePlugs();
        /// @brief Checks for duplicate port node assignment.
        virtual bool checkPortDuplication(const int, const int) const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsBasicJumper(const GunnsBasicJumper& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicJumper& operator =(const GunnsBasicJumper& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--) Always returns true.
///
/// @details  Overrides the GunnsBasicLink implementation to allow the jumper link to connect both
///           ports to the same node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsBasicJumper::checkPortDuplication(const int, const int) const
{
    return true;
}

/// @}

#endif
