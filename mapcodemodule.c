#include "Python.h"
#include "mapcoder.c"

PyObject *mapcode_module;




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
Verify if the provided string has the right mapcode syntax.\n";

static PyObject *isvalid(PyObject *self, PyObject *args)
{
    char *mapcode;
    int includes_territory;

    if (!PyArg_ParseTuple(args, "si", &mapcode, &includes_territory))
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
            latitude = 0;
            longitude = 0;
            return Py_False;
        }
    } else
        territorycode = 0;

    decodeMapcodeToLatLon(&latitude, &longitude, mapcode, territorycode);

    return Py_BuildValue("dd", latitude, longitude);
}



char encode_result[MAX_NR_OF_MAPCODE_RESULTS*2];
static char **encode(double latitude, double longitude, char *territory, int extra_digits)
{
    int territorycode = 0;
    char **s = (char **) encode_result;

    if (territory) {
        territorycode = convertTerritoryIsoNameToCode(territory, 0);
/*
        printf("debug1: encode: territorystring: %s, code: %d\n", territory, territorycode);
*/
        if (territorycode < 0) {
            /* terminate array pointer so caller can detect it's empty */
            s[0] = 0;
            return s;
        }
    }

/*    printf("debug2: encode: territorystring: %s, code: %d\n", territory, territorycode);
*/
    int n = encodeLatLonToMapcodes((char **) &encode_result, latitude, longitude, territorycode, extra_digits);
    if (n > 0) {
/*      for (int i = 0; i < n; ++i) {
            printf("debug: %s - %s\n", s[i*2], s[(i*2)+1]);
        }
        printf("debug: count = %d\n", i);
*/
        /* terminate array pointer at the end */
        s[n * 2] = 0;

        return s;
    } else {
        /* terminate array pointer at beginning */
        s[0] = 0;
    }
    return s;
}

/*

static char *encode_single(double latitude, double longitude, char *territory, int extra_digits)
*/

static PyObject *encode_single(PyObject *self, PyObject *args)
{
    double latitude, longitude;
    char *territoryname = NULL, *encode_single_result;
    int extra_digits = 0, territorycode = 0;
    PyObject *result;

    if (!PyArg_ParseTuple(args, "dd|si", &latitude, &longitude, &territoryname, &extra_digits))
       return NULL;

    // printf("encode_single: args: %f, %f, %x, %i\n", latitude, longitude, territoryname, extra_digits);

    if (territoryname) {
        territorycode = convertTerritoryIsoNameToCode(territoryname, 0);
        if (territorycode < 0)
            return Py_False;
    }

    // printf("debug: encode_single: territorystring: %s, code: %d\n", territoryname, territorycode);

    encode_single_result = malloc(MAX_NR_OF_MAPCODE_RESULTS);
    if (encodeLatLonToSingleMapcode(encode_single_result, latitude, longitude, territorycode, extra_digits) > 0) {
        result = Py_BuildValue("s", encode_single_result);
        free(encode_single_result);
        return result;
    } else
        return Py_False;
}


/* The methods we have. */

static PyMethodDef mapcode_methods[] = {
    { "version", version, METH_VARARGS, version_doc },
    { "isvalid", isvalid, METH_VARARGS, isvalid_doc },
    { "decode", decode, METH_VARARGS, decode_doc },
    // { "encode", encode, METH_VARARGS, NULL},
    { "encode_single", encode_single, METH_VARARGS, "Do a mapcode geocode"},
    { NULL, NULL, 0, NULL }
};


/* Initialisation that gets called when module is imported. */

PyMODINIT_FUNC initmapcode(void)
{
    mapcode_module = Py_InitModule("mapcode", mapcode_methods);
}