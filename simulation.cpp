#include "simulation.h"

#include "dcm.h"
#include "SGP4.h"
#include "igrf.h"
#include "time.h"
#include "frame.h"
#include "rigid.h"

#include <QDebug>
#include <QObject>
#include <QThread>

QVector<double> t, bx, by, bz, q0, q1, q2, q3;
bool stop_flag = false;

simulation::simulation(QObject *parent)
    : QObject{parent}
{
    // Simulation params
    stepmin = 1;
    startmin = 0;
    stopmin = 600;
    steps = (stopmin - startmin) / stepmin;

    // Moment of Inertia
    double I[3][3] = {{10000, 0, 0}, {0, 9000, 0}, {0, 0, 12000}};

    // Parse TLE
    char line1[70];
    char line2[70];
    strncpy(line1, "1 25544U 98067A   25264.51782528 -.00002182  00000-0 -11606-4 0  2927", 69); line1[69] = '\0';
    strncpy(line2, "2 25544  51.6416 247.4627 0006703 130.5360 325.0288 15.72125391563537", 69); line2[69] = '\0';
    tle.parseLines(line1, line2);
}

void simulation::run(void)
{
    // Control gains
    double kp = 50;
    double kd = 500;

    // Initial state
    state_t x0;
    x0.q[0] = 0.153;
    x0.q[1] = 0.685;
    x0.q[2] = 0.695;
    x0.q[3] = 0.153;
    x0.w[0] = -0.53 * D2R;
    x0.w[1] = 0.53 * D2R;
    x0.w[2] = 0.053 * D2R;

    // Desired state
    state_t xd;
    xd.q[0] = 1.0f;
    xd.q[1] = 0.0f;
    xd.q[2] = 0.0f;
    xd.q[3] = 0.0f;
    xd.w[0] = 0.0f;
    xd.w[1] = 0.0f;
    xd.w[2] = 0.0f;

    while(!stop_flag)
    {
        double dcm[3][3], r_ecef[3], r[3], v[3], lla[3], b[3];
        double q[4] = {1, 0, 0, 0};
        double time = startmin + i * stepmin;
        state_t x;

        // Orbit propagation and IGRF
        tle.getRV(time, r, v);
        frame_eci_to_ecef_dcm(tle.dt, dcm);
        dcm_rotate(dcm, r, r_ecef);
        frame_ecef_to_lla(r_ecef, lla);
        igrf(tle.dt, lla, IGRF_GEODETIC, b);

        // Rotational dynamics
        state_t xerr = get_err(xd, x)

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
