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

struct SensorData
{
    char distance1 = 0;
    char distance2 = 0;
    char distance3 = 0;
    char distance4 = 0;
    char distance5 = 0;
    char distance6 = 0;
    char distance7 = 0;
    char angle = 0;
    char line_deviation = 0;
    char line_type = 0;
};

#endif /* SENSOR_DATA_H */
