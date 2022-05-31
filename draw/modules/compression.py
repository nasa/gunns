#!/usr/bin/python
# @copyright Copyright 2022 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, February 2019, --, Initial implementation.}
# @revs_end
#
import base64, zlib

# Python 2.7 vs 3 defnition of our compress and decompress functions, by feature detection.
#
# Python 3.  It has strict distinction between string and bytes types.
try:
    import urllib.parse as URLLIB

    # Decompresses the given string, a, containing compressed data from draw.io
    # XML files (either the diagram data or shape data in a custom shape library).
    # This is the python equivalent of the decode function in their:
    #   https://jgraph.github.io/drawio-tools/tools/convert.html
    # Returns a string containing XML which should be parseable by etree.
    def decompress(a):
        # a = the compressed goop string
        b = base64.decodebytes(bytes(a, 'utf-8'))  # equiv. to JS atob function.
        c = str(zlib.decompress(b, -15), 'utf-8')  # equiv. to JS pako.inflateRaw
        d = URLLIB.unquote(c)                      # equiv. to JS decodeURIComponents
        return d

    # Compresses the given string, d, into the format used by draw.io.
    # This is the opposite process of decompress(a).
    def compress(d):
        c = bytes(URLLIB.quote(d), 'utf-8')
        co = zlib.compressobj(zlib.Z_DEFAULT_COMPRESSION, zlib.DEFLATED, -15)
        b = co.compress(c)
        b += co.flush()
        a = str(base64.encodebytes(b), 'utf-8')
        return a

# Python 2.7
except ImportError:
    import urllib as URLLIB

    # Decompresses the given string, a, containing compressed data from draw.io
    # XML files (either the diagram data or shape data in a custom shape library).
    # This is the python equivalent of the decode function in their:
    #   https://jgraph.github.io/drawio-tools/tools/convert.html
    # Returns a string containing XML which should be parseable by etree.
    def decompress(a):
        # a = the compressed goop string
        b = base64.decodestring(a)  # equiv. to JS atob function.
        c = zlib.decompress(b, -15) # equiv. to JS pako.inflateRaw
        d = URLLIB.unquote(c)       # equiv. to JS decodeURIComponents
        return d

    # Compresses the given string, d, into the format used by draw.io.
    # This is the opposite process of decompress(a).
    def compress(d):
        c = URLLIB.quote(d)
        co = zlib.compressobj(zlib.Z_DEFAULT_COMPRESSION, zlib.DEFLATED, -15)
        b = co.compress(c)
        b += co.flush()
        a = base64.encodestring(b)
        return a

# Test function.
def test():
    expected = '<tag1><tag2 foo="Foo" bar="Bar" />thingy</tag2>'
    compressed = compress(expected)
    print(compressed)
    result = decompress(compressed)
    print(result)
    return (result == expected)
