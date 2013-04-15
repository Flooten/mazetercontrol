#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

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

        // Nollställ mätare
        ui->progressBar_left_engine->setValue(0);
        ui->progressBar_right_engine->setValue(0);

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
            // Ignorera eventet om återupprepat event.
            event->ignore();
        }
        else
        {
            switch (event->key())
            {
            case Qt::Key_section:
                terminal_->show();
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
            // Ignorera eventet om återupprepat event.
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

    void MainWindow::enableWidgets()
    {
        ui->lineEdit_mode->setEnabled(true);
        ui->lineEdit_claw->setEnabled(true);
        ui->progressBar_left_engine->setEnabled(true);
        ui->progressBar_right_engine->setEnabled(true);
    }

    void MainWindow::disableWidgets()
    {
        ui->lineEdit_mode->setEnabled(false);
        ui->lineEdit_claw->setEnabled(false);
        ui->progressBar_left_engine->setEnabled(false);
        ui->progressBar_right_engine->setEnabled(false);
    }

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

    void MainWindow::btConnected()
    {
        // Blåtandslänk öppen
        enableWidgets();
        statusMessage(CONNECTION_OPEN);
    }

    void MainWindow::btDisconnected()
    {
        // Blåtandslänk stängd
        disableWidgets();
        statusMessage(CONNECTION_CLOSED);
    }

} // namespace MC
