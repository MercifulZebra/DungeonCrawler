#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "configaccessor.h"
#include "logger.h"

//Screen
#include "homescreen.h"
#include "mapview.h"
#include "mapwindow.h"

//Project objects
#include "project.h"

//QT Objects
#include <QApplication>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    log(NULL),
    accessor(NULL),
    activeProject(NULL),
    mainStack(NULL),
    homeScreen(NULL),
    mapView(NULL),
    homeScreen_index(-1),
    mapScreen_index(-1)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete log;
    log = NULL;

    delete activeProject;
    activeProject = NULL;

    delete ui;
}

bool MainWindow::init(QString config_filename) {
    bool initSuccess_flag = true;

    log = new logger::Logger();

    if (log->init("config.txt")) {
        log->info("Successfully initialized Logger.");

        log->debug("Initializing Main Stack Widget.");
        mainStack = new QStackedWidget(this);
        setCentralWidget(mainStack);

        log->debug("Initialize Configuration Accessor.");
        accessor = new ConfigAccessor(this);
        if (!accessor->loadConfig(config_filename)) {
            log->err("Failed to load configuration file. Exiting initialization.");
            initSuccess_flag = false;
        }

        if (initSuccess_flag) {
            log->debug("Starting HomeScreen Initialization.");
            homeScreen = new HomeScreen(this);
            if (!homeScreen->init(log, accessor)) {
                initSuccess_flag = false;
            }

            log->debug("Starting MapView Initialization.");
            mapView = new MapView(this);
            mapView->initView(log, accessor);

            log->debug("Starting Style initialization.");
            updateStyle();

            if (initSuccess_flag) {
                homeScreen->prepareHomeScreen();

                homeScreen_index = mainStack->addWidget(homeScreen);
                mapScreen_index = mainStack->addWidget(mapView);

                mainStack->setCurrentIndex(mapScreen_index);
            }
        }
    }
    else {
        initSuccess_flag = false;
    }


    return initSuccess_flag;
}

bool MainWindow::initStyle(QString styleConfig_filename) {
    bool styleSuccess_flag = true;

    if (styleConfig_filename.isEmpty()) {
        log->info("Initializing Style from Default.");

        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53,53,53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25,25,25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53,53,53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        QApplication::setPalette(darkPalette);
        //QApplication::setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

    }
    else {
        log->warn("MainWindow.initStyle: Loading from config not implemented.");
        styleSuccess_flag = false;
    }

    return styleSuccess_flag;
}

bool MainWindow::updateStyle(QString styleConfig_filename) {
    bool styleSuccess_flag = true;

    styleSuccess_flag = initStyle(styleConfig_filename);

    //Update other Screen Palette's
    log->info("Updating HomeScreenStyle.");
    homeScreen->initStyle(this->palette());

    return styleSuccess_flag;
}

QString MainWindow::getLog(){
    QString rString;

    if (log != NULL) {
        rString = log->getLogString();
    }

    return rString;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {

}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {

}
