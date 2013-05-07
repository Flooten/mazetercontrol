/*
 * FILNAMN:       overviewscene.cc
 * PROJEKT:       MazeterControl
 * PROGRAMMERARE: Marcus Eriksson
 * DATUM:         2013-04-18
 *
 */

#include "overviewscene.h"

#include <QFont>

namespace MC
{
    OverviewScene::OverviewScene(QObject *parent)
        : QGraphicsScene(parent)
        , background_image_(new QGraphicsPixmapItem(QPixmap(":/images/resources/overview.png")))
    {
        createTextItems();
        createButtons();
        addItem(background_image_);

        hide();
    }

    OverviewScene::~OverviewScene()
    {
        delete background_image_;
        clear();
    }

    /* Uppdaterar */
    void OverviewScene::updateSensorData(const SensorData &sensor_data)
    {
        // Giltig sensordata finns i sensor_data, uppdatera texten hos alla graphics items.
        sensor_data_[FRONT_LEFT]->setPlainText(QString::number((unsigned char)sensor_data.distance1) + UNIT_CM);
        sensor_data_[FRONT_RIGHT]->setPlainText(QString::number((unsigned char)sensor_data.distance2) + UNIT_CM);
        sensor_data_[LEFT_LONG]->setPlainText(QString::number((unsigned char)sensor_data.distance3) + UNIT_CM);
        sensor_data_[RIGHT_LONG]->setPlainText(QString::number((unsigned char)sensor_data.distance4) + UNIT_CM);
        sensor_data_[LEFT_SHORT]->setPlainText(QString::number((unsigned char)sensor_data.distance5) + UNIT_CM);
        sensor_data_[RIGHT_SHORT]->setPlainText(QString::number((unsigned char)sensor_data.distance6) + UNIT_CM);
        sensor_data_[BACK]->setPlainText(QString::number((unsigned char)sensor_data.distance7) + UNIT_CM);
        sensor_data_[ANGLE]->setPlainText("Angle: " + QString::number((unsigned short int)sensor_data.angle / 100) + UNIT_DEG);
        sensor_data_[LINE_DEVIATION]->setPlainText("Line deviation: " + QString::number((char)sensor_data.line_deviation) + UNIT_CM);
        sensor_data_[LINE_TYPE]->setPlainText("Line type: " + QString::number((unsigned char)sensor_data.line_type));

        this->update();
    }

    /* Tänder en knapp */
    void OverviewScene::buttonPressed(QKeyEvent* event)
    {
        switch (event->key())
        {
        case Qt::Key_Up:
        case Qt::Key_W:
            buttons_[KEY_UP]->setPixmap(QPixmap(":/images/resources/upp.png"));
            break;

        case Qt::Key_Down:
        case Qt::Key_S:
            buttons_[KEY_DOWN]->setPixmap(QPixmap(":/images/resources/downp.png"));
            break;

        case Qt::Key_Left:
            buttons_[KEY_LEFT]->setPixmap(QPixmap(":/images/resources/leftp.png"));
            break;

        case Qt::Key_A:
            buttons_[KEY_ROTATE_LEFT]->setPixmap(QPixmap(":/images/resources/rotleftp.png"));
            break;

        case Qt::Key_Right:
            buttons_[KEY_RIGHT]->setPixmap(QPixmap(":/images/resources/rightp.png"));
            break;

        case Qt::Key_D:
            buttons_[KEY_ROTATE_RIGHT]->setPixmap(QPixmap(":/images/resources/rotrightp.png"));
            break;
        }
    }

    /* Släcker en knapp */
    void OverviewScene::buttonReleased(QKeyEvent* event)
    {
        switch (event->key())
        {
        case Qt::Key_Up:
        case Qt::Key_W:
            buttons_[KEY_UP]->setPixmap(QPixmap(":/images/resources/upr.png"));
            break;

        case Qt::Key_Down:
        case Qt::Key_S:
            buttons_[KEY_DOWN]->setPixmap(QPixmap(":/images/resources/downr.png"));
            break;

        case Qt::Key_Left:
            buttons_[KEY_LEFT]->setPixmap(QPixmap(":/images/resources/leftr.png"));
            break;            

        case Qt::Key_A:
            buttons_[KEY_ROTATE_LEFT]->setPixmap(QPixmap(":/images/resources/rotleftr.png"));
            break;

        case Qt::Key_Right:
            buttons_[KEY_RIGHT]->setPixmap(QPixmap(":/images/resources/rightr.png"));
            break;

        case Qt::Key_D:
            buttons_[KEY_ROTATE_RIGHT]->setPixmap(QPixmap(":/images/resources/rotrightr.png"));
            break;
        }
    }

    /* Visar items */
    void OverviewScene::show()
    {
        background_image_->show();

        buttons_[KEY_UP]->show();
        buttons_[KEY_DOWN]->show();
        buttons_[KEY_LEFT]->show();
        buttons_[KEY_RIGHT]->show();
        buttons_[KEY_ROTATE_LEFT]->show();
        buttons_[KEY_ROTATE_RIGHT]->show();

        QMapIterator<SensorDataIndex, QGraphicsTextItem*> itr(sensor_data_);

        while (itr.hasNext())
        {
            itr.next();
            itr.value()->show();
        }
    }

    /* Gömmer alla items på scenen */
    void OverviewScene::hide()
    {
        background_image_->hide();

        buttons_[KEY_UP]->hide();
        buttons_[KEY_DOWN]->hide();
        buttons_[KEY_LEFT]->hide();
        buttons_[KEY_RIGHT]->hide();
        buttons_[KEY_ROTATE_LEFT]->hide();
        buttons_[KEY_ROTATE_RIGHT]->hide();

        QMapIterator<SensorDataIndex, QGraphicsTextItem*> itr(sensor_data_);

        while (itr.hasNext())
        {
            itr.next();
            itr.value()->hide();
        }
    }

    /*
     *  Private
     */

    void OverviewScene::createButtons()
    {
        QPixmap button(":/images/resources/upr.png");

        QPoint button_dimensions(button.width(), button.height());
        QPoint up_pos(430, 260);
        int spacing = 5;

        // Skapa knappar
        buttons_.insert(KEY_UP, new QGraphicsPixmapItem(button));
        buttons_.insert(KEY_DOWN, new QGraphicsPixmapItem(QPixmap(":/images/resources/downr.png")));
        buttons_.insert(KEY_LEFT, new QGraphicsPixmapItem(QPixmap(":/images/resources/leftr.png")));
        buttons_.insert(KEY_RIGHT, new QGraphicsPixmapItem(QPixmap(":/images/resources/rightr.png")));
        buttons_.insert(KEY_ROTATE_LEFT, new QGraphicsPixmapItem(QPixmap(":/images/resources/rotleftr.png")));
        buttons_.insert(KEY_ROTATE_RIGHT, new QGraphicsPixmapItem(QPixmap(":/images/resources/rotrightr.png")));

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

        buttons_[KEY_ROTATE_LEFT]->setPos(up_pos.x() - button_dimensions.x() - spacing,
                                          up_pos.y());
        addItem(buttons_[KEY_ROTATE_LEFT]);

        buttons_[KEY_ROTATE_RIGHT]->setPos(up_pos.x() + button_dimensions.x() + spacing,
                                           up_pos.y());
        addItem(buttons_[KEY_ROTATE_RIGHT]);
    }

    /* Skapar alla textlådor och placerar ut dessa. */
    void OverviewScene::createTextItems()
    {
        sensor_data_.insert(FRONT_LEFT, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(FRONT_RIGHT, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(LEFT_LONG, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(RIGHT_LONG, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(LEFT_SHORT, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(RIGHT_SHORT, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(BACK, new QGraphicsTextItem("0 cm"));
        sensor_data_.insert(ANGLE, new QGraphicsTextItem("Angle: 0 degrees"));
        sensor_data_.insert(LINE_DEVIATION, new QGraphicsTextItem("Line deviation: 0 cm"));
        sensor_data_.insert(LINE_TYPE, new QGraphicsTextItem("Line type: 0"));

        // Sätt rätt font
        QFont font("Ubuntu");
        font.setPointSize(10);
        QMapIterator<SensorDataIndex, QGraphicsTextItem*> itr(sensor_data_);

        while (itr.hasNext())
        {
            itr.next();
            itr.value()->setFont(font);
        }

        placeTextItems();
    }

    /* Placerar ut textlådorna */
    void OverviewScene::placeTextItems()
    {
        int left_edge_x = -40;
        int right_edge_x = 305;
        int first_line_y = 90;
        int second_line_y = 275;
        int top_edge_y = -30;
        int bottom_edge_y = 395;
        int center_horizontal = background_image_->boundingRect().width() / 2;
        int center_vertical = background_image_->boundingRect().height() / 2;
        int center_offset = 40;

        sensor_data_[FRONT_LEFT]->setPos(60, top_edge_y);
        addItem(sensor_data_[FRONT_LEFT]);

        sensor_data_[FRONT_RIGHT]->setPos(205, top_edge_y);
        addItem(sensor_data_[FRONT_RIGHT]);

        sensor_data_[LEFT_LONG]->setPos(left_edge_x, first_line_y);
        addItem(sensor_data_[LEFT_LONG]);

        sensor_data_[RIGHT_LONG]->setPos(right_edge_x, first_line_y);
        addItem(sensor_data_[RIGHT_LONG]);

        sensor_data_[LEFT_SHORT]->setPos(left_edge_x, second_line_y);
        addItem(sensor_data_[LEFT_SHORT]);

        sensor_data_[RIGHT_SHORT]->setPos(right_edge_x, second_line_y);
        addItem(sensor_data_[RIGHT_SHORT]);

        sensor_data_[BACK]->setPos(center_horizontal - 18, bottom_edge_y);
        addItem(sensor_data_[BACK]);

        sensor_data_[ANGLE]->setPos(center_horizontal - center_offset, center_vertical);
        addItem(sensor_data_[ANGLE]);

        sensor_data_[LINE_DEVIATION]->setPos(center_horizontal - center_offset, center_vertical + 20);
        addItem(sensor_data_[LINE_DEVIATION]);

        sensor_data_[LINE_TYPE]->setPos(center_horizontal - center_offset, center_vertical - 20);
        addItem(sensor_data_[LINE_TYPE]);
    }

} // namespace MC
