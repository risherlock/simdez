#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>
#include "TLE.h"

class simulation : public QObject
{
    Q_OBJECT
public:
    explicit simulation(QObject *parent = nullptr);
    bool stop_flag = false;

Q_SIGNALS:
    void data_generated(double t, double lla[3], double b[3], double q[4]);

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
