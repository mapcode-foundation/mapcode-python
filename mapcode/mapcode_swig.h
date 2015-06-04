char *version(void);

int isvalid(char *mapcode, int includesTerritory);
char **encode(double latitude, double longitude, char *territory, int extra_digits);
char *encode_single(double latitude, double longitude, char *territory, int extra_digits);
int decode(double *lat, double *lon, const char *mapcode, char *territory);
