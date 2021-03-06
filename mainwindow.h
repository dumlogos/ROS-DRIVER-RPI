#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "can_handler.h"


#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>
#include <qwt_curve_fitter.h>
#include <qwt_painter.h>
#include <qevent.h>

#include <QString>
#include <QMainWindow>
#include <QDoubleValidator>
#include <QVector>
#include <QPointF>
#include <QDebug>

double toPointDouble(QString commaDouble);

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    CAN_Handler *CAN_handler;
    QDoubleValidator *angleParameterValidator;
    QDoubleValidator *velocityParameterValidator;
    QDoubleValidator *PIDratioParameterValidator;


    QwtPlotCurve *angleCurve;
    QwtPlotCurve *velocityCurve;
    QVector<QPointF> anglePlotPoints;
    QVector<QPointF> velocityPlotPoints;

    QTimer *plotTimer;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void rePaint();
    void on_startStopButton_released();
    void on_dirButton_released();


    void angleReceived(double angle, double timeStamp);
    void velocityReceived(double velocity, double timeStamp);
    void on_clearPlotButton_released();


    void on_anglePButton_released();
    void on_angleIButton_released();
    void on_angleDButton_released();
    void on_velocityPButton_released();
    void on_velocityIButton_released();
    void on_velocityDButton_released();

    void on_clearPlotButton_2_released();

signals:
    void allowTransmitState(DriverState state);
    void allowTransmitRatio(float ratio, CAN_ID id);
    void allowTransmitAngle(float angle);



private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
