#ifndef TILEIMAGE_H
#define TILEIMAGE_H

#include <QObject>
#include <QPixmap>

class TileImage : public QObject
{
    Q_OBJECT
public:
    explicit TileImage(QObject *parent = 0);

    QPixmap img;
signals:

public slots:
};

#endif // TILEIMAGE_H
