#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QVector>

namespace logger {

enum LogType {
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
};


class Logger : QObject
{
    Q_OBJECT

public:
    Logger(QObject *parent = 0);
    bool init(QString config);

    void trace(QString nMsg);
    void debug(QString nMsg);
    void info(QString nMsg);
    void warn(QString nMsg);
    void err(QString nMsg);

    QString getLogString();

    QString log;
    QString configFilename;

    bool error_flag;
};

}

#endif // LOGGER_H
