/*
 * FILNAMN:       controlsignalsplotscene.cc
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-18
 *
 */

#include "controlsignalsplotscene.h"

namespace MC
{
    ControlSignalsPlotScene::ControlSignalsPlotScene(QObject* parent)
        : PlotScene(parent)
        , lpen_(QPen(Qt::red))
        , rpen_(QPen(Qt::blue))
    {
        // Lägg in ett idle-element
        ControlSignals cs;
        control_signals_.append(cs);
    }

    /*
     *  Public
     */

    /* Lägger till inkommen data i control_signals_ */
    void ControlSignalsPlotScene::newControlSignals(ControlSignals control_signals)
    {
        control_signals_.append(control_signals);
    }

    /* Ritar stödlinjer */
    void ControlSignalsPlotScene::drawGrid()
    {
        // Skala för vänster motorsignal
        addLine(0, MAX_LEVEL_LEFT_, width(), MAX_LEVEL_LEFT_, dashed_pen_);
        addLine(0, ZERO_LEVEL_LEFT_, width(), ZERO_LEVEL_LEFT_, dashed_pen_);
        addLine(0, MIN_LEVEL_LEFT_, width(), MIN_LEVEL_LEFT_, dashed_pen_);

        // Skala för höger motorsignal
        addLine(0, MAX_LEVEL_RIGHT_, width(), MAX_LEVEL_RIGHT_, dashed_pen_);
        addLine(0, ZERO_LEVEL_RIGHT_, width(), ZERO_LEVEL_RIGHT_, dashed_pen_);
        addLine(0, MIN_LEVEL_RIGHT_, width(), MIN_LEVEL_RIGHT_, dashed_pen_);
    }

    /*
     *  Private
     */

    /* Ritar om */
    void ControlSignalsPlotScene::draw()
    {
        QRect rect(0, 0, 1, 1);

        // Vänster
        QGraphicsEllipseItem* ldot = new QGraphicsEllipseItem(rect);
        int ypos_l = ZERO_LEVEL_LEFT_;

        if (control_signals_.last().left_direction == 1)
            // Åker framåt
            ypos_l -= control_signals_.last().left_value / 2;
        else
            // Åker bakåt
            ypos_l += control_signals_.last().left_value / 2;

        ldot->setPos(time_, ypos_l);
        ldot->setPen(lpen_);
        addItem(ldot);

        // Höger
        QGraphicsEllipseItem* rdot = new QGraphicsEllipseItem(rect);
        int ypos_r = ZERO_LEVEL_RIGHT_;

        if (control_signals_.last().right_direction == 1)
            // Åker framåt
            ypos_r -= control_signals_.last().right_value / 2;
        else
            // Åker bakåt
            ypos_r += control_signals_.last().right_value / 2;

        rdot->setPos(time_, ypos_r);
        rdot->setPen(rpen_);
        addItem(rdot);

        // Räkna upp
        ++time_;
    }
} // namespace MC
