#include "tile.h"

#include <QDebug>
#include <QPainter>


Tile::Tile(QObject *parent) : QObject(parent),
    log(NULL),
    activeSet(NULL),
    activeImage(NULL),
    boundingBox(),
    isHovered_flag(false),
    isSelected_flag(false)
{

}

bool Tile::init(logger::Logger *nLog) {
    bool initSuccess_flag = true;

    log = nLog;

    return initSuccess_flag;
}

void Tile::paintThis(QPainter *painter, int hNeighbors) {
    painter->save();

    QBrush rBrush = painter->brush();
    QPen rPen = painter->pen();

    if (isHovered()) {
        QBrush tBrush (QColor(255, 0, 0, 130));
        painter->setBrush(tBrush);
        painter->drawRect(boundingBox);
    }

    painter->setBrush(rBrush);
    if (isSelected()) {
        QPen sPen;
        sPen.setWidth(10);
        sPen.setColor(QColor(255, 255, 0, 140));
        painter->setPen(sPen);

        if (!(hNeighbors & TOP)) {
            painter->drawLine(QPoint(boundingBox.left(), boundingBox.top()), QPoint(boundingBox.right(), boundingBox.top()));
        }

        if (!(hNeighbors & BOT)) {
            painter->drawLine(QPoint(boundingBox.left(), boundingBox.bottom()), QPoint(boundingBox.right(), boundingBox.bottom()));
        }

        if (!(hNeighbors & RIGHT)) {
            painter->drawLine(QPoint(boundingBox.right(), boundingBox.top()), QPoint(boundingBox.right(), boundingBox.bottom()));
        }

        if (!(hNeighbors & LEFT)) {
            painter->drawLine(QPoint(boundingBox.left(), boundingBox.top()), QPoint(boundingBox.left(), boundingBox.bottom()));
        }

        //painter->drawRect(boundingBox);
    }

    painter->setBrush(rBrush);
    painter->setPen(rPen);
    painter->drawRect(boundingBox);

    painter->restore();
}

bool Tile::contains(int x_pix, int y_pix) {
    return boundingBox.contains(x_pix, y_pix);
}

QRect Tile::getBoundingBox() {
    return boundingBox;
}

void Tile::setBoundingBox(QRect nRect) {
    boundingBox = nRect;
}

void Tile::setHovered(bool hovered_flag) {
    isHovered_flag = hovered_flag;
}

void Tile::setSelected(bool selected_flag) {
    isSelected_flag = selected_flag;
}

bool Tile::isHovered() {
    return isHovered_flag;
}

bool Tile::isSelected() {
    return isSelected_flag;
}
