#include "drivercontroller.h"

DriverController::DriverController():
    driverState(true),
    lockKeyState(true)
{
    controllerID = controllerCount++;

    positionProportionalRatio = 0;
    positionIntegralRatio = 0;
    positionDifferentialRatio = 0;
    speedProportionalRatio = 0;
    speedIntegralRatio = 0;
    speedDifferentialRatio = 0;
}

DriverController::~DriverController()
{
    controllerCount--;
}

uint32_t DriverController::controllerCount = 0;
