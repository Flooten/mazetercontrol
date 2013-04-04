/*
 * FILNAMN:       terminal.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 *                Herman Ekwall
 * DATUM:         2013-04-04
 *
 * BESKRIVNING:   Terminal är den klass som innehåller det grafiska gränssnittet.
 *
 */

#ifndef MAZETERCONTROLDIALOG_H
#define MAZETERCONTROLDIALOG_H

#include "control.h"

#include <QDialog>
#include <QString>
#include <QKeyEvent>

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
        int current_line = 0;
        QStringList history;

        // Konstanter
        const QString INI_FILE = "mc.ini";
        const QString HIST_FILE = ".mc_history";

        // Funktioner
        bool eventFilter(QObject* obj, QEvent *event);

    public slots:
        void out(const QString& str);
        void clear();

    private slots:
        void handleCommand();

    };
} // namespace MC

#endif // MAZETERCONTROLDIALOG_H
