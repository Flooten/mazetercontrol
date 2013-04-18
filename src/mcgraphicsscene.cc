/*
 * FILNAMN:       mcgraphicsscene.cc
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-18
 *
 */

#include "mcgraphicsscene.h"

namespace MC
{
    MCGraphicsScene::MCGraphicsScene(QObject *parent)
        : QGraphicsScene(parent)
    {
        background_image_ = new QPixmap(":/images/resources/skiss.png");

        // Rita ut roboten
        addPixmap(*background_image_);

        // Rita ut textlådorna
        createTextItems();
        createButtons();
    }

    MCGraphicsScene::~MCGraphicsScene()
    {
        delete background_image_;
        // DELETE THE MAP
    }

    /* Uppdaterar */
    void MCGraphicsScene::updateSensorData(const SensorData &sensor_data)
    {
        // Giltig sensordata finns i sensor_data, uppdatera texten hos alla graphics items.
        sensor_data_["distance1"]->setPlainText(QString::number(sensor_data.distance1) + UNIT_CM);
        sensor_data_["distance2"]->setPlainText(QString::number(sensor_data.distance2) + UNIT_CM);
        sensor_data_["distance3"]->setPlainText(QString::number(sensor_data.distance3) + UNIT_CM);
        sensor_data_["distance4"]->setPlainText(QString::number(sensor_data.distance4) + UNIT_CM);
        sensor_data_["distance5"]->setPlainText(QString::number(sensor_data.distance5) + UNIT_CM);
        sensor_data_["distance6"]->setPlainText(QString::number(sensor_data.distance6) + UNIT_CM);
        sensor_data_["distance7"]->setPlainText(QString::number(sensor_data.distance7) + UNIT_CM);
        sensor_data_["angle"]->setPlainText(QString::number(sensor_data.angle) + UNIT_DEG);
        sensor_data_["line_deviation"]->setPlainText(QString::number(sensor_data.line_deviation) + UNIT_CM);
        sensor_data_["line_type"]->setPlainText(QString::number(sensor_data.line_type) + UNIT_CM);

        this->update();
    }

    /* Tänder en knapp */
    void MCGraphicsScene::buttonPressed(QKeyEvent* event)
    {
        switch (event->key())
        {
        case Qt::Key_Up:
            buttons_[KEY_UP]->setBrush(QBrush(QColor(Qt::red)));
            break;

        case Qt::Key_Down:
            buttons_[KEY_DOWN]->setBrush(QBrush(QColor(Qt::red)));
            break;

        case Qt::Key_Left:
            buttons_[KEY_LEFT]->setBrush(QBrush(QColor(Qt::red)));
            break;

        case Qt::Key_Right:
            buttons_[KEY_RIGHT]->setBrush(QBrush(QColor(Qt::red)));
            break;
        }
    }

    /* Släcker en knapp */
    void MCGraphicsScene::buttonReleased(QKeyEvent* event)
    {
        switch (event->key())
        {
        case Qt::Key_Up:
            buttons_[KEY_UP]->setBrush(QBrush(QColor(Qt::white)));
            break;

        case Qt::Key_Down:
            buttons_[KEY_DOWN]->setBrush(QBrush(QColor(Qt::white)));
            break;

        case Qt::Key_Left:
            buttons_[KEY_LEFT]->setBrush(QBrush(QColor(Qt::white)));
            break;

        case Qt::Key_Right:
            buttons_[KEY_RIGHT]->setBrush(QBrush(QColor(Qt::white)));
            break;
        }
    }

    /*
     *  Private
     */

    void MCGraphicsScene::createButtons()
    {
        int side_length = 40;

        // Skapa knappar
        buttons_.insert(KEY_UP, new QGraphicsRectItem(0, 0, side_length, side_length));
        buttons_.insert(KEY_DOWN, new QGraphicsRectItem(0, 0, side_length, side_length));
        buttons_.insert(KEY_LEFT, new QGraphicsRectItem(0, 0, side_length, side_length));
        buttons_.insert(KEY_RIGHT, new QGraphicsRectItem(0, 0, side_length, side_length));

        // Placera ut knapparna
        buttons_[KEY_UP]->setPos(300, 300);
        addItem(buttons_[KEY_UP]);
        buttons_[KEY_DOWN]->setPos(300, 340);
        addItem(buttons_[KEY_DOWN]);
        buttons_[KEY_LEFT]->setPos(260, 340);
        addItem(buttons_[KEY_LEFT]);
        buttons_[KEY_RIGHT]->setPos(340, 340);
        addItem(buttons_[KEY_RIGHT]);
    }

    /* Skapar alla textlådor och placerar ut dessa. */
    void MCGraphicsScene::createTextItems()
    {
        sensor_data_.insert("distance1", new QGraphicsTextItem("0 cm"));
        sensor_data_.insert("distance2", new QGraphicsTextItem("0 cm"));
        sensor_data_.insert("distance3", new QGraphicsTextItem("0 cm"));
        sensor_data_.insert("distance4", new QGraphicsTextItem("0 cm"));
        sensor_data_.insert("distance5", new QGraphicsTextItem("0 cm"));
        sensor_data_.insert("distance6", new QGraphicsTextItem("0 cm"));
        sensor_data_.insert("distance7", new QGraphicsTextItem("0 cm"));
        sensor_data_.insert("angle", new QGraphicsTextItem("0 degrees"));
        sensor_data_.insert("line_deviation", new QGraphicsTextItem("0 cm"));
        sensor_data_.insert("line_type", new QGraphicsTextItem("0 cm"));

        placeTextItems();
    }

    /* Placerar ut textlådorna */
    void MCGraphicsScene::placeTextItems()
    {
        int ypos = -40;
        int left_edge_x = -40;
        int right_edge_x = 330;
        int first_line_y = 75;
        int second_line_y = 170;
        int top_edge_y = -30;
        int bottom_edge_y = 280;

        sensor_data_["distance1"]->setPos(52, top_edge_y);
        addItem(sensor_data_["distance1"]);

        sensor_data_["distance2"]->setPos(240, top_edge_y);
        addItem(sensor_data_["distance2"]);

        sensor_data_["distance3"]->setPos(left_edge_x, first_line_y);
        addItem(sensor_data_["distance3"]);

        sensor_data_["distance4"]->setPos(right_edge_x, first_line_y);
        addItem(sensor_data_["distance4"]);

        sensor_data_["distance5"]->setPos(left_edge_x, second_line_y);
        addItem(sensor_data_["distance5"]);

        sensor_data_["distance6"]->setPos(right_edge_x, second_line_y);
        addItem(sensor_data_["distance6"]);

        sensor_data_["distance7"]->setPos(147, bottom_edge_y);
        addItem(sensor_data_["distance7"]);

        sensor_data_["angle"]->setPos(110, ypos);
        addItem(sensor_data_["angle"]);

        sensor_data_["line_deviation"]->setPos(170, ypos);
        addItem(sensor_data_["line_deviation"]);

        sensor_data_["line_type"]->setPos(200, ypos);
        addItem(sensor_data_["line_type"]);
    }
}
