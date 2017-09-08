#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QRect>

namespace logger {
    class Logger;
}
/*
 * Tile Types
 */

//Straight Wall
//Inside Corner Wall
//Outside Corner Wall
//Double Inside Corner
//LeftWallRightCorner
//RightWallLeftCorner
//Straight Tunnel
//Corner Tunnel

namespace tile {

enum NeighborPosition {
    NONE        = 0x00,
    TOP         = 0x01,
    TOP_RIGHT   = 0x02,
    RIGHT       = 0x04,
    BOT_RIGHT   = 0x08,
    BOT         = 0x10,
    BOT_LEFT    = 0x20,
    LEFT        = 0x40,
    TOP_LEFT    = 0x80
};


enum TileTypes {
    INVALID         = 0,
    FLOOR           = 1,
    FILL            = 2,
    WALL            = 3,
    OUTSIDE_CORNER  = 4,
    OPPOSITE_WALL   = 5,
    PENINSULA       = 6,
    ISLAND          = 7,
    INSIDE_CORNERS  = 8,
    INSIDE_CORNERDA = 9,
    INSIDE_CORNERDO = 10,
    INSIDE_CORNERT  = 11,
    INSIDE_CORNERQ  = 12
};

}

class QPainter;

class TileSet;
class TileImage;

class Tile : public QObject
{
    Q_OBJECT
public:
    explicit Tile(QObject *parent = 0);

    bool init(logger::Logger *nLog);

    void paintThis(QPainter *painter, int hNeighbors);
    void paintTileImg(QPainter *painter);


    void paintInvalid(QPainter *painter);
    void paintFill(QPainter *painter);
    void paintFloor(QPainter *painter);
    void paintWall(QPainter *painter, QVector<QPixmap> pixmap);

    void updateTile(bool isFloor_flag, int fNeighbors);
    void lightUpdateTile(int fNeighbors);
    void updateDirectZero(int fNeighbors);
    void updateDirectSingle(int fNeighbors);
    void updateDirectDouble(int fNeighbors, int dNeighbors);
    void updateDirectTriple(int fNeighbors, int dNeighbors);

    //Access Functions
    void setActiveTileSet(TileSet *nSet);

    bool contains(int x_pix, int y_pix);

    QRect getBoundingBox();
    void setBoundingBox(QRect nRect);

    void setHovered(bool hovered_flag);
    void setSelected(bool selected_flag);

    bool isHovered();
    bool isSelected();

    bool isFloor();

    int hammingWeight(int n);
private:

    logger::Logger *log;

    TileSet        *activeSet;

    tile::TileTypes activeType;
    int             rotation_clockwise;
    int             variant;

    QRect           boundingBox;

    bool            isHovered_flag;
    bool            isSelected_flag;


signals:

public slots:
};

#endif // TILE_H
