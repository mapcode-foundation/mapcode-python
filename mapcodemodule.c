/*
 * Copyright (C) 2015 Stichting Mapcode Foundation (http://www.mapcode.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Python.h"
#include "mapcoder.c"


static char version_doc[] = 
 "version()\n\
\n\
Returns the version of the Mapcode C library used by this Python module.\n";

static PyObject *version(PyObject *self, PyObject *args)
{
    return Py_BuildValue("s", mapcode_cversion);
}


static char isvalid_doc[] = 
 "isvalid()\n\
\n\
Verify if the provided mapcode has the right syntax.\n";

static PyObject *isvalid(PyObject *self, PyObject *args)
{
    char *mapcode;
    int includes_territory = 0;

    if (!PyArg_ParseTuple(args, "s|i", &mapcode, &includes_territory))
       return NULL;

    if (compareWithMapcodeFormat(mapcode, includes_territory ? 1 : 0) == 0) {
        return Py_True;
    } else {
        return Py_False;
    }
}


static char decode_doc[] = 
 "decode(mapcode, (territoryname))\n\
\n\
Decodes the provided string to latitude and longitude. Optionally\n\
a territoryname can be provided to disambiguate the mapcode.\n";

static PyObject *decode(PyObject *self, PyObject *args)
{
    char *mapcode, *territoryname = NULL;
    double latitude, longitude;
    int territorycode;

   if (!PyArg_ParseTuple(args, "s|s", &mapcode, &territoryname))
       return NULL;

    if (territoryname) {
        territorycode = convertTerritoryIsoNameToCode(territoryname, 0);
        if (territorycode < 0) {
            return Py_False;
        }
    } else
        territorycode = 0;

    if (decodeMapcodeToLatLon(&latitude, &longitude, mapcode, territorycode)) {
        /* return 999/999 as error values */
        latitude = 999;
        longitude = 999;
    }
    return Py_BuildValue("ff", latitude, longitude);
}


static char encode_doc[] = 
 "encode(latitude, longitude, (territoryname, extra_digits))\n\
\n\
Encodes the given latitude, longitude to one or more mapcodes.\n\
Returns a list with one or more pairs of mapcode and territoryname.\n\
\n\
Optionally a territoryname can be provided to generate a mapcode in partiucular territory.\n";

static PyObject *encode(PyObject *self, PyObject *args)
{
    double latitude, longitude;
    char *territoryname = NULL;
    int extra_digits = 0, territorycode = 0;
    PyObject *result;

    if (!PyArg_ParseTuple(args, "dd|si", &latitude, &longitude, &territoryname, &extra_digits))
       return NULL;

    // printf("encode: args: %f, %f, %x, %i\n", latitude, longitude, territoryname, extra_digits);

    if (territoryname) {
        territorycode = convertTerritoryIsoNameToCode(territoryname, 0);
        if (territorycode < 0) {
            return PyList_New(0);
        }
    }

    // printf("encode: territorystring: %s, code: %d\n", territoryname, territorycode);

    char *mapcode_results[MAX_NR_OF_MAPCODE_RESULTS];
    int n = encodeLatLonToMapcodes(mapcode_results, latitude, longitude, territorycode, extra_digits);
    if (n > 0) {
        result = PyList_New(n);
        while (n--) {
            // printf("debug: %d: %s - %s\n", n, mapcode_results[n * 2], mapcode_results[(n * 2) + 1]);
            PyList_SetItem(result, n, Py_BuildValue("(ss)", (mapcode_results[n * 2]), mapcode_results[(n * 2) + 1]));            
        }
        return result;
    }
    return PyList_New(0);
}


static char encode_single_doc[] = 
 "encode_single(latitude, longitude, (territoryname, extra_digits))\n\
\n\
Encodes the given latitude, longitude to a mapcode. Optionally a territoryname\n\
can be provided to generate a mapcode in partiucular territory.\n";

static PyObject *encode_single(PyObject *self, PyObject *args)
{
    double latitude, longitude;
    char *territoryname = NULL;
    int extra_digits = 0, territorycode = 0;
    PyObject *result;

    if (!PyArg_ParseTuple(args, "dd|si", &latitude, &longitude, &territoryname, &extra_digits))
       return NULL;

    // printf("encode_single: args: %f, %f, %x, %i\n", latitude, longitude, territoryname, extra_digits);

    if (territoryname) {
        territorycode = convertTerritoryIsoNameToCode(territoryname, 0);
        if (territorycode < 0)
            return Py_BuildValue("s", NULL);
    }

    // printf("debug: encode_single: territorystring: %s, code: %d\n", territoryname, territorycode);

    char encode_single_result[MAX_NR_OF_MAPCODE_RESULTS];
    if (encodeLatLonToSingleMapcode(encode_single_result, latitude, longitude, territorycode, extra_digits) > 0) {
        result = Py_BuildValue("s", encode_single_result);
        return result;
    } else
        return Py_BuildValue("s", NULL);
}


/* The methods we expose in Python. */
static PyMethodDef mapcode_methods[] = {
    { "version", version, METH_VARARGS, version_doc },
    { "isvalid", isvalid, METH_VARARGS, isvalid_doc },
    { "decode", decode, METH_VARARGS, decode_doc },
    { "encode", encode, METH_VARARGS, encode_doc },
    { "encode_single", encode_single, METH_VARARGS, encode_single_doc },
    { NULL, NULL, 0, NULL }
};


/* Initialisation that gets called when module is imported. */
PyMODINIT_FUNC initmapcode(void)
{
    Py_InitModule("mapcode", mapcode_methods);
}
