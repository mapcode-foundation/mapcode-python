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
#include "basics.h"
#include "mapcoder.h"
#include <math.h>

static char version_doc[] =
"version() -> string\n\
\n\
Returns the version of the Mapcode C library used by this module.\n";

static PyObject *version(PyObject *self, PyObject *args)
{
    return Py_BuildValue("s", mapcode_cversion);
}


static char isvalid_doc[] =
"isvalid(mapcode) -> bool\n\
\n\
Verify if the provided mapcode has the correct syntax.\n";

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
"decode(mapcode, (territoryname)) -> (float, float)\n\
\n\
Decodes the provided string to latitude and longitude. Optionally\n\
a territorycontext can be provided to disambiguate the mapcode.\n\
\n\
Returns (nan,nan) when decoding failed.\n";

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
            latitude = NAN;
            longitude = NAN;
            return Py_BuildValue("ff", latitude, longitude);
        }
    } else
        territorycode = 0;

    if (decodeMapcodeToLatLon(&latitude, &longitude, mapcode, territorycode)) {
        /* return nan,nan as error values */
        latitude = NAN;
        longitude = NAN;
    }
    return Py_BuildValue("ff", latitude, longitude);
}


static char encode_doc[] =
 "encode(latitude, longitude, (territoryname, (extra_digits))) -> [(string, string)] \n\
\n\
Encodes the given latitude, longitude to one or more mapcodes.\n\
Returns a list of tuples that contain mapcode and territory context.\n\
\n\
Optionally a territory context can be provided to generate a mapcode\n\
in particular territory context.\n";

static PyObject *encode(PyObject *self, PyObject *args)
{
    double latitude, longitude;
    char *territoryname = NULL;
    int extra_digits = 0, territorycode = 0;
    PyObject *result;

    if (!PyArg_ParseTuple(args, "dd|zi", &latitude, &longitude, &territoryname, &extra_digits))
       return NULL;

    // printf("encode: args: %f, %f, %x, %i\n", latitude, longitude, territoryname, extra_digits);

    if (territoryname) {
        territorycode = convertTerritoryIsoNameToCode(territoryname, 0);
        if (territorycode < 0) {
            return PyList_New(0);
        }
    }

    // printf("encode: territorystring: %s, code: %d\n", territoryname, territorycode);

    char *mapcode_results[2 * MAX_NR_OF_MAPCODE_RESULTS];
    int n = encodeLatLonToMapcodes_Deprecated(mapcode_results, latitude, longitude, territorycode, extra_digits);
    if (n > 0) {
        result = PyList_New(n);
        while (n--) {
            // printf("debug: %d: %s - %s\n", n, mapcode_results[n * 2], mapcode_results[(n * 2) + 1]);
            PyList_SetItem(result, n, Py_BuildValue("(ss)",
                (mapcode_results[n * 2]), mapcode_results[(n * 2) + 1]));
        }
        return result;
    }
    return PyList_New(0);
}


static char mapcode_doc[] =
"Support for mapcodes. (See http://www.mapcode.org/).\n\
\n\
This module exports the following functions:\n\
    version        Returns the version of the mapcode C-library used.\n\
    isvalid        Verifies if the provided mapcode has the correct syntax.\n\
    decode         Decodes a mapcode to latitude and longitude.\n\
    encode         Encodes latitude and longitude to one or more mapcodes.\n";

/* The methods we expose in Python. */
static PyMethodDef mapcode_methods[] = {
    { "version", version, METH_VARARGS, version_doc },
    { "isvalid", isvalid, METH_VARARGS, isvalid_doc },
    { "decode", decode, METH_VARARGS, decode_doc },
    { "encode", encode, METH_VARARGS, encode_doc },
    { NULL, NULL, 0, NULL }
};

/* Initialisation that gets called when module is imported. */
#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_mapcode(void)
{
    static struct PyModuleDef mapcode_module = {
        PyModuleDef_HEAD_INIT,
        "mapcode",           /* m_name */
        mapcode_doc,         /* m_doc */
        -1,                  /* m_size */
        mapcode_methods,     /* m_methods */
        NULL,                /* m_reload */
        NULL,                /* m_traverse */
        NULL,                /* m_clear */
        NULL,                /* m_free */
    };

    return(PyModule_Create(&mapcode_module));
}
#else
PyMODINIT_FUNC initmapcode(void)
{
    Py_InitModule3("mapcode", mapcode_methods, mapcode_doc);
}
#endif