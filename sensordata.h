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
    char distance1 = -1;
    char distance2 = -1;
    char distance3 = -1;
    char distance4 = -1;
    char distance5 = -1;
    char distance6 = -1;
    char distance7 = -1;
    char angle = -1;
    char line_deviation = -1;
    char line_type = -1;
};

#endif /* SENSOR_DATA_H */
