#ifndef CAN_ENUM_H
#define CAN_ENUM_H

enum Device_ID{
    /***All Range***/
        CAN_All,
    /*************RPi range*************/
        CAN_RPi = 0x100,
    /*************Controller 1 range*************/
        CAN_STM1 = CAN_RPi + 0x200,
    /*************Controller 2 range*************/
        CAN_STM2 = CAN_STM1 + 0x80,
    /*************Controller 3 range*************/
        CAN_STM3 = CAN_STM2 + 0x80,
    /*************Controller 4 range*************/
        CAN_STM4 = CAN_STM3 + 0x80,
    /*************Controller 5 range*************/
        CAN_STM5 = CAN_STM4 + 0x80,
    /*************Controller 6 range*************/
        CAN_STM6 = CAN_STM5 + 0x80

};

enum RPiCommand{
        All,
        Heartbeat,
    //Common commands
        Init,
        DeInit,
        Error,
        EmergencyStop,
    //Robot control commands
        MovingStart,
        MovingStop,
    //GUI commands
        T_AllDataQuery,
        T_CleanPlot,
        R_CleanPlot
};


enum ControllerCommand{
        ConnectionAcknowledge = R_CleanPlot + 1,
        Disconnection,
    //Robot control commands
        MotorMoved,
        MotorStopped,
    //Controller commands
        HeartbeatRespond,
        RegulatorRatioTransmitRequest,
        RegulatorRatioTransmitAcknowledge,
        RegulatorRatioReceiveRequest,
        RegulatorRatioReceiveAcknowledge,
        MovingStartedAcknowledge,
        MovingFinishedAcknowedge,
        ToggleLockKey,
        ToggleStopDriver
};

enum ControllerData{
        T_Position = ToggleStopDriver + 1,
        T_Speed,
        R_Position,
        R_Speed,
        R_Current,

        T_PositionProportionalRatio,
        T_PositionIntegralRatio,
        T_PositionDifferentialRatio,
        T_SpeedProportionalRatio,
        T_SpeedIntegralRatio,
        T_SpeedDifferentialRatio,

        R_PositionProportionalRatio,
        R_PositionIntegralRatio,
        R_PositionDifferentialRatio,
        R_SpeedProportionalRatio,
        R_SpeedIntegralRatio,
        R_SpeedDifferentialRatio,

        R_DriverState
};

enum DriverState{
       STOP,
       START
};

enum CAN_IFace{
       VCAN,
       CAN0
};

enum ControllerList{
       STM1 = 1,
       STM2,
       STM3,
       STM4,
       STM5,
       STM6
};


#endif // CAN_ENUM_H
