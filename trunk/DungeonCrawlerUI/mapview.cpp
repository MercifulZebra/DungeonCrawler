#include "mapview.h"
#include "ui_mapview.h"

#include "configaccessor.h"
#include "logger.h"
#include "mapwindow.h"
#include "tooltypes.h"

#include <QButtonGroup>
#include <QFile>
#include <QGridLayout>
#include <QIcon>
#include <QPushButton>

MapView::MapView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapView),
    log(NULL),
    mapWindow(NULL),
    topBar(NULL),
    rightBar(NULL),
    toolGroup(NULL),
    toolLayout(NULL),
    moveTool_button(NULL),
    selectTool_button(NULL),
    paintTool_button(NULL)
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

bool MapView::initView(logger::Logger *nLog, ConfigAccessor *nAccessor) {

    bool initSuccess_flag = true;

    log = nLog;
    accessor = nAccessor;

    //MapWindow
    log->debug("Starting MapWindow (OpenGL) initialization.");
    if (!mapWindow->initWindow(accessor, log)) {
        log->err("MapWindow failed to initialize successfully.");
        initSuccess_flag &= false;
    }

    if (!initTopBar(accessor)) {
        log->err("MapView.TopBar failed to initialize successfully.");
        initSuccess_flag &= false;
    }


    if (!initSideBar(accessor)) {
        log->err("MapView.SideBar failed to initialize successfully.");
        initSuccess_flag &= false;
    }

    if (!reload()) {
        log->err("MapView.reload failed.");
        initSuccess_flag &= false;
    }

    if (initSuccess_flag) {
        connectButtons();
    }

    return initSuccess_flag;

}

bool MapView::initTopBar(ConfigAccessor *accessor) {
    bool initSuccess_flag = true;


    return initSuccess_flag;
}

bool MapView::initSideBar(ConfigAccessor *accessor) {
    bool initSuccess_flag = true;

    toolGroup = new QButtonGroup(this);
    toolLayout = new QGridLayout(rightBar);

    toolGroup->setExclusive(true);
    toolLayout->setAlignment(Qt::AlignTop);

    moveTool_button = new QPushButton(rightBar);
    selectTool_button = new QPushButton(rightBar);
    paintTool_button = new QPushButton(rightBar);

    moveTool_button->setFixedHeight(58);
    selectTool_button->setFixedHeight(58);
    paintTool_button->setFixedHeight(58);

    moveTool_button->setCheckable(true);
    selectTool_button->setCheckable(true);
    paintTool_button->setCheckable(true);

    toolLayout->addWidget(moveTool_button, 0, 0, 1, 1);
    toolLayout->addWidget(selectTool_button, 0, 1, 1, 1);
    toolLayout->addWidget(paintTool_button, 1, 0, 1, 1);

    toolGroup->addButton(moveTool_button);
    toolGroup->addButton(selectTool_button);
    toolGroup->addButton(paintTool_button);

    //Startup position
    moveTool_button->setChecked(true);
    moveToolButton_clicked();

    return initSuccess_flag;
}

void MapView::connectButtons() {
    connect(moveTool_button, SIGNAL(clicked(bool)), this, SLOT(moveToolButton_clicked()));
    connect(selectTool_button, SIGNAL(clicked(bool)), this, SLOT(selectToolButton_clicked()));
    connect(paintTool_button, SIGNAL(clicked(bool)), this, SLOT(paintToolButton_clicked()));
}

bool MapView::reload() {
    bool reloadSuccess_flag = true;


    QPixmap move_pixmap(accessor->moveToolImage_string);
    log->debug(QString("Move Tool Image Valid: %1").arg(!move_pixmap.isNull()));
    QIcon move_icon(move_pixmap);
    moveTool_button->setIcon(move_icon);
    moveTool_button->setIconSize(QSize(50,50));

    QPixmap select_pixmap(accessor->selectToolImage_string);
    log->debug(QString("select Tool Image Valid: %1").arg(!select_pixmap.isNull()));
    QIcon select_icon(select_pixmap);
    selectTool_button->setIcon(select_icon);
    selectTool_button->setIconSize(QSize(50,50));
    log->debug(QString("Select Tool Button Size: %1 %2").arg(selectTool_button->size().width()).arg(selectTool_button->size().height()));

    QPixmap paint_pixmap(accessor->paintToolImage_string);
    log->debug(QString("paint Tool Image Valid: %1").arg(!paint_pixmap.isNull()));
    QIcon paint_icon(paint_pixmap);
    paintTool_button->setIcon(paint_icon);
    paintTool_button->setIconSize(QSize(50,50));

    return reloadSuccess_flag;
}

void MapView::moveToolButton_clicked() {
    mapWindow->setCurrentTool(MOVE_TOOL);
}

void MapView::selectToolButton_clicked() {
    mapWindow->setCurrentTool(SELECT_TOOL);
}

void MapView::paintToolButton_clicked() {
    mapWindow->setCurrentTool(PAINT_TOOL);
}
