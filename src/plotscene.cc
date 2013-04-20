/*
 * FILNAMN:       plotscene.cc
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-19
 *
 */

#include "plotscene.h"

namespace MC
{
    PlotScene::PlotScene(QObject* parent)
        : QGraphicsScene(parent)
    {}

    /* Rensar grafen */
    void PlotScene::clear()
    {
        foreach (QGraphicsItem* item, items())
            removeItem(item);

        time_ = 0;
    }
} // namespace MC
