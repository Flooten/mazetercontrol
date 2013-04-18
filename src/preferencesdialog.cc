/*
 * FILNAMN:       preferencesdialog.cc
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-16
 *
 */

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "utils.h"

namespace MC
{
    PreferencesDialog::PreferencesDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PreferencesDialog)
    {
        ui->setupUi(this);

        // Anslutningar
        connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
        connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    }


    PreferencesDialog::~PreferencesDialog()
    {
        delete ui;
    }

    /* Returnerar portnamnet */
    QString PreferencesDialog::portName() const
    {
        return ui->lineEdit_port_name->text();
    }

    /* Returnerar baudrate */
    BaudRateType PreferencesDialog::baudRate() const
    {
        return utils::toBaudRateType(ui->comboBox_baud_rate->currentText());
    }

    /* Returnerar antalet databitar */
    DataBitsType PreferencesDialog::dataBits() const
    {
        return utils::toDataBitsType(ui->comboBox_data_bits->currentText());
    }

    /* Returnerar pariteten */
    ParityType PreferencesDialog::parity() const
    {
        return utils::toParityType(ui->comboBox_parity->currentText());
    }

    /* Returnerar antalet stoppbitar */
    StopBitsType PreferencesDialog::stopBits() const
    {
        return utils::toStopBitsType(ui->comboBox_stop_bits->currentText());
    }

    /* Sätter portnamnet */
    void PreferencesDialog::setPortName(const QString& port_name)
    {
        ui->lineEdit_port_name->setText(port_name);
    }

    /* Sätter baudrate */
    void PreferencesDialog::setBaudRate(const BaudRateType baud_rate)
    {
        int index = ui->comboBox_baud_rate->findText(utils::toString(baud_rate));

        if (index != -1)
            ui->comboBox_baud_rate->setCurrentIndex(index);
        else
            ui->comboBox_baud_rate->setCurrentIndex(0);
    }

    /* Sätter antalet databitar */
    void PreferencesDialog::setDataBits(const DataBitsType data_bits)
    {
        int index = ui->comboBox_data_bits->findText(utils::toString(data_bits));

        if (index != -1)
            ui->comboBox_data_bits->setCurrentIndex(index);
        else
            ui->comboBox_data_bits->setCurrentIndex(0);
    }

    /* Sätter pariteten */
    void PreferencesDialog::setParity(const ParityType parity)
    {
        int index = ui->comboBox_parity->findText(utils::toString(parity));

        if (index != -1)
            ui->comboBox_parity->setCurrentIndex(index);
        else
            ui->comboBox_parity->setCurrentIndex(0);
    }

    /* Sätter antalet stoppbitar */
    void PreferencesDialog::setStopBits(const StopBitsType stop_bits)
    {
        int index = ui->comboBox_stop_bits->findText(utils::toString(stop_bits));

        if (index != -1)
            ui->comboBox_stop_bits->setCurrentIndex(index);
        else
            ui->comboBox_stop_bits->setCurrentIndex(0);
    }
} // namespace MC
