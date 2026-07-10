#!/usr/bin/env bash
# Run ${GUNNS_HOME}/draw/netexport.py either:
#  - on files whose path matches a provided REGEX (when 1 arg is given), or
#  - on all files listed in DEFAULT_XML_LIST (when no arg is given).
#
# Usage:
#   Regex mode:  ./export_all_gunnsdrawings.sh '.*\.xml$'
#   List mode:   ./export_all_gunnsdrawings.sh
#
# Optional:
#   DEBUG=1 to print regex match evaluations (regex mode)
#   PYTHON=/path/to/python to override interpreter

set -euo pipefail

# --- Default list defined at the top (edit these as needed) ---
# Paths can be relative or absolute; one per array element.
DEFAULT_XML_LIST=(
  "${GUNNS_HOME}/draw/drawings/DrawElect.xml"
  "${GUNNS_HOME}/draw/drawings/DrawFluid.xml"
  "${GUNNS_HOME}/draw/drawings/DrawFluidTc.xml"
  "${GUNNS_HOME}/sims/networks/basic/test/TestBasicNetwork.xml"
  "${GUNNS_HOME}/sims/networks/electrical/PvArrayRegTest.xml"
  "${GUNNS_HOME}/sims/networks/example_space_vehicle/ExampleCabinFluid.xml"
  "${GUNNS_HOME}/sims/networks/example_space_vehicle/ExampleCoolantLoopFluid.xml"
  "${GUNNS_HOME}/sims/networks/example_space_vehicle/ExampleHxThermal.xml"
  "${GUNNS_HOME}/sims/networks/example_space_vehicle/ExamplePassiveThermal.xml"
  "${GUNNS_HOME}/sims/networks/example_space_vehicle/ExamplePowerElect.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/TestFluidNetwork.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/distributedIf/DistributedIfFluid.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/distributedIf/DistributedIfFluidAccum.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/distributedIf/DistributedIfFluidLiquid.xml"
  "${GUNNS_HOME}/sims/networks/thermal/gunnshowThermalTest/GunnShowThermalTest.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow10.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow11.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow12.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow13.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow14.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow15.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow16.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow17.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow18.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow19.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow2.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow20.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow21.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow22.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow23.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow24.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow25.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow26.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow27.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow28.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow29.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow3.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow30.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow31.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow32.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow33.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow34.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow35.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow36.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow37.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow38.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow39.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow4.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow40.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow41.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow42.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow43.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow44.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow45.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow46.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow47.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow48.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow5.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow6.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow7.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow8.xml"
  "${GUNNS_HOME}/sims/networks/fluid/test/overflow/TestOverflow9.xml"
  "${GUNNS_HOME}/sims/SIM_mc/model/GunnsMcModelFluid.xml"
)

# --- Args: allow 0 or 1 ---
if [[ $# -gt 1 ]]; then
  echo "Usage: $0 [REGEX]"
  echo "Examples:"
  echo "  $0 '.*\\.xml$'   # regex mode"
  echo "  $0               # list mode (uses DEFAULT_XML_LIST defined in script)"
  exit 1
fi

regex="${1:-}"                 # empty in list mode
DEBUG="${DEBUG:-0}"

# --- Check GUNNS_HOME and resolve netexport.py path ---
if [[ -z "${GUNNS_HOME:-}" ]]; then
  echo "Error: GUNNS_HOME is not set. Please export GUNNS_HOME to your GUNNS installation root." >&2
  exit 1
fi

netexport_py="${GUNNS_HOME}/draw/netexport.py"
if [[ ! -f "$netexport_py" ]]; then
  echo "Error: netexport.py not found at: $netexport_py" >&2
  exit 1
fi

# --- Python interpreter (override with PYTHON env var if needed) ---
python_bin="${PYTHON:-python3}"
if ! command -v "$python_bin" >/dev/null 2>&1; then
  echo "Error: Python interpreter '$python_bin' not found in PATH." >&2
  exit 1
fi

# --- Mode selection ---
matched=0

if [[ -n "$regex" ]]; then
  #############################################
  # Regex mode (1 argument)
  #############################################
  while IFS= read -r -d '' file; do
    if [[ "$file" =~ $regex ]]; then
      if [[ "$DEBUG" == "1" ]]; then
        echo "DEBUG: [[ \"$file\" =~ $regex ]] => MATCH (exit=0), match='${BASH_REMATCH[0]}'"
      fi
      matched=$(( matched + 1 ))     # safe increment under set -e
      echo "→ Processing: $file"
      "$python_bin" "$netexport_py" "$file"
    else
      if [[ "$DEBUG" == "1" ]]; then
        echo "DEBUG: [[ \"$file\" =~ $regex ]] => NO MATCH (exit=1)"
      fi
    fi
  done < <(find . -type f -print0)

else
  #############################################
  # List mode (0 arguments) — uses DEFAULT_XML_LIST defined above
  #############################################
  if [[ ${#DEFAULT_XML_LIST[@]} -eq 0 ]]; then
    echo "Error: DEFAULT_XML_LIST is empty. Edit the script to add paths." >&2
    exit 1
  fi

  for file in "${DEFAULT_XML_LIST[@]}"; do
    if [[ ! -e "$file" ]]; then
      echo "Warning: listed path does not exist: $file" >&2
      continue
    fi
    matched=$(( matched + 1 ))
    echo "→ Processing (list): $file"
    "$python_bin" "$netexport_py" "$file"
  done
fi

# --- Summary ---
if [[ $matched -eq 0 ]]; then
  if [[ -n "$regex" ]]; then
    echo "No files matched regex: $regex"
  else
    echo "No files processed from DEFAULT_XML_LIST."
  fi
fi
