#include "configaccessor.h"

ConfigAccessor::ConfigAccessor(QObject *parent) : QObject(parent),
    moveToolImage_string("../DungeonCrawlerUI/pointer.png"),
    selectToolImage_string("../DungeonCrawlerUI/select.png"),
    paintToolImage_string("../DungeonCrawlerUI/paint.png")
{

}

void ConfigAccessor::clear() {

}

bool ConfigAccessor::loadConfig(QString config_filename) {
    bool initSucces_flag = true;



    return initSucces_flag;
}
