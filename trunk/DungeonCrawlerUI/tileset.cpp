#include "tileset.h"

#include "logger.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

//"resources/default/index.txt"
TileSet::TileSet(logger::Logger *nLog, QString defaultIndex_path, QObject *parent) : QObject(parent),
    log(nLog),
    index_path(),
    default_path(defaultIndex_path),
    wall_filenames("wall.png"),
    insideCornerS_filenames("inside_corner.png"),
    insideCornerDAdj_filenames("inside_corner_double_adj.png"),
    insideCornerDOpp_filenames("inside_corner_double_opp.png"),
    insideCornerT_filenames("inside_corner_triple.png"),
    insideCornerQ_filenames("inside_corner_quad.png"),
    outsideCorner_filenames("outside_corner.png"),
    oppositeWall_filenames("opposite_wall.png"),
    island_filenames("island.png"),
    fill_filenames("fill.png"),
    floor_filenames("floor.png"),
    peninsula_filenames("peninsula.png"),
    wall_pixmap(),
    insideCornerS_pixmap(),
    insideCornerDAdj_pixmap(),
    insideCornerDOpp_pixmap(),
    insideCornerT_pixmap(),
    insideCornerQ_pixmap(),
    outsideCorner_pixmap(),
    oppositeWall_pixmap(),
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


    QFile indexFile(nIndex);
    QFileInfo indexInfo(indexFile);

    QDir indexDir;
    indexDir = indexInfo.dir();

    bool loadSuccess_flag = true;
    //log->debug(QString("Index Directory: %1").arg(indexDir.path()));
    //log->debug(QString("Index Filename: %1").arg(indexFile.fileName()));

    if (indexFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        log->info(QString("Opened index file \"%1\"").arg(indexFile.fileName()));

        loadTileSet();
        indexFile.close();
    }
    else {
        log->err(QString("Unable to open index file \"%1\"").arg(indexFile.fileName()));
        loadSuccess_flag = false;
    }

    return loadSuccess_flag;
}

bool TileSet::loadTileSet() {
    bool valid_flag = false;


    //Load Floor Image
    addPixmap(floor_pixmap, floor_filenames);

    //Load Fill Image
    addPixmap(fill_pixmap, fill_filenames);

    //Load Wall Image
    addPixmap(wall_pixmap, wall_filenames);

    //Load Opposite Image
    addPixmap(oppositeWall_pixmap, oppositeWall_filenames);

    //Load Outside Corner Image
    addPixmap(outsideCorner_pixmap, outsideCorner_filenames);

    //Load Peninsula Image
    addPixmap(peninsula_pixmap, peninsula_filenames);

    //Load Island Image
    addPixmap(island_pixmap, island_filenames);

    //Load Inside Corner Image
    addPixmap(insideCornerS_pixmap, insideCornerS_filenames);

    //Load Inside Corner Image
    addPixmap(insideCornerDAdj_pixmap, insideCornerDAdj_filenames);

    //Load Inside Corner Image
    addPixmap(insideCornerDOpp_pixmap, insideCornerDOpp_filenames);

    //Load Inside Corner Image
    addPixmap(insideCornerT_pixmap, insideCornerT_filenames);

    //Load Inside Corner Image
    addPixmap(insideCornerQ_pixmap, insideCornerQ_filenames);


    return valid_flag;
}


void TileSet::addPixmap(QVector<QPixmap> &target, QStringList files) {
    for (int i = 1; i < files.count(); i++) {
        QString inputFilename = QString(index_path + "/" + files.at(i));
        QPixmap nPixmap(inputFilename);

        if (nPixmap.isNull()) {
            log->err(QString("Unable to load \"%1\" pixmap").arg(inputFilename));
        }
        else {
            target.append(nPixmap);
        }
    }

    if (target.isEmpty()) {
        QString inputFilename = QString(default_path + "/" + files.at(0));
        QPixmap nPixmap(inputFilename);

        if (nPixmap.isNull()) {
            log->err(QString("Unable to load default \"%1\" pixmap").arg(inputFilename));
        }
        else {
            target.append(nPixmap);
        }
    }
}
