#!/usr/bin/env python

from distutils.core import setup, Extension


mapcode_module = Extension('mapcode/_mapcode',
                           sources=['mapcode/mapcode_wrap.c', 'mapcode/mapcode_swig.c'],
                           include_dirs=['../mapcode-cpp/mapcodelib']
                           )

setup(
    name='mapcode',
    version='0.1',
    description='A Python module to do mapcode encoding and decoding.  See http://www.mapcode.com for more information.',
    author='Erik Bos',
    author_email='erik@xs4all.nl',
    url='https://github.com/mapcode-foundation/mapcode-python',
    packages=['mapcode'],
    ext_modules=[mapcode_module],
    license='Apache License 2.0',
    classifiers=[
       'Development Status :: 3 - Alpha',
       'License :: OSI Approved :: Apache Software License'
       ],
)
