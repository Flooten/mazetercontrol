/*
 * FILNAMN:       userinput.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 *                Herman Ekwall
 * DATUM:         2013-04-04
 *
 * BESKRIVNING:   UserInput är en stödklass för att möjliggöra enkel
 *                hantering av de kommandon som användaren matar in.
 *
 */

#ifndef USERINPUT_H
#define USERINPUT_H

#include <QString>
#include <QStringList>

namespace MC
{
    class UserInput
    {
    public:
        UserInput(const QString& input);

        enum Command
        {
            INVALID_COMMAND,
            INVALID_ARGUMENT,
            HELP,
            EXIT,
            TRANSMIT,
            STATUS,
            OPEN,
            CLOSE,
            CLEAR,
            SET,
            READ,
            FIREFLY_CONFIG
        };

        bool isEmpty() const;

        int argumentCount() const;
        QString argument(int i) const;
        QString concatenateArguments() const;

        Command command() const;
        QString commandString() const;

    private:
        // Variabler
        Command command_;
        QString command_string_;
        QStringList arguments_;

        // Funktioner
        Command toCommand(const QString& command);
        void checkValid();
    };
} // namespace MC
#endif // USERINPUT_H
