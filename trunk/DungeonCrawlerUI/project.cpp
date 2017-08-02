#include "project.h"

Project::Project(QObject *parent) : QObject(parent)
{

}

bool Project::init(logger::Logger *nLog) {
    bool initSuccess_flag = true;

    log = nLog;

    return initSuccess_flag;
}
