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
#include <QMap>
#include <QGraphicsLineItem>

#include "sensordata.h"
#include "plotscene.h"

namespace MC
{
    class SensorDataPlotScene : public PlotScene
    {
        Q_OBJECT
    public:
        SensorDataPlotScene(int view_width, int view_height, QObject* parent = NULL);

        ~SensorDataPlotScene();

        void drawGrid() override;
        void draw() override;
        void clear() override;

    public slots:
        void newSensorData(SensorData sensor_data);
        void chosenDataChanged(int index);

    private:
        QVector<SensorData> sensor_data_;

        enum ChosenData
        {
            FRONT_LEFT,
            FRONT_RIGHT,
            LEFT_LONG,
            RIGHT_LONG,
            LEFT_SHORT,
            RIGHT_SHORT,
            BACK,
            ANGLE,
            LINE_DEVIATION
        };

        enum GridLine
        {
            MAX,
            MIN,
            ZERO
        };

        ChosenData chosen_data_ = FRONT_LEFT;
        QMap<GridLine, QGraphicsLineItem*> line_map_;
        QGraphicsLineItem* last_line_ = NULL;

        int zero_level_ = 190;
        int max_level_ = 60;
        int min_level_ = zero_level_;
    };
} // namespace MC

#endif // SENSORDATAPLOTSCENE_H
