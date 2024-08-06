#!/usr/bin/python
# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, March 2019, --, Initial implementation.}
# @revs_end
#
import xml.etree.ElementTree as ET
import re

# Indents and newlines the given XML elements and subelements for reading and merging.
def formatXml(elem, level=0):
    i = "\n" + level*"  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            formatXml(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i

# This parses the given xml file with all namespaces stripped out.
#
# This aims to reproduce the ns_clean option of XMLParser in the lxml 3rd party library.
# We don't use lxml because we want to minimize the install burden on users.
#
# Returns the root element.
def parseClean(file):
    root_tag = ''
    namespaces = []
    with open(file, 'r') as fin:
        # Find the root element tag and the namespaces
        for line in fin:
            if not line.startswith('<?xml'):
                fields = line.split(' ')
                root_tag = fields[0][1:]
                for field in fields[1:]:
                    if 'xmlns' in field:
                        attribs = field.split('=')[0].split(':')
                        if len(attribs) == 2:
                            namespaces.append(attribs[1])
                break
    with open(file, 'r') as fin:
        fin_str = fin.read()

    # Replace the root element with just the tag.
    search = re.search(r'<' + root_tag + '.+?>', fin_str)
    if (search):
        sub_out = search.group(0)
        if sub_out.endswith('/>'):
            sub_in = '<' + root_tag + '/>'
        else:
            sub_in = '<' + root_tag + '>'
        fin_str = re.sub(sub_out, sub_in, fin_str)

    for namespace in namespaces:
        # Find all occurrences of the namespace, ns, in <*ns:> without a quote between
        # < and ns, and remove duplicates.
        occurrences = list(set(re.findall(r'<[^"\'>]*' + namespace + ':', fin_str)))
        for occurrence in occurrences:
            # Clean all occurrences of the namespace
            sub_in = occurrence.replace(namespace + ':', '')
            fin_str = re.sub(occurrence, sub_in, fin_str)
    return ET.fromstring(fin_str)
