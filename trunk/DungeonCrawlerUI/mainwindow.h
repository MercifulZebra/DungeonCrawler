#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace logger {
    class Logger;
}

class ConfigAccessor;
class HomeScreen;
class MapView;

class Project;

class QStackedWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool init(QString config_filename);
    bool initStyle(QString styleConfig_filename = 0);

    bool updateStyle(QString styleConfig_filename = 0);

    QString getLog();

private:
    Ui::MainWindow *ui;

    logger::Logger      *log;
    ConfigAccessor      *accessor;

    Project             *activeProject;
    QStackedWidget      *mainStack;

    //Screens
    HomeScreen          *homeScreen;
    MapView             *mapView;

    //Screen Indexes
    int                  homeScreen_index;
    int                  mapScreen_index;

};

#endif // MAINWINDOW_H
