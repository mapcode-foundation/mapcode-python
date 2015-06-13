#!/usr/bin/env python


from distutils.core import setup, Extension


setup(
    name='mapcode',
    ext_modules=[Extension('mapcode',
                           sources=['mapcodemodule.c'],
                           include_dirs=['../mapcode-cpp/mapcodelib']
                           )],
    version='0.2',
    description='A Python module to do mapcode encoding and decoding.  See http://www.mapcode.com for more information.',
    author='Erik Bos',
    author_email='erik@xs4all.nl',
    url='https://github.com/mapcode-foundation/mapcode-python',
    license='Apache License 2.0',
    classifiers=[
       'Development Status :: 3 - Alpha',
       'License :: OSI Approved :: Apache Software License'
       ],
)
