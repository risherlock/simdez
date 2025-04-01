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
    i =0;


    // Parse TLE
    char line1[70];
    char line2[70];
    strncpy(line1, parameters["orb_tle_1"].toString().toUtf8().constData(), 69); line1[69] = '\0';
    strncpy(line2, parameters["orb_tle_2"].toString().toUtf8().constData(), 69); line2[69] = '\0';
    tle.parseLines(line1, line2);
}

void simulation::run(void)
{
    qDebug() << "I am within simulation run" <<stop_flag;
    while(1)
    {
        if(i >= steps && !stop_flag)
        {
            qDebug() << i;
            stop_flag = true;
        }
        
        if(!stop_flag){
            // qDebug() << "working on a simulation";
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

            qDebug() << i << steps;

            i++;

            emit data_generated(time, lla, b, q);
            QThread::msleep(20);
        }
    }
}
