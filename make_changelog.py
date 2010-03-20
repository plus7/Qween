#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from xml.dom.minidom import parse

def getText(nodelist):
    rc = u""
    for node in nodelist:
        if node.nodeType == node.TEXT_NODE:
            rc = rc + node.data
    return rc

def print_li(li, level):
    indent = u"\t" * (level+1)
    print (indent + u"* " + getText(li.childNodes).replace(u"\n",u"")).encode("utf_8")
    for child in li.childNodes:
        if child.nodeType == child.ELEMENT_NODE and child.tagName == "ul":
            print_ul(child, level+1)

def print_ul(ul, level):
    for child in ul.childNodes:
        if child.nodeType == child.ELEMENT_NODE and child.tagName == "li":
            print_li(child, level)

def print_content(content):
    ul = content.getElementsByTagName("ul")[0]
    print_ul(ul,0)

def print_entry(entry):
    date = getText(entry.getElementsByTagName("date")[0].childNodes)
    ver = getText(entry.getElementsByTagName("version")[0].childNodes)
    content = entry.getElementsByTagName("content")[0]
    print date + u"  " + ver
    print 
    print_content(content)
    print

obj = parse("ChangeLog.xml")
entries = obj.getElementsByTagName('entry')
for entry in entries:
    print_entry(entry)