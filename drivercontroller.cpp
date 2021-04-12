#include "drivercontroller.h"

DriverController::DriverController():
    driverState(true),
    lockKeyState(true)
{
    controllerID = controllerCount++;

    positionProportionalRatio = 1;
    positionIntegralRatio = 1;
    positionDifferentialRatio = 1;
    speedProportionalRatio = 0;
    speedIntegralRatio = 0;
    speedDifferentialRatio = 0;
}

DriverController::~DriverController()
{
    controllerCount--;
}

uint32_t DriverController::controllerCount = 0;

QDataStream& operator<<(QDataStream& d, const DriverController& drC){
    d << drC.positionProportionalRatio << drC.positionIntegralRatio << drC.positionDifferentialRatio    \
      << drC.speedProportionalRatio << drC.speedIntegralRatio << drC.speedDifferentialRatio;
    return d;
}

QDataStream& operator>>(QDataStream& d, DriverController& drC){
    d >> drC.positionProportionalRatio >> drC.positionIntegralRatio >> drC.positionDifferentialRatio    \
      >> drC.speedProportionalRatio >> drC.speedIntegralRatio >> drC.speedDifferentialRatio;
    return d;
}
