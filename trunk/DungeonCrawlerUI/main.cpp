#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QStyleFactory>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    a.setStyle(QStyleFactory::create("Fusion"));

    QStringList nArgs = a.arguments();
    QString config_filename = "config.txt";

    if (nArgs.count() == 2) {
        config_filename = nArgs.at(1);
    }

    if (w.init(config_filename)) {
        w.showMaximized();
        return a.exec();
    }
    else {
        QMessageBox errBox;
        errBox.setIcon(QMessageBox::Critical);
        QString log = w.getLog();

        if (log.isEmpty()) {
            log = "ERROR - Unknown Initialization failure.";
        }

        log.append("\n");
        log.append("Contact author at:");
        log.append("\n");
        log.append("Github: MercifulZebra\n");
        log.append("Reddit: /u/MercifulZebra");
        errBox.setText(log);

        return errBox.exec();
    }
}
