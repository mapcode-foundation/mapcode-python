# Mapcode module for Python

Copyright (C) 2014-2015 Stichting Mapcode Foundation (http://www.mapcode.com)

----

This Python project contains a module to encode latitude/longitude pairs to mapcodes
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

If you encounter any problems with this module, don't hesitate to use the `Issues` session to file your issues.
Normally, one of our developers should be able to comment on them and fix.

# Installation

Get the mapcode Python repository using:

```
git clone https://github.com/mapcode-foundation/mapcode-python
cd mapcode-python
```

Compile the package in current directory: `python setup.py build_ext --inplace`

Install in your Python environment using: `python setup.py install`

# Mapcode C library

This Python module includes a copy of the Mapcode C library in the
directory mapcodelib. The latest version can be found on
https://github.com/mapcode-foundation/mapcode-cpp

Updating it by replacing with the latest mapcode library should not
be an issue. Only the defined external functions are used.

# Python methods

The module exposes a number of methods:

```python
import mapcode
print mapcode.__doc__
Support for mapcodes. (See http://www.mapcode.org/).

This module exports the following functions:
    version        Returns the version of the mapcode C-library used.
    isvalid        Verifies if the provided mapcode has the correct syntax.
    decode         Decodes a mapcode to latitude and longitude.
    encode         Encodes latitude and longitude to one or more mapcodes.
    encode_single  Encodes latitude and longitude to shortest mapcode possible.
```

## Mapcode version

Use version() to get the version of the mapcode C-library.

```python
print mapcode.version()
1.50.1
```

## Mapcode syntax validation

To validate the syntax of a mapcode string use the isvalid() method.

```python
print mapcode.isvalid('VHXG9.FQ9Z')
True
print mapcode.isvalid('Amsterdam')
False
```

As optional parameter you can pass 1 or 0: if you pass 1, full mapcodes
(including optional territory context) will be recognized. If you pass 0,
only “proper” mapcodes will be recognized.

```python
print mapcode.isvalid('NLD 49.4V', 1)
True
print mapcode.isvalid('VHXG9.FQ9Z',0)
True
print mapcode.isvalid('NLD 49.4V', 0)
False

(This one fails as it is not a full mapcode)
```


## Encoding

Convert latitude/longitude to one or more mapcodes. The response always
contains the 'international' mapcode and only contains a 'local' mapcode
if there are any non-international mapcode AND they are all of the same
territory.

Use the encode() method to convert latitude/longitude to all possible
mapcodes.

```python
print mapcode.encode(52.376514, 4.908542)
[('49.4V', 'NLD'), ('G9.VWG', 'NLD'), ('DL6.H9L', 'NLD'), ('P25Z.N3Z', 'NLD'), ('VHXGB.1J9J', 'AAA')]
```

Optionally a territory context can be provide to encode for that context.

```python
print mapcode.encode(52.376514, 4.908542, 'NLD')
[('49.4V', 'NLD'), ('G9.VWG', 'NLD'), ('DL6.H9L', 'NLD'), ('P25Z.N3Z', 'NLD')]
print mapcode.encode(39.609999,45.949999, 'AZE')
[('XLT.HWB', 'AZE'), ('2Z.05XL', 'AZE'), ('6N49.HHV', 'AZE')]
```

Use the encode_single() method to find one possible mapcode.
Optionally a territory context can be provide to encode for that context.

```python
print mapcode.encode_single(52.376514, 4.908542)
NLD 49.4V

For this coordinate there are multiple mapcodes possible, hence providing
a territory code can give a different result.

print mapcode.encode_single(41.851944, 12.433114)
ITA 0Z.0Z
print mapcode.encode_single(41.851944, 12.433114, 'AAA')
TJKM1.D2Z6
```

## Decoding

Use the decode() method to convert a mapcode to latitude and longitude.

```python
print mapcode.decode('NLD 49.4V')
(52.376514, 4.908542)
print mapcode.decode('IN VY.HV')
(43.193485, 44.826592)
print mapcode.decode('VHXG9.DNRF')
(52.371422, 4.872497)
```

An optional territory context can be provide to disambiguate the mapcode.

A territory is require to decode this mapcode:

```python
print mapcode.decode('D6.58')
False
print mapcode.decode('D6.58','RU-IN DK.CN0')
(43.259275, 44.77198)
```

A territory context is require to give context to decoding:

```python
print mapcode.decode('IN VY.HV','USA')
(39.72795, -86.118444)
print mapcode.decode('IN VY.HV','RUS')
(43.193485, 44.826592)
```
