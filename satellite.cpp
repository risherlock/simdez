#include "satellite.h"

satellite::satellite(QObject *parent)
    : QObject(parent), q()
{
    QVector3D euler(0, 0, 0); // Initial orientation
    q = QQuaternion::fromEulerAngles(euler);
}

QQuaternion satellite::get_q(void)
{
    return q;
}

void satellite::set_q(QQuaternion qin)
{
    if (q != qin)
    {
        q = qin;
        emit q_changed();
    }
}
