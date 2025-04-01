#include "simulation.h"

#include "dcm.h"
#include "igrf.h"
#include "time.h"
#include "frame.h"
#include "rigid.h"
#include "quat.h"

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

    // Parse TLE
    char line1[70];
    char line2[70];
    strncpy(line1, "1 25544U 98067A   25264.51782528 -.00002182  00000-0 -11606-4 0  2927", 69); line1[69] = '\0';
    strncpy(line2, "2 25544  51.6416 247.4627 0006703 130.5360 325.0288 15.72125391563537", 69); line2[69] = '\0';
    tle.parseLines(line1, line2);
}

state_t get_err(const state_t xd, const state_t xf)
{
    state_t xerr;

    quat_err(xd.q, xf.q, xerr.q);
    xerr.w[0] = xf.w[0];
    xerr.w[1] = xf.w[1];
    xerr.w[2] = xf.w[2];

    return xerr;
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

    state_t x = x0;

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

        // Rotational dynamics
        //state_t xerr = get_err(xd, x);

        double tau[3] = {0.0, 0.0, 0.0};
        x = rk4(x, stepmin, I, tau);

        // Simulation output
        t.append(time);
        bx.append(b[0]);
        by.append(b[1]);
        bz.append(b[2]);
        q0.append(x.q[0]);
        q1.append(x.q[1]);
        q2.append(x.q[2]);
        q3.append(x.q[3]);

        qDebug() << x.q[0] << x.q[1] << x.q[2] << x.q[3];

        i++;
        emit data_generated(time, lla, b, q);
        QThread::msleep(20);

        if(i >= steps)
        {
            stop_flag = true;
        }
    }
}
