/* File: mapcode.i */

%define DOCSTRING
"Routines to encode latitude/longitudes to mapcodes and back.

See mapcode.org for more information on Mapcode."
%enddef

%module(docstring=DOCSTRING) mapcode

%include "typemaps.i"

%{
#define SWIG_FILE_WITH_INIT
#include "mapcode_swig.h"
%}

/*
%typemap(python, out) int decode {
	printf("result: %d\n", result);
	if (result) {
	  $result = PyList_New(0);
	}
}
*/

/* this converts the char ** returned by encode() into a Python list */
%typemap(python, out) char **encode {
  int len = 0;
  while ($1[len])
     len++;

  $result = PyList_New(len);
  int i = 0;
  while ($1[i]) {
    PyList_SetItem($result, i, PyString_FromString($1[i++]));
  }
}

char *version(void);
char **encode(double latitude, double longitude, char *territory=NULL, int extra_digits=0);
char *encode_single(double latitude, double longitude, char *territory=NULL, int extra_digits=0);
int decode(double *OUTPUT, double *OUTPUT, const char *mapcode, char *territory = NULL);
int isvalid(char *mapcode, int includesTerritory);
wchar_t *alphabet(const char *mapcode, int alphabet);
