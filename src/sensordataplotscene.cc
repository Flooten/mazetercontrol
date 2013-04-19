/*
 * FILNAMN:       sensordataplotscene.h
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-18
 *
 */

#include "sensordataplotscene.h"
#include <QGraphicsEllipseItem>

namespace MC
{
    SensorDataPlotScene::SensorDataPlotScene(QObject *parent)
        : PlotScene(parent)
    {
        // Lägg in ett idle-element
        SensorData sd;
        sensor_data_.append(sd);
    }

    /*
     *  Public
     */

    /* För in ny sensordata */
    void SensorDataPlotScene::newSensorData(SensorData sensor_data)
    {
        sensor_data_.append(sensor_data);
    }

    /*
     *  Private
     */

    /* Rita ut stödlinjer */
    void SensorDataPlotScene::drawGrid()
    {
        addLine(0, zero_level_, width(), zero_level_, dashed_pen_);
        addLine(0, max_level_, width(), max_level_, dashed_pen_);
    }

    /*
     *  Slots
     */

    /* Rita ut */
    void SensorDataPlotScene::draw()
    {
        QRect rect(0, 0, 1, 1);

        // Punkt 1
        QGraphicsEllipseItem* dot = new QGraphicsEllipseItem(rect);
        int ypos = zero_level_;

        switch (chosen_data_)
        {
        case FRONT_RIGHT:
            ypos -= sensor_data_.last().distance2; // - 20 när vi väl får data
            break;

        default:
            break;
        }

        dot->setPos(time_, ypos);
        dot->setPen(pen1_);
        addItem(dot);

        // Räkna upp
        ++time_;
    }

    void SensorDataPlotScene::chosenDataChanged(int index)
    {
        chosen_data_ = static_cast<ChosenData>(index);

        switch (chosen_data_)
        {
        case FRONT_RIGHT:
            zero_level_ = 170;
            max_level_ = 40;
            min_level_ = 0;
            break;

        case LINE_DEVIATION:
            zero_level_ = 100;
            max_level_ = 0;
            min_level_ = 200;
            break;

        default:
            break;
        }

        clear();
        drawGrid();
    }

} // namespace MC
