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

    bool readIndex(QString nIndex);
    bool loadTileSet();

    void addPixmap(QVector<QPixmap> &target, QStringList files);


    QVector<QPixmap> wall_pixmap;
    QVector<QPixmap> insideCornerS_pixmap;
    QVector<QPixmap> insideCornerDAdj_pixmap;
    QVector<QPixmap> insideCornerDOpp_pixmap;
    QVector<QPixmap> insideCornerT_pixmap;
    QVector<QPixmap> insideCornerQ_pixmap;
    QVector<QPixmap> outsideCorner_pixmap;
    QVector<QPixmap> oppositeWall_pixmap;
    QVector<QPixmap> island_pixmap;
    QVector<QPixmap> fill_pixmap;
    QVector<QPixmap> floor_pixmap;
    QVector<QPixmap> peninsula_pixmap;

private:
    logger::Logger *log;

    QString index_filename;

    QString index_path;
    QString default_path;

    QStringList wall_filenames;
    QStringList insideCornerS_filenames;
    QStringList insideCornerDAdj_filenames;
    QStringList insideCornerDOpp_filenames;
    QStringList insideCornerT_filenames;
    QStringList insideCornerQ_filenames;
    QStringList outsideCorner_filenames;
    QStringList oppositeWall_filenames;
    QStringList island_filenames;
    QStringList fill_filenames;
    QStringList floor_filenames;
    QStringList peninsula_filenames;


signals:

public slots:
};

#endif // TILESET_H
