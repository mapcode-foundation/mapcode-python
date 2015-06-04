# Mapcode Library for Python

Copyright (C) 2014-2015 Stichting Mapcode Foundation (http://www.mapcode.com)

----

This Python project contains a library to encode latitude/longitude pairs to mapcodes
and to decode mapcodes back to latitude/longitude pairs.

If you wish to use mapcodes in your own application landscape, consider using running an instance of the
Mapcode REST API, which can be found on: **https://github.com/mapcode-foundation/mapcode-rest-service**

# License

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Original C library created by Pieter Geelen. Work on Java version
of the mapcode library by Rijn Buve and Matthew Lowden. Python
interface by Erik Bos.

# Status

At the moment this module is still in development!

# Bug Reports and New Feature Requests

If you encounter any problems with this library, don't hesitate to use the `Issues` session to file your issues.
Normally, one of our developers should be able to comment on them and fix. 

# Prequisites

As the Python modules relies upon the Mapcode C library you will 
need to have a compiler installed to be able to build and install
this module.

# Installation

Get both the Python and C repositories using:

```git clone https://github.com/mapcode-foundation/mapcode-python
git clone https://github.com/mapcode-foundation/mapcode-cpp
cd mapcode-python
```

Compile the current directory using:

```python setup.py --inplace
```

Install in your Python environment using:

```cd mapcode-python
python setup.py install
```

# Usage


```import mapcode

dir (mapcode)
print mapcode.__doc__
print mapcode.version()
print mapcode.decode('NLD 49.4V','')
print mapcode.decode('IN VY.HV','USA')
print mapcode.decode('IN VY.HV','RUS')
print mapcode.decode('D6.58','RU-IN DK.CN0')
print mapcode.decode('IN VY.HV','')
print mapcode.decode('RU-IN VY.HV','')
print mapcode.decode('IN VY.HV','RUS')

print mapcode.isvalid('NLD 49.4V', 1)
print mapcode.isvalid('VHXG9.FQ9Z',0)

print mapcode.encode_single(52.376514, 4.908542, 'NLD', 2)
print mapcode.encode_single(41.938434,12.433114, None, 2)

print mapcode.encode(52.376514, 4.908542, 'NLD', 2)
print mapcode.encode(39.609999,45.949999, 'AZE', 0)
```
