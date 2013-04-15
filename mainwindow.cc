#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "controlsignals.h"

namespace MC
{
    /*
     *  Public
     */
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        this->setFocusPolicy(Qt::StrongFocus);
        this->setFocus();

        // Subscriptad text
        ui->label_kd->setText("K<sub>D</sub>");
        ui->label_kp->setText("K<sub>P</sub>");

        // Uppdatera fönsterrubrik
        setWindowTitle(windowTitle() + " " + utils::VERSION);

        // Skapa instanser av Control och Terminal
        mc_ = new Control(utils::INI_FILE);
        terminal_ = new Terminal(mc_, this);

        // Anslutningar
        connect(terminal_, SIGNAL(terminalClosing()), this, SLOT(closeTerminal()));
        connect(mc_, SIGNAL(log(QString)), this, SLOT(log(QString)));
        connect(mc_, SIGNAL(btConnected()), this, SLOT(btConnected()));
        connect(mc_, SIGNAL(btDisconnected()), this, SLOT(btDisconnected()));
        connect(mc_, SIGNAL(modeChanged(Control::Mode)), this, SLOT(setMode(Control::Mode)));
        connect(mc_, SIGNAL(controlSignalsChanged(ControlSignals)), this, SLOT(setEngineGagues(ControlSignals)));
        connect(ui->actionOpenTerminal, SIGNAL(triggered()), this, SLOT(openTerminal()));

        // Disabla de widgets som är beroende av en aktiv länk
        disableWidgets();

        // Statusmeddelande
        statusMessage("No active connection.");
    }

    MainWindow::~MainWindow()
    {
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
        ui->lineEdit_mode->setEnabled(true);
        ui->lineEdit_claw->setEnabled(true);
        ui->doubleSpinBox_speed->setEnabled(true);
        ui->progressBar_left_engine_fwd->setEnabled(true);
        ui->progressBar_left_engine_rev->setEnabled(true);
        ui->progressBar_right_engine_fwd->setEnabled(true);
        ui->progressBar_right_engine_rev->setEnabled(true);
        ui->pushButton_transfer->setEnabled(true);
        ui->pushButton_calibrate->setEnabled(true);
    }

    /* Disablar alla widgets som kräver en aktiv länk */
    void MainWindow::disableWidgets()
    {
        ui->lineEdit_mode->setEnabled(false);
        ui->lineEdit_mode->clear();
        ui->lineEdit_claw->setEnabled(false);
        ui->lineEdit_claw->clear();
        ui->doubleSpinBox_speed->setEnabled(false);
        ui->doubleSpinBox_speed->setValue(0);
        ui->progressBar_left_engine_fwd->setEnabled(false);
        ui->progressBar_left_engine_fwd->setValue(0);
        ui->progressBar_left_engine_rev->setEnabled(false);
        ui->progressBar_left_engine_rev->setValue(0);
        ui->progressBar_right_engine_fwd->setEnabled(false);
        ui->progressBar_right_engine_fwd->setValue(0);
        ui->progressBar_right_engine_rev->setEnabled(false);
        ui->progressBar_right_engine_rev->setValue(0);
        ui->pushButton_transfer->setEnabled(false);
        ui->pushButton_calibrate->setEnabled(false);
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

    void MainWindow::closeTerminal()
    {
        terminal_->hide();
    }

    /* Blåtandslänk öppen */
    void MainWindow::btConnected()
    {
        enableWidgets();
        log("Connection established.");
        statusMessage("Connection established.");
    }

    /* Blåtandslänk stängd */
    void MainWindow::btDisconnected()
    {
        disableWidgets();
        log("Connection closed.");
        statusMessage("No active connection.");
    }

    /* Uppdaterar mätarna för kontrollsignalerna */
    void MainWindow::setEngineGagues(ControlSignals control_signals)
    {
        if (mc_->isConnected())
        {
            // Höger hjulpar
            if (control_signals.right_direction == 1)
            {
                // Kör framåt
                ui->progressBar_right_engine_fwd->setValue((int)control_signals.right_value);
                ui->progressBar_right_engine_rev->setValue(0);
            }
            else
            {
                // Kör bakåt
                ui->progressBar_right_engine_fwd->setValue(0);
                ui->progressBar_right_engine_rev->setValue((int)control_signals.right_value);
            }

            // Vänster hjulpar
            if (control_signals.left_direction == 1)
            {
                // Kör framåt
                ui->progressBar_left_engine_fwd->setValue((int)control_signals.left_value);
                ui->progressBar_left_engine_rev->setValue(0);
            }
            else
            {
                // Kör bakåt
                ui->progressBar_left_engine_fwd->setValue(0);
                ui->progressBar_left_engine_rev->setValue((int)control_signals.left_value);
            }

            if (control_signals.claw_value == 0)
                ui->lineEdit_claw->setText("Closed");
            else
                ui->lineEdit_claw->setText("Open");
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
                ui->lineEdit_mode->setText("Auto");
                break;

            case Control::MANUAL:
                ui->lineEdit_mode->setText("Manual");
                break;

            default:
                break;
            }
        }
    }

    /* Öppna terminalen */
    void MainWindow::openTerminal()
    {
        terminal_->show();
    }

} // namespace MC
