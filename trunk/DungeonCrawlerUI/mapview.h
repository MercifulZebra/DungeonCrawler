#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>

namespace Ui {
class MapView;
}

namespace logger {
class Logger;
}

class MapWindow;

class QGridLayout;

class MapView : public QWidget
{
    Q_OBJECT

public:
    explicit MapView(QWidget *parent = 0);
    ~MapView();

    bool initView(QString config_filename, logger::Logger *nLog);

private:
    Ui::MapView     *ui;
    logger::Logger  *log;

    MapWindow   *mapWindow;
    QWidget     *topBar;
    QWidget     *rightBar;
};

#endif // MAPVIEW_H
