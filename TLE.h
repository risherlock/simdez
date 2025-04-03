#ifndef __sgp4tleheader__
#define __sgp4tleheader__  

#include "SGP4.h"
#include "time.h"

class TLE
{
  public:
    ElsetRec rec;
    char line1[70];
    char line2[70];
    char intlid[12];
    char objectID[6];
    long epoch;
    double ndot;
    double nddot;
    double bstar;
    int elnum;
    double incDeg;
    double raanDeg;
    double ecc;
    double argpDeg;
    double maDeg;
    double n;
    int revnum;
    int sgp4Error;
    utc_t dt;

  public:

    TLE();

    TLE(char *line1, char *line2);

    void parseLines(char *line1, char *line2);

    void getRVForDate(long millisSince1970, double r[3], double v[3]);

    void getRV(double minutesAfterEpoch, double r[3], double v[3]);
};
#endif
