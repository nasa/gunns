#!/usr/bin/env python3
# Copyright 2026 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
import sys

def main():
    if 'FAILED' in open('testNetExport.out').read():
        print('\n***TEST FAILURE*** Found a "FAILED" in testNetExport.out\n')
        sys.exit(1)

    if 'FAILED' in open('testNetExport.out').read():
        print('\n***TEST FAILURE*** Found a "FAILED" in testNetExport.out\n')
        sys.exit(1)

if __name__ == "__main__":
    main()
