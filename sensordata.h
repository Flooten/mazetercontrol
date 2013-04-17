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
    char distance1;
    char distance2;
    char distance3;
    char distance4;
    char distance5;
    char distance6;
    char distance7;
    char angle;
    char line_deviation;
    char line_type;
};

#endif /* SENSOR_DATA_H */
