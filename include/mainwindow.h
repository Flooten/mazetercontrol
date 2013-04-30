/*
 * FILNAMN:       mainwindow.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-16
 *
 * BESKRIVNING:   MainWindow är klassen som representerar huvudfönstret.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "control.h"
#include "terminal.h"
#include "mcgraphicsscene.h"
#include "controlsignalsplotscene.h"
#include "sensordataplotscene.h"

#include <QMainWindow>
#include <QtCore>
#include <QTimer>

namespace Ui
{
    class MainWindow;
}

namespace MC
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        void keyPressEvent(QKeyEvent* event);
        void keyReleaseEvent(QKeyEvent* event);

    public slots:
        void log(const QString& str);
        void clearLog();
        void btConnected();
        void btDisconnected();

        void setControlGagues(ControlSignals control_signals);
        void setSensorValues(SensorData sensor_data);
        void setMode(Control::Mode mode);

    private:
        int calibrate_countdown_ = 5;
        Ui::MainWindow *ui;
        Control* mc_;
        Terminal* terminal_;
        QTimer* plot_timer_;
        QTimer* calibrate_countdown_timer_;
        MCGraphicsScene* scene_;
        ControlSignalsPlotScene* cs_scene_;
        SensorDataPlotScene* sd_scene_;

        const int PLOT_DELTA_T = 50;
        const int PLOT_VIEW_WIDTH = 770;
        const int PLOT_VIEW_HEIGHT = 255;
        const QString INI_FILE = QCoreApplication::applicationDirPath() + "/data/mc.ini";

        void enableWidgets();
        void disableWidgets();

        void setRightEngineGauge(unsigned char value, char direction);
        void setLeftEngineGauge(unsigned char value, char direction);

        void statusMessage(const QString& str);

        void clearPlots();
        void drawPlotGrid();

        void writePreferences();

    private slots:
        void openTerminal();
        void closeTerminal();
        void openAboutDialog();
        void openPreferences();
        void toggleConnection();
        void resetPlots();
        void drawPlots();
        void centerControlSignalsPlot(int time);
        void centerSensorDataPlot(int time);
        void chosenSensorDataChanged(int index);
        void transmitCalibrateSensor();
        void transmitParameters();
        void exitApplication();
        void calibrateCountdown();
        void throttleRelay();
        void throttleValueChanged(char throttle_value);
    };
} // namespace MC

#endif // MAINWINDOW_H
