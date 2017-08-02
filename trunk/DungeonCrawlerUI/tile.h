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

class QPainter;

class TileSet;
class TileImage;

class Tile : public QObject
{
    Q_OBJECT
public:
    explicit Tile(QObject *parent = 0);

    bool init(logger::Logger *nLog);

    void paintThis(QPainter *painter);

    //Access Functions
    bool contains(int x_pix, int y_pix);

    QRect getBoundingBox();
    void setBoundingBox(QRect nRect);

    void setHovered(bool hovered_flag);
    void setSelected(bool selected_flag);

    bool isHovered();
    bool isSelected();

private:

    logger::Logger *log;

    TileSet        *activeSet;
    TileImage      *activeImage;

    QRect           boundingBox;

    bool            isHovered_flag;
    bool            isSelected_flag;


signals:

public slots:
};

#endif // TILE_H
