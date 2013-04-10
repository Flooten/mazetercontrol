/*
 * FILNAMN:       control.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 *                Herman Ekwall
 * DATUM:         2013-04-04
 *
 * BESKRIVNING:   Control tillhandahåller all interface mot serieporten
 *                från terminalen.
 *
 */

#ifndef MAZETERCONTROL_H
#define MAZETERCONTROL_H

#include "serialport.h"
#include "userinput.h"

#include <QObject>
#include <QString>
#include <QMap>
#include <QTextStream>
#include <QKeyEvent>
#include <QByteArray>

namespace MC
{
    class Control : public QObject
    {
        Q_OBJECT
    public:
        explicit Control(const QString& ini_file, QObject *parent = 0);

        void parseCommand(const UserInput& input);
        void handleKeyPressEvent(const QKeyEvent* event);
        void printWelcomeMessage();

    private:
        // Variabler
        bool firefly_config_mode_ = false;

        QString welcome_message_;
        QString help_message_;
        QMap<QString, QString> help_texts_;

        SerialPort* port_;

        QByteArray data_;
        int received_byte_count_ = 0;

        // Funktioner
        void transmit(int command);
        void parseIniFile(const QString& ini_file);
        void readLine(QString& str, QString& command, QString& argument);
        void readAssignArgument(const QString& argument, QString& name, QString& value);
        QString readUntilEnd(QTextStream& is);

    signals:
        void out(const QString&);
        void clear();

    public slots:
        void readData();
        void reportWrite(qint64 bytes_written);
    };
} // namespace MC

#endif // MAZETERCONTROL_H
