#ifndef CAN_ENUM_H
#define CAN_ENUM_H

enum Device_ID{
    /*************RPi range*************/
        CAN_RPi = 0x100,

    /*************Controller 1 range*************/
        CAN_STM1 = 0x300,

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

enum RPiCommands{
        All,
        Heartbeat                           = 0x010,

    //Common commands
        Init                                = 0x100,
        DeInit,
        Error,
        EmergencyStop,

    //GUI commands
        CleanPlot,

};


enum ControllerCommands{
        ConnectionAcknowledge,
        Disconnection,


    //Robot control commands
        MovingStart,
        MovingStop,

    //Controller commands
        HeartbeatRespond,
        RegulatorRatioTransmitRequest,
        RegulatorRatioTransmitAcknowledge,
        RegulatorRatioReceiveRequest,
        RegulatorRatioReceiveAcknowledge,
        MovingStartedAcknowledge,
        MovingFinishedAcknowedge,
        DriverStop
};

enum ControllerData{
        T_Position = DriverStop + 1,
        T_Speed,
        R_Position,
        R_Speed,

        T_PositionProportionalRatio,
        T_PositionDifferentialRatio,
        T_PositionIntegralRatio,
        T_SpeedProportionalRatio,
        T_SpeedDifferentialRatio,
        T_SpeedIntegralRatio,

        R_PositionProportionalRatio,
        R_PositionDifferentialRatio,
        R_PositionIntegralRatio,
        R_SpeedProportionalRatio,
        R_SpeedDifferentialRatio,
        R_SpeedIntegralRatio
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
