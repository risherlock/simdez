#include "simulation.h"

#include "dcm.h"
#include "SGP4.h"
#include "igrf.h"
#include "time.h"
#include "frame.h"

#include <QDebug>
#include <QObject>
#include <QThread>

QVector<double> t, bx, by, bz;
bool stop_flag = false;

simulation::simulation(QObject *parent)
    : QObject{parent}
{
    // Simulation params
    stepmin = 1;
    startmin = 0;
    stopmin = 600;
    steps = (stopmin - startmin) / stepmin;

    // Parse TLE
    char line1[70];
    char line2[70];
    strncpy(line1, "1 25544U 98067A   25264.51782528 -.00002182  00000-0 -11606-4 0  2927", 69); line1[69] = '\0';
    strncpy(line2, "2 25544  51.6416 247.4627 0006703 130.5360 325.0288 15.72125391563537", 69); line2[69] = '\0';
    tle.parseLines(line1, line2);
}

void simulation::run(void)
{
    while(!stop_flag)
    {
        double dcm[3][3], r_ecef[3], r[3], v[3], lla[3], b[3];
        double q[4] = {1, 0, 0, 0};
        double time = startmin + i * stepmin;

        // Orbit propagation and IGRF
        tle.getRV(time, r, v);
        frame_eci_to_ecef_dcm(tle.dt, dcm);
        dcm_rotate(dcm, r, r_ecef);
        frame_ecef_to_lla(r_ecef, lla);
        igrf(tle.dt, lla, IGRF_GEODETIC, b);

        // Simulation output
        t.append(time);
        bx.append(b[0]);
        by.append(b[1]);
        bz.append(b[2]);

        qDebug() << b[0];

        i++;
        emit data_generated(time, lla, b, q);
        QThread::msleep(20);

        if(i >= steps)
        {
            stop_flag = true;
        }
    }
}
