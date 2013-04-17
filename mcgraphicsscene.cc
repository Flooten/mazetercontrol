#include "mcgraphicsscene.h"

namespace MC
{
    MCGraphicsScene::MCGraphicsScene(QObject *parent)
        : QGraphicsScene(parent)
    {
        background_image_ = new QPixmap(":/images/resources/skiss.png");
        sensor_values_.insert("distance1", new QGraphicsTextItem("0 cm"));
        sensor_values_.insert("distance2", new QGraphicsTextItem("0 cm"));
        sensor_values_.insert("distance3", new QGraphicsTextItem("0 cm"));
        sensor_values_.insert("distance4", new QGraphicsTextItem("0 cm"));
        sensor_values_.insert("distance5", new QGraphicsTextItem("0 cm"));
        sensor_values_.insert("distance6", new QGraphicsTextItem("0 cm"));
        sensor_values_.insert("distance7", new QGraphicsTextItem("0 cm"));
        sensor_values_.insert("angle", new QGraphicsTextItem("0 deg"));
        sensor_values_.insert("line_deviation", new QGraphicsTextItem("0 cm"));
        sensor_values_.insert("line_type", new QGraphicsTextItem("0 cm"));

        // Rita ut default
        addPixmap(*background_image_);
    }

    MCGraphicsScene::~MCGraphicsScene()
    {
        delete background_image_;
        // DELETE THE MAP
    }

    /* Uppdaterar */
    void MCGraphicsScene::update(const SensorData &sensor_data)
    {
    }

    /*
     *  Private
     */
}
