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
#include "terminal.h"

#include <QDialog>
#include <QString>
#include <QKeyEvent>
#include <QCoreApplication>

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
        explicit Terminal(Control* control, QWidget *parent = 0);
        ~Terminal();

    private:
        Ui::Terminal* ui;
        Control* mc_;
        int current_line_ = 0;
        bool history_reset_ = true;
        QStringList history_;

        const QString HIST_FILE = QCoreApplication::applicationDirPath() + "/data/.mc_history";

        bool eventFilter(QObject* obj, QEvent *event);
        void clear();

    signals:
        void terminalClosing();
        void clearingTerminal();

    public slots:
        void out(const QString& str);

    private slots:
        void handleCommand();
        void resetCurrentLine();

    };
} // namespace MC

#endif // MAZETERCONTROLDIALOG_H
