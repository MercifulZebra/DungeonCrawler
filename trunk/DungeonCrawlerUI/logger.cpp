#include "logger.h"

#include <QFile>
#include <QDebug>
#include <QDir>

using namespace logger;


Logger::Logger(QObject* parent): QObject(parent),
log(),
configFilename(),
error_flag(false)
{

}

bool Logger::init(QString config) {
    bool initSuccess_flag = true;

    //Look For config Options
    configFilename = config;
    QFile configFile(configFilename);

    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        configFile.close();
    }
    else {
        initSuccess_flag = false;
    }

    return initSuccess_flag;
}

void Logger::trace(QString nMsg) {

    if (!error_flag) {
        QString logMsg("TRACE - " + nMsg);
        qDebug().noquote() << logMsg;
        log.append(logMsg + "\n");
    }
}

void Logger::debug(QString nMsg) {
    if (!error_flag) {
        QString logMsg("DEBUG - " + nMsg);
        qDebug().noquote() << logMsg;
        log.append(logMsg + "\n");
    }
}

void Logger::info(QString nMsg) {
    if (!error_flag) {
        QString logMsg("INFO - " + nMsg);
        qDebug().noquote() << logMsg;
        log.append(logMsg + "\n");
    }
}

void Logger::warn(QString nMsg) {
    if (!error_flag) {
        QString logMsg("WARN - " + nMsg);
        qDebug().noquote() << logMsg;
        log.append(logMsg + "\n");
    }
}

void Logger::err(QString nMsg) {
    if (!error_flag) {
        QString logMsg("ERROR - " + nMsg);
        qDebug().noquote() << logMsg;
        log.append(logMsg + "\n");
    }
}

QString Logger::getLogString() {
    return log;
}
