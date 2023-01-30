#ifndef TsHsTermination_EXISTS
#define TsHsTermination_EXISTS

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_TERMINATION_PLUGIN Termination Plugin
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Component of the Health & Status message framework. Can terminate the simulation
   upon receiving a fatal error message.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- (
     (TsHsTermination.o)
  )

PROGRAMMERS:
- (
     ((Collin Blake)    (CACI)               (January 2023) (Revision to remove static)
      (Wesley A. White) (Tietronix Software) (June 2013) (Initial version))
  )

@{
*/

#include <string>
#include <set>
#include "TS_hs_msg_types.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   TsHsTermination terminates the simulation under certain conditions.
///          If a subsystem attempts to log a TS_HS_FATAL message and
///          that subsystem has been listed as one that should terminate on
///          fatal errors, then the class will terminate the simulation.
///          <b> Developer note:</b> developers can register subsystems by calling the setFatal*
///          and appendFatal* methods in the HS default data class or input file.
///
/// @details The following example shows how to change the behavior of the terminate class via
///          the HS default data file.
/// @code
///          <TerminationInstance>.setEnabled(true);
///          <TerminationInstance>setFatalToMultipleSubsystems("ECLSS:JEOD:MECH");
///
/// @endcode
/// @details The string "ECLSS:JEOD:MECH" is used to identify those systems for which TS_HS_FATAL
///          causes sim termination. The string can contain multiple subsystems if they are
///          delimited by colon, comma, or semi-colon characters. Continuing the example above,
///          sending the following message would result in the simulation being terminated:
/// @code
///          hsSendMsg(TS_HS_FATAL, "ECLSS", "TsInitializationException while updating network.");
/// @endcode
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsHsTermination
{
public:

    /// @brief Constructor
    TsHsTermination();

    // @brief Destructor
   ~TsHsTermination();

    /// @brief Log a message.
    bool msg(const std::string& file, const int line, const std::string& function, TS_HS_MSG_TYPE type, const std::string& subsystems, const std::string& hsmsg);

    /// @brief Turn on/off sim termination behavior for the specified subsystem
    void setFatalToSubsystem(const std::string subsystem, bool fatal = true);

    /// @brief Turn on sim termination behavior for the specified delimited list of subsystems (only)
    void setFatalToMultipleSubsystems(const std::string subsystems);

    /// @brief Turn on sim termination behavior for the specified delimited list of subsystems
    void appendFatalToMultipleSubsystems(const std::string subsystems);

    /// @brief Test whether or not termination behavior is "on" for the specified subsystem
    bool isFatalToSubsystem(const std::string subsystem);

    /// @brief Returns true if H&S terminations are enabled
    bool isEnabled() { return mEnabled; }

    /// @brief Enables/disables H&S terminations
    void setEnabled(bool flag) { mEnabled = flag; }

private:

    typedef std::set<std::string> StringSet;

    StringSet mFatalToSubsystem; // ** (--) Used to make termination conditional by subsystem
    bool      mEnabled;          // ** (--) Disables ability to term

    // Not implemented
    TsHsTermination(const TsHsTermination&);
    TsHsTermination& operator=(const TsHsTermination&);
};

/// @}

#endif /* TsHsTermination_EXISTS */
