#include "configaccessor.h"

ConfigAccessor::ConfigAccessor(QObject *parent) : QObject(parent),
    moveToolImage_string("resources/pointer.png"),
    selectToolImage_string("resources/select.png"),
    paintToolImage_string("resources/pencil.png")
{

}

void ConfigAccessor::clear() {

}

bool ConfigAccessor::loadConfig(QString config_filename) {
    bool initSucces_flag = true;



    return initSucces_flag;
}
