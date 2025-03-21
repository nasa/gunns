#!/usr/bin/env python

"""
Purpose: Gets fluid property tables that can be loaded into FluidProperties.cpp
Author: Sam Evani - Axiom Space - 2025
Copyright 2025 United States Government as represented by the Administrator of the National Aeronautics and Space Administration.  All Rights Reserved.
"""

# Standard imports
#-

# Additional imports
import CoolProp.CoolProp as CP
import numpy as np

# Local imports
#-


import subprocess
import re
import argparse
from pathlib import Path

# Match "Copyright 20XX" anywhere in the line
COPYRIGHT_PATTERN = re.compile(r"(Copyright\s+)(20\d{2})")
UPSTREAM_REMOTE = "upstream"
UPSTREAM_BRANCH = "master"

def run_cmd(cmd):
    return subprocess.run(cmd, shell=True, check=True, text=True, stdout=subprocess.PIPE).stdout.strip()

def ensure_upstream_remote():
    remotes = run_cmd("git remote -v")
    if UPSTREAM_REMOTE not in remotes:
        print(f"[ERROR] Remote '{UPSTREAM_REMOTE}' not found.")
        print(f"        Please add it using:")
        print(f"        git remote add {UPSTREAM_REMOTE} https://github.com/nasa/gunns.git")
        exit(1)

def fetch_upstream():
    print("[INFO] Fetching latest from upstream...")
    run_cmd(f"git fetch {UPSTREAM_REMOTE}")

def get_modified_files_in_branch():
    cmd = f"git diff --name-only {UPSTREAM_REMOTE}/{UPSTREAM_BRANCH}"
    output = run_cmd(cmd)
    return [line for line in output.split("\n") if line.strip()]

def get_all_tracked_files():
    cmd = "git ls-files"
    output = run_cmd(cmd)
    return [line for line in output.split("\n") if line.strip()]

def get_latest_modification_year(file_path):
    cmd = f"git log -1 --format=%ad --date=format:%Y -- {file_path}"
    output = run_cmd(cmd)
    return int(output) if output else None

def update_file_if_needed(file_path, mod_year):
    path = Path(file_path)
    if not path.is_file():
        return

    content = path.read_text()

    def replacer(match):
        current_year = int(match.group(2))
        if current_year != mod_year:
            return f"{match.group(1)}{mod_year}"
        return match.group(0)

    new_content, count = COPYRIGHT_PATTERN.subn(replacer, content)

    if count > 0 and new_content != content:
        path.write_text(new_content)
        print(f"[UPDATED] {file_path}")
    elif count > 0:
        print(f"[OK] Correct year already present: {file_path}")
    else:
        print(f"[SKIPPED] No copyright statement: {file_path}")
        
def main():
    parser = argparse.ArgumentParser(description="Update copyright years.")
    parser.add_argument("--mode", choices=["branch", "all"], default="branch", help="Choose 'branch' or 'all' files.")
    args = parser.parse_args()

    ensure_upstream_remote()
    fetch_upstream()

    if args.mode == "all":
        files = get_all_tracked_files()
        print("[INFO] Checking all tracked files...")
    else:
        files = get_modified_files_in_branch()
        print(f"[INFO] Checking files modified vs {UPSTREAM_REMOTE}/{UPSTREAM_BRANCH}...")

    for file_path in files:
        mod_year = get_latest_modification_year(file_path)
        if mod_year:
            update_file_if_needed(file_path, mod_year)

if __name__ == "__main__":
    main()