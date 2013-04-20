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
        , background_image_(new QPixmap(":/images/resources/overview.png"))
    {}

    MCGraphicsScene::~MCGraphicsScene()
    {
        delete background_image_;
        // DELETE THE MAP
    }

    /* Uppdaterar */
    void MCGraphicsScene::updateSensorData(const SensorData &sensor_data)
    {
        // Giltig sensordata finns i sensor_data, uppdatera texten hos alla graphics items.
        sensor_data_[FRONT_LEFT]->setPlainText(QString::number(sensor_data.distance1) + UNIT_CM);
        sensor_data_[FRONT_RIGHT]->setPlainText(QString::number(sensor_data.distance2) + UNIT_CM);
        sensor_data_[LEFT_LONG]->setPlainText(QString::number(sensor_data.distance3) + UNIT_CM);
        sensor_data_[RIGHT_LONG]->setPlainText(QString::number(sensor_data.distance4) + UNIT_CM);
        sensor_data_[LEFT_SHORT]->setPlainText(QString::number(sensor_data.distance5) + UNIT_CM);
        sensor_data_[RIGHT_SHORT]->setPlainText(QString::number(sensor_data.distance6) + UNIT_CM);
        sensor_data_[BACK]->setPlainText(QString::number(sensor_data.distance7) + UNIT_CM);
        sensor_data_[ANGLE]->setPlainText(QString::number(sensor_data.angle) + UNIT_DEG);
        sensor_data_[LINE_DEVIATION]->setPlainText(QString::number(sensor_data.line_deviation) + UNIT_CM);
        sensor_data_[LINE_TYPE]->setPlainText(QString::number(sensor_data.line_type) + UNIT_CM);

        this->update();
    }

    /* Tänder en knapp */
    void MCGraphicsScene::buttonPressed(QKeyEvent* event)
    {
        switch (event->key())
        {
        case Qt::Key_Up:
            buttons_[KEY_UP]->setPixmap(QPixmap(":/images/resources/upp.png"));
            break;

        case Qt::Key_Down:
            buttons_[KEY_DOWN]->setPixmap(QPixmap(":/images/resources/downp.png"));
            break;

        case Qt::Key_Left:
            buttons_[KEY_LEFT]->setPixmap(QPixmap(":/images/resources/leftp.png"));
            break;

        case Qt::Key_Right:
            buttons_[KEY_RIGHT]->setPixmap(QPixmap(":/images/resources/rightp.png"));
            break;
        }
    }

    /* Släcker en knapp */
    void MCGraphicsScene::buttonReleased(QKeyEvent* event)
    {
        switch (event->key())
        {
        case Qt::Key_Up:
            buttons_[KEY_UP]->setPixmap(QPixmap(":/images/resources/upr.png"));
            break;

        case Qt::Key_Down:
            buttons_[KEY_DOWN]->setPixmap(QPixmap(":/images/resources/downr.png"));
            break;

        case Qt::Key_Left:
            buttons_[KEY_LEFT]->setPixmap(QPixmap(":/images/resources/leftr.png"));
            break;

        case Qt::Key_Right:
            buttons_[KEY_RIGHT]->setPixmap(QPixmap(":/images/resources/rightr.png"));
            break;
        }
    }

    void MCGraphicsScene::draw()
    {
        // Rita ut roboten
        addPixmap(*background_image_);

        // Rita ut textlådorna
        createTextItems();
        createButtons();
    }

    /*
     *  Private
     */

    void MCGraphicsScene::createButtons()
    {
        QPixmap button(":/images/resources/upr.png");

        QPoint button_dimensions(button.width(), button.height());
        QPoint up_pos(300, 300);
        int spacing = 5;

        // Skapa knappar
        buttons_.insert(KEY_UP, new QGraphicsPixmapItem(QPixmap(":/images/resources/upr.png")));
        buttons_.insert(KEY_DOWN, new QGraphicsPixmapItem(QPixmap(":/images/resources/downr.png")));
        buttons_.insert(KEY_LEFT, new QGraphicsPixmapItem(QPixmap(":/images/resources/leftr.png")));
        buttons_.insert(KEY_RIGHT, new QGraphicsPixmapItem(QPixmap(":/images/resources/rightr.png")));

        // Placera ut knapparna
        buttons_[KEY_UP]->setPos(up_pos);
        addItem(buttons_[KEY_UP]);
        buttons_[KEY_DOWN]->setPos(up_pos.x(),
                                   up_pos.y() + button_dimensions.y() + spacing);
        addItem(buttons_[KEY_DOWN]);
        buttons_[KEY_LEFT]->setPos(up_pos.x() - button_dimensions.x() - spacing,
                                   up_pos.y() + button_dimensions.y() + spacing);
        addItem(buttons_[KEY_LEFT]);
        buttons_[KEY_RIGHT]->setPos(up_pos.x() + button_dimensions.x() + spacing,
                                    up_pos.y() + button_dimensions.y() + spacing);
        addItem(buttons_[KEY_RIGHT]);
    }

    /* Skapar alla textlådor och placerar ut dessa. */
    void MCGraphicsScene::createTextItems()
    {
        sensor_data_.insert(FRONT_LEFT, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(FRONT_RIGHT, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(LEFT_LONG, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(RIGHT_LONG, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(LEFT_SHORT, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(RIGHT_SHORT, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(BACK, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(ANGLE, new QGraphicsTextItem("0 degrees"));
        sensor_data_.insert(LINE_DEVIATION, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(LINE_TYPE, new QGraphicsTextItem("0 cm"));

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

        sensor_data_[FRONT_LEFT]->setPos(52, top_edge_y);
        addItem(sensor_data_[FRONT_LEFT]);

        sensor_data_[FRONT_RIGHT]->setPos(240, top_edge_y);
        addItem(sensor_data_[FRONT_RIGHT]);

        sensor_data_[LEFT_LONG]->setPos(left_edge_x, first_line_y);
        addItem(sensor_data_[LEFT_LONG]);

        sensor_data_[RIGHT_LONG]->setPos(right_edge_x, first_line_y);
        addItem(sensor_data_[RIGHT_LONG]);

        sensor_data_[LEFT_SHORT]->setPos(left_edge_x, second_line_y);
        addItem(sensor_data_[LEFT_SHORT]);

        sensor_data_[RIGHT_SHORT]->setPos(right_edge_x, second_line_y);
        addItem(sensor_data_[RIGHT_SHORT]);

        sensor_data_[BACK]->setPos(147, bottom_edge_y);
        addItem(sensor_data_[BACK]);

        sensor_data_[ANGLE]->setPos(110, ypos);
        addItem(sensor_data_[ANGLE]);

        sensor_data_[LINE_DEVIATION]->setPos(170, ypos);
        addItem(sensor_data_[LINE_DEVIATION]);

        sensor_data_[LINE_TYPE]->setPos(200, ypos);
        addItem(sensor_data_[LINE_TYPE]);
    }
}
