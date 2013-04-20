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
#include <QPen>

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
        void drawGrid() override;

    private:
        QVector<SensorData> sensor_data_;

        enum ChosenData
        {
            FRONT_RIGHT,
            FRONT_LEFT,
            RIGHT_SHORT,
            RIGHT_LONG,
            LEFT_SHORT,
            LEFT_LONG,
            BACK,
            ANGLE,
            LINE_DEVIATION,
            LINE_TYPE
        };

        ChosenData chosen_data_ = FRONT_RIGHT;

        // Konstanter
        int zero_level_ = 200;
        int max_level_ = 70;
        int min_level_ = zero_level_;

        QPen pen1_ = QPen(Qt::green);
        QPen pen2_ = QPen(Qt::red);

    public slots:
        void draw() override;
        void chosenDataChanged(int index);
    };
} // namespace MC

#endif // SENSORDATAPLOTSCENE_H
