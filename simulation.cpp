#include "simulation.h"

#include "dcm.h"
#include "SGP4.h"
#include "TLE.h"
#include "igrf.h"
#include "time.h"
#include "frame.h"

#include <QDebug>
#include <QThread>

simulation::simulation(QThread *parent)
    : QThread{parent}
{}

TLE tle;

void simulation::run(void)
{
    if(!stop_flag)
    {
        char line1[70];
        char line2[70];
        double stepmin = 1;
        double startmin = 0;
        double stopmin = 600;
        strncpy(line1, "1 25544U 98067A   25264.51782528 -.00002182  00000-0 -11606-4 0  2927", 69); line1[69] = '\0';
        strncpy(line2, "2 25544  51.6416 247.4627 0006703 130.5360 325.0288 15.72125391563537", 69); line2[69] = '\0';
        tle.parseLines(line1, line2);

        int steps = (stopmin - startmin) / stepmin;

        for (int i = 0; i < steps; i++)
        {
            double dcm[3][3], r_ecef[3], r[3], v[3], lla[3], b[3];
            double q[4] = {1, 0, 0, 0};
            double t = startmin + i * stepmin;

            // Geodetic coordinates of satellite
            tle.getRV(t, r, v);
            frame_eci_to_ecef_dcm(tle.dt, dcm);
            dcm_rotate(dcm, r, r_ecef);
            frame_ecef_to_lla(r_ecef, lla);

            // Compute magnetic field
            if (!igrf(tle.dt, lla, IGRF_GEODETIC, b))
            {
                qDebug() << "IGRF date error!" << (int)tle.dt.year;
            }
            //else
            //{
            qDebug() << i << b[0] << b[1] << b[2];
                //mag_add_data(ui->widget_plot_mag, (double)i, b);
            //}
            //qDebug() << r[0] << "," << r[1] << "," << r[2];
            //gt_draw(lla[0], lla[1]);

            QThread::msleep(20);
            emit data_generated(t, lla, b, q);
        }
        stop_flag = true;
    }
}
