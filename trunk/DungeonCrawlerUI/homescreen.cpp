#include "homescreen.h"
#include "ui_homescreen.h"

#include "configaccessor.h"
#include "logger.h"
#include "flatbutton.h"

#include <QDebug>
#include <QStackedWidget>

HomeScreen::HomeScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeScreen),
    log(NULL)
{
    ui->setupUi(this);
}

HomeScreen::~HomeScreen()
{
    delete ui;
}

bool HomeScreen::init(logger::Logger *nLog, ConfigAccessor *accessor) {
    bool initSuccess_flag = true;

    log = nLog;
    ui->stackGroupBox->setStyleSheet("QGroupBox { font-weight: bold; font: 18pt \"Narkisim\";} ");
    return initSuccess_flag;
}

bool HomeScreen::initStyle(QPalette p) {
    bool styleSuccess_flag = true;

    setPalette(p);

    ui->openBrowseButton->setFlatStyle();
    ui->openBrowseButton->setStandardColor(p.alternateBase().color());
    ui->openBrowseButton->setHoverColor(p.base().color());

    return styleSuccess_flag;
}

void HomeScreen::clearNewProject() {

    ui->newProjectEdit->clear();
    ui->locationEdit->clear();

    ui->randomMapCheck->setChecked(false);
    ui->importResourcesCheck->setChecked(false);

}

void HomeScreen::updateRecentProjects() {
}

void HomeScreen::prepareHomeScreen() {

    //Clear New Project
    clearNewProject();

    //Update Recent Projects
    updateRecentProjects();
}
