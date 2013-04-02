#include "control.h"

#include <QStringList>
#include <QFile>

#include "utils.h"

namespace MC
{
    /*
     *  Public
     */
    Control::Control(const QString &ini_file, QObject *parent)
        : QObject(parent)
    {
        parseIniFile(ini_file);

        // Anslutningar
        connect(port_, SIGNAL(readyRead()), this, SLOT(readData()));
        connect(port_, SIGNAL(bytesWritten(qint64)), this, SLOT(reportWrite(qint64)));
    }

    /*
     *  Parsa ett kommando. Alla kommandon som ej är UserInput::INVALID
     *  är giltiga.
     */
    void Control::parseCommand(const UserInput& input)
    {        
        switch (input.command())
        {
        case UserInput::HELP:
        {
            if (input.argumentCount() == 0)
                emit out(help_message_);
            else
            {
                // Hjälptext för specifikt kommando
                QString argument = input.argument(0);
                QString help_text = help_texts_.value(argument);

                if (help_text != "")
                    emit out(help_text);
                else
                    emit out("No help available for command '" + argument + "'.\n");
            }

            break;
        }

        case UserInput::EXIT:
        {
            exit(0);
        }

        case UserInput::TRANSMIT:
        {
            if (input.argumentCount() > 0)
            {
                if (port_->isOpen())
                {
                    if (input.argument(0).startsWith("0b"))
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
                        // Tolka som hex
                        QByteArray message;

                        for (int i = 0; i < input.argumentCount(); ++i)
                        {
                            QString arg = input.argument(i);

                            bool ok;
                            int hex = arg.toInt(&ok, 16);

                            if (hex > 255)
                            {
                                if (arg.length() % 2 == 0)
                                {
                                    // Jämnt antal tecken
                                    for (int i = 0; i < arg.length(); i += 2)
                                    {
                                        bool partial_ok;
                                        QString partial_string = arg.mid(i, 2);

                                        int partial_hex = partial_string.toInt(&partial_ok, 16);

                                        if (ok)
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
                            else
                            {
                                if (ok)
                                {
                                    message.append(hex);
                                }
                                else
                                {
                                    emit out("Error: Not a valid hex input.\n");
                                    return;
                                }
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
                     "\nStop bits: \t\t" + utils::toString(port_->stopBits()) + "\n");
            break;
        }

        case UserInput::CLEAR:
        {
            emit clear();
            break;
        }

        case UserInput::OPEN:
        {
            emit out("Opening port " + port_->portName() + "...");

            // Öppna porten
            if (port_->open())
                emit out("Port " + port_->portName() + " open.\n");
            else
                emit out("Unable to open port " +  port_->portName() + ".\n");
            break;
        }

        case UserInput::CLOSE:
        {
            emit out("Closing port " + port_->portName());
            port_->close();
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

        case UserInput::INVALID:
        default:
            break;
        }
    }

    /* Skriv ut hjälptexten */
    void Control::printWelcomeMessage()
    {
        emit out(welcome_message_);
    }

    /*
     *  Private
     */

    /*  Parsar ini-filen angiven i ini_file */
    void Control::parseIniFile(const QString& ini_file)
    {
        try
        {
            QFile file(ini_file);

            if (!file.open(QIODevice::ReadOnly))
            {
               emit out("Unable to open file " + file.fileName() + " for reading.");
            }

            QTextStream stream(&file);

            // Parsa raden
            while (!stream.atEnd())
            {
                QString str = stream.readLine();

                // Ignorera kommentarer, tomma rader
                if (str.startsWith('#') || str.startsWith('\n') || str.startsWith(' '))
                    continue;
                else if (str.startsWith('\\'))
                {
                    // Kommando
                    QString command;
                    QString argument;

                    readLine(str, command, argument);

                    if (command == "begin")
                    {
                        // Parsa sammanhängande data
                        if (argument == "help_message")
                        {
                            help_message_ = readUntilEnd(stream);
                        }
                        else if (argument == "welcome_message")
                        {
                            welcome_message_ = readUntilEnd(stream);
                        }
                        else if (argument == "help_set")
                        {
                            help_texts_.insert("set", readUntilEnd(stream));
                        }
                        else if (argument == "help_exit")
                        {
                            help_texts_.insert("exit", readUntilEnd(stream));
                        }
                        else if (argument == "help_transmit")
                        {
                            help_texts_.insert("transmit", readUntilEnd(stream));
                        }
                        else if (argument == "port")
                        {
                            QString port_name;
                            QString baud_rate;
                            QString data_bits;
                            QString parity;
                            QString stop_bits;

                            do
                            {
                                str = stream.readLine();

                                QString name;
                                QString value;

                                readAssignArgument(str, name, value);

                                if (name == "port_name")
                                    port_name = value;
                                else if (name == "baud_rate")
                                    baud_rate = value;
                                else if (name == "data_bits")
                                    data_bits = value;
                                else if (name == "parity")
                                    parity = value;
                                else if (name == "stop_bits")
                                    stop_bits = value;
                            }
                            while (!str.isNull());

                            // Skapa en port
                            if (!port_name.isEmpty() && !baud_rate.isEmpty() && !data_bits.isEmpty() && !parity.isEmpty() && !stop_bits.isEmpty())
                                port_ = new SerialPort(port_name, baud_rate, data_bits, parity, stop_bits);
                            else
                                port_ = new SerialPort();
                        }
                    }
                    else if (command == "assign")
                    {
                        // Sätt en variabel
                        QString name;
                        QString value;

                        readAssignArgument(argument, name, value);

                        // LISTA HÄR
                        if (name == "foo")
                        {}
                    }
                }
            }
        }
        catch (...)
        {
            emit out("Error while parsing ini-file.");
        }
    }

    /*
     *  Läser en rad ur en ini-fil. Kommandot som följer efter '\'
     *  läggs i command och argumentet i argument. Otillåten data
     *  resulterar i tomma strängar.
     */
    void Control::readLine(QString& str, QString& command, QString& argument)
    {
        if (!str.startsWith('\\'))
            return;
        else
            str.remove(0, 1);

        QStringList list = str.split(" ");

        command = list[0];

        if (list.length() == 2)
            argument = list[1];
        else
            argument = "";
    }

    /*
     *  Läser en tilldelningsuttryck och lagrar variabeln i name
     *  och dess värde i value. Otillåten data resilterar i tomma
     *  strängar.
     */
    void Control::readAssignArgument(const QString& argument, QString& name, QString& value)
    {
        QStringList list = argument.split("=");

        name = list[0];

        if (list.length() == 2)
            value = list[1];
        else
            value = "";
    }

    /*
     *  Läser data tills kommandot \end påträffas och tolkar
     *  denna som ren text.
     */
    QString Control::readUntilEnd(QTextStream& is)
    {
        QString str;
        QString line;

        do
        {
            // Läs rader
            line = is.readLine();

            QString command;
            QString argument;

            readLine(line, command, argument);

            if (command == "end")
                break;
            else
            {
                str.append(line);
                str.append("\n");
            }
        }
        while (!str.isNull());

        return str;
    }

    /*
     *  Slots
     */

    /* Läs den inkomna datan och skriv den till terminalen */
    void Control::readData()
    {
        qint64 bytes_available = port_->bytesAvailable();
        QByteArray message = port_->readAll();
        emit out("Recieved " + QString::number(bytes_available) + " bytes.");
        emit out("In ASCII: " + message);
        emit out("Raw data: " + utils::readableByteArray(message.toHex()) + "\n");
    }

    /* Rapportera om fullbordad sändning */
    void Control::reportWrite(qint64 bytes_written)
    {
        if (bytes_written != -1)
            emit out("Transmission succeeded. " + QString::number(bytes_written) + " bytes were sent.\n");
        else
            emit out("Transmission failed. No bytes were sent.\n");
    }
} // namespace MC
