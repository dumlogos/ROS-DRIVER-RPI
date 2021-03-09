#include "can_handler.h"

CAN_Handler::CAN_Handler(QWidget *parent) :
    QWidget(parent),
    iface("can0")
{
    CAN_Handler_Setup();
    Transmitter = new CAN_Transmitter(this);
    Receiver = new CAN_Receiver(this);
    transmitterThread = new QThread(this);
    receiverThread = new QThread(this);
}

CAN_Handler::~CAN_Handler(){
    transmitterThread->terminate();
    receiverThread->terminate();
    delete transmitterThread;
    delete receiverThread;
}

bool CAN_Handler::CAN_Handler_Setup()
{
    memset(&(CAN_comData.frame), 0, sizeof(struct can_frame));

    printf("this is a can send demo\r\n");

    //1.Create socket
    CAN_comData.s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (CAN_comData.s < 0) {
        perror("socket PF_CAN failed");
        return 1;
    }

    //2.Specify can0 device
    strcpy(CAN_comData.ifr.ifr_name, iface.toStdString().c_str());
     CAN_comData.ret = ioctl(CAN_comData.s, SIOCGIFINDEX, &(CAN_comData.ifr));
    if (CAN_comData.ret < 0) {
        qDebug() << iface.toStdString().c_str() << "connection fault";
        return 1;
    }
    else
        qDebug() << iface.toStdString().c_str() << "connected";

    //3.Bind the socket to can0
    CAN_comData.addr.can_family = AF_CAN;
    CAN_comData.addr.can_ifindex = CAN_comData.ifr.ifr_ifindex;
    CAN_comData.ret = bind(CAN_comData.s, (struct sockaddr *)&(CAN_comData.addr), sizeof(CAN_comData.addr));
    if (CAN_comData.ret < 0) {
        perror("bind failed");
        return 1;
    }

    //4.Disable filtering rules, do not receive packets, only send
    setsockopt(CAN_comData.s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

    CAN_comData.frame.can_id = 0x125;
    CAN_comData.frame.can_dlc = 8;
    for(int i = 0; i < 8; i++){
        CAN_comData.frame.data[i] = 0;
        CAN_comData.RT_data.uintData[i]=0;
    }

    return 0;
}

bool CAN_Handler::CAN_Handler_SetDown()
{
    qDebug() << "shutdown" << iface;
    UNISTD_OVERRIDE::myCloseCan(CAN_comData.s);
    return 0;
}

QString CAN_Handler::getIface()
{
    return iface;
}

bool CAN_Handler::CAN_Set_Interface(CAN_IFace iface)
{
    CAN_Handler_SetDown();
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
    return CAN_Handler_Setup();
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

CAN_Struct* CAN_Handler::getCAN_Struct(){
    return &CAN_comData;
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
