#!/usr/bin/python
# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, February 2019, --, Initial implementation.}
# @revs_end
#
# This script can rotate, scale, shift the x&y coordinates of paths
# from the draw.io shape edit schema.  Copy your shape's path from
# draw.io into the input_path.txt file, then run this script.
# The script outputs the transformed path to the console, which you
# can copy back into draw.io.
#
# Order of operations is rotate, then scale, then shift.
# Enter your transform amounts below.
#
# This assumes x & y fields are always enclosed in double-quote, i.e.
#   x="2.3", not x='2.3'
# This assumes there is always a y field immediately following an x field.
# TODO doesn't work with <arc> fields.

import os
import sys
import math

# Enter the transform amounts here:
rot   = 0.0   # angle of rotation, deg
rotx  = 0.0   # x-center of rotation
roty  = 0.0   # y-center of rotation
scale = 36.0 / 90.0
biasx = 90.0 - 45*scale
biasy = 20.0 - 45*scale

def transform_xy(x0, y0):
    # find initial vector to x from center of rotation
    x1  = x0 - rotx
    y1  = y0 - roty
    len = math.sqrt(x1 * x1 + y1 * y1)
    t1  = math.atan2(y1, x1)

    # rotate vector
    t2  = t1 + math.radians(rot)
    x2  = len * math.cos(t2)
    y2  = len * math.sin(t2)

    # new x is center plus new vector
    x3  = rotx + x2
    y3  = roty + y2

    # scale, then bias
    return ((x3 * scale + biasx), (y3 * scale + biasy))

def transform_field(fx, fy):
    # extract the real value from between the quotes to get (x, y)
    xsplit = fx.split('"')
    ysplit = fy.split('"')
    x = float( xsplit[1] )
    y = float( ysplit[1] )

    # call transform_xy(x, y) to get new (x, y)
    new_xy = transform_xy(x, y)

    # Format returned floating points to 0.00
    x_str = '%.2f' % new_xy[0]
    y_str = '%.2f' % new_xy[1]

    # replace value between quotes with new value
    x_out = xsplit[0] + '"' + x_str + '"' + xsplit[2]
    y_out = ysplit[0] + '"' + y_str + '"' + ysplit[2]
    return (x_out, y_out)

with open("input_path.txt", "r") as fin:
    for line in fin:
        line = line.rstrip()

        # Store the leading spaces
        line_out = ' ' * (len(line) - len(line.lstrip()))

        # Break the remaining line into a list of space-separated segments
        fields = line.lstrip().split(' ')

        # For each segment, if it starts with x and has = in it,
        #  then transform_fields(this segment, next_segment)
        for i in range(len(fields)):
            if 'x' in fields[i] and '=' in fields[i]:
                new_fields = transform_field(fields[i], fields[i+1])
                fields[i]   = new_fields[0]
                fields[i+1] = new_fields[1]

        # Buuild new line string as the initial spaces plus the list
        for field in fields:
            line_out = line_out + field + ' '

        line_out = line_out.rstrip()
        print(line_out)
