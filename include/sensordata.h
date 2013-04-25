/*
 * FILNAMN:       sensordata.h
 * PROJEKT:       Mazeter
 * PROGRAMMERARE: Martin Andersson
 *                Mattias Fransson
 * DATUM:         2013-04-02
 *
 * BESKRIVNING:
 *
 */

#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <cstdint>

struct SensorData
{
    uint8_t distance1 = 0;
    uint8_t distance2 = 0;
    uint8_t distance3 = 0;
    uint8_t distance4 = 0;
    uint8_t distance5 = 0;
    uint8_t distance6 = 0;
    uint8_t distance7 = 0;
    uint16_t angle = 0;
    int8_t line_deviation = 0;
    uint8_t line_type = 0;
};

#endif /* SENSOR_DATA_H */
