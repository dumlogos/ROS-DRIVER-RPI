#include "drivercontroller.h"

DriverController::DriverController():
    driverState(true),
    lockKeyState(true)
{
    controllerID = controllerCount++;
}

DriverController::~DriverController()
{
    controllerCount--;
}

uint32_t DriverController::controllerCount = 0;
