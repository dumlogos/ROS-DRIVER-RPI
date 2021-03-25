#include "can_handler.h"

CAN_Handler::CAN_Handler(QWidget *parent) :
    QWidget(parent),
    iface("can0"),
    heartbeatRequests(7)
{
    CANHandlerSetup();
    Transmitter = new CAN_Transmitter(this);
    Receiver = new CAN_Receiver(this);
    transmitterThread = new QThread(this);
    receiverThread = new QThread(this);

    heartbeatTimer = new QTimer(this);
    connect(heartbeatTimer, SIGNAL(timeout()), this, SLOT(HeartbeatTransmit()));
    //Изменить!!!!!!!!!!!!!!!!!!!!!!
    heartbeatTimer->start(20);
    connect(Receiver, SIGNAL(HeartbeatSignal()), this, SLOT(HeartbeatReceived()));
}

CAN_Handler::~CAN_Handler(){
    transmitterThread->terminate();
    receiverThread->terminate();
    delete transmitterThread;
    delete receiverThread;
}

bool CAN_Handler::CANHandlerSetup()
{
    memset(&(CAN_comData.frame), 0, sizeof(struct can_frame));

    //1.Create socket
    CAN_comData.s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (CAN_comData.s < 0) {
        return false;
    }

    //2.Specify can0 device
    strcpy(CAN_comData.ifr.ifr_name, iface.toStdString().c_str());
     CAN_comData.ret = ioctl(CAN_comData.s, SIOCGIFINDEX, &(CAN_comData.ifr));
    if (CAN_comData.ret < 0) {
        qDebug() << iface.toStdString().c_str() << "connection fault";
        return false;
    }
    else
        qDebug() << iface.toStdString().c_str() << "connected";

    //3.Bind the socket to can0
    CAN_comData.addr.can_family = AF_CAN;
    CAN_comData.addr.can_ifindex = CAN_comData.ifr.ifr_ifindex;
    CAN_comData.ret = bind(CAN_comData.s, (struct sockaddr *)&(CAN_comData.addr), sizeof(CAN_comData.addr));
    if (CAN_comData.ret < 0) {
        return false;
    }

    //4.Disable filtering rules, do not receive packets, only send
    setsockopt(CAN_comData.s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

    CAN_comData.frame.can_id = 0x125;
    CAN_comData.frame.can_dlc = 8;
    for(int i = 0; i < 8; i++){
        CAN_comData.frame.data[i] = 0;
        CAN_comData.RT_data.uintData[i]=0;
    }

    emit RatioQuery();

    return true;
}
bool CAN_Handler::CANHandlerSetDown()
{
    qDebug() << "shutdown" << iface;
    UNISTD_OVERRIDE::myCloseCan(CAN_comData.s);
    return 0;
}

bool CAN_Handler::CANSetInterface(CAN_IFace iface)
{
    CANHandlerSetDown();
    switch(iface){
        case CAN_IFace::VCAN:
            this->iface = "vcan0";
            break;
        case CAN_IFace::CAN0:
            this->iface = "can0";
            break;
        default:
            return false;
    }
    return CANHandlerSetup();
}

void CAN_Handler::Handle()
{
    connect(receiverThread, &QThread::started, Receiver, &CAN_Receiver::receiveAll);
    connect(receiverThread, &QThread::finished, Receiver, &QObject::deleteLater);
    connect(transmitterThread, &QThread::finished, Transmitter, &QObject::deleteLater);
    Transmitter->moveToThread(transmitterThread);
    Receiver->moveToThread(receiverThread);
    transmitterThread->start();
    receiverThread->start();
}

void CAN_Handler::HeartbeatTransmit()
{
    if(heartbeatRequests >= 7){
        heartbeatRequests = 0;
        Transmitter->transmitCommand(toCanId(Device_ID::CAN_All, RPiCommand::Heartbeat));
    }
}
void CAN_Handler::HeartbeatReceived()
{
    heartbeatRequests++;
}

CAN_Struct* CAN_Handler::getCAN_Struct(){
    return &CAN_comData;
}

QString CAN_Handler::getIface()
{
    return iface;
}
uint32_t toCanId(Device_ID device, ControllerCommand command)
{
    return (uint32_t)device + (uint32_t)command;
}
uint32_t toCanId(Device_ID device, ControllerData dataType)
{
    return (uint32_t)device + (uint32_t)dataType;
}
uint32_t toCanId(Device_ID device, RPiCommand command)
{
    return (uint32_t)device + (uint32_t)command;
}
