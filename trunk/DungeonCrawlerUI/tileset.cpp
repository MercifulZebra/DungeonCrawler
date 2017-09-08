#include "tileset.h"

#include "logger.h"

#include <QDebug>
#include <QDir>
#include <QFile>

//"resources/default/index.txt"
TileSet::TileSet(logger::Logger *nLog, QString defaultIndex_path, QObject *parent) : QObject(parent),
    log(nLog),
    index_path(),
    default_path(defaultIndex_path),
    wall_filenames("wall.png"),
    insideCorner_filenames("inside_corner.png"),
    outsideCorner_filenames("outside_corner.png"),
    island_filenames("island.png"),
    fill_filenames("fill.png"),
    floor_filenames("floor.png"),
    peninsula_filenames("peninsula.png"),
    wall_pixmap(),
    insideCorner_pixmap(),
    outsideCorner_pixmap(),
    island_pixmap(),
    fill_pixmap(),
    floor_pixmap(),
    peninsula_pixmap()
{


}

bool TileSet::readIndex(QString nIndex) {

    if (log == NULL) {
        return false;
    }


    index_path = nIndex.left(nIndex.lastIndexOf("/")+1);
    index_filename = nIndex.right(nIndex.count() - (nIndex.lastIndexOf("/") + 1));
    qDebug() << index_path;
    qDebug () << index_filename;

    QDir indexDir(index_path);
    qDebug() << QFile::exists(QString(indexDir.path() + "/" + index_filename));
    QFile indexFile(QString(indexDir.path() + "/" + index_filename));
    bool loadSuccess_flag = true;

    if (indexFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        log->info(QString("Opened index file \"%1\"").arg(indexFile.fileName()));

        qDebug() << indexFile.readAll();
        indexFile.close();
    }
    else {
        log->err(QString("Unable to open index file \"%1\"").arg(indexFile.fileName()));
        loadSuccess_flag = false;
    }

    return loadSuccess_flag;
}

bool TileSet::loadTileSet() {

    //Load Wall Image
    addPixmap(wall_pixmap, wall_filenames);

    //Load Floor Image
    addPixmap(floor_pixmap, floor_filenames);
}


void TileSet::addPixmap(QVector<QPixmap> target, QStringList files) {
    for (int i = 1; i < files.count(); i++) {
        QPixmap nPixmap(QString(files.at(i)));

        if (nPixmap.isNull()) {
            log->err(QString("Unable to load \"%1\" pixmap").arg(files.at(i)));
        }
        else {
            target.append(nPixmap);
        }
    }

    if (target.isEmpty()) {
        QPixmap nPixmap(files.at(0));

        if (nPixmap.isNull()) {
            log->err(QString("Unable to load default \"%1\" pixmap").arg(files.at(0)));
        }
        else {
            target.append(nPixmap);
        }
    }
}
