/*
 * FILNAMN:       terminal.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 *                Herman Ekwall
 * DATUM:         2013-04-04
 * DATUM:         2013-04-05
 *
 */

#include "terminal.h"
#include "ui_terminal.h"
#include "utils.h"
#include "userinput.h"

#include <QTextStream>
#include <QFile>

namespace MC
{
    /*
     *  Public
     */
    Terminal::Terminal(Control* control, QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::Terminal)
        , mc_(control)
    {
        ui->setupUi(this);

        // Anslutningar
        connect(ui->lineEdit_command, SIGNAL(returnPressed()), this, SLOT(handleCommand()));
        connect(ui->lineEdit_command, SIGNAL(editingFinished()), this, SLOT(resetCurrentLine()));
        connect(mc_, SIGNAL(out(QString)), this, SLOT(out(QString)));

        // Historik
        // Läs in historikfilen
        QFile file(HIST_FILE);

        if(file.open(QFile::ReadOnly | QFile::Text))
        {
            while(!file.atEnd())
            {
                history_.prepend(file.readLine().trimmed());
            }
        }

        file.close();

        ui->lineEdit_command->installEventFilter(this);
    }

    Terminal::~Terminal()
    {
        delete ui;
    }

    /*
     *  Private
     */

    /* Filter som fångar knapptryckningar för kommandoraden */
    bool Terminal::eventFilter(QObject* obj, QEvent *event)
    {
        if (obj == ui->lineEdit_command)
        {
            switch (event->type())
            {
            case QEvent::KeyPress:
            {
                QKeyEvent* key_event = static_cast<QKeyEvent*>(event);

                switch (key_event->key())
                {
                case Qt::Key_Up:
                {
                    // Mot äldre kommandon

                    if (history_reset_)
                    {
                        history_reset_ = false;
                    }
                    else
                    {
                        if (current_line_ != history_.size() - 1)
                        // Öka om ej sista elementet
                            ++current_line_;
                    }

                    QString line = history_[current_line_].trimmed();
                    ui->lineEdit_command->setText(line);

                    return true;
                }

                case Qt::Key_Down:
                {
                    // Mot nyare kommandon

                    if (current_line_ != 0)
                        // Minska om ej sista elementet
                        --current_line_;

                    QString line = history_[current_line_].trimmed();
                    ui->lineEdit_command->setText(line);

                    return true;
                }

                default:
                    // Hantera inte eventet.
                    return false;
                }
            }
            case QEvent::KeyRelease:
                event->accept();
                return true;

            default:
                return false;
            }
        }
        return QDialog::eventFilter(obj, event);
    }

    /* Rensa fönstret */
    void Terminal::clear()
    {
        ui->textEdit_history->clear();
    }

    /*
     *  Slots
     */
    /* Hanterar ett nytt kommando */
    void Terminal::handleCommand()
    {
        // Hämta kommando
        QString input_string = ui->lineEdit_command->text();

        // Rensa kommandoraden
        ui->lineEdit_command->clear();

        if (!input_string.isEmpty())
        {
            UserInput input(input_string);

            history_.prepend(input_string);

            // Sparar kommando i historiken
            QFile file(HIST_FILE);

            if(file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
            {
                QTextStream stream(&file);
                stream << input_string << endl;
                file.close();
            }
            else
            {
                emit out("Unable to open history file.");
            }

            // Fånga fall som utförs av Terminal
            switch (input.command())
            {
            case UserInput::EXIT:
                emit terminalClosing();
                break;

            case UserInput::CLEAR:
                clear();
                break;

            default:
                mc_->parseCommand(input);
            }
        }
    }

    /* Slot för att återställa current_line_ när fokus lämnar kommandoraden */
    void Terminal::resetCurrentLine()
    {
        current_line_ = 0;
        history_reset_ = true;
    }

    /* Slot för att tillåta skrivning från andra objekt */
    void Terminal::out(const QString& str)
    {
        ui->textEdit_history->append(str);
    }
} // namespace MC
