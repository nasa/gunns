/********************************* TRICK HEADER *******************************
PURPOSE:
    (Issue a health & status message)

PROGRAMMERS:
    (
      ((Jeffrey Middleton) (L3) (January 2010) (Initial version))
    )
*******************************************************************************/

TsHsStdMngr.pluginConfig[TsHsStdMngr::TS_HS_TEXT].enabled = true;
TsHsStdMngr.pluginConfig[TsHsStdMngr::TS_HS_TEXT].path = "logs/CxTF_Health_and_Status.out";

TsHsStdMngr.pluginConfig[TsHsStdMngr::TS_HS_SQLITE].enabled = true;
TsHsStdMngr.pluginConfig[TsHsStdMngr::TS_HS_SQLITE].path = "logs/CxTF_Health_and_Status.sqlite";


TsHsStdMngr.pluginConfig[TsHsStdMngr::TS_HS_TERMINATE].enabled = true;
/*
 * the path value is used to identity those systems for which TS_HS_FATAL causes sim termination
 *     ex:
 *      TS_hs_msg(TS_HS_FATAL, "ECLSS", "TsInitializationException while updating network.");
 *      would require the value of path to include ECLSS surrounded by valid delimiters.
 *      the valid delimiters are the characters in the following string: ":,; "
 */
TsHsStdMngr.pluginConfig[TsHsStdMngr::TS_HS_TERMINATE].path = "Math";
