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
        : PlotScene(parent)
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
    void SensorDataPlotScene::drawGrid()
    {
    }

    /* Rita ut */
    void SensorDataPlotScene::draw()
    {
    }

} // namespace MC
