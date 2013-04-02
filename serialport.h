#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "qextserialport.h"
#include <QString>
#include <QObject>

namespace MC
{
    class SerialPort : public QObject
    {
        Q_OBJECT
    public:
        SerialPort(QObject* parent = nullptr);
        SerialPort(const QString& port_name, const QString& baud_rate, const QString& data_bits, const QString& parity, const QString& stop_bits);
        ~SerialPort();

        // Getters
        QString portName() const;
        BaudRateType baudRate() const;
        DataBitsType dataBits() const;
        ParityType parity() const;
        StopBitsType stopBits() const;
        QString state() const;

        // Setters
        void setPortName(const QString& port_name);
        void setBaudRate(BaudRateType baud_rate);
        void setDataBits(DataBitsType data_bits);
        void setParity(ParityType parity);
        void setStopBits(StopBitsType stop_bits);

        // Funktioner
        bool open();
        void close();

        bool isOpen() const;

        void transmit(const QString& msg);
        void transmit(const QByteArray& msg);

        QByteArray readAll() const;

        qint64 bytesAvailable() const;


    private:
        // Variabler
        QextSerialPort* port_;
        PortSettings port_settings_;
        const int TIMEOUT = 1000;

        // Funktioner
        void setupConnections();
        void parsePortSettings(const QString& baud_rate, const QString& data_bits, const QString& parity, const QString& stop_bits);

    signals:
        void readyRead();
        void bytesWritten(qint64 bytes_written);
        void out(const QString& str);

    private slots:
        void readyReadRelay();
        void bytesWrittenRelay(qint64 byte_count);
    };
} // namespace MC

#endif // SERIALPORT_H
