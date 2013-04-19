/*
 * FILNAMN:       mainwindow.cc
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-16
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "controlsignals.h"
#include "preferencesdialog.h"
#include "aboutdialog.h"

namespace MC
{
    /*
     *  Public
     */
    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , mc_(new Control(utils::INI_FILE))
        , terminal_(new Terminal(mc_, this))
        , plot_timer_(new QTimer(this))
    {
        ui->setupUi(this);

        this->setFocusPolicy(Qt::StrongFocus);
        this->setFocus();

        scene_ = new MCGraphicsScene(this);
        cs_scene_ = new ControlSignalsPlotScene(this);
        sd_scene_ = new SensorDataPlotScene(this);

        // Subscriptad text
        ui->label_kd->setText("K<sub>D</sub>");
        ui->label_kp->setText("K<sub>P</sub>");

        // Uppdatera fönsterrubrik
        setWindowTitle(windowTitle() + " " + utils::VERSION);

        // Installera scener i GraphicsView
        ui->graphicsView_overview->setScene(scene_);
        ui->graphicsView_control_signals->setScene(cs_scene_);
        ui->graphicsView_sensor_data->setScene(sd_scene_);

        cs_scene_->setSceneRect(0, 0, ui->graphicsView_control_signals->width(), ui->graphicsView_control_signals->height());
        sd_scene_->setSceneRect(0, 0, ui->graphicsView_sensor_data->width(), ui->graphicsView_sensor_data->height());

        // Anslutningar
        connect(terminal_, SIGNAL(terminalClosing()), this, SLOT(closeTerminal()));
        connect(mc_, SIGNAL(log(QString)), this, SLOT(log(QString)));
        connect(mc_, SIGNAL(btConnected()), this, SLOT(btConnected()));
        connect(mc_, SIGNAL(btDisconnected()), this, SLOT(btDisconnected()));
        connect(mc_, SIGNAL(modeChanged(Control::Mode)), this, SLOT(setMode(Control::Mode)));
        connect(mc_, SIGNAL(controlSignalsChanged(ControlSignals)), this, SLOT(setControlGagues(ControlSignals)));
        connect(mc_, SIGNAL(sensorDataChanged(SensorData)), this, SLOT(setSensorValues(SensorData)));
        connect(plot_timer_, SIGNAL(timeout()), this, SLOT(drawPlots()));

        connect(ui->actionOpenTerminal, SIGNAL(triggered()), this, SLOT(openTerminal()));
        connect(ui->actionOpenPreferences, SIGNAL(triggered()), this, SLOT(openPreferences()));
        connect(ui->pushButton_toggle_connection, SIGNAL(clicked()), this, SLOT(toggleConnection()));
        connect(ui->actionAboutMazeterControl, SIGNAL(triggered()), this, SLOT(openAboutDialog()));
        connect(ui->pushButton_clear_plots, SIGNAL(clicked()), this, SLOT(clearPlots()));
        connect(ui->comboBox_sensor_data, SIGNAL(currentIndexChanged(int)), sd_scene_, SLOT(chosenDataChanged(int)));

        // Disabla de widgets som är beroende av en aktiv länk
        disableWidgets();
        clearPlots();

        // Visa overview
        ui->tabWidget->setCurrentIndex(0);

        // Statusmeddelande
        statusMessage("No active connection.");
    }

    MainWindow::~MainWindow()
    {
        delete mc_;
        delete terminal_;
        delete scene_;
        delete cs_scene_;

        // Alltid sist VIKTIGT!
        delete ui;
    }

    /* Fångar knapptryckningar */
    void MainWindow::keyPressEvent(QKeyEvent* event)
    {
        if (event->isAutoRepeat())
        {
            // Ignorera om återupprepat event eller om länken är nere.
            event->ignore();
        }
        else
        {
            switch (event->key())
            {
            case Qt::Key_section:
                openTerminal();
                break;

            default:
                mc_->handleKeyPressEvent(event);

                break;
            }

            // Tänd knappen
            scene_->buttonPressed(event);

            // Förhindra att eventet kaskadar vidare.
            event->accept();
        }
    }

    /* Fångar knappsläppningar */
    void MainWindow::keyReleaseEvent(QKeyEvent* event)
    {
        if (event->isAutoRepeat())
        {
            // Ignorera om återupprepat event eller om länken är nere.
            event->ignore();
        }
        else
        {
            // Vidarebefordra till MC
            mc_->handleKeyReleaseEvent(event);

            // Släck knappen
            scene_->buttonReleased(event);

            // Förhindra att eventet kaskadar vidare.
            event->accept();
        }
    }

    /*
     *  Private
     */

    /* Enablar alla widgets som kräver en aktiv länk */
    void MainWindow::enableWidgets()
    {
        ui->label_mode_status->setEnabled(true);
        ui->label_claw_status->setEnabled(true);
        ui->progressBar_left_engine_fwd->setEnabled(true);
        ui->progressBar_left_engine_rev->setEnabled(true);
        ui->progressBar_right_engine_fwd->setEnabled(true);
        ui->progressBar_right_engine_rev->setEnabled(true);
        ui->doubleSpinBox_kd->setEnabled(true);
        ui->doubleSpinBox_kp->setEnabled(true);
        ui->pushButton_transfer->setEnabled(true);
        ui->pushButton_calibrate->setEnabled(true);
    }

    /* Disablar alla widgets som kräver en aktiv länk */
    void MainWindow::disableWidgets()
    {
        ui->label_mode_status->setEnabled(false);
        ui->label_mode_status->setText("N/A");
        ui->label_claw_status->setEnabled(false);
        ui->label_claw_status->setText("N/A");
        ui->progressBar_left_engine_fwd->setEnabled(false);
        ui->progressBar_left_engine_fwd->setValue(0);
        ui->progressBar_left_engine_rev->setEnabled(false);
        ui->progressBar_left_engine_rev->setValue(0);
        ui->progressBar_right_engine_fwd->setEnabled(false);
        ui->progressBar_right_engine_fwd->setValue(0);
        ui->progressBar_right_engine_rev->setEnabled(false);
        ui->progressBar_right_engine_rev->setValue(0);
        ui->doubleSpinBox_kd->setEnabled(false);
        ui->doubleSpinBox_kd->setValue(0);
        ui->doubleSpinBox_kp->setEnabled(false);
        ui->doubleSpinBox_kp->setValue(0);
        ui->pushButton_transfer->setEnabled(false);
        ui->pushButton_calibrate->setEnabled(false);
    }

    /* Tillåter mjuk övergång mellan olika nivåer */
    void MainWindow::setRightEngineGauge(int value, char direction)
    {
        int right_fwd_value = ui->progressBar_right_engine_fwd->value();
        int right_rev_value = ui->progressBar_right_engine_rev->value();

        if (direction == 1)
        {
            // Ska framåt
            if (right_fwd_value > 0)
            {
                // Åker framåt
                if (value > right_fwd_value)
                {
                    // Ökar hastigheten
                    for (int i = 0; i < value - right_fwd_value; ++i)
                        ui->progressBar_right_engine_fwd->setValue(ui->progressBar_right_engine_fwd->value() + 1);
                }
                else
                {
                    // Minskar hastigheten
                    for (int i = 0; i < right_fwd_value - value; ++i)
                        ui->progressBar_right_engine_fwd->setValue(ui->progressBar_right_engine_fwd->value() - 1);
                }
            }
            else
            {
                // Åker bakåt
                for (int i = 0; i < right_rev_value; ++i)
                    ui->progressBar_right_engine_rev->setValue(ui->progressBar_right_engine_rev->value() - 1);

                for (int i = 0; i < value; ++i)
                    ui->progressBar_right_engine_fwd->setValue(ui->progressBar_right_engine_fwd->value() + 1);
            }
        }
        else
        {
            // Ska bakåt
            if (right_rev_value > 0)
            {
                // Åker bakåt
                if (value > right_rev_value)
                {
                    // Ökar hastigheten
                    for (int i = 0; i < value - right_rev_value; ++i)
                        ui->progressBar_right_engine_rev->setValue(ui->progressBar_right_engine_rev->value() + 1);
                }
                else
                {
                    // Minskar hastigheten
                    for (int i = 0; i < right_rev_value - value; ++i)
                        ui->progressBar_right_engine_rev->setValue(ui->progressBar_right_engine_rev->value() - 1);
                }
            }
            else
            {
                // Åker framåt
                for (int i = 0; i < right_fwd_value; ++i)
                    ui->progressBar_right_engine_fwd->setValue(ui->progressBar_right_engine_fwd->value() - 1);

                for (int i = 0; i < value; ++i)
                    ui->progressBar_right_engine_rev->setValue(ui->progressBar_right_engine_rev->value() + 1);
            }
        }
    }

    /* Tillåter mjuk övergång mellan olika nivåer */
    void MainWindow::setLeftEngineGauge(int value, char direction)
    {
        int left_fwd_value = ui->progressBar_left_engine_fwd->value();
        int left_rev_value = ui->progressBar_left_engine_rev->value();

        if (direction == 1)
        {
            // Ska framåt
            if (left_fwd_value > 0)
            {
                // Åker framåt
                if (value > left_fwd_value)
                {
                    // Ökar hastigheten
                    for (int i = 0; i < value - left_fwd_value; ++i)
                        ui->progressBar_left_engine_fwd->setValue(ui->progressBar_left_engine_fwd->value() + 1);
                }
                else
                {
                    // Minskar hastigheten
                    for (int i = 0; i < left_fwd_value - value; ++i)
                        ui->progressBar_left_engine_fwd->setValue(ui->progressBar_left_engine_fwd->value() - 1);
                }
            }
            else
            {
                // Åker bakåt
                for (int i = 0; i < left_rev_value; ++i)
                    ui->progressBar_left_engine_rev->setValue(ui->progressBar_left_engine_rev->value() - 1);

                for (int i = 0; i < value; ++i)
                    ui->progressBar_left_engine_fwd->setValue(ui->progressBar_left_engine_fwd->value() + 1);
            }
        }
        else
        {
            // Ska bakåt
            if (left_rev_value > 0)
            {
                // Åker bakåt
                if (value > left_rev_value)
                {
                    // Ökar hastigheten
                    for (int i = 0; i < value - left_rev_value; ++i)
                        ui->progressBar_left_engine_rev->setValue(ui->progressBar_left_engine_rev->value() + 1);
                }
                else
                {
                    // Minskar hastigheten
                    for (int i = 0; i < left_rev_value - value; ++i)
                        ui->progressBar_left_engine_rev->setValue(ui->progressBar_left_engine_rev->value() - 1);
                }
            }
            else
            {
                // Åker framåt
                for (int i = 0; i < left_fwd_value; ++i)
                    ui->progressBar_left_engine_fwd->setValue(ui->progressBar_left_engine_fwd->value() - 1);

                for (int i = 0; i < value; ++i)
                    ui->progressBar_left_engine_rev->setValue(ui->progressBar_left_engine_rev->value() + 1);
            }
        }
    }

    /* Skriv ett statusmeddelande */
    void MainWindow::statusMessage(const QString &str)
    {
        ui->statusbar->showMessage(str);
    }

    /*
     *  Slots
     */

    /* Slot för att tillåta skrivning från andra objekt */
    void MainWindow::log(const QString& str)
    {
        ui->textEdit_log->append(str);
    }

    /* Rensa fönstret */
    void MainWindow::clear()
    {
        ui->textEdit_log->clear();
    }

    /* Stänger terminalen */
    void MainWindow::closeTerminal()
    {
        terminal_->hide();
    }

    /* Öppnar AboutDialog */
    void MainWindow::openAboutDialog()
    {
        AboutDialog dlg;
        dlg.exec();
    }

    /* Blåtandslänk öppen */
    void MainWindow::btConnected()
    {
        enableWidgets();
        log("Connection established.");
        statusMessage("Connection established.");

        // Uppdatera knappen
        ui->pushButton_toggle_connection->setText("Disconnect");
        ui->pushButton_toggle_connection->setIcon(QIcon(":/icons/resources/stop.ico"));

        // Starta plot_timer_
        plot_timer_->start(PLOT_DELTA_T);
    }

    /* Blåtandslänk stängd */
    void MainWindow::btDisconnected()
    {
        disableWidgets();
        log("Connection closed.");
        statusMessage("No active connection.");

        // Uppdatera knappen
        ui->pushButton_toggle_connection->setText("Connect");
        ui->pushButton_toggle_connection->setIcon(QIcon(":/icons/resources/start.ico"));

        // Stanna plot_timer_
        plot_timer_->stop();
    }

    /* Uppdaterar mätarna för kontrollsignalerna */
    void MainWindow::setControlGagues(ControlSignals control_signals)
    {
        if (mc_->isConnected())
        {
            // Höger hjulpar
            setRightEngineGauge((int)control_signals.right_value, control_signals.right_direction);

            // Vänster hjulpar
            setLeftEngineGauge((int)control_signals.left_value, control_signals.left_direction);

            // Klo
            if (control_signals.claw_value == 0)
                ui->label_claw_status->setText("Closed");
            else
                ui->label_claw_status->setText("Open");

            // Sätt värden till KP och KD
            cs_scene_->newControlSignals(control_signals);
        }
    }

    /* Uppdaterar sensordatan som visas i scene_ */
    void MainWindow::setSensorValues(SensorData sensor_data)
    {
        scene_->updateSensorData(sensor_data);
    }

    /* Uppdaterar operationsläget */
    void MainWindow::setMode(Control::Mode mode)
    {
        if (mc_->isConnected())
        {
            switch (mode)
            {
            case Control::AUTO:
                ui->label_mode_status->setText("Auto");
                log("Mode changed to: Auto");
                break;

            case Control::MANUAL:
                ui->label_mode_status->setText("Manual");
                log("Mode changed to: Manual");
                break;

            default:
                break;
            }
        }
    }

    /* Öppnar terminalen */
    void MainWindow::openTerminal()
    {
        terminal_->show();
    }

    /* Öppnar preferences */
    void MainWindow::openPreferences()
    {
        PreferencesDialog dlg;
        dlg.setPortName(mc_->port()->portName());
        dlg.setBaudRate(mc_->port()->baudRate());
        dlg.setDataBits(mc_->port()->dataBits());
        dlg.setParity(mc_->port()->parity());
        dlg.setStopBits(mc_->port()->stopBits());

        if (dlg.exec() == QDialog::Accepted)
        {
            mc_->port()->setPortName(dlg.portName());
            mc_->port()->setBaudRate(dlg.baudRate());
            mc_->port()->setDataBits(dlg.dataBits());
            mc_->port()->setParity(dlg.parity());
            mc_->port()->setStopBits(dlg.stopBits());
        }
    }

    /* Togglar blåtandslänkens läge */
    void MainWindow::toggleConnection()
    {
        if (ui->pushButton_toggle_connection->text() == "Connect")
            // Upprätta anslutning
            mc_->parseCommand(UserInput("open"));
        else if (ui->pushButton_toggle_connection->text() == "Disconnect")
            // Bryt anslutning
            mc_->parseCommand(UserInput("close"));
    }

    /* Rensar plots */
    void MainWindow::clearPlots()
    {
        cs_scene_->clear();
        sd_scene_->clear();
    }

    /* Ritar en ny punkt */
    void MainWindow::drawPlots()
    {
        cs_scene_->draw();
        sd_scene_->draw();
    }
} // namespace MC
