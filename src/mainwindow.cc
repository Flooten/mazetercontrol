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
#include "commandtable.h"

namespace MC
{
    /*
     *  Public
     */
    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , plot_timer_(new QTimer(this))
        , calibrate_countdown_timer_(new QTimer(this))
        , scene_(new MCGraphicsScene(this))
    {
        ui->setupUi(this);

        mc_ = new Control(INI_FILE);
        terminal_ = new Terminal(mc_, this);
        cs_scene_ = new ControlSignalsPlotScene(PLOT_VIEW_WIDTH, PLOT_VIEW_HEIGHT, this);
        sd_scene_ = new SensorDataPlotScene(PLOT_VIEW_WIDTH, PLOT_VIEW_HEIGHT, this);

        this->setFocusPolicy(Qt::StrongFocus);
        this->setFocus();

        // Subscriptad text
        ui->label_kd->setText("K<sub>D</sub>");
        ui->label_kp->setText("K<sub>P</sub>");

        // Uppdatera fönsterrubrik
        setWindowTitle(windowTitle() + " v" + VERSION);

        // Installera scener i GraphicsView
        ui->graphicsView_overview->setScene(scene_);
        ui->graphicsView_control_signals->setScene(cs_scene_);
        ui->graphicsView_sensor_data->setScene(sd_scene_);


        // Generella anslutningar
        connect(terminal_, SIGNAL(terminalClosing()), this, SLOT(closeTerminal()));
        connect(mc_, SIGNAL(log(QString)), this, SLOT(log(QString)));
        connect(mc_, SIGNAL(btConnected()), this, SLOT(btConnected()));
        connect(mc_, SIGNAL(btDisconnected()), this, SLOT(btDisconnected()));
        connect(plot_timer_, SIGNAL(timeout()), this, SLOT(drawPlots()));
        connect(calibrate_countdown_timer_, SIGNAL(timeout()), this, SLOT(calibrateCountdown()));

        // Relä av nyligen inkommen data
        connect(mc_, SIGNAL(modeChanged(Control::Mode)), this, SLOT(setMode(Control::Mode)));
        connect(mc_, SIGNAL(controlSignalsChanged(ControlSignals)), this, SLOT(setControlGagues(ControlSignals)));
        connect(mc_, SIGNAL(controlSignalsChanged(ControlSignals)), cs_scene_, SLOT(newControlSignals(ControlSignals)));
        connect(mc_, SIGNAL(sensorDataChanged(SensorData)), this, SLOT(setSensorValues(SensorData)));
        connect(mc_, SIGNAL(sensorDataChanged(SensorData)), sd_scene_, SLOT(newSensorData(SensorData)));
        connect(mc_, SIGNAL(throttleValueChanged(char)), this, SLOT(throttleValueChanged(char)));

        // Ui-händelser: knapptryckningar och actions
        connect(ui->actionOpenTerminal, SIGNAL(triggered()), this, SLOT(openTerminal()));
        connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(openPreferences()));
        connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
        connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(toggleConnection()));
        connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(toggleConnection()));
        connect(ui->pushButton_toggle_connection, SIGNAL(clicked()), this, SLOT(toggleConnection()));
        connect(ui->pushButton_calibrate, SIGNAL(clicked()), this, SLOT(transmitCalibrateSensor()));
        connect(ui->actionAboutMazeterControl, SIGNAL(triggered()), this, SLOT(openAboutDialog()));
        connect(ui->pushButton_clear_plots, SIGNAL(clicked()), this, SLOT(resetPlots()));
        connect(ui->comboBox_sensor_data, SIGNAL(currentIndexChanged(int)), this, SLOT(chosenSensorDataChanged(int)));
        connect(ui->pushButton_transfer_parameters, SIGNAL(clicked()), this, SLOT(transmitParameters()));
        connect(ui->verticalSlider_throttle, SIGNAL(sliderReleased()), this, SLOT(throttleRelay()));

        // Plotcentreringssignaler
        connect(sd_scene_, SIGNAL(center(int)), this, SLOT(centerSensorDataPlot(int)));
        connect(cs_scene_, SIGNAL(center(int)), this, SLOT(centerControlSignalsPlot(int)));

        // Disabla de widgets som är beroende av en aktiv länk
        disableWidgets();

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
        delete sd_scene_;

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
     *  Public slots
     */

    /* Slot för att tillåta skrivning från andra objekt */
    void MainWindow::log(const QString& str)
    {
        ui->textEdit_log->append(str);
    }

    /* Rensa event log */
    void MainWindow::clearLog()
    {
        ui->textEdit_log->clear();
    }

    /* Blåtandslänk öppen */
    void MainWindow::btConnected()
    {
        enableWidgets();
        log("Connection established.");
        statusMessage("Connection established.");

        // Uppdatera knappen och actions
        ui->pushButton_toggle_connection->setText("Disconnect");
        ui->pushButton_toggle_connection->setIcon(QIcon(":/icons/resources/stop.ico"));
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);

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
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);

        // Stanna plot_timer_
        plot_timer_->stop();

        // Rensa plottar
        sd_scene_->clear();
        cs_scene_->clear();
    }

    /* Uppdaterar mätarna för kontrollsignalerna */
    void MainWindow::setControlGagues(ControlSignals control_signals)
    {
        if (mc_->isConnected())
        {
            // Höger hjulpar
            setRightEngineGauge((unsigned char)control_signals.right_value, (char)control_signals.right_direction);

            // Vänster hjulpar
            setLeftEngineGauge((unsigned char)control_signals.left_value, (char)control_signals.left_direction);

            // Klo
            if (control_signals.claw_value == 0)
                ui->label_claw_status->setText("Closed");
            else
                ui->label_claw_status->setText("Open");

            // Sätt värden till KP och KD
        }
    }

    /* Uppdaterar sensordatan som visas i scene_ */
    void MainWindow::setSensorValues(SensorData sensor_data)
    {
        scene_->updateSensorData(sensor_data);

        // Fånga ny data och sätt spinbox vid sd-plot till det senaste värdet.
        ui->spinBox_sensor_data_current_value->setSuffix(" cm");
        switch (ui->comboBox_sensor_data->currentIndex())
        {
        case 0:
            // Front left
            ui->spinBox_sensor_data_current_value->setValue(sensor_data.distance1);
            break;

        case 1:
            ui->spinBox_sensor_data_current_value->setValue(sensor_data.distance2);
            break;

        case 2:
            ui->spinBox_sensor_data_current_value->setValue(sensor_data.distance3);
            break;

        case 3:
            ui->spinBox_sensor_data_current_value->setValue(sensor_data.distance4);
            break;

        case 4:
            ui->spinBox_sensor_data_current_value->setValue(sensor_data.distance5);
            break;

        case 5:
            ui->spinBox_sensor_data_current_value->setValue(sensor_data.distance6);
            break;

        case 6:
            ui->spinBox_sensor_data_current_value->setValue(sensor_data.distance7);
            break;

        case 7:
            ui->spinBox_sensor_data_current_value->setValue(sensor_data.angle);
            ui->spinBox_sensor_data_current_value->setSuffix(" degrees");
            break;

        case 8:
            ui->spinBox_sensor_data_current_value->setValue(sensor_data.line_deviation);
            break;
        }
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
        ui->spinBox_kd_left->setEnabled(true);
        ui->spinBox_kp_left->setEnabled(true);
        ui->spinBox_kd_right->setEnabled(true);
        ui->spinBox_kp_right->setEnabled(true);
        ui->pushButton_transfer_parameters->setEnabled(true);
        ui->pushButton_calibrate->setEnabled(true);
        ui->graphicsView_overview->setEnabled(true);
        ui->verticalSlider_throttle->setEnabled(true);
        ui->spinBox_throttle->setEnabled(true);
        ui->pushButton_clear_plots->setEnabled(true);
        ui->graphicsView_control_signals->setEnabled(true);
        ui->graphicsView_sensor_data->setEnabled(true);
        ui->comboBox_sensor_data->setEnabled(true);

        scene_->draw();
        resetPlots();
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
        ui->spinBox_kd_left->setEnabled(false);
        ui->spinBox_kd_left->setValue(0);
        ui->spinBox_kp_left->setEnabled(false);
        ui->spinBox_kp_left->setValue(0);
        ui->spinBox_kd_right->setEnabled(false);
        ui->spinBox_kd_right->setValue(0);
        ui->spinBox_kp_right->setEnabled(false);
        ui->spinBox_kp_right->setValue(0);
        ui->spinBox_throttle->setEnabled(false);
        ui->spinBox_throttle->setValue(0);
        ui->verticalSlider_throttle->setEnabled(false);
        ui->pushButton_transfer_parameters->setEnabled(false);
        ui->pushButton_calibrate->setEnabled(false);
        ui->pushButton_clear_plots->setEnabled(false);
        ui->graphicsView_control_signals->setEnabled(false);
        ui->graphicsView_overview->setEnabled(false);
        ui->graphicsView_sensor_data->setEnabled(false);
        ui->comboBox_sensor_data->setEnabled(false);

        clearPlots();
        scene_->clear();
    }

    /* Tillåter mjuk övergång mellan olika nivåer */
    void MainWindow::setRightEngineGauge(unsigned char value, char direction)
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
    void MainWindow::setLeftEngineGauge(unsigned char value, char direction)
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

    /* Rensar plots */
    void MainWindow::clearPlots()
    {
        cs_scene_->clear();
        ui->graphicsView_control_signals->centerOn(0, ui->graphicsView_control_signals->height() / 2);

        sd_scene_->clear();
        ui->graphicsView_sensor_data->centerOn(0, ui->graphicsView_sensor_data->height() / 2);
    }

    /* Ritar stödlinjer */
    void MainWindow::drawPlotGrid()
    {
        cs_scene_->drawGrid();
        sd_scene_->drawGrid();
    }

    /* Skriver portinställningar till ini-filen */
    void MainWindow::writePreferences()
    {
        QFile ini_file(INI_FILE);

        if (!ini_file.open(QFile::ReadOnly))
        {
            log("Error: unable to open ini-file to read port preferences.");
            return;
        }

        QTextStream stream(&ini_file);

        QString complete_file = stream.readAll();

        ini_file.close();

        QStringList lines(complete_file.split("\n"));

        QMutableStringListIterator itr(lines);

        while (itr.hasNext())
        {
            QString line = itr.next();

            if (line.startsWith("port_name"))
            {
                itr.remove();
                itr.insert("port_name=" + mc_->port()->portName());
            }
            else if (line.startsWith("baud_rate"))
            {
                itr.remove();
                itr.insert("baud_rate=" + utils::toString(mc_->port()->baudRate()));
            }
            else if (line.startsWith("data_bits"))
            {
                itr.remove();
                itr.insert("data_bits=" + utils::toString(mc_->port()->dataBits()));
            }
            else if (line.startsWith("parity"))
            {
                itr.remove();
                itr.insert("parity=" + utils::toString(mc_->port()->parity()));
            }
            else if (line.startsWith("stop_bits"))
            {
                itr.remove();
                itr.insert("stop_bits=" + utils::toString(mc_->port()->stopBits()));
            }
        }

        if (!ini_file.open(QFile::WriteOnly | QFile::Truncate))
        {
            log("Error: unable to open ini-file to save port preferences.");
            return;
        }

        stream.setDevice(&ini_file);

        stream << lines.join("\n");

        ini_file.close();
    }

    /*
     *  Private slots
     */

    /* Öppnar terminalen */
    void MainWindow::openTerminal()
    {
        terminal_->show();
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

            writePreferences();
        }
    }

    /* Togglar blåtandslänkens läge */
    void MainWindow::toggleConnection()
    {
        if (!mc_->isConnected())
            // Upprätta anslutning
            mc_->parseCommand(UserInput("open"));
        else
            // Bryt anslutning
            mc_->parseCommand(UserInput("close"));
    }

    /* Nollställer plots */
    void MainWindow::resetPlots()
    {
        clearPlots();
        drawPlotGrid();
    }

    /* Ritar en ny punkt */
    void MainWindow::drawPlots()
    {
        cs_scene_->draw();
        sd_scene_->draw();
    }

    /* Skiftar innehållet i plotfönstret för kontrollsignalerna till att visa ny data */
    void MainWindow::centerControlSignalsPlot(int time)
    {
        ui->graphicsView_control_signals->centerOn(time - ui->graphicsView_control_signals->width() / 2, ui->graphicsView_control_signals->height() / 2);
    }

    /* Skiftar innehållet i plotfönstret för sensordatan till att visa ny data */
    void MainWindow::centerSensorDataPlot(int time)
    {
        ui->graphicsView_sensor_data->centerOn(time - ui->graphicsView_sensor_data->width() / 2, ui->graphicsView_sensor_data->height() / 2);
    }

    /* Centrerar fönstret vid byte av signal */
    void MainWindow::chosenSensorDataChanged(int index)
    {
        ui->graphicsView_sensor_data->centerOn(0, ui->graphicsView_sensor_data->height() / 2);

        sd_scene_->chosenDataChanged(index);
    }

    /* Skicka kalibreringsmeddelande */
    void MainWindow::transmitCalibrateSensor()
    {
        mc_->parseCommand(UserInput("transmit " + QString::number(CALIBRATE_LINE_SENSOR, 16) + "00"));
        calibrateCountdown();
    }

    /* Skickar de angivna parametervärdena */
    void MainWindow::transmitParameters()
    {        
        // Hämta värden och skapa representativa strängar
        // uppdelade i LSB och MSB, data skickas little-endian.

        QString size_str = "02";

        int parameters[4];

        parameters[0] = ui->spinBox_kd_left->value();
        parameters[1] = ui->spinBox_kd_right->value();
        parameters[2] = ui->spinBox_kp_left->value();
        parameters[3] = ui->spinBox_kp_right->value();

        for (int i = 0; i < 4; ++i)
        {
            int parameters_lsb = parameters[i] & 0xff;
            int parameters_msb = (parameters[i] & 0xff00) >> 8;

            QString parameters_str_lsb = QString::number(parameters_lsb, 16);
            QString parameters_str_msb = QString::number(parameters_msb, 16);

            if (parameters_lsb <= 0xf)
                parameters_str_lsb.prepend("0");

            if (parameters_msb <= 0xf)
                parameters_str_msb.prepend("0");

            int command_code;
            QString log_message = "Transmitting parameter: ";

            // Avgör kommandokod
            switch (i)
            {
            case 0:
                command_code = PARA_KD_LEFT;
                log_message.append("Kd left.");
                break;
            case 1:
                command_code = PARA_KD_RIGHT;
                log_message.append("Kd right.");
                break;
            case 2:
                command_code = PARA_KP_LEFT;
                log_message.append("Kp left.");
                break;
            case 3:
                command_code = PARA_KP_RIGHT;
                log_message.append("Kp right.");
                break;
            }

            log(log_message);

            // Sätt samman och skicka kommando
            mc_->parseCommand(UserInput("transmit " +
                                        QString::number(command_code, 16) +
                                        size_str +
                                        parameters_str_lsb +
                                        parameters_str_msb));
        }
    }

    /* Avslutar MC */
    void MainWindow::exitApplication()
    {
        exit(0);
    }

    /* Räknar ner från 5 */
    void MainWindow::calibrateCountdown()
    {
        if (calibrate_countdown_ == 0)
        {
            calibrate_countdown_timer_->stop();
            ui->pushButton_calibrate->setEnabled(true);
            ui->pushButton_calibrate->setText("Calibrate line sensor");
            calibrate_countdown_ = 5;
        }
        else
        {
            calibrate_countdown_timer_->start(1000);
            ui->pushButton_calibrate->setEnabled(false);
            ui->pushButton_calibrate->setText(QString::number(calibrate_countdown_));
            setFocus();
            --calibrate_countdown_;
        }
    }

    /* Tillåter throttleändring via vertical slider */
    void MainWindow::throttleRelay()
    {
        mc_->setThrottleValue(ui->verticalSlider_throttle->value());
    }

    void MainWindow::throttleValueChanged(char throttle_value)
    {
        ui->verticalSlider_throttle->setValue((int)throttle_value);
    }
} // namespace MC
