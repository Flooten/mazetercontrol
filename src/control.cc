/*
 * FILNAMN:       control.cc
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 *                Herman Ekwall
 * DATUM:         2013-04-10
 *
 */

#include "control.h"
#include "utils.h"
#include "commandtable.h"

#include <QStringList>
#include <QFile>

namespace MC
{
    /*
     *  Public
     */
    Control::Control(const QString& ini_file, QObject* parent)
        : QObject(parent)
        , port_settings_(new XmlControl(PORT_SETTINGS_))
        , ini_file_(new XmlControl(ini_file))
    {        
        port_ = new SerialPort(port_settings_->attributeValue("port-name", "value"),
                               port_settings_->attributeValue("baud-rate", "value"),
                               port_settings_->attributeValue("data-bits", "value"),
                               port_settings_->attributeValue("parity", "value"),
                               port_settings_->attributeValue("stop-bits", "value"));

        // Skapa ett acknowledge-meddelade att lyssna efter.
        acknowledge_message_.resize(2);
        acknowledge_message_[0] = BT_CONNECT;
        acknowledge_message_[1] = 0x0;

        // Anslutningar
        connect(port_, SIGNAL(readyRead()), this, SLOT(readData()));
        connect(port_, SIGNAL(bytesWritten(qint64)), this, SLOT(reportWrite(qint64)));
        connect(port_, SIGNAL(portNameChanged(QString)), this, SLOT(portNameChanged(QString)));
        connect(port_, SIGNAL(baudRateChanged(QString)), this, SLOT(baudRateChanged(QString)));
        connect(port_, SIGNAL(dataBitsChanged(QString)), this, SLOT(dataBitsChanged(QString)));
        connect(port_, SIGNAL(parityChanged(QString)), this, SLOT(parityChanged(QString)));
        connect(port_, SIGNAL(stopBitsChanged(QString)), this, SLOT(stopBitsChanged(QString)));
    }

    Control::~Control()
    {
        // Underrätta kommunikationsenheten.
        transmitCommand(BT_DISCONNECT);
        delete port_settings_;
        delete ini_file_;
        delete port_;
    }

    void Control::setThrottleValue(int throttle_value)
    {
        throttle_value_ = (char)throttle_value;
        emit out("Changing speed. Current value: " + QString::number(throttle_value_));
        transmitCommand(CONTROL_THROTTLE, 1, &throttle_value_);
    }

    /*
     *  Parsa ett kommando. Alla kommandon som ej är UserInput::INVALID
     *  är giltiga.
     */
    void Control::parseCommand(const UserInput& input)
    {        
        switch (input.command())
        {
        case UserInput::INVALID_COMMAND:
        {
            emit out("Invalid command '" + input.commandString() + "'\n");
            break;
        }

        case UserInput::INVALID_ARGUMENT:
        {
            emit out("Invalid argument(s) for command '" + input.commandString() + "'\n");
            break;
        }

        case UserInput::HELP:
        {
            if (input.argumentCount() == 0)
                emit out(ini_file_->text("help_general"));
            else
            {
                // Hjälptext för specifikt kommando
                QString argument = input.argument(0);
                QString help_text = ini_file_->text("help_" + argument); //help_texts_.value(argument);

                if (help_text != "")
                    emit out(help_text);
                else
                    emit out("No help available for command '" + argument + "'.\n");
            }

            break;
        }

        case UserInput::TRANSMIT:
        {
            if (input.argumentCount() > 0)
            {
                if (port_->isOpen())
                {
                    if (firefly_config_mode_)
                    {
                        // Tolka som sträng
                        QString message = input.argument(0);

                        if (message.endsWith("&&"))
                        {
                            message.remove("&&");
                            message.append("\r\n");
                        }

                        emit out("Transmitting configuration command...");
                        emit out("In ASCII:" + message);

                        port_->transmit(message);

                    }
                    else if (input.argument(0).startsWith("0b"))
                    {
                        // Binärt skall skickas
                        QByteArray message;

                        for (int i = 0; i < input.argumentCount(); ++i)
                        {
                            QString arg = input.argument(i);

                            if (arg.startsWith("0b"))
                            {
                                // Binär representation följer
                                arg.remove("0b");

                                // Konvertera
                                bool ok;
                                int binary = arg.toInt(&ok, 2);

                                if (ok)
                                {
                                    // Lägg till i meddelandet
                                    message.append(binary);
                                }
                                else
                                {
                                    emit out("Error: Not a valid binary input.\n");
                                    return;
                                }
                            }
                        }

                        emit out("Transmitting...");
                        emit out("Raw data: " + utils::readableByteArray(message.toHex()) + "\n");

                        port_->transmit(message);
                    }
                    else
                    {
                        // Tolka som hex.
                        QByteArray message;

                        for (int i = 0; i < input.argumentCount(); ++i)
                        {
                            // Gå igenom alla angivna argument.
                            QString arg = input.argument(i);

                            // Stöd för ej byteseparerad input.
                            if (arg.length() % 2 == 0)
                            {
                                // Jämnt antal tecken.
                                for (int i = 0; i < arg.length(); i += 2)
                                {
                                    bool partial_ok;
                                    QString partial_string = arg.mid(i, 2);

                                    int partial_hex = partial_string.toInt(&partial_ok, 16);

                                    if (partial_ok)
                                    {
                                        message.append(partial_hex);
                                    }
                                    else
                                    {
                                        emit out("Error: Not a valid hex input.\n");
                                        return;
                                    }
                                }
                            }
                            else
                            {
                                emit out("Not an even number of characters.\n");
                                return;
                            }
                        }

                        emit out("Transmitting... ");
                        emit out("Raw data: " + utils::readableByteArray(message.toHex()) + "\n");
                        port_->transmit(message);
                    }
                }
                else
                    emit out("Error: Unable to transmit, the port is closed.\n");
            }
            else
                emit out("Error: Nothing to transmit.\n");
            break;
        }

        case UserInput::STATUS:
        {
            emit out("Port name: \t\t" + port_->portName() +
                     "\nPort status:\t" + port_->state() +
                     "\nBaud rate: \t\t" + utils::toString(port_->baudRate()) +
                     "\nData bits: \t\t" + utils::toString(port_->dataBits()) +
                     "\nParity: \t\t" + utils::toString(port_->parity()) +
                     "\nStop bits: \t\t" + utils::toString(port_->stopBits()) +
                     "\nFirefly config. mode: \t" + utils::toString(firefly_config_mode_) + "\n");
            break;
        }

        case UserInput::OPEN:
        {
            emit out("Opening port " + port_->portName() + "...");

            // Öppna porten
            if (port_->open())
            {
                emit out("Port " + port_->portName() + " open.\n");

                // Underrätta kommunikationsenheten.
                transmitCommand(BT_CONNECT);

                // Sätt throttle
                transmitCommand(CONTROL_THROTTLE, 1, &throttle_value_);
                emit throttleValueChanged(throttle_value_);
            }
            else
            {
                emit out("Unable to open port " +  port_->portName() + ".\n");
            }
            break;
        }

        case UserInput::CLOSE:
        {
            emit out("Closing port " + port_->portName());

            // Underrätta kommunikationsenheten.
            transmitCommand(BT_DISCONNECT);
            port_->flush();
            port_->close();

            bt_connected_ = false;

            emit out("Port " + port_->portName() + " closed.\n");
            break;
        }

        case UserInput::SET:
        {
            QString var = input.argument(0);
            QString val = input.argument(1);

            if (var == "port-name")
            {
                port_->setPortName(val);
                emit out("Port name successfully set to " + val + ".\n");
            }
            else if (var == "baud-rate")
            {
                BaudRateType baud_rate = utils::toBaudRateType(val);
                if (baud_rate != BAUDINVALID)
                {
                    port_->setBaudRate(baud_rate);
                    emit out("Baud rate successfully set to " + val + ".\n");
                }
                else
                {
                    emit out("Error: Not a valid baud rate. Type 'help set' for a list of valid baud rates.\n");
                }
            }
            else if (var == "data-bits")
            {
                DataBitsType data_bits = utils::toDataBitsType(val);
                if (data_bits != DATAINVALID)
                {
                    port_->setDataBits(data_bits);
                    emit out("Number of data bits successfully set to " + val + ".\n");
                }
                else
                {
                    emit out("Error: Not a valid number of data bits. Type 'help set' for a list of valid baud rates.\n");
                }
            }
            else if (var == "parity")
            {
                ParityType parity = utils::toParityType(val);
                if (parity != PARINVALID)
                {
                    port_->setParity(parity);
                    emit out("Parity mode successfully set to " + val + ".\n");
                }
                else
                {
                    emit out("Error: Not a valid parity mode. Type 'help set' for a list of valid baud rates.\n");
                }
            }
            else if (var == "stop-bits")
            {
                StopBitsType stop_bits = utils::toStopBitsType(val);
                if (stop_bits != STOPINVALID)
                {
                    port_->setStopBits(stop_bits);
                    emit out("Number of stop bits successfully set to " + val + ".\n");
                }
                else
                {
                    emit out("Error: Not a valid number of stop bits. Type 'help set' for a list of valid baud rates.\n");
                }
            }

            break;
        }

        case UserInput::READ:
        {
            // Läs en fil med kommandon på varje rad.
            QFile file(input.argument(0));

            emit out("Parsing file '" + input.argument(0) + "'\n");

            if (file.open(QFile::ReadOnly))
            {
                while(!file.atEnd())
                {
                    QString line = file.readLine().trimmed();
                    UserInput input(line);
                    parseCommand(input);
                }

                emit out("Finished parsing the file.\n");
            }
            else
            {
                emit out("Error: Could not open the file.\n");
            }
            break;
        }

        case UserInput::FIREFLY_CONFIG:
        {
            if (firefly_config_mode_)
            {
                emit out("Leaving firefly configuration mode...\n");
                firefly_config_mode_ = false;
            }
            else
            {
                emit out("Entering firefly configuration mode...\n");
                firefly_config_mode_ = true;
            }
        }

        default:
            break;
        }
    }

    /* Hantera knapptryckningar */
    void Control::handleKeyPressEvent(QKeyEvent* event)
    {
        if (!bt_connected_)
            return;

        // Hantera knapptryckningar.
        switch(event->key())
        {
        case Qt::Key_Up:
        case Qt::Key_W:
        {
            emit out("Command: Steer straight.");
            transmitCommand(STEER_STRAIGHT);
            key_up_pressed_ = true;
            break;
        }

        case Qt::Key_Down:
        case Qt::Key_S:
        {
            emit out("Command: Steer back.");
            transmitCommand(STEER_BACK);
            key_down_pressed_ = true;
            break;
        }

        case Qt::Key_Left:
        {
            emit out("Command: Steer left.");
            transmitCommand(STEER_STRAIGHT_LEFT);
            break;
        }

        case Qt::Key_Right:
        {
            emit out("Command: Steer right.");
            transmitCommand(STEER_STRAIGHT_RIGHT);
            break;
        }

        case Qt::Key_A:
        {
            emit out("Command: Rotate left.");
            transmitCommand(STEER_ROTATE_LEFT);
            break;
        }

        case Qt::Key_D:
        {
            emit out("Command: Rotate right.");
            transmitCommand(STEER_ROTATE_RIGHT);
            break;
        }

        case Qt::Key_G:
        {
            emit out("Command: Close Claw.");
            transmitCommand(CLAW_CLOSE);
            break;
        }

        case Qt::Key_B:
        {
            emit out("Command: Open Claw.");
            transmitCommand(CLAW_OPEN);
            break;
        }

        case Qt::Key_Plus:
        {
            increaseThrottle();
            emit out("Command: Increasing speed. Current value: " + QString::number(throttle_value_));
            emit throttleValueChanged(throttle_value_);
            transmitCommand(CONTROL_THROTTLE, 1, &throttle_value_);
            break;
        }

        case Qt::Key_Minus:
        {
            decreaseThrottle();
            emit out("Command: Increasing speed. Current value: " + QString::number(throttle_value_));
            emit throttleValueChanged(throttle_value_);
            transmitCommand(CONTROL_THROTTLE, 1, &throttle_value_);
            break;
        }

        default:
            break;
        }
    }

    /* Hantera knappsläppning */
    void Control::handleKeyReleaseEvent(QKeyEvent* event)
    {
        if (!bt_connected_)
            return;

        switch(event->key())
        {
        case Qt::Key_Up:
        case Qt::Key_W:
        {
            emit out("Command: Stop.");
            transmitCommand(STEER_STOP);
            key_up_pressed_ = false;
            break;
        }
        case Qt::Key_Down:
        case Qt::Key_S:
        {
            emit out("Command: Stop.");
            transmitCommand(STEER_STOP);
            key_down_pressed_ = false;
            break;
        }
        case Qt::Key_A:
        case Qt::Key_D:
        {
            emit out("Command: Stop.");
            transmitCommand(STEER_STOP);
            break;
        }

        case Qt::Key_Left:
        case Qt::Key_Right:
        {
            if (key_up_pressed_)
            {
                Qt::KeyboardModifier modifier = Qt::NoModifier;
                handleKeyPressEvent(new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Up, modifier));
            }
            else if (key_down_pressed_)
            {
                Qt::KeyboardModifier modifier = Qt::NoModifier;
                handleKeyPressEvent(new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Down, modifier));
            }
            else
            {
                emit out("Command: Stop.");
                transmitCommand(STEER_STOP);
                break;
            }
        }

        default:
            break;
        }
    }

    bool Control::isConnected() const
    {
        return bt_connected_;
    }

    /* Returnerar port_ för publik åtkomst */
    SerialPort* Control::port() const
    {
        return port_;
    }

    /*
     *  Public slots
     */

    /* Läs den inkomna datan och skriv den till terminalen */
    void Control::readData()
    {
        // Läs porten
        data_.append(port_->readAll());

        // Lyssna efter acc från kommunikationsenheten.
        if (!bt_connected_ && data_.contains(acknowledge_message_))
        {
            int index = data_.indexOf(acknowledge_message_);
            data_.remove(0, index + 2);

            bt_connected_ = true;
            emit btConnected();
        }

        while (data_.length() >= 2)
        {
            // Hantera fallet att det sista meddelandet har size 0
            if (data_.length() == 2 && data_.at(1) == 0)
            {
                // Presentera
                printData(data_);

                // Hantera meddelandet
                parseMessage(data_);

                // Nollställ
                data_.clear();
            }
            else
            {
                // Avsluta om data_ innehåller ett ofullständigt meddelande.
                if (data_.at(1) > data_.length() - 2)
                    return;

                // Ett helt eller mer än ett helt meddelande emottaget.
                if (data_.length() - 2 >= data_.at(1))
                {
                    int message_length = data_.at(1) + 2;

                    QByteArray message = data_.left(message_length);

                    // Presentera
                    printData(message);

                    // Hantera meddelandet
                    parseMessage(message);

                    // Ta bort ur data_
                    data_.remove(0, message_length);
                }
            }
        }
    }

    /* Rapportera om fullbordad sändning */
    void Control::reportWrite(qint64 bytes_written)
    {
        if (bytes_written != -1)
            emit out("Transmission succeeded.\n");
        else
            emit out("Transmission failed.\n");
    }


    /*
     *  Private
     */

    /* Skicka fördefinierade meddelanden */
    void Control::transmitCommand(char command, char size, char* data)
    {
        QByteArray message;

        // Lägg till kommandot
        message.append(command);

        // Sätt size
        message.append(size);

        // Lägg till data
        if (data != 0)
            message.append(data, size);

        if (command == BT_DISCONNECT)
        {
            bt_connected_ = false;
            mode_ = NO_MODE;
            algorithm_ = NO_ALGORITHM;
            emit btDisconnected();
        }

        if (port_->isOpen())
        {
            emit out("Transmitting " + QString::number(message.size()) + " bytes: " + utils::readableByteArray(message.toHex()));
            port_->transmit(message);
        }
        else
        {
            emit out("Error: Unable to transmit, the port is closed.\n");
        }
    }

    /* Öka hastigheten */
    void Control::increaseThrottle()
    {
        if (throttle_value_ < 100)
            throttle_value_ += THROTTLE_INCREMENT_;
    }

    /* Minska hastigheten */
    void Control::decreaseThrottle()
    {
        if (throttle_value_ >= THROTTLE_INCREMENT_)
            throttle_value_ -= THROTTLE_INCREMENT_;
    }

    /* Tolkar det meddelande som är lagrat i data */
    void Control::parseMessage(const QByteArray& data)
    {
        switch (data.at(0))
        {
        case CONTROL_SIGNALS:
        {
            updateControlSignals(data);
            emit controlSignalsChanged(control_signals_);
            break;
        }

        case FLAG_AUTO:
        {
            if (mode_ != AUTO)
            {
                mode_ = AUTO;
                emit modeChanged(mode_);
            }
            break;
        }

        case FLAG_MANUAL:
        {
            if (mode_ != MANUAL)
            {
                mode_ = MANUAL;
                algorithm_ = NO_ALGORITHM;
                last_turn_ = INVALID;
                emit modeChanged(mode_);
                emit algorithmChanged(algorithm_);
            }
            break;
        }

        case SENSOR_DATA_ALL:
        {
            updateSensorData(data);
            emit sensorDataChanged(sensor_data_);
            break;
        }

        case TURN_STACK_TOP:
        {
            TurnType turn_stack_top = (TurnType)data.at(2);

            switch (turn_stack_top)
            {
            case LEFT_TURN:
                emit log("Registered left turn.");
                emit out("Registered left turn.");
                break;

            case RIGHT_TURN:
                emit log("Registered right turn.");
                emit out("Registered right turn.");
                break;

            case STRAIGHT:
                emit log("Registered straight ahead.");
                emit out("Registered straight ahead.");
                break;

            default:
                emit out("Error: Invalid turn stack type.");
                break;
            }

            break;
        }

        case ALGO_STATE:
        {
            Algorithm new_algoritm = (Algorithm)data.at(2);

            if ((new_algoritm != NO_ALGORITHM) && (algorithm_ != new_algoritm))
            {
                algorithm_ = (Algorithm)data.at(2);
                emit algorithmChanged(algorithm_);
            }

            break;
        }

        case RUN_START:
        {
            emit startAutonomousRun();
            break;
        }

        default:
            break;
        }
    }

    /* Uppdaterar control_signals_ */
    void Control::updateControlSignals(const QByteArray& control_signals_data)
    {
        if (control_signals_data.size() - 2 == CONTROL_SIGNALS_SIZE)
        {
            control_signals_.right_value = control_signals_data.at(2);
            control_signals_.left_value = control_signals_data.at(3);
            control_signals_.right_direction = control_signals_data.at(4);
            control_signals_.left_direction = control_signals_data.at(5);
            control_signals_.claw_value = control_signals_data.at(6);
        }
        else
            emit out("Error: Did not receive a complete control signal struct.");
    }

    /* Uppdaterar sensor_data_ */
    void Control::updateSensorData(const QByteArray& sensor_data)
    {
        if (sensor_data.size() - 2 == SENSOR_DATA_SIZE)
        {
            sensor_data_.distance1 = sensor_data.at(2);
            sensor_data_.distance2 = sensor_data.at(3);
            sensor_data_.distance3 = sensor_data.at(4);
            sensor_data_.distance4 = sensor_data.at(5);
            sensor_data_.distance5 = sensor_data.at(6);
            sensor_data_.distance6 = sensor_data.at(7);
            sensor_data_.distance7 = sensor_data.at(8);

            // Angle skickas little-endian, vänd.
            sensor_data_.angle = (unsigned char)(sensor_data.at(10));
            sensor_data_.angle = (sensor_data_.angle << 8);
            sensor_data_.angle += (unsigned char)(sensor_data.at(9));

            sensor_data_.line_deviation = sensor_data.at(11);
            sensor_data_.line_type = sensor_data.at(12);
        }
        else
            emit out("Error: Did not receive a complete sensor data struct.");
    }

    /* Skriver ut data till terminalen */
    void Control::printData(QByteArray data)
    {
        if (firefly_config_mode_)
        {
            emit out(data);
        }
        else
        {
            if (data.at(0) == SEND_STRING)
            {
                // Tolka resten som en sträng
                data.remove(0, 2);
                emit out("In ASCII: " + data);
            }
            else
            {
                emit out("Recieved " + QString::number(data.length()) + " bytes.");
                emit out("Raw data: " + utils::readableByteArray(data.toHex()) + "\n");
            }
        }
    }

    void Control::portNameChanged(QString port_name)
    {
        port_settings_->setAttributeValue("port-name", "value", port_name);
    }

    void Control::baudRateChanged(QString baud_rate)
    {
        port_settings_->setAttributeValue("baud-rate", "value", baud_rate);
    }

    void Control::dataBitsChanged(QString data_bits)
    {
        port_settings_->setAttributeValue("data-bits", "value", data_bits);
    }

    void Control::parityChanged(QString parity)
    {
        port_settings_->setAttributeValue("parity", "value", parity);
    }

    void Control::stopBitsChanged(QString stop_bits)
    {
        port_settings_->setAttributeValue("stop-bits", "value", stop_bits);
    }
} // namespace MC
