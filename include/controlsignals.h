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

#include <cstdint>

namespace MC
{
    struct ControlSignals
    {
        uint8_t right_value = 0;
        uint8_t left_value = 0;
        uint8_t right_direction = 1;
        uint8_t left_direction = 1;
        uint8_t claw_value = 0;
    };
} // namespace MC

#endif // CONTROLSIGNALS_H
