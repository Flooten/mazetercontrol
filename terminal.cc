/*
 * FILNAMN:       terminal.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 *                Herman Ekwall
 * DATUM:         2013-04-04
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
    Terminal::Terminal(QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::Terminal)
    {
        ui->setupUi(this);

        // Uppdatera fönsterrubriken
        setWindowTitle(windowTitle() + " " + utils::VERSION);

        // Skapa en instans av Control
        mc_ = new Control(INI_FILE);

        // Anslutningar
        connect(ui->lineEdit_command, SIGNAL(returnPressed()), this, SLOT(handleCommand()));
        connect(ui->lineEdit_command, SIGNAL(editingFinished()), this, SLOT(resetCurrentLine()));
        connect(mc_, SIGNAL(out(QString)), this, SLOT(out(QString)));
        connect(mc_, SIGNAL(clear()), this, SLOT(clear()));

        // Historik
        QFile file(HIST_FILE);

        if(file.open(QFile::ReadOnly | QFile::Text))
        {
            while(!file.atEnd())
            {
                history_.prepend(file.readLine().trimmed());
            }
        }

        ui->lineEdit_command->installEventFilter(this);

        // Skriv ut välkomstmeddelande
        mc_->printWelcomeMessage();
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
            if (event->type() == QEvent::KeyPress)
            {
                QKeyEvent* key_event = static_cast<QKeyEvent*>(event);

                if (key_event->key() == Qt::Key_Up)
                {
                    // Mot äldre kommandon

                    if (current_line_ != history_.size() - 1)
                    // Öka om ej sista elementet
                        ++current_line_;

                    QString line = history_[current_line_].trimmed();
                    ui->lineEdit_command->setText(line);

                    return true;
                }
                else if(key_event->key() == Qt::Key_Down)
                {
                    // Mot nyare kommandon

                    if (current_line_ != 0)
                        // Minska om ej sista elementet
                        --current_line_;

                    QString line = history_[current_line_].trimmed();
                    ui->lineEdit_command->setText(line);

                    return true;
                }
            }
            return false;
        }
        return QDialog::eventFilter(obj, event);
    }

    /*
     *  Slots
     */
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

            if(file.open(QFile::Append | QFile::Text))
            {
                QTextStream stream(&file);
                stream << input_string << endl;
                file.close();
            }
            else
            {
                emit out("Unable to open history file.");
            }

            mc_->parseCommand(input);
        }
    }

    /* Slot för att återställa current_line_ när fokus lämnar kommandoraden */
    void Terminal::resetCurrentLine()
    {
        current_line_ = 0;
    }

    /* Slot för att tillåta skrivning från andra objekt */
    void Terminal::out(const QString& str)
    {
        ui->textEdit_history->append(str);
    }

    /* Rensa fönstret */
    void Terminal::clear()
    {
        ui->textEdit_history->clear();
    }

} // namespace MC
