#ifndef UTILS_H
#define UTILS_H

#include "qextserialport.h"

#include <QString>
#include <QByteArray>

namespace MC
{
    namespace utils
    {
        /* Konstanter */
        const QString VERSION = "v0.1";

        // Konverteringsfunktioner portspecifikationer <->
        BaudRateType toBaudRateType(const QString& baud_rate);
        DataBitsType toDataBitsType(const QString& data_bits);
        ParityType toParityType(const QString& parity);
        StopBitsType toStopBitsType(const QString& stop_bits);

        QString toString(BaudRateType baud_rate);
        QString toString(DataBitsType data_bits);
        QString toString(ParityType parity);
        QString toString(StopBitsType stop_bits);

        // Stycka upp en QByteArray i ett mer läsligt format
        QString readableByteArray(const QByteArray& ba);
    } // namespace utils
} // namespace MC

#endif // UTILS_H
