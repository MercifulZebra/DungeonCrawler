#include "mapview.h"
#include "ui_mapview.h"

#include "logger.h"
#include "mapwindow.h"

#include <QGridLayout>

MapView::MapView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapView),
    log(NULL),
    mapWindow(NULL),
    topBar(NULL),
    rightBar(NULL)
{
    ui->setupUi(this);

    mapWindow   = ui->mapWindow;
    topBar      = ui->topBar;
    rightBar    = ui->rightBar;

}

MapView::~MapView()
{
    delete ui;
}

bool MapView::initView(QString config_filename, logger::Logger *nLog) {

    bool initSuccess_flag = true;

    log = nLog;

    //MapWindow
    log->debug("Starting MapWindow (OpenGL) initialization.");
    if (!mapWindow->initWindow(config_filename, log)) {
        log->err("MapWindow failed to initialize successfully.");
    }

    return initSuccess_flag;

}
