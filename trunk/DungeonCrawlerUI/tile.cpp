#include "tile.h"
#include "tileset.h"

#include <QDebug>
#include <QPainter>


Tile::Tile(QObject *parent) : QObject(parent),
    log(NULL),
    activeSet(NULL),
    activeType(tile::FILL),
    rotation_clockwise(0),
    variant(0),
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

    //Draw Image
    paintTileImg(painter);

    if (isHovered()) {
        QBrush tBrush (QColor(0, 0, 255, 130));
        painter->setBrush(tBrush);
        painter->drawRect(boundingBox);
    }


    painter->setBrush(rBrush);
    if (isSelected()) {
        QPen sPen;
        sPen.setWidth(10);
        sPen.setColor(QColor(255, 255, 0, 140));
        painter->setPen(sPen);

        if (!(hNeighbors & tile::TOP)) {
            painter->drawLine(QPoint(boundingBox.left(), boundingBox.top()), QPoint(boundingBox.right(), boundingBox.top()));
        }

        if (!(hNeighbors & tile::BOT)) {
            painter->drawLine(QPoint(boundingBox.left(), boundingBox.bottom()), QPoint(boundingBox.right(), boundingBox.bottom()));
        }

        if (!(hNeighbors & tile::RIGHT)) {
            painter->drawLine(QPoint(boundingBox.right(), boundingBox.top()), QPoint(boundingBox.right(), boundingBox.bottom()));
        }

        if (!(hNeighbors & tile::LEFT)) {
            painter->drawLine(QPoint(boundingBox.left(), boundingBox.top()), QPoint(boundingBox.left(), boundingBox.bottom()));
        }

        //painter->drawRect(boundingBox);
    }

    painter->setBrush(rBrush);
    painter->setPen(rPen);
    painter->drawRect(boundingBox);

    painter->restore();
}

void Tile::paintTileImg(QPainter *painter) {

    switch(activeType) {
    case tile::FILL:
        paintFill(painter);
        break;

    case tile::FLOOR:
        paintFloor(painter);
        break;

    case tile::WALL:
        paintWall(painter, activeSet->wall_pixmap);
        break;

    case tile::OPPOSITE_WALL:
        paintWall(painter, activeSet->oppositeWall_pixmap);
        break;

    case tile::OUTSIDE_CORNER:
        paintWall(painter, activeSet->outsideCorner_pixmap);
        break;

    case tile::PENINSULA:
        paintWall(painter, activeSet->peninsula_pixmap);
        break;

    case tile::INSIDE_CORNERS:
        paintWall(painter, activeSet->insideCornerS_pixmap);
        break;

    case tile::INSIDE_CORNERDA:
        paintWall(painter, activeSet->insideCornerDAdj_pixmap);
        break;

    case tile::INSIDE_CORNERDO:
        paintWall(painter, activeSet->insideCornerDOpp_pixmap);
        break;

    case tile::INSIDE_CORNERT:
        paintWall(painter, activeSet->insideCornerT_pixmap);
        break;

    case tile::INSIDE_CORNERQ:
        paintWall(painter, activeSet->insideCornerQ_pixmap);
        break;
    default:
        paintInvalid(painter);
        break;
    }
}

void Tile::paintInvalid(QPainter *painter) {

}

void Tile::paintFill(QPainter *painter) {

    if (!activeSet->fill_pixmap.isEmpty()) {
        if (activeSet->fill_pixmap.count() <= variant) {
            variant = 0;
        }

        painter->drawPixmap(boundingBox, activeSet->fill_pixmap.at(variant));
    }

}

void Tile::paintFloor(QPainter *painter) {
    if (!activeSet->floor_pixmap.isEmpty()) {
        if (activeSet->floor_pixmap.count() <= variant) {
            variant = 0;
        }

        painter->drawPixmap(boundingBox, activeSet->floor_pixmap.at(variant));
    }
}

void Tile::paintWall(QPainter *painter, QVector<QPixmap> pixmap) {
    if (!pixmap.isEmpty()) {
        if (pixmap.count() <= variant) {
            variant = 0;
        }

        painter->save();
        painter->translate(boundingBox.center().x(), boundingBox.center().y());
        painter->rotate(90 * rotation_clockwise);
        painter->drawPixmap(-(boundingBox.width() / 2), -(boundingBox.height() / 2), boundingBox.width(), boundingBox.height(), pixmap.at(variant));
        painter->restore();
    }
}

void Tile::updateTile(bool isFloor_flag, int fNeighbors) {

    if (isFloor_flag) {
        activeType = tile::FLOOR;
    }
    else {
        activeType = tile::FILL;
        lightUpdateTile(fNeighbors);
    }
}

void Tile::lightUpdateTile(int fNeighbors) {
    if (!isFloor()) {

        activeType = tile::INVALID;
        rotation_clockwise = 0;

        int directNeighbors = fNeighbors & (tile::TOP | tile::BOT | tile::LEFT | tile::RIGHT);

        int weightDirect = hammingWeight(directNeighbors);

        if (fNeighbors == 0) {
            activeType = tile::FILL;
        }
        else if (weightDirect == 0) {
            updateDirectZero(fNeighbors);
        }
        else if (weightDirect == 1) {
            updateDirectSingle(fNeighbors);
        }
        else if (weightDirect == 2) {
            updateDirectDouble(fNeighbors, directNeighbors);
        }
        else if (weightDirect == 3) {
            updateDirectTriple(fNeighbors, directNeighbors);
        }
        else if (weightDirect == 4) {
            activeType = tile::ISLAND;
        }


    }
}

void Tile::updateDirectZero(int fNeighbors) {

    int diagNeighbors = fNeighbors & (tile::TOP_LEFT | tile::BOT_LEFT | tile::TOP_RIGHT | tile::BOT_RIGHT);
    int weightDiagonal = hammingWeight(diagNeighbors);

    if (weightDiagonal == 1) {
        activeType = tile::INSIDE_CORNERS;

        if ((diagNeighbors & tile::BOT_RIGHT)) {
            rotation_clockwise = 0;
        }
        else if ((diagNeighbors & tile::BOT_LEFT)) {
            rotation_clockwise = 1;
        }
        else if ((diagNeighbors & tile::TOP_LEFT)) {
            rotation_clockwise = 2;
        }
        else if ((diagNeighbors & tile::TOP_RIGHT)) {
            rotation_clockwise = 3;
        }
    }
    else if (weightDiagonal == 2) {
        if (diagNeighbors == (tile::TOP_LEFT | tile::BOT_RIGHT)) {
            activeType = tile::INSIDE_CORNERDO;
        }
        else if (diagNeighbors == (tile::TOP_RIGHT | tile::BOT_LEFT)) {
            activeType = tile::INSIDE_CORNERDO;
            rotation_clockwise = 1;
        }
        else if (diagNeighbors == (tile::TOP_RIGHT | tile::BOT_RIGHT)) {
            activeType = tile::INSIDE_CORNERDA;
            rotation_clockwise = 0;
        }
        else if (diagNeighbors == (tile::BOT_RIGHT | tile::BOT_LEFT)) {
            activeType = tile::INSIDE_CORNERDA;
            rotation_clockwise = 1;
        }
        else if (diagNeighbors == (tile::TOP_LEFT | tile::BOT_LEFT)) {
            activeType = tile::INSIDE_CORNERDA;
            rotation_clockwise = 2;
        }
        else if (diagNeighbors == (tile::TOP_LEFT | tile::TOP_RIGHT)) {
            activeType = tile::INSIDE_CORNERDA;
            rotation_clockwise = 3;
        }

    }
    else if (weightDiagonal == 3) {
        activeType = tile::INSIDE_CORNERT;

        if (!(diagNeighbors & tile::BOT_RIGHT)) {
            rotation_clockwise = 0;
        }
        else if (!(diagNeighbors & tile::BOT_LEFT)) {
            rotation_clockwise = 1;
        }
        else if (!(diagNeighbors & tile::TOP_LEFT)) {
            rotation_clockwise = 2;
        }
        else if (!(diagNeighbors & tile::TOP_RIGHT)) {
            rotation_clockwise = 3;
        }

    }
    else if (weightDiagonal == 4) {
        activeType = tile::INSIDE_CORNERQ;

    }
}

void Tile::updateDirectSingle(int fNeighbors) {

    int diagNeighbors = fNeighbors & (tile::TOP_LEFT | tile::BOT_LEFT | tile::TOP_RIGHT | tile::BOT_RIGHT);
    int weightDiagonal = hammingWeight(diagNeighbors);

    if (fNeighbors & tile::TOP) {
        activeType = tile::WALL;
        rotation_clockwise = 3;
    }
    else if (fNeighbors & tile::RIGHT) {
        activeType = tile::WALL;
        rotation_clockwise = 0;
    }
    else if (fNeighbors & tile::BOT) {
        activeType = tile::WALL;
        rotation_clockwise = 1;
    }
    else if (fNeighbors & tile::LEFT) {
        activeType = tile::WALL;
        rotation_clockwise = 2;
    }
    else {
        activeType = tile::INVALID;
    }
}

void Tile::updateDirectDouble(int fNeighbors, int dNeighbors) {

    int diagNeighbors = fNeighbors & (tile::TOP_LEFT | tile::BOT_LEFT | tile::TOP_RIGHT | tile::BOT_RIGHT);
    int weightDiagonal = hammingWeight(diagNeighbors);

    if ((dNeighbors == (tile::TOP | tile::BOT))) {
        activeType = tile::OPPOSITE_WALL;
        rotation_clockwise = 1;
    }
    else if ((dNeighbors == (tile::LEFT | tile::RIGHT))) {
        activeType = tile::OPPOSITE_WALL;
        rotation_clockwise = 0;

    }
    else if ((dNeighbors == (tile::TOP | tile::LEFT))) {
        activeType = tile::OUTSIDE_CORNER;
        rotation_clockwise = 2;
    }
    else if ((dNeighbors == (tile::TOP | tile::RIGHT))) {
        activeType = tile::OUTSIDE_CORNER;
        rotation_clockwise = 3;
    }
    else if ((dNeighbors == (tile::BOT | tile::LEFT))) {
        activeType = tile::OUTSIDE_CORNER;
        rotation_clockwise = 1;
    }
    else if ((dNeighbors == (tile::BOT | tile::RIGHT))) {
        activeType = tile::OUTSIDE_CORNER;
        rotation_clockwise = 0;
    }
}

void Tile::updateDirectTriple(int fNeighbors, int dNeighbors) {

    int diagNeighbors = fNeighbors & (tile::TOP_LEFT | tile::BOT_LEFT | tile::TOP_RIGHT | tile::BOT_RIGHT);
    int weightDiagonal = hammingWeight(diagNeighbors);

    if (!(dNeighbors & (tile::TOP))) {
        activeType = tile::PENINSULA;
        rotation_clockwise = 0;
    }
    else if (!(dNeighbors & (tile::BOT))) {
        activeType = tile::PENINSULA;
        rotation_clockwise = 2;
    }
    else if (!(dNeighbors & (tile::LEFT))) {
        activeType = tile::PENINSULA;
        rotation_clockwise = 3;
    }
    else if (!(dNeighbors & (tile::RIGHT))) {
        activeType = tile::PENINSULA;
        rotation_clockwise = 1;
    }
}

void Tile::setActiveTileSet(TileSet *nSet) {
    activeSet = nSet;
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

bool Tile::isFloor() {
    return (activeType == tile::FLOOR);
}

int Tile::hammingWeight(int n) {
    int r = n ? 1 : 0;
    while (n &= (n - 1)) ++ r;
    return r;
}
