#include "tileset.h"

#include "logger.h"

#include <QFile>
//"resources/default/index.txt"
TileSet::TileSet(logger::Logger *nLog, QString defaultIndex_path, QObject *parent) : QObject(parent),
    log(nLog),
    index_path(defaultIndex_path),
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

bool TileSet::loadTileSet(QString nIndex) {

    if (log == NULL) {
        return false;
    }

    QFile indexFile(index_path);
    bool loadSuccess_flag = true;

    if (!indexFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

    }
    else {
        log->err(QString("Unable to open index file \"%1\"").arg(index_path));
        loadSuccess_flag = false;
    }

    return loadSuccess_flag;
}
