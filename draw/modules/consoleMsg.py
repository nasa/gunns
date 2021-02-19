#!/usr/bin/python
# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, February 2019, --, Initial implementation.}
# @revs_end
#
# Reference for the ANSI escape codes for color:
#   https://www.geeksforgeeks.org/print-colors-python-terminal/

# Prints a final completion message.
def success(dt):
    print('...\033[32mcompleted\033[0m in ' + str(dt) + ' seconds!')

# Returns the given message with a grey 'Note: ' added to the front.
def note(msg):
    return '\033[36mNote:\033[0m ' + msg

# Returns the given message with a yellow 'Warning: ' added to the front.
def warn(msg):
    return '\033[33mWarning:\033[0m ' + msg

# Returns the given message with a red 'Aborted: ' added to the front.
def abort(msg):
    return '\033[31mAborted:\033[0m ' + msg

