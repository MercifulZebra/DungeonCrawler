#ifndef MAP_H
#define MAP_H

#include <QObject>

namespace logger {
    class Logger;
}

class Map : public QObject
{
    Q_OBJECT
public:
    explicit Map(QObject *parent = 0);

    bool init(logger::Logger *nLog);

private:

    logger::Logger *log;

signals:

public slots:
};

#endif // MAP_H
