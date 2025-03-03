#ifndef _SATELLITE_H_
#define _SATELLITE_H_

#include <QTimer>
#include <QObject>
#include <QQuaternion>

class satellite : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuaternion q READ get_q WRITE set_q NOTIFY q_changed)

public:
    explicit satellite(QObject *parent = nullptr);
    void set_q(QQuaternion qin);
    QQuaternion get_q(void);

signals:
    void q_changed();

private:
    QQuaternion q;
};

#endif // _SATELLITE_H_
