#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>

namespace logger {
    class Logger;
}

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(QObject *parent = 0);

    bool init(logger::Logger *nLog);

private:

    logger::Logger *log;

signals:

public slots:
};

#endif // PROJECT_H
