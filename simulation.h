#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>
#include "TLE.h"

extern QVector<double> t, bx, by, bz;
extern bool stop_flag;

class simulation : public QObject
{
    Q_OBJECT

signals:
    void data_generated(double time, double lla[3], double b[3], double q[4]);

public:
    explicit simulation(QObject *parent = nullptr);

private:
    double stepmin;
    double startmin;
    double stopmin;
    int steps;
    int i;
    TLE tle;

public slots:
    void run(void);
};

#endif // SIMULATION_H
