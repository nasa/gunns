#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "simulation/hs/TsHsStdMngr.hh"
#include "TsHsStdMngrDefaultData.hh"

void TsHsStdMngrDefaultData::initialize(TsHsStdMngr* mgr)
{
    mgr->pluginConfig[TsHsStdMngr::TS_HS_TEXT].enabled = true;
    TS_STRDUP(mgr->pluginConfig[TsHsStdMngr::TS_HS_TEXT].path, "logs/TS_Health_and_Status.out");

    mgr->pluginConfig[TsHsStdMngr::TS_HS_SQLITE].enabled = true;
    TS_STRDUP(mgr->pluginConfig[TsHsStdMngr::TS_HS_SQLITE].path, "logs/TS_Health_and_Status.sqlite");

    mgr->pluginConfig[TsHsStdMngr::TS_HS_TERMINATE].enabled = true;
    /*
     * the path value is used to identify those systems for which TS_HS_FATAL
     * causes sim termination
     *     ex:
     *      hsSendMsg(TS_HS_FATAL, "ECLSS", "TsInitializationException while updating network.");
     *      would require the value of path to include ECLSS surrounded by valid delimiters.
     *      the valid delimiters are the characters in the following string: ":,; "
     */
    TS_STRDUP(mgr->pluginConfig[TsHsStdMngr::TS_HS_TERMINATE].path, "Math");
}
