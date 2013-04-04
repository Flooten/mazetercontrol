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
