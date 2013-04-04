/*
 * FILNAMN:       userinput.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 *                Herman Ekwall
 * DATUM:         2013-04-04
 *
 */

#include "userinput.h"

namespace MC
{
    /*
     *  Public
     */
    UserInput::UserInput(const QString& input)
    {
        // Dela på kommandot (första ordet) och
        // eventuella argument.
        arguments_ = input.split(" ");

        if (!arguments_.isEmpty())
        {
            command_string_ = arguments_.takeFirst();

            // Gör lowercase
            command_string_.toLower();

            command_ = toCommand(command_string_);
        }

        // Kolla giltighet
        checkValid();
    }

    bool UserInput::isEmpty() const
    {
        return command_string_.isEmpty();
    }

    UserInput::Command UserInput::command() const
    {
        return command_;
    }

    int UserInput::argumentCount() const
    {
        return arguments_.length();
    }

    QString UserInput::argument(int i) const
    {
        if (i >= arguments_.length())
            return "";
        else
            return arguments_[i];
    }

    QString UserInput::concatenateArguments() const
    {
        return arguments_.join(" ");
    }

    QString UserInput::commandString() const
    {
        return command_string_;
    }

    /*
     *  Private
     */
    UserInput::Command UserInput::toCommand(const QString& command)
    {
        if (command == "help")
            return HELP;
        else if (command == "clear")
            return CLEAR;
        else if (command == "exit")
            return EXIT;
        else if (command == "transmit" || command == "tmit")
            return TRANSMIT;
        else if (command == "open")
            return OPEN;
        else if (command == "close")
            return CLOSE;
        else if (command == "set")
            return SET;
        else if (command == "status")
            return STATUS;
        else if (command == "read")
            return READ;
        else
            return INVALID_COMMAND;
    }

    void UserInput::checkValid()
    {
        int argument_count = argumentCount();
        switch (command_)
        {
        case INVALID_COMMAND:
        case INVALID_ARGUMENT:
            break;

        case STATUS:
        case OPEN:
        case CLOSE:
        case CLEAR:
        case EXIT:
            if (argument_count > 0)
                // Inget argument
                command_ = INVALID_ARGUMENT;
            break;

        case HELP:
            if (argument_count > 1)
                // Ett eller inget argument
                command_ = INVALID_ARGUMENT;
            break;

        case READ:
            if (argument_count != 1)
                // Exakt ett argument
                command_ = INVALID_ARGUMENT;
            break;

        case SET:
            if (argument_count != 2)
                // Exakt två argument
                command_ = INVALID_ARGUMENT;
            break;

        case TRANSMIT:
        default:
            break;
        }
    }


} // namespace MC
