#!/usr/bin/python2.6
# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
import sys

def main():
    if 'FAILURES' in open('make_all_ut.out').read():
        sys.exit(1)

    if 'Failures' in open('make_all_ut.out').read():
        sys.exit(1)

    if 'NO TEST OUTPUT' in open('make_all_ut.out').read():
        sys.exit(1)

if __name__ == "__main__":
    main()
