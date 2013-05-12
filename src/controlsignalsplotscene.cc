/*
 * FILNAMN:       controlsignalsplotscene.cc
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-18
 *
 */

#include "controlsignalsplotscene.h"

#include <QLineF>

namespace MC
{
    ControlSignalsPlotScene::ControlSignalsPlotScene(int view_width, int view_height, QObject* parent)
        : PlotScene(view_width, view_height, parent)
    {
        // Lägg in ett idle-element
        ControlSignals cs;
        control_signals_.append(cs);

        line_vector_.append(new QGraphicsLineItem(0, MAX_LEVEL_LEFT_, view_width_, MAX_LEVEL_LEFT_));
        line_vector_.append(new QGraphicsLineItem(0, ZERO_LEVEL_LEFT_, view_width_, ZERO_LEVEL_LEFT_));
        line_vector_.append(new QGraphicsLineItem(0, MIN_LEVEL_LEFT_, view_width_, MIN_LEVEL_LEFT_));

        line_vector_.append(new QGraphicsLineItem(0, MAX_LEVEL_RIGHT_, view_width_, MAX_LEVEL_RIGHT_));
        line_vector_.append(new QGraphicsLineItem(0, ZERO_LEVEL_RIGHT_, view_width_, ZERO_LEVEL_RIGHT_));
        line_vector_.append(new QGraphicsLineItem(0, MIN_LEVEL_RIGHT_, view_width_, MIN_LEVEL_RIGHT_));
    }

    ControlSignalsPlotScene::~ControlSignalsPlotScene()
    {
        // Återlämna minne
        QVectorIterator<QGraphicsLineItem*> itr(line_vector_);

        while (itr.hasNext())
            delete itr.next();

        delete last_lline_;
        delete last_rline_;
    }

    /*
     *  Public
     */

    /* Ritar stödlinjer */
    void ControlSignalsPlotScene::drawGrid()
    {
        QVectorIterator<QGraphicsLineItem*> itr(line_vector_);

        while(itr.hasNext())
        {
            itr.peekNext()->setPen(dashed_pen_);
            addItem(itr.next());
        }
    }

    /* Ritar om */
    void ControlSignalsPlotScene::draw()
    {
        int ypos_l = ZERO_LEVEL_LEFT_;
        int ypos_r = ZERO_LEVEL_RIGHT_;

        if (control_signals_.last().left_direction == 1)
            // Åker framåt
            ypos_l -= control_signals_.last().left_value / 2;
        else
            // Åker bakåt
            ypos_l += control_signals_.last().left_value / 2;

        if (control_signals_.last().right_direction == 1)
            // Åker framåt
            ypos_r -= control_signals_.last().right_value / 2;
        else
            // Åker bakåt
            ypos_r += control_signals_.last().right_value / 2;

        // Höger
        if ((last_rline_ != NULL) && last_rline_->line().y2() == ypos_r)
        {
            // Utöka den gamla linjen
            QLineF line = last_rline_->line();
            line.setP2(QPointF(time_, line.y2()));
            last_rline_->setLine(line);
        }
        else
        {
            // En ny linje ska ritas
            QGraphicsLineItem* rline = new QGraphicsLineItem();

            if (ypos_r < MAX_LEVEL_RIGHT_)
            {
                ypos_r = MAX_LEVEL_RIGHT_;
                rline->setPen(red_pen_);
            }
            else if (ypos_r > MIN_LEVEL_RIGHT_)
            {
                ypos_r = MIN_LEVEL_RIGHT_;
                rline->setPen(red_pen_);
            }
            else
            {
                rline->setPen(blue_pen_);
            }

            if ((last_rline_ != NULL) && abs(ypos_r - last_rline_->line().y2()) >= 2)
            {
                QGraphicsLineItem* connector = new QGraphicsLineItem(time_, last_rline_->line().y2(), time_, ypos_r);
                connector->setPen(blue_pen_);
                addItem(connector);
            }

            rline->setLine(time_, ypos_r, time_, ypos_r);

            last_rline_ = rline;
            addItem(rline);
        }

        // Vänster
        if ((last_lline_ != NULL) && last_lline_->line().y2() == ypos_l)
        {
            // Utöka den gamla linjen
            QLineF line = last_lline_->line();
            line.setP2(QPointF(time_, line.y2()));
            last_lline_->setLine(line);
        }
        else
        {
            // En ny linje ska ritas
            QGraphicsLineItem* lline = new QGraphicsLineItem();

            if (ypos_l < MAX_LEVEL_LEFT_)
            {
                ypos_l = MAX_LEVEL_LEFT_;
                lline->setPen(red_pen_);
            }
            else if (ypos_l > MIN_LEVEL_LEFT_)
            {
                ypos_l = MIN_LEVEL_LEFT_;
                lline->setPen(red_pen_);
            }
            else
            {
                lline->setPen(blue_pen_);
            }

            if ((last_lline_ != NULL) && abs(ypos_l - last_lline_->line().y2()) >= 2)
            {
                QGraphicsLineItem* connector = new QGraphicsLineItem(time_, last_lline_->line().y2(), time_, ypos_l);
                connector->setPen(blue_pen_);
                addItem(connector);
            }

            lline->setLine(time_, ypos_l, time_, ypos_l);

            last_lline_ = lline;
            addItem(lline);
        }

        if (time_ > view_width_)
        {
            QVectorIterator<QGraphicsLineItem*> itr(line_vector_);

            while (itr.hasNext())
            {
                // Förläng gridlines
                QLineF line = itr.peekNext()->line();

                // Lägg till en punkt
                line.setP2(QPointF(time_, line.y2()));
                itr.next()->setLine(line);
            }

            emit center(time_);
        }

        // Räkna upp
        ++time_;
    }

    /* Plockar bort linjerna och kallar på PlotScene::clear() */
    void ControlSignalsPlotScene::clear()
    {
        QVectorIterator<QGraphicsLineItem*> itr(line_vector_);

        while(itr.hasNext())
        {
            if (items().contains(itr.next()))
                removeItem(itr.peekPrevious());
        }

        last_lline_ = NULL;
        last_rline_ = NULL;

        PlotScene::clear();
    }

    /*
     *  Public slots
     */

    /* Lägger till inkommen data i control_signals_ */
    void ControlSignalsPlotScene::newControlSignals(ControlSignals control_signals)
    {
        control_signals_.append(control_signals);
    }
} // namespace MC
