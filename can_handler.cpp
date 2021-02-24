#include "can_handler.h"

CAN_Handler::CAN_Handler(QWidget *parent) : QWidget(parent)
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
    strcpy(CAN_comData.ifr.ifr_name, "can0");
     CAN_comData.ret = ioctl(CAN_comData.s, SIOCGIFINDEX, &(CAN_comData.ifr));
    if (CAN_comData.ret < 0) {
        perror("ioctl failed");
        return 1;
    }

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
    printf("sleeeep\n");
    UNISTD_OVERRIDE::myCloseCan(CAN_comData.s);
    return 0;
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
