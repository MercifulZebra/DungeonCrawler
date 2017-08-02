#include "map.h"

Map::Map(QObject *parent) : QObject(parent)
{

}

bool Map::init(logger::Logger *nLog) {
    bool initSuccess_flag = true;

    log = nLog;

    return initSuccess_flag;
}
