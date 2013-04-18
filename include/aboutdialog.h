/*
 * FILNAMN:       preferencesdialog.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-18
 *
 * BESKRIVNING:   AboutDialog representerar det dialogfönster som
 *                visar information om programmer för användaren.
 *
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui
{
    class AboutDialog;
}

namespace MC
{
    class AboutDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit AboutDialog(QWidget *parent = 0);
        ~AboutDialog();

    private:
        Ui::AboutDialog *ui;
    };
} // namespace MC
#endif // ABOUTDIALOG_H
