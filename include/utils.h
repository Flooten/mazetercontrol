/*
 * FILNAMN:       utils.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-03
 *
 * BESKRIVNING:   Definierar stödfunktioner som används av fler
 *                än en klass.
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include "qextserialport.h"

#include <QString>
#include <QByteArray>
#include <QCoreApplication>

namespace MC
{
    /* Konstanter */
    const QString VERSION = "1.0";
    const int SENSOR_DATA_SIZE = 11;
    const int CONTROL_SIGNALS_SIZE = 5;

    namespace utils
    {
        // Konverteringsfunktioner portspecifikationer <->
        BaudRateType toBaudRateType(const QString& baud_rate);
        DataBitsType toDataBitsType(const QString& data_bits);
        ParityType toParityType(const QString& parity);
        StopBitsType toStopBitsType(const QString& stop_bits);

        QString toString(BaudRateType baud_rate);
        QString toString(DataBitsType data_bits);
        QString toString(ParityType parity);
        QString toString(StopBitsType stop_bits);
        QString toString(const bool logic);

        // Stycka upp en QByteArray i ett mer läsligt format
        QString readableByteArray(const QByteArray& ba);
    } // namespace utils
} // namespace MC

#endif // UTILS_H
