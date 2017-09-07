#ifndef TILESET_H
#define TILESET_H

#include <QObject>
#include <QPixmap>
#include <QStringList>
#include <QVector>

namespace logger {
class Logger;
}

class TileSet : public QObject
{
    Q_OBJECT
public:
    explicit TileSet(logger::Logger *nLog, QString defaultIndex_path, QObject *parent = 0);

    bool loadTileSet(QString nIndex);

private:
    logger::Logger *log;

    QString index_path;
    QString default_path;

    QStringList wall_filenames;
    QStringList insideCorner_filenames;
    QStringList outsideCorner_filenames;
    QStringList island_filenames;
    QStringList fill_filenames;
    QStringList floor_filenames;
    QStringList peninsula_filenames;

    QVector<QPixmap> wall_pixmap;
    QVector<QPixmap> insideCorner_pixmap;
    QVector<QPixmap> outsideCorner_pixmap;
    QVector<QPixmap> island_pixmap;
    QVector<QPixmap> fill_pixmap;
    QVector<QPixmap> floor_pixmap;
    QVector<QPixmap> peninsula_pixmap;

signals:

public slots:
};

#endif // TILESET_H
