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
    accelerationCurve = new QwtPlotCurve("A(t)");
    currentCurve = new QwtPlotCurve("I(t");


    QwtPlotGrid *angleGrid = new QwtPlotGrid();
    QwtPlotGrid *velocityGrid = new QwtPlotGrid();
    QwtPlotGrid *accelerationGrid= new QwtPlotGrid();
    QwtPlotGrid *currentGrid= new QwtPlotGrid();
    angleGrid->setMajorPen(QPen(Qt::gray, 1));
    angleGrid->attach(ui->anglePlot);
    velocityGrid->setMajorPen(QPen(Qt::gray, 1));
    velocityGrid->attach(ui->velocityPlot);
    accelerationGrid->setMajorPen(QPen(Qt::gray, 1));
    accelerationGrid->attach(ui->accelerationPlot);
    currentGrid->setMajorPen(QPen(Qt::gray, 1));
    currentGrid->attach(ui->currentPlot);

    QwtPlotMagnifier *anglePlotMagnifier = new QwtPlotMagnifier(ui->anglePlot->canvas());
    QwtPlotMagnifier *velocityPlotMagnifier = new QwtPlotMagnifier(ui->velocityPlot->canvas());
    QwtPlotMagnifier *accelerationPlotMagnifier = new QwtPlotMagnifier(ui->accelerationPlot->canvas());
    QwtPlotMagnifier *currentPlotMagnifier = new QwtPlotMagnifier(ui->currentPlot->canvas());
    anglePlotMagnifier->setMouseButton(Qt::MidButton);
    velocityPlotMagnifier->setMouseButton(Qt::MidButton);
    accelerationPlotMagnifier->setMouseButton(Qt::MidButton);
    currentPlotMagnifier->setMouseButton(Qt::MidButton);

    QwtPlotPanner *anglePlotPanner = new QwtPlotPanner(ui->anglePlot->canvas());
    QwtPlotPanner *velocityPlotPanner = new QwtPlotPanner(ui->velocityPlot->canvas());
    QwtPlotPanner *accelerationPlotPanner = new QwtPlotPanner(ui->accelerationPlot->canvas());
    QwtPlotPanner *currentPlotPanner = new QwtPlotPanner(ui->currentPlot->canvas());
    anglePlotPanner->setMouseButton(Qt::RightButton);
    velocityPlotPanner->setMouseButton(Qt::RightButton);
    accelerationPlotPanner->setMouseButton(Qt::RightButton);
    currentPlotPanner->setMouseButton(Qt::RightButton);

    QwtPlotPicker *anglePlotPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                                       QwtPlotPicker::CrossRubberBand,
                                                       QwtPicker::ActiveOnly,
                                                       ui->anglePlot->canvas());
    QwtPlotPicker *velocityPlotPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                                       QwtPlotPicker::CrossRubberBand,
                                                       QwtPicker::ActiveOnly,
                                                       ui->velocityPlot->canvas());
    QwtPlotPicker *accelerationPlotPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                                       QwtPlotPicker::CrossRubberBand,
                                                       QwtPicker::ActiveOnly,
                                                       ui->accelerationPlot->canvas());
    QwtPlotPicker *currentPlotPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                                       QwtPlotPicker::CrossRubberBand,
                                                       QwtPicker::ActiveOnly,
                                                       ui->currentPlot->canvas());
    anglePlotPicker->setRubberBandPen(QColor(Qt::red));
    anglePlotPicker->setTrackerPen(QColor(Qt::black));
    anglePlotPicker->setStateMachine(new QwtPickerDragPointMachine());
    velocityPlotPicker->setRubberBandPen(QColor(Qt::red));
    velocityPlotPicker->setTrackerPen(QColor(Qt::black));
    velocityPlotPicker->setStateMachine(new QwtPickerDragPointMachine());
    accelerationPlotPicker->setRubberBandPen(QColor(Qt::red));
    accelerationPlotPicker->setTrackerPen(QColor(Qt::black));
    accelerationPlotPicker->setStateMachine(new QwtPickerDragPointMachine());
    currentPlotPicker->setRubberBandPen(QColor(Qt::red));
    currentPlotPicker->setTrackerPen(QColor(Qt::black));
    currentPlotPicker->setStateMachine(new QwtPickerDragPointMachine());


    angleCurve->setPen(QPen(Qt::blue));
    angleCurve->setStyle(QwtPlotCurve::Lines);
    angleCurve->attach(ui->anglePlot);
    velocityCurve->setPen(QPen(Qt::red));
    velocityCurve->setStyle(QwtPlotCurve::Lines);
    velocityCurve->attach(ui->velocityPlot);
    accelerationCurve->setPen(QPen(Qt::yellow));
    accelerationCurve->setStyle(QwtPlotCurve::Lines);
    accelerationCurve->attach(ui->accelerationPlot);
    currentCurve->setPen(QPen(Qt::yellow));
    currentCurve->setStyle(QwtPlotCurve::Lines);
    currentCurve->attach(ui->accelerationPlot);

    ui->anglePlot->setAxisTitle(QwtPlot::Axis::xBottom, "Время");
    ui->anglePlot->setAxisTitle(QwtPlot::Axis::yLeft, "Положение");
    ui->velocityPlot->setAxisTitle(QwtPlot::Axis::xBottom, "Время");
    ui->velocityPlot->setAxisTitle(QwtPlot::Axis::yLeft, "Скорость");
    ui->accelerationPlot->setAxisTitle(QwtPlot::Axis::xBottom, "Время");
    ui->accelerationPlot->setAxisTitle(QwtPlot::Axis::yLeft, "Ускорение");
    ui->currentPlot->setAxisTitle(QwtPlot::Axis::xBottom, "Время");
    ui->currentPlot->setAxisTitle(QwtPlot::Axis::yLeft, "Ток");

    ui->anglePlot->replot();
    ui->velocityPlot->replot();
    ui->accelerationPlot->replot();
    ui->currentPlot->replot();

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
    qRegisterMetaType<DriverState>("RPiCommand");
    qRegisterMetaType<DriverState>("ControllerCommand");
    qRegisterMetaType<DriverState>("ControllerData");


    connect(CAN_handler->Receiver, SIGNAL(AngleSignal(double, double)),
                            this, SLOT(angleReceived(double, double)));
    connect(CAN_handler->Receiver, SIGNAL(VelocitySignal(double, double)),
                            this, SLOT(velocityReceived(double, double)));
    connect(CAN_handler->Receiver, SIGNAL(CurrentSignal(double, double)),
                            this, SLOT(currentReceived(double, double)));

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
    if(accelerationPlotPoints.count() >= 250)
        accelerationPlotPoints.removeFirst();
    if(currentPlotPoints.count() >= 250)
        currentPlotPoints.removeFirst();
    }
    angleCurve->setSamples(anglePlotPoints);
    velocityCurve->setSamples(velocityPlotPoints);
    accelerationCurve->setSamples(accelerationPlotPoints);
    currentCurve->setSamples(currentPlotPoints);

    ui->anglePlot->replot();
    ui->velocityPlot->replot();
    ui->accelerationPlot->replot();
    ui->currentPlot->replot();

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
    if(!velocityPlotPoints.isEmpty())
        accelerationPlotPoints.append(QPointF(timeStamp, (velocity - velocityPlotPoints.last().y())/10/1000));
    else
        accelerationPlotPoints.append(QPoint(timeStamp, 0));

    qDebug() << "velocity " << velocity << timeStamp;
    velocityPlotPoints.append(QPointF(timeStamp, velocity));
    ui->velocityLabel->setText("Текущая угловая скорость: " + QString::number(velocity, 'f', 3));
}
void MainWindow::currentReceived(double current, double timeStamp)
{
    qDebug() << "current" << current << timeStamp;
    currentPlotPoints.append(QPointF(timeStamp, current));

    ui->currentLabel->setText("Текущий ток: " + QString::number(current, 'f', 3));
}

/************Кнопки очистки графиков и смены состояния**************/
void MainWindow::on_clearPlotButton_released()
{
    anglePlotPoints.clear();
    velocityPlotPoints.clear();
    accelerationPlotPoints.clear();
    currentPlotPoints.clear();
    rePaint();
}
void MainWindow::on_clearPlotButton_2_released()
{
    emit allowTransmitCommand(Device_ID::CAN_All, RPiCommand::T_CleanPlot);
}
void MainWindow::on_dirButton_released()
{
    if(ui->dirButton->styleSheet() != "QPushButton {background-color: rgb(194, 204, 126);}")
        ui->dirButton->setStyleSheet("QPushButton {background-color: rgb(194, 204, 126);}");
    else
        ui->dirButton->setStyleSheet("QPushButton {background-color: yellow;}");
}
void MainWindow::on_startStopButton_released()
{
    if(ui->angleLineEdit->hasAcceptableInput() /*&& ui->velocityLineEdit->hasAcceptableInput()*/){
        if(ui->startStopButton->styleSheet() != "QPushButton {background-color: red; }" ||
           ui->startStopButton->styleSheet() == "QPushButton {background-color: orange;  }"){
                ui->startStopButton->setStyleSheet("QPushButton {background-color: red; }");
                emit allowTransmitAngle(toPointDouble(ui->angleLineEdit->text()), Device_ID::CAN_STM1);
                emit allowTransmitCommand(Device_ID::CAN_STM1, ControllerCommand::MotorMoved);
                ui->startStopButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0);}");

        }
        else{
                ui->startStopButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0);}");
                emit allowTransmitCommand(Device_ID::CAN_STM1, ControllerCommand::MotorStopped);
        }
    }
    else
        ui->startStopButton->setStyleSheet("QPushButton {background-color: orange;}");
}

/************Кнопки отправки коэффициентов регулирования**************/
void MainWindow::on_anglePButton_released()
{
    if(ui->anglePLE->hasAcceptableInput()){
        if(ui->anglePButton->styleSheet() != "QPushButton {background-color: red;}"){
                ui->anglePButton->setStyleSheet("QPushButton {background-color: red;}");
                emit allowTransmitRatio(toPointDouble(ui->anglePLE->text()),
                                        Device_ID::CAN_STM1, ControllerData::T_PositionProportionalRatio);
                ui->anglePButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0);}");

        }
    }
    else
        ui->anglePButton->setStyleSheet("QPushButton {background-color: orange;}");

}
void MainWindow::on_angleIButton_released()
{
    if(ui->angleILE->hasAcceptableInput()){
        if(ui->angleIButton->styleSheet() != "QPushButton {background-color: red;}"){
                ui->angleIButton->setStyleSheet("QPushButton {background-color: red;}");
                emit allowTransmitRatio(toPointDouble(ui->angleILE->text()),
                                        Device_ID::CAN_STM1, ControllerData::T_PositionIntegralRatio);
                ui->angleIButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0);}");
        }
    }
    else
        ui->angleIButton->setStyleSheet("QPushButton {background-color: orange;}");
}
void MainWindow::on_angleDButton_released()
{
    if(ui->angleDLE->hasAcceptableInput()){
        if(ui->angleDButton->styleSheet() != "QPushButton {background-color: red;}"){
                ui->angleDButton->setStyleSheet("QPushButton {background-color: red;}");
                emit allowTransmitRatio(toPointDouble(ui->angleDLE->text()),
                                        Device_ID::CAN_STM1, ControllerData::T_PositionDifferentialRatio);
                ui->angleDButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0);}");
        }
    }
    else
        ui->angleDButton->setStyleSheet("QPushButton {background-color: orange;}");
}
void MainWindow::on_velocityPButton_released()
{
    if(ui->velocityPLE->hasAcceptableInput()){
        if(ui->velocityPButton->styleSheet() != "QPushButton {background-color: red;}"){
                ui->velocityPButton->setStyleSheet("QPushButton {background-color: red;}");
                emit allowTransmitRatio(toPointDouble(ui->velocityPLE->text()),
                                        Device_ID::CAN_STM1, ControllerData::T_SpeedProportionalRatio);
                ui->velocityPButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0);}");
        }
    }
    else
        ui->velocityPButton->setStyleSheet("QPushButton {background-color: orange;}");

}
void MainWindow::on_velocityIButton_released()
{
    if(ui->velocityILE->hasAcceptableInput()){
        if(ui->velocityIButton->styleSheet() != "QPushButton {background-color: red;}"){
                ui->velocityIButton->setStyleSheet("QPushButton {background-color: red;}");
                emit allowTransmitRatio(toPointDouble(ui->velocityILE->text()),
                                        Device_ID::CAN_STM1, ControllerData::T_SpeedIntegralRatio);
                ui->velocityIButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0);}");
        }
    }
    else
        ui->velocityIButton->setStyleSheet("QPushButton {background-color: orange;}");
}
void MainWindow::on_velocityDButton_released()
{
    if(ui->velocityDLE->hasAcceptableInput()){
        if(ui->velocityDButton->styleSheet() != "QPushButton {background-color: red;}"){
                ui->velocityDButton->setStyleSheet("QPushButton {background-color: red;}");
                emit allowTransmitRatio(toPointDouble(ui->velocityDLE->text()),
                                        Device_ID::CAN_STM1, ControllerData::T_SpeedDifferentialRatio);
                ui->velocityDButton->setStyleSheet("QPushButton {background-color: rgb(85, 255, 0);}");
        }
    }
    else
        ui->velocityDButton->setStyleSheet("QPushButton {background-color: orange;}");
}

/************События выбора CAN-интерфейса**************/
void MainWindow::on_updateIfaceBtn_released()
{
    QString iface = CAN_handler->getIface();
    if(ui->canRadioBtn->isChecked() && iface!="can0")
        CAN_handler->CAN_Set_Interface(CAN_IFace::CAN0);
    else if(ui->vcanRadioBtn->isChecked() && iface!="vcan0")
        CAN_handler->CAN_Set_Interface(CAN_IFace::VCAN);

}
/************Вспомогательные функции**************/
double toPointDouble(QString commaDouble)
{
    return commaDouble.replace(",", ".").toDouble();
}


