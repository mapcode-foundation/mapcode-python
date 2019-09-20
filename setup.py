import setuptools

module_version = '2.5.5.0'

setuptools.setup(
    name='mapcode',
    ext_modules=[setuptools.Extension('mapcode',
                           sources=['mapcodemodule.c', 'mapcodelib/mapcoder.c', 'mapcodelib/mapcode_legacy.c'],
                           include_dirs=['mapcodelib']
                           )],
    version=module_version,
    description='A Python module to do mapcode encoding and decoding. See http://www.mapcode.com for more information.',
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
