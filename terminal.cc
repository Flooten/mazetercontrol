/*
 * FILNAMN:       terminal.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 *                Herman Ekwall
 * DATUM:         2013-04-03
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
        mc = new Control(INI_FILE);

        // Anslutningar
        connect(ui->lineEdit_command, SIGNAL(returnPressed()), this, SLOT(handleCommand()));
        connect(mc, SIGNAL(out(QString)), this, SLOT(out(QString)));
        connect(mc, SIGNAL(clear()), this, SLOT(clear()));

        // Historik
        QFile file(HIST_FILE);

        if(file.open(QFile::ReadOnly | QFile::Text))
        {
            while(!file.atEnd())
            {
                history.prepend(file.readLine().trimmed());
            }
        }


        ui->lineEdit_command->installEventFilter(this);

        // Skriv ut välkomstmeddelande
        mc->printWelcomeMessage();
    }

    Terminal::~Terminal()
    {
        delete ui;
    }

    /*
     *  Private
     */

    /*  */
    bool Terminal::eventFilter(QObject* obj, QEvent *event)
    {
        if (obj == ui->lineEdit_command)
        {
            if (event->type() == QEvent::KeyPress)
            {
                QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
                if (keyEvent->key() == Qt::Key_Up)
                {
                    current_line++;

                    if(current_line <= history.size())
                    {
                        QString line = history[current_line - 1];
                        ui->lineEdit_command->setText(line);
                    }
                    else
                        current_line = history.size() - 1;

                    return true;
                }
                else if(keyEvent->key() == Qt::Key_Down)
                {

                    if(current_line > 0)
                    {
                        current_line--;
                        QString line;
                        line.trimmed();

                        if (current_line != 0)
                            line = history[current_line - 1];

                        ui->lineEdit_command->setText(line);
                    }
                    else
                        current_line = 2;

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

            history.prepend(input_string);

            // Sparar kommando i historiken
            QFile file(HIST_FILE);

            if(file.open(QFile::Append | QFile::Text))
            {
                QTextStream stream(&file);
                stream << input_string << endl;
                file.close();
            }

            if (input.isValid())
                // Parsa kommandot
                mc->parseCommand(input);
            else
                ui->textEdit_history->append("Invalid command: " + input.commandString());
        }
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
