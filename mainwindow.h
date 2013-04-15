#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "control.h"
#include "terminal.h"

#include <QMainWindow>
#include <QtGui>
#include <QtCore>

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

        void enableWidgets();
        void disableWidgets();

        void statusMessage(const QString& str);

    public slots:
        void log(const QString& str);
        void clear();
        void closeTerminal();
        void btConnected();
        void btDisconnected();

        void setEngineGagues(ControlSignals control_signals);
        void setMode(Control::Mode mode);

    private slots:
    };
} // namespace MC

#endif // MAINWINDOW_H
