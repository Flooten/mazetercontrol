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
    PlotScene::PlotScene(int view_width, int view_height, QObject* parent)
        : QGraphicsScene(parent)
        , view_width_(view_width)
        , view_height_(view_height)
    {}

    /* Rensar grafen */
    void PlotScene::clear()
    {
        foreach (QGraphicsItem* item, items())
        {
            removeItem(item);
            delete item;
        }

        time_ = 0;
    }
} // namespace MC
