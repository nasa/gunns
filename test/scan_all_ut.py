#!/usr/bin/env python3
# Copyright 2023 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
import sys

def main():
    if 'FAILURES' in open('make_all_ut.out').read():
        print('\n***TEST FAILURE*** Found a "FAILURES" in make_all_ut.out\n')
        sys.exit(1)

    if 'Failures' in open('make_all_ut.out').read():
        print('\n***TEST FAILURE*** Found a "Failures" in make_all_ut.out\n')
        sys.exit(1)

    if 'NO TEST OUTPUT' in open('make_all_ut.out').read():
        print('\n***TEST FAILURE*** Found a "NO TEST OUTPUT" in make_all_ut.out\n')
        sys.exit(1)

if __name__ == "__main__":
    main()
