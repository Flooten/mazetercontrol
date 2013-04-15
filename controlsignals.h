/*
 * FILNAMN:       controlsignals.h
 * PROJEKT:       Mazeter
 * PROGRAMMERARE: Fredrik Stenmark
 *                Mattias Fransson
 *                Marcus Eriksson
 * DATUM:         2013-04-15
 *
 * BESKRIVNING:
 *
 */

#ifndef CONTROLSIGNALS_H
#define CONTROLSIGNALS_H

namespace MC
{
    struct ControlSignals
    {
        char right_value = 0;
        char left_value = 0;
        char right_direction = 1;
        char left_direction = 1;
        char claw_value = 0;
    };
} // namespace MC

#endif // CONTROLSIGNALS_H
