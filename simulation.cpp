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

QVector<double> t, bx, by, bz, q0, q1, q2, q3, w0, w1, w2;
bool stop_flag = false;

int get_sign(double x)
{
    if (x < 0)
    {
        return -1;
    }

    return 1;
}

simulation::simulation(QObject *parent, const QMap<QString, QVariant> &parameters)
    : QObject{parent}
{
    updateParameters(parameters);
    qDebug() << "Simulation has been initialized";
}

void simulation::updateParameters(const QMap<QString, QVariant> &parameters)
{
    qDebug() << "updating simulation parameters";
    qDebug() << stepmin << startmin << stopmin << steps;

    // Simulation params
    stepmin = parameters["sim_step_time"].toDouble();
    startmin = 0;
    stopmin = parameters["sim_stop_time"].toDouble();
    steps = (stopmin - startmin) / stepmin;
    i = 0;

    // Parse TLE
    char line1[70];
    char line2[70];
    strncpy(line1, parameters["orb_tle_1"].toString().toUtf8().constData(), 69);
    line1[69] = '\0';
    strncpy(line2, parameters["orb_tle_2"].toString().toUtf8().constData(), 69);
    line2[69] = '\0';
    tle.parseLines(line1, line2);
}

// tau = -kp * sign(dq(4)) * dq(1 : 3) - kd * w;
void control_law(const state_t err, const double kp, const double kd, double tau[3])
{
    int q0_sign = get_sign(err.q[0]);
    tau[0] = -kp * q0_sign * err.q[1] - kd * err.w[0];
    tau[1] = -kp * q0_sign * err.q[2] - kd * err.w[1];
    tau[2] = -kp * q0_sign * err.q[3] - kd * err.w[2];
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
    qDebug() << "I am within simulation run" << stop_flag;

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

    while (1)
    {
        if (i >= steps && !stop_flag)
        {
            qDebug() << i;
            stop_flag = true;
        }

        if (!stop_flag)
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

            state_t xerr = get_err(xd, x);
            double tau[3] = {0.0, 0.0, 0.0};
            control_law(xerr, kp, kd, tau);

            // Rotational dynamics
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
            w0.append(x.w[0]);
            w1.append(x.w[1]);
            w2.append(x.w[2]);

            qDebug() << x.q[0] << x.q[1] << x.q[2] << x.q[3];

            i++;
            emit data_generated(time, lla, b, q);
            QThread::msleep(20);
            // // qDebug() << "working on a simulation";
            // double dcm[3][3], r_ecef[3], r[3], v[3], lla[3], b[3];
            // double q[4] = {1, 0, 0, 0};
            // double time = startmin + i * stepmin;

            // // Orbit propagation and IGRF
            // tle.getRV(time, r, v);
            // frame_eci_to_ecef_dcm(tle.dt, dcm);
            // dcm_rotate(dcm, r, r_ecef);
            // frame_ecef_to_lla(r_ecef, lla);
            // igrf(tle.dt, lla, IGRF_GEODETIC, b);

            // // Simulation output
            // t.append(time);
            // bx.append(b[0]);
            // by.append(b[1]);
            // bz.append(b[2]);

            // qDebug() << i << steps;

            // i++;

            // emit data_generated(time, lla, b, q);
            // QThread::msleep(20);
        }
    }
}
