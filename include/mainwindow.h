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

        // Funktioner
        void keyPressEvent(QKeyEvent* event);
        void keyReleaseEvent(QKeyEvent* event);

    private:
        Ui::MainWindow *ui;
        Control* mc_;
        Terminal* terminal_;
        MCGraphicsScene* scene_;
        ControlSignalsPlotScene* cs_scene_;
        SensorDataPlotScene* sd_scene_;
        QTimer* plot_timer_;

        const int PLOT_DELTA_T = 50;

        void enableWidgets();
        void disableWidgets();

        void setRightEngineGauge(int value, char direction);
        void setLeftEngineGauge(int value, char direction);

        void statusMessage(const QString& str);

    public slots:
        void log(const QString& str);
        void clear();
        void btConnected();
        void btDisconnected();

        void setControlGagues(ControlSignals control_signals);
        void setSensorValues(SensorData sensor_data);
        void setMode(Control::Mode mode);

    private slots:
        void openTerminal();
        void closeTerminal();
        void openAboutDialog();
        void openPreferences();
        void toggleConnection();
        void clearPlots();
    };
} // namespace MC

#endif // MAINWINDOW_H