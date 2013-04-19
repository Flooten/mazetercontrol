/*
 * FILNAMN:       sensordataplotscene.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-18
 *
 * BESKRIVNING:   SensorDataPlotScene är klassen som representerar den scen
 *                som presenterar en tidsplot av robotens sensorvärden.
 *
 */

#ifndef SENSORDATAPLOTSCENE_H
#define SENSORDATAPLOTSCENE_H

#include <QGraphicsScene>
#include <QObject>

#include "sensordata.h"
#include "plotscene.h"

namespace MC
{
    class SensorDataPlotScene : public PlotScene
    {
        Q_OBJECT
    public:
        SensorDataPlotScene(QObject* parent = NULL);

        void newSensorData(SensorData sensor_data);

    private:
        QVector<SensorData> sensor_data_;

        // Konstanter

        void drawGrid() override;

    public slots:
        void draw() override;
    };
} // namespace MC

#endif // SENSORDATAPLOTSCENE_H
