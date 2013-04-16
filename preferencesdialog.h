/*
 * FILNAMN:       preferencesdialog.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-16
 *
 * BESKRIVNING:   PreferencesDialog representerar det dialogfönster som
 *                tillåter användaren att göra ändringar i programvariabler.
 *
 */

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include "qextserialport.h"

#include <QDialog>
#include <QString>

namespace Ui
{
    class PreferencesDialog;
}

namespace MC
{
    class PreferencesDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit PreferencesDialog(QWidget *parent = 0);
        ~PreferencesDialog();

        // Getters
        QString portName() const;
        BaudRateType baudRate() const;
        DataBitsType dataBits() const;
        ParityType parity() const;
        StopBitsType stopBits() const;

        // Setters
        void setPortName(const QString& port_name);
        void setBaudRate(BaudRateType baud_rate);
        void setDataBits(DataBitsType data_bits);
        void setParity(ParityType parity);
        void setStopBits(StopBitsType stop_bits);

    private:
        Ui::PreferencesDialog* ui;
    };
} // namespace MC

#endif // PREFERENCESDIALOG_H
