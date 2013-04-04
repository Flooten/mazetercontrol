/*
 * FILNAMN:       terminal.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 *                Herman Ekwall
 * DATUM:         2013-04-03
 *
 * BESKRIVNING:   Terminal är den klass som innehåller det grafiska gränssnittet.
 *
 */

#ifndef MAZETERCONTROLDIALOG_H
#define MAZETERCONTROLDIALOG_H

#include "control.h"

#include <QDialog>
#include <QString>

namespace Ui
{
    class Terminal;
}

namespace MC
{
    class Terminal : public QDialog
    {
        Q_OBJECT

    public:
        explicit Terminal(QWidget *parent = 0);
        ~Terminal();

    private:
        // Variabler
        Ui::Terminal* ui;
        Control* mc;

        // Konstanter
        const QString INI_FILE = "mc.ini";

        // Funktioner

    public slots:
        void out(const QString& str);
        void clear();

    private slots:
        void handleCommand();

    };
} // namespace MC

#endif // MAZETERCONTROLDIALOG_H
