#ifndef CANCLASS_H
#define CANCLASS_H

#include "UNISTD_CLOSE_OVERRIDE.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <math.h>

#include <QObject>
#include <QWidget>


class CAN_Class : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool transmitRunnning READ getTransmitRunnning WRITE setTransmitRunnning NOTIFY transmitRunnningChanged)
    Q_PROPERTY(bool receiveRunning READ getReceiveRunning WRITE setReceiveRunning NOTIFY receiveRunningChanged)
    Q_PROPERTY(double angle READ getAngle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(double velocity READ getVelocity WRITE setVelocity NOTIFY velocityChanged)

private:

    typedef union{
        unsigned char uint8t[8];
        struct floatsM{
            float fl1;
            float fl2;
        } floats;
    } uintFloat;
    uintFloat myUintFloat;
    int ret;
    int s, nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    unsigned int paramId;
    const double degToRad = 0.0174;

    double m_angle;
    double m_velocity;

    bool m_transmitRunnning;

    bool m_receiveRunning;

public:
    explicit CAN_Class(QWidget *parent = nullptr);

    void dataTransmit(double *per, double *variable);
    void dataTransmit(double per, double variable, double variable2);
    void brakeSwitch();
    int canSetUp();
    int canSetDown();

    double getAngle() const
    {
        return m_angle;
    }
    double getVelocity() const
    {
        return m_velocity;
    }   
    bool getTransmitRunnning() const
    {
        return m_transmitRunnning;
    }
    bool getReceiveRunning() const
    {
        return m_receiveRunning;
    }

public slots:
    void setAngle(double angle)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_angle, angle))
            return;

        m_angle = angle;
        emit angleChanged(m_angle);
    }

    void setVelocity(double velocity)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_velocity, velocity))
            return;

        m_velocity = velocity;
        emit velocityChanged(m_velocity);
    }

    void setTransmitRunnning(bool transmitRunnning)
    {
        if (m_transmitRunnning == transmitRunnning)
            return;

        m_transmitRunnning = transmitRunnning;
        emit transmitRunnningChanged(m_transmitRunnning);
    }

    void setReceiveRunning(bool receiveRunning)
    {
        if (m_receiveRunning == receiveRunning)
            return;

        m_receiveRunning = receiveRunning;
        emit receiveRunningChanged(m_receiveRunning);
    }

signals:

    void angleChanged(double angle);
    void velocityChanged(double velocity);
    void transmitRunnningChanged(bool transmitRunnning);
    void receiveRunningChanged(bool receiveRunning);
};

#endif // CANCLASS_H
