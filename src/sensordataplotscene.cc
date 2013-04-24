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
    SensorDataPlotScene::SensorDataPlotScene(int view_width, int view_height, QObject *parent)
        : PlotScene(view_width, view_height, parent)
    {
        // Lägg in ett idle-element
        SensorData sd;
        sensor_data_.append(sd);

        line_map_.insert(MAX, new QGraphicsLineItem(0, max_level_, view_width_, max_level_));
        line_map_.insert(MIN, new QGraphicsLineItem(0, min_level_, view_width_, min_level_));
        line_map_.insert(ZERO, new QGraphicsLineItem(0, zero_level_, view_width_, zero_level_));
    }

    SensorDataPlotScene::~SensorDataPlotScene()
    {
        // Återlämna minne
        QMapIterator<GridLine, QGraphicsLineItem*> itr(line_map_);

        while (itr.hasNext())
        {
            itr.next();
            delete itr.value();
        }
    }

    /*
     *  Public
     */

    /* Rita ut stödlinjer */
    void SensorDataPlotScene::drawGrid()
    {
        QMapIterator<GridLine, QGraphicsLineItem*> itr(line_map_);

        while(itr.hasNext())
        {
            itr.next();
            itr.value()->setPen(dashed_pen_);
            addItem(itr.value());
        }
    }

    /* Rita ut */
    void SensorDataPlotScene::draw()
    {
        QRect rect(0, 0, 1, 1);
        int long_offset = 20;
        int short_offset = 20;

        // Punkt 1
        QGraphicsEllipseItem* dot = new QGraphicsEllipseItem(rect);
        int ypos = zero_level_;

        // Position beroende på vald signal
        switch (chosen_data_)
        {
        case FRONT_LEFT:
            ypos -= sensor_data_.last().distance1 - long_offset; // - 20 när vi väl får data
            break;

        case FRONT_RIGHT:
            ypos -= sensor_data_.last().distance2 - long_offset; // - 20 när vi väl får data
            break;

        case LEFT_LONG:
            ypos -= sensor_data_.last().distance3 - long_offset;
            break;

        case RIGHT_LONG:
            ypos -= sensor_data_.last().distance4 - long_offset;
            break;

        case LEFT_SHORT:
            ypos -= sensor_data_.last().distance5 * 5 - short_offset;
            break;

        case RIGHT_SHORT:
            ypos -= sensor_data_.last().distance6 * 5 - short_offset;
            break;

        case BACK:
            ypos -= sensor_data_.last().distance7 - long_offset;
            break;

        case ANGLE:
            ypos -= sensor_data_.last().angle / 200;
            break;

        case LINE_DEVIATION:
            ypos -= sensor_data_.last().line_deviation;
            break;

        default:
            break;
        }

        dot->setPos(time_, ypos);
        dot->setPen(pen1_);
        addItem(dot);

        // Om scrollning behövs
        if (time_ > view_width_)
        {
            QMapIterator<GridLine, QGraphicsLineItem*> itr(line_map_);

            while (itr.hasNext())
            {
                itr.next();
                // Förläng gridlines
                QLineF line = itr.value()->line();

                // Lägg till en punkt
                line.setP2(QPointF(time_, line.y2()));
                itr.value()->setLine(line);
            }

            emit center(time_);
        }

        // Räkna upp
        ++time_;
    }

    /* Plockar bort linjerna och kallar på PlotScene::clear() */
    void SensorDataPlotScene::clear()
    {
        QMapIterator<GridLine, QGraphicsLineItem*> itr(line_map_);

        while(itr.hasNext())
        {
            itr.next();

            if (items().contains(itr.value()))
                removeItem(itr.value());
        }

        PlotScene::clear();
    }

    /*
     *  Public slots
     */

    /* För in ny sensordata */
    void SensorDataPlotScene::newSensorData(SensorData sensor_data)
    {
        sensor_data_.append(sensor_data);
    }

    void SensorDataPlotScene::chosenDataChanged(int index)
    {
        chosen_data_ = static_cast<ChosenData>(index);

        QMapIterator<GridLine, QGraphicsLineItem*> itr(line_map_);

        switch (chosen_data_)
        {
        case FRONT_RIGHT:
        case FRONT_LEFT:
        case RIGHT_LONG:
        case LEFT_LONG:
        case BACK:
        case LEFT_SHORT:
        case RIGHT_SHORT:
        {
            zero_level_ = 200;
            max_level_ = 70;
            min_level_ = zero_level_;

            while(itr.hasNext())
            {
                itr.next();

                QLineF line = itr.value()->line();

                // Modifiera linjen
                switch (itr.key())
                {
                case ZERO:
                {
                    line.setPoints(QPointF(0, zero_level_), QPointF(view_width_, zero_level_));
                    itr.value()->setLine(line);
                    break;
                }
                case MAX:
                {
                    line.setPoints(QPointF(0, max_level_), QPointF(view_width_, max_level_));
                    itr.value()->setLine(line);
                    break;
                }
                case MIN:
                {
                    line.setPoints(QPointF(0, min_level_), QPointF(view_width_, min_level_));
                    itr.value()->setLine(line);
                    break;
                }
                default:
                    break;
                }
            }
            break;
        }

        case LINE_DEVIATION:
        {
            zero_level_ = 130;
            max_level_ = 60;
            min_level_ = 200;

            while(itr.hasNext())
            {
                itr.next();
                // Förläng gridlines
                QLineF line = itr.value()->line();

                // Modifiera linjen
                switch (itr.key())
                {
                case ZERO:
                {
                    line.setPoints(QPointF(0, zero_level_), QPointF(view_width_, zero_level_));
                    itr.value()->setLine(line);
                    break;
                }
                case MAX:
                {
                    line.setPoints(QPointF(0, max_level_), QPointF(view_width_, max_level_));
                    itr.value()->setLine(line);
                    break;
                }
                case MIN:
                {
                    line.setPoints(QPointF(0, min_level_), QPointF(view_width_, min_level_));
                    itr.value()->setLine(line);
                    break;
                }
                default:
                    break;
                }
            }
            break;
        }

        case ANGLE:
            zero_level_ = 230;
            max_level_ = 50;
            min_level_ = zero_level_;

            while(itr.hasNext())
            {
                itr.next();
                // Förläng gridlines
                QLineF line = itr.value()->line();

                // Modifiera linjen
                switch (itr.key())
                {
                case ZERO:
                {
                    line.setPoints(QPointF(0, zero_level_), QPointF(view_width_, zero_level_));
                    itr.value()->setLine(line);
                    break;
                }
                case MAX:
                {
                    line.setPoints(QPointF(0, max_level_), QPointF(view_width_, max_level_));
                    itr.value()->setLine(line);
                    break;
                }
                case MIN:
                {
                    line.setPoints(QPointF(0, min_level_), QPointF(view_width_, min_level_));
                    itr.value()->setLine(line);
                    break;
                }
                default:
                    break;
                }
            }
            break;

        default:
            break;
        }

        clear();
        drawGrid();
    }

} // namespace MC
