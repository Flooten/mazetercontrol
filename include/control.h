/*
 * FILNAMN:       control.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 *                Herman Ekwall
 * DATUM:         2013-05-07
 *
 * BESKRIVNING:   Control tillhandahåller all interface mot serieporten
 *                från terminalen.
 *
 */

#ifndef CONTROL_H
#define CONTROL_H

#include "serialport.h"
#include "userinput.h"
#include "controlsignals.h"
#include "sensordata.h"
#include "xmlcontrol.h"

#include <QObject>
#include <QString>
#include <QMap>
#include <QTextStream>
#include <QKeyEvent>
#include <QByteArray>
#include <QTimer>
#include <QCoreApplication>

namespace MC
{
    class Control : public QObject
    {
        Q_OBJECT
    public:
        enum Mode
        {
            AUTO,
            MANUAL,
            NO_MODE
        };        

        enum Algorithm
        {
            NO_ALGORITHM,
            ALGO_IN,
            ALGO_OUT,
            ALGO_GOAL,
            ALGO_GOAL_REVERSE,
            ALGO_DONE
        };

        explicit Control(const QString& ini_file, QObject *parent = 0);

        ~Control();

        void setThrottleValue(int throttle_value);
        void parseCommand(const UserInput& input);
        void handleKeyPressEvent(QKeyEvent *event);
        void handleKeyReleaseEvent(QKeyEvent* event);

        bool isConnected() const;
        SerialPort* port() const;

    public slots:
        void readData();
        void reportWrite(qint64 bytes_written);

    private:
        enum TurnType
        {
            INVALID,
            LEFT_TURN,
            RIGHT_TURN,
            STRAIGHT
        };

        bool firefly_config_mode_ = false;
        bool bt_connected_ = false;
        bool key_up_pressed_ = false;
        bool key_down_pressed_ = false;

        QString help_message_;
        QMap<QString, QString> help_texts_;
        const QString PORT_SETTINGS_ = QCoreApplication::applicationDirPath() + "/data/port_settings.xml";

        XmlControl* port_settings_;
        XmlControl* ini_file_;

        QByteArray data_;
        QByteArray acknowledge_message_;

        SerialPort* port_;

        char throttle_value_ = 50;
        const char THROTTLE_INCREMENT_ = 5;
        ControlSignals control_signals_;
        SensorData sensor_data_;
        Mode mode_ = NO_MODE;
        Algorithm algorithm_ = NO_ALGORITHM;
        TurnType last_turn_ = INVALID;

        void transmitCommand(char command, char size = 0, char* data = 0);
        void increaseThrottle();
        void decreaseThrottle();
        void parseMessage(const QByteArray &data);
        void updateControlSignals(const QByteArray& control_signals_data);
        void updateSensorData(const QByteArray& sensor_data);
        void printData(QByteArray data);

    private slots:
        void portNameChanged(QString port_name);
        void baudRateChanged(QString baud_rate);
        void dataBitsChanged(QString data_bits);
        void parityChanged(QString parity);
        void stopBitsChanged(QString stop_bits);

    signals:
        void out(const QString&);
        void log(const QString&);
        void clear();
        void btConnected();
        void btDisconnected();
        void modeChanged(Control::Mode);
        void algorithmChanged(Control::Algorithm);
        void startAutonomousRun();
        void controlSignalsChanged(ControlSignals);
        void sensorDataChanged(SensorData);
        void throttleValueChanged(char);
    };
} // namespace MC

#endif // CONTROL_H
