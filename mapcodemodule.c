#include "Python.h"
#include <wchar.h>
#include "mapcoder.c"

PyObject *mapcode_module;

/*
    mapcode.version method.

    Returns the version of the C library used.
*/

static PyObject *version(PyObject *self, PyObject *args)
{
    return Py_BuildValue("s", mapcode_cversion);
}


int isvalid(char *mapcode, int includes_territory)
{
    if (compareWithMapcodeFormat(mapcode, includes_territory ? 1 : 0) == 0) {
        return 1;
    } else {
        return 0;
    }
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

static char encode_single_result[MAX_NR_OF_MAPCODE_RESULTS];

static PyObject *encode_single(PyObject *self, PyObject *args)
{
    double latitude, longitude;
    char *territory;
    int extra_digits, territorycode = 0;

    if (territory) {
        territorycode = convertTerritoryIsoNameToCode(territory, 0);
        if (territorycode < 0)
            return NULL;
    }

    printf("debug: encode_single: territorystring: %s, code: %d\n", territory, territorycode);
    if (encodeLatLonToSingleMapcode(encode_single_result, latitude, longitude, territorycode, extra_digits) > 0) {
        return Py_BuildValue("s", encode_single_result);
    } else
        return NULL;
}


static int decode(double *latitude, double *longitude, const char *mapcode, char *territory)
{
    int territorycode = 0;

    if (territory) {
        territorycode = convertTerritoryIsoNameToCode(territory, 0);
        if (territorycode < 0) {
            *latitude = 0;
            *longitude = 0;
            return 1;
        }
    }
    return decodeMapcodeToLatLon(latitude, longitude, mapcode, territorycode);
}


/* 

The methods we have

*/

static PyMethodDef mapcode_methods[] = {
    { "version", version, METH_VARARGS, "Returns the version of the Mapcode C library"},
    // { "encode", encode, METH_VARARGS, NULL},
    { (char *)"encode_single", encode_single, METH_VARARGS, NULL},
    // { (char *)"decode", _wrap_decode, METH_VARARGS, NULL},
    // { (char *)"isvalid", _wrap_isvalid, METH_VARARGS, NULL},
    // { (char *)"alphabet", _wrap_alphabet, METH_VARARGS, NULL},
    { NULL, NULL, 0, NULL }
};


/* 

Initialisation that gets called when module is imported.

*/

PyMODINIT_FUNC initmapcode(void)
{
    mapcode_module = Py_InitModule("mapcode", mapcode_methods);
}