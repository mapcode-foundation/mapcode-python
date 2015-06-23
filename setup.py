#!/usr/bin/env python


from distutils.core import setup, Extension


setup(
    name='mapcode',
    ext_modules=[Extension('mapcode',
                           sources=['mapcodemodule.c', 'mapcodelib/mapcoder.c'],
                           include_dirs=['mapcodelib']
                           )],
    version='0.3',
    description='A Python module to do mapcode encoding and decoding.  See http://www.mapcode.com for more information.',
    author='Erik Bos',
    author_email='erik@xs4all.nl',
    url='https://github.com/mapcode-foundation/mapcode-python',
    download_url='https://github.com/mapcode-foundation/mapcode-python/tarball/v0.3',
    license='Apache License 2.0',
    classifiers=[
       'Development Status :: 4 - Beta',
       'License :: OSI Approved :: Apache Software License'
       ],
)
