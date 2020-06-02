#!/usr/bin/perl

# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
use warnings;
use strict;

# Note that these arguments are absolute paths and expect the
# compiled object library to be structured like so:
# <object library>/<directory>/*.o
#
# For example:
# <directory> = /users/you/repos/gunns/core
# <object library> = /users/you/repos/gunns/lib/test/build
#
# Then the objects should be in:
# /users/you/repos/gunns/lib/test/build/users/you/repos/gunns/core/
#
if (@ARGV < 3) {
    die "usage: lcov-wrapper <directory> <object library> <output file>\n";
}

my $system_dir = $ARGV[0];
my $object_lib = $ARGV[1];
my $output_file = $ARGV[2];

# Remove the temporary output file.
sub clean() {
    unlink "$output_file.tmp";
}

# Clean up before dying.
sub clean_fail() {
    clean();
    exit 1;
}

# Copy gcov coverage files from the compiled object library to our local build folder.
system("cp $object_lib/$system_dir/*.gc* build");

# Capture lcov output.
system("lcov -c -q -b $system_dir/test -d $system_dir/test/build -o $output_file.tmp");
if ($? >> 8) {clean_fail;}

# Strip out the system includes and test classes.
system("lcov -q -r $output_file.tmp \"/usr/include/**\" \"/usr/local/include/**\" \"*/test/*\" -o $output_file");

# Nominal cleanup.
clean();
