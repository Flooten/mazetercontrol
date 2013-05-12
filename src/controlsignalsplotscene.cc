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
        , lpen_(QPen(Qt::red))
        , rpen_(QPen(Qt::blue))
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

        delete last_ldot_;
        delete last_rdot_;
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
        QRect rect(0, 0, 1, 1);

        // Vänster
        QGraphicsEllipseItem* ldot = new QGraphicsEllipseItem(rect);
        QGraphicsEllipseItem* rdot = new QGraphicsEllipseItem(rect);

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

        ldot->setPos(time_, ypos_l);
        ldot->setPen(lpen_);

        rdot->setPos(time_, ypos_r);
        rdot->setPen(rpen_);

        // Rita förbindelser mellan punkterna
        if ((last_ldot_ != NULL) && abs(ypos_l - last_ldot_->y()) >= 2)
        {
            QGraphicsLineItem* connector = new QGraphicsLineItem(time_, last_ldot_->y(), time_, ypos_l);
            connector->setPen(lpen_);
            addItem(connector);
        }

        if ((last_rdot_ != NULL) && abs(ypos_r - last_rdot_->y()) >= 2)
        {
            QGraphicsLineItem* connector = new QGraphicsLineItem(time_, last_rdot_->y(), time_, ypos_r);
            connector->setPen(rpen_);
            addItem(connector);
        }

        last_ldot_ = ldot;
        last_rdot_ = rdot;

        addItem(ldot);
        addItem(rdot);

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

        last_ldot_ = NULL;
        last_rdot_ = NULL;

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
