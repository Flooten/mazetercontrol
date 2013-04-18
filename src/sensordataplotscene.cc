/*
 * FILNAMN:       sensordataplotscene.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-18
 *
 */

#include "sensordataplotscene.h"

namespace MC
{
    SensorDataPlotScene::SensorDataPlotScene(QObject *parent)
        : QGraphicsScene(parent)
    {
    }

    /*
     *  Public
     */

    /* För in ny sensordata */
    void SensorDataPlotScene::newSensorData(SensorData sensor_data)
    {
    }

    /*
     *  Private
     */

    /* Rita ut stödlinjer */
    void SensorDataPlotScene::drawScale()
    {
    }

    /* Nollställ grafen */
    void SensorDataPlotScene::clear()
    {
    }

    /* Rita ut */
    void SensorDataPlotScene::draw()
    {
    }

} // namespace MC
