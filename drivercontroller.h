#ifndef DRIVERCONTROLLER_H
#define DRIVERCONTROLLER_H

#include <QObject>
#include <QDataStream>
#include <CAN_ENUM.h>


class DriverController
{
public:
    DriverController();
    ~DriverController();

    bool driverState;
    bool lockKeyState;

    static uint32_t controllerCount;
    uint32_t controllerID;

    float curPosition;
    float curSpeed;
    float curCurrent;

    float positionProportionalRatio;
    float positionIntegralRatio;
    float positionDifferentialRatio;
    float speedProportionalRatio;
    float speedIntegralRatio;
    float speedDifferentialRatio;

    friend QDataStream& operator<<(QDataStream& d, const DriverController& drC);
    friend QDataStream& operator>>(QDataStream& d, DriverController& drC);

public slots:


};


#endif // DRIVERCONTROLLER_H
