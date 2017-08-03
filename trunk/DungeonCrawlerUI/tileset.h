#ifndef TILESET_H
#define TILESET_H

#include <QObject>

class TileSet : public QObject
{
    Q_OBJECT
public:
    explicit TileSet(QObject *parent = 0);

signals:

public slots:
};

#endif // TILESET_H