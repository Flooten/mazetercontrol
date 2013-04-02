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
            INVALID,
            HELP,
            EXIT,
            TRANSMIT,
            STATUS,
            OPEN,
            CLOSE,
            CLEAR,
            SET
        };

        bool isValid() const;
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
