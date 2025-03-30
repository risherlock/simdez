#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>

class simulation : public QObject
{
    Q_OBJECT
public:
    explicit simulation(QObject *parent = nullptr);
    bool stop_flag = false;

signals:
    void data_generated(double t, double lla[3], double b[3], double q[4]);

public slots:
    void run(void);
};

#endif // SIMULATION_H
