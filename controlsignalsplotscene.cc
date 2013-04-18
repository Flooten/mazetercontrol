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
        : QGraphicsScene(parent)
        , lpen(new QPen(Qt::red))
        , rpen(new QPen(Qt::blue))
    {
        // Lägg in ett idle-element
        ControlSignals cs;
        cs.left_direction = 1;
        cs.right_direction = 1;
        cs.left_value = 0;
        cs.right_value = 0;

        control_signals_.append(cs);
    }

    ControlSignalsPlotScene::~ControlSignalsPlotScene()
    {
        delete lpen;
        delete rpen;
    }

    /*
     *  Public
     */

    /* Lägger till inkommen data i control_signals_ */
    void ControlSignalsPlotScene::newControlSignals(ControlSignals control_signals)
    {
        control_signals_.append(control_signals);
    }

    void ControlSignalsPlotScene::clear()
    {
        foreach (QGraphicsItem* item, items())
            removeItem(item);

        time_ = 0;
    }

    /*
     *  Private
     */

    /* Ritar om */
    void ControlSignalsPlotScene::draw()
    {
        QRect rect(0, 0, 1, 1);
        ++time_;

        // Vänster
        QGraphicsEllipseItem* ldot = new QGraphicsEllipseItem(rect);
        int ypos_l = zero_level_left_;

        if (control_signals_.last().left_direction == 1)
            // Åker framåt
            ypos_l -= control_signals_.last().left_value / 2;
        else
            // Åker bakåt
            ypos_l += control_signals_.last().left_value / 2;

        ldot->setPos(time_, ypos_l);
        ldot->setPen(*lpen);
        addItem(ldot);

        // Höger
        QGraphicsEllipseItem* rdot = new QGraphicsEllipseItem(rect);
        int ypos_r = zero_level_right_;

        if (control_signals_.last().right_direction == 1)
            // Åker framåt
            ypos_r -= control_signals_.last().right_value / 2;
        else
            // Åker bakåt
            ypos_r += control_signals_.last().right_value / 2;

        rdot->setPos(time_, ypos_r);
        rdot->setPen(*rpen);
        addItem(rdot);
    }
} // namespace MC
