#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QWidget>

namespace logger {
class Logger;
}

namespace Ui {
class HomeScreen;
}

class HomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit HomeScreen(QWidget *parent = 0);
    ~HomeScreen();

    bool init(logger::Logger *nLog);
    bool initStyle(QPalette p);

    void clearNewProject();
    void updateRecentProjects();
    void prepareHomeScreen();

private slots:

private:
    Ui::HomeScreen *ui;

    logger::Logger *log;
};

#endif // HOMESCREEN_H
