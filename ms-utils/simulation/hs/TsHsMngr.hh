#ifndef TsHsMngr_EXISTS
#define TsHsMngr_EXISTS

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_MANAGER  Manager
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
- (Health and status message manager base class)

SWIG: (No)

PROGRAMMERS:
- (((Jeffrey Middleton) (L3)))

@{
*/

#include <string>
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "TS_hs_msg_types.h"

/// @brief Health and Status (HS) logging base class. Provides interfaces for init, update, msg,
///        and shutdown methods. Provides an implementation for registerSelf method.
class TsHsMngr
{
    TS_MAKE_SIM_COMPATIBLE(TsHsMngr);

    public:

        /// @brief Default constructor
        TsHsMngr();

        /// @brief Destructor
        virtual ~TsHsMngr();

        /// @brief Should be called once to initialize the manager. Typically called during trick initialization.
        virtual void init();

        /// @brief Sets global pointer used by TsHsMsg class. Deprecated. Init does this now.
        virtual void registerSelf();

        /// @brief Should be called during a checkpoint restart
        virtual void restart() = 0;

        /// @brief Should be called periodically to update the manager. Typically called as a trick scheduled job.
        virtual void update() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief   Should be called as needed to log a message.
        ///
        /// @param[in] file      (--) name of file which initiated logging the message.
        /// @param[in] line      (--) line of file which initiated logging the message.
        /// @param[in] function  (--) function which initiated logging the message.
        /// @param[in] type      (--) the type of message (e.g. info, warning, etc.).
        /// @param[in] subsys    (--) the subsystem from which the message originated.
        /// @param[in] mtext     (--) the message text.
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void msg(const std::string& file, const int line, const std::string& function,
                TS_HS_MSG_TYPE  type, const std::string& subsys, const std::string& mtext) = 0;

        /// @brief Should be called once to shut down the manager. Typically called as a trick shutdown job.
        virtual void shutdown() = 0;

    private:

        // Not implemented
        TsHsMngr(const TsHsMngr&);
        const TsHsMngr& operator=(const TsHsMngr&);
};

extern TsHsMngr* tsGlobalHsMngr;

/// @}

#endif /* TsHsMngr_EXISTS */
