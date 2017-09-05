#include "mapview.h"
#include "ui_mapview.h"

#include "configaccessor.h"
#include "logger.h"
#include "mapwindow.h"

#include <QFile>
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

bool MapView::initView(logger::Logger *nLog, ConfigAccessor *accessor) {

    bool initSuccess_flag = true;

    log = nLog;

    //MapWindow
    log->debug("Starting MapWindow (OpenGL) initialization.");
    if (!mapWindow->initWindow(accessor, log)) {
        log->err("MapWindow failed to initialize successfully.");
        initSuccess_flag &= false;
    }

    if (!initTopBar(accessor)) {
        log->err("MapView.TopBar failed ot initialize successfully.");
        initSuccess_flag &= false;
    }


    if (!initSideBar(accessor)) {
        log->err("MapView.SideBar failed ot initialize successfully.");
        initSuccess_flag &= false;
    }
    return initSuccess_flag;

}

bool MapView::initTopBar(ConfigAccessor *accessor) {
    bool initSuccess_flag = true;


    return initSuccess_flag;
}

bool MapView::initSideBar(ConfigAccessor *accessor) {
    bool initSuccess_flag = true;


    return initSuccess_flag;

}
