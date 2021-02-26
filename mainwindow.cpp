#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    angleCurve = new QwtPlotCurve("S(t)");
    velocityCurve = new QwtPlotCurve("U(t)");

    angleCurve->setStyle(QwtPlotCurve::Lines);
    velocityCurve->setStyle(QwtPlotCurve::Lines);
    angleCurve->setPen(QPen(Qt::blue));
    velocityCurve->setPen(QPen(Qt::red));
    angleCurve->attach(ui->anglePlot);
    velocityCurve->attach(ui->velocityPlot);
    ui->anglePlot->replot();
    ui->velocityPlot->replot();

    plotTimer = new QTimer(this);
    connect(plotTimer, SIGNAL(timeout()), this, SLOT(rePaint()));
    plotTimer->start(5);

    angleParameterValidator = new QDoubleValidator(-11520, 11520, 2, this);
    velocityParameterValidator = new QDoubleValidator(0, 50, 2, this);
    PIDratioParameterValidator = new QDoubleValidator(0, 10, 8, this);
    angleParameterValidator->setNotation(QDoubleValidator::StandardNotation);
    velocityParameterValidator->setNotation(QDoubleValidator::StandardNotation);
    PIDratioParameterValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->angleLineEdit->setValidator(angleParameterValidator);
    ui->velocityLineEdit->setValidator(velocityParameterValidator);
    ui->anglePLE->setValidator(PIDratioParameterValidator);
    ui->angleILE->setValidator(PIDratioParameterValidator);
    ui->angleDLE->setValidator(PIDratioParameterValidator);
    ui->velocityPLE->setValidator(PIDratioParameterValidator);
    ui->velocityILE->setValidator(PIDratioParameterValidator);
    ui->velocityDLE->setValidator(PIDratioParameterValidator);

    CAN_handler = new CAN_Handler(this);

    qRegisterMetaType<DriverState>("Device_ID");
    qRegisterMetaType<DriverState>("RPiCommands");
    qRegisterMetaType<DriverState>("ControllerCommand");
    qRegisterMetaType<DriverState>("ControllerData");


    connect(CAN_handler->Receiver, SIGNAL(AngleSignal(double, double)),
                            this, SLOT(angleReceived(double, double)));
    connect(CAN_handler->Receiver, SIGNAL(VelocitySignal(double, double)),
                            this, SLOT(velocityReceived(double, double)));

    connect(CAN_handler->Receiver, SIGNAL(CleanPlotSignal()),
                            this, SLOT(on_clearPlotButton_released()));
    connect(CAN_handler->Receiver, SIGNAL(CleanPlotSignal()),
            CAN_handler->Transmitter, SLOT(transmitClearPlot()));

    connect(this , SIGNAL(allowTransmitAngle(float, Device_ID)),
            CAN_handler->Transmitter, SLOT(transmitAngle(float, Device_ID)));
    connect(this , SIGNAL(allowTransmitRatio(float, Device_ID, ControllerData)),
            CAN_handler->Transmitter, SLOT(transmitRatio(float, Device_ID, ControllerData)));
    connect(this , SIGNAL(allowTransmitCommand(Device_ID, ControllerCommand, uint8_t*)),
            CAN_handler->Transmitter, SLOT(transmitCommand(Device_ID, ControllerCommand, uint8_t*)));
    connect(this , SIGNAL(allowTransmitCommand(Device_ID, RPiCommand, uint8_t*)),
            CAN_handler->Transmitter, SLOT(transmitCommand(Device_ID, RPiCommand, uint8_t*)));

    CAN_handler->Handle();
}



MainWindow::~MainWindow()
{
    CAN_handler->CAN_Handler_SetDown();
    delete ui;
    delete CAN_handler;
    delete angleCurve;
    delete velocityCurve;
}

void MainWindow::rePaint()
{
    if(!anglePlotPoints.isEmpty() && !velocityPlotPoints.isEmpty()){
    if(anglePlotPoints.count() >= 250)
        anglePlotPoints.removeFirst();
    if(velocityPlotPoints.count() >= 250)
        velocityPlotPoints.removeFirst();
    }
    angleCurve->setSamples(anglePlotPoints);
    velocityCurve->setSamples(velocityPlotPoints);

    ui->anglePlot->replot();
    ui->velocityPlot->replot();

}

void MainWindow::on_startStopButton_released()
{
    if(ui->angleLineEdit->hasAcceptableInput() /*&& ui->velocityLineEdit->hasAcceptableInput()*/){
        if(ui->startStopButton->styleSheet() != "QPushButton {background-color: red; border: none; }" ||
           ui->startStopButton->styleSheet() == "QPushButton {background-color: orange; border: none; }"){
                ui->startStopButton->setStyleSheet("QPushButton {background-color: red; border: none; }");
                emit allowTransmitAngle(toPointDouble(ui->angleLineEdit->text()), Device_ID::CAN_STM1);
                emit allowTransmitCommand(Device_ID::CAN_STM1, ControllerCommand::MotorMoved);
                ui->startStopButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0); border: none; }");

        }
        else{
                ui->startStopButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0); border: none; }");
                emit allowTransmitCommand(Device_ID::CAN_STM1, ControllerCommand::MotorStopped);
        }
    }
    else
        ui->startStopButton->setStyleSheet("QPushButton {background-color: orange; border: none; }");
}

/************События приёма данных о положении и скорости**************/

void MainWindow::angleReceived(double angle, double timeStamp)
{
    qDebug() << "angle" << angle << timeStamp;
    anglePlotPoints.append(QPointF(timeStamp, angle));

    ui->angleLabel->setText("Текущий угол поворота: " + QString::number(angle, 'f', 3));
}
void MainWindow::velocityReceived(double velocity, double timeStamp)
{
    qDebug() << "velocity " << velocity << timeStamp;
    velocityPlotPoints.append(QPointF(timeStamp, velocity));

    ui->velocityLabel->setText("Текущая угловая скорость: " + QString::number(velocity, 'f', 3));
}

/************Кнопки очистки графиков и смены состояния**************/
void MainWindow::on_clearPlotButton_released()
{
    anglePlotPoints.clear();
    velocityPlotPoints.clear();
    rePaint();
}
void MainWindow::on_clearPlotButton_2_released()
{
    emit allowTransmitCommand(Device_ID::CAN_All, RPiCommand::T_CleanPlot);
}
void MainWindow::on_dirButton_released()
{
    if(ui->dirButton->styleSheet() != "QPushButton {background-color: lightblue; border: none; }")
        ui->dirButton->setStyleSheet("QPushButton {background-color: lightblue; border: none; }");
    else
        ui->dirButton->setStyleSheet("QPushButton {background-color: yellow; border: none; }");
}

/************Кнопки отправки коэффициентов регулирования**************/
void MainWindow::on_anglePButton_released()
{
    if(ui->anglePLE->hasAcceptableInput()){
        if(ui->anglePButton->styleSheet() != "QPushButton {background-color: red; border: none; }"){
                ui->anglePButton->setStyleSheet("QPushButton {background-color: red; border: none; }");
                emit allowTransmitRatio(toPointDouble(ui->anglePLE->text()),
                                        Device_ID::CAN_STM1, ControllerData::T_PositionProportionalRatio);
                ui->anglePButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0); border: none; }");

        }
    }
    else
        ui->anglePButton->setStyleSheet("QPushButton {background-color: orange; border: none; }");

}
void MainWindow::on_angleIButton_released()
{
    if(ui->angleILE->hasAcceptableInput()){
        if(ui->angleIButton->styleSheet() != "QPushButton {background-color: red; border: none; }"){
                ui->angleIButton->setStyleSheet("QPushButton {background-color: red; border: none; }");
                emit allowTransmitRatio(toPointDouble(ui->angleILE->text()),
                                        Device_ID::CAN_STM1, ControllerData::T_PositionIntegralRatio);
                ui->angleIButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0); border: none; }");
        }
    }
    else
        ui->angleIButton->setStyleSheet("QPushButton {background-color: orange; border: none; }");
}
void MainWindow::on_angleDButton_released()
{
    if(ui->angleDLE->hasAcceptableInput()){
        if(ui->angleDButton->styleSheet() != "QPushButton {background-color: red; border: none; }"){
                ui->angleDButton->setStyleSheet("QPushButton {background-color: red; border: none; }");
                emit allowTransmitRatio(toPointDouble(ui->angleDLE->text()),
                                        Device_ID::CAN_STM1, ControllerData::T_PositionDifferentialRatio);
                ui->angleDButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0); border: none; }");
        }
    }
    else
        ui->angleDButton->setStyleSheet("QPushButton {background-color: orange; border: none; }");
}
void MainWindow::on_velocityPButton_released()
{
    if(ui->velocityPLE->hasAcceptableInput()){
        if(ui->velocityPButton->styleSheet() != "QPushButton {background-color: red; border: none; }"){
                ui->velocityPButton->setStyleSheet("QPushButton {background-color: red; border: none; }");
                emit allowTransmitRatio(toPointDouble(ui->velocityPLE->text()),
                                        Device_ID::CAN_STM1, ControllerData::T_SpeedProportionalRatio);
                ui->velocityPButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0); border: none; }");
        }
    }
    else
        ui->velocityPButton->setStyleSheet("QPushButton {background-color: orange; border: none; }");

}
void MainWindow::on_velocityIButton_released()
{
    if(ui->velocityILE->hasAcceptableInput()){
        if(ui->velocityIButton->styleSheet() != "QPushButton {background-color: red; border: none; }"){
                ui->velocityIButton->setStyleSheet("QPushButton {background-color: red; border: none; }");
                emit allowTransmitRatio(toPointDouble(ui->velocityILE->text()),
                                        Device_ID::CAN_STM1, ControllerData::T_SpeedIntegralRatio);
                ui->velocityIButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0); border: none; }");
        }
    }
    else
        ui->velocityIButton->setStyleSheet("QPushButton {background-color: orange; border: none; }");
}
void MainWindow::on_velocityDButton_released()
{
    if(ui->velocityDLE->hasAcceptableInput()){
        if(ui->velocityDButton->styleSheet() != "QPushButton {background-color: red; border: none; }"){
                ui->velocityDButton->setStyleSheet("QPushButton {background-color: red; border: none; }");
                emit allowTransmitRatio(toPointDouble(ui->velocityDLE->text()),
                                        Device_ID::CAN_STM1, ControllerData::T_SpeedDifferentialRatio);
                ui->velocityDButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0); border: none; }");
        }
    }
    else
        ui->velocityDButton->setStyleSheet("QPushButton {background-color: orange; border: none; }");
}

/************Вспомогательные функции**************/
double toPointDouble(QString commaDouble)
{
    return commaDouble.replace(",", ".").toDouble();
}
