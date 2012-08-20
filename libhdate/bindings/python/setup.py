#!/usr/bin/env python
# setup.py
#
# Windows installer:
#   "python setup.py bdist_wininst"
#
# Direct install (all systems):
#   "python setup.py install"
#

from distutils.core import setup, Extension

setup(
    name = "hdate",
    description = "Python Hebrew dates Extension",
    version = '0.1',
    author = "Yaacov Zamir",
    author_email = "kobi.zamir@gmail.com",
    url = "http://libhdate.sourceforge.net/",
    
    py_modules = ['hdate'],
    ext_modules=[Extension('_hdate', ['../../src/deprecated.c','../../src/hdate_julian.c','../../src/hdate_strings.c',
'../../src/hdate_holyday.c','../../src/hdate_parasha.c','../../src/hdate_sun_time.c','hdate_wrap.cxx'])],
    
)
