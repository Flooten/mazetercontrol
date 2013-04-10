/*
 * FILNAMN:       terminal.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 *                Herman Ekwall
 * DATUM:         2013-04-05
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
        Control* mc_;
        int current_line_ = 0;
        bool history_reset_ = true;
        QStringList history_;

        // Konstanter
        const QString INI_FILE = "mc.ini";
        const QString HIST_FILE = ".mc_history";

        // Funktioner
        bool eventFilter(QObject* obj, QEvent *event);
        void keyPressEvent(QKeyEvent *event);

    public slots:
        void out(const QString& str);
        void clear();

    private slots:
        void handleCommand();
        void resetCurrentLine();

    };
} // namespace MC

#endif // MAZETERCONTROLDIALOG_H
