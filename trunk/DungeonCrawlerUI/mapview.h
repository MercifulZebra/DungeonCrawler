#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>

namespace Ui {
class MapView;
}

namespace logger {
class Logger;
}

class ConfigAccessor;

class MapWindow;

class QGridLayout;
class QPushButton;
class QButtonGroup;

class MapView : public QWidget
{
    Q_OBJECT

public:
    explicit MapView(QWidget *parent = 0);
    ~MapView();

    bool initView(logger::Logger *nLog, ConfigAccessor *nAccessor);
    bool initTopBar(ConfigAccessor *accessor);
    bool initSideBar(ConfigAccessor *accessor);
    void connectButtons();

    bool reload();


private:
    Ui::MapView     *ui;
    logger::Logger  *log;

    ConfigAccessor *accessor;

    MapWindow   *mapWindow;
    QWidget     *topBar;
    QWidget     *rightBar;

    //Right Bar
    QButtonGroup *toolGroup;
    QGridLayout  *toolLayout;
    QPushButton  *moveTool_button;
    QPushButton  *selectTool_button;
    QPushButton  *paintTool_button;

signals:

public slots:
    void moveToolButton_clicked();
    void selectToolButton_clicked();
    void paintToolButton_clicked();

};

#endif // MAPVIEW_H
