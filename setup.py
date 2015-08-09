#!/usr/bin/env python

from __future__ import print_function
from distutils.core import setup, Extension
import re


def mapcode_cversion():
    HEADERFILE = 'mapcode-cpp/mapcodelib/mapcoder.h'

    with open(HEADERFILE, 'r') as f:
        for line in f:
            m = re.match('^#define mapcode_cversion.*"(.*)"', line)
            if m:
                return m.group(1)

    raise ValueError('Can not find version number in ' + HEADERFILE)


module_version = mapcode_cversion() + '.1'
print('Module version: ' + module_version)

setup(
    name='mapcode',
    ext_modules=[Extension('mapcode',
                           sources=['mapcodemodule.c', 'mapcode-cpp/mapcodelib/mapcoder.c'],
                           include_dirs=['mapcode-cpp/mapcodelib']
                           )],
    version=module_version,
    description='A Python module to do mapcode encoding and decoding.  See http://www.mapcode.com for more information.',
    author='Erik Bos',
    author_email='erik@xs4all.nl',
    url='https://github.com/mapcode-foundation/mapcode-python',
    download_url='https://github.com/mapcode-foundation/mapcode-python/tarball/v' + module_version,
    license='Apache License 2.0',
    classifiers=[
       'Development Status :: 5 - Production/Stable',
       'Topic :: Scientific/Engineering :: GIS',
       'License :: OSI Approved :: Apache Software License',
       'Programming Language :: Python :: 2.6',
       'Programming Language :: Python :: 2.7',
       'Programming Language :: Python :: 3'
       ],
)
